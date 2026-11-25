// Verify the shared bridge used by the interpreter and LLVM callback against a
// real backing memory. Fixed entropy makes collision/exhaustion cases repeatable.
#define UWVM_TEST
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/runtime/memtag/impl.h>
#include <array>
#include <barrier>
#include <cstdio>
#include <cstdlib>
#include <thread>

namespace mt = uwvm2::extension::memtag;
namespace rt = uwvm2::runtime::memtag;
namespace linear = uwvm2::object::memory::linear;
namespace resource = uwvm2::utils::allocator::resource;
using op = mt::opcode;
static void check(bool ok, unsigned line)
{
    if(!ok) { std::fprintf(stderr,"managed-allocation assertion at %u\n",line); std::abort(); }
}
#define CHECK(...) check(static_cast<bool>((__VA_ARGS__)), __LINE__)

template<class A, bool Shared>
void cases()
{
    using view = mt::shadow_view<A,Shared>;
    constexpr auto status = sizeof(A) == 4 ? linear::mmap_memory_status_t::wasm32 : linear::mmap_memory_status_t::wasm64;
    linear::mmap_memory_t owner{65536,status};
    owner.init_by_page_count(1,2);
    CHECK(owner.enable_memtag(sizeof(A)==4 ? mt::address_width::i32 : mt::address_width::i64, Shared) == mt::storage_error::none);
    unsigned consumed{};
    std::uint8_t byte{0x80};
    bool entropy_ok{true};
    auto run = [&](op instruction,A ptr,A bytes)
    {
        A const args[]{ptr,bytes};
        return rt::execute<true,A,Shared>(owner,instruction,args,[&](std::uint8_t& out) noexcept
        { ++consumed; out=byte; return entropy_ok; });
    };
    auto accepts = [&](A ptr,A bytes) { return rt::check_access<A,Shared>(owner,ptr,0,bytes); };
    auto left=run(op::reserve,0,16); CHECK(left && !accepts(left.operation.value,16));
    auto future=left.operation.value;
    CHECK(accepts(0,16));
    CHECK(!run(op::claim,0,16));
    CHECK(!run(op::release,view::insert(0,view::extract(future)+1),16));
    CHECK(!run(op::activate,future,32));
    auto boundary=run(op::claim,80,16); CHECK(boundary);
    CHECK(view::extract(boundary.operation.value)!=view::extract(future));
    CHECK(!run(op::store,view::insert(16,view::extract(future)),16));
    CHECK(!run(op::untagstore,0,16));
    unsigned before=consumed;
    CHECK(run(op::activate,future,16)); CHECK(consumed==before && accepts(future,16) && !accepts(0,16));
    CHECK(!run(op::activate,future,16));
    CHECK(!run(op::release,view::untag(future),16));
    CHECK(run(op::release,boundary.operation.value,16).operation.value==80);
    CHECK(run(op::release,future,16).operation.value==0);
    CHECK(accepts(0,16) && !accepts(future,16));
    CHECK(!run(op::release,future,16));
    // A gap of 80 bytes is outside the exclusion window.
    auto a=run(op::claim,0,16), b=run(op::claim,96,16);
    CHECK(a && b && view::extract(a.operation.value)==view::extract(b.operation.value));
    CHECK(run(op::release,a.operation.value,16)); CHECK(run(op::release,b.operation.value,16));
    auto reserved=run(op::reserve,160,16); CHECK(reserved); CHECK(run(op::release,reserved.operation.value,16));
    CHECK(!run(op::activate,reserved.operation.value,16));

    auto& registry=owner.memtag_shadow.allocation_directory();
    CHECK(registry.empty());
    CHECK(run(op::store,view::insert(320,3),16));
    CHECK(registry.add_permanent(20,20,3));
    CHECK(!registry.add_permanent(20,21,4));
    CHECK(!run(op::claim,view::insert(320,3),16));
    CHECK(!run(op::release,view::insert(320,3),16));
    CHECK(!run(op::untagstore,320,16));
    CHECK(!registry.remove_permanent(20,20,4));
    CHECK(registry.remove_permanent(20,20,3)); CHECK(registry.empty());
    CHECK(run(op::untagstore,320,16));
    before=consumed;
    CHECK(!run(op::claim,1,16) && !run(op::claim,0,15) && !run(op::claim,65520,32) && !run(op::claim,0,0));
    CHECK(consumed==before);
    owner.memory_begin[144]=std::byte{0x55};
    entropy_ok=false; CHECK(!run(op::claimz,144,16));
    CHECK(owner.memory_begin[144]==std::byte{0x55} && registry.empty());
    entropy_ok=true; byte=0; before=consumed;
    CHECK(!run(op::claimz,144,16) && consumed==before+32);
    CHECK(owner.memory_begin[144]==std::byte{0x55} && registry.empty());
    byte=0x80;
    auto& limits=resource::policies[static_cast<unsigned>(resource::pool::memtag)];
    auto const saved=limits; limits.single=0;
    auto failed=run(op::claimz,144,16); limits=saved;
    CHECK(!failed && failed.operation.error==mt::operation_error::allocation_resource_limit);
    CHECK(owner.memory_begin[144]==std::byte{0x55} && registry.empty());
    auto zero=run(op::claimz,144,16); CHECK(zero && owner.memory_begin[144]==std::byte{});
    CHECK(run(op::release,zero.operation.value,16));
    for(unsigned i{}; i!=257; ++i)
    {
        auto const index=(i*117)%257;
        CHECK(registry.add_permanent(index,index,1));
    }
    for(unsigned i{}; i!=257; ++i)
    {
        auto const index=(i*103)%257;
        CHECK(registry.remove_permanent(index,index,1));
    }
    CHECK(registry.empty());
    // Identity mode, including an in-bounds checked zero fill and out-of-bounds rejection.
    owner.memory_begin[48]=std::byte{0x55};
    A args[]{48,16};
    for(auto instruction : {op::claim,op::reserve,op::activate,op::release,op::claimz})
    {
        auto disabled=rt::execute<false,A,Shared>(owner,instruction,args);
        CHECK(disabled && disabled.operation.value==48);
    }
    CHECK(owner.memory_begin[48]==std::byte{} && registry.empty());
    args[0]=65535; CHECK(!rt::execute<false,A,Shared>(owner,op::claimz,args));
}

