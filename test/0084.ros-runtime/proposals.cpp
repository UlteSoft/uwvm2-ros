/** @file @brief Production proposal/model/sealed-snapshot integration safety. */
#include <uwvm2/imported/ros/scene/proposal_codec.h>
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
namespace p = uwvm2::imported::ros::scene::proposal_admission;
namespace q = uwvm2::imported::ros::scene::proposals;
namespace c = uwvm2::imported::ros::proposal_contract;
namespace w = uwvm2::imported::ros::scene::wire;
namespace s = uwvm2::imported::ros::scene::schema_contract;
using b::error;
using p::result;
static std::atomic<std::uint64_t> time_now{1000};

static std::uint64_t clock_now() noexcept { return time_now.load(); }
#if defined(UWVM_PROPOSAL_STORAGE_FAULT_INJECTION)
static bool fail_allocation{};
extern "C" void* __real_malloc(std::size_t);

extern "C" void* __wrap_malloc(std::size_t size)
{
    if(fail_allocation)
    {
        fail_allocation = false;
        return nullptr;
    }
    return __real_malloc(size);
}
#endif
static constexpr std::array<std::uint8_t, 4> data{1, 2, 3, 4};
static constexpr std::array<std::uint8_t, 1> parameters{7}, arguments{9};

static b::profile profile(std::string_view name)
{
    b::profile value;
    assert(value.identity.assign(name));
    value.sha256.fill(0x43);
    return value;
}

static b::plan buffer_plan()
{
    b::plan value;
    value.schema = profile("tensor");
    value.objects = 8;
    value.bytes = 32;
    value.object_bytes = value.transfer_bytes = 4;
    value.workspace_bytes = 32;
    value.leases = 16;
    value.elements = 4;
    value.rank = 1;
    value.element_mask = value.order_mask = value.color_mask = 1;
    return value;
}

static m::plan model_plan()
{
    m::plan value;
    value.model = profile("brain");
    value.task = profile("predict");
    assert(value.parameter_type.assign("parameters") && value.output_type.assign("result") && value.encoding.assign("bytes"));
    value.sessions = 1;
    value.requests = 4;
    value.input_bytes = 32;
    value.input_count = 8;
    value.parameter_bytes = 4;
    value.output_bytes = 32;
    value.output_items = 8;
    value.workspace_bytes = 64;
    value.deadline_ns = 1000;
    value.validate_parameters = [](auto value) noexcept { return value.size() == 1 && value[0] == 7; };
    value.validate_output = [](auto value, bool) noexcept { return value.size() <= 4; };
    return value;
}

static q::tool_profile tool(p::observation_policy policy)
{
    q::tool_profile value;
    value.profile = profile("approved.tool");
    value.schema = profile("tool.schema");
    assert(value.argument_type.assign("tool.arguments") && value.encoding.assign("bytes"));
    auto& selected = value.semantics;
    selected.profile = 11;
    selected.schema = 12;
    selected.target_capability = 13;
    selected.target = 14;
    selected.provider_generation = 15;
    selected.frame = 16;
    selected.operation = c::target_action::device_submit;
    selected.max_observation_age_ns = 200;
    selected.effect = p::effect_class::physical;
    selected.target_authority_epoch = 17;
    selected.physical_scope = 18;
    selected.controller_lease_epoch = 19;
    selected.minimum_observations = 1;
    selected.observations = policy;
    value.validate_arguments = [](auto bytes) noexcept { return bytes.size() == 1 && bytes[0] == 9; };
    return value;
}

static p::grant arbiter(std::optional<std::uint64_t> rate = {})
{
    p::grant value;
    value.actor = 200;
    value.capability = 201;
    value.proposal_capability = 101;
    value.target_capability = 13;
    value.authority_epoch = 202;
    value.target_actions = c::all_actions;
    value.read = value.accept = true;
    value.calls = rate;
    value.window_ns = 100;
    return value;
}

