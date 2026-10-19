using System;
namespace Uwvm.RosWasm.Owned;
/// <summary>Cursor accounting over the existing scalar codec; no second wire format.</summary>
public static class ControlWire {
    public static byte[] Utf8(string value)=>Codecs.Utf8(value);
    public static string Text(byte[] value)=>Codecs.Text(value);
    public sealed class Writer {
        private readonly Codecs.Writer writer;public int Position { get; private set; }
        public Writer(int maximum) { writer=new Codecs.Writer(maximum); }
        public Writer Raw(byte[] bytes) { writer.Raw(bytes);Position=checked(Position+bytes.Length);return this; }
        public Writer Unsigned(ulong value,int width) { writer.Unsigned(value,width);Position=checked(Position+width);return this; }
        public Writer Float64(double value) { writer.Float64(value);Position=checked(Position+8);return this; }
        public byte[] Finish()=>writer.Finish();
    }
    public sealed class Reader {
        private readonly Codecs.Reader reader;private readonly int original;
        public Reader(byte[] bytes) { reader=new Codecs.Reader(bytes);original=bytes.Length; }
        public int Remaining=>reader.Remaining;public int Position=>original-reader.Remaining;
        public byte[] Take(int n)=>reader.Take(n);public ulong Unsigned(int n)=>reader.Unsigned(n);public double Float64()=>reader.Float64();public void End()=>reader.End();
    }
}
