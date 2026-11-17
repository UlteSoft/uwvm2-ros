/** @file
 * @brief Exercise actual exchange frame transfers across bounds, tags and growth.
 * @details The destination uses real VM memory backends. A mismatch confined
 * to the final payload granule must leave the prefix and payload untouched.
 * Overflow/null inputs must refuse before dereference. Shared writes race
 * actual allocator growth under TSan; no plain concurrent guest byte is used.
 * Identity, exposure, grants and erasure are covered by the full VM suite.
 */
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/runtime/lib/uwvm_runtime_ros_exchange_frame.h>
#include <array>
#include <cassert>
#include <cstdio>
#include <thread>
namespace mt=uwvm2::extension::memtag;
namespace rt=uwvm2::runtime::memtag;
namespace linear=uwvm2::object::memory::linear;
using uwvm2::runtime::lib::exchange_memory_details::write_frame;
using uwvm2::runtime::lib::exchange_memory_details::read_input;

template<class Memory>
static void consume(Memory& memory,std::uint64_t raw,std::size_t normalized,
    std::span<std::byte const> prefix,std::span<std::byte const> payload)
{
    std::array<std::byte,51> output;output.fill(std::byte{0xd7});
    assert(read_input(memory,raw,output.data()+1,prefix.size()+payload.size()));
    assert(output.front()==std::byte{0xd7}&&output.back()==std::byte{0xd7});
    for(std::size_t i{};i!=prefix.size();++i)assert(output[i+1]==prefix[i]);
    for(std::size_t i{};i!=payload.size();++i)assert(output[1+prefix.size()+i]==payload[i]);
    for(std::size_t i{};i!=prefix.size()+payload.size();++i)assert(memory.memory_begin[normalized+i]==std::byte{});
}

