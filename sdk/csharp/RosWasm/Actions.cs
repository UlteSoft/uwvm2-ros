using System;
namespace Uwvm.RosWasm;

public readonly struct ActionCompletion<T> {
    public readonly ulong Id;public readonly byte Outcome;public readonly T Value;
    public ActionCompletion(ulong id,byte outcome,T value) { Id=id;Outcome=outcome;Value=value; }
}
/// <summary>Complete typed custom Action payloads over the real Runtime-owned ID/outcome envelope.</summary>
public sealed class ActionClient<G,R,F> {
    public Capability Send { get; } public Capability CancelOwn { get; } public Capability Feedback { get; } public Capability Result { get; }
    private readonly ICodec<G> goal;private readonly ICodec<R> result;private readonly ICodec<F> feedback;
    private readonly ulong[] goals;private readonly Capability[] owners;private int count;private bool dispatching;
    internal ActionClient(Capability send,Capability cancel,Capability feedbackOwner,Capability resultOwner,ICodec<G> g,ICodec<R> r,ICodec<F> f,int maximum) {
        if(send.Session!=cancel.Session||send.Session!=feedbackOwner.Session||send.Session!=resultOwner.Session)throw new ArgumentException("same-session Action roles");
        Send=send;CancelOwn=cancel;Feedback=feedbackOwner;Result=resultOwner;goal=g;result=r;feedback=f;goals=new ulong[maximum];owners=new[]{send,cancel,feedbackOwner,resultOwner};
    }
    private void EnterGroup() {
        if(dispatching)throw new RosException(new Uwvm.RosWasm.Result(17));
        foreach(Capability owner in owners)if(owner.Busy)throw new RosException(new Uwvm.RosWasm.Result(17));
        dispatching=true;foreach(Capability owner in owners)owner.Enter();
    }
    private void LeaveGroup() { foreach(Capability owner in owners)owner.Leave();dispatching=false; }
    private int Index(ulong id) { for(int i=0;i<count;i++)if(goals[i]==id)return i;return -1; }
    public bool Owns(ulong id)=>id!=0&&Index(id)>=0;
    public ulong SendGoal(G value) {
        if(count==goals.Length)throw new RosException(new Uwvm.RosWasm.Result(90));EnterGroup();
        try { byte[] bytes=Send.Invoke("action_send_goal",Codecs.Wire(goal.Encode(value)),8).Checked().Data;var rd=new Codecs.Reader(bytes);ulong id=rd.Unsigned(8);rd.End();if(id==0||Index(id)>=0)throw new FormatException("invalid/duplicate Action ID");goals[count++]=id;return id; }
        finally { LeaveGroup(); }
    }
    public Result Cancel(ulong id) { if(!Owns(id))return new Uwvm.RosWasm.Result(9);EnterGroup();try { return CancelOwn.Invoke("action_cancel_own_goal",Codecs.GoalId(id),1); }finally { LeaveGroup(); } }
    public F ReceiveFeedback(ulong id,int maximumPayload) {
        if(!Owns(id))throw new RosException(new Uwvm.RosWasm.Result(9));if(maximumPayload<0||maximumPayload>Codecs.MaximumOutput-8)throw new ArgumentException("feedback bound");
        EnterGroup();
        try { byte[] bytes=Feedback.Invoke("action_receive_feedback",Codecs.GoalId(id),maximumPayload+8).Checked().Data;var rd=new Codecs.Reader(bytes);if(rd.Unsigned(8)!=id)throw new FormatException("foreign feedback ID");F decoded=feedback.Decode(rd.Take(rd.Remaining));rd.End();return decoded; }
        finally { LeaveGroup(); }
    }
    public ActionCompletion<R> ReceiveResult(ulong id,int maximumPayload) {
        int index=Index(id);if(index<0)throw new RosException(new Uwvm.RosWasm.Result(9));if(maximumPayload<0||maximumPayload>Codecs.MaximumOutput-9)throw new ArgumentException("result bound");
        EnterGroup();
        try {
            byte[] bytes=Result.Invoke("action_receive_result",Codecs.GoalId(id),maximumPayload+9).Checked().Data;
            var rd=new Codecs.Reader(bytes);if(rd.Unsigned(8)!=id)throw new FormatException("foreign result ID");byte kind=(byte)rd.Unsigned(1);if(kind<2||kind>4)throw new FormatException("nonterminal Action outcome");
            R decoded=result.Decode(rd.Take(rd.Remaining));rd.End();var complete=new ActionCompletion<R>(id,kind,decoded);
            // Retire only AFTER matching full envelope and complete typed result. Timeout/idle/error keeps the ID.
            goals[index]=goals[--count];goals[count]=0;return complete;
        } finally { LeaveGroup(); }
    }
    public CleanupReport CloseChecked(Exception? primary=null) {
        if(dispatching||Send.Busy||CancelOwn.Busy||Feedback.Busy||Result.Busy)throw new RosException(new Uwvm.RosWasm.Result(17));
        var report=new CleanupReport(primary,4);Capability[] roles={Result,Feedback,CancelOwn,Send}; // Allocate all storage before close.
        foreach(Capability owner in roles) {
            var row=new CleanupItem{Alias=owner.Alias,Handle=owner.Handle};
            try { row.Result=owner.CloseChecked(); }catch(Exception e) { row.Exception=e; }
            row.Retirement=owner.LastRetirement;
            report.Items[report.Count++]=row;
        }
        Array.Clear(goals,0,count);count=0;return report;
    }
}

