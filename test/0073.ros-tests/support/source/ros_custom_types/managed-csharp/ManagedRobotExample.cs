#nullable enable
using System;
using Uwvm.RosWasm;
using static Uwvm.RosWasm.Generated.BoundedRecords;

// Compile against actual ROS2 RobotMsg generated source + the real SDK. This
// optional example has no Main/native hook and is not an acceptance run. The
// caller owns Session cleanup and records its primary+cleanup report separately.
public static class ManagedRobotExample {
    public static Result PublishOne(Session session,string explicitlyGrantedAlias) {
        if(session==null)throw new ArgumentNullException(nameof(session));
        session.CheckAbi();
        var value=new _Ros2Xarm_msgsMsgRobotMsg();
        value.@header.@frame_id="robot";
        value.@angle=new float[7];
        // pose/offset are actual fixed6 arrays initialized by the real emitter.
        var publisher=new Node(session).CreatePublisher(
            Ros2Xarm_msgsMsgRobotMsgCodec.Instance,explicitlyGrantedAlias);
        // Session retains the actual capability across publish failure. The caller
        // must use CloseChecked(primary) and inspect every cleanup outcome.
        return publisher.Publish(value).Checked();
    }
}
