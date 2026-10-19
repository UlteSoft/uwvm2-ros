/** @file
 * @brief Genuine installed ROS2 Message identity-export boundary contract.
 *
 * Build against the actual selected ROS2 SDK, introspection/type-support
 * libraries and OpenSSL; no fake Native, metadata struct or type support is
 * supplied. Run only in the unified current-source Linux cgroup lane after
 * full compiler/MD/link/ELF/SDK/proc ownership qualification. This exercises
 * declarations and exporter refusal; it does not create a Node, publish a ROS
 * message, move a robot or qualify network delivery/official-suite coverage.
 */
#include "../../gateways/ros2/compat/metadata_export.h"
#include <builtin_interfaces/msg/time.hpp>
#include <std_msgs/msg/header.hpp>
#include <array>
#include <cassert>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>

#if !UWVM_ROS2_HAVE_OPENSSL
#error "Use actual target OpenSSL Crypto; missing Crypto is not a native acceptance result"
#endif

namespace ipc=uwvm2::imported::ros::ipc;
namespace compat=uwvm2::ros_gateway::ros2_compat;
using field=ipc::custom_native_field;
using record=ipc::custom_native_record;
using Time=::builtin_interfaces::msg::Time;
using Header=::std_msgs::msg::Header;

template<class Native>
bool refused(ipc::custom_native_schema schema,std::string_view name)
{
    try { (void)compat::legacy_metadata_export::message<Native>(schema,name);return false; }
    catch(std::runtime_error const&) { return true; }
}

#ifdef NDEBUG
#error "Compile the native identity contract with assertions enabled (-UNDEBUG)"
#endif

int main()
{
    constexpr field time_fields[]{{"sec","int32"},{"nanosec","uint32"}};
    constexpr field header_fields[]{{"stamp","builtin_interfaces/msg/Time"},{"frame_id","string"}};
    record time[]{{"builtin_interfaces/msg/Time",time_fields,2}};
    record header[]{{"std_msgs/msg/Header",header_fields,2},time[0]};
    auto support=::rosidl_typesupport_cpp::get_message_type_support_handle<Time>();
    assert(support);
    if constexpr(compat::has_baked_hash<std::remove_pointer_t<decltype(support)>>::value)
    {
        // Even a healthy modern SDK cannot use this pre-RIHS fallback. A null,
        // malformed or unsupported modern hash cannot opt itself into it.
        assert(refused<Time>({time,1},time[0].type));
        assert(refused<Header>({header,2},header[0].type));
    }
    else
    {
        auto text=compat::legacy_metadata_export::message<Time>({time,1},time[0].type);
        auto expected=compat::rihs_identity({time,1},time[0].type);
        assert(!expected.empty()&&text.find(expected)!=std::string::npos);
        assert(text.find("LEGACY_NATIVE_INTROSPECTION_DERIVED_RIHS01")!=std::string::npos);
        assert(text.find("\"native_baked_hash_available\": false")!=std::string::npos);
        assert(text.find("\"name\": \"sec\"")!=std::string::npos);
        assert(text.find("\"name\": \"nanosec\"")!=std::string::npos);
        text=compat::legacy_metadata_export::message<Header>({header,2},header[0].type);
        assert(text.find("std_msgs/msg/Header")!=std::string::npos);
        assert(text.find("builtin_interfaces/msg/Time")!=std::string::npos);
        assert(text.find("\"name\": \"frame_id\"")!=std::string::npos);
    }
    // These refusal cases must work on both real old and real modern SDKs.
    assert(refused<Time>({},time[0].type));
    assert(refused<Time>({time,1},"builtin_interfaces/msg/Missing"));
    auto reversed_fields=std::array<field,2>{time_fields[1],time_fields[0]};
    record changed[]{{time[0].type,reversed_fields.data(),2}};
    assert(refused<Time>({changed,1},time[0].type));
    auto wrong_fields=std::array<field,2>{time_fields[0],time_fields[1]};
    wrong_fields[0].type="uint32";changed[0].fields=wrong_fields.data();
    assert(refused<Time>({changed,1},time[0].type));
    wrong_fields[0]=time_fields[0];wrong_fields[1].name="other";
    assert(refused<Time>({changed,1},time[0].type));
    changed[0]=time[0];changed[0].field_count=1;
    assert(refused<Time>({changed,1},time[0].type));
    record duplicate[]{time[0],time[0]};
    assert(refused<Time>({duplicate,2},time[0].type));
    record disconnected[]{time[0],header[0]};
    assert(refused<Time>({disconnected,2},time[0].type));
    assert(refused<Header>({header,1},header[0].type));
    auto bounded_header_fields=std::array<field,2>{header_fields[0],header_fields[1]};
    bounded_header_fields[1].string_bound=128;
    record narrowed_native[]{header[0],time[0]};narrowed_native[0].fields=bounded_header_fields.data();
    // A stricter canonical bound must never replace the actual native bound.
    assert(refused<Header>({narrowed_native,2},header[0].type));
    return 0;
}
