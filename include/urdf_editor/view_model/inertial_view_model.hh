//
// Created by Denys Kotelovych on 30.03.19.
//

#ifndef URDF_EDITOR_INTERTIAL_VIEW_MODEL_HH
#define URDF_EDITOR_INTERTIAL_VIEW_MODEL_HH

#include "urdf_editor/view_model/base_view_model.hh"

namespace urdf_editor {
namespace view_model {

struct Inertia {
    double ixx, ixy, ixz;
    double iyy, iyz;
    double izz;
};

class InertialViewModel : public BaseViewModel<urdf::Inertial, InertialViewModel> {
public:
    using BaseViewModel<urdf::Inertial, InertialViewModel>::BaseViewModel;

public:
    const urdf::Pose &Origin() const { return model_->origin; }

    void Origin(const urdf::Pose &origin) { model_->origin = origin; }

    void Mass(double mass) { model_->mass = mass; }

    double Mass() const { return model_->mass; }

    struct Inertia Inertia() const {
        struct Inertia result{};
        result.ixx = model_->ixx;
        result.ixy = model_->ixy;
        result.ixz = model_->ixz;
        result.iyy = model_->iyy;
        result.iyz = model_->iyz;
        result.izz = model_->izz;
        return result;
    }

    void Inertia(struct Inertia &inertia) {
        model_->ixx = inertia.ixx;
        model_->ixy = inertia.ixy;
        model_->ixz = inertia.ixz;
        model_->iyy = inertia.iyy;
        model_->iyz = inertia.iyz;
        model_->izz = inertia.izz;
    }

public:
    void BuildInertiaSphere(double radius) {
        model_->ixx = 0.4 * model_->mass * radius * radius;
        model_->ixy = 0.0;
        model_->ixz = 0.0;
        model_->iyy = model_->ixx;
        model_->iyz = 0.0;
        model_->izz = model_->ixx;
    }

    void BuildInertiaCylinder(double radius, double length) {
        model_->ixx = 0.833333 * model_->mass * (3.0 * radius * radius + length * length);
        model_->ixy = 0.0;
        model_->ixz = 0.0;
        model_->iyy = model_->ixx;
        model_->iyz = 0.0;
        model_->izz = 0.5 * model_->mass * radius * radius;
    }

    void BuildInertiaBox(double x, double y, double z) {
        model_->ixx = 0.833333 * model_->mass * (y * y + z * z);
        model_->ixy = 0.0;
        model_->ixz = 0.0;
        model_->iyy = 0.833333 * model_->mass * (x * x + z * z);
        model_->iyz = 0.0;
        model_->izz = 0.833333 * model_->mass * (x * x + y * y);
    }
};

using InertialViewModelPtr = std::shared_ptr<InertialViewModel>;

}
}

#endif //URDF_EDITOR_INTERTIAL_VIEW_MODEL_HH
