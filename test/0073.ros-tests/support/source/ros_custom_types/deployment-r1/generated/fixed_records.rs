//! Generated fixed topic values and checked codecs; no I/O or authority.
//! Add this file as a module to a guest using roswasm with its alloc feature.
extern crate alloc;
use alloc::vec::Vec;
use roswasm::{Error,codes};
use roswasm::typed::Canonical;
/// Fixed ROS1 uwvm_custom_ros1_fixture/Packet value. All arrays have exact lengths.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros1Uwvm_custom_ros1_fixturePacket {
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
impl Default for Ros1Uwvm_custom_ros1_fixturePacket {fn default()->Self {Self {
sequence:Default::default(),
active:Default::default(),
measurement:Default::default(),
tag:core::array::from_fn(|_|Default::default()),
samples:core::array::from_fn(|_|Default::default()),
}}}
impl Ros1Uwvm_custom_ros1_fixturePacket {
    /// Exact canonical byte count; no native serialization is implied.
    pub const WIRE_BYTES:usize=23;
    /// Compile-time canonical schema identity; the host independently verifies it.
    pub const CODEC_IDENTITY:&'static str="UWVMROS1_eb0eef4515ec546678bae4030eb858c151af98afa1bfa3ca08b3a0e02bd48544";
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
impl Canonical for Ros1Uwvm_custom_ros1_fixturePacket {
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
/// Fixed ROS2 uwvm_ros_custom_test_msgs/msg/FixedSample value. All arrays have exact lengths.
#[derive(Clone,Debug,PartialEq)]
pub struct Ros2Uwvm_ros_custom_test_msgsFixedSample {
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
impl Default for Ros2Uwvm_ros_custom_test_msgsFixedSample {fn default()->Self {Self {
sequence:Default::default(),
enabled:Default::default(),
temperature:Default::default(),
acceleration:core::array::from_fn(|_|Default::default()),
tag:core::array::from_fn(|_|Default::default()),
thresholds:core::array::from_fn(|_|Default::default()),
}}}
impl Ros2Uwvm_ros_custom_test_msgsFixedSample {
    /// Exact canonical byte count; no native serialization is implied.
    pub const WIRE_BYTES:usize=45;
    /// Compile-time canonical schema identity; the host independently verifies it.
    pub const CODEC_IDENTITY:&'static str="UWVMROS1_3f390efa2dccaf91a53c14a50fcead8790bb1bbc103c6ac6b29f176d045e2516";
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
impl Canonical for Ros2Uwvm_ros_custom_test_msgsFixedSample {
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
