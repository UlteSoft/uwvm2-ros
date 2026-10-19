package org.uwvm.roswasm;

import org.teavm.interop.Address;

/** Explicit production selection. Ordinary81 and optional client3 retain their
 * existing transport; owned12 never enters generic string-operation dispatch.
 * Constructing this transport/Session creates no Host grant or native endpoint.
 */
public final class Ros2OwnedServiceTransport implements ServiceClientTransport, RetirementTransport {
    private final Ros2ServiceClientTransport business = new Ros2ServiceClientTransport();
    public int backend(){return 2;}
    public OpenCleanup lastOpenCleanup(){return business.lastOpenCleanup();}
    public Result invoke(String operation,long handle,byte[] input,int capacity,long other,long correlation){
        return business.invoke(operation,handle,input,capacity,other,correlation);
    }
    public Result retire(String operation,long handle,byte[] input,RetirementAttempt attempt){return business.retire(operation,handle,input,attempt);}
    public int serviceClientAbiVersion(){return business.serviceClientAbiVersion();}
    public Result observeServiceClient(boolean features,long availability,long source,long waitNs){return business.observeServiceClient(features,availability,source,waitNs);}

    // A strong root owns ALL input/output scratch through the synchronous raw
    // import. TeaVM0.13.1 linear WASI only: pin() is a no-op, so it is not used.
    // No managed allocation/callback lies between offset extraction and import.
    // The group refuses callback/factory/cleanup reentry with errno17 beforehand.
    private static OwnedService.Endpoint active;
    static int call(int operation,OwnedService.Endpoint e,long a,long b,int size){
        if(active!=null)return 17;
        active=e;
        try {
            switch(operation){
            case OwnedService.ABI:return Ros2OwnedServiceRaw.abiVersion();
            case OwnedService.ACTIVATE:
                if(e.role==3)return Ros2OwnedServiceRaw.clientActivate(e.source(),e.peer(),0,Address.ofData(e.original).toLong());
                return Ros2OwnedServiceRaw.serviceActivate(e.source(),e.peer(),0,Address.ofData(e.original).toLong());
            case OwnedService.INSPECT:return Ros2OwnedServiceRaw.inspect(e.source(),e.receipt.incarnation,0,Address.ofData(e.inspection).toLong());
            case OwnedService.SUBMIT:return Ros2OwnedServiceRaw.submit(e.source(),e.receipt.incarnation,0,Address.ofData(e.input).toLong(),size,Address.ofData(e.ticket).toLong());
            case OwnedService.RESPOND:return Ros2OwnedServiceRaw.respond(e.source(),e.receipt.incarnation,e.peer(),a,0,Address.ofData(e.input).toLong(),size);
            case OwnedService.ABANDON:return Ros2OwnedServiceRaw.abandon(e.source(),e.receipt.incarnation,a);
            case OwnedService.PEEK:return Ros2OwnedServiceRaw.peek(e.source(),e.receipt.incarnation,16,0,Address.ofData(e.packet).toLong(),e.packetCapacity);
            case OwnedService.COMMIT:return Ros2OwnedServiceRaw.commit(e.source(),e.receipt.incarnation,a,b);
            case OwnedService.CANCEL:return Ros2OwnedServiceRaw.cancel(e.source(),e.receipt.incarnation,a,b);
            case OwnedService.AVAILABLE:return Ros2OwnedServiceRaw.available(e.source(),e.receipt.incarnation,e.owners[2].handle(),a,0,Address.ofData(e.available).toLong());
            case OwnedService.RETIRE:return Ros2OwnedServiceRaw.retire(e.source(),e.receipt.incarnation);
            default:return 22;
            }
        } finally {active=null;}
    }
}
