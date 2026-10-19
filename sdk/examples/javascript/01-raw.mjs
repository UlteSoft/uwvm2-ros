/** Lesson 1: exact ABI calls through an explicitly supplied guest bridge.
 * Run this JavaScript in an authority-aware embedder, not as uwvm input.
 * The embedder reserves input/output memory and provisions the alias externally.
 * No capability, native ROS import implementation or network socket is supplied
 * by this example. The raw adapter verifies every function signature argument.
 */
import { createRawAdapter, memoryRange, readU64 } from '../../javascript/raw.mjs';

/** Open one alias from pre-encoded UTF-8 memory and return its precise BigInt handle.
 * @param {object} bridge Explicit trusted bridge with complete ABI functions and shared memory.
 * @param {number} version Exactly ROS 1 or ROS 2.
 * @param {bigint} aliasOffset Address of UTF-8 bytes reserved by the bridge.
 * @param {bigint} aliasLength UTF-8 byte count, not JavaScript string length.
 * @param {bigint} slotOffset Address of eight writable bytes, disjoint from the alias.
 * @returns {object} Unchanged status plus handle only on success. Default-deny is normally 13.
 */
export function openAlias(bridge, version, aliasOffset, aliasLength, slotOffset) {
  const raw = createRawAdapter(version, bridge);
  if (raw.abi_version() !== 1) return { status: 95, handle: null };
  memoryRange(raw.memory, aliasOffset, aliasLength);
  const slot = memoryRange(raw.memory, slotOffset, 8n);
  new DataView(slot.buffer, slot.byteOffset, 8).setBigUint64(0, 0n, true);
  const status = raw.capability_open(0, aliasOffset, aliasLength, slotOffset);
  return { status, handle: status ? null : readU64(raw.memory, slotOffset) };
}

/** Explicitly release one successfully opened guest-owned handle after use.
 * A close error can follow handle retirement, so callers release local ownership
 * after this attempt rather than automatically retrying it.
 */
export function closeAlias(bridge, version, handle) {
  return createRawAdapter(version, bridge).capability_close(handle);
}
