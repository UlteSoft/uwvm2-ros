/* SPDX-License-Identifier: Apache-2.0
 * Same actual operator input/one-alias protocol as guest_state.c. */
#include <wasi/api.h>
#include <roswasm/ros2.h>
#include "../../sdk.h"
#include "document.hpp"
#include <cstring>
static uwvm::spot::protobuf::RobotState state;
static mm_spot_buffers buffers;
static mm_spot_State_response response;
static struct {std::uint32_t primary,transport,provider,unknown,decode,cleanup;} report;
struct OwnerGuard {
    roswasmc_capability *owner;
    void close() noexcept {if(owner->handle)report.cleanup=roswasmc_close(owner);}
    ~OwnerGuard(){close();}
};
static bool exact(unsigned char *p,std::size_t n) {
    while(n) {
        __wasi_iovec_t iov{p,n};__wasi_size_t got{};
        if(__wasi_fd_read(0,&iov,1,&got)||!got||got>n)return false;
        p+=got;n-=got;
    }
    return true;
}
int main(int argc,char **argv) {
    unsigned char input[24];mm_spot_State_request request{};
    roswasmc_context context{};roswasmc_capability source{};
    if(argc!=2||!argv[1]||!argv[1][0]||std::strlen(argv[1])>255||!exact(input,sizeof input))return 22;
    request.token=uwvm_ros_load_u64(input);request.nonce=uwvm_ros_load_u64(input+8);
    request.budget_ns=uwvm_ros_load_u64(input+16);
    if(!request.token||!request.nonce||!request.budget_ns||request.budget_ns>5000000000ull)return 22;
    context.backend=roswasmc_ros2_backend();roswasmc_capability_init(&source,&context);
    OwnerGuard guard{&source};
    report.primary=roswasmc_open(&source,argv[1],std::strlen(argv[1]));
    try {
    if(!report.primary) {
        auto result=mm_spot_State_call(&source,&request,&response,&buffers);
        report.transport=result.error;
        if(result.error)report.primary=result.error;
        else {
            report.provider=response.error;report.unknown=response.unknown;
            report.primary=response.error?response.error:(response.unknown?115:0);
            if(!report.primary) {
                report.decode=state.decode(response.native_proto.data,response.native_proto.size);
                report.primary=report.decode;
                // The complete true bosdyn::api::RobotState is retained, not a
                // byte facade or a limited summary. No second request occurs.
            }
        }
    }
    } catch(...) {if(!report.primary)report.primary=5;}
    guard.close();
    if(!report.primary)report.primary=report.cleanup;
    return report.primary?static_cast<int>(report.primary<126?report.primary:125):0;
}
