/** AssemblyScript: generated fixed records use the existing Codec<T> interface.
 * Compile with the bundled guest-only generated-r3 codec. Real native delivery
 * additionally requires official identities and a reviewed registered adapter.
 * Host `packet-pub` must explicitly grant the exact ROS1 native/codec identities
 * and all Topic limits. Managed object pointers are never used as payload bytes.
 * Compile this ROS1 entry with the selected AssemblyScript compiler and a
 * separate /tmp output. Inspect exact imports and full Wasm validation; no
 * environment, clock or logger permission is implicit. A harness must invoke
 * _start once and require lastStatus()==0: a void return/command exit alone is
 * insufficient. Publication0 is not delivery; an independent matching native
 * peer and qualified generated host adapter are separate prerequisites.
 * This preparation source does not claim compiled/native acceptance.
 */
import {NativeApi,Node} from '../../assemblyscript/ros1/roswasm';
import {Ros1Example_interfaces_customPacket,Ros1Example_interfaces_customPacketCodec} from '../generated-r3/fixed_records';
export {abortGuest} from '../../assemblyscript/api';
let status:u32=0, primary:u32=0, cleanup:u32=0;

/** Encode/publish once; Node releases its owned alias after the attempt. */
export function _start():void {
  const node=new Node(new NativeApi(),'fixed-packet');
  const packet=new Ros1Example_interfaces_customPacket();
  packet.sequence=7;packet.active=true;packet.measurement=1.25;
  packet.tag[0]=0x41;packet.samples[0]=-3;
  const publisher=node.createPublisher<Ros1Example_interfaces_customPacket>('packet-pub',new Ros1Example_interfaces_customPacketCodec());
  status=publisher.capability.openStatus;
  if(!status)status=publisher.publish(packet);
  primary=status;
  const outcomes=node.close();
  for(let i=0;i<outcomes.length;++i) if(outcomes[i]) { if(!cleanup) cleanup=outcomes[i]; if(!status) status=outcomes[i]; }
}

/** Report status without requiring a log or stdio capability. */
export function lastStatus():u32{return status;}
/** Primary open/publication result, retained even if checked close also fails. */
export function operationStatus():u32{return primary;}
/** First checked close error; no failed notification is replayed automatically. */
export function cleanupStatus():u32{return cleanup;}
