// Licensed under APL-2.0 (see LICENSE).
#pragma once
#include <uwvm2/utils/container/standard.h>
#ifndef UWVM_MODULE
# include "snapshot_archive.h"
# include <algorithm>
# include <array>
# include <limits>
# include <memory>
# include <span>
# include <vector>
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::runtime::diagnostic::snapshot_wasip1
{
    namespace archive = ::uwvm2::runtime::diagnostic::snapshot_archive;
    using bytes = ::uwvm2::utils::container::std_vector<::std::byte>;
    // No native handle/address or permission-granting object is serialized.
    // External resource contents and already committed effects are NOT rolled
    // back. Rebinding always needs an independently authorized current resource.
    enum class resource_kind : ::std::uint8_t { external_file, directory, socket, managed_file, stdio };
    struct resource
    {
        resource_kind kind{};
        ::std::uint8_t stdio_index{};
        bool offset_known{}, flags_known{}, follow{};
        ::std::uint16_t flags{}; // WASI flags, never the origin OS's O_* bits.
        ::std::uint64_t offset{};
        bytes mount{}, path{}, content{};
    };
    struct binding
    { ::std::uint32_t descriptor{}, resource_index{}; ::std::uint64_t base{}, inheriting{}; };
    struct reservation
    {
        ::std::uint32_t descriptor{};
        bool null_resource{}, protected_slot{};
        ::std::uint64_t base{}, inheriting{};
    };
    struct environment
    {
        ::std::array<::std::byte,16u> recording{};
        ::std::array<::std::byte,32u> original_wasm{}, builtin_interface{};
        ::std::uint64_t module{}, opens_size{};
        ::uwvm2::utils::container::std_vector<::std::uint64_t> members{};
        ::uwvm2::utils::container::std_vector<bytes> arguments{}, variables{};
        ::uwvm2::utils::container::std_vector<resource> resources{};
        ::uwvm2::utils::container::std_vector<binding> bindings{};
        ::uwvm2::utils::container::std_vector<reservation> reserved{};
        ::uwvm2::utils::container::std_vector<::std::uint32_t> closed{};
        static constexpr bool grants_restore_authority() noexcept { return false; }
    };
    enum class error : unsigned char
    { none, invalid_data, invalid_record, disclosure_denied, unsupported_schema, write_failed, allocation_failed };
    struct result
    {
        error status{error::invalid_data};
        ::std::uint64_t bytes{};
        explicit operator bool() const noexcept { return status==error::none; }
    };
    namespace detail
    {
        inline bool text(bytes const& value) noexcept
        { return ::std::find(value.begin(),value.end(),::std::byte{})==value.end(); }
        inline bool relative_path(bytes const& value) noexcept
        {
            if(value.empty()) { return true; }
            ::std::size_t start{};
            for(::std::size_t i{};i<=value.size();++i)
            {
                if(i<value.size() && value[i]!=::std::byte{'/'})
                {
                    if(value[i]==::std::byte{} || value[i]==::std::byte{'\\'} || value[i]==::std::byte{':'}) { return false; }
                    continue;
                }
                auto const count=i-start;
                if(!count || (count==1u && value[start]==::std::byte{'.'}) ||
                   (count==2u && value[start]==::std::byte{'.'} && value[start+1u]==::std::byte{'.'})) { return false; }
                start=i+1u;
            }
            return true;
        }
        inline constexpr ::std::array<::std::byte,8u> magic{
            ::std::byte{'U'},::std::byte{'W'},::std::byte{'W'},::std::byte{'A'},
            ::std::byte{'S'},::std::byte{'I'},::std::byte{'1'},::std::byte{}};
    }
    [[nodiscard]] inline bool valid(environment const& env)
    {
        constexpr ::std::uint64_t descriptors=::std::uint64_t{INT32_MAX}+1u;
        if(env.opens_size>descriptors || env.resources.size()>descriptors || env.bindings.size()>descriptors ||
           env.reserved.size()>descriptors-env.bindings.size() ||
           env.closed.size()>descriptors-env.bindings.size()-env.reserved.size() || env.closed.size()>env.opens_size ||
           env.members.empty() ||
           ::std::all_of(env.recording.begin(),env.recording.end(),[](::std::byte b){return b==::std::byte{};})) { return false; }
        for(::std::size_t i{1u};i<env.members.size();++i) { if(env.members[i-1u]>=env.members[i]) { return false; } }
        if(!::std::binary_search(env.members.begin(),env.members.end(),env.module)) { return false; }
        for(auto const& t:env.arguments) { if(!detail::text(t)) { return false; } }
        for(auto const& t:env.variables) { if(!detail::text(t)) { return false; } }
        for(auto const& r:env.resources)
        {
            if(static_cast<unsigned>(r.kind)>static_cast<unsigned>(resource_kind::stdio) || r.flags>31u ||
               (!r.flags_known && r.flags) || (!r.offset_known && r.offset) ||
               (r.kind==resource_kind::stdio?r.stdio_index>2u:r.stdio_index!=0u) ||
               (!r.content.empty() && r.kind!=resource_kind::managed_file) ||
               !detail::text(r.mount) || !detail::relative_path(r.path)) { return false; }
            if(r.kind==resource_kind::managed_file && (!r.offset_known || !r.flags_known || !r.mount.empty() || !r.path.empty() || r.follow)) { return false; }
            if((r.kind==resource_kind::socket || r.kind==resource_kind::stdio) &&
               (r.offset_known || !r.mount.empty() || !r.path.empty() || r.follow)) { return false; }
            if(r.kind==resource_kind::directory && (r.offset_known || r.mount.empty())) { return false; }
            if((!r.path.empty() || r.follow) && r.mount.empty()) { return false; }
        }
        ::uwvm2::utils::container::std_vector<::std::uint32_t> numbers{};
        numbers.reserve(env.bindings.size()+env.reserved.size()+env.closed.size());
        ::uwvm2::utils::container::std_vector<bool> used(env.resources.size());
        constexpr ::std::uint64_t rights_mask{0x3fffffffu};
        for(auto const& b:env.bindings)
        {
            if(b.descriptor>INT32_MAX || b.resource_index>=env.resources.size() || ((b.base|b.inheriting)&~rights_mask)) { return false; }
            numbers.push_back(b.descriptor);used[b.resource_index]=true;
        }
        for(auto const& b:env.reserved)
        {
            if(b.descriptor>INT32_MAX || ((b.base|b.inheriting)&~rights_mask) ||
               (b.protected_slot && (b.base || b.inheriting))) { return false; }
            numbers.push_back(b.descriptor);
        }
        for(auto n:env.closed) { if(n>=env.opens_size) { return false; } numbers.push_back(n); }
        for(bool present:used) { if(!present) { return false; } }
        ::std::sort(numbers.begin(),numbers.end());
        for(::std::size_t i{};i<numbers.size();++i)
        { if((i && numbers[i-1u]==numbers[i]) || (i<env.opens_size && numbers[i]!=i)) { return false; } }
        return numbers.size()>=env.opens_size;
    }
    namespace detail
    {
        struct output
        {
            void* context{}; archive::writer write{}; ::std::uint64_t size{}; bool good{true};
            void raw(::std::span<::std::byte const> data) noexcept
            {
                if(!good) { return; }
                if(data.size()>UINT64_MAX-size || (write && !data.empty() && !write(context,data))) { good=false;return; }
                size+=data.size();
            }
            void number(::std::uint64_t value, unsigned width) noexcept
            { ::std::array<::std::byte,8u> data{};for(unsigned i{};i<width;++i)data[i]=static_cast<::std::byte>(value>>(8u*i));raw({data.data(),width}); }
            void blob(bytes const& data) noexcept { number(data.size(),8u);raw(data); }
        };
        inline void emit(environment const& env, output& out) noexcept
        {
            out.raw(magic);out.number(1u,4u);out.number(0u,4u);
            out.raw(env.recording);out.raw(env.original_wasm);out.raw(env.builtin_interface);
            out.number(env.module,8u);out.number(env.opens_size,8u);
            for(auto n:{env.members.size(),env.arguments.size(),env.variables.size(),env.resources.size(),env.bindings.size(),env.reserved.size(),env.closed.size()})out.number(n,8u);
            for(auto n:env.members)out.number(n,8u);
            for(auto const& data:env.arguments)out.blob(data);
            for(auto const& data:env.variables)out.blob(data);
            for(auto const& r:env.resources)
            {
                out.number(static_cast<unsigned>(r.kind),1u);out.number(r.stdio_index,1u);
                out.number(unsigned(r.offset_known)|(unsigned(r.flags_known)<<1u)|(unsigned(r.follow)<<2u),1u);out.number(0u,1u);
                out.number(r.flags,2u);out.number(0u,2u);out.number(r.offset,8u);
                out.blob(r.mount);out.blob(r.path);out.blob(r.content);
            }
            for(auto const& b:env.bindings)
            { out.number(b.descriptor,4u);out.number(b.resource_index,4u);out.number(b.base,8u);out.number(b.inheriting,8u); }
            for(auto const& r:env.reserved)
            { out.number(r.descriptor,4u);out.number(unsigned(r.null_resource)|(unsigned(r.protected_slot)<<1u),1u);out.number(0u,3u);out.number(r.base,8u);out.number(r.inheriting,8u); }
            for(auto n:env.closed)out.number(n,4u);
        }
        struct input
        {
            ::std::span<::std::byte const> data{}; ::std::size_t pos{}; bool good{true};
            ::std::uint64_t number(unsigned width) noexcept
            {
                if(!good || width>data.size()-pos) { good=false;return 0u; }
                ::std::uint64_t value{};for(unsigned i{};i<width;++i)value|=::std::uint64_t{::std::to_integer<unsigned char>(data[pos++])}<<(8u*i);return value;
            }
            bool raw(::std::span<::std::byte> value) noexcept
            {
                if(!good || value.size()>data.size()-pos) { good=false;return false; }
                ::std::copy_n(data.begin()+pos,value.size(),value.begin());pos+=value.size();return true;
            }
            void blob(bytes& value)
            {
                auto const n=number(8u);
                if(!good || n>data.size()-pos || n>value.max_size()) { good=false;return; }
                value.assign(data.begin()+pos,data.begin()+pos+static_cast<::std::size_t>(n));pos+=static_cast<::std::size_t>(n);
            }
        };
    }
    [[nodiscard]] inline result measure(environment const& env) noexcept
    {
        try
        {
            if(!valid(env)) { return {}; }
            detail::output out{};detail::emit(env,out);
            return {out.good?error::none:error::invalid_data,out.size};
        }
        catch(...) { return {error::allocation_failed}; }
    }
    [[nodiscard]] inline result encode(environment const& env,void* context,archive::writer write) noexcept
    {
        auto checked=measure(env);if(!checked)return checked;
        if(!write)return {error::write_failed};
        detail::output out{context,write};detail::emit(env,out);
        return {out.good?error::none:error::write_failed,out.size};
    }
    [[nodiscard]] inline result decode(::std::span<::std::byte const> data,environment& destination) noexcept
    {
        try
        {
            if(data.size()<168u || !::std::equal(detail::magic.begin(),detail::magic.end(),data.begin()))return {};
            detail::input in{data,8u};
            if(in.number(4u)!=1u)return {error::unsupported_schema};
            if(in.number(4u))return {};
            environment env{};in.raw(env.recording);in.raw(env.original_wasm);in.raw(env.builtin_interface);
            env.module=in.number(8u);env.opens_size=in.number(8u);
            ::std::array<::std::size_t,7u> counts{};
            constexpr unsigned widths[]{8u,8u,8u,40u,24u,24u,4u};
            auto remaining=data.size()-168u;
            for(::std::size_t i{};i<counts.size();++i)
            {
                auto const count=in.number(8u);
                // Physical input length bounds ALL count-derived allocations;
                // a forged count cannot allocate before this aggregate check.
                if(!in.good || count>remaining/widths[i])return {};
                counts[i]=static_cast<::std::size_t>(count);remaining-=counts[i]*widths[i];
            }
            env.members.resize(counts[0]);env.arguments.resize(counts[1]);env.variables.resize(counts[2]);
            env.resources.resize(counts[3]);env.bindings.resize(counts[4]);env.reserved.resize(counts[5]);env.closed.resize(counts[6]);
            for(auto& n:env.members)n=in.number(8u);
            for(auto& b:env.arguments)in.blob(b);
            for(auto& b:env.variables)in.blob(b);
            for(auto& r:env.resources)
            {
                r.kind=static_cast<resource_kind>(in.number(1u));r.stdio_index=static_cast<::std::uint8_t>(in.number(1u));
                auto bits=in.number(1u);if(bits>7u || in.number(1u))return {};
                r.offset_known=bits&1u;r.flags_known=bits&2u;r.follow=bits&4u;
                r.flags=static_cast<::std::uint16_t>(in.number(2u));if(in.number(2u))return {};
                r.offset=in.number(8u);in.blob(r.mount);in.blob(r.path);in.blob(r.content);
            }
            for(auto& b:env.bindings)
            { b.descriptor=static_cast<::std::uint32_t>(in.number(4u));b.resource_index=static_cast<::std::uint32_t>(in.number(4u));b.base=in.number(8u);b.inheriting=in.number(8u); }
            for(auto& r:env.reserved)
            {
                r.descriptor=static_cast<::std::uint32_t>(in.number(4u));auto bits=in.number(1u);
                if(bits>3u || in.number(3u))return {};
                r.null_resource=bits&1u;r.protected_slot=bits&2u;r.base=in.number(8u);r.inheriting=in.number(8u);
            }
            for(auto& n:env.closed)n=static_cast<::std::uint32_t>(in.number(4u));
            if(!in.good || in.pos!=data.size() || !valid(env))return {};
            destination=::std::move(env);return {error::none,data.size()};
        }
        catch(...) { return {error::allocation_failed}; }
    }
    class environment_stream
    {
        ::std::shared_ptr<environment const> environment_{}; ::std::uint64_t bytes_{};
        static bool emit(void* context,void* out,archive::writer write) noexcept
        { auto const& self=*static_cast<environment_stream const*>(context);return self.environment_ && static_cast<bool>(encode(*self.environment_,out,write)); }
    public:
        [[nodiscard]] result bind(::std::shared_ptr<environment const> env) noexcept
        {
            if(!env)return {};
            auto checked=measure(*env);if(!checked)return checked;
            environment_=::std::move(env);bytes_=checked.bytes;return checked;
        }
        [[nodiscard]] archive::stream_record record() noexcept
        { return {archive::component::wasip1,archive::completeness::complete,environment_?environment_->module:0u,1u,bytes_,this,emit}; }
    };
    [[nodiscard]] inline result decode(archive::visibility disclosure,archive::record const& record,environment& out) noexcept
    {
        if(disclosure!=archive::visibility::full)return {error::disclosure_denied};
        if(record.kind!=archive::component::wasip1 || record.capture!=archive::completeness::complete)return {error::invalid_record};
        if(record.schema!=1u)return {error::unsupported_schema};
        environment temporary{};auto result=decode(record.payload,temporary);
        if(!result)return result;
        if(record.id!=temporary.module)return {error::invalid_record};
        out=::std::move(temporary);return result;
    }
}
