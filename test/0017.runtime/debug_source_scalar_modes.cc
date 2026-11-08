// Supported build-mode checks over owned DATA only, with no guest/read grant.
#include <uwvm2/uwvm/debugger/source_scalar_expression.h>
#include <cstdio>
#include <cstdlib>
#include <type_traits>
namespace s=uwvm2::uwvm::debugger::source_scalar_expression;
static void require(bool ok) { if(!ok) { std::fputs("FAIL scalar build mode\n",stderr);std::abort(); } }
static_assert(std::is_nothrow_move_assignable_v<s::program>);
static_assert(noexcept(s::parse({},std::declval<s::program&>())));
int main() {
    auto text=std::string(2048,'(')+"1"+std::string(2048,')');s::program p;s::integer value{99u};unsigned callbacks{};
    auto resolve=[&](auto const&,bool,auto&)->bool { ++callbacks;return false; };
    static_assert(noexcept(s::evaluate(p,resolve,value)));
    require(s::parse(text,p)==s::error::none);
    require(s::evaluate(p,resolve,value)==s::error::none&&value.bits==1u&&!callbacks);
    p.root=p.nodes.size();value={99u};
    require(s::evaluate(p,resolve,value)==s::error::malformed&&!value.bits&&!callbacks);
    require(s::parse("1+",p)!=s::error::none&&p.nodes.empty()&&p.original.empty());
    s::expression_limits cap{};cap.max_nodes=2u;
    require(s::parse("1+2",p,s::language_semantics::shared_numeric,cap)==s::error::limit_exceeded&&p.nodes.empty()&&p.original.empty());
    require(s::parse("sizeof(1+2)",p)==s::error::none);cap={};cap.max_type_work=0u;value={99u};
    require(s::evaluate(p,resolve,value,32u,s::details::no_type_resolver{},s::language_semantics::shared_numeric,cap)==s::error::limit_exceeded&&!value.bits&&!callbacks);
#if defined(__cpp_exceptions) || defined(_CPPUNWIND)
    std::puts("PASS scalar mode: exceptions enabled; recoverable allocation failures qualified separately");
#else
    std::puts("PASS scalar mode: exceptions disabled; normal execution, quotas and malformed cleanup; standard allocator OOM is nonrecoverable");
#endif
}
