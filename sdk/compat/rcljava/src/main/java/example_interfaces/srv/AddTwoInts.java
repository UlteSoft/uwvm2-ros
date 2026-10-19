package example_interfaces.srv;
import org.ros2.rcljava.interfaces.ServiceDefinition;
/** Complete service role names from the pinned official srv.java.em template.
 * This class never loads JNI or fabricates a native type-support pointer.
 * Its actual native identity still belongs to an independently bound Host alias.
 */
public final class AddTwoInts implements ServiceDefinition {
 public static final Class<AddTwoInts_Request> RequestType=AddTwoInts_Request.class;
 public static final Class<AddTwoInts_Response> ResponseType=AddTwoInts_Response.class;
 public static long getServiceTypeSupport() { throw new UnsupportedOperationException("native service type support is not a Wasm address"); }
}
