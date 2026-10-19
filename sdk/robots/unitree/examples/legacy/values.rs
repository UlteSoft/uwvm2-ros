//! Complete generated legacy Guest data; no ROS2/native packet-layout reuse.
//! Parent imports this module beside the entire freshly Noetic-bound vendor.
//! All fractions are finite/exact binary32. crc constants are payload fields.
use crate::vendor::{Ros1Unitree_legged_msgsLowCmd as Command,Ros1Unitree_legged_msgsLowState as State};
pub fn command()->Command {
    let mut v=Command::default();v.head=[0xfe,0xef];v.levelFlag=0xff;v.frameReserve=7;
    v.SN=[0x10203040,0x50607080];v.version=[0x11223344,0x55667788];v.bandWidth=321;
    for i in 0..20 {let m=&mut v.motorCmd[i];m.mode=(1+i%4) as u8;
        m.q=(i+1) as f32/32.0;m.dq=-((i+1) as f32)/64.0;m.tau=(i+1) as f32/128.0;
        m.Kp=(i+1) as f32/8.0;m.Kd=(i+1) as f32/256.0;
        for j in 0..3 {m.reserve[j]=0x10000000+(i*16+j+1) as u32;}}
    v.bms.off=0;v.bms.reserve=[0x31,0x32,0x33];
    for i in 0..40 {v.wirelessRemote[i]=(17+13*i) as u8;}
    v.reserve=0x24681357;v.crc=0x89abcdef;v
}
pub fn state()->State {
    let mut v=State::default();v.head=[0xfe,0xef];v.levelFlag=0xff;v.frameReserve=9;
    v.SN=[0x20304050,0x60708090];v.version=[0x22334455,0x66778899];v.bandWidth=654;
    for i in 0..4 {v.imu.quaternion[i]=(i+1) as f32/8.0;}
    for i in 0..3 {v.imu.gyroscope[i]=-((i+1) as f32)/16.0;v.imu.accelerometer[i]=(i+1) as f32/32.0;v.imu.rpy[i]=-((i+1) as f32)/64.0;}
    v.imu.temperature=-9;
    for i in 0..20 {let m=&mut v.motorState[i];m.mode=(1+i%4) as u8;
        m.q=(i+1) as f32/32.0;m.dq=-((i+1) as f32)/64.0;m.ddq=(i+1) as f32/128.0;m.tauEst=(i+1) as f32/256.0;
        m.q_raw=-((i+1) as f32)/512.0;m.dq_raw=(i+1) as f32/1024.0;m.ddq_raw=-((i+1) as f32)/2048.0;
        m.temperature=i as i8-10;for j in 0..2 {m.reserve[j]=0x20000000+(i*16+j+1) as u32;}}
    let b=&mut v.bms;b.version_h=3;b.version_l=7;b.bms_status=11;b.SOC=75;b.current=-1234;b.cycle=456;
    b.BQ_NTC=[-2,3];b.MCU_NTC=[-4,5];for i in 0..10 {b.cell_vol[i]=(3100+7*i) as u16;}
    for i in 0..4 {v.footForce[i]=if i&1!=0 {18+i as i16} else {-17-i as i16};
        v.footForceEst[i]=if i&1!=0 {-34-i as i16} else {33+i as i16};
        let raw=&mut v.eeForceRaw[i];raw.x=(i+1) as f32/8.0;raw.y=-((i+1) as f32)/16.0;raw.z=(i+1) as f32/32.0;
        let force=&mut v.eeForce[i];force.x=-((i+1) as f32)/64.0;force.y=(i+1) as f32/128.0;force.z=-((i+1) as f32)/256.0;}
    v.tick=42;for i in 0..40 {v.wirelessRemote[i]=(29+11*i) as u8;}
    v.reserve=0x35792468;v.crc=0x13572468;
    v.position.x=-0.25;v.position.y=0.5;v.position.z=0.75;
    v.velocity.x=0.125;v.velocity.y=-0.0625;v.velocity.z=0.03125;
    v.velocity_w.x=-0.015625;v.velocity_w.y=0.0078125;v.velocity_w.z=-0.00390625;v
}
