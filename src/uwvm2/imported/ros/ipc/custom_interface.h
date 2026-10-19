/*************************************************************
 * UWVM2 bounded custom Service/Action envelopes. APL-2.0.    *
 *************************************************************/
#pragma once
#include "custom_type.h"

#if !defined(UWVM_ROS_CUSTOM_BOUNDED_INTERFACES)
namespace uwvm2::imported::ros::ipc
{
    // No generated trusted provider means no supported custom interface. An
    // empty table neither registers a ROS entity nor adds policy authority.
    inline constexpr custom_service_registry custom_service_entries() noexcept {return {nullptr,0};}
    inline constexpr custom_action_registry custom_action_entries() noexcept {return {nullptr,0};}
}
#endif

namespace uwvm2::imported::ros::ipc
{
    namespace custom_registry_detail
    {
#if defined(UWVM_ROS_ORIGINAL_CONTROL_MANIFEST)
        inline constexpr auto legacy_services=custom_service_entries();
        inline constexpr auto control_services=original_control_generated::custom_service_entries();
        inline constexpr bool service_sources_valid=mergeable(legacy_services,control_services);
        inline constexpr ::std::size_t service_count=service_sources_valid?legacy_services.size+control_services.size:0;
        inline constexpr auto services=concatenate<custom_service_descriptor,service_count>(legacy_services,control_services);
        inline constexpr auto legacy_actions=custom_action_entries();
        inline constexpr auto control_actions=original_control_generated::custom_action_entries();
        inline constexpr bool action_sources_valid=mergeable(legacy_actions,control_actions);
        inline constexpr ::std::size_t action_count=action_sources_valid?legacy_actions.size+control_actions.size:0;
        inline constexpr auto actions=concatenate<custom_action_descriptor,action_count>(legacy_actions,control_actions);
#endif
    }
    inline constexpr custom_service_registry effective_custom_service_entries() noexcept
    {
#if defined(UWVM_ROS_ORIGINAL_CONTROL_MANIFEST)
        if(!custom_registry_detail::service_sources_valid)return {nullptr,129};
        return {custom_registry_detail::services.data(),custom_registry_detail::services.size()};
#else
        return custom_service_entries();
#endif
    }
    inline constexpr custom_action_registry effective_custom_action_entries() noexcept
    {
#if defined(UWVM_ROS_ORIGINAL_CONTROL_MANIFEST)
        if(!custom_registry_detail::action_sources_valid)return {nullptr,129};
        return {custom_registry_detail::actions.data(),custom_registry_detail::actions.size()};
#else
        return custom_action_entries();
#endif
    }
    inline constexpr bool custom_interface_name(unsigned version,::std::string_view type,::std::string_view kind) noexcept
    {
        if(version==1) { return custom_topic_name(version,type); }
        auto slash{type.find('/')};
        if(version!=2||slash==::std::string_view::npos||type.size()>255) {return false;}
        auto tail{type.substr(slash+1)};
        if(tail.substr(0,kind.size())!=kind||tail.size()<=kind.size()||tail[kind.size()]!='/') {return false;}
        // Reuse the checked package/message identifier grammar without an
        // allocation. Validate package and terminal name via separate slices.
        auto package{type.substr(0,slash)},name{tail.substr(kind.size()+1)};
        if(package.empty()||package.size()>63||package.front()<'a'||package.front()>'z'||name.empty()||name.front()<'A'||name.front()>'Z') {return false;}
        for(auto reserved : {"builtin","std_msgs","std_srvs","geometry_msgs","sensor_msgs","rcl_interfaces","lifecycle_msgs","actionlib_tutorials","action_tutorials_interfaces"})
        {if(package==reserved) {return false;}}
        for(char c:package) {if(!((c>='a'&&c<='z')||(c>='0'&&c<='9')||c=='_')) {return false;}}
        for(char c:name) {if(!((c>='a'&&c<='z')||(c>='A'&&c<='Z')||(c>='0'&&c<='9'))) {return false;}}
        return true;
    }
    inline constexpr bool custom_native_identity_valid(unsigned version,::std::string_view identity) noexcept
    {
        return version==1?(identity.size()==32&&custom_hexadecimal(identity)):
            version==2&&identity.size()==71&&identity.substr(0,7)=="RIHS01_"&&custom_hexadecimal(identity.substr(7));
    }
    /** Component metadata names include the official _Request/_Goal suffixes.
     * This helper validates schemas only; it never makes a component a Topic. */
    inline constexpr bool custom_native_record_name(unsigned version,::std::string_view type) noexcept
    {
        if(custom_topic_name(version,type)) {return true;}
        if(version!=2||type.size()>255) {return false;}auto slash{type.find('/')};
        if(slash==::std::string_view::npos||!slash||slash>63) {return false;}
        auto package{type.substr(0,slash)},tail{type.substr(slash+1)};
        if(package.front()<'a'||package.front()>'z') {return false;}
        for(auto reserved : {"builtin","std_msgs","std_srvs","geometry_msgs","sensor_msgs","rcl_interfaces","lifecycle_msgs","actionlib_tutorials","action_tutorials_interfaces"})
        {if(package==reserved) {return false;}}
        for(char c:package) {if(!((c>='a'&&c<='z')||(c>='0'&&c<='9')||c=='_')) {return false;}}
        if(tail.substr(0,4)=="srv/") {tail.remove_prefix(4);}
        else if(tail.substr(0,7)=="action/") {tail.remove_prefix(7);}
        else {return false;}
        if(tail.empty()||tail.front()<'A'||tail.front()>'Z') {return false;}
        for(char c:tail) {if(!((c>='a'&&c<='z')||(c>='A'&&c<='Z')||(c>='0'&&c<='9')||c=='_')) {return false;}}
        return true;
    }
    inline constexpr bool custom_component_valid(unsigned version,custom_topic_descriptor const* component) noexcept
    {
        return component&&component->ros_version==version&&
            (component->complete_vendor ? (!component->original_control&&!component->official_standard&&component->bounded_layout&&complete_vendor_native_name(version,component->type)) :
                component->original_control ? (!component->official_standard&&component->bounded_layout&&original_control_record_name(version,component->type)) :
                custom_native_record_name(version,component->type))&&custom_topic_layout_valid(*component)&&
            custom_codec_identity(component->codec_identity)&&custom_native_identity_valid(version,component->native_identity);
    }
    inline constexpr bool custom_component_owner(::std::string_view owner,custom_topic_descriptor const* part,::std::string_view role) noexcept
    {
        if(!part) {return false;}auto name{part->type};auto separator{part->ros_version==2?1u:0u};
        return name.size()==owner.size()+separator+role.size()&&name.substr(0,owner.size())==owner&&
            (!separator||name[owner.size()]=='_')&&name.substr(owner.size()+separator)==role;
    }
    /** Complete immutable Service row admission, without Native effects or
     * authority. New control pairs must both opt in and name one exact original
     * Service. Owner suffixes and both independent identities remain mandatory.
     */
    inline constexpr bool custom_service_shape_valid(custom_service_descriptor const& item) noexcept
    {
        bool const control=item.request&&item.response&&(item.request->original_control||item.response->original_control);
        bool const complete=item.request&&item.response&&(item.request->complete_vendor||item.response->complete_vendor);
        return (complete ? (!control&&item.request->complete_vendor&&item.response->complete_vendor&&
                    complete_vendor_endpoint_name(item.ros_version,item.type,"srv")) :
                control ? (item.request->original_control&&item.response->original_control&&original_control_service_name(item.ros_version,item.type)) :
                    custom_interface_name(item.ros_version,item.type,"srv"))&&custom_native_identity_valid(item.ros_version,item.native_identity)&&
            custom_codec_identity(item.codec_identity)&&custom_component_valid(item.ros_version,item.request)&&custom_component_valid(item.ros_version,item.response)&&
            custom_component_owner(item.type,item.request,"Request")&&custom_component_owner(item.type,item.response,"Response");
    }
    inline constexpr bool custom_component_charge(custom_topic_descriptor const& part,::std::size_t& total) noexcept
    {
        custom_wire_extent extent{};::std::size_t count{};
        if(part.bounded_layout) {if(!custom_wire_extent_of(*part.bounded_layout,extent)) {return false;}count=extent.cells;}
        else {for(::std::size_t i{};i!=part.layout_count;++i) {if(part.layout[i].count>16384||count>16384-part.layout[i].count) {return false;}count+=part.layout[i].count;}}
        if(count>16384||total>16384-count) {return false;}total+=count;return true;
    }
    inline constexpr bool custom_interface_registries_valid() noexcept
    {
        auto services{effective_custom_service_entries()};auto actions{effective_custom_action_entries()};auto topics{effective_custom_topic_entries()};
        if(!custom_topic_registry_valid||services.size>128||actions.size>128||topics.size+services.size+actions.size>128||
           (services.size&&!services.data)||(actions.size&&!actions.data)) {return false;}
        ::std::size_t expanded{};
        for(::std::size_t i{};i!=topics.size;++i) {if(!custom_component_charge(topics.data[i],expanded)) {return false;}}
        for(::std::size_t i{};i!=services.size;++i)
        {
            auto const& item{services.data[i]};
            if(!custom_service_shape_valid(item)||!custom_component_charge(*item.request,expanded)||
               !custom_component_charge(*item.response,expanded)) {return false;}
            for(::std::size_t j{};j!=i;++j) {if(item.ros_version==services.data[j].ros_version&&item.type==services.data[j].type) {return false;}}
            for(::std::size_t j{};j!=topics.size;++j) {if(item.ros_version==topics.data[j].ros_version&&item.type==topics.data[j].type) {return false;}}
        }
        for(::std::size_t i{};i!=actions.size;++i)
        {
            auto const& item{actions.data[i]};
            bool const complete=(item.goal&&item.goal->complete_vendor)||(item.result&&item.result->complete_vendor)||(item.feedback&&item.feedback->complete_vendor);
            if((item.goal&&item.goal->original_control)||(item.result&&item.result->original_control)||(item.feedback&&item.feedback->original_control)||
               !(complete ? (item.goal&&item.result&&item.feedback&&item.goal->complete_vendor&&item.result->complete_vendor&&item.feedback->complete_vendor&&
                    complete_vendor_endpoint_name(item.ros_version,item.type,"action")) : custom_interface_name(item.ros_version,item.type,"action"))||!custom_native_identity_valid(item.ros_version,item.native_identity)||
               !custom_codec_identity(item.codec_identity)||!custom_component_valid(item.ros_version,item.goal)||
               !custom_component_valid(item.ros_version,item.result)||!custom_component_valid(item.ros_version,item.feedback)||
               !custom_component_owner(item.type,item.goal,"Goal")||!custom_component_owner(item.type,item.result,"Result")||!custom_component_owner(item.type,item.feedback,"Feedback")||
               !custom_component_charge(*item.goal,expanded)||!custom_component_charge(*item.result,expanded)||!custom_component_charge(*item.feedback,expanded)) {return false;}
            for(::std::size_t j{};j!=i;++j) {if(item.ros_version==actions.data[j].ros_version&&item.type==actions.data[j].type) {return false;}}
            for(::std::size_t j{};j!=topics.size;++j) {if(item.ros_version==topics.data[j].ros_version&&item.type==topics.data[j].type) {return false;}}
            for(::std::size_t j{};j!=services.size;++j) {if(item.ros_version==services.data[j].ros_version&&item.type==services.data[j].type) {return false;}}
        }
        return true;
    }
    inline constexpr bool custom_interface_registry_valid{custom_interface_registries_valid()};
    inline constexpr bool custom_interface_registry_available() noexcept {return custom_interface_registry_valid;}
    inline constexpr custom_service_descriptor const* find_custom_service(unsigned version,::std::string_view type) noexcept
    {
        if(!custom_interface_registry_valid) {return nullptr;}auto registry{effective_custom_service_entries()};
        for(::std::size_t i{};i!=registry.size;++i) {if(registry.data[i].ros_version==version&&registry.data[i].type==type) {return registry.data+i;}}
        return nullptr;
    }
    inline constexpr custom_action_descriptor const* find_custom_action(unsigned version,::std::string_view type) noexcept
    {
        if(!custom_interface_registry_valid) {return nullptr;}auto registry{effective_custom_action_entries()};
        for(::std::size_t i{};i!=registry.size;++i) {if(registry.data[i].ros_version==version&&registry.data[i].type==type) {return registry.data+i;}}
        return nullptr;
    }
    inline constexpr ::std::string_view custom_service_type_identity(unsigned version,::std::string_view type) noexcept
    {auto item{find_custom_service(version,type)};return item ? item->native_identity : ::std::string_view{};}
    inline constexpr ::std::string_view custom_action_type_identity(unsigned version,::std::string_view type) noexcept
    {auto item{find_custom_action(version,type)};return item ? item->native_identity : ::std::string_view{};}
    /** Require a nonzero Runtime-owned ticket before evaluating its component.
     * A correct payload cannot create or adopt a ticket: ownership/one-shot
     * response, timeout and cancel scope remain the Capability Engine's job. */
    inline bool custom_ticket_component(custom_topic_descriptor const* part,::std::uint8_t const* bytes,::std::size_t size,::std::size_t prefix) noexcept
    {return part&&bytes&&(prefix==8||prefix==9)&&size>=prefix&&custom_wire_bits(bytes,8)&&validate_custom_topic(*part,bytes+prefix,size-prefix);}
    inline bool validate_custom_service_request(custom_service_descriptor const& item,::std::string_view operation,::std::uint8_t const* bytes,::std::size_t size) noexcept
    {
        if(!item.request||!item.response) {return false;}
        if(operation=="service.call") {return validate_custom_topic(*item.request,bytes,size);}
        if(operation=="service.advertise") {return size==0;}
        if(operation=="service.respond") {return custom_ticket_component(item.response,bytes,size,8);}
        return false;
    }
    inline bool validate_custom_service_output(custom_service_descriptor const& item,::std::string_view operation,::std::uint8_t const* bytes,::std::size_t size) noexcept
    {return item.response&&(operation=="service.call"?validate_custom_topic(*item.response,bytes,size):(operation=="service.advertise"||operation=="service.respond")&&size==0);}
    inline bool validate_custom_service_event(custom_service_descriptor const& item,::std::uint8_t const* bytes,::std::size_t size) noexcept
    {return custom_ticket_component(item.request,bytes,size,8);}
    inline bool validate_custom_action_request(custom_action_descriptor const& item,::std::string_view operation,::std::uint8_t const* bytes,::std::size_t size) noexcept
    {
        if(!item.goal||!item.result||!item.feedback) {return false;}
        if(operation=="action.send-goal") {return validate_custom_topic(*item.goal,bytes,size);}
        if(operation=="action.advertise") {return size==0;}
        if(operation=="action.respond")
        {
            if(!bytes||size<9||bytes[8]<1||bytes[8]>4) {return false;}
            return custom_ticket_component(bytes[8]==1?item.feedback:item.result,bytes,size,9);
        }
        if(operation=="action.cancel-own-goal"||operation=="action.receive-feedback"||operation=="action.receive-result")
        {return bytes&&size==8&&custom_wire_bits(bytes,8)!=0;}
        return operation=="action.cancel-any-goal"&&bytes&&size==8;
    }
    inline bool validate_custom_action_output(custom_action_descriptor const& item,::std::string_view operation,::std::uint8_t const* bytes,::std::size_t size) noexcept
    {
        if(operation=="action.send-goal") {return bytes&&size==8&&custom_wire_bits(bytes,8)!=0;}
        if(operation=="action.cancel-own-goal"||operation=="action.cancel-any-goal") {return bytes&&size==1&&bytes[0]<=1;}
        if(operation=="action.receive-feedback") {return custom_ticket_component(item.feedback,bytes,size,8);}
        if(operation=="action.receive-result") {return bytes&&size>=9&&bytes[8]>=2&&bytes[8]<=4&&custom_ticket_component(item.result,bytes,size,9);}
        return (operation=="action.advertise"||operation=="action.respond")&&size==0;
    }
    inline bool validate_custom_action_event(custom_action_descriptor const& item,::std::uint8_t const* bytes,::std::size_t size) noexcept
    {return custom_ticket_component(item.goal,bytes,size,8);}
}