static void competing_allocators()
{
    using A=std::uint32_t;
    using view=mt::shadow_view<A,true>;
    linear::mmap_memory_t owner; owner.init_by_page_count(1,2);
    CHECK(owner.enable_memtag(mt::address_width::i32,true)==mt::storage_error::none);
    std::barrier rendezvous{2};
    std::array<A,2> tokens{};
    auto allocate=[&](unsigned index)
    {
        for(unsigned i{};i!=1000;++i)
        {
            A args[]{static_cast<A>(index*16),16};
            auto execute=[&](op code) { return rt::execute<true,A,true>(owner,code,args,
                [](std::uint8_t& b) noexcept { b=0x80; return true; }); };
            rendezvous.arrive_and_wait();
            auto allocation=execute(op::claim); CHECK(allocation); tokens[index]=allocation.operation.value;
            rendezvous.arrive_and_wait();
            CHECK(view::extract(tokens[0])!=view::extract(tokens[1]));
            CHECK(rt::check_access<A,true>(owner,tokens[index],0,16));
            args[0]=tokens[index]; CHECK(execute(op::release));
        }
    };
    std::thread worker(allocate,0); allocate(1); worker.join();
    CHECK(owner.memtag_shadow.allocation_directory().empty());
}

int main()
{
    cases<std::uint32_t,false>(); cases<std::uint32_t,true>();
    cases<std::uint64_t,false>(); cases<std::uint64_t,true>();
    competing_allocators();
    std::puts("PASS managed-allocation four owner profiles, rollback, radius and 1000 concurrent pairs");
}
#include <uwvm2/utils/macro/pop_macros.h>
