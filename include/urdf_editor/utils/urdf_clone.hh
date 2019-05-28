//
// Created by Denys Kotelovych on 27.03.19.
//

#ifndef URDF_EDITOR_URDF_CLONE_HH
#define URDF_EDITOR_URDF_CLONE_HH

#include <urdf_model/link.h>
#include <urdf_model/joint.h>

namespace urdf_editor {
namespace utils {

template<typename T>
inline boost::shared_ptr<T> Clone(const boost::shared_ptr<T> &ptr) {
    if (!ptr)
        return nullptr;
    boost::shared_ptr<T> result;
    result.reset(new T(*ptr));
    return result;
}

inline urdf::VisualSharedPtr Clone(const urdf::VisualSharedPtr &visual) {
    if (!visual)
        return nullptr;
    auto result = Clone<urdf::Visual>(visual);
    result->geometry = Clone<urdf::Geometry>(visual->geometry);
    result->material = Clone<urdf::Material>(visual->material);
    return result;
}

inline urdf::CollisionSharedPtr Clone(const urdf::CollisionSharedPtr &collision) {
    if (!collision)
        return nullptr;
    auto result = Clone<urdf::Collision>(collision);
    result->geometry = Clone<urdf::Geometry>(collision->geometry);
    return result;
}

inline urdf::JointCalibrationSharedPtr Clone(const urdf::JointCalibrationSharedPtr &calibration) {
    if (!calibration)
        return nullptr;
    auto result = Clone<urdf::JointCalibration>(calibration);
    result->falling = Clone<double>(calibration->falling);
    result->rising = Clone<double>(calibration->rising);
    return result;
}

inline urdf::JointSharedPtr Clone(const urdf::JointSharedPtr &joint) {
    if (!joint)
        return nullptr;
    auto result = Clone<urdf::Joint>(joint);
    result->dynamics = Clone<urdf::JointDynamics>(joint->dynamics);
    result->limits = Clone<urdf::JointLimits>(joint->limits);
    result->safety = Clone<urdf::JointSafety>(joint->safety);
    result->mimic = Clone<urdf::JointMimic>(joint->mimic);
    result->calibration = Clone(joint->calibration);
    return result;
}

inline urdf::LinkSharedPtr Clone(const urdf::LinkSharedPtr &link) {
    if (!link)
        return nullptr;
    auto result = Clone<urdf::Link>(link);
    result->visual = Clone(link->visual);
    result->collision = Clone(link->collision);
    result->inertial = Clone<urdf::Inertial>(link->inertial);
    result->parent_joint = Clone<urdf::Joint>(link->parent_joint);
    return result;
}

}
}

#endif //URDF_EDITOR_URDF_CLONE_HH
