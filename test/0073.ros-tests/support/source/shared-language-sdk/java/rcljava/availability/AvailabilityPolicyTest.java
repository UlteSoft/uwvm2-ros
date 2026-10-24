package org.uwvm.rcljava.availability;
import java.time.Duration;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import org.ros2.rcljava.migration.*;
import org.ros2.rcljava.node.Node;
import org.ros2.rcljava.client.Client;
import org.uwvm.roswasm.*;

/** Local, deterministic contract models only. All methods are authored and
 * unexecuted. Synthetic time/booleans are not actual native or Wasm evidence.
 * Original service Future tests remain separate and wholly unchanged.
 */
public final class AvailabilityPolicyTest {
 static int checks;
 static void check(boolean b){++checks;if(!b)throw new AssertionError("availability check "+checks);}
 interface Work{void run()throws Exception;}
 static void code(int expected,Work w)throws Exception{try{w.run();throw new AssertionError("missing "+expected);}catch(RosException e){check(e.result.error==expected);}}
 static final class Model implements ServiceClientTransport {
  long next=1,now,features=7;int abi=1,queryError,featureError,clockError,closeError,queries,abiCalls,featureCalls,watches,submits;
  String denied="";boolean ready;long[] waits=new long[64];int falseBefore;long advanceOverride=-1;
  byte[] featureBytes,queryBytes;Runnable inQuery,inOpen;final List<String> opens=new ArrayList<>();final List<Long> closes=new ArrayList<>();
  public int backend(){return 2;}
  public int serviceClientAbiVersion(){++abiCalls;return abi;}
  public Result observeServiceClient(boolean f,long a,long s,long ns){
   check(a!=0&&s!=0&&a!=s);
   if(f){++featureCalls;return featureError!=0?new Result(featureError):new Result(0,featureBytes!=null?featureBytes:Codecs.INT64.encode(features),null,null);}
   waits[queries++]=ns;if(inQuery!=null)inQuery.run();if(queryError!=0)return new Result(queryError);
   now+=advanceOverride>=0?advanceOverride:ns;
   boolean answer=ready&&queries>falseBefore;
   return new Result(0,queryBytes!=null?queryBytes:new byte[]{(byte)(answer?1:0)},null,null);
  }
  public Result invoke(String op,long h,byte[] b,int capacity,long other,long correlation){
   if(op.equals("abi_version"))return Result.value(1);
   if(op.equals("capability_open")){String alias=Codecs.text(b);opens.add(alias);if(inOpen!=null)inOpen.run();return alias.equals(denied)?new Result(13):Result.value(next++);}
   if(op.equals("capability_close")){closes.add(h);return new Result(closeError);}
   if(op.equals("event_watch")){++watches;return new Result(0);}
   if(op.equals("time_read_steady"))return clockError!=0?new Result(clockError):new Result(0,Codecs.INT64.encode(now),null,null);
   if(op.equals("service_submit")){++submits;return Result.value(1);}
   if(op.startsWith("graph_")||op.equals("service_call"))throw new AssertionError("fake readiness/sync fallback");
   return new Result(95);
  }
 }
 static Deployment.ServiceBinding row(Deployment.AvailabilityBinding a){return new Deployment.ServiceBinding("/client","/add","example_interfaces/srv/AddTwoInts","official-default-service-profile","client","call","observe",4096,2,a);}
 static Deployment.AvailabilityBinding opt(boolean clock,int passes,Deployment.WaitCheckpoint checkpoint){return new Deployment.AvailabilityBinding("available",clock?"steady":null,10,passes,checkpoint);}
 static MigrationContext context(Model m,Deployment.AvailabilityBinding a,int owners){MigrationContext c=new MigrationContext(new Deployment.Profile(m,new Deployment.Binding[0],()->true,4,owners,8,16,new Deployment.ServiceBinding[]{row(a)}));c.init();return c;}
 static Client<?> client(MigrationContext c){return c.createNode("client","").createClient(example_interfaces.srv.AddTwoInts.class,"add");}
 static void testOldConstructorNoOptionalNamespace()throws Exception{
  Model m=new Model();MigrationContext c=context(m,null,4);Client<?> x=client(c);check(m.opens.size()==2&&m.abiCalls==0&&m.featureCalls==0);code(95,()->x.isServiceAvailable());code(95,()->x.waitForService());check(m.queries==0);c.shutdown();
 }
 static void testExplicitQueryPairAndBoolean()throws Exception{Model m=new Model();m.ready=true;MigrationContext c=context(m,opt(false,4,null),3);Client<?> x=client(c);check(m.opens.equals(Arrays.asList("call","observe","available")));check(x.isServiceAvailable()&&m.waits[0]==0&&m.submits==0);c.shutdown();check(m.closes.equals(Arrays.asList(3L,2L,1L)));}
 static void testFalseIsOnlySuccessfulZero()throws Exception{Model m=new Model();MigrationContext c=context(m,opt(false,4,null),3);check(!client(c).isServiceAvailable()&&m.queries==1);c.shutdown();}
 static void testEveryQueryErrorPropagates()throws Exception{for(int error:new int[]{11,13,17,22,90,95,107,110,125}){Model m=new Model();MigrationContext c=context(m,opt(false,4,null),3);Client<?> x=client(c);m.queryError=error;code(error,()->x.isServiceAvailable());check(m.queries==1&&m.submits==0);c.shutdown();}}
 static void testMalformedBooleanNeverFalse()throws Exception{for(byte[] data:new byte[][]{{},{2},{(byte)255},{0,1}}){Model m=new Model();MigrationContext c=context(m,opt(false,4,null),3);Client<?> x=client(c);m.queryBytes=data;code(22,()->x.isServiceAvailable());c.shutdown();}}
 static void testFeatureSentinelAndWrongExtentRollback()throws Exception{for(byte[] bytes:new byte[][]{{},{7},{-1,-1,-1,-1,-1,-1,-1,-1}}){Model m=new Model();m.featureBytes=bytes;MigrationContext c=context(m,opt(false,4,null),3);code(22,()->client(c));check(m.closes.equals(Arrays.asList(3L,2L,1L)));c.shutdown();}}
 static void testExplicitZeroFeatureRefuses()throws Exception{Model m=new Model();m.features=0;MigrationContext c=context(m,opt(false,4,null),3);code(95,()->client(c));check(m.watches==0&&m.closes.size()==3);c.shutdown();}
 static void testOptionalAbiBeforeFirstOpen()throws Exception{Model m=new Model();m.abi=2;MigrationContext c=context(m,opt(false,4,null),3);code(95,()->client(c));check(m.opens.isEmpty());c.shutdown();}
 static void testFourOwnerCapacityBeforeAnyOpen()throws Exception{Model m=new Model();MigrationContext c=context(m,opt(true,4,null),3);code(90,()->client(c));check(m.opens.isEmpty()&&m.abiCalls==0);c.shutdown();}
 static void testThirdOpenFailureRollback()throws Exception{Model m=new Model();m.denied="available";MigrationContext c=context(m,opt(true,4,null),4);code(13,()->client(c));check(m.closes.equals(Arrays.asList(2L,1L)));c.shutdown();}
 static void testFourthOpenFailureRollback()throws Exception{Model m=new Model();m.denied="steady";MigrationContext c=context(m,opt(true,4,null),4);code(13,()->client(c));check(m.closes.equals(Arrays.asList(3L,2L,1L)));c.shutdown();}
 static void testProfileAliasCollisionBeforeEffects()throws Exception{Model m=new Model();try{new Deployment.ServiceBinding("/client","/add","example_interfaces/srv/AddTwoInts","official-default-service-profile","client","call","observe",4096,2,new Deployment.AvailabilityBinding("call",null,10,3,null));throw new AssertionError();}catch(IllegalArgumentException expected){check(m.opens.isEmpty());}
  try{new Deployment.Profile(m,new Deployment.Binding[]{new Deployment.Binding("/client","/client","","","time_read_steady","available",8)},()->true,4,8,8,8,new Deployment.ServiceBinding[]{row(opt(false,4,null))});throw new AssertionError();}catch(IllegalArgumentException expected){check(m.opens.isEmpty());}}
 static void testFiniteWaitUsesActualElapsedAndRemainder()throws Exception{Model m=new Model();MigrationContext c=context(m,opt(true,10,null),4);check(!client(c).waitForService(Duration.ofNanos(25)));check(m.queries==3&&m.waits[0]==10&&m.waits[1]==10&&m.waits[2]==5&&m.now==25);c.shutdown();}
 static void testReadyReturnsSnapshotNoBusinessSend()throws Exception{Model m=new Model();m.ready=true;m.falseBefore=1;MigrationContext c=context(m,opt(true,8,null),4);check(client(c).waitForService(Duration.ofNanos(40))&&m.queries==2&&m.now==20&&m.submits==0);c.shutdown();}
 static void testQueryOnlyWaitRequiresClockBeforeQuery()throws Exception{Model m=new Model();MigrationContext c=context(m,opt(false,4,null),3);Client<?> x=client(c);code(13,()->x.waitForService(Duration.ofNanos(1)));code(13,()->x.waitForService());check(m.queries==0);check(!x.waitForService(Duration.ZERO)&&m.waits[0]==0);c.shutdown();}
 static void testNegativeInfiniteUsesFinitePhysicalSlices()throws Exception{Model m=new Model();m.ready=true;m.falseBefore=2;MigrationContext c=context(m,opt(true,8,null),4);check(client(c).waitForService(Duration.ofNanos(-2)));check(m.queries==3&&m.waits[2]==10);c.shutdown();}
 static void testPassExhaustionIsNotTimeoutOrFalse()throws Exception{Model m=new Model();MigrationContext c=context(m,opt(true,2,null),4);Client<?> x=client(c);code(90,()->x.waitForService());check(m.queries==2&&m.now==20);c.shutdown();}
 static void testShutdownBetweenQueriesNotBusy()throws Exception{Model m=new Model();MigrationContext[] c={null};Deployment.WaitCheckpoint cp=()->c[0].shutdown();c[0]=context(m,opt(true,8,cp),4);check(!client(c[0]).waitForService()&&!c[0].isValid()&&m.queries==1);check(m.closes.equals(Arrays.asList(4L,3L,2L,1L)));}
 static void testNativeQueryHoldsWholeContextLease()throws Exception{Model m=new Model();MigrationContext c=context(m,opt(true,4,null),4);Client<?> x=client(c);m.inQuery=()->{try{code(17,()->c.shutdown());code(17,()->x.dispose());code(17,()->x.isServiceAvailable());}catch(Exception e){throw new RuntimeException(e);}};check(!x.waitForService(Duration.ofNanos(10)));check(c.isValid()&&m.closes.isEmpty());c.shutdown();}
 static void testCheckpointRecursiveWaitAndSpinRefused()throws Exception{Model m=new Model();MigrationContext[] c={null};Client<?>[] x={null};Deployment.WaitCheckpoint cp=()->{try{code(17,()->x[0].waitForService());code(17,()->c[0].nextCredit());}catch(Exception e){throw new RuntimeException(e);}c[0].shutdown();};c[0]=context(m,opt(true,8,cp),4);x[0]=client(c[0]);check(!x[0].waitForService()&&m.queries==1);}
 static void testClockErrorIsNotReadiness()throws Exception{Model m=new Model();MigrationContext c=context(m,opt(true,4,null),4);Client<?> x=client(c);m.clockError=13;code(13,()->x.waitForService(Duration.ofNanos(10)));check(m.queries==0);c.shutdown();}
 static void testBackwardClockRefuses()throws Exception{Model m=new Model();m.now=20;MigrationContext c=context(m,opt(true,4,()->m.now=0),4);Client<?> x=client(c);code(22,()->x.waitForService(Duration.ofNanos(100)));check(m.queries==1);c.shutdown();}
 static void testDurationNullAndOverflowNoQueries()throws Exception{Model m=new Model();MigrationContext c=context(m,opt(true,4,null),4);Client<?> x=client(c);try{x.waitForService(null);throw new AssertionError();}catch(NullPointerException expected){}try{x.waitForService(Duration.ofSeconds(Long.MAX_VALUE));throw new AssertionError();}catch(ArithmeticException expected){}check(m.queries==0);c.shutdown();}
 static void testInterruptionPreserved()throws Exception{Model m=new Model();MigrationContext c=context(m,opt(true,4,null),4);Client<?> x=client(c);Thread.currentThread().interrupt();try{check(!x.waitForService()&&Thread.currentThread().isInterrupted()&&m.queries==0);}finally{Thread.interrupted();c.shutdown();}}
 static void testHighBitOwnersNeverNarrow()throws Exception{Model m=new Model();m.next=Long.MIN_VALUE;MigrationContext c=context(m,opt(false,4,null),3);Client<?> x=client(c);check(!x.isServiceAvailable());c.shutdown();check(m.closes.equals(Arrays.asList(Long.MIN_VALUE+2,Long.MIN_VALUE+1,Long.MIN_VALUE)));}
 static void testForeignSessionRefusedBeforeOptionalImport()throws Exception{Model m=new Model();Session a=new Session(m,2),b=new Session(m,2);Capability q=a.open("q"),s=b.open("s");code(9,()->ServiceClient.query(q,s,0));check(m.queries==0);a.close();b.close();}
 @SuppressWarnings({"unchecked","rawtypes"})
 static void testAllSelectedServiceFamilies()throws Exception{for(Class family:new Class[]{example_interfaces.srv.AddTwoInts.class,std_srvs.srv.Trigger.class,std_srvs.srv.SetBool.class}){
  Model m=new Model();String name=family==example_interfaces.srv.AddTwoInts.class?"example_interfaces/srv/AddTwoInts":family==std_srvs.srv.Trigger.class?"std_srvs/srv/Trigger":"std_srvs/srv/SetBool";
  Deployment.ServiceBinding binding=new Deployment.ServiceBinding("/client","/add",name,"official-default-service-profile","client","call","observe",4096,2,opt(false,4,null));
  MigrationContext c=new MigrationContext(new Deployment.Profile(m,new Deployment.Binding[0],null,4,3,8,0,new Deployment.ServiceBinding[]{binding}));c.init();Client<?> x=c.createNode("client","").createClient(family,"add");check(!x.isServiceAvailable()&&x.getServiceName().equals("add")&&m.submits==0);c.shutdown();
 }}
 static void testFactoryPrimaryAndAllCleanupOutcomesRetained()throws Exception{Model m=new Model();m.denied="steady";m.closeError=110;MigrationContext c=context(m,opt(true,4,null),4);code(13,()->client(c));MigrationContext.CleanupAttempt a=c.lastFactoryCleanup();check(a.primary instanceof RosException&&((RosException)a.primary).result.error==13);int actual=0;for(;a!=null;a=a.next)if(a.attempted){check(a.result.error==110);++actual;}check(actual==3&&m.closes.equals(Arrays.asList(3L,2L,1L)));c.shutdown();}
 public static void main(String[] args)throws Exception{
  testAllSelectedServiceFamilies();testFactoryPrimaryAndAllCleanupOutcomesRetained();testOldConstructorNoOptionalNamespace();testExplicitQueryPairAndBoolean();testFalseIsOnlySuccessfulZero();testEveryQueryErrorPropagates();testMalformedBooleanNeverFalse();testFeatureSentinelAndWrongExtentRollback();testExplicitZeroFeatureRefuses();testOptionalAbiBeforeFirstOpen();testFourOwnerCapacityBeforeAnyOpen();testThirdOpenFailureRollback();testFourthOpenFailureRollback();testProfileAliasCollisionBeforeEffects();testFiniteWaitUsesActualElapsedAndRemainder();testReadyReturnsSnapshotNoBusinessSend();testQueryOnlyWaitRequiresClockBeforeQuery();testNegativeInfiniteUsesFinitePhysicalSlices();testPassExhaustionIsNotTimeoutOrFalse();testShutdownBetweenQueriesNotBusy();testNativeQueryHoldsWholeContextLease();testCheckpointRecursiveWaitAndSpinRefused();testClockErrorIsNotReadiness();testBackwardClockRefuses();testDurationNullAndOverflowNoQueries();testInterruptionPreserved();testHighBitOwnersNeverNarrow();testForeignSessionRefusedBeforeOptionalImport();
  System.out.println("PASS_RCLJAVA_AVAILABILITY_MODELS groups=28 checks="+checks+" native=0");
 }
}
