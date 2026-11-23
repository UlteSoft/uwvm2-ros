#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/imported/wasi/wasip1/func/checkpoint_io.h>
#include <uwvm2/runtime/memtag/host_access.h>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <thread>
#include <sys/uio.h>
namespace linear=uwvm2::object::memory::linear;
namespace phase=uwvm2::runtime::checkpoint::host_memory_phase;
namespace func=uwvm2::imported::wasi::wasip1::func;
namespace mt=uwvm2::extension::memtag;
namespace rt=uwvm2::runtime::memtag;
using error=uwvm2::imported::wasi::wasip1::abi::errno_t;
static unsigned checks{};
static void check(bool valid,unsigned line){++checks;if(!valid){std::fprintf(stderr,"detached IO line=%u\n",line);std::abort();}}
#define CHECK(...) check(bool((__VA_ARGS__)),__LINE__)
struct turns
{
    bool owned{};unsigned begins{},ends{};
    phase::consumer callback{this,
        [](void* value,bool) noexcept {auto& self=*static_cast<turns*>(value);CHECK(!self.owned);self.owned=true;++self.begins;},
        [](void* value) noexcept {auto& self=*static_cast<turns*>(value);CHECK(self.owned);self.owned=false;++self.ends;}};
};
template<class Address>static void run()
{
    linear::mmap_memory_t memory(65536,sizeof(Address)==4?linear::mmap_memory_status_t::wasm32:linear::mmap_memory_status_t::wasm64);
    memory.init_by_page_count(1,2);std::memset(memory.memory_begin,0x11,65536);
    turns trace{};phase::current=&trace.callback;
    {
        phase::scope held{true};func::checkpoint_scatter_io<true> io{};
        fast_io::io_scatter_t rows[2]{{memory.memory_begin+16,8},{memory.memory_begin+24,8}};
        CHECK(io.remember(16,8)&&io.remember(24,8));CHECK(io.detach(memory,rows,2)==error::esuccess);
        CHECK(!trace.owned && rows[0].base!=memory.memory_begin+16);
        std::memset(memory.memory_begin+16,0x77,16);
        for(auto const& row:rows)for(unsigned i{};i<row.len;++i)CHECK(static_cast<std::byte const*>(row.base)[i]==std::byte{0x11});
        CHECK(io.commit(memory,7,96,sizeof(Address))==error::esuccess);CHECK(trace.owned);
        CHECK(rows[0].base==memory.memory_begin+16);for(unsigned i{};i<16;++i)CHECK(memory.memory_begin[16+i]==std::byte{0x77});
    }
    CHECK(!trace.owned && trace.begins==trace.ends);
    {
        phase::scope held{true};func::checkpoint_scatter_io<false> io{};
        fast_io::io_scatter_t rows[2]{{memory.memory_begin+16,8},{memory.memory_begin+20,8}};
        CHECK(io.remember(16,8)&&io.remember(20,8));CHECK(io.detach(memory,rows,2)==error::esuccess);
        std::memset(const_cast<void*>(rows[0].base),0xaa,8);std::memset(const_cast<void*>(rows[1].base),0xbb,8);
        memory.memory_begin[64]=std::byte{0x33};CHECK(io.commit(memory,11,96,sizeof(Address))==error::esuccess);
        for(unsigned i{};i<8;++i)CHECK(memory.memory_begin[16+i]==(i>=4&&i<7?std::byte{0xbb}:std::byte{0xaa}));
        CHECK(memory.memory_begin[24]==std::byte{0x77}&&memory.memory_begin[64]==std::byte{0x33});
    }
    auto saved=std::array<std::byte,16>{};std::memcpy(saved.data(),memory.memory_begin+16,16);
    {
        phase::scope held{true};func::checkpoint_scatter_io<false> io{};
        fast_io::io_scatter_t rows[2]{{memory.memory_begin+16,8},{memory.memory_begin+65535,8}};
        CHECK(io.remember(16,8)&&io.remember(65535,8));CHECK(io.detach(memory,rows,2)==error::esuccess);
        std::memset(const_cast<void*>(rows[0].base),0xcc,8);std::memset(const_cast<void*>(rows[1].base),0xdd,8);
        CHECK(io.commit(memory,16,96,sizeof(Address))==error::efault);
        CHECK(!std::memcmp(saved.data(),memory.memory_begin+16,16));
    }
    CHECK(memory.enable_memtag(sizeof(Address)==4?mt::address_width::i32:mt::address_width::i64,true)==mt::storage_error::none);
    using View=mt::shadow_view<Address,true>;auto raw=View::insert(32,3);
    std::array<Address,2> tags{raw,32};CHECK(rt::execute<true,Address,true>(memory,mt::opcode::store,tags));
    {
        phase::scope held{true};func::checkpoint_scatter_io<false,::iovec> io{};
        ::iovec rows[2]{{memory.memory_begin+16,4},{memory.memory_begin+20,4}};
        CHECK(io.remember(16,4)&&io.remember(20,4));CHECK(io.detach(memory,rows,2)==error::esuccess);
        CHECK(!trace.owned && rows[0].iov_base!=memory.memory_begin+16);
        std::memset(rows[0].iov_base,0x42,4);std::memset(rows[1].iov_base,0x43,4);
        // recv has two result buffers. Failure of the second must not publish
        // either payload, even though the kernel has already returned data.
        CHECK(io.commit(memory,8,96,sizeof(Address),65535,2)==error::efault);
        CHECK(trace.owned && !std::memcmp(saved.data(),memory.memory_begin+16,16));
    }
    {
        phase::scope held{true};func::checkpoint_scatter_io<false,::iovec> io{};
        ::iovec rows[2]{{memory.memory_begin+16,4},{memory.memory_begin+20,4}};
        CHECK(io.remember(16,4)&&io.remember(20,4));CHECK(io.detach(memory,rows,2)==error::esuccess);
        std::memset(rows[0].iov_base,0x42,4);std::memset(rows[1].iov_base,0x43,4);
        CHECK(io.commit(memory,6,96,sizeof(Address),112,2)==error::esuccess);
        for(unsigned i{};i<6;++i)CHECK(memory.memory_begin[16+i]==(i<4?std::byte{0x42}:std::byte{0x43}));
        CHECK(rows[0].iov_base==memory.memory_begin+16);
    }
    {
        phase::scope held{true};func::checkpoint_scatter_io<false> io{};
        fast_io::io_scatter_t rows[2]{{memory.memory_begin+32,16},{memory.memory_begin+48,16}};
        CHECK(io.remember(raw,16)&&io.remember(raw+16,16));CHECK(io.detach(memory,rows,2)==error::esuccess);
        std::memset(const_cast<void*>(rows[0].base),0xee,16);std::memset(const_cast<void*>(rows[1].base),0xff,16);
        tags={View::insert(48,4),16};CHECK(rt::execute<true,Address,true>(memory,mt::opcode::store,tags));
        CHECK(io.commit(memory,32,96,sizeof(Address))==error::efault);
        for(unsigned i{};i<32;++i)CHECK(memory.memory_begin[32+i]==std::byte{0x11});
        CHECK(rt::consume_host_fault().pending);
    }
    {
        phase::scope held{true};fast_io::io_scatter_t row{memory.memory_begin+16,4};
        func::checkpoint_scatter_io<false> io{};CHECK(io.remember(16,4));
        CHECK(io.detach(memory,&row,1)==error::esuccess);CHECK(!trace.owned);
        // Error/exception cleanup still reacquires before the outer phase ends.
    }
    CHECK(!trace.owned&&trace.begins==trace.ends);phase::current=nullptr;
    {
        func::checkpoint_scatter_io<false> ordinary{};fast_io::io_scatter_t row{memory.memory_begin,16};
        CHECK(ordinary.remember(UINT64_MAX,SIZE_MAX));CHECK(ordinary.detach(memory,&row,1)==error::esuccess);
        CHECK(row.base==memory.memory_begin);CHECK(ordinary.commit(memory,UINT64_MAX,UINT64_MAX,SIZE_MAX)==error::esuccess);
    }
}
int main()
{
    run<std::uint32_t>();run<std::uint64_t>();
    turns trace{};phase::current=&trace.callback;
    std::thread other([]{CHECK(phase::current==nullptr);});other.join();CHECK(phase::current==&trace.callback);phase::current=nullptr;
    std::printf("PASS %u detached IO checks\n",checks);
}
#include <uwvm2/utils/macro/pop_macros.h>
