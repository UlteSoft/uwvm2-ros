#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/runtime/memtag/bridge.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
namespace mt=uwvm2::extension::memtag;
namespace rt=uwvm2::runtime::memtag;
namespace linear=uwvm2::object::memory::linear;
static void require(bool value,unsigned line)
{ if(!value) { std::fprintf(stderr,"async access assertion line %u\n",line);std::abort(); } }
#define CHECK(...) require(static_cast<bool>((__VA_ARGS__)),__LINE__)
static std::byte* observed{};
static std::uint64_t expected_pointer{};
static void delivered(rt::deferred::fault const& value) noexcept
{
    CHECK(observed && observed[32]==std::byte{77});
    CHECK(value.raw_pointer==expected_pointer && value.access.mismatch_granule==32);
    CHECK(value.memory_index==17 && value.module_id==3 && value.function_index==5 && value.wasm_offset==71);
    CHECK(value.access.pointer_tag!=value.access.allocation_tag);
    std::puts("PASS: access occurred; immutable mismatch retained; delivery terminates before return");
    std::fflush(stdout);
    namespace crash=uwvm2::runtime::diagnostic::crash;
    crash::memory_detail const detail{value.memory_index,value.raw_pointer,value.access.mismatch_granule,
        value.access.pointer_tag,value.access.allocation_tag};
    crash::report(crash::category::memtag,"asynchronous memory tag mismatch",0,3,5,71,&detail,true);
    std::_Exit(122);
}
template<class Owner,class Address,bool Shared>
void test(char const* mode)
{
    Owner memory=[]
    {
        if constexpr(Owner::can_mmap)
            return Owner{65536,sizeof(Address)==4?linear::mmap_memory_status_t::wasm32:linear::mmap_memory_status_t::wasm64};
        else return Owner{};
    }();
    if constexpr(Owner::can_mmap) { memory.init_by_page_count(1,2); }
    else { memory.init_by_page_count(1); }
    memory.diagnostic_owner_memory_index=17;
    CHECK(memory.enable_memtag(sizeof(Address)==4?mt::address_width::i32:mt::address_width::i64,Shared)==mt::storage_error::none);
    using view=mt::shadow_view<Address,Shared>;
    auto tags=memory.memtag_shadow.template view<Address,Shared>(65536);
    constexpr unsigned good=sizeof(Address)==4?5:0xa5;
    constexpr unsigned bad=sizeof(Address)==4?6:0xb5; // Equal low nibble: preserve full memory64 tags.
    CHECK(tags.store(view::insert(0,good),65536));
    observed=memory.memory_begin;observed[32]=std::byte{42};
    auto pointer=view::insert(32,std::strcmp(mode,"valid")==0?good:bad);expected_pointer=pointer;
    rt::deferred::entry_scope entry;CHECK(entry.enter());rt::deferred::set_origin(3,5,71);
    if(std::strcmp(mode,"bounds")==0)
    {
        bool called{};
        auto result=rt::with_guest_access<Address,Shared>(memory,pointer,65536,1,[&](std::span<std::byte>) noexcept{called=true;});
        CHECK(!result && result.error==mt::access_error::out_of_bounds && !called && observed[32]==std::byte{42});
    }
    else if(std::strcmp(mode,"host")==0)
    {
        bool called{};
        auto result=rt::with_checked_access<Address,Shared>(memory,pointer,0,1,[&](std::span<std::byte>) noexcept{called=true;});
        CHECK(!result && result.error==mt::access_error::tag_mismatch && !called && observed[32]==std::byte{42});
    }
    else
    {
        bool called{};
        auto result=rt::with_guest_access<Address,Shared>(memory,pointer,0,1,[&](std::span<std::byte> bytes) noexcept
        {called=true;bytes[0]=std::byte{77};});
        if(std::strcmp(mode,"sync")==0)
        { CHECK(!result && result.error==mt::access_error::tag_mismatch && !called && observed[32]==std::byte{42}); }
        else { CHECK(result && called && observed[32]==std::byte{77}); }
    }
    entry.finish();
    CHECK(std::strcmp(mode,"async")!=0);
    std::puts("PASS: selected access contract");
}
int main(int argc,char** argv)
{
    CHECK(argc==5);
    mt::selected_fault_delivery.store(std::strcmp(argv[1],"sync")==0?mt::fault_delivery::synchronous:mt::fault_delivery::software_async);
    rt::deferred::deliver_callback.store(delivered);
    bool const mmap=std::strcmp(argv[2],"mmap")==0,bits64=std::strcmp(argv[3],"64")==0,shared=std::strcmp(argv[4],"shared")==0;
    auto dispatch=[&]<class Owner>()
    {
        if(bits64) { if(shared)test<Owner,std::uint64_t,true>(argv[1]);else test<Owner,std::uint64_t,false>(argv[1]); }
        else { if(shared)test<Owner,std::uint32_t,true>(argv[1]);else test<Owner,std::uint32_t,false>(argv[1]); }
    };
    if(mmap)dispatch.template operator()<linear::mmap_memory_t>();
    else dispatch.template operator()<linear::allocator_memory_t>();
}
