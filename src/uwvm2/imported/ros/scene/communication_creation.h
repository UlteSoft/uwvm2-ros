/** @file
 * @brief Finite exact-key creation ledger for asynchronous communication opens.
 * @details Each immutable binding reserves one current receipt and one tentative
 * carrier. reserve() is inert and precedes admission charging. commit() runs only
 * under the complete charged origin group and atomically acknowledges a released
 * previous key. Identical retries recover the existing request; changed retries
 * never launch, charge or replace anything. The next key is exactly previous+1.
 *
 * A supervisor begins/pumps RPC outside authority waits, reacquires complete
 * validation-only authority for every nonblocking send, and keeps the result
 * private until publish() under fresh complete authority. Native token and guest
 * handle are distinct. Failed guest output does not consume the published result.
 * Cancellation never refunds creation, and unknown/native cleanup debt prevents
 * release or reuse. Positive endpoint cleanup requires an exact receipt from
 * its trusted owner. Local retirement of a disclosed external attachment never
 * establishes process-wide cleanup; elapsed timeout is never evidence.
 */
#pragma once
#include "communication_open_contract.h"
#include "communication_worker_async.h"
namespace uwvm2::imported::ros::scene::communication_creation
{
    namespace contract=::uwvm2::imported::ros::communication_open_contract;
    using error=communication::error;using effect=communication::effect;
    using state=contract::state;using endpoint_kind=contract::endpoint_kind;
    inline bool same_caller(caller a,caller b)noexcept{return a.instance==b.instance&&a.generation==b.generation;}
    enum class dependency:std::uint8_t{operation,node,context,schema};
    struct origin
    {dependency kind{};communication::owner authority;handle capability{},resource{};friend bool operator==(origin const&,origin const&)=default;};
    struct identity
    {
        caller who;communication::owner authority;std::uint64_t key{},deadline{};
        std::array<origin,6> origins{};std::size_t count{};
        friend bool operator==(identity const& a,identity const& b)noexcept
        {return same_caller(a.who,b.who)&&a.authority==b.authority&&a.key==b.key&&a.deadline==b.deadline&&a.origins==b.origins&&a.count==b.count;}
    };
    struct plan
    {
        communication::owner authority;communication::profile selected;endpoint_kind kind{};
        std::uint64_t worker_binding{},provider_generation{},carrier_bytes{},max_deadline_ns{};
    };
    struct view
    {contract::receipt receipt;endpoint_kind kind{};handle published{};std::uint64_t provider_generation{};bool endpoint_live{},reserved{};};
    /** @brief Owned fixed-size origin token plus a borrowed immutable binding. */
    struct work
    {handle request{};identity original;plan const* binding{};};
    using claim_set=std::array<admission_claim,6>;
    using charge=error(*)(void*,identity const&,std::uint64_t,group_lease&)noexcept;
    /** @brief Exact creation claim roles; resource ownership needs separate validation.
     * @details Only the primary open is charged. Existing node/context/schema
     * dependencies remain validation-only, including duplicate schema roles.
     * This helper cannot turn an identity supplied by a guest into authority.
     */
    inline std::span<admission_claim const> admission_claims(identity const& identity,endpoint_kind kind,claim_set& out,std::uint64_t bytes=0)noexcept
    {
        if(!identity.count||identity.count>out.size()||!contract::valid(kind))return {};
        auto primary=kind==endpoint_kind::node?"node.open":kind==endpoint_kind::topic?"topic.open":kind==endpoint_kind::service?"service.open":"action.open";
        for(std::size_t i{};i<identity.count;++i)
        {
            auto action=i==0?primary:identity.origins[i].kind==dependency::node?"node.open":identity.origins[i].kind==dependency::context?"context.open":"type.describe";
            out[i]={identity.origins[i].capability,action,i?0:bytes,false,i!=0};
        }
        return {out.data(),identity.count};
    }
    class ledger
    {
        struct record
        {
            identity original;contract::receipt receipt;
            ::uwvm2::utils::container::std_vector<std::uint8_t> canonical;
            std::size_t size{};handle native{},published{};
            communication::node_attachment node;
            bool occupied{},endpoint_live{},managed{};
        };
        struct binding
        {plan policy;record current,reservation;};
        ::uwvm2::utils::container::std_vector<binding> bindings;
        mutable std::mutex mutex;std::mutex submitting;bool frozen{},stopping{};
#if defined(__linux__)
        pid_t process{::getpid()};
#endif
        bool same_process()const noexcept
        {
#if defined(__linux__)
            return process==::getpid();
#else
            return true;
#endif
        }
        binding* find(communication::owner owner)noexcept
        {for(auto& binding:bindings)if(binding.policy.authority==owner)return &binding;return nullptr;}
        record* find(handle request,binding*& owner)noexcept
        {for(auto& binding:bindings){if(binding.current.occupied&&binding.current.receipt.request_id==request){owner=&binding;return &binding.current;}
            if(binding.reservation.occupied&&binding.reservation.receipt.request_id==request){owner=&binding;return &binding.reservation;}}return nullptr;}
        static bool same(record const& previous,identity const& selected,wire::bytes canonical)noexcept
        {return previous.original==selected&&previous.size==canonical.size()&&std::equal(canonical.begin(),canonical.end(),previous.canonical.begin());}
        static bool valid(plan const& policy,identity const& selected)noexcept
        {
            auto expected=policy.kind==endpoint_kind::node?2u:static_cast<unsigned>(policy.kind)+2u;
            if(!selected.who.instance||!selected.who.generation||selected.authority!=policy.authority||!selected.key||!selected.deadline||
                selected.deadline>INT64_MAX||selected.count!=expected||selected.count>selected.origins.size())return false;
            for(std::size_t i{};i<selected.count;++i)
            {
                auto const& origin=selected.origins[i];
                auto kind=i==0?dependency::operation:policy.kind==endpoint_kind::node?dependency::context:i==1?dependency::node:i==2?dependency::context:dependency::schema;
                if(origin.kind!=kind||!origin.authority||origin.authority.environment!=policy.authority.environment||!origin.capability||
                    ((kind==dependency::node||kind==dependency::context)?!origin.resource:bool(origin.resource))||
                    (!i&&origin.authority!=policy.authority))return false;
            }
            for(std::size_t i{selected.count};i<selected.origins.size();++i)if(selected.origins[i]!=origin{})return false;
            return true;
        }
        static view describe(binding const& binding,record const& value,bool reserved=false)noexcept
        {return {value.receipt,binding.policy.kind,value.published,binding.policy.provider_generation,value.endpoint_live,reserved};}
        static bool matches(binding const& binding,record const& record,group_lease const& held,bool charged=false)noexcept
        {
            claim_set claims;
            if(charged)return held.matches(record.original.who,admission_claims(record.original,binding.policy.kind,claims,record.size+contract::receipt_bytes));
            // A just-consumed creation lease may also protect its first send.
            // Later sends/publication normally use a fresh zero-cost group.
            return held.matches(record.original.who,admission_claims(record.original,binding.policy.kind,claims))||
                   held.matches(record.original.who,admission_claims(record.original,binding.policy.kind,claims,record.size+contract::receipt_bytes));
        }
        static void cancel_record(record& value,error reason)noexcept
        {
            if(value.receipt.status==state::cancelled||value.receipt.status==state::failed)return;
            bool started=value.receipt.observed==effect::unknown;
            value.receipt.cleanup_debt|=started||value.native||value.node.identity()||value.endpoint_live;
            if(started)value.receipt.observed=effect::unknown;
            value.receipt.status=state::cancelled;value.receipt.result=reason;
        }
    public:
        /** @brief Allocate exact finite retry storage before any worker effect.
         * @details Total counts two complete canonical carrier reservations per
         * binding. Zero plans/budget/default owners provide no functionality.
         */
        error configure(std::span<plan const> selected,std::uint64_t carrier_reservation)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};if(frozen||stopping)return error::denied;
            if(selected.empty()||selected.size()>128||!carrier_reservation||carrier_reservation>(1ull<<30))return error::invalid;
            try
            {
                ::uwvm2::utils::container::std_vector<binding> prepared;prepared.reserve(selected.size());std::uint64_t used{};
                for(auto const& policy:selected)
                {
                    if(!policy.authority||!communication::valid(policy.selected)||!contract::valid(policy.kind)||!policy.worker_binding||!policy.provider_generation||
                        policy.carrier_bytes<16||policy.carrier_bytes>4u*1024u*1024u||!policy.max_deadline_ns||policy.max_deadline_ns>INT64_MAX||
                        policy.carrier_bytes>(carrier_reservation-used)/2)return error::invalid;
                    for(auto const& previous:prepared)if(previous.policy.authority==policy.authority||previous.policy.worker_binding==policy.worker_binding)return error::invalid;
                    binding value;value.policy=policy;value.current.canonical.resize(policy.carrier_bytes);value.reservation.canonical.resize(policy.carrier_bytes);
                    used+=2*policy.carrier_bytes;prepared.push_back(std::move(value));
                }
                bindings=std::move(prepared);frozen=true;return error::ok;
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(...){return error::io;}
        }
        /** @brief Reserve a tentative exact request without charging or native effects. */
    private:
        error reserve_impl(bool managed,identity const& original,wire::bytes canonical,view& output)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};if(!frozen||stopping)return error::cancelled;
            auto* owner=find(original.authority);if(!owner||!valid(owner->policy,original))return error::denied;
            if(canonical.size()>owner->policy.carrier_bytes)return error::too_large;
            wire::bytes semantic;contract::creation suffix;
            if(!contract::split_creation(canonical,semantic,suffix)||suffix.key!=original.key||suffix.deadline!=original.deadline)return error::invalid;
            auto& current=owner->current;auto& pending=owner->reservation;
            if(current.occupied&&current.original.key==original.key)
            {if(current.managed!=managed||!same(current,original,canonical))return error::denied;output=describe(*owner,current);return error::ok;}
            if(pending.occupied)
            {if(pending.managed!=managed||!same(pending,original,canonical))return error::busy;output=describe(*owner,pending,true);return error::ok;}
            auto last=current.occupied?current.original.key:0;
            if(last==UINT64_MAX||original.key!=last+1)return error::stale;
            if(current.occupied&&(current.receipt.status!=state::released||current.receipt.cleanup_debt||current.endpoint_live))return error::busy;
            auto now=communication::now();if(original.deadline<=now)return error::timeout;if(original.deadline-now>owner->policy.max_deadline_ns)return error::invalid;
            auto request=resources::allocate_identity();if(!request)return error::too_large;
            pending.original=original;pending.receipt={original.key,request,state::reserved,error::again,effect::none,false};pending.native=pending.published=0;
            pending.node={};
            pending.endpoint_live=false;pending.size=canonical.size();if(!canonical.empty())std::memcpy(pending.canonical.data(),canonical.data(),canonical.size());
            pending.occupied=true;pending.managed=managed;output=describe(*owner,pending,true);return error::ok;
        }
        /** @brief Commit exactly once under the complete already-charged origin group.
         * @details A retry returns the retained receipt and must not charge again.
         * No worker I/O or allocation occurs. The previous released key is
         * acknowledged only here, so an admission failure can abandon its new
         * reservation without erasing the earlier recovery receipt.
         */
        error commit_impl(bool managed,caller who,handle request,group_lease& charged,view& output)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};binding* owner{};auto* record=find(request,owner);
            if(!record||record->managed!=managed||!same_caller(record->original.who,who)||!matches(*owner,*record,charged,true))return error::denied;
            if(stopping)return error::cancelled;
            if(record==&owner->current){output=describe(*owner,*record);return error::ok;}
            if(record->receipt.status!=state::reserved)return error::denied;
            if(communication::now()>=record->original.deadline)return error::timeout;
            claim_set claims;
            if(!charged.consume(who,admission_claims(record->original,owner->policy.kind,claims,record->size+contract::receipt_bytes)))return error::denied;
            std::swap(owner->current,owner->reservation);owner->reservation.occupied=false;
            owner->current.receipt.status=state::queued;output=describe(*owner,owner->current);return error::ok;
        }
        /** @brief Abandon only an uncharged, unpublished tentative reservation. */
        error abandon_impl(bool managed,caller who,handle request)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};binding* owner{};auto* record=find(request,owner);
            if(!record||record->managed!=managed||record!=&owner->reservation||!same_caller(record->original.who,who))return error::denied;
            record->occupied=false;return error::ok;
        }
    public:
        error reserve(identity const& original,wire::bytes canonical,view& output)noexcept{return reserve_impl(false,original,canonical,output);}
        error commit(caller who,handle request,group_lease& charged,view& output)noexcept{return commit_impl(false,who,request,charged,output);}
        error abandon(caller who,handle request)noexcept{return abandon_impl(false,who,request);}
        /** @brief Reserve, charge and queue one exact creation without duplicate debit.
         * @details Concurrent or reentrant submissions return again. Exact
         * accepted retries never call charge. The callback runs outside the
         * ledger mutex and must revalidate every resource origin, then obtain
         * the exact charged open/dependency group for canonical_bytes+48.
         * Managed reservations cannot be adopted by the low-level methods.
         * No launch, native I/O, allocation or reply wait occurs here.
         */
        error enqueue(identity const& original,wire::bytes canonical,charge admission,void* context,view& output)noexcept
        {
            if(!same_process()||!admission)return error::denied;
            std::unique_lock serialized{submitting,std::try_to_lock};if(!serialized.owns_lock())return error::again;
            view pending;auto status=reserve_impl(true,original,canonical,pending);if(status!=error::ok)return status;
            if(!pending.reserved){output=pending;return error::ok;}
            group_lease held;status=admission(context,original,canonical.size()+contract::receipt_bytes,held);
            if(status==error::ok)status=commit_impl(true,original.who,pending.receipt.request_id,held,pending);
            held.release();
            if(status!=error::ok){abandon_impl(true,original.who,pending.receipt.request_id);return status;}
            output=pending;return error::ok;
        }
        /** @brief Retain a work item; callback/admission happens after this lock is released. */
        error next(work& output)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};if(stopping)return error::cancelled;
            for(auto& owner:bindings)if(owner.current.occupied&&owner.current.receipt.status==state::queued)
            {
                auto& record=owner.current;
                if(communication::now()>=record.original.deadline){cancel_record(record,error::timeout);continue;}
                output={record.receipt.request_id,record.original,&owner.policy};return error::ok;
            }
            return error::again;
        }
        /** @brief Begin one already-charged operation under fresh full origin validation.
         * @details Returns the exact retained work identity. Callers may perform
         * only bounded nonblocking attachment/write while this group is held; all
         * readiness and response polling happens after releasing it. Because
         * native startup follows outside this mutex, this transition reserves
         * unknown effect/cleanup debt BEFORE releasing the lock. Only an actual
         * complete provider result may resolve it. RPC send uses send() instead
         * and can retain an exact no-byte/no-effect result.
         */
        error begin(handle request,group_lease const& current,work& output)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};binding* owner{};auto* record=find(request,owner);
            if(!record||record!=&owner->current||!matches(*owner,*record,current))return error::denied;
            if(stopping)return error::cancelled;
            if(record->receipt.status!=state::queued&&record->receipt.status!=state::sending&&record->receipt.status!=state::waiting)return error::busy;
            if(communication::now()>=record->original.deadline){cancel_record(*record,error::timeout);return error::timeout;}
            record->receipt.status=state::sending;record->receipt.observed=effect::unknown;record->receipt.cleanup_debt=true;
            output={request,record->original,&owner->policy};return error::ok;
        }
        /** @brief Record actual transport progress, never infer effect from enqueue. */
        error progress(handle request,bool wrote,bool awaiting)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};binding* owner{};auto* record=find(request,owner);
            if(!record||record!=&owner->current)return error::bad_handle;
            if(record->receipt.status!=state::sending&&record->receipt.status!=state::waiting)return error::cancelled;
            if(wrote){record->receipt.cleanup_debt=true;record->receipt.observed=effect::unknown;}
            if(awaiting)record->receipt.status=state::waiting;
            return error::ok;
        }
