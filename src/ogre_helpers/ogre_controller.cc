//
// Created by Denys Kotelovych on 23.04.19.
//

#include <OGRE/OgreSceneManager.h>
#include <rviz/robot/robot.h>
#include <rviz/robot/robot_link.h>
#include <rviz/ogre_helpers/axes.h>
#include <rviz/ogre_helpers/movable_text.h>
#include <rviz/visualization_manager.h>

#include "urdf_editor/rviz_helpers/static_link_updater.hh"
#include "urdf_editor/rviz_helpers/display_context_proxy.hh"
#include "urdf_editor/ogre_helpers/ogre_controller.hh"

namespace urdf_editor {
namespace ogre_helpers {

struct OgreController::PImpl {
    explicit PImpl(rviz::VisualizationManager *visualizationManager)
            : Rviz(visualizationManager),
              Ogre(visualizationManager->getSceneManager()),
              LinkUpdater(nullptr) {
        Ogre.RootNode = Ogre.SceneManager->getRootSceneNode();
        Ogre.RobotNode = Ogre.RootNode->createChildSceneNode();
        Ogre.AxesNode = Ogre.RootNode->createChildSceneNode();
        Ogre.NamesNode = Ogre.RootNode->createChildSceneNode();
        DisplayContextProxy = new rviz::DisplayContextProxy(
                Ogre.SceneManager, visualizationManager->getSelectionManager());
        Rviz.Robot.reset(new rviz::Robot(Ogre.RobotNode, DisplayContextProxy, "urdf_robot_model", nullptr));
    }

    ~PImpl() {
        delete DisplayContextProxy;
        Ogre.RootNode->removeAndDestroyAllChildren();
        Ogre.SceneManager->destroySceneNode(Ogre.RootNode->getName());
    }

    struct RvizPrivate_ {
        explicit RvizPrivate_(rviz::VisualizationManager *visualizationManager)
                : VisualizationManager(visualizationManager) {}

        rviz::VisualizationManager *VisualizationManager;
        rviz::RobotPtr Robot;
        std::vector<rviz::AxesPtr> Axes;
    } Rviz;

    struct OgrePrivate_ {
        explicit OgrePrivate_(Ogre::SceneManager *sceneManager)
                : SceneManager(sceneManager) {}

        Ogre::SceneManager *SceneManager;
        Ogre::SceneNode *RootNode = nullptr;
        Ogre::SceneNode *RobotNode = nullptr;
        Ogre::SceneNode *AxesNode = nullptr;
        Ogre::SceneNode *NamesNode = nullptr;
    } Ogre;

    rviz_helpers::StaticLinkUpdaterPtr LinkUpdater;
    rviz::DisplayContextProxy *DisplayContextProxy;
};

OgreController::OgreController(rviz::VisualizationManager *visualizationManager)
        : pimpl_(new OgreController::PImpl(visualizationManager)) {}

OgreController::~OgreController() = default;

const rviz::RobotPtr &OgreController::Robot() const { return pimpl_->Rviz.Robot; }

void OgreController::Update() {
    if (pimpl_->Rviz.Robot && pimpl_->LinkUpdater) {
        pimpl_->Rviz.Robot->update(*pimpl_->LinkUpdater);
    }
}

void OgreController::Reload(const view_model::URDFViewModel &vm) {
    ReloadRobot(vm);
    ReloadAxes(vm);
}

void OgreController::ReloadRobot(const view_model::URDFViewModel &vm) {
    const auto &model = vm.URDF();
    pimpl_->Rviz.Robot->load(*model);
    pimpl_->LinkUpdater.reset(new rviz_helpers::StaticLinkUpdater(model));
}

void OgreController::ReloadAxes(const view_model::URDFViewModel &vm) {
    pimpl_->Ogre.NamesNode->removeAndDestroyAllChildren();
    pimpl_->Rviz.Axes.clear();

    const auto &model = vm.URDF();
    Ogre::Vector3 position;
    Ogre::Quaternion orientation;
    for (const auto &pair : model->links_) {
        if (!pimpl_->LinkUpdater->getLinkTransforms(pair.second->name, position, orientation, position, orientation))
            continue;

        rviz::AxesPtr axes(new rviz::Axes(pimpl_->Ogre.SceneManager, pimpl_->Ogre.AxesNode, 0.5, 0.01));
        axes->setPosition(position);
        axes->setOrientation(orientation);
        pimpl_->Rviz.Axes.push_back(axes);

        auto nameText = new rviz::MovableText(pair.second->name, "Liberation Sans", 0.1);
        nameText->setTextAlignment(rviz::MovableText::H_CENTER, rviz::MovableText::V_BELOW);

        auto nameNode = pimpl_->Ogre.NamesNode->createChildSceneNode();
        nameNode->setPosition(position);
        nameNode->attachObject(nameText);
    }
}

}
}