template<class Address,class Memory>
static void exercise(Memory& memory)
{
    memory.init_by_page_count(1);
    std::array<std::byte,16> prefix;prefix.fill(std::byte{0x29});
    std::array<std::byte,33> payload;payload.fill(std::byte{0xc5});
    auto verify=[&](std::size_t offset)
    {
        for(std::size_t i{};i!=prefix.size();++i)assert(memory.memory_begin[offset+i]==prefix[i]);
        for(std::size_t i{};i!=payload.size();++i)assert(memory.memory_begin[offset+prefix.size()+i]==payload[i]);
    };
    assert(write_frame(memory,32,prefix.data(),prefix.size(),payload.data(),payload.size()));verify(32);
    std::array<std::byte,49> retained;
    assert(read_input(memory,32,retained.data(),retained.size(),false));verify(32);
    assert(std::memcmp(retained.data(),memory.memory_begin+32,retained.size())==0);
    consume(memory,32,32,prefix,payload);
    std::array<std::byte,49> output;output.fill(std::byte{0x83});
    for(bool clear:{false,true})
    {
        assert(!read_input(memory,65520,output.data(),17,clear));
        assert(!read_input(memory,UINT64_MAX,output.data(),1,clear));
        assert(!read_input(memory,0,nullptr,1,clear));
        assert(!read_input(memory,1,output.data(),SIZE_MAX,clear));
        assert(read_input(memory,65536,nullptr,0,clear));
        assert(!read_input(memory,65537,nullptr,0,clear));
    }
    assert(!read_input(memory,65520,output.data(),17));
    assert(!read_input(memory,UINT64_MAX,output.data(),1));
    assert(!read_input(memory,0,nullptr,1));
    assert(!read_input(memory,1,output.data(),SIZE_MAX));
    assert(read_input(memory,65536,nullptr,0));
    assert(!read_input(memory,65537,nullptr,0));
    for(auto byte:output)assert(byte==std::byte{0x83});
    assert(!write_frame(memory,65520,prefix.data(),16,payload.data(),1));
    for(std::size_t i=65520;i!=65536;++i)assert(memory.memory_begin[i]==std::byte{});
    assert(!write_frame(memory,UINT64_MAX,prefix.data(),1,nullptr,0));
    assert(!write_frame(memory,0,prefix.data(),SIZE_MAX,payload.data(),1));
    assert(!write_frame(memory,0,nullptr,1,payload.data(),1));
    assert(!write_frame(memory,0,prefix.data(),1,nullptr,1));
    assert(write_frame(memory,65536,nullptr,0,nullptr,0));
    assert(!write_frame(memory,65537,nullptr,0,nullptr,0));
    assert(write_frame(memory,0,nullptr,0,payload.data(),payload.size()));
    assert(write_frame(memory,0,prefix.data(),prefix.size(),nullptr,0));

    constexpr bool shared=Memory::support_multi_thread;
    assert(memory.enable_memtag(sizeof(Address)==4?mt::address_width::i32:mt::address_width::i64,shared)==mt::storage_error::none);
    using View=mt::shadow_view<Address,shared>;
    auto raw=View::insert(32,7);
    std::array<Address,2> mark{raw,64};
    assert((rt::execute<true,Address,shared>(memory,mt::opcode::store,mark)));
    assert(write_frame(memory,raw,prefix.data(),prefix.size(),payload.data(),payload.size()));verify(32);
    // The failing tag is only in the last payload byte. No earlier prefix or
    // payload byte may be copied before this full-range check has succeeded.
    mark={View::insert(80,8),16};
    assert((rt::execute<true,Address,shared>(memory,mt::opcode::store,mark)));
    std::array<std::byte,49> before;std::memcpy(before.data(),memory.memory_begin+32,before.size());
    prefix.fill(std::byte{0x61});payload.fill(std::byte{0x92});
    assert(!write_frame(memory,raw,prefix.data(),prefix.size(),payload.data(),payload.size()));
    assert(std::memcmp(before.data(),memory.memory_begin+32,before.size())==0);
    assert(!read_input(memory,raw,output.data(),output.size()));
    assert(!read_input(memory,raw,output.data(),output.size(),false));
    assert(std::memcmp(before.data(),memory.memory_begin+32,before.size())==0);
    for(auto byte:output)assert(byte==std::byte{0x83});
    if constexpr(sizeof(Address)==4)
    {
        assert(!write_frame(memory,(std::uint64_t{1}<<32)|raw,prefix.data(),1,nullptr,0));
        assert(!read_input(memory,(std::uint64_t{1}<<32)|raw,output.data(),1));
        assert(!read_input(memory,(std::uint64_t{1}<<32)|raw,output.data(),1,false));
    }
    mark={View::insert(80,7),16};
    assert((rt::execute<true,Address,shared>(memory,mt::opcode::store,mark)));
    assert(write_frame(memory,raw,prefix.data(),prefix.size(),payload.data(),payload.size()));verify(32);
    assert(read_input(memory,raw,retained.data(),retained.size(),false));verify(32);
    assert(std::memcmp(retained.data(),memory.memory_begin+32,retained.size())==0);
    consume(memory,raw,32,prefix,payload);
    assert(write_frame(memory,raw,prefix.data(),prefix.size(),payload.data(),payload.size()));
    if constexpr(Memory::support_multi_thread)
    {
        std::atomic<bool> go{},okay{true};
        std::thread writer([&]
        {
            while(!go.load(std::memory_order_acquire))std::this_thread::yield();
            for(unsigned i{};i!=2000;++i)
            {
                std::array<std::byte,49> received;
                if(!read_input(memory,raw,received.data(),received.size(),i%2==0))okay.store(false);
                for(std::size_t n{};n!=prefix.size();++n)if(received[n]!=prefix[n])okay.store(false);
                for(std::size_t n{};n!=payload.size();++n)if(received[n+prefix.size()]!=payload[n])okay.store(false);
                if(!write_frame(memory,raw,prefix.data(),prefix.size(),payload.data(),payload.size()))okay.store(false);
            }
        });
        go.store(true,std::memory_order_release);
        for(unsigned i{};i!=4;++i)assert(memory.grow_strictly(1));
        writer.join();assert(okay.load());verify(32);
        if constexpr(!Memory::can_mmap)assert(memory.active_ops_p->load()==0);
    }
    else assert(memory.grow_strictly(1));
    assert(write_frame(memory,65536,prefix.data(),prefix.size(),payload.data(),payload.size()));verify(65536);
    consume(memory,65536,65536,prefix,payload);
}
int main()
{
    {linear::single_thread_allocator_memory_t memory;exercise<std::uint32_t>(memory);}
    {linear::allocator_memory_t memory;exercise<std::uint32_t>(memory);}
    {linear::allocator_memory_t memory;exercise<std::uint64_t>(memory);}
#if defined(UWVM_SUPPORT_MMAP)
    {linear::mmap_memory_t memory;exercise<std::uint32_t>(memory);}
    {linear::mmap_memory_t memory(65536,linear::mmap_memory_status_t::wasm64);exercise<std::uint64_t>(memory);}
#endif
    std::puts("PASS complete exchange frames: atomic bytes, full-range refusal, tagged32/64 and concurrent growth");
}
#include <uwvm2/utils/macro/pop_macros.h>
