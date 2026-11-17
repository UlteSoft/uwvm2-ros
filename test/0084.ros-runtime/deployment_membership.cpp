/** @file
 * @brief Check exact environment membership and recoverable allocation failure.
 * @details A failed binding must not grant exposure. Named groups share only
 * their explicitly enrolled modules; a one-module environment remains separate.
 * No source, provider or native device is activated by these checks.
 */
#include <uwvm2/imported/ros/scene/deployment.h>
#include <cassert>
#include <cstdio>

namespace s=uwvm2::imported::ros::scene;
static thread_local std::size_t fail_after{};
static thread_local bool injected{};
static bool fail() noexcept
{if(fail_after&&!--fail_after){injected=true;return true;}return false;}
extern "C" void* __real_malloc(std::size_t);
extern "C" void* __real_calloc(std::size_t,std::size_t);
extern "C" void* __real_realloc(void*,std::size_t);
extern "C" void* __wrap_malloc(std::size_t n){return fail()?nullptr:__real_malloc(n);}
extern "C" void* __wrap_calloc(std::size_t n,std::size_t width){return fail()?nullptr:__real_calloc(n,width);}
extern "C" void* __wrap_realloc(void* p,std::size_t n){return fail()?nullptr:__real_realloc(p,n);}

int main()
{
    constexpr auto environment="robot.control.exact.environment.with.owned.storage";
    constexpr auto module="robot.control.exact.module.with.owned.storage";
    std::size_t failures{};bool completed{};
    for(std::size_t point{1};point<32;++point)
    {
        s::deployment value;
        assert(value.create(environment)==s::error::success);
        assert(value.expose(environment,"abi",s::exposure::allow)==s::error::success);
        uwvm2::utils::container::std_string owned{module};
        s::error result{s::error::invalid};bool threw{};
        injected=false;fail_after=point;
        try{result=value.bind(environment,std::move(owned));}
        catch(std::bad_alloc const&){threw=true;}
        fail_after=0;
        if(injected)
        {
            assert(threw);++failures;
            assert(value.visible(module,"abi")==s::exposure::hidden);
            assert(value.bind(environment,module)==s::error::success);
        }
        else{assert(!threw&&result==s::error::success);completed=true;}
        assert(value.visible(module,"abi")==s::exposure::allow);
        assert(value.bind(environment,module)==s::error::conflict);
        if(completed)break;
    }
    assert(completed&&failures>=2);

    s::deployment groups;
    for(auto name:{"arm","vision","single"})assert(groups.create(name)==s::error::success);
    assert(groups.bind("arm","planner")==s::error::success);
    assert(groups.bind("arm","controller")==s::error::success);
    assert(groups.bind("vision","perception")==s::error::success);
    assert(groups.bind("single","recorder")==s::error::success);
    assert(groups.bind("vision","controller")==s::error::conflict);
    assert(groups.bind("absent","unbound")==s::error::invalid);
    assert(groups.expose("arm","abi",s::exposure::allow)==s::error::success);
    for(auto name:{"planner","controller"})assert(groups.visible(name,"abi")==s::exposure::allow);
    for(auto name:{"perception","recorder","unbound","unknown"})assert(groups.visible(name,"abi")==s::exposure::hidden);
    assert(groups.validate()==s::error::success);
    assert(groups.bind("arm","late")!=s::error::success&&groups.visible("late","abi")==s::exposure::hidden);
    groups.retire();assert(groups.visible("controller","abi")==s::exposure::hidden);
    std::printf("PASS exact membership, independent groups and %zu recoverable allocation failures\n",failures);
}
