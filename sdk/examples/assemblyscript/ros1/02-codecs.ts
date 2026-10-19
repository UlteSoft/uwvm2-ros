/** ROS 1 lesson 2: canonical serialization before the convenience layer.
 * StringCodec adds a u32 UTF-8 byte count; passing a managed string address or
 * bare UTF-8 would violate the message contract. The native type is fixed by policy.
 */
import * as raw from '../../../assemblyscript/ros1/raw';
import { StringCodec } from '../../../assemblyscript/codecs';
export { abortGuest } from '../../../assemblyscript/api';
let status: u32 = 0;

/** Publish one canonical String using exact imports and explicit ownership. */
export function _start(): void {
  if (raw.abi_version() != 1) { status = 95; return; }
  const alias = Uint8Array.wrap(String.UTF8.encode('pub'));
  const output = new Uint8Array(8); store<u64>(output.dataStart, 0);
  status = raw.capability_open(0, <u64>alias.dataStart, <u64>alias.length, <u64>output.dataStart);
  if (status) return;
  const handle = load<u64>(output.dataStart), bytes = new StringCodec().encode('hello from AssemblyScript');
  status = raw.topic_publish(handle, 0, <u64>bytes.dataStart, <u64>bytes.length);
  // Close is attempted once even after a publish error; no retry can duplicate a publish.
  const closed=raw.capability_close(handle); if(closed && !status) status=closed;
}
/** Diagnostic status from the single publish attempt. */
export function lastStatus(): u32 { return status; }
