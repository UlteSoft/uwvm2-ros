/* SPDX-License-Identifier: Apache-2.0 */
#pragma once
#include "compat/node.h"
#ifndef UWVM_CUSTOM_OWNED_SERVICE_SHARD_IMPLEMENTATION
#error "Private typed implementation belongs only to actual service shard TUs"
#endif
#include "custom_owned_service_endpoint.h"
namespace uwvm2::ros_gateway::custom_owned_services::detail
{
    template<std::size_t I>struct proof_cache
    {
        using row=std::tuple_element_t<I,rows>;
        using proof=CompiledCanonicalIdentity<typename row::Native,typename row::Request,typename row::Response>;
        struct result{std::uint32_t error;std::shared_ptr<proof const> value;};
        static result const& get(){static result const original=[] {result out{};out.error=proof::qualify(identities[I],out.value);return out;}();return original;}
    };
    template<std::size_t I,std::size_t Shard,std::size_t Shards>std::uint32_t bounds_at(std::size_t index,ros::capability_spec const& cap,canonical_limits& output)
    {
        if constexpr(I>=count){(void)index;(void)cap;(void)output;return 95;}
        else if constexpr(I%Shards!=Shard)return bounds_at<I+1,Shard,Shards>(index,cap,output);
        else{if(index!=I)return bounds_at<I+1,Shard,Shards>(index,cap,output);
            auto const& qualified=proof_cache<I>::get();if(qualified.error)return qualified.error;if(!qualified.value||!qualified.value->selected(cap))return 13;
            auto root=qualified.value->root();auto row=qualified.value->row();canonical_limits next{root->request->maximum_wire_bytes,root->response->maximum_wire_bytes,row.type,row.native,row.canonical};
            output=next;return 0;}
    }
    template<std::size_t I,std::size_t Shard,std::size_t Shards>std::uint32_t validate_at(std::size_t index,
        ros::capability_spec const& cap,bool response,std::uint8_t const* bytes,std::size_t size)
    {
        if constexpr(I>=count){(void)index;(void)cap;(void)response;(void)bytes;(void)size;return 95;}
        else if constexpr(I%Shards!=Shard)return validate_at<I+1,Shard,Shards>(index,cap,response,bytes,size);
        else{if(index!=I)return validate_at<I+1,Shard,Shards>(index,cap,response,bytes,size);
            auto const& qualified=proof_cache<I>::get();if(qualified.error)return qualified.error;
            if(!qualified.value||!qualified.value->selected(cap))return 13;
            auto root=qualified.value->root();auto part=root?(response?root->response:root->request):nullptr;
            auto const& descriptor=qualified.value->descriptor();auto shape=response?descriptor.response:descriptor.request;
            // The root/component and immutable whole bounded layout were
            // admitted together. Never validate a same-width unrelated record.
            if(!part||!shape||part->maximum_wire_bytes!=shape->wire_bytes)return 13;
            if(size>part->maximum_wire_bytes||(!bytes&&size))return 22;
            // This production recursive wire validator checks every actual
            // field, count, finite scalar, UTF8 byte and exact final extent.
            // It constructs no native object and has no transport side effect.
            return ipc::validate_custom_topic(*shape,bytes,size)?0u:22u;}
    }
    template<std::size_t I,std::size_t Shard,std::size_t Shards>std::uint32_t create_at(::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr const& node,
        std::shared_ptr<uwvm_rmw_cpp::Registry const> const& original,std::size_t index,ros::capability_spec const& cap,
        wire::binding const& owner,std::uint64_t incarnation,std::size_t history,std::shared_ptr<native::report> const& report,
        std::shared_ptr<endpoint>& output,wire::packet& observed)
    {
        if constexpr(I>=count){(void)node;(void)original;(void)index;(void)cap;(void)owner;(void)incarnation;(void)history;(void)report;(void)output;(void)observed;return 95;}
        else if constexpr(I%Shards!=Shard)return create_at<I+1,Shard,Shards>(node,original,index,cap,owner,incarnation,history,report,output,observed);
        else{if(index!=I)return create_at<I+1,Shard,Shards>(node,original,index,cap,owner,incarnation,history,report,output,observed);
            auto const& qualified=proof_cache<I>::get();if(qualified.error)return qualified.error;
            using row=std::tuple_element_t<I,rows>;using Service=typename row::Native;using Request=typename row::Request;using Response=typename row::Response;
            if(cap.operation=="service.call")return typed_endpoint<Service,Request,Response,true>::create(node,original,qualified.value,cap,owner,incarnation,history,report,output,observed);
            if(cap.operation=="service.advertise")return typed_endpoint<Service,Request,Response,false>::create(node,original,qualified.value,cap,owner,incarnation,history,report,output,observed);
            return 13;}
    }
    template<std::size_t Shard,std::size_t Shards>std::uint32_t service_shard<Shard,Shards>::bounds(std::size_t index,ros::capability_spec const& cap,canonical_limits& output)noexcept
    {
        static_assert(Shards==shards&&Shard<Shards,"same actual whole-table service partition");
        try{return bounds_at<0,Shard,Shards>(index,cap,output);}catch(std::bad_alloc const&){return 12;}catch(...){return 13;}
    }
    template<std::size_t Shard,std::size_t Shards>std::uint32_t service_shard<Shard,Shards>::validate(std::size_t index,
        ros::capability_spec const& cap,bool response,std::uint8_t const* bytes,std::size_t size)noexcept
    {
        static_assert(Shards==shards&&Shard<Shards,"same actual whole-table service partition");
        try{return validate_at<0,Shard,Shards>(index,cap,response,bytes,size);}
        catch(std::bad_alloc const&){return 12;}catch(...){return 13;}
    }
    template<std::size_t Shard,std::size_t Shards>std::uint32_t service_shard<Shard,Shards>::create(::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr const& node,
        std::shared_ptr<uwvm_rmw_cpp::Registry const> const& original,std::size_t index,ros::capability_spec const& cap,
        wire::binding const& owner,std::uint64_t incarnation,std::size_t history,std::shared_ptr<native::report> const& report,
        std::shared_ptr<endpoint>& output,wire::packet& observed)noexcept
    {
        static_assert(Shards==shards&&Shard<Shards,"same actual whole-table service partition");
        try{return create_at<0,Shard,Shards>(node,original,index,cap,owner,incarnation,history,report,output,observed);}catch(std::bad_alloc const&){return 12;}catch(...){return 5;}
    }
}
