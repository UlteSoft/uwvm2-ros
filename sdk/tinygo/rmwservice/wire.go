// Package rmwservice implements only the explicit owned-native ROS2 Service ABI.
// Wire values are observations, never permission or fabricated native identities.
package rmwservice

import "encoding/binary"

const (receiptBytes=256; overhead=352; ipcMaximum=4194304; inputMaximum=ipcMaximum-518-160)
type Binding struct { Source, Instance, Generation, Session uint64 }
type Duration struct { Seconds, Nanoseconds uint64 }
// QoS is the canonical UMRE vocabulary, not the native enum ABI. History1/2 is
// keep-last/keep-all; reliability1/2 reliable/best-effort; durability1/2
// transient-local/volatile; liveliness1/2 automatic/manual-by-topic. All eight
// domains plus avoid-namespace are retained. Durations here are normalized wire.
type QoS struct {
 History,Reliability,Durability,Liveliness uint32
 Depth uint64
 Deadline,Lifespan,Lease Duration
 AvoidNamespace bool
}
type Receipt struct { Binding Binding; Incarnation uint64; Role,GIDBytes uint8; Actual [2]QoS; GID [32]byte }
// Metadata belongs to exactly the same take as its decoded complete body.
// Native signed values preserve every bit; zero GUID bytes are legitimate data.
type Metadata struct {
 Kind,ControlFlags,WriterBytes uint8
 Error uint32
 Ticket,Sequence,Reservation uint64
 SourceTimestamp,ReceivedTimestamp,RequestSequence int64
 Writer [16]byte
}
type record struct { Metadata; body []byte }
func u16(b []byte)uint16{return binary.LittleEndian.Uint16(b)}
func u32(b []byte)uint32{return binary.LittleEndian.Uint32(b)}
func u64(b []byte)uint64{return binary.LittleEndian.Uint64(b)}
func zero(b []byte)bool {for _,v:=range b {if v!=0{return false}};return true}
func fullHandle(v uint64)bool{return uint32(v)!=0&&uint32(v>>32)!=0}
func binding(b []byte)Binding{return Binding{u64(b[16:]),u64(b[24:]),u64(b[32:]),u64(b[40:])}}
func (b Binding)valid()bool{return fullHandle(b.Source)&&b.Instance!=0&&b.Generation!=0&&b.Session!=0}
func qos(b []byte)(QoS,bool){
 q:=QoS{History:u32(b),Reliability:u32(b[4:]),Durability:u32(b[8:]),Liveliness:u32(b[12:]),Depth:u64(b[16:]),Deadline:Duration{u64(b[24:]),u64(b[32:])},Lifespan:Duration{u64(b[40:]),u64(b[48:])},Lease:Duration{u64(b[56:]),u64(b[64:])},AvoidNamespace:b[72]!=0}
 return q,b[72]<=1&&zero(b[73:80])&&q.valid()
}
func(q QoS)valid()bool{return q.History>=1&&q.History<=2&&q.Reliability>=1&&q.Reliability<=2&&q.Durability>=1&&q.Durability<=2&&q.Liveliness>=1&&q.Liveliness<=2&&(q.History!=1||q.Depth!=0)&&q.Deadline.Nanoseconds<1000000000&&q.Lifespan.Nanoseconds<1000000000&&q.Lease.Nanoseconds<1000000000}
// parseReceipt returns a cleanup-usable binding only after its fixed identity
// prefix is structurally complete. Refused full receipts remain diagnostic DATA.
func parseReceipt(b []byte,operation,role uint8,source uint64)(Receipt,uint32){
 var r Receipt
 if len(b)!=256||u32(b)!=0x45524d55||u16(b[4:])!=1||b[6]!=operation||b[7]!=2||u32(b[8:])!=256||b[12]!=role||b[13]!=2||b[15]!=0||!zero(b[56:64]) {return r,22}
 r.Binding=binding(b);r.Incarnation=u64(b[48:]);r.Role=role
 if !r.Binding.valid()||r.Binding.Source!=source||r.Incarnation==0{return Receipt{},22}
 if b[14]>32||(role==4&&b[14]!=0)||!zero(b[224+int(b[14]):256]){return r,22}
 var valid bool;r.Actual[0],valid=qos(b[64:144]);if !valid{return r,22};r.Actual[1],valid=qos(b[144:224]);if !valid{return r,22}
 r.GIDBytes=b[14];copy(r.GID[:],b[224:256]);return r,0
}
// parseRecord checks the complete UMRC160 plus SAME-record URMI192 before any
// decoder or ACK. A false borrow flag means no trustworthy token can be ACKed;
// retire the endpoint instead of inventing an ACK. Missing metadata95 never pops.
func parseRecord(b []byte,r Receipt,peer uint64,writer uint8,maximum int)(record,uint32,bool){
 var d record
 if len(b)<overhead||len(b)>ipcMaximum||u32(b)!=0x43524d55||u16(b[4:])!=1||b[6]!=5||b[7]!=2||u32(b[8:])!=160||b[12]!=r.Role||b[13]!=writer||!zero(b[14:16])||!zero(b[128:160])||binding(b)!=r.Binding||u64(b[48:])!=r.Incarnation||u64(b[56:])!=peer||u64(b[72:])==0||u64(b[80:])==0||u64(b[88:])!=0||u64(b[96:])!=uint64(len(b))||u64(b[112:])!=0||u64(b[120:])!=0{return d,22,false}
 n:=u64(b[104:]);if n<192||n>uint64(len(b)-160)||n-192>uint64(maximum){return d,90,false}
 d.Ticket=u64(b[64:]);d.Sequence=u64(b[72:]);d.Reservation=u64(b[80:]);d.body=b[352:160+int(n)]
 m:=b[160:352]
 if u32(m)!=0x494d5255||u16(m[4:])!=1||m[7]!=2||u32(m[8:])!=192||binding(m)!=r.Binding||u64(m[48:])!=r.Incarnation||u64(m[56:])!=d.Sequence||u64(m[64:])!=d.Ticket||u64(m[112:])!=uint64(len(d.body))||m[123]!=0||!zero(m[176:192]){return d,22,true}
 d.Kind=m[6];d.ControlFlags=m[122];d.Error=u32(m[124:]);flags:=u32(m[12:])
 if d.Kind==6||d.Kind==7 {
  if flags!=0||len(d.body)!=0||m[120]!=0||m[121]!=0||!zero(m[128:176])||u64(m[72:])!=0||u64(m[80:])!=0||u64(m[88:])!=^uint64(0)||u64(m[96:])!=^uint64(0)||u64(m[104:])!=0||d.ControlFlags & ^uint8(3)!=0||(d.Kind==6&&d.Error==0)||(d.Kind==7&&d.ControlFlags&1==0){return d,22,true};return d,0,true
 }
 expected:=uint8(2);if r.Role==3{expected=3}
 if d.Kind!=expected||d.Ticket==0||flags & ^uint32(35)!=0||d.Error!=0||d.ControlFlags!=0||m[120]!=0||!zero(m[128:160])||m[121]>16||!zero(m[160+int(m[121]):176])||u64(m[88:])!=^uint64(0)||u64(m[96:])!=^uint64(0){return d,22,true}
 if (flags&32!=0)!=(m[121]!=0)||(flags&1==0&&u64(m[72:])!=0)||(flags&2==0&&u64(m[80:])!=0)||(flags&32==0&&u64(m[104:])!=0){return d,22,true}
 if flags!=35||m[121]!=writer{return d,95,true}
 d.WriterBytes=m[121];copy(d.Writer[:],m[160:176]);d.SourceTimestamp=int64(u64(m[72:]));d.ReceivedTimestamp=int64(u64(m[80:]));d.RequestSequence=int64(u64(m[104:]));return d,0,true
}
