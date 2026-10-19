/** Complete SCOUT telemetry component for the real JS-in-Wasm Session.
 * The caller constructs the genuine copied Guest transport for one backend.
 * No Node-host mock, invented WebAssembly.Memory or native ROS client is used.
 * Every original Status field is retained in the selected complete codec.
 */
import {Session} from "../../../../../javascript/roswasm.mjs";
import {Ros1Scout_msgsScoutStatusCodec as ros1} from "../../../../../custom-types/agilex-scout-ros1-complete/bounded_records.mjs";
import {Ros2Scout_msgsMsgScoutStatusCodec as ros2} from "../../../../../custom-types/agilex-scout-ros2-complete/bounded_records.mjs";
/** Read once. Keep the original thrown value and every cleanup outcome.
 * A nonzero primary leaves value null; error11 is idle, not delivery.
 */
export function readOnce(session,version,alias) {
  if(!(session instanceof Session)||(version!==1&&version!==2)||session.version!==version)
    throw new TypeError('Matching actual Session and explicit ROS major required');
  const codec=version===1?ros1:ros2;
  if(session.outputCapacity<codec.maxWireBytes)throw new RangeError('Complete fixed Status storage required');
  let cap=null,primary=0,cleanup=0,value=null,error=null,cleanupError=null;
  try {
    cap=session.open(alias);cap.check();primary=session.raw.topic_subscribe(cap.handle);
    if(!primary){const result=cap.poll();primary=result.status;if(!primary)value=codec.decode(result.bytes);}
  } catch(thrown) {error=thrown;primary=code(thrown);value=null;}
  finally {
    if(cap){try{cleanup=cap.close();}catch(thrown){cleanupError=thrown;cleanup=code(thrown);}}
  }
  return {primary:primary||cleanup,cleanup,value,error,cleanupError};
}
function code(value){return Number.isInteger(value?.code)&&value.code>0?value.code:22;}
