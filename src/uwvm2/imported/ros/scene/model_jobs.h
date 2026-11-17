/** @file
 * @brief Bounded asynchronous model jobs retaining actual sealed-buffer leases.
 * @details Trusted deployment binds exact model/task/schema identities and a
 * parameter validator, then attaches an approved worker generation. Guests can
 * only open that binding and submit finite reservations. Model results remain
 * predictions/data: this provider never invokes a robot tool or grants target
 * authority. Worker attachment, claiming, emission and cleanup acknowledgement
 * are host-only APIs, not guest-selected plugins, URLs or credentials.
 *
 * Broker admission must cover the origin model action and every input's exact
 * buffer.read scope (plus explicit remote data export, when applicable). Those
 * leases remain held through pinning and admission. The job owns actual sealed
 * input leases; closing guest buffers cannot free the data or refund their
 * capacity. Cancellation requested and provider stopped are different states.
 * Unknown outcome or unconfirmed cleanup cannot be released as a terminal
 * receipt. A native worker retains its work token until all asynchronous input
 * users stop. Retirement is followed by worker cancellation/join/settlement;
 * destroying an environment is not evidence that a remote provider stopped.
 */
#pragma once
#include "buffers.h"
#include <uwvm2/utils/container/standard.h>
#include <uwvm2/utils/container/shared.h>
#include <atomic>
#include <chrono>
#include <deque>
#include <map>
#include <memory>
#include <mutex>
#include <span>
#include <string_view>
#include <utility>

namespace uwvm2::imported::ros::scene::models
{
    using error = resources::error;
    using handle = resources::handle;
    using digest = buffers::digest;
    using profile = buffers::profile;
    enum class state : std::uint8_t
    {
        queued,
        running,
        succeeded,
        failed,
        cancelled,
        outcome_unknown
    };
    enum class reason : std::uint8_t
    {
        none,
        cancel_requested,
        deadline,
        source_revoked,
        provider_lost,
        provider_failed,
        output_limit
    };
    inline constexpr std::array state_names{std::string_view{"queued"},
                                            std::string_view{"running"},
                                            std::string_view{"succeeded"},
                                            std::string_view{"failed"},
                                            std::string_view{"cancelled"},
                                            std::string_view{"outcome_unknown"}};
    inline constexpr std::array reason_names{std::string_view{},
                                             std::string_view{"cancel_requested"},
                                             std::string_view{"deadline"},
                                             std::string_view{"source_revoked"},
                                             std::string_view{"provider_lost"},
                                             std::string_view{"provider_failed"},
                                             std::string_view{"output_limit"}};

    struct request_key
    {
        handle session{}, sequence{};
        friend bool operator== (request_key const&, request_key const&) = default;
    };

    /** @brief Canonical capacity record, reserved in full before submission. */
    struct capacity
    { std::uint64_t bytes{}, items{}, workspace_bytes{}; };

    /** @brief Exact immutable worker/model/task binding with zero-default capacities.
     * @details validate_parameters is trusted, bounded, side-effect-free and
     * nonblocking. It validates the task's canonical parameter body before a
     * worker may claim the job. It cannot change grants or access guest memory.
     */
    struct plan
    {
        profile model, task;
        buffers::text<96> parameter_type, output_type;
        buffers::text<32> encoding;
        std::uint64_t sessions{}, requests{}, input_bytes{}, input_count{}, parameter_bytes{}, output_bytes{}, output_items{}, workspace_bytes{}, deadline_ns{};
        bool streaming{};
        bool (*validate_parameters)(std::span<std::uint8_t const>) noexcept {};
        bool (*validate_output)(std::span<std::uint8_t const>, bool terminal) noexcept {};
    };

    /** @brief Host-only identity/digest of one actual admitted sealed input.
     * @details Retained after cleanup to bind model-derived proposals to the
     * immutable inputs used by that request. This evidence is neither a live
     * buffer lease nor permission to access the source again.
     */
    struct input_evidence
    {
        handle buffer{};
        digest sha256{};
        friend bool operator== (input_evidence const&, input_evidence const&) = default;
    };

    /** @brief Sanitized current receipt; admitted execution is not physical authority. */
    struct receipt
    {
        handle request{};
        state phase{};
        std::uint64_t provider_generation{};
        reason why{};
        bool cleanup_confirmed{}, had_output{};
    };

    /** @brief Owned model chunk; schema/encoding are fixed by the binding. */
    struct output
    {
        std::uint64_t sequence{};
        bool terminal{};
        buffers::text<96> type;
        buffers::text<32> encoding;
        ::uwvm2::utils::container::std_vector<std::uint8_t> bytes;
    };

    struct usage
    { std::uint64_t sessions{}, requests{}, inputs{}, output_bytes{}, output_items{}, workspace{}; };

    inline std::uint64_t now() noexcept
    {
        auto value = resources::steady_now();
        return value < 0 ? 0 : static_cast<std::uint64_t>(value);
    }

    inline bool valid(plan const& value) noexcept
    {
        auto text_valid = [](auto const& text)
        {
            return text.size && text.size <= text.bytes.size() && text.view().find('\0') == std::string_view::npos &&
                   wire::utf8({reinterpret_cast<std::uint8_t const*>(text.bytes.data()), text.size});
        };
        return text_valid(value.model.identity) && text_valid(value.task.identity) && text_valid(value.parameter_type) && text_valid(value.output_type) &&
               text_valid(value.encoding) && value.sessions != UINT64_MAX && value.requests != UINT64_MAX && value.input_bytes <= INT64_MAX &&
               value.input_count <= 64 && value.parameter_bytes <= buffers::maximum_transfer && value.output_bytes <= INT64_MAX &&
               value.output_items != UINT64_MAX && value.workspace_bytes <= INT64_MAX && value.deadline_ns <= INT64_MAX && value.validate_parameters &&
               value.validate_output;
    }

