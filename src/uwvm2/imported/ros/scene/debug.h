/** @file
 * @brief Bounded detached scene metadata for dbg, DAP and editor adapters.
 * @details This DTO has no provider, filesystem, model, socket or guest-memory
 * dependency. Inspection observes stored authority/resource metadata only.
 * The registered trusted hook must use try-locks, cap traversal and honor the
 * independent debugger inspection permission. No snapshot method may admit a
 * capability, run inference, poll a provider, consume readiness or read bytes.
 */
#pragma once
#include "component_fault.h"
#include "bound_queries.h"
#include "call_audit_statistics.h"
#include <array>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <string_view>
#include "platform_module.h"

/** @brief Keep detached debugger DTOs and hooks shared across module and header consumers.
 * @details Explicit C++ language linkage gives these declarations global-module
 * attachment when exported by the scene model. Runtime glue and debugger
 * frontends therefore inspect the same hook, without creating a second owner.
 */
extern "C++" {
namespace uwvm2::imported::ros::scene::debug
{
    inline constexpr std::size_t maximum_rows=8,maximum_scan=4096;
    enum class selection : std::uint8_t { authority, resources, audit, policy };
    enum class status : std::uint8_t { ok,invalid_request,not_selected,busy,forbidden,requires_cooperative_stop,stale_stop,resource_limit };
    enum class row_kind : std::uint8_t { grant,resource };
    /** @brief Fixed-capacity copied label with explicit truncation metadata. */
    template<std::size_t Capacity> struct text
    {
        std::array<char,Capacity> bytes{};std::size_t size{};bool truncated{};
        void assign(std::string_view value) noexcept
        {size=value.size()<Capacity?value.size():Capacity;truncated=value.size()>Capacity;for(std::size_t i=0;i<size;++i)bytes[i]=value[i];}
        std::string_view view() const noexcept{return {bytes.data(),size};}
    };
    /** @brief Read-only page request fenced to a current cooperative stop.
     * @details Environment and alias are exact optional logical metadata
     * filters. No request field is a resource operation or a native descriptor.
     */
    struct request
    {
        selection selected{selection::authority};std::uint64_t first{},count{maximum_rows},stop{};
        text<255> environment,alias;
        text<64> operation;
    };
    /** @brief One detached grant or runtime resource record.
     * @details IDs are diagnostic identities, not delegated capabilities.
     * Only configured logical IDs and counters are present; content, host
     * paths, URLs, credentials and native pointers are excluded by construction.
     * Immutable artifact rows copy the registered byte length into size_bytes
     * and set immutable. Inspection never reads the artifact or queries its
     * provider. Volatile output/receipt rows carry a closed transaction state,
     * accepted bytes, retained reservation and last-receipt count. They never
     * expose a request key, digest or byte payload. Publication describes only
     * session visibility; durable must remain false for this provider. Receipt
     * inspection neither acknowledges a result nor refunds its retained slot.
     * Lifecycle rows describe one configured alias, not an exported handle:
     * id is zero, open_handles counts its retained handles, and the closed
     * logical state/generation/pending-callback fields are cached. The absolute
     * steady deadline is zero exactly when no callback is pending; inspection
     * must not sample the clock or expire it. No request ID or retry key is
     * exported. Logical finalization never asserts a physical device stop.
     * Buffer rows aggregate one sealed-buffer binding: id is zero, members is
     * retained objects, reserved_bytes is retained payload, open_handles counts
     * live guest handles, lease_count is retained read-only pins, and
     * workspace_bytes is detached read-copy capacity. No individual buffer,
     * schema, digest, payload or native address is exposed. Inspection must
     * use try_inspect; contention withholds the entire page without effects.
     * Model rows aggregate one local-model binding: id is zero, open_handles
     * counts retained sessions, members counts retained requests, input_bytes
     * counts pinned inputs, output_bytes/output_items count reserved output
     * capacity, and workspace_bytes counts carrier/job scratch reservations.
     * No prompt, prediction, profile digest, retry key or worker PID is exposed.
     * Inspection does not claim a job, expire a deadline or sample inference.
     * Transform rows aggregate one local-transform binding: id is zero,
     * open_handles counts live sessions, members counts all retained sessions,
     * reserved_bytes/workspace_bytes/query_bytes are retained session capacity,
     * and carrier_bytes counts currently retained copied-carrier capacity.
     * sample_count/query_count/pending_queries are cached counts of published
     * sessions. Closed-but-retained sessions can keep capacity charged without
     * retaining a guest handle. Frames, transforms, query IDs, profile digests
     * and timestamps are absent. Inspection must not sample a clock, advance
     * an epoch, expire a query, interpolate a transform or consume a result.
     * Log rows contain retained record counts/bytes, accepted/exported/lost
     * counters and copy/full-queue failures only. Diagnostic rows contain
     * retained row/storage counts and pending/busy flags only. Neither includes
     * messages, logger names, scope/cursor identities, expiry times or counter
     * contents. Both use nonblocking cached inspection; no capture or drain runs.
     * Recording rows contain retained writer/object handles, sealed object
     * counts/bytes, active message counts, replay counts and pins. Replay rows
     * contain only their own retained cursor handles and reservation. Neither
     * exposes object IDs, digests, retry keys, timestamps or cursor positions;
     * active/idle/retired describe cached local state, never durable storage.
     * Sensor rows contain retained/live handles, queued records and payload,
     * source reservation, captures, loss/refusal counts, native ingress byte
     * and frame counts and cached status. No descriptor, frame, schema, sample
     * identity or timestamp is included. Inspection cannot read an ingress fd.
     * Model proposal rows contain retained proposals, reader leases, capture
     * and tool counts and workspace. No prompt, proposed arguments, source
     * identities, approval token or target state is exposed or accepted here.
     * Communication rows describe one alias in its worker-local registry. id
     * is zero, members is one configured binding, open_handles counts published
     * resources, and retained_receipts counts cached orphaned resources only.
     * configured/ready/retired describe the registry, not native transport or
     * physical safety. No provider poll, process query, message, endpoint name,
     * request key, native identifier or worker image path is exposed. This row
     * is not a complete count of asynchronous creation-ledger cleanup debt.
     * Parameter rows aggregate one local-parameters store. members counts all
     * retained scopes, open_handles counts live scopes, sample_count counts
     * parameters, query_count counts cursors, retained_bytes counts values,
     * workspace_bytes counts active scratch, input_bytes counts queued event
     * bytes, and output_items counts queued events. No key, value, descriptor,
     * node, cursor, request identity or deadline is copied. Cached inspection
     * does not expire validation, advance a cursor or access a native service.
     * Media rows aggregate immutable-buffer-media streams: members counts
     * reserved streams, open_handles counts published streams, sample_count
     * counts pending output records, reserved_bytes is fixed registry capacity,
     * workspace_bytes counts active scratch, output_items counts completed
     * transforms and exported_records counts successfully consumed deliveries.
     * No buffer identity, content, profile hash, key or timestamp is disclosed.
     * Inspection neither executes a codec nor consumes a queued output.
     * Other rows leave buffer/transaction fields empty/zero/false.
     */
    struct row
    {
        row_kind kind{};text<255> environment,alias,resource,binding,platform;
        /** @brief Cached startup child switch; this is never effective device authority. */
        bool platform_enabled{};
        /// @brief Frozen module.call input-carrier policy; inapplicable rows use false.
        bool clear_after_copy{};
        text<32> type;
        text<16> state;
        std::uint64_t id{},open_handles{},admissions{},refused{},sequence{},members{},size_bytes{},accepted_bytes{},reserved_bytes{},retained_receipts{};
        std::uint64_t generation{},transition{},deadline_ns{},status_code{},lease_count{},workspace_bytes{};
        std::uint64_t input_bytes{},output_bytes{},output_items{};
        std::uint64_t sample_count{},query_count{},pending_queries{},query_bytes{},carrier_bytes{};
        std::uint64_t emitted_records{},exported_records{},lost_records{},retained_bytes{},copy_failures{};
        bool revoked{},attenuated{},retired{},armed{},immutable{},published{},durable{};
        bool callback_pending{},callback_delivered{};
        bool counters_saturated{},pending_page{},busy{},exit_observed{};
    };
    /** @brief Validate detached metadata without reading a provider or receipt.
     * @details Inputs and volatile outputs have distinct closed schemas.
     * Receipt-only rows reserve no payload bytes and retain exactly one result;
     * zero identity is allowed only for an admitted failure without an owned object.
     * Session publication never claims crash durability. The same checks are
     * independently applied by DAP before exposing a complete page to an IDE.
     */
    inline bool valid(row const& value) noexcept
    {
        auto bounded=[](auto const& label){return label.size<=label.bytes.size();};
        if(value.kind>row_kind::resource||!bounded(value.environment)||!bounded(value.alias)||
           !bounded(value.resource)||!bounded(value.binding)||!bounded(value.type)||!bounded(value.state)||!bounded(value.platform))return false;
        if(value.platform.size)
        {
            if(value.platform.truncated||!platform_extensions::module_name(value.platform.view())||
               (value.type.view()!="device"&&value.type.view()!="control"&&value.type.view()!="driver"))return false;
        }
        else if(value.platform_enabled)return false;
        // Registered messages expose only cached capacity and erasure-debt
        // counts. No clock sampling, expiry, token, endpoint or payload access.
        if(value.kind==row_kind::resource&&value.type.view()=="module.call")
        {
            auto state=value.state.view();
            return value.binding.view()=="registered-message"&&value.open_handles<=value.members&&
                value.sample_count<=value.open_handles&&value.query_count<=value.open_handles-value.sample_count&&
                value.retired==(state=="retired")&&(state=="ready"||state=="active"||state=="quarantined"||state=="retired")&&
                (value.retired||(state=="quarantined"?value.query_count!=0:!value.query_count))&&
                !value.id&&!value.admissions&&!value.refused&&!value.sequence&&!value.size_bytes&&!value.accepted_bytes&&!value.retained_receipts&&
                !value.generation&&!value.transition&&!value.deadline_ns&&!value.status_code&&!value.lease_count&&!value.workspace_bytes&&
                !value.input_bytes&&!value.output_bytes&&!value.output_items&&!value.pending_queries&&!value.query_bytes&&!value.carrier_bytes&&
                !value.emitted_records&&!value.exported_records&&!value.lost_records&&!value.retained_bytes&&!value.copy_failures&&
                !value.revoked&&!value.attenuated&&!value.armed&&!value.immutable&&!value.published&&!value.durable&&
                !value.callback_pending&&!value.callback_delivered&&!value.counters_saturated&&!value.pending_page&&!value.busy&&!value.exit_observed;
        }
        if(value.kind==row_kind::resource&&value.type.view()=="driver")
        {
            auto state=value.state.view();
            return value.binding.view()=="registered-driver"&&value.status_code<=UINT32_MAX&&value.query_count<=1&&
                (state=="prepared"||state=="handshake"||state=="ready"||state=="pending"||state=="result"||state=="closed"||state=="failed")&&
                (!value.query_count||state=="closed"||state=="failed")&&
                !value.id&&!value.members&&!value.open_handles&&!value.sample_count&&!value.generation&&!value.transition&&!value.deadline_ns&&
                !value.armed&&!value.durable&&!value.published&&!value.callback_pending&&!value.callback_delivered&&!value.exit_observed&&
                !value.admissions&&!value.refused&&!value.size_bytes&&!value.accepted_bytes&&!value.reserved_bytes&&!value.retained_receipts&&
                !value.lease_count&&!value.workspace_bytes&&!value.input_bytes&&!value.output_bytes&&!value.output_items&&!value.pending_queries&&
                !value.query_bytes&&!value.carrier_bytes&&!value.emitted_records&&!value.exported_records&&!value.lost_records&&!value.retained_bytes&&
                !value.copy_failures&&!value.revoked&&!value.attenuated&&!value.retired&&!value.immutable&&!value.counters_saturated&&!value.pending_page&&!value.busy&&!value.clear_after_copy;
        }
        bool const input=value.kind==row_kind::resource&&value.type.view()=="artifact";
        if(value.clear_after_copy)return false; // Only the module.call branch above owns this flag.
        bool const output=value.kind==row_kind::resource&&value.type.view()=="artifact.output";
        bool const receipt=value.kind==row_kind::resource&&value.type.view()=="artifact.receipt";
        bool const lifecycle=value.kind==row_kind::resource&&value.type.view()=="lifecycle";
        bool const buffer=value.kind==row_kind::resource&&value.type.view()=="buffer";
        bool const model=value.kind==row_kind::resource&&value.type.view()=="model";
        bool const transform=value.kind==row_kind::resource&&value.type.view()=="transform";
        bool const log=value.kind==row_kind::resource&&value.type.view()=="log";
        bool const diagnostic=value.kind==row_kind::resource&&value.type.view()=="diagnostic";
        bool const recording=value.kind==row_kind::resource&&value.type.view()=="recording";
        bool const replay=value.kind==row_kind::resource&&value.type.view()=="replay";
        bool const sensor=value.kind==row_kind::resource&&value.type.view()=="sensor";
        bool const proposal=value.kind==row_kind::resource&&value.type.view()=="model.proposal";
        bool const communication=value.kind==row_kind::resource&&value.type.view()=="communication";
        bool const parameter=value.kind==row_kind::resource&&value.type.view()=="parameter";
        bool const component=value.kind==row_kind::resource&&value.type.view()=="component";
        if(!component&&value.exit_observed)return false;
        bool const bridge=value.kind==row_kind::resource&&value.type.view()=="bridge";
        bool const device=value.kind==row_kind::resource&&(value.type.view()=="device"||value.type.view()=="control");
        bool const media=value.kind==row_kind::resource&&value.type.view()=="media";
        if(!log&&(value.emitted_records||(!media&&value.exported_records)||value.copy_failures))return false;
        if(!log&&!sensor&&!bridge&&(value.lost_records||value.counters_saturated))return false;
        if(!log&&!sensor&&!recording&&!parameter&&value.retained_bytes)return false;
        if((!diagnostic&&value.pending_page)||(!diagnostic&&!log&&!sensor&&!bridge&&value.busy))return false;
        if((!buffer&&!recording&&!sensor&&!proposal&&!device&&value.lease_count)||(!buffer&&!model&&!transform&&!sensor&&!proposal&&!parameter&&!media&&value.workspace_bytes))return false;
        if((!model&&!sensor&&!parameter&&(value.input_bytes||(!media&&!bridge&&!component&&value.output_items)))||(!model&&value.output_bytes))return false;
        if((!transform&&!recording&&!sensor&&!proposal&&!parameter&&!media&&!bridge&&!device&&!component&&value.sample_count)||(!transform&&!recording&&!proposal&&!parameter&&!bridge&&!device&&!component&&value.query_count)||
           (!transform&&(value.pending_queries||value.query_bytes||value.carrier_bytes)))return false;
        if(input!=value.immutable||(!input&&value.size_bytes))return false;
        if(lifecycle)
        {
            if(value.id||value.binding.view()!="local-lifecycle"||!value.generation||value.accepted_bytes||value.reserved_bytes||
               value.retained_receipts||value.published||value.durable)return false;
            auto state=value.state.view();
            bool const primary=state=="unconfigured"||state=="inactive"||state=="active"||state=="finalized";
            bool const intermediate=state=="configuring"||state=="cleaning_up"||state=="shutting_down"||
                state=="activating"||state=="deactivating"||state=="error_processing";
            if((!primary&&!intermediate)||value.callback_pending!=intermediate)return false;
            if(value.status_code&&((value.status_code!=110&&value.status_code!=125)||state!="finalized"))return false;
            if(!value.callback_pending)return !value.transition&&!value.deadline_ns&&!value.callback_delivered;
            if(!value.deadline_ns||value.deadline_ns>INT64_MAX||value.transition<1||value.transition>7||value.status_code)return false;
            return (state=="configuring"&&value.transition==1)||(state=="cleaning_up"&&value.transition==2)||
                (state=="activating"&&value.transition==3)||(state=="deactivating"&&value.transition==4)||
                (state=="shutting_down"&&value.transition>=5)||state=="error_processing";
        }
        if((!device&&value.generation)||value.transition||value.deadline_ns||(!sensor&&!component&&value.status_code)||value.callback_pending||value.callback_delivered)return false;
        if(component)
        {
            auto state=value.state.view();
            return value.binding.view()=="isolated-roles"&&!value.id&&value.members<=1&&value.open_handles<=1&&value.query_count<=value.sample_count&&
                components::valid_fault(value.status_code)&&(state!="failed"||value.status_code)&&
                (!value.status_code||state=="failed"||state=="retired"||state=="outcome_unknown")&&
                (!value.exit_observed||!value.members)&&(state!="exited"||value.exit_observed)&&value.retired==(state=="retired")&&
                (state=="prepared"||state=="starting"||state=="ready"||state=="stopping"||state=="exited"||state=="failed"||state=="retired"||state=="outcome_unknown")&&
                !value.accepted_bytes&&!value.retained_receipts&&!value.admissions&&!value.refused&&!value.sequence&&!value.revoked&&!value.attenuated&&!value.armed&&!value.published&&!value.durable;
        }
        if(device)
        {
            auto state=value.state.view();bool cycle=value.type.view()=="control";
            return value.binding.view()==(cycle?"framed-control":"framed-scalar")&&!value.id&&value.query_count<=value.sample_count&&
                (!cycle||value.open_handles<=1)&&(!value.sequence||cycle)&&(!value.generation||cycle)&&
                !value.accepted_bytes&&!value.retained_receipts&&!value.admissions&&!value.refused&&!value.revoked&&!value.attenuated&&!value.published&&!value.durable&&
                value.retired==(state=="retired")&&value.armed==(state=="ready")&&
                (state=="prepared"||state=="ready"||state=="stopping"||state=="stop_accepted"||state=="outcome_unknown"||state=="retired")&&
                (value.retired||((state=="outcome_unknown")==bool(value.query_count)));
        }
        if(bridge)
        {
            auto state=value.state.view();
            return value.binding.view()=="topic-forwarder"&&!value.id&&value.members==1&&value.open_handles<=1&&
                value.sample_count<=1&&value.query_count<=value.sample_count&&!value.accepted_bytes&&!value.retained_receipts&&
                !value.admissions&&!value.refused&&!value.sequence&&!value.revoked&&!value.attenuated&&!value.armed&&!value.published&&!value.durable&&
                value.retired==(state=="retired")&&(state=="idle"||state=="running"||state=="outcome_unknown"||state=="retired")&&
                (value.retired||((state=="outcome_unknown")==bool(value.query_count)));
        }
        if(media)
        {
            auto state=value.state.view();
            return value.binding.view()=="immutable-buffer-media"&&!value.id&&value.open_handles<=value.members&&
                value.exported_records<=value.output_items&&!value.accepted_bytes&&!value.retained_receipts&&!value.admissions&&!value.refused&&
                !value.sequence&&!value.revoked&&!value.attenuated&&!value.armed&&!value.published&&!value.durable&&
                value.retired==(state=="retired")&&(state=="idle"||state=="active"||state=="retired");
        }
        if(parameter)return value.binding.view()=="local-parameters"&&!value.id&&value.open_handles<=value.members&&
            !value.state.size&&!value.accepted_bytes&&!value.reserved_bytes&&!value.retained_receipts&&
            !value.admissions&&!value.refused&&!value.sequence&&!value.revoked&&!value.attenuated&&!value.armed&&!value.published&&!value.durable;
        if(communication)
        {
            auto state=value.state.view();
            return (value.binding.view()=="native-communication"||value.binding.view()=="native-action"||value.binding.view()=="native-graph"||value.binding.view()=="event-stream")&&
                !value.id&&value.members==1&&(value.binding.view()=="event-stream"||value.open_handles<=1)&&value.retained_receipts<=1&&
                !value.accepted_bytes&&!value.reserved_bytes&&!value.admissions&&!value.refused&&!value.sequence&&!value.revoked&&!value.attenuated&&
                !value.armed&&!value.published&&!value.durable&&value.retired==(state=="retired")&&(state=="configured"||state=="ready"||state=="retired");
        }
        if(recording||replay||sensor||proposal)
        {
            if(value.id||value.accepted_bytes||value.retained_receipts||value.published||value.durable||
               value.admissions||value.sequence||value.revoked||value.attenuated||value.armed)return false;
            auto state=value.state.view();
            if(value.retired!=(state=="retired")||(!value.retired&&state!="active"&&state!="idle"&&!(sensor&&state=="failed")))return false;
            if(recording)return value.binding.view()=="private-recording"&&!value.refused&&value.retained_bytes<=value.reserved_bytes&&value.members<=value.open_handles;
            if(replay)return value.binding.view()=="private-replay"&&!value.refused&&value.members==value.open_handles;
            if(sensor)return value.binding.view()=="bounded-sensor"&&value.open_handles<=value.members&&value.retained_bytes<=value.reserved_bytes&&
                value.status_code<=UINT32_MAX&&(value.retired||((state=="failed")==static_cast<bool>(value.status_code)));
            return value.binding.view()=="model-proposals"&&!value.open_handles&&!value.reserved_bytes&&!value.refused;
        }
        if(log||diagnostic)
        {
            if(value.id||value.open_handles||value.state.size||value.accepted_bytes||value.retained_receipts||value.published||value.durable||
               value.admissions||value.sequence||value.revoked||value.attenuated||value.armed)return false;
            if(log)return value.binding.view()=="private-log"&&value.retained_bytes<=value.reserved_bytes&&
                value.exported_records<=value.emitted_records&&value.lost_records<=value.emitted_records&&value.members<=value.emitted_records;
            return value.binding.view()=="local-diagnostic"&&!value.refused;
        }
        if(buffer)return !value.id&&value.binding.view()=="sealed-buffer"&&value.open_handles<=value.members&&
            !value.state.size&&!value.accepted_bytes&&!value.retained_receipts&&!value.published&&!value.durable;
        if(model)return !value.id&&value.binding.view()=="local-model"&&!value.state.size&&!value.accepted_bytes&&
            !value.reserved_bytes&&!value.retained_receipts&&!value.published&&!value.durable;
        if(transform)return !value.id&&value.binding.view()=="local-transform"&&value.open_handles<=value.members&&value.pending_queries<=value.query_count&&
            !value.state.size&&!value.accepted_bytes&&!value.retained_receipts&&!value.published&&!value.durable&&
            !value.admissions&&!value.refused&&!value.sequence&&!value.revoked&&!value.attenuated&&!value.armed;
        if(!output&&!receipt)
            return !value.state.size&&!value.accepted_bytes&&!value.reserved_bytes&&!value.retained_receipts&&!value.published&&!value.durable;
        auto state=value.state.view();
        if(state!="none"&&state!="staged"&&state!="published"&&state!="aborted"&&state!="removed")return false;
        if(value.durable||value.published!=(state=="published")||value.retained_receipts>1)return false;
        if(output)return value.id&&(state=="staged"||state=="published")&&value.accepted_bytes<=value.reserved_bytes;
        return value.retained_receipts==1&&!value.reserved_bytes&&
            ((state=="none"&&!value.id&&!value.accepted_bytes)||((state=="aborted"||state=="removed")&&value.id));
    }
    /** @brief Detached import rule. All descriptors below are static closed literals.
     * Effective exposure applies to environment members and confers no resource grant.
     */
    struct policy_row
    {
        text<255> environment;
        text<64> operation;
        std::string_view configured{},effective{},origin{};
    };
    inline bool valid(policy_row const& value) noexcept
    {
        auto mode=[](std::string_view v){return v=="hidden"||v=="allow"||v=="deny";};
        if(!value.environment.size||value.environment.truncated||!value.operation.size||value.operation.truncated||
           (!mode(value.configured)&&value.configured!="unset")||!mode(value.effective))return false;
        if(value.origin=="retired")return value.effective=="hidden";
        if(value.configured!="unset")return value.origin=="explicit"&&value.effective==value.configured;
        return is_bound_query(value.operation.view()) ? value.origin=="self-default"&&value.effective=="allow" :
                                                            value.origin=="default"&&value.effective=="hidden";
    }
    /** @brief Bounded page; unavailable snapshots carry no partial rows. */
    struct view
    {
        status result{status::not_selected};selection selected{};
        call_audit::statistics audit{};
        std::array<policy_row,maximum_rows> policy{};
        std::array<row,maximum_rows> rows{};std::size_t size{};
        std::uint64_t total{},next{};bool more{};
    };
    static_assert(maximum_rows*(4*(4*255+32+16)+1536)+512<65536);
    inline bool valid(request const& value) noexcept
    {
        auto exact=[](auto const& label){return label.size<=label.bytes.size()&&!label.truncated;};
        return value.selected<=selection::policy&&exact(value.operation)&&
            (value.selected==selection::policy?!value.alias.size:!value.operation.size)&&
            (value.selected!=selection::audit || (!value.environment.size && !value.alias.size))&&value.count&&value.count<=maximum_rows&&
            value.first<=maximum_scan&&exact(value.environment)&&exact(value.alias);
    }
    /** @brief Append a matching row while preserving bounded deterministic pagination. */
    inline void add(request const& query,view& output,row const& value) noexcept
    {
        if((query.environment.size&&query.environment.view()!=value.environment.view())||
           (query.alias.size&&query.alias.view()!=value.alias.view()))return;
        auto index=output.total++;
        if(index<query.first)return;
        if(output.size<query.count&&output.size<output.rows.size())
        {output.rows[output.size++]=value;output.next=index+1;}
        else output.more=true;
    }
    using snapshot_function=void(*)(request const&,view&) noexcept;
    inline std::atomic<snapshot_function> snapshot_hook{};
    /** @brief Invoke only the installed metadata hook after request validation. */
    inline void snapshot(request const& query,view& output) noexcept
    {
        output={};output.selected=query.selected;
        if(!valid(query)){output.result=status::invalid_request;return;}
        if(auto hook=snapshot_hook.load(std::memory_order_acquire))hook(query,output);
    }
}
}
