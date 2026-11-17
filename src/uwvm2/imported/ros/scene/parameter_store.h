/** @file
 * @brief Local parameter scopes with finite authority-independent reservations.
 * @details A scope is a local transactional store attached to one admitted node
 * lifetime. It does not impersonate native parameter services. The broker must
 * hold exact parameter and node capability leases for every operation. All
 * payload storage uses recoverable fast_io allocation; no guest pointer or
 * native node pointer is retained. ROS1 and ROS2 value algebras remain separate.
 *
 * Default capacities are zero. Data, pending candidates, event retention,
 * request receipts and transient workspace are separate finite reservations;
 * none is a rate limit. An absent optional rate budget is not interpreted here.
 * Cancellation and delivery callbacks execute outside the registry mutex.
 * Parent-node live fencing is sampled at the final local commit point. A later
 * concurrent parent shutdown cannot retroactively undo an already committed
 * local transaction. Closing a scope destroys local state and cancels pending
 * validation; it never changes a native parameter or actuator.
 */
#pragma once
#include "parameter_contract.h"
#include "communication.h"
#include <uwvm2/utils/container/standard.h>
#include <uwvm2/utils/container/shared.h>
#include <uwvm2/utils/container/unique.h>
#include <uwvm2/utils/container/function_ref.h>
#include <algorithm>
#include <array>
#include <chrono>
#include <limits>
#include <memory>
#include <mutex>
#include <span>
#include <string_view>
#include <utility>
#if defined(__linux__)
#include <unistd.h>
#endif

namespace uwvm2::imported::ros::scene::parameters
{
    namespace contract=::uwvm2::imported::ros::parameter_contract;
    namespace container=::uwvm2::utils::container;
    using error=resources::error;
    using handle=resources::handle;
    using operation=contract::operation;
    using request=contract::request;
    using bytes=wire::bytes;
    using byte_vector=container::std_vector<std::uint8_t>;
    using sink=container::function_ref<bool(bytes)>;
    using cancellation=container::function_ref<bool()>;
    using final_admission=container::function_ref<error(std::uint64_t)>;
    using clock_source=std::uint64_t(*)() noexcept;
    inline std::uint64_t steady_now() noexcept
    {auto now=resources::steady_now();return now<0?0:static_cast<std::uint64_t>(now);}
    enum class receipt_state : std::uint8_t {queued,succeeded,failed,cancelled};
    enum class receipt_effect : std::uint8_t {none,locally_accepted,committed,partial};
    enum class reason : std::uint8_t
    {none,missing,read_only,type,range,revision,validation_pending,validation_rejected,validation_timeout,cancelled,empty_request,capacity};
    inline constexpr std::array<std::string_view,4> state_names{"queued","succeeded","failed","cancelled"};
    inline constexpr std::array<std::string_view,4> effect_names{"none","locally_accepted","committed","partial"};
    inline constexpr std::array<std::string_view,12> reason_names{
        "","missing","read_only","type_mismatch","range","revision","validation_pending","validation_rejected","validation_timeout","cancelled","empty_request","capacity"};
    struct receipt
    {
        handle request{},generation{};receipt_state state{receipt_state::failed};receipt_effect effect{};reason why{};
        buffers::text<contract::maximum_metadata> explanation;
    };
    struct decision
    {buffers::text<contract::maximum_name> name;bool accepted{};reason why{};};
    enum class change : std::uint8_t {declare,undeclare,set};
    enum class request_operation : std::uint8_t {describe,release};
    inline constexpr std::array<std::string_view,3> change_names{"declare","undeclare","set"};
    /** @brief Exact trusted key constraint; guest descriptors may only narrow it.
     * @details Type bits are the canonical parameter_value tags. A deployment
     * override is an already encoded value selected at declaration, never an
     * ambient YAML/file lookup. Free-text descriptions cannot relax constraints.
     */
    struct key_policy
    {
        buffers::text<contract::maximum_name> name;
        std::uint64_t types{};
        bool read_only{},dynamic_type{},integer_constraint{},floating_constraint{},has_override{};
        parameter_values::integer_range integers;
        parameter_values::floating_range floating;
        byte_vector override_value;
        bool fixed_descriptor{};
        contract::descriptor descriptor;
    };
    /** @brief Explicit finite capacities and immutable profile selection.
     * @details Limits aggregate over all scopes on one alias. Only metadata
     * slots preallocated per scope multiply by max_scopes; configuration checks
     * this product. max_parameters bounds declared objects, while max_total_bytes
     * counts canonical current value bytes. max_receipt_bytes counts retained
     * exact request/response bytes. Event payloads and pending candidate values
     * have separate budgets. Workspace covers staging and detached output.
     */
    struct plan
    {
        buffers::profile selected,node_profile;
        parameter_values::limits values;
        container::std_vector<key_policy> keys; // Strictly increasing canonical names; duplicate/unsorted profiles are rejected.
        std::uint64_t max_scopes{},max_parameters{},max_total_bytes{},max_events{},max_event_bytes{},max_cursors{},
            max_workspace{},max_batch{},max_request_bytes{},max_receipt_bytes{},max_pending_bytes{},max_cursor_bytes{},
            validation_timeout_ns{},cursor_timeout_ns{};
        bool deferred_validation{};
        double floating_tolerance{};
        /** @brief Explicit ROS1 ancestor-search boundary; both empty/zero disable search.
         * @details max_search_depth counts candidate keys, including the start
         * namespace. Each candidate still requires an exact key policy before
         * its existence can be observed. Exhaustion is an error, not not-found.
         */
        buffers::text<contract::maximum_name> search_root;
        std::uint64_t max_search_depth{};
    };
    struct usage
    {std::uint64_t scopes{},parameters{},bytes{},events{},event_bytes{},cursors{},workspace{},receipt_bytes{},pending_bytes{},cursor_bytes{},retained_scopes{};};
    struct dependency
    {handle capability{},node{};communication::owner authority;};
    inline bool add(std::uint64_t left,std::uint64_t right,std::uint64_t& output) noexcept
    {if(right>UINT64_MAX-left)return false;output=left+right;return true;}
    inline bool permitted_type(parameter_values::algebra algebra,parameter_values::kind type) noexcept
    {
        auto n=static_cast<std::uint64_t>(type);
        return algebra==parameter_values::algebra::ros2?n<=9:algebra==parameter_values::algebra::ros1&&((n>=1&&n<=5)||(n>=32&&n<=34));
    }
    /** @brief Reject unknown types, unlimited sentinels and inconsistent policy. */
    inline bool valid(plan const& value) noexcept
    {
        auto text_valid=[](auto const& text)
        {return text.size&&text.size<=text.bytes.size()&&text.view().find('\0')==std::string_view::npos&&
            wire::utf8({reinterpret_cast<std::uint8_t const*>(text.bytes.data()),text.size});};
        if(value.selected.identity.size>buffers::maximum_identity||value.node_profile.identity.size>buffers::maximum_identity||
           !text_valid(value.selected.identity)||!text_valid(value.node_profile.identity)||!parameter_values::valid(value.values)||
           value.keys.size()>65536||value.max_scopes>1024||value.max_parameters>65536||value.max_total_bytes>(1ull<<30)||
           value.max_events>65536||value.max_event_bytes>(1ull<<30)||value.max_cursors>4096||value.max_workspace>(1ull<<30)||
           value.max_batch>contract::maximum_batch||value.max_request_bytes>4u*1024u*1024u||value.max_receipt_bytes>(1ull<<30)||
           value.max_pending_bytes>(1ull<<30)||value.max_cursor_bytes>(1ull<<30)||value.validation_timeout_ns>INT64_MAX||value.cursor_timeout_ns>INT64_MAX||
           !std::isfinite(value.floating_tolerance)||value.floating_tolerance<0||
           (value.deferred_validation&&!value.validation_timeout_ns)||
           (value.max_cursors&&!value.cursor_timeout_ns)||value.max_scopes*(value.max_events+value.max_cursors)>1048576)return false;
        if(value.search_root.size>value.search_root.bytes.size()||value.max_search_depth>256||
            bool(value.search_root.size)!=bool(value.max_search_depth)||
            (value.max_search_depth&&(value.values.selected!=parameter_values::algebra::ros1||
             (value.search_root.view()!="/"&&!communication::absolute_name(value.search_root.view())))))return false;
        std::uint64_t mask{};
        for(auto type:contract::type_kinds)if(permitted_type(value.values.selected,type))mask|=std::uint64_t{1}<<static_cast<unsigned>(type);
        for(std::size_t i{};i!=value.keys.size();++i)
        {
            auto const& key=value.keys[i];
            if(!text_valid(key.name)||(i&&value.keys[i-1].name.view()>=key.name.view())||!key.types||(key.types&~mask)||
               (key.integer_constraint&&!key.integers.valid())||(key.floating_constraint&&!key.floating.valid())||
               (key.integer_constraint&&!(key.types&(1ull<<2)))||(key.floating_constraint&&!(key.types&(1ull<<3))))return false;
            if(key.has_override)
            {parameter_values::scalar scalar;if(!parameter_values::validate(key.override_value,value.values,scalar)||
                !(key.types&(1ull<<static_cast<unsigned>(scalar.type))))return false;}
            else if(!key.override_value.empty())return false;
            if(key.fixed_descriptor)
            {
                auto const& d=key.descriptor;auto type=static_cast<unsigned>(d.declared_type);
                if(d.name!=key.name||type>63||!(key.types&(1ull<<type))||d.description.size>contract::maximum_metadata||
                   d.additional_constraints.size>contract::maximum_metadata||d.description.view().find('\0')!=std::string_view::npos||
                   d.additional_constraints.view().find('\0')!=std::string_view::npos||d.read_only!=key.read_only||d.dynamic_type!=key.dynamic_type||
                   (d.has_range&&(d.floating_range?d.declared_type!=parameter_values::kind::floating||!d.floating.valid():
                       d.declared_type!=parameter_values::kind::integer||!d.integers.valid())))return false;
                for(auto const* metadata:{&d.description,&d.additional_constraints})
                    if(!wire::utf8({reinterpret_cast<std::uint8_t const*>(metadata->bytes.data()),metadata->size}))return false;
            }
        }
        return true;
    }
    inline bool write_receipt(wire::writer& output,receipt const& value) noexcept
    {return output.u64(value.request)&&output.text(state_names[static_cast<unsigned>(value.state)])&&
        output.text(effect_names[static_cast<unsigned>(value.effect)])&&output.u64(value.generation)&&
        output.text(value.explanation.size?value.explanation.view():reason_names[static_cast<unsigned>(value.why)]);}
    inline bool write_decision(wire::writer& output,decision const& value) noexcept
    {return output.text(value.name.view())&&output.boolean(value.accepted)&&output.text(reason_names[static_cast<unsigned>(value.why)]);}

