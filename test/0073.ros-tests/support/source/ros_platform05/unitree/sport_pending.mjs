/** Pure contract reusable inside the selected embedded JS runtime. Executing
 * this module with Node establishes host-only evidence, never Guest transport. */
import {Ros2Unitree_apiMsgRequestCodec as Request,Ros2Unitree_apiMsgResponseCodec as Response} from '../../../../../../sdk/origin/robots/unitree/tests/bounded_records.mjs';
import {SportPending,OPERATIONS} from '../../../../../../sdk/origin/robots/unitree/tests/sport.mjs';
function require(ok){if(!ok)throw new Error('Sport pending invariant');}
function refuse(code,call){try{call();}catch(error){require(error.code===code);return;}throw new Error('missing refusal '+code);}
function request(id,api){const value=Request.defaultValue();value.header.identity.id=id;value.header.identity.api_id=api;
  value.header.lease.id=-9007199254740993n;value.header.policy.priority=-101;value.parameter='{"complete":"λ"}';value.binary=[0,255,1,128];return value;}
function response(id,api){const value=Response.defaultValue();value.header.identity.id=id;value.header.identity.api_id=api;
  value.header.status.code=-2147483647;value.data='{"data":false,"full":"λ"}';value.binary=[0,-1,1,-128];return value;}
export function localPending(){
  const pending=new SportPending();let id=9007199254740993n;
  for(const [number]of Object.values(OPERATIONS)){
    const api=BigInt(number),req=request(id,api),before=Request.encode(req);
    pending.register(req,7n,11n);require(Request.encode(req).every((byte,i)=>byte===before[i]));
    require(pending.pendingCount===1);refuse(17,()=>pending.register(req,7n,11n));
    const reply=response(id,api);refuse(9,()=>pending.check(response(id,api+90000n),8n));
    pending.check(reply,10n);refuse(110,()=>pending.accept(reply,11n));
    const invalid=response(id,api);invalid.data='x'.repeat(4097);refuse(22,()=>pending.accept(invalid,10n));
    require(pending.pendingCount===1);const accepted=pending.accept(reply,10n);
    require(accepted.header.identity.id===id&&accepted.header.identity.api_id===api&&accepted.header.status.code===-2147483647);
    require(accepted.data===reply.data&&accepted.binary.length===4&&accepted.binary[1]===-1&&accepted.binary[3]===-128);
    reply.binary[0]=7;require(accepted.binary[0]===0);refuse(9,()=>pending.accept(accepted,10n));
    refuse(17,()=>pending.register(req,7n,11n));++id;
  }
  const max=0xffffffffffffffffn;
  for(let id=9007199254741032n;id<9007199254741048n;++id)pending.register(request(id,1003n),0n,max);
  const next=request(9007199254741048n,1003n);refuse(11,()=>pending.register(next,0n,max));require(pending.pendingCount===16);
  require(pending.expire(9007199254741032n,1003n,max-1n)===11);require(pending.expire(9007199254741032n,1003n,max)===110);
  pending.register(next,0n,max);pending.forget(next.header.identity.id,1003n);
  const invalid=request(0x7fffffffffffffffn,1003n);invalid.header.policy.noreply=true;
  refuse(22,()=>pending.register(invalid,0n,1n));invalid.header.policy.noreply=false;
  invalid.header.identity.api_id=66539n;refuse(22,()=>pending.register(invalid,0n,1n));invalid.header.identity.api_id=1003n;
  refuse(22,()=>pending.register(invalid,1n,1n));refuse(22,()=>pending.register(invalid,0,1n));
  refuse(22,()=>pending.register(invalid,0n,max+1n));pending.register(invalid,max-1n,max);pending.forget(invalid.header.identity.id,1003n);
  refuse(17,()=>pending.register(invalid,0n,1n));pending.close();require(pending.pendingCount===0);
  refuse(9,()=>pending.register(invalid,0n,1n));refuse(9,()=>pending.expire(9007199254741033n,1003n,max));
  return 'SPORT_PENDING_OK operations=39 capacity=16 identity_bits=64';
}
