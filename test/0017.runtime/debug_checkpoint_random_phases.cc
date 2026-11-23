// Real entropy acquisition with a concurrent guest-memory writer. No mock I/O.
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/imported/wasi/wasip1/func/random_get.h>
#include <uwvm2/runtime/memtag/host_access.h>
#include <atomic>
#include <condition_variable>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <mutex>
#include <thread>
namespace phase=uwvm2::runtime::checkpoint::host_memory_phase;
namespace func=uwvm2::imported::wasi::wasip1::func;
namespace linear=uwvm2::object::memory::linear;
namespace mt=uwvm2::extension::memtag;
namespace rt=uwvm2::runtime::memtag;
static std::atomic<unsigned> checks{};
static void check(bool valid,unsigned line)
{checks.fetch_add(1u,std::memory_order_relaxed);if(!valid){std::fprintf(stderr,"random phases line=%u\n",line);std::abort();}}
#define CHECK(...) check(bool((__VA_ARGS__)),__LINE__)
struct turns
{
    std::mutex turn{},exchange{};
    std::condition_variable changed{};
    bool owned{},writing{},released{},completed{};
    unsigned reads{},writes{};
    phase::consumer callback{this,
        [](void* pointer,bool writing) noexcept
        {
            auto& self=*static_cast<turns*>(pointer);self.turn.lock();CHECK(!self.owned);
            self.owned=true;self.writing=writing;
            if(writing)++self.writes;else ++self.reads;
        },
        [](void* pointer) noexcept
        {
            auto& self=*static_cast<turns*>(pointer);CHECK(self.owned);
            bool const first=!self.writing&&self.reads==1u;self.owned=false;self.turn.unlock();
            if(first)
            {
                // The real input-validation phase has ended. A guest writer
                // must acquire the turn before native input/output continues.
                std::unique_lock lock{self.exchange};self.released=true;self.changed.notify_all();
                self.changed.wait(lock,[&]{return self.completed;});
            }
        }};
};
template<class Address>static void run(bool tagged)
{
    constexpr std::size_t offset=32u,count=65553u,extent=131072u;
    linear::mmap_memory_t memory(65536,sizeof(Address)==4?linear::mmap_memory_status_t::wasm32:linear::mmap_memory_status_t::wasm64);
    memory.init_by_page_count(2,2);std::memset(memory.memory_begin,0x7d,extent);
    Address raw=offset;
    if(tagged)
    {
        CHECK(memory.enable_memtag(sizeof(Address)==4?mt::address_width::i32:mt::address_width::i64,true)==mt::storage_error::none);
        raw=mt::shadow_view<Address,true>::insert(offset,3u);
        std::array<Address,2> operands{raw,Address{(count+15u)/16u*16u}};
        CHECK(rt::execute<true,Address,true>(memory,mt::opcode::store,operands));
    }
    turns trace{};phase::current=&trace.callback;
    std::thread writer([&]
    {
        CHECK(phase::current==nullptr);
        {std::unique_lock lock{trace.exchange};trace.changed.wait(lock,[&]{return trace.released;});}
        {std::lock_guard lock{trace.turn};CHECK(!trace.owned);std::memset(memory.memory_begin+offset,0x42,count);}
        {std::lock_guard lock{trace.exchange};trace.completed=true;trace.changed.notify_all();}
    });
    auto& calls=uwvm2::imported::wasi::wasip1::environment::process_budget_ledger.native_random_calls;
    auto const before=calls.load();
    CHECK(func::random_get_shared_tagged(memory,raw,count)==uwvm2::imported::wasi::wasip1::abi::errno_t::esuccess);
    writer.join();phase::current=nullptr;
    CHECK(!trace.owned&&trace.completed&&trace.reads==1u&&trace.writes>=2u);
    CHECK(calls.load()-before>=2u);
    for(std::size_t i{};i<offset;++i)CHECK(memory.memory_begin[i]==std::byte{0x7d});
    for(std::size_t i=offset+count;i<extent;++i)CHECK(memory.memory_begin[i]==std::byte{0x7d});
    // Invalid ranges fail before entropy or guest writes; this also proves
    // the detached helper does not turn its batch size into a request limit.
    auto const after=calls.load();
    CHECK(func::random_get_shared_tagged(memory,extent-8u,16u)==uwvm2::imported::wasi::wasip1::abi::errno_t::efault);
    CHECK(calls.load()==after);
}
int main()
{
    run<std::uint32_t>(false);run<std::uint32_t>(true);
    run<std::uint64_t>(false);run<std::uint64_t>(true);
    std::printf("PASS %u random phase checks\n",checks.load());
}
#include <uwvm2/utils/macro/pop_macros.h>
