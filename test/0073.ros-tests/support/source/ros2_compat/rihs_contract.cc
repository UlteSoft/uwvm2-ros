/*************************************************************
 * UWVM2 ROS2 canonical identity regression. APL-2.0.          *
 *************************************************************/
#include <uwvm2/imported/ros/ipc/custom_type.h>
#include "../../../../../src/ros/gateway/ros2/compat/rihs.h"
#include <cassert>
#include <array>
#include <string_view>
#include <utility>
#if !UWVM_ROS2_HAVE_OPENSSL
#error "Select actual target OpenSSL Crypto for this identity regression; no stub success"
#endif

/** Test the production RIHS01 implementation against independently exported
 * Jazzy native identities. This executable performs no ROS operation and grants
 * no capability. Root builds/runs it serially in the authorized Linux cgroup;
 * cross targets require their own compiler, Crypto library, loader and QEMU.
 * Missing dependencies are unverified, never a successful native test.
 */
#ifdef NDEBUG
#error "Compile the native identity contract with assertions enabled (-UNDEBUG)"
#endif

int main()
{
    namespace ipc = uwvm2::imported::ros::ipc;
    using field = ipc::custom_native_field;
    using record = ipc::custom_native_record;
    using uwvm2::ros_gateway::ros2_compat::matches_rihs;
    constexpr field point[]{{"value","int32"},{"enabled","bool"}};
    record point_record[]{ {"uwvm_ros_bounded_v2/msg/Point",point,2} };
    constexpr std::string_view point_hash{"RIHS01_7f58e859cebde43421f1dc78d6f7c71effd3adad3bd3620f64c48405a2f1cc91"};
    assert(matches_rihs({point_record,1},point_record[0].type,point_hash));
    // The exported identity uses the same independent actual-native golden;
    // a failed digest derivation must preserve caller-owned destination bytes.
    using uwvm2::ros_gateway::ros2_compat::rihs_digest;
    using uwvm2::ros_gateway::ros2_compat::rihs_identity;
    assert(rihs_identity({point_record,1},point_record[0].type)==point_hash);
    std::array<unsigned char,32> digest{};digest.fill(0xa5);auto original_digest=digest;
    assert(!rihs_digest({},point_record[0].type,digest)&&digest==original_digest);
    assert(rihs_identity({},point_record[0].type).empty());
    assert(!rihs_digest({point_record,1},"uwvm_ros_bounded_v2/msg/Missing",digest)&&digest==original_digest);
    assert(rihs_digest({point_record,1},point_record[0].type,digest));
    assert(!matches_rihs({point_record,1},point_record[0].type,"RIHS01_0000000000000000000000000000000000000000000000000000000000000000"));
    assert(!matches_rihs({point_record,1},"uwvm_ros_bounded_v2/msg/Missing",point_hash));
    assert(!matches_rihs({},point_record[0].type,point_hash));
    assert(!matches_rihs({point_record,1},point_record[0].type,point_hash.substr(1)));
    constexpr field root[]{ {"request_message","uwvm_ros_bounded_v2/srv/Empty_Request"},
        {"response_message","uwvm_ros_bounded_v2/srv/Empty_Response"},{"event_message","uwvm_ros_bounded_v2/srv/Empty_Event"} };
    constexpr field empty[]{{"structure_needs_at_least_one_member","uint8"}};
    constexpr field event[]{{"info","service_msgs/msg/ServiceEventInfo"},
        {"request","uwvm_ros_bounded_v2/srv/Empty_Request",0,false,0,true,1},
        {"response","uwvm_ros_bounded_v2/srv/Empty_Response",0,false,0,true,1}};
    constexpr field info[]{{"event_type","uint8"},{"stamp","builtin_interfaces/msg/Time"},
        {"client_gid","uint8",16,true},{"sequence_number","int64"}};
    constexpr field time[]{{"sec","int32"},{"nanosec","uint32"}};
    record service[]{ {"uwvm_ros_bounded_v2/srv/Empty",root,3},
        {"uwvm_ros_bounded_v2/srv/Empty_Request",empty,1}, {"uwvm_ros_bounded_v2/srv/Empty_Response",empty,1},
        {"uwvm_ros_bounded_v2/srv/Empty_Event",event,3}, {"service_msgs/msg/ServiceEventInfo",info,4},
        {"builtin_interfaces/msg/Time",time,2} };
    constexpr std::string_view service_hash{"RIHS01_a0f5e07741124efbd845dab80bdb5cc355cabe501c2718b51d2377a3cfa6527b"};
    assert(matches_rihs({service,6},service[0].type,service_hash));
    // Caller-supplied record order is immaterial; declaration field order is exact.
    std::swap(service[1],service[5]);
    assert(matches_rihs({service,6},service[0].type,service_hash));
    service[0].field_count=2;
    assert(!matches_rihs({service,6},service[0].type,service_hash));
    service[0].field_count=3;
    assert(!matches_rihs({service,5},service[0].type,service_hash));
    record duplicate[]{point_record[0],point_record[0]};
    assert(!matches_rihs({duplicate,2},point_record[0].type,point_hash));
    // A disconnected row and a recursive dependency both fail closed.
    record unrelated[]{point_record[0],{"builtin_interfaces/msg/Time",time,2}};
    assert(!matches_rihs({unrelated,2},point_record[0].type,point_hash));
    constexpr field recursive[]{{"self","uwvm_ros_bounded_v2/msg/Point"}};
    point_record[0].fields=recursive;point_record[0].field_count=1;
    assert(!matches_rihs({point_record,1},point_record[0].type,point_hash));
}
