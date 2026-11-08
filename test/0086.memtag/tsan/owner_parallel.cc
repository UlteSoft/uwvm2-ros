#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/runtime/memtag/impl.h>
#include <atomic>
#include <thread>
#include <cstdio>
#include <cstdlib>
#include <type_traits>
#include <unistd.h>
namespace mt=uwvm2::extension::memtag;
namespace rt=uwvm2::runtime::memtag;
namespace linear=uwvm2::object::memory::linear;
#define CHECK(...) do{if(!(__VA_ARGS__)){std::fprintf(stderr,"FAIL line=%u\n",__LINE__);std::abort();}}while(false)
template<class Owner,class A> void exercise(unsigned cycle) {
    auto create=[] {
        if constexpr(Owner::can_mmap) return Owner{65536,sizeof(A)==4?linear::mmap_memory_status_t::wasm32:linear::mmap_memory_status_t::wasm64};
        else return Owner{};
    };
    Owner owner=create(),independent=create();
    if constexpr(Owner::can_mmap){owner.init_by_page_count(1,32);independent.init_by_page_count(1,2);}
    else {owner.init_by_page_count(1);independent.init_by_page_count(1);}
    constexpr auto width=sizeof(A)==4?mt::address_width::i32:mt::address_width::i64;
    CHECK(owner.enable_memtag(width,true)==mt::storage_error::none);
    CHECK(independent.enable_memtag(width,true)==mt::storage_error::none);
    auto* alias=&owner;
    using V=mt::shadow_view<A,true>;
    CHECK(owner.memtag_shadow.template view<A,true>(65536).store(V::insert(0,3),256));
    CHECK(!rt::check_access<A,true>(independent,V::insert(0,3),0,1));
    std::atomic<unsigned> ready{},go{},passes{},denials{},tail_reads{};
    auto rendezvous=[&] {
        ready.fetch_add(1,std::memory_order_release);
        while(!go.load(std::memory_order_acquire))std::this_thread::yield();
    };
    auto read=[&](unsigned identity) {
        rendezvous();
        std::printf("READER tid=%ld alias=%u width=%zu cycle=%u\n",static_cast<long>(gettid()),identity,sizeof(A)*8,cycle);
        for(unsigned i=0;i<20000;++i) {
            auto& selected=identity?*alias:owner;
            auto result=rt::with_checked_access<A,true>(selected,V::insert(0,3),0,8,
                [&](std::span<std::byte> bytes) noexcept {
                    auto* data=reinterpret_cast<unsigned char*>(bytes.data());
                    (void)__atomic_load_n(data,__ATOMIC_RELAXED);
                    passes.fetch_add(1,std::memory_order_relaxed);
                });
            if(!result){CHECK(result.error==mt::access_error::tag_mismatch);denials.fetch_add(1,std::memory_order_relaxed);}
            rt::details::with_snapshot(selected,[&](std::span<std::byte> data) noexcept {
                auto view=selected.memtag_shadow.template view<A,true>(data.size());
                CHECK(view.check(static_cast<A>(data.size()-64),0,64));
                CHECK(__atomic_load_n(reinterpret_cast<unsigned char*>(data.data()+data.size()-1),__ATOMIC_RELAXED)==0);
                tail_reads.fetch_add(1,std::memory_order_relaxed);
            });
        }
    };
    std::thread first(read,0),second(read,1);
    std::thread retagger([&] {
        rendezvous();
        for(unsigned i=0;i<20000;++i)
            rt::details::with_snapshot(owner,[&](std::span<std::byte> data) noexcept {
                auto view=owner.memtag_shadow.template view<A,true>(data.size());
                CHECK(view.store(V::insert(0,(i&1)?3:(sizeof(A)==8?19:7)),256));
            });
    });
    std::thread grower([&] {
        rendezvous();
        for(unsigned i=0;i<15;++i) { CHECK(owner.grow_strictly(1));std::this_thread::yield(); }
    });
    while(ready.load(std::memory_order_acquire)!=4)std::this_thread::yield();
    go.store(1,std::memory_order_release);
    first.join();second.join();retagger.join();grower.join();
    CHECK(passes.load()+denials.load()==40000 && tail_reads.load()==40000);
    CHECK(owner.get_page_size()==16 && independent.get_page_size()==1);
    CHECK(independent.memtag_shadow.template view<A,true>(65536).check(0,0,65536));
    CHECK(owner.memtag_shadow.template view<A,true>(16*65536).store(V::insert(0,3),256));
    auto copy=rt::with_checked_copy(owner,*alias,V::insert(64,3),V::insert(0,3),64,
        [](std::span<std::byte> dst,std::span<std::byte> src) noexcept {
            for(unsigned i=0;i<64;++i)__atomic_store_n(reinterpret_cast<unsigned char*>(dst.data()+i),
               __atomic_load_n(reinterpret_cast<unsigned char*>(src.data()+i),__ATOMIC_RELAXED),__ATOMIC_RELAXED);
        });
    CHECK(copy);
    std::printf("PASS backend=%s width=%zu cycle=%u accepted=%u mismatch=%u grew=15 joined=4\n",
        Owner::can_mmap?"mmap":"moving",sizeof(A)*8,cycle,passes.load(),denials.load());
    // Production owner retirement requires enclosing execution/worker leases to
    // have drained. Do not invent concurrent reset support: all workers joined.
    owner.clear();CHECK(!owner.memtag_shadow.enabled());
}
int main(){
    for(unsigned i=0;i<2;++i){
        exercise<linear::allocator_memory_t,std::uint32_t>(i);
        exercise<linear::allocator_memory_t,std::uint64_t>(i);
        exercise<linear::mmap_memory_t,std::uint32_t>(i);
        exercise<linear::mmap_memory_t,std::uint64_t>(i);
    }
    std::puts("PASS 32 actual workers: shared tags/alias/isolation/grow/pinned copy/join-before-retirement");
}
#include <uwvm2/utils/macro/pop_macros.h>
