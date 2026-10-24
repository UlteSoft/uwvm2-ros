#pragma once
/** @file Complete deterministic official legacy ROS1 transport fixture values.
 *
 * These are actual generated unitree_legged_msgs types from the eleven-message
 * unitree_ros_to_real/b989870124913091fbe75e0dcfb047eb4ca00a09 profile.
 * No ROS2 GO/HG layout, wire struct, vendor header, MD5 or native padding is
 * copied here. Every LowCmd/LowState field is assigned deliberately, including
 * every nested Motor/IMU/BMS/Cartesian field and fixed-array element.
 *
 * The two crc members are selected payload values. Equality proves their real
 * transport alongside every other field. It is NOT firmware CRC validation,
 * hardware packet authentication, actuator control or a CRC-layout claim.
 */
#include <unitree_legged_msgs/LowCmd.h>
#include <unitree_legged_msgs/LowState.h>
#include <cstdint>

namespace uwvm_legacy_ros1_fixture {
inline constexpr std::uint32_t state_crc_field=UINT32_C(0x13572468);
inline constexpr std::uint32_t command_crc_field=UINT32_C(0x89abcdef);
inline unitree_legged_msgs::LowCmd command() {
    unitree_legged_msgs::LowCmd v{};
    v.head[0]=0xfe;v.head[1]=0xef;v.levelFlag=0xff;v.frameReserve=7;
    v.SN[0]=0x10203040;v.SN[1]=0x50607080;v.version[0]=0x11223344;v.version[1]=0x55667788;v.bandWidth=321;
    for(unsigned i=0;i<20;++i) {
        auto& m=v.motorCmd[i];m.mode=static_cast<std::uint8_t>(1+i%4);
        m.q=static_cast<float>(i+1)/32;m.dq=-static_cast<float>(i+1)/64;
        m.tau=static_cast<float>(i+1)/128;m.Kp=static_cast<float>(i+1)/8;m.Kd=static_cast<float>(i+1)/256;
        for(unsigned j=0;j<3;++j)m.reserve[j]=0x10000000u+i*16u+j+1u;
    }
    v.bms.off=0;for(unsigned i=0;i<3;++i)v.bms.reserve[i]=static_cast<std::uint8_t>(0x31+i);
    for(unsigned i=0;i<40;++i)v.wirelessRemote[i]=static_cast<std::uint8_t>(17+13*i);
    v.reserve=0x24681357;v.crc=command_crc_field;return v;
}
inline unitree_legged_msgs::LowState state() {
    unitree_legged_msgs::LowState v{};
    v.head[0]=0xfe;v.head[1]=0xef;v.levelFlag=0xff;v.frameReserve=9;
    v.SN[0]=0x20304050;v.SN[1]=0x60708090;v.version[0]=0x22334455;v.version[1]=0x66778899;v.bandWidth=654;
    for(unsigned i=0;i<4;++i)v.imu.quaternion[i]=static_cast<float>(i+1)/8;
    for(unsigned i=0;i<3;++i) {
        v.imu.gyroscope[i]=-static_cast<float>(i+1)/16;v.imu.accelerometer[i]=static_cast<float>(i+1)/32;
        v.imu.rpy[i]=-static_cast<float>(i+1)/64;
    }
    v.imu.temperature=-9;
    for(unsigned i=0;i<20;++i) {
        auto& m=v.motorState[i];m.mode=static_cast<std::uint8_t>(1+i%4);
        m.q=static_cast<float>(i+1)/32;m.dq=-static_cast<float>(i+1)/64;m.ddq=static_cast<float>(i+1)/128;
        m.tauEst=static_cast<float>(i+1)/256;m.q_raw=-static_cast<float>(i+1)/512;
        m.dq_raw=static_cast<float>(i+1)/1024;m.ddq_raw=-static_cast<float>(i+1)/2048;
        m.temperature=static_cast<std::int8_t>(static_cast<int>(i)-10);
        for(unsigned j=0;j<2;++j)m.reserve[j]=0x20000000u+i*16u+j+1u;
    }
    auto& b=v.bms;b.version_h=3;b.version_l=7;b.bms_status=11;b.SOC=75;b.current=-1234;b.cycle=456;
    b.BQ_NTC[0]=-2;b.BQ_NTC[1]=3;b.MCU_NTC[0]=-4;b.MCU_NTC[1]=5;
    for(unsigned i=0;i<10;++i)b.cell_vol[i]=static_cast<std::uint16_t>(3100+7*i);
    for(unsigned i=0;i<4;++i) {
        v.footForce[i]=static_cast<std::int16_t>((i&1)?18+static_cast<int>(i):-17-static_cast<int>(i));
        v.footForceEst[i]=static_cast<std::int16_t>((i&1)?-34-static_cast<int>(i):33+static_cast<int>(i));
        auto& raw=v.eeForceRaw[i];raw.x=static_cast<float>(i+1)/8;raw.y=-static_cast<float>(i+1)/16;raw.z=static_cast<float>(i+1)/32;
        auto& force=v.eeForce[i];force.x=-static_cast<float>(i+1)/64;force.y=static_cast<float>(i+1)/128;force.z=-static_cast<float>(i+1)/256;
    }
    v.tick=42;for(unsigned i=0;i<40;++i)v.wirelessRemote[i]=static_cast<std::uint8_t>(29+11*i);
    v.reserve=0x35792468;v.crc=state_crc_field;
    v.position.x=-0.25f;v.position.y=0.5f;v.position.z=0.75f;
    v.velocity.x=0.125f;v.velocity.y=-0.0625f;v.velocity.z=0.03125f;
    v.velocity_w.x=-0.015625f;v.velocity_w.y=0.0078125f;v.velocity_w.z=-0.00390625f;
    return v;
}
}
