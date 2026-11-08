// POLICY DATA regression for an observed return followed by a new call.
// The real ledger here mints event identities, but this component has no VM,
// pause ticket, publication or source owner authority. Production must obtain
// BOTH chains from separate authenticated all-stopped runtime transactions.
#include <uwvm2/uwvm/debugger/source_step_policy.h>
#include <uwvm2/runtime/lib/uwvm_runtime_debug_activation.h>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <span>
#include <utility>
#include <vector>
namespace step = uwvm2::uwvm::debugger::source_step;
namespace activation = uwvm2::runtime::lib::details::debug_activation;
static void check(bool value, char const* message)
{
    if(!value) { ::std::fprintf(stderr,"debug_source_step_return_into: %s\n",message); ::std::abort(); }
}
static ::std::vector<activation::frame> capture(activation::ledger const& ledger)
{
    check(ledger.complete(),"component ledger has one complete event island");
    ::std::vector<activation::frame> result; auto const data{ledger.data()};
    for(::std::size_t i{}; i != ledger.count(); ++i) { result.push_back(data[i]); }
    return result;
}
static step::activation_relation relation(::std::vector<activation::frame> const& first,
    ::std::vector<activation::frame> const& second)
{ return step::compare_event_chains(::std::span{first},::std::span{second}); }
static void expect(step::origin const& saved, step::position const& current, step::action action, step::error reason)
{
    auto const result{step::decide(saved,current)};
    check(result.result==action && result.reason==reason,"precise return-to-ancestor source policy result");
}
int main()
{
    activation::ledger ledger{};
    auto const root{ledger.enter(0u,1u,1u,7u)};
    auto const scheduler{ledger.enter(0u,2u,1u,7u)};
    auto const original{ledger.enter(0u,3u,1u,7u)};
    check(root && scheduler && original,"actual generated-entry model identities");
    auto const initial_chain{capture(ledger)};
    step::position initial{};
    initial.source_owner=::std::make_shared<int>(1); // comparison-only fixture token
    initial.module=0u;initial.function=3u;initial.runtime_epoch=7u;initial.function_generation=1u;
    initial.mapping=step::mapping_kind::line_only;initial.file="Main.hs";initial.line=8u;initial.column=1u;
    initial.is_statement=true;initial.metadata_ready=true;initial.physical_depth=initial_chain.size();
    initial.status=step::position_status::mapped;initial.trace=step::trace_status::complete;
    initial.activation=step::activation_relation::same;
    ::std::array<step::origin,3u> origins{};
    constexpr ::std::array policies{step::policy::into,step::policy::over,step::policy::out};
    for(::std::size_t i{};i!=policies.size();++i)
    { check(step::prepare(initial,policies[i],origins[i])==step::error::none,"complete source policy setup"); }

    // A copied unrelated same-depth sibling cannot be called a return. The
    // controller must refuse unknown comparison BEFORE calling observe_return.
    auto forged{initial_chain};++forged.back().incarnation;forged.back().continuation=forged.back().incarnation;
    check(relation(initial_chain,forged)==step::activation_relation::unknown,"fresh sibling needs a witnessed ancestor");
    auto invalid{initial};invalid.activation=relation(initial_chain,forged);
    check(step::observe_return(origins[0],invalid)!=step::error::none && !origins[0].returned_anchor_ready,
          "unknown sibling cannot install a returned ancestor");
    forged=initial_chain;forged.back().parent=99u;
    check(relation(initial_chain,forged)==step::activation_relation::unknown,"forged parent chain rejected");
    forged=initial_chain;++forged.back().runtime_epoch;
    check(relation(initial_chain,forged)==step::activation_relation::unknown,"stale epoch chain rejected");
    forged=initial_chain;++forged.back().function_generation;
    check(relation(initial_chain,forged)==step::activation_relation::unknown,"stale function generation rejected");

    // Only this actual ledger leave creates the authenticated-chain DATA
    // counterexample: Main returns into an unmapped runtime scheduler.
    ledger.leave(original,activation::exit_kind::returned);
    auto const ancestor_chain{capture(ledger)};
    auto ancestor{initial};ancestor.function=2u;ancestor.physical_depth=ancestor_chain.size();
    ancestor.activation=relation(initial_chain,ancestor_chain);
    ancestor.status=step::position_status::unmapped;ancestor.file.clear();ancestor.line=ancestor.column=0u;ancestor.is_statement=false;
    check(ancestor.activation==step::activation_relation::returned,"complete original prefix proves the return");
    for(auto& origin:origins)
    {
        expect(origin,ancestor,step::action::keep_running,step::error::none);
        check(step::observe_return(origin,ancestor)==step::error::none && origin.returned_anchor_ready,
              "into/over/out retain the same witnessed unmapped ancestor");
    }

    auto const child{ledger.enter(0u,4u,1u,7u)};check(child!=0u,"new scheduler-owned closure activation");
    auto const child_chain{capture(ledger)};
    check(relation(initial_chain,child_chain)==step::activation_relation::unknown,"retired origin alone cannot prove the sibling");
    check(relation(ancestor_chain,child_chain)==step::activation_relation::deeper,"retained real ancestor proves a new descendant");
    auto current{initial};current.function=4u;current.file="Closure.hs";current.line=21u;
    current.physical_depth=child_chain.size();current.activation=relation(ancestor_chain,child_chain);
    expect(origins[0],current,step::action::stop,step::error::none);
    expect(origins[1],current,step::action::keep_running,step::error::none);
    expect(origins[2],current,step::action::keep_running,step::error::none);
    auto unavailable{current};unavailable.status=step::position_status::unmapped;unavailable.file.clear();unavailable.line=0u;
    expect(origins[0],unavailable,step::action::keep_running,step::error::none);
    unavailable.metadata_ready=false;expect(origins[0],unavailable,step::action::decline,step::error::unmapped);
    unavailable=current;unavailable.activation=step::activation_relation::unknown;
    expect(origins[0],unavailable,step::action::decline,step::error::unknown_activation);
    unavailable=current;++unavailable.runtime_epoch;
    expect(origins[0],unavailable,step::action::decline,step::error::stale_generation);
    auto foreign{::std::make_shared<int>(2)};unavailable=current;
    unavailable.source_owner=::std::shared_ptr<void const>{foreign,current.source_owner.get()};
    expect(origins[0],unavailable,step::action::decline,step::error::stale_generation);
    unavailable=current;unavailable.trace=step::trace_status::missing;
    expect(origins[0],unavailable,step::action::decline,step::error::missing_trace);

    // A tail successor is also compared to the NEW actual ancestor chain.
    ledger.leave(child,activation::exit_kind::typed_tail);
    auto const tail{ledger.enter(0u,5u,1u,7u)};check(tail!=0u,"typed-tail successor identity");
    auto const tail_chain{capture(ledger)};current.function=5u;current.line=31u;
    current.activation=relation(ancestor_chain,tail_chain);
    check(current.activation==step::activation_relation::deeper,"tail successor stays below the retained ancestor");
    expect(origins[0],current,step::action::stop,step::error::none);
    expect(origins[1],current,step::action::keep_running,step::error::none);
    expect(origins[2],current,step::action::keep_running,step::error::none);

    auto missing{initial};missing.trace=step::trace_status::missing;step::origin incomplete{};
    // DWARF into may describe an ordinary line change without a complete trace;
    // that weaker DATA setup must NEVER acquire return rebasing authority.
    missing.mapping=step::mapping_kind::dwarf_scope;missing.scope_path={{1u,1u}};
    check(step::prepare(missing,step::policy::into,incomplete)==step::error::none,"weak DWARF-into model setup");
    check(step::observe_return(incomplete,ancestor)==step::error::inconsistent_activation && !incomplete.returned_anchor_ready,
          "missing original complete chain refuses returned-anchor retention");
    ledger.leave(tail,activation::exit_kind::returned);ledger.leave(scheduler,activation::exit_kind::returned);
    ledger.leave(root,activation::exit_kind::returned);
    check(ledger.valid() && ledger.count()==0u,"all component ledger events retired");
    ::std::puts("debug_source_step_return_into PASS");
}
