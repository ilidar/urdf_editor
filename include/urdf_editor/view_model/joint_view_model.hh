//
// Created by Denys Kotelovych on 26.03.19.
//

#ifndef URDF_EDITOR_JOINT_VIEW_MODEL_HH
#define URDF_EDITOR_JOINT_VIEW_MODEL_HH

#include <QStringList>

#include "urdf_editor/view_model/base_view_model.hh"
#include "urdf_editor/view_model/joint_limits_view_model.hh"
#include "urdf_editor/utils/urdf_clone.hh"

namespace urdf_editor {
namespace view_model {

class JointViewModel;

using JointViewModelPtr = std::shared_ptr<JointViewModel>;
using V_JointViewModelPtr = std::vector<JointViewModelPtr>;

class JointViewModel : public BaseViewModel<urdf::Joint, JointViewModel> {
public:
    explicit JointViewModel(const urdf::JointSharedPtr &model)
            : BaseViewModel<urdf::Joint, JointViewModel>(model) {
        if (model_->type == urdf::Joint::UNKNOWN)
            model_->type = urdf::Joint::FIXED;

        if (LimitsEnabled())
            limits_.reset(new JointLimitsViewModel(model_->limits));
    }

public:
    QString Name() const { return QString::fromStdString(model_->name); }

    void Name(const QString &name) { model_->name = name.toStdString(); }

    QString Type();

    void Type(const QString &type);

    const urdf::Pose &Origin() const { return model_->parent_to_joint_origin_transform; }

    void Origin(const urdf::Pose &origin) { model_->parent_to_joint_origin_transform = origin; }

    QString ParentLinkName() const { return QString::fromStdString(model_->parent_link_name); }

    void ParentLinkName(const QString &name) { model_->parent_link_name = name.toStdString(); }

    QString ChildLinkName() const { return QString::fromStdString(model_->child_link_name); }

    void ChildLinkName(const QString &name) { model_->child_link_name = name.toStdString(); }

    const QStringList &UsedLinkNames() const { return linkNames_; }

    void UsedLinkNames(const QStringList &links) { linkNames_ = links; }

    const urdf::Vector3 &Axis() const { return model_->axis; }

    void Axis(const urdf::Vector3 &axis) { model_->axis = axis; }

    bool LimitsEnabled() const {
        return model_->type == urdf::Joint::REVOLUTE || model_->type == urdf::Joint::PRISMATIC;
    }

    const JointLimitsViewModelPtr &Limits() const { return limits_; }

public:
    void Sync() override {
        if (LimitsEnabled())
            model_->limits = limits_->Model();
    }

    void GenerateName() {
        Name(ChildLinkName() + "_to_" + ParentLinkName() + "_joint");
    }

private:
    QStringList linkNames_;
    JointLimitsViewModelPtr limits_;
};

}
}

#endif //URDF_EDITOR_JOINT_VIEW_MODEL_HH
