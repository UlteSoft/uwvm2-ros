/** @file @brief Test actual sealed-input model jobs and confirmed cleanup accounting. */
#include <uwvm2/imported/ros/scene/model_jobs.h>
#include <uwvm2/imported/ros/scene/model_contract.h>
#include <uwvm2/imported/ros/scene/schema_contract.h>
#include <array>
#include <atomic>
#include <cassert>
#include <iostream>
#include <thread>
#include <barrier>
#include <sys/wait.h>
#include <unistd.h>

namespace b = uwvm2::imported::ros::scene::buffers;
namespace m = uwvm2::imported::ros::scene::models;
using b::error;
#if defined(UWVM_MODEL_FAULT_INJECTION)
// Test-only linker boundary: fail one real strict allocator call, then let
// exception construction and cleanup allocate normally. Production has no hook.
static std::size_t allocation_countdown{};
static bool allocation_failed{};
extern "C" void* __real_malloc(std::size_t);

extern "C" void* __wrap_malloc(std::size_t size)
{
    if(allocation_countdown && !--allocation_countdown)
    {
        allocation_failed = true;
        return nullptr;
    }
    return __real_malloc(size);
}
#endif
static std::atomic<std::uint64_t> clock_time{100};

static std::uint64_t clock_now() noexcept { return clock_time.load(); }

static b::profile profile(std::string_view name)
{
    b::profile value;
    assert(value.identity.assign(name));
    value.sha256.fill(0x42);
    return value;
}

static b::plan buffer_plan()
{
    b::plan value;
    value.schema = profile("tensor");
    value.objects = 4;
    value.bytes = 16;
    value.object_bytes = 4;
    value.transfer_bytes = 4;
    value.workspace_bytes = 16;
    value.leases = 8;
    value.elements = 4;
    value.rank = 1;
    value.element_mask = 1;
    value.order_mask = 1;
    value.color_mask = 1;
    return value;
}

static m::plan model_plan()
{
    m::plan value;
    value.model = profile("model.identity");
    value.task = profile("task.identity");
    assert(value.parameter_type.assign("identity.parameters") && value.output_type.assign("identity.result") && value.encoding.assign("bytes"));
    value.sessions = 1;
    value.requests = 2;
    value.input_bytes = 8;
    value.input_count = 2;
    value.parameter_bytes = 8;
    value.output_bytes = 16;
    value.output_items = 4;
    value.workspace_bytes = 16;
    value.deadline_ns = 100;
    value.streaming = true;
    value.validate_parameters = [](std::span<std::uint8_t const> bytes) noexcept { return bytes.size() == 1 && bytes[0] == 7; };
    value.validate_output = [](std::span<std::uint8_t const> bytes, bool) noexcept { return bytes.size() <= 4; };
    return value;
}

static constexpr std::array<std::uint8_t, 4> input_bytes{1, 2, 3, 4};
static constexpr std::array<std::uint8_t, 1> parameters{7};
using leases = ::uwvm2::utils::container::std_vector<b::registry::sealed_lease>;

struct fixture
{
    b::registry buffers;
    m::registry models{clock_now};
    b::handle input{}, session{};

    fixture()
    {
        clock_time = 100;
        assert(buffers.bind("data", buffer_plan()) == error::success);
        assert(models.bind("brain", model_plan()) == error::success);
        b::layout format;
        format.schema = profile("tensor");
        format.shape[0] = format.bytes = 4;
        format.strides[0] = 1;
        format.rank = 1;
        assert(buffers.create("data",
                              format,
                              4,
                              [&](auto id)
                              {
                                  input = id;
                                  return true;
                              }) == error::success);
        std::uint64_t count{};
        assert(buffers.write("data", input, 0, input_bytes, count) == error::success);
        assert(buffers.seal("data", input, b::hash(input_bytes)) == error::success);
        assert(models.attach("brain", 10) == error::success);
        assert(models.open("brain",
                           profile("model.identity"),
                           1,
                           [&](auto id)
                           {
                               session = id;
                               return true;
                           }) == error::success);
    }

    leases pin()
    {
        leases value;
        value.resize(1);
        assert(buffers.pin("data", input, value[0]) == error::success);
        return value;
    }