    class registry
    {
        struct account
        {
            plan bounds;
            std::atomic<std::uint64_t> sessions{}, requests{}, inputs{}, output_bytes{}, output_items{}, workspace{}, generation{};
            std::atomic<bool> connected{}, retired{};
            std::uint64_t (*clock)() noexcept;
#if defined(__linux__)
            ::pid_t process{::getpid()};
#endif
            account(plan value, std::uint64_t (*time)() noexcept) : bounds{std::move(value)}, clock{time} {}
        };

        struct session
        {
            std::shared_ptr<account> owner;
            handle id{};
            std::uint64_t generation{}, last_sequence{};
            digest last_hash{};
            receipt last_receipt;
            bool published{};
            std::atomic<bool> closed{};

            explicit session(std::shared_ptr<account> value) : owner{std::move(value)} {}

            ~session() { owner->sessions.fetch_sub(1, std::memory_order_relaxed); }
        };

        struct input_bundle
        {
            std::shared_ptr<account> owner;
            ::uwvm2::utils::container::std_vector<buffers::registry::sealed_lease> values;
            std::uint64_t bytes{};

            ~input_bundle()
            {
                values.clear();
                if(owner) { owner->inputs.fetch_sub(bytes, std::memory_order_relaxed); }
            }
        };

        struct job
        {
            mutable std::mutex mutex;
            std::shared_ptr<session> origin;
            std::shared_ptr<input_bundle> inputs;
            std::array<input_evidence, 64> evidence{};
            std::size_t input_count{};
            handle id{};
            std::uint64_t deadline{}, generation{}, emitted_bytes{}, emitted_items{};
            capacity reserved;
            std::atomic<std::uint64_t> scratch_bytes{};
            ::uwvm2::utils::container::std_vector<std::uint8_t> parameters;
            ::uwvm2::utils::container::std_deque<std::shared_ptr<output const>> outputs;
            state phase{state::queued};
            reason why{};
            std::atomic<bool> cancel_requested{};
            bool cleanup_confirmed{}, delivering{}, charged{};
            state completed_as{};
            digest completion_digest{};
            std::uint64_t completion_bytes{};

            ~job()
            {
                outputs.clear();
                decltype(parameters){}.swap(parameters);
                inputs.reset();
                if(charged)
                {
                    auto& owner = *origin->owner;
                    owner.requests.fetch_sub(1, std::memory_order_relaxed);
                    owner.output_bytes.fetch_sub(reserved.bytes, std::memory_order_relaxed);
                    owner.output_items.fetch_sub(reserved.items, std::memory_order_relaxed);
                    owner.workspace.fetch_sub(reserved.workspace_bytes, std::memory_order_relaxed);
                }
            }
        };

        mutable std::mutex mutex;
        ::uwvm2::utils::container::std_map<::uwvm2::utils::container::std_string, std::shared_ptr<account>, std::less<>> bindings;
        ::uwvm2::utils::container::std_map<handle, std::shared_ptr<session>> sessions;
        ::uwvm2::utils::container::std_map<handle, std::shared_ptr<job>> jobs;
        bool retired{};
        std::uint64_t (*clock)() noexcept;
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

        std::shared_ptr<account> binding(std::string_view alias) const noexcept
        {
            auto it = bindings.find(alias);
            return retired || it == bindings.end() || it->second->retired.load() ? nullptr : it->second;
        }

        std::shared_ptr<session> owned_session(std::string_view alias, handle id) const noexcept
        {
            auto scope = binding(alias);
            auto it = sessions.find(id);
            return !scope || it == sessions.end() || it->second->owner != scope || !it->second->published || it->second->closed ? nullptr : it->second;
        }

        std::shared_ptr<job> owned_job(std::string_view alias, handle id) const noexcept
        {
            auto scope = binding(alias);
            auto it = jobs.find(id);
            return !scope || it == jobs.end() || it->second->origin->owner != scope ? nullptr : it->second;
        }

        static receipt sample(job const& value) noexcept
        { return {value.id, value.phase, value.generation, value.why, value.cleanup_confirmed, value.emitted_items != 0}; }

        static bool terminal(state phase) noexcept { return phase == state::succeeded || phase == state::failed || phase == state::cancelled; }

        static void cancel(job& value, reason why) noexcept
        {
            if(terminal(value.phase)) { return; }
            value.cancel_requested.store(true, std::memory_order_release);
            value.why = why;
            if(value.phase == state::queued)
            {
                value.phase = state::cancelled;
                value.cleanup_confirmed = true;
                value.inputs.reset();
            }
        }

        static void expire(job& value) noexcept
        {
            if(value.origin->owner->clock() >= value.deadline) { cancel(value, reason::deadline); }
        }

