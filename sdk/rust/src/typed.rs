//! Owned canonical builtin values, enabled by the `alloc` feature.
//!
//! No generated ROS type or native ROS serializer is linked. These are the
//! closed adapters accepted by the runtime, not arbitrary CDR/ROS serialization.
//! Encoding returns a new bounded byte vector; decoding returns a new value,
//! so refusals never replace a caller's value. The hard wire limits below do not
//! replace smaller explicitly configured native byte/element/rate quotas.
use crate::{codec, codes, Error};
use alloc::{string::String, vec::Vec};
/// Conservative canonical request-data hard limit, excluding IPC framing.
pub const MAX_BYTES: usize = 4_193_786;
/// Canonical array element ceiling; each host grant must explicitly select its bound.
pub const MAX_ELEMENTS: usize = 1_048_576;
fn invalid() -> Error {
    Error::new(codes::INVALID)
}
fn large() -> Error {
    Error::new(codes::TOO_LARGE)
}
struct Writer(Vec<u8>);
impl Writer {
    fn new() -> Self {
        Self(Vec::new())
    }
    fn add(&mut self, b: &[u8]) -> Result<(), Error> {
        if b.len() > MAX_BYTES - self.0.len() {
            return Err(large());
        }
        self.0.extend_from_slice(b);
        Ok(())
    }
    fn u8(&mut self, v: u8) -> Result<(), Error> {
        self.add(&[v])
    }
    fn u32(&mut self, v: u32) -> Result<(), Error> {
        self.add(&v.to_le_bytes())
    }
    fn u64(&mut self, v: u64) -> Result<(), Error> {
        self.add(&v.to_le_bytes())
    }
    fn real(&mut self, v: f64) -> Result<(), Error> {
        if !v.is_finite() {
            return Err(invalid());
        }
        self.u64(v.to_bits())
    }
    fn blob(&mut self, b: &[u8]) -> Result<(), Error> {
        self.u32(u32::try_from(b.len()).map_err(|_| large())?)?;
        self.add(b)
    }
    fn text(&mut self, s: &str) -> Result<(), Error> {
        self.blob(s.as_bytes())
    }
}
struct Reader<'a> {
    b: &'a [u8],
    at: usize,
}
impl<'a> Reader<'a> {
    fn new(b: &'a [u8]) -> Result<Self, Error> {
        if b.len() > MAX_BYTES {
            return Err(large());
        }
        Ok(Self { b, at: 0 })
    }
    fn bytes(&mut self, n: usize) -> Result<&'a [u8], Error> {
        if n > self.b.len() - self.at {
            return Err(invalid());
        }
        let b = &self.b[self.at..self.at + n];
        self.at += n;
        Ok(b)
    }
    fn u8(&mut self) -> Result<u8, Error> {
        Ok(self.bytes(1)?[0])
    }
    fn u32(&mut self) -> Result<u32, Error> {
        Ok(u32::from_le_bytes(
            self.bytes(4)?.try_into().map_err(|_| invalid())?,
        ))
    }
    fn u64(&mut self) -> Result<u64, Error> {
        Ok(u64::from_le_bytes(
            self.bytes(8)?.try_into().map_err(|_| invalid())?,
        ))
    }
    fn real(&mut self) -> Result<f64, Error> {
        let v = f64::from_bits(self.u64()?);
        if !v.is_finite() {
            Err(invalid())
        } else {
            Ok(v)
        }
    }
    fn blob(&mut self) -> Result<&'a [u8], Error> {
        let n = self.u32()? as usize;
        self.bytes(n)
    }
    fn text(&mut self) -> Result<String, Error> {
        Ok(String::from(
            core::str::from_utf8(self.blob()?).map_err(|_| invalid())?,
        ))
    }
    fn done(&self) -> Result<(), Error> {
        if self.at == self.b.len() {
            Ok(())
        } else {
            Err(invalid())
        }
    }
    fn count(&mut self, width: usize) -> Result<usize, Error> {
        let n = self.u32()? as usize;
        if n > MAX_ELEMENTS {
            return Err(large());
        }
        if n > (self.b.len() - self.at) / width {
            return Err(invalid());
        }
        Ok(n)
    }
}
fn event_name(s: &str, node: bool) -> bool {
    if s.is_empty() || s.len() > if node { 1024 } else { 255 } {
        return false;
    }
    let b = s.as_bytes();
    if node && (b.len() < 2 || b[0] != b'/') {
        return false;
    }
    let mut first = true;
    for c in &b[if node { 1 } else { 0 }..] {
        if *c == if node { b'/' } else { b'.' } {
            if first {
                return false;
            }
            first = true;
            continue;
        }
        if !c.is_ascii_alphabetic() && *c != b'_' && !(c.is_ascii_digit() && !first) {
            return false;
        }
        first = false;
    }
    !first
}
fn stamp(seconds: u32, nanos: u32) -> Result<(), Error> {
    if seconds > i32::MAX as u32 || nanos >= 1_000_000_000 {
        Err(invalid())
    } else {
        Ok(())
    }
}
/// Encode one typed builtin value to the canonical bounded wire representation.
/// Implementations also decode complete envelopes, rejecting trailing bytes.
pub trait Canonical: Sized {
    /// Encode a fresh bounded value. This does not imply host authorization.
    fn encode(&self) -> Result<Vec<u8>, Error>;
    /// Decode one complete checked canonical value.
    fn decode(bytes: &[u8]) -> Result<Self, Error>;
}
impl Canonical for String {
    fn encode(&self) -> Result<Vec<u8>, Error> {
        let mut w = Writer::new();
        w.text(self)?;
        Ok(w.0)
    }
    fn decode(b: &[u8]) -> Result<Self, Error> {
        let mut r = Reader::new(b)?;
        let v = r.text()?;
        r.done()?;
        Ok(v)
    }
}
impl Canonical for bool {
    fn encode(&self) -> Result<Vec<u8>, Error> {
        Ok(alloc::vec![u8::from(*self)])
    }
    fn decode(b: &[u8]) -> Result<Self, Error> {
        codec::decode_bool(b)
    }
}
/// Header fields carried by Image and Transform. ROS2 sequence is always zero;
/// ROS1 native transports may generate a Header sequence of their own.
#[derive(Debug, PartialEq)]
pub struct Header {
    /// Header sequence; supply zero for ROS2.
    pub sequence: u32,
    /// Full u32 seconds for ROS1 Image. ROS2 Image and both Transform adapters
    /// require at most i32::MAX, checked by their selected host/typed adapter.
    pub seconds: u32,
    /// Nanoseconds below one billion.
    pub nanoseconds: u32,
    /// Canonical UTF-8 frame name, checked against the host grant.
    pub frame: String,
}
impl Header {
    fn write(&self, w: &mut Writer) -> Result<(), Error> {
        if self.nanoseconds >= 1_000_000_000 {
            return Err(invalid());
        }
        w.u32(self.sequence)?;
        w.u32(self.seconds)?;
        w.u32(self.nanoseconds)?;
        w.text(&self.frame)
    }
    fn read(r: &mut Reader<'_>) -> Result<Self, Error> {
        let v = Self {
            sequence: r.u32()?,
            seconds: r.u32()?,
            nanoseconds: r.u32()?,
            frame: r.text()?,
        };
        if v.nanoseconds >= 1_000_000_000 {
            return Err(invalid());
        }
        Ok(v)
    }
}
/// Canonical Image builtin. Encoding carries checked fields and full bounded
/// bytes; the host additionally validates supported encoding/step/dimensions.
#[derive(Debug, PartialEq)]
pub struct Image {
    /// Image timestamp and frame header.
    pub header: Header,
    /// Pixel row count.
    pub height: u32,
    /// Pixel column count.
    pub width: u32,
    /// Explicit native encoding name, such as rgb8.
    pub encoding: String,
    /// Endianness flag encoded as a canonical Bool.
    pub big_endian: bool,
    /// Bytes per row, including any explicit padding.
    pub step: u32,
    /// Raw image bytes; no compressed format is inferred.
    pub data: Vec<u8>,
}
impl Canonical for Image {
    fn encode(&self) -> Result<Vec<u8>, Error> {
        let mut w = Writer::new();
        self.header.write(&mut w)?;
        w.u32(self.height)?;
        w.u32(self.width)?;
        w.text(&self.encoding)?;
        w.u8(u8::from(self.big_endian))?;
        w.u32(self.step)?;
        w.blob(&self.data)?;
        Ok(w.0)
    }
    fn decode(b: &[u8]) -> Result<Self, Error> {
        let mut r = Reader::new(b)?;
        let header = Header::read(&mut r)?;
        let height = r.u32()?;
        let width = r.u32()?;
        let encoding = r.text()?;
        let flag = r.u8()?;
        if flag > 1 {
            return Err(invalid());
        }
        let step = r.u32()?;
        let data = r.blob()?.to_vec();
        r.done()?;
        Ok(Self {
            header,
            height,
            width,
            encoding,
            big_endian: flag != 0,
            step,
            data,
        })
    }
}
/// Canonical TransformStamped. All seven values must be finite; frame matching,
/// timestamp/history semantics and optional bounds are enforced by the host.
#[derive(Debug, PartialEq)]
pub struct Transform {
    /// Parent frame and timestamp.
    pub header: Header,
    /// Child frame fixed by the explicitly granted TF frame pair.
    pub child_frame: String,
    /// Translation xyz followed by quaternion xyzw; no normalization is performed.
    pub values: [f64; 7],
}
impl Canonical for Transform {
    fn encode(&self) -> Result<Vec<u8>, Error> {
        stamp(self.header.seconds, self.header.nanoseconds)?;
        let mut w = Writer::new();
        self.header.write(&mut w)?;
        w.text(&self.child_frame)?;
        for v in self.values {
            w.real(v)?;
        }
        Ok(w.0)
    }
    fn decode(b: &[u8]) -> Result<Self, Error> {
        let mut r = Reader::new(b)?;
        let header = Header::read(&mut r)?;
        stamp(header.seconds, header.nanoseconds)?;
        let child_frame = r.text()?;
        let mut values = [0.0; 7];
        for v in &mut values {
            *v = r.real()?;
        }
        r.done()?;
        Ok(Self {
            header,
            child_frame,
            values,
        })
    }
}
/// Parameter adapter kinds. Select the exact configured type when decoding;
/// raw parameter payloads carry no implicit type tag or numeric coercion.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum ParameterKind {
    /// One canonical Bool.
    Bool,
    /// Signed four-byte scalar, ROS1 only.
    Int32,
    /// Signed eight-byte scalar, ROS2 only.
    Int64,
    /// Finite Float64 scalar.
    Float64,
    /// Canonical UTF-8 String; scalar NUL allowance is wire-specific.
    String,
    /// u32 count followed by canonical Bool bytes.
    BoolArray,
    /// u32 count followed by signed i32 values, ROS1 only.
    Int32Array,
    /// u32 count followed by signed i64 values, ROS2 only.
    Int64Array,
    /// u32 count followed by finite Float64 values.
    Float64Array,
    /// u32 count followed by UTF-8 NUL-free Strings.
    StringArray,
    /// u32 count followed by unsigned byte values, ROS2 only.
    ByteArray,
}
/// Selected canonical parameter value. Numeric bounds and positive array
/// element quotas must be supplied in the host policy; SDK types grant nothing.
#[derive(Debug, PartialEq)]
pub enum ParameterValue {
    /// Bool value.
    Bool(bool),
    /// Signed int32 value for ROS1.
    Int32(i32),
    /// Signed int64 value for ROS2, never converted through Float64.
    Int64(i64),
    /// Finite Float64 value, preserving signed zero bits.
    Float64(f64),
    /// Canonical UTF-8 scalar String.
    String(String),
    /// Canonical Bool values.
    BoolArray(Vec<bool>),
    /// Signed ROS1 int32 values.
    Int32Array(Vec<i32>),
    /// Signed ROS2 int64 values.
    Int64Array(Vec<i64>),
    /// Finite Float64 values.
    Float64Array(Vec<f64>),
    /// UTF-8 NUL-free String values; ROS1 additionally checks XML1.0 validity.
    StringArray(Vec<String>),
    /// Unsigned bytes, distinct from Bool values.
    ByteArray(Vec<u8>),
}
impl ParameterValue {
    /// Return the exact adapter kind; there is no numeric widening/coercion.
    pub fn kind(&self) -> ParameterKind {
        match self {
            Self::Bool(_) => ParameterKind::Bool,
            Self::Int32(_) => ParameterKind::Int32,
            Self::Int64(_) => ParameterKind::Int64,
            Self::Float64(_) => ParameterKind::Float64,
            Self::String(_) => ParameterKind::String,
            Self::BoolArray(_) => ParameterKind::BoolArray,
            Self::Int32Array(_) => ParameterKind::Int32Array,
            Self::Int64Array(_) => ParameterKind::Int64Array,
            Self::Float64Array(_) => ParameterKind::Float64Array,
            Self::StringArray(_) => ParameterKind::StringArray,
            Self::ByteArray(_) => ParameterKind::ByteArray,
        }
    }
    /// Encode the raw selected parameter value, without an invented outer tag.
    pub fn encode(&self) -> Result<Vec<u8>, Error> {
        let mut w = Writer::new();
        macro_rules! array {
            ($v:expr, $write:expr) => {{
                let v = $v;
                if v.len() > MAX_ELEMENTS {
                    return Err(large());
                }
                w.u32(v.len() as u32)?;
                for x in v {
                    ($write)(&mut w, x)?;
                }
            }};
        }
        match self {
            Self::Bool(v) => w.u8(u8::from(*v))?,
            Self::Int32(v) => w.u32(*v as u32)?,
            Self::Int64(v) => w.u64(*v as u64)?,
            Self::Float64(v) => w.real(*v)?,
            Self::String(v) => w.text(v)?,
            Self::BoolArray(v) => array!(v, |w: &mut Writer, x: &bool| w.u8(u8::from(*x))),
            Self::Int32Array(v) => array!(v, |w: &mut Writer, x: &i32| w.u32(*x as u32)),
            Self::Int64Array(v) => array!(v, |w: &mut Writer, x: &i64| w.u64(*x as u64)),
            Self::Float64Array(v) => array!(v, |w: &mut Writer, x: &f64| w.real(*x)),
            Self::StringArray(v) => array!(v, |w: &mut Writer, x: &String| if x
                .as_bytes()
                .contains(&0)
            {
                Err(invalid())
            } else {
                w.text(x)
            }),
            Self::ByteArray(v) => {
                if v.len() > MAX_ELEMENTS {
                    return Err(large());
                }
                w.blob(v)?;
            }
        };
        Ok(w.0)
    }
    /// Decode exactly one raw value using the explicit configured adapter kind.
    pub fn decode(kind: ParameterKind, b: &[u8]) -> Result<Self, Error> {
        let mut r = Reader::new(b)?;
        macro_rules! array {
            ($width:expr, $read:expr, $variant:ident) => {{
                let n = r.count($width)?;
                let mut v = Vec::with_capacity(n);
                for _ in 0..n {
                    v.push(($read)(&mut r)?);
                }
                Self::$variant(v)
            }};
        }
        let v = match kind {
            ParameterKind::Bool => {
                let v = r.u8()?;
                if v > 1 {
                    return Err(invalid());
                }
                Self::Bool(v != 0)
            }
            ParameterKind::Int32 => Self::Int32(r.u32()? as i32),
            ParameterKind::Int64 => Self::Int64(r.u64()? as i64),
            ParameterKind::Float64 => Self::Float64(r.real()?),
            ParameterKind::String => Self::String(r.text()?),
            ParameterKind::BoolArray => array!(
                1,
                |r: &mut Reader<'_>| {
                    let v = r.u8()?;
                    if v > 1 {
                        Err(invalid())
                    } else {
                        Ok(v != 0)
                    }
                },
                BoolArray
            ),
            ParameterKind::Int32Array => array!(
                4,
                |r: &mut Reader<'_>| Ok::<i32, Error>(r.u32()? as i32),
                Int32Array
            ),
            ParameterKind::Int64Array => array!(
                8,
                |r: &mut Reader<'_>| Ok::<i64, Error>(r.u64()? as i64),
                Int64Array
            ),
            ParameterKind::Float64Array => array!(8, |r: &mut Reader<'_>| r.real(), Float64Array),
            ParameterKind::StringArray => array!(
                4,
                |r: &mut Reader<'_>| {
                    let s = r.text()?;
                    if s.as_bytes().contains(&0) {
                        Err(invalid())
                    } else {
                        Ok(s)
                    }
                },
                StringArray
            ),
            ParameterKind::ByteArray => {
                let n = r.count(1)?;
                Self::ByteArray(r.bytes(n)?.to_vec())
            }
        };
        r.done()?;
        Ok(v)
    }
}
/// ROS2 typed parameter-event projection tags, preserving ABI values0..9.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum EventTag {
    /// Separate deletion-only NOT_SET notification; no previous type is attested.
    Deletion = 0,
    /// Bool projection.
    Bool = 1,
    /// Int64 projection.
    Int64 = 2,
    /// Finite Float64 projection.
    Float64 = 3,
    /// UTF-8 scalar String projection.
    String = 4,
    /// Unsigned byte-array projection.
    ByteArray = 5,
    /// Bool-array projection.
    BoolArray = 6,
    /// Int64-array projection.
    Int64Array = 7,
    /// Float64-array projection.
    Float64Array = 8,
    /// UTF-8 NUL-free String-array projection.
    StringArray = 9,
}
impl EventTag {
    fn from_byte(v: u8) -> Result<Self, Error> {
        Ok(match v {
            0 => Self::Deletion,
            1 => Self::Bool,
            2 => Self::Int64,
            3 => Self::Float64,
            4 => Self::String,
            5 => Self::ByteArray,
            6 => Self::BoolArray,
            7 => Self::Int64Array,
            8 => Self::Float64Array,
            9 => Self::StringArray,
            _ => return Err(invalid()),
        })
    }
    fn parameter(self) -> Result<ParameterKind, Error> {
        Ok(match self {
            Self::Deletion => return Err(invalid()),
            Self::Bool => ParameterKind::Bool,
            Self::Int64 => ParameterKind::Int64,
            Self::Float64 => ParameterKind::Float64,
            Self::String => ParameterKind::String,
            Self::ByteArray => ParameterKind::ByteArray,
            Self::BoolArray => ParameterKind::BoolArray,
            Self::Int64Array => ParameterKind::Int64Array,
            Self::Float64Array => ParameterKind::Float64Array,
            Self::StringArray => ParameterKind::StringArray,
        })
    }
}
/// Independently authorized typed ROS2 parameter event. A deleted event has no
/// value; wire decoding cannot prove the native prior value/type/bounds.
#[derive(Debug, PartialEq)]
pub struct TypedParameterEvent {
    /// Nonnegative event seconds.
    pub seconds: u32,
    /// Event nanoseconds below one billion.
    pub nanoseconds: u32,
    /// New1, changed2, deleted3.
    pub kind: u8,
    /// Exact projection tag selected by the host grant.
    pub tag: EventTag,
    /// Exact configured absolute ROS2 node.
    pub node: String,
    /// Exact configured parameter name.
    pub name: String,
    /// Selected value for new/changed; None for deletion.
    pub value: Option<ParameterValue>,
}
impl Canonical for TypedParameterEvent {
    fn encode(&self) -> Result<Vec<u8>, Error> {
        stamp(self.seconds, self.nanoseconds)?;
        if !(1..=3).contains(&self.kind)
            || !event_name(&self.node, true)
            || !event_name(&self.name, false)
        {
            return Err(invalid());
        }
        let payload = if self.kind == 3 {
            if self.value.is_some() {
                return Err(invalid());
            }
            Vec::new()
        } else {
            let value = self.value.as_ref().ok_or_else(invalid)?;
            if value.kind() != self.tag.parameter()? {
                return Err(invalid());
            }
            value.encode()?
        };
        let mut w = Writer::new();
        w.u32(self.seconds)?;
        w.u32(self.nanoseconds)?;
        w.u8(self.kind)?;
        w.u8(2)?;
        w.u8(self.tag as u8)?;
        w.text(&self.node)?;
        w.text(&self.name)?;
        w.blob(&payload)?;
        Ok(w.0)
    }
    fn decode(b: &[u8]) -> Result<Self, Error> {
        let mut r = Reader::new(b)?;
        let seconds = r.u32()?;
        let nanoseconds = r.u32()?;
        stamp(seconds, nanoseconds)?;
        let kind = r.u8()?;
        if !(1..=3).contains(&kind) || r.u8()? != 2 {
            return Err(invalid());
        }
        let tag = EventTag::from_byte(r.u8()?)?;
        let node = r.text()?;
        let name = r.text()?;
        if !event_name(&node, true) || !event_name(&name, false) {
            return Err(invalid());
        }
        let payload = r.blob()?;
        r.done()?;
        let value = if kind == 3 {
            if !payload.is_empty() {
                return Err(invalid());
            }
            None
        } else {
            Some(ParameterValue::decode(tag.parameter()?, payload)?)
        };
        Ok(Self {
            seconds,
            nanoseconds,
            kind,
            tag,
            node,
            name,
            value,
        })
    }
}
/// Legacy ROS2 String parameter event. This requires its own legacy String
/// subscription grant; it does not imply any typed projection authority.
#[derive(Debug, PartialEq)]
pub struct LegacyParameterEvent {
    /// Nonnegative seconds.
    pub seconds: u32,
    /// Nanoseconds below one billion.
    pub nanoseconds: u32,
    /// New1, changed2, deleted3.
    pub kind: u8,
    /// Selected absolute node.
    pub node: String,
    /// Selected parameter name.
    pub name: String,
    /// Selected String value, empty on deletion.
    pub value: String,
}
impl Canonical for LegacyParameterEvent {
    fn encode(&self) -> Result<Vec<u8>, Error> {
        stamp(self.seconds, self.nanoseconds)?;
        if !(1..=3).contains(&self.kind) || (self.kind == 3 && !self.value.is_empty()) {
            return Err(invalid());
        }
        let mut w = Writer::new();
        w.u32(self.seconds)?;
        w.u32(self.nanoseconds)?;
        w.u8(self.kind)?;
        w.text(&self.node)?;
        w.text(&self.name)?;
        w.text(&self.value)?;
        Ok(w.0)
    }
    fn decode(b: &[u8]) -> Result<Self, Error> {
        let mut r = Reader::new(b)?;
        let v = Self {
            seconds: r.u32()?,
            nanoseconds: r.u32()?,
            kind: r.u8()?,
            node: r.text()?,
            name: r.text()?,
            value: r.text()?,
        };
        r.done()?;
        stamp(v.seconds, v.nanoseconds)?;
        if !(1..=3).contains(&v.kind) || (v.kind == 3 && !v.value.is_empty()) {
            return Err(invalid());
        }
        Ok(v)
    }
}
/// Canonical lifecycle state response: one state ID and a UTF-8 label.
#[derive(Debug, PartialEq)]
pub struct LifecycleState {
    /// Native lifecycle state ID.
    pub id: u8,
    /// Native lifecycle state label.
    pub label: String,
}
impl Canonical for LifecycleState {
    fn encode(&self) -> Result<Vec<u8>, Error> {
        let mut w = Writer::new();
        w.u8(self.id)?;
        w.text(&self.label)?;
        Ok(w.0)
    }
    fn decode(b: &[u8]) -> Result<Self, Error> {
        let mut r = Reader::new(b)?;
        let v = Self {
            id: r.u8()?,
            label: r.text()?,
        };
        r.done()?;
        Ok(v)
    }
}

/// Owned finite Twist adapter over the fixed six-field canonical layout.
impl Canonical for [f64; 6] {
    fn encode(&self) -> Result<Vec<u8>, Error> {
        let mut b = [0; 48];
        codec::encode_twist(*self, &mut b)?;
        Ok(b.to_vec())
    }
    fn decode(b: &[u8]) -> Result<Self, Error> {
        codec::decode_twist(b)
    }
}
