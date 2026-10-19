//! Checked canonical wire helpers, independent of native ROS serialization.
//!
//! These functions allocate nothing. Encode into an explicitly bounded caller
//! buffer, then pass exactly the returned prefix to the import. Decode only a
//! successful returned prefix. Strings are UTF-8 u32 byte length plus bytes;
//! integers and finite IEEE754 doubles are little endian. Failure is an Error,
//! never a fallback empty value. The host still applies all configured quotas.
use crate::{codes, Error};
fn invalid() -> Error {
    Error::new(codes::INVALID)
}
fn capacity(out: &[u8], required: usize) -> Result<(), Error> {
    if out.len() < required {
        Err(Error {
            code: codes::TOO_LARGE,
            required: Some(required as u64),
        })
    } else {
        Ok(())
    }
}
fn u64_at(input: &[u8], at: usize) -> Result<u64, Error> {
    let bytes = input.get(at..at + 8).ok_or_else(invalid)?;
    Ok(u64::from_le_bytes(bytes.try_into().map_err(|_| invalid())?))
}
/// Encode a canonical String; embedded NUL is part of the wire value, but
/// individual native adapters can impose stricter interoperability rules.
pub fn encode_string(value: &str, out: &mut [u8]) -> Result<usize, Error> {
    let len = u32::try_from(value.len()).map_err(|_| invalid())?;
    let n = value.len().checked_add(4).ok_or_else(invalid)?;
    capacity(out, n)?;
    out[..4].copy_from_slice(&len.to_le_bytes());
    out[4..n].copy_from_slice(value.as_bytes());
    Ok(n)
}
/// Decode exactly one canonical UTF-8 String, borrowing its data without copying.
pub fn decode_string(value: &[u8]) -> Result<&str, Error> {
    if value.len() < 4 {
        return Err(invalid());
    }
    let n = u32::from_le_bytes(value[..4].try_into().map_err(|_| invalid())?) as usize;
    if n != value.len() - 4 {
        return Err(invalid());
    }
    core::str::from_utf8(&value[4..]).map_err(|_| invalid())
}
/// Encode one canonical Bool as byte0 or byte1; it also serves SetBool requests.
pub fn encode_bool(value: bool, out: &mut [u8]) -> Result<usize, Error> {
    capacity(out, 1)?;
    out[0] = u8::from(value);
    Ok(1)
}
/// Decode exactly one canonical Bool, rejecting bytes other than0 and1.
pub fn decode_bool(value: &[u8]) -> Result<bool, Error> {
    if value.len() != 1 || value[0] > 1 {
        return Err(invalid());
    }
    Ok(value[0] != 0)
}
/// Twist consists of linear xyz then angular xyz. Every field must be finite;
/// host capabilities additionally enforce independently configured magnitude bounds.
pub fn encode_twist(value: [f64; 6], out: &mut [u8]) -> Result<usize, Error> {
    if value.iter().any(|x| !x.is_finite()) {
        return Err(invalid());
    }
    capacity(out, 48)?;
    for (i, field) in value.iter().enumerate() {
        out[i * 8..(i + 1) * 8].copy_from_slice(&field.to_bits().to_le_bytes());
    }
    Ok(48)
}
/// Decode exactly six finite little-endian Float64 Twist fields.
pub fn decode_twist(value: &[u8]) -> Result<[f64; 6], Error> {
    if value.len() != 48 {
        return Err(invalid());
    }
    let mut out = [0.0; 6];
    for (i, field) in out.iter_mut().enumerate() {
        *field = f64::from_bits(u64_at(value, i * 8)?);
        if !field.is_finite() {
            return Err(invalid());
        }
    }
    Ok(out)
}
/// A Trigger/SetBool response: one Bool and one canonical UTF-8 message.
#[derive(Debug, PartialEq, Eq)]
pub struct ServiceResponse<'a> {
    /// Native service success field; it is separate from the host error code.
    pub success: bool,
    /// Borrowed native response message.
    pub message: &'a str,
}
/// Encode a Trigger or SetBool response, validating full size before modifying out.
pub fn encode_service_response(
    value: &ServiceResponse<'_>,
    out: &mut [u8],
) -> Result<usize, Error> {
    u32::try_from(value.message.len()).map_err(|_| invalid())?;
    let n = value.message.len().checked_add(5).ok_or_else(invalid)?;
    capacity(out, n)?;
    out[0] = u8::from(value.success);
    encode_string(value.message, &mut out[1..n])?;
    Ok(n)
}
/// Decode the complete Trigger/SetBool response without allocating a String.
pub fn decode_service_response(value: &[u8]) -> Result<ServiceResponse<'_>, Error> {
    if value.len() < 5 {
        return Err(invalid());
    }
    Ok(ServiceResponse {
        success: decode_bool(&value[..1])?,
        message: decode_string(&value[1..])?,
    })
}
/// An opaque ID read from a Gateway-issued service event. SDK construction is
/// restricted to checked decoding; the Gateway independently verifies ownership.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct RequestId(u64);
impl RequestId {
    /// Read the exact ID for an explicitly reviewed low-level operation.
    pub fn raw(self) -> u64 {
        self.0
    }
}
/// Decode a Trigger service event: exactly one nonzero eight-byte request ID.
pub fn decode_trigger_event(value: &[u8]) -> Result<RequestId, Error> {
    if value.len() != 8 {
        return Err(invalid());
    }
    let n = u64_at(value, 0)?;
    if n == 0 {
        return Err(invalid());
    }
    Ok(RequestId(n))
}
/// Decode a SetBool service event: a nonzero request ID followed by one Bool.
pub fn decode_set_bool_event(value: &[u8]) -> Result<(RequestId, bool), Error> {
    if value.len() != 9 {
        return Err(invalid());
    }
    Ok((
        decode_trigger_event(&value[..8])?,
        decode_bool(&value[8..])?,
    ))
}
/// One complete builtin Trigger/SetBool service event borrowed from caller scratch.
/// The Gateway-issued ID and canonical request stay associated for one dispatch.
#[derive(Debug, PartialEq, Eq)]
pub struct ServiceRequest<'a> {
    /// Nonzero request ID obtained by checked event decoding, never synthesized.
    pub id: RequestId,
    /// Empty Trigger bytes or exactly one canonical SetBool byte.
    pub request: &'a [u8],
}
/// Decode a complete supported Trigger/SetBool event without allocation.
/// Custom native Service adapters are not registered by this helper. Extra,
/// missing, zero-ID or noncanonical Bool bytes are rejected before callbacks.
pub fn decode_service_request(value: &[u8]) -> Result<ServiceRequest<'_>, Error> {
    if value.len() != 8 && value.len() != 9 { return Err(invalid()); }
    let id = decode_trigger_event(&value[..8])?;
    if value.len() == 9 { decode_bool(&value[8..])?; }
    Ok(ServiceRequest { id, request: &value[8..] })
}
/// Encode a service-server reply using an ID obtained from the advertised queue.
/// Advertising and replying require separate grants on the same resource/type.
pub fn encode_service_reply(
    id: RequestId,
    value: &ServiceResponse<'_>,
    out: &mut [u8],
) -> Result<usize, Error> {
    let n = value.message.len().checked_add(13).ok_or_else(invalid)?;
    u32::try_from(value.message.len()).map_err(|_| invalid())?;
    capacity(out, n)?;
    out[..8].copy_from_slice(&id.0.to_le_bytes());
    encode_service_response(value, &mut out[8..n])?;
    Ok(n)
}
/// An opaque Gateway-issued Fibonacci goal ID. Copying the value within one
/// guest is allowed; it never creates a grant or transfers native ownership.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct GoalId(u64);
impl GoalId {
    /// Read the exact ID for a reviewed raw operation; never synthesize one.
    pub fn raw(self) -> u64 {
        self.0
    }
    /// Return canonical bytes for feedback/result/cancel requests.
    pub fn bytes(self) -> [u8; 8] {
        self.0.to_le_bytes()
    }
}
/// Encode a Fibonacci goal order0..46. The host may configure a smaller bound.
pub fn encode_fibonacci_goal(order: i32, out: &mut [u8]) -> Result<usize, Error> {
    if !(0..=46).contains(&order) {
        return Err(invalid());
    }
    capacity(out, 4)?;
    out[..4].copy_from_slice(&order.to_le_bytes());
    Ok(4)
}
/// Decode a successful send-goal response, which is exactly one nonzero u64 ID.
pub fn decode_goal_id(value: &[u8]) -> Result<GoalId, Error> {
    if value.len() != 8 {
        return Err(invalid());
    }
    let id = u64_at(value, 0)?;
    if id == 0 {
        return Err(invalid());
    }
    Ok(GoalId(id))
}
/// Decode an advertised Fibonacci goal event: nonzero ID followed by i32 order.
pub fn decode_fibonacci_goal_event(value: &[u8]) -> Result<(GoalId, i32), Error> {
    if value.len() != 12 {
        return Err(invalid());
    }
    let id = decode_goal_id(&value[..8])?;
    let order = i32::from_le_bytes(value[8..].try_into().map_err(|_| invalid())?);
    if !(0..=46).contains(&order) {
        return Err(invalid());
    }
    Ok((id, order))
}
/// A checked borrowed Fibonacci sequence envelope. It exposes a checked iterator
/// instead of casting byte storage to an aligned native i32 array.
#[derive(Debug)]
pub struct FibonacciSequence<'a> {
    /// Nonzero Gateway-issued goal ID.
    pub goal: GoalId,
    /// Feedback=1, success=2, abort=3 or cancel=4; feedback has no wire kind byte.
    pub kind: u8,
    values: &'a [u8],
}
impl FibonacciSequence<'_> {
    /// Read exact nonnegative i32 fields without alignment or host-endian assumptions.
    pub fn values(&self) -> impl Iterator<Item = i32> + '_ {
        self.values
            .chunks_exact(4)
            .map(|v| i32::from_le_bytes([v[0], v[1], v[2], v[3]]))
    }
}
fn sequence(value: &[u8], has_kind: bool) -> Result<FibonacciSequence<'_>, Error> {
    let prefix = if has_kind { 13 } else { 12 };
    if value.len() < prefix {
        return Err(invalid());
    }
    let goal = decode_goal_id(&value[..8])?;
    let kind = if has_kind { value[8] } else { 1 };
    if has_kind && !(2..=4).contains(&kind) {
        return Err(invalid());
    }
    let at = if has_kind { 9 } else { 8 };
    let n = u32::from_le_bytes(value[at..at + 4].try_into().map_err(|_| invalid())?) as usize;
    if n > 48 || value.len() - prefix != n * 4 {
        return Err(invalid());
    }
    for bytes in value[prefix..].chunks_exact(4) {
        if i32::from_le_bytes([bytes[0], bytes[1], bytes[2], bytes[3]]) < 0 {
            return Err(invalid());
        }
    }
    Ok(FibonacciSequence {
        goal,
        kind,
        values: &value[prefix..],
    })
}
/// Decode feedback, preserving exact ID and count; host ownership remains authoritative.
pub fn decode_fibonacci_feedback(value: &[u8]) -> Result<FibonacciSequence<'_>, Error> {
    sequence(value, false)
}
/// Decode a terminal result with outcome2/3/4 and at most48 nonnegative values.
pub fn decode_fibonacci_result(value: &[u8]) -> Result<FibonacciSequence<'_>, Error> {
    sequence(value, true)
}
/// Encode an Action server feedback/result. Kind1/2/3/4 has the canonical server
/// envelope; kind4 is accepted by the host only for a genuinely cancelling goal.
pub fn encode_fibonacci_reply(
    id: GoalId,
    kind: u8,
    values: &[i32],
    out: &mut [u8],
) -> Result<usize, Error> {
    if !(1..=4).contains(&kind) || values.len() > 48 || values.iter().any(|v| *v < 0) {
        return Err(invalid());
    }
    let n = 13 + values.len() * 4;
    capacity(out, n)?;
    out[..8].copy_from_slice(&id.0.to_le_bytes());
    out[8] = kind;
    out[9..13].copy_from_slice(&(values.len() as u32).to_le_bytes());
    for (i, value) in values.iter().enumerate() {
        out[13 + i * 4..17 + i * 4].copy_from_slice(&value.to_le_bytes());
    }
    Ok(n)
}
/// Decode the separately granted clock's exact u64 nanosecond response.
pub fn decode_time(value: &[u8]) -> Result<u64, Error> {
    if value.len() != 8 {
        Err(invalid())
    } else {
        u64_at(value, 0)
    }
}