    m::receipt submit(std::uint64_t sequence = 1)
    {
        auto inputs = pin();
        m::receipt receipt;
        assert(
            models.submit("brain", session, {session, sequence}, b::hash(parameters), profile("task.identity"), parameters, inputs, {8, 2, 8}, 150, receipt) ==
            error::success);
        assert(inputs.empty() && receipt.phase == m::state::queued);
        return receipt;
    }

    m::usage used()
    {
        m::usage value;
        assert(models.try_inspect("brain", value) == error::success);
        return value;
    }

    b::usage buffer_used()
    {
        b::usage value;
        assert(buffers.inspect("data", value) == error::success);
        return value;
    }
};

static void zero_scope_and_exact_profiles()
{
    m::registry registry{clock_now};
    auto plan = model_plan();
    assert(registry.open("brain",
                         plan.model,
                         1,
                         [](auto)
                         {
                             assert(false);
                             return true;
                         }) == error::denied);
    assert(registry.bind("brain", plan) == error::success);
    assert(registry.open("brain",
                         plan.model,
                         1,
                         [](auto)
                         {
                             assert(false);
                             return true;
                         }) == error::unsupported);
    assert(registry.attach("brain", 0) == error::conflict);
    assert(registry.attach("brain", 1) == error::success);
    assert(registry.open("brain",
                         profile("wrong"),
                         1,
                         [](auto)
                         {
                             assert(false);
                             return true;
                         }) == error::denied);
    assert(registry.open("brain",
                         plan.model,
                         0,
                         [](auto)
                         {
                             assert(false);
                             return true;
                         }) == error::again);
    assert(registry.open("brain", plan.model, 1, [](auto) { return false; }) == error::io);
    m::usage used;
    assert(registry.try_inspect("brain", used) == error::success && !used.sessions);
    plan = model_plan();
    plan.validate_output = nullptr;
    assert(!m::valid(plan));
    plan = model_plan();
    plan.input_count = 65;
    assert(!m::valid(plan));
    plan = model_plan();
    plan.sessions = plan.requests = plan.input_bytes = plan.output_bytes = plan.output_items = plan.workspace_bytes = 0;
    m::registry zero{clock_now};
    assert(zero.bind("zero", plan) == error::success && zero.attach("zero", 1) == error::success);
    assert(zero.open("zero",
                     plan.model,
                     1,
                     [](auto)
                     {
                         assert(false);
                         return true;
                     }) == error::again);
}

static void sealed_input_and_real_worker_result()
{
    fixture f;
    auto admitted = f.submit();
    assert(f.used().inputs == 4 && f.used().requests == 1 && f.used().output_bytes == 8);
    assert(f.buffers.close("data", f.input) == error::success && f.buffer_used().bytes == 4);
    m::registry::work work;
    assert(f.models.claim("brain", 10, work) == error::success);
    std::array<std::uint8_t, 4> result{};
    assert(work.with_parameters([](auto bytes) { return bytes.size() == 1 && bytes[0] == 7; }) == error::success);
    assert(work.with_input(0,
                           [&](auto bytes, auto const&, auto const&)
                           {
                               for(std::size_t i{}; i != 4; ++i) { result[i] = bytes[i] * 2; }
                               return true;
                           }) == error::success);
    std::array<std::uint8_t, 2> partial{9, 8};
    assert(work.emit(partial) == error::success);
    assert(work.finish(m::state::succeeded, result) == error::success);
    assert(work.finish(m::state::succeeded, result) == error::success);
    assert(work.finish(m::state::succeeded, partial) == error::conflict);
    assert(f.used().inputs == 0 && f.buffer_used().bytes == 0 && f.buffer_used().leases == 0);
    std::array<m::input_evidence, 1> evidence{{{f.input, b::hash(input_bytes)}}};
    assert(f.models.check_inputs("brain", admitted.request, evidence) == error::success);
    evidence[0].sha256[0] ^= 1;
    assert(f.models.check_inputs("brain", admitted.request, evidence) == error::conflict);
    assert(f.models.check_inputs("brain", admitted.request, {}) == error::conflict);
    assert(f.models.take("brain",
                         f.session,
                         admitted.request,
                         [](auto receipt, auto chunk)
                         {
                             assert(receipt.cleanup_confirmed && chunk->sequence == 1);
                             return false;
                         }) == error::io);
    assert(f.models.take("brain",
                         f.session,
                         admitted.request,
                         [&](auto receipt, auto chunk)
                         {
                             assert(receipt.phase == m::state::succeeded && chunk->sequence == 1 && !chunk->terminal && chunk->bytes[0] == 9);
                             assert(f.models.take("brain",
                                                  f.session,
                                                  admitted.request,
                                                  [](auto, auto)
                                                  {
                                                      assert(false);
                                                      return true;
                                                  }) == error::again);
                             assert(f.models.request_release("brain", admitted.request) == error::again);
                             return true;
                         }) == error::success);
    assert(f.models.take("brain",
                         f.session,
                         admitted.request,
                         [](auto, auto chunk) { return chunk && chunk->terminal && chunk->sequence == 2 && chunk->bytes[3] == 8; }) == error::success);
    assert(f.models.take("brain", f.session, admitted.request, [](auto receipt, auto chunk) { return !chunk && receipt.cleanup_confirmed; }) == error::success);
    assert(f.models.request_release("brain", admitted.request) == error::success);
    assert(f.used().requests == 1);
    work = {};
    assert(f.used().requests == 0 && f.used().workspace == 0 && f.used().output_bytes == 0);
    assert(f.models.close("brain", f.session) == error::success && f.used().sessions == 0);
}

