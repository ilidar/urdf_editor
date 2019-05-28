//
// Created by Denys Kotelovych on 04.04.19.
//

#ifndef URDF_EDITOR_JOINT_LIMITS_VIEW_MODEL_HH
#define URDF_EDITOR_JOINT_LIMITS_VIEW_MODEL_HH

#include "urdf_editor/view_model/base_view_model.hh"

namespace urdf_editor {
namespace view_model {

class JointLimitsViewModel : public BaseViewModel<urdf::JointLimits, JointLimitsViewModel> {
public:
    using BaseViewModel<urdf::JointLimits, JointLimitsViewModel>::BaseViewModel;

public:
    double Lower() const { return model_->lower; }

    void Lower(double lower) { model_->lower = lower; }

    double Upper() const { return model_->upper; }

    void Upper(double upper) { model_->upper = upper; }

    double Effort() const { return model_->effort; }

    void Effort(double effort) { model_->effort = effort; }

    double Velocity() const { return model_->velocity; }

    void Velocity(double velocity) { model_->velocity = velocity; }
};

using JointLimitsViewModelPtr = std::shared_ptr<JointLimitsViewModel>;

}
}

#endif //URDF_EDITOR_JOINT_LIMITS_VIEW_MODEL_HH
