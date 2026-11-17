/** @file @brief Closed transform definitions, explicit zero limits and atomic aggregate budgets. */
#include <uwvm2/imported/ros/scene/transform_policy.h>
#include <cassert>
#include <iostream>
namespace s=uwvm2::imported::ros::scene;
namespace t=s::transforms;
using t::error;
using bytes=uwvm2::utils::container::std_vector<std::uint8_t>;
static bytes definition(bool cycle=false)
{
    std::array<std::uint8_t,1024> bytes{};s::wire::writer out{bytes};
    assert(out.text(t::definition_magic)&&out.text("frames")&&out.u64(2)&&out.text("base")&&out.text("map")&&out.u64(cycle?2:1));
    assert(out.u64(1)&&out.u64(0)&&out.boolean(true)&&out.boolean(false));
    if(cycle)assert(out.u64(0)&&out.u64(1)&&out.boolean(true)&&out.boolean(false));
    assert(out.f64(1000)&&out.f64(1e-12)&&out.f64(10)&&out.f64(5));
    return {out.written().begin(),out.written().end()};
}
static s::buffers::profile profile(s::wire::bytes definition)
{s::buffers::profile value;assert(value.identity.assign("frames"));value.sha256=s::buffers::hash(definition);return value;}
static void limits(t::configuration& value,std::size_t skip=SIZE_MAX)
{
    constexpr std::array<std::uint64_t,t::limit_names.size()> numbers{16,8,8,10000,0,1000,4,1000,2,1000000,1000000,1000000,65536};
    for(std::size_t i{};i<numbers.size();++i)if(i!=skip)assert(value.limit(t::limit_names[i],numbers[i])==error::success);
}
static t::configuration configured()
{t::configuration value;auto bytes=definition();assert(value.profile(profile(bytes))==error::success&&value.definition(bytes)==error::success);limits(value);return value;}
static void all_fields_and_digest()
{
    t::configuration value;auto bytes=definition();assert(value.definition(bytes)==error::denied&&!value.limits());
    auto p=profile(bytes);p.sha256[0]^=1;assert(value.profile(p)==error::success&&value.definition(bytes)==error::conflict&&!value.frozen());
    value={};assert(value.profile(profile(bytes))==error::success&&value.profile(profile(bytes))==error::conflict);
    assert(value.definition(bytes)==error::success&&value.definition(bytes)==error::conflict);
    limits(value,4);assert(value.freeze()==error::invalid&&!value.frozen());
    assert(value.limit("publish.future-ns",0)==error::success&&value.limit("publish.future-ns",0)==error::conflict);
    assert(value.freeze()==error::success&&value.frozen()&&value.limits()&&value.limits()->session.history.future_ns==0);
    assert(value.freeze()==error::conflict&&value.limit("sessions",1)==error::denied&&value.definition(bytes)==error::denied&&value.profile(p)==error::denied);
    // Limits may precede the immutable definition without being erased.
    t::configuration ordered;limits(ordered);assert(ordered.profile(profile(bytes))==error::success&&ordered.definition(bytes)==error::success&&ordered.freeze()==error::success);
    assert(ordered.limits()->session.history.max_samples==16&&ordered.limits()->session.history.max_batch==8);
}
static void malformed_and_cyclic_records()
{
    auto complete=definition();
    for(std::size_t n{};n<complete.size();++n)
    {
        s::wire::bytes bytes{complete.data(),n};t::configuration value;assert(value.profile(profile(bytes))==error::success);
        assert(value.definition(bytes)==error::invalid&&!value.frozen());
    }
    complete.push_back(0);t::configuration trailing;assert(trailing.profile(profile(complete))==error::success&&trailing.definition(complete)==error::invalid);
    auto loop=definition(true);t::configuration cyclic;assert(cyclic.profile(profile(loop))==error::success&&cyclic.definition(loop)==error::success);limits(cyclic);
    assert(cyclic.freeze()==error::invalid&&!cyclic.frozen());
    std::array<std::uint8_t,t::maximum_definition_bytes+1> oversized{};t::configuration large;assert(large.profile(profile(oversized))==error::success);
    assert(large.definition(oversized)==error::too_large);
}
static void no_unlimited_or_implicit_capacity()
{
    t::configuration value;assert(value.limit("unknown",0)==error::invalid&&value.limit("sessions",UINT64_MAX)==error::too_large);
    assert(value.limit("sessions",0)==error::success);
    auto complete=definition();assert(value.profile(profile(complete))==error::success&&value.definition(complete)==error::success);limits(value,8);
    assert(value.freeze()==error::success&&value.limits()->sessions==0);
    auto empty=profile(complete);empty.identity={};t::configuration bad;assert(bad.profile(empty)==error::invalid);
    auto narrow=configured();assert(narrow.freeze()==error::success);auto p=*narrow.limits();
    assert(p.session.history.data_bytes&&p.session.request_bytes&&p.session.history.data_bytes==p.session.history.workspace_bytes);
    p.data_bytes=p.session.history.data_bytes*p.sessions-1;assert(!t::valid(p));
}
static void aggregate_no_partial_update()
{
    auto value=configured();assert(value.freeze()==error::success);auto const& p=*value.limits();
    t::budget maximum{p.sessions,p.sessions*p.session.requests,p.data_bytes,p.workspace_bytes,p.request_bytes,p.carrier_bytes},used{};
    assert(t::accumulate(maximum,p,used)&&used==maximum);auto before=used;
    assert(!t::accumulate(maximum,p,used)&&used==before);
    used={};auto partial=maximum;partial.carrier_bytes--;
    assert(!t::accumulate(partial,p,used)&&used==t::budget{});
    partial=maximum;partial.sessions=UINT64_MAX;assert(!t::accumulate(partial,p,used)&&used==t::budget{});
}
int main()
{
    all_fields_and_digest();malformed_and_cyclic_records();no_unlimited_or_implicit_capacity();aggregate_no_partial_update();
    std::cout<<"passed 4 transform policy security groups\n";
}
