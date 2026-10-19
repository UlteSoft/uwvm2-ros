/*************************************************************
 * UWVM2 concrete bounded ROS2 Service adapter. APL-2.0.     *
 *************************************************************/
#pragma once
#include "compat/node.h"
#include "custom_topics.h"
#include "qos.h"
#include "compat/deferred_service.h"
#include "../common/service.h"
#include <rosidl_typesupport_cpp/service_type_support.hpp>
#include <future>

#ifndef UWVM_ROS_CUSTOM_ROS2_SERVICE_TYPES
#define UWVM_ROS_CUSTOM_ROS2_SERVICE_TYPES(M)
#endif

// Trusted compile-time partitions only; never a runtime selector or grant.
#ifndef UWVM_ROS2_CUSTOM_SERVICE_SHARDS
#define UWVM_ROS2_CUSTOM_SERVICE_SHARDS 32
#endif

namespace uwvm2::ros_gateway::ros2_custom_services
{
    /** Qualify the official root interface and both complete native components.
     * The typed aliases are compiled against the same installed official package;
     * a genuine root hash beside unrelated request/response codecs is refused.
     * No Node/entity/permission is created by these metadata-only checks. */
    template<class Service,class RequestCodec,class ResponseCodec>
    struct binding
    {
        static_assert(std::is_same<typename Service::Request,typename RequestCodec::Native>::value&&
            std::is_same<typename Service::Response,typename ResponseCodec::Native>::value,
            "custom service codecs must cover the actual official request and response types");
        static bool component(ipc::custom_topic_descriptor const* part,bool request)
        {
            if(!part||ipc::complete_vendor_codec<RequestCodec>::value!=ipc::complete_vendor_codec<ResponseCodec>::value||
               part->complete_vendor!=ipc::complete_vendor_codec<RequestCodec>::value) {return false;}
            return request?(part->type==RequestCodec::type&&part->native_identity==RequestCodec::native_identity&&
                part->codec_identity==RequestCodec::codec_identity&&part->wire_bytes==RequestCodec::wire_bytes&&
                ros2_custom_topics::binding<typename Service::Request,RequestCodec>::native_contract()):
                (part->type==ResponseCodec::type&&part->native_identity==ResponseCodec::native_identity&&
                part->codec_identity==ResponseCodec::codec_identity&&part->wire_bytes==ResponseCodec::wire_bytes&&
                ros2_custom_topics::binding<typename Service::Response,ResponseCodec>::native_contract());
        }
        static bool known(ros::capability_spec const& cap,std::string_view type,
            std::string_view identity,std::string_view canonical)
        {
            if(cap.operation!="service.call"&&cap.operation!="service.advertise"&&cap.operation!="service.respond") {return false;}
            auto item{ipc::find_custom_service(2,type)};auto selected{cap.limits.find("adapter-codec-identity")};
            if(!item||cap.type!=type||cap.type_identity!=identity||item->native_identity!=identity||
                item->codec_identity!=canonical||selected==cap.limits.end()||selected->second!=canonical||
                !component(item->request,true)||!component(item->response,false)) {return false;}
            auto support{rosidl_typesupport_cpp::get_service_type_support_handle<Service>()};
            return ros2_custom_topics::native_interface_contract<Service,ipc::complete_vendor_codec<RequestCodec>::value>(support,type,identity);
        }
    };
    class adapter;
    namespace detail
    {
        // Counting the original macro does not instantiate Service/Q/S types.
#define UWVM_CUSTOM_SERVICE_COUNT(Native,Type,Identity,Canonical,Request,Response) +1
        inline constexpr std::size_t service_count=0 UWVM_ROS_CUSTOM_ROS2_SERVICE_TYPES(UWVM_CUSTOM_SERVICE_COUNT);
#undef UWVM_CUSTOM_SERVICE_COUNT
        inline constexpr std::size_t service_shards=UWVM_ROS2_CUSTOM_SERVICE_SHARDS;
        static_assert(service_shards>=1&&service_shards<=256,"custom Service shard count must be1..256");
        template<std::size_t Shard,std::size_t Shards> struct service_shard
        {
            static bool known(std::size_t,ros::capability_spec const&);
            static ipc::call_result call(adapter&,std::size_t,ros::capability_spec const&,
                std::vector<std::uint8_t> const&,std::uint32_t);
            static ipc::call_result advertise(adapter&,std::size_t,ros::capability_spec const&,
                std::shared_ptr<event_queue>);
        private:
            template<std::size_t Index> static bool known_at(std::size_t,ros::capability_spec const&);
            template<std::size_t Index> static ipc::call_result call_at(adapter&,std::size_t,
                ros::capability_spec const&,std::vector<std::uint8_t> const&,std::uint32_t);
            template<std::size_t Index> static ipc::call_result advertise_at(adapter&,std::size_t,
                ros::capability_spec const&,std::shared_ptr<event_queue>);
        };
        bool dispatch_known(std::size_t,ros::capability_spec const&);
        ipc::call_result dispatch_call(adapter&,std::size_t,ros::capability_spec const&,
            std::vector<std::uint8_t> const&,std::uint32_t);
        ipc::call_result dispatch_advertise(adapter&,std::size_t,ros::capability_spec const&,
            std::shared_ptr<event_queue>);
    }
    inline bool known(ros::capability_spec const& cap)
    {
        unsigned matches{};bool accepted{};std::size_t type_index{};(void)type_index;
#define UWVM_CUSTOM_SERVICE_KNOWN(Native,Type,Identity,Canonical,Request,Response) \
        if(cap.type==Type){++matches;accepted=detail::dispatch_known(type_index,cap);} ++type_index;
        UWVM_ROS_CUSTOM_ROS2_SERVICE_TYPES(UWVM_CUSTOM_SERVICE_KNOWN)
#undef UWVM_CUSTOM_SERVICE_KNOWN
        return matches==1&&accepted;
    }
    /** Session-owned native service endpoints. Client calls have one absolute
     * deadline and send a request once; server request IDs use the shared bounded
     * pending-state machine. Respond and advertise remain separate authorities. */
    class adapter
    {
        template<std::size_t,std::size_t> friend struct detail::service_shard;
        ::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr node_;
        struct endpoint {ros::capability_spec cap;std::shared_ptr<service_state> state;std::shared_ptr<rclcpp::ServiceBase> entity;};
        std::map<std::string,endpoint> servers_;
        template<class Service,class RequestCodec,class ResponseCodec>
        ipc::call_result invoke(ros::capability_spec const& cap,std::vector<std::uint8_t> const& bytes,std::uint32_t timeout);
        template<class Service,class RequestCodec,class ResponseCodec>
        ipc::call_result serve(ros::capability_spec const& cap,std::shared_ptr<event_queue> events);
    public:
        explicit adapter(::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr node):node_{std::move(node)} {if(!node_) {throw std::invalid_argument("custom service Node is null");}}
        adapter(adapter const&)=delete;adapter& operator=(adapter const&)=delete;
        ~adapter(){for(auto& item:servers_) {item.second.state->deactivate();}}
        ipc::call_result call(ros::capability_spec const& cap,std::vector<std::uint8_t> const& bytes,std::uint32_t timeout)
        {
            if(!known(cap)||cap.operation!="service.call") {return {13,{}};}
            std::size_t type_index{};(void)type_index;
#define UWVM_CUSTOM_SERVICE_CALL(Native,Type,Identity,Canonical,Request,Response) \
            if(cap.type==Type)return detail::dispatch_call(*this,type_index,cap,bytes,timeout); ++type_index;
            UWVM_ROS_CUSTOM_ROS2_SERVICE_TYPES(UWVM_CUSTOM_SERVICE_CALL)
#undef UWVM_CUSTOM_SERVICE_CALL
            return {95,{}};
        }
        ipc::call_result advertise(ros::capability_spec const& cap,std::shared_ptr<event_queue> events)
        {
            if(!known(cap)||cap.operation!="service.advertise") {return {13,{}};}
            std::size_t type_index{};(void)type_index;
#define UWVM_CUSTOM_SERVICE_ADVERTISE(Native,Type,Identity,Canonical,Request,Response) \
            if(cap.type==Type)return detail::dispatch_advertise(*this,type_index,cap,std::move(events)); ++type_index;
            UWVM_ROS_CUSTOM_ROS2_SERVICE_TYPES(UWVM_CUSTOM_SERVICE_ADVERTISE)
#undef UWVM_CUSTOM_SERVICE_ADVERTISE
            return {95,{}};
        }
        ipc::call_result respond(ros::capability_spec const& cap,std::vector<std::uint8_t> const& bytes,std::uint32_t timeout)
        {
            if(!known(cap)||cap.operation!="service.respond") {return {13,{}};}
            auto until{ipc::after(static_cast<std::uint32_t>(std::min<std::uint64_t>(timeout,limit(cap,"timeout-ms"))))};
            for(auto const& item:servers_) {if(item.second.cap.resource==cap.resource)
            {
                if(!ros2_qos::service(*item.second.entity,ros2_qos::configured(cap))) {return {95,{}};}
                return item.second.state->answer(cap,bytes,until);
            }}return {9,{}};
        }
        void close(std::string const& alias)
        {
            auto item{servers_.find(alias)};if(item==servers_.end()) {return;}
            item->second.state->deactivate();servers_.erase(item);ros2_compat::notify(node_);
        }
    };
}
