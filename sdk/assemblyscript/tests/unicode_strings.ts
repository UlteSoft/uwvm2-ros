/** Prepared pure Wasm Unicode framing regression fixture. A leading U+FEFF is
 * literal String content, not a BOM to consume. These cases do not change the
 * shared 57/11 language-neutral vectors and import no ROS or native functions.
 * Actual AssemblyScript compilation/execution remains pending in the cgroup.
 */
import { StringCodec, Writer, ParameterValue, ParameterKind, encodeParameter,
  decodeParameter, ServiceResponse, encodeServiceResponse, decodeServiceResponse,
  TypedParameterEvent, encodeTypedParameterEvent, decodeTypedParameterEvent } from '../codecs';
export { abortGuest } from '../api';
let checks: u32 = 0;
/** Fail through a local trap without adding a logger or external abort import. */
function require(value: bool): void { if (!value) unreachable(); checks++; }

/** Execute prepared literal-BOM roundtrips through all selected text envelopes. */
export function _start(): void {
  const text = '\uFEFFrobot', codec = new StringCodec(), wire = codec.encode(text);
  require(codec.valid(wire) && codec.decode(wire) == text);
  require(wire.length == 12 && wire[4] == 0xef && wire[5] == 0xbb && wire[6] == 0xbf);
  require(codec.decode(codec.encode('\uFEFF')) == '\uFEFF');
  const value = new ParameterValue(ParameterKind.String); value.stringValue = text;
  const scalar = decodeParameter(encodeParameter(value), ParameterKind.String); require(scalar.valid && scalar.stringValue == text);
  const array = new ParameterValue(ParameterKind.StringArray); array.stringArray = [text, 'plain', '\uFEFF'];
  const decodedArray = decodeParameter(encodeParameter(array), ParameterKind.StringArray); require(decodedArray.valid && decodedArray.stringArray.length == 3 && decodedArray.stringArray[0] == text && decodedArray.stringArray[2] == '\uFEFF');
  const service = decodeServiceResponse(encodeServiceResponse(new ServiceResponse(true, text))); require(service.valid && service.message == text);
  const event = new TypedParameterEvent(); event.seconds = 1; event.nanoseconds = 2; event.kind = 2; event.node = '/demo'; event.name = 'value'; event.value = value;
  const decodedEvent = decodeTypedParameterEvent(encodeTypedParameterEvent(event)); require(decodedEvent.valid && decodedEvent.value.stringValue == text);
  event.value = array; const arrayEvent = decodeTypedParameterEvent(encodeTypedParameterEvent(event)); require(arrayEvent.valid && arrayEvent.value.stringArray[0] == text);
}
/** Count only after _start returns without a trap; prepared source is not a PASS. */
export function completedChecks(): u32 { return checks; }
