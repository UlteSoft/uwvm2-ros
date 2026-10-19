// Optional same-Wasm Mono/WASI bridge. This file is excluded by the ordinary SDK project.
using System;
using System.Runtime.InteropServices;
using System.Threading;
namespace Uwvm.RosWasm;
#if !UWVM_ROS2 || !UWVM_ROSWASM_OWNED_SERVICE
#error The owned bridge requires explicit UWVM_ROSWASM_OWNED_SERVICE and ROS2
#endif

internal enum OwnedOperation { Version, BusinessVersion, Open, ActivateClient, ActivateService, Inspect, Submit, Abandon, Respond, Peek, Commit, Cancel, Available, Retire }

/// <summary>Owned before an effect. Entered records the concrete interop attempt only.</summary>
public sealed class OwnedAttempt {
    public bool Entered { get; internal set; }
    public bool Returned { get; internal set; }
    public uint Code { get; internal set; }
    public Exception? Exception { get; internal set; }
    public Exception? CleanupException { get; internal set; }
    internal void Reset() { Entered=false;Returned=false;Code=0;Exception=null;CleanupException=null; }
    public bool Succeeded=>Returned&&Code==0&&Exception==null&&CleanupException==null;
}
internal sealed class OwnedCall {
    internal OwnedOperation Operation;
    internal ulong Source,Incarnation,Peer,Ticket,Sequence,Nonce,Wait;
    internal byte[] Input=Array.Empty<byte>(),Output=Array.Empty<byte>();
    internal int InputLength;
    internal readonly OwnedAttempt Evidence;
    internal OwnedCall(OwnedAttempt evidence) { Evidence=evidence; }
}

public sealed partial class NativeTransport {
#if UWVM_ROSWASM_OWNED_GUEST_FAULTS
    internal enum OwnedFaultPoint { BeforeEntry,AfterReturnedStatus }
    internal static Action<OwnedOperation,OwnedFaultPoint>? OwnedGuestFault;
#endif
    internal void InvokeOwned(OwnedCall call) {
        var e=call.Evidence;e.Reset();
        GCHandle input=default,output=default;bool locked=false,admitted=false;
        try {
            // The compiled C object also refuses a non-Wasm target. Do not permit
            // an ambient desktop CLR to resolve the named bridge as a native ROS library.
            if(!OperatingSystem.IsWasi()||IntPtr.Size!=4||Backend!=2) { e.Code=95;return; }
            if(call.Input==null||call.Output==null||call.InputLength<0||call.InputLength>call.Input.Length||
                call.InputLength>32768||call.Output.Length>4194304) { e.Code=22;return; }
            if(call.Operation!=OwnedOperation.Version&&call.Operation!=OwnedOperation.BusinessVersion&&call.Operation!=OwnedOperation.Open&&
                ((uint)call.Source==0||(uint)(call.Source>>32)==0)) { e.Code=22;return; }
            bool paired=(uint)call.Peer!=0&&(uint)(call.Peer>>32)!=0&&call.Peer!=call.Source;
            bool form=call.Operation switch {
                OwnedOperation.Version or OwnedOperation.BusinessVersion=>call.InputLength==0&&call.Output.Length==0,
                OwnedOperation.Open=>call.InputLength==call.Input.Length&&call.InputLength>=1&&call.InputLength<=255&&call.Output.Length==8&&Array.IndexOf(call.Input,(byte)0)<0,
                OwnedOperation.ActivateClient or OwnedOperation.ActivateService=>paired&&call.Incarnation==0&&call.InputLength==0&&call.Output.Length==256,
                OwnedOperation.Inspect=>call.Incarnation!=0&&call.InputLength==0&&call.Output.Length==256,
                OwnedOperation.Submit=>call.Incarnation!=0&&call.InputLength==call.Input.Length&&call.Output.Length==8,
                OwnedOperation.Abandon=>call.Incarnation!=0&&call.Ticket!=0&&call.InputLength==0&&call.Output.Length==0,
                OwnedOperation.Respond=>paired&&call.Incarnation!=0&&call.Ticket!=0&&call.InputLength==call.Input.Length&&call.Output.Length==0,
                OwnedOperation.Peek=>call.Incarnation!=0&&call.InputLength==0&&call.Output.Length>=352,
                OwnedOperation.Commit or OwnedOperation.Cancel=>call.Incarnation!=0&&call.Sequence!=0&&call.Nonce!=0&&call.InputLength==0&&call.Output.Length==0,
                OwnedOperation.Available=>paired&&call.Incarnation!=0&&call.Wait<=60000000000UL&&call.InputLength==0&&call.Output.Length==1,
                OwnedOperation.Retire=>call.Incarnation!=0&&call.InputLength==0&&call.Output.Length==0,
                _=>false
            };
            if(!form) { e.Code=22;return; }
            // No managed allocation, setter, callback, marshalling array or delegate
            // participates in the entered transition. All pointer storage is pinned now.
            input=GCHandle.Alloc(call.Input,GCHandleType.Pinned);
            output=GCHandle.Alloc(call.Output,GCHandleType.Pinned);
            ulong p=Address(input.AddrOfPinnedObject()),q=Address(output.AddrOfPinnedObject());
            Monitor.Enter(Gate,ref locked);
            if(active) { e.Code=17;return; }
            active=true;admitted=true;
#if UWVM_ROSWASM_OWNED_GUEST_FAULTS
            // Fixture only: no callback between Entered and its concrete import.
            OwnedGuestFault?.Invoke(call.Operation,OwnedFaultPoint.BeforeEntry);
#endif
            uint code;
            switch(call.Operation) {
                case OwnedOperation.Version: e.Entered=true;code=OwnedRaw.Version();break;
                case OwnedOperation.BusinessVersion:e.Entered=true;code=SelectedRaw.abi_version();break;
                case OwnedOperation.Open:e.Entered=true;code=SelectedRaw.capability_open(0,p,(ulong)call.InputLength,q);break;
                case OwnedOperation.ActivateClient: e.Entered=true;code=OwnedRaw.ClientActivate(call.Source,call.Peer,0,q);break;
                case OwnedOperation.ActivateService: e.Entered=true;code=OwnedRaw.ServiceActivate(call.Source,call.Peer,0,q);break;
                case OwnedOperation.Inspect: e.Entered=true;code=OwnedRaw.Inspect(call.Source,call.Incarnation,0,q);break;
                case OwnedOperation.Submit: e.Entered=true;code=OwnedRaw.Submit(call.Source,call.Incarnation,0,p,(ulong)call.InputLength,q);break;
                case OwnedOperation.Abandon: e.Entered=true;code=OwnedRaw.Abandon(call.Source,call.Incarnation,call.Ticket);break;
                case OwnedOperation.Respond: e.Entered=true;code=OwnedRaw.Respond(call.Source,call.Incarnation,call.Peer,call.Ticket,0,p,(ulong)call.InputLength);break;
                case OwnedOperation.Peek: e.Entered=true;code=OwnedRaw.Peek(call.Source,call.Incarnation,16,0,q,(ulong)call.Output.Length);break;
                case OwnedOperation.Commit: e.Entered=true;code=OwnedRaw.Commit(call.Source,call.Incarnation,call.Sequence,call.Nonce);break;
                case OwnedOperation.Cancel: e.Entered=true;code=OwnedRaw.Cancel(call.Source,call.Incarnation,call.Sequence,call.Nonce);break;
                case OwnedOperation.Available: e.Entered=true;code=OwnedRaw.Available(call.Source,call.Incarnation,call.Peer,call.Wait,0,q);break;
                case OwnedOperation.Retire: e.Entered=true;code=OwnedRaw.Retire(call.Source,call.Incarnation);break;
                default:e.Code=22;return;
            }
            e.Code=code;e.Returned=true;
#if UWVM_ROSWASM_OWNED_GUEST_FAULTS
            OwnedGuestFault?.Invoke(call.Operation,OwnedFaultPoint.AfterReturnedStatus);
#endif
        } catch(Exception error) { e.Exception=error;if(e.Entered&&!e.Returned)e.Code=115; }
        finally {
            try { if(admitted)active=false;if(locked)Monitor.Exit(Gate); }
            catch(Exception error) { e.CleanupException=error; }
            try { if(output.IsAllocated)output.Free();if(input.IsAllocated)input.Free(); }
            catch(Exception error) { if(e.CleanupException==null)e.CleanupException=error; }
        }
    }
}

