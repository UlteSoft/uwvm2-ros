/** Source-authored adversarial contracts for the complete ROS1 definition
 * parser and explicit schema domain. No ROS node, SDK identity or robot is
 * simulated. Compile/run only in the admitted unified test stage. */
#include <uwvm2/imported/ros/ipc/complete_vendor_domain.h>
#include "../../../../../../src/ros/gateway/ros1/complete_vendor_definition.h"
#include <string_view>

namespace ipc=uwvm2::imported::ros::ipc;
namespace complete=uwvm2::ros_gateway::ros1_complete_vendor;
using std::string_view;
struct NativeSentinel {};
struct OrdinaryCodec {};
struct CompleteCodec {static constexpr bool complete_vendor_profile=true;};
static_assert(!ipc::complete_vendor_native_traits<NativeSentinel>::enabled);
static_assert(!ipc::complete_vendor_codec<OrdinaryCodec>::value);
static_assert(ipc::complete_vendor_codec<CompleteCodec>::value);
static_assert(ipc::complete_vendor_native_name(1,"vendor_msgs/State"));
static_assert(ipc::complete_vendor_native_name(2,"control_msgs/action/FollowJointTrajectory_GetResult_Response"));
static_assert(ipc::complete_vendor_endpoint_name(2,"control_msgs/action/FollowJointTrajectory","action"));
static_assert(!ipc::complete_vendor_endpoint_name(2,"std_msgs/msg/String","msg"));
static_assert(!ipc::complete_vendor_endpoint_name(2,"std_srvs/srv/Trigger","srv"));
static_assert(!ipc::complete_vendor_endpoint_name(1,"std_srvs/Trigger","srv"));
static_assert(!ipc::complete_vendor_endpoint_name(2,"rcl_interfaces/srv/GetParameters","srv"));
static_assert(!ipc::complete_vendor_endpoint_name(2,"builtin_interfaces/msg/Time","msg"));
static_assert(!ipc::complete_vendor_native_name(2,"vendor/msg/../State"));
static_assert(!ipc::complete_vendor_native_name(2,"vendor/srv/State","msg"));
static_assert(!ipc::complete_vendor_native_name(2,"Vendor/msg/State"));

struct Entry {string_view type,fields;};
constexpr Entry expected[]={
    {"vendor_msgs/State","int8 MODE=1\nstring LABEL=\"state#one\"\nHeader header\nfloat64[2] positions\n"},
    {"std_msgs/Header","uint32 seq\ntime stamp\nstring frame_id\n"}
};
/** The Gateway supplies its existing real ROS1 field-type comparator. This
 * isolated test deliberately uses exact field text to exercise only the new
 * constant/dependency/comment state machine, without pretending to test ROS
 * generated traits or alias normalization through a substitute SDK. */
static bool exact_field(string_view a,string_view b,string_view) noexcept {return a==b;}
static bool matches(string_view body,Entry const* rows=expected,std::size_t count=2)
{return complete::matches("vendor_msgs/State",body,rows,count,exact_field);}
constexpr string_view original=
    "# original full definition\nint8 MODE = 1 # numeric constant\n"
    "string LABEL=\"state#one\" # comment after quoted value\n"
    "Header header\nfloat64[2] positions\n"
    "================================================================================\n"
    "MSG: std_msgs/Header\nuint32 seq\ntime stamp\nstring frame_id\n";
int main()
{
    if(!matches(original))return 1;
    // A cold generated C++ constant check is also mandatory in the real
    // binding. These cases prove that declaration lines cannot be omitted.
    if(matches("Header header\nfloat64[2] positions\n"))return 2;
    if(matches("int8 MODE=2\nstring LABEL=\"state#one\"\nHeader header\nfloat64[2] positions\n"))return 3;
    if(matches("int8 MODE=1\nstring LABEL=\"state#two\"\nHeader header\nfloat64[2] positions\n"))return 4;
    if(matches("int8 MODE=1\nstring LABEL=\"unterminated\nHeader header\nfloat64[2] positions\n"))return 5;
    if(matches("int8 MODE=1\nstring LABEL=\"state#one\"\nHeader header\nfloat64[1] positions\n"))return 6;
    if(matches("MSG: std_msgs/Header\nuint32 seq\n"))return 7;
    if(matches("===\nMSG: vendor_msgs/State\n"))return 8;
    if(matches("===\nMSG: other_msgs/Header\n"))return 9;
    if(matches("===\n===\nMSG: std_msgs/Header\n"))return 10;
    if(matches(original,nullptr,2)||matches(original,expected,0)||matches(original,expected,129))return 11;
    constexpr Entry duplicate[]={expected[0],expected[1],expected[1]};
    if(matches(original,duplicate,3))return 12;
    constexpr Entry malformed[]={{"vendor_msgs/State",expected[0].fields},{"../Header",expected[1].fields}};
    if(matches(original,malformed,2))return 13;
    // Exact order, all fields, no extra section or replay of a dependency.
    constexpr Entry shortened[]={{"vendor_msgs/State",expected[0].fields},{"std_msgs/Header","time stamp\nstring frame_id\n"}};
    if(matches(original,shortened,2))return 14;
    constexpr Entry reordered[]={{"vendor_msgs/State","int8 MODE=1\nHeader header\nstring LABEL=\"state#one\"\nfloat64[2] positions\n"},expected[1]};
    if(matches(original,reordered,2))return 15;
    return 0;
}