    /** @brief Exact owned allocation, movable without allocation at commit. */
    struct owned_bytes
    {
        container::std_unique_ptr<std::uint8_t[]> storage;
        std::size_t extent{};
        owned_bytes()=default;
        owned_bytes(owned_bytes const&)=delete;
        owned_bytes& operator=(owned_bytes const&)=delete;
        owned_bytes(owned_bytes&& other) noexcept:storage(std::move(other.storage)),extent(std::exchange(other.extent,0)){}
        owned_bytes& operator=(owned_bytes&& other) noexcept
        {if(this!=&other){storage=std::move(other.storage);extent=std::exchange(other.extent,0);}return *this;}
        bytes view() const noexcept{return {storage.get(),extent};}
        std::span<std::uint8_t> writable() noexcept{return {storage.get(),extent};}
        static owned_bytes allocate(std::size_t size)
        {
            owned_bytes value;
            if(size)value.storage=container::make_unique_for_overwrite<std::uint8_t[]>(size);
            value.extent=size;return value;
        }
        static owned_bytes copy(bytes source)
        {auto value=allocate(source.size());if(!source.empty())std::memcpy(value.storage.get(),source.data(),source.size());return value;}
    };
    struct capacity_account
    {std::atomic<std::uint64_t> used{};std::uint64_t maximum{};};
    /** @brief Independent finite reservation released after its owned allocation.
     * @details The exact allocation is freed before its reservation is released.
     * This token outlives registry teardown and never calls a native provider.
     */
    class capacity_lease
    {
        friend class registry;
        std::shared_ptr<capacity_account> account;
        std::uint64_t amount_{};
    public:
        capacity_lease()=default;
        capacity_lease(capacity_lease const&)=delete;
        capacity_lease& operator=(capacity_lease const&)=delete;
        capacity_lease(capacity_lease&& other) noexcept:account(std::move(other.account)),amount_(std::exchange(other.amount_,0)){}
        capacity_lease& operator=(capacity_lease&& other) noexcept
        {if(this!=&other){reset();account=std::move(other.account);amount_=std::exchange(other.amount_,0);}return *this;}
        ~capacity_lease(){reset();}
        void reset() noexcept{if(account){account->used.fetch_sub(amount_,std::memory_order_release);account.reset();amount_=0;}}
        explicit operator bool() const noexcept{return bool(account);}
        std::uint64_t size() const noexcept{return amount_;}
    };

    using workspace_lease=capacity_lease;

