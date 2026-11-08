// Actual production metadata parsers and event ledger. These detached DATA
// models do not mint a runtime pause/source/read authority.
#include <uwvm2/uwvm/debugger/source_step_policy.h>
#include <uwvm2/uwvm/debugger/source_frames.h>
#include <uwvm2/uwvm/debugger/source_map.h>
#include <uwvm2/uwvm/debugger/source_dwarf_query.h>
#include <uwvm2/runtime/lib/uwvm_runtime_debug_activation.h>
#include <fast_io.h>
#include <source_location>
#include <string>
namespace dbg=uwvm2::uwvm::debugger;
namespace dwarf=dbg::source_dwarf;
namespace activation=uwvm2::runtime::lib::details::debug_activation;
static void check(bool value,::std::source_location at=::std::source_location::current())
{ if(!value) { ::fast_io::io::perrln("unbounded debugger metadata line ",at.line()); ::fast_io::fast_terminate(); } }
int main()
{
    dbg::source_step::position position{};
    position.source_owner=::std::make_shared<int>(0); // comparison-only test token
    position.module=1u;position.function=2u;position.runtime_epoch=1u;position.function_generation=1u;
    position.file=::std::string(8193u,'s');position.line=1u;position.is_statement=true;
    position.physical_depth=4097u;position.trace=dbg::source_step::trace_status::complete;
    position.activation=dbg::source_step::activation_relation::same;position.status=dbg::source_step::position_status::mapped;
    for(::std::uint64_t i{};i!=257u;++i) { position.scope_path.push_back({1u,i+1u}); }
    dbg::source_step::origin origin{};
    check(dbg::source_step::prepare(position,dbg::source_step::policy::over,origin)==dbg::source_step::error::none);
    auto current{position};++current.line;
    check(dbg::source_step::decide(origin,current).result==dbg::source_step::action::stop);
    dbg::source_step::limits quota{};quota.max_file_bytes=4096u;
    check(dbg::source_step::prepare(position,dbg::source_step::policy::over,origin,quota)==dbg::source_step::error::limit_exceeded && !origin.ready);
    ::std::vector<dwarf::scope_record> scopes(258u);
    for(::std::size_t i{};i!=scopes.size();++i)
    {
        auto& scope{scopes[i]};scope.identity={1u,i+1u};scope.concrete=true;
        scope.kind=i==0u?dwarf::scope_kind::subprogram:dwarf::scope_kind::inline_subprogram;
        scope.parent=i==0u?dwarf::no_record:i-1u;scope.ranges={{1u,2u}};
        scope.name=::std::string(257u,'n');scope.call_file=position.file;
    }
    ::std::vector<dbg::source_frames::frame> frames{};
    check(dbg::source_frames::current_frames(scopes,1u,frames)==dbg::source_frames::error::none && frames.size()==scopes.size());
    ::std::vector<dwarf::inline_frame> inlines{};
    check(dwarf::query_inline_frames(scopes,1u,inlines)==dwarf::inline_query_error::none && inlines.size()+1u==scopes.size());
    dwarf::inline_query_limits inline_quota{};inline_quota.max_frames=64u;
    check(dwarf::query_inline_frames(scopes,1u,inlines,inline_quota)==dwarf::inline_query_error::limit_exceeded && inlines.empty());
    auto json{::std::string{"{\"version\":3,\"sources\":[\""}+position.file+"\"],\"mappings\":\"AAAA\",\"unknown\":"};
    json.append(4097u,'[');json+="true";json.append(4097u,']');json+="}";
    dbg::source_map_sections sections{};sections.source_map_declared=true;sections.source_map_json=json;
    sections.module_file_size=10u;sections.code_section_content_size=10u;
    dbg::source_map map{};
    check(dbg::source_map::parse(1u,sections,map)==dbg::source_map_error::none && map.lookup(1u,0u)->file.size()>4096u);
    dbg::source_map_limits map_quota{};map_quota.max_path_bytes=4096u;
    check(dbg::source_map::parse(1u,sections,map,map_quota)==dbg::source_map_error::limit_exceeded);
    check(map.lookup(1u,0u)->file.size()>4096u); // failed parse does not publish a partial replacement
    auto malformed{json};malformed.insert(malformed.size()-2u,1u,',');sections.source_map_json=malformed;
    check(dbg::source_map::parse(1u,sections,map)!=dbg::source_map_error::none);
    // Linked maps can carry sourceContent above the former 16 MiB envelope.
    auto large{::std::string{"{\"version\":3,\"sources\":[\"s\"],\"mappings\":\"AAAA\",\"sourcesContent\":[\""}};
    large.append(16u*1024u*1024u+1u,'x');large+="\"]}";sections.source_map_json=large;
    check(dbg::source_map::parse(1u,sections,map)==dbg::source_map_error::none);
    activation::ledger ledger{};
    auto const root{ledger.enter(1u,0u,1u,1u)};check(root!=0u);
    activation::frame expected{ledger.data()[0u]};
    // Cross both former deep-chain count and default 256 MiB storage ceilings.
    constexpr ::std::size_t depth{4u*1024u*1024u+1u};
    for(::std::size_t i{1u};i!=depth;++i) { check(ledger.enter(1u,i,1u,1u)!=0u); }
    check(ledger.storage_bytes()>256u*1024u*1024u && ledger.complete() && ledger.matches_native_descendant(&expected,1u));
    ++expected.runtime_epoch;check(!ledger.matches_native_descendant(&expected,1u));
    while(ledger.count()!=0u) { ledger.leave(ledger.data()[ledger.count()-1u].incarnation,activation::exit_kind::returned); }
    check(ledger.ready_for_root_entry());
    activation::ledger manual{activation::ledger::inline_storage_bytes};
    for(::std::size_t i{};i!=activation::frames_per_block;++i) { check(manual.enter(0u,i,1u,1u)!=0u); }
    check(manual.enter(0u,0u,1u,1u)==0u && manual.failure_reason()==activation::failure::resource_exhausted);
    ::fast_io::io::println("PASS complete metadata extents, iterative JSON, stable growable activation ledger, explicit quota and malformed/stale refusal");
}
