package org.uwvm.roswasm;
import java.util.Arrays;

/** Explicitly selected real optional transport. The contained ordinary ROS2
 * transport retains exactly its original business and retirement boundaries.
 * Construction performs no imports or capability opens. No retry/fallback or
 * reconnect policy is introduced. An errored native wait remains that error.
 */
public final class Ros2ServiceClientTransport implements ServiceClientTransport,RetirementTransport {
    private static final byte[] EMPTY=new byte[0];
    private final Ros2Transport business=new Ros2Transport();
    public int backend(){return 2;}
    public OpenCleanup lastOpenCleanup(){return business.lastOpenCleanup();}
    public Result invoke(String op,long handle,byte[] input,int capacity,long other,long correlation){return business.invoke(op,handle,input,capacity,other,correlation);}
    public Result retire(String op,long handle,byte[] empty,RetirementAttempt attempt){return business.retire(op,handle,empty,attempt);}
    public int serviceClientAbiVersion(){return Ros2ServiceClientRaw.abi_version();}
    public Result observeServiceClient(boolean features,long available,long source,long waitNs){
        if(available==0||source==0||available==source||waitNs<0||waitNs>ServiceClient.MAXIMUM_WAIT_NS||features&&waitNs!=0)
            throw new IllegalArgumentException("complete distinct pair and finite wait");
        LinearMemory.Frame frame=LinearMemory.enter(EMPTY,features?8:1);
        try {
            // All storage/strong roots and sentinel initialization precede the
            // final numeric address extraction. No allocation separates that
            // extraction from the actual numeric import. The frame remains
            // reachable until complete output has been copied and checked.
            Arrays.fill(frame.output,(byte)255);
            long output=frame.outputOffset();
            int code=features?Ros2ServiceClientRaw.features(available,source,0,output):
                Ros2ServiceClientRaw.service_available(available,source,waitNs,0,output);
            return frame.finish(code,LinearMemory.READY);
        } finally { LinearMemory.leave(frame); }
    }
}
