package std_srvs.srv;
import org.ros2.rcljava.interfaces.ServiceDefinition;
/** Complete service role names from the pinned official srv.java.em template.
 * This class never loads JNI or fabricates a native type-support pointer.
 * Its actual native identity still belongs to an independently bound Host alias.
 */
public final class SetBool implements ServiceDefinition {
 public static final Class<SetBool_Request> RequestType=SetBool_Request.class;
 public static final Class<SetBool_Response> ResponseType=SetBool_Response.class;
 public static long getServiceTypeSupport() { throw new UnsupportedOperationException("native service type support is not a Wasm address"); }
}
