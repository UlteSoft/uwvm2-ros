/** @file
 * @brief Capture actual native QoS/GIDs and same-record receive information.
 *
 * This trusted Gateway helper includes the original selected ROS SDK. It does
 * not reconstruct a generated message, publish a probe, project a configuration
 * as actual QoS, or read metadata from another message. All output commits are
 * transactional. Missing native members/APIs return95 or leave explicitly
 * absent metadata bits; strict upstream consumers require the complete mask.
 *
 * The owner must retain the SAME native entity through activation, subsequent
 * business operations and explicit retirement. A separate probe's QoS/GID is
 * insufficient. Session/owner/incarnation checks belong to the owning transport.
 */
#pragma once
#include "compat/features.h"
#include <uwvm2/imported/ros/ipc/rmw_metadata.h>
#include <rmw/types.h>
#include <rmw/rmw.h>
#if UWVM_ROS2_HAVE_COMPLETE_RMW_QOS
#include <rmw/time.h>
#endif
#include <rcl/client.h>
#include <iterator>
#include <type_traits>
#include <utility>

namespace uwvm2::ros_gateway::native_rmw_metadata
{
    namespace wire=::uwvm2::imported::ros::ipc::rmw_endpoint;
    namespace info=::uwvm2::imported::ros::ipc::rmw_metadata;
    template<class NativeDuration>bool actual_duration(NativeDuration const& native,wire::duration& output) noexcept
    {
#if UWVM_ROS2_HAVE_COMPLETE_RMW_QOS
        // Preserve the selected original RMW semantics, including saturation
        // at its infinite duration. An arithmetic carry alone would preserve
        // representation differences which rmw_time_equal declares equal.
        rmw_time_t observed{native.sec,native.nsec};
        auto normalized{rmw_time_normalize(observed)};
        output={normalized.sec,normalized.nsec};return true;
#else
        (void)native;(void)output;return false;
#endif
    }
    /** Map a genuinely queried native effective profile. The caller must obtain
     * it from the retained entity's real actual-QoS getter, never from requested
     * options. Unknown/system-default policies are deliberately unrepresentable.
     * No native enum's numeric representation is assumed to be our wire enum. */
    inline bool actual_profile(rmw_qos_profile_t const& value,wire::qos& output) noexcept
    {
#if UWVM_ROS2_HAVE_COMPLETE_RMW_QOS
        wire::qos q{};
        switch(value.history){case RMW_QOS_POLICY_HISTORY_KEEP_LAST:q.history=1;break;case RMW_QOS_POLICY_HISTORY_KEEP_ALL:q.history=2;break;default:return false;}
        switch(value.reliability){case RMW_QOS_POLICY_RELIABILITY_RELIABLE:q.reliability=1;break;case RMW_QOS_POLICY_RELIABILITY_BEST_EFFORT:q.reliability=2;break;default:return false;}
        switch(value.durability){case RMW_QOS_POLICY_DURABILITY_TRANSIENT_LOCAL:q.durability=1;break;case RMW_QOS_POLICY_DURABILITY_VOLATILE:q.durability=2;break;default:return false;}
        switch(value.liveliness){case RMW_QOS_POLICY_LIVELINESS_AUTOMATIC:q.liveliness=1;break;case RMW_QOS_POLICY_LIVELINESS_MANUAL_BY_TOPIC:q.liveliness=2;break;default:return false;}
        q.depth=value.depth;
        if(!actual_duration(value.deadline,q.deadline)||!actual_duration(value.lifespan,q.lifespan)||
            !actual_duration(value.liveliness_lease_duration,q.lease)){return false;}
        q.avoid_ros_namespace_conventions=value.avoid_ros_namespace_conventions;
        if(!wire::valid(q)){return false;}output=q;return true;
#else
        (void)value;(void)output;return false;
#endif
    }
    inline bool actual_gid(rmw_gid_t const& native,std::array<std::uint8_t,32>& bytes,std::uint8_t& extent) noexcept
    {
        // Retain only value bytes. implementation_identifier is a native SDK
        // pointer, not a portable Guest pointer or an independent credential.
        if(!native.implementation_identifier||!native.implementation_identifier[0]){return false;}
        if constexpr(sizeof(native.data)>32||sizeof(native.data)==0){return false;}
        else
        {
            std::array<std::uint8_t,32> copy{};std::copy(std::begin(native.data),std::end(native.data),copy.begin());
            bytes=copy;extent=static_cast<std::uint8_t>(sizeof(native.data));return true;
        }
    }
    inline rmw_qos_profile_t const& profile(rmw_qos_profile_t const& value) noexcept{return value;}
    template<class T>auto profile(T const& value)->decltype(value.get_rmw_qos_profile()){return value.get_rmw_qos_profile();}
    template<class T,class=void>struct has_entity_actual:std::false_type{};
    template<class T>struct has_entity_actual<T,std::void_t<decltype(profile(std::declval<T const&>().get_actual_qos()))>>:std::true_type{};
    template<class T,class=void>struct has_gid:std::false_type{};
    template<class T>struct has_gid<T,std::void_t<decltype(std::declval<T const&>().get_gid())>>:std::true_type{};
    template<class T,class=void>struct has_client_actual:std::false_type{};
    template<class T>struct has_client_actual<T,std::void_t<decltype(profile(std::declval<T const&>().get_request_publisher_actual_qos())),
        decltype(profile(std::declval<T const&>().get_response_subscription_actual_qos()))>>:std::true_type{};
    template<class T,class=void>struct has_service_actual:std::false_type{};
    template<class T>struct has_service_actual<T,std::void_t<decltype(profile(std::declval<T const&>().get_request_subscription_actual_qos())),
        decltype(profile(std::declval<T const&>().get_response_publisher_actual_qos()))>>:std::true_type{};
    template<class T>auto raw_handle(T const& handle,int)->decltype(handle.get()){return handle.get();}
    template<class T>T* raw_handle(T* handle,long)noexcept{return handle;}
    template<class Client>auto client_gid(Client const& selected,std::array<std::uint8_t,32>& output,std::uint8_t& extent,int)
        ->decltype(rmw_get_gid_for_client(rcl_client_get_rmw_handle(raw_handle(selected.get_client_handle(),0)),static_cast<rmw_gid_t*>(nullptr)),std::uint32_t{})
    {
        rmw_gid_t gid{};auto result{rmw_get_gid_for_client(rcl_client_get_rmw_handle(raw_handle(selected.get_client_handle(),0)),&gid)};
        if(result==RMW_RET_UNSUPPORTED){return 95;}if(result!=RMW_RET_OK){return 5;}
        return actual_gid(gid,output,extent)?0u:95u;
    }
    template<class Client>std::uint32_t client_gid(Client const&,std::array<std::uint8_t,32>&,std::uint8_t&,long)noexcept{return 95;}
    /** Caller supplies a validated response association/incarnation. No owner is
     * created here. Query failures leave the entire caller packet unchanged. */
    template<class Publisher>std::uint32_t publisher(Publisher const& selected,wire::packet& output) noexcept
    {
        if constexpr(!has_entity_actual<Publisher>::value||!has_gid<Publisher>::value){return 95;}
        else
        {
            try
            {
                auto p{output};if(p.kind!=wire::role::publisher){return 22;}
                auto actual{selected.get_actual_qos()};
                if(!actual_profile(profile(actual),p.actual[0])||!actual_gid(selected.get_gid(),p.gid,p.gid_size)){return 95;}
                p.profiles=1;if(!wire::valid_response(p)){return 22;}output=p;return 0;
            }
            catch(...){return 5;}
        }
    }
    template<class Subscription>std::uint32_t subscription(Subscription const& selected,wire::packet& output) noexcept
    {
        if constexpr(!has_entity_actual<Subscription>::value){return 95;}
        else
        {
            try
            {
                auto p{output};if(p.kind!=wire::role::subscription){return 22;}
                auto actual{selected.get_actual_qos()};if(!actual_profile(profile(actual),p.actual[0])){return 95;}
                p.profiles=1;p.gid_size=0;p.gid.fill(0);if(!wire::valid_response(p)){return 22;}output=p;return 0;
            }
            catch(...){return 5;}
        }
    }
    template<class Client>std::uint32_t client(Client const& selected,wire::packet& output) noexcept
    {
        if constexpr(!has_client_actual<Client>::value){return 95;}
        else
        {
            try
            {
                auto p{output};if(p.kind!=wire::role::client){return 22;}
                auto request{selected.get_request_publisher_actual_qos()},response{selected.get_response_subscription_actual_qos()};
                if(!actual_profile(profile(request),p.actual[0])||!actual_profile(profile(response),p.actual[1])){return 95;}
                p.profiles=2;p.gid_size=0;p.gid.fill(0);
                // A real native client-GID API is optional across old SDKs.
                // Its absence does not fabricate a global client identifier.
                auto code{client_gid(selected,p.gid,p.gid_size,0)};if(code&&code!=95){return code;}
                if(!wire::valid_response(p)){return 22;}output=p;return 0;
            }
            catch(...){return 5;}
        }
    }
    template<class Service>std::uint32_t service(Service const& selected,wire::packet& output) noexcept
    {
        if constexpr(!has_service_actual<Service>::value){return 95;}
        else
        {
            try
            {
                auto p{output};if(p.kind!=wire::role::service){return 22;}
                auto request{selected.get_request_subscription_actual_qos()},response{selected.get_response_publisher_actual_qos()};
                if(!actual_profile(profile(request),p.actual[0])||!actual_profile(profile(response),p.actual[1])){return 95;}
                p.profiles=2;p.gid_size=0;p.gid.fill(0);if(!wire::valid_response(p)){return 22;}output=p;return 0;
            }
            catch(...){return 5;}
        }
    }
    template<class T,class=void>struct has_times:std::false_type{};
    template<class T>struct has_times<T,std::void_t<decltype(std::declval<T const&>().source_timestamp),decltype(std::declval<T const&>().received_timestamp)>>:std::true_type{};
    template<class T,class=void>struct has_sequences:std::false_type{};
    template<class T>struct has_sequences<T,std::void_t<decltype(std::declval<T const&>().publication_sequence_number),decltype(std::declval<T const&>().reception_sequence_number)>>:std::true_type{};
    template<class NativeMessageInfo>std::uint32_t topic(NativeMessageInfo const& native,info::header& output) noexcept
    {
        auto h{output};if(h.kind!=::uwvm2::imported::ros::ipc::event_kind::topic_message){return 22;}
        h.flags=0;h.error=0;h.control_flags=0;h.source_timestamp=h.received_timestamp=h.request_sequence=0;
        h.publication_number=h.reception_number=UINT64_MAX;h.writer_size=0;h.writer.fill(0);
        if(!actual_gid(native.publisher_gid,h.gid,h.gid_size)){return 95;}h.flags|=info::publisher_gid;
        if constexpr(has_times<NativeMessageInfo>::value)
        {h.source_timestamp=native.source_timestamp;h.received_timestamp=native.received_timestamp;h.flags|=info::source_time|info::received_time;}
        if constexpr(has_sequences<NativeMessageInfo>::value)
        {
            if(native.publication_sequence_number!=UINT64_MAX){h.publication_number=native.publication_sequence_number;h.flags|=info::publication_sequence;}
            if(native.reception_sequence_number!=UINT64_MAX){h.reception_number=native.reception_sequence_number;h.flags|=info::reception_sequence;}
        }
        if(!info::valid(h)){return 22;}output=h;return 0;
    }
    template<class NativeServiceInfo>std::uint32_t service_record(NativeServiceInfo const& native,info::header& output) noexcept
    {
        // Production callers supply the original rmw_service_info_t obtained in
        // the SAME rcl_take_*_with_info operation as the canonical body. Old
        // rclcpp callbacks carrying only rmw_request_id_t cannot supply this API.
        if constexpr(!has_times<NativeServiceInfo>::value){return 95;}
        else
        {
            auto h{output};using kind=::uwvm2::imported::ros::ipc::event_kind;
            if(h.kind!=kind::service_request&&h.kind!=kind::service_response){return 22;}
            if constexpr(sizeof(native.request_id.writer_guid)>16||sizeof(native.request_id.writer_guid)==0){return 95;}
            else
            {
                h.flags=info::source_time|info::received_time|info::native_request_id;h.error=0;h.control_flags=0;
                h.source_timestamp=native.source_timestamp;h.received_timestamp=native.received_timestamp;
                h.request_sequence=native.request_id.sequence_number;
                h.publication_number=h.reception_number=UINT64_MAX;h.gid_size=0;h.gid.fill(0);h.writer.fill(0);
                h.writer_size=static_cast<std::uint8_t>(sizeof(native.request_id.writer_guid));
                std::copy(std::begin(native.request_id.writer_guid),std::end(native.request_id.writer_guid),h.writer.begin());
                if(!info::valid(h)){return 22;}output=h;return 0;
            }
        }
    }
}
