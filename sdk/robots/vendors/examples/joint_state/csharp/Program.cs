using System;
using System.IO;
using Uwvm.RosWasm;
#if UWVM_ROS1 && UWVM_ROS2
#error Exactly one actual backend is required.
#endif
#if UWVM_ROS1
using JointState = Uwvm.RosWasm.Generated.BoundedRecords._Ros1Sensor_msgsJointState;
using JointStateCodec = Uwvm.RosWasm.Generated.BoundedRecords.Ros1Sensor_msgsJointStateCodec;
#elif UWVM_ROS2
using JointState = Uwvm.RosWasm.Generated.BoundedRecords._Ros2Sensor_msgsMsgJointState;
using JointStateCodec = Uwvm.RosWasm.Generated.BoundedRecords.Ros2Sensor_msgsMsgJointStateCodec;
#else
#error Supply RosBackend matching the actual roswasm_bridge.o.
#endif

/// <summary>Actual Mono/WASI command; SOURCE_ONLY_UNEXECUTED.</summary>
internal static class Program {
#if UWVM_JOINT_STATE_POSITIVE
    private const int Mode = 1;
#else
    private const int Mode = 0;
#endif
    public static Exception? LastPrimary;
    public static Exception? LastCleanupException;
    public static CleanupReport? LastCleanup;
    public static Session? LastSession;
    public static int ReceivedCount;
    public static bool DeniedVerified;

    private static void Observe(JointState value) {
#if UWVM_ROS1
        bool header = value.@header.@seq == 19U && value.@header.@stamp.@sec == 42U &&
            value.@header.@stamp.@nsec == 300000000U;
#else
        bool header = value.@header.@stamp.@sec == -2 &&
            value.@header.@stamp.@nanosec == 300000000U;
#endif
        if (!header || value.@header.@frame_id != "base" || value.@name.Length != 2 ||
            value.@name[0] != "left" || value.@name[1] != "right" ||
            value.@position.Length != 2 || value.@position[0] != 1.25 || value.@position[1] != -2.5 ||
            value.@velocity.Length != 0 || value.@effort.Length != 2 ||
            value.@effort[0] != 0.5 || value.@effort[1] != -0.25)
            throw new FormatException("complete JointState differs from observer-fixture.json");
        ++ReceivedCount;
    }

    private static int Main(string[] args) {
        if (args.Length != 0) throw new ArgumentException("fresh build-selected command mode");
        var transport = new NativeTransport();
        if (JointStateCodec.ROS_VERSION != transport.Backend ||
            JointStateCodec.NATIVE_TYPE != (transport.Backend == 1 ? "sensor_msgs/JointState" : "sensor_msgs/msg/JointState") ||
            JointStateCodec.MAX_WIRE_BYTES < 1 || JointStateCodec.MAX_WIRE_BYTES > 32768)
            throw new ArgumentException("actual complete generated codec bound required");
        var session = new Session(transport, maximumOwners:1);
        LastSession = session; // retain original LastSetupCleanup/owner diagnostics
        Exception? primary = null;
        try {
            session.CheckAbi();
            if (Mode == 0) {
                try {
                    session.Open("joint-state");
                    throw new InvalidOperationException("default-denied open unexpectedly succeeded");
                } catch (RosException denied) {
                    if (denied.Result.Error != 13 || denied.Result.Value != null) throw;
                    DeniedVerified = true;
                }
            } else {
                Subscription<JointState> subscription = new Node(session).CreateSubscription(
                    JointStateCodec.Instance, "joint-state", JointStateCodec.MAX_WIRE_BYTES, Observe);
                using Stream input = Console.OpenStandardInput();
                for (int credit=0; credit<64 && ReceivedCount==0; ++credit) {
                    if (input.ReadByte() != (int)'R')
                        throw new FormatException("one actual binary R byte per poll");
                    Result result = subscription.PollOnce();
                    if (result.Error != 0 && result.Error != 11) result.Checked();
                }
                if (ReceivedCount != 1)
                    throw new InvalidOperationException("no one complete value within64 credits");
            }
        } catch (Exception failure) { primary=LastPrimary=failure; }
        finally {
            // Keep application Primary and every cleanup result independently.
            try { LastCleanup=session.CloseChecked(primary); }
            catch (Exception cleanup) { LastCleanupException=cleanup;if(primary==null)primary=cleanup; }
        }
        if (primary != null) throw primary;
        if (LastCleanup == null) throw new InvalidOperationException("no cleanup report");
        LastCleanup.ThrowIfFailed();
        if (!LastCleanup.Succeeded)
            throw new InvalidOperationException("cleanup failed; no success marker");
        if (Mode==0 && (!DeniedVerified || ReceivedCount!=0))
            throw new InvalidOperationException("default denial oracle");
        // Actual native close(s) returned and were checked before success.
        Console.WriteLine(Mode==0 ? "PASS_JOINT_STATE_DEFAULT_DENIED" : "PASS_JOINT_STATE_FULL_VALUE");
        Console.Out.Flush();
        return 0;
    }
}
