/** @file Second TU for real codec-source and selected-profile linkage. */
#include <platform/ros/cpp/roswasm/library_profile.hpp>
extern "C" uint32_t profile_peer(){unsigned char bytes[8];size_t used=0;
    return roswasmcpp::library_encode_string("peer",4,bytes,sizeof(bytes),used)||used!=8?0:roswasmcpp::library_profile();}
