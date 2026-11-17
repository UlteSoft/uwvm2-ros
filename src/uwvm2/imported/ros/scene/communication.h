/** @file
 * @brief Owned, bounded ROS communication resources with a real provider boundary.
 * @details This provider never reads legacy ROS/WASI authority. Trusted deployment
 * installs immutable context, node, type and directional endpoint profiles. The
 * caller must hold its scene authority lease; owner triples below are supplied by
 * the broker, never accepted as guest authentication. Missing configuration grants
 * nothing. Resource and receive-buffer capacity is reserved before native creation.
 * Native implementations own real ROS objects and must not infer them from graph
 * names. Exact local publication replay prevents duplicate invocation within this
 * live provider; it cannot provide remote exactly-once or durable recovery.
 *
 * Subscriber ingress uses a preallocated complete-record ring. A failed output
 * copy does not consume its reserved front record. Ordinary close/revocation never
 * fabricates a physical stop. A backend call may block in middleware: deployment
 * must isolate native workers and enforce their lifetime/deadline; a postcheck is
 * not a hard realtime guarantee. Optional traffic attenuation belongs to broker
 * admission and is independent from the finite capacities required here.
 */
#pragma once
#include "communication_operation.h"
#include "communication_validation.h"
#include "../runtime/resources.h"
#include <uwvm2/utils/container/standard.h>
#include <uwvm2/utils/container/shared.h>
#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <memory>
#include <mutex>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <vector>
#if defined(__linux__)
# include <unistd.h>
#endif

namespace uwvm2::imported::ros::scene::communication
{
    enum class error : std::uint32_t
    {
        ok = 0,
        io = 5,
        bad_handle = 9,
        again = 11,
        no_memory = 12,
        denied = 13,
        busy = 16,
        invalid = 22,
        too_large = 90,
        unsupported = 95,
        timeout = 110,
        stale = 116,
        cancelled = 125
    };
    enum class effect : std::uint8_t
    {
        none,
        locally_accepted,
        observed_remote,
        unknown
    };
    /** @brief Accept only the closed communication result status vocabulary. */
    inline constexpr bool known_status(error value)noexcept
    {
        switch(value)
        {
            case error::ok:case error::io:case error::bad_handle:case error::again:case error::no_memory:case error::denied:
            case error::busy:case error::invalid:case error::too_large:case error::unsupported:case error::timeout:
            case error::stale:case error::cancelled:return true;
        }
        return false;
    }
    enum class role : std::uint8_t
    {
        publisher,
        subscription
    };
    enum class overflow : std::uint8_t
    {
        reject,
        drop_oldest
    };
    using handle = std::uint64_t;

    struct request_key
    {
        handle session{}, sequence{};
        friend bool operator==(request_key const&,request_key const&)=default;
    };
    struct receipt
    {
        handle request{},provider_generation{};request_key key;
        error status{};effect observed{};
    };
    /** @brief Trusted identity of the caller that owns a receive reservation.
     * @details A dispatcher derives these fields from its current VM caller and
     * actual reserve capability. They are never decoded from a guest carrier.
     * Ack/release additionally need their independently admitted exact action.
     */
    struct delivery_consumer
    {
        void const* instance{};handle generation{},capability{};
        explicit operator bool()const noexcept{return instance&&generation&&capability;}
        friend bool operator==(delivery_consumer const&,delivery_consumer const&)=default;
    };
    /** @brief Exact queue incarnation, record sequence and non-reusable borrow nonce. */
    struct delivery_reference
    {
        handle source{},generation{},sequence{},nonce{};
        friend bool operator==(delivery_reference const&,delivery_reference const&)=default;
    };
    /** @brief Local queue evidence, distinct from a remote provider completion.
     * @details source_generation is the queue incarnation. A supervising worker
     * adapter must supply its independently verified provider generation when
     * translating this local result into an externally visible request receipt.
     */
    struct delivery_receipt
    {handle request{},source_generation{};request_key key;error status{};effect observed{};};

    struct profile
    {
        ::uwvm2::utils::container::std_string identity;
        std::array<std::uint8_t, 32> digest{};
        friend bool operator== (profile const&, profile const&) = default;
    };

    struct duration
    {
        std::uint64_t seconds{}, nanoseconds{};
        friend bool operator== (duration const&, duration const&) = default;
    };

    /** @brief Closed effective QoS vocabulary; middleware defaults are not facts.
     * @details history=1 keep-last/2 keep-all; reliability=1 reliable/2 best-effort;
     * durability=1 transient-local/2 volatile; liveliness=1 automatic/2 manual.
     * Deployment reserves finite local depth even when native keep-all is chosen.
     */
    struct qos
    {
        std::uint32_t history{}, reliability{}, durability{}, liveliness{};
        std::uint64_t depth{};
        duration deadline{}, lifespan{}, lease{};
        friend bool operator== (qos const&, qos const&) = default;
    };

    struct metadata
    {
        std::uint64_t source{}, generation{}, sequence{}, loss_before{};
        std::optional<std::int64_t> source_nanoseconds, received_nanoseconds;
        std::array<std::uint8_t, 32> publisher{};
        std::size_t publisher_bytes{};
    };

    struct delivery
    {
        metadata information;
        std::span<std::uint8_t const> body;
    };

    inline std::uint64_t now() noexcept
    {
        auto n = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
        return n < 0 ? 0 : static_cast<std::uint64_t>(n);
    }

    inline bool valid(profile const& value) noexcept
    {
        return !value.identity.empty() && value.identity.size() <= 255 && std::any_of(value.digest.begin(), value.digest.end(), [](auto b) { return b != 0; });
    }

    inline bool valid(qos const& q) noexcept
    {
        return q.history >= 1 && q.history <= 2 && q.reliability >= 1 && q.reliability <= 2 && q.durability >= 1 && q.durability <= 2 && q.liveliness >= 1 &&
               q.liveliness <= 2 && q.depth && q.deadline.nanoseconds < 1000000000 && q.lifespan.nanoseconds < 1000000000 && q.lease.nanoseconds < 1000000000;
    }

