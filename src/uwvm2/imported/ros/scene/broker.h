/** @file
 * @brief Purpose-bound dispatch for the independent robot scene host.
 * @details Caller identity is supplied only by the runtime. All request bytes
 * are copied before admission; there are no guest addresses, sockets or host
 * paths in this layer. The caller prevalidates writable output and supplies a
 * transactional sink. A false sink must not consume a guard/timer event.
 * Owned clocks, timers and waitsets belong to one explicit policy environment.
 */
#pragma once
#include <uwvm2/utils/container/standard.h>
#include "authority.h"
#include "module_exchange.h"
#include "artifacts.h"
#include "outputs.h"
#include "managed_lifecycle.h"
#include "managed_lifecycle_contract.h"
#include "buffer_contract.h"
#include "model_dispatch.h"
#include "model_provider.h"
#include "sensor_dispatch.h"
#include "media_dispatch.h"
#include "bridge_dispatch.h"
#include "device_supervisor.h"
#include "driver_dispatch.h"
#include "proposal_guest.h"
#include "proposal_lifecycle_target.h"
#include "transform_dispatch.h"
#include "log_diagnostic_dispatch.h"
#include "recording_dispatch.h"
#include "communication_host.h"
#include "communication_provider.h"
#include "parameter_host.h"
#include "../runtime/contract.h"
#include <bit>
#include <functional>
#include <uwvm2/utils/container/function_ref.h>
#include <span>

namespace uwvm2::imported::ros::scene
{
    inline constexpr std::string_view module_name="uwvm_ros_v2";
    inline constexpr std::size_t transfer_limit=4u*1024u*1024u;
    using bytes=std::span<std::uint8_t const>;
    using sink=::uwvm2::utils::container::function_ref<bool(bytes)>;
    using buffer_sink=::uwvm2::utils::container::function_ref<bool(bytes,bytes)>;

    /** @brief Little-endian word codec; no host alignment or pointer layout. */
    inline std::uint64_t word(bytes input,std::size_t index) noexcept
    {std::uint64_t value{};for(unsigned n{};n!=8;++n)value|=std::uint64_t(input[index*8+n])<<(8*n);return value;}
    inline void put(std::span<std::uint8_t> output,std::size_t index,std::uint64_t value) noexcept
    {for(unsigned n{};n!=8;++n)output[index*8+n]=static_cast<std::uint8_t>(value>>(8*n));}

    /** @brief Independent environment containing only explicitly configured authority.
     * @details Metadata query and retirement never call native ROS, filesystem
     * or models. Runtime operation methods below are real implementations;
     * other purposes in the semantic policy catalogue require separate providers
     * and are not automatically made callable by adding a grant.
     */
    class broker
    {
        // Native worker facades retain this authority across broker teardown;
        // provider retirement still fences every job before its data is freed.
        std::shared_ptr<authority> authority_owner{::uwvm2::utils::container::make_shared<authority>()};
    public:
        authority& permissions{*authority_owner};
        module_exchange::router module_calls;
        std::shared_ptr<driver_dispatch> driver_store;
        resources::registry runtime;
        artifacts::registry inputs;
        outputs::registry output_store;
        managed_lifecycle::registry managed_nodes;
        buffers::registry buffer_store;
        model_dispatch model_store{authority_owner,buffer_store};
        ::uwvm2::utils::container::std_unique_ptr<sensor_dispatch> sensor_store;
        ::uwvm2::utils::container::std_unique_ptr<media_dispatch> media_store;
        ::uwvm2::utils::container::std_unique_ptr<proposal_lifecycle_target> proposal_target_store;
        ::uwvm2::utils::container::std_unique_ptr<proposal_guest_dispatch> proposal_store;
        transform_dispatch transform_store{authority_owner,runtime};
        ::uwvm2::utils::container::std_unique_ptr<log_diagnostic_dispatch> log_diagnostic_store;
        ::uwvm2::utils::container::std_unique_ptr<recording_dispatch> recording_store;
        ::uwvm2::utils::container::std_unique_ptr<communication_host> communication_store;
        ::uwvm2::utils::container::std_unique_ptr<bridge_dispatch> bridge_store;
        ::uwvm2::utils::container::std_unique_ptr<device_dispatch> device_store;
        ::uwvm2::utils::container::std_unique_ptr<device_supervisor> device_worker;
        ::uwvm2::utils::container::std_unique_ptr<parameter_host> parameter_store;
        // Filled only by trusted preparation, then immutable while guests run.
        // Its owners are destroyed before the dispatcher referenced by workers.
        ::uwvm2::utils::container::std_map<::uwvm2::utils::container::std_string,std::shared_ptr<models::provider_lifetime>,std::less<>> model_workers;
        ::uwvm2::utils::container::std_map<::uwvm2::utils::container::std_string,std::shared_ptr<communication::provider_lifetime>,std::less<>> communication_workers;

        ~broker()
        {
            if(device_worker)device_worker->retire();
            if(bridge_store)bridge_store->retire();
            if(parameter_store)parameter_store->retire();
            for(auto const& [alias,worker]:model_workers){(void)alias;worker->request_stop();}
            for(auto const& [name,worker]:communication_workers){(void)name;worker->request_stop();}
            for(auto const& [alias,worker]:model_workers){(void)alias;worker->retire();}
            for(auto const& [name,worker]:communication_workers){(void)name;worker->retire();}
        }

