/**
 * @file guest.mjs
 * @brief JavaScript application entry for an engine embedded in real Wasm.
 *
 * roswasm:guest_native is a closed C module registered by guest/engine.c inside
 * that Wasm. It is not available to Node, does not implement native ROS, and
 * cannot select a Runtime identity. The generated Wasm imports and per-instance
 * CLI policy determine authority. Importing this entry opens no capability.
 */
import * as native from 'roswasm:guest_native';
import { CopiedTransport } from './copied_transport.mjs';
import { Session } from './roswasm.mjs';
import { AsyncSession } from './async.mjs';
export * from './roswasm.mjs';
export * as Async from './async.mjs';
export * from './codecs.mjs';
export * from './events.mjs';

/** Allocate one explicitly serialized SDK session over this module's bounded
 * native scratch. Do not create two live sessions over that reservation.
 * Handle ownership still requires explicit close; garbage collection grants
 * nothing and never calls ROS imports on an application's behalf.
 */
export function createGuestSession() {
  const description = native.describe();
  return new Session(description.backend, new CopiedTransport(description.backend, native));
}

/** Same copied boundary with Promise-facing APIs. Blocking ROS calls retain
 * their original semantics; service_submit remains actual native admission.
 * The command/reactor executor explicitly advances finite Promise jobs.
 */
export function createAsyncGuestSession() {
  const description = native.describe();
  return AsyncSession.fromCopiedTransport(new CopiedTransport(description.backend, native));
}

/** Basic output only, through the selected Wasm's own bounded stdio import.
 * Console/file/process/network modules are never added by this SDK entry.
 */
export function writeGuestOutput(bytes) {
  if (!(bytes instanceof Uint8Array)) throw new TypeError('Uint8Array output required');
  return native.write(bytes);
}
/** Read exactly one basic stdin gate byte. This blocks in the real Wasm import
 * until the explicit supervisor supplies a byte; external deadlines own waits.
 * It never starts an actor or reads an environment/file/network configuration.
 */
export function readGuestByte() { return native.read(); }
