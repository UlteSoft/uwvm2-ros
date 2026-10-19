/* Real optional-Wasm entry. This fixture opens ONLY two Host-configured aliases.
 * Root selects --deny or --query EXPECTED_0_OR_1 in a separately admitted run;
 * no guessed handle, ROS identity, server state or successful result is baked.
 * It never sends a Service request or changes Guest/Host management authority. */
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <roswasm/service_client_v1.h>
#if UWVM_ROS_BACKEND == 1
#include <roswasm/abi/ros1.h>
#define OPEN uwvm_ros1_capability_open
#define CLOSE uwvm_ros1_capability_close
#define FEATURES uwvm_ros1_client_v1_features
#define QUERY uwvm_ros1_client_v1_service_available
#define VERSION uwvm_ros1_client_v1_abi_version
#elif UWVM_ROS_BACKEND == 2
#include <roswasm/abi/ros2.h>
#define OPEN uwvm_ros2_capability_open
#define CLOSE uwvm_ros2_capability_close
#define FEATURES uwvm_ros2_client_v1_features
#define QUERY uwvm_ros2_client_v1_service_available
#define VERSION uwvm_ros2_client_v1_abi_version
#else
#error "Select actual backend1 or2"
#endif
static uint64_t address(void const* p){return (uint64_t)(uintptr_t)p;}
int main(int argc,char** argv)
{
    if(argc<2||VERSION()!=1)return 2;
    uint64_t available=0,source=0;uint8_t ready=0xa5;
    char const a[]="available",s[]="call";
    uint32_t error=OPEN(0,address(a),sizeof(a)-1,address(&available));
    if(!strcmp(argv[1],"--deny"))
    {
        if(error!=13||available!=0)return 3;
        error=QUERY(0,0,0,0,address(&ready));
        return (error==9||error==13)&&ready==0xa5?0:3;
    }
    if(!strcmp(argv[1],"--source-only"))
    {
        if(error!=13||available!=0)return 3;
        if(OPEN(0,address(s),sizeof(s)-1,address(&source)))return 3;
        error=QUERY(source,source,0,0,address(&ready));
        if(error!=13||ready!=0xa5){CLOSE(source);return 3;}
        return CLOSE(source)?3:0;
    }
    if(strcmp(argv[1],"--query")||argc!=3||(strcmp(argv[2],"0")&&strcmp(argv[2],"1")))return 4;
    if(error)return 5;
    error=OPEN(0,address(s),sizeof(s)-1,address(&source));if(error){CLOSE(available);return 6;}
    /* Bounds are checked before native connection/client creation. */
    error=QUERY(available,source,0,UINT32_MAX,address(&ready));
    if(error!=22||ready!=0xa5){CLOSE(source);CLOSE(available);return 7;}
    error=QUERY(available,source,0,0,UINT64_MAX);
    if(error!=22||ready!=0xa5){CLOSE(source);CLOSE(available);return 7;}
    uint64_t features=0;error=FEATURES(available,source,0,address(&features));
    if(error||features!=(UWVM_ROS_BACKEND==1?11u:7u)){CLOSE(source);CLOSE(available);return 7;}
    /* Wrong operation cannot borrow live call authority; output is unchanged. */
    error=QUERY(source,available,0,0,address(&ready));
    if(error!=13||ready!=0xa5){CLOSE(source);CLOSE(available);return 8;}
    error=QUERY(available,source,0,0,address(&ready));
    if(error||ready!=(uint8_t)(argv[2][0]-'0')){CLOSE(source);CLOSE(available);return 9;}
    /* Actual source retirement precedes any native best-effort cleanup. */
    error=CLOSE(source);if(error){CLOSE(available);return 10;}ready=0xa5;
    error=QUERY(available,source,0,0,address(&ready));
    if((error!=9&&error!=125)||ready!=0xa5){CLOSE(available);return 11;}
    return CLOSE(available)?12:0;
}