        /** @brief Allocate finite communication routing without granting or starting native work.
         * @details Trusted deployment binds canonical plans, retained creation
         * sources and independently owned worker lifetimes before prepare().
         * Recoverable allocation failure publishes no partial communication host.
         */
        error configure_communication_dispatch(std::uint64_t identity,communication_host::limits limits)
        {
            if(communication_store)return error::conflict;
            if(!identity)return error::invalid;
            auto* value=::uwvm2::utils::allocator::fast_io_strict::try_object<communication_host>(
                [&](auto* p){return std::construct_at(p,authority_owner,identity,identity,limits);});
            if(!value)return error::no_memory;
            communication_store.reset(value);return error::success;
        }
        /** @brief Exact supported communication purpose; a name alone cannot choose a provider. */
        static bool communication_policy(grant const& policy)noexcept
        {return service_observer(policy)||(policy.binding=="native-communication"&&(policy.kind==purpose::context||policy.kind==purpose::node||
            policy.kind==purpose::type||policy.kind==purpose::topic||policy.kind==purpose::service))||
            (policy.binding=="native-action"&&policy.kind==purpose::action)||
            (policy.binding=="native-graph"&&policy.kind==purpose::discovery)||(policy.binding=="event-stream"&&policy.kind==purpose::event);}
        error invoke_communication(caller who,communication_contract::operation action,handle cap,bytes request,std::size_t capacity,
                                   std::size_t& required,sink deliver,communication_host::cancellation cancelled={})
        {required=0;return communication_store?communication_store->invoke(who,action,cap,request,capacity,required,deliver,cancelled):error::denied;}
        error invoke_communication_receive(caller who,communication_receive_contract::operation action,handle cap,bytes request,std::size_t capacity,
                                           std::size_t& required,sink deliver,communication_host::cancellation cancelled={})
        {required=0;return communication_store?communication_store->invoke_receive(who,action,cap,request,capacity,required,deliver,cancelled):error::denied;}
        error invoke_communication_action(caller who,action_contract::operation action,handle cap,bytes request,std::size_t capacity,
                                          std::size_t& required,sink deliver,communication_host::cancellation cancelled={})
        {required=0;return communication_store?communication_store->invoke_action(who,action,cap,request,capacity,required,deliver,cancelled):error::denied;}
        template<auto Action>error invoke_communication_observation(caller who,handle cap,bytes request,std::size_t capacity,
                                          std::size_t& required,sink deliver,communication_host::cancellation cancelled={})
        {required=0;return communication_store?communication_store->invoke_observation<Action>(who,cap,request,capacity,required,deliver,cancelled):error::denied;}
        error invoke_communication_creation(caller who,communication_open_dispatch::operation action,handle cap,bytes request,std::size_t capacity,
                                            std::size_t& required,sink deliver,communication_host::cancellation cancelled={})
        {required=0;return communication_store?communication_store->invoke_creation(who,action,cap,request,capacity,required,deliver,cancelled):error::denied;}
        /** @brief Construct exact-node parameter stores only after communication routing exists. */
        error configure_parameter_dispatch(std::uint64_t bindings)
        {
            if(parameter_store)return error::conflict;
            if(!communication_store||!bindings||bindings>128)return error::invalid;
            auto* value=::uwvm2::utils::allocator::fast_io_strict::try_object<parameter_host>(
                [&](auto* p){return std::construct_at(p,authority_owner,*communication_store,bindings);});
            if(!value)return error::no_memory;
            parameter_store.reset(value);return error::success;
        }
        error invoke_parameter(caller who,parameter_contract::operation action,handle cap,bytes request,std::size_t capacity,
                               std::size_t& required,sink deliver,parameters::cancellation cancelled={})
        {
            required=0;if(!parameter_store)return error::denied;
            std::uint64_t needed{};auto status=parameter_store->invoke(who,action,cap,request,capacity,needed,deliver,cancelled);
            if(needed>SIZE_MAX)return error::too_large;
            required=static_cast<std::size_t>(needed);return status;
        }
        /** @brief Close one capability and fence unpublished startup using that exact handle.
         * @details Published endpoints retain their independently authorized
         * cleanup path. Closing a capability never resets its grant's budget.
         */
        error close_capability(caller who,handle cap)
        {
            auto status=permissions.close(who,cap);
            if(status==error::success&&communication_store)communication_store->capability_closed(cap);
            return status;
        }

        /** @brief Reserve device/control storage without probing native descriptors or creating authority. */

        error configure_device_dispatch(std::uint64_t identity,device_dispatch::limits capacity,device_supervisor::plan supervision)
        {
            if(device_store||device_worker)return error::conflict;
            if(!identity)return error::invalid;
            auto dispatcher=::uwvm2::utils::container::make_unique<device_dispatch>(authority_owner,buffer_store,identity,identity,capacity);
            auto supervisor=::uwvm2::utils::container::make_unique<device_supervisor>(*dispatcher,supervision);
            device_store=std::move(dispatcher);device_worker=std::move(supervisor);return error::success;
        }
        /** @brief Route the closed scalar device and complete control-cycle records. */
        error invoke_device(caller who,robot_contract::operation action,handle cap,bytes request,std::size_t capacity,std::size_t& required,sink deliver,
                            ::uwvm2::utils::container::function_ref<bool()> cancelled={})
        {return device_store?device_store->invoke(who,action,cap,request,capacity,required,deliver,cancelled):error::denied;}
        /** @brief Reserve finite media bindings without issuing source or destination authority. */
        error configure_media_dispatch(std::uint64_t identity,media_dispatch::limits limits)
        {
            if(media_store)return error::conflict;
            if(!identity)return error::invalid;
            auto* value=::uwvm2::utils::allocator::fast_io_strict::try_object<media_dispatch>(
                [&](auto* p){return std::construct_at(p,authority_owner,buffer_store,identity,limits);});
            if(!value)return error::no_memory;
            media_store.reset(value);return error::success;
        }
        /** @brief Bind a directed bridge to the existing independently admitted communication host. */
        error configure_bridge_dispatch(std::uint64_t identity,bridge_dispatch::limits limits)
        {
            if(bridge_store)return error::conflict;
            if(!identity||!communication_store)return error::invalid;
            auto* value=::uwvm2::utils::allocator::fast_io_strict::try_object<bridge_dispatch>(
                [&](auto* p){return std::construct_at(p,authority_owner,*communication_store,identity,limits);});
            if(!value)return error::no_memory;
            bridge_store.reset(value);return error::success;
        }
        error invoke_bridge(caller who,robot_contract::operation action,handle cap,bytes request,std::size_t capacity,std::size_t& required,sink deliver,
                            ::uwvm2::utils::container::function_ref<bool()> cancelled={})
        {return bridge_store?bridge_store->invoke(who,action,cap,request,capacity,required,deliver,cancelled):error::denied;}
        /** @brief Execute a closed media operation under independent input/output grants. */
        error invoke_media(caller who,robot_contract::operation action,handle cap,bytes request,std::size_t capacity,std::size_t& required,sink deliver,
                           ::uwvm2::utils::container::function_ref<bool()> cancelled={})
        {return media_store?media_store->invoke(who,action,cap,request,capacity,required,deliver,cancelled):error::denied;}
        /** @brief Create finite sensor dispatch without attaching an endpoint or issuing authority. */
        error configure_sensor_dispatch(std::uint64_t identity,sensor_dispatch::limits limits)
        {
            if(sensor_store)return error::conflict;
            if(!identity)return error::invalid;
            auto* value=::uwvm2::utils::allocator::fast_io_strict::try_object<sensor_dispatch>(
                [&](auto* p){return std::construct_at(p,authority_owner,buffer_store,identity,identity,limits);});
            if(!value)return error::no_memory;
            sensor_store.reset(value);return error::success;
        }
        /** @brief Configure inert proposal/capture storage and only an actual local lifecycle target facade. */
        error configure_proposal_dispatch(std::uint64_t identity,proposal_dispatch::limits limits,proposals::guest_limits guest)
        {
            if(proposal_store||proposal_target_store)return error::conflict;
            if(!identity)return error::invalid;
            auto* targets=::uwvm2::utils::allocator::fast_io_strict::try_object<proposal_lifecycle_target>(
                [&](auto* p){return std::construct_at(p,authority_owner,managed_nodes,identity,identity);});
            if(!targets)return error::no_memory;
            ::uwvm2::utils::container::std_unique_ptr<proposal_lifecycle_target> target{targets};
            auto* value=::uwvm2::utils::allocator::fast_io_strict::try_object<proposal_guest_dispatch>(
                [&](auto* p){return std::construct_at(p,authority_owner,model_store,buffer_store,identity,identity,limits);});
            if(!value)return error::no_memory;
            ::uwvm2::utils::container::std_unique_ptr<proposal_guest_dispatch> prepared{value};
            auto status=prepared->configure(guest);if(status!=error::success)return status;
            proposal_target_store=std::move(target);proposal_store=std::move(prepared);return error::success;
        }

