using System;
using Uwvm.RosWasm;

#if !UWVM_ROSWASM_RETIREMENT_FAULT_TESTS
#error Compile only the private instrumented native-retirement source, never a production SDK assembly.
#endif

// SOURCE ONLY: not executed. Synthetic IDs are explicitly not Host authority.
// They are used only in fault paths that stop before the actual SelectedRaw call.
// The final two cases require real grants and execute the unchanged real P/Invoke.
internal static class RetirementFaultCases {
    private const ulong High=0xFEDCBA9876543210UL;
    private static readonly OutOfMemoryException PreparationFailure=new("private preallocated prepare fault");
    private static readonly Exception CallFailure=new("private preallocated call-site fault");
    private static readonly Exception ReleaseFailure=new("private preallocated gate-release fault");
    private static int callSites,rawReturns,transportEntries;
    private static readonly Exception UnexpectedNativeEntry=new("managed open unexpectedly entered a raw native transport");
    private static void Need(bool value,string message) { if(!value)throw new Exception(message); }
    private static void BeforeFrame(NativeTransport.RetirementFaultPoint point) {
        if(point==NativeTransport.RetirementFaultPoint.BeforeFrameAllocation)throw PreparationFailure;
        if(point==NativeTransport.RetirementFaultPoint.AfterCallSiteEntered)callSites++;
    }
    private static void PrepareAndRelease(NativeTransport.RetirementFaultPoint point) {
        if(point==NativeTransport.RetirementFaultPoint.AfterGateAdmission)throw PreparationFailure;
        if(point==NativeTransport.RetirementFaultPoint.AfterGateReleased)throw ReleaseFailure;
        if(point==NativeTransport.RetirementFaultPoint.AfterCallSiteEntered)callSites++;
    }
    private static void CallAndRelease(NativeTransport.RetirementFaultPoint point) {
        if(point==NativeTransport.RetirementFaultPoint.AfterCallSiteEntered) { callSites++;throw CallFailure; }
        if(point==NativeTransport.RetirementFaultPoint.AfterRawReturned)rawReturns++;
        if(point==NativeTransport.RetirementFaultPoint.AfterGateReleased)throw ReleaseFailure;
    }
    private static void Returned(NativeTransport.RetirementFaultPoint point) {
        if(point==NativeTransport.RetirementFaultPoint.AfterCallSiteEntered)callSites++;
        if(point==NativeTransport.RetirementFaultPoint.AfterRawReturned) { rawReturns++;throw CallFailure; }
    }
    private static void RefuseNativeEntry(NativeTransport.RetirementFaultPoint point) {
        if(point==NativeTransport.RetirementFaultPoint.BeforeTransportInvoke) { transportEntries++;throw UnexpectedNativeEntry; }
        if(point==NativeTransport.RetirementFaultPoint.AfterCallSiteEntered) { callSites++;throw UnexpectedNativeEntry; }
    }
    private static void ExactFailure(Action operation,Exception expected) {
        try { operation(); }catch(Exception error) { Need(ReferenceEquals(error,expected),"primary exception was replaced");return; }
        throw new Exception("expected private failure did not occur");
    }
    private static void DiscardSynthetic(Session session,params Capability[] owners) {
        NativeTransport.RetirementFaultHook=null;
        // No real Host cap was opened. Do not send these fake IDs to a Host.
        foreach(var owner in owners)owner.Invalidate();
        session.CloseChecked().ThrowIfFailed();
    }
    private static void PrepareBeforeTombstone() {
        var s=new Session(new NativeTransport());var owner=s.SeedSyntheticRetirementOwner("synthetic",High);
        int sites=callSites;
        try {
            NativeTransport.RetirementFaultHook=BeforeFrame;
            ExactFailure(()=>owner.CloseChecked(),PreparationFailure);
            var d=owner.LastRetirement;
            Need(owner.Handle==High&&d.OriginalHandle==High&&d.Phase==RetirementPhase.NotAttempted&&
                 d.Boundary==RetirementBoundary.None&&ReferenceEquals(d.PrimaryException,PreparationFailure)&&
                 d.CleanupException==null&&callSites==sites,"preparation lost full ID or attempted native call");
        }finally { DiscardSynthetic(s,owner); }
    }
    private static void PrepareCleanupKeepsPrimary() {
        var s=new Session(new NativeTransport());var owner=s.SeedSyntheticRetirementOwner("synthetic",ulong.MaxValue);
        int sites=callSites;
        try {
            NativeTransport.RetirementFaultHook=PrepareAndRelease;
            ExactFailure(()=>owner.CloseChecked(),PreparationFailure);
            var d=owner.LastRetirement;
            Need(owner.Handle==ulong.MaxValue&&d.OriginalHandle==ulong.MaxValue&&
                 d.Phase==RetirementPhase.NotAttempted&&ReferenceEquals(d.PrimaryException,PreparationFailure)&&
                 ReferenceEquals(d.CleanupException,ReleaseFailure)&&callSites==sites,"gate cleanup obscured preparation failure");
        }finally { DiscardSynthetic(s,owner); }
    }
    private static void UnknownCloseNeverReplays() {
        var s=new Session(new NativeTransport());var owner=s.SeedSyntheticRetirementOwner("synthetic",High);
        int sites=callSites,returns=rawReturns;
        try {
            NativeTransport.RetirementFaultHook=CallAndRelease;
            ExactFailure(()=>owner.CloseChecked(),CallFailure);
            var d=owner.LastRetirement;
            Need(owner.Handle==0&&d.OriginalHandle==High&&d.Phase==RetirementPhase.AttemptedEffectUnknown&&
                 d.Boundary==RetirementBoundary.NativeInteropCallSite&&ReferenceEquals(d.PrimaryException,CallFailure)&&
                 ReferenceEquals(d.CleanupException,ReleaseFailure),"unknown close restored authority or lost diagnostics");
            Need(owner.CloseChecked().Error==9&&callSites==sites+1&&rawReturns==returns,"unknown close replayed");
        }finally { DiscardSynthetic(s,owner); }
    }
    private static void RevokePreparationPreservesSiblings() {
        var s=new Session(new NativeTransport());var a=s.SeedSyntheticRetirementOwner("same",High);
        var b=s.SeedSyntheticRetirementOwner("same",ulong.MaxValue);var other=s.SeedSyntheticRetirementOwner("other",0x8000000000000000UL);
        int sites=callSites;
        try {
            NativeTransport.RetirementFaultHook=BeforeFrame;
            ExactFailure(()=>a.RevokeChecked(),PreparationFailure);
            Need(a.Handle==High&&b.Handle==ulong.MaxValue&&other.Handle==0x8000000000000000UL&&callSites==sites,
                 "revoke preparation tombstoned siblings");
            Need(a.LastRetirement.OriginalHandle==High&&b.LastRetirement.OriginalHandle==ulong.MaxValue&&
                 a.LastRetirement.Phase==RetirementPhase.NotAttempted&&b.LastRetirement.Phase==RetirementPhase.NotAttempted,
                 "revoke did not retain both original IDs");
        }finally { DiscardSynthetic(s,a,b,other); }
    }
    private static void UnknownRevokeWholeAlias() {
        var s=new Session(new NativeTransport());var a=s.SeedSyntheticRetirementOwner("same",High);
        var b=s.SeedSyntheticRetirementOwner("same",ulong.MaxValue);var other=s.SeedSyntheticRetirementOwner("other",0x8000000000000000UL);
        int sites=callSites,returns=rawReturns;
        try {
            NativeTransport.RetirementFaultHook=CallAndRelease;
            ExactFailure(()=>a.RevokeChecked(),CallFailure);
            Need(a.Handle==0&&b.Handle==0&&other.Handle==0x8000000000000000UL&&
                 a.LastRetirement.OriginalHandle==High&&b.LastRetirement.OriginalHandle==ulong.MaxValue&&
                 a.LastRetirement.Phase==RetirementPhase.AttemptedEffectUnknown&&b.LastRetirement.Phase==RetirementPhase.AttemptedEffectUnknown,
                 "unknown revoke did not tombstone the complete original sibling group");
            Need(a.RevokeChecked().Error==9&&callSites==sites+1&&rawReturns==returns,"unknown revoke replayed");
        }finally { DiscardSynthetic(s,a,b,other); }
    }
    private static void SessionKeepsUnattemptedOwners() {
        var s=new Session(new NativeTransport());var a=s.SeedSyntheticRetirementOwner("a",High);
        var b=s.SeedSyntheticRetirementOwner("b",ulong.MaxValue);
        int sites=callSites;
        try {
            NativeTransport.RetirementFaultHook=BeforeFrame;
            CleanupReport first=s.CloseChecked();
            Need(first.Count==2&&!first.Succeeded&&a.Handle==High&&b.Handle==ulong.MaxValue,"cleanup dropped unattempted owner");
            CleanupReport second=s.CloseChecked(); // Explicit retry only; first made no interop attempt.
            Need(second.Count==2&&callSites==sites,"ledger was cleared or native call was replayed");
            foreach(var item in first.Items)Need(item.Retirement?.Phase==RetirementPhase.NotAttempted&&
                item.Retirement?.OriginalHandle==item.Handle&&ReferenceEquals(item.Exception,PreparationFailure),"cleanup diagnostic incomplete");
        }finally { DiscardSynthetic(s,a,b); }
    }
    private static void ManagedInvokeCloseCannotBypass() {
        var s=new Session(new NativeTransport());var owner=s.SeedSyntheticRetirementOwner("synthetic",High);
        int sites=callSites,returns=rawReturns;
        try {
            NativeTransport.RetirementFaultHook=CallAndRelease;
            ExactFailure(()=>owner.Invoke("capability_close"),CallFailure);
            Need(owner.Handle==0&&owner.LastRetirement.OriginalHandle==High&&
                 owner.LastRetirement.Phase==RetirementPhase.AttemptedEffectUnknown,"managed Invoke close bypassed owner state");
            Need(owner.Invoke("capability_close").Error==9&&owner.CloseChecked().Error==9&&
                 callSites==sites+1&&rawReturns==returns,"managed Invoke close replayed uncertain retirement");
        }finally { DiscardSynthetic(s,owner); }
    }
    private static void ManagedInvokeRevokeCannotBypass() {
        var s=new Session(new NativeTransport());var a=s.SeedSyntheticRetirementOwner("same",High);
        var b=s.SeedSyntheticRetirementOwner("same",ulong.MaxValue);var other=s.SeedSyntheticRetirementOwner("other",0x8000000000000000UL);
        int sites=callSites,returns=rawReturns;
        try {
            NativeTransport.RetirementFaultHook=CallAndRelease;
            ExactFailure(()=>a.Invoke("capability_revoke"),CallFailure);
            Need(a.Handle==0&&b.Handle==0&&other.Handle==0x8000000000000000UL&&
                 a.LastRetirement.OriginalHandle==High&&b.LastRetirement.OriginalHandle==ulong.MaxValue&&
                 a.LastRetirement.Phase==RetirementPhase.AttemptedEffectUnknown&&b.LastRetirement.Phase==RetirementPhase.AttemptedEffectUnknown,
                 "managed Invoke revoke bypassed sibling state");
            Need(a.Invoke("capability_revoke").Error==9&&a.RevokeChecked().Error==9&&
                 callSites==sites+1&&rawReturns==returns,"managed Invoke revoke replayed uncertain retirement");
        }finally { DiscardSynthetic(s,a,b,other); }
    }
    private static void ManagedRetirementShapeAndBusy() {
        var s=new Session(new NativeTransport());var a=s.SeedSyntheticRetirementOwner("same",High);
        var b=s.SeedSyntheticRetirementOwner("same",ulong.MaxValue);int sites=callSites;
        try {
            NativeTransport.RetirementFaultHook=BeforeFrame;
            Need(a.Invoke("capability_close",new byte[]{1}).Error==22&&
                 a.Invoke("capability_revoke",capacity:1).Error==22&&
                 a.Invoke("capability_close",other:b).Error==22&&
                 a.Invoke("capability_revoke",correlation:1).Error==22,"retirement accepted nonempty shape");
            a.Enter();
            try { Need(a.Invoke("capability_close").Error==17&&b.Invoke("capability_revoke").Error==17,"generic retirement bypassed busy owner/group"); }
            finally { a.Leave(); }
            s.Shutdown();Need(a.Invoke("capability_close").Error==125,"generic retirement bypassed inactive Session");
            Need(a.Handle==High&&b.Handle==ulong.MaxValue&&callSites==sites,"refused generic retirement changed ownership/called native");
        }finally { DiscardSynthetic(s,a,b); }
    }
    private static void ManagedOpenMustUseSessionLedger() {
        var s=new Session(new NativeTransport(),2);var a=s.SeedSyntheticRetirementOwner("existing",High);Capability? b=null;
        int entries=transportEntries,sites=callSites;
        try {
            NativeTransport.RetirementFaultHook=RefuseNativeEntry;
            bool refused=false;
            try { a.Invoke("capability_open",Codecs.Utf8("must-use-session")); }
            catch(ArgumentException error) { refused=error.ParamName=="operation"&&error.Message.Contains("Session.Open"); }
            Need(refused&&a.Handle==High&&transportEntries==entries&&callSites==sites,"managed open did not fail before native entry");
            // Explicit synthetic seed uses the same real finite owner ledger,
            // proving the refused generic operation did not consume its last slot.
            b=s.SeedSyntheticRetirementOwner("remaining-slot",ulong.MaxValue);
            bool full=false;
            try { s.Open("real-api-limit-check"); }
            catch(RosException error) { full=error.Result.Error==90; }
            Need(full&&b.Handle==ulong.MaxValue&&transportEntries==entries&&callSites==sites,
                 "Session.Open bypassed its original quota or reached a Host");
        }finally { NativeTransport.RetirementFaultHook=null;a.Invalidate();b?.Invalidate();s.CloseChecked().ThrowIfFailed(); }
    }
    private static void ActualReturnedClose(string alias) {
        var s=new Session(new NativeTransport());Capability? owner=null;
        try {
            owner=s.Open(alias);ulong original=owner.Handle;int sites=callSites,returns=rawReturns;
            NativeTransport.RetirementFaultHook=Returned;
            ExactFailure(()=>owner.CloseChecked(),CallFailure);
            var d=owner.LastRetirement;
            Need(owner.Handle==0&&d.OriginalHandle==original&&d.Phase==RetirementPhase.Returned&&d.ReturnedError==0&&
                 callSites==sites+1&&rawReturns==returns+1,"actual close did not return expected errno0");
            Need(owner.CloseChecked().Error==9&&callSites==sites+1,"returned close replayed");
        }finally { NativeTransport.RetirementFaultHook=null;s.CloseChecked().ThrowIfFailed(); }
    }
    private static void ActualReturnedRevoke(string alias) {
        var s=new Session(new NativeTransport());
        try {
            Capability a=s.Open(alias),b=s.Open(alias);ulong ai=a.Handle,bi=b.Handle;
            int sites=callSites,returns=rawReturns;
            NativeTransport.RetirementFaultHook=Returned;
            ExactFailure(()=>a.RevokeChecked(),CallFailure);
            Need(a.Handle==0&&b.Handle==0&&a.LastRetirement.OriginalHandle==ai&&b.LastRetirement.OriginalHandle==bi&&
                 a.LastRetirement.Phase==RetirementPhase.Returned&&b.LastRetirement.Phase==RetirementPhase.Returned&&
                 a.LastRetirement.ReturnedError==0&&callSites==sites+1&&rawReturns==returns+1,"actual revoke did not retire exact siblings");
            Need(a.RevokeChecked().Error==9&&callSites==sites+1,"returned revoke replayed");
        }finally { NativeTransport.RetirementFaultHook=null;s.CloseChecked().ThrowIfFailed(); }
    }
    private static int Main(string[] args) {
        if(args.Length!=2||args[0]==args[1]) { Console.Error.WriteLine("two separate real granted aliases required: close-alias revoke-alias");return 2; }
        try {
            PrepareBeforeTombstone();PrepareCleanupKeepsPrimary();UnknownCloseNeverReplays();
            RevokePreparationPreservesSiblings();UnknownRevokeWholeAlias();SessionKeepsUnattemptedOwners();
            ManagedInvokeCloseCannotBypass();ManagedInvokeRevokeCannotBypass();ManagedRetirementShapeAndBusy();
            ManagedOpenMustUseSessionLedger();
            ActualReturnedClose(args[0]);ActualReturnedRevoke(args[1]);
            Console.WriteLine("retirement private fault cases=12; synthetic=10 actual-PInvoke=2; native-ROS-delivery=false");return 0;
        }catch(Exception error) { Console.Error.WriteLine(error);return 1; }
        finally { NativeTransport.RetirementFaultHook=null; }
    }
}
