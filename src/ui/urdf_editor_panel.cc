//
// Created by Denys Kotelovych on 25.03.19.
//

#include <QListWidget>
#include <QFileDialog>
#include <QTreeWidget>
#include <QtWidgets/QtWidgets>
#include <QGridLayout>

#include <ros/ros.h>
#include <rviz/robot/robot.h>
#include <rviz/robot/robot_link.h>

#include "urdf_editor/ui/urdf_editor_panel.hh"
#include "urdf_editor/rviz_helpers/display_context_proxy.hh"
#include "urdf_editor/ui/update_link_dialog.hh"
#include "urdf_editor/ui/add_link_dialog.hh"
#include "urdf_editor/ui/widget_item.hh"
#include "urdf_editor/rviz_helpers/static_link_updater.hh"
#include "ui_urdf_editor_panel.h"

static const std::int32_t kRobotModelUpdateInterval = 5;

namespace urdf_editor {
namespace ui {

URDFEditorPanel::URDFEditorPanel(QWidget *parent)
        : rviz::Panel(parent),
          ui_(new Ui::URDFEditorPanelUI()),
          ogreController_(nullptr) {
    ui_->setupUi(this);

    connect(ui_->LoadButton, SIGNAL(released()), this, SLOT(LoadButtonClicked()));
    connect(ui_->NewButton, SIGNAL(released()), this, SLOT(NewButtonClicked()));
    connect(ui_->SaveButton, SIGNAL(released()), this, SLOT(SaveButtonClicked()));
    connect(ui_->SaveAsButton, SIGNAL(released()), this, SLOT(SaveAsButtonClicked()));

    connect(ui_->LinkTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem * , int)), this,
            SLOT(LinkTreeWidgetItemClicked(QTreeWidgetItem * , int)));
    connect(ui_->LinkTreeWidget, SIGNAL(itemChanged(QTreeWidgetItem * , int)), this,
            SLOT(LinkTreeWidgetItemChanged(QTreeWidgetItem * , int)));
    connect(ui_->LinkTreeWidget, SIGNAL(customContextMenuRequested( const QPoint &)),
            this, SLOT(LinkTreeContextMenuRequested(const QPoint &)));

    connect(ui_->AddLinkAction, SIGNAL(triggered(bool)), this, SLOT(AddLinkActionToggled(bool)));
    connect(ui_->RemoveLinkAction, SIGNAL(triggered(bool)), this, SLOT(RemoveLinkActionToggled(bool)));
    connect(ui_->CloneLinkAction, SIGNAL(triggered(bool)), this, SLOT(CloneLinkActionToggled(bool)));

    updateTimer_ = new QTimer;
    connect(updateTimer_, SIGNAL(timeout()), this, SLOT(OnUpdate()));

    const auto &vm = std::make_shared<view_model::LinkViewModel>(nullptr);
    linkDialog_ = new UpdateLinkDialog(vm);
    linkDialog_->setVisible(false);
    ui_->scrollAreaWidgetContents->layout()->addWidget(linkDialog_);

    connect(linkDialog_, SIGNAL(Changed()), this, SLOT(LinkDialogChanged()));
}

URDFEditorPanel::~URDFEditorPanel() {
    updateTimer_->stop();
    delete updateTimer_;
    delete linkDialog_;
}

void URDFEditorPanel::onInitialize() {
    Panel::onInitialize();

    ogreController_.reset(new ogre_helpers::OgreController(vis_manager_));
    updateTimer_->start(kRobotModelUpdateInterval);
}

void URDFEditorPanel::load(const rviz::Config &config) {
    Panel::load(config);
}

void URDFEditorPanel::save(rviz::Config config) const {
    Panel::save(config);
}

void URDFEditorPanel::NewButtonClicked() {
    ROS_DEBUG_STREAM_NAMED("urdf_editor", "URDFEditorPanel::NewButtonClicked");

    vm_.New();
    ui_->PathLabel->setText("");

    Reload();
}

void URDFEditorPanel::LoadButtonClicked() {
    ROS_DEBUG_STREAM_NAMED("urdf_editor", "URDFEditorPanel::LoadButtonClicked");

    QString fileName = QFileDialog::getOpenFileName(
            this, tr("Load URDF"), QDir::homePath(), tr("URDF (*.urdf);;All Files (*)"));

    if (fileName.isEmpty())
        return;

    if (!vm_.Load(fileName)) {
        ROS_ERROR_STREAM_NAMED("urdf_editor", "Failed to parse urdf file");
        return;
    }

    ui_->PathLabel->setText(fileName);

    Reload();
}

void URDFEditorPanel::SaveButtonClicked() {
    ROS_DEBUG_STREAM_NAMED("urdf_editor", "URDFEditorPanel::SaveButtonClicked");

    if (ui_->PathLabel->text().isEmpty()) {
        SaveAsButtonClicked();
        return;
    }

    if (!vm_.Save(ui_->PathLabel->text())) {
        ROS_ERROR_STREAM_NAMED("urdf_editor", "Failed to save urdf");
        return;
    }
}

void URDFEditorPanel::SaveAsButtonClicked() {
    ROS_DEBUG_STREAM_NAMED("urdf_editor", "URDFEditorPanel::SaveAsButtonClicked");

    QString fileName = QFileDialog::getSaveFileName(
            this, tr("Save URDF"), QDir::homePath(), tr("URDF (*.urdf);;All Files (*)"));

    if (fileName.isEmpty())
        return;

    if (!vm_.Save(fileName)) {
        ROS_ERROR_STREAM_NAMED("urdf_editor", "Failed to save urdf");
        return;
    }

    ui_->PathLabel->setText(fileName);
}