struct fixture
{
    b::registry buffers;
    m::registry models{clock_now};
    q::provider proposals{clock_now};
    b::handle input{}, session{}, request{};
    p::identity source{}, selected{}, reader{};
    p::observation metadata;
    q::provider::snapshot_token captured;

    explicit fixture(p::observation_policy policy = p::observation_policy::bounded_age, std::optional<std::uint64_t> rate = {})
    {
        time_now = 1000;
        assert(buffers.bind("sensor-data", buffer_plan()) == error::success);
        assert(models.bind("brain", model_plan()) == error::success && models.attach("brain", 10) == error::success);
        assert(models.open("brain",
                           profile("brain"),
                           1,
                           [&](auto value)
                           {
                               session = value;
                               return true;
                           }) == error::success);
        assert(proposals.configure("proposal", "brain", {4, 64, 2, 4, 4, 4, 1000, 500}, 100, 101) == result::success);
        assert(proposals.bind_tool(tool(policy), selected) == result::success && proposals.bind_reader(arbiter(rate), 201, reader) == result::success);
        input = create();
        metadata = {21, input, 23, 24, 25, 26, 16, 27, 1000, 10, 30, 31, 32};
        b::registry::sealed_lease lease;
        assert(buffers.pin("sensor-data", input, lease) == error::success);
        assert(proposals.bind_source(metadata, "robot.base", lease, source) == result::success && !lease);
        assert(proposals.capture(source, captured) == result::success);
        ::uwvm2::utils::container::std_vector<b::registry::sealed_lease> inputs(1);
        assert(buffers.pin("sensor-data", input, inputs[0]) == error::success);
        m::receipt receipt;
        assert(models.submit("brain", session, {session, 1}, b::hash(parameters), profile("predict"), parameters, inputs, {4, 1, 8}, 1800, receipt) ==
               error::success);
        request = receipt.request;
        m::registry::work work;
        assert(models.claim("brain", 10, work) == error::success);
        assert(work.finish(m::state::succeeded, data) == error::success);
    }

    b::handle create()
    {
        b::layout format;
        format.schema = profile("tensor");
        format.shape[0] = format.bytes = 4;
        format.strides[0] = 1;
        format.rank = 1;
        b::handle id{};
        assert(buffers.create("sensor-data",
                              format,
                              8,
                              [&](auto value)
                              {
                                  id = value;
                                  return true;
                              }) == error::success);
        std::uint64_t count{};
        assert(buffers.write("sensor-data", id, 0, data, count) == error::success && buffers.seal("sensor-data", id, b::hash(data)) == error::success);
        return id;
    }

    b::handle advance()
    {
        auto next = create();
        ++time_now;
        metadata.buffer = next;
        ++metadata.sample_sequence;
        ++metadata.source_ns;
        metadata.captured_steady_ns = time_now.load();
        b::registry::sealed_lease lease;
        assert(buffers.pin("sensor-data", next, lease) == error::success);
        assert(proposals.update_source(source, metadata, "robot.base", lease) == result::success && !lease);
        return next;
    }

    p::identity publish()
    {
        p::identity id{};
        std::array tokens{captured};
        assert(proposals.publish(models, request, selected, arguments, tokens, 1500, id) == result::success);
        return id;
    }

    p::accept_request accept_request(p::identity id, std::uint64_t sequence = 1)
    {
        return {
            {reader, sequence},
            101,
            id,
            13,
            14,
            1400
        };
    }

    result accept(p::identity id, p::receipt& receipt)
    {
        return proposals.accept("proposal", 200, accept_request(id), {101, 13}, [](auto const&, auto const&) { return result::success; }, receipt);
    }

    b::usage used()
    {
        b::usage value;
        assert(buffers.inspect("sensor-data", value) == error::success);
        return value;
    }
};

