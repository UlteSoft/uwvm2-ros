package org.uwvm.roswasm.vendor;

import org.uwvm.roswasm.*;
import org.uwvm.roswasm.generated.BoundedRecords.Ros1Sensor_msgsJointState;

/** Byte-exact stage as Selected.java; only ROS1 generated declarations needed. */
final class Selected {
    static Transport transport() { return new Ros1Transport(); }
    static void checkCodec() {
        if (Ros1Sensor_msgsJointState.ROS_VERSION != 1 ||
            !"sensor_msgs/JointState".equals(Ros1Sensor_msgsJointState.NATIVE_TYPE) ||
            Ros1Sensor_msgsJointState.MAX_WIRE_BYTES < 1 ||
            Ros1Sensor_msgsJointState.MAX_WIRE_BYTES > 32768)
            throw new IllegalArgumentException("actual complete ROS1 generated codec bound");
    }
    static Node.Pollable subscribe(Node node) {
        return node.createSubscription(Ros1Sensor_msgsJointState.CODEC,
            "joint-state", Ros1Sensor_msgsJointState.MAX_WIRE_BYTES, Selected::observe);
    }
    private static void observe(Ros1Sensor_msgsJointState value) {
        if (value.header.seq != 19 || value.header.stamp.sec != 42 ||
            value.header.stamp.nsec != 300000000L ||
            !"base".equals(value.header.frame_id) || value.name.length != 2 ||
            !"left".equals(value.name[0]) || !"right".equals(value.name[1]) ||
            value.position.length != 2 || value.position[0] != 1.25 || value.position[1] != -2.5 ||
            value.velocity.length != 0 || value.effort.length != 2 ||
            value.effort[0] != 0.5 || value.effort[1] != -0.25)
            throw new IllegalArgumentException("complete ROS1 JointState fixture differs");
        ++JointStateLesson.receivedCount;
    }
}
