/** Real installed-SDK, full-value codec oracle. This creates no ROS entity.
 * A marker is not delivery: actual compiler/MD/native ELF/link/maps/SDK/root
 * preservation and the independent publisher/Guest result remain mandatory.
 */
#include "tests/ros_custom_types/bounded-v2/native_protocol.h"
#include "fixture.hpp"
#include <limits>
namespace {
unsigned checks{},failures{};
void check(bool value,char const* reason){++checks;if(!value){++failures;bounded_native_io::print(STDERR_FILENO,"FAIL_XARM_CIO_STATE_NATIVE ",reason,'\n');}}
}
int main() {
    using namespace xarm_cio_state_fixture;
    check(actual_identities(),"actual installed CIOState/Header/Time native identities");
    auto value=sample();std::vector<std::uint8_t> wire;
    check(Codec::encode(value,wire),"complete native encode");
    Native decoded{};check(Codec::decode(wire.data(),wire.size(),decoded),"complete native decode");
    check(same(value,decoded),"all Header/state/code and all six fixed arrays");
    const auto original=wire;auto sentinel=value;sentinel.header.frame_id="unchanged";const auto before=sentinel;
    wire.push_back(0);check(!Codec::decode(wire.data(),wire.size(),sentinel),"trailing input refused");
    check(same(sentinel,before),"trailing refusal preserves complete destination");
    wire=original;if(!wire.empty())wire.pop_back();check(!Codec::decode(wire.data(),wire.size(),sentinel),"truncated final fixed-array element refused");
    check(same(sentinel,before),"truncation preserves destination");
    auto bad=value;auto unchanged=original;
    bad=value;bad.header.frame_id.assign(129,'x');std::size_t extent=777;
    check(!Codec::encoded_size(bad,extent)&&extent==777,"full Header frame_id bound128 with transactional extent");
    bad=value;bad.input_analogs[0]=std::numeric_limits<float>::quiet_NaN();
    check(!Codec::encode(bad,unchanged)&&unchanged==original,"nonfinite actual float32 refused without output change");
#if UWVM_CUSTOM_NATIVE_ROS_VERSION == 1
    bad=value;bad.header.stamp.nsec=1000000000;
#else
    bad=value;bad.header.stamp.nanosec=1000000000;
#endif
    check(!Codec::encode(bad,unchanged)&&unchanged==original,"normalized Header time required");
    // Every fixed native element stays present. Preserve full unsigned bit
    // patterns, rather than projecting GPIO words/bytes into a few booleans.
    auto boundary=value;boundary.input_digitals[1]=65535;boundary.output_conf[15]=255;wire.clear();
    check(Codec::encode(boundary,wire)&&Codec::decode(wire.data(),wire.size(),decoded)&&same(boundary,decoded),"full uint16/uint8 fixed-array boundaries preserved");
    bounded_native_io::print(STDOUT_FILENO,"PASS_XARM_CIO_STATE_NATIVE_CODEC checks=",checks," failures=",failures,'\n');
    return failures?1:0;
}
