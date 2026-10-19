/** @file
 * Session-owned actual publisher routing. Every receipt refers to the same
 * retained native endpoint used for inspection, publication and retirement.
 * The owning authenticated server admits the complete policy and session before
 * calling this registry. Nothing here opens, discovers or grants a capability.
 */
#pragma once
#include "compat/node.h"
#include "native_rmw_builtin.h"
#include <uwvm2/imported/ros/common/rmw_endpoint_history.h>
#include <uwvm2/imported/ros/ipc/rmw_publish.h>
#include <functional>
#include <utility>

namespace uwvm2::ros_gateway::native_rmw_publisher
{
    inline std::uint32_t create_selected(::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr const& node,ros::capability_spec const& cap,
        wire::binding const& owner,std::uint64_t incarnation,std::shared_ptr<endpoint>& output,wire::packet& actual)
    {
        if(cap.type==native_rmw_builtin::string_codec::type){return native_rmw_builtin::publisher<native_rmw_builtin::string_codec>::create(node,cap,owner,incarnation,output,actual);}
        if(cap.type==native_rmw_builtin::bool_codec::type){return native_rmw_builtin::publisher<native_rmw_builtin::bool_codec>::create(node,cap,owner,incarnation,output,actual);}
        if(cap.type==native_rmw_builtin::twist_codec::type){return native_rmw_builtin::publisher<native_rmw_builtin::twist_codec>::create(node,cap,owner,incarnation,output,actual);}
        if(cap.type==native_rmw_builtin::image_codec::type){return native_rmw_builtin::publisher<native_rmw_builtin::image_codec>::create(node,cap,owner,incarnation,output,actual);}
        // The trusted full custom macro table is partitioned by the existing
        // maintained shard configuration. No Guest row or native type erasure
        // can instantiate a provider or choose a different generated Message.
        return ros2_custom_topics::create_owned_rmw_publisher(node,cap,owner,incarnation,output,actual);
    }
    /** Host-selected cold creation only. Captures may retain a complete
     * immutable native profile. The selected callable validates the actual type,
     * identity and QoS against the explicit grant, keeps partial native state in
     * local RAII, and commits output only after the whole receipt is valid.
     * Guest code cannot provide a callable or obtain authority through it.
     * Publication continues directly through the retained endpoint.
     */
    using factory=std::function<std::uint32_t(::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr const&,ros::capability_spec const&,
        wire::binding const&,std::uint64_t,std::shared_ptr<endpoint>&,wire::packet&)>;
    class registry
    {
        using history=::uwvm2::imported::ros::rmw_endpoint_history;
        ::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr node_;
        factory create_;
        history history_;
        std::map<std::uint64_t,std::shared_ptr<endpoint>> objects_;
        bool sealed_{};
        static ipc::call_result encode(wire::packet const& packet)
        {
            std::array<std::uint8_t,wire::packet_bytes> raw{};
            if(!wire::encode(packet,true,raw)){return {22,{}};}
            return {0,{raw.begin(),raw.end()}};
        }
        std::uint32_t retire_one(std::uint64_t handle) noexcept
        {
            if(!history_.claim_cleanup(handle)){return 0;}
            auto found{objects_.find(handle)};auto row{history_.rows().find(handle)};
            std::uint32_t error{};
            if(found!=objects_.end()&&found->second&&row!=history_.rows().end())
            {
                try{error=found->second->retire(row->second.observed.owner,row->second.observed.incarnation);}
                catch(...){error=5;}
                // Release even after a retirement error. There is one attempt,
                // never a second publication/creation or unbounded retry.
                found->second.reset();
            }
            else{error=107;}
            history_.cleanup_result(handle,error);return error;
        }
    public:
        explicit registry(::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr node,factory selected=&create_selected):node_{std::move(node)},create_{std::move(selected)}{}
        registry(registry const&)=delete;registry& operator=(registry const&)=delete;
        ~registry(){clear();}
        auto const& diagnostics() const noexcept{return history_.rows();}
        ipc::call_result control(ros::capability_spec const& cap,wire::packet const& request,ipc::deadline until)
        {
            if(sealed_){return {125,{}};}
            if(!wire::valid_request(request)||request.kind!=wire::role::publisher||cap.operation!="topic.publish"){return {22,{}};}
            if(std::chrono::steady_clock::now()>=until){return {110,{}};}
            history::entry const* row{};
            if(request.operation==wire::command::activate)
            {
                if(!node_||!create_){return {95,{}};}
                auto maximum{limit(cap,"max-publishers")};
                if(!maximum||maximum>history::maximum_entries){return {22,{}};}
                auto error{history_.reserve(request,cap.alias,static_cast<std::size_t>(maximum),row)};
                if(error){return {error,{}};}
                // Every map/control-block slot exists before native creation.
                // A denied/failed activation stays in the finite history.
                std::map<std::uint64_t,std::shared_ptr<endpoint>>::iterator slot;
                try{slot=objects_.emplace(request.owner.source,nullptr).first;}
                catch(...){history_.abandon(request.owner.source,12);return {12,{}};}
                auto incarnation{history_.reserve_native_incarnation()};
                if(!incarnation){history_.abandon(request.owner.source,90);return {90,{}};}
                wire::packet actual{};
                try{error=create_(node_,cap,request.owner,incarnation,slot->second,actual);}
                catch(std::bad_alloc const&){error=12;}catch(...){error=5;}
                if(error)
                {
                    history_.abandon(request.owner.source,error);slot->second.reset();return {error,{}};
                }
                if(actual.incarnation!=incarnation||!slot->second){history_.abandon(request.owner.source,13);slot->second.reset();return {13,{}};}
                error=history_.activate(actual);
                if(error){history_.abandon(request.owner.source,error);slot->second.reset();return {error,{}};}
                if(std::chrono::steady_clock::now()>=until)
                {history_.abandon(request.owner.source,110);(void)retire_one(request.owner.source);return {110,{}};}
                try{return encode(actual);}
                catch(...){history_.abandon(request.owner.source,12);(void)retire_one(request.owner.source);return {12,{}};}
            }
            auto error{history_.find(request.owner,request.incarnation,request.kind,row)};
            if(error){return {error,{}};}if(row->alias!=cap.alias){return {13,{}};}
            auto object{objects_.find(request.owner.source)};
            if(object==objects_.end()||!object->second){return {125,{}};}
            if(request.operation==wire::command::inspect)
            {
                wire::packet actual{};error=object->second->inspect(request.owner,request.incarnation,actual);
                if(error){return {error,{}};}error=history_.inspect(actual);if(error){return {error,{}};}
                if(std::chrono::steady_clock::now()>=until){return {110,{}};}
                return encode(actual);
            }
            error=history_.retire(request.owner,request.incarnation,request.kind);if(error){return {error,{}};}
            error=retire_one(request.owner.source);if(error){return {error,{}};}
            return encode(request);
        }
        ipc::call_result publish(ros::capability_spec const& cap,ipc::rmw_publish::header const& envelope,
            std::uint8_t const* bytes,std::size_t count,ipc::deadline until)
        {
            if(sealed_){return {125,{}};}
            if(cap.operation!="topic.publish"||envelope.payload_bytes!=count){return {22,{}};}
            history::entry const* row{};auto error{history_.find(envelope.owner,envelope.incarnation,wire::role::publisher,row)};
            if(error){return {error,{}};}if(row->alias!=cap.alias){return {13,{}};}
            auto found{objects_.find(envelope.owner.source)};
            if(found==objects_.end()||!found->second){return {125,{}};}
            if(std::chrono::steady_clock::now()>=until){return {110,{}};}
            error=found->second->publish(envelope.owner,envelope.incarnation,bytes,count);
            if(!error&&std::chrono::steady_clock::now()>=until){return {110,{}};}
            return {error,{}};
        }
        void close_alias(std::string_view alias,std::uint32_t reason=125) noexcept
        {
            for(auto const& item:history_.rows()){if(item.second.alias==alias){history_.abandon(item.first,reason);}}
            for(auto const& item:history_.rows()){if(item.second.alias==alias){(void)retire_one(item.first);}}
        }
        void clear() noexcept
        {
            if(sealed_){return;}sealed_=true;history_.seal();
            for(auto const& item:history_.rows()){(void)retire_one(item.first);}
            objects_.clear();
        }
    };
}
