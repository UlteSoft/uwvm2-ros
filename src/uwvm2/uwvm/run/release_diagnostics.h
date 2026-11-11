#pragma once
#include <string>
#include <uwvm2/runtime/diagnostic/crash_policy.h>
#include <uwvm2/runtime/lib/uwvm_runtime_diagnostics.h>
namespace uwvm2::uwvm::run::release_diagnostics
{
    inline ::std::string snapshot() noexcept
    {
#if defined(UWVM_CPP_EXCEPTIONS)
        try
#endif
        {
            ::uwvm2::runtime::lib::diagnostic_snapshot v{};
            if(!::uwvm2::runtime::lib::runtime_diagnostic_snapshot_host_api(v))
            { return "{\"schema\":1,\"status\":\"unavailable\",\"debug_sealed\":true}\n"; }
            ::std::string s{v.collection_lost ? "{\"schema\":1,\"status\":\"incomplete\"" : "{\"schema\":1,\"status\":\"ok\""};
            s += ",\"product\":\"uwvm2-ros\",\"diagnostic_scope\":\"non-sensitive\"";
            auto number{[&](char const* name, ::std::uint64_t n) { s += ",\""; s += name; s += "\":"; s += ::std::to_string(n); }};
            auto boolean{[&](char const* name, bool n) { s += ",\""; s += name; s += n ? "\":true" : "\":false"; }};
            auto hex{[&](auto const& bytes) { s += '"'; for(auto c : bytes) { s += "0123456789abcdef"[c>>4]; s += "0123456789abcdef"[c&15]; } s += '"'; }};
#if defined(UWVM_VERSION_X) && defined(UWVM_VERSION_Y) && defined(UWVM_VERSION_Z) && defined(UWVM_VERSION_S)
            s += ",\"runtime_version\":\"" + ::std::to_string(UWVM_VERSION_X) + "." + ::std::to_string(UWVM_VERSION_Y) +
                 "." + ::std::to_string(UWVM_VERSION_Z) + "." + ::std::to_string(UWVM_VERSION_S) + "\"";
#endif
            boolean("update_in_progress", v.update_in_progress);
            boolean("current_update_facts_available", v.update_managed && !v.collection_lost && !v.update_in_progress);
            number("runtime_generation",v.runtime_generation); number("sequence",v.sequence);
            number("roots_started",v.roots_started); number("roots_completed",v.roots_completed); number("roots_active",v.roots_active);
            boolean("counters_sampled_independently",true); boolean("collection_lost",v.collection_lost);
            namespace crash=::uwvm2::runtime::diagnostic::crash;
            crash::event terminal{};
            bool const has_terminal=crash::snapshot_host_api(terminal);
            boolean("faulted",v.faulted || has_terminal); number("fault_kind",v.fault_kind); boolean("stack_truncated",v.stack_truncated);
            s += ",\"crash\":";
            if(has_terminal){auto event=crash::format_event_json(terminal);s.append(event.bytes,event.size-1u);}
            else s += "null";
            boolean("frame_offsets_available",false); s += ",\"frames\":[";
            for(::std::size_t i{}; i < v.frame_count; ++i)
            {
                if(i) s += ',';
                auto const& f{v.frames[i]};
                s += "{\"module\":" + ::std::to_string(f.module) + ",\"function\":" + ::std::to_string(f.function) +
                     ",\"generation\":" + ::std::to_string(f.generation) + '}';
            }
            s += ']'; boolean("update_managed",v.update_managed); number("update_phase",v.update_phase);
            boolean("admission_active",v.active); boolean("coordinator_connected",v.coordinator_connected);
            number("installed_epoch",v.installed_epoch); number("authenticated_commit_epoch",v.authenticated_commit_epoch);
            number("decision_sequence",v.decision_sequence); s += ",\"installed_sha256\":"; hex(v.installed_sha256);
            number("total_initial_modules",v.total_modules); boolean("modules_truncated",v.module_truncated);
            s += ",\"initial_modules\":[";
            for(::std::size_t i{}; i < v.module_count; ++i)
            {
                if(i) s += ',';
                s += "{\"source_index\":" + ::std::to_string(i) + ",\"bytes\":" + ::std::to_string(v.modules[i].bytes) + ",\"sha256\":";
                hex(v.modules[i].sha256); s += '}';
            }
            s += "]}\n"; return s;
        }
#if defined(UWVM_CPP_EXCEPTIONS)
        catch(...) { return {}; } // Allocation failure never becomes a Guest fault or an unbounded report.
#endif
    }
}
