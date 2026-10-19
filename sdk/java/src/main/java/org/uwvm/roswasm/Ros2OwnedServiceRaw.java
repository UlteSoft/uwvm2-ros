package org.uwvm.roswasm;

import org.teavm.interop.Import;
import org.teavm.interop.Unmanaged;

/** Optional backend2 only. No JVM/JNI implementation, fallback or implicit grant. */
final class Ros2OwnedServiceRaw {
    private Ros2OwnedServiceRaw() {}
    @Import(module="uwvm_ros2_rmw_srv_v1",name="abi_version") @Unmanaged static native int abiVersion();
    @Import(module="uwvm_ros2_rmw_srv_v1",name="client_activate") @Unmanaged static native int clientActivate(long source,long observer,int memory,long receipt);
    @Import(module="uwvm_ros2_rmw_srv_v1",name="service_activate") @Unmanaged static native int serviceActivate(long source,long responder,int memory,long receipt);
    @Import(module="uwvm_ros2_rmw_srv_v1",name="endpoint_inspect") @Unmanaged static native int inspect(long source,long incarnation,int memory,long receipt);
    @Import(module="uwvm_ros2_rmw_srv_v1",name="client_submit") @Unmanaged static native int submit(long source,long incarnation,int memory,long input,long size,long ticket);
    @Import(module="uwvm_ros2_rmw_srv_v1",name="client_abandon") @Unmanaged static native int abandon(long source,long incarnation,long ticket);
    @Import(module="uwvm_ros2_rmw_srv_v1",name="service_respond") @Unmanaged static native int respond(long source,long incarnation,long responder,long ticket,int memory,long input,long size);
    @Import(module="uwvm_ros2_rmw_srv_v1",name="endpoint_peek") @Unmanaged static native int peek(long source,long incarnation,int writerBytes,int memory,long packet,long capacity);
    @Import(module="uwvm_ros2_rmw_srv_v1",name="endpoint_commit") @Unmanaged static native int commit(long source,long incarnation,long sequence,long reservation);
    @Import(module="uwvm_ros2_rmw_srv_v1",name="endpoint_cancel") @Unmanaged static native int cancel(long source,long incarnation,long sequence,long reservation);
    @Import(module="uwvm_ros2_rmw_srv_v1",name="client_available") @Unmanaged static native int available(long source,long incarnation,long query,long waitNs,int memory,long output);
    @Import(module="uwvm_ros2_rmw_srv_v1",name="endpoint_retire") @Unmanaged static native int retire(long source,long incarnation);
}
