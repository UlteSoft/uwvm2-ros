// SPDX-License-Identifier: Apache-2.0
#pragma once
#include "deployment.h"
#include <uwvm2/imported/ros/common/audit_io.h>
#include <atomic>
#include <charconv>
#include <chrono>
#include <mutex>

// Trusted, explicitly enabled process diagnostics. This is not a Guest import,
// a permission source, a native session credential, or a command retry path.
namespace uwvm2::imported::ros::portable::runtime_audit
{
    enum class event { vm_goal_accepted, vm_terminal, native_terminal };
    struct observation
    {
        event kind{};
        ::std::uint64_t runtime_generation{},owner_handle{},command_id{};
        ::std::uint64_t native_observed_ns{},observed_sequence{};
        ::std::array<::std::uint8_t,16> goal_uuid{};
        outcome result{};
        ::std::int32_t native_status{};
        digest command_sha256{};
    };
    // Observation-only correlation IDs for an actual runtime object/generation
    // and its actual permission selection. These IDs grant no authority.
    struct call_context
    {
        ::std::uint64_t module_instance{},permission_session{},call_sequence{},runtime_generation{};
        unsigned function_index{};
        digest wasm_sha256{},adapter_sha256{},policy_sha256{};
        bool allowed{};
    };
    namespace detail
    {
        class record
        {
            ::std::array<char,4095> bytes_{};
            ::std::size_t size_{};
            bool valid_{true};
        public:
            void append(::std::string_view value) noexcept
            {
                if(!valid_||value.size()>bytes_.size()-size_) { valid_=false;return; }
                for(auto c:value) { bytes_[size_++]=c; }
            }
            void quoted(::std::string_view value) noexcept
            {
                constexpr char digits[]{"0123456789abcdef"};
                append("\"");
                for(unsigned char c:value)
                {
                    if(c=='"'||c=='\\') { char escaped[]{'\\',static_cast<char>(c)};append({escaped,2}); }
                    else if(c<0x20u) { char escaped[]{'\\','u','0','0',digits[c>>4u],digits[c&15u]};append({escaped,6}); }
                    else { auto byte{static_cast<char>(c)};append({&byte,1}); }
                }
                append("\"");
            }
            template<typename Integer> void number(Integer value) noexcept
            {
                ::std::array<char,32> bytes;
                auto result{::std::to_chars(bytes.data(),bytes.data()+bytes.size(),value)};
                if(result.ec!=::std::errc{}) { valid_=false;return; }
                append({bytes.data(),static_cast<::std::size_t>(result.ptr-bytes.data())});
            }
            template<::std::size_t N> void hex(::std::array<::std::uint8_t,N> const& value) noexcept
            { hex(::std::span<::std::uint8_t const>{value}); }
            void hex(::std::span<::std::uint8_t const> value) noexcept
            {
                constexpr char digits[]{"0123456789abcdef"};append("\"");
                for(auto c:value) { char pair[]{digits[c>>4u],digits[c&15u]};append({pair,2}); }
                append("\"");
            }
            [[nodiscard]] ::std::string_view view() const noexcept { return valid_ ? ::std::string_view{bytes_.data(),size_} : ::std::string_view{}; }
        };
    }
    class channel final
    {
        ::std::array<char,32> run_id_{};
        ::std::atomic<bool> configured_{},enabled_{};
        ::std::atomic<unsigned> attempts_{},dropped_{};
        struct module_slot { void const* instance{};::std::uint64_t generation{};digest wasm{}; };
        struct session_slot
        {
            ::std::uint64_t module_instance{},sequence{};
            joint_deployment const* selection{};
            bool allowed{};
        };
        ::std::mutex correlation_mutex_{};
        ::std::array<module_slot,8> modules_{};
        ::std::array<session_slot,8> sessions_{};
        unsigned module_count_{},session_count_{};
        digest observed_adapter_{},observed_policy_{};
        bool inputs_configured_{};
        [[nodiscard]] bool reserve_record() noexcept
        {
            auto count{attempts_.load(::std::memory_order_relaxed)};
            do { if(count==maximum_records) { return false; } }
            while(!attempts_.compare_exchange_weak(count,count+1,::std::memory_order_relaxed));
            return true;
        }
        static error observe_file(::std::string const& path,digest& output)
        {
#if defined(__linux__)
            // Bounded metadata snapshots only. This deliberately cannot select
            // deployment_selection or register a permission/native owner.
            if(path.empty()||path.front()!='/'||path.size()>4096u||path.find('\0')!=path.npos) { return error::malformed; }
            int fd{::open(path.c_str(),O_RDONLY|O_CLOEXEC|O_NOFOLLOW|O_NONBLOCK)};
            if(fd<0) { return error::unavailable; }
            struct close_fd { int fd;~close_fd(){::close(fd);} } guard{fd};
            struct stat before{},after{};
            if(::fstat(fd,&before)!=0||!S_ISREG(before.st_mode)||before.st_size<=0||before.st_size>32768) { return error::malformed; }
            ::std::string bytes(static_cast<::std::size_t>(before.st_size),'\0');::std::size_t at{};
            while(at!=bytes.size())
            {
                auto count{::pread(fd,bytes.data()+at,bytes.size()-at,static_cast<off_t>(at))};
                if(count<0&&errno==EINTR) { continue; }
                if(count<=0) { return error::unavailable; }
                at+=static_cast<::std::size_t>(count);
            }
            char extra{};
            if(::pread(fd,&extra,1,before.st_size)!=0||::fstat(fd,&after)!=0||before.st_dev!=after.st_dev||
               before.st_ino!=after.st_ino||before.st_size!=after.st_size||
               before.st_mtim.tv_sec!=after.st_mtim.tv_sec||before.st_mtim.tv_nsec!=after.st_mtim.tv_nsec||
               before.st_ctim.tv_sec!=after.st_ctim.tv_sec||before.st_ctim.tv_nsec!=after.st_ctim.tv_nsec) { return error::stale; }
            output=hash_bytes(bytes);return error::ok;
#else
            (void)path;(void)output;return error::unsupported;
#endif
        }
    public:
        static constexpr unsigned maximum_records{8};
        // Called only by the trusted process entry point, before execution.
        // One immutable run ID per process; inherited environment does not enable it.
        [[nodiscard]] bool configure(::std::string_view run_id) noexcept
        {
#if defined(__linux__)
            if(run_id.size()!=run_id_.size()) { return false; }
            bool nonzero{};
            for(auto c:run_id)
            {
                if(!((c>='0'&&c<='9')||(c>='a'&&c<='f'))) { return false; }
                nonzero|=c!='0';
            }
            bool expected{};
            if(!nonzero||!configured_.compare_exchange_strong(expected,true,::std::memory_order_relaxed)) { return false; }
            ::std::copy(run_id.begin(),run_id.end(),run_id_.begin());
            enabled_.store(true,::std::memory_order_release);return true;
#else
            (void)run_id;return false;
#endif
        }
        // Trusted pre-execution setup only. Even a valid grant read here remains
        // an observation input; only the existing --portable-robot path enables it.
        [[nodiscard]] error configure_inputs(::std::string const& adapter,::std::string const& policy)
        {
            if(inputs_configured_) { return error::denied; }
            digest a{},p{};
            if(auto e{observe_file(adapter,a)};e!=error::ok) { return e; }
            if(auto e{observe_file(policy,p)};e!=error::ok) { return e; }
            observed_adapter_=a;observed_policy_=p;inputs_configured_=true;return error::ok;
        }
        [[nodiscard]] call_context enter_call(void const* instance,::std::uint64_t generation,digest const& wasm,
            unsigned function,joint_deployment const* selection,bool allowed) noexcept
        {
            if(!enabled()) { return {}; }
            if(!instance||!generation||function<1u||function>6u||(allowed&&!selection)||
               (!selection&&!inputs_configured_)) { return {}; }
            ::std::lock_guard lock{correlation_mutex_};
            unsigned module{};
            while(module!=module_count_&&(modules_[module].instance!=instance||modules_[module].generation!=generation)) { ++module; }
            if(module==module_count_)
            {
                if(module_count_==modules_.size()) { return {}; }
                modules_[module]={instance,generation,wasm};++module_count_;
            }
            else if(modules_[module].wasm!=wasm) { return {}; }
            auto id{static_cast<::std::uint64_t>(module)+1u};unsigned session{};
            while(session!=session_count_&&(sessions_[session].module_instance!=id||
                  sessions_[session].selection!=selection||sessions_[session].allowed!=allowed)) { ++session; }
            if(session==session_count_)
            {
                if(session_count_==sessions_.size()) { return {}; }
                sessions_[session]={id,0u,selection,allowed};++session_count_;
            }
            auto& current{sessions_[session]};if(current.sequence==UINT64_MAX) { return {}; }
            return {id,static_cast<::std::uint64_t>(session)+1u,++current.sequence,generation,function,wasm,
                    selection?selection->adapter_sha256:observed_adapter_,
                    selection?selection->policy_sha256:observed_policy_,allowed};
        }
        [[nodiscard]] bool emit_binding(call_context const& value,digest const* command=nullptr,
            ::std::array<::std::uint8_t,16> const* goal=nullptr) noexcept
        {
            if(!enabled()) { return false; }
#if defined(__linux__)
            ::uwvm2::imported::ros::audit_io::details::errno_guard preserve_errno;
            if(!reserve_record()) { return false; }
            auto fail=[this]() noexcept { dropped_.fetch_add(1,::std::memory_order_relaxed);return false; };
            if(!value.module_instance||!value.permission_session||!value.call_sequence||!value.runtime_generation||
               value.function_index<1u||value.function_index>6u) { return fail(); }
            if(value.allowed)
            {
                if(value.function_index!=4u||!command||!goal) { return fail(); }
                bool nonzero{};for(auto byte:*goal) { nonzero|=byte!=0; }if(!nonzero) { return fail(); }
            }
            else if(command||goal) { return fail(); }
            auto stamp{::std::chrono::duration_cast<::std::chrono::nanoseconds>(::std::chrono::steady_clock::now().time_since_epoch()).count()};
            auto pid{::getpid()};if(stamp<=0||pid<=0) { return fail(); }
            constexpr ::std::array<::std::string_view,6> names{"open","describe","read_state","submit","wait","close"};
            detail::record row;
            row.append("{\"format\":\"uwvm-sdk-simulation-command-binding-v1\",\"producer\":\"uwvm2-ros/runtime/portable-host\"");
            row.append(",\"run_id\":");row.quoted({run_id_.data(),run_id_.size()});
            row.append(",\"pid\":");row.number(pid);
            row.append(",\"monotonic_ns\":");row.number(stamp);
            row.append(",\"module_sha256\":");row.hex(value.wasm_sha256);
            row.append(",\"module_instance\":");row.number(value.module_instance);
            row.append(",\"runtime_generation\":");row.number(value.runtime_generation);
            row.append(",\"permission_session\":");row.number(value.permission_session);
            row.append(",\"call_sequence\":");row.number(value.call_sequence);
            row.append(",\"portable_call\":");row.quoted(names[value.function_index-1u]);
            row.append(",\"adapter_sha256\":");row.hex(value.adapter_sha256);
            row.append(",\"policy_sha256\":");row.hex(value.policy_sha256);
            if(value.allowed)
            {
                row.append(",\"decision\":\"ALLOW\",\"phase\":\"native-goal-accepted\",\"guest_command_sha256\":");row.hex(*command);
                row.append(",\"native_operation_id\":");row.hex(*goal);
                row.append(",\"receipt_delivered\":true}");
            }
            else { row.append(",\"decision\":\"DENY\",\"phase\":\"permission-denied\",\"error_code\":13,\"native_operation_id\":null}"); }
            auto bytes{row.view()};
            if(bytes.empty()||!::uwvm2::imported::ros::audit_io::write_record_to(::uwvm2::imported::ros::audit_io::stream::output,bytes)) { return fail(); }
            return true;
#else
            (void)value;(void)command;(void)goal;return false;
#endif
        }
        // Called only after the permissioned Host has successfully copied these
        // exact native response bytes into the active caller's selected memory.
        // A delivery observation does not assert that Guest processing succeeded.
        [[nodiscard]] bool emit_state_delivery(call_context const& call,joint_deployment const& deployment,
            ::std::uint64_t owner_handle,::std::uint64_t output_offset,::std::span<::std::uint8_t const> payload,
            receipt const* command=nullptr,::std::array<::std::uint8_t,16> const* goal=nullptr) noexcept
        {
            if(!enabled()) { return false; }
#if defined(__linux__)
            ::uwvm2::imported::ros::audit_io::details::errno_guard preserve_errno;
            if(!reserve_record()) { return false; }
            auto fail=[this]() noexcept { dropped_.fetch_add(1,::std::memory_order_relaxed);return false; };
            state sample{};
            if(!call.allowed||call.function_index!=3u||!call.module_instance||!call.permission_session||
               !call.call_sequence||!call.runtime_generation||!owner_handle||
               call.wasm_sha256!=deployment.wasm_sha256||call.adapter_sha256!=deployment.adapter_sha256||
               call.policy_sha256!=deployment.policy_sha256||!deployment.policy_epoch||!deployment.native_generation||
               payload.size()>320u||output_offset>UINT64_MAX-payload.size()||!decode(payload,sample)||
               sample.epoch!=deployment.mapping.epoch||sample.count!=deployment.mapping.count||
               sample.valid_until_ns-sample.observed_ns>250000000u) { return fail(); }
            for(::std::uint32_t i{};i!=sample.count;++i)
            {
                auto const& bound{deployment.mapping.joints[i]};
                if(sample.positions[i]<bound.lower||sample.positions[i]>bound.upper) { return fail(); }
            }
            auto stamp{::std::chrono::duration_cast<::std::chrono::nanoseconds>(::std::chrono::steady_clock::now().time_since_epoch()).count()};
            auto pid{::getpid()};
            if(stamp<=0||pid<=0||sample.observed_ns>static_cast<::std::uint64_t>(stamp)||
               static_cast<::std::uint64_t>(stamp)>=sample.valid_until_ns) { return fail(); }
            if(command)
            {
                bool nonzero{};if(goal) { for(auto byte:*goal) { nonzero|=byte!=0; } }
                if(!nonzero||!valid(*command)||command->epoch!=sample.epoch||
                   command->observed_ns>static_cast<::std::uint64_t>(stamp)||
                   command->observed_sequence>sample.sequence) { return fail(); }
            }
            else if(goal) { return fail(); }
            detail::record row;
            row.append("{\"format\":\"uwvm-sdk-simulation-state-delivery-v1\",\"producer\":\"uwvm2-ros/runtime/portable-host\"");
            row.append(",\"run_id\":");row.quoted({run_id_.data(),run_id_.size()});
            row.append(",\"pid\":");row.number(pid);
            row.append(",\"monotonic_ns\":");row.number(stamp);
            row.append(",\"module_sha256\":");row.hex(call.wasm_sha256);
            row.append(",\"module_instance\":");row.number(call.module_instance);
            row.append(",\"runtime_generation\":");row.number(call.runtime_generation);
            row.append(",\"permission_session\":");row.number(call.permission_session);
            row.append(",\"call_sequence\":");row.number(call.call_sequence);
            row.append(",\"portable_call\":\"read_state\",\"decision\":\"ALLOW\",\"adapter_sha256\":");row.hex(call.adapter_sha256);
            row.append(",\"policy_sha256\":");row.hex(call.policy_sha256);
            row.append(",\"native_provenance_sha256\":");row.hex(deployment.native_provenance);
            row.append(",\"owner_handle\":");row.number(owner_handle);
            row.append(",\"mapping_epoch\":");row.number(sample.epoch);
            row.append(",\"policy_epoch\":");row.number(deployment.policy_epoch);
            row.append(",\"native_generation\":");row.number(deployment.native_generation);
            row.append(",\"observed_sequence\":");row.number(sample.sequence);
            row.append(",\"native_observed_ns\":");row.number(sample.observed_ns);
            row.append(",\"valid_until_ns\":");row.number(sample.valid_until_ns);
            row.append(",\"memory_index\":0,\"output_offset\":");row.number(output_offset);
            row.append(",\"output_bytes\":");row.number(payload.size());
            row.append(",\"payload_hex\":");row.hex(payload);
            row.append(",\"payload_sha256\":");row.hex(hash_bytes({reinterpret_cast<char const*>(payload.data()),payload.size()}));
            row.append(",\"guest_memory_written\":true,\"guest_processing_proven\":false,\"command_id\":");row.number(command?command->command_id:0u);
            row.append(",\"command_outcome\":");row.number(command?static_cast<unsigned>(command->result):0u);
            row.append(",\"command_observed_sequence\":");row.number(command?command->observed_sequence:0u);
            row.append(",\"native_operation_id\":");
            if(command) { row.hex(*goal);row.append(",\"guest_command_sha256\":");row.hex(command->command_sha256); }
            else { row.append("null"); }
            row.append("}");
            auto bytes{row.view()};
            if(bytes.empty()||!::uwvm2::imported::ros::audit_io::write_record_to(::uwvm2::imported::ros::audit_io::stream::output,bytes)) { return fail(); }
            return true;
#else
            (void)call;(void)deployment;(void)owner_handle;(void)output_offset;(void)payload;(void)command;(void)goal;return false;
#endif
        }
        [[nodiscard]] bool enabled() const noexcept { return enabled_.load(::std::memory_order_acquire); }
        [[nodiscard]] unsigned attempts() const noexcept { return attempts_.load(::std::memory_order_relaxed); }
        [[nodiscard]] unsigned dropped() const noexcept { return dropped_.load(::std::memory_order_relaxed); }
        [[nodiscard]] bool emit(joint_deployment const& deployment,observation const& value) noexcept
        {
            // No formatting, clock, PID lookup, allocation or sink access while off.
            if(!enabled()) { return false; }
#if defined(__linux__)
            ::uwvm2::imported::ros::audit_io::details::errno_guard preserve_errno;
            if(!reserve_record()) { return false; }
            auto fail=[this]() noexcept { dropped_.fetch_add(1,::std::memory_order_relaxed);return false; };
            ::std::string_view name{};
            switch(value.kind)
            {
                case event::vm_goal_accepted:name="vm_goal_accepted";break;
                case event::vm_terminal:name="vm_terminal";break;
                case event::native_terminal:name="native_terminal";break;
                default:return fail();
            }
            auto result{static_cast<unsigned>(value.result)};
            bool uuid{};for(auto byte:value.goal_uuid) { uuid|=byte!=0; }
            if(!uuid||!value.runtime_generation||!value.owner_handle||!value.command_id||!value.native_observed_ns||
               !deployment.mapping.epoch||!deployment.policy_epoch||!deployment.native_generation||
               deployment.module.empty()||deployment.module.size()>255u||deployment.alias.empty()||deployment.alias.size()>255u||
               !deployment_detail::utf8(deployment.module)||!deployment_detail::utf8(deployment.alias)||
               (value.kind==event::vm_goal_accepted ? result!=1u : result<2u||result>4u)||
               ((result==1u||result==2u)&&value.native_status!=0)) { return fail(); }
            auto stamp{::std::chrono::duration_cast<::std::chrono::nanoseconds>(::std::chrono::steady_clock::now().time_since_epoch()).count()};
            auto pid{::getpid()};
            if(stamp<=0||pid<=0||value.native_observed_ns>static_cast<::std::uint64_t>(stamp)) { return fail(); }
            detail::record row;
            row.append("{\"format\":\"uwvm-portable-robot-audit-v1\",\"event\":");row.quoted(name);
            row.append(",\"run_id\":");row.quoted({run_id_.data(),run_id_.size()});
            row.append(",\"pid\":");row.number(pid);
            row.append(",\"monotonic_ns\":");row.number(stamp);
            row.append(",\"module\":");row.quoted(deployment.module);
            row.append(",\"alias\":");row.quoted(deployment.alias);
            row.append(",\"wasm_sha256\":");row.hex(deployment.wasm_sha256);
            row.append(",\"adapter_sha256\":");row.hex(deployment.adapter_sha256);
            row.append(",\"policy_sha256\":");row.hex(deployment.policy_sha256);
            row.append(",\"native_provenance_sha256\":");row.hex(deployment.native_provenance);
            row.append(",\"command_sha256\":");row.hex(value.command_sha256);
            row.append(",\"runtime_generation\":");row.number(value.runtime_generation);
            row.append(",\"owner_handle\":");row.number(value.owner_handle);
            row.append(",\"mapping_epoch\":");row.number(deployment.mapping.epoch);
            row.append(",\"policy_epoch\":");row.number(deployment.policy_epoch);
            row.append(",\"native_generation\":");row.number(deployment.native_generation);
            row.append(",\"command_id\":");row.number(value.command_id);
            row.append(",\"goal_uuid\":");row.hex(value.goal_uuid);
            row.append(",\"outcome\":");row.number(result);
            row.append(",\"native_status\":");row.number(value.native_status);
            row.append(",\"native_observed_ns\":");row.number(value.native_observed_ns);
            row.append(",\"observed_sequence\":");row.number(value.observed_sequence);
            row.append(",\"fresh_state_confirmed\":");row.append(result==2u?"true":"false");
            row.append(value.kind==event::native_terminal?",\"reply_sent\":true}":",\"receipt_delivered\":true}");
            auto bytes{row.view()};
            if(bytes.empty()||!::uwvm2::imported::ros::audit_io::write_record_to(::uwvm2::imported::ros::audit_io::stream::output,bytes)) { return fail(); }
            return true;
#else
            (void)deployment;(void)value;return false;
#endif
        }
    };
    inline channel runtime{};
}
