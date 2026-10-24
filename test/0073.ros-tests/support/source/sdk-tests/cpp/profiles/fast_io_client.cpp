/** @file Old-standard caller of the separately compiled C++20 stdout component. */
#include <platform/ros/cpp/roswasm/fast_io_bridge.hpp>
#include <platform/ros/cpp/roswasm/library_profile.hpp>
#include <cassert>
int main(){
    assert(roswasmcpp::library_profile()==ROSWASM_CPP_STANDARD);
    assert(roswasm_fast_io_stdout_v1(0,1)==22);
    assert(roswasm_fast_io_stdout_v1(0,0)==0);
    static char const text[]="CPP_C_ABI_FAST_IO\n";
    assert(roswasm_fast_io_stdout_v1(text,sizeof(text)-1)==0);return 0;
}
