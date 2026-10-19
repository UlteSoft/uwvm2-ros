#pragma once
#include <uwvm2/imported/ros/ipc/event_queue.h>
namespace uwvm2::ros_gateway
{
    // The same allocation-bounded queue code is shared by trusted Worker and
    // Gateway state. No native ROS type or Guest permission is part of metrics.
    using event_stream=::uwvm2::imported::ros::ipc::event_stream;
    using event_stream_settings=::uwvm2::imported::ros::ipc::event_stream_settings;
    using event_queue_statistics=::uwvm2::imported::ros::ipc::event_queue_statistics;
}
