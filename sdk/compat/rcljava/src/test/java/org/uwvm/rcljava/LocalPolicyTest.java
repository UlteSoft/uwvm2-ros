package org.uwvm.rcljava;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.concurrent.TimeUnit;
import org.ros2.rcljava.concurrent.Callback;
import org.ros2.rcljava.interfaces.Disposable;
import org.ros2.rcljava.migration.Deployment;
import org.ros2.rcljava.migration.MigrationContext;
import org.ros2.rcljava.node.Node;
import org.ros2.rcljava.publisher.Publisher;
import org.ros2.rcljava.subscription.Subscription;
import org.ros2.rcljava.timer.WallTimer;
import org.ros2.rcljava.qos.QoSProfile;
import org.uwvm.roswasm.Codecs;
import org.uwvm.roswasm.Result;
import org.uwvm.roswasm.RosException;
import org.uwvm.roswasm.Transport;

/** Authored JVM-only models. No Runtime, target bridge, native wait-set, RMW,
 * JNI, Guest, native ROS or delivery result is inferred from these assertions.
 */
public final class LocalPolicyTest {
    static int checks;
    static void check(boolean value) { ++checks;if(!value)throw new AssertionError("local contract "+checks); }
    interface Operation { void run(); }
    static void code(int expected,Operation operation) {
        try { operation.run();throw new AssertionError("expected errno "+expected); }
        catch(RosException error) { check(error.result.error==expected); }
    }
    static void unsupported(Operation operation) {
        try { operation.run();throw new AssertionError("unsupported native feature accepted"); }
        catch(UnsupportedOperationException expected) { ++checks; }
    }
    public static final class Model implements Transport {
        public final List<String> calls=new ArrayList<String>();
        public final List<Long> closed=new ArrayList<Long>();
        public final List<byte[]> published=new ArrayList<byte[]>();
        public long next=1,clock;public int version=1,openError,subscribeError,clockError,closeError;
        public Throwable closeFailure,clockFailure;
        public Result poll=new Result(11);
        public int backend() { return 2; }
        public Result invoke(String operation,long owner,byte[] bytes,int capacity,long other,long correlation) {
            calls.add(operation);
            if(operation.equals("abi_version"))return Result.value(version);
            if(operation.equals("capability_open"))return openError!=0?new Result(openError):Result.value(next++);
            if(operation.equals("capability_close")) { closed.add(owner);if(closeFailure instanceof Error)throw (Error)closeFailure;if(closeFailure instanceof RuntimeException)throw (RuntimeException)closeFailure;return new Result(closeError); }
            if(operation.equals("topic_subscribe"))return new Result(subscribeError);
            if(operation.equals("topic_publish")){published.add(bytes.clone());return new Result(0);}
            if(operation.equals("time_read_steady")){if(clockFailure instanceof Error)throw (Error)clockFailure;if(clockFailure instanceof RuntimeException)throw (RuntimeException)clockFailure;return clockError!=0?new Result(clockError):new Result(0,Codecs.INT64.encode(clock),null,null);}
            if(operation.equals("event_poll"))return poll;
            return new Result(0);
        }
        int count(String name) { int n=0;for(String c:calls)if(c.equals(name))++n;return n; }
    }
    static Deployment.Binding topic(String node,boolean publish) {
        return new Deployment.Binding(node,"/topic","std_msgs/msg/String","official-default-profile",publish?"topic_publish":"topic_subscribe",publish?"publish":"subscribe",publish?0:1024);
    }
    static Deployment.Binding clock(String node) { return new Deployment.Binding(node,node,"","","time_read_steady","steady",8); }
    static MigrationContext context(Model model,int resources,Deployment.Binding...bindings) {
        MigrationContext c=new MigrationContext(new Deployment.Profile(model,bindings,null,8,64,resources,0));c.init();return c;
    }
    static std_msgs.msg.String message(String text) { std_msgs.msg.String v=new std_msgs.msg.String();v.setData(text);return v; }
    static void testDefaultDenialHasNoOpen() {
        Model m=new Model();MigrationContext c=context(m,8);Node n=c.createNode("robot","");
        code(13,()->n.createPublisher(std_msgs.msg.String.class,"topic"));
        check(m.count("capability_open")==0);c.shutdown();
    }
    static void testFailedAbiDoesNotCommit() {
        Model m=new Model();m.version=2;MigrationContext c=new MigrationContext(new Deployment.Profile(m,new Deployment.Binding[0],null,4,8,4,0));
        code(95,()->c.init());check(!c.isValid());code(125,()->c.createNode("bad",""));check(m.count("capability_open")==0);
    }
    static void testCompleteStringAndUnsignedOwner() {
        Model m=new Model();m.next=-1L;MigrationContext c=context(m,8,topic("/robot",true));Node n=c.createNode("robot","");
        Publisher<std_msgs.msg.String> p=n.createPublisher(std_msgs.msg.String.class,"topic");p.publish(message("rcljava\0π中🙂"));
        check(Codecs.STRING.decode(m.published.get(0)).equals("rcljava\0π中🙂"));p.dispose();check(m.closed.equals(Arrays.asList(-1L)));c.shutdown();check(m.closed.size()==1);
    }
    static void testActivationFailureRollsBackOne() {
        Model m=new Model();m.subscribeError=13;m.closeError=107;MigrationContext c=context(m,8,topic("/robot",false));Node n=c.createNode("robot","");
        code(13,()->n.createSubscription(std_msgs.msg.String.class,"topic",v->{}));
        check(m.closed.equals(Arrays.asList(1L)));check(n.getSubscriptions().isEmpty());
        check(c.lastFactoryCleanup().attempted&&c.lastFactoryCleanup().result.error==107);c.shutdown();check(m.closed.size()==1);
    }
    static void testSharedPrimaryIdentityDoesNotSelfSuppress() {
        Model m=new Model();Error shared=new OutOfMemoryError("injected close");m.closeFailure=shared;
        MigrationContext c=context(m,8,clock("/robot"));Node n=c.createNode("robot","");m.clockFailure=shared;
        try { n.createWallTimer(1,TimeUnit.SECONDS,()->{});throw new AssertionError(); }
        catch(Error primary) { check(primary==shared);check(c.lastFactoryCleanup().failure==shared);check(c.lastFactoryCleanup().primary==shared);check(primary.getSuppressed().length==0); }
        check(m.closed.size()==1);check(n.getTimers().isEmpty());c.shutdown();
    }
    static void testAllCleanupAttemptsAfterError() {
        Model m=new Model();MigrationContext c=context(m,8,topic("/a",true),topic("/b",true));
        c.createNode("a","").createPublisher(std_msgs.msg.String.class,"topic");c.createNode("b","").createPublisher(std_msgs.msg.String.class,"topic");
        Error shared=new OutOfMemoryError("two actual model closes");m.closeFailure=shared;
        try { c.shutdown();throw new AssertionError(); }catch(MigrationContext.CleanupFailure e) { check(e.report==c.lastCleanup()); }
        check(m.closed.equals(Arrays.asList(2L,1L)));check(c.lastCleanup().completed);check(!c.isValid());
        int errors=0;for(MigrationContext.CleanupAttempt a:c.lastCleanup().attempts)if(a!=null&&a.failure==shared)++errors;check(errors==2);
    }
    static void testWholeContextCallbackLeaseAndRelease() {
        Model m=new Model();MigrationContext c=context(m,8,topic("/a",false),topic("/b",true));
        Node a=c.createNode("a",""),b=c.createNode("b","");Publisher<std_msgs.msg.String> pub=b.createPublisher(std_msgs.msg.String.class,"topic");
        final int[] callbacks={0};
        Subscription<std_msgs.msg.String> sub=a.createSubscription(std_msgs.msg.String.class,"topic",v->{
            check(v.getData().equals("native-model-only"));++callbacks[0];code(17,()->pub.dispose());code(17,()->c.revoke(pub));code(17,()->c.shutdown());code(17,()->b.createPublisher(std_msgs.msg.String.class,"topic"));
        });
        Object executor=new Object();c.attachExecutor(a,executor);m.poll=new Result(0,Codecs.STRING.encode("native-model-only"),null,null);
        check(c.poll(a,executor)==0&&callbacks[0]==1);check(m.closed.isEmpty());check(!c.isDispatching());c.detachExecutor(a,executor);c.shutdown();check(m.closed.size()==2);
    }
    static void testCallbackExceptionReleasesLease() {
        Model m=new Model();MigrationContext c=context(m,8,topic("/a",false));Node a=c.createNode("a","");RuntimeException primary=new RuntimeException("application");
        a.createSubscription(std_msgs.msg.String.class,"topic",v->{throw primary;});Object executor=new Object();c.attachExecutor(a,executor);
        m.poll=new Result(0,Codecs.STRING.encode("one"),null,null);
        try { c.poll(a,executor);throw new AssertionError(); }catch(RuntimeException e) { check(e==primary); }
        check(!c.isDispatching());c.detachExecutor(a,executor);c.shutdown();check(m.closed.size()==1);
    }
    static void testCapacityBeforeNewClockEffect() {
        Model m=new Model();MigrationContext c=context(m,1,topic("/a",true),clock("/a"));Node n=c.createNode("a","");n.createPublisher(std_msgs.msg.String.class,"topic");
        int before=m.calls.size();code(90,()->n.createWallTimer(1,TimeUnit.SECONDS,()->{}));check(m.calls.size()==before);c.shutdown();
    }
    static void testClockFailureRetiresOnlyFreshOwner() {
        Model m=new Model();MigrationContext c=context(m,8,clock("/a"));Node n=c.createNode("a","");m.clockError=110;
        code(110,()->n.createWallTimer(1,TimeUnit.SECONDS,()->{}));check(m.closed.equals(Arrays.asList(1L)));check(m.count("time_read_steady")==1);c.shutdown();
    }
    static void testTimerPhaseSkipsMissedPeriods() {
        Model m=new Model();MigrationContext c=context(m,8,clock("/a"));Node n=c.createNode("a","");int[] called={0};WallTimer t=n.createWallTimer(500,TimeUnit.MILLISECONDS,()->++called[0]);Object executor=new Object();c.attachExecutor(n,executor);
        m.clock=1750000000L;check(c.poll(n,executor)==0&&called[0]==1);
        m.clock=1800000000L;check(t.timeUntilNextCall()==200000000L);
        m.clock=1900000000L;check(c.poll(n,executor)==11&&called[0]==1);
        m.clock=2100000000L;check(c.poll(n,executor)==0&&called[0]==2);
        t.cancel();m.clock=Long.MAX_VALUE;check(c.poll(n,executor)==11);c.detachExecutor(n,executor);c.shutdown();
    }
    static void testClockRollbackResetsPhase() {
        Model m=new Model();m.clock=1000000000L;MigrationContext c=context(m,8,clock("/a"));Node n=c.createNode("a","");WallTimer t=n.createWallTimer(500,TimeUnit.MILLISECONDS,()->{});
        m.clock=100L;check(!t.isReady());check(t.timeUntilNextCall()==500000000L);c.shutdown();
    }
    static void testNativePointersAndUnsupportedTypesRefuse() {
        Model m=new Model();MigrationContext c=context(m,8);Node n=c.createNode("a","");
        unsupported(()->n.getHandle());unsupported(()->message("x").getTypeSupportInstance());unsupported(()->QoSProfile.keepLast(10));
        check(m.count("capability_open")==0);c.shutdown();
    }
    static void testDeadlineAndNoAutomaticReplay() {
        Model m=new Model();MigrationContext c=context(m,8,topic("/a",false));Node n=c.createNode("a","");n.createSubscription(std_msgs.msg.String.class,"topic",v->{throw new AssertionError();});Object x=new Object();c.attachExecutor(n,x);
        m.poll=new Result(90);code(90,()->c.poll(n,x));check(m.count("event_poll")==1);
        m.poll=new Result(110);code(110,()->c.poll(n,x));check(m.count("event_poll")==2);c.detachExecutor(n,x);c.shutdown();
    }
    static void testBoundedDriverEvenIfItAlwaysReturnsTrue() {
        Model m=new Model();final int[] credits={0};MigrationContext c=new MigrationContext(new Deployment.Profile(m,new Deployment.Binding[0],()->{++credits[0];return true;},4,8,4,2));c.init();
        check(c.nextCredit());check(c.nextCredit());check(!c.nextCredit());check(credits[0]==2);c.shutdown();
    }
    public static void main(String[] args) {
        testDefaultDenialHasNoOpen();testFailedAbiDoesNotCommit();testCompleteStringAndUnsignedOwner();testActivationFailureRollsBackOne();testSharedPrimaryIdentityDoesNotSelfSuppress();testAllCleanupAttemptsAfterError();testWholeContextCallbackLeaseAndRelease();testCallbackExceptionReleasesLease();testCapacityBeforeNewClockEffect();testClockFailureRetiresOnlyFreshOwner();testTimerPhaseSkipsMissedPeriods();testClockRollbackResetsPhase();testNativePointersAndUnsupportedTypesRefuse();testDeadlineAndNoAutomaticReplay();testBoundedDriverEvenIfItAlwaysReturnsTrue();
        System.out.println("PASS LOCAL_RCLJAVA_MIGRATION_POLICY_ONLY checks="+checks+" native_ros_cases=0");
    }
}
