#include <uwvm2/utils/control/host_diagnostics.h>
#include <cassert>
#include <cstdlib>
#include <new>
#include <iostream>
static bool fail_allocation{};
void* operator new(std::size_t n) { if(fail_allocation)throw std::bad_alloc{}; if(auto p=std::malloc(n?n:1))return p; throw std::bad_alloc{}; }
void operator delete(void* p) noexcept {std::free(p);}
void operator delete(void* p,std::size_t) noexcept {std::free(p);}
namespace d=uwvm2::utils::control::host_diagnostics;
namespace a=uwvm2::utils::control::host_interfaces;
static unsigned fatal_count{},sink_count{};
static std::string output;
static void receive(std::string_view value) noexcept {++sink_count;try{output=value;}catch(...){}}
int main()
{
    d::sink=receive;d::fatal=+[]() noexcept{++fatal_count;};
    constexpr auto random=a::find(a::family::wasip1,std::string_view{"random_get"});
    constexpr auto ros=a::find(a::family::ros2,std::string_view{"uwvm_ros2_v1.abi_version"});
    static_assert(random<a::invalid && ros<a::invalid);
    assert(d::selected(random,d::event::denied)==d::severity::warning);
    assert(d::selected(random,d::event::limited)==d::severity::off);
    assert(!d::configure(std::string_view{"all_host_calls_typo"},d::severity::off));
    assert(!d::configure(std::string_view{"host_limited_ros2.random_get"},d::severity::error));
    assert(d::configure(std::string_view{"all_host_calls"},d::severity::off));
    assert(d::configure(std::string_view{"all_host_limited_wasip1"},d::severity::warning));
    assert(d::selected(random,d::event::denied)==d::severity::off);
    assert(d::selected(ros,d::event::limited)==d::severity::off);
    {
        d::call_scope call{random,"single","main"};
        d::limited("random.window-calls",true);
        d::limited("random.window-calls",true);
        assert(sink_count==0);
        call.flush();assert(sink_count==1);call.flush();assert(sink_count==1);
        assert(output.find("synchronously waiting")!=output.npos);
        assert(output.find("--wasip1-single-limit 'main' random.window-calls")!=output.npos);
        { d::call_scope silent{ros,"single","worker"};d::limited("max-rate-hz",false); }
        assert(sink_count==1);
        d::limited("random.window-calls",false);
    }
    assert(sink_count==2 && d::active==nullptr);
    assert(d::configure(std::string_view{"host_limited_wasip1.random_get"},d::severity::error));
    {
        d::call_scope call{random,"single","main"};
        fail_allocation=true;d::limited("random.window-calls",false);
        call.flush();fail_allocation=false;
    }
    assert(fatal_count==1);
    assert(d::configure(std::string_view{"host_denied_wasip1.random_get"},d::severity::error));
    fail_allocation=true;d::report(random,d::event::denied,"single","main","denied");fail_allocation=false;
    assert(fatal_count==2);
    std::string name(20000,'x');name+="\n\x1b'";auto escaped=d::quoted(name);
    assert(escaped.size()>20000 && escaped.find('\n')==escaped.npos && escaped.find('\x1b')==escaped.npos);
    a::policy p;assert(!p.exposed(random));assert(p.set(random,a::access::deny));
    assert(p.exposed(random)&&!p.allowed(random));assert(!p.set(random,a::access::allow));
    std::cout<<"host diagnostic allocation, selector and policy checks passed\n";
}
