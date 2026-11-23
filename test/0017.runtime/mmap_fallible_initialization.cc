// Exercise the real mmap owner on all five supported Linux ABIs. No alternate
// allocator, fake byte array or signal handler replaces the production mapping.
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/object/memory/linear/impl.h>
#include <uwvm2/utils/thread/stop_token.h>
#include <fast_io.h>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <utility>
namespace linear=::uwvm2::object::memory::linear;
namespace resource=::uwvm2::utils::allocator::resource;
unsigned checks{};
#define CHECK(c) do{++checks;if(!(c)){::std::fprintf(stderr,"FAIL line %d: %s\n",__LINE__,#c);::std::abort();}}while(false)
int main()
{
    auto const pool=static_cast<unsigned>(resource::pool::memory);
    auto const initial=resource::live_bytes[pool].load();
    for(auto width:{linear::mmap_memory_status_t::wasm32,linear::mmap_memory_status_t::wasm64})for(bool shared:{false,true})
    {
        linear::mmap_memory_t memory{linear::mmap_memory_t::fallible_construction_t{}};
        CHECK(memory.memory_length_p&&memory.growing_mutex_p);
        memory.status=width;memory.sequentially_consistent_size=shared;
        CHECK(!memory.try_init_by_page_count(2,1,7));
        CHECK(!memory.memory_begin&&!memory.reserved_begin&&resource::live_bytes[pool].load()==initial);
        auto saved=resource::policies[pool];resource::policies[pool]={0,0};
        bool denied=!memory.try_init_by_page_count(1,3,7);resource::policies[pool]=saved;
        CHECK(denied&&!memory.memory_begin&&resource::live_bytes[pool].load()==initial);
        CHECK(memory.try_init_by_page_count(1,3,7));
        CHECK(memory.diagnostic_owner_memory_index==7&&memory.get_page_size()==1);
        CHECK(resource::live_bytes[pool].load()==initial+65536u);
        CHECK(!memory.try_init_by_page_count(1,3,7));
        memory.memory_begin[0]=::std::byte{0x51};auto* base=memory.memory_begin;
        auto result=memory.grow_threaded_profile(1,3u*65536u,{});
        CHECK(result.status==linear::threaded_grow_status::success&&result.old_pages==1);
        CHECK(memory.memory_begin==base&&memory.get_page_size()==2&&memory.memory_begin[0]==::std::byte{0x51});
        CHECK(memory.memory_begin[65536]==::std::byte{});
        result=memory.grow_threaded_profile(SIZE_MAX,3u*65536u,{});
        CHECK(result.status==linear::threaded_grow_status::limit&&memory.get_page_size()==2);
        result=memory.grow_threaded_profile(0,3u*65536u,{});
        CHECK(result.status==linear::threaded_grow_status::success&&result.old_pages==2);
        result=memory.grow_threaded_profile(2,3u*65536u,{});
        CHECK(result.status==linear::threaded_grow_status::limit&&memory.get_page_size()==2);
        linear::mmap_memory_t moved{::std::move(memory)};
        CHECK(moved.memory_begin==base&&!memory.memory_begin&&moved.get_page_size()==2);
        memory=::std::move(moved);CHECK(!moved.memory_begin&&memory.memory_begin==base);
        memory.clear();CHECK(!memory.memory_begin&&resource::live_bytes[pool].load()==initial);
        CHECK(memory.try_init_by_page_count(0,1,8)&&memory.get_page_size()==0);
        result=memory.grow_threaded_profile(1,65536u,{});
        CHECK(result.status==linear::threaded_grow_status::success&&result.old_pages==0);
        CHECK(memory.memory_begin[0]==::std::byte{});
    }
    CHECK(resource::live_bytes[pool].load()==initial);
    ::std::printf("PASS fallible mmap: %u checks, memory32/64 shared/private, grow, denial, zero, move, accounting\n",checks);
}
