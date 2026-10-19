import java.lang.reflect.Field;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.Arrays;
import org.uwvm.roswasm.Codec;
import org.uwvm.roswasm.generated.BoundedRecords;

/** JVM-only full-value contract against one ACTUALLY generated catalog.
 * Reflection selects only the six literal family/version roles below, never
 * native type support or ROS authority. Not a TeaVM reflection support claim.
 */
public final class FamilyContract {
    private static int checks;
    private static void need(boolean value) {++checks;if(!value)throw new AssertionError("complete family codec");}
    private static Field field(Object value,String name) throws ReflectiveOperationException {return value.getClass().getField(name);}
    private static Object get(Object value,String name) throws ReflectiveOperationException {return field(value,name).get(value);}
    private static void put(Object value,String name,Object item) throws ReflectiveOperationException {field(value,name).set(value,item);}
    private static void number(Object value,String name,long item) throws ReflectiveOperationException {
        Field f=field(value,name);Class<?> type=f.getType();
        if(type==short.class)f.setShort(value,(short)item);
        else if(type==int.class)f.setInt(value,(int)item);
        else if(type==long.class)f.setLong(value,item);
        else throw new AssertionError("actual generated integer field type");
    }
    @SuppressWarnings("unchecked")
    private static Codec<Object> codec(Class<?> type) throws ReflectiveOperationException {return (Codec<Object>)type.getField("CODEC").get(null);}
    private static void header(Object value,int version) throws ReflectiveOperationException {
        Object h=get(value,"header"),stamp=get(h,"stamp");
        if(version==1)number(h,"seq",77);
        number(stamp,"sec",3);number(stamp,version==1?"nsec":"nanosec",4);put(h,"frame_id","robot");
    }
    private static ByteBuffer headerWire(int version) {
        ByteBuffer b=ByteBuffer.allocate(2048).order(ByteOrder.LITTLE_ENDIAN);
        if(version==1)b.putInt(77);
        b.putInt(3).putInt(4).putInt(5).put(new byte[]{'r','o','b','o','t'});
        return b;
    }
    private static byte[] complete(ByteBuffer b) {return Arrays.copyOf(b.array(),b.position());}
    private static void robot(Object value,ByteBuffer b) throws ReflectiveOperationException {
        String[] names={"state","mode","cmdnum","mt_brake","mt_able","err","warn"};
        short[] numbers={1,2,3,4,5,-6,7};
        for(int i=0;i<names.length;++i){number(value,names[i],numbers[i]);b.putShort(numbers[i]);}
        float[] angles=new float[7],pose=new float[6],offset=new float[6];
        b.putInt(7);for(int i=0;i<7;++i){angles[i]=i/2.0f;b.putFloat(angles[i]);}
        for(int i=0;i<6;++i){pose[i]=-i-1;b.putFloat(pose[i]);}
        for(int i=0;i<6;++i){offset[i]=i+10;b.putFloat(offset[i]);}
        put(value,"angle",angles);put(value,"pose",pose);put(value,"offset",offset);
    }
    private static void cio(Object value,ByteBuffer b) throws ReflectiveOperationException {
        number(value,"state",-1);number(value,"code",2);b.putShort((short)-1).putShort((short)2);
        put(value,"input_digitals",new int[]{0,65535});put(value,"output_digitals",new int[]{42,32768});
        b.putShort((short)0).putShort((short)65535).putShort((short)42).putShort((short)32768);
        put(value,"input_analogs",new float[]{-0.0f,1.25f});put(value,"output_analogs",new float[]{2.5f,-3.5f});
        b.putFloat(-0.0f).putFloat(1.25f).putFloat(2.5f).putFloat(-3.5f);
        int[] inputs=new int[16],outputs=new int[16];
        for(int i=0;i<16;++i){inputs[i]=i;b.put((byte)i);}
        for(int i=0;i<16;++i){outputs[i]=240+i;b.put((byte)(240+i));}
        put(value,"input_conf",inputs);put(value,"output_conf",outputs);
    }
    private static void joint(Object value,ByteBuffer b) throws ReflectiveOperationException {
        put(value,"name",new String[]{"joint1"});put(value,"position",new double[]{1.25});
        put(value,"velocity",new double[]{0.5});put(value,"effort",new double[]{-0.75});
        b.putInt(1).putInt(6).put(new byte[]{'j','o','i','n','t','1'});
        b.putInt(1).putDouble(1.25).putInt(1).putDouble(0.5).putInt(1).putDouble(-0.75);
    }
    public static void main(String[] args) throws ReflectiveOperationException {
        if(args.length!=1||!Arrays.asList("robot1","robot2","cio1","cio2","joint1","joint2").contains(args[0]))
            throw new IllegalArgumentException("one literal family/version role required");
        int version=args[0].endsWith("1")?1:2;
        String leaf=args[0].startsWith("robot")?"RobotMsg":args[0].startsWith("cio")?"CIOState":"JointState";
        String nativeType=(leaf.equals("JointState")?"sensor_msgs/":"xarm_msgs/")+(version==2?"msg/":"")+leaf;
        Class<?> selected=null;
        for(Class<?> type:BoundedRecords.class.getDeclaredClasses()) {
            if(type.getField("NATIVE_TYPE").get(null).equals(nativeType)) {
                need(selected==null);selected=type;
            }
        }
        need(selected!=null);
        Object value=selected.getConstructor().newInstance();header(value,version);ByteBuffer b=headerWire(version);
        if(leaf.equals("RobotMsg"))robot(value,b);else if(leaf.equals("CIOState"))cio(value,b);else joint(value,b);
        Codec<Object> codec=codec(selected);byte[] expected=complete(b),wire=codec.encode(value);
        need(Arrays.equals(wire,expected));
        Object decoded=codec.decode(expected);need(Arrays.equals(codec.encode(decoded),expected));
        need(selected.getField("NATIVE_IDENTITY").get(null)==null);
        System.out.println("managed Java family="+args[0]+" full-wire-bytes="+wire.length+" checks="+checks+" native/ROS=false");
    }
}