static void retries_and_failed_admission()
{
    fixture f;
    auto admitted = f.submit();
    m::receipt repeated{};
    bool found{};
    assert(f.models.probe_retry("brain", f.session, {f.session, 1}, b::hash(parameters), repeated, found) == error::success && found &&
           repeated.request == admitted.request);
    auto wrong = b::hash(parameters);
    wrong[0] ^= 1;
    assert(f.models.probe_retry("brain", f.session, {f.session, 1}, wrong, repeated, found) == error::conflict);
    assert(f.models.probe_retry("brain", f.session, {f.session, 3}, wrong, repeated, found) == error::conflict);
    auto inputs = f.pin();
    std::array<std::uint8_t, 1> bad{8};
    assert(f.models.submit("brain", f.session, {f.session, 2}, wrong, profile("task.identity"), bad, inputs, {8, 2, 8}, 150, repeated) == error::invalid &&
           !inputs.empty());
    assert(f.models.submit("brain", f.session, {f.session, 2}, wrong, profile("wrong"), parameters, inputs, {8, 2, 8}, 150, repeated) == error::invalid);
    assert(f.models.submit("brain", f.session, {f.session, 2}, wrong, profile("task.identity"), parameters, inputs, {UINT64_MAX, 2, 8}, 150, repeated) ==
           error::again);
    assert(f.models.submit("brain", f.session, {f.session, 2}, wrong, profile("task.identity"), parameters, inputs, {8, 2, 8}, UINT64_MAX, repeated) ==
           error::timeout);
    assert(f.used().requests == 1 && f.used().inputs == 4);
    inputs.clear();
    f.buffers.revoke("data");
    assert(f.models.probe_retry("brain", f.session, {f.session, 1}, b::hash(parameters), repeated, found) == error::success &&
           found);  // receipt recovery needs no new source pin
}

static void cancellation_deadline_and_cleanup()
{
    for(bool running: {false, true})
    {
        fixture f;
        auto admitted = f.submit();
        m::registry::work work;
        if(running) { assert(f.models.claim("brain", 10, work) == error::success); }
        m::receipt receipt;
        assert(f.models.cancel_request("brain", f.session, admitted.request, receipt) == error::success);
        assert(receipt.cleanup_confirmed == !running && receipt.phase == (running ? m::state::running : m::state::cancelled));
        if(running)
        {
            assert(f.used().inputs == 4 && f.models.request_release("brain", admitted.request) == error::again);
            assert(work.cancellation_requested() && work.emit(parameters) == error::cancelled);
            assert(work.finish(m::state::succeeded, input_bytes) == error::success);  // confirmed stopped, result discarded
            assert(f.models.request_describe("brain", admitted.request, receipt) == error::success && receipt.phase == m::state::cancelled &&
                   receipt.cleanup_confirmed);
        }
        assert(f.used().inputs == 0 && f.models.request_release("brain", admitted.request) == error::success);
        work = {};
        assert(f.used().requests == 0);
    }
    fixture f;
    auto admitted = f.submit();
    m::registry::work work;
    assert(f.models.claim("brain", 10, work) == error::success);
    clock_time = 150;
    m::receipt receipt;
    assert(f.models.request_describe("brain", admitted.request, receipt) == error::success && receipt.phase == m::state::running &&
           !receipt.cleanup_confirmed && receipt.why == m::reason::deadline);
    assert(work.finish(m::state::succeeded, input_bytes) == error::success);
    assert(f.models.take("brain",
                         f.session,
                         admitted.request,
                         [](auto receipt, auto chunk)
                         { return !chunk && receipt.phase == m::state::cancelled && receipt.cleanup_confirmed; }) == error::success);
}

