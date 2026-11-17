/** @file
 * @brief Explicit device and control authority over framed scalar receivers.
 * @details Device handles, controller leases and sensor/buffer grants remain
 * independent. Control open retains explicit source references; no alias lookup
 * opens a capability on behalf of a guest. Commands use complete finite SI
 * schemas and actual sealed sensor witnesses. Native writes require the entire
 * original authority cohort, current controller epoch, source incarnation and
 * deadline. The receiver separately installs a lease before any command.
 *
 * The framed-scalar binding needs a deployment-qualified independent receiver
 * watchdog. Its four startup descriptors are directional message channels, not
 * guest-selectable sockets or in-process plugins. Separate reserved stop traffic
 * continues after revocation. A transport ACK is never a physical stopped
 * witness. This implementation has one controller per native session and no
 * implicit rearm, lease renewal or automatic retry after an unknown outcome.
 */
#pragma once
#include "device_execution.h"
#include "control_cycle.h"
#include "sensor_dispatch.h"
#include "robot_provider_response.h"
#include "platform_extension_policy.h"

namespace uwvm2::imported::ros::scene
{
    class device_dispatch
    {
    public:
        using operation=robot_contract::operation;using bytes=wire::bytes;
        using sink=proposal_dispatch::sink;using cancellation=proposal_dispatch::cancellation;
        struct device_plan
        {
            buffers::text<255> alias;buffers::profile mode,watchdog;
            devices::plan ledger;devices::execution::plan transport;control::plan arbitration;
            std::uint64_t retained_bytes{};
        };
        struct control_plan
        {
            buffers::text<255> alias,device_alias;buffers::profile selected,stop;
            std::array<proposals::source_rule,devices::maximum_observations> sources{};
            std::size_t source_count{};std::uint64_t lease_ns{},priority{},write_window_ns{},retained_bytes{};
        };
        struct limits {std::uint64_t devices{},controllers{},retained_bytes{};};
        struct inspection
        {std::uint64_t handles{},requests{},unknown{},pins{},retained_bytes{},controller_epoch{},sample_sequence{};bool latched{},retired{},ready{},stop_pending{},stop_acknowledged{};
            devices::execution::fault_record fault;};
    private:
        struct opened
        {caller who{};handle cap{},id{},last_key{};buffers::digest last_digest{};devices::receipt receipt;bool published{};};
        struct device_binding
        {
            device_plan policy;mutable std::mutex mutex;devices::owner owner;
            devices::core ledger;control::arbitration arbitration;devices::execution native{ledger,arbitration};
            std::array<opened,devices::maximum_handles> handles{};bool retired{};std::atomic<bool> fault_fenced{};
        };
        struct retained_sources
        {
            std::array<proposal_dispatch::source_sample,devices::maximum_observations> actual{};
            std::array<devices::observation,devices::maximum_observations> evidence{};
            std::size_t count{};
        };
        struct command_sources {handle request{};retained_sources sources;};
        struct control_binding
        {
            control_plan policy;std::shared_ptr<device_binding> device;mutable std::mutex mutex;
            ::uwvm2::utils::container::std_unique_ptr<control::cycle> cycle;
            control::principal principal;control::lease controller;caller who{};
            handle cap{},id{},device_cap{},device_id{},stop_key{},submit_key{},clock_id{};
            buffers::digest stop_digest{},submit_digest{};devices::receipt stopped,submitted;
            std::array<robot_contract::control_source,devices::maximum_observations> references{};
            retained_sources sampled;std::array<command_sources,devices::maximum_requests> commands{};
            bool published{},opening{},busy{},retired{};
        };
        std::shared_ptr<authority> permissions_;buffers::registry& buffers_;proposal_dispatch::source_access source_;
        handle environment_{},generation_{};limits capacity_;std::uint64_t retained_{};
        mutable std::mutex configuration_;
        ::uwvm2::utils::container::std_vector<std::shared_ptr<device_binding>> devices_;
        ::uwvm2::utils::container::std_vector<std::shared_ptr<control_binding>> controls_;
        bool prepared_{},retired_{};
        platform_extensions::policy extensions_;
        bool extensions_set_{};
        std::uint64_t(*clock_)()noexcept;
#if defined(__linux__)
        pid_t process_{::getpid()};
#endif
        bool same_process()const noexcept
        {
#if defined(__linux__)
            return process_==::getpid();
#else
            return true;
#endif
        }
        static std::uint64_t steady()noexcept{return static_cast<std::uint64_t>(resources::steady_now());}
        static bool same(caller a,caller b)noexcept{return a.instance==b.instance&&a.generation==b.generation;}
        static bool control_action(operation action)noexcept{return action>=operation::control_open&&action<=operation::control_stop;}
        static bool accepts(operation action)noexcept{return action<=operation::control_stop;}
        static std::string_view name(operation action)noexcept{return robot_contract::operations[static_cast<std::size_t>(action)].name;}
        static bool match(robot_contract::profile const& a,buffers::profile const& b)noexcept
        {return a.identity==b.identity.view()&&a.digest.size()==32&&std::equal(a.digest.begin(),a.digest.end(),b.sha256.begin());}
        static bool profile(wire::writer& out,buffers::profile const& value)noexcept{return out.text(value.identity.view())&&out.blob(value.sha256);}
        static bool receipt(wire::writer& out,devices::receipt const& value)noexcept
        {
            constexpr std::array<std::string_view,7> states{"admitted","queued","running","succeeded","failed","cancelled","outcome_unknown"};
            constexpr std::array<std::string_view,6> effects{"none","locally_accepted","committed","observed_remote","partial","unknown"};
            constexpr std::array<std::string_view,9> reasons{"","deadline","closed","retired","stopped","provider.changed","source.unavailable","native.rejected","native.unknown"};
            return out.u64(value.request)&&out.text(states[static_cast<unsigned>(value.status)])&&out.text(effects[static_cast<unsigned>(value.outcome)])&&
                out.u64(value.provider_generation)&&out.text(reasons[static_cast<unsigned>(value.why)]);
        }
        std::shared_ptr<device_binding> find_device(std::string_view alias)const noexcept
        {for(auto const& row:devices_)if(row->policy.alias.view()==alias)return row;return {};}
        std::shared_ptr<control_binding> find_control(std::string_view alias)const noexcept
        {for(auto const& row:controls_)if(row->policy.alias.view()==alias)return row;return {};}
        static opened* opened_for(device_binding& row,caller who,handle cap,handle id,bool history=false)noexcept
        {for(auto& item:row.handles)if((item.published||history)&&item.id==id&&item.cap==cap&&same(item.who,who))return &item;return nullptr;}
        static void collect_handles(device_binding& row)noexcept
        {for(auto& item:row.handles)if(item.id&&!item.published&&!row.ledger.retains(row.owner,item.id))item={};}
        error select(caller who,operation action,handle cap,std::shared_ptr<device_binding>& device,std::shared_ptr<control_binding>& controller)
        {
            if(!same_process()||!permissions_||!accepts(action))return error::denied;
            lease held;auto status=permissions_->admit(who,cap,name(action),0,held,false);if(status!=error::success)return status;
            auto const& grant=held.specification();std::lock_guard lock{configuration_};if(!prepared_||retired_)return error::denied;
            if(control_action(action))
            {if(grant.kind!=purpose::control||grant.binding!="framed-control"||!(controller=find_control(grant.alias)))return error::denied;device=controller->device;}
            else if(grant.kind!=purpose::device||grant.binding!="framed-scalar"||!(device=find_device(grant.alias)))return error::denied;
            if(!extensions_.allows(device->policy.alias.view())||!extensions_.allows(grant.alias))return error::denied;
            return error::success;
        }
        /** @brief Hold the original source, output and device/control scopes atomically.
         * @details Complete-cycle admission charges device.submit only once at
         * sample. Submit/result within that reserved phase revalidate all scopes
         * without borrowing another normal rate allowance. Stop uses the reserved
         * cleanup action independently of normal attenuation.
         */
        error group(caller who,handle cap,operation action,control_binding const& row,group_lease& output,
                    std::uint64_t bytes=0,bool charge=false,bool cycle_charge=false)
        {
            std::array<admission_claim,32> claims{};std::size_t count{};
            claims[count++]={cap,name(action),bytes,false,false,action==operation::control_stop||action==operation::device_stop};
            if(action==operation::control_stop||action==operation::device_stop)
            {auto status=permissions_->admit_group(who,{claims.data(),count},output,charge);
                return status!=error::success?status:!charge||output.consume(who,{claims.data(),count})?error::success:error::denied;}
            claims[count++]={row.device_cap,"device.submit",cycle_charge?row.device->policy.ledger.body_bytes:0,false,!cycle_charge};
            claims[count++]={row.device_cap,"device.stop",0,false,true};
            if(!control_action(action))claims[count++]={row.cap,"control.submit",0,false,true};
            auto base=count;
            for(std::size_t i{};i<row.policy.source_count;++i)
            {
                claims[count++]={row.references[i].sensor.capability,"resource.observe",0,true,true};
                claims[count++]={row.references[i].buffer_capability,"buffer.read",0,false,true};
                claims[count++]={row.references[i].buffer_capability,"data.export",0,true,true};
            }
            auto status=permissions_->admit_group(who,{claims.data(),count},output,charge);if(status!=error::success)return status;
            auto grant=output.specification(1);if(!grant||grant->kind!=purpose::device||grant->binding!="framed-scalar"||grant->alias!=row.policy.device_alias.view())return error::denied;
            if(!control_action(action))
            {grant=output.specification(3);if(!grant||grant->kind!=purpose::control||grant->binding!="framed-control"||grant->alias!=row.policy.alias.view())return error::denied;}
            for(std::size_t i{};i<row.policy.source_count;++i)
            {
                auto source=output.specification(base+i*3),buffer=output.specification(base+i*3+1);
                if(!source||source->kind!=purpose::sensor||source->binding!="bounded-sensor"||source->alias!=row.policy.sources[i].alias.view()||
                    !buffer||buffer->kind!=purpose::buffer||buffer->binding!="sealed-buffer"||buffer->alias!=row.policy.sources[i].buffer_alias.view())return error::denied;
            }
            return !charge||output.consume(who,{claims.data(),count})?error::success:error::denied;
        }
        /** @brief Capture real ingress witnesses and pin their exact immutable allocations. */
        error capture(control_binding const& row,retained_sources& retained,devices::observations& output)
        {
            retained={};auto time=clock_();
            for(std::size_t i{};i<row.policy.source_count;++i)
            {
                auto const& rule=row.policy.sources[i];auto const& reference=row.references[i];auto& actual=retained.actual[i];
                auto status=source_.capture(source_.context,row.who,reference.sensor.capability,reference.sensor.resource,rule,actual);if(status!=error::success)return status;
                auto& witness=actual.observation;witness.capability=reference.buffer_capability;
                if(actual.environment!=environment_||actual.owner_generation!=generation_||actual.profile!=rule.profile||actual.frame!=rule.frame||
                    witness.source!=reference.sensor.resource||!witness.buffer||!witness.captured_steady_ns||witness.captured_steady_ns>time||
                    time-witness.captured_steady_ns>row.device->policy.ledger.maximum_age_ns)return error::denied;
                status=source_.validate(source_.context,row.who,reference.sensor.capability,rule,actual,witness,proposals::admission::observation_policy::latest_required);
                if(status!=error::success)return status;
                buffers::information info;status=buffers_.describe(rule.buffer_alias.view(),witness.buffer,info);if(status!=error::success)return status;
                if(!info.sealed||info.value.schema!=rule.buffer_profile)return error::denied;
                status=buffers_.pin(rule.buffer_alias.view(),witness.buffer,output.pins[i]);if(status!=error::success)return status;
                output.values[i]={witness.source,witness.source_generation,witness.sample_sequence,witness.source_authority_epoch,witness.buffer,
                    witness.captured_steady_ns,info.sha256,rule.frame};retained.evidence[i]=output.values[i];
                ++output.count;++retained.count;
            }
            return error::success;
        }
        error validate(control_binding const& row,retained_sources const& retained,std::uint64_t time)
        {
            if(retained.count!=row.policy.source_count)return error::denied;
            for(std::size_t i{};i<retained.count;++i)
            {
                auto const& actual=retained.actual[i];auto const& witness=actual.observation;
                if(!time||time<witness.captured_steady_ns||time-witness.captured_steady_ns>row.device->policy.ledger.maximum_age_ns)return error::timeout;
                auto status=source_.validate(source_.context,row.who,row.references[i].sensor.capability,row.policy.sources[i],actual,witness,
                    proposals::admission::observation_policy::latest_required);if(status!=error::success)return status;
            }
            return error::success;
        }
        static bool observation(wire::writer& out,robot_contract::control_source const& reference,proposal_dispatch::source_sample const& sample)noexcept
        {
            auto const& value=sample.observation;
            return out.u64(reference.buffer_capability)&&out.u64(value.buffer)&&out.u64(value.source)&&out.u64(value.source_generation)&&
                out.boolean(true)&&out.u64(value.clock)&&out.u64(value.epoch)&&out.i64(value.source_ns)&&out.boolean(true)&&out.text(sample.frame.view());
        }
        static bool claims_match(robot_contract::request const& request,control_binding const& row,retained_sources const& sources)noexcept
        {
            if(request.dependencies.count||request.observations.count!=sources.count)return false;
            robot_contract::decoder in{request.observations.encoded,{request.observations.encoded.size(),0,255,0}};
            for(std::size_t i{};i<sources.count;++i)
            {
                robot_contract::observation input;if(!in.source(input))return false;bool found{};
                for(std::size_t j{};j<sources.count;++j)
                {
                    auto const& actual=sources.actual[j];auto const& witness=actual.observation;
                    if(input.input.capability!=row.references[j].buffer_capability||input.input.resource!=witness.buffer)continue;
                    found=input.source==witness.source&&input.generation==witness.source_generation&&input.stamped&&input.clock==witness.clock&&
                        input.epoch==witness.epoch&&input.nanoseconds==witness.source_ns&&input.framed&&input.frame==actual.frame.view();break;
                }
                if(!found)return false;
            }
            return in.done();
        }
    public:
        device_dispatch(std::shared_ptr<authority> permissions,buffers::registry& buffers,handle environment,handle generation,limits capacity,
                        std::uint64_t(*clock)()noexcept=steady)
            :permissions_(std::move(permissions)),buffers_(buffers),environment_(environment),generation_(generation),capacity_(capacity),clock_(clock?clock:steady){}
        /** @brief Install frozen child restrictions before opening any native lane.
         * @details Registration and switches do not grant a resource. Policy is
         * immutable while guests run; disabling a running provider uses parent
         * capability revocation, which preserves reserved stop delivery.
         */
        error configure_extensions(platform_extensions::policy value)
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{configuration_};
            if(prepared_||retired_||extensions_set_||!devices_.empty()||!controls_.empty())return error::denied;
            if(auto status=value.seal();status!=error::success)return status;
            extensions_=std::move(value);extensions_set_=true;return error::success;
        }
        static bool required_storage(device_plan const& value,std::uint64_t& output)noexcept
        {
            std::uint64_t core{};if(!identifier(value.alias.view())||!proposals::envelope_profile(value.mode)||!proposals::envelope_profile(value.watchdog)||
                !devices::core::required_storage(value.ledger,core)||value.ledger.storage_bytes<core||!value.transport.maximum_write||value.transport.maximum_write>4096||
                !value.transport.stop_ack_ns||value.transport.stop_ack_ns>INT64_MAX||value.arbitration.preemption||
                !value.arbitration.maximum_lease_ns||value.arbitration.maximum_lease_ns>INT64_MAX||!value.arbitration.prepare_ns||
                value.arbitration.prepare_ns>value.arbitration.maximum_lease_ns||value.arbitration.maximum_priority>UINT32_MAX)return false;
            output=sizeof(device_binding);return true;
        }
        static bool required_storage(control_plan const& value,std::uint64_t& output)noexcept
        {
            if(!identifier(value.alias.view())||!identifier(value.device_alias.view())||!proposals::envelope_profile(value.selected)||!proposals::envelope_profile(value.stop)||
                !value.source_count||value.source_count>value.sources.size()||!value.lease_ns||value.lease_ns>INT64_MAX||value.priority>UINT32_MAX||
                !value.write_window_ns||value.write_window_ns>value.lease_ns)return false;
            for(std::size_t i{};i<value.source_count;++i)
            {
                auto const& source=value.sources[i];if(!proposals::policy_text(source.alias,true)||!proposals::policy_text(source.buffer_alias,true)||
                    !proposals::envelope_profile(source.profile)||!proposals::envelope_profile(source.buffer_profile)||!proposals::policy_text(source.frame))return false;
                for(std::size_t j{};j<i;++j)if(value.sources[j].alias==source.alias)return false;
            }
            output=sizeof(control_binding)+sizeof(control::cycle);return true;
        }
        /** @brief Bind the complete native message lane before guest activation; no command bytes are written. */
        error bind(device_plan const& policy,std::array<int,4> endpoints)
        {
            if(!same_process())return error::denied;
            std::uint64_t required{};if(!environment_||!generation_||!required_storage(policy,required)||policy.retained_bytes<required)return error::invalid;
            std::lock_guard lock{configuration_};if(prepared_||retired_)return error::denied;if(find_device(policy.alias.view()))return error::conflict;
            if(devices_.size()>=capacity_.devices||retained_>capacity_.retained_bytes||required>capacity_.retained_bytes-retained_)return error::too_large;
            try
            {
                auto row=::uwvm2::utils::container::make_shared<device_binding>();row->policy=policy;
                row->owner={environment_,resources::allocate_identity(),generation_};
                auto status=row->ledger.configure(row->owner,generation_,policy.ledger);if(status!=error::success)return status;
                status=row->arbitration.configure(environment_,generation_,generation_,policy.arbitration);if(status!=error::success)return status;
                // A disabled child retains only inert common records. Even
                // descriptor inspection belongs to explicit child activation.
                if(extensions_.allows(policy.alias.view()))
                {
                    status=row->native.bind(endpoints[0],endpoints[1],endpoints[2],endpoints[3],policy.ledger.schema,policy.ledger.stop_profile,policy.transport);
                    if(status!=error::success)return status;
                }
                devices_.push_back(std::move(row));retained_+=required;return error::success;
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(std::length_error const&){return error::too_large;}
        }
        /** @brief Bind one fixed controller and exact source rules to an existing device. */
        error bind(control_plan const& policy)
        {
            if(!same_process())return error::denied;
            std::uint64_t required{};if(!required_storage(policy,required)||policy.retained_bytes<required)return error::invalid;
            std::lock_guard lock{configuration_};if(prepared_||retired_)return error::denied;if(find_control(policy.alias.view()))return error::conflict;
            auto device=find_device(policy.device_alias.view());if(!device)return error::denied;
            for(auto const& row:controls_)if(row->device==device)return error::conflict;
            auto const& allowed=device->policy;
            if(policy.lease_ns>allowed.arbitration.maximum_lease_ns||policy.priority>allowed.arbitration.maximum_priority||
                policy.write_window_ns>allowed.ledger.maximum_deadline_ns||policy.stop!=allowed.ledger.stop_profile||
                policy.source_count<allowed.ledger.minimum_observations||policy.source_count>allowed.ledger.observations)return error::invalid;
            for(std::size_t i{};i<policy.source_count;++i)if(policy.sources[i].frame!=allowed.ledger.schema.frame)return error::invalid;
            if(controls_.size()>=capacity_.controllers||retained_>capacity_.retained_bytes||required>capacity_.retained_bytes-retained_)return error::too_large;
            try
            {
                auto row=::uwvm2::utils::container::make_shared<control_binding>();row->policy=policy;row->device=device;
                row->cycle=::uwvm2::utils::container::make_unique<control::cycle>(device->arbitration,device->ledger,clock_);
                row->principal={environment_,resources::allocate_identity(),generation_,resources::allocate_identity()};if(!row->principal)return error::too_large;
                row->clock_id=resources::allocate_identity();if(!row->clock_id)return error::too_large;
                controls_.push_back(std::move(row));retained_+=required;return error::success;
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(std::length_error const&){return error::too_large;}
        }
        error prepare(proposal_dispatch::source_access source)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{configuration_};if(prepared_||retired_||!permissions_||!source.context||!source.capture||!source.validate||devices_.size()!=controls_.size())return error::invalid;
            source_=source;prepared_=true;return error::success;
        }
        error preflight(caller who,operation action,handle cap,std::size_t extent)
        {if(extent>16384)return error::too_large;std::shared_ptr<device_binding> device;std::shared_ptr<control_binding> control;return select(who,action,cap,device,control);}
        /** @brief Guest carrier entry; complete results are reserved before any new state transition. */
        error invoke(caller who,operation action,handle cap,bytes input,std::size_t capacity,std::size_t& required,sink deliver,cancellation cancelled={})noexcept;
        /** @brief Route a reviewed extension through the same common driver admission.
         * @details The alias comes from a live parent grant. The extension owns
         * no capability, counters, receipts or native descriptors. Common
         * authority, exposure, source evidence, arbitration and cancellation are
         * checked again by invoke, including around guest result delivery.
         * This is a trusted typed-carrier entry, not a guest raw-byte escape.
         */
        error invoke_extension(caller who,std::string_view module,std::string_view external,handle cap,bytes input,
            std::size_t capacity,std::size_t& required,sink deliver,cancellation cancelled={})noexcept
        {
            required=0;
            try
            {
                if(!same_process()||!permissions_)return error::denied;
                operation action{};
                {std::lock_guard lock{configuration_};if(!prepared_||retired_)return error::denied;
                    if(auto status=extensions_.operation(module,external,action);status!=error::success)return status;}
                std::shared_ptr<device_binding> device;std::shared_ptr<control_binding> controller;
                auto status=select(who,action,cap,device,controller);if(status!=error::success)return status;
                {std::lock_guard lock{configuration_};
                    status=extensions_.resolve(module,external,device->policy.alias.view(),action);}
                if(status!=error::success)return status;
                return invoke(who,action,cap,input,capacity,required,deliver,cancelled);
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(std::length_error const&){return error::too_large;}
        }
        /** @brief Trusted nonblocking scheduler step; independent stop pumping survives ordinary revocation. */
        error pump(std::string_view alias)noexcept;
        /** @brief Fence the exact device/controller pair after an isolated provider fails.
         * @details Only the owning host supervisor calls this entry. The
         * generation must match the current deployment; a delayed exit event
         * cannot retire a replacement. Both authority accounts are revoked in
         * one transaction before resource retirement. No effect is retried and
         * no process is restarted. Transport debt, source pins and the fixed
         * stop reservation survive. Unrelated devices and sensor grants do not
         * lose authority. The operation is idempotent for this incarnation.
         */
        error provider_failed(std::string_view alias,handle generation)noexcept;
        /** @brief One bounded step for each immutable prepared binding; no allocation or new authority. */
        void pump_all()noexcept
        {
            for(std::size_t i{};;++i)
            {
                buffers::text<255> alias;
                {std::lock_guard lock{configuration_};if(!prepared_||i>=devices_.size())break;alias=devices_[i]->policy.alias;}
                (void)pump(alias.view());
            }
        }
        /** @brief Determine whether native supervision is needed from frozen policy only. */
        bool has_active_bindings()const noexcept
        {std::lock_guard lock{configuration_};for(auto const& row:devices_)if(extensions_.allows(row->policy.alias.view()))return true;return false;}
        /** @brief Check reserved cleanup debt without treating an ACK as physical stopping. */
        bool drained()const noexcept
        {
            std::lock_guard lock{configuration_};
            for(auto const& row:devices_)
            {
                devices::execution::status state;if(row->native.inspect(state)!=error::success)return false;
                if(!state.stop_armed||state.stop_unknown)continue;
                devices::usage use;if(row->ledger.inspect(row->owner,use)!=error::success)return false;
                if(!use.stop_acknowledged)return false;
            }
            return true;
        }
        std::uint64_t stop_bound()const noexcept
        {std::lock_guard lock{configuration_};std::uint64_t bound{};for(auto const& row:devices_)bound=std::max(bound,row->policy.transport.stop_ack_ns);return bound;}
        error close(caller who,handle cap,handle id)noexcept;
        error inspect(std::string_view alias,inspection& output)const noexcept;
        void revoke(std::string_view alias)noexcept;
        void retire()noexcept;
    };

    inline error device_dispatch::invoke(caller who,operation action,handle cap,bytes input,std::size_t capacity,std::size_t& required,sink deliver,cancellation cancelled)noexcept
    {
        required=0;
        try
        {
            auto status=preflight(who,action,cap,input.size());if(status!=error::success)return status;
            if(!deliver)return error::invalid;
            robot_contract::request request;if(!robot_contract::decode(action,input,{16384,8192,255,32},request))return error::invalid;
            std::shared_ptr<device_binding> device;std::shared_ptr<control_binding> controller;
            status=select(who,action,cap,device,controller);if(status!=error::success)return status;
            auto check=[&]()
            {if(cancelled&&cancelled())return error::cancelled;std::shared_ptr<device_binding> d;std::shared_ptr<control_binding> c;return select(who,action,cap,d,c);};
            status=check();if(status!=error::success)return status;
            std::array<std::uint8_t,4096> storage;wire::writer out{storage};
            auto delivery=[&]()
            {required=out.size();if(capacity<required)return error::too_large;auto checked=check();if(checked!=error::success)return checked;
                if(!deliver(out.written())){return error::io;}return check();};
            if(action==operation::device_open)
            {
                required=8;if(capacity<required)return error::too_large;
                if(!match(request.selected,device->policy.ledger.profile))return error::denied;
                lease allowed;status=permissions_->admit(who,cap,name(action),input.size()+8,allowed,true);if(status!=error::success)return status;
                opened* slot{};auto maximum=allowed.specification().max_resources;
                {std::lock_guard lock{device->mutex};if(device->retired)return error::denied;
                    collect_handles(*device);
                    for(auto& row:device->handles)if(!row.cap){slot=&row;row.cap=cap;row.who=who;break;}}
                if(!slot)return error::again;
                allowed.release();error copied=error::success;
                status=device->ledger.open(device->owner,device->policy.ledger.profile,maximum,[&](handle id)
                {
                    {std::lock_guard lock{device->mutex};slot->id=id;}
                    out.u64(id);copied=delivery();return copied==error::success;
                });
                {std::lock_guard lock{device->mutex};if(status==error::success&&copied==error::success&&!device->retired)slot->published=true;
                    else{*slot={};if(status==error::success)status=error::cancelled;}}
                return copied!=error::success?copied:status;
            }
            if(action==operation::control_open)
            {
                required=32;if(capacity<required)return error::too_large;
                if(!match(request.selected,controller->policy.selected)||request.bindings.count!=controller->policy.source_count)return error::denied;
                lease primary;status=permissions_->preflight(who,cap,name(action),input.size()+32,primary);if(status!=error::success)return status;
                if(!primary.specification().max_resources)return error::again;
                {
                    std::lock_guard lock{device->mutex};if(!opened_for(*device,who,request.source.capability,request.source.resource))return error::bad_handle;
                }
                {
                    std::lock_guard lock{controller->mutex};if(controller->retired)return error::denied;
                    if(controller->published||controller->opening||controller->id)return error::again;
                    robot_contract::decoder in{request.bindings.encoded,{request.bindings.encoded.size(),0,0,0}};
                    for(std::size_t i{};i<controller->policy.source_count;++i)
                    {if(!in.control_input(controller->references[i])||controller->references[i].slot!=i+1)return error::invalid;}
                    if(!in.done())return error::invalid;
                    controller->who=who;controller->cap=cap;controller->device_cap=request.source.capability;controller->device_id=request.source.resource;controller->opening=true;
                }
                struct rollback
                {
                    control_binding& row;bool committed{};
                    ~rollback(){if(!committed){std::lock_guard lock{row.mutex};row.opening=false;row.cap=row.id=row.device_cap=row.device_id=0;row.who={};row.references={};}}
                } pending{*controller};
                group_lease scope;status=group(who,cap,action,*controller,scope);if(status!=error::success)return status;
                retained_sources sources;devices::observations pins;status=capture(*controller,sources,pins);if(status!=error::success)return status;
                control::prepared candidate;status=device->arbitration.prepare({controller->principal,controller->policy.priority,controller->policy.lease_ns},clock_(),candidate);
                if(status!=error::success)return status;
                struct discard {control::arbitration& arb;handle ticket;~discard(){if(ticket)(void)arb.discard(ticket);}} staged{device->arbitration,candidate.ticket};
                group_lease charged;status=group(who,cap,action,*controller,charged,input.size()+32,true);if(status!=error::success)return status;
                auto id=resources::allocate_identity();if(!id)return error::too_large;
                out.u64(id);out.u64(candidate.candidate.epoch);out.u64(candidate.candidate.provider_generation);out.u64(candidate.candidate.expires);
                charged.release();scope.release();primary.release();status=delivery();if(status!=error::success)return status;
                status=group(who,cap,action,*controller,scope);if(status!=error::success)return status;
                std::lock_guard lock{controller->mutex};if(controller->retired)return error::cancelled;
                status=device->arbitration.commit(candidate,clock_());staged.ticket=0;if(status!=error::success)return status;
                status=controller->cycle->configure(controller->principal,device->owner,controller->device_id,candidate.candidate,{controller->policy.write_window_ns},clock_());
                if(status==error::success)status=device->native.stage_controller(candidate.candidate,clock_());
                if(status!=error::success){controller->retired=true;device->native.retire();return status;}
                controller->controller=candidate.candidate;controller->id=id;controller->published=true;controller->opening=false;pending.committed=true;
                return error::success;
            }
            if(!control_action(action))
            {
                lease primary;status=permissions_->admit(who,cap,name(action),0,primary,false);if(status!=error::success)return status;
                {
                    std::lock_guard lock{device->mutex};if(!opened_for(*device,who,cap,request.resource,action==operation::device_result))return error::bad_handle;
                }
                if(action==operation::device_describe)
                {
                    devices::usage usage;devices::execution::status native;
                    status=device->ledger.inspect(device->owner,usage);if(status!=error::success)return status;
                    status=device->native.inspect(native);if(status!=error::success)return status;
                    auto const& p=device->policy;
                    bool armed=native.controller_ready&&!usage.latched&&!usage.retired;
                    auto state=usage.retired?"retired":usage.unknown||native.unknown||native.stop_unknown?"outcome_unknown":
                        usage.stopped_observed?"stopped":usage.stop_pending?"stopping":armed?"ready":"prepared";
                    profile(out,p.ledger.profile);profile(out,p.ledger.schema.profile);profile(out,p.ledger.stop_profile);out.text(state);out.u64(generation_);
                    profile(out,p.watchdog);out.text("verified_receiver_lease");out.u64(generation_);out.boolean(native.controller_ready);out.boolean(armed);
                    out.u64(p.arbitration.maximum_lease_ns);out.u64(usage.stop_reservation);
                    out.u64(p.ledger.body_bytes);out.u64(p.ledger.requests);out.u64(p.ledger.observations);out.u64(0);
                    out.u64(p.ledger.maximum_age_ns);out.u64(p.ledger.maximum_deadline_ns);out.u64(usage.unknown);
                    required=out.size();if(capacity<required)return error::too_large;
                    lease charged;status=permissions_->admit(who,cap,name(action),input.size()+required,charged,true);if(status!=error::success)return status;
                    charged.release();primary.release();return delivery();
                }
                required=action==operation::device_result?264:256;if(capacity<required)return error::too_large;
                if(action==operation::device_result)
                {
                    devices::receipt value;status=device->ledger.result(device->owner,request.resource,request.request_id,value);if(status!=error::success)return status;
                    lease charged;status=permissions_->admit(who,cap,name(action),input.size()+required,charged,true);if(status!=error::success)return status;
                    receipt(out,value);out.boolean(false);charged.release();primary.release();return delivery();
                }
                if(action==operation::device_stop)
                {
                    if(!match(request.selected,device->policy.ledger.stop_profile))return error::denied;
                    admission_claim claim{cap,name(action),0,false,false,true};group_lease stop;
                    status=permissions_->admit_group(who,{&claim,1},stop,true);if(status!=error::success)return status;
                    if(!stop.consume(who,{&claim,1}))return error::denied;
                    devices::receipt value;status=device->ledger.stop(device->owner,request.resource,request.key.sequence,device->policy.ledger.stop_profile,value);
                    if(status!=error::success){return status;}status=device->native.arm_stop();if(status!=error::success)return status;
                    handle id{};(void)device->arbitration.stop(id);
                    receipt(out,value);stop.release();primary.release();return delivery();
                }
                auto digest=buffers::hash(input);
                {
                    std::lock_guard lock{device->mutex};auto opened=opened_for(*device,who,cap,request.resource);if(!opened)return error::bad_handle;
                    if(request.key.sequence==opened->last_key)
                    {if(digest!=opened->last_digest)return error::conflict;receipt(out,opened->receipt);primary.release();}
                }
                if(out.size())return delivery();
                lease control_right;status=permissions_->admit(who,request.controller.capability,"control.submit",0,control_right,false);if(status!=error::success)return status;
                {std::lock_guard lock{configuration_};controller=find_control(control_right.specification().alias);}
                if(!controller||controller->device!=device)return error::denied;
                {std::lock_guard lock{controller->mutex};if(!controller->published||controller->retired)return error::denied;}
                group_lease scope;status=group(who,cap,action,*controller,scope);if(status!=error::success)return status;
                {
                    std::lock_guard lock{controller->mutex};if(controller->retired||!controller->published||controller->busy||!same(controller->who,who)||
                        controller->cap!=request.controller.capability||controller->id!=request.controller.resource||controller->device_id!=request.resource||
                        controller->device_cap!=cap||controller->controller.epoch!=request.epoch)return error::denied;
                    if(!match(request.mode,device->policy.mode)||!match(request.schema,device->policy.ledger.schema.profile)||
                        request.command.type!=devices::scalar_type||request.command.encoding!=devices::scalar_encoding)return error::denied;
                    devices::scalar_command decoded;status=devices::decode(request.command.body,device->policy.ledger.schema,decoded);if(status!=error::success)return status;
                    devices::observations pins;retained_sources sources;status=capture(*controller,sources,pins);if(status!=error::success)return status;
                    if(!claims_match(request,*controller,sources))return error::denied;
                    command_sources* retained{};for(auto& item:controller->commands)if(!item.request){retained=&item;break;}if(!retained)return error::again;
                    group_lease charged;status=group(who,cap,action,*controller,charged,input.size()+required,true);if(status!=error::success)return status;
                    status=device->arbitration.validate(controller->principal,controller->controller,clock_());if(status!=error::success)return status;
                    devices::receipt value;status=device->ledger.submit(device->owner,{request.resource,request.key.sequence,request.deadline,controller->controller,
                        device->policy.ledger.schema.profile,request.command.body},pins,clock_(),value);if(status!=error::success)return status;
                    retained->request=value.request;retained->sources=sources;
                    {std::lock_guard own{device->mutex};auto opened=opened_for(*device,who,cap,request.resource);if(!opened)return error::cancelled;
                        opened->last_key=request.key.sequence;opened->last_digest=digest;opened->receipt=value;}
                    receipt(out,value);
                }
                scope.release();primary.release();control_right.release();return delivery();
            }
            lease primary;status=permissions_->admit(who,cap,name(action),0,primary,false);if(status!=error::success)return status;
            {
                std::lock_guard lock{controller->mutex};if(!controller->published||controller->cap!=cap||controller->id!=request.resource||!same(controller->who,who))return error::bad_handle;
                if(controller->busy)return error::again;
            }
            if(action==operation::control_stop)
            {
                required=256;if(capacity<required)return error::too_large;
                if(!match(request.selected,controller->policy.stop))return error::denied;
                group_lease stop;status=group(who,cap,action,*controller,stop,0,true);if(status!=error::success)return status;
                {
                    std::lock_guard lock{controller->mutex};auto digest=buffers::hash(input);
                    if(controller->stop_key==request.key.sequence)
                    {if(controller->stop_digest!=digest)return error::conflict;}
                    else
                    {
                        if(controller->stop_key==UINT64_MAX||request.key.sequence!=controller->stop_key+1)return error::conflict;
                        status=controller->cycle->stop(controller->principal,controller->stopped);if(status!=error::success)return status;
                        controller->stop_key=request.key.sequence;controller->stop_digest=digest;
                    }
                    devices::receipt latest;if(device->ledger.pending_stop(latest)==error::success)controller->stopped=latest;
                    receipt(out,controller->stopped);
                }
                stop.release();primary.release();return delivery();
            }
            if(action==operation::control_result)
            {
                required=256;if(capacity<required)return error::too_large;devices::receipt value;
                status=controller->cycle->result(controller->principal,request.sequence,value);if(status!=error::success)return status;
                receipt(out,value);primary.release();return delivery();
            }
            auto digest=buffers::hash(input);
            if(action==operation::control_submit)
            {
                required=256;if(capacity<required)return error::too_large;
                {std::lock_guard lock{controller->mutex};if(request.key.sequence==controller->submit_key)
                    {if(digest!=controller->submit_digest)return error::conflict;receipt(out,controller->submitted);}}
                if(out.size()){primary.release();return delivery();}
            }
            group_lease scope;status=group(who,cap,action,*controller,scope);if(status!=error::success)return status;
            if(action==operation::control_sample)
            {
                required=128+controller->policy.source_count*384;if(capacity<required)return error::too_large;
                control::cycle::status before;status=controller->cycle->inspect(before);if(status!=error::success)return status;
                if(before.current==control::cycle::phase::submitted)
                {
                    devices::receipt prior;status=controller->cycle->result(controller->principal,before.sequence,prior);if(status!=error::success)return status;
                    status=controller->cycle->inspect(before);if(status!=error::success)return status;
                    if(before.current==control::cycle::phase::submitted)return error::again;
                }
                bool replay=before.current==control::cycle::phase::sampled;
                if(before.current==control::cycle::phase::latched)return error::cancelled;
                devices::observations pins;retained_sources sources;
                {
                    std::lock_guard lock{controller->mutex};if(controller->retired||controller->busy)return error::again;
                    if(replay)sources=controller->sampled;
                    else{status=capture(*controller,sources,pins);if(status!=error::success)return status;}
                    controller->busy=true;
                }
                struct leave {control_binding& row;~leave(){std::lock_guard lock{row.mutex};row.busy=false;}} done{*controller};
                group_lease charged;if(!replay)
                {status=group(who,cap,action,*controller,charged,input.size()+required+device->policy.ledger.body_bytes+512,true,true);if(status!=error::success)return status;}
                charged.release();scope.release();primary.release();
                error copied=error::success;
                status=controller->cycle->sample(controller->principal,clock_(),pins,[&](control::cycle::sample_record const& frame)
                {
                    out.u64(controller->id);out.u64(frame.sequence);out.u64(controller->controller.epoch);out.u64(controller->clock_id);out.u64(generation_);out.i64(static_cast<std::int64_t>(frame.captured_ns));
                    out.u64(frame.deadline);out.u64(sources.count);
                    // Canonical observations are ordered by the independent buffer reference.
                    std::array<std::size_t,devices::maximum_observations> order{};for(std::size_t i{};i<sources.count;++i)order[i]=i;
                    std::sort(order.begin(),order.begin()+static_cast<std::ptrdiff_t>(sources.count),[&](auto a,auto b)
                        {return robot_contract::ordered({controller->references[a].buffer_capability,sources.actual[a].observation.buffer},
                            {controller->references[b].buffer_capability,sources.actual[b].observation.buffer});});
                    for(std::size_t i{};i<sources.count;++i)if(!observation(out,controller->references[order[i]],sources.actual[order[i]])){copied=error::too_large;return false;}
                    out.u64(frame.missed);out.u64(0);
                    copied=check();if(copied==error::success)copied=group(who,cap,action,*controller,scope);
                    scope.release();if(copied==error::success)copied=delivery();
                    if(copied==error::success){copied=group(who,cap,action,*controller,scope);}return copied==error::success;
                });
                if(status==error::success){std::lock_guard lock{controller->mutex};controller->sampled=sources;
                    // The core acknowledges the previous terminal request when a new sample commits.
                    if(!replay)for(auto& item:controller->commands)if(item.request==before.request)item={};}
                return copied!=error::success?copied:status;
            }
            {
                std::lock_guard lock{controller->mutex};if(controller->retired||controller->busy)return error::cancelled;
                if(request.epoch!=controller->controller.epoch||!match(request.schema,device->policy.ledger.schema.profile)||
                    request.command.type!=devices::scalar_type||request.command.encoding!=devices::scalar_encoding)return error::denied;
                command_sources* retained{};for(auto& item:controller->commands)if(!item.request){retained=&item;break;}if(!retained)return error::again;
                devices::receipt value;status=controller->cycle->submit(controller->principal,request.key.sequence,request.sequence,
                    device->policy.ledger.schema.profile,request.command.body,clock_(),[&](auto const&,auto time){return validate(*controller,controller->sampled,time);},value);
                if(status!=error::success)return status;
                retained->request=value.request;retained->sources=controller->sampled;controller->submit_key=request.key.sequence;
                controller->submit_digest=digest;controller->submitted=value;receipt(out,value);
            }
            scope.release();primary.release();return delivery();
        }
        catch(std::bad_alloc const&){return error::no_memory;}catch(std::length_error const&){return error::too_large;}catch(...){return error::io;}
    }

    inline error device_dispatch::pump(std::string_view alias)noexcept
    {
        if(!same_process())return error::denied;
        try
        {
            std::shared_ptr<control_binding> row;
            {std::lock_guard lock{configuration_};if(!prepared_)return error::denied;
                if(!extensions_.allows(alias))return error::denied;
                for(auto const& selected:controls_)if(selected->policy.device_alias.view()==alias){row=selected;break;}}
            if(!row)return error::denied;
            caller who;handle cap{};bool published{};
            {std::lock_guard lock{row->mutex};who=row->who;cap=row->cap;published=row->published&&!row->retired;}
            error result;
            {
                group_lease scope;auto allowed=published?group(who,cap,operation::control_submit,*row,scope):error::denied;
                std::lock_guard lock{row->mutex};
                result=row->device->native.pump(clock_(),[&](devices::work const& work,std::uint64_t time)
                {
                    if(allowed!=error::success||!row->published||row->retired||work.controller!=row->controller)return error::denied;
                    if(row->busy)return error::again;
                    if(!work.ticket)return error::success;
                    for(auto const& command:row->commands)if(command.request==work.result.request)return validate(*row,command.sources,time);
                    return error::denied;
                });
            }
            // Release resource locks and the normal admission lease before
            // taking the irreversible authority -> resource fault transaction.
            devices::execution::status state;
            if(!row->device->fault_fenced.load(std::memory_order_acquire)&&row->device->native.inspect(state)==error::success&&
               state.fault.cause!=devices::execution::failure::none)(void)provider_failed(alias,generation_);
            return result;
        }
        catch(...){return error::io;}
    }
    inline error device_dispatch::provider_failed(std::string_view alias,handle generation)noexcept
    {
        if(!same_process()||!permissions_||generation!=generation_)return error::denied;
        try
        {
            std::shared_ptr<control_binding> controller;
            {std::lock_guard lock{configuration_};if(!prepared_)return error::denied;
                for(auto const& row:controls_)if(row->policy.device_alias.view()==alias){controller=row;break;}}
            if(!controller)return error::bad_handle;
            auto device=controller->device;
            if(device->fault_fenced.load(std::memory_order_acquire))return error::success;
            std::array<std::string_view,2> aliases{device->policy.alias.view(),controller->policy.alias.view()};
            return permissions_->revoke_group(aliases,[&]()noexcept
            {
                if(device->fault_fenced.load(std::memory_order_acquire))return;
                device->native.fail(clock_());
                revoke(device->policy.alias.view());
                device->fault_fenced.store(true,std::memory_order_release);
            });
        }
        catch(...){return error::io;}
    }
    inline error device_dispatch::close(caller who,handle cap,handle id)noexcept
    {
        if(!same_process())return error::denied;
        try
        {
            lease held;auto status=permissions_->admit(who,cap,"resource.close",0,held,false);if(status!=error::success)return status;
            auto const& grant=held.specification();std::shared_ptr<device_binding> device;std::shared_ptr<control_binding> control;
            {std::lock_guard lock{configuration_};device=find_device(grant.alias);control=find_control(grant.alias);}
            if(control)
            {
                std::lock_guard lock{control->mutex};if(!control->published||control->id!=id||control->cap!=cap||!same(control->who,who))return error::bad_handle;
                control->retired=true;control->published=false;(void)control->cycle->retire();control->device->native.retire();return error::success;
            }
            if(!device)return error::denied;
            bool closed{};
            {std::lock_guard lock{device->mutex};if(auto open=opened_for(*device,who,cap,id))
                {status=device->ledger.close(device->owner,id);if(status!=error::success)return status;open->published=false;closed=true;collect_handles(*device);}}
            if(closed)
            {
                // Match normal control->device lock order. Closing an activated
                // handle retires its controller even while no command is queued.
                std::lock_guard lock{configuration_};for(auto const& row:controls_)if(row->device==device)
                {std::lock_guard owned{row->mutex};if(row->device_id==id&&row->id)
                    {row->retired=true;(void)row->cycle->retire();row->device->native.retire();}}
                return error::success;
            }
            // Request identities can only be released by the original device capability/caller.
            bool owned{};{std::lock_guard lock{device->mutex};for(auto const& open:device->handles)if(open.cap==cap&&same(open.who,who))
                {devices::receipt receipt;if(device->ledger.result(device->owner,open.id,id,receipt)==error::success){owned=true;break;}}}
            if(!owned)return error::bad_handle;
            status=device->ledger.release(device->owner,id);if(status!=error::success)return status;
            {std::lock_guard lock{device->mutex};collect_handles(*device);}
            {std::lock_guard lock{configuration_};for(auto const& row:controls_)if(row->device==device)
                {std::lock_guard owned{row->mutex};for(auto& command:row->commands)if(command.request==id)command={};}}
            return error::success;
        }
        catch(...){return error::io;}
    }
    inline error device_dispatch::inspect(std::string_view alias,inspection& output)const noexcept
    {
        if(!same_process())return error::denied;
        std::shared_ptr<device_binding> device;std::shared_ptr<control_binding> control;
        {std::unique_lock lock{configuration_,std::try_to_lock};if(!lock.owns_lock())return error::again;
            device=find_device(alias);if(!device){control=find_control(alias);if(!control)return error::denied;device=control->device;}}
        devices::usage usage;auto status=device->ledger.inspect(device->owner,usage);if(status!=error::success)return status;
        devices::execution::status native;status=device->native.inspect(native);if(status!=error::success)return status;
        inspection value{usage.handles,usage.requests,usage.unknown,usage.pins,device->policy.retained_bytes,0,0,
            usage.latched,usage.retired,native.controller_ready,usage.stop_pending,usage.stop_acknowledged,native.fault};
        if(control)
        {std::unique_lock lock{control->mutex,std::try_to_lock};if(!lock.owns_lock())return error::again;
            value.handles=control->published?1:0;value.retained_bytes=control->policy.retained_bytes;value.controller_epoch=control->controller.epoch;value.retired=control->retired;
            if(control->id){control::cycle::status cached;if(control->cycle->inspect(cached)!=error::success)return error::again;value.sample_sequence=cached.sequence;}}
        output=value;return error::success;
    }
    inline void device_dispatch::revoke(std::string_view alias)noexcept
    {
        if(!same_process())return;
        std::lock_guard lock{configuration_};
        for(auto const& row:controls_)
        {
            bool dependent=row->policy.alias.view()==alias||row->policy.device_alias.view()==alias;
            for(std::size_t i{};i<row->policy.source_count;++i)dependent|=row->policy.sources[i].alias.view()==alias||row->policy.sources[i].buffer_alias.view()==alias;
            if(!dependent)continue;
            std::lock_guard owned{row->mutex};row->retired=true;if(row->id)(void)row->cycle->retire();row->device->native.retire();
            {std::lock_guard device{row->device->mutex};row->device->retired=true;}
            (void)row->device->ledger.retire();(void)row->device->arbitration.retire();
        }
    }
    inline void device_dispatch::retire()noexcept
    {
        if(!same_process())return;
        std::lock_guard lock{configuration_};retired_=true;
        for(auto const& row:controls_){std::lock_guard owned{row->mutex};row->retired=true;if(row->id)(void)row->cycle->retire();}
        for(auto const& row:devices_){std::lock_guard owned{row->mutex};row->retired=true;row->native.retire();(void)row->ledger.retire();(void)row->arbitration.retire();}
    }
}
