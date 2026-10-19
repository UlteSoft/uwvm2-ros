/** @file
 * @brief Explicit pre-RIHS Message admission from a complete installed IDL proof.
 *
 * Trusted generated specializations pair captured original installed IDL with
 * actual C++ types, constructor defaults/constants, typed offsets and genuine
 * introspection. A null introspection default pointer proves nothing. Modern
 * hash-capable SDKs never enter this path, including with null/broken hashes.
 *
 * This header neither reads IDL nor authenticates a build. The independent owner
 * must retain the physical SDK/source/MD/link and actual process evidence which
 * pairs the generated proof with its original installed inputs. Source digests
 * alone are not that evidence. No Node, entity, registry or authority is created.
 * Message-only: Service/Action protocol admission remains a separate contract.
 */
#pragma once
#include "identity.h"
#include <array>
#include <cstddef>
#include <cstring>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>
#if __has_include(<rosidl_runtime_cpp/bounded_vector.hpp>)
#include <rosidl_runtime_cpp/bounded_vector.hpp>
#elif __has_include(<rosidl_generator_cpp/bounded_vector.hpp>)
#include <rosidl_generator_cpp/bounded_vector.hpp>
#endif

namespace uwvm2::ros_gateway::ros2_compat::installed_source
{
    namespace ipc=::uwvm2::imported::ros::ipc;