static void unknown_outcome_and_provider_fencing()
{
    fixture f;
    auto admitted = f.submit();
    {
        m::registry::work work;
        assert(f.models.claim("brain", 10, work) == error::success);
    }  // worker abandoned without proof of stop
    m::receipt receipt;
    assert(f.models.request_describe("brain", admitted.request, receipt) == error::success && receipt.phase == m::state::outcome_unknown &&
           !receipt.cleanup_confirmed);
    assert(f.used().inputs == 4 && f.models.request_release("brain", admitted.request) == error::again);
    assert(f.models.reconcile("brain", admitted.request, 9, m::state::failed) == error::conflict);
    assert(f.models.reconcile("brain", admitted.request, 10, m::state::failed) == error::success && f.used().inputs == 0);
    assert(f.models.request_release("brain", admitted.request) == error::success);
    auto second = f.submit(2);
    m::registry::work work;
    assert(f.models.claim("brain", 10, work) == error::success);
    assert(f.models.detach("brain", 10) == error::success);
    assert(f.models.request_describe("brain", second.request, receipt) == error::success && receipt.phase == m::state::outcome_unknown);
    assert(work.with_input(0,
                           [](auto, auto const&, auto const&)
                           {
                               assert(false);
                               return true;
                           }) == error::cancelled);
    assert(f.models.attach("brain", 10) == error::conflict && f.models.attach("brain", 11) == error::success);
    auto inputs = f.pin();
    assert(f.models.submit("brain", f.session, {f.session, 3}, b::hash(parameters), profile("task.identity"), parameters, inputs, {8, 2, 8}, 150, receipt) ==
           error::cancelled);
    assert(f.models.reconcile("brain", second.request, 10, m::state::cancelled) == error::success);
    assert(f.models.request_release("brain", second.request) == error::success);
    work = {};
    inputs.clear();
    assert(f.models.close("brain", f.session) == error::success);
}

static void source_revocation_close_and_fork()
{
    {
        fixture queued;
        auto admitted = queued.submit();
        queued.buffers.revoke("data");
        m::registry::work work;
        assert(queued.models.claim("brain", 10, work) == error::again);
        m::receipt receipt;
        assert(queued.models.request_describe("brain", admitted.request, receipt) == error::success);
        assert(receipt.phase == m::state::cancelled && receipt.cleanup_confirmed && receipt.why == m::reason::source_revoked && queued.used().inputs == 0);
    }
    for(bool emit: {false, true})
    {
        fixture changed;
        auto admitted = changed.submit();
        m::registry::work work;
        assert(changed.models.claim("brain", 10, work) == error::success);
        assert(work.with_input(0, [](auto, auto const&, auto const&) { return true; }) == error::success);
        changed.buffers.revoke("data");
        if(emit) { assert(work.emit(parameters) == error::cancelled); }
        assert(work.finish(m::state::succeeded, input_bytes) == error::success);
        m::receipt receipt;
        assert(changed.models.request_describe("brain", admitted.request, receipt) == error::success);
        assert(receipt.phase == m::state::cancelled && receipt.why == m::reason::source_revoked && !receipt.had_output && receipt.cleanup_confirmed);
    }
    fixture f;
    auto admitted = f.submit();
    m::registry::work work;
    assert(f.models.claim("brain", 10, work) == error::success);
    f.buffers.revoke("data");
    assert(work.with_input(0,
                           [](auto, auto const&, auto const&)
                           {
                               assert(false);
                               return true;
                           }) == error::denied);
    assert(work.cancellation_requested() && f.used().inputs == 4);
    assert(work.finish(m::state::cancelled) == error::success && f.used().inputs == 0);
    assert(f.models.close("brain", f.session) == error::success && f.used().sessions == 1);
    assert(f.models.take("brain",
                         f.session,
                         admitted.request,
                         [](auto, auto)
                         {
                             assert(false);
                             return true;
                         }) == error::bad_handle);
    assert(f.models.request_release("brain", admitted.request) == error::success);
    work = {};
    assert(f.used().sessions == 0);
    fixture forked;
    forked.submit();
    m::registry::work inherited;
    assert(forked.models.claim("brain", 10, inherited) == error::success);
    auto child = ::fork();
    assert(child >= 0);
    if(!child)
    {
        auto status = inherited.emit(parameters);
        ::_exit(status == error::denied ? 0 : 1);
    }
    int status{};
    assert(::waitpid(child, &status, 0) == child && WIFEXITED(status) && WEXITSTATUS(status) == 0);
    forked.models.revoke("brain");
    assert(inherited.cancellation_requested());
    assert(inherited.finish(m::state::cancelled) == error::success);
}