static void exact_alias_zero_and_ids()
{
    q::provider empty{clock_now};
    p::identity unchanged = 77;
    assert(empty.bind_tool(tool(p::observation_policy::bounded_age), unchanged) == result::denied && unchanged == 77);
    assert(empty.configure("proposal", "brain", {}, 100, 101) == result::success);
    assert(empty.bind_reader(arbiter(), 201, unchanged) == result::again);
    fixture f;
    auto id = f.publish();
    assert(f.reader == arbiter().capability);  // guest knows its arbiter cap; it need not guess a private reader ID
    std::array all{f.input, f.session, f.request, f.source, f.selected, id};
    for(std::size_t i{}; i != all.size(); ++i)
    {
        for(std::size_t j{}; j != i; ++j) { assert(all[i] != all[j] && all[i]); }
    }
    assert(f.proposals.read("wrong",
                            200,
                            f.reader,
                            id,
                            [](auto const&, auto const&, auto)
                            {
                                assert(false);
                                return true;
                            }) == result::denied);
    p::receipt receipt;
    assert(f.proposals.accept(
               "proposal",
               100,
               f.accept_request(id),
               {101, 13},
               [](auto const&, auto const&)
               {
                   assert(false);
                   return result::success;
               },
               receipt) == result::denied);
    fixture zero{p::observation_policy::bounded_age, 0};
    auto zero_id = zero.publish();
    assert(zero.accept(zero_id, receipt) == result::again);
    q::usage inspected;
    assert(f.proposals.try_inspect("wrong", inspected) == result::denied);
    assert(f.proposals.try_inspect("proposal", inspected) == result::success && inspected.proposals == 1 && inspected.source_slots == 1 &&
           inspected.reader_slots == 1 && inspected.tool_slots == 1 && inspected.snapshot_references == 2 && !inspected.retired);
    time_now = 9999;
    q::usage after;
    assert(f.proposals.try_inspect("proposal", after) == result::success && after.proposals == inspected.proposals &&
           after.snapshot_references == inspected.snapshot_references);
    f.proposals.retire();
    assert(f.proposals.try_inspect("proposal", after) == result::success && after.retired);
}

static void slow_model_snapshot_and_real_lifetime()
{
    fixture f;
    auto new_input = f.advance();
    assert(f.buffers.close("sensor-data", f.input) == error::success && f.used().bytes == 8);
    auto id = f.publish();
    f.captured = {};
    assert(f.proposals.read("proposal",
                            200,
                            f.reader,
                            id,
                            [&](auto const& record, auto const&, auto inputs)
                            {
                                assert(record.observations[0].buffer == f.input && record.observations[0].sample_sequence == 10 &&
                                       inputs[0]->metadata.source_ns == 27);
                                assert(f.proposals.release("proposal", id) == result::again);
                                return false;
                            }) == result::delivery_failed);
    assert(f.used().bytes == 8);
    p::receipt receipt;
    assert(f.accept(id, receipt) == result::success);
    assert(f.proposals.release("proposal", id) == result::success && f.used().bytes == 4);
    auto previous = receipt;
    assert(f.accept(id, receipt) == result::success && receipt == previous);                     // historical replay survives explicit release
    assert(f.buffers.close("sensor-data", new_input) == error::success && f.used().bytes == 4);  // current source pin still charged
    fixture latest{p::observation_policy::latest_required};
    latest.advance();
    std::array tokens{latest.captured};
    p::identity unchanged = 77;
    assert(latest.proposals.publish(latest.models, latest.request, latest.selected, arguments, tokens, 1500, unchanged) == result::conflict && unchanged == 77);
}

