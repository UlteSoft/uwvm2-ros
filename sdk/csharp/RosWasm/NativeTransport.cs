using System;
using System.Buffers.Binary;
using System.Runtime.InteropServices;
using System.Threading;
namespace Uwvm.RosWasm;
#if UWVM_ROS1 && UWVM_ROS2
#error Exactly one ROS backend must be selected
#endif
#if !UWVM_ROS1 && !UWVM_ROS2
#error Define UWVM_ROS1 or UWVM_ROS2 and compile the matching original C object
#endif

/// <summary>Real copied-buffer Mono/WASI native bridge. Construction imports no alias or authority.</summary>
public sealed partial class NativeTransport : ITransport {
#if UWVM_ROS1
    public int Backend=>1;
#else
    public int Backend=>2;
#endif
    private static readonly object Gate=new();
    private static bool active;
    public static uint? LastOpenCleanupError { get; private set; }
    public static Exception? LastOpenCleanupException { get; private set; }
    /// <summary>Latest scalar call only; capture immediately. Capability retains its own diagnostic.</summary>
    public static RetirementDiagnostic LastScalarRetirement { get; private set; }
#if UWVM_ROSWASM_RETIREMENT_FAULT_TESTS
    internal enum RetirementFaultPoint { BeforeFrameAllocation,AfterGateAdmission,AfterCallSiteEntered,AfterRawReturned,AfterGateReleased,BeforeTransportInvoke }
    internal static Action<RetirementFaultPoint>? RetirementFaultHook;
    private static void RetirementFault(RetirementFaultPoint point) { RetirementFaultHook?.Invoke(point); }
#endif
    internal sealed class ScalarRetirementFrame {
        internal readonly ulong Handle;
        internal readonly RetirementOperation Operation;
        private readonly Result outcome;
        internal bool GateHeld,OwnsActive;
        internal RetirementPhase Phase { get; private set; }
        internal RetirementBoundary Boundary { get; private set; }
        internal uint? Error { get; private set; }
        internal Exception? CleanupException { get; private set; }
        internal ScalarRetirementFrame(RetirementOperation operation,ulong handle) {
            Handle=handle;Operation=operation;outcome=new Result(0);
        }
        internal Result Attempt() {
            if(!GateHeld||!OwnsActive||Phase!=RetirementPhase.NotAttempted)
                throw new InvalidOperationException("scalar frame is not an unused admitted native frame");
            Boundary=RetirementBoundary.NativeInteropCallSite;
            Phase=RetirementPhase.AttemptedEffectUnknown;
#if UWVM_ROSWASM_RETIREMENT_FAULT_TESTS
            RetirementFault(RetirementFaultPoint.AfterCallSiteEntered);
#endif
            uint code=Operation==RetirementOperation.Close?SelectedRaw.capability_close(Handle):SelectedRaw.capability_revoke(Handle);
            Error=code;Phase=RetirementPhase.Returned;outcome.SetScalarRetirementError(code);
#if UWVM_ROSWASM_RETIREMENT_FAULT_TESTS
            RetirementFault(RetirementFaultPoint.AfterRawReturned);
#endif
            return outcome;
        }
        // Never throw while preserving a primary. The caller records cleanup
        // separately, and throws it only if there was no primary exception.
        internal void Release() {
            if(OwnsActive) { active=false;OwnsActive=false; }
            if(!GateHeld)return;
            try { Monitor.Exit(Gate);GateHeld=false;
#if UWVM_ROSWASM_RETIREMENT_FAULT_TESTS
                RetirementFault(RetirementFaultPoint.AfterGateReleased);
#endif
            }catch(Exception error) { CleanupException=error; }
        }
    }
    internal void PrepareScalarRetirement(RetirementOperation operation,ulong handle,out ScalarRetirementFrame? frame) {
        frame=null;
#if UWVM_ROSWASM_RETIREMENT_FAULT_TESTS
        RetirementFault(RetirementFaultPoint.BeforeFrameAllocation);
#endif
        // Every allocation precedes local retirement. A failed constructor never
        // entered the gate; a later failure leaves its real frame visible to finally.
        frame=new ScalarRetirementFrame(operation,handle);
        bool entered=false;
        try { Monitor.Enter(Gate,ref entered); }
        finally { frame.GateHeld=entered; }
        if(active)throw new InvalidOperationException("reentrant native bridge");
        active=true;frame.OwnsActive=true;
        LastOpenCleanupError=null;LastOpenCleanupException=null;
#if UWVM_ROSWASM_RETIREMENT_FAULT_TESTS
        RetirementFault(RetirementFaultPoint.AfterGateAdmission);
#endif
    }
    internal static RetirementDiagnostic ScalarDiagnostic(ulong handle,RetirementOperation operation,
        ScalarRetirementFrame? frame,Exception? primary) {
        var value=new RetirementDiagnostic(handle,operation,frame?.Phase??RetirementPhase.NotAttempted,
            frame?.Boundary??RetirementBoundary.None,frame?.Error,primary,frame?.CleanupException);
        LastScalarRetirement=value;return value;
    }
    private Result InvokeScalarRetirement(RetirementOperation operation,ulong handle) {
        ScalarRetirementFrame? frame=null;Exception? primary=null;
        try { PrepareScalarRetirement(operation,handle,out frame);return frame!.Attempt(); }
        catch(Exception error) { primary=error;throw; }
        finally {
            frame?.Release();ScalarDiagnostic(handle,operation,frame,primary);
            if(primary==null&&frame?.CleanupException is Exception cleanup)throw cleanup;
        }
    }
    private enum OutputKind { None,Length,Value,Ready }
    // Explicit wasm32 zero extension, exactly as the real generic Mono native proof.
    private static ulong Address(IntPtr p)=>IntPtr.Size==4?unchecked((ulong)(uint)p.ToInt32()):unchecked((ulong)p.ToInt64());
    public Result Invoke(string operation,ulong handle,byte[] input,int capacity,ulong other=0,ulong correlation=0) {
#if UWVM_ROSWASM_RETIREMENT_FAULT_TESTS
        RetirementFault(RetirementFaultPoint.BeforeTransportInvoke);
#endif
        if(operation==null||input==null||input.Length>4193786||capacity<0||capacity>4194304)throw new ArgumentException("bounded copied-buffer request");
        // Scalar-only ABI: no clone/output/slot/GCHandle preparation is needed.
        if(operation=="capability_close")return InvokeScalarRetirement(RetirementOperation.Close,handle);
        if(operation=="capability_revoke")return InvokeScalarRetirement(RetirementOperation.Revoke,handle);
        lock(Gate) {
            if(active)throw new InvalidOperationException("reentrant native bridge");
            // Allocate complete managed storage before extracting any numeric address.
            byte[] request=(byte[])input.Clone(),output=new byte[capacity],slot=new byte[8];
            BinaryPrimitives.WriteUInt64LittleEndian(slot,ulong.MaxValue);
            GCHandle a=default,b=default,c=default;ulong opened=0;active=true;LastOpenCleanupError=null;LastOpenCleanupException=null;
            try {
                a=GCHandle.Alloc(request,GCHandleType.Pinned);b=GCHandle.Alloc(output,GCHandleType.Pinned);c=GCHandle.Alloc(slot,GCHandleType.Pinned);
                ulong p=Address(a.AddrOfPinnedObject()),q=Address(b.AddrOfPinnedObject()),slotOffset=Address(c.AddrOfPinnedObject());
                uint code;OutputKind kind;
                switch(operation) {
#if UWVM_ROS1
                case "abi_version":
                    return new Result(0,value:SelectedRaw.abi_version());
                case "capability_open":
                    if(request.Length==0||request.Length>255)throw new ArgumentException("alias byte length");
                    BinaryPrimitives.WriteUInt64LittleEndian(slot,0);
                    code=SelectedRaw.capability_open(0, p, (ulong)request.Length, slotOffset);
                    if(code==0)opened=BinaryPrimitives.ReadUInt64LittleEndian(slot);
                    kind=OutputKind.Value;break;
                case "capability_close":
                    code=SelectedRaw.capability_close(handle);
                    kind=OutputKind.None;break;
                case "capability_revoke":
                    code=SelectedRaw.capability_revoke(handle);
                    kind=OutputKind.None;break;
                case "topic_publish":
                    code=SelectedRaw.topic_publish(handle, 0, p, (ulong)request.Length);
                    kind=OutputKind.None;break;
                case "topic_subscribe":
                    code=SelectedRaw.topic_subscribe(handle);
                    kind=OutputKind.None;break;
                case "event_poll":
                    code=SelectedRaw.event_poll(handle, 0, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "event_watch":
                    code=SelectedRaw.event_watch(handle, other, correlation);
                    kind=OutputKind.None;break;
                case "event_poll_v1":
                    code=SelectedRaw.event_poll_v1(handle, 0, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "event_ready":
                    if(capacity!=32)throw new ArgumentException("event_ready requires32 bytes");
                    code=SelectedRaw.event_ready(handle, 0, q);
                    kind=OutputKind.Ready;break;
                case "service_submit":
                    BinaryPrimitives.WriteUInt64LittleEndian(slot,0);
                    code=SelectedRaw.service_submit(handle, other, 0, p, (ulong)request.Length, slotOffset);
                    kind=OutputKind.Value;break;
                case "service_call":
                    code=SelectedRaw.service_call(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "service_advertise":
                    code=SelectedRaw.service_advertise(handle);
                    kind=OutputKind.None;break;
                case "service_respond":
                    code=SelectedRaw.service_respond(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "action_send_goal":
                    code=SelectedRaw.action_send_goal(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "action_cancel_own_goal":
                    code=SelectedRaw.action_cancel_own_goal(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "action_cancel_any_goal":
                    code=SelectedRaw.action_cancel_any_goal(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "action_receive_feedback":
                    code=SelectedRaw.action_receive_feedback(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "action_receive_result":
                    code=SelectedRaw.action_receive_result(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "action_advertise":
                    code=SelectedRaw.action_advertise(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "action_respond":
                    code=SelectedRaw.action_respond(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "tf_lookup":
                    code=SelectedRaw.tf_lookup(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "tf_broadcast":
                    code=SelectedRaw.tf_broadcast(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "parameter_get":
                    code=SelectedRaw.parameter_get(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "parameter_set":
                    code=SelectedRaw.parameter_set(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "parameter_delete":
                    code=SelectedRaw.parameter_delete(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "parameter_exists":
                    code=SelectedRaw.parameter_exists(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "parameter_list":
                    code=SelectedRaw.parameter_list(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "parameter_search":
                    code=SelectedRaw.parameter_search(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "graph_inspect_topic":
                    code=SelectedRaw.graph_inspect_topic(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "graph_inspect_node":
                    code=SelectedRaw.graph_inspect_node(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "graph_inspect_service":
                    code=SelectedRaw.graph_inspect_service(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "graph_list_topics":
                    code=SelectedRaw.graph_list_topics(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "graph_list_nodes":
                    code=SelectedRaw.graph_list_nodes(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "graph_list_services":
                    code=SelectedRaw.graph_list_services(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "time_read_ros":
                    code=SelectedRaw.time_read_ros(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "time_read_wall":
                    code=SelectedRaw.time_read_wall(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "log_write":
                    code=SelectedRaw.log_write(handle, 0, p, (ulong)request.Length);
                    kind=OutputKind.None;break;
#endif
#if UWVM_ROS2
                case "abi_version":
                    return new Result(0,value:SelectedRaw.abi_version());
                case "capability_open":
                    if(request.Length==0||request.Length>255)throw new ArgumentException("alias byte length");
                    BinaryPrimitives.WriteUInt64LittleEndian(slot,0);
                    code=SelectedRaw.capability_open(0, p, (ulong)request.Length, slotOffset);
                    if(code==0)opened=BinaryPrimitives.ReadUInt64LittleEndian(slot);
                    kind=OutputKind.Value;break;
                case "capability_close":
                    code=SelectedRaw.capability_close(handle);
                    kind=OutputKind.None;break;
                case "capability_revoke":
                    code=SelectedRaw.capability_revoke(handle);
                    kind=OutputKind.None;break;
                case "topic_publish":
                    code=SelectedRaw.topic_publish(handle, 0, p, (ulong)request.Length);
                    kind=OutputKind.None;break;
                case "topic_subscribe":
                    code=SelectedRaw.topic_subscribe(handle);
                    kind=OutputKind.None;break;
                case "event_poll":
                    code=SelectedRaw.event_poll(handle, 0, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "event_watch":
                    code=SelectedRaw.event_watch(handle, other, correlation);
                    kind=OutputKind.None;break;
                case "event_poll_v1":
                    code=SelectedRaw.event_poll_v1(handle, 0, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "event_ready":
                    if(capacity!=32)throw new ArgumentException("event_ready requires32 bytes");
                    code=SelectedRaw.event_ready(handle, 0, q);
                    kind=OutputKind.Ready;break;
                case "service_submit":
                    BinaryPrimitives.WriteUInt64LittleEndian(slot,0);
                    code=SelectedRaw.service_submit(handle, other, 0, p, (ulong)request.Length, slotOffset);
                    kind=OutputKind.Value;break;
                case "service_call":
                    code=SelectedRaw.service_call(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "service_advertise":
                    code=SelectedRaw.service_advertise(handle);
                    kind=OutputKind.None;break;
                case "service_respond":
                    code=SelectedRaw.service_respond(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "action_send_goal":
                    code=SelectedRaw.action_send_goal(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "action_cancel_own_goal":
                    code=SelectedRaw.action_cancel_own_goal(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "action_cancel_any_goal":
                    code=SelectedRaw.action_cancel_any_goal(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "action_receive_feedback":
                    code=SelectedRaw.action_receive_feedback(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "action_receive_result":
                    code=SelectedRaw.action_receive_result(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "action_advertise":
                    code=SelectedRaw.action_advertise(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "action_respond":
                    code=SelectedRaw.action_respond(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "tf_lookup":
                    code=SelectedRaw.tf_lookup(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "tf_broadcast":
                    code=SelectedRaw.tf_broadcast(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "parameter_get":
                    code=SelectedRaw.parameter_get(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "parameter_set":
                    code=SelectedRaw.parameter_set(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "parameter_list":
                    code=SelectedRaw.parameter_list(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "parameter_describe":
                    code=SelectedRaw.parameter_describe(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "parameter_declare":
                    code=SelectedRaw.parameter_declare(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "parameter_undeclare":
                    code=SelectedRaw.parameter_undeclare(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "graph_inspect_topic":
                    code=SelectedRaw.graph_inspect_topic(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "graph_inspect_node":
                    code=SelectedRaw.graph_inspect_node(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "graph_inspect_service":
                    code=SelectedRaw.graph_inspect_service(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "graph_inspect_action":
                    code=SelectedRaw.graph_inspect_action(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "parameter_subscribe_events":
                    code=SelectedRaw.parameter_subscribe_events(handle);
                    kind=OutputKind.None;break;
                case "graph_list_topics":
                    code=SelectedRaw.graph_list_topics(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "graph_list_nodes":
                    code=SelectedRaw.graph_list_nodes(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "graph_list_services":
                    code=SelectedRaw.graph_list_services(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "lifecycle_get_state":
                    code=SelectedRaw.lifecycle_get_state(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "lifecycle_change_state":
                    code=SelectedRaw.lifecycle_change_state(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "time_read_ros":
                    code=SelectedRaw.time_read_ros(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "time_read_system":
                    code=SelectedRaw.time_read_system(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "time_read_steady":
                    code=SelectedRaw.time_read_steady(handle, 0, p, (ulong)request.Length, q, (ulong)capacity, slotOffset);
                    kind=OutputKind.Length;break;
                case "log_write":
                    code=SelectedRaw.log_write(handle, 0, p, (ulong)request.Length);
                    kind=OutputKind.None;break;
#endif
                default:return new Result(95);
                }
                ulong value=BinaryPrimitives.ReadUInt64LittleEndian(slot);
                Result result;
                if(code!=0) result=new Result(code,required:kind==OutputKind.Length&&code==90&&value!=ulong.MaxValue?value:null);
                else if(kind==OutputKind.Value) {
                    if(value==0)throw new InvalidOperationException("successful value slot was not written");
                    result=new Result(0,value:value); // UINT64_MAX remains a valid opaque value.
                } else if(kind==OutputKind.Length) {
                    if(value==ulong.MaxValue||value>(ulong)capacity)throw new InvalidOperationException("invalid successful output length");
                    byte[] exact=new byte[(int)value];Buffer.BlockCopy(output,0,exact,0,exact.Length);result=new Result(0,exact);
                } else result=new Result(0,kind==OutputKind.Ready?output:Array.Empty<byte>());
                opened=0;return result;
            } catch {
                // A known successful open belongs to this frame until Result construction succeeds.
                // Cleanup does not allocate, replay an admission, or reinterpret a submit ID as a handle.
                if(opened!=0) { try {LastOpenCleanupError=SelectedRaw.capability_close(opened);}catch(Exception cleanup) {LastOpenCleanupException=cleanup;} }
                throw;
            } finally {
                if(c.IsAllocated)c.Free();if(b.IsAllocated)b.Free();if(a.IsAllocated)a.Free();active=false;
                // All arrays are held by GCHandle until the complete synchronous import has returned.
            }
        }
    }
}
