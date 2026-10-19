package org.uwvm.roswasm;

/** Reserved diagnostic storage for an open whose result could not be delivered.
 * A transport constructs this storage before any import. It records the primary
 * failure and the one close attempt without allocating after that attempt.
 * Suppressed exceptions are supplementary: disabling suppression, allocation
 * failure, or the same Throwable being raised again cannot replace the primary.
 * The transport resets this view before its next capability_open invocation;
 * callers that need longer retention must copy it before opening another alias.
 */
public final class OpenCleanup {
    private boolean attempted;
    private int code;
    private Throwable primary, failure;

    public boolean attempted() { return attempted; }
    /** Meaningful after attempted(), when failure() is null. */
    public int code() { return code; }
    public Throwable primary() { return primary; }
    public Throwable failure() { return failure; }

    void reset() { attempted=false; code=0; primary=null; failure=null; }
    void started(Throwable original) { primary=original; attempted=true; }
    void returned(int value) {
        code=value;
        if(value!=0) {
            try { suppress(new RosException(new Result(value))); }
            catch(RuntimeException|Error unavailable) { /* The numeric code remains available. */ }
        }
    }
    void failed(Throwable value) { failure=value; suppress(value); }
    private void suppress(Throwable value) {
        if(value==primary) return;
        try { primary.addSuppressed(value); }
        catch(RuntimeException|Error unavailable) { /* Reserved fields retain both outcomes. */ }
    }
}