        /** @brief Exact-purpose sensor carrier; no absent provider is silently activated. */
        error invoke_sensor(caller who,sensor_contract::operation action,handle cap,bytes request,std::size_t capacity,std::size_t& required,sink deliver,
                            ::uwvm2::utils::container::function_ref<bool()> cancelled={})
        {return sensor_store?sensor_store->invoke(who,action,cap,request,capacity,required,deliver,cancelled):error::denied;}
        /** @brief Explicit captured-input/model-output publication or inert read/admission carrier. */
        error invoke_proposal(caller who,proposal_contract::operation action,handle cap,bytes request,std::size_t capacity,std::size_t& required,sink deliver,
                              ::uwvm2::utils::container::function_ref<bool()> cancelled={})
        {return proposal_store?proposal_store->invoke(who,action,cap,request,capacity,required,deliver,cancelled):error::denied;}

        /** @brief Create a private log/diagnostic dispatcher from already validated finite reservations.
         * @details Only trusted deployment calls this before enrollment. No file,
         * console, socket or native logger is selected, and no capability is minted.
         */
        error configure_log_diagnostic(std::uint64_t identity,log_diagnostic_dispatch::limits limits)
        {
            if(log_diagnostic_store)return error::conflict;
            if(!identity)return error::invalid;
            log_diagnostic_store=::uwvm2::utils::container::make_unique<log_diagnostic_dispatch>(authority_owner,identity,limits);
            return error::success;
        }

        /** @brief Create an inert finite recording dispatcher using recoverable fast_io allocation.
         * @details Trusted deployment binds its channels and source profiles
         * before enrollment. Construction performs no file, socket or ROS I/O.
         */
        error configure_recording_dispatch(std::uint64_t identity,recording_dispatch::limits limits)
        {
            if(recording_store)return error::conflict;
            if(!identity)return error::invalid;
            auto* value=::uwvm2::utils::allocator::fast_io_strict::try_object<recording_dispatch>(
                [&](auto* p){return std::construct_at(p,authority_owner,buffer_store,identity,limits);});
            if(!value)return error::no_memory;
            recording_store.reset(value);return error::success;
        }

        /** @brief Recheck current authority after a possibly reentrant cancellation hook.
         * @details A recursive admission lease does not prevent its own thread
         * from revoking a grant or closing a capability in a trusted callback.
         * A false cancellation result therefore never substitutes for current
         * authorization before an effect or output copy. Rechecks do not charge.
         */
        template<typename Action> error callback_checkpoint(caller who,handle cap,Action action,
            ::uwvm2::utils::container::function_ref<bool()> cancelled={})
        {
            if(cancelled&&cancelled())return error::cancelled;
            lease current;return permissions.admit(who,cap,action,0,current,false);
        }

        /** @brief Dispatch locally bound model jobs through compound source authority.
         * @details The provider must be explicitly bound and attached by a
         * trusted deployment. No model name loads code or selects a URL. The
         * worker facade copies inputs under short authority leases and runs
         * inference outside those locks. Current source rights are checked
         * again before any predicted output is published.
         */
        error invoke_model(caller who,model_contract::operation action,handle cap,bytes request,
                           std::size_t capacity,std::size_t& required,sink deliver,
                           ::uwvm2::utils::container::function_ref<bool()> cancelled={})
        {
            if(action==model_contract::operation::request_describe||action==model_contract::operation::request_release)
            {
                lease origin;auto name=action==model_contract::operation::request_release?"request.release":"request.describe";
                auto status=permissions.admit(who,cap,name,0,origin,false);if(status!=error::success)return status;
                if(communication_policy(origin.specification()))
                    return invoke_communication_creation(who,action==model_contract::operation::request_release?communication_open_dispatch::operation::release:
                        communication_open_dispatch::operation::describe,cap,request,capacity,required,deliver,cancelled);
                if(origin.specification().kind==purpose::parameter&&origin.specification().binding=="local-parameters")
                {
                    if(!parameter_store)return error::denied;
                    std::uint64_t needed{};status=parameter_store->invoke_request(who,action==model_contract::operation::request_release,
                        cap,request,capacity,needed,deliver,cancelled);
                    if(needed>SIZE_MAX)return error::too_large;
                    required=static_cast<std::size_t>(needed);return status;
                }
                if(origin.specification().kind==purpose::transform)
                    return transform_store.invoke_request(who,action==model_contract::operation::request_release,cap,request,capacity,required,deliver,cancelled);
                if(origin.specification().kind==purpose::arbiter)
                {
                    if(!proposal_store)return error::denied;
                    if(request.size()!=8)return error::invalid;
                    return proposal_store->request(who,cap,word(request,0),action==model_contract::operation::request_release,capacity,required,deliver,cancelled);
                }
            }
            return model_store.invoke(who,action,cap,request,capacity,required,deliver,cancelled);
        }
        /** @brief Route shared request preflight by the already granted purpose. */
        error preflight_model_or_request(caller who,model_contract::operation action,handle cap,std::size_t bytes,bool fits=true)
        {
            if(action==model_contract::operation::request_describe||action==model_contract::operation::request_release)
            {
                lease origin;auto name=action==model_contract::operation::request_release?"request.release":"request.describe";
                auto status=permissions.admit(who,cap,name,0,origin,false);if(status!=error::success)return status;
                if(communication_policy(origin.specification()))
                    return communication_store?communication_store->preflight_creation(who,action==model_contract::operation::request_release?
                        communication_open_dispatch::operation::release:communication_open_dispatch::operation::describe,cap,bytes):error::denied;
                if(origin.specification().kind==purpose::transform)
                    return transform_store.preflight_request(who,action==model_contract::operation::request_release,cap,bytes,fits);
                if(origin.specification().kind==purpose::parameter&&origin.specification().binding=="local-parameters")
                    return parameter_store?parameter_store->preflight_request(who,action==model_contract::operation::request_release,cap,bytes,fits):error::denied;
                if(origin.specification().kind==purpose::arbiter)
                {
                    if(!proposal_store||origin.specification().binding!="proposal-arbiter")return error::denied;
                    if(bytes!=8)return error::invalid;
                    return fits?permissions.preflight(who,cap,name,bytes,origin):error::success;
                }
            }
            return model_store.preflight(who,action,cap,bytes,fits);
        }

        /** @brief Refuse ungranted/oversized buffer requests before carrier allocation.
         * @details This observes exact action authority and the configured
         * transfer ceiling without charging. It cannot create, seal, pin or
         * read a buffer. Final admission repeats the authority check after the
         * complete owned request has been decoded.
         */
        error preflight_buffer(caller who,buffer_contract::operation action,handle cap,std::size_t size,bool complete_receipt=true)
        {
            auto index=static_cast<std::size_t>(action);if(index>=buffer_contract::operations.size())return error::unsupported;
            if(size>transfer_limit+24)return error::too_large;
            auto minimum_reply=action==buffer_contract::operation::seal?16u:8u;
            lease allowed;auto status=complete_receipt?
                permissions.preflight(who,cap,buffer_contract::operations[index].name,size+minimum_reply,allowed):
                permissions.admit(who,cap,buffer_contract::operations[index].name,0,allowed,false);
            if(status!=error::success)return status;
            auto const& spec=allowed.specification();if(spec.kind!=purpose::buffer||spec.binding!="sealed-buffer")return error::denied;
            std::uint64_t transfer{};status=buffer_store.transfer_limit(spec.alias,transfer);if(status!=error::success)return status;
            return size<=buffer_contract::request_bound(action,transfer)?error::success:error::too_large;
        }