void URDFEditorPanel::LinkTreeWidgetItemClicked(QTreeWidgetItem *item, int) {
    ROS_DEBUG_STREAM_NAMED("urdf_editor", "URDFEditorPanel::LinkTreeWidgetItemClicked");

    auto linkItem = dynamic_cast<LinkTreeWidgetItem *>(item);
    auto linkName = linkItem->ViewModel()->Name().toStdString();
    auto robot = ogreController_->Robot();
    for (const auto &link : robot->getLinks())
        link.second->unsetColor();
    robot->getLink(linkName)->setColor(0.0, 1.0, 0.0);

    const auto &vm = linkItem->ViewModel()->Clone();
    vm->UsedLinkNames(vm_.LinkNames());
    linkDialog_->setVisible(true);
    linkDialog_->ViewModel(vm);
    selectedViewModel_ = vm;
}

void URDFEditorPanel::LinkTreeWidgetItemChanged(QTreeWidgetItem *item, int) {
    ROS_DEBUG_STREAM_NAMED("urdf_editor", "URDFEditorPanel::LinkTreeWidgetItemChanged");

    if (!ogreController_->Robot())
        return;
    auto linkItem = dynamic_cast<LinkTreeWidgetItem *>(item);
    auto linkName = linkItem->ViewModel()->Name().toStdString();
    auto link = ogreController_->Robot()->getLink(linkName);
    if (!link)
        return;
    link->setRobotAlpha(item->checkState(0) == Qt::Unchecked ? 0.0f : 1.0f);
}

void URDFEditorPanel::LinkTreeContextMenuRequested(const QPoint &point) {
    ROS_DEBUG_STREAM_NAMED("urdf_editor", "URDFEditorPanel::LinkTreeContextMenuRequested");

    QMenu menu(this);
    menu.addAction(ui_->AddLinkAction);
    menu.addAction(ui_->CloneLinkAction);
    menu.addAction(ui_->RemoveLinkAction);
    menu.exec(ui_->LinkTreeWidget->mapToGlobal(point));
}

void URDFEditorPanel::AddLinkActionToggled(bool) {
    ROS_DEBUG_STREAM_NAMED("urdf_editor", "URDFEditorPanel::AddLinkActionToggled");

    const auto &vm = std::make_shared<view_model::LinkViewModel>(nullptr);
    vm->UsedLinkNames(vm_.LinkNames());

    AddLinkDialog dialog(vm);
    const auto result = dialog.exec();

    if (result == QDialog::Accepted) {
        vm_.AddLink(vm);
        Reload();
    }
}

void URDFEditorPanel::RemoveLinkActionToggled(bool) {
    ROS_DEBUG_STREAM_NAMED("urdf_editor", "URDFEditorPanel::RemoveLinkActionToggled");

    const auto &items = ui_->LinkTreeWidget->selectedItems();
    if (items.empty())
        return;
    const auto &front = dynamic_cast<LinkTreeWidgetItem *>(items.front());
    if (!front)
        return;
    vm_.RemoveLink(front->ViewModel());
    Reload();
    linkDialog_->setVisible(false);
}

void URDFEditorPanel::CloneLinkActionToggled(bool) {
    ROS_DEBUG_STREAM_NAMED("urdf_editor", "URDFEditorPanel::CloneLinkActionToggled");

    const auto &items = ui_->LinkTreeWidget->selectedItems();
    if (items.empty())
        return;
    const auto &front = dynamic_cast<LinkTreeWidgetItem *>(items.front());
    vm_.CloneLink(front->ViewModel());
    Reload();
}

void URDFEditorPanel::OnUpdate() {
    ogreController_->Update();
}

void URDFEditorPanel::LinkDialogChanged() {
    if (!selectedViewModel_)
        return;
    vm_.UpdateLink(selectedViewModel_, linkDialog_->ViewModel());
    Reload();
}

void URDFEditorPanel::Reload() {
    ROS_DEBUG_STREAM_NAMED("urdf_editor", "URDFEditorPanel::Reload");

    ReloadLinkTree();
    ReloadRobot();
}

void URDFEditorPanel::ReloadLinkTree() {
    ui_->LinkTreeWidget->blockSignals(true);
    ui_->LinkTreeWidget->clear();

    std::queue<std::pair<view_model::LinkViewModelPtr, QTreeWidgetItem *>> Q;
    Q.push({vm_.RootLinkViewModel(), new LinkTreeWidgetItem(vm_.RootLinkViewModel(), ui_->LinkTreeWidget)});

    while (!Q.empty()) {
        auto t = Q.front();
        Q.pop();

        const auto &vm = t.first;
        const auto &parent = t.second;

        if (!vm)
            continue;

        parent->setText(0, vm->Name());
        parent->setFlags(parent->flags() | Qt::ItemIsUserCheckable);
        parent->setCheckState(0, Qt::Checked);

        for (const auto &child : vm->Children()) {
            auto item = new LinkTreeWidgetItem(child);
            parent->addChild(item);
            Q.push({child, item});
        }
    }

    ui_->LinkTreeWidget->expandAll();
    ui_->LinkTreeWidget->blockSignals(false);
}

void URDFEditorPanel::ReloadRobot() {
    ogreController_->Reload(vm_);
}


}
}

#include <pluginlib/class_list_macros.h>

PLUGINLIB_EXPORT_CLASS(urdf_editor::ui::URDFEditorPanel, rviz::Panel)
