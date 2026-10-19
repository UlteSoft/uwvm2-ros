package org.uwvm.rcljava.services.examples;
import java.util.concurrent.Future;
import org.ros2.rcljava.RCLJava;
import org.ros2.rcljava.node.Node;
import org.ros2.rcljava.service.RMWRequestId;
import org.ros2.rcljava.client.Client;
/** Concrete complete three-family lessons. The client lesson has an explicit
 * independently established native-ready precondition. It does not bypass or
 * implement the still-refused official client's waitForService() call.
 */
public final class SelectedServices {
 public static void main(String[] args)throws Exception{
  if(args.length!=2||(!args[0].equals("client")&&!args[0].equals("server")))throw new IllegalArgumentException("role/family");
  boolean client=args[0].equals("client");String family=args[1];
  if(!family.equals("add")&&!family.equals("trigger")&&!family.equals("setbool"))throw new IllegalArgumentException("family");
  RCLJava.rclJavaInit();Node node=RCLJava.createNode("lesson_"+family+"_"+args[0]);
  String resource=family.equals("add")?"/add_two_ints":family.equals("trigger")?"/rcljava_trigger":"/rcljava_set_bool";
  if(!client){
   if(family.equals("add"))node.createService(example_interfaces.srv.AddTwoInts.class,resource,(RMWRequestId h,example_interfaces.srv.AddTwoInts_Request q,example_interfaces.srv.AddTwoInts_Response r)->{if(h!=null||q.getA()!=41||q.getB()!=1)throw new IllegalArgumentException("actual complete request/header");r.setSum(42);});
   else if(family.equals("trigger"))node.createService(std_srvs.srv.Trigger.class,resource,(RMWRequestId h,std_srvs.srv.Trigger_Request q,std_srvs.srv.Trigger_Response r)->{if(h!=null)throw new IllegalArgumentException("no native header is available");r.setSuccess(true).setMessage("ready");});
   else node.createService(std_srvs.srv.SetBool.class,resource,(RMWRequestId h,std_srvs.srv.SetBool_Request q,std_srvs.srv.SetBool_Response r)->{if(h!=null||!q.getData())throw new IllegalArgumentException("complete request");r.setSuccess(true).setMessage("accepted");});
   RCLJava.spin(node);
  }else{
   if(family.equals("add")){Client<example_interfaces.srv.AddTwoInts> c=node.createClient(example_interfaces.srv.AddTwoInts.class,resource);Future<example_interfaces.srv.AddTwoInts_Response> f=c.asyncSendRequest(new example_interfaces.srv.AddTwoInts_Request().setA(41).setB(1));if(f.get().getSum()!=42)throw new AssertionError("complete native sum");}
   else if(family.equals("trigger")){Client<std_srvs.srv.Trigger> c=node.createClient(std_srvs.srv.Trigger.class,resource);Future<std_srvs.srv.Trigger_Response> f=c.asyncSendRequest(new std_srvs.srv.Trigger_Request());std_srvs.srv.Trigger_Response r=f.get();if(!r.getSuccess()||!r.getMessage().equals("ready"))throw new AssertionError("complete native Trigger response");}
   else{Client<std_srvs.srv.SetBool> c=node.createClient(std_srvs.srv.SetBool.class,resource);Future<std_srvs.srv.SetBool_Response> f=c.asyncSendRequest(new std_srvs.srv.SetBool_Request().setData(true));std_srvs.srv.SetBool_Response r=f.get();if(!r.getSuccess()||!r.getMessage().equals("accepted"))throw new AssertionError("complete native SetBool response");}
   System.out.println("COMPLETE_RCLJAVA_SERVICE_RESPONSE "+family);
  }
  RCLJava.shutdown();
 }
}
