package org.uwvm.rcljava.services.examples;
import org.ros2.rcljava.migration.Deployment;
import org.uwvm.roswasm.Ros2Transport;
/** Exact lesson selection, no implicit ROS/clock/discovery/policy permission. */
public final class SelectedServicesBootstrap {
 public static void main(String[] args)throws Exception{
  if(args.length!=2||(!args[0].equals("client")&&!args[0].equals("server")))throw new IllegalArgumentException("role/family");
  String family=args[1],type,resource;
  if(family.equals("add")){type="example_interfaces/srv/AddTwoInts";resource="/add_two_ints";}
  else if(family.equals("trigger")){type="std_srvs/srv/Trigger";resource="/rcljava_trigger";}
  else if(family.equals("setbool")){type="std_srvs/srv/SetBool";resource="/rcljava_set_bool";}
  else throw new IllegalArgumentException("family");
  boolean client=args[0].equals("client");
  Deployment.ServiceBinding row=new Deployment.ServiceBinding("/lesson_"+family+"_"+args[0],resource,type,"official-default-service-profile",args[0],client?"call":"advertise",client?"observe":"respond",4096,2);
  Deployment.install(new Deployment.Profile(new Ros2Transport(),new Deployment.Binding[0],new Deployment.StdinCredits(System.in,64),8,16,8,64,new Deployment.ServiceBinding[]{row}));
  Throwable primary=null;try{SelectedServices.main(args);}catch(Exception|Error failure){primary=failure;throw failure;}finally{Deployment.shutdown(primary);}
 }
}
