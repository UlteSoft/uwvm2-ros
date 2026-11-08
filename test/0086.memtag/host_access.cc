#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/runtime/memtag/host_access.h>
#include <uwvm2/runtime/memtag/host_fault.h>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <thread>
namespace mt=uwvm2::extension::memtag;
namespace rt=uwvm2::runtime::memtag;
namespace linear=uwvm2::object::memory::linear;
static unsigned checks;
static void require(bool v,unsigned line) {++checks;if(!v){std::fprintf(stderr,"host access line %u\n",line);std::abort();}}
#define CHECK(...) require(static_cast<bool>((__VA_ARGS__)),__LINE__)
template<class Address,bool Shared,class Owner> void run(Owner& m)
{
    CHECK(rt::check_host_access(m,0,65536));
    CHECK(!rt::check_host_access(m,UINT64_MAX,0));
    CHECK(!rt::check_host_access(m,65535,2));
    CHECK(m.enable_memtag(sizeof(Address)==4?mt::address_width::i32:mt::address_width::i64,Shared)==mt::storage_error::none);
    using View=mt::shadow_view<Address,Shared>;
    auto const raw=View::insert(32,7);
    std::array<Address,2> mark{raw,48};
    CHECK(rt::execute<true,Address,Shared>(m,mt::opcode::store,mark));
    std::array<std::byte,48> in{},out{};in.fill(std::byte{0x37});out.fill(std::byte{0x55});
    CHECK(rt::copy_host_access<true>(m,raw,in.data(),in.size()));
    CHECK(rt::copy_host_access<false>(m,raw,out.data(),out.size()));
    CHECK(in==out);
    CHECK(!rt::copy_host_access<true>(m,32,in.data(),in.size()));
    CHECK(!rt::copy_host_access<false>(m,32,out.data(),out.size()));
    if constexpr(sizeof(Address)==4) CHECK(!rt::check_host_access(m,(std::uint64_t{1}<<32)|raw,1));
    CHECK(rt::check_host_access(m,65536,0));
    CHECK(!rt::check_host_access(m,65537,0));
    CHECK(!rt::check_host_access(m,raw,UINT64_MAX));
    // Failure at the LAST granule performs neither a partial write nor a partial read.
    mark={View::insert(64,8),16};CHECK(rt::execute<true,Address,Shared>(m,mt::opcode::store,mark));
    in.fill(std::byte{0x66});out.fill(std::byte{0x77});
    auto bad=rt::copy_host_access<true>(m,raw,in.data(),in.size());
    CHECK(!bad && bad.error==mt::access_error::tag_mismatch && bad.mismatch_granule==64);
    CHECK(!rt::copy_host_access<false>(m,raw,out.data(),out.size()));
    for(unsigned i=0;i<48;++i){CHECK(m.memory_begin[32+i]==std::byte{0x37});CHECK(out[i]==std::byte{0x77});}
    CHECK(!rt::copy_host_access<true>(m,raw,nullptr,1));
    CHECK(!rt::copy_host_access<false>(m,raw,nullptr,1));
    CHECK(rt::copy_host_access<false>(m,raw,nullptr,0));
    rt::record_host_fault(3,raw,bad);rt::record_host_fault(5,0,bad);
    auto fault=rt::consume_host_fault();
    CHECK(fault.pending && fault.memory_index==3 && fault.pointer==raw && fault.granule==64 && fault.pointer_tag==7 && fault.allocation_tag==8);
    CHECK(!rt::consume_host_fault().pending);
    auto oob=rt::check_host_access(m,UINT64_MAX,1);rt::record_host_fault(3,UINT64_MAX,oob);
    CHECK(!rt::consume_host_fault().pending);
    bool called{};CHECK(!rt::with_host_access(m,raw,48,[&](std::span<std::byte>) noexcept {called=true;}));CHECK(!called);
    if constexpr(Owner::support_multi_thread && !Owner::can_mmap)CHECK(m.active_ops_p->load()==0);
    CHECK(m.grow_strictly(1));
}
int main()
{
    {linear::single_thread_allocator_memory_t m;m.init_by_page_count(1);run<std::uint32_t,false>(m);}
    {linear::allocator_memory_t m;m.init_by_page_count(1);run<std::uint64_t,true>(m);}
#if defined(UWVM_SUPPORT_MMAP)
    {linear::mmap_memory_t m;m.init_by_page_count(1);run<std::uint32_t,true>(m);}
    {linear::mmap_memory_t m(65536,linear::mmap_memory_status_t::wasm64);m.init_by_page_count(1);run<std::uint64_t,false>(m);}
#endif
    // Independent TLS fault records cannot cross guest host-call threads.
    rt::pending_host_fault={true,9,11,16,1,2};
    std::thread worker([]{CHECK(!rt::consume_host_fault().pending);rt::pending_host_fault={true,2,3,4,5,6};});
    worker.join();CHECK(rt::consume_host_fault().memory_index==9);
    std::printf("PASS %u checked host-buffer assertions\n",checks);
}

#include <uwvm2/utils/macro/pop_macros.h>
