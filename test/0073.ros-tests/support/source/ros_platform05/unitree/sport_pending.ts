/** Actual-Wasm local state contract. Stage beside sport.ts/bounded_records.ts;
 * it tests no native transport and needs only basic WASI stdout. */
import {Ros2Unitree_apiMsgRequest as Request,Ros2Unitree_apiMsgResponse as Response} from '../../../../../../sdk/origin/robots/unitree/tests/bounded_records';
import {SportPending} from '../../../../../../sdk/origin/robots/unitree/tests/sport';
@external('wasi_snapshot_preview1','fd_write') declare function fd_write(fd:u32,iov:usize,count:usize,written:usize):u32;
const slots=new StaticArray<u32>(3);
function write(text:string):void{const bytes=String.UTF8.encode(text);slots[0]=<u32>changetype<usize>(bytes);slots[1]=<u32>bytes.byteLength;slots[2]=0;const at=changetype<usize>(slots);if(fd_write(1,at,1,at+8)!=0||slots[2]!=slots[1])unreachable();}
function require(ok:bool):void{if(!ok)unreachable();}
function request(id:i64,api:i64):Request{
  const value=new Request();value.header.identity.id=id;value.header.identity.api_id=api;
  value.header.lease.id=-9007199254740993;value.header.policy.priority=-101;
  value.parameter='{"complete":"λ"}';value.binary=[0,255,1,128];return value;
}
function response(id:i64,api:i64):Response{
  const value=new Response();value.header.identity.id=id;value.header.identity.api_id=api;
  value.header.status.code=-2147483647;value.data='{"data":false,"full":"λ"}';value.binary=[0,-1,1,-128];return value;
}
export function _start():void{
  const apis:StaticArray<i64>=[1001,1002,1003,1004,1005,1006,1007,1008,1009,1010,1015,1016,1017,1020,1022,1023,1027,1028,1029,1030,1031,1032,1036,1061,1062,1063,2041,2043,2044,2045,2046,2047,2048,2049,2050,2051,2054,2055,2058];
  const pending=new SportPending();
  for(let offset:i32=0;offset<39;++offset){
    const id:i64=9007199254740993+<i64>offset,api=apis[offset],req=request(id,api);
    require(id>=9007199254740993);
    require(pending.register(req,7,11)==0);require(pending.pendingCount==1);require(pending.register(req,7,11)==17);
    const reply=response(id,api);require(pending.check(response(id,api+90000),8)==9);
    require(pending.check(reply,10)==0);require(pending.accept(reply,11).status==110);
    const invalid=response(id,api);invalid.data='x'.repeat(4097);require(pending.accept(invalid,10).status==22);
    require(pending.pendingCount==1);const accepted=pending.accept(reply,10);require(accepted.status==0&&accepted.value!==null);
    const value=changetype<Response>(accepted.value);require(value.header.identity.id==id&&value.header.identity.api_id==api);
    require(value.header.status.code==-2147483647&&value.data==reply.data&&value.binary.length==4);
    require(value.binary[1]==-1&&value.binary[3]==-128);reply.binary[0]=7;require(value.binary[0]==0);
    require(pending.accept(value,10).status==9);require(pending.register(req,7,11)==17);
  }
  for(let id:i64=9007199254741032;id<9007199254741048;++id)require(pending.register(request(id,1003),0,u64.MAX_VALUE)==0);
  const next=request(9007199254741048,1003);require(pending.register(next,0,u64.MAX_VALUE)==11);
  require(pending.pendingCount==16);require(pending.expire(9007199254741032,1003,u64.MAX_VALUE-1)==11);
  require(pending.expire(9007199254741032,1003,u64.MAX_VALUE)==110);require(pending.register(next,0,u64.MAX_VALUE)==0);
  require(pending.forget(next.header.identity.id,1003)==0);
  const invalid=request(i64.MAX_VALUE,1003);invalid.header.policy.noreply=true;
  require(pending.register(invalid,0,1)==22);invalid.header.policy.noreply=false;
  invalid.header.identity.api_id=65536+1003;require(pending.register(invalid,0,1)==22);
  invalid.header.identity.api_id=1003;require(pending.register(invalid,1,1)==22);
  require(pending.register(invalid,u64.MAX_VALUE-1,u64.MAX_VALUE)==0);require(pending.forget(i64.MAX_VALUE,1003)==0);
  require(pending.register(invalid,0,1)==17);pending.close();require(pending.pendingCount==0);
  require(pending.register(invalid,0,1)==9);require(pending.expire(9007199254741033,1003,u64.MAX_VALUE)==9);
  write('SPORT_PENDING_OK operations=39 capacity=16 identity_bits=64\n');
}
