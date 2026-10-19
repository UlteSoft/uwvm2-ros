package std_srvs.srv;
import org.ros2.rcljava.interfaces.MessageDefinition;
/** Complete original std_srvs/srv/SetBool Request fields, owning Java values.
 * Getters/setters preserve the official generated naming and native signedness.
 * No native converter address, guessed identity or projected field is supplied.
 */
public final class SetBool_Request implements MessageDefinition {
 private boolean data=false;
 public boolean getData() {return data;}
 public SetBool_Request setData(boolean value) {data=value;return this;}
 private UnsupportedOperationException nativePointer() {return new UnsupportedOperationException("native converters/type support do not exist in this Guest profile");}
 public static long getDestructor() {throw new UnsupportedOperationException("native destructor unavailable");}
 public static long getFromJavaConverter() {throw new UnsupportedOperationException("native converter unavailable");}
 public static long getToJavaConverter() {throw new UnsupportedOperationException("native converter unavailable");}
 public static long getTypeSupport() {throw new UnsupportedOperationException("native type support unavailable");}
 public long getFromJavaConverterInstance() {throw nativePointer();}
 public long getToJavaConverterInstance() {throw nativePointer();}
 public long getTypeSupportInstance() {throw nativePointer();}
 public long getDestructorInstance() {throw nativePointer();}
}