        /** @brief Fence source registry retirement before publishing new predictions.
         * @details The native adapter must also hold current source authority
         * while publishing. This checks the real allocation scopes without
         * treating immutable cached bytes as continuing permission to use them.
         */
        static void check_sources(job& value) noexcept
        {
            if(!value.inputs) { return; }
            for(auto const& input: value.inputs->values)
            {
                if(input.visit([](auto, auto const&, auto const&) { return true; }) != error::success)
                {
                    cancel(value, reason::source_revoked);
                    return;
                }
            }
        }

        static bool same_process(account const& value) noexcept
        {
#if defined(__linux__)
            return value.process == ::getpid();
#else
            static_cast<void>(value);
            return true;
#endif
        }

    public:
        class work;

        /** @brief Subreservation of the job's already charged native workspace.
         * @details Actual host scratch must be freed before this token. Retaining
         * the job keeps its complete quota charged even after request.release.
         * The token grants no new model/source access and cannot escape fork.
         */
        class scratch_lease
        {
            friend class work;
            std::shared_ptr<job> value;
            std::uint64_t bytes{};

        public:
            scratch_lease() = default;
            scratch_lease(scratch_lease const&) = delete;
            scratch_lease& operator= (scratch_lease const&) = delete;

            scratch_lease(scratch_lease&& other) noexcept : value{std::move(other.value)}, bytes{std::exchange(other.bytes, 0)} {}

            scratch_lease& operator= (scratch_lease&& other) noexcept
            {
                if(this != &other)
                {
                    reset();
                    value = std::move(other.value);
                    bytes = std::exchange(other.bytes, 0);
                }
                return *this;
            }

            ~scratch_lease() { reset(); }

            explicit operator bool() const noexcept { return static_cast<bool>(value); }

            std::uint64_t reserved_bytes() const noexcept { return value ? bytes : 0; }

            bool live() const noexcept
            { return value && same_process(*value->origin->owner) && !value->origin->owner->retired.load() && !value->cancel_requested.load(); }

            void reset() noexcept
            {
                if(value)
                {
                    value->scratch_bytes.fetch_sub(bytes, std::memory_order_relaxed);
                    value.reset();
                    bytes = 0;
                }
            }
        };

        /** @brief Shared charge for host-owned request carrier heap storage.
         * @details Declare before the owned carrier so bytes are freed before
         * refund. This grants no model, input or export authority. Revocation
         * rejects use while destruction remains safe after registry teardown.
         */
        class workspace_lease
        {
            friend class registry;
            std::shared_ptr<account> owner;
            std::uint64_t bytes{};

        public:
            workspace_lease() = default;
            workspace_lease(workspace_lease const&) = delete;
            workspace_lease& operator= (workspace_lease const&) = delete;

            workspace_lease(workspace_lease&& other) noexcept : owner{std::move(other.owner)}, bytes{std::exchange(other.bytes, 0)} {}

            workspace_lease& operator= (workspace_lease&& other) noexcept
            {
                if(this != &other)
                {
                    reset();
                    owner = std::move(other.owner);
                    bytes = std::exchange(other.bytes, 0);
                }
                return *this;
            }

            ~workspace_lease() { reset(); }

            explicit operator bool() const noexcept { return static_cast<bool>(owner); }

            std::uint64_t reserved_bytes() const noexcept { return owner ? bytes : 0; }

            bool live() const noexcept { return owner && same_process(*owner) && !owner->retired.load(std::memory_order_acquire); }

            void reset() noexcept
            {
                if(owner)
                {
                    owner->workspace.fetch_sub(bytes, std::memory_order_relaxed);
                    owner.reset();
                    bytes = 0;
                }
            }
        };

        /** @brief Reserve carrier bytes before allocation under the job workspace limit.
         * @details Independent authority preflight precedes this allocation-free
         * reservation. Job parameters and the live carrier overlap in memory,
         * so both remain charged until the carrier is actually destroyed.
         */
        error reserve_workspace(std::string_view alias, std::uint64_t bytes, workspace_lease& output)
        {
            if(!same_process()) { return error::denied; }
            if(output || !bytes || bytes > SIZE_MAX) { return error::invalid; }
            std::lock_guard lock{mutex};
            auto scope = binding(alias);
            if(!scope) { return error::denied; }
            auto used = scope->workspace.load(std::memory_order_relaxed);
            if(used > scope->bounds.workspace_bytes || bytes > scope->bounds.workspace_bytes - used) { return error::again; }
            scope->workspace.fetch_add(bytes, std::memory_order_relaxed);
            output.owner = std::move(scope);
            output.bytes = bytes;
            return error::success;
        }

        /** @brief Copy immutable policy for bounded decoding after authority preflight.
         * @details No allocation, charge, provider I/O or guest memory access.
         * Function pointers are trusted host policy, never serialized to guests.
         */
        error limits(std::string_view alias, plan& output) const
        {
            if(!same_process()) { return error::denied; }
            std::lock_guard lock{mutex};
            auto scope = binding(alias);
            if(!scope) { return error::denied; }
            output = scope->bounds;
            return error::success;
        }

        /** @brief Move-only native work token retaining job/input lifetime.
         * @details The worker must retain this token until all asynchronous
         * users stop. Dropping an unfinished token records unknown outcome;
         * it does not claim cancellation or cleanup. Input visitors must not
         * retain spans; native work retains the token and borrows per use.
         */
        class work
        {
            friend class registry;
            std::shared_ptr<job> value;

            void abandoned() noexcept
            {
                if(!value || !same_process(*value->origin->owner)) { return; }
                std::lock_guard lock{value->mutex};
                if(!terminal(value->phase) && !value->cleanup_confirmed)
                {
                    value->phase = state::outcome_unknown;
                    value->why = reason::provider_lost;
                    value->cancel_requested.store(true);
                }
            }

