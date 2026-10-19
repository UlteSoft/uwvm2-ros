import java.util.Arrays;
import org.uwvm.roswasm.generated.BoundedRecords.*;

/** Actual generated Java + real Codecs contract. Authored, not compiled/run.
 * No Transport, ROS import, native type identity or mock ROS success is used.
 */
public final class ManagedContract {
    private static int checks;
    private static void need(boolean value) {
        ++checks;
        if (!value) throw new AssertionError("managed complete-record contract");
    }
    private static void refuses(Runnable operation) {
        boolean rejected=false;
        try { operation.run(); } catch (IllegalArgumentException error) { rejected=true; }
        need(rejected);
    }
    private static byte[] hex(String text) {
        byte[] bytes=new byte[text.length()/2];
        for(int i=0;i<bytes.length;++i)bytes[i]=(byte)Integer.parseInt(text.substring(i*2,i*2+2),16);
        return bytes;
    }
    private static Ros2Managed_fixtureMsgScalars scalar() {
        Ros2Managed_fixtureMsgScalars value=new Ros2Managed_fixtureMsgScalars();
        value.flag=true;value.u8=255;value.i8=-128;value.u16=65535;value.i16=-32768;
        value.u32=0xffffffffL;value.i32=Integer.MIN_VALUE;
        value.u64=-1L;value.i64=Long.MIN_VALUE; // uint64 MAX retains every bit.
        value.f32=-0.0f;value.f64=1.25;value.text="π";
        return value;
    }
    private static void completeScalarGolden() {
        byte[] bytes=Ros2Managed_fixtureMsgScalars.CODEC.encode(scalar());
        // Independent literal: bool/u8/i8/u16/i16/u32/i32/u64/i64/f32/f64/u32/text.
        byte[] expected=hex("01ff80ffff0080ffffffff00000080ffffffffffffffff"+
            "000000000000008000000080000000000000f43f02000000cf80");
        need(Arrays.equals(bytes,expected));
        Ros2Managed_fixtureMsgScalars value=Ros2Managed_fixtureMsgScalars.CODEC.decode(expected);
        need(value.u64==-1L&&Long.toUnsignedString(value.u64).equals("18446744073709551615"));
        need(value.i64==Long.MIN_VALUE&&value.i32==Integer.MIN_VALUE);
        need(Float.floatToRawIntBits(value.f32)==0x80000000);
    }
    private static void unsignedRanges() {
        Ros2Managed_fixtureMsgScalars value=scalar();value.u8=256;
        refuses(()->Ros2Managed_fixtureMsgScalars.CODEC.encode(value));
        value.u8=0;value.u16=-1;refuses(()->Ros2Managed_fixtureMsgScalars.CODEC.encode(value));
        value.u16=0;value.u32=0x100000000L;refuses(()->Ros2Managed_fixtureMsgScalars.CODEC.encode(value));
    }
    private static void finiteAndSignedZero() {
        Ros2Managed_fixtureMsgScalars value=scalar();value.f32=Float.POSITIVE_INFINITY;
        refuses(()->Ros2Managed_fixtureMsgScalars.CODEC.encode(value));
        value.f32=-0.0f;value.f64=Double.NaN;
        refuses(()->Ros2Managed_fixtureMsgScalars.CODEC.encode(value));
        value.f64=-0.0;
        Ros2Managed_fixtureMsgScalars decoded=Ros2Managed_fixtureMsgScalars.CODEC.decode(Ros2Managed_fixtureMsgScalars.CODEC.encode(value));
        need(Double.doubleToRawLongBits(decoded.f64)==Long.MIN_VALUE);
    }
    private static void strictText() {
        Ros2Managed_fixtureMsgScalars value=scalar();value.text="🙂🙂🙂🙂";
        need(Ros2Managed_fixtureMsgScalars.CODEC.decode(Ros2Managed_fixtureMsgScalars.CODEC.encode(value)).text.equals(value.text));
        value.text+="🙂";refuses(()->Ros2Managed_fixtureMsgScalars.CODEC.encode(value));
        value.text="bad\0text";refuses(()->Ros2Managed_fixtureMsgScalars.CODEC.encode(value));
        value.text="\ud800";refuses(()->Ros2Managed_fixtureMsgScalars.CODEC.encode(value));
    }
    private static void exactFullDecode() {
        byte[] bytes=Ros2Managed_fixtureMsgScalars.CODEC.encode(scalar());
        refuses(()->Ros2Managed_fixtureMsgScalars.CODEC.decode(Arrays.copyOf(bytes,bytes.length-1)));
        refuses(()->Ros2Managed_fixtureMsgScalars.CODEC.decode(Arrays.copyOf(bytes,bytes.length+1)));
        byte[] bad=bytes.clone();bad[0]=2;
        refuses(()->Ros2Managed_fixtureMsgScalars.CODEC.decode(bad));
        bad[0]=1;bad[bad.length-2]=(byte)0xc0;bad[bad.length-1]=(byte)0x80;
        refuses(()->Ros2Managed_fixtureMsgScalars.CODEC.decode(bad));
    }
    private static void nestedArrays() {
        Ros2Managed_fixtureMsgOuter value=new Ros2Managed_fixtureMsgOuter();
        value.inner.scalar=scalar();value.fixed[0].tags=new String[]{"first","π"};
        value.fixed[1].pair=new double[]{3,-2};
        value.items=new Ros2Managed_fixtureMsgInner[]{new Ros2Managed_fixtureMsgInner()};
        value.items[0].scalar.i64=-7;
        Ros2Managed_fixtureMsgOuter decoded=Ros2Managed_fixtureMsgOuter.CODEC.decode(Ros2Managed_fixtureMsgOuter.CODEC.encode(value));
        need(decoded.inner.scalar.u64==-1L&&decoded.fixed[1].pair[1]==-2);
        need(decoded.fixed[0].tags[1].equals("π")&&decoded.items[0].scalar.i64==-7);
    }
    private static void arrayBounds() {
        Ros2Managed_fixtureMsgOuter value=new Ros2Managed_fixtureMsgOuter();value.fixed=new Ros2Managed_fixtureMsgInner[0];
        refuses(()->Ros2Managed_fixtureMsgOuter.CODEC.encode(value));
        value.fixed=new Ros2Managed_fixtureMsgOuter().fixed;
        value.items=new Ros2Managed_fixtureMsgInner[]{new Ros2Managed_fixtureMsgInner(),new Ros2Managed_fixtureMsgInner(),new Ros2Managed_fixtureMsgInner()};
        refuses(()->Ros2Managed_fixtureMsgOuter.CODEC.encode(value));
        value.items=new Ros2Managed_fixtureMsgInner[0];value.inner.tags[0]="🙂🙂🙂";
        refuses(()->Ros2Managed_fixtureMsgOuter.CODEC.encode(value));
    }
    private static void independentDefaults() {
        Ros2Managed_fixtureMsgOuter a=new Ros2Managed_fixtureMsgOuter(),b=new Ros2Managed_fixtureMsgOuter();
        need(a.rotation.w==1&&a.pose.orientation.w==1&&a.rotations[0].w==1&&a.rotations[1].w==1);
        need(a.dynamic.length==0&&a.items.length==0);
        a.rotations[0].w=0;a.fixed[0].pair[0]=9;
        need(a.rotations[1].w==1&&b.rotations[0].w==1&&b.fixed[0].pair[0]==0);
    }
    private static void noMissingDefaultWire() {
        Ros2Geometry_msgsMsgQuaternion value=new Ros2Geometry_msgsMsgQuaternion();value.w=0;
        byte[] bytes=Ros2Geometry_msgsMsgQuaternion.CODEC.encode(value);
        need(bytes.length==32&&Ros2Geometry_msgsMsgQuaternion.CODEC.decode(bytes).w==0);
        refuses(()->Ros2Geometry_msgsMsgQuaternion.CODEC.decode(Arrays.copyOf(bytes,31)));
    }
    private static void completeComponents() {
        need(Ros2Managed_fixtureSrvProbe_Request.CODEC.encode(new Ros2Managed_fixtureSrvProbe_Request()).length==0);
        Ros2Managed_fixtureSrvProbe_Request.CODEC.decode(new byte[0]);
        refuses(()->Ros2Managed_fixtureSrvProbe_Request.CODEC.decode(new byte[]{0}));
        Ros2Managed_fixtureActionTrajectory_Goal value=new Ros2Managed_fixtureActionTrajectory_Goal();value.step=17;
        need(Ros2Managed_fixtureActionTrajectory_Goal.CODEC.decode(Ros2Managed_fixtureActionTrajectory_Goal.CODEC.encode(value)).step==17);
    }
    private static void normalizedROS1Primitives() {
        Ros1Managed_fixtureTimes value=new Ros1Managed_fixtureTimes();value.stamp.nsec=1000000000L;
        refuses(()->Ros1Managed_fixtureTimes.CODEC.encode(value));
        value.stamp.nsec=999999999L;value.duration_value.sec=-1;value.duration_value.nsec=-1;
        refuses(()->Ros1Managed_fixtureTimes.CODEC.encode(value));
        value.duration_value.nsec=1;
        need(Ros1Managed_fixtureTimes.CODEC.decode(Ros1Managed_fixtureTimes.CODEC.encode(value)).duration_value.sec==-1);
    }
    private static void decodedInputOwnership() {
        byte[] bytes=Ros2Managed_fixtureMsgScalars.CODEC.encode(scalar());
        Ros2Managed_fixtureMsgScalars value=Ros2Managed_fixtureMsgScalars.CODEC.decode(bytes);
        Arrays.fill(bytes,(byte)0);
        need(value.u64==-1L&&value.text.equals("π"));
        need(Ros2Managed_fixtureMsgScalars.NATIVE_IDENTITY==null);
    }
    private static void fixedIndexFields() {
        Ros2Managed_fixtureMsgStringIndex strings=new Ros2Managed_fixtureMsgStringIndex();
        need(strings.i.length==2&&strings.i[0].equals("")&&strings.i[1].equals(""));
        strings.i[0]="a";strings.i[1]="π";
        byte[] expected=hex("010000006102000000cf80");
        need(Arrays.equals(Ros2Managed_fixtureMsgStringIndex.CODEC.encode(strings),expected));
        need(Ros2Managed_fixtureMsgStringIndex.CODEC.decode(expected).i[1].equals("π"));
        Ros2Managed_fixtureMsgRecordIndex records=new Ros2Managed_fixtureMsgRecordIndex();
        need(records.i.length==2&&records.i[0]!=null&&records.i[1]!=null&&records.i[0]!=records.i[1]);
        records.i[0].pair[0]=3;
        need(records.i[1].pair[0]==0);
        need(Ros2Managed_fixtureMsgRecordIndex.CODEC.decode(Ros2Managed_fixtureMsgRecordIndex.CODEC.encode(records)).i[0].pair[0]==3);
        strings.i[0]=null;refuses(()->Ros2Managed_fixtureMsgStringIndex.CODEC.encode(strings));
        records.i[1]=null;refuses(()->Ros2Managed_fixtureMsgRecordIndex.CODEC.encode(records));
    }
    private static void contextualFieldNames() {
        // JLS17 contextual type/declaration words remain legal field Identifiers.
        Ros2Managed_fixtureMsgClassScope value=new Ros2Managed_fixtureMsgClassScope();
        value.classmethod=17;value.list="a";value.str="π";value.values=new int[]{0,255};
        value.record=1;value.sealed=2;value.permits=3;
        byte[] expected=hex("11010000006102000000cf800200000000ff010203");
        need(Arrays.equals(Ros2Managed_fixtureMsgClassScope.CODEC.encode(value),expected));
        Ros2Managed_fixtureMsgClassScope result=Ros2Managed_fixtureMsgClassScope.CODEC.decode(expected);
        need(result.classmethod==17&&result.list.equals("a")&&result.str.equals("π"));
        need(result.record==1&&result.sealed==2&&result.permits==3);
    }
    public static void main(String[] args) {
        Runnable[] cases={ManagedContract::completeScalarGolden,ManagedContract::unsignedRanges,
            ManagedContract::finiteAndSignedZero,ManagedContract::strictText,ManagedContract::exactFullDecode,
            ManagedContract::nestedArrays,ManagedContract::arrayBounds,ManagedContract::independentDefaults,
            ManagedContract::noMissingDefaultWire,ManagedContract::completeComponents,
            ManagedContract::normalizedROS1Primitives,ManagedContract::decodedInputOwnership,
            ManagedContract::fixedIndexFields,ManagedContract::contextualFieldNames};
        for(Runnable test:cases)test.run();
        if(cases.length!=14)throw new AssertionError("complete named case inventory");
        System.out.println("managed Java record cases="+cases.length+" checks="+checks+" native/ROS=false");
    }
}
