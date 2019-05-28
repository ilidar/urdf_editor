//
// Created by Denys Kotelovych on 25.03.19.
//

#ifndef URDF_EDITOR_URDF_EDITOR_PANEL_HH
#define URDF_EDITOR_URDF_EDITOR_PANEL_HH

#include <QListWidgetItem>
#include <QTreeWidgetItem>
#include <QTimer>
#include <rviz/panel.h>

#ifndef Q_MOC_RUN

#include <urdf/model.h>
#include "urdf_editor/rviz_helpers/static_link_updater.hh"
#include "urdf_editor/view_model/urdf_view_model.hh"
#include "urdf_editor/ogre_helpers/ogre_controller.hh"
#include "urdf_editor/ui/update_link_dialog.hh"

#endif

namespace Ui {
class URDFEditorPanelUI;
}

namespace urdf_editor {
namespace ui {

class LinkTreeWidgetItem;

class URDFEditorPanel : public rviz::Panel {
Q_OBJECT
public:
    explicit URDFEditorPanel(QWidget *parent = nullptr);

    ~URDFEditorPanel() override;

    void onInitialize() override;

    void load(const rviz::Config &config) override;

    void save(rviz::Config config) const override;

private Q_SLOTS:

    void NewButtonClicked();

    void LoadButtonClicked();

    void SaveButtonClicked();

    void SaveAsButtonClicked();

    void LinkTreeWidgetItemClicked(QTreeWidgetItem *item, int column);

    void LinkTreeWidgetItemChanged(QTreeWidgetItem *item, int column);

    void LinkTreeContextMenuRequested(const QPoint &);

    void AddLinkActionToggled(bool);

    void RemoveLinkActionToggled(bool);

    void CloneLinkActionToggled(bool);

    void OnUpdate();

    void LinkDialogChanged();

private:
    void Reload();

    void ReloadLinkTree();

    void ReloadRobot();

private:
    Ui::URDFEditorPanelUI *ui_;
    view_model::URDFViewModel vm_;
    ogre_helpers::OgreControllerPtr ogreController_;
    QTimer *updateTimer_;

    UpdateLinkDialog *linkDialog_;
    view_model::LinkViewModelPtr selectedViewModel_;
};

}
}

#endif //URDF_EDITOR_URDF_EDITOR_PANEL_HH