static void exact_model_pairing_and_fences()
{
    fixture f;
    f.advance();
    q::provider::snapshot_token fresh;
    assert(f.proposals.capture(f.source, fresh) == result::success);
    std::array tokens{fresh};
    p::identity unchanged = 77;
    assert(f.proposals.publish(f.models, f.request, f.selected, arguments, tokens, 1500, unchanged) == result::conflict && unchanged == 77);
    fixture other;
    tokens[0] = other.captured;
    assert(f.proposals.publish(f.models, f.request, f.selected, arguments, tokens, 1500, unchanged) == result::denied);
    auto id = f.publish();
    p::receipt receipt;
    assert(f.proposals.accept(
               "proposal",
               200,
               f.accept_request(id),
               {101, 13},
               [&](auto const&, auto const&)
               {
                   assert(f.proposals.fence_target(f.selected, 16, 17, 19) == result::success);
                   return result::success;
               },
               receipt) == result::conflict);
    fixture source;
    auto source_id = source.publish();
    assert(source.proposals.accept(
               "proposal",
               200,
               source.accept_request(source_id),
               {101, 13},
               [&](auto const&, auto const&)
               {
                   source.buffers.revoke("sensor-data");
                   return result::success;
               },
               receipt) == result::denied);
    fixture authority;
    auto auth_id = authority.publish();
    assert(authority.proposals.fence_source_authority(authority.source, 33) == result::success);
    assert(authority.proposals.capture(authority.source, fresh) == result::denied && authority.accept(auth_id, receipt) == result::conflict);
    fixture expired;
    auto expiry_id = expired.publish();
    time_now = 1201;
    assert(expired.accept(expiry_id, receipt) == result::timeout);
}

static void canonical_records_and_concurrency()
{
    fixture f;
    auto id = f.publish();
    std::array<std::uint8_t, 2048> bytes{};
    assert(f.proposals.read("proposal",
                            200,
                            f.reader,
                            id,
                            [&](auto const& record, auto const& tool, auto inputs)
                            {
                                w::writer encoded{bytes};
                                assert(c::encode(encoded, record, tool, inputs));
                                assert(s::validate(encoded.written(), s::type::proposal_read__result, {bytes.size(), 256, 128, 16}));
                                auto measure = w::writer::measuring(bytes.size());
                                assert(c::encode(measure, record, tool, inputs) && measure.size() == encoded.size());
                                return true;
                            }) == result::success);
    w::writer request{bytes};
    auto expected = f.accept_request(id);
    assert(request.u64(expected.key.session) && request.u64(expected.key.sequence) && request.u64(expected.proposal_capability) &&
           request.u64(expected.proposal) && request.u64(expected.target_capability) && request.u64(expected.target) && request.u64(expected.deadline));
    p::accept_request decoded;
    assert(c::decode(c::operation::accept, request.written(), decoded) && decoded == expected);
    for(std::size_t n{}; n != request.size(); ++n) { assert(!c::decode(c::operation::accept, {bytes.data(), n}, decoded)); }
    std::barrier validating{2};
    result accepted{};
    p::receipt receipt;
    std::thread first{[&]
                      {
                          accepted = f.proposals.accept(
                              "proposal",
                              200,
                              expected,
                              {101, 13},
                              [&](auto const&, auto const&)
                              {
                                  validating.arrive_and_wait();
                                  validating.arrive_and_wait();
                                  return result::success;
                              },
                              receipt);
                      }};
    validating.arrive_and_wait();
    p::receipt untouched;
    assert(f.accept(id, untouched) == result::again && f.proposals.release("proposal", id) == result::again);
    validating.arrive_and_wait();
    first.join();
    assert(accepted == result::success);
    w::writer encoded{bytes};
    assert(c::encode_receipt(encoded, receipt) && s::validate(encoded.written(), s::type::proposal_accept__result, {bytes.size(), 64, 32, 8}));
    auto child = ::fork();
    assert(child >= 0);
    if(!child) { ::_exit(f.accept(id, untouched) == result::denied ? 0 : 1); }
    int status{};
    assert(::waitpid(child, &status, 0) == child && WIFEXITED(status) && !WEXITSTATUS(status));
}

