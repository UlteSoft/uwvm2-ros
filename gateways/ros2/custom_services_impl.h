/*************************************************************
 * UWVM2 original ROS2 custom-Service shard implementation. *
 * APL-2.0. All native behavior remains the original adapter. *
 *************************************************************/
#pragma once
#ifndef UWVM_ROS2_CUSTOM_SERVICE_SHARD_IMPLEMENTATION
#error "Custom Service typed definitions belong only to generated shard translation units"
#endif
#include "custom_services.h"
#include <tuple>

namespace uwvm2::ros_gateway::ros2_custom_services
{
    template<class Service,class RequestCodec,class ResponseCodec>
    ipc::call_result adapter::invoke(ros::capability_spec const& cap,std::vector<std::uint8_t> const& bytes,std::uint32_t timeout)
        {
            // An unavailable actual-QoS API is known before entity creation.
            // Do not briefly create a client merely to discover that fact.
            if constexpr(!ros2_qos::supports_client_qos<rclcpp::Client<Service>>::value) { return {95,{}}; }
            auto item{ipc::find_custom_service(2,cap.type)};
            if(!item||!ipc::validate_custom_service_request(*item,"service.call",bytes.data(),bytes.size())) {return {22,{}};}
            if(bytes.size()>limit(cap,"max-request-bytes")) {return {90,{}};}
            auto until{ipc::after(static_cast<std::uint32_t>(std::min<std::uint64_t>(timeout,limit(cap,"timeout-ms"))))};
            auto request{std::make_shared<typename Service::Request>()};
            if(!RequestCodec::decode(bytes.data(),bytes.size(),*request)) {return {22,{}};}
            auto configured{ros2_qos::configured(cap)};auto client{ros2_compat::create_client<Service>(node_,cap.resource,configured)};
            if(!client||!ros2_qos::client(*client,configured)) {return {95,{}};}
            auto remaining{until-std::chrono::steady_clock::now()};
            if(remaining<=decltype(remaining)::zero()||!client->wait_for_service(remaining)||std::chrono::steady_clock::now()>=until) {return {110,{}};}
            auto pending{client->async_send_request(request)};
            // A timed-out request is removed from this ephemeral client's map.
            // This does not resend/cancel an already admitted native operation.
            using client_pointer=decltype(client);using pending_future=decltype(pending);
            struct pending_guard {client_pointer& selected_client;pending_future& selected_pending;~pending_guard(){try{ros2_compat::remove_pending(*selected_client,selected_pending);}catch(std::exception const&) {}}} guard{client,pending};
            if(pending.wait_until(until)!=std::future_status::ready||std::chrono::steady_clock::now()>=until) {return {110,{}};}
            auto response{pending.get()};std::vector<std::uint8_t> out;
            size_t response_size{};if(!response||!ResponseCodec::encoded_size(*response,response_size)) {return {22,{}};}if(response_size>limit(cap,"max-response-bytes")) {return {90,{}};}
            if(!ResponseCodec::encode(*response,out)||
                !ipc::validate_custom_service_output(*item,"service.call",out.data(),out.size())) {return {22,{}};}
            if(out.size()>limit(cap,"max-response-bytes")) {return {90,{}};}
            return std::chrono::steady_clock::now()>=until?ipc::call_result{110,{}}:ipc::call_result{0,std::move(out)};
        }
    template<class Service,class RequestCodec,class ResponseCodec>
    ipc::call_result adapter::serve(ros::capability_spec const& cap,std::shared_ptr<event_queue> events)
        {
#if !UWVM_ROS2_HAVE_DEFERRED_SERVICE && !UWVM_ROS2_HAVE_LEGACY_DEFERRED_SERVICE
            // Refuse before constructing any native endpoint. A synchronous
            // service callback must never invent an automatic empty response.
            (void)cap;(void)events;return {95,{}};
#else
            if constexpr(!ros2_qos::supports_service_qos<rclcpp::Service<Service>>::value) { return {95,{}}; }
            if(!events||events->capability.alias!=cap.alias||events->capability.resource!=cap.resource||events->capability.type!=cap.type||
                events->capability.type_identity!=cap.type_identity||events->capability.limits!=cap.limits) {return {13,{}};}
            for(auto const& item:servers_) {if(item.second.cap.resource==cap.resource) {return {17,{}};}}
            auto state{std::make_shared<service_state>(cap,events)};std::weak_ptr<service_state> weak{state};
            auto verified{std::make_shared<std::atomic<bool>>(false)};
            auto callback{[weak,verified](std::shared_ptr<rclcpp::Service<Service>> service,std::shared_ptr<rmw_request_id_t> header,
                                         std::shared_ptr<typename Service::Request> request)
            {
                if(!verified->load(std::memory_order_acquire)||!request||!RequestCodec::valid(*request)) {return;}
                auto state{weak.lock()};size_t request_size{};if(!state||!RequestCodec::encoded_size(*request,request_size)||request_size>limit(state->capability,"max-request-bytes")) {return;}std::vector<std::uint8_t> bytes;
                if(!RequestCodec::encode(*request,bytes)||bytes.size()>limit(state->capability,"max-request-bytes")) {return;}
                state->begin(std::move(bytes),[service,header](std::vector<std::uint8_t> const& response,ipc::deadline until)->std::uint32_t
                {
                    typename Service::Response native{};
                    if(!ResponseCodec::decode(response.data(),response.size(),native)) {return 22;}
                    if(std::chrono::steady_clock::now()>=until) {return 110;}
                    service->send_response(*header,native);return std::chrono::steady_clock::now()>=until?110:0;
                });
            }};
            auto configured{ros2_qos::configured(cap)};auto entity{ros2_compat::create_deferred_service<Service>(node_,cap.resource,std::move(callback),configured)};
            if(!entity||!ros2_qos::service(*entity,configured)) {state->deactivate();return {95,{}};}
            servers_.emplace(cap.alias,endpoint{cap,state,std::move(entity)});verified->store(true,std::memory_order_release);return {};
#endif
        }
    namespace detail
    {
        // Full Native/Request/Response aliases exist only in shard TUs. Every
        // tuple ordinal is derived from the complete original trusted table.
        template<class Native,class Request,class Response> struct service_row
        { using native_type=Native;using request_codec=Request;using response_codec=Response; };
        struct service_end {};
#define UWVM_CUSTOM_SERVICE_ROW(Native,Type,Identity,Canonical,Request,Response) service_row<Native,Request,Response>,
        using service_rows=std::tuple<UWVM_ROS_CUSTOM_ROS2_SERVICE_TYPES(UWVM_CUSTOM_SERVICE_ROW) service_end>;
#undef UWVM_CUSTOM_SERVICE_ROW
        struct service_literals {std::string_view type,identity,canonical;};
#define UWVM_CUSTOM_SERVICE_LITERALS(Native,Type,Identity,Canonical,Request,Response) service_literals{Type,Identity,Canonical},
        inline constexpr service_literals declared_services[]{
            UWVM_ROS_CUSTOM_ROS2_SERVICE_TYPES(UWVM_CUSTOM_SERVICE_LITERALS) {}};
#undef UWVM_CUSTOM_SERVICE_LITERALS
        static_assert(std::tuple_size<service_rows>::value==service_count+1,"complete original Service row count differs");
        static_assert(sizeof(declared_services)/sizeof(declared_services[0])==service_count+1,"original Service literals differ");

