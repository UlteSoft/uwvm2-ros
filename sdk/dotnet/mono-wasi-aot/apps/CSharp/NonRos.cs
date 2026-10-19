using System;
using System.Runtime.CompilerServices;
using Uwvm.RosWasm.Aot;
internal static class NonRos {
    private struct Sample { public long Signed;public ulong FullId;public double Fraction; }
    [MethodImpl(MethodImplOptions.NoInlining)]
    private static long FrameThree(long input,Sample sample) {
        long typedLocal=checked(input+sample.Signed);
        Console.Error.WriteLine("UWVM_AOT_DBG_READY csharp");
        int control=Console.Read(); // DBG_STOP_CSHARP: caller supplies one G after inspection.
        if(control!='G')throw new InvalidOperationException("explicit debugger release G required");
        return typedLocal;
    }
    [MethodImpl(MethodImplOptions.NoInlining)]
    private static long FrameTwo(long input,Sample sample) {
        long result=FrameThree(input,sample);Console.Error.WriteLine("UWVM_AOT_DBG_RESUME_TWO");return result;
    }
    [MethodImpl(MethodImplOptions.NoInlining)]
    private static long FrameOne(long input,Sample sample,int depth) {
        long result=depth==0?FrameTwo(input,sample):FrameOne(input,sample,depth-1);
        Console.Error.WriteLine("UWVM_AOT_DBG_RESUME_DEPTH="+depth);return result;
    }
    private static int Main(string[] args) {
        try {
            AotWitness.CheckGuestRoots();
            if(args.Length!=2)throw new ArgumentException("signed-int64 expected-nonASCII-label");
            long number=long.Parse(args[0],System.Globalization.CultureInfo.InvariantCulture);
            var sample=new Sample { Signed=-7,FullId=ulong.MaxValue,Fraction=1.25 };
            long result=FrameOne(number,sample,2);
            Console.WriteLine("label="+args[1]+" result="+result+" full-id="+sample.FullId+" fraction="+sample.Fraction);
            return 0;
        }catch(Exception error) {Console.Error.WriteLine(error);return 1;}
    }
}
