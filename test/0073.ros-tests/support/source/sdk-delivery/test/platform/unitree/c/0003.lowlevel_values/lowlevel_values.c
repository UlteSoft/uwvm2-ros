/** @file Complete GO/HG mathematical packet fixtures for language comparison.
 * Every carrier slot is populated, with named reserves explicitly zero. These
 * values are synthetic representation tests, never safe robot commands.
 * Uses the actual portable C packer. This is not an upstream native ROS oracle.
 * Build with UWVM_UNITREE_VALUES_GO=1 or 0; stdout contains four u32LE-framed
 * complete padded packets. No ROS import, endpoint or authority is requested.
 */
#include <platform/unitree/c/roswasm_unitree/crc.h>
#include <stdio.h>
#ifndef UWVM_UNITREE_VALUES_GO
#error "Choose GO=1 or HG=0 explicitly"
#endif
int main(void) {
    static unsigned const seeds[4]={0,1,17,243};
    unsigned case_index;
    for(case_index=0;case_index<4;++case_index){
        unsigned seed=seeds[case_index],i;
        uint8_t packet[UWVM_UNITREE_HG_LOWCMD_BYTES],extent[4];
        uint32_t crc=0;size_t length=0;
#if UWVM_UNITREE_VALUES_GO
        uwvm_unitree_go_lowcmd value={0};
        value.head[0]=254;value.head[1]=239;value.level_flag=(uint8_t)(seed+3);
        value.sn[0]=UINT32_C(0x01020304)+seed;value.sn[1]=UINT32_C(0xfedcba98)-seed;
        value.version[0]=UINT32_C(0x10203040)+seed;value.version[1]=UINT32_C(0xa1b2c3d4)-seed;
        value.bandwidth=(uint16_t)(0x1234+seed);value.bms_off=(uint8_t)(seed+7);
        for(i=0;i<40;++i)value.wireless_remote[i]=(uint8_t)(seed+3*i);
        for(i=0;i<12;++i)value.led[i]=(uint8_t)(seed+5*i);
        value.fan[0]=(uint8_t)(seed+11);value.fan[1]=(uint8_t)(seed+13);value.gpio=(uint8_t)(seed+19);
#else
        uwvm_unitree_hg_lowcmd value={0};
        value.mode_pr=(uint8_t)(seed+3);value.mode_machine=(uint8_t)(seed+7);
#endif
        for(i=0;i<sizeof value.motor_cmd/sizeof value.motor_cmd[0];++i){
            value.motor_cmd[i].mode=(uint8_t)(seed+i);
            value.motor_cmd[i].q=((float)seed-(float)i)/8.0f;
            value.motor_cmd[i].dq=(3.0f*(float)i-(float)seed)/16.0f;
            value.motor_cmd[i].tau=((float)i+(float)seed)/32.0f;
            value.motor_cmd[i].kp=((float)seed+2.0f*(float)i)/64.0f;
            value.motor_cmd[i].kd=((float)seed-5.0f*(float)i)/128.0f;
        }
#if UWVM_UNITREE_VALUES_GO
        if(uwvm_unitree_go_lowcmd_pack_crc(&value,packet,sizeof packet,&length,&crc))return 1;
#else
        if(uwvm_unitree_hg_lowcmd_pack_crc(&value,packet,sizeof packet,&length,&crc))return 1;
#endif
        uwvm_unitree_put32(extent,(uint32_t)length);
        if(fwrite(extent,1,4,stdout)!=4||fwrite(packet,1,length,stdout)!=length)return 5;
    }
    return fflush(stdout)?5:0;
}
