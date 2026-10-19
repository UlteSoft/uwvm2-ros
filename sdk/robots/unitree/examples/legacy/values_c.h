#pragma once
/** Full typed ROS1 legacy Guest values, not native layouts or hardware CRCs.
 * Stage the ENTIRE newly Noetic-bound generated bounded_records.h beside it.
 * Every actual field is set; C and C++ share only these test data equations.
 */
#include "bounded_records.h"
typedef roswasmc_bounded_1_unitree_legged_msgs_LowCmd LegacyCmd;
typedef roswasmc_bounded_1_unitree_legged_msgs_LowState LegacyState;

static const uint32_t state_crc_field=UINT32_C(0x13572468);
static const uint32_t command_crc_field=UINT32_C(0x89abcdef);
static inline LegacyCmd legacy_expected_command(void) {
    LegacyCmd v={0};
    v.head[0]=0xfe;v.head[1]=0xef;v.levelFlag=0xff;v.frameReserve=7;
    v.SN[0]=0x10203040;v.SN[1]=0x50607080;v.version[0]=0x11223344;v.version[1]=0x55667788;v.bandWidth=321;
    for(unsigned i=0;i<20;++i) {
        roswasmc_bounded_1_unitree_legged_msgs_MotorCmd* m=&v.motorCmd[i];m->mode=((uint8_t)(1+i%4));
        m->q=((float)(i+1))/32;m->dq=-((float)(i+1))/64;
        m->tau=((float)(i+1))/128;m->Kp=((float)(i+1))/8;m->Kd=((float)(i+1))/256;
        for(unsigned j=0;j<3;++j)m->reserve[j]=0x10000000u+i*16u+j+1u;
    }
    v.bms.off=0;for(unsigned i=0;i<3;++i)v.bms.reserve[i]=((uint8_t)(0x31+i));
    for(unsigned i=0;i<40;++i)v.wirelessRemote[i]=((uint8_t)(17+13*i));
    v.reserve=0x24681357;v.crc=command_crc_field;return v;
}
static inline LegacyState legacy_expected_state(void) {
    LegacyState v={0};
    v.head[0]=0xfe;v.head[1]=0xef;v.levelFlag=0xff;v.frameReserve=9;
    v.SN[0]=0x20304050;v.SN[1]=0x60708090;v.version[0]=0x22334455;v.version[1]=0x66778899;v.bandWidth=654;
    for(unsigned i=0;i<4;++i)v.imu.quaternion[i]=((float)(i+1))/8;
    for(unsigned i=0;i<3;++i) {
        v.imu.gyroscope[i]=-((float)(i+1))/16;v.imu.accelerometer[i]=((float)(i+1))/32;
        v.imu.rpy[i]=-((float)(i+1))/64;
    }
    v.imu.temperature=-9;
    for(unsigned i=0;i<20;++i) {
        roswasmc_bounded_1_unitree_legged_msgs_MotorState* m=&v.motorState[i];m->mode=((uint8_t)(1+i%4));
        m->q=((float)(i+1))/32;m->dq=-((float)(i+1))/64;m->ddq=((float)(i+1))/128;
        m->tauEst=((float)(i+1))/256;m->q_raw=-((float)(i+1))/512;
        m->dq_raw=((float)(i+1))/1024;m->ddq_raw=-((float)(i+1))/2048;
        m->temperature=((int8_t)(((int)(i))-10));
        for(unsigned j=0;j<2;++j)m->reserve[j]=0x20000000u+i*16u+j+1u;
    }
    roswasmc_bounded_1_unitree_legged_msgs_BmsState* b=&v.bms;b->version_h=3;b->version_l=7;b->bms_status=11;b->SOC=75;b->current=-1234;b->cycle=456;
    b->BQ_NTC[0]=-2;b->BQ_NTC[1]=3;b->MCU_NTC[0]=-4;b->MCU_NTC[1]=5;
    for(unsigned i=0;i<10;++i)b->cell_vol[i]=((uint16_t)(3100+7*i));
    for(unsigned i=0;i<4;++i) {
        v.footForce[i]=((int16_t)((i&1)?18+((int)(i)):-17-((int)(i))));
        v.footForceEst[i]=((int16_t)((i&1)?-34-((int)(i)):33+((int)(i))));
        roswasmc_bounded_1_unitree_legged_msgs_Cartesian* raw=&v.eeForceRaw[i];raw->x=((float)(i+1))/8;raw->y=-((float)(i+1))/16;raw->z=((float)(i+1))/32;
        roswasmc_bounded_1_unitree_legged_msgs_Cartesian* force=&v.eeForce[i];force->x=-((float)(i+1))/64;force->y=((float)(i+1))/128;force->z=-((float)(i+1))/256;
    }
    v.tick=42;for(unsigned i=0;i<40;++i)v.wirelessRemote[i]=((uint8_t)(29+11*i));
    v.reserve=0x35792468;v.crc=state_crc_field;
    v.position.x=-0.25f;v.position.y=0.5f;v.position.z=0.75f;
    v.velocity.x=0.125f;v.velocity.y=-0.0625f;v.velocity.z=0.03125f;
    v.velocity_w.x=-0.015625f;v.velocity_w.y=0.0078125f;v.velocity_w.z=-0.00390625f;
    return v;
}

