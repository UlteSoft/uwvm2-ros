package org.uwvm.roswasm.vendor;

import org.uwvm.roswasm.*;
import org.uwvm.roswasm.generated.BoundedRecords.Ros2Sensor_msgsMsgJointState;

/** Byte-exact stage as Selected.java; no nonexistent ROS2 Header.seq access. */
final class Selected {
    static Transport transport() { return new Ros2Transport(); }
    static void checkCodec() {
        if (Ros2Sensor_msgsMsgJointState.ROS_VERSION != 2 ||
            !"sensor_msgs/msg/JointState".equals(Ros2Sensor_msgsMsgJointState.NATIVE_TYPE) ||
            Ros2Sensor_msgsMsgJointState.MAX_WIRE_BYTES < 1 ||
            Ros2Sensor_msgsMsgJointState.MAX_WIRE_BYTES > 32768)
            throw new IllegalArgumentException("actual complete ROS2 generated codec bound");
    }
    static Node.Pollable subscribe(Node node) {
        return node.createSubscription(Ros2Sensor_msgsMsgJointState.CODEC,
            "joint-state", Ros2Sensor_msgsMsgJointState.MAX_WIRE_BYTES, Selected::observe);
    }
    private static void observe(Ros2Sensor_msgsMsgJointState value) {
        if (value.header.stamp.sec != -2 || value.header.stamp.nanosec != 300000000L ||
            !"base".equals(value.header.frame_id) || value.name.length != 2 ||
            !"left".equals(value.name[0]) || !"right".equals(value.name[1]) ||
            value.position.length != 2 || value.position[0] != 1.25 || value.position[1] != -2.5 ||
            value.velocity.length != 0 || value.effort.length != 2 ||
            value.effort[0] != 0.5 || value.effort[1] != -0.25)
            throw new IllegalArgumentException("complete ROS2 JointState fixture differs");
        ++JointStateLesson.receivedCount;
    }
}
