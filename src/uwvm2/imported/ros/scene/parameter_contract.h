/** @file
 * @brief Canonical parameter records with independently bounded ROS value algebras.
 * @details Decoding owns no storage and grants no authority. Every view borrows
 * the complete immutable carrier copy. The selected deployment profile supplies
 * the ROS1 or ROS2 algebra and cumulative parser limits; the guest cannot select
 * their union or widen an allocation budget. Store admission separately checks
 * exact names, descriptors, live node attachment, revision and request ledger.
 */
#pragma once
#include <array>
#include <cstdint>
#include <string_view>

#define UWVM_ROS_PARAMETER_FUNCTIONS(X) \
 X(parameter_open,open,"parameter.open") \
 X(parameter_declare,declare,"parameter.declare") \
 X(parameter_undeclare,undeclare,"parameter.undeclare") \
 X(parameter_get,get,"parameter.get") \
 X(parameter_describe,describe,"parameter.describe") \
 X(parameter_list,list,"parameter.list") \
 X(parameter_set,set,"parameter.set") \
 X(parameter_set_atomic,set_atomic,"parameter.set.atomic") \
 X(parameter_take,take,"parameter.take") \
 X(parameter_respond,respond,"parameter.respond") \
 X(parameter_exists,exists,"parameter.exists") \
 X(parameter_search,search,"parameter.search")

#ifndef UWVM_SCENE_PARAMETER_CONTRACT_MACROS_ONLY
#include "parameter_values.h"
#include "buffers.h"

