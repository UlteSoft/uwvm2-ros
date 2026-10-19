# Canonical wire format v1

These records describe the **guest ABI**, not native ROS serialization. The
Gateway decodes them into its official generated ROS types after checking the
grant, type identity, values and quotas. All integer and IEEE754 fields are
little-endian. Records have no alignment padding, pointers, object headers or
trailing bytes. Strings contain a u32 UTF-8 byte count and exactly those bytes,
without a NUL terminator. A compiler's native string layout is never this wire.

The language-neutral [golden fixtures](../abi/wire-vectors-v1.json) contain 57
positive records and 11 structural refusals. Their large i64/u64 values are JSON
decimal **strings**. A generator produces specification bytes independently;
the existence of a fixture is not a successful codec or network test.

## Topic messages and elementary values

| Value | Exact field order | Additional checks |
| --- | --- | --- |
| String | u32 byte count, UTF-8 bytes | Reject malformed UTF-8; no implicit transcoding or terminator. |
| Bool | u8 | Exactly one byte, 0 or 1. |
| Twist | f64 linear x/y/z, f64 angular x/y/z | Exactly48 bytes; all six fields finite. Grant bounds remain separate. |
| Image | u32 sequence/seconds/nanoseconds, String frame, u32 height/width, String encoding, Bool bigendian, u32 step, u32 data byte count, data | Supported encodings are mono8/mono16/rgb8/bgr8/rgba8/bgra8. Dimensions, stride and byte count must agree; frame and timestamp are checked by the Runtime. |
| TransformStamped | u32 sequence/seconds/nanoseconds, String parent/child frame, seven f64 translation x/y/z and quaternion x/y/z/w | All fields finite; frames must match the explicitly granted pair. No implicit quaternion normalization. |

ROS2's canonical sequence field is zero because its native Header has no ROS1
sequence field. Timestamp nanoseconds are below 1000000000; ROS2 seconds must
fit signed int32. ROS1 native transport may assign its own Header sequence.

## Parameter values and arrays

Scalar Bool is one byte. ROS1 int32 is four two's-complement bytes; ROS2 int64 is
eight. Float64 is eight finite IEEE754 bytes, preserving signed zero. String
uses the ordinary length-prefixed UTF-8 layout. Numeric capability bounds are
inclusive and do not pass signed integers through floating point.

An array is u32 **element count** followed by its canonical elements. Bool and
byte have width 1; int32 width 4; int64/Float64 width 8. Each String-array element
has its own u32 byte length. Count zero is the four-byte zero record. Every Bool
is0/1, every Float64 is finite, and every String-array element valid UTF-8 without
NUL. ROS1 additionally requires XML1.0-compatible String-array codepoints.
Array grants require an explicit `max-parameter-elements` field. Byte arrays
and int64 arrays are ROS2 adapters; int32 arrays are ROS1 adapters.

Canonical scalar String permits embedded NUL in the parameter adapter, but
that does not guarantee native DDS String interoperability. The tested Jazzy
FastDDS parameter-event field rejects such a native scalar String before
delivery and may truncate a native String-array element. Do not use the SDK's
canonical allowance as proof about a native RMW's String representation.

## Services

Trigger's request is empty; SetBool's request is one Bool. Both responses are
Bool success followed by String message. A service server receives u64 owned
request ID followed by the canonical request. It responds with that ID followed
by the response through an independent `service.respond` grant. ID zero is
invalid. A server grant does not authorize responses or new service names.

## Fibonacci Actions

| Operation/event | Exact record |
| --- | --- |
| Send Goal | i32 order,0..46 and within the explicit bound |
| Returned Goal ID / cancel request | u64 opaque Goal ID |
| Incoming Goal event | u64 nonzero Goal ID, i32 order |
| Feedback | u64 Goal ID, u32 count, nonnegative i32 elements |
| Terminal result | u64 Goal ID, u8 outcome, u32 count, nonnegative i32 elements |
| Server response | u64 Goal ID, u8 kind, u32 count, nonnegative i32 elements |
| Cancel response | Bool accepted |

Sequence count is at most 48. Server kind 1 publishes feedback,2 succeeds,3
aborts,4 reports cancellation only for a goal actually cancelling. Client
terminal results use outcomes 2/3/4. ID zero is permitted only by the separately
authorized `action.cancel-any-goal` operation. Terminal consumption, deadline,
disconnect and revocation can retire the ID. ROS2 advertise/respond grants must
have matching explicit40-field endpoint QoS contracts; the SDK does not grant
an alternative responder contract.

## Graph, TF, lifecycle, clocks and logging

Graph results are one canonical String containing sorted newline rows and,
where applicable, a tab-separated type. Empty requests use the graph filter
already fixed by policy. TF lookup requests carry u64 nanoseconds; zero means
latest. TF results/broadcasts use TransformStamped above. Lifecycle GetState
returns u8 ID plus String label; ChangeState takes an empty request and returns
Bool, using only the transition fixed by policy. Clocks return u64 nanoseconds
in the separately granted clock domain. Log writes contain one String.

## ROS2 parameter events

Legacy String events are u32 seconds/nanoseconds, u8 kind 1/2/3, String node,
String name and String value. A deletion has an empty value String. This legacy
grant is separate from the following typed projections.

Typed events are u32 seconds/nanoseconds, u8 kind, u8 wire version2, u8 value tag,
then u32 byte length + node bytes, u32 byte length + name bytes, u32 byte length
+ **raw selected payload**. Fixed overhead is 23 bytes. The outer payload byte
length is separate from an inner String length or array count. Node is a
canonical absolute name; the parameter name is dotted. Both are checked against
the exact node/name selected by the grant.

| Tag | Projection | Nondeleted selected payload |
| --- | --- | --- |
| 0 | deletion-only | Only deleted kind 3, empty payload |
| 1 | Bool | one0/1 byte |
| 2 | int64 | eight signed bytes |
| 3 | Float64 | eight finite bytes |
| 4 | String | canonical String |
| 5 | byte-array | u32 count and unsigned bytes |
| 6 | Bool-array | u32 count and0/1 bytes |
| 7 | int64-array | u32 count and signed eight-byte elements |
| 8 | Float64-array | u32 count and finite eight-byte elements |
| 9 | String-array | u32 count and NUL-free canonical Strings |

Deleted kind 3 has an empty outer payload for every selected tag. It carries no
old value and cannot attest a prior type/value or numeric bound from guest wire
alone. Tag0 never permits new/changed events. Empty nondeleted String/array
values still have a four-byte zero inner length/count. A scalar read/write grant
does not authorize this event subscription; each projection has its own alias.

## Validation and resource limits

SDK validation catches malformed records early and preserves public decoded
outputs on refusal where documented. It does not replace host validation or
authorize an operation. Always check the operation's error before decoding.
Quotas, native type checks and Instance/Goal/request ownership remain enforced
by the Runtime/Gateway. See [porting-and-errors.md](porting-and-errors.md) for
conditional output lengths, admitted external effects and safe cleanup.
