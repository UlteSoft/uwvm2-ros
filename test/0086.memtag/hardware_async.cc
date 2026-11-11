#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/runtime/memtag/bridge.h>
#include <cstdio>
#include <cstdlib>
#if defined(__linux__) && defined(__aarch64__)
#include <csignal>
#include <sys/wait.h>
#include <sys/prctl.h>
#include <unistd.h>
namespace mt=uwvm2::extension::memtag;
namespace rt=uwvm2::runtime::memtag;
namespace sig=uwvm2::object::memory::signal;
namespace linear=uwvm2::object::memory::linear;
#define CHECK(...) do { if(!(__VA_ARGS__)) { std::fprintf(stderr,"hardware async assertion %u\n",__LINE__);std::abort(); } } while(false)
static int report_fd;
static volatile unsigned char* good;
struct evidence { bool hardware{},unknown{};unsigned value{},tag{},actual{};std::uint64_t granule{}; };
static void delivered(sig::hardware_memtag_fault const& value) noexcept
{
    evidence report{true,value.asynchronous && value.memory_index==SIZE_MAX && value.offset==UINT64_MAX &&
        value.raw_address==0 && value.instruction_address==0 && !value.pointer_tag_available,*good,0,0,UINT64_MAX};
    static_cast<void>(::write(report_fd,&report,sizeof report));::_Exit(122);
}
static void software_delivered(rt::deferred::fault const& value) noexcept
{
    evidence report{false,false,*good,value.access.pointer_tag,value.access.allocation_tag,value.access.mismatch_granule};
    static_cast<void>(::write(report_fd,&report,sizeof report));::_Exit(122);
}
static void previous(int,siginfo_t*,void*) noexcept { ::_Exit(91); }
template<class Address,bool Shared> void test(bool high,bool store)
{
    linear::mmap_memory_t memory(65536,sizeof(Address)==4?linear::mmap_memory_status_t::wasm32:linear::mmap_memory_status_t::wasm64);
    memory.init_by_page_count(1,2,17);
    CHECK(memory.enable_memtag(sizeof(Address)==4?mt::address_width::i32:mt::address_width::i64,Shared)==mt::storage_error::none);
    auto view=memory.memtag_shadow.template view<Address,Shared>(65536);
    unsigned const tag=sizeof(Address)==4?11:171;
    CHECK(view.store(view.insert(32,tag),16));CHECK(rt::activate_owner_hardware(memory));
    auto* correct=mt::hardware_mte::tagged_pointer(memory.memory_begin+32,tag);
    good=reinterpret_cast<volatile unsigned char*>(correct);*good=42;
    int pipefd[2];CHECK(::pipe(pipefd)==0);auto child=::fork();CHECK(child>=0);
    if(child==0)
    {
        ::close(pipefd[0]);report_fd=pipefd[1];
        sig::detail::hardware_memtag_fault_func=delivered;
        rt::deferred::deliver_callback.store(software_delivered);
        CHECK(rt::prepare_hardware_thread());CHECK((::prctl(56,0ul,0ul,0ul,0ul)&6)==4);
        rt::deferred::entry_scope entry;CHECK(entry.enter());rt::deferred::set_origin(3,7,41);
        auto const pointer=view.insert(32,high?187:(sizeof(Address)==4?1:161));
        auto const result=rt::with_guest_access<Address,Shared>(memory,pointer,0,1,[&](std::span<std::byte> bytes) noexcept
        {
            auto* actual=reinterpret_cast<volatile unsigned char*>(bytes.data());
            if(store)*actual=77;else {auto observed=*actual;(void)observed;}
        });
        CHECK(result);rt::deferred::poll(true);::_Exit(99);
    }
    ::close(pipefd[1]);evidence result{};auto bytes=::read(pipefd[0],&result,sizeof result);::close(pipefd[0]);
    int status{};CHECK(::waitpid(child,&status,0)==child);CHECK(WIFEXITED(status) && WEXITSTATUS(status)==122 && bytes==sizeof result);
    CHECK(result.value==(store?77u:42u));
    if(high) { CHECK(!result.hardware && result.tag==187 && result.actual==171 && result.granule==32); }
    else { CHECK(result.hardware && result.unknown); }
}
int main()
{
    if(!mt::hardware_mte::supported()) {std::puts("UNSUPPORTED: OS checked MTE mappings unavailable");return 77;}
    mt::selected_fault_delivery.store(mt::fault_delivery::hardware_async);
    CHECK(mt::hardware_mte::enable_current_thread_selected());
    struct sigaction handler{};handler.sa_sigaction=previous;handler.sa_flags=SA_SIGINFO;::sigemptyset(&handler.sa_mask);
    CHECK(::sigaction(SIGSEGV,&handler,nullptr)==0);
    for(bool store:{false,true})
    {
        test<std::uint32_t,false>(false,store);test<std::uint32_t,true>(false,store);
        test<std::uint64_t,false>(false,store);test<std::uint64_t,true>(false,store);
        test<std::uint64_t,false>(true,store);test<std::uint64_t,true>(true,store);
    }
    auto child=::fork();CHECK(child>=0);
    if(!child)
    {
        auto* bytes=static_cast<std::byte*>(::mmap(nullptr,4096,PROT_READ|PROT_WRITE|0x20,MAP_PRIVATE|MAP_ANONYMOUS,-1,0));
        CHECK(bytes!=MAP_FAILED);mt::hardware_mte::store_tag(bytes,4);mt::hardware_mte::finish_tag_writes();
        *reinterpret_cast<volatile unsigned char*>(bytes)=1;
        asm volatile("dsb sy" ::: "memory");static_cast<void>(::prctl(56,0ul,0ul,0ul,0ul));::_Exit(99);
    }
    int status{};CHECK(::waitpid(child,&status,0)==child);CHECK(WIFEXITED(status) && WEXITSTATUS(status)==91);
    std::puts("PASS: 12 actual async read/write faults, full memory64 upper tag, exact mode, unknown hardware location, unowned forwarding");
}
#else
int main(){std::puts("UNSUPPORTED: requires AArch64 Linux MTE");return 77;}
#endif
