//! Generated fixed topic values and checked codecs; no I/O or authority.
//! Add this file as a module to a guest using roswasm with its alloc feature.
extern crate alloc;
use alloc::vec::Vec;
use roswasm::{Error,codes};
use roswasm::typed::Canonical;
/// Fixed ROS1 example_interfaces_custom/Packet value. All arrays have exact lengths.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Example_interfaces_customPacket {
    /// Native fixed field sequence.
    pub sequence: i32,
    /// Native fixed field active.
    pub active: bool,
    /// Native fixed field measurement.
    pub measurement: f64,
    /// Native fixed field tag.
    pub tag: [u8;4],
    /// Native fixed field samples.
    pub samples: [i16;3],
}
impl Default for Ros1Example_interfaces_customPacket {fn default()->Self {Self {
sequence:Default::default(),
active:Default::default(),
measurement:Default::default(),
tag:core::array::from_fn(|_|Default::default()),
samples:core::array::from_fn(|_|Default::default()),
}}}
impl Ros1Example_interfaces_customPacket {
    /// Exact canonical byte count; no native serialization is implied.
    pub const WIRE_BYTES:usize=23;
    /// Compile-time canonical schema identity; the host independently verifies it.
    pub const CODEC_IDENTITY:&'static str="UWVMROS1_e68d0934f545cd81105ac13682379dc84f023c860c070c046e69c7f70e94d407";
    /// Validate every finite field without allocating output.
    pub fn valid(&self)->Result<(),Error> {
        if !self.measurement.is_finite() {return Err(Error::new(codes::INVALID));}
        Ok(())
    }
    /// Encode into separate caller-owned bytes after complete validation.
    pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error> {
        if out.len()<23 {return Err(Error::new(codes::TOO_LARGE));}self.valid()?;
        out[0..4].copy_from_slice(&self.sequence.to_le_bytes());
        out[4]=u8::from(self.active);
        out[5..13].copy_from_slice(&self.measurement.to_le_bytes());
        out[13..14].copy_from_slice(&self.tag[0].to_le_bytes());
        out[14..15].copy_from_slice(&self.tag[1].to_le_bytes());
        out[15..16].copy_from_slice(&self.tag[2].to_le_bytes());
        out[16..17].copy_from_slice(&self.tag[3].to_le_bytes());
        out[17..19].copy_from_slice(&self.samples[0].to_le_bytes());
        out[19..21].copy_from_slice(&self.samples[1].to_le_bytes());
        out[21..23].copy_from_slice(&self.samples[2].to_le_bytes());
        Ok(23)
    }
}
impl Canonical for Ros1Example_interfaces_customPacket {
    /// Allocate exactly one fixed output after validating all fields.
    fn encode(&self)->Result<Vec<u8>,Error> {self.valid()?;let mut out=alloc::vec![0u8;23];self.encode_into(&mut out)?;Ok(out)}
    /// Decode exactly one fixed record; truncation and trailing bytes fail.
    fn decode(bytes:&[u8])->Result<Self,Error> {if bytes.len()!=23 {return Err(Error::new(codes::INVALID));}let mut next=Self::default();
        next.sequence=i32::from_le_bytes(bytes[0..4].try_into().map_err(|_|Error::new(codes::INVALID))?);
        if bytes[4]>1 {return Err(Error::new(codes::INVALID));}next.active=bytes[4]!=0;
        next.measurement=f64::from_le_bytes(bytes[5..13].try_into().map_err(|_|Error::new(codes::INVALID))?);
        if !next.measurement.is_finite() {return Err(Error::new(codes::INVALID));}
        next.tag[0]=u8::from_le_bytes(bytes[13..14].try_into().map_err(|_|Error::new(codes::INVALID))?);
        next.tag[1]=u8::from_le_bytes(bytes[14..15].try_into().map_err(|_|Error::new(codes::INVALID))?);
        next.tag[2]=u8::from_le_bytes(bytes[15..16].try_into().map_err(|_|Error::new(codes::INVALID))?);
        next.tag[3]=u8::from_le_bytes(bytes[16..17].try_into().map_err(|_|Error::new(codes::INVALID))?);
        next.samples[0]=i16::from_le_bytes(bytes[17..19].try_into().map_err(|_|Error::new(codes::INVALID))?);
        next.samples[1]=i16::from_le_bytes(bytes[19..21].try_into().map_err(|_|Error::new(codes::INVALID))?);
        next.samples[2]=i16::from_le_bytes(bytes[21..23].try_into().map_err(|_|Error::new(codes::INVALID))?);
        Ok(next)
    }
}
/// Fixed ROS1 example_interfaces_custom/ScalarRecord value. All arrays have exact lengths.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Example_interfaces_customScalarRecord {
    /// Native fixed field enabled.
    pub enabled: bool,
    /// Native fixed field unsigned_small.
    pub unsigned_small: u8,
    /// Native fixed field signed_small.
    pub signed_small: i8,
    /// Native fixed field unsigned_medium.
    pub unsigned_medium: u16,
    /// Native fixed field signed_medium.
    pub signed_medium: i16,
    /// Native fixed field unsigned_word.
    pub unsigned_word: u32,
    /// Native fixed field signed_word.
    pub signed_word: i32,
    /// Native fixed field unsigned_large.
    pub unsigned_large: u64,
    /// Native fixed field signed_large.
    pub signed_large: i64,
    /// Native fixed field single_value.
    pub single_value: f32,
    /// Native fixed field double_value.
    pub double_value: f64,
}
impl Default for Ros1Example_interfaces_customScalarRecord {fn default()->Self {Self {
enabled:Default::default(),
unsigned_small:Default::default(),
signed_small:Default::default(),
unsigned_medium:Default::default(),
signed_medium:Default::default(),
unsigned_word:Default::default(),
signed_word:Default::default(),
unsigned_large:Default::default(),
signed_large:Default::default(),
single_value:Default::default(),
double_value:Default::default(),
}}}
impl Ros1Example_interfaces_customScalarRecord {
    /// Exact canonical byte count; no native serialization is implied.
    pub const WIRE_BYTES:usize=43;
    /// Compile-time canonical schema identity; the host independently verifies it.
    pub const CODEC_IDENTITY:&'static str="UWVMROS1_dfab9988d24ce356a6a914d4ae6f6a7c8c44dc8118cfc768e41fa8bf47f386c6";
    /// Validate every finite field without allocating output.
    pub fn valid(&self)->Result<(),Error> {
        if !self.single_value.is_finite() {return Err(Error::new(codes::INVALID));}
        if !self.double_value.is_finite() {return Err(Error::new(codes::INVALID));}
        Ok(())
    }
    /// Encode into separate caller-owned bytes after complete validation.
    pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error> {
        if out.len()<43 {return Err(Error::new(codes::TOO_LARGE));}self.valid()?;
        out[0]=u8::from(self.enabled);
        out[1..2].copy_from_slice(&self.unsigned_small.to_le_bytes());
        out[2..3].copy_from_slice(&self.signed_small.to_le_bytes());
        out[3..5].copy_from_slice(&self.unsigned_medium.to_le_bytes());
        out[5..7].copy_from_slice(&self.signed_medium.to_le_bytes());
        out[7..11].copy_from_slice(&self.unsigned_word.to_le_bytes());
        out[11..15].copy_from_slice(&self.signed_word.to_le_bytes());
        out[15..23].copy_from_slice(&self.unsigned_large.to_le_bytes());
        out[23..31].copy_from_slice(&self.signed_large.to_le_bytes());
        out[31..35].copy_from_slice(&self.single_value.to_le_bytes());
        out[35..43].copy_from_slice(&self.double_value.to_le_bytes());
        Ok(43)
    }
}
impl Canonical for Ros1Example_interfaces_customScalarRecord {
    /// Allocate exactly one fixed output after validating all fields.
    fn encode(&self)->Result<Vec<u8>,Error> {self.valid()?;let mut out=alloc::vec![0u8;43];self.encode_into(&mut out)?;Ok(out)}
    /// Decode exactly one fixed record; truncation and trailing bytes fail.
    fn decode(bytes:&[u8])->Result<Self,Error> {if bytes.len()!=43 {return Err(Error::new(codes::INVALID));}let mut next=Self::default();
        if bytes[0]>1 {return Err(Error::new(codes::INVALID));}next.enabled=bytes[0]!=0;
        next.unsigned_small=u8::from_le_bytes(bytes[1..2].try_into().map_err(|_|Error::new(codes::INVALID))?);
        next.signed_small=i8::from_le_bytes(bytes[2..3].try_into().map_err(|_|Error::new(codes::INVALID))?);
        next.unsigned_medium=u16::from_le_bytes(bytes[3..5].try_into().map_err(|_|Error::new(codes::INVALID))?);
        next.signed_medium=i16::from_le_bytes(bytes[5..7].try_into().map_err(|_|Error::new(codes::INVALID))?);
        next.unsigned_word=u32::from_le_bytes(bytes[7..11].try_into().map_err(|_|Error::new(codes::INVALID))?);
        next.signed_word=i32::from_le_bytes(bytes[11..15].try_into().map_err(|_|Error::new(codes::INVALID))?);
        next.unsigned_large=u64::from_le_bytes(bytes[15..23].try_into().map_err(|_|Error::new(codes::INVALID))?);
        next.signed_large=i64::from_le_bytes(bytes[23..31].try_into().map_err(|_|Error::new(codes::INVALID))?);
        next.single_value=f32::from_le_bytes(bytes[31..35].try_into().map_err(|_|Error::new(codes::INVALID))?);
        if !next.single_value.is_finite() {return Err(Error::new(codes::INVALID));}
        next.double_value=f64::from_le_bytes(bytes[35..43].try_into().map_err(|_|Error::new(codes::INVALID))?);
        if !next.double_value.is_finite() {return Err(Error::new(codes::INVALID));}
        Ok(next)
    }
}
/// Fixed ROS1 example_interfaces_custom/NestedRecord value. All arrays have exact lengths.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Example_interfaces_customNestedRecord {
    /// Native fixed field readings.
    pub readings: [Ros1Example_interfaces_customScalarRecord;2],
    /// Native fixed field positions.
    pub positions: [i32;3],
}
impl Default for Ros1Example_interfaces_customNestedRecord {fn default()->Self {Self {
readings:core::array::from_fn(|_|Default::default()),
positions:core::array::from_fn(|_|Default::default()),
}}}
impl Ros1Example_interfaces_customNestedRecord {
    /// Exact canonical byte count; no native serialization is implied.
    pub const WIRE_BYTES:usize=98;
    /// Compile-time canonical schema identity; the host independently verifies it.
    pub const CODEC_IDENTITY:&'static str="UWVMROS1_b0856889a47eb923055e44730a21c04d498472c373a1b9539a8f213a7c09ae5c";
    /// Validate every finite field without allocating output.
    pub fn valid(&self)->Result<(),Error> {
        if !self.readings[0].single_value.is_finite() {return Err(Error::new(codes::INVALID));}
        if !self.readings[0].double_value.is_finite() {return Err(Error::new(codes::INVALID));}
        if !self.readings[1].single_value.is_finite() {return Err(Error::new(codes::INVALID));}
        if !self.readings[1].double_value.is_finite() {return Err(Error::new(codes::INVALID));}
        Ok(())
    }
    /// Encode into separate caller-owned bytes after complete validation.
    pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error> {
        if out.len()<98 {return Err(Error::new(codes::TOO_LARGE));}self.valid()?;
        out[0]=u8::from(self.readings[0].enabled);
        out[1..2].copy_from_slice(&self.readings[0].unsigned_small.to_le_bytes());
        out[2..3].copy_from_slice(&self.readings[0].signed_small.to_le_bytes());
        out[3..5].copy_from_slice(&self.readings[0].unsigned_medium.to_le_bytes());
        out[5..7].copy_from_slice(&self.readings[0].signed_medium.to_le_bytes());
        out[7..11].copy_from_slice(&self.readings[0].unsigned_word.to_le_bytes());
        out[11..15].copy_from_slice(&self.readings[0].signed_word.to_le_bytes());
        out[15..23].copy_from_slice(&self.readings[0].unsigned_large.to_le_bytes());
        out[23..31].copy_from_slice(&self.readings[0].signed_large.to_le_bytes());
        out[31..35].copy_from_slice(&self.readings[0].single_value.to_le_bytes());
        out[35..43].copy_from_slice(&self.readings[0].double_value.to_le_bytes());
        out[43]=u8::from(self.readings[1].enabled);
        out[44..45].copy_from_slice(&self.readings[1].unsigned_small.to_le_bytes());
        out[45..46].copy_from_slice(&self.readings[1].signed_small.to_le_bytes());
        out[46..48].copy_from_slice(&self.readings[1].unsigned_medium.to_le_bytes());
        out[48..50].copy_from_slice(&self.readings[1].signed_medium.to_le_bytes());
        out[50..54].copy_from_slice(&self.readings[1].unsigned_word.to_le_bytes());
        out[54..58].copy_from_slice(&self.readings[1].signed_word.to_le_bytes());
        out[58..66].copy_from_slice(&self.readings[1].unsigned_large.to_le_bytes());
        out[66..74].copy_from_slice(&self.readings[1].signed_large.to_le_bytes());
        out[74..78].copy_from_slice(&self.readings[1].single_value.to_le_bytes());
        out[78..86].copy_from_slice(&self.readings[1].double_value.to_le_bytes());
        out[86..90].copy_from_slice(&self.positions[0].to_le_bytes());
        out[90..94].copy_from_slice(&self.positions[1].to_le_bytes());
        out[94..98].copy_from_slice(&self.positions[2].to_le_bytes());
        Ok(98)
    }
}
impl Canonical for Ros1Example_interfaces_customNestedRecord {
    /// Allocate exactly one fixed output after validating all fields.
    fn encode(&self)->Result<Vec<u8>,Error> {self.valid()?;let mut out=alloc::vec![0u8;98];self.encode_into(&mut out)?;Ok(out)}
    /// Decode exactly one fixed record; truncation and trailing bytes fail.
    fn decode(bytes:&[u8])->Result<Self,Error> {if bytes.len()!=98 {return Err(Error::new(codes::INVALID));}let mut next=Self::default();
        if bytes[0]>1 {return Err(Error::new(codes::INVALID));}next.readings[0].enabled=bytes[0]!=0;
        next.readings[0].unsigned_small=u8::from_le_bytes(bytes[1..2].try_into().map_err(|_|Error::new(codes::INVALID))?);
        next.readings[0].signed_small=i8::from_le_bytes(bytes[2..3].try_into().map_err(|_|Error::new(codes::INVALID))?);
        next.readings[0].unsigned_medium=u16::from_le_bytes(bytes[3..5].try_into().map_err(|_|Error::new(codes::INVALID))?);
        next.readings[0].signed_medium=i16::from_le_bytes(bytes[5..7].try_into().map_err(|_|Error::new(codes::INVALID))?);
        next.readings[0].unsigned_word=u32::from_le_bytes(bytes[7..11].try_into().map_err(|_|Error::new(codes::INVALID))?);
        next.readings[0].signed_word=i32::from_le_bytes(bytes[11..15].try_into().map_err(|_|Error::new(codes::INVALID))?);
        next.readings[0].unsigned_large=u64::from_le_bytes(bytes[15..23].try_into().map_err(|_|Error::new(codes::INVALID))?);
        next.readings[0].signed_large=i64::from_le_bytes(bytes[23..31].try_into().map_err(|_|Error::new(codes::INVALID))?);
        next.readings[0].single_value=f32::from_le_bytes(bytes[31..35].try_into().map_err(|_|Error::new(codes::INVALID))?);
        if !next.readings[0].single_value.is_finite() {return Err(Error::new(codes::INVALID));}
        next.readings[0].double_value=f64::from_le_bytes(bytes[35..43].try_into().map_err(|_|Error::new(codes::INVALID))?);
        if !next.readings[0].double_value.is_finite() {return Err(Error::new(codes::INVALID));}
        if bytes[43]>1 {return Err(Error::new(codes::INVALID));}next.readings[1].enabled=bytes[43]!=0;
        next.readings[1].unsigned_small=u8::from_le_bytes(bytes[44..45].try_into().map_err(|_|Error::new(codes::INVALID))?);
        next.readings[1].signed_small=i8::from_le_bytes(bytes[45..46].try_into().map_err(|_|Error::new(codes::INVALID))?);
        next.readings[1].unsigned_medium=u16::from_le_bytes(bytes[46..48].try_into().map_err(|_|Error::new(codes::INVALID))?);
        next.readings[1].signed_medium=i16::from_le_bytes(bytes[48..50].try_into().map_err(|_|Error::new(codes::INVALID))?);
        next.readings[1].unsigned_word=u32::from_le_bytes(bytes[50..54].try_into().map_err(|_|Error::new(codes::INVALID))?);
        next.readings[1].signed_word=i32::from_le_bytes(bytes[54..58].try_into().map_err(|_|Error::new(codes::INVALID))?);
        next.readings[1].unsigned_large=u64::from_le_bytes(bytes[58..66].try_into().map_err(|_|Error::new(codes::INVALID))?);
        next.readings[1].signed_large=i64::from_le_bytes(bytes[66..74].try_into().map_err(|_|Error::new(codes::INVALID))?);
        next.readings[1].single_value=f32::from_le_bytes(bytes[74..78].try_into().map_err(|_|Error::new(codes::INVALID))?);
        if !next.readings[1].single_value.is_finite() {return Err(Error::new(codes::INVALID));}
        next.readings[1].double_value=f64::from_le_bytes(bytes[78..86].try_into().map_err(|_|Error::new(codes::INVALID))?);
        if !next.readings[1].double_value.is_finite() {return Err(Error::new(codes::INVALID));}
        next.positions[0]=i32::from_le_bytes(bytes[86..90].try_into().map_err(|_|Error::new(codes::INVALID))?);
        next.positions[1]=i32::from_le_bytes(bytes[90..94].try_into().map_err(|_|Error::new(codes::INVALID))?);
        next.positions[2]=i32::from_le_bytes(bytes[94..98].try_into().map_err(|_|Error::new(codes::INVALID))?);
        Ok(next)
    }
}
/// Fixed ROS2 example_interfaces_custom/msg/FixedSample value. All arrays have exact lengths.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Example_interfaces_customFixedSample {
    /// Native fixed field sequence.
    pub sequence: u32,
    /// Native fixed field enabled.
    pub enabled: bool,
    /// Native fixed field temperature.
    pub temperature: f32,
    /// Native fixed field acceleration.
    pub acceleration: [f64;3],
    /// Native fixed field tag.
    pub tag: [u8;8],
    /// Native fixed field thresholds.
    pub thresholds: [i16;2],
}
impl Default for Ros2Example_interfaces_customFixedSample {fn default()->Self {Self {
sequence:Default::default(),
enabled:Default::default(),
temperature:Default::default(),
acceleration:core::array::from_fn(|_|Default::default()),
tag:core::array::from_fn(|_|Default::default()),
thresholds:core::array::from_fn(|_|Default::default()),
}}}
impl Ros2Example_interfaces_customFixedSample {
    /// Exact canonical byte count; no native serialization is implied.
    pub const WIRE_BYTES:usize=45;
    /// Compile-time canonical schema identity; the host independently verifies it.
    pub const CODEC_IDENTITY:&'static str="UWVMROS1_b794a6eb1863626cf279af1fbad1f071fce945d6585f3fdb22979c8f24e32663";
    /// Validate every finite field without allocating output.
    pub fn valid(&self)->Result<(),Error> {
        if !self.temperature.is_finite() {return Err(Error::new(codes::INVALID));}
        if !self.acceleration[0].is_finite() {return Err(Error::new(codes::INVALID));}
        if !self.acceleration[1].is_finite() {return Err(Error::new(codes::INVALID));}
        if !self.acceleration[2].is_finite() {return Err(Error::new(codes::INVALID));}
        Ok(())
    }
    /// Encode into separate caller-owned bytes after complete validation.
    pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error> {
        if out.len()<45 {return Err(Error::new(codes::TOO_LARGE));}self.valid()?;
        out[0..4].copy_from_slice(&self.sequence.to_le_bytes());
        out[4]=u8::from(self.enabled);
        out[5..9].copy_from_slice(&self.temperature.to_le_bytes());
        out[9..17].copy_from_slice(&self.acceleration[0].to_le_bytes());
        out[17..25].copy_from_slice(&self.acceleration[1].to_le_bytes());
        out[25..33].copy_from_slice(&self.acceleration[2].to_le_bytes());
        out[33..34].copy_from_slice(&self.tag[0].to_le_bytes());
        out[34..35].copy_from_slice(&self.tag[1].to_le_bytes());
        out[35..36].copy_from_slice(&self.tag[2].to_le_bytes());
        out[36..37].copy_from_slice(&self.tag[3].to_le_bytes());
        out[37..38].copy_from_slice(&self.tag[4].to_le_bytes());
        out[38..39].copy_from_slice(&self.tag[5].to_le_bytes());
        out[39..40].copy_from_slice(&self.tag[6].to_le_bytes());
        out[40..41].copy_from_slice(&self.tag[7].to_le_bytes());
        out[41..43].copy_from_slice(&self.thresholds[0].to_le_bytes());
        out[43..45].copy_from_slice(&self.thresholds[1].to_le_bytes());
        Ok(45)
    }
}
impl Canonical for Ros2Example_interfaces_customFixedSample {
    /// Allocate exactly one fixed output after validating all fields.
    fn encode(&self)->Result<Vec<u8>,Error> {self.valid()?;let mut out=alloc::vec![0u8;45];self.encode_into(&mut out)?;Ok(out)}
    /// Decode exactly one fixed record; truncation and trailing bytes fail.
    fn decode(bytes:&[u8])->Result<Self,Error> {if bytes.len()!=45 {return Err(Error::new(codes::INVALID));}let mut next=Self::default();
        next.sequence=u32::from_le_bytes(bytes[0..4].try_into().map_err(|_|Error::new(codes::INVALID))?);
        if bytes[4]>1 {return Err(Error::new(codes::INVALID));}next.enabled=bytes[4]!=0;
        next.temperature=f32::from_le_bytes(bytes[5..9].try_into().map_err(|_|Error::new(codes::INVALID))?);
        if !next.temperature.is_finite() {return Err(Error::new(codes::INVALID));}
        next.acceleration[0]=f64::from_le_bytes(bytes[9..17].try_into().map_err(|_|Error::new(codes::INVALID))?);
        if !next.acceleration[0].is_finite() {return Err(Error::new(codes::INVALID));}
        next.acceleration[1]=f64::from_le_bytes(bytes[17..25].try_into().map_err(|_|Error::new(codes::INVALID))?);
        if !next.acceleration[1].is_finite() {return Err(Error::new(codes::INVALID));}
        next.acceleration[2]=f64::from_le_bytes(bytes[25..33].try_into().map_err(|_|Error::new(codes::INVALID))?);
        if !next.acceleration[2].is_finite() {return Err(Error::new(codes::INVALID));}
        next.tag[0]=u8::from_le_bytes(bytes[33..34].try_into().map_err(|_|Error::new(codes::INVALID))?);
        next.tag[1]=u8::from_le_bytes(bytes[34..35].try_into().map_err(|_|Error::new(codes::INVALID))?);
        next.tag[2]=u8::from_le_bytes(bytes[35..36].try_into().map_err(|_|Error::new(codes::INVALID))?);
        next.tag[3]=u8::from_le_bytes(bytes[36..37].try_into().map_err(|_|Error::new(codes::INVALID))?);
        next.tag[4]=u8::from_le_bytes(bytes[37..38].try_into().map_err(|_|Error::new(codes::INVALID))?);
        next.tag[5]=u8::from_le_bytes(bytes[38..39].try_into().map_err(|_|Error::new(codes::INVALID))?);
        next.tag[6]=u8::from_le_bytes(bytes[39..40].try_into().map_err(|_|Error::new(codes::INVALID))?);
        next.tag[7]=u8::from_le_bytes(bytes[40..41].try_into().map_err(|_|Error::new(codes::INVALID))?);
        next.thresholds[0]=i16::from_le_bytes(bytes[41..43].try_into().map_err(|_|Error::new(codes::INVALID))?);
        next.thresholds[1]=i16::from_le_bytes(bytes[43..45].try_into().map_err(|_|Error::new(codes::INVALID))?);
        Ok(next)
    }
}
