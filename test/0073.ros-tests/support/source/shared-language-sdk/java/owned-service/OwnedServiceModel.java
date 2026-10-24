package org.uwvm.roswasm;

import java.util.Arrays;

/** Local state/wire model only. No JVM run, Host registration, Native ownership
 * or ROS acceptance is claimed. Future execution belongs to Linux keeper.
 */
public final class OwnedServiceModel {
    static void check(boolean condition){if(!condition)throw new AssertionError();}
    static final long TICKET=0xfedcba9876543210L;
    static final String ID="UWVMROS1_"+"a".repeat(64);
    static OwnedService.Qos qos(){return new OwnedService.Qos(1,1,1,1,8,0,0,0,0,0,0,false);}
    static final class Business implements Transport {
        long next=0x8000000100000001L;int opens,closes;long[] order=new long[3];int closeError;
        public int backend(){return 2;}
        public Result invoke(String operation,long handle,byte[] input,int capacity,long other,long correlation){
            if(operation.equals("abi_version"))return Result.value(1);
            if(operation.equals("capability_open")){opens++;return Result.value(next++);}
            if(operation.equals("capability_close")){order[closes++]=handle;if(closeError!=0)return new Result(closeError);return new Result(0);}
            return new Result(95);
        }
    }
    static final class Native implements OwnedService.NativeCalls {
        int submissions,commits,cancels,retires,peeks,responses;long nonce,ticketValue=TICKET,sequence=1,nativeSequence=Long.MIN_VALUE+7;boolean unknownSubmit,unknownCommit,unknownRetire;boolean malformed;int body=1;
        public int call(int operation,OwnedService.Endpoint e,long a,long b,int size){
            switch(operation){
            case OwnedService.ABI:return 1;
            case OwnedService.ACTIVATE:receipt(e);return 0;
            case OwnedService.INSPECT:System.arraycopy(e.original,0,e.inspection,0,256);e.inspection[6]=2;return 0;
            case OwnedService.SUBMIT:submissions++;if(unknownSubmit)throw new IllegalStateException("lost submit result model");OwnedServiceWire.put(e.ticket,0,ticketValue,8);return 0;
            case OwnedService.ABANDON:return 0;
            case OwnedService.PEEK:peeks++;packet(e);return 0;
            case OwnedService.COMMIT:commits++;if(unknownCommit)throw new IllegalStateException("lost ACK model");return 0;
            case OwnedService.CANCEL:cancels++;return 0;
            case OwnedService.AVAILABLE:e.available[0]=1;return 0;
            case OwnedService.RESPOND:responses++;return 0;
            case OwnedService.RETIRE:retires++;if(unknownRetire)throw new OutOfMemoryError("unknown retire model");return 0;
            default:return 22;
            }
        }
        void receipt(OwnedService.Endpoint e){byte[] b=e.original;Arrays.fill(b,(byte)0);put(b,0,0x45524d55L,4);put(b,4,1,2);b[6]=1;b[7]=2;put(b,8,256,4);b[12]=(byte)e.role;b[13]=2;b[14]=(byte)(e.role==3?32:0);binding(b,0,e,0x8000000000000001L);
            for(int at:new int[]{64,144}){for(int i=0;i<4;i++)put(b,at+i*4,1,4);put(b,at+16,8,8);}}
        void packet(OwnedService.Endpoint e){byte[] b=e.packet;Arrays.fill(b,(byte)0);put(b,0,0x43524d55L,4);put(b,4,1,2);b[6]=5;b[7]=2;put(b,8,160,4);b[12]=(byte)e.role;b[13]=16;binding(b,0,e,e.receipt.incarnation);put(b,56,e.peer(),8);put(b,64,ticketValue,8);put(b,72,sequence,8);put(b,80,++nonce,8);put(b,96,e.packetCapacity,8);put(b,104,193,8);
            int at=160;put(b,at,0x494d5255L,4);put(b,at+4,1,2);b[at+6]=(byte)(e.role==3?3:2);b[at+7]=2;put(b,at+8,192,4);put(b,at+12,35,4);binding(b,at,e,e.receipt.incarnation);put(b,at+56,sequence,8);put(b,at+64,ticketValue,8);put(b,at+72,-3,8);put(b,at+80,0,8);put(b,at+88,-1,8);put(b,at+96,-1,8);put(b,at+104,nativeSequence,8);put(b,at+112,1,8);b[at+121]=16;b[352]=(byte)body;if(malformed)b[at+176]=1;}
        void binding(byte[] b,int at,OwnedService.Endpoint e,long incarnation){b[at+7]=2;put(b,at+16,e.source(),8);put(b,at+24,1,8);put(b,at+32,2,8);put(b,at+40,3,8);put(b,at+48,incarnation,8);}
        void put(byte[] b,int at,long value,int size){OwnedServiceWire.put(b,at,value,size);}
    }
    static final class Fixture {
        final Business business=new Business();final Native nativeCalls=new Native();final Session session=new Session(business,3);
        final OwnedService.Group group=new OwnedService.Group(session,new OwnedService.Options(1,4,1,1,0,8192),nativeCalls,true);
        OwnedService.Client<Boolean,Boolean> client;
        void open(Codec<Boolean> response){OwnedService.Definition<Boolean,Boolean>d=new OwnedService.Definition<>("rcl_interfaces/srv/Model",null,ID,Codecs.BOOL,1,response,1);OwnedService.Opening<OwnedService.Client<Boolean,Boolean>> o=group.client(d,"caller","observer","query",qos(),qos());check(o.code==0);client=o.value;}
    }
    static void fullWidthAndAbandon(){Fixture f=new Fixture();final int[] decoded={0};f.open(new Codec<Boolean>(){public byte[] encode(Boolean v){return Codecs.BOOL.encode(v);}public Boolean decode(byte[] b){decoded[0]++;check(f.group.closeChecked()==17);check(f.client.abandon(TICKET)==17);return Codecs.BOOL.decode(b);}});
        OwnedService.Submission s=f.client.submit(true);check(s.code==0&&s.ticket==TICKET);check(f.client.abandon(TICKET)==0&&f.client.abandon(TICKET)==9);
        OwnedService.Delivery<Boolean> d=f.client.takeResponse();check(d.code==125&&d.abandoned&&!d.hasValue&&decoded[0]==1&&d.ticket==TICKET&&d.sourceTimestamp==-3&&d.receivedTimestamp==0&&d.nativeSequence==Long.MIN_VALUE+7);
        OwnedService.Diagnostic snapshot=f.group.diagnostic(0);snapshot.operations[OwnedService.RETIRE].attempted=true;snapshot.lastReservation=-1L;snapshot.original[0]=0;
        f.session.shutdown();check(f.group.closeChecked()==0);check(f.nativeCalls.retires==1&&f.business.closes==3&&f.business.order[0]>f.business.order[1]);check(f.group.closeChecked()==0&&f.nativeCalls.retires==1);
    }
    static void decodeBeforeAck(){Fixture f=new Fixture();f.open(Codecs.BOOL);check(f.client.submit(true).code==0);f.nativeCalls.body=2;check(f.client.takeResponse().code==22&&f.nativeCalls.commits==0&&f.nativeCalls.cancels==1);f.nativeCalls.body=1;check(f.client.takeResponse().code==0&&f.nativeCalls.commits==1&&f.nativeCalls.nonce==2);f.group.closeChecked();}
    static void ambiguousAck(){Fixture f=new Fixture();f.open(Codecs.BOOL);check(f.client.submit(true).code==0);f.nativeCalls.unknownCommit=true;OwnedService.Delivery<Boolean>d=f.client.takeResponse();check(d.code==115&&!d.hasValue&&f.nativeCalls.commits==1&&f.nativeCalls.retires==1);check(f.client.takeResponse().code==125&&f.nativeCalls.commits==1);f.business.closeError=90;check(f.group.closeChecked()==90&&f.business.closes==3&&f.nativeCalls.retires==1);}
    static void ambiguousSubmit(){Fixture f=new Fixture();f.open(Codecs.BOOL);f.nativeCalls.unknownSubmit=true;check(f.client.submit(true).code==115&&f.nativeCalls.submissions==1&&f.nativeCalls.retires==1);check(f.client.submit(true).code==125&&f.nativeCalls.submissions==1);f.group.closeChecked();}
    static void malformedMetadata(){Fixture f=new Fixture();f.open(Codecs.BOOL);check(f.client.submit(true).code==0);f.nativeCalls.malformed=true;check(f.client.takeResponse().code!=0&&f.nativeCalls.commits==0&&f.nativeCalls.cancels==1&&f.nativeCalls.retires==1);f.nativeCalls.unknownRetire=true;f.group.closeChecked();check(f.business.closes==3);}
    static void onceCleanupUnknown(){Fixture f=new Fixture();f.open(Codecs.BOOL);f.nativeCalls.unknownRetire=true;check(f.group.closeChecked()==115&&f.business.closes==3&&f.nativeCalls.retires==1);OwnedService.Diagnostic d=f.group.diagnostic(0);check(d.operations[OwnedService.RETIRE].failure instanceof OutOfMemoryError);check(f.group.closeChecked()==115&&f.nativeCalls.retires==1);}
    static void realServerStateModel(){Business business=new Business();Native io=new Native();Session session=new Session(business,2);OwnedService.Group group=new OwnedService.Group(session,new OwnedService.Options(1,4,1,1,0,8192),io,true);
        OwnedService.Definition<Boolean,Boolean> definition=new OwnedService.Definition<>("rcl_interfaces/srv/Model",null,ID,Codecs.BOOL,1,Codecs.BOOL,1);
        OwnedService.Opening<OwnedService.Server<Boolean,Boolean>> opening=group.server(definition,"advertise","responder",qos(),qos());check(opening.code==0);OwnedService.Server<Boolean,Boolean> server=opening.value;
        OwnedService.Delivery<Boolean>d=server.takeRequest();check(d.code==0&&d.hasValue&&d.ticket==TICKET&&d.nativeSequence==Long.MIN_VALUE+7);check(server.respond(d.ticket,true)==0&&server.respond(d.ticket,true)==9&&io.responses==1);
        io.ticketValue=TICKET+1;io.sequence=2;check(server.takeRequest().code==13&&io.cancels==1&&io.commits==1&&io.retires==1);group.closeChecked();check(business.closes==2&&io.retires==1);
    }
    static void shutdownDuringDecoder(){Fixture f=new Fixture();f.open(new Codec<Boolean>(){public byte[] encode(Boolean v){return Codecs.BOOL.encode(v);}public Boolean decode(byte[] b){f.session.shutdown();return Codecs.BOOL.decode(b);}});check(f.client.submit(true).code==0);check(f.client.takeResponse().code==125&&f.nativeCalls.commits==0&&f.nativeCalls.cancels==1);check(f.group.closeChecked()==0&&f.nativeCalls.retires==1&&f.business.closes==3);}
    static void externallyClosedOwnerDoesNotBlockOthers(){Fixture f=new Fixture();f.open(Codecs.BOOL);check(f.group.entries[0].owners[1].closeChecked().error==0&&f.business.closes==1);check(f.group.closeChecked()==0&&f.business.closes==3&&f.nativeCalls.retires==1);}
    public static void main(String[] args){fullWidthAndAbandon();decodeBeforeAck();ambiguousAck();ambiguousSubmit();malformedMetadata();onceCleanupUnknown();realServerStateModel();shutdownDuringDecoder();externallyClosedOwnerDoesNotBlockOthers();}
}
