using System;
namespace Uwvm.RosWasm.Owned;

public sealed class OwnedServiceException : Exception {
    public uint Code { get; }
    internal OwnedServiceException(uint code):base("owned ROS2 service error "+code+"; no automatic replay") { Code=code; }
}
public sealed class ServiceDefinition<Q,S> {
    public string NativeType { get; } public string CodecIdentity { get; }
    public ICodec<Q> Request { get; } public ICodec<S> Response { get; }
    public int MaximumRequest { get; } public int MaximumResponse { get; }
    public ServiceDefinition(string nativeType,string codecIdentity,ICodec<Q> request,ICodec<S> response,int maximumRequest,int maximumResponse) {
        if(nativeType==null||nativeType.Length>255||!System.Text.RegularExpressions.Regex.IsMatch(nativeType,"^[a-z][a-z0-9_]*/srv/[A-Z][A-Za-z0-9_]*$")||
            codecIdentity==null||!System.Text.RegularExpressions.Regex.IsMatch(codecIdentity,"^UWVMROS1_[0-9a-f]{64}$")||request==null||response==null||
            maximumRequest<0||maximumRequest>32768||maximumResponse<0||maximumResponse>32768)throw new ArgumentException("complete explicit service root/codec bounds required");
        NativeType=nativeType;CodecIdentity=codecIdentity;Request=request;Response=response;MaximumRequest=maximumRequest;MaximumResponse=maximumResponse;
    }
}
public readonly struct Ticket {
    internal readonly int Endpoint,Row;internal readonly ulong Source;
    public ulong Value { get; }
    internal Ticket(int endpoint,int row,ulong source,ulong value) { Endpoint=endpoint;Row=row;Source=source;Value=value; }
}
public enum RequestState { Unused,Reserved,Admitted,Delivered,Completed,Abandoned,Discarded,Unknown }
public sealed class Delivery<T> {
    public bool Present { get; internal set; } public bool IsControl { get; internal set; }
    public Ticket Ticket { get; internal set; } public T? Value { get; internal set; }
    public ServiceInformation Information { get; internal set; } public SourceControl Control { get; internal set; }
}
public struct RawCloseEvidence {
    public ulong FullHandle;public Result? Result;public Exception? Exception;public RetirementDiagnostic Retirement;
}
public sealed class EndpointDiagnostic {
    public uint PrimaryCode { get; internal set; } public Exception? PrimaryException { get; internal set; }
    public ulong Source { get; internal set; } public ulong Peer { get; internal set; } public ulong Query { get; internal set; }
    public bool Terminal { get; internal set; } public bool CleanupRequested { get; internal set; }
    public bool HasReceipt { get; internal set; } public EndpointReceipt Receipt { get; internal set; }
    internal readonly OwnedAttempt[] opens={new(),new(),new()};
    internal readonly ulong[] returnedOpenHandles=new ulong[3];
    public ReadOnlySpan<OwnedAttempt> Opens=>opens;
    public ReadOnlySpan<ulong> ReturnedOpenHandles=>returnedOpenHandles;
    public readonly OwnedAttempt Activation=new(),Inspection=new(),Submit=new(),Abandon=new(),Respond=new(),Peek=new(),Commit=new(),Cancel=new(),Available=new();
    internal readonly OwnedAttempt[] retires=MakeRetires();internal int retireCount;
    private static OwnedAttempt[] MakeRetires() { var a=new OwnedAttempt[8];for(int i=0;i<a.Length;i++)a[i]=new OwnedAttempt();return a; }
    public OwnedAttempt NativeRetire=>retires[retireCount==0?0:retireCount-1];
    public ReadOnlySpan<OwnedAttempt> NativeRetireHistory=>retires.AsSpan(0,retireCount);
    internal readonly RawCloseEvidence[] closes=new RawCloseEvidence[24];internal int closeCount;
    internal readonly ulong[] remaining=new ulong[3];
    public ReadOnlySpan<RawCloseEvidence> RawCloses=>closes.AsSpan(0,closeCount);
    public ReadOnlySpan<ulong> RemainingRawOwners=>remaining;
    public Exception? CloseHookException { get; internal set; }
    public bool CleanupSucceeded {
        get { if(CloseHookException!=null)return false;if(HasReceipt&&!NativeRetire.Succeeded)return false;
            for(int i=0;i<retireCount;i++)if(!retires[i].Succeeded)return false;
            for(int i=0;i<closeCount;i++)if(closes[i].Exception!=null||closes[i].Result?.Error!=0)return false;return true; }
    }
}
internal sealed class Transaction {
    internal ulong Ticket;internal RequestState State;internal uint Error;
    internal ServiceInformation Information;
}
internal sealed class Slot {
    internal readonly Capability?[] Owners=new Capability?[3];internal readonly string?[] Aliases=new string?[3];
    internal readonly byte[]?[] AliasBytes=new byte[]?[3];internal readonly OwnedCall[] OpenCalls=new OwnedCall[3];
    internal readonly Transaction[] Rows;internal int Used;internal bool Occupied,Closing,HookInvoked;
    internal readonly byte[] ReceiptBytes=new byte[256];internal ulong LastSequence,LastNonce,LastCommitted;
    internal readonly EndpointDiagnostic Diagnostic=new();
    internal readonly OwnedCall Activate,Inspect,Submit,Abandon,Respond,Peek,Commit,Cancel,Available;
    internal readonly OwnedCall[] Retire=new OwnedCall[8];
    internal Slot(int rows) {
        Rows=new Transaction[rows];for(int i=0;i<rows;i++)Rows[i]=new Transaction();var d=Diagnostic;
        for(int i=0;i<3;i++)OpenCalls[i]=new OwnedCall(d.opens[i]);
        Activate=new OwnedCall(d.Activation);Inspect=new OwnedCall(d.Inspection);Submit=new OwnedCall(d.Submit);Abandon=new OwnedCall(d.Abandon);Respond=new OwnedCall(d.Respond);
        Peek=new OwnedCall(d.Peek);Commit=new OwnedCall(d.Commit);Cancel=new OwnedCall(d.Cancel);Available=new OwnedCall(d.Available);
        for(int i=0;i<Retire.Length;i++)Retire[i]=new OwnedCall(d.retires[i]);
    }
}

