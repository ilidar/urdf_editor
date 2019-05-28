//
// Created by Denys Kotelovych on 02.04.19.
//

#include "urdf_editor/ui/update_link_dialog.hh"
#include "ui_update_link_dialog.h"

namespace urdf_editor {
namespace ui {

void UpdateLinkDialog::Connect() {
    connect(ui_->NameLineEdit, SIGNAL(textChanged(
                                              const QString &)), this,
            SLOT(LinkNameLineEditTextChanged(
                         const QString &)));
    connect(ui_->JointNameLineEdit, SIGNAL(textChanged(
                                                   const QString &)), this,
            SLOT(JointNameLineEditTextChanged(
                         const QString &)));
    connect(ui_->VisualNameLineEdit, SIGNAL(textChanged(
                                                    const QString &)), this,
            SLOT(VisualNameLineEditTextChanged(
                         const QString &)));
    connect(ui_->VisualGeometryMeshPathLineEdit, SIGNAL(textChanged(
                                                                const QString &)), this,
            SLOT(VisualGeometryMeshPathLineEditTextChanged(
                         const QString &)));

    connect(ui_->CollisionNameLineEdit, SIGNAL(textChanged(
                                                       const QString &)), this,
            SLOT(CollisionNameLineEditTextChanged(
                         const QString &)));
    connect(ui_->CollisionGeometryMeshPathLineEdit, SIGNAL(textChanged(
                                                                   const QString &)), this,
            SLOT(CollisionGeometryMeshPathEditTextChanged(
                         const QString &)));

    connect(ui_->MaterialNameLineEdit, SIGNAL(textChanged(
                                                      const QString &)), this,
            SLOT(MaterialNameLineEditTextChanged(
                         const QString &)));
    connect(ui_->MaterialTexturePathLineEdit, SIGNAL(textChanged(
                                                             const QString &)), this,
            SLOT(MaterialTexturePathLineEditTextChanged(
                         const QString &)));

    connect(ui_->VisualGeometryTypeComboBox, SIGNAL(currentIndexChanged(int)), this,
            SLOT(VisualGeometryTypeComboBoxIndexChanged(int)));
    connect(ui_->CollisionGeometryTypeComboBox, SIGNAL(currentIndexChanged(int)), this,
            SLOT(CollisionGeometryTypeComboBoxIndexChanged(int)));

    connect(ui_->VisualOriginXSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(VisualSpinBoxValueChanged(double)));
    connect(ui_->VisualOriginYSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(VisualSpinBoxValueChanged(double)));
    connect(ui_->VisualOriginZSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(VisualSpinBoxValueChanged(double)));
    connect(ui_->VisualOriginRollSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(VisualSpinBoxValueChanged(double)));
    connect(ui_->VisualOriginPitchSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(VisualSpinBoxValueChanged(double)));
    connect(ui_->VisualOriginYawSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(VisualSpinBoxValueChanged(double)));
    connect(ui_->VisualGeometryBoxWidthSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(VisualSpinBoxValueChanged(double)));
    connect(ui_->VisualGeometryBoxLengthSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(VisualSpinBoxValueChanged(double)));
    connect(ui_->VisualGeometryBoxHeightSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(VisualSpinBoxValueChanged(double)));
    connect(ui_->VisualGeometrySphereRadiusSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(VisualSpinBoxValueChanged(double)));
    connect(ui_->VisualGeometryCylinderLengthSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(VisualSpinBoxValueChanged(double)));
    connect(ui_->VisualGeometryCylinderRadiusSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(VisualSpinBoxValueChanged(double)));
    connect(ui_->MaterialColorRedSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(VisualSpinBoxValueChanged(double)));
    connect(ui_->MaterialColorGreenSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(VisualSpinBoxValueChanged(double)));
    connect(ui_->MaterialColorBlueSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(VisualSpinBoxValueChanged(double)));

    connect(ui_->CollisionOriginXSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(CollisionSpinBoxValueChanged(double)));
    connect(ui_->CollisionOriginYSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(CollisionSpinBoxValueChanged(double)));
    connect(ui_->CollisionOriginZSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(CollisionSpinBoxValueChanged(double)));
    connect(ui_->CollisionOriginRollSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(CollisionSpinBoxValueChanged(double)));
    connect(ui_->CollisionOriginPitchSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(CollisionSpinBoxValueChanged(double)));
    connect(ui_->CollisionOriginYawSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(CollisionSpinBoxValueChanged(double)));

    connect(ui_->CollisionGeometryBoxWidthSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(CollisionSpinBoxValueChanged(double)));
    connect(ui_->CollisionGeometryBoxLengthSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(CollisionSpinBoxValueChanged(double)));
    connect(ui_->CollisionGeometryBoxHeightSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(CollisionSpinBoxValueChanged(double)));
    connect(ui_->CollisionGeometrySphereRadiusSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(CollisionSpinBoxValueChanged(double)));
    connect(ui_->CollisionGeometryCylinderLengthSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(CollisionSpinBoxValueChanged(double)));
    connect(ui_->CollisionGeometryCylinderRadiusSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(CollisionSpinBoxValueChanged(double)));

    connect(ui_->VisualListWidget, SIGNAL(itemClicked(QListWidgetItem * )), this,
            SLOT(VisualListWidgetItemClicked(QListWidgetItem * )));
    connect(ui_->CollisionListWidget, SIGNAL(itemClicked(QListWidgetItem * )), this,
            SLOT(CollisionListWidgetItemClicked(QListWidgetItem * )));

    connect(ui_->JointOriginXSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(JointSpinBoxValueChanged(double)));
    connect(ui_->JointOriginYSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(JointSpinBoxValueChanged(double)));
    connect(ui_->JointOriginZSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(JointSpinBoxValueChanged(double)));
    connect(ui_->JointOriginRollSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(JointSpinBoxValueChanged(double)));
    connect(ui_->JointOriginPitchSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(JointSpinBoxValueChanged(double)));
    connect(ui_->JointOriginYawSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(JointSpinBoxValueChanged(double)));
    connect(ui_->JointAxisXSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(JointSpinBoxValueChanged(double)));
    connect(ui_->JointAxisYSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(JointSpinBoxValueChanged(double)));
    connect(ui_->JointAxisZSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(JointSpinBoxValueChanged(double)));
    connect(ui_->JointLimitLowerSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(JointSpinBoxValueChanged(double)));
    connect(ui_->JointLimitUpperSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(JointSpinBoxValueChanged(double)));
    connect(ui_->JointLimitEffortSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(JointSpinBoxValueChanged(double)));
    connect(ui_->JointLimitVelocitySpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(JointSpinBoxValueChanged(double)));

    connect(ui_->JointParentLinkComboBox, SIGNAL(currentIndexChanged(int)), this,
            SLOT(JointParentComboBoxIndexChanged(int)));
    connect(ui_->JointTypeComboBox, SIGNAL(currentIndexChanged(int)), this,
            SLOT(JointTypeComboBoxIndexChanged(int)));

    connect(ui_->InertialOriginXSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(InertialSpinBoxValueChanged(double)));
    connect(ui_->InertialOriginYSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(InertialSpinBoxValueChanged(double)));
    connect(ui_->InertialOriginZSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(InertialSpinBoxValueChanged(double)));
    connect(ui_->InertialOriginRollSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(InertialSpinBoxValueChanged(double)));
    connect(ui_->InertialOriginPitchSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(InertialSpinBoxValueChanged(double)));
    connect(ui_->InertialOriginYawSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(InertialSpinBoxValueChanged(double)));
    connect(ui_->InertialMassSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(InertialSpinBoxValueChanged(double)));
    connect(ui_->InertiaIXXSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(InertialSpinBoxValueChanged(double)));
    connect(ui_->InertiaIXYSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(InertialSpinBoxValueChanged(double)));
    connect(ui_->InertiaIXZSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(InertialSpinBoxValueChanged(double)));
    connect(ui_->InertiaIYYSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(InertialSpinBoxValueChanged(double)));
    connect(ui_->InertiaIYZSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(InertialSpinBoxValueChanged(double)));
    connect(ui_->InertiaIZZSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(InertialSpinBoxValueChanged(double)));

    connect(ui_->AddVisualButton, SIGNAL(released()), this, SLOT(AddVisualButtonClicked()));
    connect(ui_->RemoveVisualButton, SIGNAL(released()), this, SLOT(RemoveVisualButtonClicked()));
    connect(ui_->AddCollisionButton, SIGNAL(released()), this, SLOT(AddCollisionButtonClicked()));
    connect(ui_->RemoveCollisionButton, SIGNAL(released()), this, SLOT(RemoveCollisionButtonClicked()));
    connect(ui_->MaterialColorPickButton, SIGNAL(released()), this, SLOT(MaterialColorPickButtonClicked()));

    connect(ui_->BuildInertiaBoxButton, SIGNAL(released()), this, SLOT(BuildInertiaBoxButtonClicked()));
    connect(ui_->BuildInertiaCylinderButton, SIGNAL(released()), this, SLOT(BuildInertiaCylinderButtonClicked()));
    connect(ui_->BuildInertiaSphereButton, SIGNAL(released()), this, SLOT(BuildInertiaSphereButtonClicked()));
}

}
}