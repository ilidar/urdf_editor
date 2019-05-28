//
// Created by Denys Kotelovych on 26.03.19.
//

#ifndef URDF_EDITOR_STATIC_LINK_UPDATER_HH
#define URDF_EDITOR_STATIC_LINK_UPDATER_HH

#include <unordered_map>
#include <urdf/model.h>
#include <rviz/robot/link_updater.h>
#include <OGRE/OgreMatrix4.h>

namespace urdf_editor {
namespace rviz_helpers {

class StaticLinkUpdater : public rviz::LinkUpdater {
public:
    explicit StaticLinkUpdater(urdf::ModelSharedPtr urdfPtr);

public:
    bool getLinkTransforms(const std::string &link_name, Ogre::Vector3 &visual_position,
                           Ogre::Quaternion &visual_orientation, Ogre::Vector3 &collision_position,
                           Ogre::Quaternion &collision_orientation) const override;

    void setLinkStatus(rviz::StatusLevel level,
                       const std::string &link_name,
                       const std::string &text) const override;

private:
    Ogre::Matrix4 FindTransform(const urdf::LinkConstSharedPtr &link);

private:
    urdf::ModelSharedPtr urdf_;
    std::unordered_map<std::string, Ogre::Matrix4> transforms_;
};

using StaticLinkUpdaterPtr = std::shared_ptr<StaticLinkUpdater>;

}
}

#endif //URDF_EDITOR_STATIC_LINK_UPDATER_HH

