/** Prepared pure Wasm codec specification fixture.
 * The 57 positive and 11 negative cases come from abi/wire-vectors-v1.json.
 * It imports no ROS functions. An actual cgroup compiler/runtime run is required
 * before this file proves any AssemblyScript behavior; source preparation alone
 * is not a PASS. Negative zero and all 64-bit integer values retain their bits.
 */
import * as C from '../../../../../../../sdk/origin/assemblyscript/codecs';
export { abortGuest } from '../../../../../../../sdk/origin/assemblyscript/api';
import { ParameterValue, ParameterKind } from '../../../../../../../sdk/origin/assemblyscript/codecs';
let checks: u32 = 0;
/** Convert independent golden hex into owned bytes, without a language codec. */
function fromHex(text: string): Uint8Array { const out = new Uint8Array(text.length / 2); for (let i = 0; i < out.length; ++i) out[i] = <u8>parseInt(text.substring(i * 2, i * 2 + 2), 16); return out; }
/** Copy literal unsigned bytes without relying on optional TypedArray.from APIs. */
function bytesFrom(values:Array<u8>):Uint8Array { const out=new Uint8Array(values.length);for(let i=0;i<out.length;++i)out[i]=values[i];return out; }
/** Compare exact wire bytes. Any mismatch traps the pure guest fixture. */
function equal(actual: Uint8Array, expected: string): void { const golden = fromHex(expected); if (actual.length != golden.length) unreachable(); for (let i = 0; i < golden.length; ++i) if (actual[i] != golden[i]) unreachable(); checks++; }
/** Record a structural validity condition without importing env.abort. */
function require(value: bool): void { if (!value) unreachable(); checks++; }
/** Execute all prepared specification fixtures once. */
export function _start(): void {
  // string-empty
  {
    const codec = new C.StringCodec(), wire = codec.encode("");
    equal(wire, "00000000");
    require(codec.valid(wire));
    equal(codec.encode(codec.decode(wire)), "00000000");
  }
  // string-ascii
  {
    const codec = new C.StringCodec(), wire = codec.encode("hello");
    equal(wire, "0500000068656c6c6f");
    require(codec.valid(wire));
    equal(codec.encode(codec.decode(wire)), "0500000068656c6c6f");
  }
  // string-utf8
  {
    const codec = new C.StringCodec(), wire = codec.encode("robot 🌍");
    equal(wire, "0a000000726f626f7420f09f8c8d");
    require(codec.valid(wire));
    equal(codec.encode(codec.decode(wire)), "0a000000726f626f7420f09f8c8d");
  }
  // bool-false
  {
    const codec = new C.BoolCodec(), wire = codec.encode(false);
    equal(wire, "00");
    require(codec.valid(wire));
    equal(codec.encode(codec.decode(wire)), "00");
  }
  // bool-true
  {
    const codec = new C.BoolCodec(), wire = codec.encode(true);
    equal(wire, "01");
    require(codec.valid(wire));
    equal(codec.encode(codec.decode(wire)), "01");
  }
  // int32--2147483648
  {
    const v = new ParameterValue(ParameterKind.Int32);
    v.int32Value = -2147483648;
    const wire = C.encodeParameter(v);
    equal(wire, "00000080");
    const decoded = C.decodeParameter(wire, ParameterKind.Int32);
    require(decoded.valid);
    equal(C.encodeParameter(decoded), "00000080");
  }
  // int32--1
  {
    const v = new ParameterValue(ParameterKind.Int32);
    v.int32Value = -1;
    const wire = C.encodeParameter(v);
    equal(wire, "ffffffff");
    const decoded = C.decodeParameter(wire, ParameterKind.Int32);
    require(decoded.valid);
    equal(C.encodeParameter(decoded), "ffffffff");
  }
  // int32-2147483647
  {
    const v = new ParameterValue(ParameterKind.Int32);
    v.int32Value = 2147483647;
    const wire = C.encodeParameter(v);
    equal(wire, "ffffff7f");
    const decoded = C.decodeParameter(wire, ParameterKind.Int32);
    require(decoded.valid);
    equal(C.encodeParameter(decoded), "ffffff7f");
  }
  // int64--9223372036854775808
  {
    const v = new ParameterValue(ParameterKind.Int64);
    v.int64Value = <i64>0x8000000000000000;
    const wire = C.encodeParameter(v);
    equal(wire, "0000000000000080");
    const decoded = C.decodeParameter(wire, ParameterKind.Int64);
    require(decoded.valid);
    equal(C.encodeParameter(decoded), "0000000000000080");
  }
  // int64--1
  {
    const v = new ParameterValue(ParameterKind.Int64);
    v.int64Value = -1;
    const wire = C.encodeParameter(v);
    equal(wire, "ffffffffffffffff");
    const decoded = C.decodeParameter(wire, ParameterKind.Int64);
    require(decoded.valid);
    equal(C.encodeParameter(decoded), "ffffffffffffffff");
  }
  // int64-9223372036854775807
  {
    const v = new ParameterValue(ParameterKind.Int64);
    v.int64Value = 9223372036854775807;
    const wire = C.encodeParameter(v);
    equal(wire, "ffffffffffffff7f");
    const decoded = C.decodeParameter(wire, ParameterKind.Int64);
    require(decoded.valid);
    equal(C.encodeParameter(decoded), "ffffffffffffff7f");
  }
  // float64-zero
  {
    const v = new ParameterValue(ParameterKind.Float64);
    v.float64Value = 0.0;
    const wire = C.encodeParameter(v);
    equal(wire, "0000000000000000");
    const decoded = C.decodeParameter(wire, ParameterKind.Float64);
    require(decoded.valid);
    equal(C.encodeParameter(decoded), "0000000000000000");
  }
  // float64-negative-zero
  {
    const v = new ParameterValue(ParameterKind.Float64);
    v.float64Value = reinterpret<f64>(<u64>0x8000000000000000);
    const wire = C.encodeParameter(v);
    equal(wire, "0000000000000080");
    const decoded = C.decodeParameter(wire, ParameterKind.Float64);
    require(decoded.valid);
    equal(C.encodeParameter(decoded), "0000000000000080");
  }
  // float64-fraction
  {
    const v = new ParameterValue(ParameterKind.Float64);
    v.float64Value = 1.25;
    const wire = C.encodeParameter(v);
    equal(wire, "000000000000f43f");
    const decoded = C.decodeParameter(wire, ParameterKind.Float64);
    require(decoded.valid);
    equal(C.encodeParameter(decoded), "000000000000f43f");
  }
  // twist
  {
    const codec = new C.TwistCodec(), wire = codec.encode(new C.Twist(1, -2, 3, 0, reinterpret<f64>(<u64>0x8000000000000000), 0.5));
    equal(wire, "000000000000f03f00000000000000c0000000000000084000000000000000000000000000000080000000000000e03f");
    require(codec.valid(wire));
    equal(codec.encode(codec.decode(wire)), "000000000000f03f00000000000000c0000000000000084000000000000000000000000000000080000000000000e03f");
  }
  // array-bool
  {
    const v = new ParameterValue(ParameterKind.BoolArray);
    v.boolArray = [false, true, false];
    const wire = C.encodeParameter(v);
    equal(wire, "03000000000100");
    const decoded = C.decodeParameter(wire, ParameterKind.BoolArray);
    require(decoded.valid);
    equal(C.encodeParameter(decoded), "03000000000100");
  }
  // array-byte
  {
    const v = new ParameterValue(ParameterKind.Bytes);
    v.byteArray = bytesFrom([0, 127, 255]);
    const wire = C.encodeParameter(v);
    equal(wire, "03000000007fff");
    const decoded = C.decodeParameter(wire, ParameterKind.Bytes);
    require(decoded.valid);
    equal(C.encodeParameter(decoded), "03000000007fff");
  }
  // array-int32
  {
    const v = new ParameterValue(ParameterKind.Int32Array);
    v.int32Array = [-1, 2147483647];
    const wire = C.encodeParameter(v);
    equal(wire, "02000000ffffffffffffff7f");
    const decoded = C.decodeParameter(wire, ParameterKind.Int32Array);
    require(decoded.valid);
    equal(C.encodeParameter(decoded), "02000000ffffffffffffff7f");
  }
  // array-int64
  {
    const v = new ParameterValue(ParameterKind.Int64Array);
    v.int64Array = [<i64>0x8000000000000000, 9223372036854775807];
    const wire = C.encodeParameter(v);
    equal(wire, "020000000000000000000080ffffffffffffff7f");
    const decoded = C.decodeParameter(wire, ParameterKind.Int64Array);
    require(decoded.valid);
    equal(C.encodeParameter(decoded), "020000000000000000000080ffffffffffffff7f");
  }
  // array-float64
  {
    const v = new ParameterValue(ParameterKind.Float64Array);
    v.float64Array = [1.25, reinterpret<f64>(<u64>0x8000000000000000)];
    const wire = C.encodeParameter(v);
    equal(wire, "02000000000000000000f43f0000000000000080");
    const decoded = C.decodeParameter(wire, ParameterKind.Float64Array);
    require(decoded.valid);
    equal(C.encodeParameter(decoded), "02000000000000000000f43f0000000000000080");
  }
  // array-string
  {
    const v = new ParameterValue(ParameterKind.StringArray);
    v.stringArray = ["", "hello", "λ"];
    const wire = C.encodeParameter(v);
    equal(wire, "03000000000000000500000068656c6c6f02000000cebb");
    const decoded = C.decodeParameter(wire, ParameterKind.StringArray);
    require(decoded.valid);
    equal(C.encodeParameter(decoded), "03000000000000000500000068656c6c6f02000000cebb");
  }
  // array-empty
  {
    const v = new ParameterValue(ParameterKind.StringArray);
    v.stringArray = [];
    const wire = C.encodeParameter(v);
    equal(wire, "00000000");
    const decoded = C.decodeParameter(wire, ParameterKind.StringArray);
    require(decoded.valid);
    equal(C.encodeParameter(decoded), "00000000");
  }
  // image-rgb8
  {
    const v = new C.Image();
    v.sequence = 0;
    v.seconds = 1;
    v.nanoseconds = 2;
    v.height = 1;
    v.width = 1;
    v.encoding = "rgb8";
    v.bigendian = false;
    v.step = 3;
    v.frameId = "camera";
    v.data = bytesFrom([255, 0, 7]);
    const wire = C.encodeImage(v);
    equal(wire, "0000000001000000020000000600000063616d65726101000000010000000400000072676238000300000003000000ff0007");
    const decoded = C.decodeImage(wire);
    require(decoded.valid);
    equal(C.encodeImage(decoded), "0000000001000000020000000600000063616d65726101000000010000000400000072676238000300000003000000ff0007");
  }
  // transform
  {
    const v = new C.Transform();
    v.sequence = 0;
    v.seconds = 1;
    v.nanoseconds = 2;
    v.parentFrame = "map";
    v.childFrame = "base";
    v.x = 1;
    v.y = 2;
    v.z = 3;
    v.qx = 0;
    v.qy = 0;
    v.qz = 0;
    v.qw = 1;
    const wire = C.encodeTransform(v);
    equal(wire, "000000000100000002000000030000006d61700400000062617365000000000000f03f00000000000000400000000000000840000000000000000000000000000000000000000000000000000000000000f03f");
    const decoded = C.decodeTransform(wire);
    require(decoded.valid);
    equal(C.encodeTransform(decoded), "000000000100000002000000030000006d61700400000062617365000000000000f03f00000000000000400000000000000840000000000000000000000000000000000000000000000000000000000000f03f");
  }
  // trigger-request
  {
    const wire = new Uint8Array(0);
    equal(wire, "");
  }
  // set-bool-request
  {
    const codec = new C.BoolCodec(), wire = codec.encode(true);
    equal(wire, "01");
    require(codec.valid(wire));
    equal(codec.encode(codec.decode(wire)), "01");
  }
  // service-response
  {
    const wire = C.encodeServiceResponse(new C.ServiceResponse(true, "ok"));
    equal(wire, "01020000006f6b");
    const decoded = C.decodeServiceResponse(wire);
    require(decoded.valid);
    equal(C.encodeServiceResponse(decoded), "01020000006f6b");
  }
  // service-request-event
  {
    const wire = new C.Writer().u64(<u64>9223372036854775813).bytes(fromHex("01")).finish();
    equal(wire, "050000000000008001");
    const decoded = C.decodeServiceRequest(wire);
    require(decoded.valid);
    equal(new C.Writer().u64(decoded.id).bytes(decoded.request).finish(), "050000000000008001");
  }
  // service-response-envelope
  {
    const wire = C.encodeServiceReply(<u64>9223372036854775813, C.decodeServiceResponse(fromHex("01020000006f6b")));
    equal(wire, "050000000000008001020000006f6b");
  }
  // fibonacci-order
  {
    const wire = C.encodeFibonacciGoal(5);
    equal(wire, "05000000");
    const r = new C.Reader(wire); require(r.i32() == 5 && r.done());
  }
  // goal-id
  {
    const wire = new C.Writer(8).u64(<u64>9223372036854775813).finish();
    equal(wire, "0500000000000080");
    const r = new C.Reader(wire); require(r.u64() == <u64>9223372036854775813 && r.done());
  }
  // incoming-goal
  {
    const wire = new C.Writer().u64(<u64>9223372036854775813).i32(5).finish();
    equal(wire, "050000000000008005000000");
    const decoded = C.decodeIncomingGoal(wire);
    require(decoded.valid);
    equal(new C.Writer().u64(decoded.id).i32(decoded.order).finish(), "050000000000008005000000");
  }
  // feedback
  {
    const wire = C.encodeActionSequence(new C.ActionSequence(<u64>9223372036854775813, 0, [0, 1, 1, 2, 3, 5]), false);
    equal(wire, "050000000000008006000000000000000100000001000000020000000300000005000000");
    const decoded = C.decodeActionSequence(wire, false);
    require(decoded.valid);
    equal(C.encodeActionSequence(decoded, false), "050000000000008006000000000000000100000001000000020000000300000005000000");
  }
  // success
  {
    const wire = C.encodeActionSequence(new C.ActionSequence(<u64>9223372036854775813, 2, [0, 1, 1, 2, 3, 5]), true);
    equal(wire, "05000000000000800206000000000000000100000001000000020000000300000005000000");
    const decoded = C.decodeActionSequence(wire, true);
    require(decoded.valid);
    equal(C.encodeActionSequence(decoded, true), "05000000000000800206000000000000000100000001000000020000000300000005000000");
  }
  // server-feedback
  {
    const wire = C.encodeActionSequence(new C.ActionSequence(<u64>9223372036854775813, 1, [0, 1, 1, 2, 3, 5]), true);
    equal(wire, "05000000000000800106000000000000000100000001000000020000000300000005000000");
    const decoded = C.decodeActionSequence(wire, true);
    require(decoded.valid);
    equal(C.encodeActionSequence(decoded, true), "05000000000000800106000000000000000100000001000000020000000300000005000000");
  }
  // lifecycle-state
  {
    const wire = new C.Writer().u8(3).text("active").finish();
    equal(wire, "0306000000616374697665");
    const decoded = C.decodeLifecycleState(wire);
    require(decoded.valid);
    equal(new C.Writer().u8(decoded.id).text(decoded.label).finish(), "0306000000616374697665");
  }
  // time-nanoseconds
  {
    const wire = new C.Writer(8).u64(<u64>9223372036854775813).finish();
    equal(wire, "0500000000000080");
    const r = new C.Reader(wire); require(r.u64() == <u64>9223372036854775813 && r.done());
  }
  // legacy-parameter-event
  {
    const v = new C.ParameterEvent();
    v.seconds = 1;
    v.nanoseconds = 2;
    v.kind = 2;
    v.node = "/demo";
    v.name = "value";
    v.value = "true";
    const wire = C.encodeParameterEvent(v);
    equal(wire, "010000000200000002050000002f64656d6f0500000076616c75650400000074727565");
    const decoded = C.decodeParameterEvent(wire);
    require(decoded.valid);
    equal(C.encodeParameterEvent(decoded), "010000000200000002050000002f64656d6f0500000076616c75650400000074727565");
  }
  // typed-event-bool
  {
    const event = new C.TypedParameterEvent();
    event.seconds = 1;
    event.nanoseconds = 2;
    event.kind = 2;
    event.node = "/demo";
    event.name = "value";
    const v = new ParameterValue(ParameterKind.Bool);
    v.boolValue = true;
    event.value = v;
    const wire = C.encodeTypedParameterEvent(event);
    equal(wire, "0100000002000000020201050000002f64656d6f0500000076616c75650100000001");
    const decoded = C.decodeTypedParameterEvent(wire);
    require(decoded.valid);
    equal(C.encodeTypedParameterEvent(decoded), "0100000002000000020201050000002f64656d6f0500000076616c75650100000001");
  }
  // typed-event-int64
  {
    const event = new C.TypedParameterEvent();
    event.seconds = 1;
    event.nanoseconds = 2;
    event.kind = 2;
    event.node = "/demo";
    event.name = "value";
    const v = new ParameterValue(ParameterKind.Int64);
    v.int64Value = 9223372036854775807;
    event.value = v;
    const wire = C.encodeTypedParameterEvent(event);
    equal(wire, "0100000002000000020202050000002f64656d6f0500000076616c756508000000ffffffffffffff7f");
    const decoded = C.decodeTypedParameterEvent(wire);
    require(decoded.valid);
    equal(C.encodeTypedParameterEvent(decoded), "0100000002000000020202050000002f64656d6f0500000076616c756508000000ffffffffffffff7f");
  }
  // typed-event-float64
  {
    const event = new C.TypedParameterEvent();
    event.seconds = 1;
    event.nanoseconds = 2;
    event.kind = 2;
    event.node = "/demo";
    event.name = "value";
    const v = new ParameterValue(ParameterKind.Float64);
    v.float64Value = 1.25;
    event.value = v;
    const wire = C.encodeTypedParameterEvent(event);
    equal(wire, "0100000002000000020203050000002f64656d6f0500000076616c756508000000000000000000f43f");
    const decoded = C.decodeTypedParameterEvent(wire);
    require(decoded.valid);
    equal(C.encodeTypedParameterEvent(decoded), "0100000002000000020203050000002f64656d6f0500000076616c756508000000000000000000f43f");
  }
  // typed-event-string
  {
    const event = new C.TypedParameterEvent();
    event.seconds = 1;
    event.nanoseconds = 2;
    event.kind = 2;
    event.node = "/demo";
    event.name = "value";
    const v = new ParameterValue(ParameterKind.String);
    v.stringValue = "hello";
    event.value = v;
    const wire = C.encodeTypedParameterEvent(event);
    equal(wire, "0100000002000000020204050000002f64656d6f0500000076616c7565090000000500000068656c6c6f");
    const decoded = C.decodeTypedParameterEvent(wire);
    require(decoded.valid);
    equal(C.encodeTypedParameterEvent(decoded), "0100000002000000020204050000002f64656d6f0500000076616c7565090000000500000068656c6c6f");
  }
  // typed-event-byte-array
  {
    const event = new C.TypedParameterEvent();
    event.seconds = 1;
    event.nanoseconds = 2;
    event.kind = 2;
    event.node = "/demo";
    event.name = "value";
    const v = new ParameterValue(ParameterKind.Bytes);
    v.byteArray = bytesFrom([0, 255]);
    event.value = v;
    const wire = C.encodeTypedParameterEvent(event);
    equal(wire, "0100000002000000020205050000002f64656d6f0500000076616c7565060000000200000000ff");
    const decoded = C.decodeTypedParameterEvent(wire);
    require(decoded.valid);
    equal(C.encodeTypedParameterEvent(decoded), "0100000002000000020205050000002f64656d6f0500000076616c7565060000000200000000ff");
  }
  // typed-event-bool-array
  {
    const event = new C.TypedParameterEvent();
    event.seconds = 1;
    event.nanoseconds = 2;
    event.kind = 2;
    event.node = "/demo";
    event.name = "value";
    const v = new ParameterValue(ParameterKind.BoolArray);
    v.boolArray = [false, true];
    event.value = v;
    const wire = C.encodeTypedParameterEvent(event);
    equal(wire, "0100000002000000020206050000002f64656d6f0500000076616c756506000000020000000001");
    const decoded = C.decodeTypedParameterEvent(wire);
    require(decoded.valid);
    equal(C.encodeTypedParameterEvent(decoded), "0100000002000000020206050000002f64656d6f0500000076616c756506000000020000000001");
  }
  // typed-event-int64-array
  {
    const event = new C.TypedParameterEvent();
    event.seconds = 1;
    event.nanoseconds = 2;
    event.kind = 2;
    event.node = "/demo";
    event.name = "value";
    const v = new ParameterValue(ParameterKind.Int64Array);
    v.int64Array = [-1, 9223372036854775807];
    event.value = v;
    const wire = C.encodeTypedParameterEvent(event);
    equal(wire, "0100000002000000020207050000002f64656d6f0500000076616c75651400000002000000ffffffffffffffffffffffffffffff7f");
    const decoded = C.decodeTypedParameterEvent(wire);
    require(decoded.valid);
    equal(C.encodeTypedParameterEvent(decoded), "0100000002000000020207050000002f64656d6f0500000076616c75651400000002000000ffffffffffffffffffffffffffffff7f");
  }
  // typed-event-float64-array
  {
    const event = new C.TypedParameterEvent();
    event.seconds = 1;
    event.nanoseconds = 2;
    event.kind = 2;
    event.node = "/demo";
    event.name = "value";
    const v = new ParameterValue(ParameterKind.Float64Array);
    v.float64Array = [1.25];
    event.value = v;
    const wire = C.encodeTypedParameterEvent(event);
    equal(wire, "0100000002000000020208050000002f64656d6f0500000076616c75650c00000001000000000000000000f43f");
    const decoded = C.decodeTypedParameterEvent(wire);
    require(decoded.valid);
    equal(C.encodeTypedParameterEvent(decoded), "0100000002000000020208050000002f64656d6f0500000076616c75650c00000001000000000000000000f43f");
  }
  // typed-event-string-array
  {
    const event = new C.TypedParameterEvent();
    event.seconds = 1;
    event.nanoseconds = 2;
    event.kind = 2;
    event.node = "/demo";
    event.name = "value";
    const v = new ParameterValue(ParameterKind.StringArray);
    v.stringArray = ["hello", ""];
    event.value = v;
    const wire = C.encodeTypedParameterEvent(event);
    equal(wire, "0100000002000000020209050000002f64656d6f0500000076616c756511000000020000000500000068656c6c6f00000000");
    const decoded = C.decodeTypedParameterEvent(wire);
    require(decoded.valid);
    equal(C.encodeTypedParameterEvent(decoded), "0100000002000000020209050000002f64656d6f0500000076616c756511000000020000000500000068656c6c6f00000000");
  }
  // typed-deletion-tag-0
  {
    const event = new C.TypedParameterEvent();
    event.seconds = 1;
    event.nanoseconds = 2;
    event.kind = 3;
    event.node = "/demo";
    event.name = "value";
    event.value = new ParameterValue(<ParameterKind>0);
    const wire = C.encodeTypedParameterEvent(event);
    equal(wire, "0100000002000000030200050000002f64656d6f0500000076616c756500000000");
    const decoded = C.decodeTypedParameterEvent(wire);
    require(decoded.valid);
    equal(C.encodeTypedParameterEvent(decoded), "0100000002000000030200050000002f64656d6f0500000076616c756500000000");
  }
  // typed-deletion-tag-1
  {
    const event = new C.TypedParameterEvent();
    event.seconds = 1;
    event.nanoseconds = 2;
    event.kind = 3;
    event.node = "/demo";
    event.name = "value";
    event.value = new ParameterValue(<ParameterKind>1);
    const wire = C.encodeTypedParameterEvent(event);
    equal(wire, "0100000002000000030201050000002f64656d6f0500000076616c756500000000");
    const decoded = C.decodeTypedParameterEvent(wire);
    require(decoded.valid);
    equal(C.encodeTypedParameterEvent(decoded), "0100000002000000030201050000002f64656d6f0500000076616c756500000000");
  }
  // typed-deletion-tag-2
  {
    const event = new C.TypedParameterEvent();
    event.seconds = 1;
    event.nanoseconds = 2;
    event.kind = 3;
    event.node = "/demo";
    event.name = "value";
    event.value = new ParameterValue(<ParameterKind>2);
    const wire = C.encodeTypedParameterEvent(event);
    equal(wire, "0100000002000000030202050000002f64656d6f0500000076616c756500000000");
    const decoded = C.decodeTypedParameterEvent(wire);
    require(decoded.valid);
    equal(C.encodeTypedParameterEvent(decoded), "0100000002000000030202050000002f64656d6f0500000076616c756500000000");
  }
  // typed-deletion-tag-3
  {
    const event = new C.TypedParameterEvent();
    event.seconds = 1;
    event.nanoseconds = 2;
    event.kind = 3;
    event.node = "/demo";
    event.name = "value";
    event.value = new ParameterValue(<ParameterKind>3);
    const wire = C.encodeTypedParameterEvent(event);
    equal(wire, "0100000002000000030203050000002f64656d6f0500000076616c756500000000");
    const decoded = C.decodeTypedParameterEvent(wire);
    require(decoded.valid);
    equal(C.encodeTypedParameterEvent(decoded), "0100000002000000030203050000002f64656d6f0500000076616c756500000000");
  }
  // typed-deletion-tag-4
  {
    const event = new C.TypedParameterEvent();
    event.seconds = 1;
    event.nanoseconds = 2;
    event.kind = 3;
    event.node = "/demo";
    event.name = "value";
    event.value = new ParameterValue(<ParameterKind>4);
    const wire = C.encodeTypedParameterEvent(event);
    equal(wire, "0100000002000000030204050000002f64656d6f0500000076616c756500000000");
    const decoded = C.decodeTypedParameterEvent(wire);
    require(decoded.valid);
    equal(C.encodeTypedParameterEvent(decoded), "0100000002000000030204050000002f64656d6f0500000076616c756500000000");
  }
  // typed-deletion-tag-5
  {
    const event = new C.TypedParameterEvent();
    event.seconds = 1;
    event.nanoseconds = 2;
    event.kind = 3;
    event.node = "/demo";
    event.name = "value";
    event.value = new ParameterValue(<ParameterKind>5);
    const wire = C.encodeTypedParameterEvent(event);
    equal(wire, "0100000002000000030205050000002f64656d6f0500000076616c756500000000");
    const decoded = C.decodeTypedParameterEvent(wire);
    require(decoded.valid);
    equal(C.encodeTypedParameterEvent(decoded), "0100000002000000030205050000002f64656d6f0500000076616c756500000000");
  }
  // typed-deletion-tag-6
  {
    const event = new C.TypedParameterEvent();
    event.seconds = 1;
    event.nanoseconds = 2;
    event.kind = 3;
    event.node = "/demo";
    event.name = "value";
    event.value = new ParameterValue(<ParameterKind>6);
    const wire = C.encodeTypedParameterEvent(event);
    equal(wire, "0100000002000000030206050000002f64656d6f0500000076616c756500000000");
    const decoded = C.decodeTypedParameterEvent(wire);
    require(decoded.valid);
    equal(C.encodeTypedParameterEvent(decoded), "0100000002000000030206050000002f64656d6f0500000076616c756500000000");
  }
  // typed-deletion-tag-7
  {
    const event = new C.TypedParameterEvent();
    event.seconds = 1;
    event.nanoseconds = 2;
    event.kind = 3;
    event.node = "/demo";
    event.name = "value";
    event.value = new ParameterValue(<ParameterKind>7);
    const wire = C.encodeTypedParameterEvent(event);
    equal(wire, "0100000002000000030207050000002f64656d6f0500000076616c756500000000");
    const decoded = C.decodeTypedParameterEvent(wire);
    require(decoded.valid);
    equal(C.encodeTypedParameterEvent(decoded), "0100000002000000030207050000002f64656d6f0500000076616c756500000000");
  }
  // typed-deletion-tag-8
  {
    const event = new C.TypedParameterEvent();
    event.seconds = 1;
    event.nanoseconds = 2;
    event.kind = 3;
    event.node = "/demo";
    event.name = "value";
    event.value = new ParameterValue(<ParameterKind>8);
    const wire = C.encodeTypedParameterEvent(event);
    equal(wire, "0100000002000000030208050000002f64656d6f0500000076616c756500000000");
    const decoded = C.decodeTypedParameterEvent(wire);
    require(decoded.valid);
    equal(C.encodeTypedParameterEvent(decoded), "0100000002000000030208050000002f64656d6f0500000076616c756500000000");
  }
  // typed-deletion-tag-9
  {
    const event = new C.TypedParameterEvent();
    event.seconds = 1;
    event.nanoseconds = 2;
    event.kind = 3;
    event.node = "/demo";
    event.name = "value";
    event.value = new ParameterValue(<ParameterKind>9);
    const wire = C.encodeTypedParameterEvent(event);
    equal(wire, "0100000002000000030209050000002f64656d6f0500000076616c756500000000");
    const decoded = C.decodeTypedParameterEvent(wire);
    require(decoded.valid);
    equal(C.encodeTypedParameterEvent(decoded), "0100000002000000030209050000002f64656d6f0500000076616c756500000000");
  }
  // Reject: Bool must be exactly 0 or 1.
  require(!(new C.BoolCodec().valid(fromHex("02"))));
  // Reject: Reject trailing bytes.
  require(!(new C.BoolCodec().valid(fromHex("0100"))));
  // Reject: Declared UTF-8 byte length exceeds the remaining buffer.
  require(!(new C.StringCodec().valid(fromHex("050000006869"))));
  // Reject: Overlong UTF-8 is invalid.
  require(!(new C.StringCodec().valid(fromHex("02000000c0af"))));
  // Reject: Float64 must be finite.
  require(!(C.decodeParameter(fromHex("000000000000f07f"), ParameterKind.Float64).valid));
  // Reject: Every Float64 array element must be finite.
  require(!(C.decodeParameter(fromHex("01000000000000000000f87f"), ParameterKind.Float64Array).valid));
  // Reject: Every Bool array element must be 0 or 1.
  require(!(C.decodeParameter(fromHex("0100000002"), ParameterKind.BoolArray).valid));
  // Reject: String-array elements are NUL-free.
  require(!(C.decodeParameter(fromHex("0100000003000000610062"), ParameterKind.StringArray).valid));
  // Reject: An owned Goal ID is nonzero.
  require(!(C.decodeActionSequence(fromHex("00000000000000000200000000")).valid));
  // Reject: Deletion-only tag 0 requires deleted kind 3.
  require(!(C.decodeTypedParameterEvent(fromHex("0100000002000000010200050000002f64656d6f0500000076616c756500000000")).valid));
  // Reject: Deleted events have an empty selected payload.
  require(!(C.decodeTypedParameterEvent(fromHex("0100000002000000030201050000002f64656d6f0500000076616c75650100000001")).valid));
}
/** Number of successful assertions only after _start returned without a trap. */
export function completedChecks():u32 { return checks; }
