package org.uwvm.rcljava.services;
import org.ros2.rcljava.migration.Deployment;
import org.uwvm.roswasm.Codecs;
/** Execute only when Root compiles copied inputs. It calls the entire byte-exact
 * original server main in a fresh JVM with a labelled local Transport model;
 * it establishes no native ROS/Host/Wasm/availability result.
 */
public final class OfficialServiceSourceTest {
 public static void main(String[] args)throws Exception{
  ServicePolicyTest.Model model=new ServicePolicyTest.Model();int[] credits={0};
  Deployment.ServiceBinding binding=new Deployment.ServiceBinding("/minimal_service","/add_two_ints","example_interfaces/srv/AddTwoInts","official-default-service-profile","server","advertise","respond",4096,2);
  Deployment.install(new Deployment.Profile(model,new Deployment.Binding[0],()->credits[0]++==0,8,16,8,4,new Deployment.ServiceBinding[]{binding}));
  model.request=Codecs.prefixId(-7L,new Codecs.Writer(16).integer(2,8,true).integer(3,8,true).finish());
  Throwable primary=null;
  try{
   org.ros2.rcljava.examples.service.AddTwoIntsService.main(new String[0]);
   Codecs.Reader wire=new Codecs.Reader(model.response);
   if(wire.unsigned(8)!=-7L||wire.unsigned(8)!=5L)throw new AssertionError("complete original source response");wire.end();
   if(model.count("service_respond")!=1)throw new AssertionError("no replay");
  }catch(Exception|Error failure){primary=failure;throw failure;}finally{Deployment.shutdown(primary);}
  if(model.closes.size()!=2)throw new AssertionError("both actual model owners close once");
  System.out.println("PASS_RCLJAVA_UNCHANGED_OFFICIAL_SERVER_LOCAL_MODEL native=0");
 }
}
