using System;
using System.Buffers.Binary;
namespace Uwvm.RosWasm;

/// <summary>One validated versioned64-byte ABI event; payload is a complete owned copy.</summary>
public sealed class RosEvent {
    public byte Kind { get; } public byte Backend { get; } public uint Flags { get; } public uint Error { get; }
    public ulong Source { get; } public ulong Correlation { get; } public ulong Sequence { get; }
    public ulong Instance { get; } public ulong Session { get; } public byte[] Payload { get; }
    private RosEvent(byte kind,byte backend,uint flags,uint error,ulong source,ulong correlation,ulong sequence,ulong instance,ulong session,byte[] payload) {
        Kind=kind;Backend=backend;Flags=flags;Error=error;Source=source;Correlation=correlation;Sequence=sequence;Instance=instance;Session=session;Payload=payload;
    }
    public static RosEvent Decode(byte[] data) {
        if(data==null||data.Length<64||data.Length>Codecs.MaximumOutput)throw new FormatException("bounded event envelope");
        ReadOnlySpan<byte> b=data;uint magic=BinaryPrimitives.ReadUInt32LittleEndian(b);ushort version=BinaryPrimitives.ReadUInt16LittleEndian(b.Slice(4));
        ushort rawKind=BinaryPrimitives.ReadUInt16LittleEndian(b.Slice(6));byte kind=(byte)rawKind,backend=b[8];uint flags=b[9],error=BinaryPrimitives.ReadUInt32LittleEndian(b.Slice(12));
        ulong source=BinaryPrimitives.ReadUInt64LittleEndian(b.Slice(16)),correlation=BinaryPrimitives.ReadUInt64LittleEndian(b.Slice(24)),sequence=BinaryPrimitives.ReadUInt64LittleEndian(b.Slice(32)),instance=BinaryPrimitives.ReadUInt64LittleEndian(b.Slice(40)),session=BinaryPrimitives.ReadUInt64LittleEndian(b.Slice(48)),size=BinaryPrimitives.ReadUInt64LittleEndian(b.Slice(56));
        if(magic!=0x45525755||version!=1||rawKind<1||rawKind>7||BinaryPrimitives.ReadUInt16LittleEndian(b.Slice(10))!=0||(backend!=1&&backend!=2)||source==0||sequence==0||instance==0||session==0||size!=(ulong)data.Length-64)throw new FormatException("event header/extent");
        if(kind<6) {
            if(error!=0||(kind==1&&(correlation!=0||(flags!=0&&flags!=8)))||(kind==2&&(correlation==0||(flags!=0&&flags!=4)))||(kind>=3&&(correlation==0||flags!=0)))throw new FormatException("event data flags/ID");
            if((kind==2||kind==4||kind==5)&&(size<8||BinaryPrimitives.ReadUInt64LittleEndian(b.Slice(64))!=correlation))throw new FormatException("event body ID");
        } else if(size!=0||(flags&~3U)!=0||(kind==6&&error==0)||(kind==7&&(flags&1)==0))throw new FormatException("event resource state");
        byte[] payload=new byte[(int)size];Buffer.BlockCopy(data,64,payload,0,payload.Length);
        return new RosEvent(kind,backend,flags,error,source,correlation,sequence,instance,session,payload);
    }
}
/// <summary>Exclusive fresh observer stream. Sequence belongs to the whole observer queue, not each source.</summary>
public sealed class Observer : IPollable,IDisposable {
    public Capability Owner { get; }
    private readonly int capacity;
    private readonly Binding[] bindings;private int count;
    private ulong sequence,instance,session;
    public RosEvent? LastEvent { get; private set; }
    private readonly struct Binding { public readonly ulong Source,Correlation;public Binding(ulong source,ulong correlation) { Source=source;Correlation=correlation; } }
    public Observer(Capability owner,int capacity,int maximumWatches=64) {
        if(owner==null||capacity<64||capacity>Codecs.MaximumOutput||maximumWatches<1||maximumWatches>65536)throw new ArgumentException("bounded observer");
        Owner=owner;this.capacity=capacity;bindings=new Binding[maximumWatches];
    }
    public Result Watch(Capability source,ulong correlation=0) {
        if(source==null||source.Session!=Owner.Session||source.Handle==0)return new Result(9);
        if(Owner.Busy)return new Result(17);if(count==bindings.Length)return new Result(90);
        for(int i=0;i<count;i++)if(bindings[i].Source==source.Handle&&bindings[i].Correlation==correlation)return new Result(17);
        var next=new Binding(source.Handle,correlation);Result r=Owner.Invoke("event_watch",other:source,correlation:correlation);
        if(r.Error==0)bindings[count++]=next;return r;
    }
    public Result PollOnce() {
        if(!Owner.Enter())return new Result(17);
        try {
            Result r=Owner.Invoke("event_poll_v1",capacity:capacity);if(r.Error!=0)return r;
            RosEvent e=RosEvent.Decode(r.Data);bool bound=false;
            for(int i=0;i<count;i++)bound|=bindings[i].Source==e.Source&&(bindings[i].Correlation==0||bindings[i].Correlation==e.Correlation||e.Correlation==0);
            if(!bound||e.Backend!=Owner.Session.Transport.Backend||sequence==ulong.MaxValue||e.Sequence!=sequence+1||(instance!=0&&instance!=e.Instance)||(session!=0&&session!=e.Session))throw new FormatException("foreign/stale observer stream");
            sequence=e.Sequence;instance=e.Instance;session=e.Session;LastEvent=e;return r;
        } finally { Owner.Leave(); }
    }
    public void Dispose()=>Owner.Dispose();
}
