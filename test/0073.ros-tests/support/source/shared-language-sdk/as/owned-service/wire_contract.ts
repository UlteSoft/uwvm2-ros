/** Authored pure byte contracts, NOT executed and not Native ROS evidence. */
import { Binding,Delivery,receipt,sameReceipt,delivery,fullHandle } from '../../../../../../../sdk/origin/assemblyscript/ros2/rmw_service/wire';
import { endpoint,packet,SOURCE,PEER } from './fixtures';
export { abortGuest } from '../../../../../../../sdk/origin/assemblyscript/api';
let checks:u32=0;
function require(v:bool):void{if(!v)unreachable();++checks;}
export function _start():void{
  const a=new Uint8Array(256),b=new Uint8Array(256),owner=new Binding();endpoint(a);b.set(a);b[6]=2;
  require(fullHandle(SOURCE)&&!fullHandle(1)&&!fullHandle(0x100000000));
  require(receipt(a,1,3,SOURCE,owner)==0&&owner.incarnation==u64.MAX_VALUE&&owner.generation==u64.MAX_VALUE);
  require(receipt(b,2,3,SOURCE,new Binding())==0&&sameReceipt(a,b));
  b[224]=1;require(!sameReceipt(a,b));b.set(a);b[15]=1;
  const sentinel=new Binding();sentinel.source=7;require(receipt(b,1,3,SOURCE,sentinel)==22&&sentinel.source==7);
  b.set(a);store<u64>(b.dataStart+96,1000000000);require(receipt(b,1,3,SOURCE,sentinel)==22);
  const bytes=new Uint8Array(360),item=new Delivery();packet(bytes);
  require(delivery(bytes,owner,PEER,16,item)==0&&item.associated&&item.ticket==u64.MAX_VALUE);
  require(item.nativePresent&&item.nativeSequence==-1&&item.sourceTimestamp==i64.MIN_VALUE&&item.receivedTimestamp==0);
  let allzero=true;for(let i=0;i<16;++i)if(item.writer[i])allzero=false;require(allzero);
  store<u64>(bytes.dataStart+80,u64.MAX_VALUE);require(delivery(bytes,owner,PEER,16,item)==0&&item.reservation==u64.MAX_VALUE);
  packet(bytes);bytes[14]=1;require(delivery(bytes,owner,PEER,16,item)==22&&!item.associated);
  packet(bytes);bytes[283]=1;require(delivery(bytes,owner,PEER,16,item)==22&&item.associated);
  packet(bytes);bytes[281]=8;require(delivery(bytes,owner,PEER,16,item)==95&&item.associated);
  packet(bytes);store<u64>(bytes.dataStart+160+64,7);require(delivery(bytes,owner,PEER,16,item)==22);
  packet(bytes);store<u64>(bytes.dataStart+104,191);require(delivery(bytes,owner,PEER,16,item)==22&&!item.associated);
  packet(bytes);store<u64>(bytes.dataStart+104,201);require(delivery(bytes,owner,PEER,16,item)==22&&!item.associated);
  packet(bytes);store<u64>(bytes.dataStart+160+112,0);require(delivery(bytes,owner,PEER,16,item)==22&&item.associated);
  packet(bytes);store<u32>(bytes.dataStart+160+12,3);require(delivery(bytes,owner,PEER,16,item)==22&&item.associated);
}
export function completedChecks():u32{return checks;}
