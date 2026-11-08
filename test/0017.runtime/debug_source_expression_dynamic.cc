// Pure syntax tests: parsing does not authorize a guest/native memory access.
#include <uwvm2/uwvm/debugger/source_dwarf_expression.h>
#include <cstdio>
#include <cstdlib>
namespace dwarf=uwvm2::uwvm::debugger::source_dwarf;
using dwarf::object_selector_error;
using dwarf::source_expression_step_kind;
static void require(bool ok) { if(!ok) { std::fputs("FAIL dynamic source expression\n",stderr); std::abort(); } }
static dwarf::source_expression parse(std::string_view text) {
    dwarf::source_expression result;
    require(dwarf::parse_source_expression(text,result)==object_selector_error::none);
    return result;
}
static void rejected(std::string_view text, dwarf::source_expression_limits const& cap={}) {
    dwarf::source_expression result{"stale",{{source_expression_step_kind::dereference,{},{}}}};
    require(dwarf::parse_source_expression(text,result,cap)!=object_selector_error::none);
    require(result.root_name.empty()&&result.steps.empty());
}
int main() {
    constexpr std::size_t depth=131072;
    auto nested=std::string(depth,'(')+"producer::p"+std::string(depth,')');
    require(parse(nested).root_name=="producer::p");
    require(parse(nested).steps.empty());
    nested.pop_back();rejected(nested);nested.push_back(')');
    rejected(nested+")");rejected(nested+"()");
    auto stars=std::string(depth,'*')+"p[7]";
    auto parsed=parse(stars);require(parsed.steps.size()==depth+1);
    require(parsed.steps.front().kind==source_expression_step_kind::index && parsed.steps.front().index==7);
    for(std::size_t i=1;i<parsed.steps.size();++i)require(parsed.steps[i].kind==source_expression_step_kind::dereference);
    std::string path="root";
    for(std::size_t i=0;i<8192;++i)path+="->member[-3].*";
    parsed=parse(path);require(parsed.steps.size()==8192*4);
    for(std::size_t i=0;i<8192;++i) {
        require(parsed.steps[4*i].kind==source_expression_step_kind::dereference);
        require(parsed.steps[4*i+1].kind==source_expression_step_kind::member && parsed.steps[4*i+1].member=="member");
        require(parsed.steps[4*i+2].kind==source_expression_step_kind::index && parsed.steps[4*i+2].index==-3);
        require(parsed.steps[4*i+3].kind==source_expression_step_kind::dereference);
    }
    auto name=std::string(65537,'x');require(parse(name).root_name==name);
    parsed=parse("len("+std::string(depth,'(')+"p"+std::string(depth,')')+")");
    require(parsed.root_name=="p"&&parsed.steps.size()==1&&parsed.steps[0].kind==source_expression_step_kind::go_length);
    for(auto text:{"len(cap(p))","*len(p)","(len(p)).x","cap(p)[0]","cap(p).*","p()","(p)=q","p[-9223372036854775809]"})
        rejected(text);
    dwarf::source_expression_limits cap{};cap.max_nesting=depth;
    parsed={};require(dwarf::parse_source_expression(nested,parsed,cap)==object_selector_error::none);
    rejected("("+nested+")",cap);
    cap={};cap.max_steps=2;parsed={};
    require(dwarf::parse_source_expression("p->x",parsed,cap)==object_selector_error::none);
    rejected("p->x[1]",cap);
    cap={};cap.max_expression_bytes=name.size();parsed={};
    require(dwarf::parse_source_expression(name,parsed,cap)==object_selector_error::none);
    rejected(name+" ",cap);
    // The scalar parser's primary entry consumes its own group only. It must
    // leave both postfix and following scalar operations for the owning parser.
    dwarf::expression_details::parser primary{"len(((p))) + 2",{},0,32,32};
    require(primary.primary(0)==object_selector_error::none && primary.cursor==10);
    require(primary.pending.steps.size()==1);
    dwarf::expression_details::parser limited{"((p))",{},0,32,4};
    require(limited.primary(3)==object_selector_error::limit_exceeded);
    std::puts("PASS dynamic expression: 131072 depth, 32768 steps, long root, explicit budgets and refusal semantics");
}
