//
// Created by Denys Kotelovych on 08.04.19.
//

#ifndef URDF_EDITOR_OGRE_CONTROLLER_HH
#define URDF_EDITOR_OGRE_CONTROLLER_HH

#include "urdf_editor/view_model/urdf_view_model.hh"

namespace rviz {
class VisualizationManager;

class Robot;

class Axes;

using RobotPtr = std::shared_ptr<Robot>;
using AxesPtr = std::shared_ptr<Axes>;
}

namespace urdf_editor {
namespace ogre_helpers {

class OgreController {
public:
    OgreController(rviz::VisualizationManager *visualizationManager);

    ~OgreController();

public:
    const rviz::RobotPtr &Robot() const;

public:
    void Update();

    void Reload(const view_model::URDFViewModel &vm);

    void ReloadRobot(const view_model::URDFViewModel &vm);

    void ReloadAxes(const view_model::URDFViewModel &vm);

private:
    struct PImpl;
    std::unique_ptr<PImpl> pimpl_;
};

using OgreControllerPtr = std::shared_ptr<OgreController>;

}
}

#endif //URDF_EDITOR_OGRE_CONTROLLER_HH
