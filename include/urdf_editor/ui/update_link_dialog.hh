//
// Created by Denys Kotelovych on 26.03.19.
//

#ifndef URDF_EDITOR_UPDATE_LINK_DIALOG_HH
#define URDF_EDITOR_UPDATE_LINK_DIALOG_HH

#include <map>
#include <QDialog>
#include <QFrame>
#include <QListWidgetItem>

#include "urdf_editor/view_model/link_view_model.hh"
#include "urdf_editor/view_model/joint_view_model.hh"

namespace Ui {
class UpdateLinkDialogUI;

using UpdateLinkDialogUIPtr = std::shared_ptr<UpdateLinkDialogUI>;
}

namespace urdf_editor {
namespace ui {

class UpdateLinkDialog : public QDialog {
Q_OBJECT
public:
    explicit UpdateLinkDialog(const view_model::LinkViewModelPtr &vm, QWidget *parent = nullptr);

private:
    void Connect();

public:
    void done(int) override;

public:
    void ViewModel(const view_model::LinkViewModelPtr &vm);

    const view_model::LinkViewModelPtr &ViewModel() const { return vm_; }

Q_SIGNALS:
    void Changed();

private Q_SLOTS:

    void VisualGeometryTypeComboBoxIndexChanged(int index);

    void CollisionGeometryTypeComboBoxIndexChanged(int index);

    void JointParentComboBoxIndexChanged(int index);

    void JointTypeComboBoxIndexChanged(int index);

    void JointSpinBoxValueChanged(double);

    void VisualSpinBoxValueChanged(double);

    void CollisionSpinBoxValueChanged(double);

    void InertialSpinBoxValueChanged(double);

    void LinkNameLineEditTextChanged(const QString &text);

    void JointNameLineEditTextChanged(const QString &text);

    void VisualNameLineEditTextChanged(const QString &text);

    void VisualGeometryMeshPathLineEditTextChanged(const QString &text);

    void CollisionNameLineEditTextChanged(const QString &text);

    void CollisionGeometryMeshPathEditTextChanged(const QString &text);

    void MaterialNameLineEditTextChanged(const QString &text);

    void MaterialTexturePathLineEditTextChanged(const QString &text);

    void VisualListWidgetItemClicked(QListWidgetItem *);

    void CollisionListWidgetItemClicked(QListWidgetItem *);

    void AddVisualButtonClicked();

    void RemoveVisualButtonClicked();

    void AddCollisionButtonClicked();

    void RemoveCollisionButtonClicked();

    void MaterialColorPickButtonClicked();

    void BuildInertiaBoxButtonClicked();

    void BuildInertiaCylinderButtonClicked();

    void BuildInertiaSphereButtonClicked();

private:
    static void ArrangeVisualGeometryTypeFrames(const std::map<QString, QFrame *> &map, const QString &typeName);

    void ReadFromVM(const view_model::VisualViewModelPtr &visual);

    void ReadFromVM(const view_model::CollisionViewModelPtr &collision);

    void ReadFromVM(const view_model::InertialViewModelPtr &inertial);

    void ReadFromVM(const view_model::JointViewModelPtr &joint);

    void ReadFromUI(const view_model::VisualViewModelPtr &visual);

    void ReadFromUI(const view_model::CollisionViewModelPtr &collision);

    void ReadFromUI(const view_model::JointViewModelPtr &joint);

    void ReadFromUI(const view_model::InertialViewModelPtr &inertial);

    void AddVisual();

    void AddCollision();

    void RemoveVisual();

    void RemoveCollision();

    void BlockSignals(bool block);

    void EmitChanged();

private:
    Ui::UpdateLinkDialogUIPtr ui_;
    view_model::LinkViewModelPtr vm_;
    view_model::VisualViewModelPtr vvm_;
    view_model::CollisionViewModelPtr cvm_;

    struct {
        std::map<QString, QFrame *> VisualGeometry;
        std::map<QString, QFrame *> CollisionGeometry;
    } frameNameToFrameMap_;
};

}
}


#endif //URDF_EDITOR_UPDATE_LINK_DIALOG_HH
