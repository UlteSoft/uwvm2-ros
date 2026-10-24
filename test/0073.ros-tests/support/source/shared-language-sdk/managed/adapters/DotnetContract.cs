using System;
using Uwvm.RosWasm;
using F = Uwvm.RosWasm.Generated.FSharp;
using V = Uwvm.RosWasm.Generated.VisualBasic;

// CLR execution qualifies the generated adapters' value semantics only.
// The same source is a future Mono/WASI candidate; no ROS import is simulated.
internal static class DotnetContract {
    private static int checks;
    private static void Check(bool ok) { ++checks; if (!ok) throw new Exception("adapter assertion " + checks); }
    private static void Refuses(Action action) {
        bool refused = false;
        try { action(); } catch (ArgumentException) { refused = true; }
        catch (FormatException) { refused = true; }
        Check(refused);
    }
    private static readonly byte[] Golden = Convert.FromHexString(
        "01ff80ffff0080ffffffff00000080ffffffffffffffff0000000000000080" +
        "00000080000000000000f43f02000000cf80");
    public static int Main() {
        var f = new F.Ros2Managed_fixtureMsgScalars {
            flag=true,u8=255,i8=-128,u16=65535,i16=-32768,u32=uint.MaxValue,i32=int.MinValue,
            u64=ulong.MaxValue,i64=long.MinValue,f32=-0.0f,f64=1.25,text="π" };
        var v = new V.Ros2Managed_fixtureMsgScalars {
            flag=true,u8=255,i8=-128,u16=65535,i16=-32768,u32=uint.MaxValue,i32=int.MinValue,
            u64=ulong.MaxValue,i64=long.MinValue,f32=-0.0f,f64=1.25,text="π" };
        var fc = F.Ros2Managed_fixtureMsgScalarsCodec.Instance;
        var vc = V.Ros2Managed_fixtureMsgScalarsCodec.Instance;
        Check(fc.Encode(f).AsSpan().SequenceEqual(Golden));
        Check(vc.Encode(v).AsSpan().SequenceEqual(Golden));
        var fd = fc.Decode(Golden); var vd = vc.Decode(Golden);
        Check(fd.u64==ulong.MaxValue && fd.i64==long.MinValue && fd.text=="π");
        Check(vd.u64==ulong.MaxValue && vd.i64==long.MinValue && vd.text=="π");
        Check(BitConverter.SingleToInt32Bits(fd.f32)==int.MinValue);
        Check(BitConverter.SingleToInt32Bits(vd.f32)==int.MinValue);
        var bad = (byte[])Golden.Clone(); bad[0]=2;
        Refuses(()=>fc.Decode(bad)); Refuses(()=>vc.Decode(bad));
        Refuses(()=>fc.Decode(new byte[Golden.Length+1]));
        Refuses(()=>vc.Decode(Golden[..^1]));
        f.text="\ud800"; v.text="\ud800";
        Refuses(()=>fc.Encode(f)); Refuses(()=>vc.Encode(v));
        f.text="a\0b"; v.text="a\0b";
        Refuses(()=>fc.Encode(f)); Refuses(()=>vc.Encode(v));
        var fo = new F.Ros2Managed_fixtureMsgOuter();
        var vo = new V.Ros2Managed_fixtureMsgOuter();
        Check(fo.@fixed.Length==2 && fo.@fixed[0] != fo.@fixed[1]);
        Check(vo.@fixed.Length==2 && vo.@fixed[0] != vo.@fixed[1]);
        Check(fo.inner.tags.Length==2 && fo.inner.tags[0]=="");
        Check(vo.inner.tags.Length==2 && vo.inner.tags[0]=="");
        Check(fo.rotation.w==1.0 && vo.rotation.w==1.0);
        Check(fo.rotations[1].w==1.0 && vo.rotations[1].w==1.0);
        fo.@fixed[1].scalar.u64=ulong.MaxValue;
        vo.@fixed[1].scalar.i64=long.MinValue;
        var foc=F.Ros2Managed_fixtureMsgOuterCodec.Instance;
        var voc=V.Ros2Managed_fixtureMsgOuterCodec.Instance;
        Check(foc.Decode(foc.Encode(fo)).@fixed[1].scalar.u64==ulong.MaxValue);
        Check(voc.Decode(voc.Encode(vo)).@fixed[1].scalar.i64==long.MinValue);
        var bridge = F.Ros2Managed_fixtureMsgOuterCodec.ToBridge(fo);
        bridge.@fixed[1].scalar.u64=0;
        Check(fo.@fixed[1].scalar.u64==ulong.MaxValue);
        fo.items=new F.Ros2Managed_fixtureMsgInner[3];
        vo.items=new V.Ros2Managed_fixtureMsgInner[3];
        Refuses(()=>foc.Encode(fo)); Refuses(()=>voc.Encode(vo));
        Check(F.Ros2Managed_fixtureMsgScalarsCodec.NATIVE_IDENTITY is null);
        Check(V.Ros2Managed_fixtureMsgScalarsCodec.NATIVE_IDENTITY is null);
        Console.WriteLine("PASS_CLR_ADAPTER_VALUE_CHECKS=" + checks);
        return 0;
    }
}