static void allocation_rollback_and_invalid_schema()
{
#if defined(UWVM_PROPOSAL_STORAGE_FAULT_INJECTION)
    fixture f;
    std::array tokens{f.captured};
    p::identity unchanged = 77;
    bool threw{};
    fail_allocation = true;
    try
    {
        static_cast<void>(f.proposals.publish(f.models, f.request, f.selected, arguments, tokens, 1500, unchanged));
    }
    catch(std::bad_alloc const&)
    {
        threw = true;
    }
    assert(threw && !fail_allocation && unchanged == 77);
    auto id = f.publish();
    assert(id);
    auto input = f.create();
    auto next = f.metadata;
    next.buffer = input;
    next.sample_sequence = 11;
    b::registry::sealed_lease lease;
    assert(f.buffers.pin("sensor-data", input, lease) == error::success);
    threw = false;
    fail_allocation = true;
    try
    {
        static_cast<void>(f.proposals.update_source(f.source, next, "robot.base", lease));
    }
    catch(std::bad_alloc const&)
    {
        threw = true;
    }
    assert(threw && !fail_allocation && lease);
    q::provider::snapshot_token original;
    assert(f.proposals.capture(f.source, original) == result::success && original.buffer() == f.input);
    std::array<std::uint8_t, 1> invalid{8};
    assert(f.proposals.publish(f.models, f.request, f.selected, invalid, tokens, 1500, unchanged) == result::invalid);
    next.calibration_generation = 0;
    assert(f.proposals.update_source(f.source, next, "robot.base", lease) == result::invalid && lease);
#else
    assert(false && "Qualification requires allocation fault injection");
#endif
}

static void trusted_scope_mapping_and_raw_retries()
{
    fixture f;
    auto id = f.publish();
    auto request = f.accept_request(id);
    request.proposal_capability = 901;
    request.target_capability = 902;
    p::receipt receipt;
    auto validator = [](auto const&, auto const&) { return result::success; };
    assert(f.proposals.accept("proposal", 200, request, {}, validator, receipt) == result::denied);
    assert(f.proposals.accept("proposal", 200, request, {999, 13}, validator, receipt) == result::denied);
    assert(f.proposals.accept("proposal", 200, request, {101, 999}, validator, receipt) == result::denied);
    assert(f.proposals.accept("proposal", 200, request, {101, 13}, validator, receipt) == result::success);
    auto previous = receipt;
    assert(f.proposals.accept("proposal", 200, request, {101, 13}, validator, receipt) == result::success && receipt == previous);
    ++request.proposal_capability;
    assert(f.proposals.accept("proposal", 200, request, {101, 13}, validator, receipt) == result::conflict);
    request = f.accept_request(id);
    request.key.session = 999;
    assert(f.proposals.accept("proposal", 200, request, {101, 13}, validator, receipt) == result::denied);
    for(auto policy: {p::observation_policy::bounded_age, p::observation_policy::latest_required})
    {
        fixture changed{policy};
        auto proposal = changed.publish();
        auto status = changed.proposals.accept(
            "proposal",
            200,
            changed.accept_request(proposal),
            {101, 13},
            [&](auto const&, auto const&)
            {
                changed.advance();
                return result::success;
            },
            receipt);
        assert(status == (policy == p::observation_policy::bounded_age ? result::success : result::conflict));
    }
    fixture rate;
    auto proposal = rate.publish();
    assert(rate.proposals.accept(
               "proposal",
               200,
               rate.accept_request(proposal),
               {101, 13},
               [&](auto const&, auto const&)
               {
                   assert(rate.proposals.attenuate_reader(rate.reader, 0, 100, 203) == result::success);
                   return result::success;
               },
               receipt) == result::conflict);
    assert(rate.accept(proposal, receipt) == result::again);
}

int main()
{
    exact_alias_zero_and_ids();
    slow_model_snapshot_and_real_lifetime();
    exact_model_pairing_and_fences();
    canonical_records_and_concurrency();
    allocation_rollback_and_invalid_schema();
    trusted_scope_mapping_and_raw_retries();
    std::cout << "passed 6 production proposal safety groups\n";
}
