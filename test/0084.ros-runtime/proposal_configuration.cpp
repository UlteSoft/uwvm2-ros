/** @file @brief Closed, SHA-bound proposal rules and fully explicit finite budgets. */
#pragma push_macro("main")
#undef main
#define main proposal_dispatch_fixture_main
#include "proposal_dispatch.cpp"
#pragma pop_macro("main")
static std::vector<std::uint8_t> definition(p::plan const& policy)
{
    std::array<std::uint8_t,32768> buffer{};s::wire::writer out{buffer};
    assert(out.text(p::definition_magic)&&out.text(policy.selected.identity.view())&&out.text(policy.model_alias.view())&&p::write_profile(out,policy.model)&&p::write_profile(out,policy.task)&&out.u64(policy.source_count));
    for(std::size_t index{};index<policy.source_count;++index)
    {auto const& v=policy.sources[index];assert(out.text(v.alias.view())&&p::write_profile(out,v.profile)&&out.text(v.buffer_alias.view())&&p::write_profile(out,v.buffer_profile)&&out.text(v.frame.view()));}
    assert(out.u64(policy.tool_count));
    for(std::size_t index{};index<policy.tool_count;++index)
    {auto const& v=policy.tools[index];assert(p::write_profile(out,v.profile)&&p::write_profile(out,v.schema)&&p::write_profile(out,v.target_profile)&&out.text(v.target_alias.view())&&out.text(pc::target_names[static_cast<std::size_t>(v.action)])&&out.text(v.effect==a::effect_class::physical?"physical":"non_physical")&&out.text(v.observations==a::observation_policy::latest_required?"latest_required":"bounded_age")&&out.text(v.argument_type.view())&&out.text(v.encoding.view())&&out.text(v.frame.view())&&out.u64(v.max_observation_age_ns)&&out.u64(v.minimum_observations));}
    return {out.written().begin(),out.written().end()};
}
static void configure_limits(p::configuration& cfg,int omit=-1)
{
    constexpr std::array<std::uint64_t,11> values{4,4096,1,8,1,1,1000,1000,4,65536,16};
    for(std::size_t i{};i<values.size();++i)if(static_cast<int>(i)!=omit)assert(cfg.limit(p::configuration::limit_names[i],values[i])==error::success);
}
static p::configuration configured(std::vector<std::uint8_t> const& wire,int omit=-1)
{
    auto policy=proposal_plan();policy.selected.sha256=s::buffers::hash(wire);p::configuration cfg;
    assert(cfg.profile(policy.selected,policy.model_alias.view(),policy.model,policy.task)==error::success);configure_limits(cfg,omit);return cfg;
}
static void canonical_definition_and_required_limits()
{
    auto policy=proposal_plan();auto wire=definition(policy);auto cfg=configured(wire);p::plan output;
    assert(cfg.definition(wire)==error::success&&cfg.freeze(output)==error::success&&p::valid(output));
    assert(output.model==policy.model&&output.source_count==1&&output.tool_count==1&&output.tools[0].target_alias.view()=="motor"&&output.selected.sha256==s::buffers::hash(wire));
    assert(cfg.freeze(output)==error::conflict&&cfg.definition(wire)==error::conflict&&cfg.limit("proposals",1)==error::conflict);
    for(int missing=0;missing<11;++missing)
    {auto incomplete=configured(wire,missing);assert(incomplete.definition(wire)==error::success&&incomplete.freeze(output)==error::invalid);}
    auto wrong=configured(wire);auto changed=wire;changed.back()^=1;assert(wrong.definition(changed)==error::conflict);
    auto duplicate=configured(wire);assert(duplicate.limit("proposals",4)==error::conflict&&duplicate.limit("unknown",1)==error::invalid);
    auto reverse=configured(wire,0);assert(reverse.definition(wire)==error::success&&reverse.limit("proposals",0)==error::success&&reverse.freeze(output)==error::success&&output.storage.proposals==0);
}
static void malformed_canonical_even_with_matching_digest()
{
    auto valid=definition(proposal_plan());
    for(std::size_t bytes=0;bytes<valid.size();++bytes)
    {auto truncated=valid;truncated.resize(bytes);auto cfg=configured(truncated);assert(cfg.definition(truncated)!=error::success);}
    auto extra=valid;extra.push_back(0);auto cfg=configured(extra);assert(cfg.definition(extra)==error::invalid);
    for(auto word:{std::string_view{"physical"},std::string_view{"latest_required"},std::string_view{"device.submit"}})
    {
        auto changed=valid;auto found=std::search(changed.begin(),changed.end(),word.begin(),word.end());assert(found!=changed.end());*found='?';auto bad=configured(changed);assert(bad.definition(changed)==error::invalid);
    }
    auto policy=proposal_plan();policy.sources[0].alias.size=256;assert(!p::valid(policy));policy=proposal_plan();policy.tools[0].minimum_observations=0;assert(!p::valid(policy));
    policy=proposal_plan();policy.tools[0].effect=a::effect_class::unclassified;assert(!p::valid(policy));policy=proposal_plan();policy.tool_count=9;assert(!p::valid(policy));
    policy=proposal_plan();policy.storage.max_lifetime_ns=UINT64_MAX;assert(!p::valid(policy));
    p::arbiter_plan arbiter;assert(arbiter.proposal_alias.assign("suggestions")&&arbiter.target_alias.assign("motor"));arbiter.tool_mask=2;assert(!p::valid(arbiter,proposal_plan()));
}
static void whole_envelope_and_workspace()
{
    proposal_fixture value;auto bytes=value.envelope();p::envelope output;
    for(std::size_t extent{};extent<bytes.size();++extent)assert(!p::decode_envelope({bytes.data(),extent},output));
    bytes.push_back(0);assert(!p::decode_envelope(bytes,output));bytes.pop_back();assert(p::decode_envelope(bytes,output));
    auto invalid=output;invalid.target=0;std::array<std::uint8_t,8192> buffer{};s::wire::writer writer{buffer};assert(!p::encode_envelope(writer,invalid));
    p::workspace workspace;assert(workspace.configure(16,1)==error::success);p::workspace::lease first,other;
    assert(workspace.reserve(8,first)==error::success&&workspace.reserve(0,other)==error::again&&first.grow(9)==error::again&&first.grow(8)==error::success);
    std::uint64_t size{},calls{};workspace.inspect(size,calls);assert(size==16&&calls==1);auto moved=std::move(first);first.reset();workspace.inspect(size,calls);assert(size==16&&calls==1);
    moved.reset();assert(workspace.reserve(16,other)==error::success);workspace.retire();assert(other.grow(0)==error::cancelled);other.reset();workspace.inspect(size,calls);assert(!size&&!calls);
    p::workspace zero;assert(zero.configure(0,0)==error::success&&zero.reserve(0,other)==error::again);
}
int main()
{
    assert(proposal_dispatch_fixture_main()==0);canonical_definition_and_required_limits();malformed_canonical_even_with_matching_digest();whole_envelope_and_workspace();
    std::puts("passed 3 proposal configuration security groups");return 0;
}
