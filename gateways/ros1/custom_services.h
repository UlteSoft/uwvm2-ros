/*************************************************************
 * UWVM2 concrete bounded ROS1 Service adapter. APL-2.0.     *
 *************************************************************/
#pragma once
#include "custom_topics.h"
#include "bounded_service.h"
#include "../common/service.h"
#include <ros/service_traits.h>

#ifndef UWVM_ROS_CUSTOM_ROS1_SERVICE_TYPES
#define UWVM_ROS_CUSTOM_ROS1_SERVICE_TYPES(M)
#endif

namespace uwvm2::ros_gateway::ros1_custom_services
{
    /** Official Request/Response MD5 traits are authoritative, including the
     * ROS1 convention that service components can share the whole Service MD5.
     * The generator supplies actual exported traits; no hypothetical standalone
     * .msg MD5 is derived from a .srv component. Definition and DataType are
     * independently compared against the complete generated component schema. */
    template<class Service,class RequestCodec,class ResponseCodec> struct binding
    {
        static_assert(std::is_same<typename Service::Request,typename RequestCodec::Native>::value&&
            std::is_same<typename Service::Response,typename ResponseCodec::Native>::value,
            "custom service codecs must cover the official request and response types");
        static bool known(ros::capability_spec const& cap,std::string_view type,std::string_view identity,std::string_view canonical)
        {
            if(cap.operation!="service.call"&&cap.operation!="service.advertise"&&cap.operation!="service.respond") {return false;}
            auto item{ipc::find_custom_service(1,type)};auto selected{cap.limits.find("adapter-codec-identity")};
            return item&&cap.type==type&&cap.type_identity==identity&&item->native_identity==identity&&item->codec_identity==canonical&&
                selected!=cap.limits.end()&&selected->second==canonical&&
                std::string_view{::ros::service_traits::MD5Sum<Service>::value()}==identity&&
                std::string_view{::ros::service_traits::DataType<Service>::value()}==type&&
                item->request->type==RequestCodec::type&&item->request->native_identity==RequestCodec::native_identity&&
                item->request->codec_identity==RequestCodec::codec_identity&&item->request->wire_bytes==RequestCodec::wire_bytes&&
                item->response->type==ResponseCodec::type&&item->response->native_identity==ResponseCodec::native_identity&&
                item->response->codec_identity==ResponseCodec::codec_identity&&item->response->wire_bytes==ResponseCodec::wire_bytes&&
                item->request->complete_vendor==ipc::complete_vendor_codec<RequestCodec>::value&&
                item->response->complete_vendor==ipc::complete_vendor_codec<ResponseCodec>::value&&
                ipc::complete_vendor_codec<RequestCodec>::value==ipc::complete_vendor_codec<ResponseCodec>::value&&
                ros1_custom::binding<typename Service::Request,RequestCodec>::native_contract()&&
                ros1_custom::binding<typename Service::Response,ResponseCodec>::native_contract();
        }
    };
    inline bool known(ros::capability_spec const& cap)
    {
        unsigned matches{};bool accepted{};
#define UWVM_CUSTOM_SERVICE_KNOWN(Native,Type,Identity,Canonical,Request,Response) \
        if(cap.type==Type){++matches;accepted=binding<Native,Request,Response>::known(cap,Type,Identity,Canonical);}
        UWVM_ROS_CUSTOM_ROS1_SERVICE_TYPES(UWVM_CUSTOM_SERVICE_KNOWN)
#undef UWVM_CUSTOM_SERVICE_KNOWN
        return matches==1&&accepted;
    }
    /** This session owns only explicitly advertised endpoints. Calling a
     * Service uses the shared deadline-bounded TCPROS transport; official ROS1
     * serialization is validated before allocation and then converted back to
     * canonical bytes. Advertise and respond require independent aliases. */
    class adapter
    {
        ::ros::NodeHandle& node_;
        struct endpoint {ros::capability_spec cap;std::shared_ptr<service_state> state;::ros::ServiceServer entity;};
        std::map<std::string,endpoint> servers_;
        template<class Service,class RequestCodec,class ResponseCodec>
        ipc::call_result invoke(ros::capability_spec const& cap,std::vector<std::uint8_t> const& bytes,std::uint32_t timeout)
        {
            // Conversion, discovery, official serialization and canonical
            // response encoding share one call budget; none starts a fresh
            // timeout after consuming CPU under a slow native/QEMU backend.
            auto until{ipc::after(static_cast<std::uint32_t>(std::min<std::uint64_t>(timeout,limit(cap,"timeout-ms"))))};
            auto item{ipc::find_custom_service(1,cap.type)};
            if(!item||!ipc::validate_custom_service_request(*item,"service.call",bytes.data(),bytes.size())) {return {22,{}};}
            if(bytes.size()>limit(cap,"max-request-bytes")) {return {90,{}};}
            Service native{};if(!RequestCodec::decode(bytes.data(),bytes.size(),native.request)) {return {22,{}};}
            // In this finite profile the ROS1 serialization field structure
            // matches canonical LE scalars/counts. This is a pre-deserialization
            // structural check, never an assertion that every native wire format
            // equals canonical or a shortcut forwarding native response bytes.
            auto remaining{std::chrono::duration_cast<std::chrono::milliseconds>(until-std::chrono::steady_clock::now()).count()};
            if(remaining<=0) {return {110,{}};}
            auto error{ros1_service::call(cap,native,static_cast<std::uint32_t>(remaining),[item](std::vector<std::uint8_t> const& body)
            {return ipc::validate_custom_topic(*item->response,body.data(),body.size());})};
            if(error) {return {error,{}};}std::vector<std::uint8_t> out;
            size_t response_size{};if(!ResponseCodec::encoded_size(native.response,response_size)||response_size>limit(cap,"max-response-bytes")) {return {90,{}};}
            if(!ResponseCodec::encode(native.response,out)||
                !ipc::validate_custom_service_output(*item,"service.call",out.data(),out.size())) {return {22,{}};}
            if(std::chrono::steady_clock::now()>=until) {return {110,{}};}
            return out.size()>limit(cap,"max-response-bytes")?ipc::call_result{90,{}}:ipc::call_result{0,std::move(out)};
        }
        template<class Service,class RequestCodec,class ResponseCodec>
        ipc::call_result serve(ros::capability_spec const& cap,std::shared_ptr<event_queue> events)
        {
            if(!events||events->capability.alias!=cap.alias||events->capability.resource!=cap.resource||events->capability.type!=cap.type||
                events->capability.type_identity!=cap.type_identity||events->capability.limits!=cap.limits) {return {13,{}};}
            for(auto const& item:servers_) {if(item.second.cap.resource==cap.resource) {return {17,{}};}}
            auto state{std::make_shared<service_state>(cap,events)};std::weak_ptr<service_state> weak{state};
            boost::function<bool(typename Service::Request&,typename Service::Response&)> callback{[weak](auto& request,auto& response)
            {
                auto state{weak.lock()};size_t request_size{};if(!state||!RequestCodec::encoded_size(request,request_size)||request_size>limit(state->capability,"max-request-bytes")) {return false;}
                std::vector<std::uint8_t> body;if(!RequestCodec::encode(request,body)||body.size()>limit(state->capability,"max-request-bytes")) {return false;}
                auto ticket{state->begin(std::move(body))};if(!ticket) {return false;}
                std::vector<std::uint8_t> answer;if(!state->wait(ticket,answer)) {return false;}
                typename Service::Response native{};if(!ResponseCodec::decode(answer.data(),answer.size(),native)) {return false;}
                response=std::move(native);return true;
            }};
            auto descriptor{ipc::find_custom_service(1,cap.type)};auto maximum{limit(cap,"max-request-bytes")};
            ::ros::AdvertiseServiceOptions options;options.init<typename Service::Request,typename Service::Response>(cap.resource,callback);
            options.helper=boost::make_shared<ros1_custom_input::service>(options.helper,[descriptor,maximum](std::uint8_t const* bytes,std::size_t size)
            {return descriptor&&size<=maximum&&ipc::validate_custom_topic(*descriptor->request,bytes,size);});
            auto entity{node_.advertiseService(options)};
            if(!entity) {state->deactivate();return {107,{}};}servers_.emplace(cap.alias,endpoint{cap,state,std::move(entity)});return {};
        }
    public:
        explicit adapter(::ros::NodeHandle& node):node_{node} {}
        adapter(adapter const&)=delete;adapter& operator=(adapter const&)=delete;
        ~adapter(){for(auto& item:servers_) {item.second.state->deactivate();}}
        ipc::call_result call(ros::capability_spec const& cap,std::vector<std::uint8_t> const& bytes,std::uint32_t timeout)
        {
            if(!known(cap)||cap.operation!="service.call") {return {13,{}};}
#define UWVM_CUSTOM_SERVICE_CALL(Native,Type,Identity,Canonical,Request,Response) \
            if(cap.type==Type)return invoke<Native,Request,Response>(cap,bytes,timeout);
            UWVM_ROS_CUSTOM_ROS1_SERVICE_TYPES(UWVM_CUSTOM_SERVICE_CALL)
#undef UWVM_CUSTOM_SERVICE_CALL
            return {95,{}};
        }
        ipc::call_result advertise(ros::capability_spec const& cap,std::shared_ptr<event_queue> events)
        {
            if(!known(cap)||cap.operation!="service.advertise") {return {13,{}};}
#define UWVM_CUSTOM_SERVICE_ADVERTISE(Native,Type,Identity,Canonical,Request,Response) \
            if(cap.type==Type)return serve<Native,Request,Response>(cap,std::move(events));
            UWVM_ROS_CUSTOM_ROS1_SERVICE_TYPES(UWVM_CUSTOM_SERVICE_ADVERTISE)
#undef UWVM_CUSTOM_SERVICE_ADVERTISE
            return {95,{}};
        }
        ipc::call_result respond(ros::capability_spec const& cap,std::vector<std::uint8_t> const& bytes,std::uint32_t timeout)
        {
            if(!known(cap)||cap.operation!="service.respond") {return {13,{}};}
            auto until{ipc::after(static_cast<std::uint32_t>(std::min<std::uint64_t>(timeout,limit(cap,"timeout-ms"))))};
            for(auto const& item:servers_) {if(item.second.cap.resource==cap.resource) {return item.second.state->answer(cap,bytes,until);}}
            return {9,{}};
        }
        void close(std::string const& alias)
        {auto item{servers_.find(alias)};if(item!=servers_.end()) {item->second.state->deactivate();servers_.erase(item);}}
    };
}
