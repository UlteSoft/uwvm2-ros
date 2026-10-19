package rmwservice

// Synthetic wire contracts exercise the published parser. These bytes are not
// native identities, real receipts, permission grants or Native qualification.
import (
 "encoding/binary"
 "testing"
)
func put(b []byte,n uint64,width int){switch width{case 2:binary.LittleEndian.PutUint16(b,uint16(n));case 4:binary.LittleEndian.PutUint32(b,uint32(n));case 8:binary.LittleEndian.PutUint64(b,n)}}
func fixtureQoS()QoS{return QoS{History:1,Reliability:1,Durability:2,Liveliness:1,Depth:10,Deadline:Duration{0,0},Lifespan:Duration{0,0},Lease:Duration{0,0}}}
func fixtureReceipt(source uint64,role uint8)[256]byte{
 var b [256]byte;put(b[:],0x45524d55,4);put(b[4:],1,2);b[6]=1;b[7]=2;put(b[8:],256,4);b[12]=role;b[13]=2
 put(b[16:],source,8);put(b[24:],7,8);put(b[32:],8,8);put(b[40:],9,8);put(b[48:],10,8)
 for _,o:=range []int{64,144}{put(b[o:],1,4);put(b[o+4:],1,4);put(b[o+8:],2,4);put(b[o+12:],1,4);put(b[o+16:],10,8)};return b
}
func fixturePacket(r Receipt,peer,ticket,sequence,nonce uint64,body []byte)[]byte{
 b:=make([]byte,overhead+len(body));put(b,0x43524d55,4);put(b[4:],1,2);b[6]=5;b[7]=2;put(b[8:],160,4);b[12]=r.Role;b[13]=16
 for _,o:=range []int{0,160}{put(b[o+16:],r.Binding.Source,8);put(b[o+24:],r.Binding.Instance,8);put(b[o+32:],r.Binding.Generation,8);put(b[o+40:],r.Binding.Session,8);put(b[o+48:],r.Incarnation,8)}
 put(b[56:],peer,8);put(b[64:],ticket,8);put(b[72:],sequence,8);put(b[80:],nonce,8);put(b[96:],uint64(len(b)),8);put(b[104:],uint64(192+len(body)),8)
 m:=b[160:];put(m,0x494d5255,4);put(m[4:],1,2);m[6]=3;if r.Role==4{m[6]=2};m[7]=2;put(m[8:],192,4);put(m[12:],35,4);put(m[56:],sequence,8);put(m[64:],ticket,8)
 put(m[72:],^uint64(0),8);put(m[80:],uint64(1)<<63,8);put(m[88:],^uint64(0),8);put(m[96:],^uint64(0),8);put(m[104:],^uint64(0),8);put(m[112:],uint64(len(body)),8);m[121]=16;copy(b[352:],body);return b
}
func admittedFixture(t *testing.T,role uint8)Receipt{t.Helper();b:=fixtureReceipt(0xffffffffffffffff,role);r,n:=parseReceipt(b[:],1,role,^uint64(0));if n!=0{t.Fatal(n)};return r}
func TestReceiptCompleteZeroGIDAndFullHandle(t *testing.T){b:=fixtureReceipt(^uint64(0),3);b[14]=32;r,n:=parseReceipt(b[:],1,3,^uint64(0));if n!=0||r.GIDBytes!=32||!zero(r.GID[:]){t.Fatal(r,n)}}
func TestReceiptServiceCannotClaimClientGID(t *testing.T){b:=fixtureReceipt(^uint64(0),4);b[14]=1;if _,n:=parseReceipt(b[:],1,4,^uint64(0));n!=22{t.Fatal(n)}}
func TestReceiptAllQoSDomainsAndReserved(t *testing.T){for _,at:=range []int{64,68,72,76,136,143}{b:=fixtureReceipt(^uint64(0),3);b[at]=255;if _,n:=parseReceipt(b[:],1,3,^uint64(0));n!=22{t.Fatal(at,n)}}}
func TestReceiptNormalizedWireDurations(t *testing.T){for _,at:=range []int{96,112,128,176,192,208}{b:=fixtureReceipt(^uint64(0),3);put(b[at:],1000000000,8);if _,n:=parseReceipt(b[:],1,3,^uint64(0));n!=22{t.Fatal(at,n)}}}
func TestRecordFullOpaqueTicketZeroWriterSignedTimes(t *testing.T){r:=admittedFixture(t,3);b:=fixturePacket(r,0x200000002,^uint64(0),1,1,[]byte{4});d,n,borrow:=parseRecord(b,r,0x200000002,16,1);if n!=0||!borrow||d.Ticket!=^uint64(0)||d.SourceTimestamp!=-1||d.ReceivedTimestamp!=(-9223372036854775807-1)||d.RequestSequence!=-1||!zero(d.Writer[:]){t.Fatal(d,n,borrow)}}
func TestRecordSameTakeIdentityEveryDomain(t *testing.T){r:=admittedFixture(t,3);for _,at:=range []int{16,24,32,40,48,56,64,72,80,176,184,192,200,208,216,224}{b:=fixturePacket(r,0x200000002,4,1,1,nil);b[at]^=1;if _,n,_:=parseRecord(b,r,0x200000002,16,0);n==0{t.Fatal(at)}}}
func TestRecordMissingNativeFact95WithoutBodyMutation(t *testing.T){r:=admittedFixture(t,3);b:=fixturePacket(r,0x200000002,4,1,1,[]byte{7});put(b[172:],34,4);put(b[232:],0,8);d,n,borrow:=parseRecord(b,r,0x200000002,16,1);if n!=95||!borrow||len(d.body)!=1||d.body[0]!=7{t.Fatal(n,borrow)}}
func TestRecordShortOversizeAndNoInventedACK(t *testing.T){r:=admittedFixture(t,3);b:=fixturePacket(r,0x200000002,4,1,1,[]byte{1,2});if _,n,borrow:=parseRecord(b,r,0x200000002,16,1);n!=90||borrow{t.Fatal(n,borrow)};if _,n,borrow:=parseRecord(b[:351],r,0x200000002,16,1);n!=22||borrow{t.Fatal(n,borrow)}}
func TestRecordLogicalEmptyStaysZero(t *testing.T){r:=admittedFixture(t,4);b:=fixturePacket(r,0x200000002,4,1,1,nil);d,n,borrow:=parseRecord(b,r,0x200000002,16,0);if n!=0||!borrow||len(d.body)!=0{t.Fatal(n,borrow)}}
func TestControlsActualFlagsReasonAndNoFakeInfo(t *testing.T){r:=admittedFixture(t,3);b:=fixturePacket(r,0x200000002,0,1,1,nil);m:=b[160:];m[6]=7;put(m[12:],0,4);put(m[72:],0,8);put(m[80:],0,8);put(m[104:],0,8);m[121]=0;m[122]=3;put(m[124:],125,4);d,n,ok:=parseRecord(b,r,0x200000002,16,0);if n!=0||!ok||d.Error!=125||d.ControlFlags!=3||d.WriterBytes!=0{t.Fatal(d,n,ok)};m[122]=2;if _,n,_=parseRecord(b,r,0x200000002,16,0);n!=22{t.Fatal(n)}}