        /** @brief Charge a large owned buffer carrier before its heap allocation.
         * @details Inline carriers have fixed bounded stack storage. Larger
         * requests reserve their entire encoded extent, including framing,
         * from the binding's shared read/input workspace. The token outlives
         * the copied bytes and does not replace final charged admission.
         */
        error reserve_buffer_workspace(caller who,buffer_contract::operation action,handle cap,std::size_t size,
                                       buffers::registry::workspace_lease& output)
        {
            auto index=static_cast<std::size_t>(action);if(index>=buffer_contract::operations.size())return error::unsupported;
            if(size<=byte_buffer::inline_capacity||size>transfer_limit+24)return error::invalid;
            lease allowed;auto status=permissions.preflight(who,cap,buffer_contract::operations[index].name,size+8,allowed);
            if(status!=error::success)return status;
            auto const& spec=allowed.specification();if(spec.kind!=purpose::buffer||spec.binding!="sealed-buffer")return error::denied;
            std::uint64_t transfer{};status=buffer_store.transfer_limit(spec.alias,transfer);if(status!=error::success)return status;
            if(size>buffer_contract::request_bound(action,transfer))return error::too_large;
            return buffer_store.reserve_workspace(spec.alias,size,output);
        }
        /** @brief Dispatch canonical buffer records with transactional publication.
         * @details Delivery has two spans to encode a read's length prefix and
         * provider-owned detached data without a second large allocation. Both
         * spans are synchronous and never retained. All capacities are checked
         * before effects; failed create delivery rolls back its unpublished
         * handle. Write/seal failures after an effect recover by exact retry.
         * Buffer refs contain this exact independently authorized capability.
         */
        error invoke_buffer(caller who,buffer_contract::operation action,handle cap,bytes request,
                            std::size_t capacity,std::size_t& required,buffer_sink const& deliver,
                            ::uwvm2::utils::container::function_ref<bool()> const& cancelled={})
        {
            using op=buffer_contract::operation;required=0;
            auto index=static_cast<std::size_t>(action);if(index>=buffer_contract::operations.size())return error::unsupported;
            if(capacity>transfer_limit+buffer_contract::maximum_layout_wire||request.size()>transfer_limit+24)return error::invalid;
            auto status=preflight_buffer(who,action,cap,request.size(),capacity>=(action==op::seal?16u:8u));if(status!=error::success)return status;
            buffer_contract::request decoded;if(!buffer_contract::decode(action,request,decoded))return error::invalid;
            lease admitted;status=permissions.admit(who,cap,buffer_contract::operations[index].name,0,admitted,false);
            if(status!=error::success)return status;
            auto const& spec=admitted.specification();
            if(spec.kind!=purpose::buffer||spec.binding!="sealed-buffer")return error::denied;
            std::array<std::uint8_t,1024> output{};
            buffers::information description;
            if(action==op::describe)
            {
                status=buffer_store.describe(spec.alias,decoded.buffer,description);if(status!=error::success)return status;
                auto measure=wire::writer::measuring(output.size());
                if(!buffer_contract::encode_description(measure,description))return error::invalid;
                required=measure.size();
            }
            else required=action==op::seal?16:action==op::read?static_cast<std::size_t>(decoded.bytes)+8:8;
            if(capacity<required)return error::too_large;
            lease charged;status=permissions.admit(who,cap,buffer_contract::operations[index].name,request.size()+required,charged,true);
            if(status!=error::success)return status;
            status=callback_checkpoint(who,cap,buffer_contract::operations[index].name,cancelled);if(status!=error::success)return status;
            auto emit=[&](bytes first,bytes second=bytes{})
            {return callback_checkpoint(who,cap,buffer_contract::operations[index].name,cancelled)==error::success&&deliver(first,second)&&
                callback_checkpoint(who,cap,buffer_contract::operations[index].name,cancelled)==error::success;};
            switch(action)
            {
            case op::create:
                return buffer_store.create(spec.alias,decoded.format,spec.max_resources,[&](handle id)
                {put(output,0,id);return emit({output.data(),8});});
            case op::write:
            {
                std::uint64_t accepted{};status=buffer_store.write(spec.alias,decoded.buffer,decoded.offset,decoded.payload,accepted);
                if(status!=error::success)return status;
                put(output,0,accepted);return emit({output.data(),8})?error::success:error::invalid;
            }
            case op::seal:
                status=buffer_store.seal(spec.alias,decoded.buffer,decoded.sha256);if(status!=error::success)return status;
                put(output,0,cap);put(output,1,decoded.buffer);return emit({output.data(),16})?error::success:error::invalid;
            case op::read:
                return buffer_store.read(spec.alias,decoded.buffer,decoded.offset,decoded.bytes,[&](bytes data)
                {put(output,0,data.size());return emit({output.data(),8},data);});
            case op::describe:
            {
                wire::writer encoded{output};if(!buffer_contract::encode_description(encoded,description))return error::invalid;
                return emit(encoded.written())?error::success:error::invalid;
            }
            }
            return error::unsupported;
        }

