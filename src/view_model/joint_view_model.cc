//
// Created by Denys Kotelovych on 28.03.19.
//

#include "urdf_editor/view_model/joint_view_model.hh"
#include "urdf_editor/utils/urdf_clone.hh"

using JointType = decltype(urdf::Joint::type);

static const std::map<JointType, QString> kJointTypeToNameMap = {
        {urdf::Joint::FIXED,      "Fixed"},
        {urdf::Joint::REVOLUTE,   "Revolute"},
        {urdf::Joint::CONTINUOUS, "Continuous"},
        {urdf::Joint::PRISMATIC,  "Prismatic"},
        {urdf::Joint::FLOATING,   "Floating"},
        {urdf::Joint::PLANAR,     "Planar"},
};

static const std::map<QString, JointType> kJointNameToTypeMap = {
        {"Fixed",      urdf::Joint::FIXED},
        {"Revolute",   urdf::Joint::REVOLUTE},
        {"Continuous", urdf::Joint::CONTINUOUS},
        {"Prismatic",  urdf::Joint::PRISMATIC},
        {"Floating",   urdf::Joint::FLOATING},
        {"Planar",     urdf::Joint::PLANAR},
};

namespace urdf_editor {
namespace view_model {

QString JointViewModel::Type() {
    return kJointTypeToNameMap.at(model_->type);
}

void JointViewModel::Type(const QString &type) {
    model_->type = kJointNameToTypeMap.at(type);

    if (LimitsEnabled())
        limits_.reset(new JointLimitsViewModel(model_->limits));
    else
        limits_ = nullptr;
}

}
}
