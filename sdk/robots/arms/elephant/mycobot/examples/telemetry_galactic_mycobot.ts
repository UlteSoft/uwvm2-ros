/** Real AssemblyScript Elephant Robotics state reader; no stub NativeApi or host mock.
 * Compile this backend alone. readOnce receives an explicit resource alias and
 * returns the primary ABI status. cleanupStatus preserves independent cleanup.
 * lastValue changes only after full checked decode; inspect status before using
 * it as a new observation. No clock, socket, thread or policy API is imported.
 */
import {Capability,NativeApi} from "../../../../../assemblyscript/ros2/roswasm";
import {Ros2Mycobot_interfacesMsgMycobotAngles as Status,Ros2Mycobot_interfacesMsgMycobotAnglesCodec as StatusCodec} from "../../../../../custom-types/elephant-mycobot-galactic-complete/bounded_records";
export let cleanupStatus:u32=0;
let observed:Status|null=null;
export function lastValue():Status|null {return observed;}
export function readOnce(alias:string):u32 {
  cleanupStatus=0;const cap=new Capability(new NativeApi());let primary=cap.open(alias);
  if(!primary)primary=cap.subscribe();
  if(!primary){const reply=cap.poll(Status.maxWireBytes);primary=reply.status;
    if(!primary){const decoded=new StatusCodec().decodeChecked(reply.bytes);primary=decoded.status;
      if(!primary)observed=decoded.value;}}
  if(cap.isOpen())cleanupStatus=cap.close();
  return primary;
}
