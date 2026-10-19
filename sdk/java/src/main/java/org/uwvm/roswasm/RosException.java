package org.uwvm.roswasm;

/** Runtime refusal retaining errno, optional required length and owned bytes. */
public final class RosException extends RuntimeException {
    public final Result result;
    public RosException(Result result) {
        super("ROS ABI error " + result.error + "; required=" + result.required);
        this.result = result;
    }
}