        public:
            work() = default;
            work(work const&) = delete;
            work& operator= (work const&) = delete;

            work(work&& other) noexcept : value{std::move(other.value)} {}

            work& operator= (work&& other) noexcept
            {
                if(this != &other)
                {
                    abandoned();
                    value = std::move(other.value);
                }
                return *this;
            }

            ~work() { abandoned(); }

            explicit operator bool() const noexcept { return static_cast<bool>(value); }

            handle identity() const noexcept { return value ? value->id : 0; }

            /** @brief Immutable host metadata; the facade checks authority before disclosure. */
            std::size_t input_count() const noexcept { return value ? value->input_count : 0; }

            /** @brief Absolute steady deadline for a supervising native process. */
            std::uint64_t deadline() const noexcept { return value ? value->deadline : 0; }

            bool cancellation_requested() const noexcept
            {
                return !value || !same_process(*value->origin->owner) || value->cancel_requested.load(std::memory_order_acquire) ||
                       value->origin->owner->retired.load();
            }

            /** @brief Reserve native copies within this job's existing workspace budget.
             * @details The authority facade revalidates before this operation.
             * Resident parameters and every concurrent scratch allocation count
             * together. The helper allocates nothing and never expands capacity.
             */
            error reserve_scratch(std::uint64_t bytes, scratch_lease& output) const
            {
                if(!value) { return error::bad_handle; }
                if(!same_process(*value->origin->owner)) { return error::denied; }
                if(output || !bytes || bytes > SIZE_MAX) { return error::invalid; }
                std::lock_guard lock{value->mutex};
                expire(*value);
                if(value->phase != state::running || cancellation_requested()) { return error::cancelled; }
                auto maximum = value->reserved.workspace_bytes - value->parameters.size();
                auto used = value->scratch_bytes.load(std::memory_order_relaxed);
                if(used > maximum || bytes > maximum - used) { return error::again; }
                value->scratch_bytes.fetch_add(bytes, std::memory_order_relaxed);
                output.value = value;
                output.bytes = bytes;
                return error::success;
            }

            /** @brief Visit one real sealed input; revoked source authority fails closed. */
            template <typename Visitor>
            error with_input(std::size_t index, Visitor&& visitor) const
            {
                if(!value) { return error::bad_handle; }
                if(!same_process(*value->origin->owner)) { return error::denied; }
                std::shared_ptr<input_bundle> retained;
                {
                    std::lock_guard lock{value->mutex};
                    expire(*value);
                    if(value->phase != state::running || cancellation_requested()) { return error::cancelled; }
                    retained = value->inputs;
                    if(!retained || index >= retained->values.size()) { return error::invalid; }
                }
                auto status = retained->values[index].visit(std::forward<Visitor>(visitor));
                if(status != error::success)
                {
                    std::lock_guard lock{value->mutex};
                    cancel(*value, status == error::denied || status == error::cancelled ? reason::source_revoked : reason::provider_failed);
                }
                return status;
            }

            /** @brief Borrow validated task parameters without exposing guest memory. */
            template <typename Visitor>
            error with_parameters(Visitor&& visitor) const
            {
                if(!value) { return error::bad_handle; }
                if(!same_process(*value->origin->owner)) { return error::denied; }
                auto retained = value;
                {
                    std::lock_guard lock{retained->mutex};
                    expire(*retained);
                    if(retained->phase != state::running || cancellation_requested()) { return error::cancelled; }
                }
                try
                {
                    return visitor(std::span<std::uint8_t const>{retained->parameters}) ? error::success : error::io;
                }
                catch(...)
                {
                    return error::io;
                }
            }

            /** @brief Append a bounded prediction chunk; cumulative output never refills on take. */
            error emit(std::span<std::uint8_t const> bytes)
            {
                if(!value) { return error::bad_handle; }
                if(!same_process(*value->origin->owner)) { return error::denied; }
                if(bytes.size() > buffers::maximum_transfer) { return error::too_large; }
                if(!value->origin->owner->bounds.validate_output(bytes, false)) { return error::invalid; }
                std::lock_guard lock{value->mutex};
                expire(*value);
                check_sources(*value);
                if(value->phase != state::running || cancellation_requested()) { return error::cancelled; }
                auto const& policy = value->origin->owner->bounds;
                if(!policy.streaming) { return error::unsupported; }
                if(bytes.size() > buffers::maximum_transfer || value->emitted_items >= value->reserved.items ||
                   bytes.size() > value->reserved.bytes - value->emitted_bytes)
                {
                    cancel(*value, reason::output_limit);
                    return error::too_large;
                }
                auto next = ::uwvm2::utils::container::make_shared<output>();
                next->sequence = value->emitted_items + 1;
                next->type = policy.output_type;
                next->encoding = policy.encoding;
                next->bytes.assign(bytes.begin(), bytes.end());
                value->outputs.emplace_back(std::move(next));
                value->emitted_bytes += bytes.size();
                ++value->emitted_items;
                return error::success;
            }

