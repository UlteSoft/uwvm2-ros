package std_msgs.msg;
import org.ros2.rcljava.interfaces.MessageDefinition;
/** Complete std_msgs/String field API observed in the pinned official examples.
 * Native conversion/type-support addresses do not exist in a Wasm capability
 * SDK. They throw rather than returning zero/fabricated JNI pointers. Native
 * type identity is still independently checked by the granted Host/Gateway.
 */
public final class String implements MessageDefinition {
    private java.lang.String data = "";
    public java.lang.String getData() { return data; }
    public void setData(java.lang.String value) { if(value==null)throw new IllegalArgumentException("String.data");data=value; }
    private UnsupportedOperationException unavailable() { return new UnsupportedOperationException("JNI/native type-support pointers are unavailable in Wasm"); }
    public long getFromJavaConverterInstance() { throw unavailable(); }
    public long getToJavaConverterInstance() { throw unavailable(); }
    public long getTypeSupportInstance() { throw unavailable(); }
    public long getDestructorInstance() { throw unavailable(); }
}
