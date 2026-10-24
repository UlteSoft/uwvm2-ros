#nullable enable
using System;
using static Uwvm.RosWasm.Generated.BoundedRecords;

// One separate actually-generated local catalog. Exact field spellings include
// C# keywords, the IR symbol and metadata spelling. No reflection or ROS import.
internal static class NamesContract {
    public static int Main() {
        var value=new _Ros1Csharp_fixtureValue { @event=1,@checked=2,@lock=3,@out=4,
            @params=5,@ulong=6,@Ros1Csharp_fixtureValue=7,@CODEC_IDENTITY=8 };
        byte[] wire=Ros1Csharp_fixtureValueCodec.Instance.Encode(value);
        if(wire.Length!=8)throw new Exception("complete native field count");
        for(int i=0;i<8;i++)if(wire[i]!=i+1)throw new Exception("native field wire order");
        var result=Ros1Csharp_fixtureValueCodec.Instance.Decode(wire);
        if(result.@event!=1||result.@checked!=2||result.@lock!=3||result.@out!=4||result.@params!=5||
           result.@ulong!=6||result.@Ros1Csharp_fixtureValue!=7||result.@CODEC_IDENTITY!=8)
            throw new Exception("complete original field spellings");
        if(!Ros1Csharp_fixtureValueCodec.CODEC_IDENTITY.StartsWith("UWVMROS1_",StringComparison.Ordinal)||
           Ros1Csharp_fixtureValueCodec.NATIVE_IDENTITY!=null)
            throw new Exception("IR codec metadata is separate from native byte field");
        Console.WriteLine("managed C# names cases=1 fields=8 native/ROS=false reflection=false");
        return 0;
    }
}