/// <summary>Incoming typed Action goals use separate advertise/respond rights and locally owned tickets.</summary>
public sealed class ActionServer<G,R,F> : IPollable {
    public Capability Advertise { get; } public Capability Respond { get; }
    private readonly ICodec<G> goal;private readonly ICodec<R> result;private readonly ICodec<F> feedback;
    private readonly Action<ulong,G,ActionServer<G,R,F>> callback;private readonly int capacity;
    private readonly ulong[] live;private int count;private bool dispatching;
    internal ActionServer(Capability advertise,Capability respond,ICodec<G> g,ICodec<R> r,ICodec<F> f,int capacity,int maximum,Action<ulong,G,ActionServer<G,R,F>> callback) {
        Advertise=advertise;Respond=respond;goal=g;result=r;feedback=f;this.capacity=capacity;live=new ulong[maximum];this.callback=callback;
    }
    private int Index(ulong id) { for(int i=0;i<count;i++)if(live[i]==id)return i;return -1; }
    public Result PollOnce() {
        if(dispatching||Advertise.Busy||Respond.Busy)return new Result(17);if(count==live.Length)return new Result(90);
        Advertise.Enter();Respond.Enter();dispatching=true;
        try {
            Result r=Advertise.Invoke("event_poll",capacity:capacity);if(r.Error!=0)return r;
            var rd=new Codecs.Reader(r.Data);ulong id=rd.Unsigned(8);if(id==0||Index(id)>=0)throw new FormatException("invalid incoming Action ID");
            G value=goal.Decode(rd.Take(rd.Remaining));rd.End();live[count++]=id;
            callback(id,value,this);return r;
        } finally { dispatching=false;Respond.Leave();Advertise.Leave(); }
    }
    /// <summary>May be called by the current guest callback; sends exactly one complete envelope.</summary>
    public Result RespondFeedback(ulong id,F value) { return Send(id,1,feedback.Encode(value)); }
    public Result RespondResult(ulong id,byte outcome,R value) { if(outcome<2||outcome>4)return new Result(22);return Send(id,outcome,result.Encode(value)); }
    private Result Send(ulong id,byte kind,byte[] value) {
        int index=Index(id);if(id==0||index<0)return new Result(9);if(value.Length>Codecs.MaximumInput-9)throw new ArgumentException("Action envelope bound");
        byte[] data=new Codecs.Writer().Unsigned(id,8).Unsigned(kind,1).Raw(value).Finish();
        Result r=Respond.Invoke("action_respond",data);if(r.Error==0&&kind>=2) { live[index]=live[--count];live[count]=0; }return r;
    }
    public CleanupReport CloseChecked(Exception? primary=null) {
        if(dispatching||Advertise.Busy||Respond.Busy)throw new RosException(new Result(17));
        var report=new CleanupReport(primary,2);Capability[] roles={Respond,Advertise};
        foreach(Capability owner in roles) { var item=new CleanupItem{Alias=owner.Alias,Handle=owner.Handle};try { item.Result=owner.CloseChecked(); }catch(Exception e) { item.Exception=e; }item.Retirement=owner.LastRetirement;report.Items[report.Count++]=item; }
        Array.Clear(live,0,count);count=0;return report;
    }
}
