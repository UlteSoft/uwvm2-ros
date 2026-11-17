// Private actual complete-stop recording management API.
#pragma once
#if defined(UWVM_RUNTIME_LLVM_JIT) && defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD) && defined(UWVM_CPP_EXCEPTIONS)
extern "C++" {
    extern "C++" llvm_jit_checkpoint_instance_capture_result llvm_jit_checkpoint_record_host_api(
        ::uwvm2::utils::thread::cooperative_pause_domain::pause_ticket const& ticket,
        ::std::span<llvm_jit_checkpoint_thread_capture_owner const> supplied,
        ::std::array<::std::byte, 16u> const& label,
        unsigned action) noexcept
    {
        if(!::uwvm2::utils::control::debug_permissions::full_allowed()) { return {}; }
        namespace mode = ::uwvm2::uwvm::runtime::runtime_mode;
        using output_status = llvm_jit_checkpoint_instance_capture_status;
        llvm_jit_checkpoint_instance_capture_result out{};
        if(mode::global_runtime_mode != mode::runtime_mode_t::full_compile ||
           mode::global_runtime_compiler != mode::runtime_compiler_t::llvm_jit_only)
        { out.status = output_status::requires_llvm_jit_full; return out; }
        runtime_checkpoint_coherent_manager::complete_instance_request requested{};
        requested.recording_id = label; requested.include_wasip1=action!=1u; requested.recording_action=action;
        if(action!=1u && action!=2u && action!=3u)return out;
        if(!ticket || supplied.empty())
        { out.status = output_status::unavailable_capture; return out; }
        runtime_checkpoint_capture_buffer captures{supplied.size()};
        if(!captures) { out.status = output_status::allocation_failed; return out; }
        for(::std::size_t index{}; index != supplied.size(); ++index)
        {
            // [request-owned wrapper span ... index<size] end
            // [safe] canonical registry compares get AND control block BEFORE
            // reading supplied payload; no public/private span cast is allowed.
            auto const wrapper{llvm_jit_checkpoint_thread_capture::canonical(supplied[index])};
            if(!wrapper) { out.status = output_status::unavailable_capture; return out; }
            captures[index] = wrapper->actual_; // Only the real retained wrapper is read.
        }
        // The manager resolves the second private capture registry again under
        // its genuine lease/ONE complete cohort before any native field access.
        auto actual{runtime_checkpoint_coherent_manager::capture_complete_current_instance(
            ticket, {captures.data(), supplied.size()}, requested)};
        out.data_error = actual.data_error;
        out.wasip1_checkpoint_required=actual.wasip1_checkpoint_required;
        out.wasip1_captured_together=actual.wasip1_captured_together;
        out.wasip1_environments=::std::move(actual.wasip1_environments);
        out.ros_environments=::std::move(actual.ros_environments);
        if(actual.captured)
        {
            // Zero-copy const graph alias retains the immutable logical graph
            // owner. It exposes copied DATA only, never native
            // carriers/source/engine/leases or a restore-issuer credential.
            out.graph = {actual.captured, ::std::addressof(actual.captured->graph())};
            out.status = output_status::captured;
        }
        else if(actual.data_error == ::uwvm2::uwvm::debugger::checkpoint::error::malformed)
        { out.status = output_status::invalid_recording_label; }
        else if(actual.observation == runtime_checkpoint_coherent_manager::status::allocation_failed)
        { out.status = output_status::allocation_failed; }
        else if(actual.observation != runtime_checkpoint_coherent_manager::status::not_selected)
        { out.status = output_status::unavailable_capture; }
        return out;
    }
}
#else
extern "C++" llvm_jit_checkpoint_instance_capture_result llvm_jit_checkpoint_record_host_api(
    ::uwvm2::utils::thread::cooperative_pause_domain::pause_ticket const&,
    ::std::span<llvm_jit_checkpoint_thread_capture_owner const>,::std::array<::std::byte,16u> const&,unsigned) noexcept {return {};}
#endif
