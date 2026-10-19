package org.uwvm.roswasm;

/** Numeric Wasm imports behind a copied-buffer guest interface.
 * Production instances are Ros1Transport or Ros2Transport. A test double proves
 * local lifetime logic only, never native ROS/Runtime acceptance. Calling this
 * interface cannot obtain authority beyond the separately granted host alias.
 */
public interface Transport {
    int backend();
    Result invoke(String operation, long handle, byte[] input, int capacity, long other, long correlation);
    /** Convenience for empty or ordinary single-owner requests. */
    default Result invoke(String operation, long handle, byte[] input, int capacity) {
        return invoke(operation, handle, input, capacity, 0, 0);
    }
}
