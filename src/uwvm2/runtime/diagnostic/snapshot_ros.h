// SPDX-License-Identifier: Apache-2.0
#pragma once
#include <uwvm2/utils/container/standard.h>
#ifndef UWVM_MODULE
#include "snapshot_archive.h"
#include <uwvm2/imported/ros/common/checkpoint_environment.h>
#include <algorithm>
#include <memory>
#include <set>
#include <span>
#include <type_traits>
#include <utility>
#endif
#ifndef UWVM_MODULE_EXPORT
#define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::runtime::diagnostic::snapshot_ros
{
    namespace data=::uwvm2::imported::ros::checkpoint::data;
    namespace archive=::uwvm2::runtime::diagnostic::snapshot_archive;
    using environment=data::environment;
    inline constexpr ::std::array<::std::byte,8u> magic{
        ::std::byte{'U'},::std::byte{'W'},::std::byte{'R'},::std::byte{'O'},
        ::std::byte{'S'},::std::byte{'1'},::std::byte{},::std::byte{}};
    namespace detail
    {
        template<typename IO,typename T> void fields(IO& io,T& v)
        {
            using V=::std::remove_cv_t<T>;
            if constexpr(::std::is_same_v<V,data::field>)io(v.name,v.value);
            else if constexpr(::std::is_same_v<V,data::member>)io(v.module,v.name,v.registered,v.retired);
            else if constexpr(::std::is_same_v<V,data::capability>)io(v.alias,v.operation,v.resource,v.type,v.type_identity,
                v.limits,v.revoked,v.accounting,v.fractions,v.audit,v.audit_records);
            else if constexpr(::std::is_same_v<V,data::handle>)io(v.slot,v.generation,v.alias,v.active,v.retired);
            else if constexpr(::std::is_same_v<V,data::event>)io(v.location,v.present,v.request,v.response_credit,v.owned_bytes,
                v.kind,v.flags,v.error,v.correlation,v.payload);
            else if constexpr(::std::is_same_v<V,data::queue>)io(v.state,v.configured,v.active,v.totals,v.events);
            else if constexpr(::std::is_same_v<V,data::observation>)io(v.handle,v.source_handle,v.correlation,v.native_sequence,
                v.delivered,v.observer_alias,v.source_alias,v.retired,v.current_session,v.requests,v.local);
            else if constexpr(::std::is_same_v<V,data::availability>)io(v.handle,v.source_handle,v.availability_alias,v.source_alias,
                v.retired,v.source_retired,v.availability_revoked,v.source_revoked,v.current_session);
            else if constexpr(::std::is_same_v<V,data::group>)io(v.scope,v.name,v.enabled,v.registered,v.alive,v.session_present,
                v.connected,v.availability_sealed,v.timeout,v.next_service_request,v.session_label,v.members,v.connection,
                v.interfaces,v.capabilities,v.handles,v.observations,v.availability_history);
            else if constexpr(::std::is_same_v<V,environment>)io(v.recording,v.backend,v.groups);
            else static_assert(!::std::is_same_v<V,V>,"unknown ROS snapshot field type");
        }
        struct output
        {
            void* context{};archive::writer write{};::std::uint64_t size{};bool good{true};
            void raw(::std::span<::std::byte const> bytes) noexcept
            {
                if(!good)return;
                if(bytes.size()>UINT64_MAX-size || (write && !bytes.empty() && !write(context,bytes))){good=false;return;}
                size+=bytes.size();
            }
            template<typename T> void value(T const& v)
            {
                if constexpr(::std::is_integral_v<T> || ::std::is_same_v<T,::std::byte>)
                {
                    ::std::array<::std::byte,sizeof(T)> b{};
                    auto n=static_cast<::std::uint64_t>(v);
                    for(unsigned i{};i<sizeof(T);++i)b[i]=static_cast<::std::byte>(n>>(8u*i));raw(b);
                }
                else if constexpr(::std::is_same_v<T,::uwvm2::utils::container::std_string>)
                {value(static_cast<::std::uint64_t>(v.size()));raw({reinterpret_cast<::std::byte const*>(v.data()),v.size()});}
                else if constexpr(requires {v.begin();v.end();})
                {
                    if constexpr(requires {v.capacity();})value(static_cast<::std::uint64_t>(v.size()));
                    if constexpr(::std::is_same_v<typename T::value_type,::std::byte>)raw({v.data(),v.size()});
                    else for(auto const& item:v)value(item);
                }
                else fields(*this,v);
            }
            template<typename... T> void operator()(T const&... v){(value(v),...);}
        };
        template<typename T> ::std::size_t minimum()
        {static auto const n=[](){output out{};T value{};out.value(value);return static_cast<::std::size_t>(out.size);}();return n;}
        struct input
        {
            ::std::span<::std::byte const> bytes{};::std::size_t pos{};bool good{true};
            template<typename T> void value(T& v)
            {
                if(!good)return;
                if constexpr(::std::is_integral_v<T> || ::std::is_same_v<T,::std::byte>)
                {
                    if(sizeof(T)>bytes.size()-pos){good=false;return;}
                    ::std::uint64_t n{};
                    for(unsigned i{};i<sizeof(T);++i)n|=::std::uint64_t{::std::to_integer<unsigned char>(bytes[pos++])}<<(8u*i);
                    if constexpr(::std::is_same_v<T,bool>){if(n>1u){good=false;return;}}
                    v=static_cast<T>(n);
                }
                else if constexpr(::std::is_same_v<T,::uwvm2::utils::container::std_string>)
                {
                    ::std::uint64_t n{};value(n);
                    if(!good || n>bytes.size()-pos || n>v.max_size()){good=false;return;}
                    v.assign(reinterpret_cast<char const*>(bytes.data()+pos),static_cast<::std::size_t>(n));pos+=static_cast<::std::size_t>(n);
                    if(v.find('\0')!=v.npos)good=false;
                }
                else if constexpr(requires {v.begin();v.end();})
                {
                    if constexpr(requires {v.capacity();})
                    {
                        ::std::uint64_t n{};value(n);auto width=minimum<typename T::value_type>();
                        if(!good || !width || n>(bytes.size()-pos)/width || n>v.max_size()){good=false;return;}
                        v.resize(static_cast<::std::size_t>(n));
                    }
                    if constexpr(::std::is_same_v<typename T::value_type,::std::byte>)
                    {
                        if(v.size()>bytes.size()-pos){good=false;return;}
                        ::std::copy_n(bytes.data()+pos,v.size(),v.data());pos+=v.size();
                    }
                    else for(auto& item:v){value(item);if(!good)return;}
                }
                else fields(*this,v);
            }
            template<typename... T> void operator()(T&... v){(value(v),...);}
        };
        template<typename Range,typename Key> bool ordered(Range const& rows,Key key)
        {for(::std::size_t i{1};i<rows.size();++i)if(!(key(rows[i-1])<key(rows[i])))return false;return true;}
        inline bool fields_valid(::uwvm2::utils::container::std_vector<data::field> const& rows)
        {
            for(auto const& row:rows)if(row.name.empty() || row.name.find('\0')!=row.name.npos || row.value.find('\0')!=row.value.npos)return false;
            return ordered(rows,[](auto const& r)->auto const&{return r.name;});
        }
    }
    [[nodiscard]] inline bool valid(environment const& e)
    {
        if(e.backend<1 || e.backend>2 || ::std::all_of(e.recording.begin(),e.recording.end(),[](auto b){return b==::std::byte{};}))return false;
        if(!detail::ordered(e.groups,[](auto const& g){return ::std::pair{g.scope,g.name};}))return false;
        ::uwvm2::utils::container::std_set<::std::uint64_t> members;
        for(auto const& g:e.groups)
        {
            if(g.scope>2 || g.members.empty() || (g.scope==1 && g.members.size()!=1) ||
               !detail::ordered(g.members,[](auto const& m){return m.module;}) ||
               !detail::fields_valid(g.connection) || !detail::fields_valid(g.interfaces) ||
               !detail::ordered(g.capabilities,[](auto const& c)->auto const&{return c.alias;}) ||
               !detail::ordered(g.handles,[](auto const& h){return h.slot;}) ||
               !detail::ordered(g.observations,[](auto const& o){return o.handle;}) ||
               !detail::ordered(g.availability_history,[](auto const& a){return a.handle;}))return false;
            if(g.session_present!=g.registered || (!g.session_present && (g.connected || g.next_service_request ||
                g.session_label || !g.handles.empty() || !g.observations.empty() || !g.availability_history.empty())))return false;
            for(auto const& m:g.members)if(m.name.empty() || !members.insert(m.module).second || (m.registered && !g.registered))return false;
            ::uwvm2::utils::container::std_set<::std::string_view> aliases;
            for(auto const& c:g.capabilities)
            {
                if(c.alias.empty() || c.operation.empty() || !detail::fields_valid(c.limits) ||
                   c.accounting[10]>1 || c.accounting[11]>1 || c.accounting[12]>1 || c.audit[14]>1)return false;
                aliases.insert(c.alias);
            }
            for(auto const& i:g.interfaces)if(i.value!="0" && i.value!="1" && i.value!="2")return false;
            for(auto const& h:g.handles)if(!h.slot || !h.generation || (h.active && h.retired) || !aliases.contains(h.alias))return false;
            for(auto const& o:g.observations)
            {
                auto const& q=o.local;
                if(!o.handle || !o.source_handle || !aliases.contains(o.observer_alias) || !aliases.contains(o.source_alias) ||
                   !detail::ordered(o.requests,[](auto n){return n;}) || q.state[5]>4 || q.state[6]>127 ||
                   !q.state[7] || q.state[10]>4)return false;
                unsigned previous{};::std::uint64_t request{};unsigned optional_mask{};
                for(auto const& v:q.events)
                {
                    if(v.location>4 || v.location<previous || (!v.present && v.location!=4))return false;
                    previous=v.location;
                    if(v.location>0 && v.location<4)
                    {auto bit=1u<<v.location;if(optional_mask&bit)return false;optional_mask|=bit;}
                    if(v.location==4){if(!v.request || v.request<=request)return false;request=v.request;}
                    else if(v.request || v.response_credit)return false;
                    if(v.present)
                    {
                        if(v.kind<1 || v.kind>7 || v.owned_bytes<64 || v.payload.size()>v.owned_bytes-64)return false;
                        if(v.kind>=6 && (!v.payload.empty() || (v.flags&~3u)))return false;
                        if(v.kind==6 && !v.error)return false;
                        if(v.kind==7 && !(v.flags&1u))return false;
                    }
                    else if(v.kind || v.flags || v.error || v.correlation || !v.payload.empty())return false;
                }
            }
            for(auto const& a:g.availability_history)if(!a.handle || !a.source_handle ||
                !aliases.contains(a.availability_alias) || !aliases.contains(a.source_alias))return false;
        }
        return true;
    }
    [[nodiscard]] inline bool encode(environment const& e,void* context,archive::writer write,::std::uint64_t& size) noexcept
    {
        try
        {
            if(!valid(e))return false;
            detail::output out{context,write};out.raw(magic);out.value(::std::uint32_t{1u});out.value(::std::uint32_t{});out.value(e);
            if(!out.good)return false;size=out.size;return true;
        }
        catch(...){return false;}
    }
    [[nodiscard]] inline bool decode(::std::span<::std::byte const> bytes,environment& destination) noexcept
    {
        try
        {
            if(bytes.size()<41u || !::std::equal(magic.begin(),magic.end(),bytes.begin()))return false;
            detail::input in{bytes,8u};::std::uint32_t version{},reserved{};in(version,reserved);
            if(version!=1 || reserved)return false;
            environment next{};in.value(next);
            if(!in.good || in.pos!=bytes.size() || !valid(next))return false;
            destination=::std::move(next);return true;
        }
        catch(...){return false;}
    }
    [[nodiscard]] inline bool decode(archive::visibility visibility,archive::record const& record,environment& out) noexcept
    {
        if(visibility!=archive::visibility::full || record.schema!=2 || record.id ||
           record.capture!=archive::completeness::partial ||
           (record.kind!=archive::component::ros1 && record.kind!=archive::component::ros2))return false;
        environment next{};
        if(!decode(record.payload,next) || next.backend!=(record.kind==archive::component::ros1?1:2))return false;
        out=::std::move(next);return true;
    }
    class environment_stream
    {
        ::std::shared_ptr<environment const> owner_{};::std::uint64_t size_{};
        static bool emit(void* opaque,void* output,archive::writer write) noexcept
        {auto const& self=*static_cast<environment_stream const*>(opaque);::std::uint64_t n{};return write && self.owner_ && encode(*self.owner_,output,write,n) && n==self.size_;}
    public:
        bool bind(::std::shared_ptr<environment const> const& owner) noexcept
        {if(!owner || !encode(*owner,nullptr,nullptr,size_))return false;owner_=owner;return true;}
        archive::stream_record record() noexcept
        {return {owner_ && owner_->backend==1?archive::component::ros1:archive::component::ros2,
            archive::completeness::partial,0u,2u,size_,this,emit};}
    };
}
