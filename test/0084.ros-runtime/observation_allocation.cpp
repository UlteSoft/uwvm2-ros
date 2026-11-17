/** @file
 * @brief Observe report zeroization before free, including partially encoded failures.
 * @details Allocation bytes begin poisoned so the test also detects writes
 * beyond the encoded extent. The saved allocation remains alive until the
 * wrapped free validates it; no access after free is used.
 */
#include <uwvm2/imported/ros/scene/permission_report.h>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <new>
namespace p=uwvm2::imported::ros::scene::permission_report;
static bool watch_next{},fail_next{};
static void* allocation{};
static std::uint8_t const* payload{};
static std::size_t used{},checked{};
extern "C" void* __real_malloc(std::size_t);
extern "C" void __real_free(void*);
extern "C" void* __wrap_malloc(std::size_t bytes)
{
    if(fail_next){fail_next=false;return nullptr;}
    auto result=__real_malloc(bytes);
    if(watch_next&&result){watch_next=false;allocation=result;std::memset(result,0xa5,bytes);}
    return result;
}
extern "C" void __wrap_free(void* raw)
{
    if(raw&&raw==allocation)
    {
        assert(payload&&used>=40&&used<=p::maximum_response);
        auto const* bytes=static_cast<volatile std::uint8_t const*>(payload);
        for(std::size_t i{};i<used;++i)assert(bytes[i]==0);
        for(std::size_t i=used;i<p::maximum_response;++i)assert(bytes[i]==0xa5);
        ++checked;allocation=nullptr;payload=nullptr;
    }
    __real_free(raw);
}
int main()
{
    for(unsigned scenario{};scenario<5;++scenario)
    {
        watch_next=true;p::owned_page owned{scenario==4?33u:0u};auto& page=owned.get();
        assert(allocation);
        if(scenario==0)assert(page.add("self","module",0,"sensitive-name")&&page.finish(true,true));
        if(scenario==1)
        {
            assert(!page.add("private-scope","private-key",0,std::string_view{"\xff",1}));
            assert(!page.finish(true,true)); // previous valid fields must still be erased
        }
        if(scenario==2)
        {
            assert(page.add("scope","key",123));
            for(std::size_t i=1;i<p::maximum_scan;++i)assert(page.add("scope","key",i));
            assert(!page.add("scope","key",999)&&!page.finish(true,true));
        }
        if(scenario==3)
        {
            std::array<char,1024> text;text.fill('s');
            for(unsigned i{};i<32;++i)assert(page.add({text.data(),255},{text.data(),255},i,{text.data(),1024}));
            assert(page.finish(true,true));
        }
        if(scenario==4)assert(page.add("scope","key",0)&&!page.finish(true,true));
        payload=page.bytes().data();used=page.bytes().size();
    }
    assert(checked==5);
    fail_next=true;bool refused{};
    try{p::owned_page denied{0};}catch(std::bad_alloc const&){refused=true;}
    assert(refused&&!allocation&&!payload);
    std::puts("PASS owned report pages: success, partial UTF-8 failure, scan exhaustion, maximum page, invalid cursor and recoverable allocation failure; encoded bytes zero before free");
}
