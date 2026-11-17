/** @file
 * @brief Exercise the production two-extent erase under a counted memory pin.
 * @details The pin is test-owned; bounds and atomic stores use the exact runtime
 * helper. This is separate from real-VM lifetime, growth and tag qualification.
 */
#include <array>
#include <atomic>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <pthread.h>
namespace uwvm2::runtime::lib
{
    struct native_memory_t {std::byte* begin;std::size_t extent;bool allowed{true};};
    static unsigned pins{};
    template<class Callback> bool with_native_preload_copy_access(native_memory_t const& memory,Callback callback) noexcept
    {++pins;return memory.allowed&&callback(memory.begin,memory.extent);}
    #include <uwvm2/runtime/lib/uwvm_runtime_ros_exchange_erase.h>
}
namespace r=uwvm2::runtime::lib;
static std::array<std::byte,66> storage;
static std::atomic<bool> stopping{};
static void* stores(void*)
{
    while(!stopping.load())for(unsigned i=1;i<=64;++i)std::atomic_ref<std::byte>{storage[i]}.store(std::byte{0xa5},std::memory_order_relaxed);
    return nullptr;
}
int main()
{
    auto fill=[](){storage.fill(std::byte{0xa5});r::pins=0;};
    auto erased=[](unsigned begin,unsigned count){for(auto i=begin;i<begin+count;++i)assert(storage[1+i]==std::byte{});};
    auto intact=[](unsigned begin,unsigned count){for(auto i=begin;i<begin+count;++i)assert(storage[1+i]==std::byte{0xa5});};
    auto guards=[](){assert(storage.front()==std::byte{0xa5}&&storage.back()==std::byte{0xa5}&&r::pins==1);};
    r::native_memory_t memory{storage.data()+1,64};
    fill();assert(r::exchange_memory_details::erase(memory,0,24,56,8));erased(0,24);intact(24,32);erased(56,8);guards();
    fill();assert(!r::exchange_memory_details::erase(memory,63,2,56,8));intact(0,56);erased(56,8);guards();
    fill();assert(!r::exchange_memory_details::erase(memory,0,24,63,2));erased(0,24);intact(24,40);guards();
    fill();assert(!r::exchange_memory_details::erase(memory,UINT64_MAX,8,56,8));intact(0,56);erased(56,8);guards();
    fill();assert(!r::exchange_memory_details::erase(memory,0,UINT64_MAX,56,8));intact(0,56);erased(56,8);guards();
    fill();assert(!r::exchange_memory_details::erase(memory,0,24,UINT64_MAX,8));erased(0,24);intact(24,40);guards();
    fill();assert(r::exchange_memory_details::erase(memory,64,0,UINT64_MAX,0));intact(0,64);guards();
    fill();memory.allowed=false;assert(!r::exchange_memory_details::erase(memory,0,24,56,8));intact(0,64);guards();memory.allowed=true;
    fill();assert(r::exchange_memory_details::erase(memory,0,24,16,8));erased(0,24);intact(24,40);guards();
    fill();pthread_t writer{};assert(!pthread_create(&writer,nullptr,stores,nullptr));
    for(unsigned i{};i<10000;++i)assert(r::exchange_memory_details::erase(memory,0,56,56,8));
    stopping.store(true);assert(!pthread_join(writer,nullptr));r::pins=0;
    assert(r::exchange_memory_details::erase(memory,0,56,56,8));erased(0,64);guards();
    std::puts("PASS paired erasure: independent bounds, overflow, partial failure, absent extent, one pin and concurrent atomic stores");
}