    inline bool absolute_name(std::string_view value) noexcept
    {
        if(value.size() < 2 || value.size() > 255 || value.front() != '/' || value.back() == '/') { return false; }
        bool first = true;
        for(auto c: value.substr(1))
        {
            if(c == '/')
            {
                if(first) { return false; }
                first = true;
                continue;
            }
            if(!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || (!first && c >= '0' && c <= '9'))) { return false; }
            first = false;
        }
        return !first;
    }

    /** @brief Preallocated queue pairing actual native metadata with its body.
     * @details A single front reservation remains pinned across delivery. A
     * producer cannot overwrite it. Sequence/loss counters saturate by refusing
     * new ingress, never wrapping. No allocation or guest callback occurs under
     * the queue lock; allocation is confined to construction before activation.
     */
    class receive_queue
    {
        struct slot
        {
            metadata info;
            ::uwvm2::utils::container::std_vector<std::uint8_t> bytes;
            std::size_t size{};
        };

        std::mutex mutex;
        ::uwvm2::utils::container::std_vector<slot> slots;
        std::size_t front{}, count{}, maximum{};
        std::uint64_t sequence{}, loss{}, nonce{}, revision{1};
        handle source{}, generation{};
        overflow policy;
        bool reserved{}, retired{};
        bool borrowed{},settled_ack{};
        delivery_consumer consumer{},settled_consumer{};
        delivery_reference reference{},settled_reference{};
        std::uint64_t borrow_deadline{};
        delivery_receipt settlement{};
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

    public:
        receive_queue(std::size_t depth, std::size_t bytes, overflow mode) : slots(depth), maximum(bytes), policy(mode)
        {
            for(auto& s: slots) { s.bytes=::uwvm2::utils::container::std_vector<std::uint8_t>(bytes); }
        }

        /** @brief Copy a bounded record, optionally retaining caller ownership on pressure.
         * @details retained=true is for a trusted provider that keeps the same
         * message until retry succeeds. A full queue then changes no sequence,
         * loss or delivery state. The default callback mode records a drop.
         */
        error push(metadata value, std::span<std::uint8_t const> body,bool retained=false) noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};
            if(retired) { return error::cancelled; }
            if(body.size() > maximum || slots.empty() || !value.source || !value.generation || value.publisher_bytes > 32) { return error::invalid; }
            if(source && (source != value.source || generation != value.generation)) { return error::stale; }
            if(sequence == UINT64_MAX || loss == UINT64_MAX || revision == UINT64_MAX) { return error::too_large; }
            if(count == slots.size())
            {
                if(retained)return error::again;
                ++loss;
                if(policy == overflow::reject || reserved || borrowed) { return error::again; }
                front = (front + 1) % slots.size();
                --count;
            }
            auto& s = slots[(front + count) % slots.size()];
            value.sequence = ++sequence;
            value.loss_before = loss;
            s.info = value;
            s.size = body.size();
            std::copy(body.begin(), body.end(), s.bytes.begin());
            ++count;
            ++revision;
            return error::ok;
        }

        template <class Sink>
        error take(Sink&& sink)
        {
            if(!same_process())return error::denied;
            delivery view;
            std::uint64_t attempt{};
            {
                std::lock_guard lock{mutex};
                if(retired) { return error::cancelled; }
                if(reserved || borrowed || !count) { return error::again; }
                if(nonce == UINT64_MAX || revision == UINT64_MAX) { return error::too_large; }
                reserved = true;
                attempt = ++nonce;
                auto const& s = slots[front];
                view = {
                    s.info,
                    {s.bytes.data(), s.size}
                };
            }
            bool copied{};
            try
            {
                copied = sink(view);
            }
            catch(...)
            {
            }
            std::lock_guard lock{mutex};
            if(!reserved || nonce != attempt) { return error::stale; }
            reserved = false;
            if(retired) { return error::cancelled; }
            if(!copied) { return error::io; }
            front = (front + 1) % slots.size();
            --count;
            ++revision;
            return error::ok;
        }

        /** @brief Pin one original queue record across guest processing and IPC.
         * @details No copy, acknowledgement or allocation happens under this
         * mutex. The original slot remains pinned after a failed/throwing sink;
         * exact-consumer retries recover its reference and bytes. A different
         * consumer cannot steal it, and drop-oldest cannot overwrite it. All
         * ordinary takes are blocked until explicit acknowledgement or release.
         * The caller must bound deadline against its deployment plan and supply
         * trusted steady time. Expiry refuses access/ack but never silently drops
         * or unlocks the original record. Release remains available after expiry.
         * This core grants no authority; native and VM dispatchers must validate
         * the full node/context/schema group before reservation and delivery.
         */
        template<typename Sink>
        error reserve(delivery_consumer who,std::uint64_t deadline,std::uint64_t time,Sink&& sink)
        {
            if(!same_process())return error::denied;
            delivery view;delivery_reference selected;
            {
                std::lock_guard lock{mutex};
                if(!who)return error::denied;
                if(!time||deadline>INT64_MAX||deadline<=time)return error::timeout;
                if(retired)return error::cancelled;
                if(reserved)return error::again;
                if(borrowed)
                {
                    if(who!=consumer)return error::busy;
                    if(deadline!=borrow_deadline)return error::stale;
                }
                else
                {
                    if(!count)return error::again;
                    if(!source||!generation)return error::denied;
                    if(nonce==UINT64_MAX||revision==UINT64_MAX)return error::too_large;
                    consumer=who;borrow_deadline=deadline;borrowed=true;
                    reference={source,generation,slots[front].info.sequence,++nonce};++revision;
                }
                reserved=true;selected=reference;auto const& slot=slots[front];view={slot.info,{slot.bytes.data(),slot.size}};
            }
            bool copied{};
            try{copied=sink(selected,deadline,view);}catch(...){}
            std::lock_guard lock{mutex};reserved=false;
            if(!borrowed||reference!=selected)return error::stale;
            if(retired)return error::cancelled;
            return copied?error::ok:error::io;
        }

        /** @brief Consume or release precisely one owned reservation with an exact receipt.
         * @details Ack removes the original record once; release leaves it at the
         * head for another explicitly authorized reservation. Repeating the same
         * key, consumer, reference and decision is inert even after retirement.
         * Changed replays and skipped keys are rejected. The receipt and next
         * nonce never wrap. Release is local cleanup and must not be throttled by
         * a traffic/creation budget. This synchronous queue has definite local
         * effects only; an isolated adapter must retain lost-IPC uncertainty.
         */
        error settle(delivery_consumer who,delivery_reference selected,request_key key,bool acknowledge,
                     std::uint64_t time,delivery_receipt& output)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};
            if(!who||key.session!=source||!key.sequence)return error::denied;
            if(settlement.request&&key==settlement.key)
            {
                if(who!=settled_consumer||selected!=settled_reference||acknowledge!=settled_ack)return error::stale;
                output=settlement;return error::ok;
            }
            if(!borrowed||who!=consumer||selected!=reference)return error::denied;
            if(reserved)return error::again;
            if(settlement.key.sequence==UINT64_MAX||key.sequence!=settlement.key.sequence+1)return error::stale;
            if(acknowledge&&(retired||!time||time>=borrow_deadline))return retired?error::cancelled:error::timeout;
            if(revision==UINT64_MAX)return error::too_large;
            auto request=resources::allocate_identity();if(!request)return error::too_large;
            if(acknowledge){front=(front+1)%slots.size();--count;}
            settlement={request,generation,key,error::ok,effect::locally_accepted};
            settled_consumer=who;settled_reference=selected;settled_ack=acknowledge;
            borrowed=false;consumer={};reference={};borrow_deadline=0;++revision;
            output=settlement;return error::ok;
        }

        /** @brief Inspect local borrow ownership without polling or changing expiry. */
        bool has_reservation()noexcept
        {if(!same_process())return true;std::lock_guard lock{mutex};return borrowed||reserved;}

        void retire() noexcept
        {
            if(!same_process())return;
            std::lock_guard lock{mutex};
            retired = true;
            if(revision != UINT64_MAX) { ++revision; }
        }

        error bind(handle selected, handle incarnation) noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};
            if(reserved||borrowed) { return error::busy; }
            if(revision == UINT64_MAX) { return error::too_large; }
            if(!selected || !incarnation) { return error::invalid; }
            source = selected;
            generation = incarnation;
            front = count = 0;
            retired = false;
            settlement={};settled_consumer={};settled_reference={};settled_ack=false;
            ++revision;
            return error::ok;
        }

        void observe(std::uint64_t& version, bool& ready, bool& terminal) noexcept
        {
            if(!same_process()){version=0;ready=false;terminal=true;return;}
            std::lock_guard lock{mutex};
            version = revision;
            ready = count != 0 && !retired && !borrowed && !reserved;
            terminal = retired;
        }
    };

    enum class transport : std::uint8_t
    {
        disabled,
        unauthenticated_loopback
    };

    struct context_plan
    {
        owner authority;
        profile selected;
        std::uint32_t domain{};
        ::uwvm2::utils::container::std_string enclave;
        transport network{};
    };

    struct node_plan
    {
        owner authority;
        profile selected, context;
        ::uwvm2::utils::container::std_string name, name_space;
        std::size_t max_observers{};
    };

    struct type_plan
    {
        profile selected;
        ::uwvm2::utils::container::std_string type_identity, encoding;
        ::uwvm2::utils::container::std_vector<std::uint8_t> descriptor;
        message_validator validate;
        owner authority;
        bool revoked{};
    };

    struct topic_plan
    {
        owner authority;
        profile selected, node, type;
        ::uwvm2::utils::container::std_string resolved_name;
        role direction{};
        qos quality;
        std::size_t max_message_bytes{}, queue_depth{}, queue_bytes{};
        overflow overflow_policy{};
        std::uint64_t max_deadline_ns{};
    };

    enum class service_role : std::uint8_t
    {
        client,
        server
    };

    /** @brief Separate request/response schemas and finite native correlation slots.
     * @details Both directions use one immutable resolved service name. A client
     * can submit only requests; a server can answer only a delivered request.
     * Pending slots include completed but not yet copied replies. Timeout does
     * not refund a slot or imply remote cancellation; explicit abandon retires
     * local interest. The worker's middleware allocation budget is additional.
     */
    struct service_plan
    {
        owner authority;
        profile selected, node, request_type, response_type;
        ::uwvm2::utils::container::std_string resolved_name, service_type;
        service_role direction{};
        qos quality;
        std::size_t pending{}, max_request_bytes{}, max_response_bytes{}, reserved_bytes{};
        std::uint64_t max_deadline_ns{};
    };

    struct service_delivery
    {
        metadata information;
        handle correlation{};
        bool response{};
        std::span<std::uint8_t const> body;
    };

    using service_sink = bool (*)(void*, service_delivery const&);
    using service_reservation_sink=bool(*)(void*,delivery_reference,std::uint64_t,service_delivery const&);

    /** @brief Native acceptance evidence, including deferred owned-queue work.
     * @details request_identity is a HOST request minted by the registry or an
     * exact retained retry of it, never a worker's remote resource/goal ID.
     * provider_generation is the actual isolated worker incarnation when set.
     * deferred means only the host queue accepted the operation; stateful cores
     * must wait for a trusted completion before advancing observed native state.
     */
    struct native_result
    {
        error status{error::io};effect observed{effect::none};
        handle request_identity{},provider_generation{};bool deferred{};
    };
    /** @brief Preserve uncertain mutation outcomes at the native callback boundary.
     * @details A callback has already run, so an invalid enum, foreign request,
     * contradictory success/no-effect or failure/accepted pair cannot safely be
     * returned as a pre-effect rejection. Retain an io/unknown receipt using the
     * host's own request identity. A deferred result must prove queue acceptance
     * with its exact host request and worker generation; it is never completion.
     * This allocation-free check neither grants permission nor retries work.
     */
    inline native_result checked_mutation(native_result value,handle request)noexcept
    {
        bool invalid=!known_status(value.status)||value.observed>effect::unknown||
            (value.request_identity&&value.request_identity!=request)||
            (value.status==error::ok&&value.observed==effect::none)||
            (value.status!=error::ok&&value.observed!=effect::none&&value.observed!=effect::unknown)||
            (value.deferred&&(value.status!=error::ok||value.observed!=effect::locally_accepted||!request||
                value.request_identity!=request||!value.provider_generation));
        if(invalid){value.status=error::io;value.observed=effect::unknown;value.request_identity=request;value.deferred=false;}
        return value;
    }

    struct native_context
    {
        virtual ~native_context() = default;
        virtual bool live() const noexcept = 0;
        virtual error shutdown() noexcept = 0;
        virtual native_result shutdown(operation_token const&)noexcept
        {auto status=shutdown();return {status,status==error::ok?effect::locally_accepted:effect::unknown};}
    };

    struct native_node
    {
        virtual ~native_node() = default;
        /** @brief Cached verified provider incarnation; zero means unavailable.
         * @details This observation must not call middleware, allocate or wait.
         * It is independent of the authority generation. Providers without
         * verified incarnation evidence cannot enter asynchronous node receipts.
         */
        virtual handle provider_generation()const noexcept{return 0;}
    };

    /** @brief Cached evidence from the exact retained native endpoint.
     * @details Unknown provider incarnation and unavailable native identity are
     * absent. A deployment profile, grant generation or all-zero GID is not
     * evidence. Providers populate this record during successful creation and
     * return it without middleware calls, allocation or waiting.
     */
    struct endpoint_identity
    {
        std::optional<handle> provider_generation;
        std::array<std::uint8_t,32> native{};
        std::size_t size{};
    };
    inline bool valid(endpoint_identity const& value)noexcept
    {
        return (!value.provider_generation||*value.provider_generation)&&value.size<=value.native.size()&&
            (!value.size||std::any_of(value.native.begin(),value.native.begin()+value.size,[](auto byte){return byte!=0;}));
    }
    struct native_topic
    {
        virtual ~native_topic() = default;
        /** @brief Trusted immutable accounting choice, sampled only at creation.
         * @details True delegates mutation charging to the bounded owned queue.
         * Such a provider must reserve before charging, reject tokenless effects
         * and consume the complete current group exactly once. It is not a guest
         * option or an exemption from authority checks. Reads retain their own
         * explicit admission boundary.
         */
        virtual bool queued_mutations()const noexcept{return false;}
        /** @brief Freeze independent reservation-first receive accounting at creation.
         * @details An isolated receiver owns and charges its complete RPC before
         * publication. Its token-aware reserve/settle methods retain original
         * native records and unknown outcomes. The local queue is not a second
         * acknowledgement authority, and tokenless calls are forbidden.
         */
        virtual bool queued_receives()const noexcept{return false;}
        /// @brief Immutable reservation-first charging for asynchronous QoS observations.
        virtual bool queued_observations()const noexcept{return false;}
        using reservation_sink=bool(*)(void*,delivery_reference,std::uint64_t,delivery const&);
        virtual error reserve(delivery_consumer,std::uint64_t,void*,reservation_sink,operation_token const&)noexcept{return error::unsupported;}
        virtual error settle(delivery_consumer,delivery_reference,request_key,bool,delivery_receipt&,operation_token const&)noexcept{return error::unsupported;}
        virtual native_result publish(std::span<std::uint8_t const>, std::uint64_t) noexcept = 0;
        virtual error inspect(qos&, std::array<std::uint8_t, 32>&, std::size_t&) noexcept = 0;
        virtual native_result assert_liveliness() noexcept = 0;
        virtual error wait_acknowledged(std::uint64_t) noexcept = 0;
        virtual endpoint_identity identity()const noexcept{return {};}
        /** @brief Per-call trusted-origin entry points for isolated queued providers.
         * @details Such providers reject tokenless mutation methods. Accepted
         * calls only retain bounded owned work; no worker reply wait occurs under
         * the caller's authority group. Direct local fixtures may use the
         * synchronous default forwarding implementation.
         */
        virtual native_result publish(std::span<std::uint8_t const> body,std::uint64_t deadline,operation_token const&)noexcept{return publish(body,deadline);}
        virtual error inspect(qos& quality,std::array<std::uint8_t,32>& gid,std::size_t& size,operation_token const&)noexcept{return inspect(quality,gid,size);}
        virtual native_result assert_liveliness(operation_token const&)noexcept{return assert_liveliness();}
        virtual native_result wait_acknowledged(std::uint64_t deadline,operation_token const&)noexcept
        {auto status=wait_acknowledged(deadline);return {status,status==error::ok?effect::observed_remote:effect::unknown};}
        virtual error poll(operation_token const&)noexcept{return error::ok;}
        virtual bool occupied()noexcept{return false;}
        /** @brief Advance isolated destruction; synchronous local objects close by destruction.
         * @details Isolated providers override both methods. A host resource may
         * be removed only after its exact native close receipt is confirmed.
         */
        virtual error close(operation_token const&)noexcept{return error::ok;}
        virtual bool cleanup_confirmed()const noexcept{return true;}
    };

    struct native_service
    {
        virtual ~native_service() = default;
        /** @brief Same immutable mutation-accounting contract as native_topic. */
        virtual bool queued_mutations()const noexcept{return false;}
        /// @brief Immutable reservation-first accounting, independent of mutation accounting.
        virtual bool queued_receives()const noexcept{return false;}
        /// @brief Immutable reservation-first charging for asynchronous QoS/availability observations.
        virtual bool queued_observations()const noexcept{return false;}
        virtual native_result submit(handle, std::span<std::uint8_t const>, std::uint64_t) noexcept = 0;
        virtual native_result respond(handle, std::span<std::uint8_t const>, std::uint64_t) noexcept = 0;
        virtual error take(void*, service_sink) noexcept = 0;
        /** @brief Optional persistent borrow of an original preallocated service record. */
        virtual error reserve(delivery_consumer,std::uint64_t,void*,service_reservation_sink)noexcept{return error::unsupported;}
        /** @brief Exact local ack/release; an isolated adapter separately preserves IPC uncertainty. */
        virtual error settle(delivery_consumer,delivery_reference,request_key,bool,delivery_receipt&)noexcept{return error::unsupported;}
        virtual error available(bool&) noexcept = 0;
        /** @brief Read actual request/response endpoint QoS, never inferred defaults. */
        virtual error inspect(qos&) noexcept {return error::unsupported;}
        virtual native_result abandon(handle) noexcept = 0;
        virtual bool occupied() noexcept = 0;
        /** @brief Fence new calls without joining a provider or waiting for admission.
         * @details Registry revocation may hold authority and registry locks.
         * A native supervisor must perform any blocking drain separately after
         * releasing those locks, while retaining callback and queue ownership.
         */
        virtual void retire() noexcept = 0;
        /// @brief Same exact isolated teardown contract as native_topic; no implicit abandonment.
        virtual error close(operation_token const&)noexcept{return error::ok;}
        virtual bool cleanup_confirmed()const noexcept{return true;}
        virtual endpoint_identity identity()const noexcept{return {};}
        /** @brief Return independent cached request/response getter evidence.
         * @details Index zero is the request leg; index one is the response leg.
         * Missing native getters remain absent. This must not invoke middleware,
         * wait, allocate, or copy the requested policy into either actual leg.
         */
        virtual std::array<std::optional<qos>,2> channels()const noexcept{return {};}
        virtual native_result submit(handle request,std::span<std::uint8_t const> body,std::uint64_t deadline,operation_token const&)noexcept{return submit(request,body,deadline);}
        virtual native_result respond(handle correlation,std::span<std::uint8_t const> body,std::uint64_t deadline,operation_token const&)noexcept{return respond(correlation,body,deadline);}
        virtual error take(void* into,service_sink sink,operation_token const&)noexcept{return take(into,sink);}
        virtual error reserve(delivery_consumer who,std::uint64_t deadline,void* into,service_reservation_sink sink,operation_token const&)noexcept
        {return reserve(who,deadline,into,sink);}
        virtual error settle(delivery_consumer who,delivery_reference reference,request_key key,bool acknowledge,delivery_receipt& result,operation_token const&)noexcept
        {return settle(who,reference,key,acknowledge,result);}
        virtual error available(bool& result,operation_token const&)noexcept{return available(result);}
        virtual error inspect(qos& quality,operation_token const&)noexcept{return inspect(quality);}
        virtual native_result abandon(handle correlation,operation_token const&)noexcept{return abandon(correlation);}
    };

    /** @brief Trusted identity of one pending node-open attempt, never wire authority.
     * @details A worker stage must match every field and its fixed node plan.
     * Request digest covers the complete canonical node.open carrier, including
     * exact context capability/resource and node profile. A stage cannot migrate
     * to a different enrolled caller or caller incarnation.
     */
    struct node_stage_request
    {
        void const* caller_instance{};
        std::uint64_t caller_generation{};
        handle node_capability{},context_capability{},context{};
        owner context_authority;
        std::array<std::uint8_t,32> request_digest{};
        friend bool operator==(node_stage_request const&,node_stage_request const&)=default;
    };
    /** @brief Trusted factory receiving only immutable admitted profiles.
     * @details Success must return the actual retained context/node/endpoint.
     * A missing concrete type/operation is unsupported, never an empty success.
     * The factory cannot expose credentials, transport sockets or native pointers
     * to guest memory. Native roots are separate from the VM's portable types.
     */
    struct backend
    {
        virtual ~backend() = default;
        virtual error create_context(context_plan const&, std::shared_ptr<native_context>&) noexcept = 0;
        /** @brief Advance a bounded private startup without publishing transport.
         * @details This hook must not wait: pending startup returns again. The
         * caller already validated complete origins, capacity and creation-budget
         * preflight, but has not charged the successful creation unit. Only the
         * later create_node call may enable the broker after fresh admission.
         * No stage is created at deployment/configure time. Default synchronous
         * providers need no staging and return ok without effects.
         */
        virtual error prepare_node(node_plan const&,std::shared_ptr<native_context> const&,node_stage_request const&) noexcept{return error::ok;}
        /** @brief Fence matching unpublished stages without blocking for cleanup. */
        virtual void retire_owner(owner) noexcept{}
        virtual void capability_closed(handle) noexcept{}
        virtual void context_closed(owner,handle) noexcept{}

        virtual error create_node(node_plan const&, std::shared_ptr<native_context> const&, std::shared_ptr<native_node>&) noexcept = 0;
        virtual error create_topic(topic_plan const&,
                                   type_plan const&,
                                   std::shared_ptr<native_node> const&,
                                   handle,
                                   std::shared_ptr<receive_queue> const&,
                                   std::shared_ptr<native_topic>&) noexcept = 0;

        virtual error create_service(service_plan const&,
                                     type_plan const&,
                                     type_plan const&,
                                     std::shared_ptr<native_node> const&,
                                     handle,
                                     std::shared_ptr<native_service>&) noexcept
        { return error::unsupported; }
    };

    struct limits
    { std::size_t contexts{}, nodes{}, topics{}, types{}, total_queue_bytes{}, max_receipt_bytes{}, services{}; };

    struct context_info
    {
        profile selected;
        bool active{};
        handle generation{};
    };

    struct node_info
    {
        profile selected;
        ::uwvm2::utils::container::std_string name, name_space;
        handle generation{};
    };

    struct topic_info
    {
        profile selected;
        handle node{};
        ::uwvm2::utils::container::std_string name, type, encoding;
        role direction{};
        endpoint_identity native;
    };

    struct type_info
    {
        profile selected;
        ::uwvm2::utils::container::std_string identity, encoding;
        ::uwvm2::utils::container::std_vector<std::uint8_t> descriptor;
    };

    struct service_info
    {
        profile selected;
        handle node{};
        ::uwvm2::utils::container::std_string name,type;
        service_role direction{};
        endpoint_identity native;
        qos requested;
        std::array<std::optional<qos>,2> channels;
    };

    struct node_anchor
    {
        std::atomic_bool live{};
        std::atomic<handle> resource{};
        std::atomic<std::size_t> children{};
#if defined(__linux__)
        ::pid_t process{::getpid()};
#endif
    };

    /** @brief Move-only trusted native attachment counted against parent lifetime.
     * @details Only registry can mint this lease. It retains the actual node and
     * a revocation fence. Its raw native pointer is for trusted provider code and
     * must never be exposed to a guest or retained beyond the lease. Ordinary
     * node close refuses while any attachment lives; context shutdown/revocation
     * fences existing attachments before native teardown. This does not replace
     * per-operation authority admission for the attached provider.
     */
    class node_lease
    {
        friend class registry;
        std::shared_ptr<node_anchor> anchor;
        std::shared_ptr<native_node> value;
        handle parent_context{}, node_identity{};
        owner origin;

    public:
        node_lease() = default;
        node_lease(node_lease const&) = delete;
        node_lease& operator= (node_lease const&) = delete;

        node_lease(node_lease&& other) noexcept :
            anchor(std::move(other.anchor)), value(std::move(other.value)), parent_context(other.parent_context), node_identity(other.node_identity),
            origin(other.origin)
        {
        }

        node_lease& operator= (node_lease&& other) noexcept
        {
            if(this != &other)
            {
                reset();
                anchor = std::move(other.anchor);
                value = std::move(other.value);
                parent_context = other.parent_context;
                node_identity = other.node_identity;
                origin = other.origin;
            }
            return *this;
        }

        ~node_lease() { reset(); }

        void reset() noexcept
        {
#if defined(__linux__)
            if(anchor && anchor->process != ::getpid())
            {
                // A fork child must not run inherited middleware deleters or
                // touch possibly locked control blocks. End these local owner
                // lifetimes without releasing their inherited references. The
                // inaccessible child copy is reclaimed by exec/_exit; this is
                // not a mechanism for reusing a native provider after fork.
                std::construct_at(std::addressof(value));
                std::construct_at(std::addressof(anchor));
                return;
            }
#endif
            value.reset();
            if(anchor)
            {
                anchor->children.fetch_sub(1, std::memory_order_release);
                anchor.reset();
            }
        }

        explicit operator bool() const noexcept { return bool(anchor); }

        bool live() const noexcept
        {
#if defined(__linux__)
            if(anchor && anchor->process != ::getpid()) { return false; }
#endif
            return anchor && anchor->live.load(std::memory_order_acquire);
        }

        handle context() const noexcept { return parent_context; }

        handle identity() const noexcept { return node_identity; }

        owner authority() const noexcept { return origin; }

        native_node* get() const noexcept { return live() ? value.get() : nullptr; }
    };

    /** @brief Opaque registry evidence for an actual privately created node.
     * @details A creation request or integer token cannot construct this proof.
     * Registry mints it only from a live native node with verified provider
     * incarnation. The weak native reference does not delay physical cleanup.
     * Retained anchor state fences close/revoke/context shutdown and fork reuse.
     * This is not an authority lease or permission to call the native node.
     */
    class node_attachment
    {
        friend class registry;
        std::shared_ptr<node_anchor> anchor;
        std::weak_ptr<native_node> value;
        owner scope,parent_scope;
        handle resource{},context{},incarnation{};
    public:
        node_attachment()=default;
        bool live()const noexcept
        {
#if defined(__linux__)
            if(anchor&&anchor->process!=::getpid())return false;
#endif
            return anchor&&anchor->live.load(std::memory_order_acquire)&&anchor->resource.load(std::memory_order_acquire)==resource&&
                !value.expired()&&bool(scope)&&bool(parent_scope)&&resource&&context&&incarnation;
        }
        owner authority()const noexcept{return scope;}
        owner context_authority()const noexcept{return parent_scope;}
        handle identity()const noexcept{return resource;}
        handle context_identity()const noexcept{return context;}
        handle provider_generation()const noexcept{return incarnation;}
    };

    /** @brief Opaque host resource reservation for typed asynchronous adoption.
     * @details Only registry can mint this token. It is not a guest resource and
     * contains no native endpoint identity. Dropping it does not refund a slot:
     * the supervising creation ledger must establish no native effect or confirm
     * exact native cleanup before explicitly abandoning the reservation. Move
     * construction transfers the token; copying or replacement is forbidden.
     */
    class endpoint_slot
    {
        friend class registry;
        void const* registry_identity{};
        handle reserved{};
        owner authority;
        operation_family family{};
        endpoint_slot(void const* registry,handle id,owner origin,operation_family kind)noexcept
            :registry_identity(registry),reserved(id),authority(origin),family(kind){}
        void clear()noexcept{registry_identity=nullptr;reserved=0;authority={};family={};}
    public:
        endpoint_slot()=default;endpoint_slot(endpoint_slot const&)=delete;endpoint_slot& operator=(endpoint_slot const&)=delete;
        endpoint_slot(endpoint_slot&& other)noexcept:registry_identity(other.registry_identity),reserved(other.reserved),authority(other.authority),family(other.family)
        {other.clear();}
        endpoint_slot& operator=(endpoint_slot&&)=delete;
        explicit operator bool()const noexcept{return reserved!=0;}
        handle resource()const noexcept{return reserved;}
    };

    class registry
    {
        struct context_entry
        {
            context_plan plan;
            handle id{};
            std::shared_ptr<native_context> value;
            bool active{}, revoked{}, busy{}, has_receipt{};
            request_key previous;
            receipt last;
        };

        struct node_entry
        {
            node_plan plan;
            handle id{}, context{},provider_generation{};
            std::shared_ptr<native_node> value;
            std::shared_ptr<node_anchor> anchor;
            bool revoked{},creating{};
        };

        struct topic_entry
        {
            topic_plan plan;
            handle id{}, node{},adoption{};
            std::shared_ptr<native_topic> value;
            std::shared_ptr<receive_queue> queue;
            bool busy{}, has_receipt{}, revoked{},recovery_required{},pending_mutation{},queue_charged{},receive_queued{},observation_queued{};
            request_key previous;
            receipt last;
            ::uwvm2::utils::container::std_vector<std::uint8_t> previous_body,attempt_body;
            std::uint64_t previous_deadline{};
            unsigned previous_operation{};
        };

        struct service_entry
        {
            service_plan plan;
            handle id{}, node{},adoption{};
            std::shared_ptr<native_service> value;
            bool revoked{}, busy{}, has_receipt{}, recovery_required{},pending_mutation{},queue_charged{},receive_queued{},observation_queued{};
            request_key previous;
            receipt last;
            ::uwvm2::utils::container::std_vector<std::uint8_t> previous_body, attempt_body;
            std::uint64_t previous_deadline{};
            handle previous_correlation{};
            unsigned previous_operation{};
        };

        std::mutex mutex;
        std::shared_ptr<backend> native;
        limits capacity;
        ::uwvm2::utils::container::std_vector<context_entry> contexts;
        ::uwvm2::utils::container::std_vector<node_entry> nodes;
        ::uwvm2::utils::container::std_vector<topic_entry> topics;
        ::uwvm2::utils::container::std_vector<type_plan> types;
        ::uwvm2::utils::container::std_vector<service_entry> services;
        std::size_t reserved_bytes{};
        bool frozen{}, retired{};
#if defined(__linux__)
        ::pid_t process{::getpid()};
#endif
        bool same_process() const noexcept
        {
#if defined(__linux__)
            return process == ::getpid();
#else
            return true;
#endif
        }

        static handle allocate() noexcept { return resources::allocate_identity(); }

        template <class Rows>
        static auto* find(Rows& rows, handle id)
        {
            for(auto& r: rows)
            {
                if(r.id == id && id) { return &r; }
            }
            return static_cast<typename Rows::value_type*>(nullptr);
        }

        bool active_node(handle id)
        {
            auto node = find(nodes, id);
            if(!node || node->revoked || !node->value) { return false; }
            auto context = find(contexts, node->context);
            return context && context->active && !context->revoked && context->value && context->value->live();
        }

        type_plan const* type(profile const& p) const
        {
            for(auto const& t: types)
            {
                if(t.selected == p) { return &t; }
            }
            return nullptr;
        }

    public:
        explicit registry(std::shared_ptr<backend> provider, limits bound) : native(std::move(provider)), capacity(bound)
        {
            contexts.reserve(bound.contexts);
            nodes.reserve(bound.nodes);
            topics.reserve(bound.topics);
            types.reserve(bound.types);
            services.reserve(bound.services);
        }

        /** @brief Never finalize an inherited native graph in a fork child.
         * @details Fork is unsupported for provider reuse. On a different PID,
         * local ownership containers are abandoned without executing their
         * inherited allocator/native cleanup; exec/_exit reclaims that address
         * space. Parent resources and parent attachment counts remain untouched.
         */
        ~registry()
        {
            if(!same_process())
            {
                std::construct_at(std::addressof(services));
                std::construct_at(std::addressof(types));
                std::construct_at(std::addressof(topics));
                std::construct_at(std::addressof(nodes));
                std::construct_at(std::addressof(contexts));
                std::construct_at(std::addressof(native));
            }
        }

        /** @brief Trusted configuration, frozen on first guest resource operation. */
        error configure(context_plan value)
        {
            if(!same_process()) { return error::denied; }
            std::lock_guard lock{mutex};
            if(frozen || retired || !native) { return error::denied; }
            if(!value.authority || !valid(value.selected) || value.domain > 232 || value.network != transport::unauthenticated_loopback)
            {
                return error::invalid;
            }
            if(contexts.size() == capacity.contexts) { return error::again; }
            for(auto const& p: contexts)
            {
                if(p.plan.selected.identity == value.selected.identity) { return error::invalid; }
            }
            context_entry next;
            next.plan = std::move(value);
            contexts.push_back(std::move(next));
            return error::ok;
        }

        error configure(node_plan value)
        {
            if(!same_process()) { return error::denied; }
            std::lock_guard lock{mutex};
            if(frozen || retired || !native) { return error::denied; }
            if(!value.authority || !valid(value.selected) || !valid(value.context) || value.name.empty() ||
               value.name.find('/') != ::uwvm2::utils::container::std_string::npos || !absolute_name('/' + value.name) ||
               (value.name_space != "/" && !absolute_name(value.name_space)))
            {
                return error::invalid;
            }
            if(nodes.size() == capacity.nodes) { return error::again; }
            for(auto const& p: nodes)
            {
                if(p.plan.selected.identity == value.selected.identity) { return error::invalid; }
            }
            node_entry next;
            next.plan = std::move(value);
            next.anchor = ::uwvm2::utils::container::make_shared<node_anchor>();
            nodes.push_back(std::move(next));
            return error::ok;
        }

        error configure(type_plan value)
        {
            if(!same_process()) { return error::denied; }
            std::lock_guard lock{mutex};
            if(frozen || retired || !native) { return error::denied; }
            if(!value.authority || !valid(value.selected) || value.type_identity.empty() || value.type_identity.size() > 255 || value.encoding.empty() ||
               value.encoding.size() > 255 || !value.validate || value.descriptor.size() > capacity.max_receipt_bytes)
            {
                return error::invalid;
            }
            if(types.size() == capacity.types) { return error::again; }
            for(auto const& p: types)
            {
                if(p.selected.identity == value.selected.identity || p.type_identity == value.type_identity) { return error::invalid; }
            }
            types.push_back(std::move(value));
            return error::ok;
        }

        error configure(topic_plan value)
        {
            if(!same_process()) { return error::denied; }
            std::lock_guard lock{mutex};
            if(frozen || retired || !native) { return error::denied; }
            if(!value.authority || !valid(value.selected) || !valid(value.node) || !type(value.type) || !absolute_name(value.resolved_name) ||
               !valid(value.quality) || value.direction > role::subscription || value.overflow_policy > overflow::drop_oldest || !value.max_message_bytes ||
               value.max_message_bytes > capacity.max_receipt_bytes || !value.max_deadline_ns || value.max_deadline_ns > INT64_MAX)
            {
                return error::invalid;
            }
            if(value.direction == role::subscription && (!value.queue_depth || value.queue_depth > value.queue_bytes / value.max_message_bytes))
            {
                return error::invalid;
            }
            if(value.queue_bytes > capacity.total_queue_bytes - reserved_bytes || topics.size() == capacity.topics) { return error::again; }
            for(auto const& p: topics)
            {
                if(p.plan.selected.identity == value.selected.identity) { return error::invalid; }
            }
            topic_entry next;
            next.plan = std::move(value);
            next.previous_body.reserve(next.plan.max_message_bytes);
            next.attempt_body.reserve(next.plan.max_message_bytes);
            if(next.plan.direction == role::subscription)
            {
                next.queue =
                    ::uwvm2::utils::container::make_shared<receive_queue>(next.plan.queue_depth, next.plan.max_message_bytes, next.plan.overflow_policy);
            }
            reserved_bytes += next.plan.queue_bytes;
            topics.push_back(std::move(next));
            return error::ok;
        }

        error configure(service_plan value)
        {
            if(!same_process()) { return error::denied; }
            std::lock_guard lock{mutex};
            if(frozen || retired || !native) { return error::denied; }
            if(!value.authority || !valid(value.selected) || !valid(value.node) || !type(value.request_type) || !type(value.response_type) ||
               !absolute_name(value.resolved_name) || value.service_type.empty() || value.service_type.size() > 255 || value.direction > service_role::server ||
               !valid(value.quality) || !value.pending || !value.max_request_bytes || !value.max_response_bytes ||
               value.max_request_bytes > capacity.max_receipt_bytes || value.max_response_bytes > capacity.max_receipt_bytes || !value.max_deadline_ns ||
               value.max_deadline_ns > INT64_MAX)
            {
                return error::invalid;
            }
            auto extent = std::max(value.max_request_bytes, value.max_response_bytes);
            if(value.pending > value.reserved_bytes / extent) { return error::invalid; }
            if(value.reserved_bytes > capacity.total_queue_bytes - reserved_bytes || services.size() == capacity.services) { return error::again; }
            for(auto const& row: services)
            {
                if(row.plan.selected.identity == value.selected.identity) { return error::invalid; }
            }
            service_entry row;
            row.plan = std::move(value);
            row.previous_body.reserve(extent);
            row.attempt_body.reserve(extent);
            reserved_bytes += row.plan.reserved_bytes;
            services.push_back(std::move(row));
            return error::ok;
        }

        error open_context(owner authority, profile const& selected, handle& output)
        {
            if(!same_process()) { return error::denied; }
            std::lock_guard lock{mutex};
            frozen = true;
            if(retired || !native) { return error::cancelled; }
            for(auto& c: contexts)
            {
                if(c.plan.selected == selected && c.plan.authority == authority && !c.revoked)
                {
                    if(c.id) { return error::busy; }
                    auto id = allocate();
                    if(!id) { return error::again; }
                    std::shared_ptr<native_context> value;
                    auto e = native->create_context(c.plan, value);
                    if(e != error::ok) { return e; }
                    if(!value) { return error::io; }
                    c.value = std::move(value);
                    c.id = id;
                    c.active = true;
                    output = id;
                    return error::ok;
                }
            }
            return error::denied;
        }

        /** @brief Nonblocking startup poll under trusted complete-origin admission.
         * @details The provider owns a finite stage and cleanup debt. No guest
         * resource identity is allocated and no creation rate is charged here.
         * Node/context plans and real retained native context are checked before
         * the provider receives the caller-bound stage identity. The immutable
         * plan and retained context survive a lock-free provider call; a second
         * registry check rejects concurrent close/revoke. In particular, a
         * provider's bilateral admission callback may inspect this registry.
         * Cleanup debt remains with the provider if a late check refuses startup.
         */
        error prepare_node(owner authority,owner context_authority,handle context,profile const& selected,node_stage_request const& request)
        {
            if(!same_process()||authority.environment!=context_authority.environment)return error::denied;
            if(!request.caller_instance||!request.caller_generation||!request.node_capability||!request.context_capability||
               request.context!=context||request.context_authority!=context_authority)return error::invalid;
            std::unique_lock lock{mutex};frozen=true;if(retired)return error::cancelled;
            auto retained=find(contexts,context);
            if(!retained||!retained->active||retained->revoked||retained->plan.authority!=context_authority)return error::denied;
            for(auto const& node:nodes)if(node.plan.selected==selected&&node.plan.authority==authority&&node.plan.context==retained->plan.selected&&!node.revoked)
            {
                if(node.id||node.creating)return error::busy;
                // Configuration is frozen, so plan/table addresses are stable.
                // Retain the actual objects, then call without a registry lock.
                auto context_value=retained->value;auto provider=native;
                lock.unlock();auto status=provider->prepare_node(node.plan,context_value,request);lock.lock();
                if(retired||node.revoked||retained->revoked||!retained->active||retained->value!=context_value)status=error::cancelled;
                else if(node.id||node.creating)status=error::busy;
                lock.unlock();return status;
            }
            return error::denied;
        }
        /** @brief Retire unpublished stages whose original capability was closed. */
        void capability_closed(handle cap)noexcept
        {if(same_process()&&native)native->capability_closed(cap);}

        /** @brief Create privately without holding the registry across provider callbacks.
         * @details An in-progress row excludes duplicate creation and context
         * close. Immutable plans and retained provider/context objects survive
         * the unlocked call. A late revoke/shutdown refuses publication; the
         * provider coordinator retains its previously recorded cleanup debt.
         * Native objects discarded on failure are destroyed after unlocking.
         */
        error open_node(owner authority, owner context_authority, handle context, profile const& selected, handle& output)
        {
            if(!same_process()) { return error::denied; }
            if(authority.environment != context_authority.environment) { return error::denied; }
            std::shared_ptr<native_node> value;
            std::shared_ptr<native_context> context_value;
            std::shared_ptr<backend> provider;
            std::unique_lock lock{mutex};
            frozen = true;
            if(retired) { return error::cancelled; }
            auto c = find(contexts, context);
            if(!c || !c->active || c->revoked || c->plan.authority != context_authority) { return error::denied; }
            for(auto& n: nodes)
            {
                if(n.plan.selected == selected && n.plan.authority == authority && n.plan.context == c->plan.selected && !n.revoked)
                {
                    if(n.id||n.creating) { return error::busy; }
                    auto id = allocate();
                    if(!id) { return error::again; }
                    context_value=c->value;provider=native;n.creating=true;n.context=context;
                    lock.unlock();auto e=provider->create_node(n.plan,context_value,value);
                    auto incarnation=value?value->provider_generation():0;lock.lock();n.creating=false;n.context=0;
                    if(retired||n.revoked||c->revoked||!c->active||c->value!=context_value)return error::cancelled;
                    if(e != error::ok) { return e; }
                    if(!value) { return error::io; }
                    n.provider_generation=incarnation;
                    n.value = std::move(value);
                    n.anchor->resource.store(id, std::memory_order_release);
                    n.anchor->live.store(true, std::memory_order_release);
                    n.id = id;
                    n.context = context;
                    output = id;
                    return error::ok;
                }
            }
            return error::denied;
        }

        error open_topic(owner authority,
                         owner node_authority,
                         handle node,
                         profile const& selected,
                         std::string_view name,
                         std::string_view type_identity,
                         role direction,
                         handle& output)
        {
            if(!same_process()) { return error::denied; }
            if(authority.environment != node_authority.environment) { return error::denied; }
            std::lock_guard lock{mutex};
            frozen = true;
            if(retired) { return error::cancelled; }
            auto n = find(nodes, node);
            if(!n || !active_node(node) || n->plan.authority != node_authority) { return error::denied; }
            for(auto& t: topics)
            {
                if(t.plan.selected == selected && t.plan.authority == authority && t.plan.node == n->plan.selected && !t.revoked)
                {
                    auto schema = type(t.plan.type);
                    if(!schema || schema->revoked || schema->type_identity != type_identity || t.plan.resolved_name != name || t.plan.direction != direction)
                    {
                        return error::denied;
                    }
                    if(t.id||t.adoption) { return error::busy; }
                    auto id = allocate();
                    if(!id) { return error::again; }
                    if(t.queue)
                    {
                        auto e = t.queue->bind(id, authority.generation);
                        if(e != error::ok) { return e; }
                    }
                    std::shared_ptr<native_topic> value;
                    auto e = native->create_topic(t.plan, *schema, n->value, id, t.queue, value);
                    if(e != error::ok) { return e; }
                    if(!value) { return error::io; }
                    t.queue_charged=value->queued_mutations();
                    t.receive_queued=value->queued_receives();
                    t.observation_queued=value->queued_observations();
                    t.value = std::move(value);
                    t.id = id;
                    t.node = node;
                    output = id;
                    return error::ok;
                }
            }
            return error::denied;
        }

        error open_service(owner a,
                           owner parent_owner,
                           handle parent,
                           profile const& selected,
                           std::string_view name,
                           std::string_view service_type,
                           service_role direction,
                           handle& output)
        {
            if(!same_process() || a.environment != parent_owner.environment) { return error::denied; }
            std::lock_guard lock{mutex};
            frozen = true;
            if(retired) { return error::cancelled; }
            auto n = find(nodes, parent);
            if(!n || !active_node(parent) || n->plan.authority != parent_owner) { return error::denied; }
            for(auto& row: services)
            {
                if(row.plan.selected != selected || row.plan.authority != a || row.plan.node != n->plan.selected || row.revoked) { continue; }
                if(row.plan.resolved_name != name || row.plan.service_type != service_type || row.plan.direction != direction) { return error::denied; }
                if(row.id||row.adoption) { return error::busy; }
                auto request_schema = type(row.plan.request_type), response_schema = type(row.plan.response_type);
                if(!request_schema || !response_schema || request_schema->revoked || response_schema->revoked) { return error::denied; }
                auto id = allocate();
                if(!id) { return error::again; }
                std::shared_ptr<native_service> value;
                auto status = native->create_service(row.plan, *request_schema, *response_schema, n->value, id, value);
                if(status != error::ok) { return status; }
                if(!value) { return error::io; }
                row.queue_charged=value->queued_mutations();
                row.receive_queued=value->queued_receives();
                row.observation_queued=value->queued_observations();
                row.value = std::move(value);
                row.id = id;
                row.node = parent;
                output = id;
                return error::ok;
            }
            return error::denied;
        }

        /** @brief Reserve a preconfigured endpoint slot before asynchronous native creation.
         * @details The actual counted node lease and all schemas must still be
         * live. The immutable plan already reserved queue/receipt storage. This
         * method allocates only a host identity, invokes no provider and charges
         * no rate. Pending slots prevent competing synchronous/asynchronous opens
         * and parent close; their identities cannot be used by normal operations.
         * Complete caller/capability admission belongs to the dispatcher.
         */
        error prepare_adoption(owner a,node_lease const& parent,profile const& selected,operation_family family,endpoint_slot& output)noexcept
        {
            if(!same_process()||output||!parent.live()||a.environment!=parent.authority().environment)return error::denied;
            std::lock_guard lock{mutex};frozen=true;
            auto n=find(nodes,parent.identity());
            if(retired||!n||!active_node(n->id)||n->plan.authority!=parent.authority()||n->context!=parent.context()||n->value.get()!=parent.get())return error::denied;
            auto reserve=[&](auto& row)
            {
                if(row.revoked)return error::denied;
                if(row.id||row.adoption)return error::busy;
                auto id=allocate();if(!id)return error::too_large;
                row.adoption=id;row.node=n->id;output.registry_identity=this;output.reserved=id;output.authority=a;output.family=family;return error::ok;
            };
            if(family==operation_family::topic)
            {
                for(auto& row:topics)if(row.plan.authority==a&&row.plan.selected==selected&&row.plan.node==n->plan.selected)
                {auto schema=type(row.plan.type);return schema&&!schema->revoked?reserve(row):error::denied;}
            }
            else if(family==operation_family::service)
            {
                for(auto& row:services)if(row.plan.authority==a&&row.plan.selected==selected&&row.plan.node==n->plan.selected)
                {auto request=type(row.plan.request_type),response=type(row.plan.response_type);return request&&response&&!request->revoked&&!response->revoked?reserve(row):error::denied;}
            }
            return error::denied;
        }
        /** @brief Adopt a verified typed topic into its exact previously reserved host slot.
         * @details The facade must already be bound to slot.resource() and the
         * verified native endpoint in the creation receipt. Cached provider
         * generation must match that receipt, independently of grant generation.
         * No native creation, admission, allocation or guest copy occurs here.
         * On failure the supervisor retains both token and native cleanup debt.
         */
        error adopt_topic(endpoint_slot& slot,std::shared_ptr<native_topic> value,handle provider_generation)noexcept
        {
            if(!same_process()||!value||!provider_generation||slot.registry_identity!=this||slot.family!=operation_family::topic)return error::denied;
            auto evidence=value->identity();if(!valid(evidence)||evidence.provider_generation!=provider_generation)return error::denied;
            bool mutations=value->queued_mutations(),receives=value->queued_receives(),observations=value->queued_observations();
            std::lock_guard lock{mutex};
            for(auto& row:topics)if(row.adoption==slot.reserved&&row.plan.authority==slot.authority)
            {
                auto schema=type(row.plan.type);
                if(!slot.reserved||retired||row.revoked||row.id||!active_node(row.node)||!schema||schema->revoked)return error::denied;
                if(row.queue){auto status=row.queue->bind(slot.reserved,slot.authority.generation);if(status!=error::ok)return status;}
                row.value=std::move(value);row.id=slot.reserved;row.adoption=0;row.queue_charged=mutations;row.receive_queued=receives;row.observation_queued=observations;slot.clear();return error::ok;
            }
            return error::denied;
        }
        /** @brief Adopt a typed service, retaining independent request/response evidence.
         * @details Both approved schemas and the original actual node must remain
         * live. The host resource is never the worker's request or endpoint ID.
         * Callback flags are sampled once; guest calls cannot change accounting.
         */
        error adopt_service(endpoint_slot& slot,std::shared_ptr<native_service> value,handle provider_generation)noexcept
        {
            if(!same_process()||!value||!provider_generation||slot.registry_identity!=this||slot.family!=operation_family::service)return error::denied;
            auto evidence=value->identity();auto channels=value->channels();
            if(!valid(evidence)||evidence.provider_generation!=provider_generation||(channels[0]&&!valid(*channels[0]))||(channels[1]&&!valid(*channels[1])))return error::denied;
            bool mutations=value->queued_mutations(),receives=value->queued_receives(),observations=value->queued_observations();
            std::lock_guard lock{mutex};
            for(auto& row:services)if(row.adoption==slot.reserved&&row.plan.authority==slot.authority)
            {
                auto request=type(row.plan.request_type),response=type(row.plan.response_type);
                if(!slot.reserved||retired||row.revoked||row.id||!active_node(row.node)||!request||!response||request->revoked||response->revoked)return error::denied;
                row.value=std::move(value);row.id=slot.reserved;row.adoption=0;row.queue_charged=mutations;row.receive_queued=receives;row.observation_queued=observations;slot.clear();return error::ok;
            }
            return error::denied;
        }
        /** @brief Reclaim an unpublished slot only with trusted positive cleanup evidence.
         * @details The supervisor passes true only for proven unsent/no-effect
         * creation, a verified endpoint-close receipt, or confirmed namespace
         * teardown. Cancel, timeout and process signalling alone are insufficient.
         * This can release capacity after revocation but cannot reactivate a plan.
         */
        error abandon_adoption(endpoint_slot& slot,bool native_absent_confirmed)noexcept
        {
            if(!same_process()||slot.registry_identity!=this||!slot.reserved)return error::denied;
            if(!native_absent_confirmed)return error::busy;
            std::lock_guard lock{mutex};
            auto clear=[&](auto& rows)
            {
                for(auto& row:rows)if(row.adoption==slot.reserved&&row.plan.authority==slot.authority)
                {row.adoption=row.node=0;slot.clear();return error::ok;}
                return error::denied;
            };
            return slot.family==operation_family::topic?clear(topics):slot.family==operation_family::service?clear(services):error::denied;
        }

        /** @brief Commit one typed service mutation with a recoverable exact receipt.
         * @details Native request correlation is the retained receipt request for
         * submit, and the exact previously delivered server correlation for reply.
         * An uncertain native call remains latched and cannot be repeated by a new
         * key. An exact retry only returns history, including after output failure.
         */
        error service_mutate(owner a,
                             handle id,
                             request_key key,
                             handle correlation,
                             std::string_view identity,
                             std::string_view encoding,
                             std::span<std::uint8_t const> body,
                             std::uint64_t deadline,
                             bool response,
                             receipt& output,
                             operation_token const* operation=nullptr)
        {
            if(!same_process()) { return error::denied; }
            if(operation&&!operation->valid(response?permission::service_respond:permission::service_submit,operation_family::service,a,id,false))return error::denied;
            std::unique_lock lock{mutex};
            auto row = find(services, id);
            if(!row || row->plan.authority != a || retired || row->revoked || !active_node(row->node) ||
               (response ? row->plan.direction != service_role::server : row->plan.direction != service_role::client))
            {
                return error::denied;
            }
            if(row->busy) { return error::again; }
            auto schema = type(response ? row->plan.response_type : row->plan.request_type);
            if(!schema || schema->revoked || schema->type_identity != identity || schema->encoding != encoding) { return error::denied; }
            if(key.session != id || !key.sequence || (!response && correlation) || (response && !correlation)) { return error::invalid; }
            if(row->has_receipt && key == row->previous)
            {
                if(row->previous_operation != (response ? 2u : 1u) || row->previous_deadline != deadline || row->previous_correlation != correlation ||
                   !std::equal(body.begin(), body.end(), row->previous_body.begin(), row->previous_body.end()))
                {
                    return error::stale;
                }
                output = row->last;
                return error::ok;
            }
            if(row->pending_mutation || row->recovery_required || (row->has_receipt && row->last.observed == effect::unknown)) { return error::busy; }
            if((row->has_receipt && row->previous.sequence == UINT64_MAX) || key.sequence != (row->has_receipt ? row->previous.sequence + 1 : 1))
            {
                return error::stale;
            }
            if(body.size() > (response ? row->plan.max_response_bytes : row->plan.max_request_bytes)) { return error::too_large; }
            if(!schema->validate(body)) { return error::invalid; }
            auto time = now();
            if(deadline <= time || deadline > INT64_MAX || deadline - time > row->plan.max_deadline_ns) { return error::timeout; }
            auto request = allocate();
            if(!request) { return error::again; }
            row->attempt_body.assign(body.begin(), body.end());
            row->busy = true;
            auto value = row->value;
            lock.unlock();
            auto authorized=operation?operation->with_request(request,key.session,key.sequence):operation_token{};
            auto result = response ? (operation?value->respond(correlation,body,deadline,authorized):value->respond(correlation,body,deadline)) :
                (operation?value->submit(request,body,deadline,authorized):value->submit(request,body,deadline));
            result=checked_mutation(result,request);
            lock.lock();
            row->busy = false;
            // A pre-effect resource rejection leaves the caller key reusable.
            if(result.observed == effect::none && result.status != error::ok) { return result.status; }
            row->previous_body.swap(row->attempt_body);
            row->has_receipt = true;
            row->previous = key;
            row->previous_deadline = deadline;
            row->previous_correlation = correlation;
            row->previous_operation = response ? 2 : 1;
            row->pending_mutation=result.deferred;
            row->last = {result.request_identity?result.request_identity:request,result.provider_generation?result.provider_generation:a.generation,key,result.status,result.observed};
            output = row->last;
            return error::ok;
        }

        error service_take(owner a, handle id, void* argument, service_sink sink,operation_token const* operation=nullptr)
        {
            if(!same_process() || !sink) { return error::denied; }
            if(operation&&!operation->valid(permission::service_take,operation_family::service,a,id,false))return error::denied;
            std::shared_ptr<native_service> value;
            {
                std::lock_guard lock{mutex};
                auto row = find(services, id);
                if(!row || row->plan.authority != a || retired || row->revoked || !active_node(row->node)) { return error::denied; }
                if(row->busy) { return error::again; }
                row->busy = true;
                value = row->value;
            }
            auto result = operation?value->take(argument,sink,*operation):value->take(argument,sink);
            {
                std::lock_guard lock{mutex};
                auto row = find(services, id);
                if(row) { row->busy = false; }
                if(!row || retired || row->revoked || !active_node(row->node)) { return error::cancelled; }
            }
            return result;
        }

        /** @brief Reserve one service request/response without marking it delivered.
         * @details The native adapter keeps the original header, correlation and
         * payload in bounded storage. The current caller's complete action and
         * parent/schema authority must be admitted by the dispatcher. Failed
         * copies retain the reservation; callbacks execute outside this mutex.
         */
        error reserve_service(owner a,handle id,delivery_consumer who,std::uint64_t deadline,void* argument,service_reservation_sink sink,
                              operation_token const* operation=nullptr)
        {
            if(!same_process()||!who||!sink)return error::denied;
            if(operation&&(!operation->valid(permission::service_reserve,operation_family::service,a,id,false)||
                who!=delivery_consumer{operation->caller_instance,operation->caller_generation,operation->capability}))return error::denied;
            std::shared_ptr<native_service> value;
            {
                std::lock_guard lock{mutex};auto row=find(services,id);
                if(!row||row->plan.authority!=a||retired||row->revoked||!active_node(row->node))return error::denied;
                if(row->busy)return error::again;
                if(row->receive_queued&&!operation)return error::denied;
                auto time=now();if(deadline>INT64_MAX||deadline<=time||deadline-time>row->plan.max_deadline_ns)return error::timeout;
                row->busy=true;value=row->value;
            }
            auto status=operation?value->reserve(who,deadline,argument,sink,*operation):value->reserve(who,deadline,argument,sink);
            {std::lock_guard lock{mutex};auto row=find(services,id);if(row)row->busy=false;}
            return status;
        }
        /** @brief Explicit service ack/release, with release available after parent fencing.
         * @details Acknowledgement does not send a service response or cancel a
         * remote request. Unknown cross-process effects belong to the isolated
         * adapter's retained receipt, not this synchronous local queue result.
         */
        error settle_service(owner a,handle id,delivery_consumer who,delivery_reference reference,request_key key,bool acknowledge,
                             delivery_receipt& output,operation_token const* operation=nullptr)noexcept
        {
            if(!same_process())return error::denied;
            if(operation&&(!operation->valid(acknowledge?permission::service_acknowledge:permission::service_release,operation_family::service,a,id,false)||
                who!=delivery_consumer{operation->caller_instance,operation->caller_generation,operation->capability}))return error::denied;
            std::shared_ptr<native_service> value;
            {
                std::lock_guard lock{mutex};auto row=find(services,id);
                if(!row||row->plan.authority!=a)return error::denied;
                if(acknowledge&&(retired||row->revoked||!active_node(row->node)))return error::denied;
                if(row->busy)return error::again;
                if(row->receive_queued&&!operation)return error::denied;
                if(!row->value)return error::cancelled;
                row->busy=true;value=row->value;
            }
            auto status=operation?value->settle(who,reference,key,acknowledge,output,*operation):value->settle(who,reference,key,acknowledge,output);
            {std::lock_guard lock{mutex};auto row=find(services,id);if(row)row->busy=false;}
            return status;
        }

        error service_available(owner a, handle id, bool& output,operation_token const* operation=nullptr)
        {
            if(!same_process()) { return error::denied; }
            if(operation&&!operation->valid(permission::service_available,operation_family::service,a,id,false))return error::denied;
            std::shared_ptr<native_service> value;
            {
                std::lock_guard lock{mutex};auto row=find(services,id);
                if(!row||row->plan.authority!=a||retired||row->revoked||!active_node(row->node)||row->plan.direction!=service_role::client)return error::denied;
                if(row->busy)return error::again;
                row->busy=true;value=row->value;
            }
            // Queued providers revalidate the complete origin cohort here.
            // Never retain the registry mutex across that callback. busy pins
            // the resource against close; revocation still wins after return.
            auto status=operation?value->available(output,*operation):value->available(output);
            {
                std::lock_guard lock{mutex};auto row=find(services,id);
                if(row)row->busy=false;
                if(!row||retired||row->revoked||!active_node(row->node))return error::cancelled;
            }
            return status;
        }

        /** @brief Retain the outcome of abandoning local service interest.
         * @details Queued providers return again until their independent cleanup
         * receipt is complete; exact retries may return the original host request
         * identity. A failed operation with an unknown effect is a receipt, not
         * a pre-effect error: preserve its status and fence subsequent mutations.
         * Local acceptance never implies that remote execution was cancelled.
         */
        error service_abandon(owner a, handle id, handle request, receipt& output,operation_token const* operation=nullptr)
        {
            if(!same_process()) { return error::denied; }
            if(operation&&!operation->valid(permission::service_abandon,operation_family::service,a,id,false))return error::denied;
            std::shared_ptr<native_service> value;
            handle local_request{request};
            {
                std::lock_guard lock{mutex};
                auto row = find(services, id);
                if(!row || row->plan.authority != a || retired || row->revoked || !active_node(row->node) || row->plan.direction != service_role::client)
                {
                    return error::denied;
                }
                if(row->busy) { return error::again; }
                if(operation){local_request=allocate();if(!local_request)return error::again;}
                row->busy = true;
                value = row->value;
            }
            auto authorized=operation?operation->with_request(local_request):operation_token{};
            auto result = operation?value->abandon(request,authorized):value->abandon(request);
            result=checked_mutation(result,result.request_identity?result.request_identity:local_request);
            if(result.deferred){result.status=error::io;result.observed=effect::unknown;result.deferred=false;}
            {
                std::lock_guard lock{mutex};
                auto row = find(services, id);
                if(row)
                {
                    row->busy = false;
                    if(result.observed == effect::unknown) { row->recovery_required = true; }
                }
            }
            if(result.status != error::ok&&result.observed==effect::none) { return result.status; }
            output = {result.request_identity?result.request_identity:local_request,result.provider_generation?result.provider_generation:a.generation,{},result.status,result.observed};
            return error::ok;
        }

        /** @brief Acquire one finite trusted observer attachment to an exact node.
         * @details The immutable node plan must reserve max_observers explicitly.
         * No native API call occurs here; a failed acquisition changes no handle
         * or count. The caller must move the returned lease into its retained
         * provider resource before any native object borrows the node.
         */
        error attach_node(owner a, handle id, profile const& selected, node_lease& output)
        {
            if(!same_process()) { return error::denied; }
            if(output) { return error::invalid; }
            std::lock_guard lock{mutex};
            auto row = find(nodes, id);
            if(retired || !row || row->plan.authority != a || row->plan.selected != selected || !active_node(id) || !row->anchor->live.load())
            {
                return error::denied;
            }
            if(row->anchor->children.load() >= row->plan.max_observers) { return error::again; }
            row->anchor->children.fetch_add(1, std::memory_order_relaxed);
            output.anchor = row->anchor;
            output.value = row->value;
            output.parent_context = row->context;
            output.node_identity = id;
            output.origin = a;
            return error::ok;
        }

        /** @brief Observe only owned queue readiness; never takes or returns a payload.
         * @details The broker separately admits resource.observe on the origin
         * grant. Exact owner/generation/PID checks prevent forged handle observation.
         * A closed or revoked source fails instead of exposing another owner's data.
         */
        error observe(owner a, handle id, handle generation, std::uint64_t& revision, bool& ready, bool& terminal)
        {
            if(!same_process() || generation != a.generation) { return error::denied; }
            std::lock_guard lock{mutex};
            if(retired) { return error::cancelled; }
            auto row = find(topics, id);
            if(!row || row->plan.authority != a || row->revoked || !active_node(row->node) || row->plan.direction != role::subscription)
            {
                return error::denied;
            }
            row->queue->observe(revision, ready, terminal);
            return error::ok;
        }

        /** @brief Check a trusted node attachment without exposing native objects.
         * @details This is a point-in-time observation, not a retained resource
         * lease. Callers must recheck under their authority transaction before
         * effects; additional native children must join the registry's lifetime
         * census rather than treating this boolean as permission to retain a node.
         */
        error validate_node(owner a, handle id, profile const& selected)
        {
            if(!same_process()) { return error::denied; }
            std::lock_guard lock{mutex};
            auto row = find(nodes, id);
            if(retired || !row || row->plan.authority != a || row->plan.selected != selected || !active_node(id)) { return error::denied; }
            return error::ok;
        }

        /** @brief Capture exact typed node evidence without creating an observer.
         * @details Called by the trusted creation coordinator under complete
         * current node/context authority. No native pointer escapes, no capacity
         * is added and no provider callback executes. The incarnation was cached
         * from the actual native object at creation. Missing evidence fails closed.
         */
        error node_evidence(owner a,handle id,node_attachment& output)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto node=find(nodes,id);
            if(retired||!node||node->plan.authority!=a||!active_node(id))return error::denied;
            if(!node->provider_generation)return error::unsupported;
            auto context=find(contexts,node->context);if(!context||!context->active||context->revoked)return error::denied;
            node_attachment actual;actual.anchor=node->anchor;actual.value=node->value;actual.scope=a;actual.parent_scope=context->plan.authority;
            actual.resource=id;actual.context=node->context;actual.incarnation=node->provider_generation;
            output=std::move(actual);return error::ok;
        }

        /** @brief Validate an exact live context without allocating a diagnostic copy. */
        error validate_context(owner a,handle id,profile const& selected)
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto row=find(contexts,id);
            return !retired&&row&&!row->revoked&&row->active&&row->plan.authority==a&&row->plan.selected==selected&&row->value&&row->value->live()?
                error::ok:error::denied;
        }

        error describe(owner a, handle id, context_info& output)
        {
            if(!same_process()) { return error::denied; }
            std::lock_guard lock{mutex};
            auto c = find(contexts, id);
            if(!c || c->plan.authority != a || retired || c->revoked) { return error::denied; }
            output = {c->plan.selected, c->active && c->value && c->value->live(), a.generation};
            return error::ok;
        }

        error describe(owner a, handle id, node_info& output)
        {
            if(!same_process()) { return error::denied; }
            std::lock_guard lock{mutex};
            auto n = find(nodes, id);
            if(!n || n->plan.authority != a || retired || !active_node(id)) { return error::denied; }
            output = {n->plan.selected, n->plan.name, n->plan.name_space, a.generation};
            return error::ok;
        }

        error describe(owner a, handle id, topic_info& output)
        {
            if(!same_process()) { return error::denied; }
            std::lock_guard lock{mutex};
            auto t = find(topics, id);
            if(!t || t->plan.authority != a || retired || t->revoked || !active_node(t->node)) { return error::denied; }
            auto schema = type(t->plan.type);
            if(!schema||schema->revoked||!t->value)return error::denied;
            auto evidence=t->value->identity();if(!valid(evidence))return error::io;
            output = {t->plan.selected, t->node, t->plan.resolved_name, schema->type_identity, schema->encoding, t->plan.direction,evidence};
            return error::ok;
        }

        /** @brief Describe the exact retained service without invoking middleware.
         * @details Schema revocation and parent retirement deny observation.
         * Missing native evidence stays absent; malformed evidence is an error.
         * This method grants no submission, response or service discovery rights.
         */
        error describe(owner a,handle id,service_info& output)
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto endpoint=find(services,id);
            if(!endpoint||endpoint->plan.authority!=a||retired||endpoint->revoked||!endpoint->value||!active_node(endpoint->node))return error::denied;
            auto request=type(endpoint->plan.request_type),response=type(endpoint->plan.response_type);
            if(!request||!response||request->revoked||response->revoked)return error::denied;
            auto identity=endpoint->value->identity();auto channels=endpoint->value->channels();
            if(!valid(identity)||(channels[0]&&!valid(*channels[0]))||(channels[1]&&!valid(*channels[1])))return error::io;
            output={endpoint->plan.selected,endpoint->node,endpoint->plan.resolved_name,endpoint->plan.service_type,
                endpoint->plan.direction,identity,endpoint->plan.quality,channels};return error::ok;
        }

        error describe_type(owner a, std::string_view identity, type_info& output)
        {
            if(!same_process()) { return error::denied; }
            std::lock_guard lock{mutex};
            if(retired) { return error::cancelled; }
            for(auto const& schema: types)
            {
                if(schema.type_identity == identity && schema.authority == a && !schema.revoked)
                {
                    output = {schema.selected, schema.type_identity, schema.encoding, schema.descriptor};
                    return error::ok;
                }
            }
            return error::denied;
        }

        /** @brief One complete publication with exact last-key replay.
         * @details Receipt reservation and full schema validation precede native
         * publication. A native failure after possible effect is retained as
         * unknown. Same key/different bytes or deadline conflicts; stale keys do
         * not reexecute. The broker must recover failed output via exact retry.
         */
        error publish(owner a,
                      handle id,
                      request_key key,
                      std::string_view identity,
                      std::string_view encoding,
                      std::span<std::uint8_t const> body,
                      std::uint64_t deadline,
                      receipt& output,
                      operation_token const* operation=nullptr)
        {
            if(!same_process()) { return error::denied; }
            if(operation&&!operation->valid(permission::topic_publish,operation_family::topic,a,id,false))return error::denied;
            std::unique_lock lock{mutex};
            auto t = find(topics, id);
            if(!t || t->plan.authority != a || retired || t->revoked || !active_node(t->node) || t->plan.direction != role::publisher) { return error::denied; }
            if(t->busy) { return error::again; }
            auto schema = type(t->plan.type);
            if(schema->revoked || schema->type_identity != identity || schema->encoding != encoding) { return error::denied; }
            if(key.session != id || !key.sequence) { return error::invalid; }
            if(t->has_receipt && key == t->previous)
            {
                if(t->previous_operation != 1 || t->previous_deadline != deadline ||
                   !std::equal(body.begin(), body.end(), t->previous_body.begin(), t->previous_body.end()))
                {
                    return error::stale;
                }
                output = t->last;
                return error::ok;
            }
            if(t->pending_mutation||t->recovery_required||(t->has_receipt && t->last.observed == effect::unknown)) { return error::busy; }
            if(key.sequence != (t->has_receipt ? t->previous.sequence + 1 : 1) || (t->has_receipt && t->previous.sequence == UINT64_MAX))
            {
                return error::stale;
            }
            if(body.size() > t->plan.max_message_bytes) { return error::too_large; }
            if(!schema->validate(body)) { return error::invalid; }
            auto time = now();
            if(deadline <= time || deadline > INT64_MAX || deadline - time > t->plan.max_deadline_ns) { return error::timeout; }
            auto request = allocate();
            if(!request) { return error::again; }
            t->attempt_body.assign(body.begin(), body.end());
            t->busy = true;
            auto provider = t->value;
            lock.unlock();
            auto authorized=operation?operation->with_request(request,key.session,key.sequence):operation_token{};
            auto result = operation?provider->publish(body,deadline,authorized):provider->publish(body,deadline);
            result=checked_mutation(result,request);
            lock.lock();
            t->busy = false;
            if(result.observed==effect::none&&result.status!=error::ok)return result.status;
            t->previous_body.swap(t->attempt_body);
            t->has_receipt = true;
            t->previous = key;
            t->previous_deadline = deadline;
            t->previous_operation = 1;
            t->pending_mutation=result.deferred;
            t->last = {result.request_identity?result.request_identity:request,result.provider_generation?result.provider_generation:a.generation,key,result.status,result.observed};
            output = t->last;
            return error::ok;
        }

        template <class Sink>
        error take(owner a, handle id, Sink&& sink,operation_token const* operation=nullptr)
        {
            if(!same_process()) { return error::denied; }
            if(operation&&!operation->valid(permission::topic_take,operation_family::topic,a,id,false))return error::denied;
            std::shared_ptr<receive_queue> queue;
            std::shared_ptr<native_topic> provider;
            {
                std::lock_guard lock{mutex};
                auto t = find(topics, id);
                if(!t || t->plan.authority != a || retired || t->revoked || !active_node(t->node) || t->plan.direction != role::subscription)
                {
                    return error::denied;
                }
                if(t->busy) { return error::again; }
                t->busy = true;
                queue = t->queue;
                provider=t->value;
            }
            auto ready=operation?provider->poll(*operation):error::ok;
            auto result = ready==error::ok?queue->take(std::forward<Sink>(sink)):ready;
            {
                std::lock_guard lock{mutex};
                auto t = find(topics, id);
                if(t) { t->busy = false; }
            }
            return result;
        }

        error describe_qos(owner a, handle id, qos& quality, std::array<std::uint8_t, 32>& gid, std::size_t& size,operation_token const* operation=nullptr)
        {
            if(!same_process()) { return error::denied; }
            std::shared_ptr<native_topic> value;
            std::shared_ptr<native_service> service;
            {
                std::lock_guard lock{mutex};
                auto t = find(topics, id);
                if(t)
                {
                    if(t->plan.authority != a || retired || t->revoked || !active_node(t->node)) { return error::denied; }
                    value = t->value;
                }
                else
                {
                    auto row=find(services,id);
                    if(!row||row->plan.authority!=a||retired||row->revoked||!active_node(row->node))return error::denied;
                    service=row->value;
                }
            }
            if(operation&&!operation->valid(permission::qos_describe,service?operation_family::service:operation_family::topic,a,id,false))return error::denied;
            if(service){auto status=operation?service->inspect(quality,*operation):service->inspect(quality);if(status==error::ok){gid={};size=0;}return status;}
            return operation?value->inspect(quality,gid,size,*operation):value->inspect(quality,gid,size);
        }

        /** @brief Reserve a live subscription's actual owned record with complete provenance.
         * @details The dispatcher supplies the real consumer and validates its
         * independent reserve action and retained node/context/schema grants.
         * This method enforces the finite endpoint deadline before provider poll.
         * Failed delivery retains the original record. The core mutex is never
         * held across polling or a sink; busy and queue pins prevent close/rebind.
         */
        template<typename Sink>
        error reserve_topic(owner a,handle id,delivery_consumer who,std::uint64_t deadline,Sink&& sink,operation_token const* operation=nullptr)
        {
            if(!same_process())return error::denied;
            if(operation&&(!operation->valid(permission::topic_reserve,operation_family::topic,a,id,false)||
                who!=delivery_consumer{operation->caller_instance,operation->caller_generation,operation->capability}))return error::denied;
            std::shared_ptr<receive_queue> queue;std::shared_ptr<native_topic> provider;bool queued{};
            auto time=now();
            {
                std::lock_guard lock{mutex};auto row=find(topics,id);
                if(!who||!row||row->plan.authority!=a||retired||row->revoked||!active_node(row->node)||row->plan.direction!=role::subscription)return error::denied;
                if(row->busy)return error::again;
                queued=row->receive_queued;if(queued&&!operation)return error::denied;
                if(deadline>INT64_MAX||deadline<=time||deadline-time>row->plan.max_deadline_ns)return error::timeout;
                row->busy=true;queue=row->queue;provider=row->value;
            }
            error status;
            if(queued)
            {
                auto callback=[&](delivery_reference reference,std::uint64_t until,delivery const& value){return sink(reference,until,value);};
                using callback_type=decltype(callback);
                status=provider->reserve(who,deadline,&callback,[](void* target,delivery_reference ref,std::uint64_t until,delivery const& value)
                    {return (*static_cast<callback_type*>(target))(ref,until,value);},*operation);
            }
            else
            {
                status=operation?provider->poll(*operation):error::ok;
                if(status==error::ok)status=queue->reserve(who,deadline,now(),std::forward<Sink>(sink));
            }
            {std::lock_guard lock{mutex};auto row=find(topics,id);if(row)row->busy=false;}
            return status;
        }

        /** @brief Inspect receive cleanup debt without starting another reservation.
         * @details Only a current, exact topic.release token may inspect this
         * host-internal state. A busy provider, retained record, outstanding RPC
         * or unknown result is never reported as quiescent. No payload, network
         * operation, acknowledgement or deadline extension is produced.
         */
        error topic_receive_quiescent(operation_token const& operation,bool& output)noexcept
        {
            output=false;if(!same_process()||!operation.valid(permission::topic_release,operation_family::topic,
                operation.authority,operation.resource,false))return error::denied;
            std::lock_guard lock{mutex};auto row=find(topics,operation.resource);
            if(!row||row->plan.authority!=operation.authority||row->plan.direction!=role::subscription)return error::denied;
            if(row->busy)return error::again;
            output=row->receive_queued?row->value&&!row->value->occupied():row->queue&&!row->queue->has_reservation();
            return error::ok;
        }

        /** @brief Settle one pinned topic record without losing local evidence.
         * @details Ack requires live parents; release is cleanup and may run
         * after parent revocation, using the exact original consumer/reference.
         * The dispatcher separately admits the chosen action. No native send is
         * performed here and the returned generation is the local source only.
         */
        error settle_topic(owner a,handle id,delivery_consumer who,delivery_reference reference,request_key key,bool acknowledge,
                           delivery_receipt& output,operation_token const* operation=nullptr)noexcept
        {
            if(!same_process())return error::denied;
            if(operation&&(!operation->valid(acknowledge?permission::topic_acknowledge:permission::topic_release,operation_family::topic,a,id,false)||
                who!=delivery_consumer{operation->caller_instance,operation->caller_generation,operation->capability}))return error::denied;
            std::shared_ptr<receive_queue> queue;std::shared_ptr<native_topic> provider;bool queued{};
            {
                std::lock_guard lock{mutex};auto row=find(topics,id);
                if(!row||row->plan.authority!=a||row->plan.direction!=role::subscription)return error::denied;
                if(acknowledge&&(retired||row->revoked||!active_node(row->node)))return error::denied;
                if(row->busy)return error::again;
                queued=row->receive_queued;if(queued&&!operation)return error::denied;
                if(queued&&!row->value)return error::cancelled;
                row->busy=true;queue=row->queue;provider=row->value;
            }
            auto status=queued?provider->settle(who,reference,key,acknowledge,output,*operation):queue->settle(who,reference,key,acknowledge,now(),output);
            {std::lock_guard lock{mutex};auto row=find(topics,id);if(row)row->busy=false;}
            return status;
        }

        error assert_liveliness(owner a, handle id, request_key key, receipt& output,operation_token const* operation=nullptr)
        {
            if(!same_process()) { return error::denied; }
            if(operation&&!operation->valid(permission::qos_liveliness_assert,operation_family::topic,a,id,false))return error::denied;
            std::unique_lock lock{mutex};
            auto t = find(topics, id);
            if(!t || t->plan.authority != a || retired || t->revoked || !active_node(t->node) || t->plan.direction != role::publisher) { return error::denied; }
            if(t->busy) { return error::again; }
            if(key.session != id || !key.sequence) { return error::invalid; }
            if(t->has_receipt && key == t->previous)
            {
                if(t->previous_operation != 2) { return error::stale; }
                output = t->last;
                return error::ok;
            }
            if(t->pending_mutation||t->recovery_required||(t->has_receipt && t->last.observed == effect::unknown)) { return error::busy; }
            if(key.sequence != (t->has_receipt ? t->previous.sequence + 1 : 1) || (t->has_receipt && t->previous.sequence == UINT64_MAX))
            {
                return error::stale;
            }
            auto request = allocate();
            if(!request) { return error::again; }
            t->busy = true;
            auto value = t->value;
            lock.unlock();
            auto authorized=operation?operation->with_request(request,key.session,key.sequence):operation_token{};
            auto result = operation?value->assert_liveliness(authorized):value->assert_liveliness();
            result=checked_mutation(result,request);
            lock.lock();
            t->busy = false;
            if(result.observed==effect::none&&result.status!=error::ok)return result.status;
            t->has_receipt = true;
            t->previous = key;
            t->previous_operation = 2;
            t->pending_mutation=result.deferred;
            t->previous_body.clear();
            t->previous_deadline = 0;
            t->last = {result.request_identity?result.request_identity:request,result.provider_generation?result.provider_generation:a.generation,key,result.status,result.observed};
            output = t->last;
            return error::ok;
        }

        error wait_acknowledged(owner a, handle id, std::uint64_t deadline, receipt& output,operation_token const* operation=nullptr)
        {
            if(!same_process()) { return error::denied; }
            if(operation&&!operation->valid(permission::qos_acknowledgements_wait,operation_family::topic,a,id,false))return error::denied;
            std::shared_ptr<native_topic> value;
            handle request{};
            {
                std::lock_guard lock{mutex};
                auto t = find(topics, id);
                if(!t || t->plan.authority != a || retired || t->revoked || !active_node(t->node) || t->plan.direction != role::publisher)
                {
                    return error::denied;
                }
                auto time = now();
                // An isolated observation may be recovering its retained result
                // after the deadline. Its owned queue checks deadlines before a
                // new enqueue; local providers still require a future deadline.
                if(!t->queue_charged&&(deadline <= time || deadline > INT64_MAX || deadline - time > t->plan.max_deadline_ns)) { return error::timeout; }
                request = allocate();
                if(!request) { return error::again; }
                value = t->value;
            }
            auto authorized=operation?operation->with_request(request):operation_token{};
            native_result result;
            if(operation)result=value->wait_acknowledged(deadline,authorized);
            else{auto status=value->wait_acknowledged(deadline);result={status,status==error::ok?effect::observed_remote:effect::unknown};}
            if(result.status==error::again&&!result.deferred)return error::again;
            {
                std::lock_guard lock{mutex};
                auto t = find(topics, id);
                if(retired || !t || t->revoked || t->plan.authority != a || !active_node(t->node)) { return error::cancelled; }
            }
            output = {result.request_identity?result.request_identity:request,result.provider_generation?result.provider_generation:a.generation,{},result.status,result.observed};
            return error::ok;
        }

        /** @brief Keyed context shutdown with exact recovery after failed output.
         * @details Local child admission is fenced before the native shutdown.
         * The receipt describes context teardown, never a physical safe stop.
         * Replays return the committed status even though the context is inactive.
         */
        error shutdown(owner a, handle id, request_key key, receipt& output,operation_token const* operation=nullptr)
        {
            if(!same_process()) { return error::denied; }
            if(operation&&!operation->valid(permission::context_shutdown,operation_family::context,a,id,false))return error::denied;
            std::unique_lock lock{mutex};
            auto c = find(contexts, id);
            if(!c || c->plan.authority != a || retired || c->revoked) { return error::denied; }
            if(c->busy) { return error::again; }
            if(key.session != id || !key.sequence) { return error::invalid; }
            if(c->has_receipt && c->previous == key)
            {
                output = c->last;
                return error::ok;
            }
            if(c->has_receipt) { return error::stale; }
            auto request = allocate();
            if(!request) { return error::again; }
            for(auto& row: nodes)
            {
                if(row.context == id && row.anchor) { row.anchor->live.store(false, std::memory_order_release); }
            }
            c->busy = true;
            c->active = false;
            auto value = c->value;
            for(auto& row: services)
            {
                auto n = find(nodes, row.node);
                if(n && n->context == id && row.value) { row.value->retire(); }
            }
            for(auto& row: topics)
            {
                auto n = find(nodes, row.node);
                if(n && n->context == id && row.queue) { row.queue->retire(); }
            }
            lock.unlock();
            auto authorized=operation?operation->with_request(request,key.session,key.sequence):operation_token{};
            native_result result;
            if(operation)result=value->shutdown(authorized);
            else{auto status=value->shutdown();result={status,status==error::ok?effect::locally_accepted:effect::unknown};}
            result=checked_mutation(result,request);
            lock.lock();
            c->busy = false;
            c->has_receipt = true;
            c->previous = key;
            c->last = {result.request_identity?result.request_identity:request,result.provider_generation?result.provider_generation:a.generation,key,result.status,result.observed};
            output = c->last;
            return error::ok;
        }

        error shutdown(owner a, handle id)
        {
            receipt result;
            auto status = shutdown(a, id, {id, 1}, result);
            return status == error::ok ? result.status : status;
        }

        /** @brief Read the accounting mode frozen with the actual native object.
         * @details This cached query performs no provider callback or native I/O.
         * Logical contexts retain dispatcher charging. Only trusted topic and
         * service adapters can select reservation-first queue charging.
         */
        error queued_submission(owner a,handle id,bool& output)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};
            if(auto row=find(topics,id);row&&row->plan.authority==a){output=row->queue_charged;return error::ok;}
            if(auto row=find(services,id);row&&row->plan.authority==a){output=row->queue_charged;return error::ok;}
            if(auto row=find(contexts,id);row&&row->plan.authority==a){output=false;return error::ok;}
            return error::denied;
        }
        /** @brief Read the receive accounting mode retained with the exact endpoint.
         * @details This performs no provider callback or I/O. It remains readable
         * for explicit release after parent retirement and cannot grant access.
         */
        error queued_receive(owner a,handle id,bool& output)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};
            if(auto row=find(topics,id);row&&row->plan.authority==a){output=row->receive_queued;return error::ok;}
            if(auto row=find(services,id);row&&row->plan.authority==a){output=row->receive_queued;return error::ok;}
            return error::denied;
        }
        /** @brief Read the immutable native-observation accounting mode without I/O. */
        error queued_observation(owner a,handle id,bool& output)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};
            if(auto row=find(topics,id);row&&row->plan.authority==a){output=row->observation_queued;return error::ok;}
            if(auto row=find(services,id);row&&row->plan.authority==a){output=row->observation_queued;return error::ok;}
            return error::denied;
        }
        /** @brief Read one retained exact receipt, including after resource fencing.
         * @details This is trusted cached bookkeeping; the dispatcher must check
         * the recovering caller's exact primary permission before delivery. It
         * does not validate or reactivate the dependencies needed for new effects.
         */
        error receipt_for(owner a,handle id,handle request,receipt& output)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};
            auto read=[&](auto* row)
            {if(!row||row->plan.authority!=a||!row->has_receipt||row->last.request!=request)return error::denied;output=row->last;return error::ok;};
            if(auto row=find(topics,id))return read(row);
            if(auto row=find(services,id))return read(row);
            return read(find(contexts,id));
        }
        /** @brief Reconcile one exact deferred mutation after all queue locks end.
         * @details A worker incarnation and HOST request must match the retained
         * receipt. A completion racing the submitting call returns again; the
         * supervisor must retain and retry it. Known no-effect failure permits a
         * later new key. Unknown effect latches recovery and prevents reuse.
         * Exact duplicate settlement is inert; a different duplicate rejects.
         * Retirement cannot turn a native failure into success or erase evidence.
         */
        error settle_operation(operation_token const& token,native_result result)noexcept
        {
            if(!same_process()||!known_status(result.status)||result.deferred||!token.request||result.request_identity!=token.request||!result.provider_generation||
                result.observed>effect::unknown||!token.valid(token.action,token.family,token.authority,token.resource))return error::invalid;
            result=checked_mutation(result,token.request);
            std::lock_guard lock{mutex};
            auto settle=[&](auto* row)
            {
                if(!row||row->plan.authority!=token.authority)return error::denied;
                if(row->busy)return error::again;
                if(!row->has_receipt||row->last.request!=token.request||row->last.provider_generation!=result.provider_generation)return error::stale;
                if(!row->pending_mutation)return row->last.status==result.status&&row->last.observed==result.observed?error::ok:error::stale;
                row->last.status=result.status;row->last.observed=result.observed;row->pending_mutation=false;
                row->recovery_required|=result.observed==effect::unknown;return error::ok;
            };
            if(token.family==operation_family::topic&&(token.action==permission::topic_publish||token.action==permission::qos_liveliness_assert))
                return settle(find(topics,token.resource));
            if(token.family==operation_family::service&&(token.action==permission::service_submit||token.action==permission::service_respond))
                return settle(find(services,token.resource));
            return error::denied;
        }

        /** @brief Advance admitted endpoint cleanup outside the registry mutex.
         * @details The dispatcher supplies a current resource.close token with
         * the exact retained cohort. A worker may return again until its bounded
         * close reply arrives. Busy/unknown state is not reset, and no traffic
         * or original parent grant is borrowed as implicit cleanup authority.
         */
        error close_owned(owner a,handle id,operation_token const& operation)
        {
            if(!same_process()||!operation.valid(permission::resource_close,operation.family,a,id,false)||
                (operation.family!=operation_family::topic&&operation.family!=operation_family::service))return error::denied;
            std::shared_ptr<native_topic> topic;std::shared_ptr<native_service> service;
            {
                std::lock_guard lock{mutex};
                auto acquire=[&](auto* row,auto& target)
                {
                    if(!row||row->plan.authority!=a||!row->value)return error::denied;
                    if(row->busy||row->pending_mutation||row->recovery_required||(row->has_receipt&&row->last.observed==effect::unknown))return error::busy;
                    row->busy=true;target=row->value;return error::ok;
                };
                auto status=operation.family==operation_family::topic?acquire(find(topics,id),topic):acquire(find(services,id),service);
                if(status!=error::ok)return status;
            }
            auto status=topic?topic->close(operation):service->close(operation);
            {
                std::lock_guard lock{mutex};
                if(topic){if(auto row=find(topics,id))row->busy=false;}
                else if(auto row=find(services,id))row->busy=false;
            }
            return status==error::ok?close(a,id):status;
        }
        /** @brief Release one owned resource without discarding unresolved effects.
         * @details Close refuses a borrowed/inflight endpoint or unknown native
         * publication receipt. Parent resources require children to close first.
         * Reopening mints a new identity; old native callbacks cannot push into
         * its rebound queue. Native object destruction occurs outside the registry
         * mutex. The broker's quota state is not reset by this operation.
         */
        error close(owner a, handle id)
        {
            if(!same_process()) { return error::denied; }
            std::shared_ptr<native_topic> topic_value;
            std::shared_ptr<native_service> service_value;
            std::shared_ptr<native_node> node_value;
            std::shared_ptr<native_context> context_value;
            {
                std::lock_guard lock{mutex};
                if(auto t = find(topics, id))
                {
                    if(t->plan.authority != a) { return error::denied; }
                    if(t->busy||t->pending_mutation||t->recovery_required||(t->has_receipt&&t->last.observed==effect::unknown)||(t->value&&t->value->occupied())) { return error::busy; }
                    if(t->queue&&t->queue->has_reservation())return error::busy;
                    if(t->value&&!t->value->cleanup_confirmed())return error::busy;
                    if(t->queue) { t->queue->retire(); }
                    topic_value = std::move(t->value);
                    t->id = t->node = 0;
                    t->has_receipt = false;
                    t->previous = {};
                    t->last = {};
                    t->previous_body.clear();
                    t->previous_operation = 0;
                    t->previous_deadline = 0;
                }
                else if(auto row = find(services, id))
                {
                    if(row->plan.authority != a) { return error::denied; }
                    if(row->busy || row->pending_mutation || row->recovery_required || (row->has_receipt && row->last.observed == effect::unknown) ||
                       (row->value && row->value->occupied()))
                    {
                        return error::busy;
                    }
                    if(row->value&&!row->value->cleanup_confirmed())return error::busy;
                    service_value = std::move(row->value);
                    row->id = row->node = 0;
                    row->has_receipt = false;
                    row->previous = {};
                    row->last = {};
                    row->previous_body.clear();
                    row->previous_deadline = row->previous_correlation = 0;
                    row->previous_operation = 0;
                }
                else if(auto n = find(nodes, id))
                {
                    if(n->plan.authority != a) { return error::denied; }
                    if(n->anchor->children.load(std::memory_order_acquire)) { return error::busy; }
                    for(auto const& t: topics)
                    {
                        if((t.id||t.adoption) && t.node == id) { return error::busy; }
                    }
                    for(auto const& row: services)
                    {
                        if((row.id||row.adoption) && row.node == id) { return error::busy; }
                    }
                    n->anchor->live.store(false, std::memory_order_release);
                    node_value = std::move(n->value);
                    n->id = n->context = 0;
                }
                else if(auto c = find(contexts, id))
                {
                    if(c->plan.authority != a) { return error::denied; }
                    if(c->busy || (c->has_receipt && c->last.observed == effect::unknown)) { return error::busy; }
                    for(auto const& n: nodes)
                    {
                        if((n.id||n.creating) && n.context == id) { return error::busy; }
                    }
                    context_value = std::move(c->value);
                    c->id = 0;
                    c->active = false;
                    c->has_receipt = false;
                    c->previous = {};
                    c->last = {};
                }
                else
                {
                    return error::bad_handle;
                }
            }
            if(context_value) { if(native)native->context_closed(a,id);return context_value->shutdown(); }
            return error::ok;
        }

        void retire() noexcept
        {
            if(!same_process()) { return; }
            {
                std::lock_guard lock{mutex};
                retired = true;
                frozen = true;
            }
            // Freeze guarantees stable vectors. Revoke performs all cleanup
            // outside the registry lock, including native executor shutdown.
            for(auto const& row: contexts) { revoke(row.plan.authority); }
            for(auto const& row: nodes) { revoke(row.plan.authority); }
            for(auto const& row: topics) { revoke(row.plan.authority); }
            for(auto const& row: services) { revoke(row.plan.authority); }
        }

        /** @brief Fence one grant and its dependent resources without allocating.
         * @details Ongoing calls retain their admitted native object until return;
         * the broker must linearize their effect with authority revocation. New
         * traffic is refused before cleanup. Receipts/IDs remain tombstoned and
         * unknown outcomes remain charged. Native destruction is outside locks.
         */
        void revoke(owner a) noexcept
        {
            if(!same_process()) { return; }
            {
                std::lock_guard lock{mutex};
                frozen = true;
                for(auto& t: types)
                {
                    if(t.authority == a) { t.revoked = true; }
                }
                for(auto& c: contexts)
                {
                    if(c.plan.authority == a)
                    {
                        c.revoked = true;
                        c.active = false;
                    }
                }
                for(auto& n: nodes)
                {
                    auto c = find(contexts, n.context);
                    if(n.plan.authority == a || (c && c->revoked))
                    {
                        n.revoked = true;
                        if(n.anchor) { n.anchor->live.store(false, std::memory_order_release); }
                    }
                }
                for(auto& row: services)
                {
                    auto n = find(nodes, row.node);
                    if(row.plan.authority == a || (n && n->revoked) || type(row.plan.request_type)->revoked || type(row.plan.response_type)->revoked)
                    {
                        row.revoked = true;
                        if(row.value)
                        {
                            row.recovery_required |= row.value->occupied();
                            row.value->retire();
                        }
                    }
                }
                for(auto& t: topics)
                {
                    auto n = find(nodes, t.node);
                    if(t.plan.authority == a || (n && n->revoked) || type(t.plan.type)->revoked)
                    {
                        t.revoked = true;
                        if(t.queue) { t.queue->retire(); }
                    }
                }
            }
            if(native)native->retire_owner(a);
            for(std::size_t i{}; i < services.size(); ++i)
            {
                std::shared_ptr<native_service> value;
                {
                    std::lock_guard lock{mutex};
                    if(services[i].revoked) { value = std::move(services[i].value); }
                }
            }
            for(std::size_t i{}; i < topics.size(); ++i)
            {
                std::shared_ptr<native_topic> value;
                {
                    std::lock_guard lock{mutex};
                    if(topics[i].revoked) { value = std::move(topics[i].value); }
                }
            }
            for(std::size_t i{}; i < nodes.size(); ++i)
            {
                std::shared_ptr<native_node> value;
                {
                    std::lock_guard lock{mutex};
                    if(nodes[i].revoked) { value = std::move(nodes[i].value); }
                }
            }
            for(std::size_t i{}; i < contexts.size(); ++i)
            {
                std::shared_ptr<native_context> value;
                {
                    std::lock_guard lock{mutex};
                    if(contexts[i].revoked) { value = contexts[i].value; }
                }
                if(value) { (void)value->shutdown(); }
            }
        }
    };
}  // namespace uwvm2::imported::ros::scene::communication