            /** @brief Confirm actual provider completion and release input leases.
             * @details Call only once native users have stopped. Unknown outcome
             * is deliberately not accepted here; supervisor reconciliation is a
             * separate trusted operation. Optional final bytes consume the same
             * cumulative reservation and are marked terminal, never a command.
             */
            error finish(state outcome, std::span<std::uint8_t const> final_bytes = {})
            {
                if(!value) { return error::bad_handle; }
                if(!same_process(*value->origin->owner)) { return error::denied; }
                if(!terminal(outcome)) { return error::invalid; }
                if(final_bytes.size() > buffers::maximum_transfer) { return error::too_large; }
                if(!final_bytes.empty() && !value->origin->owner->bounds.validate_output(final_bytes, true)) { return error::invalid; }
                auto completion_digest = buffers::hash(final_bytes);
                std::lock_guard lock{value->mutex};
                if(value->cleanup_confirmed)
                {
                    return value->completed_as == outcome && value->completion_bytes == final_bytes.size() && value->completion_digest == completion_digest
                               ? error::success
                               : error::conflict;
                }
                if(value->phase != state::running && value->phase != state::outcome_unknown) { return error::conflict; }
                expire(*value);
                check_sources(*value);
                bool cancelled = cancellation_requested();
                if(!final_bytes.empty() && !cancelled)
                {
                    if(outcome != state::succeeded) { return error::invalid; }
                    if(final_bytes.size() > buffers::maximum_transfer || value->emitted_items >= value->reserved.items ||
                       final_bytes.size() > value->reserved.bytes - value->emitted_bytes)
                    {
                        return error::too_large;
                    }
                    auto next = ::uwvm2::utils::container::make_shared<output>();
                    next->sequence = value->emitted_items + 1;
                    next->terminal = true;
                    next->type = value->origin->owner->bounds.output_type;
                    next->encoding = value->origin->owner->bounds.encoding;
                    next->bytes.assign(final_bytes.begin(), final_bytes.end());
                    value->outputs.emplace_back(std::move(next));
                    value->emitted_bytes += final_bytes.size();
                    ++value->emitted_items;
                }
                value->completed_as = outcome;
                value->completion_digest = completion_digest;
                value->completion_bytes = final_bytes.size();
                value->phase = cancelled ? state::cancelled : outcome;
                value->cleanup_confirmed = true;
                value->inputs.reset();
                if(outcome == state::failed && value->why == reason::none) { value->why = reason::provider_failed; }
                return error::success;
            }
        };

        explicit registry(std::uint64_t (*trusted_clock)() noexcept = now) noexcept : clock{trusted_clock ? trusted_clock : now} {}

        registry(registry const&) = delete;
        registry& operator= (registry const&) = delete;

        ~registry() { retire(); }

        /** @brief Bind a disabled exact provider scope; attach is a separate trusted step. */
        error bind(::uwvm2::utils::container::std_string alias, plan policy)
        {
            if(!same_process()) { return error::denied; }
            if(alias.empty() || alias.size() > 255 || alias.find('\0') != alias.npos || !valid(policy)) { return error::invalid; }
            std::lock_guard lock{mutex};
            if(retired) { return error::denied; }
            if(bindings.contains(alias)) { return error::conflict; }
            bindings.emplace(std::move(alias), ::uwvm2::utils::container::make_shared<account>(std::move(policy), clock));
            return error::success;
        }

        /** @brief Attach an approved native worker with a strictly fresh generation. */
        error attach(std::string_view alias, std::uint64_t generation)
        {
            if(!same_process()) { return error::denied; }
            std::lock_guard lock{mutex};
            auto scope = binding(alias);
            if(!scope) { return error::denied; }
            if(!generation || generation <= scope->generation.load() || scope->connected.load()) { return error::conflict; }
            scope->generation.store(generation);
            scope->connected.store(true);
            return error::success;
        }

        /** @brief Publish one owned session only after successful result delivery. */
        template <typename Sink>
        error open(std::string_view alias, profile const& requested, std::uint64_t max_resources, Sink&& sink)
        {
            if(!same_process()) { return error::denied; }
            std::shared_ptr<session> value;
            {
                std::lock_guard lock{mutex};
                auto scope = binding(alias);
                if(!scope) { return error::denied; }
                if(requested != scope->bounds.model) { return error::denied; }
                if(!scope->connected.load()) { return error::unsupported; }
                if(scope->sessions.load() >= scope->bounds.sessions || scope->sessions.load() >= max_resources) { return error::again; }
                value = ::uwvm2::utils::container::make_shared<session>(scope);
                scope->sessions.fetch_add(1);
                value->id = resources::allocate_identity();
                if(!value->id) { return error::too_large; }
                value->generation = scope->generation.load();
                sessions.emplace(value->id, value);
            }
            bool delivered{};
            try
            {
                delivered = sink(value->id);
            }
            catch(...)
            {
                delivered = false;
            }
            std::lock_guard lock{mutex};
            auto scope = binding(alias);
            if(!delivered || scope != value->owner || !scope->connected.load() || scope->generation.load() != value->generation)
            {
                sessions.erase(value->id);
                return delivered ? error::cancelled : error::io;
            }
            value->published = true;
            return error::success;
        }

        /** @brief Recover an exact last submission before pinning inputs again.
         * @details Canonical request SHA256 includes every request field. Replay
         * exposes only the original admission receipt, never fresh execution
         * permission or source payload. A missing receipt preserves output.
         */
        error probe_retry(std::string_view alias, handle session_id, request_key key, digest const& request_hash, receipt& output, bool& found)
        {
            if(!same_process()) { return error::denied; }
            std::lock_guard lock{mutex};
            auto value = owned_session(alias, session_id);
            if(!value) { return error::bad_handle; }
            if(key.session != session_id || !key.sequence) { return error::invalid; }
            if(key.sequence == value->last_sequence)
            {
                if(request_hash != value->last_hash) { return error::conflict; }
                output = value->last_receipt;
                found = true;
                return error::success;
            }
            if(value->last_sequence == UINT64_MAX || key.sequence != value->last_sequence + 1) { return error::conflict; }
            found = false;
            return error::success;
        }

