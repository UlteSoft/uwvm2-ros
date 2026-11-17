#include <uwvm2/uwvm/debugger/wasithread_state.h>
#include <uwvm2/utils/control/wasithread_system.h>
#include <cassert>
#include <string>
namespace d=uwvm2::uwvm::debugger::wasithread_state;
namespace p=uwvm2::utils::control::debug_permissions;
namespace t=uwvm2::utils::control::wasithread;
int main(int argc,char** argv)
{
    assert(argc==2);std::string_view mode=argv[1];p::permission_level permission;
    if(mode=="full")permission=p::permission_level::full;
    else if(mode=="read-only")permission=p::permission_level::read_only;
    else if(mode=="none")permission=p::permission_level::none;
    else{assert(mode=="limited");permission=p::permission_level::limited;}
    assert(p::configure_trusted_launch(permission));
    auto& policy=t::runtime_policy();assert(policy.create(t::scope::group,u8"workers"));
    assert(policy.bind(t::scope::group,u8"workers",u8"module with spaces"));
    assert(policy.bind(t::scope::group,u8"workers",u8"robot-机器人"));
    assert(policy.enable(t::scope::group,u8"workers",true));
    assert(policy.expose(t::scope::group,u8"workers",t::exposure::allow));
    auto main=policy.reserve(u8"module with spaces",false);assert(main.held);
    auto child=policy.reserve(u8"robot-机器人",true);assert(child.held);
    d::request query;assert(d::parse("info wasithread policy 0 8 11",query));
    for(auto bad:{"info wasithread allow 0 8 11","info wasithread policy 0 0 11", "info wasithread policy 0 9 11",
                 "info wasithread policy 0 8 0","info wasithread policy -1 8 11", "info wasithread policy 1x 8 11",
                 "info wasithread policy 18446744073709551616 8 11", "info wasithread policy 0 8 11\ncontinue"})
        assert(!d::parse(fast_io::string_view{bad,std::char_traits<char>::length(bad)},query));
    d::view value;d::read(query,value);
    bool inspection=mode=="full"||mode=="read-only";
    assert((value.result==d::status::ok)==inspection);
    if(!inspection){assert(value.size==0);return 0;}
    assert(value.total==2&&value.size==2&&value.sealed);
    assert(value.policies[0].occupied.mains==1&&value.policies[0].occupied.children==1);
    assert(value.policies[1].occupied.children==1);
    auto out=fast_io::out();d::print(out,value,11);
    query.selected=d::selection::members;d::read(query,value);d::print(out,value,11);
    assert(policy.inspect(t::scope::global).occupied.spawn_admissions==1);
    assert(!policy.enable(t::scope::group,u8"workers",false));
}
