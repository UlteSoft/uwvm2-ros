/** @file
 * @brief Bounded transform sessions with exact mutation replay and owned waits.
 * @details The enclosing broker owns the session and holds complete transform
 * and clock authority leases. This class does not turn a handle, digest, frame
 * or clock number into permission. All requests are copied before admission.
 * One monotonically increasing key stream covers publish/submit/clear. The
 * last accepted mutation has a detached receipt; exact retries never execute
 * it again, even after the resulting query is released. Rejected requests do
 * not consume a key. A new accepted mutation acknowledges the preceding key.
 *
 * Query slots are reserved with recoverable fast_io allocation at configure.
 * Every later operation is allocation-free. Pending queries are evaluated on
 * submission, publication, take, describe and explicit readiness observation.
 * Absolute host-steady deadlines are enforced at those access points. There
 * is no autonomous scheduler or watchdog; inspection does not advance time.
 * Sinks run without this mutex. A reservation and generation fence protect
 * close, clear, retirement, reentrancy and failed/throwing delivery. The broker
 * must also revalidate its complete authority group after an external copy.
 */
#pragma once
#include "transform_cache.h"
#include <uwvm2/utils/container/function_ref.h>
#include <mutex>
#include <optional>

namespace uwvm2::imported::ros::scene::transforms
{
    enum class query_state:std::uint8_t {queued,succeeded,failed,cancelled};
    enum class query_reason:std::uint8_t {none,deadline,requested,history_cleared,clock_changed,retired,invalid_query};
    inline constexpr std::array query_state_names{std::string_view{"queued"},std::string_view{"succeeded"},std::string_view{"failed"},std::string_view{"cancelled"}};
    inline constexpr std::array query_reason_names{std::string_view{},std::string_view{"deadline"},std::string_view{"cancel_requested"},
        std::string_view{"history_cleared"},std::string_view{"clock_changed"},std::string_view{"retired"},std::string_view{"invalid_query"}};
    struct receipt
    {
        resources::handle request{};query_state state{};std::uint64_t generation{};query_reason reason{};
        bool mutation{};
        friend bool operator==(receipt const&,receipt const&)=default;
    };
    struct request_key {resources::handle session{};std::uint64_t sequence{};};
    /** @brief Binding-local finite capacities, distinct from optional rate limits. */
    struct session_plan
    {
        buffers::profile profile;cache_plan history;
        std::size_t requests{};std::uint64_t request_bytes{},deadline_ns{};
    };
    struct session_usage
    {
        resources::handle id{},clock_capability{},clock{};
        cache_usage history;
        std::uint64_t pending{},requests{},request_bytes{},last_sequence{};
        bool retired{};
    };
    inline bool encode_receipt(wire::writer& writer,receipt value) noexcept
    {
        auto state=static_cast<std::size_t>(value.state),reason=static_cast<std::size_t>(value.reason);
        if(!value.request||state>=query_state_names.size()||reason>=query_reason_names.size())return false;
        auto effect=value.state==query_state::queued?"locally_accepted":
            value.state==query_state::succeeded?(value.mutation?"committed":"none"):"none";
        return writer.u64(value.request)&&writer.text(query_state_names[state])&&writer.text(effect)&&writer.u64(value.generation)&&writer.text(query_reason_names[reason]);
    }
    class session
    {
        struct query_slot
        {
            receipt status{};contract::query query{};contract::transform result{};
            std::uint64_t deadline{},fence{};
            bool used{},delivering{},delivered{},has_result{};
        };
        mutable std::mutex mutex_;
        cache history_;
        ::uwvm2::utils::container::std_vector<query_slot> queries_;
        session_plan policy_{};
        resources::handle id_{},clock_capability_{},clock_{};
        std::uint64_t last_sequence_{},fence_{1};
        contract::operation last_action_{};buffers::digest last_digest_{};receipt last_receipt_{};
        bool retired_{};
        std::uint64_t (*steady_)() noexcept;
#if defined(__linux__)
        ::pid_t process_{::getpid()};
#endif
        bool same_process() const noexcept
        {
#if defined(__linux__)
            return process_==::getpid();
#else
            return true;
#endif
        }
        static std::uint64_t steady_now() noexcept
        {auto now=resources::steady_now();return now<0?0:static_cast<std::uint64_t>(now);}
        error live() const noexcept
        {return !id_?error::bad_handle:retired_?error::cancelled:error::success;}
        query_slot* find(resources::handle id) noexcept
        {for(auto& value:queries_)if(value.used&&value.status.request==id)return &value;return nullptr;}
        void update_last(query_slot const& value) noexcept
        {if(last_receipt_.request==value.status.request)last_receipt_=value.status;}
        void finish(query_slot& value,query_state state,query_reason reason) noexcept
        {value.status.state=state;value.status.reason=reason;value.has_result=false;update_last(value);}
        error clock_check(clock_context clock,bool permit_epoch_change=false) const noexcept
        {
            if(clock.identity!=clock_)return error::denied;
            if(!clock.started)return error::again;
            if(!permit_epoch_change&&clock.epoch!=history_.inspect().epoch)return error::conflict;
            return error::success;
        }
        error key_check(contract::operation action,request_key key,buffers::digest const& digest,bool& replay) const noexcept
        {
            replay=false;
            if(key.session!=id_||!key.sequence)return error::denied;
            if(last_sequence_&&key.sequence==last_sequence_)
            {
                if(action!=last_action_||digest!=last_digest_)return error::conflict;
                replay=true;return error::success;
            }
            if(last_sequence_==UINT64_MAX||key.sequence!=last_sequence_+1)return error::conflict;
            return error::success;
        }
        void accepted(contract::operation action,request_key key,buffers::digest digest,receipt value) noexcept
        {last_action_=action;last_sequence_=key.sequence;last_digest_=digest;last_receipt_=value;}
        void refresh(query_slot& value,clock_context clock,std::uint64_t now) noexcept
        {
            if(!value.used||value.status.state!=query_state::queued)return;
            if(clock.epoch!=history_.inspect().epoch){finish(value,query_state::cancelled,query_reason::clock_changed);return;}
            if(now>=value.deadline){finish(value,query_state::failed,query_reason::deadline);return;}
            auto status=history_.lookup(value.query,clock,value.result);
            if(status==error::again)return;
            if(status!=error::success){finish(value,query_state::failed,query_reason::invalid_query);return;}
            value.status.state=query_state::succeeded;value.status.reason=query_reason::none;
            value.status.generation=history_.inspect().generation;value.has_result=true;update_last(value);
        }
        void refresh_all(clock_context clock) noexcept
        {auto now=steady_();for(auto& value:queries_)refresh(value,clock,now);}
    public:
        using delivery_sink=::uwvm2::utils::container::function_ref<bool(receipt const&,contract::transform const*)>;
        explicit session(std::uint64_t (*clock)() noexcept=steady_now):steady_{clock?clock:steady_now}{}
        session(session const&)=delete;session& operator=(session const&)=delete;
        static bool request_storage(session_plan const& plan,std::uint64_t& bytes) noexcept
        {
            constexpr auto max_allocation{SIZE_MAX < static_cast<::std::uint64_t>(INT64_MAX) ? SIZE_MAX : static_cast<::std::uint64_t>(INT64_MAX)};
            if(plan.requests>max_allocation/sizeof(query_slot))return false;
            bytes=plan.requests*sizeof(query_slot);return true;
        }
        /** @brief Prepare both cache and query storage; no automatic clock grant exists. */
        error configure(session_plan const& plan,resources::handle clock_capability,clock_context clock) noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};if(id_)return error::conflict;if(retired_)return error::cancelled;
            std::uint64_t bytes{};
            if(!clock_capability||!plan.profile.identity.size||plan.profile.identity.size>buffers::maximum_identity||
                !wire::utf8({reinterpret_cast<std::uint8_t const*>(plan.profile.identity.bytes.data()),plan.profile.identity.size})||
                plan.profile.identity.view().find('\0')!=std::string_view::npos||!request_storage(plan,bytes)||bytes>plan.request_bytes||
                plan.request_bytes>INT64_MAX||plan.deadline_ns>INT64_MAX||!cache::valid(plan.history))return error::invalid;
            try
            {
                ::uwvm2::utils::container::std_vector<query_slot> queries;queries.resize(plan.requests);
                if(queries.capacity()>plan.request_bytes/sizeof(query_slot))return error::too_large;
                auto identity=resources::allocate_identity();if(!identity)return error::too_large;
                auto result=history_.configure(plan.history,clock);if(result!=error::success)return result;
                queries_=std::move(queries);policy_=plan;id_=identity;clock_capability_=clock_capability;clock_=clock.identity;
                return error::success;
            }
            catch(std::bad_alloc const&){return error::no_memory;}
            catch(std::length_error const&){return error::too_large;}
        }
        /** @brief Snapshot only; it deliberately neither reads steady time nor expires jobs. */
        error inspect(session_usage& out) const noexcept
        {
            if(!same_process())return error::denied;
            std::unique_lock lock{mutex_,std::try_to_lock};if(!lock.owns_lock())return error::again;
            session_usage value{id_,clock_capability_,clock_,history_.inspect(),0,0,queries_.capacity()*sizeof(query_slot),last_sequence_,retired_};
            for(auto const& item:queries_)if(item.used){++value.requests;if(item.status.state==query_state::queued)++value.pending;}
            out=value;return error::success;
        }
        /** @brief Recover the last accepted receipt using origin authority only.
         * @details The broker must compute digest from the complete owned
         * canonical request. This method never re-reads a clock or source.
         * replay=false is not admission to execute: acquire the full group next.
         */
        error replay(contract::operation action,request_key key,buffers::digest const& digest,bool& replay,receipt& out) noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};auto status=live();if(status!=error::success)return status;
            status=key_check(action,key,digest,replay);if(status==error::success&&replay)out=last_receipt_;return status;
        }
        /** @brief Check owned request membership without expiring it or sampling time. */
        error owns_request(resources::handle request,bool& out) noexcept
        {
            if(!same_process())return error::denied;
            std::unique_lock lock{mutex_,std::try_to_lock};if(!lock.owns_lock())return error::again;
            auto status=live();if(status!=error::success)return status;
            out=find(request)!=nullptr;return error::success;
        }
        /** @brief Commit a whole publication or return its retained receipt. */
        error publish(request_key key,buffers::digest digest,std::span<contract::transform const> input,clock_context clock,bool replace_static,receipt& out,std::uint64_t expected_generation=0) noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};auto status=live();if(status!=error::success)return status;
            bool replay{};status=key_check(contract::operation::publish,key,digest,replay);if(status!=error::success)return status;
            if(replay){out=last_receipt_;return error::success;}
            auto id=resources::allocate_identity();if(!id)return error::too_large;
            status=history_.publish(input,clock,replace_static,expected_generation);if(status!=error::success)return status;
            receipt value{id,query_state::succeeded,history_.inspect().generation,query_reason::none,true};
            accepted(contract::operation::publish,key,digest,value);refresh_all(clock);out=value;return error::success;
        }
        error lookup(contract::query const& query,clock_context clock,contract::transform& out) noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};auto status=live();if(status!=error::success)return status;
            return history_.lookup(query,clock,out);
        }
        error velocity(contract::velocity_query const& query,clock_context clock,math::velocity& out) noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};auto status=live();if(status!=error::success)return status;
            return history_.velocity(query,clock,out);
        }
        /** @brief Reserve a finite owned query with a positive absolute steady deadline. */
        error submit(request_key key,buffers::digest digest,contract::query const& query,std::uint64_t deadline,clock_context clock,receipt& out) noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};auto status=live();if(status!=error::success)return status;
            bool replay{};status=key_check(contract::operation::submit,key,digest,replay);if(status!=error::success)return status;
            if(replay){out=last_receipt_;return error::success;}
            status=clock_check(clock);if(status!=error::success)return status;
            auto now=steady_();if(!deadline||deadline>INT64_MAX||deadline<=now)return error::timeout;
            if(deadline-now>policy_.deadline_ns)return error::too_large;
            query_slot* destination{};for(auto& value:queries_)if(!value.used){destination=&value;break;}
            if(!destination)return error::too_large;
            query_slot staged;staged.query=query;staged.deadline=deadline;staged.fence=fence_;staged.used=true;
            status=history_.lookup(query,clock,staged.result);
            if(status!=error::success&&status!=error::again)return status;
            auto id=resources::allocate_identity();if(!id)return error::too_large;
            staged.status={id,status==error::success?query_state::succeeded:query_state::queued,history_.inspect().generation,query_reason::none,false};
            staged.has_result=status==error::success;*destination=staged;
            accepted(contract::operation::submit,key,digest,staged.status);out=staged.status;return error::success;
        }
        /** @brief Deliver one terminal query result without consuming failed copies.
         * @details Queued work returns again. Reentrant take/release on the
         * reserved slot returns again. Clear/retire during the sink fences the
         * result; bytes already copied must be ignored unless the call succeeds.
         */
        error take(resources::handle request,clock_context clock,delivery_sink deliver) noexcept
        {
            if(!same_process())return error::denied;
            std::unique_lock lock{mutex_};auto status=live();if(status!=error::success)return status;
            if(!deliver)return error::invalid;
            status=clock_check(clock);if(status!=error::success)return status;
            auto* slot=find(request);if(!slot)return error::bad_handle;
            if(slot->delivering||slot->delivered)return error::again;
            refresh(*slot,clock,steady_());if(slot->status.state==query_state::queued)return error::again;
            auto receipt=slot->status;auto result=slot->result;auto has_result=slot->has_result;auto fence=fence_;
            slot->delivering=true;lock.unlock();bool copied{};
            try{copied=deliver(receipt,has_result?&result:nullptr);}catch(...){copied=false;}
            lock.lock();slot->delivering=false;
            if(retired_||fence!=fence_||slot->fence!=fence)return error::cancelled;
            if(!copied)return error::io;
            slot->delivered=true;slot->has_result=false;return error::success;
        }
        /** @brief Cancel only a still-pending query; completed outcomes remain known. */
        error cancel(resources::handle request,receipt& out) noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};auto status=live();if(status!=error::success)return status;
            auto* slot=find(request);if(!slot)return error::bad_handle;
            if(slot->status.state==query_state::queued)finish(*slot,query_state::cancelled,query_reason::requested);
            out=slot->status;return error::success;
        }
        error describe(resources::handle request,clock_context clock,receipt& out) noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};auto status=live();if(status!=error::success)return status;
            status=clock_check(clock);if(status!=error::success)return status;
            auto* slot=find(request);if(!slot)return error::bad_handle;
            refresh(*slot,clock,steady_());out=slot->status;return error::success;
        }
        /** @brief Describe completion without reading a source clock or transform.
         * @details Generic request.describe needs only origin authority. It
         * may enforce the recorded host-steady deadline, but cannot resolve a
         * pending query using new frame data. Cleanup receipts remain available
         * after a source capability or resource is closed.
         */
        error receipt_for(resources::handle request,receipt& out) noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};auto status=live();if(status!=error::success)return status;
            auto* slot=find(request);if(!slot)return error::bad_handle;
            if(slot->status.state==query_state::queued&&steady_()>=slot->deadline)finish(*slot,query_state::failed,query_reason::deadline);
            out=slot->status;return error::success;
        }
        /** @brief Release terminal query storage; a last-key receipt remains detached. */
        error release(resources::handle request) noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};auto status=live();if(status!=error::success)return status;
            auto* slot=find(request);if(!slot)return error::bad_handle;
            if(slot->delivering||slot->status.state==query_state::queued)return error::again;
            update_last(*slot);*slot={};return error::success;
        }
        /** @brief Explicit clock-epoch acknowledgement and invalidation of outstanding output. */
        error clear(request_key key,buffers::digest digest,std::uint64_t generation,clock_context clock,receipt& out) noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};auto status=live();if(status!=error::success)return status;
            bool replay{};status=key_check(contract::operation::clear,key,digest,replay);if(status!=error::success)return status;
            if(replay){out=last_receipt_;return error::success;}
            if(fence_==UINT64_MAX)return error::too_large;
            auto id=resources::allocate_identity();if(!id)return error::too_large;
            status=history_.clear(clock,generation);if(status!=error::success)return status;
            ++fence_;
            for(auto& value:queries_)if(value.used&&!value.delivered)
            {finish(value,query_state::cancelled,query_reason::history_cleared);value.fence=fence_;}
            receipt value{id,query_state::succeeded,history_.inspect().generation,query_reason::none,true};
            accepted(contract::operation::clear,key,digest,value);out=value;return error::success;
        }
        /** @brief Observe level readiness with current authority/clock; no callback or allocation. */
        error ready(clock_context clock,bool& out) noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};auto status=live();if(status!=error::success)return status;
            status=clock_check(clock);if(status!=error::success)return status;
            refresh_all(clock);bool ready{};
            for(auto const& value:queries_)if(value.used&&!value.delivered&&value.status.state!=query_state::queued)ready=true;
            out=ready;return error::success;
        }
        /** @brief Nonblocking local retirement; no native effects or automatic reactivation. */
        void retire() noexcept
        {
            if(!same_process())return;
            std::lock_guard lock{mutex_};retired_=true;
            for(auto& value:queries_)if(value.used&&!value.delivered)finish(value,query_state::cancelled,query_reason::retired);
        }
    };
}
