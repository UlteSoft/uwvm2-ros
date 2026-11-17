/** @file
 * @brief Real supervisor rollback after injected stack and pthread failures.
 * @details Only the selected host syscall boundary fails; all preceding
 * threads, guarded stacks, sealed images and joins are real. No component
 * start is authorized. The reference must expose retained threads or permit
 * reuse after failure. Join callbacks verify that inspection locks are free.
 */
#define UWVM_COMPONENT_DISPATCH_TEST_MAIN previous_component_main
#include "component_dispatch.cpp"
#include <cstring>

namespace prepare_fault
{
    enum class point {none,guard,attribute,stack,thread};
    point selected{};
    unsigned ordinal{},observed{};
    std::atomic<unsigned> created{},joined{},attributes{},destroyed{};
    s::component_dispatch* target{};
    bool inspecting{};
    // Sanitizer runtimes also initialize pthread attributes. Interpose only
    // direct calls on the test's explicit owner/retirement threads, excluding
    // nested runtime work inside the real pthread entry point.
    thread_local bool enabled{};
    thread_local unsigned depth{};
    struct boundary
    {
        bool active{enabled&&depth==0};
        boundary()noexcept{++depth;}
        ~boundary(){--depth;}
    };
    bool hit(point value)noexcept{return selected==value&&++observed==ordinal;}
}
extern "C" int __real_mprotect(void*,std::size_t,int)noexcept;
extern "C" int __wrap_mprotect(void* address,std::size_t count,int protection)noexcept
{
    prepare_fault::boundary scope;
    if(scope.active&&protection==PROT_NONE&&prepare_fault::hit(prepare_fault::point::guard)){errno=ENOMEM;return -1;}
    return __real_mprotect(address,count,protection);
}
extern "C" int __real_pthread_attr_init(pthread_attr_t*)noexcept;
extern "C" int __wrap_pthread_attr_init(pthread_attr_t* value)noexcept
{
    prepare_fault::boundary scope;
    if(scope.active&&prepare_fault::hit(prepare_fault::point::attribute))return ENOMEM;
    auto status=__real_pthread_attr_init(value);if(scope.active&&!status)++prepare_fault::attributes;return status;
}
extern "C" int __real_pthread_attr_destroy(pthread_attr_t*)noexcept;
extern "C" int __wrap_pthread_attr_destroy(pthread_attr_t* value)noexcept
{prepare_fault::boundary scope;auto status=__real_pthread_attr_destroy(value);if(scope.active&&!status)++prepare_fault::destroyed;return status;}
extern "C" int __real_pthread_attr_setstack(pthread_attr_t*,void*,std::size_t)noexcept;
extern "C" int __wrap_pthread_attr_setstack(pthread_attr_t* attr,void* memory,std::size_t size)noexcept
{
    prepare_fault::boundary scope;
    if(scope.active&&prepare_fault::hit(prepare_fault::point::stack))return EINVAL;
    return __real_pthread_attr_setstack(attr,memory,size);
}
extern "C" int __real_pthread_create(pthread_t*,pthread_attr_t const*,void*(*)(void*),void*)noexcept;
extern "C" int __wrap_pthread_create(pthread_t* thread,pthread_attr_t const* attr,void*(*entry)(void*),void* data)noexcept
{
    prepare_fault::boundary scope;
    if(scope.active&&prepare_fault::hit(prepare_fault::point::thread))return EAGAIN;
    auto status=__real_pthread_create(thread,attr,entry,data);if(scope.active&&!status)++prepare_fault::created;return status;
}
extern "C" int __real_pthread_join(pthread_t,void**);
extern "C" int __wrap_pthread_join(pthread_t thread,void** result)
{
    prepare_fault::boundary scope;
    if(scope.active&&prepare_fault::inspecting)
    {
        s::component_dispatch::inspection usage;
        assert(prepare_fault::target->inspect("component",usage)==error::success);
        assert(usage.retired&&!usage.handles&&!usage.processes&&!usage.messages);
    }
    auto status=__real_pthread_join(thread,result);if(scope.active&&!status)++prepare_fault::joined;return status;
}
static void reset_fault(prepare_fault::point point,unsigned ordinal)
{
    prepare_fault::enabled=true;
    prepare_fault::selected=point;prepare_fault::ordinal=ordinal;prepare_fault::observed=0;
    prepare_fault::created=0;prepare_fault::joined=0;prepare_fault::attributes=0;prepare_fault::destroyed=0;
}
static void bind_three(s::component_dispatch& target,component_fixture const& fixture)
{
    auto plan=fixture.policy;
    for(auto alias:{"component","component.two","component.three"})
    {plan.alias.assign(alias);assert(target.bind(plan,fixture.images)==error::success);}
}
static void failed_prepare(component_fixture& fixture,prepare_fault::point point,unsigned ordinal)
{
    s::component_dispatch target{fixture.permissions,fixture.inputs,s::bridge_dispatch::transport::from(fixture),
        73,{4,4*fixture.policy.retained_bytes}};
    bind_three(target,fixture);reset_fault(point,ordinal);
    prepare_fault::target=&target;prepare_fault::inspecting=true;
    auto status=target.prepare();prepare_fault::selected=prepare_fault::point::none;
    if(status!=error::io)std::fprintf(stderr,"preparation status=%u injected=%u observed=%u created=%u joined=%u attributes=%u destroyed=%u\n",
        static_cast<unsigned>(status),ordinal,prepare_fault::observed,prepare_fault::created.load(),prepare_fault::joined.load(),
        prepare_fault::attributes.load(),prepare_fault::destroyed.load());
    assert(status==error::io);
    auto failed_row=point==prepare_fault::point::guard?(ordinal-1)/2:(ordinal-1);
    assert(prepare_fault::created==failed_row);
    assert(prepare_fault::joined==failed_row);
    assert(prepare_fault::attributes==prepare_fault::destroyed);
    for(auto alias:{"component","component.two","component.three"})
    {
        s::component_dispatch::inspection usage;assert(target.inspect(alias,usage)==error::success);
        assert(usage.retired&&!usage.ready&&!usage.handles&&!usage.processes&&!usage.exit_observed&&!usage.messages&&!usage.unknown);
    }
    assert(target.prepare()==error::denied);
    auto plan=fixture.policy;plan.alias.assign("component.four");assert(target.bind(plan,fixture.images)==error::denied);
    assert(target.preflight(fixture.who,cop::component_open,fixture.component_cap,0)==error::denied);
    target.retire();target.retire();assert(prepare_fault::joined==failed_row);
    prepare_fault::inspecting=false;prepare_fault::target=nullptr;
    assert(!fixture.native->publisher->publications);
}
static void normal_prepare(component_fixture& fixture)
{
    s::component_dispatch target{fixture.permissions,fixture.inputs,s::bridge_dispatch::transport::from(fixture),
        73,{3,3*fixture.policy.retained_bytes}};
    bind_three(target,fixture);reset_fault(prepare_fault::point::none,0);
    assert(target.prepare()==error::success&&prepare_fault::created==3&&prepare_fault::joined==0);
    assert(target.prepare()==error::denied);
    assert(target.preflight(fixture.who,cop::component_open,fixture.component_cap,0)==error::success);
    target.retire();target.retire();assert(prepare_fault::joined==3&&prepare_fault::attributes==prepare_fault::destroyed);
    s::component_dispatch stopped{fixture.permissions,fixture.inputs,{},74,{1,fixture.policy.retained_bytes}};
    stopped.request_stop();assert(stopped.prepare()==error::denied&&prepare_fault::created==3);
}
static void racing_retirement(component_fixture& fixture)
{
    for(unsigned iteration{};iteration<24;++iteration)
    {
        s::component_dispatch target{fixture.permissions,fixture.inputs,s::bridge_dispatch::transport::from(fixture),
            75,{3,3*fixture.policy.retained_bytes}};
        bind_three(target,fixture);reset_fault(prepare_fault::point::none,0);
        struct input{s::component_dispatch* target;std::atomic<bool> begin{};} input{&target};
        pthread_t retiring{};
        assert(!__real_pthread_create(&retiring,nullptr,[](void* raw)->void*
        {auto& value=*static_cast<struct input*>(raw);while(!value.begin.load())::sched_yield();prepare_fault::enabled=true;value.target->retire();return nullptr;},&input));
        input.begin.store(true);auto status=target.prepare();assert(status==error::success||status==error::denied);
        assert(!__real_pthread_join(retiring,nullptr));target.retire();
        assert(prepare_fault::created==prepare_fault::joined&&prepare_fault::attributes==prepare_fault::destroyed);
        assert(target.prepare()==error::denied);
    }
}
int main(int argc,char** argv)
{
    assert(argc==12);component_fixture fixture{argv+1,1};fixture.component->retire();
    std::string_view mode{argv[10]};unsigned ordinal=static_cast<unsigned>(argv[11][0]-'0');
    if(mode=="success")normal_prepare(fixture);
    else if(mode=="race")racing_retirement(fixture);
    else
    {
        using point=prepare_fault::point;
        auto selected=mode=="guard"?point::guard:mode=="attribute"?point::attribute:mode=="stack"?point::stack:point::thread;
        assert(mode=="guard"||mode=="attribute"||mode=="stack"||mode=="thread");
        assert(ordinal>=1&&ordinal<=(selected==point::guard?6u:3u));failed_prepare(fixture,selected,ordinal);
    }
    std::printf("PASS component preparation %s %u\n",argv[10],ordinal);
}
