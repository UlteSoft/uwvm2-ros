package org.uwvm.rcljava.availability.examples;
import org.ros2.rcljava.migration.Deployment;
import org.uwvm.roswasm.Ros2ServiceClientTransport;

/** Explicit optional selection before the byte-exact original client main.
 * The launcher does not install Host grants. Root must independently grant the
 * exact call, event.observe response/error/closed source, service.available
 * paired source and steady Clock, with actual native identity/codec/QoS.
 * No graph, request, ambient WASI Clock or JVM/JNI substitutes for readiness.
 * Source-only: the current JAR producer's optional-namespace ceiling requires a
 * separate HLL review before this entry can be produced/admitted as a Guest.
 */
public final class OriginalClientAvailabilityBootstrap {
 public static void main(String[] args)throws Exception{
  if(args.length!=0)throw new IllegalArgumentException("no ambient override");
  Deployment.AvailabilityBinding availability=new Deployment.AvailabilityBinding("available","steady",10000000L,65536,null);
  Deployment.ServiceBinding client=new Deployment.ServiceBinding("/minimal_client","/add_two_ints","example_interfaces/srv/AddTwoInts","official-default-service-profile","client","call","observe",4096,2,availability);
  Deployment.install(new Deployment.Profile(new Ros2ServiceClientTransport(),new Deployment.Binding[0],new Deployment.StdinCredits(System.in,64),8,16,8,64,new Deployment.ServiceBinding[]{client}));
  Throwable primary=null;
  try{org.ros2.rcljava.examples.client.AddTwoIntsClient.main(new String[0]);}
  catch(Exception|Error failure){primary=failure;throw failure;}
  finally{Deployment.shutdown(primary);}
 }
}
