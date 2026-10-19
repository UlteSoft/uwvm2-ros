/** Guest-local UTF-8 globals, installed before SDK module evaluation.
 * These classes call native functions compiled into this Wasm, not Node or a
 * host codec. Streaming decode is explicitly unsupported; complete framed ROS
 * values use one strict decode with fatal:true and ignoreBOM:true.
 */
import { encode, decode } from 'roswasm:utf8';

export class TextEncoder {
  get encoding() { return 'utf-8'; }
  encode(input = '') { return encode(String(input)); }
  /** Encode once, then copy only whole UTF-8 scalars. read counts UTF-16 units,
   * including one unit for each replaced unpaired surrogate. No per-character
   * native call or growing concatenation is needed.
   */
  encodeInto(input, destination) {
    if (!(destination instanceof Uint8Array)) throw new TypeError('Uint8Array destination required');
    const bytes = encode(String(input)); let written = 0, read = 0;
    while (written < bytes.length) {
      const first = bytes[written], count = first < 0x80 ? 1 : first < 0xe0 ? 2 : first < 0xf0 ? 3 : 4;
      if (count > destination.length - written) break;
      written += count; read += count === 4 ? 2 : 1;
    }
    destination.set(bytes.subarray(0, written)); return { read, written };
  }
}
export class TextDecoder {
  constructor(label = 'utf-8', { fatal = false, ignoreBOM = false } = {}) {
    const name = String(label).trim().toLowerCase();
    if (!['utf-8', 'utf8', 'unicode-1-1-utf-8'].includes(name)) throw new RangeError('Only UTF-8 is supported');
    this._fatal = Boolean(fatal); this._ignoreBOM = Boolean(ignoreBOM);
  }
  get encoding() { return 'utf-8'; }
  get fatal() { return this._fatal; }
  get ignoreBOM() { return this._ignoreBOM; }
  decode(input = new Uint8Array(), { stream = false } = {}) {
    if (stream) throw new TypeError('Complete-frame UTF-8 decoder required');
    let bytes;
    if (input instanceof Uint8Array) bytes = input;
    else if (input instanceof ArrayBuffer) bytes = new Uint8Array(input);
    else if (ArrayBuffer.isView(input)) bytes = new Uint8Array(input.buffer, input.byteOffset, input.byteLength);
    else throw new TypeError('ArrayBuffer or typed view required');
    return decode(bytes, this._fatal, this._ignoreBOM);
  }
}
globalThis.TextEncoder = TextEncoder;
globalThis.TextDecoder = TextDecoder;
