package org.uwvm.roswasm;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

/** JVM-only wire/lifetime regressions. No Runtime/native ROS case is claimed.
 * Compile/run a /tmp copy with a bounded Java heap and one javac process.
 */
public final class ContractTest {
    private static int assertions;
    private static void check(boolean value){++assertions;if(!value)throw new AssertionError("contract " + assertions);}
    private static void rejects(Runnable operation){try{operation.run();}catch(IllegalArgumentException error){++assertions;return;}throw new AssertionError("expected refusal");}
    private static final class LocalTransport implements Transport {
        final List<String> calls=new ArrayList<>();final List<Long> closed=new ArrayList<>();
        long next=1;Result poll=new Result(11);int closeCode,openCode,subscribeCode;
        public int backend(){return 2;}
        public Result invoke(String operation,long handle,byte[] data,int capacity,long other,long correlation){
            calls.add(operation);
            switch(operation){
            case "abi_version":return Result.value(1);
            case "capability_open":return openCode==0?Result.value(next++):new Result(openCode);
            case "capability_close":closed.add(handle);return new Result(closeCode);
            case "topic_subscribe":return new Result(subscribeCode);
            case "event_poll":return poll;
            default:return new Result(0);
            }
        }
    }
    public static void main(String[] args){
        for(String text:new String[]{"","a\0b","\ufeffrobot","机器人🙂"})check(Codecs.STRING.decode(Codecs.STRING.encode(text)).equals(text));
        for(byte[] wire:new byte[][]{{},{1,0,0,0},{1,0,0,0,(byte)0xff},{0,0,0,0,1},{3,0,0,0,(byte)0xed,(byte)0xa0,(byte)0x80}})rejects(()->Codecs.STRING.decode(wire));
        rejects(()->Codecs.STRING.encode("\ud800"));
        check(Codecs.BOOL.decode(new byte[]{1}));check(!Codecs.BOOL.decode(new byte[]{0}));
        rejects(()->Codecs.BOOL.decode(new byte[]{2}));rejects(()->Codecs.BOOL.decode(new byte[]{1,0}));
        double[] twist={0.0,-0.0,2.5,-3.0,1e-20,1e20};
        check(Arrays.equals(Codecs.TWIST.decode(Codecs.TWIST.encode(twist)),twist));
        rejects(()->Codecs.TWIST.encode(new double[]{Double.NaN,0,0,0,0,0}));
        rejects(()->Codecs.TWIST.decode(new byte[47]));
        for(long value:new long[]{0,1,Long.MIN_VALUE,Long.MAX_VALUE,-1L})check(Codecs.INT64.decode(Codecs.INT64.encode(value))==value);
        rejects(()->Codecs.INT64.decode(new byte[9]));
        Codecs.Writer writer=new Codecs.Writer(32);
        byte[] mixed=writer.integer(255,1,false).integer(-32768,2,true).integer(0xffffffffL,4,false).integer(-1L,8,false).f32(-0.0f).f64(1.25).finish();
        Codecs.Reader reader=new Codecs.Reader(mixed);
        check(reader.integer(1,false)==255);check(reader.integer(2,true)==-32768);check(reader.integer(4,false)==0xffffffffL);check(reader.integer(8,false)==-1L);
        check(Float.floatToRawIntBits(reader.f32())==Float.floatToRawIntBits(-0.0f));check(reader.f64()==1.25);reader.end();
        rejects(()->new Codecs.Writer(1).integer(256,1,false));rejects(()->new Codecs.Writer(1).integer(-129,1,true));
        rejects(()->new Codecs.Writer(4).text("x"));rejects(()->new Codecs.Writer(4).f32(Float.POSITIVE_INFINITY));
        Codecs.Response response=new Codecs.Response(true,"成功\0");
        Codecs.Response decoded=Codecs.SERVICE_RESPONSE.decode(Codecs.SERVICE_RESPONSE.encode(response));
        check(decoded.success&&decoded.message.equals(response.message));
        rejects(()->Codecs.prefixId(0,new byte[0]));
        check(Codecs.INT64.decode(Arrays.copyOf(Codecs.prefixId(-1L,new byte[]{9}),8))==-1L);

        LocalTransport transport=new LocalTransport();Session session=new Session(transport,16);session.checkAbi();
        transport.openCode=13;
        try{session.open("denied");throw new AssertionError();}catch(RosException error){check(error.result.error==13);}
        check(transport.closed.isEmpty());transport.openCode=0;transport.next=-1L;
        Capability high=session.open("high");check(high.handle()==-1L);
        transport.closeCode=110;check(high.closeChecked().error==110);check(high.closeChecked().error==9);check(transport.closed.size()==1);
        transport.next=11;transport.closeCode=0;
        Capability first=session.open("first"),second=session.open("second");
        Node node=new Node(session);final boolean[] delivered={false};
        Node.Subscription<String> sub=node.createSubscription(Codecs.STRING,"sub",1024,value->{
            check(value.equals("callback"));check(session.closeChecked().get(0).error==17);delivered[0]=true;
        });
        transport.poll=new Result(0,Codecs.STRING.encode("callback"),null,null);sub.pollOnce().checked();check(delivered[0]);
        check(first.handle()!=0&&second.handle()!=0);
        transport.closeCode=107;List<Result> cleanup=session.closeChecked();
        check(cleanup.size()==3);check(cleanup.stream().allMatch(r->r.error==107));
        check(transport.closed.equals(Arrays.asList(-1L,13L,12L,11L)));
        check(first.handle()==0&&second.handle()==0&&sub.owner.handle()==0);

        LocalTransport failed=new LocalTransport();failed.subscribeCode=13;failed.closeCode=107;
        Session failedSession=new Session(failed,8);
        try{new Node(failedSession).createSubscription(Codecs.STRING,"sub",64,value->{});throw new AssertionError();}
        catch(RosException error){check(error.result.error==13);check(error.getSuppressed().length==1);check(((RosException)error.getSuppressed()[0]).result.error==107);}
        check(failed.closed.equals(Arrays.asList(1L)));
        failedSession.closeChecked();check(failed.closed.size()==1);
        // Result-delivery cleanup must retain the setup failure, including when
        // the same preallocated Error is also raised by close (self suppression).
        OpenCleanup report=new OpenCleanup();Error primary=new OutOfMemoryError("local injected failure");
        report.started(primary);report.failed(primary);
        check(report.attempted());check(report.primary()==primary);check(report.failure()==primary);
        check(primary.getSuppressed().length==0);
        report.reset();check(!report.attempted()&&report.primary()==null&&report.failure()==null&&report.code()==0);
        RuntimeException setup=new RuntimeException("local setup");report.started(setup);report.returned(107);
        check(report.code()==107&&report.failure()==null&&report.primary()==setup);
        check(setup.getSuppressed().length==1&&((RosException)setup.getSuppressed()[0]).result.error==107);
        report.reset();report.started(setup);RuntimeException cleanupFailure=new RuntimeException("local close");report.failed(cleanupFailure);
        check(report.failure()==cleanupFailure&&report.primary()==setup);
        System.out.println("PASS JVM_LOCAL_LOGIC_ONLY assertions="+assertions+" runtime_ros_cases=0");
    }
}