namespace uwvm2::imported::ros::parameter_contract
{
    namespace wire=scene::wire;
    namespace values=scene::parameter_values;
    inline constexpr std::size_t maximum_batch=128,maximum_name=256,maximum_metadata=512;
    inline constexpr std::array<std::string_view,13> type_names{
        "not_set","boolean","integer","floating","string","bytes","booleans","integers","floats","strings",
        "xmlrpc_array","xmlrpc_struct","xmlrpc_datetime"};
    inline constexpr std::array<values::kind,13> type_kinds{
        values::kind::not_set,values::kind::boolean,values::kind::integer,values::kind::floating,values::kind::string,
        values::kind::bytes,values::kind::booleans,values::kind::integers,values::kind::floats,values::kind::strings,
        values::kind::xmlrpc_array,values::kind::xmlrpc_struct,values::kind::xmlrpc_datetime};
    enum class operation : std::uint8_t {open,declare,undeclare,get,describe,list,set,set_atomic,take,respond,exists,search};
    struct operation_descriptor {operation id;std::string_view name;};
    inline constexpr std::array operations{
#define UWVM_PARAMETER_DESCRIPTOR(symbol,action,name) operation_descriptor{operation::action,name},
        UWVM_ROS_PARAMETER_FUNCTIONS(UWVM_PARAMETER_DESCRIPTOR)
#undef UWVM_PARAMETER_DESCRIPTOR
    };
    struct request_key {std::uint64_t session{},sequence{};friend bool operator==(request_key const&,request_key const&)=default;};
    /** @brief Bounded descriptor metadata; free text is never executable policy. */
    struct descriptor
    {
        scene::buffers::text<maximum_name> name;
        scene::buffers::text<maximum_metadata> description,additional_constraints;
        values::kind declared_type{};
        bool read_only{},dynamic_type{},has_range{},floating_range{};
        values::integer_range integers;
        values::floating_range floating;
    };
    struct item_view {std::string_view name;wire::bytes value;values::scalar scalar;};
    /** @brief One allocation-free request with fixed maxima for all array views. */
    struct request
    {
        request_key key;
        std::uint64_t node_capability{},node{},scope{},revision{},cursor{},depth{},limit{},request_id{};
        bool has_revision{},has_cursor{},accepted{},has_replacement{};
        scene::buffers::profile profile;
        descriptor declaration;
        std::array<item_view,maximum_batch> items{};
        std::array<std::string_view,maximum_batch> names{};
        std::size_t item_count{},name_count{};
        std::string_view reason,start_namespace;
    };
    inline bool read_optional(wire::reader& input,bool& present,std::uint64_t& value) noexcept
    {return input.boolean(present)&&(!present||input.u64(value));}
    inline bool read_name(wire::reader& input,std::string_view& name) noexcept
    {return input.text(name)&&!name.empty()&&name.size()<=maximum_name&&name.find('\0')==name.npos;}
    inline bool read_descriptor(wire::reader& input,descriptor& output) noexcept
    {
        descriptor value;std::string_view text;
        if(!read_name(input,text)||!value.name.assign(text)||!input.text(text))return input.reject();
        auto found=std::find(type_names.begin(),type_names.end(),text);
        if(found==type_names.end())return input.reject();
        value.declared_type=type_kinds[static_cast<std::size_t>(found-type_names.begin())];
        if(!input.text(text)||!value.description.assign(text)||!input.text(text)||!value.additional_constraints.assign(text)||
           !input.boolean(value.read_only)||!input.boolean(value.dynamic_type)||!input.boolean(value.has_range))return input.reject();
        if(value.has_range)
        {
            std::uint64_t tag{};if(!input.u64(tag)||(tag!=1&&tag!=2))return input.reject();
            value.floating_range=tag==2;
            if(value.floating_range)
            {
                if(!input.f64(value.floating.minimum)||!input.f64(value.floating.maximum)||!input.f64(value.floating.step)||
                   !value.floating.valid()||value.declared_type!=values::kind::floating)return input.reject();
            }
            else if(!input.i64(value.integers.minimum)||!input.i64(value.integers.maximum)||!input.u64(value.integers.step)||
                    !value.integers.valid()||value.declared_type!=values::kind::integer)return input.reject();
        }
        output=value;return true;
    }
    inline bool write_descriptor(wire::writer& output,descriptor const& value) noexcept
    {
        auto found=std::find(type_kinds.begin(),type_kinds.end(),value.declared_type);
        if(found==type_kinds.end())return false;
        if(!output.text(value.name.view())||!output.text(type_names[static_cast<std::size_t>(found-type_kinds.begin())])||
           !output.text(value.description.view())||!output.text(value.additional_constraints.view())||
           !output.boolean(value.read_only)||!output.boolean(value.dynamic_type)||!output.boolean(value.has_range))return false;
        if(!value.has_range)return true;
        if(value.floating_range)return output.u64(2)&&output.f64(value.floating.minimum)&&output.f64(value.floating.maximum)&&output.f64(value.floating.step);
        return output.u64(1)&&output.i64(value.integers.minimum)&&output.i64(value.integers.maximum)&&output.u64(value.integers.step);
    }
    /** @brief Decode a complete value while retaining its exact canonical bytes. */
    inline bool read_value(wire::reader& input,wire::bytes source,values::limits const& policy,values::budget& budget,item_view& value) noexcept
    {
        auto begin=input.position();
        if(!values::read(input,policy,budget,value.scalar))return false;
        value.value=source.subspan(begin,input.position()-begin);
        return value.value.size()<=policy.bytes;
    }
    inline bool read_items(wire::reader& input,wire::bytes source,values::limits const& policy,values::budget& budget,request& output) noexcept
    {
        std::uint64_t count{};if(!input.u64(count)||count>maximum_batch)return input.reject();
        output.item_count=static_cast<std::size_t>(count);
        for(std::size_t i{};i!=output.item_count;++i)
        {
            auto& item=output.items[i];if(!read_name(input,item.name)||!read_value(input,source,policy,budget,item))return input.reject();
            for(std::size_t j{};j!=i;++j)if(output.items[j].name==item.name)return input.reject();
        }
        return true;
    }
    inline bool read_names(wire::reader& input,request& output,bool allow_empty=true) noexcept
    {
        std::uint64_t count{};if(!input.u64(count)||(!count&&!allow_empty)||count>maximum_batch)return input.reject();
        output.name_count=static_cast<std::size_t>(count);
        for(std::size_t i{};i!=output.name_count;++i)
        {
            if(!read_name(input,output.names[i]))return input.reject();
            for(std::size_t j{};j!=i;++j)if(output.names[j]==output.names[i])return input.reject();
        }
        return true;
    }
    /** @brief Reject unknown/trailing fields and cumulative value exhaustion.
     * @details Duplicate transaction names are rejected even for non-atomic
     * requests, so each returned decision unambiguously identifies one item.
     * An empty list prefix array means all keys already approved by the profile;
     * it cannot expand the scope. Replacement keys are checked against the
     * pending request again by the store before any commit.
     */
    inline bool decode(operation action,wire::bytes bytes,values::limits const& policy,request& output) noexcept
    {
        if(!values::valid(policy)||bytes.size()>4u*1024u*1024u)return false;
        wire::reader input{bytes};values::budget budget{policy.nodes,policy.elements};request value;
        switch(action)
        {
        case operation::open:
        {
            std::string_view identity;wire::bytes digest;
            if(!input.u64(value.node_capability)||!input.u64(value.node)||!input.text(identity)||identity.empty()||
               !value.profile.identity.assign(identity)||!input.blob(digest)||digest.size()!=32)return false;
            std::copy(digest.begin(),digest.end(),value.profile.sha256.begin());break;
        }
        case operation::declare:
            if(!input.u64(value.key.session)||!input.u64(value.key.sequence)||!input.u64(value.scope)||
               !read_descriptor(input,value.declaration)||!read_value(input,bytes,policy,budget,value.items[0]))return false;
            value.item_count=1;break;
        case operation::undeclare:
            if(!input.u64(value.key.session)||!input.u64(value.key.sequence)||!input.u64(value.scope)||
               !read_name(input,value.names[0])||!read_optional(input,value.has_revision,value.revision))return false;
            value.name_count=1;break;
        case operation::get:case operation::describe:
            if(!input.u64(value.scope)||!read_names(input,value))return false;
            break;
        case operation::exists:case operation::search:
            if(!input.u64(value.scope)||!value.scope||!read_name(input,value.names[0])||
                (action==operation::search&&!read_name(input,value.start_namespace)))return false;
            value.name_count=1;break;
        case operation::list:
            if(!input.u64(value.scope)||!read_names(input,value,true)||!input.u64(value.depth)||value.depth>256||
               !read_optional(input,value.has_cursor,value.cursor)||!input.u64(value.limit)||!value.limit||value.limit>maximum_batch)return false;
            break;
        case operation::set:case operation::set_atomic:
            if(!input.u64(value.key.session)||!input.u64(value.key.sequence)||!input.u64(value.scope)||
               !read_optional(input,value.has_revision,value.revision)||!read_items(input,bytes,policy,budget,value))return false;
            break;
        case operation::take:
            if(!input.u64(value.scope))return false;
            break;
        case operation::respond:
            if(!input.u64(value.scope)||!input.u64(value.request_id)||!input.boolean(value.accepted)||!input.boolean(value.has_replacement)||
               (value.has_replacement&&(!value.accepted||!read_items(input,bytes,policy,budget,value)))||
               !input.text(value.reason)||value.reason.size()>maximum_metadata||value.reason.find('\0')!=value.reason.npos)return false;
            break;
        default:return false;
        }
        if(!input.done())return false;
        output=value;return true;
    }
}
#endif
