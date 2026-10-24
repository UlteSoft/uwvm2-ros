// Explicit actual-Guest fixture source. NOT executed, not a positive ROS result.
using System;
using Uwvm.RosWasm;
using Uwvm.RosWasm.Owned;
using Uwvm.RosWasm.Generated.Control;
using Q=Uwvm.RosWasm.Generated.Control.BoundedRecords._Ros2Type_description_interfacesSrvGetTypeDescription_Request;
using S=Uwvm.RosWasm.Generated.Control.BoundedRecords._Ros2Type_description_interfacesSrvGetTypeDescription_Response;
internal sealed class GuestDecodeOom<T> : ICodec<T> {
    private readonly ICodec<T> real;
    internal GuestDecodeOom(ICodec<T> real) { this.real=real; }
    public byte[] Encode(T value)=>real.Encode(value);
    public T Decode(byte[] bytes) { _=real.Decode(bytes);throw new OutOfMemoryException("explicit Guest fixture OOM before ACK"); }
}
internal static class GuestFaultFixture {
    private static int Main(string[] a) {
        // MODE call observer actual-type actual-RIHS finite-poll-steps
        if(a.Length!=6)return 2;
        OwnedGroup? group=null;Exception? primary=null;
        var injected=new InvalidOperationException("explicit owned Guest fault; actual returned status retained");
        try {
            string mode=a[0];if(mode!="pre-submit"&&mode!="after-submit"&&mode!="after-commit"&&mode!="decode-oom"&&mode!="cleanup"&&mode!="reentry"&&mode!="abandon-late")throw new ArgumentException("unknown fixture mode");
            int steps=AppCommon.Steps(a[5]);var d=ControlRoots.Ros2Type_description_interfacesSrvGetTypeDescription();
            if(mode=="decode-oom")d=new ServiceDefinition<Q,S>(d.NativeType,d.CodecIdentity,d.Request,new GuestDecodeOom<S>(d.Response),d.MaximumRequest,d.MaximumResponse);
            group=new OwnedGroup(1,1,d.MaximumRequest,d.MaximumResponse);var c=group.OpenClient(d,a[1],a[2]);
            NativeTransport.OwnedGuestFault=(op,point)=> {
                if(mode=="pre-submit"&&op==OwnedOperation.Submit&&point==NativeTransport.OwnedFaultPoint.BeforeEntry)throw injected;
                if(mode=="after-submit"&&op==OwnedOperation.Submit&&point==NativeTransport.OwnedFaultPoint.AfterReturnedStatus)throw injected;
                if(mode=="after-commit"&&op==OwnedOperation.Commit&&point==NativeTransport.OwnedFaultPoint.AfterReturnedStatus)throw injected;
                if(mode=="cleanup"&&op==OwnedOperation.Retire&&point==NativeTransport.OwnedFaultPoint.AfterReturnedStatus)throw injected;
            };
            if(mode=="reentry") { var report=c.Close(()=>c.Close());if(report.CloseHookException is not OwnedServiceException e||e.Code!=17)throw new InvalidOperationException("close reentry contract failed");throw injected; }
            if(mode=="cleanup") { _=c.Close();throw injected; }
            var t=c.Submit(new Q { @type_name=a[3],@type_hash=a[4],@include_type_sources=true });
            if(mode=="abandon-late")c.Abandon(t);
            bool actualDelivery=false;
            for(int i=0;i<steps;i++) { var item=c.Poll();
                if(mode=="abandon-late") { if(item.Present)throw new InvalidOperationException("abandoned value/control exposed unexpectedly; retain actual evidence"); }
                else if(item.Present) { actualDelivery=true;break; }
            }
            if(mode=="abandon-late")throw new InvalidOperationException("explicit abandoned local outcome; controlled provider timing required to establish late-disposal evidence");
            if(!actualDelivery)throw new InvalidOperationException("no actual full response before work bound; fault was not qualified");
            throw new InvalidOperationException("requested after-effect/OOM fault did not occur");
        }catch(Exception error) { primary=error; }
        finally { NativeTransport.OwnedGuestFault=null; }
        if(group==null) { Console.Error.WriteLine(primary);return 1; }
        // Fault outcomes intentionally remain nonzero. An external runner must
        // inspect exact attempt/status/cancel/close evidence, not call nonzero PASS.
        return AppCommon.Finish(group,primary);
    }
}
