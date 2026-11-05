/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)           *
 * Copyright (c) 2025-present UlteSoft. All rights reserved.  *
 * Licensed under the APL-2.0 License (see LICENSE file).     *
 *************************************************************/
#pragma once
#ifndef UWVM_MODULE
# include <memory>
# include <limits>
# include <uwvm2/imported/wasi/wasip1/environment/impl.h>
#endif
#include "protocol.h"

#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
namespace uwvm2::utils::update
{
    namespace wasi_policy_environment = ::uwvm2::imported::wasi::wasip1::environment;
    inline constexpr ::std::size_t maximum_wasi_policy_bytes{65536u};
    inline constexpr ::std::size_t maximum_wasi_policy_strings{maximum_wasi_policy_bytes / 8u};
    inline constexpr ::std::size_t maximum_effective_wasi_policy_bytes{1048576u};
    using wasi_policy_string = ::uwvm2::utils::container::u8string;
    using wasi_policy_strings = ::uwvm2::utils::container::vector<wasi_policy_string>;
    using wasi_policy_views = ::uwvm2::utils::container::vector<::uwvm2::utils::container::u8string_view>;
    struct wasi_policy_spec
    {
        bool interface_enabled{};
        wasi_policy_environment::permission_t non_fd_permissions{};
        ::std::uint64_t fd_limit{static_cast<::std::uint64_t>(::std::numeric_limits<::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_t>::max())};
        wasi_policy_environment::resource_limits limits{};
        wasi_policy_strings argv{}, envs{};
    };
    namespace wasi_policy_detail
    {
        inline constexpr ::std::array<unsigned char,8> magic{'U','W','P','O','L','4',0,0};
        [[nodiscard]] inline bool valid_text(::uwvm2::utils::container::u8string_view value) noexcept
        {
            // Strict UTF-8, including rejection of embedded NUL, surrogates,
            // overlong encodings and out-of-range scalar values.
            ::std::size_t i{};
            while(i<value.size())
            {
                auto first{static_cast<unsigned char>(value[i++])};
                if(first==0u)return false;
                if(first<0x80u)continue;
                unsigned count{},point{},minimum{};
                if(first>=0xc2u && first<=0xdfu){count=1;point=first&0x1fu;minimum=0x80u;}
                else if(first>=0xe0u && first<=0xefu){count=2;point=first&0x0fu;minimum=0x800u;}
                else if(first>=0xf0u && first<=0xf4u){count=3;point=first&7u;minimum=0x10000u;}
                else return false;
                if(count>value.size()-i)return false;
                for(unsigned n{};n<count;++n)
                {auto c{static_cast<unsigned char>(value[i++])};if((c&0xc0u)!=0x80u)return false;point=(point<<6u)|(c&0x3fu);}
                if(point<minimum || point>0x10ffffu || (point>=0xd800u && point<=0xdfffu))return false;
            }
            return true;
        }
        template<typename Entries>
        [[nodiscard]] inline bool validate_strings(Entries const& values,::std::uint64_t maximum,::std::uint64_t& total,bool environment) noexcept
        {
            if(values.size()>maximum_wasi_policy_strings)return false;
            for(::std::size_t i{};i<values.size();++i)
            {
                auto const& value{values.index_unchecked(i)};
                if(value.size()>=maximum || total>maximum-value.size()-1u || !valid_text(::uwvm2::utils::container::u8string_view{value.data(),value.size()}))return false;
                total+=value.size()+1u;
                if(environment)
                {
                    ::std::size_t key{};while(key<value.size()&&value[key]!=u8'=')++key;
                    if(key==0u||key==value.size())return false;
                    for(::std::size_t j{};j<i;++j)
                    {
                        auto const& previous{values.index_unchecked(j)};
                        if(previous.size()>key && previous[key]==u8'=' && ::std::memcmp(previous.data(),value.data(),key)==0)return false;
                    }
                }
            }
            return true;
        }
        [[nodiscard]] inline bool valid_spec(wasi_policy_spec const& spec) noexcept
        {
            constexpr ::std::uint64_t permission_mask{((::std::uint64_t{1}<<25u)-1u)&~::std::uint64_t{128}};
            auto const permissions{static_cast<::std::uint32_t>(spec.non_fd_permissions)};
            if((permissions&~permission_mask)!=0u ||
               spec.fd_limit<3u || spec.fd_limit>static_cast<::std::uint64_t>(::std::numeric_limits<::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_t>::max()))return false;
            if(!spec.interface_enabled && (permissions!=0u || !spec.argv.empty() || !spec.envs.empty()))return false;
            for(auto const& limit:spec.limits.operations)
            {
                if(limit.native_wait_exit_action!=wasi_policy_environment::exit_wait_action::cancel &&
                   limit.native_wait_exit_action!=wasi_policy_environment::exit_wait_action::wait)return false;
                for(unsigned i{};i<limit.actions.size();++i)
                {
                    auto action{limit.actions[i]};
                    if(action!=wasi_policy_environment::limit_action::reject && action!=wasi_policy_environment::limit_action::wait)return false;
                    if(action==wasi_policy_environment::limit_action::wait && i!=1u && i!=2u && i!=5u)return false;
                    auto exit_action{limit.exit_actions[i]};
                    if(exit_action!=wasi_policy_environment::exit_wait_action::cancel && exit_action!=wasi_policy_environment::exit_wait_action::wait)return false;
                    if(exit_action==wasi_policy_environment::exit_wait_action::wait && i!=1u && i!=2u && i!=5u)return false;
                }
            }
            for(auto grain:spec.limits.clock_granularity_ns)if(grain==0u)return false;
            ::std::uint64_t total{};
            if(!validate_strings(spec.argv,spec.limits.max_args_env_bytes,total,false) ||
               !validate_strings(spec.envs,spec.limits.max_args_env_bytes,total,true))return false;
            if(!spec.argv.empty() && (permissions&static_cast<::std::uint32_t>(wasi_policy_environment::permission_t::args))==0u)return false;
            if(!spec.envs.empty() && (permissions&static_cast<::std::uint32_t>(wasi_policy_environment::permission_t::environ))==0u)return false;
            return true;
        }
        struct reader
        {
            ::std::span<unsigned char const> bytes{};::std::size_t position{};
            bool number(::std::uint64_t& value) noexcept
            {if(bytes.size()-position<8u)return false;value=0;for(unsigned i{};i<8u;++i)value|=static_cast<::std::uint64_t>(bytes[position++])<<(i*8u);return true;}
            template<typename Action> bool action(Action& value) noexcept
            {if(position==bytes.size()||bytes[position]>1u)return false;value=static_cast<Action>(bytes[position++]);return true;}
            bool strings(wasi_policy_strings& result)
            {
                ::std::uint64_t count{};if(!number(count)||count>maximum_wasi_policy_strings)return false;
                result.reserve(static_cast<::std::size_t>(count));
                for(::std::uint64_t i{};i<count;++i)
                {
                    ::std::uint64_t length{};if(!number(length)||length>bytes.size()-position)return false;
                    result.emplace_back(::uwvm2::utils::container::u8string_view{reinterpret_cast<char8_t const*>(bytes.data()+position),static_cast<::std::size_t>(length)});
                    position+=static_cast<::std::size_t>(length);
                }
                return true;
            }
        };
        [[nodiscard]] inline bool decode(::std::span<unsigned char const> bytes,wasi_policy_spec& spec)
        {
            if(bytes.size()>maximum_wasi_policy_bytes || bytes.size()<magic.size() || !::std::equal(magic.begin(),magic.end(),bytes.begin()))return false;
            reader input{bytes,magic.size()};::std::uint64_t enabled{},permissions{};
            if(!input.number(enabled)||enabled>1u||!input.number(permissions)||permissions>UINT32_MAX||!input.number(spec.fd_limit))return false;
            spec.interface_enabled=enabled!=0u;spec.non_fd_permissions=static_cast<wasi_policy_environment::permission_t>(permissions);
            for(auto& limit:spec.limits.operations)
            {if(!input.number(limit.request_bytes)||!input.number(limit.window_bytes)||!input.number(limit.window_calls)||!input.number(limit.total_bytes)||!input.number(limit.total_calls)||!input.number(limit.concurrent))return false;for(auto& action:limit.actions)if(!input.action(action))return false;for(auto& action:limit.exit_actions)if(!input.action(action))return false;if(!input.action(limit.native_wait_exit_action))return false;}
            for(auto& grain:spec.limits.clock_granularity_ns)if(!input.number(grain))return false;
            if(!input.number(spec.limits.max_poll_subscriptions)||!input.number(spec.limits.max_poll_wait_ns)||!input.number(spec.limits.max_args_env_bytes)||
               !input.number(spec.limits.filesystem_base_rights)||!input.number(spec.limits.filesystem_inheriting_rights)||
               !input.strings(spec.argv)||!input.strings(spec.envs)||input.position!=bytes.size())return false;
            return valid_spec(spec);
        }
        template<typename Left,typename Right>
        [[nodiscard]] inline bool same_strings(Left const& left,Right const& right) noexcept
        {
            if(left.size()!=right.size())return false;
            for(::std::size_t i{};i<left.size();++i)
            {
                auto const& a{left.index_unchecked(i)};auto const& b{right.index_unchecked(i)};
                if(a.size()!=b.size() || ::std::memcmp(a.data(),b.data(),a.size())!=0)return false;
            }
            return true;
        }
        template<wasi_policy_environment::wasip1_memory Memory>
        [[nodiscard]] inline bool tightens(wasi_policy_environment::wasip1_environment<Memory> const& env,wasi_policy_spec const& next) noexcept
        {
            if((env.interface_enabled&&!next.interface_enabled) ||
               (static_cast<::std::uint32_t>(env.non_fd_permissions)&~static_cast<::std::uint32_t>(next.non_fd_permissions))!=0u ||
               next.fd_limit<env.fd_storage.fd_limit || !same_strings(env.argv,next.argv) || !same_strings(env.envs,next.envs))return true;
            for(::std::size_t i{};i<env.limits.operations.size();++i)
            {
                auto const& a{env.limits.operations[i]};auto const& b{next.limits.operations[i]};
                if(b.request_bytes<a.request_bytes||b.window_bytes<a.window_bytes||b.window_calls<a.window_calls||b.total_bytes<a.total_bytes||b.total_calls<a.total_calls||b.concurrent<a.concurrent||a.actions!=b.actions||a.exit_actions!=b.exit_actions||a.native_wait_exit_action!=b.native_wait_exit_action)return true;
            }
            for(::std::size_t i{};i<4u;++i)if(next.limits.clock_granularity_ns[i]>env.limits.clock_granularity_ns[i])return true;
            return next.limits.max_poll_subscriptions<env.limits.max_poll_subscriptions || next.limits.max_poll_wait_ns<env.limits.max_poll_wait_ns ||
                next.limits.max_args_env_bytes<env.limits.max_args_env_bytes;
        }
        template<wasi_policy_environment::wasip1_memory Memory>
        [[nodiscard]] inline status resources(wasi_policy_environment::wasip1_environment<Memory> const& env,wasi_policy_spec const& next) noexcept
        {
            if(next.limits.filesystem_base_rights!=env.limits.filesystem_base_rights ||
               next.limits.filesystem_inheriting_rights!=env.limits.filesystem_inheriting_rights)return status::unsupported;
            if(!env.fd_storage.fits_occupied_slot_limit(static_cast<::std::size_t>(next.fd_limit)))return status::invalid_input;
            for(unsigned i{};i<3u;++i)
            {
                auto bit{i==0u?wasi_policy_environment::permission_t::stdin_read:i==1u?wasi_policy_environment::permission_t::stdout_write:wasi_policy_environment::permission_t::stderr_write};
                bool const granted{(next.non_fd_permissions&bit)!=wasi_policy_environment::permission_t{}};
                if(!granted)continue;
                // Never manufacture or reopen an object when expanding authority.
                if(i>=env.fd_storage.opens.size())return status::unsupported;
                auto const* slot{env.fd_storage.opens.index_unchecked(i).fd_p};
                if(slot==nullptr)return status::unsupported;
                if(slot->close_pos!=SIZE_MAX || slot->wasi_fd.ptr==nullptr)
                {if(!env.permits(bit))return status::unsupported;continue;}
                bool approved{};
                for(auto const& resource:env.policy_resources)
                {if(resource.kind==i && resource.logical_slot==i && resource.incarnation==reinterpret_cast<::std::uintptr_t>(slot->wasi_fd.ptr)){approved=true;break;}}
                if(!approved)return status::unsupported;
            }
            return status::ok;
        }
        template<typename Serializer>
        [[nodiscard]] inline bool collect(Serializer&& serializer,::std::vector<unsigned char>& bytes)
        {
            bytes.clear();bool overflow{};
            auto sink{[&](unsigned char c){if(bytes.size()==maximum_effective_wasi_policy_bytes){overflow=true;return;}bytes.push_back(c);}};
            return serializer(sink)&&!overflow;
        }
    }
    // Authoring helper only. Neither encoding nor decoding conveys authority.
    [[nodiscard]] inline bool encode_wasi_policy(wasi_policy_spec const& spec,::std::vector<unsigned char>& bytes)
    {
        if(!wasi_policy_detail::valid_spec(spec))return false;
        ::std::vector<unsigned char> output(wasi_policy_detail::magic.begin(),wasi_policy_detail::magic.end());
        auto number{[&](::std::uint64_t n){append_u64(output,n);}};
        number(spec.interface_enabled);number(static_cast<::std::uint32_t>(spec.non_fd_permissions));number(spec.fd_limit);
        for(auto const& limit:spec.limits.operations)
        {number(limit.request_bytes);number(limit.window_bytes);number(limit.window_calls);number(limit.total_bytes);number(limit.total_calls);number(limit.concurrent);for(auto action:limit.actions)output.push_back(static_cast<unsigned char>(action));for(auto action:limit.exit_actions)output.push_back(static_cast<unsigned char>(action));output.push_back(static_cast<unsigned char>(limit.native_wait_exit_action));}
        for(auto grain:spec.limits.clock_granularity_ns)number(grain);
        number(spec.limits.max_poll_subscriptions);number(spec.limits.max_poll_wait_ns);number(spec.limits.max_args_env_bytes);
        number(spec.limits.filesystem_base_rights);number(spec.limits.filesystem_inheriting_rights);
        auto strings{[&](auto const& values){number(values.size());for(auto const& value:values){number(value.size());for(auto c:value)output.push_back(static_cast<unsigned char>(c));}}};
        strings(spec.argv);strings(spec.envs);
        if(output.size()>maximum_wasi_policy_bytes)return false;
        bytes.swap(output);return true;
    }
    template<wasi_policy_environment::wasip1_memory Memory>
    [[nodiscard]] inline wasi_policy_spec current_wasi_policy(wasi_policy_environment::wasip1_environment<Memory> const& env)
    {
        wasi_policy_spec result{};result.interface_enabled=env.interface_enabled;
        result.non_fd_permissions=env.non_fd_permissions;result.fd_limit=env.fd_storage.fd_limit;result.limits=env.limits;
        for(auto value:env.argv)result.argv.emplace_back(value);
        for(auto value:env.envs)result.envs.emplace_back(value);
        return result;
    }
    template<wasi_policy_environment::wasip1_memory Memory> class prepared_wasi_policy;
    template<wasi_policy_environment::wasip1_memory Memory>
    struct wasi_policy_prepare_result {status result{status::invalid_input};::std::unique_ptr<prepared_wasi_policy<Memory>> plan{};};
    template<wasi_policy_environment::wasip1_memory Memory>
    class prepared_wasi_policy
    {
        using environment_type=wasi_policy_environment::wasip1_environment<Memory>;
        environment_type const* owner_{};
        wasi_policy_spec proposed_{};wasi_policy_views argv_views_{},env_views_{};
        digest old_hash_{},new_hash_{},measurement_{};
        ::std::vector<unsigned char> canonical_request_{};
        ::std::uint64_t base_generation_{},target_generation_{},base_revocation_{},target_revocation_{};
        bool consumed_{};
        template<wasi_policy_environment::wasip1_memory M>
        friend wasi_policy_prepare_result<M> prepare_wasi_policy(wasi_policy_environment::wasip1_environment<M>&,::std::span<unsigned char const>,::std::uint64_t,::std::uint64_t,::std::uint64_t,::std::uint64_t);
        template<wasi_policy_environment::wasip1_memory M>
        friend status preflight_wasi_policy(wasi_policy_environment::wasip1_environment<M> const&,prepared_wasi_policy<M> const&,::std::uint64_t,::std::uint64_t);
        template<wasi_policy_environment::wasip1_memory M,typename Sink>
        friend bool serialize_projected(wasi_policy_environment::wasip1_environment<M> const&,prepared_wasi_policy<M> const&,Sink&&);
        template<wasi_policy_environment::wasip1_memory M>
        friend bool apply_wasi_policy(wasi_policy_environment::wasip1_environment<M>&,prepared_wasi_policy<M>&) noexcept;
    public:
        prepared_wasi_policy()=default;
        prepared_wasi_policy(prepared_wasi_policy const&)=delete;
        prepared_wasi_policy& operator=(prepared_wasi_policy const&)=delete;
        [[nodiscard]] digest const& old_policy_hash() const noexcept{return old_hash_;}
        [[nodiscard]] digest const& new_policy_hash() const noexcept{return new_hash_;}
        [[nodiscard]] digest const& measurement() const noexcept{return measurement_;}
        [[nodiscard]] ::std::uint64_t base_generation() const noexcept{return base_generation_;}
        [[nodiscard]] ::std::uint64_t target_generation() const noexcept{return target_generation_;}
        [[nodiscard]] ::std::uint64_t base_revocation_generation() const noexcept{return base_revocation_;}
        [[nodiscard]] ::std::uint64_t revocation_generation() const noexcept{return target_revocation_;}
        [[nodiscard]] ::std::span<unsigned char const> canonical_request() const noexcept{return canonical_request_;}
        [[nodiscard]] bool interface_enabled() const noexcept{return proposed_.interface_enabled;}
        [[nodiscard]] wasi_policy_environment::permission_t permissions() const noexcept{return proposed_.non_fd_permissions;}
    };
    template<wasi_policy_environment::wasip1_memory Memory>
    [[nodiscard]] inline wasi_policy_prepare_result<Memory> prepare_wasi_policy(
        wasi_policy_environment::wasip1_environment<Memory>& env,::std::span<unsigned char const> bytes,
        ::std::uint64_t base_generation,::std::uint64_t target_generation,::std::uint64_t base_revocation,::std::uint64_t target_revocation)
    {
#if defined(__cpp_exceptions) || defined(_CPPUNWIND)
        try
        {
#endif
            if(base_generation==0u || base_generation==UINT64_MAX || target_generation!=base_generation+1u || target_revocation<base_revocation)
                return {status::invalid_input,{}};
            auto plan{::std::make_unique<prepared_wasi_policy<Memory>>()};
            if(!wasi_policy_detail::decode(bytes,plan->proposed_))return {status::invalid_input,{}};
            if(auto allowed{wasi_policy_detail::resources(env,plan->proposed_)};allowed!=status::ok)return {allowed,{}};
            if(wasi_policy_detail::tightens(env,plan->proposed_)&&target_revocation==base_revocation)return {status::unauthorized,{}};
            plan->owner_=&env;plan->base_generation_=base_generation;plan->target_generation_=target_generation;
            plan->base_revocation_=base_revocation;plan->target_revocation_=target_revocation;
            if(!encode_wasi_policy(plan->proposed_,plan->canonical_request_) ||
               plan->canonical_request_.size()!=bytes.size() || !::std::equal(bytes.begin(),bytes.end(),plan->canonical_request_.begin()))return {status::invalid_input,{}};
            for(auto const& value:plan->proposed_.argv)plan->argv_views_.emplace_back(value.data(),value.size());
            for(auto const& value:plan->proposed_.envs)plan->env_views_.emplace_back(value.data(),value.size());
            ::std::vector<unsigned char> old_bytes,new_bytes;
            if(!wasi_policy_detail::collect([&](auto& sink){return wasi_policy_environment::serialize_policy(env,sink);},old_bytes) ||
               !wasi_policy_detail::collect([&](auto& sink){return wasi_policy_environment::serialize_policy_with_configuration(env,plan->proposed_,sink);},new_bytes))return {status::invalid_input,{}};
            if(!hash(old_bytes,plan->old_hash_)||!hash(new_bytes,plan->new_hash_))return {status::unsupported,{}};
            ::std::vector<unsigned char> material;
            constexpr ::std::string_view domain{"uwvm.wasi.policy.change.v1"};for(char c:domain)material.push_back(static_cast<unsigned char>(c));material.push_back(0);
            append_u64(material,reinterpret_cast<::std::uintptr_t>(&env));append_u64(material,base_generation);append_u64(material,target_generation);
            append_u64(material,base_revocation);append_u64(material,target_revocation);
            material.insert(material.end(),plan->old_hash_.begin(),plan->old_hash_.end());material.insert(material.end(),plan->new_hash_.begin(),plan->new_hash_.end());
            material.insert(material.end(),plan->canonical_request_.begin(),plan->canonical_request_.end());
            if(!hash(material,plan->measurement_))return {status::unsupported,{}};
            return {status::ok,::std::move(plan)};
#if defined(__cpp_exceptions) || defined(_CPPUNWIND)
        }
        catch(...){return {status::exhausted,{}};}
#endif
    }
    template<wasi_policy_environment::wasip1_memory Memory,typename Sink>
    [[nodiscard]] inline bool serialize_projected(wasi_policy_environment::wasip1_environment<Memory> const& env,prepared_wasi_policy<Memory> const& plan,Sink&& sink)
    {return plan.owner_==&env && !plan.consumed_ && wasi_policy_environment::serialize_policy_with_configuration(env,plan.proposed_,static_cast<Sink&&>(sink));}
    template<wasi_policy_environment::wasip1_memory Memory>
    [[nodiscard]] inline status preflight_wasi_policy(wasi_policy_environment::wasip1_environment<Memory> const& env,prepared_wasi_policy<Memory> const& plan,
        ::std::uint64_t current_generation,::std::uint64_t current_revocation)
    {
        if(plan.owner_!=&env || plan.consumed_)return status::invalid_state;
        if(current_generation!=plan.base_generation_ || current_revocation!=plan.base_revocation_)return status::stale_base;
        if(auto allowed{wasi_policy_detail::resources(env,plan.proposed_)};allowed!=status::ok)return allowed;
#if defined(__cpp_exceptions) || defined(_CPPUNWIND)
        try
        {
#endif
            ::std::vector<unsigned char> bytes;digest measured{};
            if(!wasi_policy_detail::collect([&](auto& sink){return wasi_policy_environment::serialize_policy(env,sink);},bytes) || !hash(bytes,measured))return status::invalid_input;
            return measured==plan.old_hash_?status::ok:status::stale_base;
#if defined(__cpp_exceptions) || defined(_CPPUNWIND)
        }
        catch(...){return status::exhausted;}
#endif
    }
    // Private runtime publication primitive, NOT an authorization entry point.
    // Its caller must own actual closed admission + drained execution, verify
    // the signed manifest via protocol, match this plan's hashes/generations in
    // PREPARED/READY, and run preflight under that same fence before COMMIT.
    // No "verified" flag or guest-provided proof is accepted here. Publication
    // does not allocate, destroy native resources, wait, reset budgets or reopen
    // admission. Old owned strings remain in the plan until safe retirement.
    template<wasi_policy_environment::wasip1_memory Memory>
    [[nodiscard]] inline bool apply_wasi_policy(wasi_policy_environment::wasip1_environment<Memory>& env,prepared_wasi_policy<Memory>& plan) noexcept
    {
        if(plan.owner_!=&env || plan.consumed_)return false;
        auto const old_permissions{env.non_fd_permissions};
        env.policy_owned_argv.swap(plan.proposed_.argv);env.argv.swap(plan.argv_views_);
        env.policy_owned_envs.swap(plan.proposed_.envs);env.envs.swap(plan.env_views_);
        env.interface_enabled=plan.proposed_.interface_enabled;
        env.non_fd_permissions=plan.proposed_.non_fd_permissions;env.limits=plan.proposed_.limits;
        env.fd_storage.fd_limit=static_cast<::std::size_t>(plan.proposed_.fd_limit);
        for(unsigned i{};i<3u&&i<env.fd_storage.opens.size();++i)
        {
            auto* slot{env.fd_storage.opens.index_unchecked(i).fd_p};
            if(slot!=nullptr&&slot->close_pos==SIZE_MAX&&slot->wasi_fd.ptr!=nullptr)
            {
                using rights=::uwvm2::imported::wasi::wasip1::abi::rights_t;
                using permission=wasi_policy_environment::permission_t;
                auto const direction{i==0u?permission::stdin_read:i==1u?permission::stdout_write:permission::stderr_write};
                if(!env.grants_stdio(i)){slot->rights_base={};}
                else
                {
                    // Preserve a guest's own fd_set_rights reductions unless
                    // this signed policy explicitly grants that operation anew.
                    auto desired{slot->rights_base & env.stdio_base_rights(i==0u)};
                    if((old_permissions&direction)==permission{})desired=env.stdio_base_rights(i==0u);
                    if((old_permissions&permission::stdio_metadata)==permission{} && env.permits(permission::stdio_metadata))desired|=rights::right_fd_filestat_get;
                    if((old_permissions&permission::stdio_readiness)==permission{} && env.permits(permission::stdio_readiness))desired|=rights::right_poll_fd_readwrite;
                    slot->rights_base=desired;
                }
                slot->rights_inherit={};
            }
        }
        plan.consumed_=true;return true;
    }
}
#endif
