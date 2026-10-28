#include <uwvm2/runtime/debug_audit/stream.h>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <thread>

namespace audit = uwvm2::runtime::debug_audit;
static unsigned checks{}, cases{};
#define CHECK(x) do { ++checks; if(!(x)) { std::fprintf(stderr, "FAIL line %d: %s\n", __LINE__, #x); std::exit(1); } } while(false)
static audit::principal owner(std::uint64_t instance = 1)
{ audit::principal p{}; p.wasm_sha256.fill(0x31); p.instance=instance; p.generation=2; p.policy_generation=3; p.trust_domain=instance; return p; }
static audit::authorization authority()
{ return {owner(),owner(),7,9,audit::bit(audit::capability::observe_calls)|audit::bit(audit::capability::observe_writes)}; }
static audit::event call()
{ audit::event e{}; e.location={0,2,0,13}; e.operation=1; return e; }
static audit::event write()
{ auto e=call(); e.category=audit::feature::store_scalar; e.outcome=audit::phase::committed; e.causal_call=1; e.memory={41,1,0,65528,8,65536}; return e; }
static constexpr auto requested=audit::calls|audit::writes;
template<std::size_t Capacity> static void drain(audit::stream<Capacity>& s)
{ audit::record r{}; while(s.peek(r)) { CHECK(s.acknowledge(r.sequence,true)); } }

