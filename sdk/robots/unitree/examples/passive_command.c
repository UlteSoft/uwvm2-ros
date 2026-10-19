/** @file C11 real SDK topic publication with caller-owned inline codec storage.
 * Same default-denial/SIM-only contract as passive_command.cpp; the selected
 * generated complete family and native authority must match independently.
 * This is an executable main, not a native ROS object or fake Wasm import test.
 */
#include <roswasm/ros2.h>
#include <roswasm_unitree/crc.h>
#include "bounded_records.h"
#ifndef UWVM_UNITREE_SEND
#define UWVM_UNITREE_SEND 0
#endif
#if UWVM_UNITREE_FAMILY == 1
typedef roswasmc_bounded_2_unitree_go_msg_LowCmd Command;
#define ENCODE roswasmc_bounded_2_unitree_go_msg_LowCmd_encode
#define CAPACITY ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_LOWCMD_MAX_WIRE_BYTES
#elif UWVM_UNITREE_FAMILY == 2
typedef roswasmc_bounded_2_unitree_hg_msg_LowCmd Command;
#define ENCODE roswasmc_bounded_2_unitree_hg_msg_LowCmd_encode
#define CAPACITY ROSWASMC_BOUNDED_2_UNITREE_HG_MSG_LOWCMD_MAX_WIRE_BYTES
#else
#error "Explicit GO1 or HG2 family required"
#endif
int main(void) {
    roswasmc_context context={roswasmc_ros2_backend()};roswasmc_capability publisher;
    Command command={0};unsigned char canonical[CAPACITY];size_t length=0,packed_length=0;
    uint32_t crc=0,error,cleanup;
    if(uwvm_ros2_abi_version()!=1)return 95;
    roswasmc_capability_init(&publisher,&context);
    error=roswasmc_open(&publisher,"unitree-lowcmd",14);
    if(!UWVM_UNITREE_SEND){
        if(error==13&&!roswasmc_is_open(&publisher))return 0;
        if(roswasmc_is_open(&publisher)){cleanup=roswasmc_close(&publisher);(void)cleanup;}
        return 22;
    }
    if(error)return (int)error;
#if UWVM_UNITREE_FAMILY == 1
    {uwvm_unitree_go_lowcmd view={0};uint8_t packed[UWVM_UNITREE_GO_LOWCMD_BYTES];
    view.head[0]=command.head[0]=0xfe;view.head[1]=command.head[1]=0xef;view.level_flag=command.level_flag=0xff;
    error=uwvm_unitree_go_lowcmd_pack_crc(&view,packed,sizeof packed,&packed_length,&crc);}
#else
    {uwvm_unitree_hg_lowcmd view={0};uint8_t packed[UWVM_UNITREE_HG_LOWCMD_BYTES];
    error=uwvm_unitree_hg_lowcmd_pack_crc(&view,packed,sizeof packed,&packed_length,&crc);}
#endif
    if(!error){command.crc=crc;error=ENCODE(&command,canonical,sizeof canonical,&length);}
    if(!error)error=roswasmc_publish(&publisher,canonical,length);
    cleanup=roswasmc_close(&publisher);if(!error)error=cleanup;
    return (int)error;
}
