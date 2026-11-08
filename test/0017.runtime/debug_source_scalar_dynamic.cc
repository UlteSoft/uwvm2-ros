// Pure owned scalar syntax/type/value tests. No callback grants guest authority.
#include <uwvm2/uwvm/debugger/source_scalar_expression.h>
#include <cstdio>
#include <cstdlib>
#include <new>
#include <sys/resource.h>
namespace scalar=uwvm2::uwvm::debugger::source_scalar_expression;
namespace dwarf=uwvm2::uwvm::debugger::source_dwarf;
static std::ptrdiff_t allocation_budget=-1;
static std::size_t allocations{},live_allocations{};
static void* allocate(std::size_t n,std::size_t alignment=0) {
    if(allocation_budget==0)throw std::bad_alloc{};
    if(allocation_budget>0)--allocation_budget;
    void* p{};
    if(alignment) { if(posix_memalign(&p,alignment,n?n:1))throw std::bad_alloc{}; }
    else { p=std::malloc(n?n:1);if(!p)throw std::bad_alloc{}; }
    ++allocations;++live_allocations;return p;
}
void* operator new(std::size_t n) { return allocate(n); }
void* operator new[](std::size_t n) { return allocate(n); }
void* operator new(std::size_t n,std::align_val_t a) { return allocate(n,static_cast<std::size_t>(a)); }
void* operator new[](std::size_t n,std::align_val_t a) { return allocate(n,static_cast<std::size_t>(a)); }
static void release(void* p) noexcept { if(p) { --live_allocations;std::free(p); } }
void operator delete(void* p) noexcept { release(p); }
void operator delete[](void* p) noexcept { release(p); }
void operator delete(void* p,std::size_t) noexcept { release(p); }
void operator delete[](void* p,std::size_t) noexcept { release(p); }
void operator delete(void* p,std::align_val_t) noexcept { release(p); }
void operator delete[](void* p,std::align_val_t) noexcept { release(p); }
void operator delete(void* p,std::size_t,std::align_val_t) noexcept { release(p); }
void operator delete[](void* p,std::size_t,std::align_val_t) noexcept { release(p); }
static void require(bool ok,char const* reason) { if(!ok) { std::fprintf(stderr,"FAIL scalar dynamic: %s\n",reason);std::abort(); } }
static std::size_t reads{},type_reads{},dead_reads{};
static auto resolve=[](dwarf::source_expression const& ref,bool size,scalar::integer& out) {
    ++reads;if(ref.root_name=="never") { ++dead_reads;return false; }
    out={size?4u:ref.root_name=="zero"?0u:7u,32u,false,false,scalar::value_category::integer};return true;
};
static auto resolve_type=[](dwarf::source_expression const&,scalar::integer& out) {
    ++type_reads;out={0u,32u,false,false,scalar::value_category::integer};return true;
};
static scalar::program parsed(std::string_view text,scalar::language_semantics language=scalar::language_semantics::shared_numeric) {
    scalar::program out;require(scalar::parse(text,out,language)==scalar::error::none,"parse");return out;
}
static scalar::integer value(scalar::program const& code,scalar::language_semantics language=scalar::language_semantics::shared_numeric) {
    scalar::integer out;require(scalar::evaluate(code,resolve,out,32u,resolve_type,language)==scalar::error::none,"evaluate");return out;
}
static void reject_graph(scalar::program const& code) {
    auto const before=reads+type_reads;scalar::integer out{99u};
    require(scalar::evaluate(code,resolve,out,32u,resolve_type)!=scalar::error::none,"bad graph rejected");
    require(!out.bits && reads+type_reads==before,"bad graph cleared before callbacks");
}
static void parse_allocation_failures(std::string_view text,scalar::language_semantics language=scalar::language_semantics::shared_numeric) {
    auto before=allocations;{ auto code=parsed(text,language); }auto const count=allocations-before;
    for(std::size_t i=0;i<count;++i) {
        scalar::program out;auto const live=live_allocations;allocation_budget=static_cast<std::ptrdiff_t>(i);
        auto const status=scalar::parse(text,out,language);allocation_budget=-1;
        require(status==scalar::error::allocation_failure,"each parse allocation refused");
        require(out.nodes.empty()&&out.original.empty()&&live_allocations==live,"parse failure has no output or leaked continuation");
    }
    std::printf("parse allocation failures: %zu\n",count);
}
static void evaluation_allocation_failures(scalar::program const& code,scalar::language_semantics language=scalar::language_semantics::shared_numeric) {
    auto before=allocations;require(value(code,language).bits==14u,"allocation fixture value");auto const count=allocations-before;
    for(std::size_t i=0;i<count;++i) {
        auto const live=live_allocations;scalar::integer out{99u};allocation_budget=static_cast<std::ptrdiff_t>(i);
        auto const status=scalar::evaluate(code,resolve,out,32u,resolve_type,language);allocation_budget=-1;
        require(status==scalar::error::allocation_failure,"each evaluation allocation refused");
        require(!out.bits&&live_allocations==live,"evaluation failure has no output or leaked continuation");
    }
    std::printf("evaluation allocation failures: %zu\n",count);
}
int main() {
    rlimit stack{};require(getrlimit(RLIMIT_STACK,&stack)==0,"stack query");stack.rlim_cur=512u*1024u;
    require(setrlimit(RLIMIT_STACK,&stack)==0,"small native stack");
    constexpr std::size_t depth=131072u;
    auto groups=std::string(depth,'(')+"x"+std::string(depth,')');
    require(value(parsed(groups)).bits==7u,"131072 groups on small native stack");
    groups.pop_back();scalar::program empty;require(scalar::parse(groups,empty)!=scalar::error::none&&empty.nodes.empty(),"deep incomplete syntax cleared");groups.push_back(')');
    auto stars=std::string(depth,'*')+"x[3]";auto code=parsed(stars);
    require(code.nodes.size()==1u&&code.nodes[code.root].reference.steps.size()==depth+1u,"dynamic reference extension without quadratic copies");
    require(value(code).bits==7u,"large owned reference");
    auto prefix=std::string(depth,'!')+"0";require(value(parsed(prefix)).bits==0u,"131072 unary nodes");
    auto zeros=std::string(65537u,'0')+"1";require(value(parsed(zeros)).bits==1u,"long normalized integer");
    require(value(parsed(zeros+".0")).floating,"long normalized float");
    require(value(parsed(zeros+"u32",scalar::language_semantics::rust),scalar::language_semantics::rust).bits==1u,"long normalized Rust integer");
    std::string terms="1";for(std::size_t i=1;i<8192u;++i)terms+=" + 1";
    code=parsed(terms);require(code.nodes.size()==16383u&&value(code).bits==8192u,"large AST");
    require(value(parsed("sizeof("+terms+")")).bits==4u,"deep type inference");
    std::string rust_terms="1i32";for(std::size_t i=1;i<8192u;++i)rust_terms+=" + 1i32";
    require(value(parsed(rust_terms,scalar::language_semantics::rust),scalar::language_semantics::rust).bits==8192u,"dynamic Rust constraints across large AST");
    for(auto language:{scalar::language_semantics::go,scalar::language_semantics::rust,scalar::language_semantics::zig}) {
        auto const text=language==scalar::language_semantics::zig?"false and never":"false && never";
        auto const lazy=parsed(text,language);scalar::integer result{};
        auto boolean_type=[](auto const&,scalar::integer& target) { target={0u,8u,true,false,scalar::value_category::boolean};return true; };
        auto const before=dead_reads;
        require(scalar::evaluate(lazy,resolve,result,32u,boolean_type,language)==scalar::error::none&&!result.bits&&dead_reads==before,"language Boolean short circuit never reads dead value");
    }
    std::string sizes="x";for(std::size_t i=0;i<4096u;++i)sizes="sizeof("+sizes+")";
    require(value(parsed(sizes)).bits==4u,"nested sizeof without operand rescanning");
    for(auto text:{"0 && never","1 || never","1 ? x : never","0 ? never : x","1 ? x : (1 / 0)"}) {
        auto const dead=dead_reads;require(value(parsed(text)).bits==(text[0]=='0'&&text[2]=='&'?0u:text[2]=='|'?1u:7u),"lazy result");
        require(dead_reads==dead,"dead value callback never invoked");
    }
    // Shared DAG evaluation and type inference visit each owned node once.
    code={};scalar::node leaf{};leaf.op=scalar::operation::reference;leaf.reference.root_name="zero";code.nodes.push_back(std::move(leaf));
    for(std::size_t i=1;i<96u;++i) { scalar::node n{};n.op=scalar::operation::add;n.lhs=n.rhs=i-1u;code.nodes.push_back(std::move(n)); }
    scalar::node size{};size.op=scalar::operation::expression_size;size.lhs=95u;code.nodes.push_back(std::move(size));
    scalar::node sum{};sum.op=scalar::operation::add;sum.lhs=96u;sum.rhs=95u;code.nodes.push_back(std::move(sum));code.root=97u;
    auto r=reads,t=type_reads;require(value(code).bits==4u&&reads==r+1u&&type_reads==t+1u,"shared DAG bounded owned memo");
    code.nodes[0].op=scalar::operation::positive;code.nodes[0].lhs=code.root;reject_graph(code);
    code=parsed("1");code.root=code.nodes.size();reject_graph(code);
    code.root=0;scalar::node bad{};bad.op=scalar::operation::positive;bad.lhs=1u;code.nodes.push_back(bad);reject_graph(code); // unreachable self cycle
    code.nodes[1].lhs=999u;reject_graph(code);code.nodes[1].op=static_cast<scalar::operation>(255u);reject_graph(code);
    code=parsed("10i32 + 32i32",scalar::language_semantics::rust);auto const n=code.nodes.size();std::reverse(code.nodes.begin(),code.nodes.end());
    for(auto& item:code.nodes) { auto const count=scalar::details::child_count(item.op);if(count) item.lhs=n-1u-item.lhs;if(count>1) item.rhs=n-1u-item.rhs;if(count>2) item.third=n-1u-item.third; }
    code.root=n-1u-code.root;require(value(code,scalar::language_semantics::rust).bits==42u,"forward acyclic Rust graph");
    scalar::expression_limits cap{};cap.max_nodes=2u;code={};require(scalar::parse("1+2",code,scalar::language_semantics::shared_numeric,cap)==scalar::error::limit_exceeded&&code.nodes.empty(),"caller node quota");
    cap={};cap.max_expression_bytes=3u;require(scalar::parse("  1 ",code,scalar::language_semantics::shared_numeric,cap)==scalar::error::limit_exceeded,"caller byte quota");
    cap={};cap.max_reference_steps=1u;require(scalar::parse("p->a",code,scalar::language_semantics::shared_numeric,cap)==scalar::error::limit_exceeded,"caller reference quota");
    cap={};cap.max_nesting=4u;require(scalar::parse("(((1)))",code,scalar::language_semantics::shared_numeric,cap)==scalar::error::limit_exceeded,"caller nesting quota");
    code=parsed("sizeof(x+1)");cap={};cap.max_type_work=1u;scalar::integer out{99u};r=reads;
    require(scalar::evaluate(code,resolve,out,32u,resolve_type,scalar::language_semantics::shared_numeric,cap)==scalar::error::limit_exceeded&&!out.bits&&reads==r,"caller inference work quota");
    for(auto text:{"run()","x++","&x","(int*)x","static_cast<void*>(x)","x=1"}) { require(scalar::parse(text,code)!=scalar::error::none,"unsupported semantics remain refused"); }
    parse_allocation_failures("((1.25 + 2) * (producer->member[3] ? 4 : 5))");
    parse_allocation_failures("123_456 + 0x1234 + 1.25e2");
    parse_allocation_failures("1.2500000000000000000000000000000000000000000001 + 2.0",scalar::language_semantics::rust);
    code=parsed("sizeof(x + 2) + (x != 0 ? x + 3 : x / 0)");evaluation_allocation_failures(code);
    code=parsed("7i32 + 7i32",scalar::language_semantics::rust);evaluation_allocation_failures(code,scalar::language_semantics::rust);
    for(auto fail:{4096,32768,131072}) {
        auto const live=live_allocations;allocation_budget=fail;
        auto const status=scalar::parse(groups,empty);allocation_budget=-1;
        require(status==scalar::error::allocation_failure&&empty.nodes.empty()&&live_allocations==live,"deep parse allocation cleanup stays iterative");
    }
    code=parsed("1 + x");
    auto const throwing_value=[](auto const&,bool,scalar::integer&)->bool { throw std::bad_alloc{}; };
    auto live=live_allocations;out={99u};
    require(scalar::evaluate(code,throwing_value,out,32u,resolve_type)==scalar::error::allocation_failure&&!out.bits&&live_allocations==live,"throwing value callback cleans all suspended continuations");
    code=parsed("1 ? x : never");
    auto const throwing_type=[](auto const&,scalar::integer&)->bool { throw std::bad_alloc{}; };
    live=live_allocations;out={99u};r=reads;
    require(scalar::evaluate(code,resolve,out,32u,throwing_type)==scalar::error::allocation_failure&&!out.bits&&reads==r&&live_allocations==live,"throwing metadata callback has no value reads or leaked continuations");
    code=parsed(prefix);
    for(auto fail:{4096,32768,131072}) {
        auto const live=live_allocations;allocation_budget=fail;
        auto const status=scalar::evaluate(code,resolve,out,32u,resolve_type);allocation_budget=-1;
        require(status==scalar::error::allocation_failure&&!out.bits&&live_allocations==live,"deep evaluate allocation cleanup stays iterative");
    }
    std::puts("PASS dynamic scalar: 131072 depth, large AST, DAG validation/memo, lazy values, caller quotas, allocation cleanup");
}
