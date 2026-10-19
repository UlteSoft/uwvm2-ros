package org.uwvm.roswasm;

/** Borrowed two-owner adapter for the optional availability namespace.
 * The independent availability grant and ordinary call owner must be live in
 * the SAME Session/backend. Host authorization, native type/codec/QoS, current
 * Instance/generation/native session and configured source alias remain real
 * Host/Gateway obligations; these local checks do not grant or attest them.
 * Every import is synchronous and both real owners are leased until return.
 * No owner is closed or retained by this helper; the caller owns their lifetime.
 */
public final class ServiceClient {
    public static final long MAXIMUM_WAIT_NS=60000000000L;
    private ServiceClient() {}
    public static void checkAbi(Transport transport){
        if(!(transport instanceof ServiceClientTransport)||transport.backend()!=2)throw error(95);
        if(((ServiceClientTransport)transport).serviceClientAbiVersion()!=1)throw error(95);
    }
    private static RosException error(int code){return new RosException(new Result(code));}
    private static Result observe(Capability available,Capability source,boolean features,long waitNs){
        if(available==null||source==null||available.session!=source.session||available.handle()==0||source.handle()==0)throw error(9);
        Session session=source.session;
        if(!session.active())throw error(125);
        if(session.retiring())throw error(17);
        if(available.handle()==source.handle()||session.transport.backend()!=2||!(session.transport instanceof ServiceClientTransport))throw error(95);
        if(waitNs<0||waitNs>MAXIMUM_WAIT_NS)throw new IllegalArgumentException("finite native wait range");
        if(!available.acquire())throw error(17);
        try {
            if(!source.acquire())throw error(17);
            try {
                checkAbi(session.transport);
                Result result=((ServiceClientTransport)session.transport).observeServiceClient(features,available.handle(),source.handle(),waitNs);
                if(result==null)throw error(22);
                result.checked();
                if(result.data==null||result.data.length!=(features?8:1)||result.value!=null||result.required!=null)throw error(22);
                return result;
            } finally { source.release(); }
        } finally { available.release(); }
    }
    public static long features(Capability available,Capability source){
        byte[] bytes=observe(available,source,true,0).data;
        long value=0;for(int i=0;i<8;++i)value|=((long)bytes[i]&255L)<<(8*i);
        if(value!=0&&value!=7)throw error(22);
        return value;
    }
    public static boolean query(Capability available,Capability source,long waitNs){
        int value=observe(available,source,false,waitNs).data[0]&255;
        if(value>1)throw error(22);
        return value==1;
    }
}
