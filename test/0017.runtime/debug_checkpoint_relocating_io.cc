#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/imported/wasi/wasip1/func/checkpoint_io.h>
#include <uwvm2/runtime/memtag/host_access.h>
#include <array>
#include <atomic>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <thread>
#include <sys/uio.h>
#include <unistd.h>
namespace linear=uwvm2::object::memory::linear;
namespace phase=uwvm2::runtime::checkpoint::host_memory_phase;
namespace func=uwvm2::imported::wasi::wasip1::func;
namespace mem=uwvm2::imported::wasi::wasip1::memory;
namespace mt=uwvm2::extension::memtag;
namespace rt=uwvm2::runtime::memtag;
using error=uwvm2::imported::wasi::wasip1::abi::errno_t;
static std::atomic<unsigned> checks{};
static void check(bool valid,unsigned line)
{checks.fetch_add(1);if(!valid){std::fprintf(stderr,"relocating IO line=%u\n",line);std::abort();}}
#define CHECK(...) check(bool((__VA_ARGS__)),__LINE__)
struct turns
{
    bool owned{};unsigned begins{},ends{};
    phase::consumer callback{this,
        [](void* value,bool) noexcept {auto& self=*static_cast<turns*>(value);CHECK(!self.owned);self.owned=true;++self.begins;},
        [](void* value) noexcept {auto& self=*static_cast<turns*>(value);CHECK(self.owned);self.owned=false;++self.ends;}};
};
template<class Address,bool Writing>static void run(bool tagged,bool retag)
{
    linear::allocator_memory_t memory;memory.init_by_page_count(1);
    std::memset(memory.memory_begin,0x11,65536);
    using View=mt::shadow_view<Address,true>;
    Address raw=32;
    if(tagged)
    {
        CHECK(memory.enable_memtag(sizeof(Address)==4?mt::address_width::i32:mt::address_width::i64,true)==mt::storage_error::none);
        raw=View::insert(32,3);std::array<Address,2> tags{raw,32};
        CHECK(rt::execute<true,Address,true>(memory,mt::opcode::store,tags));
    }
    auto const old_address=reinterpret_cast<std::uintptr_t>(memory.memory_begin);
    int fds[2]{};CHECK(::pipe(fds)==0);
    std::atomic<bool> relocated{};
    std::array<unsigned char,24> actual{};
    turns trace{};phase::current=&trace.callback;
    {
        phase::scope held{true};auto pin=mem::lock_memory(memory);
        ::iovec rows[2]{{memory.memory_begin+32,16},{memory.memory_begin+48,16}};
        func::checkpoint_scatter_io<Writing,::iovec> io{memory,pin};
        CHECK(memory.active_ops_p->load()==1u);
        CHECK(io.remember(raw,16)&&io.remember(raw+16,16));
        CHECK(io.detach(memory,rows,2)==error::esuccess);
        CHECK(!trace.owned && memory.active_ops_p->load()==0u);
        std::thread peer([&]
        {
            CHECK(phase::current==nullptr);
            // Real grow must complete BEFORE the peer supplies/consumes I/O.
            // Retaining the operation pin would deadlock this test.
            CHECK(memory.grow_strictly(8));
            CHECK(reinterpret_cast<std::uintptr_t>(memory.memory_begin)!=old_address);
            if constexpr(Writing)std::memset(memory.memory_begin+32,0x77,32);
            if(retag)
            {
                std::array<Address,2> tags{View::insert(48,4),16};
                CHECK(rt::execute<true,Address,true>(memory,mt::opcode::store,tags));
            }
            relocated.store(true);relocated.notify_all();
            if constexpr(Writing)CHECK(::read(fds[0],actual.data(),actual.size())==static_cast<ssize_t>(actual.size()));
            else
            {
                actual.fill(0xa5);CHECK(::write(fds[1],actual.data(),actual.size())==static_cast<ssize_t>(actual.size()));
            }
        });
        if constexpr(Writing)
        {
            while(!relocated.load())relocated.wait(false);
            rows[1].iov_len=8;
            CHECK(::writev(fds[1],rows,2)==24);
            rows[1].iov_len=16;
        }
        else CHECK(::readv(fds[0],rows,2)==24);
        peer.join();
        CHECK(memory.active_ops_p->load()==0u);
        auto result=io.commit(memory,24,96,sizeof(Address));
        CHECK(result==(retag && !Writing?error::efault:error::esuccess));
        CHECK(trace.owned && memory.active_ops_p->load()==1u);
        CHECK(rows[0].iov_base==nullptr && rows[1].iov_base==nullptr);
        for(unsigned n{};n<32u;++n)
            CHECK(memory.memory_begin[32+n]==static_cast<std::byte>(Writing?0x77u:retag || n>=24u?0x11u:0xa5u));
        if constexpr(Writing)for(auto byte:actual)CHECK(byte==0x11);
        if(retag && !Writing)CHECK(rt::consume_host_fault().pending);
    }
    CHECK(memory.active_ops_p->load()==0u && !trace.owned && trace.begins==trace.ends);
    {
        phase::scope held{true};auto pin=mem::lock_memory(memory);
        fast_io::io_scatter_t row{memory.memory_begin,4};
        func::checkpoint_scatter_io<false> io{memory,pin};
        CHECK(io.remember(0,4));CHECK(io.detach(memory,&row,1)==error::esuccess);
        std::thread peer([&]{CHECK(memory.grow_strictly(1));});peer.join();
        // A native failure skips commit; the helper destructor must still
        // reacquire exactly once before the original guard's RAII release.
    }
    CHECK(memory.active_ops_p->load()==0u && !trace.owned && trace.begins==trace.ends);
    phase::current=nullptr;CHECK(::close(fds[0])==0);CHECK(::close(fds[1])==0);
}
int main()
{
    for(bool tagged:{false,true})for(bool retag:{false,true})
    {
        if(retag && !tagged)continue;
        run<std::uint32_t,false>(tagged,retag);run<std::uint32_t,true>(tagged,retag);
        run<std::uint64_t,false>(tagged,retag);run<std::uint64_t,true>(tagged,retag);
    }
    std::printf("PASS %u relocating IO checks\n",checks.load());
}
#include <uwvm2/utils/macro/pop_macros.h>
