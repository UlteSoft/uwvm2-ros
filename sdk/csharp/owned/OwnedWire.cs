using System;
namespace Uwvm.RosWasm.Owned;

public readonly struct ActualQos {
    public readonly uint History,Reliability,Durability,Liveliness;
    public readonly ulong Depth,DeadlineSeconds,DeadlineNanoseconds,LifespanSeconds,LifespanNanoseconds,LeaseSeconds,LeaseNanoseconds;
    public readonly bool AvoidRosNamespaceConventions;
    internal ActualQos(byte[] b,int p) {
        History=(uint)Wire.U(b,p,4);Reliability=(uint)Wire.U(b,p+4,4);Durability=(uint)Wire.U(b,p+8,4);Liveliness=(uint)Wire.U(b,p+12,4);
        Depth=Wire.U(b,p+16,8);DeadlineSeconds=Wire.U(b,p+24,8);DeadlineNanoseconds=Wire.U(b,p+32,8);
        LifespanSeconds=Wire.U(b,p+40,8);LifespanNanoseconds=Wire.U(b,p+48,8);LeaseSeconds=Wire.U(b,p+56,8);LeaseNanoseconds=Wire.U(b,p+64,8);
        AvoidRosNamespaceConventions=b[p+72]!=0;
        if(History<1||History>2||Reliability<1||Reliability>2||Durability<1||Durability>2||Liveliness<1||Liveliness>2||
            (History==1&&Depth==0)||DeadlineNanoseconds>=1000000000UL||LifespanNanoseconds>=1000000000UL||LeaseNanoseconds>=1000000000UL||b[p+72]>1||!Wire.Zero(b,p+73,7))throw Wire.Invalid();
    }
}
public readonly struct EndpointReceipt {
    public readonly ulong Source,Instance,RuntimeGeneration,Session,Incarnation;
    public readonly byte Role,NativeGidBytes;
    public readonly ActualQos RequestQos,ResponseQos;
    // A full 32-byte native endpoint GID is not a 16-byte service writer GUID.
    public readonly ulong NativeGid0,NativeGid1,NativeGid2,NativeGid3;
    internal EndpointReceipt(byte[] b,byte operation,byte role,ulong source) {
        if(b.Length!=256||Wire.U(b,0,4)!=0x45524d55||Wire.U(b,4,2)!=1||b[6]!=operation||b[7]!=2||Wire.U(b,8,4)!=256||
            b[12]!=role||(role!=3&&role!=4)||b[13]!=2||b[14]>32||b[15]!=0||!Wire.Zero(b,56,8)||
            (role==4&&b[14]!=0)||!Wire.Zero(b,224+b[14],32-b[14]))throw Wire.Invalid();
        Source=Wire.U(b,16,8);Instance=Wire.U(b,24,8);RuntimeGeneration=Wire.U(b,32,8);Session=Wire.U(b,40,8);Incarnation=Wire.U(b,48,8);
        if(Source!=source||!Wire.FullHandle(source)||Instance==0||RuntimeGeneration==0||Session==0||Incarnation==0)throw Wire.Invalid();
        Role=role;NativeGidBytes=b[14];RequestQos=new ActualQos(b,64);ResponseQos=new ActualQos(b,144);
        NativeGid0=Wire.U(b,224,8);NativeGid1=Wire.U(b,232,8);NativeGid2=Wire.U(b,240,8);NativeGid3=Wire.U(b,248,8);
    }
    internal bool Matches(byte[] b,int p)=>b[p+7]==2&&Wire.U(b,p+16,8)==Source&&Wire.U(b,p+24,8)==Instance&&Wire.U(b,p+32,8)==RuntimeGeneration&&Wire.U(b,p+40,8)==Session&&Wire.U(b,p+48,8)==Incarnation;
}
public readonly struct ServiceInformation {
    public readonly long SourceTimestamp,ReceivedTimestamp,NativeSequence;
    public readonly ulong WriterGuid0,WriterGuid1;
    internal ServiceInformation(byte[] b,int p) { SourceTimestamp=unchecked((long)Wire.U(b,p+72,8));ReceivedTimestamp=unchecked((long)Wire.U(b,p+80,8));NativeSequence=unchecked((long)Wire.U(b,p+104,8));WriterGuid0=Wire.U(b,p+160,8);WriterGuid1=Wire.U(b,p+168,8); }
}
public readonly struct SourceControl {
    public readonly byte Kind,Flags;public readonly uint Error;public readonly ulong Ticket;
    internal SourceControl(byte kind,byte flags,uint error,ulong ticket) { Kind=kind;Flags=flags;Error=error;Ticket=ticket; }
    public bool Terminal=>(Flags&1)!=0;
}
internal readonly struct Borrow {
    internal readonly ulong Ticket,Sequence,Nonce;internal readonly int BodyLength;internal readonly bool Control;
    internal readonly ServiceInformation Information;internal readonly SourceControl Event;
    internal Borrow(byte[] b,EndpointReceipt r,ulong peer,int maximum) {
        if(b.Length<352||Wire.U(b,0,4)!=0x43524d55||Wire.U(b,4,2)!=1||b[6]!=5||Wire.U(b,8,4)!=160||b[12]!=r.Role||b[13]!=16||
            !Wire.Zero(b,14,2)||!Wire.Zero(b,128,32)||!r.Matches(b,0)||Wire.U(b,56,8)!=peer||!Wire.FullHandle(peer)||peer==r.Source||
            Wire.U(b,72,8)==0||Wire.U(b,80,8)==0||Wire.U(b,88,8)!=0||Wire.U(b,96,8)!=(ulong)b.Length||Wire.U(b,112,8)!=0||Wire.U(b,120,8)!=0)throw Wire.Invalid();
        ulong record=Wire.U(b,104,8);
        if(record<192||record>(ulong)(b.Length-160)||record-192>(ulong)maximum)throw Wire.Invalid();
        Ticket=Wire.U(b,64,8);Sequence=Wire.U(b,72,8);Nonce=Wire.U(b,80,8);BodyLength=(int)(record-192);
        int p=160;
        if(Wire.U(b,p,4)!=0x494d5255||Wire.U(b,p+4,2)!=1||Wire.U(b,p+8,4)!=192||!r.Matches(b,p)||
            Wire.U(b,p+56,8)!=Sequence||Wire.U(b,p+64,8)!=Ticket||Wire.U(b,p+112,8)!=(ulong)BodyLength||b[p+123]!=0||!Wire.Zero(b,p+176,16))throw Wire.Invalid();
        byte kind=b[p+6];uint flags=(uint)Wire.U(b,p+12,4);Control=kind==6||kind==7;Information=default;Event=default;
        if(Control) {
            uint error=(uint)Wire.U(b,p+124,4);byte eventFlags=b[p+122];
            if(flags!=0||BodyLength!=0||b[p+120]!=0||b[p+121]!=0||!Wire.Zero(b,p+128,48)||Wire.U(b,p+72,8)!=0||Wire.U(b,p+80,8)!=0||
                Wire.U(b,p+88,8)!=ulong.MaxValue||Wire.U(b,p+96,8)!=ulong.MaxValue||Wire.U(b,p+104,8)!=0||(eventFlags&~3)!=0||(kind==6&&error==0)||(kind==7&&(eventFlags&1)==0))throw Wire.Invalid();
            Event=new SourceControl(kind,eventFlags,error,Ticket);
        } else {
            if(kind!=(r.Role==3?3:2)||flags!=35||Ticket==0||b[p+121]!=16||b[p+120]!=0||b[p+122]!=0||Wire.U(b,p+124,4)!=0||
                !Wire.Zero(b,p+128,32)||Wire.U(b,p+88,8)!=ulong.MaxValue||Wire.U(b,p+96,8)!=ulong.MaxValue)throw Wire.Invalid();
            Information=new ServiceInformation(b,p);
        }
    }
}
internal static class Wire {
    internal static FormatException Invalid()=>new FormatException("complete owned RMW association/extent required");
    internal static bool FullHandle(ulong h)=>(uint)h!=0&&(uint)(h>>32)!=0;
    internal static ulong U(byte[] b,int p,int n) { if(p<0||n<0||p>b.Length-n)throw Invalid();ulong v=0;for(int i=0;i<n;i++)v|=(ulong)b[p+i]<<(8*i);return v; }
    internal static bool Zero(byte[] b,int p,int n) { if(p<0||n<0||p>b.Length-n)return false;for(int i=0;i<n;i++)if(b[p+i]!=0)return false;return true; }
}
