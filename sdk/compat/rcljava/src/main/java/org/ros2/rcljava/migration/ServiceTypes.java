package org.ros2.rcljava.migration;
import org.ros2.rcljava.interfaces.MessageDefinition;
import org.ros2.rcljava.interfaces.ServiceDefinition;
import org.uwvm.roswasm.Codecs;

/** Closed complete service value codecs. No JNI reflection, native provider,
 * computed MD5/RIHS or schema projection occurs here. The Host independently
 * binds each native identity/codec/resource/QoS/limit to the selected alias.
 */
public final class ServiceTypes {
 private ServiceTypes() {}
 public abstract static class Descriptor {
  public final String name;public final Class<? extends MessageDefinition> request,response;
  Descriptor(String name,Class<? extends MessageDefinition> request,Class<? extends MessageDefinition> response){this.name=name;this.request=request;this.response=response;}
  public abstract byte[] encodeRequest(MessageDefinition value);
  public abstract MessageDefinition decodeRequest(byte[] bytes);
  public abstract MessageDefinition newResponse();
  public abstract byte[] encodeResponse(MessageDefinition value);
  public abstract MessageDefinition decodeResponse(byte[] bytes);
 }
 private static final Descriptor ADD=new Descriptor("example_interfaces/srv/AddTwoInts",example_interfaces.srv.AddTwoInts_Request.class,example_interfaces.srv.AddTwoInts_Response.class){
  public byte[] encodeRequest(MessageDefinition value){example_interfaces.srv.AddTwoInts_Request q=(example_interfaces.srv.AddTwoInts_Request)value;return new Codecs.Writer(16).integer(q.getA(),8,true).integer(q.getB(),8,true).finish();}
  public MessageDefinition decodeRequest(byte[] bytes){Codecs.Reader r=new Codecs.Reader(bytes);long a=r.unsigned(8),b=r.unsigned(8);r.end();example_interfaces.srv.AddTwoInts_Request q=new example_interfaces.srv.AddTwoInts_Request();q.setA(a);q.setB(b);return q;}
  public MessageDefinition newResponse(){return new example_interfaces.srv.AddTwoInts_Response();}
  public byte[] encodeResponse(MessageDefinition value){return Codecs.INT64.encode(((example_interfaces.srv.AddTwoInts_Response)value).getSum());}
  public MessageDefinition decodeResponse(byte[] bytes){long n=Codecs.INT64.decode(bytes);example_interfaces.srv.AddTwoInts_Response r=new example_interfaces.srv.AddTwoInts_Response();r.setSum(n);return r;}
 };
 private static final Descriptor TRIGGER=new Descriptor("std_srvs/srv/Trigger",std_srvs.srv.Trigger_Request.class,std_srvs.srv.Trigger_Response.class){
  public byte[] encodeRequest(MessageDefinition value){if(!(value instanceof std_srvs.srv.Trigger_Request))throw new IllegalArgumentException("Trigger request");return new byte[0];}
  public MessageDefinition decodeRequest(byte[] bytes){Codecs.TRIGGER.decode(bytes);return new std_srvs.srv.Trigger_Request();}
  public MessageDefinition newResponse(){return new std_srvs.srv.Trigger_Response();}
  public byte[] encodeResponse(MessageDefinition value){std_srvs.srv.Trigger_Response r=(std_srvs.srv.Trigger_Response)value;return Codecs.SERVICE_RESPONSE.encode(new Codecs.Response(r.getSuccess(),r.getMessage()));}
  public MessageDefinition decodeResponse(byte[] bytes){Codecs.Response data=Codecs.SERVICE_RESPONSE.decode(bytes);std_srvs.srv.Trigger_Response r=new std_srvs.srv.Trigger_Response();r.setSuccess(data.success);r.setMessage(data.message);return r;}
 };
 private static final Descriptor SETBOOL=new Descriptor("std_srvs/srv/SetBool",std_srvs.srv.SetBool_Request.class,std_srvs.srv.SetBool_Response.class){
  public byte[] encodeRequest(MessageDefinition value){return Codecs.BOOL.encode(((std_srvs.srv.SetBool_Request)value).getData());}
  public MessageDefinition decodeRequest(byte[] bytes){boolean value=Codecs.BOOL.decode(bytes);std_srvs.srv.SetBool_Request r=new std_srvs.srv.SetBool_Request();r.setData(value);return r;}
  public MessageDefinition newResponse(){return new std_srvs.srv.SetBool_Response();}
  public byte[] encodeResponse(MessageDefinition value){std_srvs.srv.SetBool_Response r=(std_srvs.srv.SetBool_Response)value;return Codecs.SERVICE_RESPONSE.encode(new Codecs.Response(r.getSuccess(),r.getMessage()));}
  public MessageDefinition decodeResponse(byte[] bytes){Codecs.Response data=Codecs.SERVICE_RESPONSE.decode(bytes);std_srvs.srv.SetBool_Response r=new std_srvs.srv.SetBool_Response();r.setSuccess(data.success);r.setMessage(data.message);return r;}
 };
 public static Descriptor find(Class<? extends ServiceDefinition> type){if(type==example_interfaces.srv.AddTwoInts.class)return ADD;if(type==std_srvs.srv.Trigger.class)return TRIGGER;if(type==std_srvs.srv.SetBool.class)return SETBOOL;throw new org.uwvm.roswasm.RosException(new org.uwvm.roswasm.Result(95));}
 public static boolean known(String name){return ADD.name.equals(name)||TRIGGER.name.equals(name)||SETBOOL.name.equals(name);}
}