/// Encode the canonical Trigger request, which is exactly empty.
pub fn encode_trigger_request() -> [u8; 0] {
    []
}
/// Check that a Trigger request is exactly empty, rejecting any trailing bytes.
pub fn decode_trigger_request(value: &[u8]) -> Result<(), Error> {
    if value.is_empty() {
        Ok(())
    } else {
        Err(invalid())
    }
}
/// Encode a received-feedback shape (ID+count+values, no kind byte). Use this
/// only as a canonical fixture/adapter helper; receiving needs a separate grant.
pub fn encode_fibonacci_feedback(
    id: GoalId,
    values: &[i32],
    out: &mut [u8],
) -> Result<usize, Error> {
    if values.len() > 48 || values.iter().any(|v| *v < 0) {
        return Err(invalid());
    }
    let n = 12 + values.len() * 4;
    capacity(out, n)?;
    out[..8].copy_from_slice(&id.0.to_le_bytes());
    out[8..12].copy_from_slice(&(values.len() as u32).to_le_bytes());
    for (i, v) in values.iter().enumerate() {
        out[12 + i * 4..16 + i * 4].copy_from_slice(&v.to_le_bytes());
    }
    Ok(n)
}
/// Decode a canonical server response with feedback1 or terminal2/3/4. This
/// structural decoder cannot attest to the native goal's cancellation state.
pub fn decode_fibonacci_reply(value: &[u8]) -> Result<FibonacciSequence<'_>, Error> {
    if value.len() < 13 || !(1..=4).contains(&value[8]) {
        return Err(invalid());
    }
    let goal = decode_goal_id(&value[..8])?;
    let n = u32::from_le_bytes(value[9..13].try_into().map_err(|_| invalid())?) as usize;
    if n > 48 || value.len() - 13 != n * 4 {
        return Err(invalid());
    }
    for b in value[13..].chunks_exact(4) {
        if i32::from_le_bytes([b[0], b[1], b[2], b[3]]) < 0 {
            return Err(invalid());
        }
    }
    Ok(FibonacciSequence {
        goal,
        kind: value[8],
        values: &value[13..],
    })
}
