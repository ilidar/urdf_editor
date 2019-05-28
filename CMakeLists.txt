cmake_minimum_required(VERSION 2.8.3)
project(urdf_editor)

add_compile_options(-std=c++11)

find_package(catkin REQUIRED
    rviz
    pluginlib
    urdf
    roslaunch)

find_package(Qt5 ${rviz_QT_VERSION} EXACT REQUIRED Core Widgets)

catkin_package(
    INCLUDE_DIRS
        include
    LIBRARIES
        urdf_editor
    CATKIN_DEPENDS
        pluginlib
        rviz
        urdf
        roslaunch)

roslaunch_add_file_check(launch/urdf_editor.launch)

include_directories(
    include
    ${catkin_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR})

set(QT_LIBRARIES Qt5::Widgets)

add_definitions(-DQT_NO_KEYWORDS)
add_definitions(-DLINUX -DPOSIX)

qt5_wrap_cpp(MOC_FILES
    include/urdf_editor/ui/urdf_editor_panel.hh
    include/urdf_editor/ui/update_link_dialog.hh
    include/urdf_editor/ui/add_link_dialog.hh
    include/urdf_editor/ui/double_map_input_dialog.hh
    include/urdf_editor/rviz_helpers/display_context_proxy.hh
    OPTIONS -DBOOST_TT_HAS_OPERATOR_HPP_INCLUDED -DBOOST_LEXICAL_CAST_INCLUDED)

qt5_wrap_ui(UIC_FILES
    ui/urdf_editor_panel.ui
    ui/update_link_dialog.ui
    ui/add_link_dialog.ui)

add_library(urdf_editor
    ${MOC_FILES}
    ${UIC_FILES}
    src/ui/urdf_editor_panel.cc
    src/ui/update_link_dialog.cc
    src/ui/add_link_dialog.cc
    src/ui/update_link_dialog_connect.cc
    src/view_model/urdf_view_model.cc
    src/view_model/link_view_model.cc
    src/view_model/joint_view_model.cc
    src/view_model/geometry_view_model.cc
    src/ogre_helpers/ogre_controller.cc
    src/rviz_helpers/static_link_updater.cc)

target_link_libraries(urdf_editor
    ${QT_LIBRARIES}
    ${catkin_LIBRARIES})