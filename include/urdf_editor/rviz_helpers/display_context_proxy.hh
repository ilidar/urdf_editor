//
// Created by Denys Kotelovych on 26.03.19.
//

#ifndef URDF_EDITOR_DISPLAY_CONTEXT_PROXY_HH
#define URDF_EDITOR_DISPLAY_CONTEXT_PROXY_HH

#include <rviz/display_context.h>

namespace rviz {

class DisplayContextProxy : public DisplayContext {
Q_OBJECT
public:
    DisplayContextProxy(Ogre::SceneManager *sceneManager, SelectionManager *selectionManager)
            : sceneManager_(sceneManager), selectionManager_(selectionManager) {}

public:
    Ogre::SceneManager *getSceneManager() const override { return sceneManager_; }

    WindowManagerInterface *getWindowManager() const override { return nullptr; }

    SelectionManager *getSelectionManager() const override { return selectionManager_; }

    FrameManager *getFrameManager() const override { return nullptr; }

    tf::TransformListener *getTFClient() const override { return nullptr; }

    QString getFixedFrame() const override { return ""; }

    uint64_t getFrameCount() const override { return 0; }

    DisplayFactory *getDisplayFactory() const override { return nullptr; }

    ros::CallbackQueueInterface *getUpdateQueue() override { return nullptr; }

    ros::CallbackQueueInterface *getThreadedQueue() override { return nullptr; }

    void handleChar(QKeyEvent *event, RenderPanel *panel) override {}

    void handleMouseEvent(const ViewportMouseEvent &event) override {}

    ToolManager *getToolManager() const override { return nullptr; }

    ViewManager *getViewManager() const override { return nullptr; }

    DisplayGroup *getRootDisplayGroup() const override { return nullptr; }

    uint32_t getDefaultVisibilityBit() const override { return 0; }

    BitAllocator *visibilityBits() override { return nullptr; }

    void setStatus(const QString &message) override {}

public Q_SLOTS:

    void queueRender() override {}

private:
    Ogre::SceneManager *sceneManager_;
    SelectionManager *selectionManager_;
};

}

#endif //URDF_EDITOR_DISPLAY_CONTEXT_PROXY_HH
