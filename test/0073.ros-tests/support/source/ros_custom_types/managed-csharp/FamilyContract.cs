#nullable enable
using System;
using System.IO;
using System.Text;
#if JOINT1
using Value=Uwvm.RosWasm.Generated.BoundedRecords._Ros1Sensor_msgsJointState;
using Codec=Uwvm.RosWasm.Generated.BoundedRecords.Ros1Sensor_msgsJointStateCodec;
#elif JOINT2
using Value=Uwvm.RosWasm.Generated.BoundedRecords._Ros2Sensor_msgsMsgJointState;
using Codec=Uwvm.RosWasm.Generated.BoundedRecords.Ros2Sensor_msgsMsgJointStateCodec;
#elif ROBOT1
using Value=Uwvm.RosWasm.Generated.BoundedRecords._Ros1Xarm_msgsRobotMsg;
using Codec=Uwvm.RosWasm.Generated.BoundedRecords.Ros1Xarm_msgsRobotMsgCodec;
#elif ROBOT2
using Value=Uwvm.RosWasm.Generated.BoundedRecords._Ros2Xarm_msgsMsgRobotMsg;
using Codec=Uwvm.RosWasm.Generated.BoundedRecords.Ros2Xarm_msgsMsgRobotMsgCodec;
#elif CIO1
using Value=Uwvm.RosWasm.Generated.BoundedRecords._Ros1Xarm_msgsCIOState;
using Codec=Uwvm.RosWasm.Generated.BoundedRecords.Ros1Xarm_msgsCIOStateCodec;
#elif CIO2
using Value=Uwvm.RosWasm.Generated.BoundedRecords._Ros2Xarm_msgsMsgCIOState;
using Codec=Uwvm.RosWasm.Generated.BoundedRecords.Ros2Xarm_msgsMsgCIOStateCodec;
#else
#error Select exactly one JOINT1 JOINT2 ROBOT1 ROBOT2 CIO1 CIO2 actual catalog role
#endif
#if (JOINT1 && (JOINT2 || ROBOT1 || ROBOT2 || CIO1 || CIO2)) || (JOINT2 && (ROBOT1 || ROBOT2 || CIO1 || CIO2)) || (ROBOT1 && (ROBOT2 || CIO1 || CIO2)) || (ROBOT2 && (CIO1 || CIO2)) || (CIO1 && CIO2)
#error Select one family role only
#endif

// No reflection: every family/version is compiled against its actual generated
// value and codec. BinaryWriter primitive writes are little endian; strings use
// explicit canonical u32 UTF8 framing, never BinaryWriter's 7-bit string API.
internal static class FamilyContract {
    private static int checks;
    private static void Need(bool value) {++checks;if(!value)throw new Exception("complete C# family");}
    private static bool Equal(byte[] a,byte[] b) {if(a.Length!=b.Length)return false;for(int i=0;i<a.Length;i++)if(a[i]!=b[i])return false;return true;}
    private static void Refuses(Action operation) {bool refused=false;try{operation();}catch(ArgumentException){refused=true;}catch(FormatException){refused=true;}Need(refused);}
    private static void Text(BinaryWriter writer,string value) {byte[] bytes=new UTF8Encoding(false,true).GetBytes(value);writer.Write((uint)bytes.Length);writer.Write(bytes);}
    public static int Main() {
        var value=new Value();
        using var stream=new MemoryStream();using var writer=new BinaryWriter(stream,new UTF8Encoding(false,true),true);
#if JOINT1 || ROBOT1 || CIO1
        value.@header.@seq=77;writer.Write((uint)77);
        value.@header.@stamp.@sec=3;value.@header.@stamp.@nsec=4;
        writer.Write((uint)3);writer.Write((uint)4);
#else
        value.@header.@stamp.@sec=3;value.@header.@stamp.@nanosec=4;
        writer.Write((int)3);writer.Write((uint)4);
#endif
        value.@header.@frame_id="robot";Text(writer,"robot");
#if ROBOT1 || ROBOT2
        value.@state=1;value.@mode=2;value.@cmdnum=3;value.@mt_brake=4;value.@mt_able=5;value.@err=-6;value.@warn=7;
        foreach(short item in new short[]{1,2,3,4,5,-6,7})writer.Write(item);
        value.@angle=new float[7];writer.Write((uint)7);
        for(int i=0;i<7;i++){value.@angle[i]=i/2.0f;writer.Write(value.@angle[i]);}
        for(int i=0;i<6;i++){value.@pose[i]=-i-1;writer.Write(value.@pose[i]);}
        for(int i=0;i<6;i++){value.@offset[i]=i+10;writer.Write(value.@offset[i]);}
#elif CIO1 || CIO2
        value.@state=-1;value.@code=2;writer.Write((short)-1);writer.Write((short)2);
        value.@input_digitals=new ushort[]{0,65535};value.@output_digitals=new ushort[]{42,32768};
        foreach(ushort item in value.@input_digitals)writer.Write(item);foreach(ushort item in value.@output_digitals)writer.Write(item);
        value.@input_analogs=new float[]{-0.0f,1.25f};value.@output_analogs=new float[]{2.5f,-3.5f};
        foreach(float item in value.@input_analogs)writer.Write(item);foreach(float item in value.@output_analogs)writer.Write(item);
        for(int i=0;i<16;i++){value.@input_conf[i]=(byte)i;writer.Write((byte)i);}
        for(int i=0;i<16;i++){value.@output_conf[i]=(byte)(240+i);writer.Write((byte)(240+i));}
#else
        value.@name=new[]{"joint1"};value.@position=new[]{1.25};value.@velocity=new[]{0.5};value.@effort=new[]{-0.75};
        writer.Write((uint)1);Text(writer,"joint1");
        writer.Write((uint)1);writer.Write(1.25);writer.Write((uint)1);writer.Write(0.5);writer.Write((uint)1);writer.Write(-0.75);
#endif
        writer.Flush();byte[] expected=stream.ToArray(),wire=Codec.Instance.Encode(value);
        Need(Equal(wire,expected));Need(Equal(Codec.Instance.Encode(Codec.Instance.Decode(expected)),expected));
        Need(Codec.NATIVE_IDENTITY==null);Need(Codec.CODEC_IDENTITY.StartsWith("UWVMROS1_",StringComparison.Ordinal));
#if JOINT1 || JOINT2
        value.@effort=new[]{0.0,1.0};Refuses(()=>Codec.Instance.Encode(value));value.@effort=new[]{-0.75};
#endif
#if JOINT1 || ROBOT1 || CIO1
        value.@header.@stamp.@nsec=1000000000;
#else
        value.@header.@stamp.@nanosec=1000000000;
#endif
        Refuses(()=>Codec.Instance.Encode(value));
        Console.WriteLine("managed C# one full family wire-bytes="+wire.Length+" checks="+checks+" native/ROS=false reflection=false");
        return 0;
    }
}