        /** @brief Reserve a complete job and move actual sealed input leases into it.
         * @details Parameters are validated before publication. On error the
         * caller's input vector is unchanged; after success it is moved into the
         * job. Exact replay is checked again under the publication lock. All
         * secondary authority must remain held across this operation.
         */
        error submit(std::string_view alias,
                     handle session_id,
                     request_key key,
                     digest const& request_hash,
                     profile const& task,
                     std::span<std::uint8_t const> parameters,
                     ::uwvm2::utils::container::std_vector<buffers::registry::sealed_lease>& inputs,
                     capacity requested,
                     std::uint64_t deadline,
                     receipt& output_receipt)
        {
            if(!same_process()) { return error::denied; }
            std::shared_ptr<session> origin;
            {
                std::lock_guard lock{mutex};
                origin = owned_session(alias, session_id);
                if(!origin) { return error::bad_handle; }
            }
            auto const& policy = origin->owner->bounds;
            if(task != policy.task || inputs.size() > policy.input_count || parameters.size() > policy.parameter_bytes ||
               parameters.size() > requested.workspace_bytes)
            {
                return error::invalid;
            }
            if(!policy.validate_parameters(parameters)) { return error::invalid; }
            std::uint64_t input_bytes{};
            std::array<input_evidence, 64> evidence{};
            std::size_t input_count{};
            for(auto const& lease: inputs)
            {
                auto status = lease.visit(
                    [&](auto bytes, auto const&, auto const& sha256)
                    {
                        if(bytes.size() > policy.input_bytes - input_bytes) { return false; }
                        input_bytes += bytes.size();
                        evidence[input_count++] = {lease.identity(), sha256};
                        return true;
                    });
                if(status != error::success) { return status; }
            }
            std::lock_guard lock{mutex};
            auto current = owned_session(alias, session_id);
            if(current != origin) { return error::cancelled; }
            auto scope = origin->owner;
            if(key.session != session_id || !key.sequence) { return error::invalid; }
            if(key.sequence == origin->last_sequence)
            {
                if(request_hash != origin->last_hash) { return error::conflict; }
                output_receipt = origin->last_receipt;
                return error::success;
            }
            if(origin->last_sequence == UINT64_MAX || key.sequence != origin->last_sequence + 1) { return error::conflict; }
            if(!scope->connected.load() || origin->generation != scope->generation.load()) { return error::cancelled; }
            auto time = scope->clock();
            if(deadline <= time || deadline - time > policy.deadline_ns) { return error::timeout; }
            if(scope->requests.load() >= policy.requests || input_bytes > policy.input_bytes - scope->inputs.load() ||
               requested.bytes > policy.output_bytes - scope->output_bytes.load() || requested.items > policy.output_items - scope->output_items.load() ||
               requested.workspace_bytes > policy.workspace_bytes - scope->workspace.load())
            {
                return error::again;
            }
            auto value = ::uwvm2::utils::container::make_shared<job>();
            auto bundle = ::uwvm2::utils::container::make_shared<input_bundle>();
            value->origin = origin;
            value->evidence = evidence;
            value->input_count = input_count;
            value->parameters.assign(parameters.begin(), parameters.end());
            value->id = resources::allocate_identity();
            if(!value->id) { return error::too_large; }
            value->generation = origin->generation;
            value->deadline = deadline;
            value->reserved = requested;
            jobs.emplace(value->id, value);
            scope->requests.fetch_add(1);
            scope->output_bytes.fetch_add(requested.bytes);
            scope->output_items.fetch_add(requested.items);
            scope->workspace.fetch_add(requested.workspace_bytes);
            value->charged = true;
            scope->inputs.fetch_add(input_bytes);
            bundle->owner = scope;
            bundle->bytes = input_bytes;
            bundle->values = std::move(inputs);
            value->inputs = std::move(bundle);
            origin->last_sequence = key.sequence;
            origin->last_hash = request_hash;
            origin->last_receipt = sample(*value);
            output_receipt = origin->last_receipt;
            return error::success;
        }

        /** @brief Host worker claims one queued request, without a guest callback. */
        error claim(std::string_view alias, std::uint64_t generation, work& output_work)
        {
            if(!same_process()) { return error::denied; }
            if(output_work) { return error::conflict; }
            std::lock_guard lock{mutex};
            auto scope = binding(alias);
            if(!scope || !scope->connected.load() || scope->generation.load() != generation) { return error::denied; }
            for(auto const& [id, value]: jobs)
            {
                static_cast<void>(id);
                if(value->origin->owner != scope || value->generation != generation) { continue; }
                std::lock_guard job_lock{value->mutex};
                expire(*value);
                check_sources(*value);
                if(value->phase != state::queued) { continue; }
                value->phase = state::running;
                output_work.value = value;
                return error::success;
            }
            return error::again;
        }

