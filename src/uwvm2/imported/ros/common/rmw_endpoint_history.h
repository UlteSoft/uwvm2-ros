/** @file
 * Finite session-bound native endpoint history shared by trusted Host/Gateway.
 * It stores values only. Every authority check and actual entity operation must
 * still hold the true capability/session lease. Owners serialize every method.
 * A failed activation remains a tombstone; neither a reconnect nor retirement
 * permits the same full source handle to create a second native object.
 */
#pragma once
#include "../ipc/rmw_endpoint.h"
#include <functional>
#include <map>
#include <string>
#include <string_view>
#include <utility>

namespace uwvm2::imported::ros
{
    class rmw_endpoint_history
    {
    public:
        using packet=ipc::rmw_endpoint::packet;
        using binding=ipc::rmw_endpoint::binding;
        using role=ipc::rmw_endpoint::role;
        enum class phase { reserved,active,retired };
        struct entry
        {
            packet observed{};
            std::string alias;
            phase state{phase::reserved};
            std::uint32_t retirement_reason{};
            bool cleanup_attempted{};
            std::uint32_t cleanup_error{};
            // Last observed transfer and last explicit cancellation are separate
            // facts. Retirement never rewrites either into a fictitious rollback.
            std::uint32_t last_transfer_error{},last_cancel_error{};
            bool cancel_attempted{};
        };
        static constexpr std::size_t maximum_entries{65536};
    private:
        std::map<std::uint64_t,entry> entries_;
        std::map<std::string,std::size_t,std::less<>> active_by_alias_;
        std::uint64_t instance_{},generation_{},next_incarnation_{1};
        bool sealed_{};
        static bool same_snapshot(packet const& left,packet const& right) noexcept
        {
            if(!ipc::rmw_endpoint::same(left.owner,right.owner)||left.kind!=right.kind||
               left.incarnation!=right.incarnation||left.profiles!=right.profiles||
               left.gid_size!=right.gid_size||left.gid!=right.gid){return false;}
            for(unsigned i{};i<left.profiles;++i)
            {if(!ipc::rmw_endpoint::same(left.actual[i],right.actual[i])){return false;}}
            return true;
        }
    public:
        rmw_endpoint_history()=default;
        rmw_endpoint_history(rmw_endpoint_history const&)=delete;
        rmw_endpoint_history& operator=(rmw_endpoint_history const&)=delete;
        auto const& rows() const noexcept{return entries_;}
        bool sealed() const noexcept{return sealed_;}
        std::size_t active_count(std::string_view alias) const noexcept
        {
            auto found{active_by_alias_.find(alias)};
            return found==active_by_alias_.end()?0:found->second;
        }
        std::uint32_t reserve(packet const& request,std::string_view alias,
            std::size_t maximum_active,entry const*& output)
        {
            if(!ipc::rmw_endpoint::valid_request(request)||request.operation!=ipc::rmw_endpoint::command::activate||
               alias.empty()||alias.size()>255||alias.find('\0')!=alias.npos||!maximum_active||maximum_active>maximum_entries){return 22;}
            if(sealed_){return 125;}
            auto const& owner{request.owner};
            if(instance_&&(instance_!=owner.instance||generation_!=owner.generation)){return 13;}
            // A backend reconnect can bind a fresh full source handle, but may
            // never revive a row from an abandoned transport session.
            auto old{entries_.find(owner.source)};
            if(old!=entries_.end())
            {
                if(!ipc::rmw_endpoint::same(old->second.observed.owner,owner)||old->second.alias!=alias||old->second.observed.kind!=request.kind){return 13;}
                return old->second.state==phase::retired?125u:17u;
            }
            if(entries_.size()>=maximum_entries||active_count(alias)>=maximum_active){return 11;}
#if defined(__cpp_exceptions)
            try
#endif
            {
                entry value{request,std::string{alias}};
                auto count{active_by_alias_.emplace(value.alias,0)};
                decltype(entries_)::iterator stored;
#if defined(__cpp_exceptions)
                try
#endif
                {
                    auto inserted{entries_.emplace(owner.source,std::move(value))};
                    if(!inserted.second)
                    {if(count.second){active_by_alias_.erase(count.first);}return 17;}
                    stored=inserted.first;
                }
#if defined(__cpp_exceptions)
                catch(...){if(count.second){active_by_alias_.erase(count.first);}throw;}
#endif
                ++count.first->second;
                instance_=owner.instance;generation_=owner.generation;
                output=&stored->second;return 0;
            }
#if defined(__cpp_exceptions)
            catch(...){return 12;}
#endif
        }
        /** Gateway only: call once for one pre-reserved actual creation. Zero
         * signals exhaustion. The counter never wraps or resets in a session.
         * Host observations never mint a native incarnation themselves. */
        std::uint64_t reserve_native_incarnation() noexcept
        {return sealed_||next_incarnation_==UINT64_MAX?0:next_incarnation_++;}
        std::uint32_t activate(packet const& actual) noexcept
        {
            if(sealed_){return 125;}
            if(!ipc::rmw_endpoint::valid_response(actual)||actual.operation!=ipc::rmw_endpoint::command::activate){return 22;}
            auto found{entries_.find(actual.owner.source)};
            if(found==entries_.end()){return 9;}
            auto& row{found->second};
            if(row.state!=phase::reserved){return row.state==phase::retired?125u:17u;}
            if(!ipc::rmw_endpoint::same(row.observed.owner,actual.owner)||row.observed.kind!=actual.kind){return 13;}
            row.observed=actual;row.state=phase::active;return 0;
        }
        std::uint32_t find(binding const& owner,std::uint64_t incarnation,role kind,entry const*& output) const noexcept
        {
            auto found{entries_.find(owner.source)};
            if(found==entries_.end()){return 9;}
            auto const& row{found->second};
            if(!ipc::rmw_endpoint::same(row.observed.owner,owner)||row.observed.incarnation!=incarnation||row.observed.kind!=kind){return 13;}
            if(sealed_||row.state==phase::retired){return 125;}
            if(row.state!=phase::active){return 17;}output=&row;return 0;
        }
        std::uint32_t inspect(packet const& actual) const noexcept
        {
            if(!ipc::rmw_endpoint::valid_response(actual)||actual.operation!=ipc::rmw_endpoint::command::inspect){return 22;}
            entry const* row{};auto error{find(actual.owner,actual.incarnation,actual.kind,row)};
            if(error){return error;}return same_snapshot(row->observed,actual)?0u:13u;
        }
        std::uint32_t retire(binding const& owner,std::uint64_t incarnation,role kind,std::uint32_t reason=125) noexcept
        {
            entry const* row{};auto error{find(owner,incarnation,kind,row)};if(error){return error;}
            abandon(owner.source,reason);return 0;
        }
        /** Trusted cleanup only. Allocation/credit/native success is never
         * necessary to tombstone known authority. The receipt remains intact
         * for a single best-effort native cleanup and independent diagnostics. */
        void abandon(std::uint64_t source,std::uint32_t reason) noexcept
        {
            auto found{entries_.find(source)};if(found==entries_.end()){return;}
            if(found->second.state!=phase::retired)
            {
                auto count{active_by_alias_.find(found->second.alias)};
                if(count!=active_by_alias_.end()&&count->second){--count->second;}
            }
            found->second.state=phase::retired;found->second.retirement_reason=reason;
        }
        bool claim_cleanup(std::uint64_t source) noexcept
        {
            auto found{entries_.find(source)};
            if(found==entries_.end()||found->second.state!=phase::retired||
               !found->second.observed.incarnation||found->second.cleanup_attempted){return false;}
            found->second.cleanup_attempted=true;found->second.cleanup_error=115;return true;
        }
        void cleanup_result(std::uint64_t source,std::uint32_t error) noexcept
        {auto found{entries_.find(source)};if(found!=entries_.end()&&found->second.cleanup_attempted){found->second.cleanup_error=error;}}
        void transfer_result(std::uint64_t source,std::uint32_t error) noexcept
        {auto found{entries_.find(source)};if(found!=entries_.end()){found->second.last_transfer_error=error;}}
        void cancel_result(std::uint64_t source,std::uint32_t error) noexcept
        {auto found{entries_.find(source)};if(found!=entries_.end()){found->second.cancel_attempted=true;found->second.last_cancel_error=error;}}
        void retire_session(std::uint64_t session,std::uint32_t reason=107) noexcept
        {for(auto& item:entries_){if(item.second.observed.owner.session==session){abandon(item.first,reason);}}}
        void seal(std::uint32_t reason=125) noexcept
        {sealed_=true;for(auto& item:entries_){abandon(item.first,reason);}}
    };
}
