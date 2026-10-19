package org.uwvm.roswasm;
import org.teavm.interop.Import;
import org.teavm.interop.Unmanaged;

/** Actual OPTIONAL numeric imports for TeaVM0.13.1 WEBASSEMBLY_WASI only.
 * This class is deliberately separate from the original43 Ros2Raw functions.
 * Java long is the complete i64 bit pattern, memory is i32; output addresses are
 * zero-extended memory0 array offsets, never WasmGC object references or JNI.
 * A producer must independently admit all three exact catalog tuples before
 * this opt-in class can be linked. Missing imports are not emulated.
 */
final class Ros2ServiceClientRaw {
    private Ros2ServiceClientRaw() {}
    @Import(module="uwvm_ros2_client_v1",name="abi_version")
    @Unmanaged
    static native int abi_version();
    @Import(module="uwvm_ros2_client_v1",name="features")
    @Unmanaged
    static native int features(long availability,long source,int memory,long output);
    @Import(module="uwvm_ros2_client_v1",name="service_available")
    @Unmanaged
    static native int service_available(long availability,long source,long waitNs,int memory,long output);
}
