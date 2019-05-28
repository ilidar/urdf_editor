//
// Created by Denys Kotelovych on 26.03.19.
//

#include <cassert>
#include <QMessageBox>
#include <QColorDialog>

#include "urdf_editor/ui/update_link_dialog.hh"
#include "urdf_editor/ui/widget_item.hh"
#include "urdf_editor/ui/double_map_input_dialog.hh"
#include "ui_update_link_dialog.h"

namespace urdf_editor {
namespace ui {

UpdateLinkDialog::UpdateLinkDialog(const view_model::LinkViewModelPtr &vm, QWidget *parent)
        : QDialog(parent), ui_(new Ui::UpdateLinkDialogUI()), vm_(vm) {
    ui_->setupUi(this);

    frameNameToFrameMap_.VisualGeometry = {
            {"Box",      ui_->VisualGeometryBoxTypeFrame},
            {"Cylinder", ui_->VisualGeometryCylinderTypeFrame},
            {"Sphere",   ui_->VisualGeometrySphereTypeFrame},
            {"Mesh",     ui_->VisualGeometryMeshTypeFrame},
    };

    frameNameToFrameMap_.CollisionGeometry = {
            {"Box",      ui_->CollisionGeometryBoxTypeFrame},
            {"Cylinder", ui_->CollisionGeometryCylinderTypeFrame},
            {"Sphere",   ui_->CollisionGeometrySphereTypeFrame},
            {"Mesh",     ui_->CollisionGeometryMeshTypeFrame},
    };

    ViewModel(vm);

    Connect();
}

void UpdateLinkDialog::ViewModel(const view_model::LinkViewModelPtr &vm) {
    vm_ = vm;

    BlockSignals(true);

    ui_->VisualListWidget->clear();
    for (const auto &visual : vm_->Visuals())
        ui_->VisualListWidget->addItem(new VisualListWidgetItem(visual));

    ui_->CollisionListWidget->clear();
    for (const auto &collision : vm_->Collisions())
        ui_->CollisionListWidget->addItem(new CollisionListWidgetItem(collision));

    ui_->VisualOriginGroupBox->setVisible(false);
    ui_->VisualGeometryGroupBox->setVisible(false);
    ui_->VisualMaterialGroupBox->setVisible(false);
    ui_->CollisionOriginGroupBox->setVisible(false);
    ui_->CollisionGeometryGroupBox->setVisible(false);

    ui_->NameLineEdit->setText(vm_->Name());

    ReadFromVM(vm_->Joint());
    ReadFromVM(vm_->Inertial());

    BlockSignals(false);
}

void UpdateLinkDialog::done(int code) {
    if (code == QDialog::Rejected) {
        QDialog::done(code);
        return;
    }

    if (ui_->NameLineEdit->text().isEmpty())
        QMessageBox::warning(this, "Error", "No name specified", QMessageBox::Cancel, QMessageBox::Cancel);
    else
        QDialog::done(code);
}

void UpdateLinkDialog::VisualGeometryTypeComboBoxIndexChanged(int) {
    const auto &cb = ui_->VisualGeometryTypeComboBox;
    ArrangeVisualGeometryTypeFrames(frameNameToFrameMap_.VisualGeometry, cb->currentText());
    vvm_->Geometry()->Type(cb->currentText());
    vm_->Sync();

    EmitChanged();
}

void UpdateLinkDialog::CollisionGeometryTypeComboBoxIndexChanged(int) {
    const auto &cb = ui_->CollisionGeometryTypeComboBox;
    ArrangeVisualGeometryTypeFrames(frameNameToFrameMap_.CollisionGeometry, cb->currentText());
    cvm_->Geometry()->Type(cb->currentText());
    vm_->Sync();

    EmitChanged();
}

void UpdateLinkDialog::LinkNameLineEditTextChanged(const QString &text) {
    vm_->Name(text);
    vm_->Joint()->ChildLinkName(text);
    vm_->Sync();

    EmitChanged();
}

void UpdateLinkDialog::JointNameLineEditTextChanged(const QString &text) {
    vm_->Joint()->Name(text);
    vm_->Sync();

    EmitChanged();
}

void UpdateLinkDialog::VisualNameLineEditTextChanged(const QString &text) {
    vvm_->Name(text);
    vm_->Sync();

    EmitChanged();
}

void UpdateLinkDialog::VisualGeometryMeshPathLineEditTextChanged(const QString &text) {
    vvm_->Geometry()->Filepath(text);
    vm_->Sync();

    EmitChanged();
}

void UpdateLinkDialog::CollisionNameLineEditTextChanged(const QString &text) {
    cvm_->Name(text);

    EmitChanged();
}

void UpdateLinkDialog::CollisionGeometryMeshPathEditTextChanged(const QString &text) {
    cvm_->Geometry()->Filepath(text);
    vm_->Sync();

    EmitChanged();
}

void UpdateLinkDialog::MaterialNameLineEditTextChanged(const QString &text) {
    vvm_->Material()->Name(text);
    vm_->Sync();

    EmitChanged();
}

void UpdateLinkDialog::MaterialTexturePathLineEditTextChanged(const QString &text) {
    vvm_->Material()->TextureFileName(text);
    vm_->Sync();

    EmitChanged();
}

void UpdateLinkDialog::JointParentComboBoxIndexChanged(int) {
    ReadFromUI(vm_->Joint());

    EmitChanged();
}

void UpdateLinkDialog::JointTypeComboBoxIndexChanged(int) {
    ReadFromUI(vm_->Joint());
    ui_->JointLimitGroupBox->setVisible(vm_->Joint()->LimitsEnabled());

    EmitChanged();
}

void UpdateLinkDialog::JointSpinBoxValueChanged(double) {
    ReadFromUI(vm_->Joint());

    EmitChanged();
}

void UpdateLinkDialog::VisualSpinBoxValueChanged(double) {
    ReadFromUI(vvm_);

    EmitChanged();
}

void UpdateLinkDialog::CollisionSpinBoxValueChanged(double) {
    ReadFromUI(cvm_);

    EmitChanged();
}

void UpdateLinkDialog::InertialSpinBoxValueChanged(double) {
    ReadFromUI(vm_->Inertial());

    EmitChanged();
}

void UpdateLinkDialog::VisualListWidgetItemClicked(QListWidgetItem *item) {
    auto visualItem = dynamic_cast<VisualListWidgetItem *>(item);
    ReadFromVM(visualItem->ViewModel());
}

void UpdateLinkDialog::CollisionListWidgetItemClicked(QListWidgetItem *item) {
    auto collisionItem = dynamic_cast<CollisionListWidgetItem *>(item);
    ReadFromVM(collisionItem->ViewModel());
}

void UpdateLinkDialog::AddVisualButtonClicked() {
    AddVisual();
    EmitChanged();
}

void UpdateLinkDialog::RemoveVisualButtonClicked() {
    RemoveVisual();
    EmitChanged();
}

void UpdateLinkDialog::AddCollisionButtonClicked() {
    AddCollision();
    EmitChanged();
}

void UpdateLinkDialog::RemoveCollisionButtonClicked() {
    RemoveCollision();
    EmitChanged();
}

void UpdateLinkDialog::MaterialColorPickButtonClicked() {
    const auto &color = vvm_->Material()->Color();

    QColorDialog dialog(QColor::fromRgbF(color.r, color.g, color.b, color.a));
    if (dialog.exec() == QDialog::Accepted) {
        vvm_->Material()->Color(dialog.currentColor());
        vm_->Sync();
        ReadFromVM(vvm_);

        EmitChanged();
    }
}

void UpdateLinkDialog::BuildInertiaBoxButtonClicked() {
    DoubleMapInputDialog dialog({"X", "Y", "Z"});
    auto result = dialog.exec();
    if (result == QDialog::Accepted) {
        const auto &fieldMap = dialog.Map();
        const auto x = fieldMap.at("X");
        const auto y = fieldMap.at("Y");
        const auto z = fieldMap.at("Z");
        vm_->Inertial()->BuildInertiaBox(x, y, z);
        vm_->Sync();
        ReadFromVM(vm_->Inertial());
        EmitChanged();
    }
}

void UpdateLinkDialog::BuildInertiaCylinderButtonClicked() {
    DoubleMapInputDialog dialog({"Radius", "Length"});
    auto result = dialog.exec();
    if (result == QDialog::Accepted) {
        const auto &fieldMap = dialog.Map();
        const auto radius = fieldMap.at("Radius");
        const auto length = fieldMap.at("Length");
        vm_->Inertial()->BuildInertiaCylinder(radius, length);
        vm_->Sync();
        ReadFromVM(vm_->Inertial());
        EmitChanged();
    }
}

void UpdateLinkDialog::BuildInertiaSphereButtonClicked() {
    DoubleMapInputDialog dialog({"Radius"});
    auto result = dialog.exec();
    if (result == QDialog::Accepted) {
        const auto &fieldMap = dialog.Map();
        const auto radius = fieldMap.at("Radius");
        vm_->Inertial()->BuildInertiaSphere(radius);
        vm_->Sync();
        ReadFromVM(vm_->Inertial());
        EmitChanged();
    }
}

void
UpdateLinkDialog::ArrangeVisualGeometryTypeFrames(const std::map<QString, QFrame *> &map, const QString &frameName) {
    for (const auto &pair : map)
        pair.second->setVisible(false);
    map.at(frameName)->setVisible(true);
}

void UpdateLinkDialog::ReadFromVM(const view_model::VisualViewModelPtr &visual) {
    vvm_ = visual;
    ui_->VisualOriginGroupBox->setVisible(visual != nullptr);
    ui_->VisualGeometryGroupBox->setVisible(visual != nullptr);
    ui_->VisualMaterialGroupBox->setVisible(visual != nullptr);

    if (!visual)
        return;

    BlockSignals(true);

    ui_->VisualNameLineEdit->setText(visual->Name());
    ui_->VisualGeometryTypeComboBox->setCurrentText(vvm_->Geometry()->Name());

    ui_->VisualOriginXSpinBox->setValue(vvm_->Origin().position.x);
    ui_->VisualOriginYSpinBox->setValue(vvm_->Origin().position.y);
    ui_->VisualOriginZSpinBox->setValue(vvm_->Origin().position.z);

    double r, p, y;
    vvm_->Origin().rotation.getRPY(r, p, y);
    ui_->VisualOriginRollSpinBox->setValue(r);
    ui_->VisualOriginPitchSpinBox->setValue(p);
    ui_->VisualOriginYawSpinBox->setValue(y);

    auto gvm = vvm_->Geometry();
    ui_->VisualGeometryMeshPathLineEdit->setText(gvm->Filepath());
    ui_->VisualGeometryBoxLengthSpinBox->setValue(gvm->Length());
    ui_->VisualGeometryBoxWidthSpinBox->setValue(gvm->Width());
    ui_->VisualGeometryBoxHeightSpinBox->setValue(gvm->Height());
    ui_->VisualGeometrySphereRadiusSpinBox->setValue(gvm->Radius());
    ui_->VisualGeometryCylinderLengthSpinBox->setValue(gvm->Length());
    ui_->VisualGeometryCylinderRadiusSpinBox->setValue(gvm->Radius());

    auto mvm = vvm_->Material();
    ui_->MaterialNameLineEdit->setText(mvm->Name());
    ui_->MaterialColorRedSpinBox->setValue(mvm->Color().r);
    ui_->MaterialColorGreenSpinBox->setValue(mvm->Color().g);
    ui_->MaterialColorBlueSpinBox->setValue(mvm->Color().b);
    ui_->MaterialTexturePathLineEdit->setText(mvm->TextureFileName());

    ArrangeVisualGeometryTypeFrames(
            frameNameToFrameMap_.VisualGeometry, ui_->VisualGeometryTypeComboBox->currentText());

    BlockSignals(false);
}

void UpdateLinkDialog::ReadFromVM(const view_model::CollisionViewModelPtr &collision) {
    cvm_ = collision;
    ui_->CollisionOriginGroupBox->setVisible(collision != nullptr);
    ui_->CollisionGeometryGroupBox->setVisible(collision != nullptr);

    if (!collision)
        return;

    BlockSignals(true);

    ui_->CollisionNameLineEdit->setText(collision->Name());
    ui_->CollisionGeometryTypeComboBox->setCurrentText(cvm_->Geometry()->Name());
    ui_->CollisionOriginXSpinBox->setValue(cvm_->Origin().position.x);
    ui_->CollisionOriginYSpinBox->setValue(cvm_->Origin().position.y);
    ui_->CollisionOriginZSpinBox->setValue(cvm_->Origin().position.z);

    double r, p, y;
    cvm_->Origin().rotation.getRPY(r, p, y);
    ui_->CollisionOriginRollSpinBox->setValue(r);
    ui_->CollisionOriginPitchSpinBox->setValue(p);
    ui_->CollisionOriginYawSpinBox->setValue(y);

    auto gvm = cvm_->Geometry();
    ui_->CollisionGeometryMeshPathLineEdit->setText(gvm->Filepath());
    ui_->CollisionGeometryBoxLengthSpinBox->setValue(gvm->Length());
    ui_->CollisionGeometryBoxWidthSpinBox->setValue(gvm->Width());
    ui_->CollisionGeometryBoxHeightSpinBox->setValue(gvm->Height());
    ui_->CollisionGeometrySphereRadiusSpinBox->setValue(gvm->Radius());
    ui_->CollisionGeometryCylinderLengthSpinBox->setValue(gvm->Length());
    ui_->CollisionGeometryCylinderRadiusSpinBox->setValue(gvm->Radius());

    ArrangeVisualGeometryTypeFrames(
            frameNameToFrameMap_.CollisionGeometry, ui_->CollisionGeometryTypeComboBox->currentText());

    BlockSignals(false);
}

void UpdateLinkDialog::ReadFromVM(const view_model::InertialViewModelPtr &inertial) {
    BlockSignals(true);

    const auto &origin = inertial->Origin();
    ui_->InertialOriginXSpinBox->setValue(origin.position.x);
    ui_->InertialOriginYSpinBox->setValue(origin.position.y);
    ui_->InertialOriginZSpinBox->setValue(origin.position.z);
    double r, p, y;
    origin.rotation.getRPY(r, p, y);
    ui_->InertialOriginRollSpinBox->setValue(r);
    ui_->InertialOriginPitchSpinBox->setValue(p);
    ui_->InertialOriginYawSpinBox->setValue(y);

    ui_->InertialMassSpinBox->setValue(vm_->Inertial()->Mass());

    const auto &inertia = inertial->Inertia();
    ui_->InertiaIXXSpinBox->setValue(inertia.ixx);
    ui_->InertiaIXYSpinBox->setValue(inertia.ixy);
    ui_->InertiaIXZSpinBox->setValue(inertia.ixz);
    ui_->InertiaIYYSpinBox->setValue(inertia.iyy);
    ui_->InertiaIYZSpinBox->setValue(inertia.iyz);
    ui_->InertiaIZZSpinBox->setValue(inertia.izz);

    BlockSignals(false);
}

void UpdateLinkDialog::ReadFromVM(const view_model::JointViewModelPtr &joint) {
    BlockSignals(true);

    ui_->JointNameLineEdit->setText(joint->Name());
    ui_->JointParentLinkComboBox->clear();
    ui_->JointParentLinkComboBox->addItems(joint->UsedLinkNames());
    if (joint->ParentLinkName().isEmpty())
        ui_->JointParentLinkComboBox->setCurrentIndex(-1);
    else
        ui_->JointParentLinkComboBox->setCurrentText(joint->ParentLinkName());
    ui_->JointTypeComboBox->setCurrentText(joint->Type());
    ui_->JointLimitGroupBox->setVisible(joint->LimitsEnabled());

    if (joint->LimitsEnabled()) {
        ui_->JointLimitLowerSpinBox->setValue(joint->Limits()->Lower());
        ui_->JointLimitUpperSpinBox->setValue(joint->Limits()->Upper());
        ui_->JointLimitEffortSpinBox->setValue(joint->Limits()->Effort());
        ui_->JointLimitVelocitySpinBox->setValue(joint->Limits()->Velocity());
    }

    const auto &origin = joint->Origin();
    ui_->JointOriginXSpinBox->setValue(origin.position.x);
    ui_->JointOriginYSpinBox->setValue(origin.position.y);
    ui_->JointOriginZSpinBox->setValue(origin.position.z);
    double r, p, y;
    origin.rotation.getRPY(r, p, y);
    ui_->JointOriginRollSpinBox->setValue(r);
    ui_->JointOriginPitchSpinBox->setValue(p);
    ui_->JointOriginYawSpinBox->setValue(y);

    ui_->JointAxisXSpinBox->setValue(joint->Axis().x);
    ui_->JointAxisYSpinBox->setValue(joint->Axis().y);
    ui_->JointAxisZSpinBox->setValue(joint->Axis().z);

    BlockSignals(false);
}

void UpdateLinkDialog::ReadFromUI(const view_model::VisualViewModelPtr &visual) {
    assert(visual != nullptr);

    urdf::Pose pose;
    pose.position.x = ui_->VisualOriginXSpinBox->value();
    pose.position.y = ui_->VisualOriginYSpinBox->value();
    pose.position.z = ui_->VisualOriginZSpinBox->value();
    pose.rotation.setFromRPY(
            ui_->VisualOriginRollSpinBox->value(),
            ui_->VisualOriginPitchSpinBox->value(),
            ui_->VisualOriginYawSpinBox->value());
    visual->Origin(pose);

    const auto &gvm = visual->Geometry();
    switch (gvm->Type()) {
        case GeometryType::BOX:
            gvm->Length(ui_->VisualGeometryBoxLengthSpinBox->value());
            gvm->Width(ui_->VisualGeometryBoxWidthSpinBox->value());
            gvm->Height(ui_->VisualGeometryBoxHeightSpinBox->value());
            break;
        case GeometryType::SPHERE:
            gvm->Radius(ui_->VisualGeometrySphereRadiusSpinBox->value());
            break;
        case GeometryType::CYLINDER:
            gvm->Radius(ui_->VisualGeometryCylinderRadiusSpinBox->value());
            gvm->Length(ui_->VisualGeometryCylinderLengthSpinBox->value());
            break;
        case GeometryType::MESH:
            gvm->Filepath(ui_->VisualGeometryMeshPathLineEdit->text());
            break;
    }

    const auto &mvm = vvm_->Material();
    mvm->Name(ui_->MaterialNameLineEdit->text());
    mvm->Color(
            ui_->MaterialColorRedSpinBox->value(),
            ui_->MaterialColorGreenSpinBox->value(),
            ui_->MaterialColorBlueSpinBox->value(),
            1.0);
    mvm->TextureFileName(ui_->MaterialTexturePathLineEdit->text());
    vm_->Sync();
}

void UpdateLinkDialog::ReadFromUI(const view_model::CollisionViewModelPtr &collision) {
    assert(collision != nullptr);

    urdf::Pose pose;
    pose.position.x = ui_->CollisionOriginXSpinBox->value();
    pose.position.y = ui_->CollisionOriginYSpinBox->value();
    pose.position.z = ui_->CollisionOriginZSpinBox->value();
    pose.rotation.setFromRPY(
            ui_->CollisionOriginRollSpinBox->value(),
            ui_->CollisionOriginPitchSpinBox->value(),
            ui_->CollisionOriginYawSpinBox->value());
    collision->Origin(pose);

    const auto &gvm = collision->Geometry();
    switch (gvm->Type()) {
        case GeometryType::BOX:
            gvm->Length(ui_->CollisionGeometryBoxLengthSpinBox->value());
            gvm->Width(ui_->CollisionGeometryBoxWidthSpinBox->value());
            gvm->Height(ui_->CollisionGeometryBoxHeightSpinBox->value());
            break;
        case GeometryType::SPHERE:
            gvm->Radius(ui_->CollisionGeometrySphereRadiusSpinBox->value());
            break;
        case GeometryType::CYLINDER:
            gvm->Radius(ui_->CollisionGeometryCylinderRadiusSpinBox->value());
            gvm->Length(ui_->CollisionGeometryCylinderLengthSpinBox->value());
            break;
        case GeometryType::MESH:
            gvm->Filepath(ui_->CollisionGeometryMeshPathLineEdit->text());
            break;
    }
    vm_->Sync();
}

void UpdateLinkDialog::ReadFromUI(const view_model::JointViewModelPtr &joint) {
    joint->Name(ui_->JointNameLineEdit->text());
    joint->ParentLinkName(ui_->JointParentLinkComboBox->currentText());
    joint->ChildLinkName(ui_->NameLineEdit->text());
    joint->Type(ui_->JointTypeComboBox->currentText());

    if (joint->LimitsEnabled()) {
        joint->Limits()->Lower(ui_->JointLimitLowerSpinBox->value());
        joint->Limits()->Upper(ui_->JointLimitUpperSpinBox->value());
        joint->Limits()->Effort(ui_->JointLimitEffortSpinBox->value());
        joint->Limits()->Velocity(ui_->JointLimitVelocitySpinBox->value());
    }

    urdf::Pose pose;
    pose.position.x = ui_->JointOriginXSpinBox->value();
    pose.position.y = ui_->JointOriginYSpinBox->value();
    pose.position.z = ui_->JointOriginZSpinBox->value();
    pose.rotation.setFromRPY(
            ui_->JointOriginRollSpinBox->value(),
            ui_->JointOriginPitchSpinBox->value(),
            ui_->JointOriginYawSpinBox->value());
    joint->Origin(pose);

    urdf::Vector3 axis;
    axis.x = ui_->JointAxisXSpinBox->value();
    axis.y = ui_->JointAxisYSpinBox->value();
    axis.z = ui_->JointAxisZSpinBox->value();
    joint->Axis(axis);

    vm_->Sync();
}

void UpdateLinkDialog::ReadFromUI(const view_model::InertialViewModelPtr &inertial) {
    urdf::Pose pose;
    pose.position.x = ui_->InertialOriginXSpinBox->value();
    pose.position.y = ui_->InertialOriginYSpinBox->value();
    pose.position.z = ui_->InertialOriginZSpinBox->value();
    pose.rotation.setFromRPY(
            ui_->InertialOriginRollSpinBox->value(),
            ui_->InertialOriginPitchSpinBox->value(),
            ui_->InertialOriginYawSpinBox->value());
    inertial->Origin(pose);

    inertial->Mass(ui_->InertialMassSpinBox->value());

    view_model::Inertia inertia{};
    inertia.ixx = ui_->InertiaIXXSpinBox->value();
    inertia.ixy = ui_->InertiaIXYSpinBox->value();
    inertia.ixz = ui_->InertiaIXZSpinBox->value();
    inertia.iyy = ui_->InertiaIYYSpinBox->value();
    inertia.iyz = ui_->InertiaIYZSpinBox->value();
    inertia.izz = ui_->InertiaIZZSpinBox->value();
    inertial->Inertia(inertia);

    vm_->Sync();
}

void UpdateLinkDialog::AddVisual() {
    const auto &vm = std::make_shared<view_model::VisualViewModel>(nullptr);
    auto item = new VisualListWidgetItem(vm);
    ui_->VisualListWidget->addItem(item);
    ui_->VisualListWidget->setCurrentItem(item);
    vm_->Add(vm);
    ReadFromVM(vm);
}

void UpdateLinkDialog::AddCollision() {
    const auto &vm = std::make_shared<view_model::CollisionViewModel>(nullptr);
    auto item = new CollisionListWidgetItem(vm);
    ui_->CollisionListWidget->addItem(item);
    ui_->CollisionListWidget->setCurrentItem(item);
    vm_->Add(vm);
    ReadFromVM(vm);
}

void UpdateLinkDialog::RemoveVisual() {
    if (ui_->VisualListWidget->selectedItems().empty())
        return;
    auto item = ui_->VisualListWidget->selectedItems().front();
    auto castedItem = dynamic_cast<VisualListWidgetItem *>(item);
    vm_->Remove(castedItem->ViewModel());
    ui_->VisualListWidget->removeItemWidget(item);
    delete item;
    if (ui_->VisualListWidget->count() > 0) {
        auto first = dynamic_cast<VisualListWidgetItem *>(ui_->VisualListWidget->item(0));
        first->setSelected(true);
        ReadFromVM(first->ViewModel());
    } else {
        ReadFromVM(std::shared_ptr<view_model::VisualViewModel>(nullptr));
    }
}

void UpdateLinkDialog::RemoveCollision() {
    if (ui_->CollisionListWidget->selectedItems().empty())
        return;
    auto item = ui_->CollisionListWidget->selectedItems().front();
    auto castedItem = dynamic_cast<CollisionListWidgetItem *>(item);
    vm_->Remove(castedItem->ViewModel());
    ui_->CollisionListWidget->removeItemWidget(item);
    delete item;
    if (ui_->CollisionListWidget->count() > 0) {
        auto first = dynamic_cast<CollisionListWidgetItem *>(ui_->CollisionListWidget->item(0));
        first->setSelected(true);
        ReadFromVM(first->ViewModel());
    } else {
        ReadFromVM(std::shared_ptr<view_model::CollisionViewModel>(nullptr));
    }
}

void UpdateLinkDialog::BlockSignals(bool block) {
    QList<QWidget *> widgetList = this->findChildren<QWidget *>();
    QList<QWidget *>::const_iterator widgetIter(widgetList.begin());
    QList<QWidget *>::const_iterator lastWidget(widgetList.end());

    while (widgetIter != lastWidget) {
        (*widgetIter)->blockSignals(block);
        ++widgetIter;
    }
}

void UpdateLinkDialog::EmitChanged() {
    Q_EMIT Changed();
}

}
}