static void concurrent_reservation_and_delivery()
{
    fixture f;
    std::array<m::receipt, 2> admitted{};
    std::barrier begin{3};
    std::array<leases, 2> pins{f.pin(), f.pin()};
    std::array<error, 2> statuses{};
    std::array<std::thread, 2> workers;
    for(std::size_t i{}; i != 2; ++i)
    {
        workers[i] = std::thread{[&, i]
                                 {
                                     begin.arrive_and_wait();
                                     statuses[i] = f.models.submit("brain",
                                                                   f.session,
                                                                   {f.session, 1},
                                                                   b::hash(parameters),
                                                                   profile("task.identity"),
                                                                   parameters,
                                                                   pins[i],
                                                                   {8, 2, 8},
                                                                   150,
                                                                   admitted[i]);
                                 }};
    }
    begin.arrive_and_wait();
    for(auto& worker: workers) { worker.join(); }
    assert(statuses[0] == error::success && statuses[1] == error::success && admitted[0].request == admitted[1].request);
    assert(f.used().requests == 1 && f.used().inputs == 4);
    pins[0].clear();
    pins[1].clear();
    m::registry::work work;
    assert(f.models.claim("brain", 10, work) == error::success && work.emit(parameters) == error::success);
    std::barrier copying{2};
    error delivery{};
    std::thread first{[&]
                      {
                          delivery = f.models.take("brain",
                                                   f.session,
                                                   admitted[0].request,
                                                   [&](auto, auto chunk)
                                                   {
                                                       assert(chunk && chunk->sequence == 1);
                                                       copying.arrive_and_wait();
                                                       copying.arrive_and_wait();
                                                       return false;
                                                   });
                      }};
    copying.arrive_and_wait();
    assert(f.models.take("brain",
                         f.session,
                         admitted[0].request,
                         [](auto, auto)
                         {
                             assert(false);
                             return true;
                         }) == error::again);
    copying.arrive_and_wait();
    first.join();
    assert(delivery == error::io);
    assert(f.models.take("brain", f.session, admitted[0].request, [](auto, auto chunk) { return chunk && chunk->sequence == 1; }) == error::success);
    assert(work.emit(parameters) == error::success);
    assert(f.models.take("brain", f.session, admitted[0].request, [](auto, auto) { return true; }) == error::success);
    assert(work.emit(parameters) == error::too_large);  // take never refills total emission allowance
    assert(work.finish(m::state::cancelled) == error::success);
}

