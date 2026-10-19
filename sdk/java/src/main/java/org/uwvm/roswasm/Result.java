package org.uwvm.roswasm;

/** One ROS import result, without retries or loss of conditional output state.
 * Long carries the full i64 bit pattern. Negative Java values remain valid
 * unsigned handles/IDs. required is null when the host left its slot unwritten.
 * A timeout110 may leave effects admitted; it never implies permission to retry.
 */
public final class Result {
    public final int error;
    public final byte[] data;
    public final Long required;
    public final Long value;

    public Result(int error) { this(error, new byte[0], null, null); }
    public Result(int error, byte[] data, Long required, Long value) {
        this.error = error; this.data = data; this.required = required; this.value = value;
    }
    /** Preserve ABI version as a value rather than mistaking version1 for errno1. */
    public static Result value(long value) { return new Result(0, new byte[0], null, value); }
    /** Throw an exception that retains the entire original import outcome. */
    public Result checked() { if (error != 0) throw new RosException(this); return this; }
}
