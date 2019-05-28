//
// Created by Denys Kotelovych on 24.04.19.
//

#include <OGRE/OgreVector3.h>
#include <OGRE/OgreQuaternion.h>

#include "urdf_editor/rviz_helpers/static_link_updater.hh"

static inline Ogre::Vector3 URDFVector3ToOgre(const urdf::Vector3 &v) {
    return Ogre::Vector3(v.x, v.y, v.z);
}

static inline Ogre::Quaternion URDFRotationToOgre(const urdf::Rotation &r) {
    return Ogre::Quaternion(r.w, r.x, r.y, r.z);
}

namespace urdf_editor {
namespace rviz_helpers {

StaticLinkUpdater::StaticLinkUpdater(urdf::ModelSharedPtr urdf)
        : urdf_(std::move(urdf)) {
    for (const auto &link : urdf_->links_)
        transforms_[link.first] = FindTransform(link.second);
}

bool StaticLinkUpdater::getLinkTransforms(const std::string &link_name, Ogre::Vector3 &visual_position,
                                          Ogre::Quaternion &visual_orientation, Ogre::Vector3 &collision_position,
                                          Ogre::Quaternion &collision_orientation) const {
    const auto &link = urdf_->getLink(link_name);
    if (!link) {
        setLinkStatus(rviz::StatusProperty::Error, link_name, "Transform not found");
        return false;
    }

    const auto &vt = transforms_.at(link->name);
    visual_position = vt.getTrans();
    visual_orientation = vt.extractQuaternion();
    collision_position = visual_position;
    collision_orientation = visual_orientation;

    setLinkStatus(rviz::StatusProperty::Ok, link_name, "Transform OK");
    return true;
}

void StaticLinkUpdater::setLinkStatus(rviz::StatusLevel level, const std::string &link_name,
                                      const std::string &text) const {}

Ogre::Matrix4 StaticLinkUpdater::FindTransform(const urdf::LinkConstSharedPtr &link) {
    std::vector<Ogre::Matrix4> matrices;

    auto cur = link;
    while (cur && urdf_->getRoot() != cur) {
        if (!cur->parent_joint)
            break;
        auto pose = cur->parent_joint->parent_to_joint_origin_transform;
        Ogre::Matrix4 m;
        m.makeTransform(
                URDFVector3ToOgre(pose.position),
                Ogre::Vector3(1.0, 1.0, 1.0),
                URDFRotationToOgre(pose.rotation));

        matrices.push_back(m);
        cur = urdf_->getLink(cur->parent_joint->parent_link_name);
    }

    std::reverse(matrices.begin(), matrices.end());

    matrices.push_back(Ogre::Matrix4::IDENTITY);
    auto result = matrices[0];
    for (int i = 1; i < matrices.size(); ++i)
        result = result * matrices[i];
    return result;
}

}
}