internal static class OwnedRaw {
    private const string Module="roswasm_owned_bridge";
    [DllImport(Module,EntryPoint="roswasm_owned_abi_version",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)] internal static extern uint Version();
    [DllImport(Module,EntryPoint="roswasm_owned_client_activate",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)] internal static extern uint ClientActivate(ulong h,ulong peer,uint memory,ulong output);
    [DllImport(Module,EntryPoint="roswasm_owned_service_activate",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)] internal static extern uint ServiceActivate(ulong h,ulong peer,uint memory,ulong output);
    [DllImport(Module,EntryPoint="roswasm_owned_endpoint_inspect",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)] internal static extern uint Inspect(ulong h,ulong i,uint memory,ulong output);
    [DllImport(Module,EntryPoint="roswasm_owned_client_submit",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)] internal static extern uint Submit(ulong h,ulong i,uint memory,ulong input,ulong n,ulong ticket);
    [DllImport(Module,EntryPoint="roswasm_owned_client_abandon",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)] internal static extern uint Abandon(ulong h,ulong i,ulong ticket);
    [DllImport(Module,EntryPoint="roswasm_owned_service_respond",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)] internal static extern uint Respond(ulong h,ulong i,ulong peer,ulong ticket,uint memory,ulong input,ulong n);
    [DllImport(Module,EntryPoint="roswasm_owned_endpoint_peek",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)] internal static extern uint Peek(ulong h,ulong i,uint writer,uint memory,ulong output,ulong capacity);
    [DllImport(Module,EntryPoint="roswasm_owned_endpoint_commit",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)] internal static extern uint Commit(ulong h,ulong i,ulong seq,ulong nonce);
    [DllImport(Module,EntryPoint="roswasm_owned_endpoint_cancel",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)] internal static extern uint Cancel(ulong h,ulong i,ulong seq,ulong nonce);
    [DllImport(Module,EntryPoint="roswasm_owned_client_available",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)] internal static extern uint Available(ulong h,ulong i,ulong query,ulong wait,uint memory,ulong output);
    [DllImport(Module,EntryPoint="roswasm_owned_endpoint_retire",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)] internal static extern uint Retire(ulong h,ulong i);
}
