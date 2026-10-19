package org.uwvm.roswasm;

/** Reserved, read-only cleanup evidence; a long retains every opaque i64 bit.
 * Allocated before the corresponding capability can be opened. A preparation
 * failure is retryable only while attempted() is false and the live owner is
 * still retained. An import trap, errno (including110/107), unknown custom
 * transport exception, or result-delivery failure is never evidence to replay.
 * This view is updated by an EXPLICIT retry; firstPreparationFailure remains
 * available and number() distinguishes attempts. It grants no new authority.
 */
public final class RetirementAttempt {
    interface Commit { void run(); }
    private final Commit commit;
    private String operation,boundary;
    private long handle,number;
    private boolean attempted,returned;
    private int code;
    private Throwable failure,firstPreparationFailure;
    RetirementAttempt(Commit commit) { this.commit=commit; }
    public String operation() { return operation; }
    public String boundary() { return boundary; }
    public long handle() { return handle; }
    public long number() { return number; }
    public boolean attempted() { return attempted; }
    public boolean returned() { return returned; }
    public int code() { return code; }
    public Throwable failure() { return failure; }
    public Throwable firstPreparationFailure() { return firstPreparationFailure; }
    public boolean notAttempted() { return operation!=null&&!attempted; }
    void begin(String value,long original) {
        operation=value;handle=original;++number;boundary=null;
        attempted=returned=false;code=0;failure=null;
    }
    // No allocation: the production caller executes this immediately before Raw.
    void markAttempted(String location) {
        attempted=true;boundary=location;commit.run();
    }
    void returned(int value) { returned=true;code=value; }
    void failed(Throwable value) {
        failure=value;
        if(!attempted&&firstPreparationFailure==null)firstPreparationFailure=value;
    }
}
