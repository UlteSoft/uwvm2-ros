package org.uwvm.rcljava.services;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.concurrent.Future;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;
import org.ros2.rcljava.migration.Deployment;
import org.ros2.rcljava.migration.MigrationContext;
import org.ros2.rcljava.migration.ServiceTypes;
import org.ros2.rcljava.node.Node;
import org.ros2.rcljava.client.Client;
import org.ros2.rcljava.service.Service;
import org.uwvm.roswasm.*;

/** Deterministic local models of the actual SDK facade. These methods are
 * authored, not executed. They prove no JNI, native ROS, WASI or delivery fact.
 */
public final class ServicePolicyTest {
 static int checks;
 static void check(boolean v){++checks;if(!v)throw new AssertionError("service local check "+checks);}
 interface Work{void run()throws Exception;}
 static void code(int expected,Work work)throws Exception{try{work.run();throw new AssertionError("missing "+expected);}catch(RosException e){check(e.result.error==expected);}}
 static void futureCode(int expected,Future<?> f)throws Exception{try{f.get();throw new AssertionError("missing Future error");}catch(ExecutionException e){check(e.getCause() instanceof MigrationContext.ServiceFailure&&((MigrationContext.ServiceFailure)e.getCause()).code==expected);}}
 static final class Model implements Transport {
  final List<String> calls=new ArrayList<>();final List<Long> closes=new ArrayList<>();
  long handle=1,id=1,source,observer,sequence;int openError,watchError,activateError,submitError,closeError;String denied="";
  byte[] event,request,response;boolean credit=true;
  public int backend(){return 2;}
  public Result invoke(String op,long h,byte[] data,int cap,long other,long correlation){
   calls.add(op);
   if(op.equals("abi_version"))return Result.value(1);
   if(op.equals("capability_open")){String alias=Codecs.text(data);if(openError!=0||alias.equals(denied))return new Result(openError!=0?openError:13);return Result.value(handle++);}
   if(op.equals("capability_close")){closes.add(h);return new Result(closeError);}
   if(op.equals("capability_revoke"))return new Result(0);
   if(op.equals("event_watch")){observer=h;source=other;return new Result(watchError);}
   if(op.equals("service_advertise"))return new Result(activateError);
   if(op.equals("service_submit")){if(submitError!=0)return new Result(submitError);request=data.clone();return Result.value(id++);}
   if(op.equals("event_poll_v1")){if(event==null)return new Result(11);byte[] b=event;event=null;return new Result(0,b,null,null);}
   if(op.equals("event_poll")){if(request==null)return new Result(11);byte[] b=request;request=null;return new Result(0,b,null,null);}
   if(op.equals("service_respond")){response=data.clone();return new Result(0,new byte[0],null,null);}
   if(op.equals("service_call"))throw new AssertionError("sync fallback is forbidden");
   return new Result(95);
  }
  int count(String name){int n=0;for(String x:calls)if(x.equals(name))++n;return n;}
  void queue(int kind,long correlation,byte[] body,int error,int flags){
   event=new Codecs.Writer(64+body.length).integer(0x45525755L,4,false).integer(1,2,false).integer(kind,2,false).integer(2,1,false).integer(flags,1,false).integer(0,2,false).integer(error,4,false).integer(source,8,false).integer(correlation,8,false).integer(++sequence,8,false).integer(77,8,false).integer(88,8,false).integer(body.length,8,false).raw(body).finish();
  }
 }
 static Deployment.ServiceBinding binding(String node,String role,String type){return new Deployment.ServiceBinding(node,"/add",type,"official-default-service-profile",role,role.equals("client")?"call":"advertise",role.equals("client")?"observe":"respond",4096,2);}
 static MigrationContext context(Model m,Deployment.ServiceBinding...bindings){MigrationContext c=new MigrationContext(new Deployment.Profile(m,new Deployment.Binding[0],()->m.credit,8,32,8,8,bindings));c.init();return c;}
 static Client<example_interfaces.srv.AddTwoInts> client(MigrationContext c)throws Exception{return c.createNode("client","").createClient(example_interfaces.srv.AddTwoInts.class,"add");}
 static example_interfaces.srv.AddTwoInts_Request request(){return new example_interfaces.srv.AddTwoInts_Request().setA(41).setB(1);}
 static void poll(MigrationContext c,Node n)throws Exception{Object token=new Object();c.attachExecutor(n,token);try{c.poll(n,token);}finally{c.detachExecutor(n,token);}}
 static void testDefaultDeniedBeforeOpen()throws Exception{Model m=new Model();MigrationContext c=context(m);code(13,()->client(c));check(m.count("capability_open")==0);c.shutdown();}
 static void testDistinctBeforeEffects()throws Exception{Model m=new Model();try{new Deployment.ServiceBinding("/client","/add","example_interfaces/srv/AddTwoInts","official-default-service-profile","client","same","same",4096,2);throw new AssertionError();}catch(IllegalArgumentException expected){check(m.calls.isEmpty());}}
 static void testSecondOpenRollback()throws Exception{Model m=new Model();m.denied="observe";m.closeError=110;MigrationContext c=context(m,binding("/client","client","example_interfaces/srv/AddTwoInts"));code(13,()->client(c));check(m.closes.equals(Arrays.asList(1L)));check(c.lastFactoryCleanup().result.error==110);try{c.shutdown();}catch(MigrationContext.CleanupFailure retained){}}
 static void testWatchFailureClosesBoth()throws Exception{Model m=new Model();m.watchError=13;MigrationContext c=context(m,binding("/client","client","example_interfaces/srv/AddTwoInts"));code(13,()->client(c));check(m.closes.equals(Arrays.asList(2L,1L)));c.shutdown();check(m.closes.size()==2);}
 static void testActivationFailureClosesBoth()throws Exception{Model m=new Model();m.activateError=13;MigrationContext c=context(m,binding("/server","server","example_interfaces/srv/AddTwoInts"));Node n=c.createNode("server","");code(13,()->n.createService(example_interfaces.srv.AddTwoInts.class,"add",(h,q,r)->{}));check(m.closes.equals(Arrays.asList(2L,1L)));c.shutdown();}
 static void testActualAsyncAndCompleteResponse()throws Exception{Model m=new Model();MigrationContext c=context(m,binding("/client","client","example_interfaces/srv/AddTwoInts"));Node n=c.createNode("client","");Client<example_interfaces.srv.AddTwoInts> client=n.createClient(example_interfaces.srv.AddTwoInts.class,"add");Future<example_interfaces.srv.AddTwoInts_Response> f=client.asyncSendRequest(request());check(!f.isDone()&&m.count("service_submit")==1&&m.count("service_call")==0);m.queue(3,1,Codecs.INT64.encode(42L),0,0);poll(c,n);check(f.get().getSum()==42);client.dispose();c.shutdown();check(m.closes.size()==2);}
 static void testFiniteGetActuallyDrivesPoll()throws Exception{Model m=new Model();MigrationContext c=context(m,binding("/client","client","example_interfaces/srv/AddTwoInts"));Client<example_interfaces.srv.AddTwoInts> client=client(c);Future<example_interfaces.srv.AddTwoInts_Response> f=client.asyncSendRequest(request());m.queue(3,1,Codecs.INT64.encode(42L),0,0);check(f.get().getSum()==42&&m.count("event_poll_v1")==1);c.shutdown();}
 static void testAvailability95NoProbe()throws Exception{Model m=new Model();MigrationContext c=context(m,binding("/client","client","example_interfaces/srv/AddTwoInts"));Client<?> client=client(c);int calls=m.calls.size();code(95,()->client.isServiceAvailable());code(95,()->client.waitForService());code(95,()->client.waitForService(java.time.Duration.ZERO));check(m.calls.size()==calls);c.shutdown();}
 static void testTimedWaitNeverTreatsStepsAsNanoseconds()throws Exception{Model m=new Model();MigrationContext c=context(m,binding("/client","client","example_interfaces/srv/AddTwoInts"));Future<?> f=client(c).asyncSendRequest(request());int calls=m.calls.size();code(95,()->f.get(1,TimeUnit.NANOSECONDS));check(m.calls.size()==calls);try{f.get(0,TimeUnit.NANOSECONDS);throw new AssertionError();}catch(TimeoutException expected){}c.shutdown();}
 static void testFullWidthId()throws Exception{Model m=new Model();m.id=-1L;MigrationContext c=context(m,binding("/client","client","example_interfaces/srv/AddTwoInts"));Node n=c.createNode("client","");Client<?> cl=n.createClient(example_interfaces.srv.AddTwoInts.class,"add");Future<example_interfaces.srv.AddTwoInts_Response> f=cl.asyncSendRequest(request());m.queue(3,-1L,Codecs.INT64.encode(-7L),0,0);poll(c,n);check(f.get().getSum()==-7L);c.shutdown();}
 static void testOutOfOrderIndependentRequests()throws Exception{Model m=new Model();MigrationContext c=context(m,binding("/client","client","example_interfaces/srv/AddTwoInts"));Node n=c.createNode("client","");Client<?> cl=n.createClient(example_interfaces.srv.AddTwoInts.class,"add");Future<example_interfaces.srv.AddTwoInts_Response> a=cl.asyncSendRequest(request()),b=cl.asyncSendRequest(request());m.queue(3,2,Codecs.INT64.encode(84L),0,0);poll(c,n);check(b.get().getSum()==84&&!a.isDone());m.queue(3,1,Codecs.INT64.encode(42L),0,0);poll(c,n);check(a.get().getSum()==42);c.shutdown();}
 static void testCapacityBeforeThirdSubmit()throws Exception{Model m=new Model();MigrationContext c=context(m,binding("/client","client","example_interfaces/srv/AddTwoInts"));Client<?> cl=client(c);cl.asyncSendRequest(request());cl.asyncSendRequest(request());code(90,()->cl.asyncSendRequest(request()));check(m.count("service_submit")==2);c.shutdown();}
 static void testPerRequestErrorPreservesOther()throws Exception{Model m=new Model();MigrationContext c=context(m,binding("/client","client","example_interfaces/srv/AddTwoInts"));Node n=c.createNode("client","");Client<?> cl=n.createClient(example_interfaces.srv.AddTwoInts.class,"add");Future<?> a=cl.asyncSendRequest(request()),b=cl.asyncSendRequest(request());m.queue(6,1,new byte[0],110,2);poll(c,n);futureCode(110,a);check(!b.isDone());c.shutdown();futureCode(125,b);}
 static void testCloseFinishesAllWithoutNativeCancellation()throws Exception{Model m=new Model();MigrationContext c=context(m,binding("/client","client","example_interfaces/srv/AddTwoInts"));Client<?> cl=client(c);Future<?> a=cl.asyncSendRequest(request()),b=cl.asyncSendRequest(request());cl.dispose();futureCode(125,a);futureCode(125,b);check(m.closes.equals(Arrays.asList(2L,1L)));c.shutdown();check(m.closes.size()==2);}
 static void testCallbackWholeContextLease()throws Exception {
  Model m=new Model();MigrationContext c=context(m,binding("/client","client","example_interfaces/srv/AddTwoInts"));
  Node n=c.createNode("client","");Client<?> cl=n.createClient(example_interfaces.srv.AddTwoInts.class,"add");
  boolean[] called={false};Future<?>[] pending={null};
  Future<?> f=cl.asyncSendRequest(request(),ready->{try {
   code(17,()->cl.dispose());code(17,()->c.shutdown());
   code(17,()->n.createClient(example_interfaces.srv.AddTwoInts.class,"add"));
   code(17,()->pending[0].get());code(17,()->pending[0].get(0,TimeUnit.NANOSECONDS));
   code(17,()->pending[0].get(0,null)); // Preserve pending lease-before-invalid-unit precedence.
   int calls=m.calls.size();check(ready.isDone());
   check(((example_interfaces.srv.AddTwoInts_Response)ready.get()).getSum()==42);
   check(((example_interfaces.srv.AddTwoInts_Response)ready.get(0,TimeUnit.NANOSECONDS)).getSum()==42);
   try { ready.get(0,null);throw new AssertionError("null TimeUnit"); }
   catch(NullPointerException expected) { }
   check(m.calls.size()==calls);called[0]=true;
  }catch(Exception e){throw new RuntimeException(e);}});
  pending[0]=cl.asyncSendRequest(request());
  m.queue(3,1,Codecs.INT64.encode(42L),0,0);poll(c,n);
  check(called[0]&&f.isDone()&&!pending[0].isDone()&&m.closes.isEmpty());c.shutdown();
 }
 static void testReadyErrorGetInsideCallback()throws Exception {
  Model m=new Model();MigrationContext c=context(m,binding("/client","client","example_interfaces/srv/AddTwoInts"));
  Node n=c.createNode("client","");Client<?> cl=n.createClient(example_interfaces.srv.AddTwoInts.class,"add");
  boolean[] called={false};
  Future<?> f=cl.asyncSendRequest(request(),ready->{try {
   int calls=m.calls.size();check(ready.isDone());futureCode(110,ready);
   try { ready.get(0,TimeUnit.NANOSECONDS);throw new AssertionError("missing terminal failure"); }
   catch(ExecutionException expected) {
    check(expected.getCause() instanceof MigrationContext.ServiceFailure);
    check(((MigrationContext.ServiceFailure)expected.getCause()).code==110);
   }
   check(m.calls.size()==calls);called[0]=true;
  }catch(Exception e){throw new RuntimeException(e);}});
  m.queue(6,1,new byte[0],110,2);poll(c,n);
  check(called[0]&&f.isDone()&&m.count("service_submit")==1);c.shutdown();
 }
 static void testMalformedCancelledTombstoneIsStillDecoded()throws Exception{Model m=new Model();MigrationContext c=context(m,binding("/client","client","example_interfaces/srv/AddTwoInts"));Node n=c.createNode("client","");Client<?> cl=n.createClient(example_interfaces.srv.AddTwoInts.class,"add");Future<?> a=cl.asyncSendRequest(request()),b=cl.asyncSendRequest(request());check(a.cancel(false));m.queue(3,1,new byte[7],0,0);try{poll(c,n);throw new AssertionError();}catch(IllegalArgumentException expected){}check(a.isCancelled()&&b.isDone()&&m.count("service_submit")==2);c.shutdown();}
 static void testUnknownCorrelationFailsClosed()throws Exception{Model m=new Model();MigrationContext c=context(m,binding("/client","client","example_interfaces/srv/AddTwoInts"));Node n=c.createNode("client","");Client<?> cl=n.createClient(example_interfaces.srv.AddTwoInts.class,"add");Future<?> a=cl.asyncSendRequest(request());m.queue(3,99,Codecs.INT64.encode(42L),0,0);code(22,()->poll(c,n));check(a.isDone());c.shutdown();}
 static void testCallbackThrowNoReplay()throws Exception{Model m=new Model();MigrationContext c=context(m,binding("/client","client","example_interfaces/srv/AddTwoInts"));Node n=c.createNode("client","");Client<?> cl=n.createClient(example_interfaces.srv.AddTwoInts.class,"add");RuntimeException primary=new RuntimeException("callback");Future<?> f=cl.asyncSendRequest(request(),ready->{throw primary;});m.queue(3,1,Codecs.INT64.encode(42L),0,0);try{poll(c,n);throw new AssertionError();}catch(RuntimeException e){check(e==primary);}check(f.isDone()&&!c.isDispatching());poll(c,n);check(m.count("service_submit")==1);c.shutdown();}
 static void testCompleteServerAndAbsentRmwHeader()throws Exception{Model m=new Model();MigrationContext c=context(m,binding("/server","server","example_interfaces/srv/AddTwoInts"));Node n=c.createNode("server","");boolean[] called={false};Service<?> s=n.createService(example_interfaces.srv.AddTwoInts.class,"add",(org.ros2.rcljava.service.RMWRequestId h,example_interfaces.srv.AddTwoInts_Request q,example_interfaces.srv.AddTwoInts_Response r)->{check(h==null&&q.getA()==41&&q.getB()==1);try{code(17,()->c.shutdown());}catch(Exception e){throw new RuntimeException(e);}r.setSum(42);called[0]=true;});m.request=Codecs.prefixId(-1L,ServiceTypes.find(example_interfaces.srv.AddTwoInts.class).encodeRequest(request()));poll(c,n);check(called[0]&&new Codecs.Reader(m.response).unsigned(8)==-1L&&m.response.length==16);s.dispose();c.shutdown();}
 static void testCompleteTriggerAndSetBoolCodecs()throws Exception{ServiceTypes.Descriptor t=ServiceTypes.find(std_srvs.srv.Trigger.class),b=ServiceTypes.find(std_srvs.srv.SetBool.class);check(t.encodeRequest(new std_srvs.srv.Trigger_Request()).length==0);std_srvs.srv.Trigger_Response r=new std_srvs.srv.Trigger_Response().setSuccess(true).setMessage("π\0中");std_srvs.srv.Trigger_Response x=(std_srvs.srv.Trigger_Response)t.decodeResponse(t.encodeResponse(r));check(x.getSuccess()&&x.getMessage().equals(r.getMessage()));check(((std_srvs.srv.SetBool_Request)b.decodeRequest(b.encodeRequest(new std_srvs.srv.SetBool_Request().setData(true)))).getData());try{b.decodeRequest(new byte[]{2});throw new AssertionError();}catch(IllegalArgumentException expected){}}
 static void testWrongSourceRefusesAll()throws Exception{Model m=new Model();MigrationContext c=context(m,binding("/client","client","example_interfaces/srv/AddTwoInts"));Node n=c.createNode("client","");Client<?> cl=n.createClient(example_interfaces.srv.AddTwoInts.class,"add");Future<?> f=cl.asyncSendRequest(request());m.source++;m.queue(3,1,Codecs.INT64.encode(42L),0,0);code(22,()->poll(c,n));check(f.isDone());c.shutdown();}
 static void testDuplicateSequenceRefuses()throws Exception{Model m=new Model();MigrationContext c=context(m,binding("/client","client","example_interfaces/srv/AddTwoInts"));Node n=c.createNode("client","");Client<?> cl=n.createClient(example_interfaces.srv.AddTwoInts.class,"add");Future<?> a=cl.asyncSendRequest(request());m.queue(3,1,Codecs.INT64.encode(42L),0,0);poll(c,n);Future<?> b=cl.asyncSendRequest(request());m.sequence=0;m.queue(3,2,Codecs.INT64.encode(84L),0,0);code(22,()->poll(c,n));check(a.isDone()&&b.isDone());c.shutdown();}
 static void testNoDriverGet95BeforePoll()throws Exception{Model m=new Model();MigrationContext c=new MigrationContext(new Deployment.Profile(m,new Deployment.Binding[0],null,4,16,8,0,new Deployment.ServiceBinding[]{binding("/client","client","example_interfaces/srv/AddTwoInts")}));c.init();Future<?> f=client(c).asyncSendRequest(request());code(95,()->f.get());check(m.count("event_poll_v1")==0&&!f.isDone());c.shutdown();}
 static void testDriverExhaustionDoesNotInventCompletion()throws Exception{Model m=new Model();m.credit=false;MigrationContext c=context(m,binding("/client","client","example_interfaces/srv/AddTwoInts"));Future<?> f=client(c).asyncSendRequest(request());try{f.get();throw new AssertionError();}catch(InterruptedException expected){check(!f.isDone()&&m.count("event_poll_v1")==0);}c.shutdown();futureCode(125,f);}
 static void testSourceRetirementTriesEveryCallback()throws Exception{Model m=new Model();MigrationContext c=context(m,binding("/client","client","example_interfaces/srv/AddTwoInts"));Node n=c.createNode("client","");Client<?> cl=n.createClient(example_interfaces.srv.AddTwoInts.class,"add");RuntimeException primary=new RuntimeException("first");boolean[] second={false};Future<?> a=cl.asyncSendRequest(request(),f->{throw primary;}),b=cl.asyncSendRequest(request(),f->{second[0]=true;});m.queue(6,0,new byte[0],107,3);try{poll(c,n);throw new AssertionError();}catch(RuntimeException e){check(e==primary);}check(a.isDone()&&b.isDone()&&second[0]);try{c.shutdown();throw new AssertionError();}catch(MigrationContext.CleanupFailure expected){check(c.lastCleanup().completed&&m.closes.size()==2);}}
 static void testAllCloseOutcomesRetained()throws Exception{Model m=new Model();MigrationContext c=context(m,binding("/client","client","example_interfaces/srv/AddTwoInts"));client(c);m.closeError=110;try{c.shutdown();throw new AssertionError();}catch(MigrationContext.CleanupFailure e){int actual=0;for(MigrationContext.CleanupAttempt a:e.report.attempts)for(;a!=null;a=a.next)if(a.attempted){check(a.result.error==110);actual++;}check(actual==2&&m.closes.equals(Arrays.asList(2L,1L)));}}
 static void testNativeHeaderInjectionRefused()throws Exception{Model m=new Model();MigrationContext c=context(m,binding("/server","server","example_interfaces/srv/AddTwoInts"));Node n=c.createNode("server","");Service<?> s=n.createService(example_interfaces.srv.AddTwoInts.class,"add",(h,q,r)->{throw new AssertionError("invented request");});int calls=m.calls.size();code(95,()->s.executeCallback(new org.ros2.rcljava.service.RMWRequestId(),request(),new example_interfaces.srv.AddTwoInts_Response()));check(m.calls.size()==calls);c.shutdown();}
 static void testDriverReentryIsLeasedBeforeCredit()throws Exception{Model m=new Model();final Future<?>[] held={null};final MigrationContext[] selected={null};boolean[] called={false};Deployment.Driver driver=()->{try{code(17,()->held[0].get());code(17,()->selected[0].shutdown());called[0]=true;}catch(Exception e){throw new RuntimeException(e);}return false;};MigrationContext c=new MigrationContext(new Deployment.Profile(m,new Deployment.Binding[0],driver,4,16,8,2,new Deployment.ServiceBinding[]{binding("/client","client","example_interfaces/srv/AddTwoInts")}));selected[0]=c;c.init();held[0]=client(c).asyncSendRequest(request());try{held[0].get();throw new AssertionError();}catch(InterruptedException expected){check(called[0]&&!c.isDispatching()&&!held[0].isDone()&&m.count("event_poll_v1")==0);}c.shutdown();}
 public static void main(String[] args)throws Exception{testDriverReentryIsLeasedBeforeCredit();testWrongSourceRefusesAll();testDuplicateSequenceRefuses();testNoDriverGet95BeforePoll();testDriverExhaustionDoesNotInventCompletion();testSourceRetirementTriesEveryCallback();testAllCloseOutcomesRetained();testNativeHeaderInjectionRefused();testDefaultDeniedBeforeOpen();testDistinctBeforeEffects();testSecondOpenRollback();testWatchFailureClosesBoth();testActivationFailureClosesBoth();testActualAsyncAndCompleteResponse();testFiniteGetActuallyDrivesPoll();testAvailability95NoProbe();testTimedWaitNeverTreatsStepsAsNanoseconds();testFullWidthId();testOutOfOrderIndependentRequests();testCapacityBeforeThirdSubmit();testPerRequestErrorPreservesOther();testCloseFinishesAllWithoutNativeCancellation();testCallbackWholeContextLease();testReadyErrorGetInsideCallback();testMalformedCancelledTombstoneIsStillDecoded();testUnknownCorrelationFailsClosed();testCallbackThrowNoReplay();testCompleteServerAndAbsentRmwHeader();testCompleteTriggerAndSetBoolCodecs();System.out.println("PASS_RCLJAVA_SERVICE_LOCAL_MODELS groups=29 checks="+checks+" native=0");}
}
