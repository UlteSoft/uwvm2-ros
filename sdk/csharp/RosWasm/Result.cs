using System;
namespace Uwvm.RosWasm;

/// <summary>One ABI outcome. A timeout may have admitted an effect; this SDK never retries it.</summary>
public sealed class Result {
    public uint Error { get; private set; }
    public byte[] Data { get; }
    public ulong? Required { get; }
    public ulong? Value { get; }
    public Result(uint error, byte[]? data=null, ulong? required=null, ulong? value=null) {
        Error=error; Data=data??Array.Empty<byte>(); Required=required; Value=value;
    }
    // Only a prepared scalar native retirement updates this preallocated outcome.
    internal void SetScalarRetirementError(uint error) { Error=error; }
    public Result Checked() { if(Error!=0) throw new RosException(this); return this; }
}
public sealed class RosException : Exception {
    public Result Result { get; }
    public RosException(Result result):base("ROS ABI error "+result.Error+"; no automatic replay") { Result=result; }
}
/// <summary>Only a live NativeTransport represents the selected Mono/WASI ABI. Test doubles prove local logic only.</summary>
public interface ITransport {
    int Backend { get; }
    Result Invoke(string operation, ulong handle, byte[] input, int capacity, ulong other=0, ulong correlation=0);
}
public interface ICodec<T> { byte[] Encode(T value); T Decode(byte[] bytes); }
public interface IPollable { Result PollOnce(); }

/// <summary>Preallocated per-effect cleanup evidence. A failure never authorizes another close attempt.</summary>
public struct CleanupItem {
    public string? Alias; public ulong Handle; public Result? Result; public Exception? Exception;
    public RetirementDiagnostic? Retirement;
}
public sealed class CleanupReport {
    public Exception? Primary { get; }
    public CleanupItem[] Items { get; }
    public int Count { get; internal set; }
    internal CleanupReport(Exception? primary, int capacity) { Primary=primary; Items=new CleanupItem[capacity]; }
    public bool Succeeded { get { for(int i=0;i<Count;i++) if(Items[i].Exception!=null||Items[i].Result?.Error!=0)return false; return true; } }
    public void ThrowIfFailed() {
        if(Primary!=null) throw Primary;
        for(int i=0;i<Count;i++) { if(Items[i].Exception is Exception e)throw e; Items[i].Result?.Checked(); }
    }
}
