/** ROS 1 lesson 1: direct, exact ROS 1 ABI imports in a genuine Wasm guest.
 * Compile this entry alone. No ROS native SDK is required. The import declaration
 * retains i64 offsets/lengths even though AssemblyScript defaults to wasm32.
 * Grant pub explicitly in uwvm CLI policy; no grant means status 13.
 */
import * as raw from '../../../assemblyscript/ros1/raw';
export { abortGuest } from '../../../assemblyscript/api';
let status: u32 = 0;

/** Wasm command entry. All managed byte arrays remain live during synchronous imports. */
export function _start(): void {
  if (raw.abi_version() != 1) { status = 95; return; }
  const alias = Uint8Array.wrap(String.UTF8.encode('pub'));
  const output = new Uint8Array(8); store<u64>(output.dataStart, 0);
  status = raw.capability_open(0, <u64>alias.dataStart, <u64>alias.length, <u64>output.dataStart);
  if (status) return;
  const handle = load<u64>(output.dataStart);
  // Reading eight bytes is mandatory; a wasm32 pointer-sized slot would truncate a handle.
  status = raw.capability_close(handle);
}
/** Optional embedder diagnostic export; uwvm's command entry does not print it automatically. */
export function lastStatus(): u32 { return status; }
