#include "../../tools/ros_scene/qualification_integer_leaf_inline.h"
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <llvm/AsmParser/Parser.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>

namespace experiment=::uwvm2::runtime::qualification::integer_leaf_inline;
static unsigned checks{};
static void require(bool okay,char const* reason)
{
    ++checks;
    if(!okay){::std::fprintf(stderr,"FAIL %s\n",reason);::std::abort();}
}
static auto parse(::llvm::LLVMContext& context,char const* ir)
{
    ::llvm::SMDiagnostic error{};
    // The parser allocates/owns its own LLVM Module; this is upstream ownership,
    // not a component-owned object allocated with the C++ standard allocator.
    auto module=::llvm::parseAssemblyString(ir,error,context);
    if(!module){error.print("integer-leaf",::llvm::errs());::std::abort();}
    return module;
}
static auto printed(::llvm::Value const& value)
{
    ::llvm::SmallString<2048> bytes{};
    ::llvm::raw_svector_ostream stream{bytes};value.print(stream);return bytes;
}
static auto printed(::llvm::Module const& value)
{
    ::llvm::SmallString<4096> bytes{};
    ::llvm::raw_svector_ostream stream{bytes};value.print(stream,nullptr);return bytes;
}
static unsigned direct_calls(::llvm::Function const& function)
{
    unsigned count{};
    for(auto const& block:function)for(auto const& instruction:block)
        if(::llvm::isa<::llvm::CallBase>(instruction)){++count;}
    return count;
}
static constexpr char positive[]=R"(
@indirect = global ptr @leaf
define i32 @leaf(i32 %x) noinline nomerge nounwind "nooutline" {
  %fold = or i32 %x, 32
  %delta = sub i32 %fold, 97
  %inside = icmp ult i32 %delta, 26
  %r = zext i1 %inside to i32
  ret i32 %r
}
define i32 @caller(i32 %x) noinline nomerge "nooutline" {
  %r = call i32 @leaf(i32 %x)
  %result = add i32 %r, %x
  ret i32 %result
}
define i32 @raw_wrapper(i32 %x) noinline {
  %r = call i32 @leaf(i32 %x)
  ret i32 %r
}
define i32 @indirect_caller(i32 %x) noinline {
  %target = load ptr, ptr @indirect
  %r = call i32 %target(i32 %x)
  ret i32 %r
}
)";

static void semantics(bool boolean_range=false)
{
    ::llvm::LLVMContext left_context{},right_context{};
    ::uwvm2::utils::container::std_string source{positive};
    if(boolean_range)
    {
        constexpr ::std::string_view original="define i32 @leaf";
        auto offset=source.find(original);require(offset!=source.npos,"real boolean leaf attribute fixture");
        source.replace(offset,original.size(),"define range(i32 0, 2) i32 @leaf");
    }
    auto left=parse(left_context,source.c_str()),right=parse(right_context,source.c_str());
    auto* leaf=right->getFunction("leaf");auto* caller=right->getFunction("caller");
    auto const public_before=printed(*leaf),wrapper_before=printed(*right->getFunction("raw_wrapper"));
    auto const indirect_before=printed(*right->getNamedGlobal("indirect"));
    auto const indirect_caller_before=printed(*right->getFunction("indirect_caller"));
    auto const caller_attributes=caller->getAttributes();
    ::std::array<::llvm::Function*,2> entries{leaf,caller};
    auto result=experiment::run(*right,entries);
    require(result.state==experiment::status::complete && result.leaves==1u && result.edges==1u,"positive exact private edge");
    require(public_before==printed(*leaf),"public leaf body attributes ABI intact");
    require(wrapper_before==printed(*right->getFunction("raw_wrapper")),"raw wrapper untouched");
    require(indirect_before==printed(*right->getNamedGlobal("indirect")),"indirect target untouched");
    require(indirect_caller_before==printed(*right->getFunction("indirect_caller")),"indirect caller untouched");
    require(caller_attributes==caller->getAttributes(),"caller attributes untouched");
    require(direct_calls(*caller)==0u && direct_calls(*left->getFunction("caller"))==1u,"only selected direct call disappears");
    for(auto const& function:*right){require(!function.getName().contains("integer.inline.body"),"no temporary private function reaches materialization");}
    // ExecutionEngine is allocated by LLVM's own factory; these pointer carriers
    // return it to LLVM's virtual destructor and perform no owned allocation.
    ::std::unique_ptr<::llvm::ExecutionEngine> left_engine{
        ::llvm::EngineBuilder(::std::move(left)).setEngineKind(::llvm::EngineKind::JIT).setOptLevel(::llvm::CodeGenOptLevel::Aggressive).create()};
    ::std::unique_ptr<::llvm::ExecutionEngine> right_engine{
        ::llvm::EngineBuilder(::std::move(right)).setEngineKind(::llvm::EngineKind::JIT).setOptLevel(::llvm::CodeGenOptLevel::Aggressive).create()};
    require(left_engine && right_engine,"actual MCJIT A/B engines");
    left_engine->finalizeObject();right_engine->finalizeObject();
    using entry=::std::uint32_t(*)(::std::uint32_t);
    auto before=reinterpret_cast<entry>(left_engine->getFunctionAddress("caller"));
    auto after=reinterpret_cast<entry>(right_engine->getFunctionAddress("caller"));
    auto public_entry=reinterpret_cast<entry>(right_engine->getFunctionAddress("leaf"));
    auto indirect=reinterpret_cast<entry>(right_engine->getFunctionAddress("indirect_caller"));
    auto wrapper=reinterpret_cast<entry>(right_engine->getFunctionAddress("raw_wrapper"));
    require(before && after && public_entry && indirect && wrapper,"public entries remain actual materialized functions");
    auto check=[&](::std::uint32_t x)
    {
        ::std::uint32_t expected=(((x|32u)-97u)<26u)?1u:0u;
        require(before(x)==x+expected && after(x)==x+expected,"actual native A/B uint32 wraparound semantics");
        require(public_entry(x)==expected && indirect(x)==expected && wrapper(x)==expected,"public raw and indirect semantics");
    };
    for(::std::uint32_t x{};x!=256u;++x){check(x);}
    for(auto x : ::std::array<::std::uint32_t,14>{0u,1u,31u,32u,64u,65u,90u,91u,96u,97u,0x7fffffffu,0x80000000u,0xfffffffeu,0xffffffffu}){check(x);}
    ::std::uint32_t random{0xb5c39a17u};
    for(unsigned i{};i!=100000u;++i){random^=random<<13u;random^=random>>17u;random^=random<<5u;check(random);}
}

