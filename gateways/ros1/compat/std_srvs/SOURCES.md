# Official standard-service source definitions

`Trigger.srv` and `SetBool.srv` are the unchanged definition texts from
`ros/ros_comm_msgs`, `noetic-devel`, package `std_srvs` version 1.11.4 (BSD).
Upstream authors listed by that package include Morgan Quigley and Dirk Thomas.

* https://github.com/ros/ros_comm_msgs/blob/noetic-devel/std_srvs/srv/Trigger.srv
* https://github.com/ros/ros_comm_msgs/blob/noetic-devel/std_srvs/srv/SetBool.srv
* https://github.com/ros/ros_comm_msgs/blob/noetic-devel/std_srvs/package.xml

Only missing selected-SDK C++ bindings are generated. CMake invokes the selected
SDK's official gencpp/genmsg/templates under package name `std_srvs`; the
generated headers retain the generator's own notices. No hand-authored C++
serializer, service identity, dynamic native loader or Guest grant is added.
The official service MD5s are Trigger `937c9679a518e3a18d831e57125ea522` and
SetBool `09fb03525b03e7ea1fd3992bafd87e16`.