#if defined(__linux__)
        /** @brief Linearize actual nonblocking request bytes with cancel/revoke.
         * @details No separate progress call is required or permitted for this
         * send. The ledger mutex covers the one nonblocking transport write and
         * its actual sent-byte observation, so cancellation cannot release a
         * request between native effect and the corresponding debt update.
         */
        error send(handle request,group_lease const& current,communication_worker_async::transport& transport,
                   communication_worker_async::ticket ticket)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};binding* owner{};auto* record=find(request,owner);
            if(!record||record!=&owner->current||!matches(*owner,*record,current))return error::denied;
            if(stopping)return error::cancelled;
            if(record->receipt.status!=state::queued&&record->receipt.status!=state::sending)return error::cancelled;
            if(communication::now()>=record->original.deadline){cancel_record(*record,error::timeout);return error::timeout;}
            auto status=transport.send(ticket,current);auto actual=transport.inspect();
            if(actual.current!=ticket)return error::stale;
            if(actual.sent){record->receipt.observed=effect::unknown;record->receipt.cleanup_debt=true;}
            if(status!=error::ok&&status!=error::again)
            {record->receipt.status=state::failed;record->receipt.result=status;return status;}
            record->receipt.status=status==error::ok?state::waiting:state::sending;return status;
        }
#endif
        /** @brief Retain a complete native result without publishing a guest resource. */
        error complete(handle request,error status,effect observed,handle native_token)noexcept
        {
            if(!same_process())return error::denied;
            if(!communication::known_status(status)||observed>effect::unknown)return error::invalid;
            std::lock_guard lock{mutex};binding* owner{};auto* record=find(request,owner);
            if(!record||record!=&owner->current)return error::bad_handle;
            if(record->receipt.status!=state::sending&&record->receipt.status!=state::waiting)return error::cancelled;
            if(owner->policy.kind==endpoint_kind::node)return error::unsupported;
            if(status==error::ok&&!native_token)return error::invalid;
            if(status!=error::ok&&observed!=effect::none)observed=effect::unknown;
            if(status==error::ok&&observed==effect::unknown)status=error::io;
            if(status==error::ok&&observed==effect::none)observed=effect::locally_accepted;
            record->receipt.result=status;record->receipt.observed=observed;record->native=native_token;
            record->receipt.cleanup_debt=observed==effect::unknown||native_token;
            record->receipt.status=status==error::ok?state::ready:state::failed;return error::ok;
        }
        /** @brief Complete node creation using actual registry evidence, never an integer substitute.
         * @details The proof must name this exact node owner, its original
         * context resource/owner and the sealed worker incarnation. It holds no
         * native ownership edge and cannot delay shutdown. Completion remains
         * private; publication additionally checks the exact host node resource.
         * This method records only known success. Failed/uncertain startup stays
         * in cancellation/debt state until the supervisor establishes cleanup.
         */
        error complete_node(handle request,communication::node_attachment const& actual)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};binding* owner{};auto* record=find(request,owner);
            if(!record||record!=&owner->current)return error::bad_handle;
            if(record->receipt.status!=state::sending&&record->receipt.status!=state::waiting)return error::cancelled;
            if(owner->policy.kind!=endpoint_kind::node||record->original.count!=2||!actual.live()||actual.authority()!=record->original.authority||
                actual.provider_generation()!=owner->policy.provider_generation||actual.context_authority()!=record->original.origins[1].authority||
                actual.context_identity()!=record->original.origins[1].resource)return error::denied;
            record->node=actual;record->receipt.result=error::ok;record->receipt.observed=effect::locally_accepted;
            record->receipt.cleanup_debt=true;record->receipt.status=state::ready;return error::ok;
        }
        /** @brief Retain the actual node startup failure without inventing effect certainty.
         * @details Only the owning coordinator calls this after a terminal
         * startup step. Cancellation already linearized by a caller or stop
         * fence wins; a later failure cannot overwrite that receipt. Pending
         * native effects and cleanup debt remain unknown until independent
         * namespace cleanup is established. No callback, allocation or I/O runs.
         */
        error fail_node(caller who,handle request,error reason,view& output)noexcept
        {
            if(!same_process()||!communication::known_status(reason)||reason==error::ok||reason==error::again)return error::invalid;
            std::lock_guard lock{mutex};binding* owner{};auto* record=find(request,owner);
            if(!record||record!=&owner->current||owner->policy.kind!=endpoint_kind::node||!same_caller(record->original.who,who))return error::denied;
            auto phase=record->receipt.status;
            if(phase==state::queued||phase==state::sending||phase==state::waiting)
            {cancel_record(*record,reason);record->receipt.status=state::failed;}
            else if(phase!=state::cancelled&&phase!=state::failed)return error::busy;
            output=describe(*owner,*record);return error::ok;
        }
        /** @brief Publish only a separately created typed guest handle under fresh authority.
         * @details Native ownership transfers to the registered endpoint. Exact
         * repeat publication returns the same kind/handle. This call performs no
         * output copy or callback, so failed guest delivery cannot consume it.
         */
        error publish(caller who,handle request,group_lease const& current,handle guest,contract::published& output)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};binding* owner{};auto* record=find(request,owner);
            if(!record||record!=&owner->current||!same_caller(record->original.who,who)||!matches(*owner,*record,current)||!guest)return error::denied;
            if(stopping)return error::cancelled;
            if(record->receipt.status==state::published)
            {if(record->published!=guest)return error::denied;output={record->receipt,owner->policy.kind,guest};return error::ok;}
            if(owner->policy.kind==endpoint_kind::node&&(!record->node.live()||record->node.identity()!=guest))return error::denied;
            if(record->receipt.status!=state::ready||(!record->native&&!record->node.identity())||record->receipt.observed==effect::unknown)return error::busy;
            if(communication::now()>=record->original.deadline){cancel_record(*record,error::timeout);return error::timeout;}
            record->published=guest;record->endpoint_live=true;record->receipt.status=state::published;record->receipt.cleanup_debt=false;
            output={record->receipt,owner->policy.kind,guest};return error::ok;
        }
        /** @brief Inspect cached state only; guest adapter separately admits request.describe. */
        error inspect(caller who,handle request,view& output)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};binding* owner{};auto* record=find(request,owner);
            if(!record||!same_caller(record->original.who,who))return error::denied;
            output=describe(*owner,*record,record==&owner->reservation);return error::ok;
        }
        /** @brief Atomically retain the cached receipt and its complete host-minted provenance.
         * @details The public control dispatcher still admits its own exact
         * take/describe/cancel/release grant. This host-only snapshot performs
         * no I/O, clock advancement or implicit publication, including for a
         * failed or unknown request. Foreign callers cannot obtain provenance.
         */
        error inspect_origin(caller who,handle request,view& output,identity& original)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};binding* owner{};auto* record=find(request,owner);
            if(!record||!same_caller(record->original.who,who))return error::denied;
            output=describe(*owner,*record,record==&owner->reservation);original=record->original;return error::ok;
        }
        /** @brief Return verified native creation evidence only to trusted typed adoption.
         * @details This is not a guest observation and grants no permission.
         * The factory must independently validate the complete immutable plan
         * and current origins before constructing and publishing its typed host
         * resource. A request identity or profile is never a native endpoint.
         */
        error provider_result(handle request,work& output,handle& native_token)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};binding* owner{};auto* record=find(request,owner);
            if(!record||record!=&owner->current)return error::bad_handle;
            if(stopping||(record->receipt.status!=state::ready&&record->receipt.status!=state::published)||
                record->receipt.result!=error::ok||record->receipt.observed==effect::unknown||!record->native)return error::busy;
            output={request,record->original,&owner->policy};native_token=record->native;return error::ok;
        }
        /** @brief Cancel independently of creation rate; return whether native cleanup is owed. */
        error cancel(caller who,handle request,view& output)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};binding* owner{};auto* record=find(request,owner);
            if(!record||record!=&owner->current||!same_caller(record->original.who,who))return error::denied;
            if(record->receipt.status==state::released)return error::stale;
            cancel_record(*record,error::cancelled);output=describe(*owner,*record);return error::ok;
        }
        /** @brief Release only a terminal known receipt; native or unknown debt is retained. */
        error release(caller who,handle request)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};binding* owner{};auto* record=find(request,owner);
            if(!record||record!=&owner->current||!same_caller(record->original.who,who))return error::denied;
            auto phase=record->receipt.status;
            if(record->endpoint_live||record->receipt.cleanup_debt||record->receipt.observed==effect::unknown||
                (phase!=state::published&&phase!=state::cancelled&&phase!=state::failed&&phase!=state::released))return error::busy;
            record->receipt.status=state::released;return error::ok;
        }
        /** @brief Trusted endpoint teardown evidence; false retains all native cleanup debt.
         * @details Only the owning supervisor may call this after an exact
         * endpoint-close receipt or independently qualified trusted cleanup
         * evidence. Disclosed external attachments do not currently supply
         * process-wide evidence. A timeout or unknown reply is not confirmation.
         */
        error cleanup(handle request,bool confirmed)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};binding* owner{};auto* record=find(request,owner);
            if(!record||record!=&owner->current)return error::bad_handle;
            if(!confirmed){record->receipt.cleanup_debt=true;return error::busy;}
            if(record->node.live())return error::busy;
            record->receipt.cleanup_debt=false;record->endpoint_live=false;record->native=0;
            record->node={};
            if(record->receipt.status==state::ready||record->receipt.status==state::published)cancel_record(*record,error::cancelled);
            return error::ok;
        }
        /** @brief Fence all committed work without waiting for independent native cleanup. */
        void retire()noexcept
        {
            if(!same_process())return;
            std::lock_guard lock{mutex};stopping=true;
            for(auto& owner:bindings){owner.reservation.occupied=false;if(owner.current.occupied&&owner.current.receipt.status!=state::released)cancel_record(owner.current,error::cancelled);}
        }
    };
}
