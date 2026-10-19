package org.uwvm.rcljava.services.examples;
import org.ros2.rcljava.RCLJava;
import org.ros2.rcljava.migration.Deployment;
import org.uwvm.roswasm.Ros2Transport;
import org.uwvm.roswasm.RosException;
/** A present exact local selection plus EMPTY Host policy must still deny the
 * actual first open13. A missing local map13 is not this Host-denial witness.
 */
public final class DefaultDeniedServiceBootstrap {
 public static void main(String[] args)throws Exception{
  if(args.length!=0)throw new IllegalArgumentException("no args");
  Deployment.ServiceBinding row=new Deployment.ServiceBinding("/minimal_service","/add_two_ints","example_interfaces/srv/AddTwoInts","official-default-service-profile","server","advertise","respond",4096,2);
  Deployment.install(new Deployment.Profile(new Ros2Transport(),new Deployment.Binding[0],null,8,16,8,0,new Deployment.ServiceBinding[]{row}));
  Throwable primary=null;
  try{RCLJava.rclJavaInit();RCLJava.createNode("minimal_service").createService(example_interfaces.srv.AddTwoInts.class,"add_two_ints",(h,q,r)->{});throw new AssertionError("Host policy unexpectedly admitted service");}
  catch(RosException denied){if(denied.result.error!=13){primary=denied;throw denied;}System.out.println("PASS_RCLJAVA_SERVICE_HOST_DEFAULT_DENIED_ERRNO13");}
  catch(Exception|Error failure){primary=failure;throw failure;}
  finally{Deployment.shutdown(primary);}
 }
}