static void reject_body(char const* name,char const* ir)
{
    ::llvm::LLVMContext context{};auto module=parse(context,ir);
    require(!::llvm::verifyModule(*module),"negative fixture has valid real LLVM IR");
    auto* leaf=module->getFunction("leaf");require(leaf!=nullptr,name);
    require(!experiment::eligible(*leaf),name);
}
static void reject_edge(char const* reason,char const* ir)
{
    ::llvm::LLVMContext context{};auto module=parse(context,ir);
    require(!::llvm::verifyModule(*module),"edge fixture has valid LLVM IR");
    auto const original=printed(*module);
    ::std::array<::llvm::Function*,2> entries{module->getFunction("leaf"),module->getFunction("caller")};
    auto result=experiment::run(*module,entries);
    require(result.state==experiment::status::complete && result.edges==0u,reason);
    require(original==printed(*module),"rejected edge preserves all LLVM IR");
}
static void rejection_matrix()
{
    reject_body("memory",R"(define i32 @leaf(i32 %x){ %p=alloca i32
      store i32 %x,ptr %p
      %r=load i32,ptr %p
      ret i32 %r })");
    reject_body("fp",R"(define i32 @leaf(i32 %x){ %f=uitofp i32 %x to float
      %r=fptoui float %f to i32
      ret i32 %r })");
    reject_body("division",R"(define i32 @leaf(i32 %x){ %r=udiv i32 12,%x
      ret i32 %r })");
    reject_body("shift",R"(define i32 @leaf(i32 %x){ %r=shl i32 1,%x
      ret i32 %r })");
    reject_body("poison flags",R"(define i32 @leaf(i32 %x){ %r=add nsw i32 %x,1
      ret i32 %r })");
    reject_body("undef",R"(define i32 @leaf(i32 %x){ %r=add i32 %x,undef
      ret i32 %r })");
    reject_body("poison",R"(define i32 @leaf(i32 %x){ %r=add i32 %x,poison
      ret i32 %r })");
    reject_body("constant expression",R"(@p=global i32 0
      define i64 @leaf(i32 %x){ret i64 ptrtoint(ptr @p to i64)})");
    reject_body("call",R"(declare i32 @external(i32)
      define i32 @leaf(i32 %x){ %r=call i32 @external(i32 %x)
      ret i32 %r })");
    reject_body("loop",R"(define i32 @leaf(i32 %x){entry: br label %loop
      loop: br label %loop })");
    reject_body("gc",R"(define i32 @leaf(i32 %x) gc "statepoint-example" { ret i32 %x })");
    reject_body("observer",R"(define i32 @leaf(i32 %x) convergent { ret i32 %x })");
    reject_body("return attribute",R"(define noundef i32 @leaf(i32 %x){ret i32 %x})");
    reject_body("unproved boolean range",R"(define range(i32 0, 2) i32 @leaf(i32 %x){ret i32 %x})");
    reject_body("different inferred range",R"(define range(i32 0, 3) i32 @leaf(i32 %x){
      %bit=icmp eq i32 %x,0
      %r=zext i1 %bit to i32
      ret i32 %r })");
    reject_body("additional return attribute",R"(define noundef range(i32 0, 2) i32 @leaf(i32 %x){
      %bit=icmp eq i32 %x,0
      %r=zext i1 %bit to i32
      ret i32 %r })");
    reject_body("parameter attribute",R"(define i32 @leaf(i32 noundef %x){ret i32 %x})");
    reject_body("narrow ABI",R"(define i8 @leaf(i8 %x){ret i8 %x})");
    reject_body("metadata",R"(define i32 @leaf(i32 %x){ %r=add i32 %x,1,!test !0
      ret i32 %r }
      !0=!{i32 1})");
    reject_edge("musttail",R"(define i32 @leaf(i32 %x){ret i32 %x}
      define i32 @caller(i32 %x){ %r=musttail call i32 @leaf(i32 %x)
      ret i32 %r })");
    reject_edge("invoke",R"(declare i32 @personality(...)
      define i32 @leaf(i32 %x){ret i32 %x}
      define i32 @caller(i32 %x) personality ptr @personality {
      entry: %r=invoke i32 @leaf(i32 %x) to label %done unwind label %failure
      done: ret i32 %r
      failure: %landing=landingpad {ptr,i32} cleanup
      resume {ptr,i32} %landing })");
    reject_edge("operand bundle",R"(define i32 @leaf(i32 %x){ret i32 %x}
      define i32 @caller(i32 %x){ %r=call i32 @leaf(i32 %x) ["deopt"(i32 %x)]
      ret i32 %r })");
    reject_edge("callbr",R"(define i32 @leaf(i32 %x){ret i32 %x}
      define i32 @caller(i32 %x){entry:
      callbr void asm sideeffect "", "!i"() to label %normal [label %other]
      normal: ret i32 %x
      other: ret i32 0 })");
    for(unsigned mode{};mode!=5u;++mode)
    {
        ::llvm::LLVMContext context{};auto module=parse(context,positive);
        auto* leaf=module->getFunction("leaf");auto* caller=module->getFunction("caller");
        auto* call=::llvm::cast<::llvm::CallInst>(&caller->front().front());
        if(mode==0u){call->setTailCallKind(::llvm::CallInst::TCK_Tail);}
        if(mode==1u){call->addFnAttr(::llvm::Attribute::NoUnwind);}
        if(mode==2u){call->setMetadata("uwvm.gc.direct.call.v1",::llvm::MDNode::get(context,{}));}
        if(mode==3u){leaf->addFnAttr(::llvm::Attribute::Convergent);}
        if(mode==4u)
        {
            auto* ret=leaf->front().getTerminator();::llvm::IRBuilder<> builder{ret};
            auto* input=&*leaf->arg_begin();::llvm::Value* value=input;
            for(unsigned i{};i!=33u;++i){value=builder.CreateAdd(value,input);}
            ret->setOperand(0,value);
        }
        auto const original=printed(*module);::std::array<::llvm::Function*,2> entries{leaf,caller};
        auto result=experiment::run(*module,entries);
        require(result.state==experiment::status::complete && result.edges==0u,"rejected edge leaves complete graph unchanged");
        require(original==printed(*module),"rejected input byte-for-byte LLVM IR unchanged");
    }
    ::llvm::LLVMContext context{};auto module=parse(context,positive);auto* leaf=module->getFunction("leaf");
    ::std::array<::llvm::Function*,2> duplicates{leaf,leaf};
    require(experiment::run(*module,duplicates).state==experiment::status::invalid_owner,"duplicate owner entry refused");
    ::std::array<::llvm::Function*,1> nulls{nullptr};
    require(experiment::run(*module,nulls).state==experiment::status::invalid_owner,"null owner entry refused");
    ::llvm::LLVMContext other_context{};auto other=parse(other_context,positive);
    ::std::array<::llvm::Function*,1> foreign{other->getFunction("leaf")};
    require(experiment::run(*module,foreign).state==experiment::status::invalid_owner,"foreign actual owner refused");
    auto* ret=leaf->front().getTerminator();::llvm::IRBuilder<> builder{ret};
    ::llvm::Value* value=&*leaf->arg_begin();
    // Existing five instructions plus 27 = exactly the static 32-instruction cap.
    for(unsigned i{};i!=27u;++i){value=builder.CreateAdd(value,&*leaf->arg_begin());}
    ret->setOperand(0,value);require(experiment::eligible(*leaf),"32-instruction inclusive boundary");
    value=builder.CreateAdd(value,&*leaf->arg_begin());ret->setOperand(0,value);
    require(!experiment::eligible(*leaf),"33-instruction boundary refused");
}
int main()
{
    require(!::llvm::InitializeNativeTarget(),"native target");
    require(!::llvm::InitializeNativeTargetAsmPrinter(),"native asm printer");
    rejection_matrix();semantics();semantics(true);::std::printf("PASS integer-leaf LLVM rejection and native A/B checks=%u\n",checks);
}
