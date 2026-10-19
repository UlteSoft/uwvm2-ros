using System;
using System.Buffers.Binary;
using System.IO;
using System.Text;
namespace Uwvm.RosWasm;

/// <summary>Complete canonical v1 payload codecs, not CDR/serialization/discovery or ROS authority.</summary>
public static class Codecs {
    public const int MaximumInput=4193786,MaximumOutput=4194304;
    private static readonly UTF8Encoding Utf8Strict=new(false,true);
    public static byte[] Utf8(string value) { if(value==null)throw new ArgumentNullException(nameof(value));if(value.Length>MaximumInput||Utf8Strict.GetByteCount(value)>MaximumInput)throw new ArgumentException("UTF8 allocation bound");return Utf8Strict.GetBytes(value); }
    public static byte[] Wire(byte[] value) { if(value==null||value.Length>MaximumInput)throw new ArgumentException("complete nonnull canonical wire required");return value; }
    public static string Text(byte[] value) { return Utf8Strict.GetString(value); }
    public sealed class Reader {
        private readonly byte[] data;private int offset;
        public Reader(byte[] data) { if(data==null||data.Length>MaximumOutput)throw new ArgumentException("bounded wire bytes");this.data=data; }
        public int Remaining=>data.Length-offset;
        public byte[] Take(int n) { if(n<0||n>Remaining)throw new FormatException("truncated canonical value");byte[] value=new byte[n];Buffer.BlockCopy(data,offset,value,0,n);offset+=n;return value; }
        public ulong Unsigned(int n) { if(n!=1&&n!=2&&n!=4&&n!=8)throw new ArgumentException("integer width");if(n>Remaining)throw new FormatException("truncated integer");ulong v=0;for(int i=0;i<n;i++)v|=(ulong)data[offset++]<<(i*8);return v; }
        public double Float64() { double d=BitConverter.Int64BitsToDouble(unchecked((long)Unsigned(8)));if(!double.IsFinite(d))throw new FormatException("nonfinite canonical double");return d; }
        public string String() { ulong n=Unsigned(4);if(n>(ulong)Remaining)throw new FormatException("truncated UTF8");return Text(Take((int)n)); }
        public void End() { if(Remaining!=0)throw new FormatException("trailing canonical bytes"); }
    }
    public sealed class Writer {
        private readonly MemoryStream stream=new();private readonly int maximum;
        public Writer(int maximum=MaximumInput) { if(maximum<0||maximum>MaximumInput)throw new ArgumentException("wire bound");this.maximum=maximum; }
        public Writer Raw(byte[] bytes) { if(bytes==null||bytes.Length>maximum-stream.Length)throw new ArgumentException("wire bound");stream.Write(bytes,0,bytes.Length);return this; }
        public Writer Unsigned(ulong v,int n) { if(n!=1&&n!=2&&n!=4&&n!=8)throw new ArgumentException("width");if(n<8&&v>=(1UL<<(8*n)))throw new ArgumentException("integer range");if(n>maximum-stream.Length)throw new ArgumentException("wire bound");for(int i=0;i<n;i++)stream.WriteByte((byte)(v>>(8*i)));return this; }
        public Writer Float64(double v) { if(!double.IsFinite(v))throw new ArgumentException("finite canonical double");return Unsigned(unchecked((ulong)BitConverter.DoubleToInt64Bits(v)),8); }
        public Writer String(string value) { if(value==null||value.Length>maximum-stream.Length-4)throw new ArgumentException("bounded text");if(Utf8Strict.GetByteCount(value)>maximum-stream.Length-4)throw new ArgumentException("UTF8 byte bound");byte[] bytes=Utf8(value);return Unsigned((ulong)bytes.Length,4).Raw(bytes); }
        public byte[] Finish()=>stream.ToArray();
    }
    private sealed class DelegateCodec<T> : ICodec<T> {
        private readonly Func<T,byte[]> encoder;private readonly Func<byte[],T> decoder;
        public DelegateCodec(Func<T,byte[]> encoder,Func<byte[],T> decoder) { this.encoder=encoder;this.decoder=decoder; }
        public byte[] Encode(T value)=>encoder(value);public T Decode(byte[] bytes)=>decoder(bytes);
    }
    public static readonly ICodec<string> String=new DelegateCodec<string>(v=>new Writer().String(v).Finish(),b=>{var r=new Reader(b);string v=r.String();r.End();return v;});
    public static readonly ICodec<bool> Bool=new DelegateCodec<bool>(v=>new[]{(byte)(v?1:0)},b=>{if(b==null||b.Length!=1||b[0]>1)throw new FormatException("Bool must be0/1");return b[0]==1;});
    public static readonly ICodec<long> Int64=new DelegateCodec<long>(v=>new Writer(8).Unsigned(unchecked((ulong)v),8).Finish(),b=>{var r=new Reader(b);long v=unchecked((long)r.Unsigned(8));r.End();return v;});
    public static readonly ICodec<double> Float64=new DelegateCodec<double>(v=>new Writer(8).Float64(v).Finish(),b=>{var r=new Reader(b);double v=r.Float64();r.End();return v;});
    public static readonly ICodec<double[]> Twist=new DelegateCodec<double[]>(v=>{if(v==null||v.Length!=6)throw new ArgumentException("six Twist components");var w=new Writer(48);foreach(double d in v)w.Float64(d);return w.Finish();},b=>{var r=new Reader(b);var v=new double[6];for(int i=0;i<6;i++)v[i]=r.Float64();r.End();return v;});
    public readonly struct Empty { }
    public static readonly ICodec<Empty> Trigger=new DelegateCodec<Empty>(_=>Array.Empty<byte>(),b=>{if(b==null||b.Length!=0)throw new FormatException("Trigger request is empty");return default;});
    public readonly struct Response { public readonly bool Success;public readonly string Message;public Response(bool success,string message) { Success=success;Message=message; } }
    public static readonly ICodec<Response> ServiceResponse=new DelegateCodec<Response>(v=>new Writer().Raw(Bool.Encode(v.Success)).String(v.Message).Finish(),b=>{var r=new Reader(b);bool ok=Bool.Decode(r.Take(1));string text=r.String();r.End();return new Response(ok,text);});
    public static byte[] PrefixId(ulong id,byte[] body) { if(id==0||body==null||body.Length>MaximumInput-8)throw new ArgumentException("nonzero owned ID/body bound");return new Writer().Unsigned(id,8).Raw(body).Finish(); }
    public static byte[] GoalId(ulong id) { if(id==0)throw new ArgumentException("nonzero ID");return new Writer(8).Unsigned(id,8).Finish(); }
    public static byte[] FibonacciGoal(int order) { if(order<0||order>46)throw new ArgumentException("Fibonacci order0..46");return new Writer(4).Unsigned((uint)order,4).Finish(); }
    /// <summary>Scalar parameter arrays have an explicit logical maximum, full count and exact consumption.</summary>
    public static ICodec<T[]> ArrayOf<T>(ICodec<T> element,int maximum,int fixedWidth=0) {
        if(element==null||maximum<0||maximum>MaximumInput||(fixedWidth!=0&&fixedWidth!=1&&fixedWidth!=8))throw new ArgumentException("scalar array contract");
        if(fixedWidth==0&&!ReferenceEquals(element,String))throw new ArgumentException("only counted String has variable scalar framing");
        if(fixedWidth==1&&!ReferenceEquals(element,Bool)||fixedWidth==8&&!ReferenceEquals(element,Int64)&&!ReferenceEquals(element,Float64))throw new ArgumentException("actual scalar framing required");
        return new DelegateCodec<T[]>(v=>{if(v==null||v.Length>maximum)throw new ArgumentException("array bound");var w=new Writer().Unsigned((uint)v.Length,4);foreach(T x in v)w.Raw(element.Encode(x));return w.Finish();},b=>{var r=new Reader(b);ulong n=r.Unsigned(4);if(n>(ulong)maximum||n>(ulong)r.Remaining/(ulong)(fixedWidth==0?4:fixedWidth))throw new FormatException("array count/bound");var a=new T[(int)n];for(int i=0;i<a.Length;i++)a[i]=fixedWidth==0?(T)(object)r.String():element.Decode(r.Take(fixedWidth));r.End();return a;});
    }
}
