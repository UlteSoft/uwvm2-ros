/** Real installed-SDK, full-value codec oracle. This creates no ROS entity.
 * A marker is not delivery: actual compiler/MD/native ELF/link/maps/SDK/root
 * preservation and the independent publisher/Guest result remain mandatory.
 */
#include "tests/ros_custom_types/bounded-v2/native_protocol.h"
#include "fixture.hpp"
#include <limits>
namespace {
unsigned checks{},failures{};
void check(bool value,char const* reason){++checks;if(!value){++failures;bounded_native_io::print(STDERR_FILENO,"FAIL_XARM_ROBOT_MSG_NATIVE ",reason,'\n');}}
}
int main() {
    using namespace xarm_robot_msg_fixture;
    check(actual_identities(),"actual installed RobotMsg/Header/Time native identities");
    auto value=sample();std::vector<std::uint8_t> wire;
    check(Codec::encode(value,wire),"complete native encode");
    Native decoded{};check(Codec::decode(wire.data(),wire.size(),decoded),"complete native decode");
    check(same(value,decoded),"all Header/scalars/dynamic angles/fixed pose/offset values");
    const auto original=wire;auto sentinel=value;sentinel.header.frame_id="unchanged";const auto before=sentinel;
    wire.push_back(0);check(!Codec::decode(wire.data(),wire.size(),sentinel),"trailing input refused");
    check(same(sentinel,before),"trailing refusal preserves complete destination");
    wire=original;if(!wire.empty())wire.pop_back();check(!Codec::decode(wire.data(),wire.size(),sentinel),"truncated final fixed-array element refused");
    check(same(sentinel,before),"truncation preserves destination");
    auto bad=value;bad.angle.push_back(2);auto unchanged=original;
    check(!Codec::encode(bad,unchanged),"canonical angle bound7 refused before allocation");
    check(unchanged==original,"angle refusal preserves encoded output");
    bad=value;bad.header.frame_id.assign(129,'x');std::size_t extent=777;
    check(!Codec::encoded_size(bad,extent)&&extent==777,"full Header frame_id bound128 with transactional extent");
    bad=value;bad.pose[0]=std::numeric_limits<float>::quiet_NaN();
    check(!Codec::encode(bad,unchanged)&&unchanged==original,"nonfinite actual float32 refused without output change");
#if UWVM_CUSTOM_NATIVE_ROS_VERSION == 1
    bad=value;bad.header.stamp.nsec=1000000000;
#else
    bad=value;bad.header.stamp.nanosec=1000000000;
#endif
    check(!Codec::encode(bad,unchanged)&&unchanged==original,"normalized Header time required");
    // RobotMsg has no JointState equal-length semantic rule. An empty dynamic
    // angle sequence is valid schema data; fixed pose/offset remain complete.
    auto empty=value;empty.angle.clear();wire.clear();
    check(Codec::encode(empty,wire)&&Codec::decode(wire.data(),wire.size(),decoded)&&same(empty,decoded),"empty dynamic angle preserved without projecting fixed fields");
    bounded_native_io::print(STDOUT_FILENO,"PASS_XARM_ROBOT_MSG_NATIVE_CODEC checks=",checks," failures=",failures,'\n');
    return failures?1:0;
}