/// <summary>Explicit, finite, synchronous owned group. The real Session is private.</summary>
public sealed class OwnedGroup : IDisposable {
    private readonly object gate=new();private bool busy,shutdown;
    private readonly NativeTransport native=new();private readonly Session session;
    private readonly Slot[] slots;private int used;
    private readonly byte[] receipt=new byte[256],ticket=new byte[8],availability=new byte[1],packet;
    private readonly OwnedAttempt version=new(),businessVersion=new();private readonly OwnedCall versionCall,businessVersionCall;
    private readonly int maximumRequest,maximumResponse;
#if UWVM_ROSWASM_OWNED_SOURCE_FIXTURES
    private Action<OwnedCall>? localModel;
    internal static OwnedGroup PrepareLocalModel(int endpoints,int rows,int maxRequest,int maxResponse,ITransport core,Action<OwnedCall> model) {
        var group=new OwnedGroup(endpoints,rows,maxRequest,maxResponse,core);group.localModel=model;return group;
    }
#endif
    public int AttemptedEndpoints { get { Enter();try { return used; }finally { Leave(); } } }
    public OwnedGroup(int maximumEndpoints,int transactionsPerEndpoint,int maximumRequest,int maximumResponse):this(maximumEndpoints,transactionsPerEndpoint,maximumRequest,maximumResponse,null) {}
    private OwnedGroup(int maximumEndpoints,int transactionsPerEndpoint,int maximumRequest,int maximumResponse,ITransport? fixtureCore) {
        // Refuse before even the legacy Session ABI query can resolve a bridge.
        // Only the compile-time LOCAL source fixture supplies fixtureCore.
        if(fixtureCore==null&&(!OperatingSystem.IsWasi()||IntPtr.Size!=4))throw new OwnedServiceException(95);
        if(maximumEndpoints<1||maximumEndpoints>256||transactionsPerEndpoint<1||transactionsPerEndpoint>65536||
            (long)maximumEndpoints*transactionsPerEndpoint>65536||maximumRequest<0||maximumRequest>32768||maximumResponse<0||maximumResponse>32768)throw new ArgumentException("explicit finite storage ceilings required");
        this.maximumRequest=maximumRequest;this.maximumResponse=maximumResponse;
        slots=new Slot[maximumEndpoints];for(int i=0;i<slots.Length;i++)slots[i]=new Slot(transactionsPerEndpoint);
        packet=new byte[352+Math.Max(maximumRequest,maximumResponse)];session=new Session(fixtureCore??native,maximumEndpoints*3);versionCall=new OwnedCall(version);businessVersionCall=new OwnedCall(businessVersion);
    }
    private void Enter() { System.Threading.Monitor.Enter(gate);if(busy) { System.Threading.Monitor.Exit(gate);throw new OwnedServiceException(17); }busy=true; }
    private void Leave() { busy=false;System.Threading.Monitor.Exit(gate); }
    private void Live() { if(shutdown)throw new OwnedServiceException(125); }
    private Slot Owner(int index) { if(index<0||index>=used||!slots[index].Occupied)throw new OwnedServiceException(9);return slots[index]; }
    private Slot Active(int index) { Live();var s=Owner(index);if(s.Diagnostic.Terminal)throw new OwnedServiceException(125);return s; }
    private static void Primary(Slot s,uint code,Exception? error=null) { if(s.Diagnostic.PrimaryCode==0&&s.Diagnostic.PrimaryException==null) { s.Diagnostic.PrimaryCode=code;s.Diagnostic.PrimaryException=error; } }
    private static uint Error(OwnedAttempt e)=>e.Code!=0?e.Code:(e.Exception!=null||e.CleanupException!=null?115u:0u);
    private void Execute(OwnedCall c) {
#if UWVM_ROSWASM_OWNED_SOURCE_FIXTURES
        if(localModel!=null) {
            c.Evidence.Reset();try { localModel(c); }catch(Exception error) { c.Evidence.Exception=error;if(c.Evidence.Entered&&!c.Evidence.Returned)c.Evidence.Code=115; }return;
        }
#endif
        native.InvokeOwned(c);
    }
    private void Run(Slot s,OwnedCall c,OwnedOperation op,byte[]? input=null,int length=0,byte[]? output=null,ulong? peer=null) {
        c.Operation=op;c.Source=s.Diagnostic.Source;c.Peer=peer??s.Diagnostic.Peer;c.Incarnation=s.Diagnostic.HasReceipt?s.Diagnostic.Receipt.Incarnation:0;
        c.Input=input??Array.Empty<byte>();c.InputLength=length;c.Output=output??Array.Empty<byte>();Execute(c);
    }
    private static void Require(OwnedAttempt e) { uint error=Error(e);if(!e.Succeeded)throw new OwnedServiceException(error!=0?error:22); }
    private void Fail(Slot s,uint code,Exception? error=null) { Primary(s,code,error);s.Diagnostic.Terminal=true; }
    private static void Alias(string alias) { if(alias==null||alias.Length==0||alias.Length>255||alias.IndexOf('\0')>=0||Codecs.Utf8(alias).Length>255)throw new ArgumentException("explicit alias1..255 UTF8 bytes without NUL required"); }
    private int Open(byte role,string source,string peer,string? query) {
        Live();Alias(source);Alias(peer);if(query!=null)Alias(query);
        if(source==peer||query==source||query==peer||(role==4&&query!=null))throw new ArgumentException("independent distinct role aliases required");
        if(used==slots.Length)throw new OwnedServiceException(90);
        for(int j=0;j<used;j++)for(int k=0;k<3;k++) { string? old=slots[j].Aliases[k];if(old!=null&&(old==source||old==peer||old==query))throw new OwnedServiceException(13); }
        businessVersionCall.Operation=OwnedOperation.BusinessVersion;Execute(businessVersionCall);
        if(!businessVersion.Returned||businessVersion.Code!=1||businessVersion.Exception!=null||businessVersion.CleanupException!=null)throw new OwnedServiceException(95);
        versionCall.Operation=OwnedOperation.Version;Execute(versionCall);
        if(!version.Returned||version.Code!=1||version.Exception!=null||version.CleanupException!=null)throw new OwnedServiceException(95);
        // ABI discovery returns the version as its scalar, not errno0.
        int index=used++;var s=slots[index];s.Occupied=true;s.Aliases[0]=source;s.Aliases[1]=peer;s.Aliases[2]=query;
        try {
            for(int k=0;k<3;k++) { string? alias=s.Aliases[k];if(alias==null)continue;s.AliasBytes[k]=Codecs.Utf8(alias);s.Owners[k]=session.ReserveOwnedOwner(alias); }
            for(int k=0;k<3;k++) { string? alias=s.Aliases[k];if(alias==null)continue;
                var cap=s.Owners[k]!;var call=s.OpenCalls[k];Array.Fill(ticket,(byte)0xa5);
                call.Operation=OwnedOperation.Open;call.Input=s.AliasBytes[k]!;call.InputLength=call.Input.Length;call.Output=ticket;Execute(call);
                // Save a genuinely returned success in the pre-reserved actual
                // Capability BEFORE throwing/allocating for any later condition.
                if(call.Evidence.Returned&&call.Evidence.Code==0) { ulong id=Wire.U(ticket,0,8);s.Diagnostic.returnedOpenHandles[k]=id;cap.Adopt(id);s.Diagnostic.remaining[k]=id; }
                Require(call.Evidence);
                if(!Wire.FullHandle(cap.Handle))throw Wire.Invalid();
                if(k==0)s.Diagnostic.Source=cap.Handle;else if(k==1)s.Diagnostic.Peer=cap.Handle;else s.Diagnostic.Query=cap.Handle;
                for(int n=0;n<k;n++)if(s.Owners[n]?.Handle==cap.Handle)throw Wire.Invalid();
            }
            Array.Fill(receipt,(byte)0xa5);Run(s,s.Activate,role==3?OwnedOperation.ActivateClient:OwnedOperation.ActivateService,output:receipt);Require(s.Diagnostic.Activation);
            var observed=new EndpointReceipt(receipt,1,role,s.Diagnostic.Source);s.Diagnostic.Receipt=observed;s.Diagnostic.HasReceipt=true;
            Buffer.BlockCopy(receipt,0,s.ReceiptBytes,0,256);return index;
        } catch(Exception error) {
            Fail(s,error is OwnedServiceException e?e.Code:22,error);CloseSlot(s,null);throw;
        }
    }
    public Client<Q,S> OpenClient<Q,S>(ServiceDefinition<Q,S> definition,string callAlias,string observerAlias,string? availabilityAlias=null) {
        if(definition==null)throw new ArgumentNullException(nameof(definition));var view=new Client<Q,S>(this,definition);Enter();
        try { Bounds(definition);view.Index=Open(3,callAlias,observerAlias,availabilityAlias);return view; }finally { Leave(); }
    }
    public Server<Q,S> OpenServer<Q,S>(ServiceDefinition<Q,S> definition,string advertiseAlias,string responderAlias) {
        if(definition==null)throw new ArgumentNullException(nameof(definition));var view=new Server<Q,S>(this,definition);Enter();
        try { Bounds(definition);view.Index=Open(4,advertiseAlias,responderAlias,null);return view; }finally { Leave(); }
    }
    private void Bounds<Q,S>(ServiceDefinition<Q,S> d) { if(d.MaximumRequest>maximumRequest||d.MaximumResponse>maximumResponse)throw new OwnedServiceException(90); }
    private void Inspect(Slot s) {
        Array.Fill(receipt,(byte)0xa5);Run(s,s.Inspect,OwnedOperation.Inspect,output:receipt);
        if(!s.Diagnostic.Inspection.Succeeded) { if(s.Diagnostic.Inspection.Entered)Fail(s,Error(s.Diagnostic.Inspection));Require(s.Diagnostic.Inspection); }
        try { _=new EndpointReceipt(receipt,2,s.Diagnostic.Receipt.Role,s.Diagnostic.Source);
            for(int i=0;i<256;i++)if(i!=6&&receipt[i]!=s.ReceiptBytes[i])throw Wire.Invalid();
        }catch(Exception error) { Fail(s,13,error);throw; }
    }
    private int Find(Slot s,ulong value) { for(int i=0;i<s.Used;i++)if(s.Rows[i].Ticket==value&&value!=0)return i;return -1; }
    private Transaction Check(Slot s,Ticket t,int index) { if(t.Endpoint!=index||t.Source!=s.Diagnostic.Source||t.Row<0||t.Row>=s.Used||t.Value==0||s.Rows[t.Row].Ticket!=t.Value)throw new OwnedServiceException(9);return s.Rows[t.Row]; }
    internal Ticket Submit<Q,S>(int index,ServiceDefinition<Q,S> d,Q value) {
        Enter();try { var s=Active(index);if(s.Used==s.Rows.Length)throw new OwnedServiceException(90);
            byte[] encoded=d.Request.Encode(value);if(encoded==null||encoded.Length>d.MaximumRequest)throw new OwnedServiceException(90);byte[] wire=(byte[])encoded.Clone();Inspect(s);
            int row=s.Used;var tx=s.Rows[row];tx.State=RequestState.Reserved;Array.Fill(ticket,(byte)0xa5);
            Run(s,s.Submit,OwnedOperation.Submit,wire,wire.Length,ticket);
            var e=s.Diagnostic.Submit;
            if(!e.Entered) { tx.State=RequestState.Unused;Require(e); }
            s.Used++; // This preallocated ledger row is never reused after attempt.
            if(!e.Succeeded) { tx.State=RequestState.Unknown;Fail(s,Error(e),e.Exception);Require(e); }
            ulong id=Wire.U(ticket,0,8);if(id==0||Find(s,id)>=0) { tx.State=RequestState.Unknown;Fail(s,13);throw new OwnedServiceException(13); }
            tx.Ticket=id;tx.State=RequestState.Admitted;return new Ticket(index,row,s.Diagnostic.Source,id);
        }finally { Leave(); }
    }
    internal RequestState State(int index,Ticket ticketValue) { Enter();try { var s=Owner(index);return Check(s,ticketValue,index).State; }finally { Leave(); } }
    internal void Abandon(int index,Ticket ticketValue) {
        Enter();try { var s=Active(index);var tx=Check(s,ticketValue,index);if(tx.State!=RequestState.Admitted)throw new OwnedServiceException(9);
            s.Abandon.Ticket=tx.Ticket;Run(s,s.Abandon,OwnedOperation.Abandon);var e=s.Diagnostic.Abandon;
            if(e.Entered)tx.State=e.Succeeded?RequestState.Abandoned:RequestState.Unknown;
            if(!e.Succeeded) { if(e.Entered)Fail(s,Error(e),e.Exception);Require(e); }
        }finally { Leave(); }
    }
    internal bool Available(int index,ulong wait) {
        Enter();try { var s=Active(index);if(s.Diagnostic.Query==0||wait>60000000000UL)throw new OwnedServiceException(22);Inspect(s);
            availability[0]=0xa5;s.Available.Wait=wait;Run(s,s.Available,OwnedOperation.Available,output:availability,peer:s.Diagnostic.Query);
            if(!s.Diagnostic.Available.Succeeded) { if(s.Diagnostic.Available.Entered)Fail(s,Error(s.Diagnostic.Available),s.Diagnostic.Available.Exception);Require(s.Diagnostic.Available); }
            if(availability[0]>1) { Fail(s,13);throw Wire.Invalid(); }return availability[0]!=0;
        }finally { Leave(); }
    }
    private void Ack(Slot s,Borrow borrow,bool cancel) {
        var c=cancel?s.Cancel:s.Commit;c.Sequence=borrow.Sequence;c.Nonce=borrow.Nonce;Run(s,c,cancel?OwnedOperation.Cancel:OwnedOperation.Commit);
        if(!c.Evidence.Succeeded) { Fail(s,Error(c.Evidence),c.Evidence.Exception);Require(c.Evidence); }
        if(!cancel)s.LastCommitted=borrow.Sequence;
    }
    internal Delivery<T> Poll<T>(int index,ICodec<T> codec,int maximum) {
        // The delivery/result container exists before the first possible peek.
        var delivery=new Delivery<T>();Enter();try { return PollCore(index,codec,maximum,delivery); }finally { Leave(); }
    }
    private Delivery<T> PollCore<T>(int index,ICodec<T> codec,int maximum,Delivery<T> delivery) {
        var s=Active(index);if(s.LastCommitted==ulong.MaxValue||s.LastNonce==ulong.MaxValue)throw new OwnedServiceException(90);
        Inspect(s);Array.Fill(packet,(byte)0xa5);Run(s,s.Peek,OwnedOperation.Peek,output:packet);var peek=s.Diagnostic.Peek;
        if(peek.Returned&&peek.Code==11&&peek.Exception==null&&peek.CleanupException==null)return delivery;
        if(!peek.Succeeded) { if(peek.Entered)Fail(s,Error(peek),peek.Exception);Require(peek); }
        Borrow borrow;
        try { borrow=new Borrow(packet,s.Diagnostic.Receipt,s.Diagnostic.Peer,maximum);
            if(borrow.Sequence<=s.LastCommitted||borrow.Sequence<s.LastSequence||borrow.Nonce<=s.LastNonce||
                (s.Diagnostic.Receipt.Role==3&&borrow.Sequence!=s.LastCommitted+1))throw Wire.Invalid();
            s.LastSequence=borrow.Sequence;s.LastNonce=borrow.Nonce;
        }catch(Exception error) { Fail(s,13,error);throw; }
        int row=Find(s,borrow.Ticket);Transaction? tx=row>=0?s.Rows[row]:null;bool abandoned=false;
        try {
            if(borrow.Control) {
                if(borrow.Ticket!=0&&(tx==null||(s.Diagnostic.Receipt.Role==3?tx.State!=RequestState.Admitted&&tx.State!=RequestState.Abandoned:tx.State!=RequestState.Delivered)))throw Wire.Invalid();
                delivery.IsControl=true;delivery.Control=borrow.Event;
            } else {
                if(s.Diagnostic.Receipt.Role==3) {
                    if(tx==null||(tx.State!=RequestState.Admitted&&tx.State!=RequestState.Abandoned))throw Wire.Invalid();abandoned=tx.State==RequestState.Abandoned;
                } else {
                    if(row>=0||s.Used==s.Rows.Length)throw new OwnedServiceException(90);row=s.Used;tx=s.Rows[row];
                    for(int i=0;i<s.Used;i++) { var old=s.Rows[i].Information;
                        if(old.WriterGuid0==borrow.Information.WriterGuid0&&old.WriterGuid1==borrow.Information.WriterGuid1&&old.NativeSequence==borrow.Information.NativeSequence)throw Wire.Invalid();
                    }
                    // Reserve the local response row BEFORE ACK. Never lose a committed request to allocation.
                    tx.Ticket=borrow.Ticket;tx.State=RequestState.Reserved;tx.Information=borrow.Information;s.Used++;
                }
                byte[] body=new byte[borrow.BodyLength];Buffer.BlockCopy(packet,352,body,0,body.Length);
                T value=codec.Decode(body);delivery.Value=value;delivery.Information=borrow.Information;
                delivery.Ticket=new Ticket(index,row,s.Diagnostic.Source,borrow.Ticket);
            }
        }catch(Exception error) {
            Primary(s,error is OwnedServiceException e?e.Code:error is OutOfMemoryException?12u:22u,error);
            try { Ack(s,borrow,true); }catch(Exception) { /* independent cancel witness already retained */ }
            s.Diagnostic.Terminal=true;throw;
        }
        // Everything needed to expose the full decoded value already exists.
        try { Ack(s,borrow,false); }catch { if(tx!=null&&!abandoned)tx.State=RequestState.Unknown;throw; }
        if(borrow.Control) {
            if(tx!=null&&tx.State!=RequestState.Abandoned) { tx.State=RequestState.Completed;tx.Error=borrow.Event.Error; }
            if(borrow.Event.Terminal) { Fail(s,borrow.Event.Error!=0?borrow.Event.Error:125);for(int i=0;i<s.Used;i++)if(s.Rows[i].State==RequestState.Admitted||s.Rows[i].State==RequestState.Delivered)s.Rows[i].State=RequestState.Abandoned; }
            delivery.Present=true;return delivery;
        }
        tx!.Information=borrow.Information;
        if(abandoned) { tx.State=RequestState.Discarded;delivery.Value=default;return delivery; }
        tx.State=s.Diagnostic.Receipt.Role==3?RequestState.Completed:RequestState.Delivered;delivery.Present=true;return delivery;
    }
    internal void Respond<Q,S>(int index,ServiceDefinition<Q,S> d,Ticket ticketValue,S value) {
        Enter();try { RespondCore(index,d,ticketValue,value); }finally { Leave(); }
    }
    private void RespondCore<Q,S>(int index,ServiceDefinition<Q,S> d,Ticket ticketValue,S value) {
        var s=Active(index);var tx=Check(s,ticketValue,index);if(tx.State!=RequestState.Delivered)throw new OwnedServiceException(9);
        byte[] encoded=d.Response.Encode(value);if(encoded==null||encoded.Length>d.MaximumResponse)throw new OwnedServiceException(90);byte[] bytes=(byte[])encoded.Clone();Inspect(s);
        s.Respond.Ticket=ticketValue.Value;Run(s,s.Respond,OwnedOperation.Respond,bytes,bytes.Length);var e=s.Diagnostic.Respond;
        if(e.Entered)tx.State=e.Succeeded?RequestState.Completed:RequestState.Unknown;
        if(!e.Succeeded) { if(e.Entered)Fail(s,Error(e),e.Exception);Require(e); }
    }
    internal bool ServeOnce<Q,S>(int index,ServiceDefinition<Q,S> d,Func<Q,ServiceInformation,S> handler) {
        if(handler==null)throw new ArgumentNullException(nameof(handler));var delivery=new Delivery<Q>();Enter();
        try { var got=PollCore(index,d.Request,d.MaximumRequest,delivery);if(!got.Present)return false;
            if(got.IsControl)throw new OwnedServiceException(got.Control.Error!=0?got.Control.Error:125);
            S response;
            try { response=handler(got.Value!,got.Information); }catch(Exception error) { Fail(Owner(index),5,error);throw; }
            RespondCore(index,d,got.Ticket,response);return true;
        }finally { Leave(); }
    }
    public EndpointDiagnostic Diagnostic(int index) { Enter();try { return Owner(index).Diagnostic; }finally { Leave(); } }
    private void CloseSlot(Slot s,Action? hook) {
        var d=s.Diagnostic;d.Terminal=true;d.CleanupRequested=true;
        for(int i=0;i<s.Used;i++)if(s.Rows[i].State==RequestState.Admitted||s.Rows[i].State==RequestState.Delivered||s.Rows[i].State==RequestState.Reserved) { s.Rows[i].State=RequestState.Abandoned;s.Rows[i].Error=125; }
        if(s.Closing)return;s.Closing=true;
        try {
            if(hook!=null&&!s.HookInvoked) { s.HookInvoked=true;try { hook(); }catch(Exception error) { d.CloseHookException=error; } }
            if(d.HasReceipt&&!d.NativeRetire.Entered) {
                if(d.retireCount==s.Retire.Length)Primary(s,90);
                else { var frame=s.Retire[d.retireCount++];Run(s,frame,OwnedOperation.Retire); }
            }
            for(int i=2;i>=0;i--) { var owner=s.Owners[i];if(owner==null||owner.Handle==0)continue;
                if(d.closeCount==d.closes.Length) { Primary(s,90);continue; }
                int row=d.closeCount++;d.closes[row].FullHandle=owner.Handle;
                try { d.closes[row].Result=owner.CloseChecked(); }catch(Exception error) { d.closes[row].Exception=error; }
                d.closes[row].Retirement=owner.LastRetirement;
                d.remaining[i]=owner.Handle;
            }
        }finally { s.Closing=false; }
    }
    internal EndpointDiagnostic Close(int index,Action? hook) { Enter();try { var s=Owner(index);CloseSlot(s,hook);return s.Diagnostic; }finally { Leave(); } }
    public bool CloseAll() {
        Enter();try { shutdown=true;bool ok=true;for(int i=used-1;i>=0;i--) { CloseSlot(slots[i],null);var d=slots[i].Diagnostic;
                if(!d.CleanupSucceeded||d.PrimaryCode!=0||d.PrimaryException!=null)ok=false;for(int j=0;j<3;j++)if(slots[i].Owners[j]?.Handle>0)ok=false;
            }return ok;
        }finally { Leave(); }
    }
    public void Dispose() { if(!CloseAll())throw new OwnedServiceException(5); }
}
public sealed class Client<Q,S> {
    private readonly OwnedGroup group;private readonly ServiceDefinition<Q,S> definition;internal int Index=-1;
    internal Client(OwnedGroup group,ServiceDefinition<Q,S> definition) { this.group=group;this.definition=definition; }
    public Ticket Submit(Q value)=>group.Submit(Index,definition,value);
    public RequestState State(Ticket value)=>group.State(Index,value);
    public void Abandon(Ticket value)=>group.Abandon(Index,value);
    public Delivery<S> Poll()=>group.Poll(Index,definition.Response,definition.MaximumResponse);
    public bool Available(ulong waitNanoseconds)=>group.Available(Index,waitNanoseconds);
    public EndpointDiagnostic Diagnostic=>group.Diagnostic(Index);
    public EndpointDiagnostic Close(Action? hook=null)=>group.Close(Index,hook);
}
public sealed class Server<Q,S> {
    private readonly OwnedGroup group;private readonly ServiceDefinition<Q,S> definition;internal int Index=-1;
    internal Server(OwnedGroup group,ServiceDefinition<Q,S> definition) { this.group=group;this.definition=definition; }
    public Delivery<Q> Poll()=>group.Poll(Index,definition.Request,definition.MaximumRequest);
    public void Respond(Ticket ticket,S value)=>group.Respond(Index,definition,ticket,value);
    public bool ServeOnce(Func<Q,ServiceInformation,S> handler)=>group.ServeOnce(Index,definition,handler);
    public EndpointDiagnostic Diagnostic=>group.Diagnostic(Index);
    public EndpointDiagnostic Close(Action? hook=null)=>group.Close(Index,hook);
}