static void carrier_workspace_and_revocation()
{
    fixture f;
    m::registry::workspace_lease carrier, other;
    m::plan copied;
    assert(f.models.limits("brain", copied) == error::success && copied.workspace_bytes == 16);
    assert(f.models.reserve_workspace("missing", 1, carrier) == error::denied);
    assert(f.models.reserve_workspace("brain", 0, carrier) == error::invalid);
    assert(f.models.reserve_workspace("brain", UINT64_MAX, carrier) != error::success);
    assert(f.models.reserve_workspace("brain", 9, carrier) == error::success && carrier.live() && carrier.reserved_bytes() == 9);
    auto inputs = f.pin();
    m::receipt receipt;
    assert(f.models.submit("brain", f.session, {f.session, 1}, b::hash(parameters), profile("task.identity"), parameters, inputs, {8, 2, 8}, 150, receipt) ==
           error::again);
    assert(f.used().workspace == 9 && f.used().requests == 0 && !inputs.empty());
    assert(f.models.reserve_workspace("brain", 8, other) == error::again);
    other = std::move(carrier);
    assert(!carrier && other.live());
    other.reset();
    assert(f.models.submit("brain", f.session, {f.session, 1}, b::hash(parameters), profile("task.identity"), parameters, inputs, {8, 2, 8}, 150, receipt) ==
           error::success);
    assert(f.models.reserve_workspace("brain", 8, carrier) == error::success && f.used().workspace == 16);
    auto child = ::fork();
    assert(child >= 0);
    if(!child) { ::_exit(carrier.live() ? 1 : 0); }
    int status{};
    assert(::waitpid(child, &status, 0) == child && WIFEXITED(status) && !WEXITSTATUS(status));
    f.models.revoke("brain");
    assert(!carrier.live() && f.used().workspace == 16);
    carrier.reset();
    assert(f.used().workspace == 8);
}

static void canonical_model_records()
{
    namespace c = uwvm2::imported::ros::model_contract;
    namespace w = uwvm2::imported::ros::scene::wire;
    namespace s = uwvm2::imported::ros::scene::schema_contract;
    std::array<std::uint8_t, 4096> bytes{};
    w::writer writer{bytes};
    assert(writer.u64(1) && writer.u64(1) && writer.u64(1) && c::write_profile(writer, profile("task.identity")) && writer.text("identity.parameters") &&
           writer.text("bytes") && writer.blob(parameters) && writer.u64(1) && writer.u64(3) && writer.u64(4) && writer.u64(0) && writer.u64(0) &&
           writer.boolean(false) && writer.boolean(false) && writer.u64(8) && writer.u64(2) && writer.u64(8) && writer.u64(150));
    auto size = writer.size();
    c::request decoded;
    assert(s::validate(writer.written(), s::type::model_submit__request, {size, 256, 128, 16}));
    assert(c::decode(c::operation::submit, writer.written(), decoded) && decoded.input_count == 1 && decoded.inputs[0].buffer == 4 &&
           decoded.reservation.items == 2);
    for(std::size_t truncated{}; truncated != size; ++truncated) { assert(!c::decode(c::operation::submit, {bytes.data(), truncated}, decoded)); }
    assert(!c::decode(c::operation::submit, {bytes.data(), size + 1}, decoded));
    fixture f;
    auto admitted = f.submit();
    m::registry::work work;
    assert(f.models.claim("brain", 10, work) == error::success);
    assert(work.finish(m::state::succeeded, input_bytes) == error::success);
    assert(f.models.take("brain",
                         f.session,
                         admitted.request,
                         [&](auto receipt, auto chunk)
                         {
                             auto measure = w::writer::measuring(bytes.size());
                             assert(c::encode_take(measure, f.session, receipt, chunk));
                             w::writer result{bytes};
                             assert(c::encode_take(result, f.session, receipt, chunk) && result.size() == measure.size());
                             assert(s::validate(result.written(), s::type::model_take__result, {bytes.size(), 256, 128, 16}));
                             for(std::size_t n{}; n != result.size(); ++n)
                             {
                                 w::writer short_output{
                                     {bytes.data(), n}
                                 };
                                 assert(!c::encode_take(short_output, f.session, receipt, chunk));
                             }
                             return true;
                         }) == error::success);
    for(auto phase: {m::state::queued, m::state::running, m::state::succeeded, m::state::failed, m::state::cancelled, m::state::outcome_unknown})
    {
        m::receipt receipt{1, phase, 10, m::reason::provider_lost, false, true};
        w::writer result{bytes};
        assert(c::encode_receipt(result, receipt) && s::validate(result.written(), s::type::model_cancel__result, {bytes.size(), 64, 32, 8}));
    }
    w::writer description{bytes};
    assert(c::encode_description(description, profile("model.identity"), profile("task.identity"), true));
    assert(s::validate(description.written(), s::type::model_describe__result, {bytes.size(), 64, 32, 8}));
}

