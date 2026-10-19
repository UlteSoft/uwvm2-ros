/** Synthetic byte fixtures only. No value here is real Native identity or QoS. */
export const SOURCE:u64=0xffffffffffffffff;
export const PEER:u64=0x8000000180000001;
export const AVAILABILITY:u64=0x8000000280000002;
export function endpoint(out:Uint8Array,operation:u8=1,kind:u8=3):void{
  out.fill(0);const p=out.dataStart;
  store<u32>(p,0x45524d55);store<u16>(p+4,1);out[6]=operation;out[7]=2;store<u32>(p+8,256);
  out[12]=kind;out[13]=2;out[14]=kind==3?16:0;
  store<u64>(p+16,SOURCE);store<u64>(p+24,0x8000000000000001);store<u64>(p+32,u64.MAX_VALUE);
  store<u64>(p+40,0x8000000000000002);store<u64>(p+48,u64.MAX_VALUE);
  for(let at=64;at<=144;at+=80){store<u32>(p+at,1);store<u32>(p+at+4,1);store<u32>(p+at+8,2);
    store<u32>(p+at+12,1);store<u64>(p+at+16,1000);}
}
export function packet(out:Uint8Array,kind:u8=3,sequence:u64=1,nonce:u64=1,ticket:u64=u64.MAX_VALUE,body:i32=1):void{
  out.fill(0);const p=out.dataStart;
  store<u32>(p,0x43524d55);store<u16>(p+4,1);out[6]=5;out[7]=2;store<u32>(p+8,160);out[12]=kind;out[13]=16;
  store<u64>(p+16,SOURCE);store<u64>(p+24,0x8000000000000001);store<u64>(p+32,u64.MAX_VALUE);
  store<u64>(p+40,0x8000000000000002);store<u64>(p+48,u64.MAX_VALUE);store<u64>(p+56,PEER);
  store<u64>(p+64,ticket);store<u64>(p+72,sequence);store<u64>(p+80,nonce);
  store<u64>(p+96,<u64>out.length);store<u64>(p+104,<u64>(192+body));
  const m=p+160;store<u32>(m,0x494d5255);store<u16>(m+4,1);out[166]=kind==3?3:2;out[167]=2;
  store<u32>(m+8,192);store<u32>(m+12,35);
  for(let at=16;at<=48;at+=8)store<u64>(m+at,load<u64>(p+at));
  store<u64>(m+56,sequence);store<u64>(m+64,ticket);store<i64>(m+72,i64.MIN_VALUE);store<i64>(m+80,0);
  store<u64>(m+88,u64.MAX_VALUE);store<u64>(m+96,u64.MAX_VALUE);store<i64>(m+104,-1);
  store<u64>(m+112,<u64>body);out[281]=16;
  if(body)out[352]=7;
}