        /** @brief Dispatch immutable artifact operations under exact authority.
         * @details The request contains no host path. Open matches deployment
         * generation/digest and rolls back failed output. Read stages a bounded
         * range and never consumes bytes or a cursor. Describe returns approved
         * metadata only. Short output reports requested size only after action
         * authorization, without charging a call. Other admitted failures keep
         * their admission charge; closing/reopening never refunds attenuation.
         */
        error invoke_artifact(caller who,artifact_contract::operation action,handle cap,bytes request,
                              std::size_t capacity,std::size_t& required,sink const& deliver,
                              ::uwvm2::utils::container::function_ref<bool()> const& cancelled={})
        {
            using op=artifact_contract::operation;
            auto index=static_cast<std::size_t>(action);required=0;
            if(index>=artifact_contract::operations.size())return error::unsupported;
            auto const& description=artifact_contract::operations[index];
            if(request.size()!=description.words*8u||capacity>transfer_limit)return error::invalid;
            std::uint64_t reservation=description.bytes<0?word(request,2):static_cast<std::uint64_t>(description.bytes);
            if(reservation>transfer_limit)return error::too_large;
            if(description.bytes<0&&reservation>UINT64_MAX-word(request,1))return error::too_large;
            lease admitted;
            auto status=permissions.admit(who,cap,action,capacity<reservation?0:request.size()+reservation,
                                          admitted,capacity>=reservation);
            if(status!=error::success)return status;
            if(capacity<reservation){required=static_cast<std::size_t>(reservation);return error::too_large;}
            status=callback_checkpoint(who,cap,action,cancelled);if(status!=error::success)return status;
            auto const& spec=admitted.specification();auto const& alias=spec.alias;
            if(action==op::read&&spec.kind==purpose::cache&&spec.binding=="volatile-output")
            {
                byte_buffer data;
                status=output_store.copy_read(alias,word(request,0),word(request,1),reservation,data);
                if(status!=error::success)return status;
                status=callback_checkpoint(who,cap,action,cancelled);if(status!=error::success)return status;
                required=data.view().size();if(!deliver(data.view()))return error::invalid;
                return callback_checkpoint(who,cap,action,cancelled);
            }
            if(!artifacts::purpose_code(spec.kind)||spec.binding!="sealed-input")return error::denied;
            std::array<std::uint8_t,64> output{};
            switch(action)
            {
            case op::open:
            {
                artifacts::digest expected{};std::copy_n(request.data()+8,32,expected.data());handle id{};
                status=inputs.open(alias,word(request,0),expected,spec.max_resources,id);
                if(status!=error::success)return status;
                struct rollback{artifacts::registry& owner;::uwvm2::utils::container::std_string const& alias;handle id;
                    ~rollback(){if(id)owner.abandon(alias,id);}} pending{inputs,alias,id};
                put(output,0,id);required=8;
                status=callback_checkpoint(who,cap,action,cancelled);if(status!=error::success)return status;
                if(!deliver({output.data(),8}))return error::invalid;
                status=callback_checkpoint(who,cap,action,cancelled);if(status!=error::success)return status;
                pending.id=0;return error::success;
            }
            case op::read:
            {
                artifacts::information metadata;
                status=inputs.describe(alias,word(request,0),metadata);if(status!=error::success)return status;
                if(reservation>metadata.transfer||word(request,1)>metadata.size)return error::too_large;
                ::uwvm2::utils::container::std_vector<std::uint8_t> data(static_cast<std::size_t>(reservation));std::size_t count{};
                status=inputs.read(alias,word(request,0),word(request,1),data,count);if(status!=error::success)return status;
                status=callback_checkpoint(who,cap,action,cancelled);if(status!=error::success)return status;
                required=count;if(!deliver({data.data(),count}))return error::invalid;
                return callback_checkpoint(who,cap,action,cancelled);
            }
            case op::describe:
            {
                artifacts::information metadata;status=inputs.describe(alias,word(request,0),metadata);
                if(status!=error::success)return status;
                put(output,0,metadata.generation);put(output,1,metadata.size);put(output,2,metadata.purpose_code);put(output,3,metadata.transfer);
                std::copy(metadata.sha256.begin(),metadata.sha256.end(),output.begin()+32);required=64;
                status=callback_checkpoint(who,cap,action,cancelled);if(status!=error::success)return status;
                if(!deliver(output))return error::invalid;
                return callback_checkpoint(who,cap,action,cancelled);
            }
            }
            return error::unsupported;
        }

        /** @brief Check exact output authority and transfer size before request allocation.
         * @details This trusted host preflight does not charge or acknowledge.
         * With complete receipt capacity, it also checks current attenuation
         * before allocating the request copy, without refreshing a window or
         * changing counters. A short receipt remains an uncharged size query.
         * The effect path repeats admission after copying the owned request;
         * concurrent revocation therefore still prevents the effect. No guest
         * data or callback is accessed while this temporary lease is held.
         */
        error preflight_output(caller who,output_contract::operation action,handle cap,std::size_t request_size,bool complete_receipt)
        {
            auto index=static_cast<std::size_t>(action);if(index>=output_contract::operations.size())return error::unsupported;
            lease checked;
            auto status=action==output_contract::operation::publish?
                permissions.admit_publish(who,cap,request_size,checked,false,complete_receipt):
                permissions.admit(who,cap,action,static_cast<std::uint64_t>(request_size)+output_contract::receipt_bytes,checked,false,complete_receipt);
            if(status!=error::success)return status;
            auto const& spec=checked.specification();
            if(spec.kind!=purpose::cache||spec.binding!="volatile-output")return error::denied;
            auto bound=action==output_contract::operation::publish?output_store.publication_size(spec.alias,request_size):
                output_store.request_size(spec.alias,request_size);
            if(bound!=error::success)return bound;
            return error::success;
        }
        /** @brief Reserve complete-publication scratch before copying guest input.
         * @details The caller retains the broker before this move-only token
         * and destroys the token first. Reservation confers no authority and
         * changes no object or receipt. Final admission repeats every check.
         */
        error reserve_output_publication(caller who,handle cap,std::size_t size,outputs::registry::publication_scratch& output)
        {
            lease allowed;auto status=permissions.admit_publish(who,cap,size,allowed,false,true);
            if(status!=error::success)return status;
            auto const& spec=allowed.specification();if(spec.kind!=purpose::cache||spec.binding!="volatile-output")return error::denied;
            return output_store.reserve_publication(spec.alias,size,output);
        }

        /** @brief Execute a bounded cache transaction and deliver its recovery receipt.
         * @details Short output performs authorization without admission charge
         * or mutation. An admitted mutation keeps its last exact request and
         * receipt even when delivery fails. Success of this carrier means only
         * receipt delivery; effect_status inside the receipt describes the
         * actual operation. Every retry and query still needs exact live action
         * authority and pays normal optional attenuation. No disk I/O occurs.
         * The complete-result path validates the live binding and transfer
         * bound inside the final registry operation, avoiding a redundant
         * PID check, lock and lookup. Short-result sizing still checks the
         * binding before reporting a required extent. The guest carrier's
         * separate allocation preflight remains mandatory; cancellation can
         * win over an invalid extent on a direct complete-result broker call.
         */
        error invoke_output(caller who,output_contract::operation action,handle cap,bytes request,
                            std::size_t capacity,std::size_t& required,sink const& deliver,
                            ::uwvm2::utils::container::function_ref<bool()> const& cancelled={},
                            outputs::registry::publication_scratch const* reserved=nullptr)
        {
            required=0;auto index=static_cast<std::size_t>(action);
            if(index>=output_contract::operations.size())return error::unsupported;
            auto const& description=output_contract::operations[index];
            if(capacity>transfer_limit||request.size()>transfer_limit+24u||
               (description.words<0?(request.size()<static_cast<std::size_t>(-description.words)*8u):(request.size()!=static_cast<std::size_t>(description.words)*8u)))return error::invalid;
            lease admitted;auto status=permissions.admit(who,cap,action,
                capacity<72?0:request.size()+72,admitted,capacity>=72);
            if(status!=error::success)return status;
            auto const& spec=admitted.specification();
            if(spec.kind!=purpose::cache||spec.binding!="volatile-output")return error::denied;
            if(capacity<72)
            {
                auto bound=action==output_contract::operation::publish?output_store.publication_size(spec.alias,request.size()):
                    output_store.request_size(spec.alias,request.size());
                if(bound!=error::success)return bound;
                required=72;return error::too_large;
            }
            status=callback_checkpoint(who,cap,action,cancelled);if(status!=error::success)return status;
            outputs::receipt receipt;
            if(action==output_contract::operation::publish)
            {
                outputs::registry::publication_scratch local;
                if(!reserved)
                {
                    status=output_store.reserve_publication(spec.alias,request.size(),local);if(status!=error::success)return status;
                    reserved=&local;
                }
                auto final_check=[&]() -> error
                {
                    if(cancelled&&cancelled())return error::cancelled;
                    // The outer admitted lease remains held. This recursive
                    // authority-only recheck cannot block behind another owner.
                    lease current;return permissions.admit_publish(who,cap,request.size(),current,false,false);
                };
                status=output_store.publish(spec.alias,request,spec.max_resources,receipt,*reserved,std::ref(final_check));
            }
            else if(action==output_contract::operation::status)status=output_store.status(spec.alias,word(request,0),receipt);
            else if(action==output_contract::operation::receipt)status=output_store.receipt_for(spec.alias,word(request,0),receipt);
            else status=output_store.mutate(spec.alias,action,request,spec.max_resources,receipt);
            if(status!=error::success)return status;
            std::array<std::uint8_t,72> output{};
            std::array values{receipt.key,receipt.operation,receipt.object,receipt.effect_status,receipt.accepted_bytes,
                receipt.total_bytes,receipt.retained_bytes,receipt.state,receipt.flags};
            for(std::size_t n{};n<values.size();++n)put(output,n,values[n]);
            required=output.size();status=callback_checkpoint(who,cap,action,cancelled);if(status!=error::success)return status;
            if(!deliver(output))return error::invalid;
            return callback_checkpoint(who,cap,action,cancelled);
        }

