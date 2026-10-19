package org.uwvm.rcljava.services.examples;
import org.ros2.rcljava.RCLJava;
import org.ros2.rcljava.migration.Deployment;
import org.uwvm.roswasm.Ros2Transport;
/** Independent deployment selection before the byte-exact upstream main.
 * It installs no policy and never changes business source. Root must bind each
 * alias to the actual native type/complete codec/QoS/resource/quota contract.
 */
public final class OfficialServiceBootstrap {
 public static void main(String[] args)throws Exception{
  if(args.length!=0)throw new IllegalArgumentException("no ambient deployment override");
  Deployment.ServiceBinding row=new Deployment.ServiceBinding("/minimal_service","/add_two_ints","example_interfaces/srv/AddTwoInts","official-default-service-profile","server","advertise","respond",4096,2);
  Deployment.install(new Deployment.Profile(new Ros2Transport(),new Deployment.Binding[0],new Deployment.StdinCredits(System.in,64),8,16,8,64,new Deployment.ServiceBinding[]{row}));
  Throwable primary=null;
  try{org.ros2.rcljava.examples.service.AddTwoIntsService.main(new String[0]);}
  catch(Exception|Error failure){primary=failure;throw failure;}
  finally{Deployment.shutdown(primary);}
 }
}
