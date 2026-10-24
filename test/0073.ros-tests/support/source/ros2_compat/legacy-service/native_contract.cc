/** @file Real installed-SDK legacy Service metadata boundary contract.
 * No fake ServiceMembers, Native type, generator, type support or native hash.
 * Build/run only through an independent original-pidfd owner with full current
 * source/SDK/compiler/MD/link/ELF/maps/cgroup evidence. No Node/entity is made.
 * A modern SDK must refuse; it cannot count as the legacy positive route.
 */
#include "src/ros/gateway/ros2/compat/metadata_export.h"
#include <std_srvs/srv/trigger.hpp>
#include <std_srvs/srv/set_bool.hpp>
#include <array>
#include <cassert>
#include <cstdio>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>

#if !UWVM_ROS2_HAVE_OPENSSL || !UWVM_ROS2_HAVE_SERVICE_INTROSPECTION_CPP
#error "Actual selected ROS2 Service introspection and linked OpenSSL required"
#endif
#ifdef NDEBUG
#error "Compile actual Service contracts with assertions enabled (-UNDEBUG)"
#endif
namespace compat=uwvm2::ros_gateway::ros2_compat;
namespace ipc=uwvm2::imported::ros::ipc;
using Field=ipc::custom_native_field;
using Record=ipc::custom_native_record;
unsigned checks{};
void check(bool value) { ++checks;assert(value); }
template<class Native> bool refused(ipc::custom_native_schema schema,std::string_view root)
{
    try {(void)compat::legacy_metadata_export::service<Native>(schema,root);return false;}
    catch(std::runtime_error const&) {return true;}
}

template<class Native> void family(std::string_view name)
{
    std::array<Record,32> storage{};auto schema=compat::builtin::schema(name,storage);
    check(schema.record_count>4);
    auto support=rosidl_typesupport_cpp::get_service_type_support_handle<Native>();check(support!=nullptr);
    if constexpr(compat::has_baked_hash<std::remove_pointer_t<decltype(support)>>::value)
    {
        check(refused<Native>(schema,name));
    }
    else
    {
        // Both these services use actual installed Request/Response metadata;
        // Trigger's empty Request exercises the official representation dummy.
        auto text=compat::legacy_metadata_export::service<Native>(schema,name);
        check(text.find("LEGACY_NATIVE_SERVICE_WITH_STANDARD_PROTOCOL_DERIVED_RIHS01")!=std::string::npos);
        check(text.find("\"native_baked_hash_available\": false")!=std::string::npos);
        check(text.find("\"standard_protocol\": \"ROS2_SERVICE_EVENT_V1\"")!=std::string::npos);
        constexpr bool actual_event=compat::legacy_metadata_export::has_event_members<::rosidl_typesupport_introspection_cpp::ServiceMembers>::value;
        check(text.find(actual_event?"\"protocol_event_native_introspection\": true":"\"protocol_event_native_introspection\": false")!=std::string::npos);
        auto identity=compat::rihs_identity(schema,name);check(!identity.empty());check(text.find(identity)!=std::string::npos);
        std::array<Record,256> rows{};
        auto request=std::string{name}+"_Request";
        auto subset=compat::legacy_metadata_export::reachable(schema,request,rows);
        auto component=compat::legacy_metadata_export::message<typename Native::Request>(subset,request);
        check(component.find("LEGACY_NATIVE_INTROSPECTION_DERIVED_RIHS01")!=std::string::npos);
    }
    check(refused<Native>({},name));check(refused<Native>(schema,"other/srv/Missing"));
    check(refused<Native>(schema,name.substr(0,name.size()-1)));
    // Mutations are only compiled declaration views, never replacements for
    // actual installed introspection/type support. Each must be refused.
    for(std::size_t index{};index!=schema.record_count;++index)
    {
        if(schema.records[index].type==name)
        {
            std::array<Field,3> fields{{schema.records[index].fields[0],schema.records[index].fields[1],schema.records[index].fields[2]}};
            fields[2].name="wrong_event";auto changed=storage;changed[index].fields=fields.data();
            check(refused<Native>({changed.data(),schema.record_count},name));
            fields[2]=schema.records[index].fields[2];fields[0].type=fields[1].type;
            check(refused<Native>({changed.data(),schema.record_count},name));
        }
        if(schema.records[index].type==std::string{name}+"_Event")
        {
            std::array<Field,3> fields{{schema.records[index].fields[0],schema.records[index].fields[1],schema.records[index].fields[2]}};
            fields[1].sequence_bound=0;auto changed=storage;changed[index].fields=fields.data();
            check(refused<Native>({changed.data(),schema.record_count},name));
        }
        if(schema.records[index].type=="service_msgs/msg/ServiceEventInfo")
        {
            std::array<Field,4> fields{{schema.records[index].fields[0],schema.records[index].fields[1],schema.records[index].fields[2],schema.records[index].fields[3]}};
            fields[2].type="char";auto changed=storage;changed[index].fields=fields.data();
            check(refused<Native>({changed.data(),schema.record_count},name));
        }
        if(schema.records[index].type=="builtin_interfaces/msg/Time")
        {
            std::array<Field,2> fields{{schema.records[index].fields[0],schema.records[index].fields[1]}};
            fields[0].type="uint32";auto changed=storage;changed[index].fields=fields.data();
            check(refused<Native>({changed.data(),schema.record_count},name));
        }
        if(schema.records[index].type==std::string{name}+"_Response")
        {
            auto changed=storage;changed[index].field_count=1;
            check(refused<Native>({changed.data(),schema.record_count},name));
            std::array<Field,2> fields{{schema.records[index].fields[0],schema.records[index].fields[1]}};
            fields[1].string_bound=32;changed=storage;changed[index].fields=fields.data();
            check(refused<Native>({changed.data(),schema.record_count},name));
        }
    }
    auto duplicate=storage;duplicate[schema.record_count]=schema.records[0];
    check(refused<Native>({duplicate.data(),schema.record_count+1},name));
    check(refused<Native>({schema.records,schema.record_count-1},name));
}
int main()
{
    family<std_srvs::srv::Trigger>("std_srvs/srv/Trigger");
    family<std_srvs::srv::SetBool>("std_srvs/srv/SetBool");
    auto support=rosidl_typesupport_cpp::get_service_type_support_handle<std_srvs::srv::Trigger>();
    constexpr bool modern=compat::has_baked_hash<std::remove_pointer_t<decltype(support)>>::value;
    std::printf("PASS_LEGACY_SERVICE_NATIVE_CONTRACT sdk=%s families=2 checks=%u\n",modern?"modern-refusal":"legacy-native",checks);
    return 0;
}