        /** @brief Dispatch one exact locally managed lifecycle action.
         * @details No native node or physical actuator is invoked. Output span
         * capacity and authority are checked before state effects. Open rolls
         * back failed handle delivery; submit/respond keep exact retry receipts.
         * Callback delivery is detached from provider locking and consumes only
         * after successful copying. Each call keeps its exact authority lease.
         */
        error invoke_managed(caller who,managed_lifecycle_contract::operation action,handle cap,bytes request,
                             std::size_t capacity,std::size_t& required,sink const& deliver,
                             ::uwvm2::utils::container::function_ref<bool()> const& cancelled={})
        {
            using op=managed_lifecycle_contract::operation;required=0;
            auto index=static_cast<std::size_t>(action);if(index>=managed_lifecycle_contract::operations.size())return error::unsupported;
            auto const& description=managed_lifecycle_contract::operations[index];
            if(request.size()!=description.words*8u||capacity>transfer_limit)return error::invalid;
            lease admitted;auto status=permissions.admit(who,cap,description.name,
                capacity<description.bytes?0:request.size()+description.bytes,admitted,capacity>=description.bytes);
            if(status!=error::success)return status;
            auto const& spec=admitted.specification();
            if(spec.kind!=purpose::lifecycle||spec.binding!="local-lifecycle")return error::denied;
            if(capacity<description.bytes){required=description.bytes;return error::too_large;}
            status=callback_checkpoint(who,cap,description.name,cancelled);if(status!=error::success)return status;
            std::array<std::uint8_t,56> output{};
            auto emit=[&](std::initializer_list<std::uint64_t> words)
            {std::size_t at{};for(auto value:words)put(output,at++,value);required=at*8;
                return callback_checkpoint(who,cap,description.name,cancelled)==error::success&&deliver({output.data(),required})&&
                    callback_checkpoint(who,cap,description.name,cancelled)==error::success;};
            auto const& alias=spec.alias;managed_lifecycle::receipt receipt;
            switch(action)
            {
            case op::open:
            {
                handle id{};status=managed_nodes.open(alias,word(request,0),spec.max_resources,id);if(status!=error::success)return status;
                struct rollback{managed_lifecycle::registry& nodes;::uwvm2::utils::container::std_string const& alias;handle id;~rollback(){if(id)nodes.abandon(alias,id);}} pending{managed_nodes,alias,id};
                if(!emit({id}))return error::invalid;
                pending.id=0;return error::success;
            }
            case op::state:
            {
                managed_lifecycle::snapshot value;status=managed_nodes.state(alias,word(request,0),value);if(status!=error::success)return status;
                return emit({value.state,value.generation,value.request,value.transition,value.deadline,value.status,value.delivered})?error::success:error::invalid;
            }
            case op::transitions:
            {std::uint64_t mask{};status=managed_nodes.transitions(alias,word(request,0),mask);if(status!=error::success)return status;return emit({mask})?error::success:error::invalid;}
            case op::submit:status=managed_nodes.submit(alias,word(request,0),word(request,1),word(request,2),word(request,3),word(request,4),receipt);break;
            case op::take:return managed_nodes.take(alias,word(request,0),[&](managed_lifecycle::callback const& value)
                {return emit({value.request,value.transition,value.generation,value.phase,value.deadline});});
            case op::respond:status=managed_nodes.respond(alias,word(request,0),word(request,1),word(request,2),word(request,3),receipt);break;
            }
            if(status!=error::success)return status;
            return emit({receipt.key,receipt.request,receipt.generation,receipt.state,receipt.status})?error::success:error::invalid;
        }

