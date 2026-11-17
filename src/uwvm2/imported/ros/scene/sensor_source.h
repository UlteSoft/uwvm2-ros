/** @file
 * @brief Typed sensor ingress, bounded sealed observations and proposal provenance.
 * @details This provider owns its sensor core. A native ingress adapter can only
 * submit complete frames after an explicitly authorized sensor.open. It cannot
 * invent a guest capability or publish model provenance. sensor.take copies the
 * core's detached, validated frame into a new bounded sealed buffer, then makes
 * the observation visible only after successful delivery and complete authority
 * revalidation. Failure retains the queue item and closes unpublished storage.
 *
 * Buffer storage is the separately configured exact buffer alias; this provider
 * never opens its capability. Proposal capture requires the caller's explicit
 * buffer.read capability. The finite history retains actual sealed allocations;
 * old model pins continue to count against buffer capacity after history eviction.
 * A fresh native frame makes latest-required proposals stale even before take.
 * No descriptor, socket, worker, polling thread or hardware watchdog is created.
 */
#pragma once
#include "sensor_core.h"
#include "proposal_dispatch.h"

namespace uwvm2::imported::ros::scene::sensors
{
    class source
    {
    public:
        inline static constexpr std::size_t maximum_history=16;
        struct configuration
        {
            plan ingress;
            buffers::text<255> alias,buffer_alias;
            buffers::profile buffer_profile;
            std::uint64_t history{},buffer_objects{};
        };
        struct retained_delivery
        {delivery const& frame;handle buffer{};buffers::digest digest{};};
        /** @brief Opaque current source-open lease, held across one bounded native read. */
        class ingress_lease
        {
            friend class source;source* provider_{};caller who_;handle capability_{},resource_{};lease authority_;
        public:
            ingress_lease()=default;ingress_lease(ingress_lease const&)=delete;ingress_lease&operator=(ingress_lease const&)=delete;
            explicit operator bool()const noexcept{return provider_!=nullptr;}
        };
        using take_preflight=::uwvm2::utils::container::function_ref<error(delivery const&,std::uint64_t&)>;
    private:
        struct opened{handle id{},capability{},epoch{};caller who;bool published{},closing{};};
        struct snapshot
        {frame_metadata metadata;handle buffer{};buffers::digest digest{};buffers::registry::sealed_lease pin;};
        std::shared_ptr<authority> permissions_;
        buffers::registry& buffers_;
        core core_;
        owner owner_;
        configuration policy_;
        handle calibration_{},transform_{};
        mutable std::mutex mutex_;
        std::array<opened,maximum_handles> handles_{};
        std::array<snapshot,maximum_history> history_{};
        std::size_t next_{},retained_{};
        bool configured_{},retired_{},taking_{};
#if defined(__linux__)
        ::pid_t process_{::getpid()};
#endif
        bool same_process()const noexcept
        {
#if defined(__linux__)
            return process_==::getpid();
#else
            return true;
#endif
        }
        static bool same(caller a,caller b)noexcept{return !(a<b)&&!(b<a);}
        opened* find(caller who,handle capability,handle id)noexcept
        {for(auto& row:handles_)if(row.id==id&&id&&row.capability==capability&&same(row.who,who))return &row;return nullptr;}
        error authorized(caller who,handle capability,std::string_view action,std::uint64_t bytes,lease& held,bool charge)
        {
            {std::lock_guard lock{mutex_};if(!configured_||retired_)return error::denied;}
            auto result=action=="resource.observe"?permissions_->dependency(who,capability,action,held):
                permissions_->admit(who,capability,action,bytes,held,charge);
            if(result!=error::success)return result;
            auto const& grant=held.specification();
            return grant.kind==purpose::sensor&&grant.alias==policy_.alias.view()&&grant.binding=="bounded-sensor"?error::success:error::denied;
        }
        error live(caller who,handle capability,handle id)noexcept
        {
            std::lock_guard lock{mutex_};auto row=find(who,capability,id);
            return !configured_||retired_?error::cancelled:!row||!row->published||row->closing?error::bad_handle:error::success;
        }
        snapshot const* latest()const noexcept
        {return retained_?&history_[(next_+static_cast<std::size_t>(policy_.history)-1)%static_cast<std::size_t>(policy_.history)]:nullptr;}
        proposal_dispatch::source_sample sample(opened const& opened,snapshot const& captured)const noexcept
        {
            auto const& frame=captured.metadata;auto const& native=frame.source;
            proposal_dispatch::source_sample result;result.environment=owner_.environment;result.owner=owner_.grant;
            result.owner_generation=owner_.generation;result.profile=policy_.ingress.profile;result.frame=policy_.ingress.frame;
            // An unstamped source explicitly uses the host receive clock. These
            // clock identities are deployment-owned and are never guest text.
            result.observation={0,captured.buffer,opened.id,native.generation,
                native.stamped?native.clock:frame.received_clock,native.stamped?native.epoch:frame.received_epoch,0,
                native.stamped?native.source_ns:static_cast<std::int64_t>(frame.captured_steady_ns),frame.captured_steady_ns,
                frame.capture_sequence,calibration_,transform_,opened.epoch};
            return result;
        }
        bool matches(proposals::source_rule const& rule)const noexcept
        {return proposals::policy_text(rule.alias,true)&&proposals::policy_text(rule.buffer_alias,true)&&proposals::envelope_profile(rule.profile)&&
            proposals::envelope_profile(rule.buffer_profile)&&proposals::policy_text(rule.frame)&&
            rule.alias==policy_.alias&&rule.buffer_alias==policy_.buffer_alias&&rule.profile==policy_.ingress.profile&&
            rule.buffer_profile==policy_.buffer_profile&&rule.frame==policy_.ingress.frame;}
        static error capture_adapter(void* context,caller who,handle cap,handle resource,proposals::source_rule const& rule,proposal_dispatch::source_sample& out)
        {return static_cast<source*>(context)->capture(who,cap,resource,rule,out);}
        static error validate_adapter(void* context,caller who,handle cap,proposals::source_rule const& rule,
                                      proposal_dispatch::source_sample const& current,proposals::admission::observation const& captured,
                                      proposals::admission::observation_policy mode)
        {return static_cast<source*>(context)->validate(who,cap,rule,current,captured,mode);}
    public:
        explicit source(std::shared_ptr<authority> permissions,buffers::registry& buffers,std::uint64_t(*clock)()noexcept=nullptr)
            :permissions_(std::move(permissions)),buffers_(buffers),core_(clock){}
        source(source const&)=delete;source&operator=(source const&)=delete;
        ~source(){retire();}
        /** @brief Bind one immutable source and exact finite auxiliary buffer scope.
         * @details Source identity is supplied by trusted native registration.
         * Calibration/transform epochs are allocated by this binding and cannot
         * be renewed by guest open. At least one additional buffer slot is needed
         * while replacing a full history; existing model pins can still refuse
         * creation. No buffers or native resources are opened by configure.
         */
        error configure(owner principal,configuration const& policy,source_identity actual)
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};if(configured_||retired_||!permissions_)return error::denied;
            if(!proposals::policy_text(policy.alias,true)||!proposals::policy_text(policy.buffer_alias,true)||!proposals::envelope_profile(policy.buffer_profile)||
                !policy.history||policy.history>maximum_history||policy.buffer_objects<=policy.history||policy.buffer_objects==UINT64_MAX)return error::invalid;
            auto calibration=resources::allocate_identity(),transform=resources::allocate_identity();if(!calibration||!transform)return error::too_large;
            auto result=core_.configure(principal,policy.ingress,actual);if(result!=error::success)return result;
            owner_=principal;policy_=policy;calibration_=calibration;transform_=transform;configured_=true;return error::success;
        }
        /** @brief Create one caller-bound source handle after explicit open admission. */
        template<typename Sink>error open(caller who,handle capability,buffers::profile const& requested,Sink&& deliver,std::uint64_t carrier_bytes=0)
        {
            if(!same_process())return error::denied;
            lease held;auto status=authorized(who,capability,"sensor.open",carrier_bytes,held,true);if(status!=error::success)return status;
            opened* row{};handle id{};
            {
                std::lock_guard lock{mutex_};if(!configured_||retired_)return error::denied;
                for(auto& candidate:handles_)if(!candidate.id){row=&candidate;break;}
                if(!row)return error::too_large;
                auto epoch=resources::allocate_identity();if(!epoch)return error::too_large;
                status=core_.open(owner_,requested,held.specification().max_resources,[&](handle value)noexcept{id=value;return true;});
                if(status!=error::success)return status;
                *row={id,capability,epoch,who,false,false};
            }
            bool copied{};try{copied=deliver(id);}catch(...){copied=false;}
            lease current;status=authorized(who,capability,"sensor.open",0,current,false);
            std::lock_guard lock{mutex_};
            if(status!=error::success||retired_||row->closing||!copied)
            {core_.close(owner_,id);*row={};return status!=error::success?status:retired_?error::cancelled:error::io;}
            row->published=true;return error::success;
        }
        /** @brief Admit one live handle's existing open authority without any creation-rate charge.
         * @details This lease never charges or consults an open rate allowance.
         * It validates current membership, capability, action and exposure only.
         * Native ingress capacity is independently configured; no hidden rate is
         * borrowed from open/take. Retain this lease across each bounded read.
         */
        error admit_ingress(ingress_lease& output)
        {
            if(!same_process()||output)return error::denied;
            std::array<opened,maximum_handles> candidates;
            {std::lock_guard lock{mutex_};if(!configured_||retired_)return error::denied;candidates=handles_;}
            for(auto const& row:candidates)
            {
                if(!row.published||row.closing)continue;
                lease allowed;auto status=authorized(row.who,row.capability,"sensor.open",0,allowed,false);
                if(status!=error::success||live(row.who,row.capability,row.id)!=error::success)continue;
                output.provider_=this;output.who_=row.who;output.capability_=row.capability;output.resource_=row.id;output.authority_=std::move(allowed);return error::success;
            }
            return error::denied;
        }
        error recheck_ingress(ingress_lease const& current)
        {
            if(!same_process()||current.provider_!=this)return error::denied;
            lease allowed;auto status=authorized(current.who_,current.capability_,"sensor.open",0,allowed,false);
            return status==error::success?live(current.who_,current.capability_,current.resource_):status;
        }
        /** @brief Cached deployment description; this trusted-only accessor creates no authority. */
        error ingress_description(configuration& output)const noexcept
        {if(!same_process())return error::denied;std::lock_guard lock{mutex_};if(!configured_||retired_)return error::denied;output=policy_;return error::success;}
        /** @brief Trusted try-lock snapshot of source counters only, including retired providers. */
        error inspect_cached(usage& output)const noexcept
        {if(!same_process())return error::denied;std::unique_lock lock{mutex_,std::try_to_lock};if(!lock.owns_lock())return error::again;if(!configured_)return error::denied;return core_.try_inspect(owner_,output);}
        /** @brief Check an existing caller-bound handle before a guest-driven native read. */
        error preflight_handle(caller who,handle cap,handle id,std::string_view action)
        {if(!same_process())return error::denied;lease allowed;auto status=authorized(who,cap,action,0,allowed,false);return status==error::success?live(who,cap,id):status;}
        /** @brief Observe queue presence under current take rights, without consuming or charging. */
        error pending(caller who,handle cap,handle id,bool& output)
        {auto status=preflight_handle(who,cap,id,"sensor.take");if(status!=error::success)return status;usage value;status=core_.describe(owner_,id,value);if(status==error::success)output=value.records!=0;return status;}
        /** @brief Trusted native ingress; zero currently authorized open handles admits no frames.
         * @details The adapter supplies complete owned bytes and actual source
         * metadata. This host method is not a guest import or transport endpoint.
         * Native source authentication/receiver isolation remains the adapter's
         * responsibility; the private core checks the bound identity and schema.
         */
        error ingest(source_sample actual,bytes body)
        {
            ingress_lease current;auto status=admit_ingress(current);return status==error::success?ingest(current,actual,body):status;
        }
        /** @brief Recheck after schema/clock reentry, before committing any native frame. */
        error ingest(ingress_lease const& current,source_sample actual,bytes body)
        {auto status=recheck_ingress(current);if(status!=error::success)return status;return core_.ingest(owner_,actual,body,nullptr,[&]{return recheck_ingress(current);});}
        /** @brief Deliver and retain one real sealed observation, with failed-copy replay.
         * @details The detached core frame is the sole source of both payload and
         * metadata. No caller-selected buffer is certified. No provider mutex is
         * held across the output sink. Local retirement and whole grant validity
         * are rechecked after callback reentry before queue/history publication.
         */
        template<typename Sink>error take(caller who,handle capability,handle id,Sink&& deliver,take_preflight preflight={})
        {
            if(!same_process())return error::denied;
            lease held;auto status=authorized(who,capability,"sensor.take",0,held,false);if(status!=error::success)return status;
            {
                std::lock_guard lock{mutex_};auto row=find(who,capability,id);if(!configured_||retired_)return error::denied;
                if(!row||!row->published||row->closing)return error::bad_handle;
                if(taking_)return error::again;
                taking_=true;
            }
            struct finish{source& self;~finish(){std::lock_guard lock{self.mutex_};self.taking_=false;for(auto& row:self.handles_)if(row.closing&&row.published)row={};}} done{*this};
            snapshot created;error callback_status=error::success;
            try
            {
                status=core_.take(owner_,id,[&](delivery const& frame)
                {
                    std::uint64_t framing{};if(preflight){callback_status=preflight(frame,framing);if(callback_status!=error::success)return false;}
                    if(framing>UINT64_MAX-frame.body.size()){callback_status=error::too_large;return false;}
                    lease charged;callback_status=authorized(who,capability,"sensor.take",frame.body.size()+framing,charged,true);
                    if(callback_status!=error::success)return false;
                    buffers::layout layout;layout.schema=policy_.buffer_profile;layout.rank=1;layout.bytes=layout.shape[0]=frame.body.size();layout.strides[0]=1;
                    callback_status=buffers_.create(policy_.buffer_alias.view(),layout,policy_.buffer_objects,[&](auto value)noexcept{created.buffer=value;return true;});
                    if(callback_status!=error::success)return false;
                    std::uint64_t written{};callback_status=buffers_.write(policy_.buffer_alias.view(),created.buffer,0,frame.body,written);
                    if(callback_status!=error::success)return false;
                    created.digest=buffers::hash(frame.body);callback_status=buffers_.seal(policy_.buffer_alias.view(),created.buffer,created.digest);
                    if(callback_status!=error::success)return false;
                    callback_status=buffers_.pin(policy_.buffer_alias.view(),created.buffer,created.pin);if(callback_status!=error::success)return false;
                    created.metadata=frame.metadata;
                    if(live(who,capability,id)!=error::success){callback_status=error::cancelled;return false;}
                    bool copied=deliver(retained_delivery{frame,created.buffer,created.digest});
                    lease current;callback_status=authorized(who,capability,"sensor.take",0,current,false);
                    if(callback_status==error::success)callback_status=live(who,capability,id);
                    if(callback_status==error::success)callback_status=created.pin.visit([&](auto,auto const&,auto const& digest)noexcept{return digest==created.digest;});
                    return copied&&callback_status==error::success;
                });
            }
            catch(std::bad_alloc const&){status=error::no_memory;}
            catch(std::length_error const&){status=error::too_large;}
            catch(...){status=error::io;}
            if(status!=error::success)
            {if(created.buffer)buffers_.close(policy_.buffer_alias.view(),created.buffer);return callback_status!=error::success?callback_status:status;}
            // All grant locks remain held through this final no-callback commit.
            std::lock_guard lock{mutex_};auto selected=find(who,capability,id);
            if(retired_||!selected||selected->closing){buffers_.close(policy_.buffer_alias.view(),created.buffer);return error::cancelled;}
            auto& replaced=history_[next_];if(replaced.buffer)buffers_.close(policy_.buffer_alias.view(),replaced.buffer);
            replaced=std::move(created);next_=(next_+1)%static_cast<std::size_t>(policy_.history);retained_=std::min(retained_+1,static_cast<std::size_t>(policy_.history));
            return error::success;
        }
        /** @brief Caller-bound close never retires somebody else's source handle. */
        error close(caller who,handle capability,handle id)
        {
            if(!same_process())return error::denied;
            lease held;auto status=authorized(who,capability,"resource.close",0,held,true);if(status!=error::success)return status;
            std::lock_guard lock{mutex_};auto row=find(who,capability,id);if(!row)return error::bad_handle;
            row->closing=true;status=core_.close(owner_,id);if(!taking_&&row->published)*row={};return status;
        }
        /** @brief Read actual cached resource counters only, after exact describe authority. */
        error describe(caller who,handle capability,handle id,usage& output,std::uint64_t carrier_bytes=0)
        {
            if(!same_process())return error::denied;
            lease held;auto status=authorized(who,capability,"sensor.describe",carrier_bytes,held,true);if(status!=error::success)return status;
            status=live(who,capability,id);return status==error::success?core_.describe(owner_,id,output):status;
        }
        proposal_dispatch::source_access access()noexcept{return {this,capture_adapter,validate_adapter};}
        /** @brief Resolve only the actual latest consumed/sealed native frame. */
        error capture(caller who,handle cap,handle resource,proposals::source_rule const& rule,proposal_dispatch::source_sample& output)
        {
            if(!same_process())return error::denied;
            lease held;auto status=authorized(who,cap,"resource.observe",0,held,false);if(status!=error::success)return status;
            std::lock_guard lock{mutex_};auto row=find(who,cap,resource);if(!configured_||retired_||!matches(rule))return error::denied;
            if(!row||!row->published||row->closing)return error::bad_handle;
            auto current=latest();if(!current)return error::again;
            frame_metadata actual;status=core_.observation(owner_,resource,actual);if(status!=error::success)return status;
            if(actual!=current->metadata)return error::again;
            status=current->pin.visit([&](auto,auto const& layout,auto const& digest)noexcept{return digest==current->digest&&layout.schema==policy_.buffer_profile;});
            if(status!=error::success)return status;
            output=sample(*row,*current);return error::success;
        }
        /** @brief Validate actual finite history, current source incarnation and real pin.
         * @details bounded_age does not synthesize a lost historical witness: a
         * snapshot evicted from this finite history is refused, even if a model
         * still holds its bytes. The proposal provider separately enforces age.
         */
        error validate(caller who,handle cap,proposals::source_rule const& rule,proposal_dispatch::source_sample const& current,
                       proposals::admission::observation const& captured,proposals::admission::observation_policy mode)
        {
            if(!same_process())return error::denied;
            lease held;auto status=authorized(who,cap,"resource.observe",0,held,false);if(status!=error::success)return status;
            std::lock_guard lock{mutex_};auto row=find(who,cap,captured.source);if(!configured_||retired_||!matches(rule))return error::denied;
            if(!row||!row->published||row->closing)return error::bad_handle;
            if(current.environment!=owner_.environment||current.owner!=owner_.grant||current.owner_generation!=owner_.generation||
               current.profile!=policy_.ingress.profile||current.frame!=policy_.ingress.frame)return error::denied;
            snapshot const* known{};for(auto const& entry:history_)if(entry.buffer&&entry.buffer==captured.buffer){known=&entry;break;}
            if(!known)return error::conflict;
            auto actual=sample(*row,*known).observation;actual.capability=captured.capability;actual.frame=captured.frame;
            if(actual!=captured)return error::conflict;
            frame_metadata latest_native;status=core_.observation(owner_,row->id,latest_native);if(status!=error::success)return status;
            if(latest_native.source.generation!=known->metadata.source.generation||latest_native.source.clock!=known->metadata.source.clock||
                latest_native.source.epoch!=known->metadata.source.epoch)return error::conflict;
            if(mode==proposals::admission::observation_policy::latest_required)
            {if(known!=latest()||known->metadata!=latest_native)return error::conflict;}
            else if(mode!=proposals::admission::observation_policy::bounded_age)return error::invalid;
            return known->pin.visit([&](auto,auto const& layout,auto const& digest)noexcept{return digest==known->digest&&layout.schema==policy_.buffer_profile;});
        }
        /** @brief Fence source disclosure/ingress; retained model pins remain charged. */
        void retire()noexcept
        {
            if(!same_process())return;
            std::lock_guard lock{mutex_};retired_=true;if(configured_)core_.retire(owner_);
            for(auto& entry:history_){if(entry.buffer)buffers_.close(policy_.buffer_alias.view(),entry.buffer);entry={};}
            retained_=0;
        }
    };
}
