using System;
namespace Uwvm.RosWasm;

/// <summary>Typed factories over explicitly granted aliases, with checked rollback and guest-driven callbacks.</summary>
public sealed class Node {
    public Session Session { get; }
    public Node(Session session) { Session=session??throw new ArgumentNullException(nameof(session)); }
    public Publisher<T> CreatePublisher<T>(ICodec<T> codec,string alias) {
        if(codec==null)throw new ArgumentNullException(nameof(codec));Session.BeginSetup();Capability owner=Session.Open(alias);
        try { return new Publisher<T>(owner,codec); } catch { Session.Rollback(owner);throw; }
    }
    public Subscription<T> CreateSubscription<T>(ICodec<T> codec,string alias,int capacity,Action<T> callback) {
        if(codec==null||callback==null||capacity<0||capacity>Codecs.MaximumOutput)throw new ArgumentException("bounded subscription");
        Session.BeginSetup();Capability owner=Session.Open(alias);
        try { var subscription=new Subscription<T>(owner,codec,capacity,callback);owner.Invoke("topic_subscribe").Checked();return subscription; }
        catch { Session.Rollback(owner);throw; }
    }
    public ServiceClient<Q,S> CreateClient<Q,S>(ICodec<Q> request,ICodec<S> response,string alias,int capacity) {
        if(request==null||response==null||capacity<0||capacity>Codecs.MaximumOutput)throw new ArgumentException("bounded Service client");
        Session.BeginSetup();Capability owner=Session.Open(alias);
        try { return new ServiceClient<Q,S>(owner,request,response,capacity); }catch { Session.Rollback(owner);throw; }
    }
    public ServiceServer<Q,S> CreateService<Q,S>(ICodec<Q> request,ICodec<S> response,string advertiseAlias,string respondAlias,int capacity,Func<Q,S> callback) {
        if(request==null||response==null||callback==null||advertiseAlias==respondAlias||capacity<8||capacity>Codecs.MaximumOutput)throw new ArgumentException("separate bounded Service roles");
        Session.BeginSetup();Capability advertise=Session.Open(advertiseAlias);Capability? respond=null;
        try { respond=Session.Open(respondAlias);var server=new ServiceServer<Q,S>(advertise,respond,request,response,capacity,callback);advertise.Invoke("service_advertise").Checked();return server; }
        catch { if(respond!=null)Session.Rollback(respond);Session.Rollback(advertise);throw; }
    }
    public Observer CreateObserver(string alias,int capacity,int maximumWatches=64) {
        if(capacity<64||capacity>Codecs.MaximumOutput||maximumWatches<1||maximumWatches>65536)throw new ArgumentException("bounded observer");
        Session.BeginSetup();Capability owner=Session.Open(alias);
        try { return new Observer(owner,capacity,maximumWatches); }catch { Session.Rollback(owner);throw; }
    }
    /// <summary>All four client roles are separate. No cancel-any fallback or grant is synthesized.</summary>
    public ActionClient<G,R,F> CreateActionClient<G,R,F>(ICodec<G> goal,ICodec<R> result,ICodec<F> feedback,string sendAlias,string cancelOwnAlias,string feedbackAlias,string resultAlias,int maximumGoals=8) {
        if(goal==null||result==null||feedback==null||maximumGoals<1||maximumGoals>65536)throw new ArgumentException("Action codecs/bound");
        string[] aliases={sendAlias,cancelOwnAlias,feedbackAlias,resultAlias};
        for(int i=0;i<aliases.Length;i++) { if(aliases[i]==null)throw new ArgumentException("explicit Action aliases");for(int j=0;j<i;j++)if(aliases[i]==aliases[j])throw new ArgumentException("distinct Action roles"); }
        Session.BeginSetup();Capability? send=null,cancel=null,feedbackOwner=null,resultOwner=null;
        try { send=Session.Open(sendAlias);cancel=Session.Open(cancelOwnAlias);feedbackOwner=Session.Open(feedbackAlias);resultOwner=Session.Open(resultAlias);return new ActionClient<G,R,F>(send,cancel,feedbackOwner,resultOwner,goal,result,feedback,maximumGoals); }
        catch { if(resultOwner!=null)Session.Rollback(resultOwner);if(feedbackOwner!=null)Session.Rollback(feedbackOwner);if(cancel!=null)Session.Rollback(cancel);if(send!=null)Session.Rollback(send);throw; }
    }
    public ActionServer<G,R,F> CreateActionServer<G,R,F>(ICodec<G> goal,ICodec<R> result,ICodec<F> feedback,string advertiseAlias,string respondAlias,int capacity,Action<ulong,G,ActionServer<G,R,F>> callback,int maximumGoals=8) {
        if(goal==null||result==null||feedback==null||callback==null||advertiseAlias==respondAlias||capacity<8||capacity>Codecs.MaximumOutput||maximumGoals<1||maximumGoals>65536)throw new ArgumentException("separate bounded incoming Action roles");
        Session.BeginSetup();Capability advertise=Session.Open(advertiseAlias);Capability? respond=null;
        try { respond=Session.Open(respondAlias);var server=new ActionServer<G,R,F>(advertise,respond,goal,result,feedback,capacity,maximumGoals,callback);advertise.Invoke("action_advertise").Checked();return server; }
        catch { if(respond!=null)Session.Rollback(respond);Session.Rollback(advertise);throw; }
    }
    /// <summary>Raw parameter/TF/graph/lifecycle/time/logger imports still use their exact existing granted alias and full canonical bytes.</summary>
    public Capability OpenOperation(string alias)=>Session.Open(alias);
}
public sealed class Publisher<T> : IDisposable {
    public Capability Owner { get; } private readonly ICodec<T> codec;
    internal Publisher(Capability owner,ICodec<T> codec) { Owner=owner;this.codec=codec; }
    public Result Publish(T value)=>Owner.Invoke("topic_publish",Codecs.Wire(codec.Encode(value)));
    public void Dispose()=>Owner.Dispose();
}
public sealed class Subscription<T> : IPollable,IDisposable {
    public Capability Owner { get; } private readonly ICodec<T> codec;private readonly int capacity;private readonly Action<T> callback;
    internal Subscription(Capability owner,ICodec<T> codec,int capacity,Action<T> callback) { Owner=owner;this.codec=codec;this.capacity=capacity;this.callback=callback; }
    public Result PollOnce() {
        if(!Owner.Enter())return new Result(17);
        try { Result r=Owner.Invoke("event_poll",capacity:capacity);if(r.Error==0)callback(codec.Decode(r.Data));return r; }
        finally { Owner.Leave(); }
    }
    public void Dispose()=>Owner.Dispose();
}
public sealed class ServiceClient<Q,S> : IDisposable {
    public Capability Owner { get; } private readonly ICodec<Q> request;private readonly ICodec<S> response;private readonly int capacity;
    internal ServiceClient(Capability owner,ICodec<Q> request,ICodec<S> response,int capacity) { Owner=owner;this.request=request;this.response=response;this.capacity=capacity; }
    public S Call(Q value)=>response.Decode(Owner.Invoke("service_call",Codecs.Wire(request.Encode(value)),capacity).Checked().Data);
    /// <summary>Returned opaque ID acknowledges admission only. Poll the explicitly watched observer for completion; never resend90/110.</summary>
    public Result Submit(Q value,Observer observer) { if(observer==null)return new Result(22);return Owner.Invoke("service_submit",Codecs.Wire(request.Encode(value)),other:observer.Owner); }
    public void Dispose()=>Owner.Dispose();
}
public sealed class ServiceServer<Q,S> : IPollable {
    public Capability Advertise { get; } public Capability Respond { get; }
    private readonly ICodec<Q> request;private readonly ICodec<S> response;private readonly int capacity;private readonly Func<Q,S> callback;
    internal ServiceServer(Capability a,Capability r,ICodec<Q> request,ICodec<S> response,int capacity,Func<Q,S> callback) { Advertise=a;Respond=r;this.request=request;this.response=response;this.capacity=capacity;this.callback=callback; }
    public Result PollOnce() {
        if(!Advertise.Enter())return new Result(17);if(!Respond.Enter()) { Advertise.Leave();return new Result(17); }
        try {
            Result r=Advertise.Invoke("event_poll",capacity:capacity);if(r.Error!=0)return r;
            var reader=new Codecs.Reader(r.Data);ulong id=reader.Unsigned(8);if(id==0)throw new FormatException("zero Service request ID");
            Q value=request.Decode(reader.Take(reader.Remaining));reader.End();
            return Respond.Invoke("service_respond",Codecs.PrefixId(id,response.Encode(callback(value))));
        } finally { Respond.Leave();Advertise.Leave(); }
    }
}
/// <summary>One caller-driven bounded pass. Source changes/reentry during callbacks are refused with17.</summary>
public sealed class Executor {
    private readonly IPollable?[] sources;private int count,position;private bool spinning;
    public Executor(int maximumSources=64) { if(maximumSources<0||maximumSources>65536)throw new ArgumentException("source bound");sources=new IPollable?[maximumSources]; }
    public void Add(IPollable source) { if(source==null)throw new ArgumentNullException(nameof(source));if(spinning)throw new RosException(new Result(17));for(int i=0;i<count;i++)if(ReferenceEquals(sources[i],source))throw new RosException(new Result(17));if(count==sources.Length)throw new RosException(new Result(90));sources[count++]=source; }
    public void Remove(IPollable source) { if(spinning)throw new RosException(new Result(17));for(int i=0;i<count;i++)if(ReferenceEquals(sources[i],source)) { Array.Copy(sources,i+1,sources,i,count-i-1);sources[--count]=null;position%=Math.Max(1,count);return; } }
    public Result SpinOnce() { if(spinning)throw new RosException(new Result(17));spinning=true;try { for(int i=0;i<count;i++) { IPollable source=sources[position]!;position=(position+1)%count;Result r=source.PollOnce();if(r.Error!=11)return r; }return new Result(11); }finally { spinning=false; } }
}