    /** @brief Registry owned by one scene environment; configuration is immutable.
     * @details Lock order is authority leases then this registry. It never
     * enters the communication registry. A successfully attached move-only
     * node_lease is consumed at open and its live fence is checked locally.
     * The registry must outlive calls; teardown first fences admission and joins
     * callers. Delivery/cancellation code cannot observe internal allocations.
     */
    class registry
    {
        struct binding
        {
            container::std_string alias;
            plan policy;
            usage used;
            std::shared_ptr<capacity_account> workspace,event_slots,event_bytes,pending_bytes,cursor_bytes,scope_slots;
            bool retired{};
        };
        struct entry
        {contract::descriptor descriptor;owned_bytes value;parameter_values::scalar scalar;std::size_t policy_index{};};
        struct event_item
        {buffers::text<contract::maximum_name> name;owned_bytes value;};
        struct event
        {
            capacity_lease slot_capacity,payload_capacity;
            handle identity{},request{};
            change changed{};
            std::uint64_t revision{},deadline{},payload_bytes{};
            bool validation{},claimed{};
            container::std_vector<event_item> items;
        };
        struct change_item
        {
            buffers::text<contract::maximum_name> name;
            owned_bytes value;
            parameter_values::scalar scalar;
            bool accepted{};
        };
        struct pending
        {
            capacity_lease payload_capacity;
            handle request{};
            std::uint64_t revision{},deadline{},payload_bytes{};
            operation action{};
            std::array<decision,contract::maximum_batch> decisions{};
            std::size_t decision_count{};
            container::std_vector<change_item> changes;
            std::shared_ptr<event> validation,committed;
            bool delivered{};
        };
        struct filter
        {
            capacity_lease payload_capacity;
            owned_bytes canonical;
            std::array<std::string_view,contract::maximum_batch> prefixes{};
            std::size_t count{};
            std::uint64_t depth{};
        };
        struct cursor
        {
            handle identity{};
            std::uint64_t revision{},expiry{};
            std::shared_ptr<filter> selected;
            buffers::text<contract::maximum_name> next_name;
        };
        struct scope
        {
            capacity_lease slot_capacity;
            handle identity{},node_capability{};
            std::shared_ptr<binding> owner;
            communication::node_lease node;
            std::uint64_t revision{1},value_bytes{};
            bool published{},retired{},busy{};
            container::std_map<container::std_string,entry,std::less<>> entries;
            container::std_vector<std::shared_ptr<event>> events;
            container::std_vector<cursor> cursors;
            contract::request_key previous_key;
            operation previous_action{};
            owned_bytes previous_request,previous_response;
            receipt previous_receipt;
            std::array<decision,contract::maximum_batch> previous_decisions{};
            std::size_t previous_decision_count{};
            bool previous_accepted{},has_receipt{},request_released{};
            std::shared_ptr<pending> waiting;
        };
        mutable std::mutex mutex;
        container::std_map<container::std_string,std::shared_ptr<binding>,std::less<>> bindings;
        container::std_map<handle,std::shared_ptr<scope>> scopes;
        clock_source clock{steady_now};
        bool retired{};
#if defined(__linux__)
        ::pid_t process{::getpid()};
#endif
        bool same_process() const noexcept
        {
#if defined(__linux__)
            return process==::getpid();
#else
            return true;
#endif
        }
        std::shared_ptr<binding> find_binding(std::string_view alias) const noexcept
        {auto found=bindings.find(alias);return retired||found==bindings.end()||found->second->retired?nullptr:found->second;}
        std::shared_ptr<scope> find_scope(std::string_view alias,handle identity) const noexcept
        {
            auto found=scopes.find(identity);
            if(retired||found==scopes.end())return {};
            auto const& value=found->second;
            if(value->retired||!value->published||value->owner->retired||value->owner->alias!=alias||!value->node.live())return {};
            return value;
        }
        static error reserve(std::shared_ptr<capacity_account> const& account,std::uint64_t amount,capacity_lease& output) noexcept
        {
            if(output)return error::invalid;
            auto used=account->used.load(std::memory_order_relaxed);
            for(;;)
            {
                if(amount>account->maximum||used>account->maximum-amount)return error::again;
                if(account->used.compare_exchange_weak(used,used+amount,std::memory_order_acq_rel,std::memory_order_relaxed))break;
            }
            output.account=account;output.amount_=amount;return error::success;
        }
        static std::size_t key_index(plan const& policy,std::string_view name) noexcept
        {
            auto found=std::lower_bound(policy.keys.begin(),policy.keys.end(),name,
                [](key_policy const& key,std::string_view text){return key.name.view()<text;});
            return found!=policy.keys.end()&&found->name.view()==name?static_cast<std::size_t>(found-policy.keys.begin()):policy.keys.size();
        }
        static reason constraint(key_policy const& key,contract::descriptor const& descriptor,parameter_values::scalar scalar) noexcept
        {
            auto kind=static_cast<unsigned>(scalar.type);
            if(kind>63||!(key.types&(std::uint64_t{1}<<kind)))return reason::type;
            if(scalar.type!=parameter_values::kind::not_set&&!descriptor.dynamic_type&&scalar.type!=descriptor.declared_type)return reason::type;
            if(scalar.type==parameter_values::kind::integer&&
               ((key.integer_constraint&&!key.integers.contains(scalar.integer))||
                (descriptor.has_range&&(!descriptor.floating_range&&!descriptor.integers.contains(scalar.integer)))))return reason::range;
            if(scalar.type==parameter_values::kind::floating&&
               ((key.floating_constraint&&!key.floating.contains(scalar.floating))||
                (descriptor.has_range&&(descriptor.floating_range&&!descriptor.floating.contains(scalar.floating)))))return reason::range;
            return reason::none;
        }
        /** @brief Keep configured metadata and constrain guest descriptors monotonically.
         * @details ROS1 dynamic parameters may declare any explicitly allowed
         * root kind; ROS2's configured declaration type remains fixed. Both
         * algebras still validate each actual value against the complete type
         * mask and both numeric constraints. Free-text metadata cannot replace
         * the deployment's exact descriptor spelling.
         */
        static bool descriptor_permitted(key_policy const& key,contract::descriptor const& candidate,parameter_values::algebra algebra) noexcept
        {
            if(!key.fixed_descriptor)return true;
            auto const& fixed=key.descriptor;
            if(candidate.name!=fixed.name||candidate.description!=fixed.description||candidate.additional_constraints!=fixed.additional_constraints||
               (candidate.declared_type!=fixed.declared_type&&!(algebra==parameter_values::algebra::ros1&&key.dynamic_type))||
               (!candidate.read_only&&fixed.read_only)||(candidate.dynamic_type&&!fixed.dynamic_type))return false;
            if(!fixed.has_range)return true;
            if(!candidate.has_range||candidate.floating_range!=fixed.floating_range)return false;
            if(fixed.floating_range)
                return candidate.floating.minimum>=fixed.floating.minimum&&candidate.floating.maximum<=fixed.floating.maximum&&
                    (fixed.floating.step==0||std::abs(candidate.floating.step)==std::abs(fixed.floating.step))&&
                    fixed.floating.contains(candidate.floating.minimum)&&fixed.floating.contains(candidate.floating.maximum);
            auto const& narrow=candidate.integers;auto const& broad=fixed.integers;
            return narrow.minimum>=broad.minimum&&narrow.maximum<=broad.maximum&&broad.contains(narrow.minimum)&&broad.contains(narrow.maximum)&&
                (!broad.step||(narrow.minimum==narrow.maximum)||(narrow.step&&narrow.step%broad.step==0));
        }
        static bool write_item(wire::writer& output,std::string_view name,bytes value) noexcept
        {return output.text(name)&&output.encoded(value);}
        static bool write_event(wire::writer& output,event const& value) noexcept
        {
            if(!output.u64(value.request)||!output.text(value.validation?"validate":"committed")||
               !output.text(change_names[static_cast<unsigned>(value.changed)])||!output.boolean(true)||!output.u64(value.revision)||
               !output.u64(value.items.size()))return false;
            for(auto const& item:value.items)if(!write_item(output,item.name.view(),item.value.view()))return false;
            return output.u64(value.deadline);
        }
        static bool write_mutation(wire::writer& output,operation action,receipt const& value,
                                   std::span<decision const> decisions,bool accepted) noexcept
        {
            if(!write_receipt(output,value))return false;
            if(action==operation::set_atomic)return output.boolean(accepted);
            if(action!=operation::set)return true;
            if(!output.u64(decisions.size()))return false;
            for(auto const& decision:decisions)if(!write_decision(output,decision))return false;
            return true;
        }
        /** @brief Reserve one complete event before allocating any retained payload. */
        static error make_event(std::shared_ptr<binding> const& owner,handle request_id,change changed,
                                std::uint64_t revision,std::uint64_t deadline,bool validation,
                                std::span<contract::item_view const> items,std::shared_ptr<event>& output)
        {
            if(items.size()>contract::maximum_batch)return error::too_large;
            std::uint64_t bytes=sizeof(event)+items.size()*sizeof(event_item);
            for(auto const& item:items)
                if(item.name.empty()||item.name.size()>contract::maximum_name||!add(bytes,item.value.size(),bytes))return error::invalid;
            capacity_lease slot,payload;
            auto status=reserve(owner->event_slots,1,slot);if(status!=error::success)return status;
            status=reserve(owner->event_bytes,bytes,payload);if(status!=error::success)return status;
            auto value=container::make_shared<event>();value->slot_capacity=std::move(slot);value->payload_capacity=std::move(payload);
            value->identity=resources::allocate_identity();if(!value->identity)return error::too_large;
            value->request=request_id;value->changed=changed;value->revision=revision;value->deadline=deadline;
            value->validation=validation;value->payload_bytes=bytes;value->items.resize(items.size());
            for(std::size_t i{};i!=items.size();++i)
            {value->items[i].name.assign(items[i].name);value->items[i].value=owned_bytes::copy(items[i].value);}
            output=std::move(value);return error::success;
        }
        /** @brief A queued validator never counts as an accepted/committed decision. */
        static void pending_decisions(std::span<decision> decisions) noexcept
        {for(auto& value:decisions)if(value.accepted){value.accepted=false;value.why=reason::validation_pending;}}
        static void rejected_decisions(std::span<decision> decisions,reason why) noexcept
        {for(auto& value:decisions)if(value.accepted||value.why==reason::validation_pending){value.accepted=false;value.why=why;}}
        static void erase_event(scope& value,std::shared_ptr<event> const& target) noexcept
        {auto found=std::find(value.events.begin(),value.events.end(),target);if(found!=value.events.end())value.events.erase(found);}
        /** @brief Finalize one expired validation only after reply capacity checks.
         * @details Deadline enforcement is lazy on an admitted operation. This
         * pure local store is not a watchdog. Inspection and rejected/short
         * output preflight do not advance deadlines or consume notifications.
         */
        static void expire(scope& value,std::uint64_t now) noexcept
        {
            if(!value.waiting||now<value.waiting->deadline)return;
            erase_event(value,value.waiting->validation);
            value.previous_receipt.state=receipt_state::cancelled;value.previous_receipt.effect=receipt_effect::none;
            value.previous_receipt.why=reason::validation_timeout;value.previous_accepted=false;
            rejected_decisions({value.previous_decisions.data(),value.previous_decision_count},reason::validation_timeout);
            value.waiting.reset();
        }
        /** @brief Return the latest receipt for the exact last canonical mutation.
         * @details Queued requests become terminal in this same ledger. Retrying
         * their original bytes observes the terminal receipt without applying
         * the values twice. A different body with the same key is a conflict.
         */
        error replay(operation action,std::string_view alias,request const& input,bytes canonical,std::uint64_t capacity,
                     sink deliver,cancellation cancelled,final_admission admit,std::uint64_t& required)
        {
            capacity_lease workspace;owned_bytes result;std::shared_ptr<scope> value;
            auto now=clock();bool timed_out{};
            {
                std::lock_guard lock{mutex};value=find_scope(alias,input.scope);if(!value)return error::bad_handle;
                if(!value->has_receipt||input.key!=value->previous_key||action!=value->previous_action||
                   !std::ranges::equal(canonical,value->previous_request.view()))return error::conflict;
                auto receipt=value->previous_receipt;
                auto decisions=value->previous_decisions;auto accepted=value->previous_accepted;
                timed_out=value->waiting&&now>=value->waiting->deadline;
                if(timed_out)
                {
                    receipt.state=receipt_state::cancelled;receipt.effect=receipt_effect::none;receipt.why=reason::validation_timeout;accepted=false;
                    rejected_decisions({decisions.data(),value->previous_decision_count},reason::validation_timeout);
                }
                auto encode=[&](wire::writer& output){return write_mutation(output,action,receipt,{decisions.data(),value->previous_decision_count},accepted);};
                auto measured=wire::writer::measuring(std::numeric_limits<std::size_t>::max());if(!encode(measured))return error::too_large;
                required=measured.size();if(capacity<required)return error::too_large;
                auto status=reserve(value->owner->workspace,required,workspace);if(status!=error::success)return status;
                result=owned_bytes::allocate(static_cast<std::size_t>(required));wire::writer output{result.writable()};if(!encode(output))return error::io;
            }
            auto status=admit_result(cancelled,admit,required);if(status!=error::success)return status;
            auto final_now=clock();
            {
                std::unique_lock lock{mutex,std::try_to_lock};if(!lock)return error::again;
                if(retired||value->retired||value->owner->retired||!value->node.live())return error::cancelled;
                if(value->previous_key!=input.key)return error::conflict;
                // A concurrent response may have finalized this exact request.
                // An already encoded queued snapshot remains a valid read;
                // a predicted timeout must not overwrite a completed response.
                if(timed_out&&!value->waiting&&value->previous_receipt.why!=reason::validation_timeout)return error::again;
                if(timed_out&&value->waiting)expire(*value,final_now);
            }
            return deliver(result.view())?error::success:error::io;
        }
        static error admit_result(cancellation cancelled,final_admission admit,std::uint64_t size)
        {
            if(cancelled&&cancelled())return error::cancelled;
            if(admit){auto result=admit(size);if(result!=error::success)return result;}
            return cancelled&&cancelled()?error::cancelled:error::success;
        }
        /** @brief Release an unpublished open outside all native callbacks. */
        void discard_open(std::shared_ptr<scope> const& value) noexcept
        {
            if(!same_process())return;
            std::lock_guard lock{mutex};auto found=scopes.find(value->identity);
            if(found!=scopes.end()&&found->second==value)
            {value->retired=true;--value->owner->used.scopes;scopes.erase(found);}
        }
        error open_scope(std::string_view alias,request const& input,std::uint64_t grant_limit,std::uint64_t capacity,
                         sink deliver,cancellation cancelled,final_admission admit,std::uint64_t& required,communication::node_lease& attachment)
        {
            std::shared_ptr<scope> value;
            {
                std::lock_guard lock{mutex};auto owner=find_binding(alias);
                if(!owner)return error::denied;
                if(owner->policy.selected!=input.profile||!input.node_capability||!input.node||!attachment||
                   attachment.identity()!=input.node||!attachment.authority()||!attachment.live())return error::denied;
                required=8;if(capacity<required)return error::too_large;
                if(owner->scope_slots->used.load(std::memory_order_acquire)>=std::min(owner->policy.max_scopes,grant_limit))return error::again;
                capacity_lease slot;auto reserved=reserve(owner->scope_slots,1,slot);if(reserved!=error::success)return reserved;
                value=container::make_shared<scope>();value->slot_capacity=std::move(slot);value->owner=owner;value->node_capability=input.node_capability;
                value->identity=resources::allocate_identity();if(!value->identity)return error::too_large;
                value->events.reserve(static_cast<std::size_t>(owner->policy.max_events));
                value->cursors.reserve(static_cast<std::size_t>(owner->policy.max_cursors));
                scopes.emplace(value->identity,value);
                ++owner->used.scopes;
                // All potentially throwing preparation is complete before the
                // native lifetime attachment moves under this registry lock.
                value->node=std::move(attachment);
            }
            struct rollback
            {
                registry* owner;std::shared_ptr<scope> value;bool committed{};
                ~rollback(){if(!committed)owner->discard_open(value);}
            } guard{this,value};
            auto status=admit_result(cancelled,admit,required);if(status!=error::success)return status;
            {
                std::unique_lock lock{mutex,std::try_to_lock};if(!lock)return error::again;
                if(retired||value->owner->retired||value->retired||!value->node.live())return error::cancelled;
            }
            std::array<std::uint8_t,8> response{};wire::writer output{response};output.u64(value->identity);
            if(!deliver(response))return error::io;
            if(cancelled&&cancelled())return error::cancelled;
            {
                std::unique_lock lock{mutex,std::try_to_lock};if(!lock)return error::again;
                if(retired||value->owner->retired||value->retired||!value->node.live())return error::cancelled;
                value->published=true;guard.committed=true;
            }
            return error::success;
        }
        /** @brief Produce one detached coherent read before releasing the lock. */
        error read_scope(operation action,std::string_view alias,request const& input,std::uint64_t capacity,
                         sink deliver,cancellation cancelled,final_admission admit,std::uint64_t& required)
        {
            capacity_lease workspace;
            owned_bytes result;
            std::shared_ptr<scope> value;
            {
                std::lock_guard lock{mutex};value=find_scope(alias,input.scope);if(!value)return error::bad_handle;
                auto& policy=value->owner->policy;
                if(input.name_count>policy.max_batch||input.name_count>contract::maximum_batch)return error::too_large;
                for(std::size_t i{};i!=input.name_count;++i)
                {
                    if(key_index(policy,input.names[i])==policy.keys.size())return error::denied;
                    if(!value->entries.contains(input.names[i]))return error::bad_handle;
                }
                auto encode=[&](wire::writer& output)
                {
                    if(!output.u64(input.name_count))return false;
                    for(std::size_t i{};i!=input.name_count;++i)
                    {
                        auto const& entry=value->entries.find(input.names[i])->second;
                        if(action==operation::get)
                        {if(!write_item(output,input.names[i],entry.value.view()))return false;}
                        else if(!contract::write_descriptor(output,entry.descriptor))return false;
                    }
                    return action!=operation::get||(output.boolean(true)&&output.u64(value->revision));
                };
                auto measured=wire::writer::measuring(std::numeric_limits<std::size_t>::max());
                if(!encode(measured))return error::too_large;
                required=measured.size();if(capacity<required)return error::too_large;
                auto status=reserve(value->owner->workspace,required,workspace);if(status!=error::success)return status;
                result=owned_bytes::allocate(static_cast<std::size_t>(required));wire::writer output{result.writable()};
                if(!encode(output))return error::io;
            }
            auto status=admit_result(cancelled,admit,required);if(status!=error::success)return status;
            {
                std::unique_lock lock{mutex,std::try_to_lock};if(!lock)return error::again;
                if(retired||value->retired||value->owner->retired||!value->node.live())return error::cancelled;
            }
            return deliver(result.view())?error::success:error::io;
        }
        /** @brief Snapshot existence and bounded ancestor search without revealing values.
         * @details Every exact candidate is authorized BEFORE consulting the
         * entry map. An unauthorized candidate aborts with denied even if a
         * higher ancestor exists. ROS2 not_set is declared but uninitialized;
         * it is distinct from an absent key. Search is ROS1-only, starts inside
         * the configured segment boundary and examines at most the explicit
         * candidate count. Incomplete searches never return a false not-found.
         * The detached result and revision describe one locked snapshot. Final
         * authority admission and node-lifetime validation precede delivery.
         */
        error locate_scope(operation action,std::string_view alias,request const& input,std::uint64_t capacity,
                           sink deliver,cancellation cancelled,final_admission admit,std::uint64_t& required)
        {
            capacity_lease workspace;owned_bytes result;std::shared_ptr<scope> value;
            {
                std::lock_guard lock{mutex};value=find_scope(alias,input.scope);if(!value)return error::bad_handle;
                auto const& policy=value->owner->policy;std::string_view found_name;bool exists{},initialized{};
                std::array<char,contract::maximum_name> candidate;
                if(action==operation::exists)
                {
                    if(key_index(policy,input.names[0])==policy.keys.size())return error::denied;
                    auto found=value->entries.find(input.names[0]);exists=found!=value->entries.end();
                    if(exists)
                    {wire::reader stored{found->second.value.view()};std::uint64_t kind{};if(!stored.u64(kind))return error::io;initialized=kind!=0;}
                }
                else
                {
                    auto root=policy.search_root.view(),space=input.start_namespace,leaf=input.names[0];
                    if(policy.values.selected!=parameter_values::algebra::ros1||!policy.max_search_depth)return error::denied;
                    if((space!="/"&&!communication::absolute_name(space))||leaf.find('/')!=leaf.npos)return error::invalid;
                    if(root!="/"&&(space.size()<root.size()||!space.starts_with(root)||(space.size()>root.size()&&space[root.size()]!='/')))return error::denied;
                    for(std::uint64_t inspected{};;)
                    {
                        if(inspected++==policy.max_search_depth)return error::too_large;
                        auto prefix=space.size()+(space=="/"?0u:1u);
                        if(prefix>candidate.size()||leaf.size()>candidate.size()-prefix)return error::too_large;
                        std::memcpy(candidate.data(),space.data(),space.size());if(space!="/")candidate[space.size()]='/';
                        std::memcpy(candidate.data()+prefix,leaf.data(),leaf.size());std::string_view key{candidate.data(),prefix+leaf.size()};
                        if(!communication::absolute_name(key))return error::invalid;
                        if(key_index(policy,key)==policy.keys.size())return error::denied;
                        if(value->entries.contains(key)){found_name=key;break;}
                        if(space==root)break;
                        auto slash=space.rfind('/');space=slash?space.substr(0,slash):std::string_view{"/"};
                    }
                }
                auto encode=[&](wire::writer& output)
                {
                    if(action==operation::exists)return output.boolean(exists)&&output.boolean(initialized)&&output.u64(value->revision);
                    return output.boolean(!found_name.empty())&&(found_name.empty()||output.text(found_name))&&output.u64(value->revision);
                };
                auto measured=wire::writer::measuring(contract::maximum_name+32);
                if(!encode(measured))return error::too_large;
                required=measured.size();if(capacity<required)return error::too_large;
                auto status=reserve(value->owner->workspace,required,workspace);if(status!=error::success)return status;
                result=owned_bytes::allocate(static_cast<std::size_t>(required));
                wire::writer output{result.writable()};if(!encode(output))return error::io;
            }
            auto status=admit_result(cancelled,admit,required);if(status!=error::success)return status;
            {
                std::unique_lock lock{mutex,std::try_to_lock};if(!lock)return error::again;
                if(retired||value->retired||value->owner->retired||!value->node.live())return error::cancelled;
            }
            return deliver(result.view())?error::success:error::io;
        }
        struct prepared_mutation
        {
            capacity_lease workspace;
            owned_bytes canonical,response,retired_response;
            std::shared_ptr<scope> target;
            std::shared_ptr<event> notification;
            std::shared_ptr<pending> waiting;
            container::std_map<container::std_string,entry,std::less<>> declaration;
            std::array<change_item,contract::maximum_batch> changes{};
            std::array<decision,contract::maximum_batch> decisions{};
            std::size_t change_count{},decision_count{};
            std::uint64_t base_revision{},next_value_bytes{};
            receipt result;
            bool accepted{},erasing{},declaring{},deferred{};
        };
        struct busy_guard
        {
            registry* owner{};std::shared_ptr<scope> target;
            ~busy_guard()
            {
                if(!owner||!owner->same_process())return;
                std::lock_guard lock{owner->mutex};target->busy=false;
            }
        };
        /** @brief Stage only changed values, then commit by allocation-free moves.
         * @details All four mutations share one exact request-key ledger. A
         * failed constraint result is itself recoverable; allocation, capacity,
         * cancellation and authority failures before admission consume no key.
         * Non-atomic set stages each admissible item independently, then exposes
         * one coherent local revision for that accepted subset. Atomic set
         * suppresses every change if any item is rejected.
         */
        error mutate(operation action,std::string_view alias,request const& input,bytes canonical,std::uint64_t capacity,
                     sink deliver,cancellation cancelled,final_admission admit,std::uint64_t& required)
        {
            prepared_mutation prepared;busy_guard busy;auto now=clock();
            {
                std::unique_lock lock{mutex};auto value=find_scope(alias,input.scope);if(!value)return error::bad_handle;
                auto& owner=*value->owner;auto const& policy=owner.policy;
                if(input.key.session!=value->identity||!input.key.sequence||input.key.sequence==UINT64_MAX)return error::invalid;
                if(value->has_receipt&&input.key==value->previous_key)
                {lock.unlock();return replay(action,alias,input,canonical,capacity,deliver,cancelled,admit,required);}
                if(input.key.sequence!=(value->has_receipt?value->previous_key.sequence+1:1))return error::conflict;
                if(value->busy||value->waiting)return error::again;
                if(canonical.size()>policy.max_request_bytes)return error::too_large;
                auto old_ledger=value->previous_request.extent+value->previous_response.extent;
                if(canonical.size()>policy.max_receipt_bytes||owner.used.receipt_bytes-old_ledger>policy.max_receipt_bytes-canonical.size())return error::again;
                prepared.target=value;prepared.base_revision=value->revision;prepared.next_value_bytes=value->value_bytes;
                prepared.result={1,value->identity,receipt_state::failed,receipt_effect::none,reason::none,{}};
                std::array<contract::item_view,contract::maximum_batch> selected{};std::size_t selected_count{};
                std::uint64_t staging_bytes=canonical.size()+old_ledger;
                if(action==operation::declare)
                {
                    auto name=input.declaration.name.view();auto index=key_index(policy,name);
                    if(index==policy.keys.size())return error::denied;
                    auto const& key=policy.keys[index];auto const& descriptor=input.declaration;
                    auto declared=static_cast<unsigned>(descriptor.declared_type);
                    if(declared>63||!(key.types&(1ull<<declared))||(descriptor.read_only==false&&key.read_only)||
                       (descriptor.dynamic_type&&!key.dynamic_type)||!descriptor_permitted(key,descriptor,policy.values.selected))return error::denied;
                    if(value->entries.contains(name))return error::conflict;
                    if(owner.used.parameters>=policy.max_parameters)return error::again;
                    bytes source=input.items[0].value;auto scalar=input.items[0].scalar;
                    if(key.has_override)
                    {source=key.override_value; if(!parameter_values::validate(source,policy.values,scalar))return error::invalid;}
                    auto checked=descriptor;checked.floating.absolute_tolerance=policy.floating_tolerance;
                    auto why=constraint(key,checked,scalar);prepared.result.why=why;
                    if(why==reason::none)
                    {
                        if(source.size()>policy.max_total_bytes||owner.used.bytes>policy.max_total_bytes-source.size())return error::again;
                        prepared.declaring=true;prepared.change_count=1;prepared.next_value_bytes+=source.size();
                        prepared.changes[0].name.assign(name);prepared.changes[0].scalar=scalar;prepared.changes[0].accepted=true;
                        selected[0]={name,source,scalar};selected_count=1;
                        if(!add(staging_bytes,source.size(),staging_bytes))return error::too_large;
                        // The node itself is allocated only after short-output
                        // and all finite payload reservations below succeed.
                    }
                }
                else if(action==operation::undeclare)
                {
                    auto name=input.names[0];auto index=key_index(policy,name);if(index==policy.keys.size())return error::denied;
                    auto found=value->entries.find(name);
                    auto why=found==value->entries.end()?reason::missing:
                        found->second.descriptor.read_only||policy.keys[index].read_only?reason::read_only:
                        input.has_revision&&input.revision!=value->revision?reason::revision:reason::none;
                    prepared.result.why=why;
                    if(why==reason::none)
                    {
                        prepared.erasing=true;prepared.change_count=1;prepared.changes[0].name.assign(name);prepared.changes[0].accepted=true;
                        prepared.next_value_bytes-=found->second.value.extent;
                        selected[0]={name,found->second.value.view(),found->second.scalar};selected_count=1;
                        if(!add(staging_bytes,found->second.value.extent,staging_bytes))return error::too_large;
                    }
                }
                else
                {
                    if(input.item_count>policy.max_batch||input.item_count>contract::maximum_batch)return error::too_large;
                    prepared.decision_count=input.item_count;
                    for(std::size_t i{};i!=input.item_count;++i)
                    {
                        auto const& item=input.items[i];auto index=key_index(policy,item.name);if(index==policy.keys.size())return error::denied;
                        auto found=value->entries.find(item.name);auto& decision=prepared.decisions[i];decision.name.assign(item.name);
                        reason why=input.has_revision&&input.revision!=value->revision?reason::revision:
                            found==value->entries.end()?reason::missing:
                            found->second.descriptor.read_only||policy.keys[index].read_only?reason::read_only:
                            constraint(policy.keys[index],found->second.descriptor,item.scalar);
                        if(why==reason::none)
                        {
                            auto next=prepared.next_value_bytes-found->second.value.extent;
                            if(!add(next,item.value.size(),next)||next>policy.max_total_bytes||
                               owner.used.bytes-value->value_bytes>policy.max_total_bytes-next)why=reason::capacity;
                            else
                            {
                                prepared.next_value_bytes=next;selected[selected_count++]=item;
                                auto& change=prepared.changes[prepared.change_count++];change.name.assign(item.name);change.scalar=item.scalar;change.accepted=true;
                                if(!add(staging_bytes,item.value.size(),staging_bytes)||!add(staging_bytes,found->second.value.extent,staging_bytes))return error::too_large;
                            }
                        }
                        decision.accepted=why==reason::none;decision.why=why;
                    }
                    if(action==operation::set_atomic&&selected_count!=input.item_count)
                    {
                        reason why=reason::none;for(std::size_t i{};i!=input.item_count;++i)if(!prepared.decisions[i].accepted){why=prepared.decisions[i].why;break;}
                        rejected_decisions({prepared.decisions.data(),prepared.decision_count},why);
                        prepared.result.why=why;selected_count=prepared.change_count=0;prepared.next_value_bytes=value->value_bytes;
                    }
                    else if(action==operation::set_atomic&&!input.item_count)prepared.result.why=reason::empty_request;
                }
                bool empty_set=action==operation::set&&!input.item_count;
                if(selected_count||empty_set)
                {
                    prepared.accepted=selected_count==input.item_count||prepared.declaring||prepared.erasing||empty_set;
                    prepared.result.state=receipt_state::succeeded;
                    prepared.result.effect=empty_set?receipt_effect::none:
                        action==operation::set&&selected_count!=input.item_count?receipt_effect::partial:receipt_effect::committed;
                }
                if(selected_count&&value->revision==UINT64_MAX)return error::too_large;
                prepared.deferred=selected_count&&policy.deferred_validation&&(action==operation::set||action==operation::set_atomic);
                std::uint64_t deadline{};
                if(prepared.deferred)
                {
                    if(!add(now,policy.validation_timeout_ns,deadline)||deadline>INT64_MAX)return error::too_large;
                    prepared.result.state=receipt_state::queued;prepared.result.effect=receipt_effect::locally_accepted;
                }
                auto decisions=prepared.decisions;
                if(prepared.deferred)pending_decisions({decisions.data(),prepared.decision_count});
                auto encode=[&](wire::writer& output){return write_mutation(output,action,prepared.result,
                    {decisions.data(),prepared.decision_count},prepared.accepted&&!prepared.deferred);};
                auto measured=wire::writer::measuring(std::numeric_limits<std::size_t>::max());if(!encode(measured))return error::too_large;
                required=measured.size();if(capacity<required)return error::too_large;
                if(!add(staging_bytes,required,staging_bytes))return error::too_large;
                auto status=reserve(owner.workspace,staging_bytes,prepared.workspace);if(status!=error::success)return status;
                prepared.result.request=resources::allocate_identity();if(!prepared.result.request)return error::too_large;
                prepared.canonical=owned_bytes::copy(canonical);
                prepared.response=owned_bytes::allocate(static_cast<std::size_t>(required));
                wire::writer encoded{prepared.response.writable()};if(!encode(encoded))return error::io;
                if(selected_count)
                {
                    auto kind=prepared.declaring?change::declare:prepared.erasing?change::undeclare:change::set;
                    status=make_event(value->owner,prepared.result.request,kind,value->revision+1,0,false,{selected.data(),selected_count},prepared.notification);
                    if(status!=error::success)return status;
                    if(prepared.declaring)
                    {
                        entry item;item.descriptor=input.declaration;item.descriptor.floating.absolute_tolerance=policy.floating_tolerance;
                        item.scalar=selected[0].scalar;item.policy_index=key_index(policy,selected[0].name);item.value=owned_bytes::copy(selected[0].value);
                        prepared.declaration.emplace(container::std_string{selected[0].name},std::move(item));
                    }
                    else if(!prepared.erasing)
                        for(std::size_t i{};i!=selected_count;++i)prepared.changes[i].value=owned_bytes::copy(selected[i].value);
                }
                if(prepared.deferred)
                {
                    std::uint64_t pending_size=sizeof(pending)+selected_count*sizeof(change_item);
                    for(auto const& item:std::span{selected.data(),selected_count})if(!add(pending_size,item.value.size(),pending_size))return error::too_large;
                    capacity_lease pending_capacity;status=reserve(owner.pending_bytes,pending_size,pending_capacity);if(status!=error::success)return status;
                    prepared.waiting=container::make_shared<pending>();auto& pending=*prepared.waiting;pending.payload_capacity=std::move(pending_capacity);
                    pending.request=prepared.result.request;pending.revision=value->revision;pending.deadline=deadline;pending.action=action;
                    pending.decision_count=prepared.decision_count;pending.decisions=prepared.decisions;pending.payload_bytes=pending_size;
                    pending.changes.reserve(selected_count);
                    for(std::size_t i{};i!=selected_count;++i)pending.changes.push_back(std::move(prepared.changes[i]));
                    pending.committed=std::move(prepared.notification);
                    status=make_event(value->owner,pending.request,change::set,value->revision,deadline,true,{selected.data(),selected_count},pending.validation);
                    if(status!=error::success)return status;
                }
                value->busy=true;busy.owner=this;busy.target=value;
            }
            auto status=admit_result(cancelled,admit,required);if(status!=error::success)return status;
            auto final_now=clock();
            {
                std::unique_lock lock{mutex,std::try_to_lock};if(!lock)return error::again;
                auto& value=*prepared.target;auto& owner=*value.owner;
                if(retired||value.retired||owner.retired||!value.node.live())return error::cancelled;
                if(value.revision!=prepared.base_revision||value.waiting)return error::again;
                if(prepared.deferred&&final_now>=prepared.waiting->deadline)return error::timeout;
                auto old_ledger=value.previous_request.extent+value.previous_response.extent;
                auto const& policy=owner.policy;
                if(prepared.canonical.extent>policy.max_receipt_bytes||
                   owner.used.receipt_bytes-old_ledger>policy.max_receipt_bytes-prepared.canonical.extent)return error::again;
                if(!prepared.deferred&&(prepared.next_value_bytes>policy.max_total_bytes||
                   owner.used.bytes-value.value_bytes>policy.max_total_bytes-prepared.next_value_bytes||
                   (prepared.declaring&&owner.used.parameters>=policy.max_parameters)))return error::again;
                if(!prepared.deferred)
                {
                    if(prepared.declaring)
                    {auto node=prepared.declaration.extract(prepared.declaration.begin());value.entries.insert(std::move(node));++owner.used.parameters;}
                    else if(prepared.erasing)
                    {
                        auto found=value.entries.find(prepared.changes[0].name.view());
                        prepared.changes[0].value=std::move(found->second.value);value.entries.erase(found);--owner.used.parameters;
                    }
                    else for(std::size_t i{};i!=prepared.change_count;++i)
                    {
                        auto& change=prepared.changes[i];auto& item=value.entries.find(change.name.view())->second;
                        std::swap(item.value,change.value);item.scalar=change.scalar;
                    }
                    if(prepared.change_count)
                    {owner.used.bytes=owner.used.bytes-value.value_bytes+prepared.next_value_bytes;value.value_bytes=prepared.next_value_bytes;++value.revision;}
                    if(prepared.notification)value.events.push_back(std::move(prepared.notification));
                }
                else
                {value.waiting=std::move(prepared.waiting);value.events.push_back(value.waiting->validation);}
                owner.used.receipt_bytes=owner.used.receipt_bytes-old_ledger+prepared.canonical.extent;
                std::swap(value.previous_request,prepared.canonical);prepared.retired_response=std::move(value.previous_response);
                value.previous_key=input.key;value.previous_action=action;value.previous_receipt=prepared.result;value.has_receipt=true;value.request_released=false;
                value.previous_decision_count=prepared.decision_count;value.previous_decisions=prepared.decisions;
                value.previous_accepted=prepared.accepted&&!prepared.deferred;
                if(prepared.deferred)pending_decisions({value.previous_decisions.data(),value.previous_decision_count});
            }
            return deliver(prepared.response.view())?error::success:error::io;
        }
        static bool same_filter(filter const& selected,request const& input) noexcept
        {
            if(selected.depth!=input.depth||selected.count!=input.name_count)return false;
            for(std::size_t i{};i!=selected.count;++i)if(selected.prefixes[i]!=input.names[i])return false;
            return true;
        }
        static bool matches(std::string_view name,request const& input,char separator) noexcept
        {
            auto depth=[&](std::string_view remainder)
            {
                if(!input.depth||remainder.empty())return true;
                return 1u+static_cast<std::uint64_t>(std::count(remainder.begin(),remainder.end(),separator))<=input.depth;
            };
            if(!input.name_count)
            {if(separator=='/'&&name.starts_with('/'))name.remove_prefix(1);return depth(name);}
            for(std::size_t i{};i!=input.name_count;++i)
            {
                auto prefix=input.names[i];if(name==prefix)return true;
                if(!name.starts_with(prefix))continue;
                auto remainder=name.substr(prefix.size());
                if(!prefix.empty()&&prefix.back()==separator){if(depth(remainder))return true;}
                else if(!remainder.empty()&&remainder.front()==separator&&depth(remainder.substr(1)))return true;
            }
            return false;
        }
        static void prune_cursors(scope& value,std::uint64_t now) noexcept
        {
            auto old=value.cursors.size();
            std::erase_if(value.cursors,[&](cursor const& current){return current.expiry<=now||current.revision!=value.revision;});
            value.owner->used.cursors-=old-value.cursors.size();
        }
        static error make_filter(std::shared_ptr<binding> const& owner,request const& input,std::shared_ptr<filter>& output)
        {
            auto encode=[&](wire::writer& writer)
            {
                if(!writer.u64(input.name_count))return false;
                for(std::size_t i{};i!=input.name_count;++i)if(!writer.text(input.names[i]))return false;
                return writer.u64(input.depth);
            };
            auto measured=wire::writer::measuring(std::numeric_limits<std::size_t>::max());if(!encode(measured))return error::too_large;
            capacity_lease reservation;auto status=reserve(owner->cursor_bytes,sizeof(filter)+measured.size(),reservation);
            if(status!=error::success)return status;
            auto value=container::make_shared<filter>();value->payload_capacity=std::move(reservation);
            value->canonical=owned_bytes::allocate(measured.size());wire::writer writer{value->canonical.writable()};if(!encode(writer))return error::io;
            wire::reader reader{value->canonical.view()};std::uint64_t count{};
            if(!reader.u64(count)||count>contract::maximum_batch)return error::io;
            value->count=static_cast<std::size_t>(count);
            for(std::size_t i{};i!=value->count;++i)if(!reader.text(value->prefixes[i]))return error::io;
            if(!reader.u64(value->depth)||!reader.done())return error::io;
            output=std::move(value);return error::success;
        }
        /** @brief Revision/filter/expiry-fenced pages with fixed canonical names.
         * @details Cursors are immutable continuation positions, not consuming
         * iterators. Exact retries reuse a still-live matching continuation.
         * A new cursor is published only after successful output delivery and
         * the second revision/node/cancellation fence. Expired cursors are
         * reclaimed only after the complete page fits the output capacity.
         */
        error list_scope(std::string_view alias,request const& input,std::uint64_t capacity,sink deliver,
                         cancellation cancelled,final_admission admit,std::uint64_t& required)
        {
            capacity_lease workspace;owned_bytes response;std::shared_ptr<scope> value;
            cursor continuation;bool needs_cursor{},create_cursor{};std::uint64_t revision{},now=clock();
            {
                std::lock_guard lock{mutex};value=find_scope(alias,input.scope);if(!value)return error::bad_handle;
                auto const& policy=value->owner->policy;
                if(input.name_count>policy.max_batch||input.name_count>contract::maximum_batch||!input.limit||input.limit>policy.max_batch||
                   input.limit>contract::maximum_batch)return error::too_large;
                std::string_view start;
                std::uint64_t expiry{};
                if(input.has_cursor)
                {
                    auto found=std::find_if(value->cursors.begin(),value->cursors.end(),[&](cursor const& current){return current.identity==input.cursor;});
                    if(found==value->cursors.end())return error::bad_handle;
                    if(found->expiry<=now)return error::timeout;
                    if(found->revision!=value->revision||!same_filter(*found->selected,input))return error::conflict;
                    continuation.selected=found->selected;start=found->next_name.view();expiry=found->expiry;
                }
                else if(!add(now,policy.cursor_timeout_ns,expiry)||expiry>INT64_MAX)return error::too_large;
                revision=value->revision;std::array<std::string_view,contract::maximum_batch> names{};std::size_t count{};
                auto position=start.empty()?value->entries.begin():value->entries.lower_bound(start);
                char separator=policy.values.selected==parameter_values::algebra::ros1?'/':'.';
                for(;position!=value->entries.end();++position)
                {
                    std::string_view name=position->first;if(!matches(name,input,separator))continue;
                    if(count==input.limit){needs_cursor=true;continuation.next_name.assign(name);break;}
                    names[count++]=name;
                }
                if(needs_cursor)
                {
                    for(auto const& current:value->cursors)
                        if(current.revision==revision&&current.expiry>now&&current.next_name==continuation.next_name&&
                           same_filter(*current.selected,input)&&(!input.has_cursor||current.expiry==expiry))
                        {continuation=current;break;}
                    create_cursor=!continuation.identity;
                    if(create_cursor){continuation.identity=1;continuation.revision=revision;continuation.expiry=expiry;}
                }
                auto encode=[&](wire::writer& output)
                {
                    if(!output.u64(revision)||!output.boolean(needs_cursor)||(needs_cursor&&!output.u64(continuation.identity))||!output.u64(count))return false;
                    for(std::size_t i{};i!=count;++i)
                    {
                        std::array<std::uint8_t,contract::maximum_name+8> body{};wire::writer text{body};
                        if(!text.text(names[i])||!output.text("ros.parameter.name")||!output.text("canonical.text")||!output.blob(text.written()))return false;
                    }
                    return true;
                };
                auto measured=wire::writer::measuring(std::numeric_limits<std::size_t>::max());if(!encode(measured))return error::too_large;
                required=measured.size();if(capacity<required)return error::too_large;
                prune_cursors(*value,now);
                if(create_cursor)
                {
                    if(!policy.max_cursors||value->owner->used.cursors>=policy.max_cursors)return error::again;
                    if(!continuation.selected)
                    {
                        for(auto const& current:value->cursors)if(same_filter(*current.selected,input)){continuation.selected=current.selected;break;}
                        if(!continuation.selected){auto status=make_filter(value->owner,input,continuation.selected);if(status!=error::success)return status;}
                    }
                    continuation.identity=resources::allocate_identity();if(!continuation.identity)return error::too_large;
                }
                auto status=reserve(value->owner->workspace,required,workspace);if(status!=error::success)return status;
                response=owned_bytes::allocate(static_cast<std::size_t>(required));wire::writer output{response.writable()};if(!encode(output))return error::io;
            }
            auto status=admit_result(cancelled,admit,required);if(status!=error::success)return status;
            auto check=[&](std::uint64_t time)
            {
                if(retired||value->retired||value->owner->retired||!value->node.live())return error::cancelled;
                if(value->revision!=revision)return error::conflict;
                if(needs_cursor&&time>=continuation.expiry)return error::timeout;
                if(input.has_cursor)
                {
                    auto found=std::find_if(value->cursors.begin(),value->cursors.end(),[&](cursor const& current){return current.identity==input.cursor;});
                    if(found==value->cursors.end()||found->expiry<=time)return error::timeout;
                }
                if(create_cursor&&value->owner->used.cursors>=value->owner->policy.max_cursors)return error::again;
                if(needs_cursor&&!create_cursor&&std::none_of(value->cursors.begin(),value->cursors.end(),
                    [&](cursor const& current){return current.identity==continuation.identity;}))return error::bad_handle;
                return error::success;
            };
            auto before_delivery=clock();
            {std::unique_lock lock{mutex,std::try_to_lock};if(!lock)return error::again;status=check(before_delivery);if(status!=error::success)return status;}
            if(!deliver(response.view()))return error::io;
            if(cancelled&&cancelled())return error::cancelled;
            auto after_delivery=clock();
            {
                std::unique_lock lock{mutex,std::try_to_lock};if(!lock)return error::again;status=check(after_delivery);if(status!=error::success)return status;
                if(create_cursor){value->cursors.push_back(std::move(continuation));++value->owner->used.cursors;}
            }
            return error::success;
        }
        struct event_guard
        {
            registry* owner{};std::shared_ptr<event> value;
            ~event_guard()
            {
                if(!owner||!owner->same_process())return;
                std::lock_guard lock{owner->mutex};value->claimed=false;
            }
        };
        /** @brief Reserve delivery, copy outside the mutex, then acknowledge once.
         * @details A false/throwing sink or a cancellation/revocation during the
         * copy leaves the event unconsumed. Validation cannot be answered until
         * this acknowledgement succeeds. Reentrant take sees again, never a
         * second simultaneous delivery of the same event.
         */
        error take_event(std::string_view alias,request const& input,std::uint64_t capacity,sink deliver,
                         cancellation cancelled,final_admission admit,std::uint64_t& required)
        {
            capacity_lease workspace;owned_bytes response;std::shared_ptr<scope> value;
            event_guard claim;auto now=clock();bool timed_out{};
            {
                std::lock_guard lock{mutex};value=find_scope(alias,input.scope);if(!value)return error::bad_handle;
                if(value->events.empty())return error::again;
                auto selected=value->events.front();if(selected->claimed)return error::again;
                auto measured=wire::writer::measuring(std::numeric_limits<std::size_t>::max());if(!write_event(measured,*selected))return error::too_large;
                required=measured.size();if(capacity<required)return error::too_large;
                timed_out=selected->validation&&now>=selected->deadline;
                if(!timed_out)
                {
                    auto status=reserve(value->owner->workspace,required,workspace);if(status!=error::success)return status;
                    response=owned_bytes::allocate(static_cast<std::size_t>(required));wire::writer output{response.writable()};
                    if(!write_event(output,*selected))return error::io;
                }
                selected->claimed=true;claim.owner=this;claim.value=std::move(selected);
            }
            auto status=admit_result(cancelled,admit,timed_out?0:required);if(status!=error::success)return status;
            auto before_delivery=clock();
            auto check=[&](std::uint64_t time)
            {
                if(retired||value->retired||value->owner->retired||!value->node.live())return error::cancelled;
                if(claim.value->validation&&time>=claim.value->deadline)
                {expire(*value,time);return error::timeout;}
                if(value->events.empty()||value->events.front()!=claim.value)return error::conflict;
                if(claim.value->validation&&(!value->waiting||value->waiting->request!=claim.value->request))return error::conflict;
                return error::success;
            };
            {std::unique_lock lock{mutex,std::try_to_lock};if(!lock)return error::again;status=check(before_delivery);if(status!=error::success)return status;}
            if(!deliver(response.view()))return error::io;
            if(cancelled&&cancelled())return error::cancelled;
            auto after_delivery=clock();
            {
                std::unique_lock lock{mutex,std::try_to_lock};if(!lock)return error::again;status=check(after_delivery);if(status!=error::success)return status;
                if(claim.value->validation)value->waiting->delivered=true;
                value->events.erase(value->events.begin());claim.value->claimed=false;
            }
            return error::success;
        }
        struct prepared_response
        {
            capacity_lease workspace,event_growth;
            std::shared_ptr<scope> target;
            std::shared_ptr<pending> waiting;
            std::array<owned_bytes,contract::maximum_batch> values,event_values;
            std::array<parameter_values::scalar,contract::maximum_batch> scalars{};
            std::array<decision,contract::maximum_batch> decisions{};
            owned_bytes canonical,response,retired_response;
            std::uint64_t next_value_bytes{},next_event_bytes{};
            receipt result;
            bool replay{},timed_out{},accepted{},replacement{};
        };
        static void explain(receipt& output,std::string_view input) noexcept
        {
            output.explanation.assign(input);
            for(std::size_t i{};i!=output.explanation.size;++i)
                if(static_cast<unsigned char>(output.explanation.bytes[i])<0x20||output.explanation.bytes[i]==0x7f)output.explanation.bytes[i]=' ';
        }
        /** @brief Resolve one delivered validation without recursive mutation.
         * @details Replacement has exactly the same candidate key set. Every
         * replacement value is rechecked against current descriptors and the
         * immutable profile. Future committed-event slots were reserved at
         * submit; a larger replacement must additionally reserve its bytes.
         * A terminal response is recoverable by exact canonical replay. Final
         * capacity/revision contention may return again after an attempt was
         * charged; that failure commits no values and leaves validation live.
         */
        error respond(std::string_view alias,request const& input,bytes canonical,std::uint64_t capacity,sink deliver,
                      cancellation cancelled,final_admission admit,std::uint64_t& required)
        {
            prepared_response prepared;busy_guard busy;auto now=clock();
            {
                std::lock_guard lock{mutex};auto value=find_scope(alias,input.scope);if(!value)return error::bad_handle;
                auto& owner=*value->owner;auto const& policy=owner.policy;
                if(!value->has_receipt||value->previous_receipt.request!=input.request_id)return error::bad_handle;
                prepared.target=value;prepared.result=value->previous_receipt;prepared.next_value_bytes=value->value_bytes;
                if(!value->waiting)
                {
                    if(value->previous_response.extent)
                    {if(!std::ranges::equal(canonical,value->previous_response.view()))return error::conflict;}
                    else if(value->previous_receipt.why!=reason::validation_timeout)return error::conflict;
                    prepared.replay=true;
                }
                else
                {
                    if(value->busy)return error::again;
                    prepared.waiting=value->waiting;auto& waiting=*prepared.waiting;
                    if(waiting.request!=input.request_id||waiting.revision!=value->revision)return error::conflict;
                    prepared.timed_out=now>=waiting.deadline;
                    if(!prepared.timed_out&&!waiting.delivered)return error::again;
                    prepared.accepted=input.accepted&&!prepared.timed_out;prepared.replacement=input.has_replacement&&prepared.accepted;
                    prepared.decisions=waiting.decisions;
                    prepared.result.state=prepared.timed_out?receipt_state::cancelled:prepared.accepted?receipt_state::succeeded:receipt_state::failed;
                    prepared.result.effect=prepared.accepted?
                        waiting.action==operation::set&&waiting.changes.size()!=waiting.decision_count?receipt_effect::partial:receipt_effect::committed:
                        receipt_effect::none;
                    prepared.result.why=prepared.timed_out?reason::validation_timeout:prepared.accepted?reason::none:reason::validation_rejected;
                    prepared.result.explanation={};if(!prepared.timed_out)explain(prepared.result,input.reason);
                    if(!prepared.accepted)rejected_decisions({prepared.decisions.data(),waiting.decision_count},prepared.result.why);
                }
                auto measured=wire::writer::measuring(std::numeric_limits<std::size_t>::max());if(!write_receipt(measured,prepared.result))return error::too_large;
                required=measured.size();if(capacity<required)return error::too_large;
                std::uint64_t workspace_bytes=required;
                std::array<contract::item_view,contract::maximum_batch> selected{};
                if(!prepared.replay)
                {
                    if(canonical.size()>policy.max_request_bytes)return error::too_large;
                    if(canonical.size()>policy.max_receipt_bytes||owner.used.receipt_bytes-value->previous_response.extent>policy.max_receipt_bytes-canonical.size())return error::again;
                    if(!add(workspace_bytes,canonical.size(),workspace_bytes)||!add(workspace_bytes,value->previous_response.extent,workspace_bytes))return error::too_large;
                    if(prepared.accepted)
                    {
                        auto& waiting=*prepared.waiting;
                        if(prepared.replacement&&input.item_count!=waiting.changes.size())return error::invalid;
                        prepared.next_event_bytes=sizeof(event)+waiting.changes.size()*sizeof(event_item);
                        for(std::size_t i{};i!=waiting.changes.size();++i)
                        {
                            auto const& change=waiting.changes[i];auto name=change.name.view();auto found=value->entries.find(name);
                            if(found==value->entries.end())return error::conflict;
                            auto const& entry=found->second;auto const& key=policy.keys[entry.policy_index];
                            selected[i]={name,change.value.view(),change.scalar};
                            if(prepared.replacement)
                            {
                                auto replacement=std::find_if(input.items.begin(),input.items.begin()+input.item_count,
                                    [&](contract::item_view const& item){return item.name==name;});
                                if(replacement==input.items.begin()+input.item_count)return error::denied;
                                selected[i]=*replacement;
                            }
                            if(entry.descriptor.read_only||key.read_only)return error::denied;
                            if(constraint(key,entry.descriptor,selected[i].scalar)!=reason::none)return error::invalid;
                            auto next=prepared.next_value_bytes-entry.value.extent;
                            if(!add(next,selected[i].value.size(),next))return error::too_large;
                            prepared.next_value_bytes=next;prepared.scalars[i]=selected[i].scalar;
                            if(!add(workspace_bytes,entry.value.extent,workspace_bytes)||!add(workspace_bytes,selected[i].value.size(),workspace_bytes)||
                               !add(prepared.next_event_bytes,selected[i].value.size(),prepared.next_event_bytes))return error::too_large;
                            if(prepared.replacement&&(!add(workspace_bytes,waiting.committed->items[i].value.extent,workspace_bytes)||
                               !add(workspace_bytes,selected[i].value.size(),workspace_bytes)))return error::too_large;
                        }
                        if(prepared.next_value_bytes>policy.max_total_bytes||owner.used.bytes-value->value_bytes>policy.max_total_bytes-prepared.next_value_bytes)return error::again;
                    }
                }
                auto status=reserve(owner.workspace,workspace_bytes,prepared.workspace);if(status!=error::success)return status;
                prepared.response=owned_bytes::allocate(static_cast<std::size_t>(required));wire::writer output{prepared.response.writable()};
                if(!write_receipt(output,prepared.result))return error::io;
                if(!prepared.replay)
                {
                    prepared.canonical=owned_bytes::copy(canonical);
                    if(prepared.accepted&&prepared.replacement)
                    {
                        auto old_event_bytes=prepared.waiting->committed->payload_bytes;
                        if(prepared.next_event_bytes>old_event_bytes)
                        {status=reserve(owner.event_bytes,prepared.next_event_bytes-old_event_bytes,prepared.event_growth);if(status!=error::success)return status;}
                        for(std::size_t i{};i!=prepared.waiting->changes.size();++i)
                        {prepared.values[i]=owned_bytes::copy(selected[i].value);prepared.event_values[i]=owned_bytes::copy(selected[i].value);}
                    }
                    value->busy=true;busy.owner=this;busy.target=value;
                }
            }
            auto status=admit_result(cancelled,admit,required);if(status!=error::success)return status;
            auto final_now=clock();
            {
                std::unique_lock lock{mutex,std::try_to_lock};if(!lock)return error::again;
                auto& value=*prepared.target;auto& owner=*value.owner;auto const& policy=owner.policy;
                if(retired||value.retired||owner.retired||!value.node.live())return error::cancelled;
                if(value.previous_receipt.request!=input.request_id)return error::conflict;
                if(!prepared.replay)
                {
                    if(value.waiting!=prepared.waiting||value.revision!=prepared.waiting->revision)return error::conflict;
                    if(!prepared.timed_out&&final_now>=prepared.waiting->deadline)
                    {expire(value,final_now);return error::timeout;}
                    if(prepared.canonical.extent>policy.max_receipt_bytes||
                       owner.used.receipt_bytes-value.previous_response.extent>policy.max_receipt_bytes-prepared.canonical.extent)return error::again;
                    auto& waiting=*prepared.waiting;
                    if(prepared.accepted)
                    {
                        if(prepared.next_value_bytes>policy.max_total_bytes||owner.used.bytes-value.value_bytes>policy.max_total_bytes-prepared.next_value_bytes||
                           value.revision==UINT64_MAX)return error::again;
                        for(std::size_t i{};i!=waiting.changes.size();++i)
                        {
                            auto& change=waiting.changes[i];auto& entry=value.entries.find(change.name.view())->second;
                            auto& replacement=prepared.replacement?prepared.values[i]:change.value;
                            std::swap(entry.value,replacement);entry.scalar=prepared.scalars[i];
                            if(prepared.replacement)std::swap(waiting.committed->items[i].value,prepared.event_values[i]);
                        }
                        if(prepared.replacement)
                        {
                            auto& capacity=waiting.committed->payload_capacity;
                            if(prepared.event_growth)
                            {
                                capacity.amount_+=prepared.event_growth.amount_;prepared.event_growth.amount_=0;prepared.event_growth.account.reset();
                            }
                            if(capacity.amount_>prepared.next_event_bytes)
                            {capacity.account->used.fetch_sub(capacity.amount_-prepared.next_event_bytes,std::memory_order_release);capacity.amount_=prepared.next_event_bytes;}
                            waiting.committed->payload_bytes=prepared.next_event_bytes;
                        }
                        owner.used.bytes=owner.used.bytes-value.value_bytes+prepared.next_value_bytes;value.value_bytes=prepared.next_value_bytes;
                        ++value.revision;waiting.committed->revision=value.revision;value.events.push_back(waiting.committed);
                    }
                    erase_event(value,waiting.validation);
                    owner.used.receipt_bytes=owner.used.receipt_bytes-value.previous_response.extent+prepared.canonical.extent;
                    prepared.retired_response=std::move(value.previous_response);value.previous_response=std::move(prepared.canonical);
                    value.previous_receipt=prepared.result;value.previous_decisions=prepared.decisions;
                    value.previous_accepted=prepared.accepted&&waiting.changes.size()==waiting.decision_count;
                    value.waiting.reset();
                }
            }
            return deliver(prepared.response.view())?error::success:error::io;
        }
        /** @brief Drop local payloads while retaining the node until active calls end. */
        static void clear_scope(scope& value) noexcept
        {
            if(value.retired)return;
            auto& used=value.owner->used;
            --used.scopes;used.parameters-=value.entries.size();used.bytes-=value.value_bytes;
            used.receipt_bytes-=value.previous_request.extent+value.previous_response.extent;used.cursors-=value.cursors.size();
            value.retired=true;value.published=false;value.value_bytes=0;
            value.entries.clear();value.previous_request={};value.previous_response={};
            value.cursors.clear();value.events.clear();value.waiting.reset();
        }
    public:
        explicit registry(clock_source source=steady_now) noexcept:clock(source?source:steady_now){}
        registry(registry const&)=delete;
        registry& operator=(registry const&)=delete;
        error configure(std::string_view alias,plan const& policy)
        {
            if(!same_process()||alias.empty()||alias.size()>96||!valid(policy))return error::invalid;
            auto value=container::make_shared<binding>();value->alias=alias;value->policy=policy;
            value->workspace=container::make_shared<capacity_account>();value->workspace->maximum=policy.max_workspace;
            value->event_slots=container::make_shared<capacity_account>();value->event_slots->maximum=policy.max_events;
            value->event_bytes=container::make_shared<capacity_account>();value->event_bytes->maximum=policy.max_event_bytes;
            value->pending_bytes=container::make_shared<capacity_account>();value->pending_bytes->maximum=policy.max_pending_bytes;
            value->cursor_bytes=container::make_shared<capacity_account>();value->cursor_bytes->maximum=policy.max_cursor_bytes;
            value->scope_slots=container::make_shared<capacity_account>();value->scope_slots->maximum=policy.max_scopes;
            auto key=value->alias;
            std::lock_guard lock{mutex};
            if(retired||bindings.contains(alias))return error::conflict;
            bindings.emplace(std::move(key),std::move(value));return error::success;
        }
        /** @brief Copy only fixed parser bounds; never exposes a native pointer. */
        error parser_policy(std::string_view alias,parameter_values::limits& output,std::uint64_t& request_bound) const
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto row=find_binding(alias);if(!row)return error::denied;
            output=row->policy.values;request_bound=row->policy.max_request_bytes;return error::success;
        }
        error reserve_workspace(std::string_view alias,std::uint64_t bytes,capacity_lease& output)
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto row=find_binding(alias);if(!row)return error::denied;
            return reserve(row->workspace,bytes,output);
        }
        error profiles(std::string_view alias,buffers::profile& selected,buffers::profile& node_profile) const
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto row=find_binding(alias);if(!row)return error::denied;
            selected=row->policy.selected;node_profile=row->policy.node_profile;return error::success;
        }
        /** @brief Execute one fully owned canonical carrier under external leases.
         * @details The broker checks exact primary/secondary actions before this
         * call. final_admission is called outside the provider lock only after
         * the complete reply fits; it atomically charges optional budgets while
         * those leases remain held. A subsequent try-lock/capacity race may
         * return again after charging an attempt, with no committed mutation.
         * The caller must not resize, reuse or change canonical during this call.
         * Open consumes an already admitted attachment for the fixed node profile;
         * all other operations use their retained scope attachment. grant_limit
         * and the finite provider max_scopes are both enforced at open.
         */
        error invoke(operation action,std::string_view alias,bytes canonical,std::uint64_t grant_limit,
                     std::uint64_t output_capacity,sink deliver,cancellation cancelled,final_admission admit,
                     std::uint64_t& required,communication::node_lease attachment={})
        {
            required=0;if(!same_process())return error::denied;
            if(!deliver||static_cast<std::size_t>(action)>=contract::operations.size()||(action!=operation::open&&attachment))return error::invalid;
            if(cancelled&&cancelled())return error::cancelled;
            parameter_values::limits policy;std::uint64_t request_bound{};auto status=parser_policy(alias,policy,request_bound);
            if(status!=error::success)return status;
            if(canonical.size()>request_bound)return error::too_large;
            request decoded;if(!contract::decode(action,canonical,policy,decoded))return error::invalid;
            switch(action)
            {
            case operation::open:return open_scope(alias,decoded,grant_limit,output_capacity,deliver,cancelled,admit,required,attachment);
            case operation::declare:case operation::undeclare:case operation::set:case operation::set_atomic:
                return mutate(action,alias,decoded,canonical,output_capacity,deliver,cancelled,admit,required);
            case operation::get:case operation::describe:
                return read_scope(action,alias,decoded,output_capacity,deliver,cancelled,admit,required);
            case operation::exists:case operation::search:
                return locate_scope(action,alias,decoded,output_capacity,deliver,cancelled,admit,required);
            case operation::list:return list_scope(alias,decoded,output_capacity,deliver,cancelled,admit,required);
            case operation::take:return take_event(alias,decoded,output_capacity,deliver,cancelled,admit,required);
            case operation::respond:return respond(alias,decoded,canonical,output_capacity,deliver,cancelled,admit,required);
            default:return error::invalid;
            }
        }
        /** @brief Close an owned local scope even if its parent node was fenced.
         * @details Only the parameter close grant is needed for this cleanup;
         * acquiring a fresh live-node lease would make retired children leak.
         * Data is discarded and validation cannot commit. The node attachment
         * is released outside the mutex after every in-flight scope use ends.
         */
        error close(std::string_view alias,handle identity)
        {
            if(!same_process())return error::denied;
            std::shared_ptr<scope> value;
            {
                std::lock_guard lock{mutex};auto found=scopes.find(identity);
                if(found==scopes.end()||found->second->owner->alias!=alias||!found->second->published)return error::bad_handle;
                value=found->second;clear_scope(*value);scopes.erase(found);
            }
            return error::success;
        }
        /** @brief Inspect or retire the last owned request without new node access.
         * @details A scope retains one exact mutation key, canonical bytes and
         * response ledger until the next accepted mutation or close. Releasing
         * its public request identity does not erase that bounded retry proof
         * or forgive a pending validation. Historical receipts require only
         * the origin parameter grant; they grant no fresh node/data access.
         * Short output, failed admission and cancelled preflight do not expire
         * validation or release an identity. Release is committed before its
         * boolean acknowledgement; a retry then returns false. Exact original
         * mutation replay remains available even after request.release.
         */
        error request_invoke(request_operation action,std::string_view alias,handle identity,std::uint64_t capacity,
                             sink deliver,cancellation cancelled,final_admission admit,std::uint64_t& required)
        {
            required=0;if(!same_process())return error::denied;
            if(!identity||!deliver||(action!=request_operation::describe&&action!=request_operation::release))return error::invalid;
            capacity_lease workspace;owned_bytes response;std::array<std::uint8_t,8> released{};
            std::shared_ptr<scope> value;std::shared_ptr<binding> owner;bool expired{},retire{};auto now=clock();
            {
                std::lock_guard lock{mutex};owner=find_binding(alias);if(!owner)return error::denied;
                for(auto const& [id,candidate]:scopes)
                {
                    static_cast<void>(id);
                    if(candidate->owner==owner&&candidate->published&&!candidate->retired&&candidate->has_receipt&&candidate->previous_receipt.request==identity)
                    {value=candidate;break;}
                }
                if(action==request_operation::release)
                {
                    required=8;if(capacity<required)return error::too_large;
                    if(value&&!value->request_released)
                    {
                        expired=value->waiting&&now>=value->waiting->deadline;
                        if(value->waiting&&!expired)return error::again;
                        retire=true;
                    }
                    wire::writer output{released};if(!output.boolean(retire))return error::io;
                }
                else
                {
                    if(!value||value->request_released)return error::bad_handle;
                    auto receipt=value->previous_receipt;expired=value->waiting&&now>=value->waiting->deadline;
                    if(expired){receipt.state=receipt_state::cancelled;receipt.effect=receipt_effect::none;receipt.why=reason::validation_timeout;receipt.explanation={};}
                    auto measured=wire::writer::measuring(std::numeric_limits<std::size_t>::max());if(!write_receipt(measured,receipt))return error::too_large;
                    required=measured.size();if(capacity<required)return error::too_large;
                    auto result=reserve(owner->workspace,required,workspace);if(result!=error::success)return result;
                    response=owned_bytes::allocate(static_cast<std::size_t>(required));wire::writer output{response.writable()};if(!write_receipt(output,receipt))return error::io;
                }
            }
            auto status=admit_result(cancelled,admit,required);if(status!=error::success)return status;
            auto final_now=clock();
            {
                std::unique_lock lock{mutex,std::try_to_lock};if(!lock)return error::again;
                if(retired||owner->retired||(value&&value->retired))return error::cancelled;
                if(value&&value->previous_receipt.request!=identity)return error::conflict;
                if(action==request_operation::describe&&value->request_released)return error::bad_handle;
                if(expired)
                {
                    if(!value->waiting&&value->previous_receipt.why!=reason::validation_timeout)return error::again;
                    if(value->waiting)expire(*value,final_now);
                }
                if(retire)
                {
                    if(value->waiting)return error::again;
                    if(value->request_released)return error::again;
                    value->request_released=true;
                }
            }
            return deliver(action==request_operation::release?bytes{released}:response.view())?error::success:error::io;
        }
        /** @brief Fence every owned scope before running any node-lease destructor. */
        void revoke(std::string_view alias)
        {
            if(!same_process())return;
            container::std_map<handle,std::shared_ptr<scope>> released;
            {
                std::lock_guard lock{mutex};auto owner=bindings.find(alias);if(owner==bindings.end())return;
                owner->second->retired=true;
                for(auto i=scopes.begin();i!=scopes.end();)
                {
                    if(i->second->owner!=owner->second){++i;continue;}
                    clear_scope(*i->second);auto old=i++;released.insert(scopes.extract(old));
                }
            }
        }
        void retire()
        {
            if(!same_process())return;
            container::std_map<handle,std::shared_ptr<scope>> released;
            {
                std::lock_guard lock{mutex};retired=true;
                for(auto& [alias,owner]:bindings){static_cast<void>(alias);owner->retired=true;}
                for(auto& [identity,value]:scopes){static_cast<void>(identity);clear_scope(*value);}
                released.swap(scopes);
            }
        }
        /** @brief Return exact secondary authority dependencies for broker admission. */
        error dependencies(std::string_view alias,handle identity,dependency& output) const
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto row=find_scope(alias,identity);if(!row)return error::bad_handle;
            output={row->node_capability,row->node.identity(),row->node.authority()};return error::success;
        }
        /** @brief Nonblocking aggregate inspection never expires or consumes state. */
        bool try_inspect(std::string_view alias,usage& output) const noexcept
        {
            if(!same_process())return false;
            std::unique_lock lock{mutex,std::try_to_lock};if(!lock)return false;
            auto found=bindings.find(alias);if(found==bindings.end())return false;
            auto const& row=*found->second;output=row.used;
            output.workspace=row.workspace->used.load(std::memory_order_acquire);
            output.events=row.event_slots->used.load(std::memory_order_acquire);output.event_bytes=row.event_bytes->used.load(std::memory_order_acquire);
            output.pending_bytes=row.pending_bytes->used.load(std::memory_order_acquire);output.cursor_bytes=row.cursor_bytes->used.load(std::memory_order_acquire);
            output.retained_scopes=row.scope_slots->used.load(std::memory_order_acquire);
            return true;
        }
    };
}