int main()
{
    {
        ++cases; auto a=authority();
        CHECK(audit::authorize(a,owner(),requested,requested)==audit::admission::accepted);
        a.subject=owner(2); // Same Wasm SHA is still another principal.
        CHECK(audit::authorize(a,owner(),requested,requested)==audit::admission::stale_owner);
        CHECK(audit::authorize(a,owner(2),requested,requested)==audit::admission::missing_capability);
        a.granted|=audit::bit(audit::capability::observe_other_instance);
        CHECK(audit::authorize(a,owner(2),requested,requested)==audit::admission::accepted);
        a.subject.policy_generation++;
        CHECK(audit::authorize(a,owner(2),requested,requested)==audit::admission::stale_owner);
        a=authority(); a.subject.generation++;
        CHECK(audit::authorize(a,owner(),requested,requested)==audit::admission::stale_owner);
    }
    {
        ++cases; auto a=authority();
        for(auto c : {audit::capability::read_values,audit::capability::write_memory,audit::capability::evaluate,audit::capability::replace_code})
        { a.granted=audit::bit(c); CHECK(audit::authorize(a,owner(),requested,requested)==audit::admission::missing_capability); }
        a=authority();
        CHECK(audit::authorize(a,owner(),requested,audit::calls)==audit::admission::unsupported_coverage);
        CHECK(audit::authorize(a,owner(),0,requested)==audit::admission::unsupported_coverage);
        CHECK(audit::authorize(a,owner(),1ull<<63,~0ull)==audit::admission::unsupported_coverage);
        a.session=0; CHECK(audit::authorize(a,owner(),requested,requested)==audit::admission::invalid_identity);
    }
    {
        ++cases; auto e=write(); CHECK(audit::valid(e));
        e.memory.offset++; CHECK(!audit::valid(e));
        e.outcome=audit::phase::attempted; CHECK(audit::valid(e));
        e.outcome=audit::phase::trapped; CHECK(audit::valid(e));
        e.memory.offset=UINT64_MAX-3; e.memory.length=8; e.memory.size_bytes=UINT64_MAX;
        e.outcome=audit::phase::committed; CHECK(!audit::valid(e));
        e.memory.offset=UINT64_MAX; e.memory.length=0;
        CHECK(!audit::valid(e)); e.outcome=audit::phase::no_write; CHECK(!audit::valid(e));
        e.category=audit::feature::memory_copy; CHECK(audit::valid(e));
        e.memory.identity=0; CHECK(!audit::valid(e));
    }
    {
        ++cases; auto e=write();
        e.category=audit::feature::compare_exchange; e.outcome=audit::phase::no_write; CHECK(audit::valid(e));
        ++e.memory.offset; CHECK(!audit::valid(e)); --e.memory.offset;
        e.outcome=audit::phase::committed; CHECK(audit::valid(e));
        e.category=audit::feature::store_scalar; e.outcome=audit::phase::no_write; CHECK(!audit::valid(e));
        e.category=audit::feature::memory_growth; CHECK(audit::valid(e));
        e.category=audit::feature::memory_copy; e.outcome=audit::phase::returned; CHECK(!audit::valid(e));
        e.memory.length=0; e.memory.offset=e.memory.size_bytes+1;
        e.outcome=audit::phase::no_write; CHECK(!audit::valid(e));
        e=write(); e.memory.length=3; CHECK(!audit::valid(e));
        e.category=audit::feature::store_simd; e.memory.offset=65520; e.memory.length=16; CHECK(audit::valid(e));
        e.category=audit::feature::store_atomic; CHECK(!audit::valid(e));
        e.memory.length=8; ++e.memory.offset; CHECK(!audit::valid(e));
        e.outcome=audit::phase::trapped; CHECK(audit::valid(e));
    }
    {
        ++cases; audit::stream<8> s(authority(),owner(),requested,requested,1,1,audit::pressure_policy::fail_recording);
        auto e=call();
        for(auto p : {audit::phase::attempted,audit::phase::entered,audit::phase::returned})
        { e.outcome=p; CHECK(s.append(e)==audit::append_result::recorded); }
        e=write(); CHECK(s.append(e)==audit::append_result::recorded);
        CHECK(!s.report().transport_complete); s.seal(); CHECK(!s.report().transport_complete);
        audit::record r{}; CHECK(s.peek(r) && r.sequence==1 && r.value.outcome==audit::phase::attempted);
        CHECK(!s.acknowledge(2,true)); drain(s);
        auto report=s.report(); CHECK(report.transport_complete && report.attempted==4 && report.delivered==4);
        CHECK(report.owner==owner() && report.stream==1 && report.thread==1 && report.session==7);
        CHECK(s.append(call())==audit::append_result::sealed);
    }
    {
        ++cases; audit::stream<2> s(authority(),owner(),requested,requested,2,1,audit::pressure_policy::drop_new);
        CHECK(s.append(call())==audit::append_result::recorded); CHECK(s.append(write())==audit::append_result::recorded);
        CHECK(s.append(call())==audit::append_result::dropped); drain(s);
        CHECK(s.append(call())==audit::append_result::recorded);
        audit::record r{}; CHECK(s.peek(r) && r.sequence==4); drain(s); s.seal();
        auto report=s.report(); CHECK(!report.transport_complete && report.dropped==1 && report.recorded==3 && report.attempted==4);
        CHECK(report.failures==static_cast<std::uint32_t>(audit::failure::overflow));
    }
    {
        ++cases; audit::stream<1> s(authority(),owner(),requested,requested,3,1,audit::pressure_policy::fail_recording);
        CHECK(s.append(call())==audit::append_result::recorded); CHECK(s.append(write())==audit::append_result::failed);
        drain(s); CHECK(s.append(call())==audit::append_result::failed); s.seal();
        CHECK(s.report().dropped==2 && !s.report().transport_complete);
    }
    {
        ++cases; audit::stream<2> s(authority(),owner(),requested,requested,4,1,audit::pressure_policy::drop_new);
        CHECK(s.append(write())==audit::append_result::recorded);
        CHECK(s.acknowledge(1,false)); CHECK(s.append(write())==audit::append_result::failed); s.seal();
        auto report=s.report(); CHECK(report.sink_failed==1 && report.delivered==0 && report.dropped==1 && !report.transport_complete);
    }
    {
        ++cases; audit::stream<2> s(authority(),owner(),audit::calls,audit::calls,5,1,audit::pressure_policy::fail_recording);
        CHECK(s.append(write())==audit::append_result::invalid); CHECK(s.append(call())==audit::append_result::failed);
        s.seal(); CHECK(s.report().invalid==1 && !s.report().transport_complete);
    }
    {
        ++cases; auto a=authority(); a.subject=owner(2);
        audit::stream<2> s(a,owner(),requested,requested,6,1,audit::pressure_policy::fail_recording);
        CHECK(s.admitted()==audit::admission::stale_owner); CHECK(s.append(call())==audit::append_result::invalid);
        s.seal(); CHECK(!s.report().transport_complete);
        audit::stream<2> bad(authority(),owner(),requested,requested,0,1,audit::pressure_policy::fail_recording);
        CHECK(bad.admitted()==audit::admission::invalid_stream);
        audit::stream<2> policy(authority(),owner(),requested,requested,6,1,static_cast<audit::pressure_policy>(99));
        CHECK(policy.admitted()==audit::admission::invalid_policy && policy.append(call())==audit::append_result::invalid);
    }
    {
        ++cases; audit::stream<2> s(authority(),owner(),requested,requested,7,1,audit::pressure_policy::drop_new);
        CHECK(s.append(call())==audit::append_result::recorded); s.revoke(); drain(s);
        CHECK(s.append(write())==audit::append_result::sealed && !s.report().transport_complete);
    }
    {
        ++cases; auto e=call(); e.outcome=audit::phase::tail_replaced;
        CHECK(!audit::valid(e)); e.category=audit::feature::call_tail; CHECK(!audit::valid(e));
        e.related_operation=2; CHECK(audit::valid(e));
        e.outcome=audit::phase::exceptional_exit; CHECK(audit::valid(e));
        e.outcome=audit::phase::committed; CHECK(!audit::valid(e));
    }
    {
        ++cases; auto a=authority(); a.granted|=audit::bit(audit::capability::observe_host);
        CHECK(audit::authorize(a,owner(),audit::host_operations,audit::host_operations)==audit::admission::accepted);
        CHECK(audit::authorize(a,owner(),audit::other_mutations,audit::known_features)==audit::admission::missing_capability);
        auto e=call(); e.category=audit::feature::wasi_operation; e.outcome=audit::phase::completed;
        e.actual_count=7; e.resource=4; e.resource_generation=2; CHECK(audit::valid(e));
        e.outcome=audit::phase::entered; CHECK(!audit::valid(e));
    }
    {
        // Real native concurrent producer and sink. Every iteration may drop;
        // compare exact recorded/lost/consumed accounting after producer join.
        ++cases; audit::stream<32> s(authority(),owner(),requested,requested,8,19,audit::pressure_policy::drop_new);
        constexpr std::uint64_t count=100000;
        std::thread producer([&] {
            for(std::uint64_t i=0;i<count;++i) { auto e=call(); e.operation=i+1; (void)s.append(e); }
            s.seal();
        });
        std::uint64_t received{}, last{};
        for(;;)
        {
            audit::record r{};
            if(s.peek(r))
            {
                CHECK(r.sequence>last && r.sequence==r.value.operation); last=r.sequence;
                CHECK(s.acknowledge(r.sequence,true)); ++received;
            }
            else if(s.report().drained) { break; }
            else { std::this_thread::yield(); }
        }
        producer.join(); auto report=s.report();
        CHECK(report.attempted==count && report.recorded==received && report.delivered==received);
        CHECK(report.recorded+report.dropped==count && report.invalid==0);
        CHECK(report.transport_complete==(report.dropped==0));
    }
    std::printf("{\"schema\":\"debug-audit-component-v1\",\"passed\":true,\"cases\":%u,\"assertions\":%u,\"scope\":\"native transport and authorization contract only; no VM hooks\"}\n",cases,checks);
}
