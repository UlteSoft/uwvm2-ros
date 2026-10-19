/** JavaScript: generated codecs work with the existing Node publisher factory.
 * Generate the example catalog with official identities before importing this
 * file. A caller supplies an existing trusted bridge Session; this example
 * does not load Wasm, connect to ROS, create a bridge or acquire implicit rights.
 * No I/O is performed. Native and Guest execution qualification remains pending.
 */
import {Node} from '../../javascript/roswasm.mjs';
import {Ros1Example_interfaces_customPacketCodec} from '../generated-r3/fixed_records.mjs';

/** Produce one complete exact record. Fixed arrays must have their full lengths. */
export function makePacket(){return {sequence:7,active:true,measurement:1.25,tag:[0x41,0,0,0],samples:[-3,0,0]};}

/** Encode/decode requires no capability and proves only guest-local wire behavior. */
export function localRoundtrip(){return Ros1Example_interfaces_customPacketCodec.decode(Ros1Example_interfaces_customPacketCodec.encode(makePacket()));}

/** Publish once under an expressly granted ROS1 alias and close its owner.
 * @param {Session} session Already instantiated trusted ROS1 bridge session.
 * @returns {number} Numeric result from the single publication attempt.
 */
export function publishPacket(session){
  if(session.version!==1)throw new TypeError('Explicit ROS1 Session required');
  const node=new Node(session,'fixed-packet');
  try{return node.createPublisher('packet-pub',Ros1Example_interfaces_customPacketCodec).publish(makePacket());}
  finally{node.close();}
}
