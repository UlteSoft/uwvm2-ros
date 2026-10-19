package org.uwvm.roswasm;

import java.util.Arrays;
import org.teavm.interop.Address;

/** Complete12 binary/zero-handle denial Guest SOURCE. Opens NO owners. This is
 * separate from real Session/Capability positive apps, never native success.
 * Execute only under the owned Linux Guest runner and actual optional ABI.
 */
public final class Raw12ImportGuest {
    private static final byte[] receipt=new byte[256],ticket=new byte[8],packet=new byte[352],available=new byte[1],empty=new byte[0];
    static void denied(int code,byte[] output){if(code==0)throw new AssertionError("zero-handle import unexpectedly succeeded");for(byte b:output)if(b!=(byte)0xa5)throw new AssertionError("error modified output");System.out.println("denied="+Integer.toUnsignedString(code));}
    static void denied(int code){if(code==0)throw new AssertionError("zero-handle import unexpectedly succeeded");System.out.println("denied="+Integer.toUnsignedString(code));}
    public static void main(String[] args){
        if(args.length!=6)throw new IllegalArgumentException("six literal zero u64 values for unprivileged denial fixture");
        long s=Long.parseUnsignedLong(args[0]),p=Long.parseUnsignedLong(args[1]),i=Long.parseUnsignedLong(args[2]),t=Long.parseUnsignedLong(args[3]),q=Long.parseUnsignedLong(args[4]),n=Long.parseUnsignedLong(args[5]);
        if((s|p|i|t|q|n)!=0)throw new IllegalArgumentException("only zero handles are used by this denial fixture");
        Arrays.fill(receipt,(byte)0xa5);Arrays.fill(ticket,(byte)0xa5);Arrays.fill(packet,(byte)0xa5);Arrays.fill(available,(byte)0xa5);
        if(Ros2OwnedServiceRaw.abiVersion()!=1)throw new AssertionError("actual optional ABI version");
        denied(Ros2OwnedServiceRaw.clientActivate(s,p,0,Address.ofData(receipt).toLong()),receipt);
        denied(Ros2OwnedServiceRaw.serviceActivate(s,p,0,Address.ofData(receipt).toLong()),receipt);
        denied(Ros2OwnedServiceRaw.inspect(s,i,0,Address.ofData(receipt).toLong()),receipt);
        denied(Ros2OwnedServiceRaw.submit(s,i,0,Address.ofData(empty).toLong(),0,Address.ofData(ticket).toLong()),ticket);
        denied(Ros2OwnedServiceRaw.abandon(s,i,t));
        denied(Ros2OwnedServiceRaw.respond(s,i,p,t,0,Address.ofData(empty).toLong(),0));
        denied(Ros2OwnedServiceRaw.peek(s,i,16,0,Address.ofData(packet).toLong(),352),packet);
        denied(Ros2OwnedServiceRaw.commit(s,i,q,n));
        denied(Ros2OwnedServiceRaw.cancel(s,i,q,n));
        denied(Ros2OwnedServiceRaw.available(s,i,p,0,0,Address.ofData(available).toLong()),available);
        denied(Ros2OwnedServiceRaw.retire(s,i));
    }
}
