/** Whole C codec contract over an actually generated complete source header.
 * Build only in the unified admitted test stage. Generate the adjacent synthetic
 * byte-sequence catalog with the exact reviewed adapter; select that one actual
 * generated include directory and sdk/include. No fake/native ROS types, Host
 * stub, network, robot or permission grant is used. These contracts are authored
 * but have not been compiled or run as part of Source implementation.
 */
#ifdef NDEBUG
#error "Whole-value byte codec checks must remain enabled"
#endif
#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include "bounded_records.h"
using Value=roswasmc_bounded_2_test_industrial_msg_BytePayload;
int main(){
    Value original{};original.payload.size=16384;original.sequence=0xfedcba98u;
    for(std::size_t i=0;i<16384;++i)original.payload.data[i]=static_cast<unsigned char>(i*131u+17u);
    std::array<unsigned char,16392> wire{};std::size_t used=987;
    assert(!roswasmc_bounded_2_test_industrial_msg_BytePayload_encode(&original,wire.data(),wire.size(),&used));
    assert(used==wire.size());Value decoded{};
    assert(!roswasmc_bounded_2_test_industrial_msg_BytePayload_decode(wire.data(),used,&decoded));
    assert(decoded.payload.size==16384&&decoded.sequence==original.sequence);
    for(std::size_t i=0;i<16384;++i)assert(decoded.payload.data[i]==original.payload.data[i]);
    auto sentinel=decoded;
    // Failure at the final trailing scalar must preserve the entire destination.
    assert(roswasmc_bounded_2_test_industrial_msg_BytePayload_decode(wire.data(),used-1,&decoded));
    assert(std::memcmp(&decoded,&sentinel,sizeof decoded)==0);
    wire[0]=1;wire[1]=64;wire[2]=wire[3]=0; //16385 elements cannot fit this schema.
    assert(roswasmc_bounded_2_test_industrial_msg_BytePayload_decode(wire.data(),used,&decoded));
    assert(std::memcmp(&decoded,&sentinel,sizeof decoded)==0);
    auto invalid=original;invalid.payload.size=16385;used=987;wire.fill(0xa5);
    assert(roswasmc_bounded_2_test_industrial_msg_BytePayload_encode(&invalid,wire.data(),wire.size(),&used));
    assert(used==987);for(auto byte:wire)assert(byte==0xa5);
}
