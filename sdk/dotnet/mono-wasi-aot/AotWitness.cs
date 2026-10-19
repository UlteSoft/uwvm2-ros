using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
namespace Uwvm.RosWasm.Aot;
/// <summary>A live pure method and real pinned-array root exercise. No ROS import
/// or success claim. Object/IR method coverage and target execution are separate.</summary>
public static class AotWitness {
    [MethodImpl(MethodImplOptions.NoInlining)]
    public static ulong CoverageSentinel(ulong value) => unchecked((value ^ 0xfedcba9800000007UL) * 0x100000001b3UL);
    public static void CheckGuestRoots() {
        if(!OperatingSystem.IsWasi() || IntPtr.Size!=4)throw new PlatformNotSupportedException("actual wasm32 WASI required");
        byte[] data=new byte[]{0x17,0x81,0xff};GCHandle root=default;
        try {
            root=GCHandle.Alloc(data,GCHandleType.Pinned);
            IntPtr first=root.AddrOfPinnedObject();GC.Collect();
            if(first!=root.AddrOfPinnedObject() || Marshal.ReadByte(first,1)!=0x81 || CoverageSentinel(7)!=0x1111104800000000UL)
                throw new InvalidOperationException("actual pinned array/root changed");
        }finally { if(root.IsAllocated)root.Free(); }
        GC.KeepAlive(data);
    }
}