        template<std::size_t Shard,std::size_t Shards>
        template<std::size_t Index>
        bool service_shard<Shard,Shards>::known_at(std::size_t type_index,ros::capability_spec const& cap)
        {
            if constexpr(Index<service_count)
            {
                if(type_index==Index)
                {
                    using row=std::tuple_element_t<Index,service_rows>;
                    auto const& declared{declared_services[Index]};
                    return binding<typename row::native_type,typename row::request_codec,typename row::response_codec>::known(
                        cap,declared.type,declared.identity,declared.canonical);
                }
                return known_at<Index+Shards>(type_index,cap);
            }
            else {return false;}
        }
        template<std::size_t Shard,std::size_t Shards>
        bool service_shard<Shard,Shards>::known(std::size_t type_index,ros::capability_spec const& cap)
        {
            static_assert(Shards==service_shards&&Shard<Shards,"original Service shard selection differs");
            return known_at<Shard>(type_index,cap);
        }
        template<std::size_t Shard,std::size_t Shards>
        template<std::size_t Index>
        ipc::call_result service_shard<Shard,Shards>::call_at(adapter& selected,std::size_t type_index,
            ros::capability_spec const& cap,std::vector<std::uint8_t> const& bytes,std::uint32_t timeout)
        {
            if constexpr(Index<service_count)
            {
                if(type_index==Index)
                {
                    using row=std::tuple_element_t<Index,service_rows>;
                    return selected.invoke<typename row::native_type,typename row::request_codec,typename row::response_codec>(cap,bytes,timeout);
                }
                return call_at<Index+Shards>(selected,type_index,cap,bytes,timeout);
            }
            else {return {95,{}};}
        }
        template<std::size_t Shard,std::size_t Shards>
        ipc::call_result service_shard<Shard,Shards>::call(adapter& selected,std::size_t type_index,
            ros::capability_spec const& cap,std::vector<std::uint8_t> const& bytes,std::uint32_t timeout)
        {
            static_assert(Shards==service_shards&&Shard<Shards,"original Service shard selection differs");
            return call_at<Shard>(selected,type_index,cap,bytes,timeout);
        }
        template<std::size_t Shard,std::size_t Shards>
        template<std::size_t Index>
        ipc::call_result service_shard<Shard,Shards>::advertise_at(adapter& selected,std::size_t type_index,
            ros::capability_spec const& cap,std::shared_ptr<event_queue> events)
        {
            if constexpr(Index<service_count)
            {
                if(type_index==Index)
                {
                    using row=std::tuple_element_t<Index,service_rows>;
                    return selected.serve<typename row::native_type,typename row::request_codec,typename row::response_codec>(cap,std::move(events));
                }
                return advertise_at<Index+Shards>(selected,type_index,cap,std::move(events));
            }
            else {return {95,{}};}
        }
        template<std::size_t Shard,std::size_t Shards>
        ipc::call_result service_shard<Shard,Shards>::advertise(adapter& selected,std::size_t type_index,
            ros::capability_spec const& cap,std::shared_ptr<event_queue> events)
        {
            static_assert(Shards==service_shards&&Shard<Shards,"original Service shard selection differs");
            return advertise_at<Shard>(selected,type_index,cap,std::move(events));
        }
    }
}
