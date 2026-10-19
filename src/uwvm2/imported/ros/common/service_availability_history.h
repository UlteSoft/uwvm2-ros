/*************************************************************
 * UWVM2 generation-bound service availability history. APL-2.0.*
 *************************************************************/
#pragma once
#include "service_availability.h"
#include <cstddef>
#include <cstdint>
#include <map>
#include <string>
#include <string_view>
#include <utility>

namespace uwvm2::imported::ros
{
    // One ledger per serialized Host backend or authenticated Gateway session.
    // Values only: no borrowed authorization/native pointer or Guest address.
    // Tombstones NEVER evict. The ceiling counts lifetime first associations,
    // including closed ones; exhaustion11 must precede native client creation.
    class service_availability_history
    {
    public:
        using context=ipc::client_availability::context;
        using retirement=ipc::client_availability::retirement;
        struct entry
        {
            context binding{};std::string availability_alias{},source_alias{};
            std::uint64_t native_session{};
            bool retired{},source_retired{},availability_revoked{},source_revoked{};
        };
    private:
        std::map<std::uint64_t,entry> rows_;
        std::uint64_t instance_{},runtime_generation_{};unsigned backend_{};bool sealed_{};
    public:
        auto const& rows() const noexcept{return rows_;}
        bool sealed() const noexcept{return sealed_;}
        std::uint32_t bind(context const& c,std::string_view a,std::string_view s,
                           std::uint64_t native_session,std::size_t maximum,entry const*& output)
        {
            output=nullptr;
            if(!ipc::client_availability::valid(c)||!native_session||a.empty()||s.empty()||a==s||
               a.size()>255||s.size()>255||a.find('\0')!=a.npos||s.find('\0')!=s.npos||!maximum||maximum>65536){return 22;}
            if(sealed_){return 125;}
            if(instance_&&(instance_!=c.instance||runtime_generation_!=c.runtime_generation||backend_!=c.backend)){return 13;}
            auto found{rows_.find(c.availability)};
            if(found!=rows_.end())
            {
                auto const& old{found->second};
                if(!ipc::client_availability::same(old.binding,c)||old.availability_alias!=a||old.source_alias!=s){return 17;}
                if(old.retired||old.source_retired||old.availability_revoked||old.source_revoked||old.native_session!=native_session){return 125;}
                output=&old;return 0;
            }
            std::size_t count{};
            for(auto const& item:rows_)
            {
                auto const& old{item.second};
                if(old.binding.availability==c.source||old.binding.source==c.availability||
                   (old.binding.source==c.source&&old.source_alias!=s)){return 13;}
                if(old.availability_alias==a){if(old.availability_revoked){return 125;}++count;}
                if(old.source_alias==s&&old.source_revoked){return 125;}
                if(old.binding.source==c.source&&old.source_retired){return 125;}
            }
            if(count>=maximum||rows_.size()>=65536){return 11;}
            // Build ALL copied metadata before publication; no second map/count
            // insertion after a row could throw and enable a retry/regrant.
#if defined(__cpp_exceptions)
            try
#endif
            {
                entry value{c,std::string{a},std::string{s},native_session};
                auto inserted{rows_.emplace(c.availability,std::move(value))};if(!inserted.second){return 17;}
                instance_=c.instance;runtime_generation_=c.runtime_generation;backend_=c.backend;
                output=&inserted.first->second;return 0;
            }
#if defined(__cpp_exceptions)
            catch(...){return 12;}
#endif
        }
        entry const* find(context const& c) const noexcept
        {auto i{rows_.find(c.availability)};return i!=rows_.end()&&ipc::client_availability::same(i->second.binding,c)?&i->second:nullptr;}
        // Host-only retirement selection: include historical/inactive rows.
        // In particular, closing a source after its last availability handle
        // must still tell Gateway which full source generation is now retired.
        entry const* retirement_reference(std::uint64_t handle,std::string_view alias,retirement scope,std::uint64_t native_session=0) const noexcept
        {
            for(auto const& item:rows_)
            {
                auto const& row{item.second};
                if(native_session&&row.native_session!=native_session){continue;}
                if((scope==retirement::availability_handle&&row.binding.availability==handle)||
                   (scope==retirement::source_handle&&row.binding.source==handle)||
                   (scope==retirement::availability_alias&&row.availability_alias==alias)||
                   (scope==retirement::source_alias&&row.source_alias==alias)){return &row;}
            }
            return nullptr;
        }
        // Allocation/quotas are NEVER required to retire. Local flags precede
        // any best-effort private IPC/native cleanup. Historical entries stay
        // available so source-close still propagates after all query handles
        // closed. Unknown source-before-first-bind is enforced by the true Host
        // lease: this ledger does not pretend it can query unseen Host handles.
        std::uint32_t retire(context const& c,retirement scope) noexcept
        {
            auto i{rows_.find(c.availability)};
            if(i==rows_.end()||!ipc::client_availability::same(i->second.binding,c)){return 9;}
            if(!ipc::client_availability::valid_retirement(scope)){return 22;}
            auto const& reference{i->second};
            for(auto& item:rows_)
            {
                auto& old{item.second};
                bool match{scope==retirement::availability_handle?old.binding.availability==c.availability:
                    scope==retirement::source_handle?old.binding.source==c.source:
                    scope==retirement::availability_alias?old.availability_alias==reference.availability_alias:old.source_alias==reference.source_alias};
                if(!match){continue;}old.retired=true;
                if(scope==retirement::source_handle||scope==retirement::source_alias){old.source_retired=true;}
                if(scope==retirement::availability_alias){old.availability_revoked=true;}
                if(scope==retirement::source_alias){old.source_revoked=true;}
            }
            return 0;
        }
        void seal() noexcept{sealed_=true;for(auto& item:rows_){item.second.retired=true;}}
        // Transport loss is not a new permission decision. Abandon all bindings
        // owned by that actual native session. Fresh explicit availability
        // handles may bind a later authenticated session if both original Host
        // authorities remain live; an existing handle never silently rebinds.
        void retire_session(std::uint64_t native_session) noexcept
        {for(auto& item:rows_){if(item.second.native_session==native_session){item.second.retired=true;}}}
    };
}
