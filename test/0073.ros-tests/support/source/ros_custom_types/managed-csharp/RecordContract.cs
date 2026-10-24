#nullable enable
using System;
using System.IO;
using static Uwvm.RosWasm.Generated.BoundedRecords;

// Real generated typed codecs + actual SDK Codecs/ICodec only. No reflection,
// transport, Runtime, generated native identity, native ROS or model backend.
internal static class RecordContract {
    private static int checks;
    private static void Need(bool value) { ++checks; if(!value)throw new Exception("complete C# record contract"); }
    private static void Refuses(Action operation) {
        bool refused=false;
        try {operation();} catch(ArgumentException) {refused=true;} catch(FormatException) {refused=true;}
        Need(refused);
    }
    private static bool Equal(byte[] a,byte[] b) { if(a.Length!=b.Length)return false;for(int i=0;i<a.Length;i++)if(a[i]!=b[i])return false;return true; }
    private static byte[] Hex(string value) { byte[] bytes=new byte[value.Length/2];for(int i=0;i<bytes.Length;i++)bytes[i]=Convert.ToByte(value.Substring(i*2,2),16);return bytes; }
    private static _Ros2Managed_fixtureMsgScalars Scalar() => new() {
        @flag=true,@u8=255,@i8=-128,@u16=65535,@i16=-32768,
        @u32=uint.MaxValue,@i32=int.MinValue,@u64=ulong.MaxValue,@i64=long.MinValue,
        @f32=BitConverter.Int32BitsToSingle(unchecked((int)0x80000000)),@f64=1.25,@text="π"
    };
    private static void CompleteScalarGolden() {
        byte[] expected=Hex("01ff80ffff0080ffffffff00000080ffffffffffffffff"+
            "000000000000008000000080000000000000f43f02000000cf80");
        Need(Equal(Ros2Managed_fixtureMsgScalarsCodec.Instance.Encode(Scalar()),expected));
        var value=Ros2Managed_fixtureMsgScalarsCodec.Instance.Decode(expected);
        Need(value.@u64==ulong.MaxValue&&value.@i64==long.MinValue);
        Need(value.@u32==uint.MaxValue&&value.@i32==int.MinValue);
        Need(BitConverter.SingleToInt32Bits(value.@f32)==unchecked((int)0x80000000));
    }
    private static void SignedWidthBits() {
        var value=Scalar();value.@i8=-1;value.@i16=-1;value.@i32=-1;value.@i64=-1;
        byte[] wire=Ros2Managed_fixtureMsgScalarsCodec.Instance.Encode(value);
        var result=Ros2Managed_fixtureMsgScalarsCodec.Instance.Decode(wire);
        Need(result.@i8==-1&&result.@i16==-1&&result.@i32==-1&&result.@i64==-1);
        // Independent positions in the fixed scalar prefix, not a C# layout cast.
        Need(wire[2]==255&&wire[5]==255&&wire[6]==255);
        for(int i=11;i<15;i++)Need(wire[i]==255);
        for(int i=23;i<31;i++)Need(wire[i]==255);
    }
    private static void CompleteUnsigned64() {
        var value=Scalar();value.@u64=1UL<<63;
        byte[] wire=Ros2Managed_fixtureMsgScalarsCodec.Instance.Encode(value);
        Need(Ros2Managed_fixtureMsgScalarsCodec.Instance.Decode(wire).@u64==(1UL<<63));
        value.@u64=ulong.MaxValue;
        Need(Ros2Managed_fixtureMsgScalarsCodec.Instance.Decode(Ros2Managed_fixtureMsgScalarsCodec.Instance.Encode(value)).@u64==ulong.MaxValue);
    }
    private static void FiniteAndSignedZero() {
        var value=Scalar();value.@f32=float.PositiveInfinity;
        Refuses(()=>Ros2Managed_fixtureMsgScalarsCodec.Instance.Encode(value));
        value.@f32=0;value.@f64=double.NaN;
        Refuses(()=>Ros2Managed_fixtureMsgScalarsCodec.Instance.Encode(value));
        value.@f64=BitConverter.Int64BitsToDouble(long.MinValue);
        var result=Ros2Managed_fixtureMsgScalarsCodec.Instance.Decode(Ros2Managed_fixtureMsgScalarsCodec.Instance.Encode(value));
        Need(BitConverter.DoubleToInt64Bits(result.@f64)==long.MinValue);
    }
    private static void StrictText() {
        var value=Scalar();value.@text="🙂🙂🙂🙂";
        Need(Ros2Managed_fixtureMsgScalarsCodec.Instance.Decode(Ros2Managed_fixtureMsgScalarsCodec.Instance.Encode(value)).@text==value.@text);
        value.@text+="🙂";Refuses(()=>Ros2Managed_fixtureMsgScalarsCodec.Instance.Encode(value));
        value.@text="bad\0text";Refuses(()=>Ros2Managed_fixtureMsgScalarsCodec.Instance.Encode(value));
        value.@text="\ud800";Refuses(()=>Ros2Managed_fixtureMsgScalarsCodec.Instance.Encode(value));
    }
    private static void ExactDecode() {
        byte[] wire=Ros2Managed_fixtureMsgScalarsCodec.Instance.Encode(Scalar());
        byte[] shortWire=new byte[wire.Length-1];Array.Copy(wire,shortWire,shortWire.Length);
        Refuses(()=>Ros2Managed_fixtureMsgScalarsCodec.Instance.Decode(shortWire));
        byte[] longWire=new byte[wire.Length+1];Array.Copy(wire,longWire,wire.Length);
        Refuses(()=>Ros2Managed_fixtureMsgScalarsCodec.Instance.Decode(longWire));
        byte[] bad=(byte[])wire.Clone();bad[0]=2;Refuses(()=>Ros2Managed_fixtureMsgScalarsCodec.Instance.Decode(bad));
        bad[0]=1;bad[^2]=0xc0;bad[^1]=0x80;Refuses(()=>Ros2Managed_fixtureMsgScalarsCodec.Instance.Decode(bad));
    }
    private static void NestedArrays() {
        var value=new _Ros2Managed_fixtureMsgOuter();value.@inner.@scalar=Scalar();
        value.@fixed[0].@tags=new[]{"first","π"};value.@fixed[1].@pair=new[]{3.0,-2.0};
        value.@items=new[]{new _Ros2Managed_fixtureMsgInner()};value.@items[0].@scalar.@i64=-7;
        var result=Ros2Managed_fixtureMsgOuterCodec.Instance.Decode(Ros2Managed_fixtureMsgOuterCodec.Instance.Encode(value));
        Need(result.@inner.@scalar.@u64==ulong.MaxValue&&result.@fixed[1].@pair[1]==-2);
        Need(result.@fixed[0].@tags[1]=="π"&&result.@items[0].@scalar.@i64==-7);
    }
    private static void ArrayBounds() {
        var value=new _Ros2Managed_fixtureMsgOuter();value.@fixed=Array.Empty<_Ros2Managed_fixtureMsgInner>();
        Refuses(()=>Ros2Managed_fixtureMsgOuterCodec.Instance.Encode(value));
        value=new _Ros2Managed_fixtureMsgOuter();
        value.@items=new[]{new _Ros2Managed_fixtureMsgInner(),new _Ros2Managed_fixtureMsgInner(),new _Ros2Managed_fixtureMsgInner()};
        Refuses(()=>Ros2Managed_fixtureMsgOuterCodec.Instance.Encode(value));
        value.@items=Array.Empty<_Ros2Managed_fixtureMsgInner>();value.@inner.@tags[0]="🙂🙂🙂";
        Refuses(()=>Ros2Managed_fixtureMsgOuterCodec.Instance.Encode(value));
        value.@inner.@tags[0]="";value.@fixed[1]=null!;
        Refuses(()=>Ros2Managed_fixtureMsgOuterCodec.Instance.Encode(value));
    }
    private static void IndependentDefaults() {
        var a=new _Ros2Managed_fixtureMsgOuter();var b=new _Ros2Managed_fixtureMsgOuter();
        Need(a.@rotation.@w==1&&a.@pose.@orientation.@w==1&&a.@rotations[0].@w==1&&a.@rotations[1].@w==1);
        Need(a.@dynamic.Length==0&&a.@items.Length==0);
        a.@rotations[0].@w=0;a.@fixed[0].@pair[0]=9;
        Need(a.@rotations[1].@w==1&&b.@rotations[0].@w==1&&b.@fixed[0].@pair[0]==0);
    }
    private static void NoMissingDefaultWire() {
        var value=new _Ros2Geometry_msgsMsgQuaternion();value.@w=0;
        byte[] wire=Ros2Geometry_msgsMsgQuaternionCodec.Instance.Encode(value);
        Need(wire.Length==32&&Ros2Geometry_msgsMsgQuaternionCodec.Instance.Decode(wire).@w==0);
        Refuses(()=>Ros2Geometry_msgsMsgQuaternionCodec.Instance.Decode(new byte[31]));
    }
    private static void CompleteComponents() {
        Need(Ros2Managed_fixtureSrvProbe_RequestCodec.Instance.Encode(new _Ros2Managed_fixtureSrvProbe_Request()).Length==0);
        Ros2Managed_fixtureSrvProbe_RequestCodec.Instance.Decode(Array.Empty<byte>());
        Refuses(()=>Ros2Managed_fixtureSrvProbe_RequestCodec.Instance.Decode(new byte[1]));
        var value=new _Ros2Managed_fixtureActionTrajectory_Goal();value.@step=17;
        Need(Ros2Managed_fixtureActionTrajectory_GoalCodec.Instance.Decode(Ros2Managed_fixtureActionTrajectory_GoalCodec.Instance.Encode(value)).@step==17);
    }
    private static void NormalizedROS1Primitives() {
        var value=new _Ros1Managed_fixtureTimes();value.@stamp.@nsec=1000000000;
        Refuses(()=>Ros1Managed_fixtureTimesCodec.Instance.Encode(value));
        value.@stamp.@nsec=999999999;value.@duration_value.@sec=-1;value.@duration_value.@nsec=-1;
        Refuses(()=>Ros1Managed_fixtureTimesCodec.Instance.Encode(value));
        value.@duration_value.@nsec=1;
        Need(Ros1Managed_fixtureTimesCodec.Instance.Decode(Ros1Managed_fixtureTimesCodec.Instance.Encode(value)).@duration_value.@sec==-1);
    }
    private static void InputOwnership() {
        byte[] wire=Ros2Managed_fixtureMsgScalarsCodec.Instance.Encode(Scalar());
        var value=Ros2Managed_fixtureMsgScalarsCodec.Instance.Decode(wire);Array.Clear(wire,0,wire.Length);
        Need(value.@u64==ulong.MaxValue&&value.@text=="π");
        Need(Ros2Managed_fixtureMsgScalarsCodec.NATIVE_IDENTITY==null);
    }
    private static void FixedIndexFields() {
        var strings=new _Ros2Managed_fixtureMsgStringIndex();Need(strings.@i.Length==2&&strings.@i[0]==""&&strings.@i[1]=="");
        strings.@i=new[]{"a","π"};byte[] expected=Hex("010000006102000000cf80");
        Need(Equal(Ros2Managed_fixtureMsgStringIndexCodec.Instance.Encode(strings),expected));
        Need(Ros2Managed_fixtureMsgStringIndexCodec.Instance.Decode(expected).@i[1]=="π");
        var records=new _Ros2Managed_fixtureMsgRecordIndex();records.@i[0].@pair[0]=3;
        Need(records.@i[1].@pair[0]==0&&Ros2Managed_fixtureMsgRecordIndexCodec.Instance.Decode(Ros2Managed_fixtureMsgRecordIndexCodec.Instance.Encode(records)).@i[0].@pair[0]==3);
    }
    private static void ClassScopeFields() {
        var value=new _Ros2Managed_fixtureMsgClassScope { @classmethod=17,@list="a",@str="π",@values=new byte[]{0,255},@record=1,@sealed=2,@permits=3 };
        byte[] expected=Hex("11010000006102000000cf800200000000ff010203");
        Need(Equal(Ros2Managed_fixtureMsgClassScopeCodec.Instance.Encode(value),expected));
        var result=Ros2Managed_fixtureMsgClassScopeCodec.Instance.Decode(expected);
        Need(result.@classmethod==17&&result.@list=="a"&&result.@str=="π"&&result.@record==1&&result.@sealed==2&&result.@permits==3);
    }
    public static int Main() {
        Action[] cases={CompleteScalarGolden,SignedWidthBits,CompleteUnsigned64,FiniteAndSignedZero,
            StrictText,ExactDecode,NestedArrays,ArrayBounds,IndependentDefaults,NoMissingDefaultWire,
            CompleteComponents,NormalizedROS1Primitives,InputOwnership,FixedIndexFields,ClassScopeFields};
        foreach(Action test in cases)test();
        Need(cases.Length==15);
        Console.WriteLine("managed C# record cases="+cases.Length+" checks="+checks+" native/ROS=false reflection=false");
        return 0;
    }
}
