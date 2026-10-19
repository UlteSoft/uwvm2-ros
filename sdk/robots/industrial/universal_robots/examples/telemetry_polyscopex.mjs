/** Complete selected I/O value. Pass a genuine copied Guest Session.
 * Importing this module grants nothing. Idle11/refusal does not become data.
 */
import {Session} from "../../../../javascript/roswasm.mjs";
import {Ros2UrinterfacesMsgDigitalIOStateStampedCodec as codec} from "../../../../custom-types/universal-robots-polyscopex-complete/bounded_records.mjs";
export function readOnce(session,alias){
 if(!(session instanceof Session)||session.version!==2)throw new TypeError("Matching actual Session required");
 if(session.outputCapacity<codec.maxWireBytes)throw new RangeError("Whole selected I/O storage required");
 let cap=null,primary=0,cleanup=0,value=null,error=null,cleanupError=null;
 try{cap=session.open(alias);cap.check();primary=session.raw.topic_subscribe(cap.handle);
  if(!primary){const r=cap.poll();primary=r.status;if(!primary)value=codec.decode(r.bytes);}}
 catch(thrown){error=thrown;primary=code(thrown);value=null;}
 finally{if(cap){try{cleanup=cap.close();}catch(thrown){cleanupError=thrown;cleanup=code(thrown);}}}
 return {primary,cleanup,value,error,cleanupError};
}
function code(value){return Number.isInteger(value?.code)&&value.code>0?value.code:22;}
