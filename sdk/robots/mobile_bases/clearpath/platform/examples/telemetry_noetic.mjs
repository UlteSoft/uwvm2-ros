/** Read one complete selected Clearpath state using an actual copied Session.
 * Import/construct grants nothing. Keep primary, cleanup and observation distinct.
 */
import {Session} from "../../../../../javascript/roswasm.mjs";
import {Ros1Clearpath_platform_msgsStatusCodec as codec} from "../../../../../custom-types/clearpath-noetic-complete/bounded_records.mjs";
export function readOnce(session,alias){
 if(!(session instanceof Session)||session.version!==1)throw new TypeError("Matching actual Session required");
 if(session.outputCapacity<codec.maxWireBytes)throw new RangeError("Whole selected state storage required");
 let cap=null,primary=0,cleanup=0,value=null,error=null,cleanupError=null;
 try{cap=session.open(alias);cap.check();primary=session.raw.topic_subscribe(cap.handle);
  if(!primary){const r=cap.poll();primary=r.status;if(!primary)value=codec.decode(r.bytes);}}
 catch(thrown){error=thrown;primary=code(thrown);value=null;}
 finally{if(cap){try{cleanup=cap.close();}catch(thrown){cleanupError=thrown;cleanup=code(thrown);}}}
 return {primary,cleanup,value,error,cleanupError};
}
function code(value){return Number.isInteger(value?.code)&&value.code>0?value.code:22;}
