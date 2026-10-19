/** Closed UMRE256/UMRC160/URMI192 readers over owned copied memory.
 * No native model, pointer, timestamp, request ID or authority is fabricated.
 */
export const RECEIPT_BYTES:i32=256;
export const PREFIX_BYTES:i32=352;
export const IPC_MAX:i32=4194304;
export const INPUT_MAX:i32=4193626; // actual max_payload_bytes -518 -UMRC160
export const MAX_WAIT_NS:u64=60000000000;
export function fullHandle(h:u64):bool{return <u32>h!=0&&<u32>(h>>32)!=0;}
function zero(b:Uint8Array,at:i32,n:i32):bool{
  for(let i=at;i<at+n;++i)if(b[i]!=0)return false;return true;
}
/** Private expected full Runtime/native association. Not a bearer credential. */
export class Binding {
  source:u64=0;instance:u64=0;generation:u64=0;session:u64=0;incarnation:u64=0;
  kind:u8=0;
}
export function sameBinding(b:Uint8Array,at:i32,e:Binding):bool {
  const p=b.dataStart+at;
  return load<u8>(p+7)==2&&load<u64>(p+16)==e.source&&load<u64>(p+24)==e.instance&&
    load<u64>(p+32)==e.generation&&load<u64>(p+40)==e.session&&load<u64>(p+48)==e.incarnation;
}
function validQos(b:Uint8Array,at:i32):bool{
  const p=b.dataStart+at,h=load<u32>(p),r=load<u32>(p+4),d=load<u32>(p+8),l=load<u32>(p+12);
  return h>=1&&h<=2&&r>=1&&r<=2&&d>=1&&d<=2&&l>=1&&l<=2&&
    (h!=1||load<u64>(p+16)!=0)&&load<u64>(p+32)<1000000000&&
    load<u64>(p+48)<1000000000&&load<u64>(p+64)<1000000000&&b[at+72]<=1&&zero(b,at+73,7);
}
/** Inspect all bytes before changing the expected binding; actual allzero GID
 * is valid when its original extent is explicitly present. Two actual QoS
 * profiles are retained as wire data, never replaced by a requested profile.
 */
export function receipt(b:Uint8Array,operation:u8,kind:u8,source:u64,out:Binding):u32{
  if(b.length!=256||!fullHandle(source)||(kind!=3&&kind!=4))return 22;
  const p=b.dataStart,gid=b[14];
  if(load<u32>(p)!=0x45524d55||load<u16>(p+4)!=1||b[6]!=operation||b[7]!=2||
    load<u32>(p+8)!=256||b[12]!=kind||b[13]!=2||gid>32||b[15]||!zero(b,56,8)||
    load<u64>(p+16)!=source||!load<u64>(p+24)||!load<u64>(p+32)||!load<u64>(p+40)||
    !load<u64>(p+48)||(kind==4&&gid!=0)||!zero(b,224+gid,32-gid)||
    !validQos(b,64)||!validQos(b,144))return 22;
  out.source=source;out.instance=load<u64>(p+24);out.generation=load<u64>(p+32);
  out.session=load<u64>(p+40);out.incarnation=load<u64>(p+48);out.kind=kind;return 0;
}
/** Different operation byte is the sole permitted inspect/original difference. */
export function sameReceipt(original:Uint8Array,observed:Uint8Array):bool{
  if(original.length!=256||observed.length!=256)return false;
  for(let i=0;i<256;++i)if(i!=6&&original[i]!=observed[i])return false;return true;
}
/** Per-call preallocated output. associated means only the complete UMRC
 * header is genuine for this exact expected binding; it enables exact cancel
 * of malformed subsequent metadata. No token is guessed from a bad header.
 */
export class Delivery {
  associated:bool=false;sequence:u64=0;reservation:u64=0;ticket:u64=0;bodyBytes:i32=0;
  source:u64=0;instance:u64=0;generation:u64=0;session:u64=0;incarnation:u64=0;role:u8=0;
  kind:u8=0;flags:u8=0;error:u32=0;nativePresent:bool=false;
  sourceTimestamp:i64=0;receivedTimestamp:i64=0;nativeSequence:i64=0;
  writer:Uint8Array=new Uint8Array(16);
}
export function delivery(b:Uint8Array,expected:Binding,peer:u64,writer:u32,out:Delivery):u32{
  out.associated=false;
  if(b.length<352||b.length>IPC_MAX||writer<1||writer>16||!fullHandle(peer)||peer==expected.source)return 22;
  const p=b.dataStart,n=load<u64>(p+104);
  if(load<u32>(p)!=0x43524d55||load<u16>(p+4)!=1||b[6]!=5||load<u32>(p+8)!=160||
    b[12]!=expected.kind||b[13]!=writer||!zero(b,14,2)||!zero(b,128,32)||
    !sameBinding(b,0,expected)||load<u64>(p+56)!=peer||!load<u64>(p+72)||!load<u64>(p+80)||
    load<u64>(p+88)!=0||load<u64>(p+96)!=<u64>b.length||load<u64>(p+112)!=0||load<u64>(p+120)!=0||
    n<192||n><u64>(b.length-160))return 22;
  out.sequence=load<u64>(p+72);out.reservation=load<u64>(p+80);out.ticket=load<u64>(p+64);
  out.bodyBytes=<i32>(n-192);out.associated=true;
  const m=p+160,kind=b[166],bits=load<u32>(m+12),flags=b[282],error=load<u32>(m+124);
  if(load<u32>(m)!=0x494d5255||load<u16>(m+4)!=1||load<u32>(m+8)!=192||
    !sameBinding(b,160,expected)||load<u64>(m+56)!=out.sequence||load<u64>(m+64)!=out.ticket||
    load<u64>(m+112)!=<u64>out.bodyBytes||b[283]||!zero(b,336,16))return 22;
  if(kind==6||kind==7){
    if(bits||out.bodyBytes||b[280]||b[281]||!zero(b,288,48)||load<u64>(m+72)||load<u64>(m+80)||
      load<u64>(m+88)!=u64.MAX_VALUE||load<u64>(m+96)!=u64.MAX_VALUE||load<u64>(m+104)||
      (flags&~<u8>3)||(kind==6?error==0:(flags&1)==0))return 22;
  }else{
    if(kind!=(expected.kind==3?3:2)||!out.ticket||bits!=35||error||flags||b[280]||
      b[281]!=writer||!zero(b,288,32)||!zero(b,320+writer,16-writer)||
      load<u64>(m+88)!=u64.MAX_VALUE||load<u64>(m+96)!=u64.MAX_VALUE)return b[281]!=writer?95:22;
  }
  out.kind=kind;out.flags=flags;out.error=error;out.nativePresent=kind<6;
  out.source=expected.source;out.instance=expected.instance;out.generation=expected.generation;
  out.session=expected.session;out.incarnation=expected.incarnation;out.role=expected.kind;
  out.sourceTimestamp=load<i64>(m+72);out.receivedTimestamp=load<i64>(m+80);out.nativeSequence=load<i64>(m+104);
  for(let i=0;i<16;++i)out.writer[i]=b[320+i];return 0;
}