        /** @brief Copy one complete output before consuming it; failed sinks preserve it. */
        template <typename Sink>
        error take(std::string_view alias, handle session_id, handle request_id, Sink&& sink)
        {
            if(!same_process()) { return error::denied; }
            std::shared_ptr<job> value;
            {
                std::lock_guard lock{mutex};
                value = owned_job(alias, request_id);
                if(!value || owned_session(alias, session_id) != value->origin) { return error::bad_handle; }
            }
            std::shared_ptr<output const> front;
            receipt sampled;
            {
                std::lock_guard lock{value->mutex};
                expire(*value);
                if(value->delivering) { return error::again; }
                if(value->outputs.empty() && !terminal(value->phase) && value->phase != state::outcome_unknown) { return error::again; }
                value->delivering = true;
                if(!value->outputs.empty()) { front = value->outputs.front(); }
                sampled = sample(*value);
            }
            bool delivered{};
            try
            {
                delivered = sink(sampled, front.get());
            }
            catch(...)
            {
                delivered = false;
            }
            std::lock_guard lock{value->mutex};
            value->delivering = false;
            if(value->origin->owner->retired.load() || value->origin->closed.load()) { return error::cancelled; }
            if(!delivered) { return error::io; }
            if(front) { value->outputs.pop_front(); }
            return error::success;
        }

        /** @brief Acknowledge cancellation intent; running work stays charged until stopped. */
        error cancel_request(std::string_view alias, handle session_id, handle request_id, receipt& output_receipt)
        {
            if(!same_process()) { return error::denied; }
            std::shared_ptr<job> value;
            {
                std::lock_guard lock{mutex};
                value = owned_job(alias, request_id);
                if(!value || owned_session(alias, session_id) != value->origin) { return error::bad_handle; }
            }
            std::lock_guard lock{value->mutex};
            cancel(*value, reason::cancel_requested);
            output_receipt = sample(*value);
            return error::success;
        }

        /** @brief Read only the owned request receipt; no additional provider operation. */
        error request_describe(std::string_view alias, handle request_id, receipt& output_receipt)
        {
            if(!same_process()) { return error::denied; }
            std::shared_ptr<job> value;
            {
                std::lock_guard lock{mutex};
                value = owned_job(alias, request_id);
                if(!value) { return error::bad_handle; }
            }
            std::lock_guard lock{value->mutex};
            expire(*value);
            output_receipt = sample(*value);
            return error::success;
        }

        /** @brief Verify a proposal's captured inputs match the completed model job.
         * @details Host-only immutable evidence, exact order/count/identity/digest.
         * The caller separately retains real snapshot leases and current source
         * authority. Successful comparison neither revives released data nor
         * permits physical execution. This comparison is the immutable evidence
         * adoption point: an already released request fails; a later concurrent
         * release does not undo an adoption that completed beforehand. It is
         * not an atomic transaction spanning the proposal registry.
         */
        error check_inputs(std::string_view alias, handle request_id, std::span<input_evidence const> expected) const
        {
            if(!same_process()) { return error::denied; }
            std::lock_guard lock{mutex};
            auto value = owned_job(alias, request_id);
            if(!value) { return error::bad_handle; }
            std::lock_guard job_lock{value->mutex};
            if(value->phase != state::succeeded || !value->cleanup_confirmed) { return error::conflict; }
            if(expected.size() != value->input_count || !std::equal(expected.begin(), expected.end(), value->evidence.begin())) { return error::conflict; }
            return error::success;
        }

        /** @brief Authenticate one complete final output and its exact original inputs.
         * @details Only nonempty bytes supplied to successful work.finish qualify.
         * Streaming emit chunks, supervisor reconciliation without final bytes,
         * unfinished/failed/cancelled/unknown jobs and released receipts cannot
         * produce this evidence. Length and SHA256 remain available after take,
         * but are neither source authority nor permission to execute a proposal.
         * The host caller owns immutable final bytes throughout this comparison
         * and separately holds all current origin/source authority leases.
         * Closing the session blocks new adoption. Cancelling an already terminal
         * successful job does not rewrite its historical outcome; explicit scope
         * revocation and downstream proposal deadlines still fence new work.
         */
        error check_completion(std::string_view alias,handle request_id,std::span<input_evidence const> expected,
                               std::span<std::uint8_t const> final_bytes) const
        {
            if(!same_process())return error::denied;
            if(final_bytes.empty()||final_bytes.size()>buffers::maximum_transfer)return error::invalid;
            auto digest=buffers::hash(final_bytes);
            std::lock_guard lock{mutex};auto value=owned_job(alias,request_id);if(!value)return error::bad_handle;
            std::lock_guard job_lock{value->mutex};
            if(value->origin->closed.load()||value->phase!=state::succeeded||value->completed_as!=state::succeeded||!value->cleanup_confirmed||
               value->cancel_requested.load()||!value->completion_bytes||value->completion_bytes!=final_bytes.size()||value->completion_digest!=digest)
                return error::conflict;
            if(expected.size()!=value->input_count||!std::equal(expected.begin(),expected.end(),value->evidence.begin()))return error::conflict;
            return error::success;
        }

        /** @brief Explicitly retire a terminal, cleaned-up receipt and any remaining outputs. */
        error request_release(std::string_view alias, handle request_id)
        {
            if(!same_process()) { return error::denied; }
            std::lock_guard lock{mutex};
            auto value = owned_job(alias, request_id);
            if(!value) { return error::bad_handle; }
            std::lock_guard job_lock{value->mutex};
            if(!terminal(value->phase) || !value->cleanup_confirmed || value->delivering) { return error::again; }
            jobs.erase(request_id);
            return error::success;
        }

