/** @file Library linkage check without sharing a standard-library object. */
#include <platform/ros/cpp/roswasm/library_profile.hpp>
#include <cassert>
extern "C" uint32_t profile_peer();
int main(){assert(profile_peer()==ROSWASM_CPP_STANDARD);assert(roswasmcpp::library_profile()==ROSWASM_CPP_STANDARD);return 0;}