        /** @brief Deliver one runtime request under exact action authority.
         * @details Input is an exact word record from runtime/contract.h.
         * Fixed outputs reserve their exact size, wait output reserves the
         * caller's bounded capacity before blocking. Byte attenuation measures
         * admitted request plus output reservation, not bytes actually copied.
         * Insufficient capacity reports the required size without side effects.
         * Wait releases admission synchronization, observes cancellation, then
         * revalidates before delivery. It never consumes source events.
         */
        error invoke(caller who,runtime_contract::operation action,handle cap,bytes request,
                     std::size_t capacity,std::size_t& required,sink const& deliver,
                     ::uwvm2::utils::container::function_ref<bool()> const& cancelled={})
        {
            namespace contract=runtime_contract;using op=contract::operation;
            auto index=static_cast<std::size_t>(action);if(index>=contract::operations.size())return error::unsupported;
            auto const& description=contract::operations[index];required=0;
            if(request.size()!=description.words*8u||capacity>transfer_limit)return error::invalid;
            auto reservation=description.bytes<0?capacity:static_cast<std::size_t>(description.bytes);
            if(description.bytes>=0&&capacity<reservation)
            {
                lease check;auto result=permissions.admit(who,cap,action,0,check,false);
                if(result!=error::success)return result;
                required=reservation;return error::too_large;
            }
            // Dispatchers owning their own close accounting must receive the
            // operation before runtime admission charges it. Validate the full
            // zero-result sink and reentrant cancellation first, then route once.
            if(action==op::resource_close)
            {
                lease route;auto checked=permissions.admit(who,cap,action,0,route,false);if(checked!=error::success)return checked;
                auto const& policy=route.specification();
                bool owned=communication_policy(policy)||(policy.kind==purpose::sensor&&policy.binding=="bounded-sensor")||
                    (policy.kind==purpose::device&&policy.binding=="framed-scalar")||
                    (policy.kind==purpose::control&&policy.binding=="framed-control")||
                    (policy.kind==purpose::media&&policy.binding=="immutable-buffer-media")||
                    (policy.kind==purpose::bridge&&policy.binding=="topic-forwarder")||
                    (policy.kind==purpose::parameter&&policy.binding=="local-parameters")||
                    (policy.kind==purpose::proposal&&policy.binding=="model-proposals")||
                    (policy.kind==purpose::recording&&policy.binding=="private-recording")||
                    (policy.kind==purpose::replay&&policy.binding=="private-replay");
                if(owned)
                {
                    checked=callback_checkpoint(who,cap,action,cancelled);if(checked!=error::success)return checked;
                    if(!deliver({}))return error::invalid;
                    checked=callback_checkpoint(who,cap,action,cancelled);if(checked!=error::success)return checked;
                    auto resource=word(request,0);
                    if(communication_policy(policy))return communication_store?communication_store->close(who,cap,resource):error::denied;
                    if(policy.kind==purpose::parameter)return parameter_store?parameter_store->close(who,cap,resource):error::denied;
                    if(policy.kind==purpose::sensor)return policy.binding=="bounded-sensor"&&sensor_store?sensor_store->close(who,cap,resource):error::denied;
                    if(policy.kind==purpose::media)return media_store?media_store->close(who,cap,resource):error::denied;
                    if(policy.kind==purpose::device||policy.kind==purpose::control)return device_store?device_store->close(who,cap,resource):error::denied;
                    if(policy.kind==purpose::bridge)return bridge_store?bridge_store->close(who,cap,resource):error::denied;
                    if(policy.kind==purpose::proposal)return policy.binding=="model-proposals"&&proposal_store?proposal_store->close(who,cap,resource):error::denied;
                    if(policy.kind==purpose::recording)return policy.binding=="private-recording"&&recording_store?recording_store->close(who,cap,resource):error::denied;
                    return policy.binding=="private-replay"&&recording_store?recording_store->close(who,cap,resource):error::denied;
                }
            }
            // Closing an existing resource is a bounded cleanup operation,
            // including local guards, timers, buffers and artifact readers.
            // Zero traffic allowance must not strand their reservations. Only
            // this exact operation skips charging; admission still checks live
            // membership, exposure and grant, and each store checks ownership.
            bool cleanup=action==op::resource_close;
            lease admitted;auto result=permissions.admit(who,cap,action,cleanup?0:request.size()+reservation,admitted,!cleanup);
            if(result!=error::success)return result;
            auto const& spec=admitted.specification();auto const& alias=spec.alias;
            std::array<std::uint64_t,5> arguments{};for(std::size_t n{};n<description.words;++n)arguments[n]=word(request,n);
            std::array<std::uint8_t,32> output{};
            auto emit=[&](std::initializer_list<std::uint64_t> values)
            {
                std::size_t at{};for(auto value:values)put(output,at++,value);
                required=values.size()*8;
                return callback_checkpoint(who,cap,action)==error::success&&deliver({output.data(),required})&&
                    callback_checkpoint(who,cap,action)==error::success;
            };
            auto created=[&](error status,handle id)
            {
                if(status!=error::success)return status;
                struct rollback
                {
                    resources::registry& registry;::uwvm2::utils::container::std_string const& owner;handle id;
                    ~rollback(){if(id)(void)registry.close(owner,id);}
                } pending{runtime,alias,id};
                if(!emit({id}))return error::invalid;
                if(action==op::timer_create)
                {lease source;if(permissions.dependency(who,arguments[0],"clock.read",source)!=error::success)return error::denied;}
                pending.id=0;return error::success;
            };
            // Zero-length delivery validates the result slot before a mutating
            // operation that has no result record. It grants no operation.
            if(description.bytes==0)
            {
                if(!deliver({}))return error::invalid;
                result=callback_checkpoint(who,cap,action);if(result!=error::success)return result;
            }
            handle id{};lease dependency;
            switch(action)
            {
            case op::guard_create:
                result=runtime.guard_create(alias,spec.max_resources,id);return created(result,id);
            case op::guard_trigger:return runtime.guard_trigger(alias,arguments[0]);
            case op::guard_take:return runtime.guard_deliver(alias,arguments[0],[&](auto count){return emit({count});});
            case op::clock_create:
            {
                auto type=spec.kind==purpose::steady_clock?resources::clock_kind::steady:
                          spec.kind==purpose::system_clock?resources::clock_kind::system:resources::clock_kind::ros;
                if(arguments[0]!=static_cast<unsigned>(type))return error::denied;
                result=runtime.clock_create(alias,spec.max_resources,type,id);return created(result,id);
            }
            case op::clock_read:
            {
                resources::clock_sample value;result=runtime.clock_read(alias,arguments[0],value);
                if(result!=error::success)return result;
                return emit({static_cast<std::uint64_t>(value.now),value.epoch,value.started,static_cast<unsigned>(value.source)})?error::success:error::invalid;
            }
            case op::clock_update:
            {
                // Update authority is on the same exact clock resource alias;
                // the extra source capability must independently permit read.
                result=permissions.dependency(who,arguments[0],"clock.read",dependency);if(result!=error::success)return result;
                auto const& source=dependency.specification();
                if(source.kind!=purpose::ros_clock||spec.kind!=purpose::ros_clock||source.resource!=spec.resource||source.binding!=spec.binding||arguments[3]>1)return error::denied;
                return runtime.clock_update(source.alias,arguments[1],std::bit_cast<std::int64_t>(arguments[2]),arguments[3]!=0);
            }
            case op::timer_create:
                result=permissions.dependency(who,arguments[0],"clock.read",dependency);if(result!=error::success)return result;
                result=runtime.timer_create(alias,spec.max_resources,dependency.specification().alias,arguments[1],id);return created(result,id);
            case op::timer_reset:return runtime.timer_reset(alias,arguments[0],std::bit_cast<std::int64_t>(arguments[1]),std::bit_cast<std::int64_t>(arguments[2]));
            case op::timer_cancel:return runtime.timer_cancel(alias,arguments[0]);
            case op::timer_take:return runtime.timer_deliver(alias,arguments[0],[&](auto const& value)
                {return emit({static_cast<std::uint64_t>(value.expected),static_cast<std::uint64_t>(value.actual),value.occurrences,value.clock_epoch});});
            case op::wait_set_create:
                result=runtime.wait_set_create(alias,spec.max_resources,id);return created(result,id);
            case op::wait_set_add:
                result=permissions.dependency(who,arguments[1],"resource.observe",dependency);if(result!=error::success)return result;
                return runtime.wait_set_add(alias,arguments[0],dependency.specification().alias,arguments[2],arguments[3],arguments[4],spec.max_wait_members);
            case op::wait_set_remove:return runtime.wait_set_remove(alias,arguments[0],arguments[1]);
            case op::wait_set_acknowledge:return runtime.wait_set_acknowledge(alias,arguments[0],arguments[1],arguments[2]);
            case op::resource_close:
                if(spec.kind==purpose::transform&&spec.binding=="local-transform")return transform_store.close(alias,arguments[0]);
                if(spec.binding=="local-model"&&(spec.kind==purpose::language_model||spec.kind==purpose::vision_model||spec.kind==purpose::world_model))
                    return model_store.close(spec.alias,word(request,0));
                if(spec.kind==purpose::buffer)return buffer_store.close(alias,arguments[0]);
                if(spec.kind==purpose::lifecycle)return managed_nodes.close(alias,arguments[0]);
                return artifacts::purpose_code(spec.kind)?inputs.close(alias,arguments[0]):runtime.close(alias,arguments[0]);
            case op::wait_set_wait:
            {
                auto duration=arguments[1];if(duration>spec.max_wait_ns)return error::denied;
                // A zero-duration poll cannot block. Keep its original lease
                // through the only readiness scan and delivery; retirement is
                // excluded, so releasing and repeating both is unnecessary.
                if(duration==0)
                {
                    result=callback_checkpoint(who,cap,action,cancelled);if(result!=error::success)return result;
                    ::uwvm2::utils::container::std_vector<resources::ready_record> records;std::size_t count{};
                    result=runtime.wait(alias,arguments[0],std::chrono::steady_clock::time_point::min(),capacity/32,records,count);
                    required=count*32;if(result!=error::success)return result;
                    ::uwvm2::utils::container::std_vector<std::uint8_t> encoded(records.size()*32);
                    for(std::size_t n{};n<records.size();++n)
                    {auto const& value=records[n];put(encoded,n*4,value.source);put(encoded,n*4+1,value.token);put(encoded,n*4+2,value.events);put(encoded,n*4+3,value.sequence);}
                    result=callback_checkpoint(who,cap,action,cancelled);if(result!=error::success)return result;
                    if(!deliver(encoded))return error::invalid;
                    return callback_checkpoint(who,cap,action,cancelled);
                }
                auto until=std::chrono::steady_clock::time_point::max();
                if(duration==UINT64_MAX){if(!spec.infinite_wait)return error::denied;}
                else
                {
                    auto now=std::chrono::steady_clock::now();
                    auto available=std::chrono::duration_cast<std::chrono::nanoseconds>(until-now).count();
                    if(available<0||duration>static_cast<std::uint64_t>(available))return error::invalid;
                    until=now+std::chrono::nanoseconds(static_cast<std::int64_t>(duration));
                }
                admitted.release();::uwvm2::utils::container::std_vector<resources::ready_record> records;std::size_t count{};
                auto stop=[&]
                {
                    if(cancelled&&cancelled())return true;
                    lease check;return permissions.admit(who,cap,action,0,check,false)!=error::success;
                };
                result=runtime.wait(alias,arguments[0],until,capacity/32,records,count,stop);
                required=count*32;
                if(result!=error::success)return result;
                result=permissions.admit(who,cap,action,0,admitted,false);if(result!=error::success)return result;
                result=callback_checkpoint(who,cap,action,cancelled);if(result!=error::success)return result;
                // Revocation/detach may race the first snapshot. The renewed
                // authority lease excludes those changes during this final
                // nonblocking readiness observation and guest delivery.
                result=runtime.wait(alias,arguments[0],std::chrono::steady_clock::now(),capacity/32,records,count);
                required=count*32;
                if(result==error::timeout)return error::again;
                if(result!=error::success)return result;
                ::uwvm2::utils::container::std_vector<std::uint8_t> encoded(records.size()*32);
                for(std::size_t n{};n<records.size();++n)
                {auto const& value=records[n];put(encoded,n*4,value.source);put(encoded,n*4+1,value.token);put(encoded,n*4+2,value.events);put(encoded,n*4+3,value.sequence);}
                result=callback_checkpoint(who,cap,action,cancelled);if(result!=error::success)return result;
                if(!deliver(encoded))return error::invalid;
                return callback_checkpoint(who,cap,action,cancelled);
            }
            }
            return error::unsupported;
        }
    private:
        /** @brief Notify all owned providers under the authority fence, without joining workers. */
        void notify_revocation(std::string_view grant)
        {module_calls.revoke(grant);runtime.revoke(grant);inputs.revoke(grant);output_store.revoke(grant);managed_nodes.revoke(grant);buffer_store.revoke(grant);model_store.revoke(grant);transform_store.revoke(grant);
             if(log_diagnostic_store)log_diagnostic_store->revoke(grant);
             if(sensor_store)sensor_store->revoke(grant);
             if(media_store)media_store->revoke(grant);
             if(bridge_store)bridge_store->revoke(grant);
             if(device_store)device_store->revoke(grant);
             if(driver_store)driver_store->revoke(grant);
             if(proposal_target_store)proposal_target_store->revoke(grant);
             if(proposal_store)proposal_store->revoke(grant);
             if(recording_store)recording_store->revoke(grant);
             if(parameter_store)parameter_store->revoke(grant);
             if(communication_store)communication_store->revoke(grant);
             auto worker=model_workers.find(grant);if(worker!=model_workers.end())worker->second->request_stop();}
    public:
        /** @brief Guest self-revocation: fixed storage, exact recovery and rate-independent fencing.
         * @details Writable capacity and cancellation are checked before the
         * irreversible fence. Lost output never rolls back authority reduction.
         * The retained receipt distinguishes local notifications from remote
         * cleanup or physical safety; no successful reply asserts either one.
         */
        error invoke_revocation(caller who,handle cap,bytes input,std::size_t capacity,std::size_t& required,sink deliver,
                                ::uwvm2::utils::container::function_ref<bool()> cancelled={})
        {
            required=0;auto status=permissions.preflight_revoke(who,cap);if(status!=error::success)return status;
            if(input.size()!=16||capacity>transfer_limit||!deliver)return error::invalid;
            if(!word(input,0)||!word(input,1))return error::invalid;
            if(word(input,1)!=cap)return error::denied;
            required=32;if(capacity<required)return error::too_large;
            if(cancelled&&cancelled())return error::cancelled;
            revocation_contract::receipt receipt;
            status=permissions.revoke_granted(who,cap,word(input,0),word(input,1),receipt,
                [&](auto alias){notify_revocation(alias);});
            if(status!=error::success)return status;
            std::array<std::uint8_t,32> output;
            put(output,0,receipt.key);put(output,1,receipt.target);put(output,2,receipt.identity);put(output,3,static_cast<std::uint64_t>(receipt.state));
            try{if(!deliver(output))return error::io;}catch(...){return error::io;}
            if(cancelled&&cancelled())return error::cancelled;
            return permissions.preflight_revoke(who,cap);
        }
        /** @brief Trusted revocation wakes waiters and retires owned resources. */
        error revoke(std::string_view alias)
        {return permissions.revoke(alias,[&](auto grant){notify_revocation(grant);});}
        /** @brief Trusted environment retirement blocks all future admission. */
        void retire(){permissions.retire([&]{module_calls.retire();runtime.retire();inputs.retire();output_store.retire();managed_nodes.retire();buffer_store.retire();model_store.retire();transform_store.retire();
            if(log_diagnostic_store)log_diagnostic_store->retire();
            if(proposal_store)proposal_store->retire();
            if(proposal_target_store)proposal_target_store->retire();
            if(sensor_store)sensor_store->retire();
            if(media_store)media_store->retire();
            if(bridge_store)bridge_store->request_stop();
            if(device_worker)device_worker->request_stop();
            if(driver_store)driver_store->retire();
            if(device_store)device_store->retire();
            if(recording_store)recording_store->retire();
            if(parameter_store)parameter_store->retire();
            if(communication_store)communication_store->retire();
            for(auto const& [alias,worker]:model_workers){(void)alias;worker->request_stop();}
            for(auto const& [name,worker]:communication_workers){(void)name;worker->request_stop();}});}
    };
}
