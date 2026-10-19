/** Lesson 01: actual guest JS codec and Unicode behavior, with no ROS resource.
 * These independent small vectors cover i64 precision, unaligned fields, BOM,
 * lone surrogates and fatal UTF-8 errors. Passing this lesson is guest-local
 * evidence only; it does not establish a native ROS connection.
 */
import { StringMessage, Parameter } from '../../javascript/codecs.mjs';
import { check, marker } from './support.mjs';
export function main() {
  const bytes = StringMessage.encode('hello');
  check(Array.from(bytes).join(',') === '5,0,0,0,104,101,108,108,111', 'Independent String bytes');
  check(StringMessage.decode(bytes) === 'hello', 'Complete String decode');
  check(StringMessage.decode(StringMessage.encode('\ufeffrobot')) === '\ufeffrobot', 'Literal BOM');
  const large = 0x7fffffffffffffffn;
  check(Parameter.Int64.decode(Parameter.Int64.encode(large)) === large, 'BigInt precision');
  const encoding = new TextEncoder().encode('\ud800x\udc00');
  check(Array.from(encoding).join(',') === '239,191,189,120,239,191,189', 'Lone surrogate replacement');
  const target = new Uint8Array(3), partial = new TextEncoder().encodeInto('😀x', target);
  check(partial.read === 0 && partial.written === 0, 'No split UTF-8 scalar');
  check(new TextDecoder().decode(Uint8Array.of(0xe2, 0x82)) === '\ufffd', 'Maximal incomplete prefix');
  let refused = false;
  try { new TextDecoder('utf-8', { fatal: true }).decode(Uint8Array.of(0xed, 0xa0, 0x80)); }
  catch { refused = true; }
  check(refused, 'Surrogate UTF-8 refusal');
  marker('PASS_JS_GUEST_CODECS'); return 0;
}