static void allocator_rollback()
{
#if defined(UWVM_MODEL_FAULT_INJECTION)
    unsigned failures{};
    for(unsigned nth = 1; nth != 17; ++nth)
    {
        fixture f;
        auto inputs = f.pin();
        m::receipt receipt;
        bool threw{};
        allocation_failed = false;
        allocation_countdown = nth;
        try
        {
            static_cast<void>(
                f.models
                    .submit("brain", f.session, {f.session, 1}, b::hash(parameters), profile("task.identity"), parameters, inputs, {8, 2, 8}, 150, receipt));
        }
        catch(std::bad_alloc const&)
        {
            threw = true;
        }
        allocation_countdown = 0;
        assert(threw == allocation_failed);
        if(threw)
        {
            ++failures;
            assert(!inputs.empty() && f.used().requests == 0 && f.used().inputs == 0 && f.used().workspace == 0 && f.buffer_used().leases == 1);
            bool found = true;
            assert(f.models.probe_retry("brain", f.session, {f.session, 1}, b::hash(parameters), receipt, found) == error::success && !found);
            assert(
                f.models
                    .submit("brain", f.session, {f.session, 1}, b::hash(parameters), profile("task.identity"), parameters, inputs, {8, 2, 8}, 150, receipt) ==
                error::success);
        }
        m::registry::work work;
        assert(f.models.claim("brain", 10, work) == error::success);
        allocation_failed = false;
        allocation_countdown = 1;
        threw = false;
        try
        {
            static_cast<void>(work.emit(parameters));
        }
        catch(std::bad_alloc const&)
        {
            threw = true;
        }
        allocation_countdown = 0;
        assert(threw && allocation_failed);
        assert(work.emit(parameters) == error::success);
        assert(f.models.take("brain", f.session, receipt.request, [](auto, auto chunk) { return chunk && chunk->sequence == 1; }) == error::success);
        assert(work.finish(m::state::succeeded) == error::success);
    }
    assert(failures >= 4);
#else
    assert(false && "Qualification requires linker allocation fault injection");
#endif
}

static void native_scratch_subreservations()
{
    fixture f;
    auto admitted = f.submit();
    m::registry::work work;
    assert(f.models.claim("brain", 10, work) == error::success);
    m::registry::scratch_lease first, second;
    assert(work.input_count() == 1 && work.deadline() == 150);
    assert(work.reserve_scratch(0, first) == error::invalid && work.reserve_scratch(8, first) == error::again);
    assert(work.reserve_scratch(7, first) == error::success && first.live() && first.reserved_bytes() == 7);
    assert(work.reserve_scratch(1, second) == error::again && work.reserve_scratch(1, first) == error::invalid);
    auto child = ::fork();
    assert(child >= 0);
    if(!child) { ::_exit(first.live() ? 1 : 0); }
    int status{};
    assert(::waitpid(child, &status, 0) == child && WIFEXITED(status) && !WEXITSTATUS(status));
    second = std::move(first);
    assert(!first && second.live());
    second.reset();
    std::array<m::registry::scratch_lease, 2> leases;
    std::array<error, 2> results{};
    std::barrier begin{3};
    std::array<std::thread, 2> threads;
    for(std::size_t i{}; i != 2; ++i)
    {
        threads[i] = std::thread{[&, i]
                                 {
                                     begin.arrive_and_wait();
                                     results[i] = work.reserve_scratch(7, leases[i]);
                                 }};
    }
    begin.arrive_and_wait();
    for(auto& thread: threads) { thread.join(); }
    assert((results[0] == error::success && results[1] == error::again) || (results[1] == error::success && results[0] == error::again));
    assert(work.finish(m::state::succeeded) == error::success && f.models.request_release("brain", admitted.request) == error::success);
    assert(work.reserve_scratch(1, first) == error::cancelled);
    work = {};
    assert(f.used().requests == 1 && f.used().workspace == 8);
    leases[0].reset();
    leases[1].reset();
    assert(f.used().requests == 0 && f.used().workspace == 0);
}

int main()
{
    zero_scope_and_exact_profiles();
    sealed_input_and_real_worker_result();
    retries_and_failed_admission();
    cancellation_deadline_and_cleanup();
    unknown_outcome_and_provider_fencing();
    source_revocation_close_and_fork();
    concurrent_reservation_and_delivery();
    carrier_workspace_and_revocation();
    canonical_model_records();
    allocator_rollback();
    native_scratch_subreservations();
    std::cout << "passed 11 model job safety groups\n";
}
