// LOCAL SOURCE MODELS ONLY. Never native/Guest/permission or performance evidence.
using System;
using System.Collections.Generic;
using Uwvm.RosWasm;
using Uwvm.RosWasm.Owned;
internal sealed class Model : ITransport {
    public int Backend=>2;internal int Opens,Submits,Commits,Cancels,Abandons,Retires,Responses;
    internal readonly List<ulong> Closes=new();internal readonly Queue<(byte Kind,ulong Ticket,byte[] Body)> Queue=new();
    internal bool RefuseSubmit,ThrowAfterSubmit,ThrowAfterCommit;internal uint CloseError;
    internal byte Role;internal ulong Ticket=0xf000000000000001UL;private byte[] endpoint=Array.Empty<byte>();
    public Result Invoke(string operation,ulong handle,byte[] input,int capacity,ulong other=0,ulong correlation=0) {
        if(operation=="abi_version")return new Result(0,value:1);
        if(operation=="capability_open")return new Result(0,value:(1UL<<32)+(ulong)++Opens);
        if(operation=="capability_close") { Closes.Add(handle);return new Result(CloseError); }
        throw new InvalidOperationException("unexpected local core operation");
    }
    private static void Put(byte[] b,int p,ulong value,int n) { for(int i=0;i<n;i++)b[p+i]=(byte)(value>>(8*i)); }
    private static void Binding(byte[] b,int p,ulong source) { b[p+7]=2;Put(b,p+16,source,8);Put(b,p+24,1,8);Put(b,p+32,1,8);Put(b,p+40,1,8);Put(b,p+48,1,8); }
    private void Receipt(OwnedCall c) {
        endpoint=new byte[256];Put(endpoint,0,0x45524d55,4);Put(endpoint,4,1,2);endpoint[6]=1;Binding(endpoint,0,c.Source);Put(endpoint,8,256,4);endpoint[12]=Role;endpoint[13]=2;
        for(int p=64;p<=144;p+=80) { Put(endpoint,p,1,4);Put(endpoint,p+4,1,4);Put(endpoint,p+8,2,4);Put(endpoint,p+12,1,4);Put(endpoint,p+16,1,8); }
        Buffer.BlockCopy(endpoint,0,c.Output,0,256);
    }
    private void Packet(OwnedCall c,byte kind,ulong ticket,byte[] body) {
        var b=c.Output;Array.Clear(b,0,b.Length);Put(b,0,0x43524d55,4);Put(b,4,1,2);b[6]=5;Binding(b,0,c.Source);Put(b,8,160,4);b[12]=Role;b[13]=16;
        Put(b,56,c.Peer,8);Put(b,64,ticket,8);Put(b,72,(ulong)Commits+1,8);Put(b,80,(ulong)Commits+Cancels+1,8);Put(b,96,(ulong)b.Length,8);Put(b,104,(ulong)(192+body.Length),8);
        int p=160;Put(b,p,0x494d5255,4);Put(b,p+4,1,2);b[p+6]=kind;Binding(b,p,c.Source);Put(b,p+8,192,4);Put(b,p+56,(ulong)Commits+1,8);Put(b,p+64,ticket,8);
        Put(b,p+88,ulong.MaxValue,8);Put(b,p+96,ulong.MaxValue,8);Put(b,p+112,(ulong)body.Length,8);
        if(kind==6||kind==7) { b[p+122]=(byte)(kind==7?1:0);Put(b,p+124,5,4); }
        else { Put(b,p+12,35,4);b[p+121]=16;Put(b,p+104,0x8000000000000001UL,8);Buffer.BlockCopy(body,0,b,352,body.Length); }
    }
    internal void Raw(OwnedCall c) {
        var e=c.Evidence;
        if(c.Operation==OwnedOperation.Submit&&RefuseSubmit) { e.Code=17;return; }
        e.Entered=true;
        switch(c.Operation) {
            case OwnedOperation.Version:case OwnedOperation.BusinessVersion:e.Code=1;e.Returned=true;return;
            case OwnedOperation.Open:Put(c.Output,0,(1UL<<32)+(ulong)++Opens,8);break;
            case OwnedOperation.ActivateClient:Role=3;Receipt(c);break;
            case OwnedOperation.ActivateService:Role=4;Receipt(c);break;
            case OwnedOperation.Inspect:Buffer.BlockCopy(endpoint,0,c.Output,0,256);c.Output[6]=2;break;
            case OwnedOperation.Submit:Submits++;Put(c.Output,0,Ticket,8);if(ThrowAfterSubmit)throw new InvalidOperationException("model effect then interop throw");break;
            case OwnedOperation.Abandon:Abandons++;break;
            case OwnedOperation.Respond:Responses++;break;
            case OwnedOperation.Peek:if(Queue.Count==0) { e.Code=11;e.Returned=true;return; }var r=Queue.Peek();Packet(c,r.Kind,r.Ticket,r.Body);break;
            case OwnedOperation.Commit:Commits++;Queue.Dequeue();if(ThrowAfterCommit)throw new InvalidOperationException("model consumed then lost ACK");break;
            case OwnedOperation.Cancel:Cancels++;break;
            case OwnedOperation.Available:c.Output[0]=0;break;
            case OwnedOperation.Retire:Retires++;break;
            default:throw new InvalidOperationException("unexpected raw model operation");
        }
        e.Code=0;e.Returned=true;
    }
}
internal sealed class OomCodec : ICodec<bool> {
    public byte[] Encode(bool value)=>Codecs.Bool.Encode(value);
    public bool Decode(byte[] value)=>throw new OutOfMemoryException("explicit local codec fault before ACK");
}
internal static class OwnedSourceFixtures {
    private static void Check(bool value,string why) { if(!value)throw new InvalidOperationException(why); }
    private static ServiceDefinition<bool,bool> Definition(ICodec<bool>? response=null)=>new("demo/srv/Toggle","UWVMROS1_"+new string('0',64),Codecs.Bool,response??Codecs.Bool,1,1);
    private static (OwnedGroup,Model,Client<bool,bool>) Client(ICodec<bool>? response=null) {
        var m=new Model();var g=OwnedGroup.PrepareLocalModel(1,4,1,1,m,m.Raw);var c=g.OpenClient(Definition(response),"explicit.call","explicit.observe");return(g,m,c);
    }
    private static void Throws(Action action) { try { action(); }catch(Exception) { return; }throw new InvalidOperationException("expected explicit fixture failure"); }
    private static int Main() {
        // Concrete class and errno do not decide whether an attempt happened.
        { var(g,m,c)=Client();m.RefuseSubmit=true;Throws(()=>c.Submit(true));Check(!c.Diagnostic.Submit.Entered&&m.Submits==0,"pre-entry busy has no dispatch");m.RefuseSubmit=false;var t=c.Submit(true);Check(t.Value>long.MaxValue,"full unsigned ticket");c.Close();Check(m.Retires==1&&m.Closes.Count==2,"explicit cleanup after preparation failure"); }
        { var(g,m,c)=Client();m.ThrowAfterSubmit=true;Throws(()=>c.Submit(true));Check(m.Submits==1&&c.Diagnostic.Submit.Entered&&!c.Diagnostic.Submit.Returned,"same exception after attempt unknown");Throws(()=>c.Submit(true));Check(m.Submits==1,"no implicit retry");g.CloseAll();g.CloseAll();Check(m.Retires==1&&m.Closes.Count==2,"unknown cleanup once"); }
        { var(g,m,c)=Client(new OomCodec());var t=c.Submit(true);m.Queue.Enqueue((3,t.Value,new byte[]{1}));Throws(()=>c.Poll());Check(m.Commits==0&&m.Cancels==1&&c.State(t)!=RequestState.Completed,"OOM decoded before ACK");g.CloseAll(); }
        { var(g,m,c)=Client();var t=c.Submit(true);m.Queue.Enqueue((3,t.Value,new byte[]{1}));m.ThrowAfterCommit=true;Throws(()=>c.Poll());Check(m.Commits==1&&c.State(t)==RequestState.Unknown,"unknown ACK not delivered");Throws(()=>c.Poll());Check(m.Commits==1,"ACK never replayed");g.CloseAll(); }
        { var(g,m,c)=Client();var t=c.Submit(true);c.Abandon(t);Throws(()=>c.Abandon(t));m.Queue.Enqueue((3,t.Value,new byte[]{1}));var d=c.Poll();Check(!d.Present&&c.State(t)==RequestState.Discarded&&m.Commits==1&&m.Abandons==1,"one abandoned late fulltake decoded ACK discarded");m.Queue.Enqueue((3,t.Value,new byte[]{1}));Throws(()=>c.Poll());Check(m.Commits==1,"duplicate late take refused");g.CloseAll(); }
        { var(g,m,c)=Client();m.CloseError=5;var d=c.Close(()=>c.Close());Check(d.CloseHookException is OwnedServiceException reentry&&reentry.Code==17,"close hook reentry before effect");Check(m.Retires==1&&m.Closes.Count==2&&m.Closes[0]>m.Closes[1],"reverse independent raw cleanup even after errors");Check(!g.CloseAll()&&m.Closes.Count==2,"all cleanup errors retained nonzero; entered owner never revived"); }
        { var(g,m,c)=Client();var t=c.Submit(true);m.Queue.Enqueue((7,0,Array.Empty<byte>()));var d=c.Poll();Check(d.Present&&d.IsControl&&m.Commits==1&&m.Retires==0,"committed terminal control precedes cleanup");m.CloseError=5;Check(!g.CloseAll()&&m.Retires==1&&m.Closes.Count==2,"terminal cleanup reports failures separately"); }
        Console.WriteLine("LOCAL_MODEL_SOURCE_CONTRACTS_ONLY; NOT ROS OR GUEST QUALIFICATION");return 0;
    }
}