    /** Only a statically selected, independently admitted native provider can
     * specialize this trait. Runtime/Guest/catalog fields cannot enable it. */
    template<class Native> struct traits
    {
        static constexpr bool enabled=false;
        inline static constexpr std::string_view type{},source_proof_sha256{};
        static constexpr ipc::custom_native_schema schema() noexcept {return {};}
        static bool prove() {return false;}
    };
    template<class T> struct shape
    {using element=void;static constexpr unsigned kind=0;static constexpr std::size_t bound=0;};
    template<class T,std::size_t N> struct shape<std::array<T,N>>
    {using element=T;static constexpr unsigned kind=1;static constexpr std::size_t bound=N;};
    template<class T,class A> struct shape<std::vector<T,A>>
    {using element=T;static constexpr unsigned kind=2;static constexpr std::size_t bound=0;};
#if __has_include(<rosidl_runtime_cpp/bounded_vector.hpp>)
    template<class T,std::size_t N,class A> struct shape<::rosidl_runtime_cpp::BoundedVector<T,N,A>>
    {using element=T;static constexpr unsigned kind=2;static constexpr std::size_t bound=N;};
#elif __has_include(<rosidl_generator_cpp/bounded_vector.hpp>)
    template<class T,std::size_t N,class A> struct shape<::rosidl_generator_cpp::BoundedVector<T,N,A>>
    {using element=T;static constexpr unsigned kind=2;static constexpr std::size_t bound=N;};
#endif
    template<class Actual,class Element,unsigned Kind,std::size_t Bound> constexpr bool field_type()
    {
        if constexpr(Kind==0)return std::is_same_v<Actual,Element>;
        else return shape<Actual>::kind==Kind&&shape<Actual>::bound==Bound&&
            std::is_same_v<typename shape<Actual>::element,Element>;
    }
    template<class T> bool same_value(T const& actual,T const& expected)
    {
        if constexpr(std::is_floating_point_v<T>)return std::memcmp(&actual,&expected,sizeof(T))==0;
        else return actual==expected;
    }
    inline bool digest(std::string_view value) noexcept
    {
        if(value.size()!=64)return false;
        for(char c:value)if(!((c>='0'&&c<='9')||(c>='a'&&c<='f')))return false;
        return true;
    }
    inline bool same_field(ipc::custom_native_field const& a,ipc::custom_native_field const& b) noexcept
    {
        return a.name==b.name&&a.type==b.type&&a.array_size==b.array_size&&a.is_array==b.is_array&&
            a.string_bound==b.string_bound&&a.is_sequence==b.is_sequence&&a.sequence_bound==b.sequence_bound&&
            a.native_default==b.native_default&&a.has_declared_default==b.has_declared_default;
    }
    inline bool same_schema(ipc::custom_native_schema a,ipc::custom_native_schema b) noexcept
    {
        if(!a.records||!b.records||!a.record_count||a.record_count>256||a.record_count!=b.record_count)return false;
        for(std::size_t i{};i<a.record_count;++i)
        {
            auto const& x=a.records[i];if(x.field_count>4096||(x.field_count&&!x.fields))return false;
            for(std::size_t j{};j<i;++j)if(a.records[j].type==x.type)return false;
            std::size_t count{};
            for(std::size_t j{};j<b.record_count;++j)if(b.records[j].type==x.type)
            {
                ++count;auto const& y=b.records[j];
                if(y.field_count!=x.field_count||(y.field_count&&!y.fields))return false;
                for(std::size_t k{};k<x.field_count;++k)if(!same_field(x.fields[k],y.fields[k]))return false;
            }
            if(count!=1)return false;
        }
        return true;
    }
#if UWVM_ROS2_HAVE_INTROSPECTION_CPP
    struct field_layout {std::size_t offset{},bytes{};};
    struct witness
    {
        std::string_view type{},installed_idl_sha256{};
        message_members const* (*members)(){};
        bool (*typed)(){};
        std::size_t native_size{};
        field_layout const* layout{};
        std::size_t fields{};
    };
    inline bool records(ipc::custom_native_schema schema,witness const* all,std::size_t count)
    {
        if(!schema.records||!schema.record_count||schema.record_count>256||!all||!count||count>256)return false;
        for(std::size_t i{};i<count;++i)
            for(std::size_t j{};j<i;++j)if(all[i].type==all[j].type)return false;
        for(std::size_t i{};i<schema.record_count;++i)
        {
            auto const& expected=schema.records[i];std::size_t found{};
            if(expected.field_count>4096||(expected.field_count&&!expected.fields))return false;
            for(std::size_t j{};j<i;++j)if(schema.records[j].type==expected.type)return false;
            for(std::size_t j{};j<count;++j)if(all[j].type==expected.type)
            {
                ++found;auto const& w=all[j];
                if(!digest(w.installed_idl_sha256)||!w.members||!w.typed||!w.typed()||
                    w.fields!=expected.field_count||(w.fields&&!w.layout))return false;
                auto actual=w.members();
                if(!actual||!metadata_name(*actual,expected.type)||actual->size_of_!=w.native_size||
                    !actual->init_function||!actual->fini_function)return false;
                if(actual->member_count_==0&&expected.field_count==1)
                {
                    auto const& f=expected.fields[0];auto const& l=w.layout[0];
                    if(f.name=="structure_needs_at_least_one_member"&&f.type=="uint8"&&
                        !f.array_size&&!f.is_array&&!f.string_bound&&!f.is_sequence&&!f.sequence_bound&&
                        f.native_default.empty()&&!f.has_declared_default&&l.offset==0&&l.bytes==1&&w.native_size==1)continue;
                }
                if(actual->member_count_!=expected.field_count||(expected.field_count&&!actual->members_))return false;
                for(std::size_t k{};k<expected.field_count;++k)
                {
                    auto const& f=expected.fields[k];auto const& m=actual->members_[k];auto const& l=w.layout[k];
                    if((f.is_array&&f.is_sequence)||!native_string(m.name_,f.name)||m.offset_!=l.offset||
                        !l.bytes||l.offset>w.native_size||l.bytes>w.native_size-l.offset||
                        m.is_array_!=(f.is_array||f.is_sequence)||m.string_upper_bound_!=f.string_bound||
                        m.array_size_!=(f.is_array?f.array_size:f.is_sequence?f.sequence_bound:0)||
                        m.is_upper_bound_!=(f.is_sequence&&f.sequence_bound!=0))return false;
                    auto scalar=scalar_type(f.type);
                    if(scalar) {if(m.type_id_!=scalar||m.members_)return false;}
                    else
                    {
                        if(m.type_id_!=::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE||!m.members_||
                            !m.members_->typesupport_identifier||std::strcmp(m.members_->typesupport_identifier,"rosidl_typesupport_introspection_cpp")||
                            !m.members_->data)return false;
                        std::size_t nested{};
                        for(std::size_t n{};n<count;++n)if(all[n].type==f.type)
                        {++nested;if(!all[n].members||all[n].members()!=m.members_->data)return false;}
                        if(nested!=1)return false;
                    }
                    if((f.is_array||f.is_sequence)&&!m.size_function)return false;
                    // m.default_value_ is deliberately not dereferenced or
                    // treated as evidence of declaration presence/absence.
                }
            }
            if(found!=1)return false;
        }
        return true;
    }
#else
    struct witness {};
    inline bool records(ipc::custom_native_schema,witness const*,std::size_t){return false;}
#endif
    template<class Native> std::string identity()
    {
        using support=std::remove_pointer_t<decltype(::rosidl_typesupport_cpp::get_message_type_support_handle<Native>())>;
        if constexpr(has_baked_hash<support>::value||!traits<Native>::enabled)return {};
        else
        {
            if(!digest(traits<Native>::source_proof_sha256)||!traits<Native>::prove())return {};
            return rihs_identity(traits<Native>::schema(),traits<Native>::type);
        }
    }
    template<class Native> bool qualify(ipc::custom_native_schema supplied,std::string_view root,std::string_view expected)
    {
        using support=std::remove_pointer_t<decltype(::rosidl_typesupport_cpp::get_message_type_support_handle<Native>())>;
        if constexpr(has_baked_hash<support>::value||!traits<Native>::enabled)return false;
        else return root==traits<Native>::type&&digest(traits<Native>::source_proof_sha256)&&
            same_schema(supplied,traits<Native>::schema())&&traits<Native>::prove()&&matches_rihs(supplied,root,expected);
    }
}
