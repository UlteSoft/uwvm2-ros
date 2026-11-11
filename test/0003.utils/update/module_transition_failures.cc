// Source-bound real parser/plan regression. Does not claim an ARM VM execution.
#include <uwvm2/utils/update/module_transition.h>
#include <array>
#include <cstdio>
#include <vector>
namespace update=uwvm2::utils::update;
using bytes=std::vector<unsigned char>;
static void u(bytes& out,unsigned value)
{do{unsigned byte=value&127u;value>>=7u;out.push_back(static_cast<unsigned char>(byte|(value?128u:0u)));}while(value);}
static void section(bytes& out,unsigned id,bytes payload)
{out.push_back(static_cast<unsigned char>(id));u(out,payload.size());out.insert(out.end(),payload.begin(),payload.end());}
static bytes make(bool changed,bool different_state,unsigned pages=1u,unsigned offset=0u,bool passive=false)
{
    bytes out{0,97,115,109,1,0,0,0};section(out,1,{1,0x60,0,0});section(out,3,{1,0});
    bytes memory{1,1};u(memory,pages);u(memory,pages);section(out,5,memory);
    section(out,6,different_state?bytes{1,0x7d,1,0x43,1,0,0,0,0x0b}:bytes{1,0x7d,1,0x43,0,0,0,0x80,0x0b});
    section(out,10,changed?bytes{1,3,0,1,0x0b}:bytes{1,2,0,0x0b});
    bytes data{2,static_cast<unsigned char>(passive?1:0)};
    if(!passive){data.push_back(0x41);u(data,offset);data.push_back(0x0b);}
    data.insert(data.end(),{4,static_cast<unsigned char>(different_state?'E':'A'),static_cast<unsigned char>(different_state?'F':'B'),static_cast<unsigned char>(different_state?'G':'C'),static_cast<unsigned char>(different_state?'H':'D')});
    data.insert(data.end(),{0,0x41,2,0x0b,2,'x','y'});section(out,11,data);return out;
}
int main()
{
    unsigned failures{},checks{};
    auto check=[&](bool okay,char const* label){++checks;if(!okay){++failures;std::printf("FAIL %s\n",label);}};
    std::array<std::uint64_t,2> scopes{0,update::global_reset_tag};
    auto old=make(false,false),next=make(true,true);update::module_transition_plan plan{};
    check(update::compare_module_transition(old,next,scopes,true,plan)==update::status::ok,"active-data reset admitted");
    check(plan.memories.size()==1 && plan.memories[0].bytes.size()==65536 && plan.memories[0].bytes[0]=='E' &&
        plan.memories[0].bytes[1]=='F' && plan.memories[0].bytes[2]=='x' && plan.memories[0].bytes[3]=='y',"later active segment wins in private state");
    check(plan.scalars.size()==1 && plan.scalars[0].old_bits[3]==0x80 && plan.scalars[0].new_bits[0]==1 &&
        plan.scalars[0].new_bits[2]==0 && plan.scalars[0].new_bits[3]==0,"f32 signed zero / finite subnormal preserved as exact bits");
    check(plan.data_segments.size()==2 && next[plan.data_segments[0].payload_offset]=='E' &&
        next[plan.data_segments[1].payload_offset]=='x',"candidate active-data spans retain actual image coordinates");
    check(update::compare_module_transition(old,next,scopes,false,plan)==update::status::scope_mismatch,"preserve cannot change initial state");
    auto code_only=make(true,false);
    check(update::compare_module_transition(old,code_only,scopes,false,plan)==update::status::ok &&
        plan.memories.size()==1 && plan.memories[0].bytes.empty(),"authorized normal preserve retains state without allocating reset image");
    auto large_old=make(false,false,1025),large_next=make(true,false,1025);
    auto large=update::compare_module_transition(large_old,large_next,scopes,true,plan);
    std::printf("budget classification: %s\n",update::name(large));
    check(large==update::status::exhausted,"private reset image over 64 MiB is resource exhaustion");
    auto invalid=make(true,true,1,65536);
    auto bounds=update::compare_module_transition(old,invalid,scopes,true,plan);
    std::printf("active-data bounds classification: %s\n",update::name(bounds));
    check(bounds==update::status::validation_failed,"invalid active-data bounds is validation failure");
    auto passive_old=make(false,false,1,0,true),passive_next=make(true,false,1,0,true);
    check(update::compare_module_transition(passive_old,passive_next,scopes,true,plan)==update::status::unsupported,"unsupported passive transition remains explicit");
    check(plan.memories.empty() && plan.data_segments.empty(),"failed candidate is never published");
    std::printf("%s %u/%u module transition checks\n",failures?"FAIL":"PASS",checks-failures,checks);
    return failures?1:0;
}
