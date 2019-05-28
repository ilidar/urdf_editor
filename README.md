# URDF Editor
Create & edit URDF files. Built on top of Rviz.
 
![](https://raw.githubusercontent.com/ilidar/urdf_editor/master/images/demo.jpg "Demo")

# Install
```
mkdir -p catkin_ws/src
git clone git@github.com:ilidar/urdf_editor.git catkin_ws/src/urdf_editor
cd catkin_ws
catkin build
source devel/setup.zsh
```

# Run
```
roslaunch urdf_editor urdf_editor.launch
```

# TODO
- [ ] Link UI
    + [x] Inertial
    + [x] Visual
    + [x] Collision
    + [x] Multiple visuals/collisions
    + [x] Connect two links
    + [x] Change joint type
    + [x] Change joint parent
    + [ ] Joint calibration/dynamics/mimic/safety
    + [ ] Fix material color bug
    + [ ] Material texture loading
- [ ] URDF UI
    + [x] Add link to selected link
    + [x] Remove link
    + [x] Select and highlight link on robot
    + [x] Show/hide checkbox
    + [x] SaveAs button
    + [x] Path to current URDF file ?
    + [ ] Show/hide collision/visual
    + [ ] Joint sliders
    + [ ] Move link/joint/visual via OGRE
- [ ] Gazebo
- [ ] Transmission 
- [ ] Properties
- [ ] Material Library
- [ ] URDF Library
- [ ] Xacro support
