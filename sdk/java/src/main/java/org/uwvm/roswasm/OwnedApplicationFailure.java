package org.uwvm.roswasm;

/** Preallocated application failure envelope for one real owned Group/Session.
 * Construct BEFORE the first open. finish() independently observes group close,
 * Session shutdown and fixed snapshots; it never prints, adds suppression or
 * constructs an exception after an effect. The thrown object is this envelope,
 * not the original primary; primaryFailure()/getCause() retain that exact object.
 * Single Guest thread only. Fatal traps still require Runtime owner teardown.
 */
public final class OwnedApplicationFailure extends RuntimeException {
    private static final long serialVersionUID=1L;
    /** Read-only fixed snapshot of a real raw owner's final retirement facts. */
    public static final class OwnerEvidence {
        private boolean present,attempted,returned;
        private String alias,operation,boundary;
        private long beforeHandle,remainingHandle,retirementHandle,number;
        private int code;
        private Throwable failure,firstPreparationFailure;
        private OwnerEvidence(){}
        private void before(OwnedService.Endpoint endpoint,int slot){
            alias=endpoint.aliases[slot];Capability owner=endpoint.owners[slot];
            if(owner!=null){present=true;beforeHandle=owner.handle();}
        }
        private void after(OwnedService.Endpoint endpoint,int slot){
            Capability owner=endpoint.owners[slot];if(owner==null)return;
            present=true;remainingHandle=owner.handle();RetirementAttempt a=owner.lastRetirement();
            operation=a.operation();boundary=a.boundary();retirementHandle=a.handle();number=a.number();
            attempted=a.attempted();returned=a.returned();code=a.code();failure=a.failure();firstPreparationFailure=a.firstPreparationFailure();
        }
        public boolean present(){return present;}
        public String alias(){return alias;}
        public long beforeHandle(){return beforeHandle;}
        public long remainingHandle(){return remainingHandle;}
        public long retirementHandle(){return retirementHandle;}
        public long number(){return number;}
        public String operation(){return operation;}
        public String boundary(){return boundary;}
        public boolean attempted(){return attempted;}
        public boolean returned(){return returned;}
        public int code(){return code;}
        public Throwable failure(){return failure;}
        public Throwable firstPreparationFailure(){return firstPreparationFailure;}
    }
    private final Session session;
    private final OwnedService.Group group;
    private final OwnedService.Diagnostic[] diagnostics;
    private final OwnerEvidence[][] owners;
    private boolean finishing,finished,cleanupAttempted,cleanupReturned;
    private int cleanupCode,rows;
    private Throwable primary,cleanupFailure,shutdownFailure,beforeSnapshotFailure,afterSnapshotFailure,reportingFailure;

    public OwnedApplicationFailure(Session session,OwnedService.Group group){
        super("owned ROS2 application failed; inspect retained primary and cleanup evidence",null,false,false);
        if(session==null||group==null||group.session!=session||group.used!=0)
            throw new IllegalArgumentException("matching unopened real Group/Session required");
        this.session=session;this.group=group;
        diagnostics=new OwnedService.Diagnostic[group.options.endpoints];owners=new OwnerEvidence[diagnostics.length][3];
        for(int row=0;row<diagnostics.length;row++){
            diagnostics[row]=new OwnedService.Diagnostic();
            for(int slot=0;slot<3;slot++)owners[row][slot]=new OwnerEvidence();
        }
    }
    /** Preserve the exact caught object. No Throwable conversion or suppression. */
    public void recordPrimary(Throwable value){if(value!=null&&primary==null)primary=value;}
    /** Optional diagnostics cannot replace either primary or cleanup evidence. */
    public void recordReportingFailure(Throwable value){if(value!=null&&reportingFailure==null)reportingFailure=value;}
    public void finish(){
        if(finishing||finished)return;finishing=true;
        try{
            try{rows=group.used;for(int row=0;row<rows;row++)for(int slot=0;slot<3;slot++)owners[row][slot].before(group.entries[row],slot);}
            catch(Throwable failure){beforeSnapshotFailure=failure;}
            cleanupAttempted=true;
            try{cleanupCode=group.closeChecked();cleanupReturned=true;}catch(Throwable failure){cleanupFailure=failure;}
            try{session.shutdown();}catch(Throwable failure){shutdownFailure=failure;}
            try{
                rows=group.used;
                for(int row=0;row<rows;row++){
                    // The targets were reserved before the first open. Do not
                    // call Group.diagnostic(), which allocates a new snapshot.
                    group.entries[row].diagnostic.copyTo(diagnostics[row]);
                    for(int slot=0;slot<3;slot++)owners[row][slot].after(group.entries[row],slot);
                }
            }catch(Throwable failure){afterSnapshotFailure=failure;}
        }finally{finished=true;finishing=false;}
    }
    private static boolean bad(OwnedService.Outcome value){return value.code!=0||value.failure!=null||value.preparationFailure!=null||value.deliveryCleanupFailure!=null||value.deliveryCleanupCode!=0;}
    public boolean cleanupFailed(){
        if(!finished||!cleanupReturned||cleanupCode!=0||cleanupFailure!=null||shutdownFailure!=null||beforeSnapshotFailure!=null||afterSnapshotFailure!=null)return true;
        for(int row=0;row<rows;row++){
            OwnedService.Diagnostic d=diagnostics[row];if(bad(d.operations[OwnedService.RETIRE]))return true;
            for(int slot=0;slot<3;slot++)if(bad(d.close[slot])||bad(d.revoke[slot])||owners[row][slot].remainingHandle!=0)return true;
        }
        return false;
    }
    public boolean failed(){return primary!=null||reportingFailure!=null||cleanupFailed();}
    /** A successful body with failed cleanup also throws this preexisting object.
     * No null Throwable, per-error wrapper or suppression-list allocation occurs.
     */
    public void throwIfFailed(){if(failed())throw this;}
    public boolean finished(){return finished;}
    public boolean cleanupAttempted(){return cleanupAttempted;}
    public boolean cleanupReturned(){return cleanupReturned;}
    public int cleanupCode(){return cleanupCode;}
    public int endpointCount(){return rows;}
    public Throwable primaryFailure(){return primary;}
    public Throwable cleanupFailure(){return cleanupFailure;}
    public Throwable shutdownFailure(){return shutdownFailure;}
    public Throwable beforeSnapshotFailure(){return beforeSnapshotFailure;}
    public Throwable afterSnapshotFailure(){return afterSnapshotFailure;}
    public Throwable reportingFailure(){return reportingFailure;}
    @Override public synchronized Throwable getCause(){
        if(primary!=null)return primary;if(cleanupFailure!=null)return cleanupFailure;if(shutdownFailure!=null)return shutdownFailure;
        if(beforeSnapshotFailure!=null)return beforeSnapshotFailure;if(afterSnapshotFailure!=null)return afterSnapshotFailure;return reportingFailure;
    }
    /** Copy into a caller's already prepared target; its mutation cannot change
     * this envelope, Group lifecycle or saved cleanup outcomes. Valid use does
     * not allocate. Allocate a Diagnostic before effects when OOM is a concern.
     */
    public void copyDiagnostic(int row,OwnedService.Diagnostic target){
        if(!finished||row<0||row>=rows||target==null)throw new IllegalArgumentException("finished endpoint and reserved target required");
        diagnostics[row].copyTo(target);
    }
    public OwnerEvidence owner(int row,int slot){
        if(!finished||row<0||row>=rows||slot<0||slot>=3)throw new IllegalArgumentException("finished raw owner snapshot required");
        return owners[row][slot];
    }
}
