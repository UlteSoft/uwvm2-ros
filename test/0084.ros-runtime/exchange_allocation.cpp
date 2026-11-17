/** @file @brief Observe zero bytes before allocator release and recoverable allocation failure. */
#include <uwvm2/imported/ros/scene/erased_buffer.h>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <new>
namespace s=uwvm2::imported::ros::scene;
static bool fail_next{},watch_next{};
static void* allocation{};
static std::uint8_t* payload{};
static std::size_t payload_size{},cleared_frees{},failed_allocations{};
extern "C" void* __real_malloc(std::size_t);
extern "C" void __real_free(void*);
extern "C" void* __wrap_malloc(std::size_t bytes)
{
    if(fail_next){fail_next=false;++failed_allocations;return nullptr;}
    void* next=__real_malloc(bytes);
    if(watch_next){watch_next=false;allocation=next;}
    return next;
}
extern "C" void __wrap_free(void* raw)
{
    if(raw&&raw==allocation)
    {
        assert(payload&&payload_size);
        auto const* observed=static_cast<volatile std::uint8_t const*>(payload);
        for(std::size_t i{};i<payload_size;++i)assert(!observed[i]);
        ++cleared_frees;allocation=nullptr;payload=nullptr;payload_size=0;
    }
    __real_free(raw);
}
int main()
{
    {
        s::erased_buffer bytes;watch_next=true;bytes.resize_for_overwrite(4096);
        assert(allocation);payload=bytes.writable().data();payload_size=bytes.view().size();
        std::memset(payload,0xa5,payload_size);
        fail_next=true;bool failed{};
        try{bytes.resize_for_overwrite(8192);}catch(std::bad_alloc const&){failed=true;}
        assert(failed&&failed_allocations==1&&bytes.writable().data()==payload);
        for(auto byte:bytes.view())assert(byte==0xa5);
    }
    assert(cleared_frees==1);
    {
        s::erased_buffer bytes;watch_next=true;bytes.resize_for_overwrite(513);
        assert(allocation);payload=bytes.writable().data();payload_size=bytes.view().size();
        std::memset(payload,0x5a,payload_size);bytes.resize_for_overwrite(16);
        assert(cleared_frees==2); // replacement erases before freeing, too
    }
    // Inline bytes have no allocator free to inspect. Keep the backing storage
    // alive and observe the bytes after explicit destruction and after shrink.
    for(std::size_t size:{1u,8u,24u,511u,512u})
    {
        alignas(s::erased_buffer) std::array<std::byte,sizeof(s::erased_buffer)> storage;
        auto* bytes=::new(storage.data()) s::erased_buffer;
        bytes->resize_for_overwrite(size);auto* original=bytes->writable().data();
        std::memset(original,0xa5,size);bytes->resize_for_overwrite(1);
        for(std::size_t i{};i<size;++i)assert(static_cast<volatile unsigned char*>(original)[i]==0);
        original[0]=0x5a;bytes->~erased_buffer();
        for(std::size_t i{};i<size;++i)assert(static_cast<volatile unsigned char*>(original)[i]==0);
    }
    {
        s::erased_buffer bytes;bytes.resize_for_overwrite(512);auto* original=bytes.writable().data();
        std::memset(original,0x5a,512);fail_next=true;bool failed{};
        try{bytes.resize_for_overwrite(513);}catch(std::bad_alloc const&){failed=true;}
        assert(failed&&bytes.writable().data()==original&&bytes.view().size()==512);
        for(auto byte:bytes.view())assert(byte==0x5a);
        bytes.resize_for_overwrite(513);
        for(std::size_t i{};i<512;++i)assert(static_cast<volatile unsigned char*>(original)[i]==0);
        bytes.resize_for_overwrite(0);assert(bytes.view().empty());
    }
    std::puts("PASS exchange allocation: recoverable failure and overwrite before allocator free at -O3");
}
