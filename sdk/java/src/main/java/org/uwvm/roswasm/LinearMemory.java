package org.uwvm.roswasm;

import java.util.Arrays;
import org.teavm.interop.Address;

/** TeaVM0.13.1 WEBASSEMBLY_WASI linear-heap buffer ownership.
 * This class is deliberately not usable as a WasmGC bridge. Address.ofData is
 * the actual array payload intrinsic; toLong zero-extends its wasm32 offset.
 * The static active frame keeps every array strongly reachable through the
 * complete synchronous import. Address.pin is a no-op on this backend and is
 * intentionally not used. No managed allocation occurs between numeric offset
 * extraction and the import. Reentrant calls are refused before any host effect.
 */
final class LinearMemory {
    static final int NONE = 0, LENGTH = 1, VALUE = 2, READY = 3;
    private static Frame active;
    private LinearMemory() {}

    static synchronized Frame enter(byte[] input, int capacity) {
        if (active != null) throw new IllegalStateException("reentrant ROS linear-memory call");
        if (input == null || input.length > 4193786 || capacity < 0 || capacity > 4194304)
            throw new IllegalArgumentException("bounded input/output storage required");
        Frame frame = new Frame(input, capacity);
        active = frame;
        return frame;
    }

    static synchronized void leave(Frame frame) {
        if (active != frame) throw new IllegalStateException("ROS frame ownership mismatch");
        active = null;
    }

    static final class Frame {
        final byte[] input, output, slot;
        Frame(byte[] input, int capacity) {
            this.input = input.clone();
            this.output = new byte[capacity];
            this.slot = new byte[8];
            Arrays.fill(slot, (byte)0xff);
        }
        long inputOffset() { return Address.ofData(input).toLong(); }
        long outputOffset() { return Address.ofData(output).toLong(); }
        long slotOffset() { return Address.ofData(slot).toLong(); }
        void clearValueSlot() { Arrays.fill(slot, (byte)0); }
        long slotValue() {
            long value = 0;
            for (int i = 0; i < 8; ++i) value |= ((long)slot[i] & 255L) << (8 * i);
            return value;
        }
        Result finish(int error, int kind) {
            long count = slotValue();
            if (error != 0) {
                return new Result(error, new byte[0], kind == LENGTH && error == 90 && count != -1L ? count : null, null);
            }
            if (kind == VALUE) {
                if (count == 0) throw new IllegalStateException("successful ROS slot was not written");
                return Result.value(count);
            }
            if (kind == LENGTH) {
                if (count == -1L || Long.compareUnsigned(count, output.length) > 0)
                    throw new IllegalStateException("invalid ROS output length");
                return new Result(0, Arrays.copyOf(output, (int)count), null, null);
            }
            return new Result(0, kind == READY ? output.clone() : new byte[0], null, null);
        }
    }
}
