package org.uwvm.roswasm;

/** Complete canonical value conversion; implementations must reject trailing bytes. */
public interface Codec<T> {
    byte[] encode(T value);
    T decode(byte[] value);
}
