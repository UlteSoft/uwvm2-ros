using System;
using System.Buffers.Binary;
using Uwvm.RosWasm;

/// <summary>Unexecuted injected local policy cases. This model is never a NativeTransport/native ROS proof.</summary>
internal static class PolicyContract {
    private sealed class Model : ITransport {
        public int Backend=>2;public ulong Next=1;public uint OperationError;public int Calls,Closes;public bool FailFirstClose;
        public string? Reject;public byte[] Poll=Array.Empty<byte>();public Result? Reply;
        public Result Invoke(string operation,ulong handle,byte[] input,int capacity,ulong other=0,ulong correlation=0) {
            Calls++;if(operation=="capability_open")return new Result(0,value:Next++);
            if(operation=="capability_close") { Closes++;if(FailFirstClose&&Closes==1)throw new InvalidOperationException("injected model close failure");return new Result(0); }
            if(operation==Reject)return new Result(OperationError,required:OperationError==90?999UL:null);
            if(operation=="event_poll"||operation=="event_poll_v1")return new Result(0,Poll);
            return Reply??new Result(0);
        }
    }
    private static void Need(bool ok) { if(!ok)throw new Exception("local policy failure"); }
    private static void Refuses(Action operation) { bool thrown=false;try { operation(); }catch(ArgumentException) { thrown=true; }catch(FormatException) { thrown=true; }Need(thrown); }
    private static byte[] Envelope(ulong source,ulong sequence) {
        byte[] b=new byte[64];BinaryPrimitives.WriteUInt32LittleEndian(b,0x45525755);BinaryPrimitives.WriteUInt16LittleEndian(b.AsSpan(4),1);BinaryPrimitives.WriteUInt16LittleEndian(b.AsSpan(6),1);b[8]=2;
        BinaryPrimitives.WriteUInt64LittleEndian(b.AsSpan(16),source);BinaryPrimitives.WriteUInt64LittleEndian(b.AsSpan(32),sequence);BinaryPrimitives.WriteUInt64LittleEndian(b.AsSpan(40),1);BinaryPrimitives.WriteUInt64LittleEndian(b.AsSpan(48),2);return b;
    }
    private static void Utf8Whole() { string s="\uFEFFzero\0π中🙂";Need(Codecs.String.Decode(Codecs.String.Encode(s))==s); }
    private static void Utf8Reject() { Refuses(()=>Codecs.String.Decode(new byte[]{2,0,0,0,0xc0,0x80})); }
    private static void BoolWhole() { Need(Codecs.Bool.Decode(Codecs.Bool.Encode(true)));Refuses(()=>Codecs.Bool.Decode(new byte[]{1,0}));Refuses(()=>Codecs.Bool.Decode(new byte[]{2})); }
    private static void TwistWhole() { double[] a={-0.0,1.25,-2.5,3,4,5};double[] b=Codecs.Twist.Decode(Codecs.Twist.Encode(a));Need(BitConverter.DoubleToInt64Bits(a[0])==BitConverter.DoubleToInt64Bits(b[0])&&b[2]==-2.5);Refuses(()=>Codecs.Twist.Encode(new[]{double.NaN,0d,0d,0d,0d,0d})); }
    private static void Int64Whole() { Need(Codecs.Int64.Decode(Codecs.Int64.Encode(long.MinValue))==long.MinValue); }
    private static void ExactEventHeader() { byte[] b=Envelope(ulong.MaxValue,1);Need(RosEvent.Decode(b).Source==ulong.MaxValue);b[10]=1;Refuses(()=>RosEvent.Decode(b)); }
    private static void WholeObserverSequence() { var m=new Model();var s=new Session(m);var a=s.Open("a");var b=s.Open("b");var o=new Node(s).CreateObserver("observer",64);o.Watch(a).Checked();o.Watch(b).Checked();m.Poll=Envelope(a.Handle,1);o.PollOnce().Checked();m.Poll=Envelope(b.Handle,2);o.PollOnce().Checked();Need(o.LastEvent!.Source==b.Handle);s.CloseChecked().ThrowIfFailed(); }
    private static void CapacityNoReplay() { var m=new Model{Reject="topic_publish",OperationError=90};var s=new Session(m);var o=s.Open("publish");int n=m.Calls;Result r=o.Invoke("topic_publish");Need(r.Error==90&&r.Required==999&&m.Calls==n+1);s.CloseChecked(); }
    private static void TimeoutNoReplay() { var m=new Model{Reject="service_call",OperationError=110};var s=new Session(m);var o=s.Open("service");int n=m.Calls;Need(o.Invoke("service_call").Error==110&&m.Calls==n+1);s.CloseChecked(); }
    private static void MaximumOpaqueOwner() { var m=new Model{Next=ulong.MaxValue};var s=new Session(m);Capability o=s.Open("full-width");Need(o.Handle==ulong.MaxValue);s.CloseChecked(); }
    private static void EveryCleanupAttempt() { var m=new Model{FailFirstClose=true};var s=new Session(m);s.Open("a");s.Open("b");s.Open("c");var report=s.CloseChecked();Need(m.Closes==3&&report.Count==3&&!report.Succeeded&&report.Items[0].Exception!=null); }
    private static void BusyCallbackPreflight() { var m=new Model{Poll=Codecs.String.Encode("callback")};var s=new Session(m);bool refused=false;var sub=new Node(s).CreateSubscription(Codecs.String,"sub",1024,_=>{try { s.CloseChecked(); }catch(RosException e)when(e.Result.Error==17) { refused=true; }});sub.PollOnce().Checked();Need(refused&&m.Closes==0);s.CloseChecked(); }
    private static void RevokeWholeAlias() { var m=new Model();var s=new Session(m);var a=s.Open("same");var b=s.Open("same");a.RevokeChecked().Checked();Need(a.Handle==0&&b.Handle==0);s.CloseChecked();Need(m.Closes==0); }
    private static void FactoryRollback() { var m=new Model{Reject="topic_subscribe",OperationError=13};var s=new Session(m);try { new Node(s).CreateSubscription(Codecs.String,"sub",1024,_=>{});throw new Exception("unexpected setup success"); }catch(RosException e)when(e.Result.Error==13) { }Need(m.Closes==1&&s.LastSetupCleanup.Length==1&&s.LastSetupCleanup[0].Result?.Error==0);s.CloseChecked();Need(m.Closes==1); }
    private static void EventExtent() { byte[] b=Envelope(1,1);BinaryPrimitives.WriteUInt64LittleEndian(b.AsSpan(56),1);Refuses(()=>RosEvent.Decode(b)); }
    private static void ScalarArrayCount() { var c=Codecs.ArrayOf(Codecs.Int64,2,8);Need(c.Decode(c.Encode(new[]{long.MinValue,long.MaxValue}))[1]==long.MaxValue);Refuses(()=>c.Decode(new byte[]{255,255,255,255})); }
    private static int Main() {
        Action[] cases={Utf8Whole,Utf8Reject,BoolWhole,TwistWhole,Int64Whole,ExactEventHeader,WholeObserverSequence,CapacityNoReplay,TimeoutNoReplay,MaximumOpaqueOwner,EveryCleanupAttempt,BusyCallbackPreflight,RevokeWholeAlias,FactoryRollback,EventExtent,ScalarArrayCount};
        foreach(Action c in cases)c();Console.WriteLine("local policy cases="+cases.Length+" native/ROS/PMU=false");return 0;
    }
}
