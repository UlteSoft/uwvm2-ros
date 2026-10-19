/** Exercise the production complete-vendor topic binding before any entity.
 * This is a real selected-SDK Native contract. It is not network delivery. */
#include "gateways/ros2/custom_topics.h"
#include <cassert>
#include <array>
#include <vector>
/** A deliberately changed expectation, never an exported native identity. */
template<class Codec> struct wrong_identity : Codec
{
    inline static std::array<char,72> const changed=[] {
        std::array<char,72> result{};
        for(unsigned i{};i!=71;++i){assert(Codec::native_identity[i]);result[i]=Codec::native_identity[i];}
        result[70]=result[70]=='0'?'1':'0';return result;
    }();
    // Array storage has a stable address throughout static initialization.
    inline static char const* native_identity=changed.data();
};
template<class Native,class Codec> void check_conversion()
{
    Native value{},restored{};std::vector<std::uint8_t> encoded,again;
    assert(Codec::valid(value));assert(Codec::encode(value,encoded));
    assert(Codec::decode(encoded.data(),encoded.size(),restored));
    assert(restored==value);assert(Codec::encode(restored,again));assert(again==encoded);
}
int main()
{
    unsigned count{};
#ifdef UWVM_EXPECT_SOURCE_REFUSAL
    constexpr bool expected=false;
#else
    constexpr bool expected=true;
#endif
#ifdef UWVM_EXPECT_SOURCE_REFUSAL
#define UWVM_CHECK_CONVERSION(Native,Codec)
#else
#define UWVM_CHECK_CONVERSION(Native,Codec) check_conversion<Native,Codec>();
#endif
#define UWVM_CHECK_BINDING(Native,Name,Identity,CodecIdentity,Bytes,Codec) \
    assert((::uwvm2::ros_gateway::ros2_custom_topics::binding<Native,Codec>::native_contract()==expected)); \
    assert((!::uwvm2::ros_gateway::ros2_custom_topics::binding<Native,wrong_identity<Codec>>::native_contract())); \
    UWVM_CHECK_CONVERSION(Native,Codec) ++count;
    UWVM_ROS_CUSTOM_ROS2_TYPES(UWVM_CHECK_BINDING)
#undef UWVM_CHECK_BINDING
#undef UWVM_CHECK_CONVERSION
    assert(count!=0);
}