        /** @brief Describe the exact session model and supported task, without content. */
        error describe(std::string_view alias, handle session_id, profile& model, profile& task, bool& streaming)
        {
            if(!same_process()) { return error::denied; }
            std::lock_guard lock{mutex};
            auto value = owned_session(alias, session_id);
            if(!value) { return error::bad_handle; }
            model = value->owner->bounds.model;
            task = value->owner->bounds.task;
            streaming = value->owner->bounds.streaming;
            return error::success;
        }

        /** @brief Close one session; requests keep its quota until their real lifetime ends. */
        error close(std::string_view alias, handle session_id)
        {
            if(!same_process()) { return error::denied; }
            std::lock_guard lock{mutex};
            auto value = owned_session(alias, session_id);
            if(!value) { return error::bad_handle; }
            value->closed = true;
            sessions.erase(session_id);
            for(auto const& [id, pending]: jobs)
            {
                static_cast<void>(id);
                if(pending->origin == value)
                {
                    std::lock_guard job_lock{pending->mutex};
                    cancel(*pending, reason::cancel_requested);
                }
            }
            return error::success;
        }

        /** @brief Disconnect one worker generation without pretending running jobs stopped. */
        error detach(std::string_view alias, std::uint64_t generation)
        {
            if(!same_process()) { return error::denied; }
            std::lock_guard lock{mutex};
            auto scope = binding(alias);
            if(!scope) { return error::denied; }
            if(scope->generation.load() != generation || !scope->connected.load()) { return error::conflict; }
            scope->connected.store(false);
            for(auto const& [id, value]: jobs)
            {
                static_cast<void>(id);
                if(value->origin->owner != scope || value->generation != generation) { continue; }
                std::lock_guard job_lock{value->mutex};
                cancel(*value, reason::provider_lost);
                if(!terminal(value->phase))
                {
                    value->phase = state::outcome_unknown;
                    value->cleanup_confirmed = false;
                }
            }
            return error::success;
        }

        /** @brief Trusted supervisor settles unknown work only after actual cleanup proof.
         * @details The exact origin and provider generation must match. This
         * does not infer cleanup from a disconnect, timeout or token loss. It
         * remains available for a revoked scope so native teardown can drain it.
         */
        error reconcile(std::string_view alias, handle request_id, std::uint64_t generation, state outcome)
        {
            if(!same_process()) { return error::denied; }
            if(!terminal(outcome)) { return error::invalid; }
            std::lock_guard lock{mutex};
            auto owner = bindings.find(alias);
            auto found = jobs.find(request_id);
            if(owner == bindings.end() || found == jobs.end() || found->second->origin->owner != owner->second) { return error::bad_handle; }
            auto value = found->second;
            std::lock_guard job_lock{value->mutex};
            if(value->generation != generation) { return error::conflict; }
            if(value->phase != state::outcome_unknown || value->cleanup_confirmed) { return error::conflict; }
            value->phase = outcome;
            value->cleanup_confirmed = true;
            value->completed_as = outcome;
            value->completion_digest = buffers::hash({});
            value->completion_bytes = 0;
            value->inputs.reset();
            return error::success;
        }

        /** @brief Nonblocking alias-level counters; diagnostics never expire or claim work. */
        error try_inspect(std::string_view alias, usage& output_usage) const noexcept
        {
            if(!same_process()) { return error::denied; }
            std::unique_lock lock{mutex, std::try_to_lock};
            if(!lock.owns_lock()) { return error::again; }
            auto it = bindings.find(alias);
            if(it == bindings.end()) { return error::bad_handle; }
            auto const& value = *it->second;
            output_usage = {value.sessions.load(),
                            value.requests.load(),
                            value.inputs.load(),
                            value.output_bytes.load(),
                            value.output_items.load(),
                            value.workspace.load()};
            return error::success;
        }

        /** @brief Revoke new access while retaining unsettled input and request capacity. */
        void revoke(std::string_view alias)
        {
            if(!same_process()) { return; }
            std::lock_guard lock{mutex};
            auto owner = bindings.find(alias);
            if(owner == bindings.end()) { return; }
            owner->second->retired.store(true);
            owner->second->connected.store(false);
            for(auto const& [id, value]: jobs)
            {
                static_cast<void>(id);
                if(value->origin->owner == owner->second)
                {
                    std::lock_guard job_lock{value->mutex};
                    cancel(*value, reason::cancel_requested);
                }
            }
            for(auto const& [id, value]: sessions)
            {
                static_cast<void>(id);
                if(value->owner == owner->second) { value->closed.store(true); }
            }
        }

        /** @brief Retire before native worker cancellation/join and environment destruction. */
        void retire() noexcept
        {
            if(!same_process()) { return; }
            std::lock_guard lock{mutex};
            retired = true;
            for(auto const& [alias, value]: bindings)
            {
                static_cast<void>(alias);
                value->retired.store(true);
                value->connected.store(false);
            }
            for(auto const& [id, value]: jobs)
            {
                static_cast<void>(id);
                std::lock_guard job_lock{value->mutex};
                cancel(*value, reason::cancel_requested);
            }
            for(auto const& [id, value]: sessions)
            {
                static_cast<void>(id);
                value->closed.store(true);
            }
        }
    };
}  // namespace uwvm2::imported::ros::scene::models
