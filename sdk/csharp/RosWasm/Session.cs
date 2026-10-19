using System;
namespace Uwvm.RosWasm;

/// <summary>One Instance/backend-local bounded owner group; constructing a Session grants nothing.</summary>
public sealed partial class Session : IDisposable {
    internal readonly ITransport Transport;
    // Initialized with the Session, before it can own a capability.
    internal readonly byte[] RetirementEmpty=Array.Empty<byte>();
    private readonly Capability?[] owners;
    private readonly object gate=new();
    private int count;
    private ulong retirementSequence;
    internal bool Active { get; private set; }=true;
    internal bool Retiring { get; private set; }
    public CleanupReport? LastCleanup { get; private set; }
    // Setup rollback needs no new allocation after an import. Four Action roles are bounded by construction.
    private readonly CleanupItem[] setupCleanup=new CleanupItem[5];
    public ReadOnlySpan<CleanupItem> LastSetupCleanup => setupCleanup.AsSpan(0,setupCount);
    private int setupCount;
    public Session(ITransport transport,int maximumOwners=64) {
        if(transport==null||(transport.Backend!=1&&transport.Backend!=2)||maximumOwners<0||maximumOwners>65536)
            throw new ArgumentException("explicit backend and finite owner bound required");
        Transport=transport;owners=new Capability?[maximumOwners];
    }
    public void CheckAbi() { var r=Transport.Invoke("abi_version",0,Array.Empty<byte>(),0).Checked(); if(r.Value!=1)throw new RosException(new Result(95)); }
    public Capability Open(string alias) {
        if(alias==null||alias.Length==0||alias.Length>255)throw new ArgumentException("alias bound");byte[] raw=Codecs.Utf8(alias);if(raw.Length==0||raw.Length>255||alias.IndexOf('\0')>=0)throw new ArgumentException("alias UTF8 length1..255, no NUL");
        lock(gate) {
            if(!Active)throw new RosException(new Result(125));if(Retiring)throw new RosException(new Result(17));
            if(count==owners.Length)throw new RosException(new Result(90));
            // Allocate and retain pending storage BEFORE the native open. A successful return only writes fields.
            var owner=new Capability(this,alias);owners[count++]=owner;
            try { var r=Transport.Invoke("capability_open",0,raw,0).Checked();if(r.Value is not ulong id||id==0)throw new InvalidOperationException("invalid successful handle");owner.Adopt(id);return owner; }
            catch { owners[--count]=null;throw; }
        }
    }
    public void Shutdown() { lock(gate)Active=false; }
    public CleanupReport CloseChecked(Exception? primary=null) {
        lock(gate) {
            if(Retiring)throw new RosException(new Result(17));
            for(int i=0;i<count;i++)if(owners[i]!.Busy)throw new RosException(new Result(17));
            // All evidence storage and the report object are allocated before any retirement or Retiring transition.
            var report=new CleanupReport(primary,count);Retiring=true;
            try {
                for(int i=count-1;i>=0;i--) {
                    var owner=owners[i]!;if(owner.Handle==0)continue;
                    var item=new CleanupItem{Alias=owner.Alias,Handle=owner.Handle};
                    try { item.Result=owner.Retire(); } catch(Exception error) { item.Exception=error; }
                    item.Retirement=owner.LastRetirement;
                    report.Items[report.Count++]=item;
                }
                // A proven pre-call preparation failure still owns a live handle.
                // Keep it in the preallocated ledger; never retry inside this call.
                int retained=0,previous=count;
                for(int i=0;i<previous;i++)if(owners[i]!.Handle!=0)owners[retained++]=owners[i];
                Array.Clear(owners,retained,previous-retained);count=retained;
                Active=false;LastCleanup=report;return report;
            } finally { Retiring=false; }
        }
    }
    internal Result Revoke(Capability source) {
        lock(gate) {
            if(Retiring)return new Result(17);
            for(int i=0;i<count;i++)if(owners[i]!.Alias==source.Alias&&owners[i]!.Busy)return new Result(17);
            ulong id=source.Handle;if(id==0)return new Result(9);
            if(retirementSequence==ulong.MaxValue)throw new InvalidOperationException("retirement diagnostic sequence exhausted");
            ulong token=++retirementSequence;
            // Pre-store every original full-width sibling ID without allocation.
            for(int i=0;i<count;i++)if(owners[i]!.Alias==source.Alias&&owners[i]!.Handle!=0) {
                owners[i]!.BeginRetirement(RetirementOperation.Revoke);owners[i]!.RetirementTag=token;
            }
            NativeTransport.ScalarRetirementFrame? frame=null;Exception? primary=null;
            RetirementPhase phase=RetirementPhase.NotAttempted;RetirementBoundary boundary=RetirementBoundary.None;
            uint? code=null;
            // Freeze group mutation/reentry while the exact sibling set is in flight.
            Retiring=true;
            try {
                if(Transport is NativeTransport native) {
                    native.PrepareScalarRetirement(RetirementOperation.Revoke,id,out frame);
                    for(int i=0;i<count;i++)if(owners[i]!.RetirementTag==token)owners[i]!.Invalidate();
                    return frame!.Attempt();
                }
                // An arbitrary custom transport offers no proven native prepare
                // boundary. Entering Invoke conservatively means effect unknown.
                for(int i=0;i<count;i++)if(owners[i]!.RetirementTag==token)owners[i]!.Invalidate();
                phase=RetirementPhase.AttemptedEffectUnknown;boundary=RetirementBoundary.CustomTransportEntry;
                Result result=Transport.Invoke("capability_revoke",id,RetirementEmpty,0);
                code=result.Error;phase=RetirementPhase.Returned;return result;
            }catch(Exception error) { primary=error;throw; }
            finally {
                frame?.Release();
                if(Transport is NativeTransport) {
                    var diagnostic=NativeTransport.ScalarDiagnostic(id,RetirementOperation.Revoke,frame,primary);
                    phase=diagnostic.Phase;boundary=diagnostic.Boundary;code=diagnostic.ReturnedError;
                }
                for(int i=0;i<count;i++)if(owners[i]!.RetirementTag==token) {
                    var owner=owners[i]!;
                    // Only the trusted concrete native frame can establish that
                    // its interop call site was never entered. Unknown stays0.
                    if(phase==RetirementPhase.NotAttempted)owner.Adopt(owner.LastRetirement.OriginalHandle);
                    owner.FinishRetirement(phase,boundary,code,primary,frame?.CleanupException);owner.RetirementTag=0;
                }
                Retiring=false;
                if(primary==null&&frame?.CleanupException is Exception cleanup)throw cleanup;
            }
        }
    }
    internal void BeginSetup() { setupCount=0;Array.Clear(setupCleanup,0,setupCleanup.Length); }
#if UWVM_ROSWASM_RETIREMENT_FAULT_TESTS
    // Test-only managed IDs are not real authority and must not reach a Host.
    // These exercise actual NativeTransport preparation/diagnostics, not ITransport models.
    internal Capability SeedSyntheticRetirementOwner(string alias,ulong id) {
        if(Transport is not NativeTransport||!Active||Retiring||id==0||count==owners.Length)
            throw new InvalidOperationException("invalid private retirement fixture");
        var owner=new Capability(this,alias);owner.Adopt(id);owners[count++]=owner;return owner;
    }
#endif
    internal void Rollback(Capability owner) {
        var row=new CleanupItem{Alias=owner.Alias,Handle=owner.Handle};
        try { row.Result=owner.CloseChecked(); } catch(Exception error) { row.Exception=error; }
        row.Retirement=owner.LastRetirement;
        if(setupCount<setupCleanup.Length)setupCleanup[setupCount++]=row;
    }
    /// <summary>Dispose reports cleanup failure. Use CloseChecked(primary) to preserve an application primary separately.</summary>
    public void Dispose() { CloseChecked().ThrowIfFailed(); }
}
public sealed class Capability : IDisposable {
    internal Session Session { get; }
    public string Alias { get; }
    public ulong Handle { get; private set; }
    internal bool Busy { get; private set; }
    public RetirementDiagnostic LastRetirement { get; private set; }
    internal ulong RetirementTag;
    internal Capability(Session session,string alias) { Session=session;Alias=alias; }
    internal void Adopt(ulong id) { Handle=id; }
    internal void Invalidate() { Handle=0; }
    internal void BeginRetirement(RetirementOperation operation) {
        LastRetirement=new RetirementDiagnostic(Handle,operation,RetirementPhase.NotAttempted,
            RetirementBoundary.None,null,null,null);
    }
    internal void FinishRetirement(RetirementPhase phase,RetirementBoundary boundary,uint? error,Exception? primary,Exception? cleanup) {
        LastRetirement=new RetirementDiagnostic(LastRetirement.OriginalHandle,LastRetirement.Operation,
            phase,boundary,error,primary,cleanup);
    }
    internal bool Enter() { if(Busy)return false;Busy=true;return true; }
    internal void Leave() { Busy=false; }
    public Result Invoke(string operation,byte[]? input=null,int capacity=0,Capability? other=null,ulong correlation=0) {
        if(Handle==0)return new Result(9);if(!Session.Active)return new Result(125);if(Session.Retiring)return new Result(17);
        if(other!=null&&(other.Session!=Session||other.Handle==0))return new Result(9);
        if(operation=="capability_open")throw new ArgumentException("Use Session.Open to register a capability owner.",nameof(operation));
        // Managed generic dispatch must not bypass owner/group retirement.
        // Raw numeric ITransport.Invoke remains a separate lower-level API.
        if(operation=="capability_close"||operation=="capability_revoke") {
            if((input?.Length??0)!=0||capacity!=0||other!=null||correlation!=0)return new Result(22);
            return operation=="capability_close"?CloseChecked():RevokeChecked();
        }
        return Session.Transport.Invoke(operation,Handle,input??Array.Empty<byte>(),capacity,other?.Handle??0,correlation);
    }
    internal Result Retire() {
        ulong id=Handle;if(id==0)return new Result(9);BeginRetirement(RetirementOperation.Close);
        NativeTransport.ScalarRetirementFrame? frame=null;Exception? primary=null;
        RetirementPhase phase=RetirementPhase.NotAttempted;RetirementBoundary boundary=RetirementBoundary.None;uint? code=null;
        try {
            if(Session.Transport is NativeTransport native) {
                native.PrepareScalarRetirement(RetirementOperation.Close,id,out frame);
                Handle=0;return frame!.Attempt();
            }
            Handle=0;phase=RetirementPhase.AttemptedEffectUnknown;boundary=RetirementBoundary.CustomTransportEntry;
            Result result=Session.Transport.Invoke("capability_close",id,Session.RetirementEmpty,0);
            code=result.Error;phase=RetirementPhase.Returned;return result;
        }catch(Exception error) { primary=error;throw; }
        finally {
            frame?.Release();
            if(Session.Transport is NativeTransport) {
                var diagnostic=NativeTransport.ScalarDiagnostic(id,RetirementOperation.Close,frame,primary);
                phase=diagnostic.Phase;boundary=diagnostic.Boundary;code=diagnostic.ReturnedError;
            }
            if(phase==RetirementPhase.NotAttempted)Handle=id;
            FinishRetirement(phase,boundary,code,primary,frame?.CleanupException);
            if(primary==null&&frame?.CleanupException is Exception cleanup)throw cleanup;
        }
    }
    public Result CloseChecked() { return Busy||Session.Retiring?new Result(17):Retire(); }
    public Result RevokeChecked() { return Session.Revoke(this); }
    public void Dispose() { CloseChecked().Checked(); }
}
