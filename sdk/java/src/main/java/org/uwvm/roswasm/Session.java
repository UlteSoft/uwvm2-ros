package org.uwvm.roswasm;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Arrays;

/** Bounded guest-local resource lifetime. No construction creates native rights.
 * Explicit closeChecked retains every cleanup result and keeps setup/application
 * errors separate. close() reports cleanup failures through RosException and
 * Java try-with-resources retains them as suppressed exceptions on a primary.
 */
public final class Session implements AutoCloseable {
    final Transport transport;
    private final int maximum;
    private final List<Capability> owners=new ArrayList<>();
    private boolean active=true,retiring;
    // These allocations precede every open; close never allocates its input.
    private final byte[] retirementInput=new byte[0];
    private String revokingAlias;
    private final RetirementAttempt revokeAttempt=new RetirementAttempt(new RetirementAttempt.Commit() {
        public void run() {
            // Indexed iteration cannot allocate an Iterator after commit starts.
            for(int i=0;i<owners.size();++i){Capability owner=owners.get(i);
                if(owner.alias.equals(revokingAlias))owner.invalidate();}
        }
    });
    public RetirementAttempt lastRevokeAttempt(){return revokeAttempt;}
    /** Read-only views for owners retained after a known pre-import failure. */
    public synchronized List<RetirementAttempt> pendingRetirements(){
        List<RetirementAttempt> result=new ArrayList<>();
        for(Capability owner:owners)if(owner.handle()!=0&&owner.lastRetirement().notAttempted())result.add(owner.lastRetirement());
        return Collections.unmodifiableList(result);
    }
    Result invokeRetirement(RetirementAttempt attempt){
        try{
            Result result;
            if(transport instanceof RetirementTransport)
                result=((RetirementTransport)transport).retire(attempt.operation(),attempt.handle(),retirementInput,attempt);
            else {
                // There is no pre-import proof for arbitrary user transports.
                attempt.markAttempted("custom Transport.invoke (effect unknown)");
                result=transport.invoke(attempt.operation(),attempt.handle(),retirementInput,0);
            }
            if(!attempt.attempted())throw new IllegalStateException("retirement transport omitted its effect boundary");
            if(!attempt.returned())attempt.returned(result.error);
            return result;
        }catch(RuntimeException|Error failure){attempt.failed(failure);throw failure;}
    }
    synchronized Result closeOne(Capability owner){
        if(retiring||owner.busy())return owner.refuseRetirement();
        retiring=true;
        try{return owner.retire();}finally{retiring=false;}
    }
    private List<Result> lastCleanupResults=Collections.emptyList();
    private List<Throwable> lastCleanupFailures=Collections.emptyList();
    /** Last reserved cleanup slots; null result means inspect matching failure. */
    public List<Result> lastCleanupResults(){return lastCleanupResults;}
    public List<Throwable> lastCleanupFailures(){return lastCleanupFailures;}
    public Session(Transport transport,int maximumOwners){
        if(transport==null || (transport.backend()!=1&&transport.backend()!=2) || maximumOwners<0 || maximumOwners>65536)
            throw new IllegalArgumentException("explicit backend/owner bound");
        this.transport=transport;this.maximum=maximumOwners;
    }
    public void checkAbi(){
        Result r=transport.invoke("abi_version",0,new byte[0],0).checked();
        if(r.value==null || r.value!=1L)throw new RosException(new Result(95));
    }
    /** Actor-free factory preflight against the real existing owner table.
     * This reserves no rights or slots. A single-Guest-thread facade must still
     * exclude reentrant factories across its subsequent opens. Retired entries
     * continue to count exactly as open() counts them; no arena is enlarged.
     */
    public synchronized void preflightOwners(int count){
        if(count<0)throw new IllegalArgumentException("owner count");
        if(!active)throw new RosException(new Result(125));
        if(retiring)throw new RosException(new Result(17));
        if(count>maximum-owners.size())throw new RosException(new Result(90));
    }
    /** Open exactly one alias, committing a nonzero owner only on success. */
    public synchronized Capability open(String alias){
        if(!active)throw new RosException(new Result(125));
        if(retiring)throw new RosException(new Result(17));
        if(owners.size()>=maximum)throw new RosException(new Result(90));
        byte[] bytes=Codecs.utf8(alias);
        if(bytes.length==0||bytes.length>255||alias.indexOf('\0')>=0)throw new IllegalArgumentException("alias");
        Capability owner=new Capability(this,0,alias);owners.add(owner);
        try{
            Result result=transport.invoke("capability_open",0,bytes,0).checked();
            if(result.value==null||result.value==0)throw new IllegalStateException("zero/unwritten handle");
            owner.adopt(result.value);return owner;
        }catch(RuntimeException|Error error){owners.remove(owner);throw error;}
    }
    /** Stop scheduling locally; existing owners remain explicitly closeable. */
    public synchronized void shutdown(){active=false;}
    synchronized boolean active(){return active;}
    synchronized boolean retiring(){return retiring;}
    /** Close every live owner once in reverse order, despite earlier failures. */
    public synchronized List<Result> closeChecked(){return closeSelected(false);}
    /** Explicit recovery of KNOWN NotAttempted closes only. Never replays an
     * attempted import, timeout, unknown custom transport, or delivered close.
     * Scheduling stays stopped; this is not a fresh Session/context lifecycle. */
    public synchronized List<Result> retryNotAttemptedClosures(){return closeSelected(true);}
    private List<Result> closeSelected(boolean onlyNotAttempted){
        if(retiring || owners.stream().anyMatch(Capability::busy))return Collections.singletonList(new Result(17));
        List<Capability> live=new ArrayList<>(owners.size());
        for(Capability owner:owners)if(owner.handle()!=0&&(!onlyNotAttempted||owner.lastRetirement().notAttempted()))live.add(owner);
        Result[] results=new Result[live.size()];Throwable[] failures=new Throwable[live.size()];
        List<Result> resultView=Collections.unmodifiableList(Arrays.asList(results));
        List<Throwable> failureView=Collections.unmodifiableList(Arrays.asList(failures));
        lastCleanupResults=resultView;lastCleanupFailures=failureView;
        Throwable primary=null;
        retiring=true;
        try{
            for(int i=0;i<live.size();++i){
                Capability owner=live.get(live.size()-1-i);
                try{results[i]=owner.retire();}catch(RuntimeException|Error e){failures[i]=e;if(primary==null)primary=e;}
            }
            // Retain still-live IDs even when an upper resource is disposed or
            // removed after factory failure. Erase tombstones, not close chances.
            for(int i=owners.size()-1;i>=0;--i)if(owners.get(i).handle()==0)owners.remove(i);
            if(!onlyNotAttempted)active=false;
            if(primary!=null){
                for(int i=0;i<results.length;++i){
                    try{if(failures[i]!=null&&failures[i]!=primary)primary.addSuppressed(failures[i]);
                        if(results[i]!=null&&results[i].error!=0)primary.addSuppressed(new RosException(results[i]));}
                    catch(RuntimeException|Error unavailable){/* Reserved reports retain all outcomes even under OOM. */}
                }
                if(primary instanceof RuntimeException)throw (RuntimeException)primary;
                throw (Error)primary;
            }
            return resultView;
        }finally{retiring=false;}
    }
    public void close(){
        RosException primary=null;
        for(Result result:closeChecked())if(result.error!=0){if(primary==null)primary=new RosException(result);else primary.addSuppressed(new RosException(result));}
        if(primary!=null)throw primary;
    }
    /** Retire all local siblings only for the explicitly requested alias revoke. */
    synchronized Result revoke(Capability source){
        if(retiring || owners.stream().anyMatch(o->o.alias.equals(source.alias)&&o.busy()))return new Result(17);
        long handle=source.handle();if(handle==0)return new Result(9);
        revokeAttempt.begin("capability_revoke",handle);revokingAlias=source.alias;retiring=true;
        try{return invokeRetirement(revokeAttempt);}
        finally{revokingAlias=null;retiring=false;}
    }
}
