package org.uwvm.roswasm;

/** One opaque Instance/backend-owned lease, never transferable across sessions. */
public final class Capability implements AutoCloseable {
    final Session session;
    final String alias;
    private long handle;
    private boolean busy;
    private final RetirementAttempt retirement=new RetirementAttempt(new RetirementAttempt.Commit() {
        public void run() { handle=0; }
    });
    /** Reserved before Session.open; never narrows the original opaque handle. */
    public RetirementAttempt lastRetirement() { return retirement; }
    Capability(Session session,long handle,String alias){this.session=session;this.handle=handle;this.alias=alias;}
    public long handle(){return handle;}
    boolean busy(){return busy;}
    void invalidate(){handle=0;}
    void adopt(long value){handle=value;}
    boolean acquire(){if(busy)return false;busy=true;return true;}
    void release(){busy=false;}
    public Result invoke(String operation,byte[] request,int capacity){return invoke(operation,request,capacity,null,0);}
    public Result invoke(String operation,byte[] request,int capacity,Capability other,long correlation){
        if(handle==0)return new Result(9);
        if(!session.active())return new Result(125);
        if(other!=null&&(other.session!=session||other.handle==0))return new Result(9);
        // Managed lifecycle operations cannot bypass owner/count/retirement rules.
        // Keep original handle/active/foreign admission order before this route.
        if("capability_open".equals(operation))throw new IllegalArgumentException("use Session.open for a managed owner");
        if("capability_close".equals(operation)||"capability_revoke".equals(operation)){
            if(request==null||request.length!=0||capacity!=0||other!=null||correlation!=0)
                throw new IllegalArgumentException("canonical managed retirement requires empty input, capacity0, no other owner/correlation");
            return "capability_close".equals(operation)?closeChecked():revokeChecked();
        }
        return session.transport.invoke(operation,handle,request,capacity,other==null?0:other.handle,correlation);
    }
    /** Preparation precedes retirement; the real import boundary tombstones it.
     * Any effect-unknown/returned outcome remains retired and is never replayed. */
    Result retire(){long value=handle;if(value==0)return new Result(9);
        retirement.begin("capability_close",value);return session.invokeRetirement(retirement);}
    Result refuseRetirement(){if(handle!=0)retirement.begin("capability_close",handle);return new Result(17);}
    public Result closeChecked(){return session.closeOne(this);}
    public Result revokeChecked(){return session.revoke(this);}
    public void close(){closeChecked().checked();}
}
