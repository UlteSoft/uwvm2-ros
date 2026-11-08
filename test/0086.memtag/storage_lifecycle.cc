#define UWVM_TEST
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/extension/memtag/storage.h>
#include <uwvm2/object/memory/linear/impl.h>
#include <uwvm2/utils/container/impl.h>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <limits>
#include <thread>
#include <utility>
#include <cerrno>

namespace mt = uwvm2::extension::memtag;
namespace linear = uwvm2::object::memory::linear;
static void require(bool okay, unsigned line) {
    if(!okay) { std::fprintf(stderr,"FAILED storage lifecycle line %u\n",line); std::abort(); }
}
#define CHECK(...) require(static_cast<bool>((__VA_ARGS__)), __LINE__)
static_assert(fast_io::freestanding::is_trivially_copyable_or_relocatable<linear::allocator_memory_t>::value);
static_assert(fast_io::freestanding::is_trivially_copyable_or_relocatable<linear::single_thread_allocator_memory_t>::value);
#if defined(UWVM_SUPPORT_MMAP)
static_assert(fast_io::freestanding::is_trivially_copyable_or_relocatable<linear::mmap_memory_t>::value);
#endif
struct old_allocator_layout {
    std::byte* memory_begin; std::size_t memory_length; unsigned custom_page_size_log2;
#if SIZE_MAX > UINT32_MAX
    bool sequentially_consistent_size;
#endif
    std::atomic_flag* growing_flag_p; std::atomic_size_t* active_ops_p;
    std::size_t diagnostic_owner_memory_index;
#if SIZE_MAX <= UINT32_MAX
    bool sequentially_consistent_size;
#endif
    std::atomic_size_t ordered_memory_length;
};
#define SAME_OFFSET(Type, Legacy, Field) static_assert(offsetof(Type,Field)==offsetof(Legacy,Field))
SAME_OFFSET(linear::allocator_memory_t,old_allocator_layout,memory_begin);
SAME_OFFSET(linear::allocator_memory_t,old_allocator_layout,memory_length);
SAME_OFFSET(linear::allocator_memory_t,old_allocator_layout,growing_flag_p);
SAME_OFFSET(linear::allocator_memory_t,old_allocator_layout,active_ops_p);
SAME_OFFSET(linear::allocator_memory_t,old_allocator_layout,ordered_memory_length);
#if defined(UWVM_SUPPORT_MMAP)
struct alignas(64) old_mmap_layout {
    std::byte* reserved_begin; std::byte* memory_begin; std::atomic_size_t* memory_length_p;
    unsigned custom_page_size_log2; bool require_dynamic_determination_memory_size_cached;
    bool sequentially_consistent_size; linear::mmap_memory_status_t status;
    uwvm2::utils::mutex::mutex_t* growing_mutex_p; std::size_t reservation_limit_bytes;
    std::size_t diagnostic_owner_memory_index;
};
SAME_OFFSET(linear::mmap_memory_t,old_mmap_layout,memory_begin);
SAME_OFFSET(linear::mmap_memory_t,old_mmap_layout,memory_length_p);
SAME_OFFSET(linear::mmap_memory_t,old_mmap_layout,status);
SAME_OFFSET(linear::mmap_memory_t,old_mmap_layout,growing_mutex_p);
SAME_OFFSET(linear::mmap_memory_t,old_mmap_layout,reservation_limit_bytes);
#endif
static mt::storage_operation fail_kind;
static bool fail_once;
static unsigned failures;
static bool fail_storage(mt::storage_operation operation,std::size_t) {
    if(fail_once && operation==fail_kind) { fail_once=false; ++failures; return true; }
    return false;
}
static void inject(mt::storage_operation operation) {
    fail_kind=operation;fail_once=true; mt::details::storage_failure_test_hook=fail_storage;
}
static void clear_injection() { mt::details::storage_failure_test_hook=nullptr;fail_once=false; }

template<class Address,bool Shared>
void storage_case(mt::storage_backend backend) {
    mt::shadow_storage owner;
    CHECK(!owner.enabled() && owner.tag_data()==nullptr && owner.tag_count()==0);
    CHECK(owner.prepare_growth(UINT64_MAX)==mt::storage_error::none);
    constexpr auto width=sizeof(Address)==4?mt::address_width::i32:mt::address_width::i64;
    CHECK(owner.enable(width,Shared,backend,17,1u<<20)==mt::storage_error::none);
    CHECK(owner.enabled() && owner.width()==width && owner.shared()==Shared);
    CHECK(owner.stable_address()==(backend==mt::storage_backend::reserved));
    CHECK(owner.tag_count()==2);
    CHECK(owner.enable(width,Shared,backend,17,1u<<20)==mt::storage_error::already_enabled);
    auto view=owner.view<Address,Shared>(17);
    auto tagged=view.insert(0,5);
    CHECK(view.store(tagged,17));
    CHECK(!owner.view<Address,!Shared>(17).check(0,0,0));
    CHECK(!owner.view<Address,Shared>(18).check(0,0,0));
    CHECK(owner.prepare_growth(31)==mt::storage_error::none);
    CHECK(owner.view<Address,Shared>(31).check(tagged,0,31)); // preserve partial last granule
    auto* before=owner.tag_data();
    CHECK(owner.prepare_growth(65536)==mt::storage_error::none);
    if(backend==mt::storage_backend::reserved)CHECK(owner.tag_data()==before);
    CHECK(owner.view<Address,Shared>(65536).check(tagged,0,31));
    CHECK(owner.view<Address,Shared>(65536).check(32,0,65504));
    CHECK(owner.prepare_growth((1u<<20)+1u)==mt::storage_error::capacity_exceeded);
    auto saved=owner.tag_data();
    mt::shadow_storage moved(std::move(owner));
    CHECK(!owner.enabled() && owner.tag_data()==nullptr && moved.tag_data()==saved);
    owner=std::move(moved);
    CHECK(!moved.enabled() && owner.view<Address,Shared>(65536).check(tagged,0,31));
    auto* owner_alias=&owner;
    owner=std::move(*owner_alias);
    CHECK(owner.tag_data()==saved);
    owner.reset();
    CHECK(!owner.enabled() && owner.tag_data()==nullptr);
}
template<class Owner>
void allocator_owner_case() {
    Owner owner(1);
    CHECK(!owner.memtag_shadow.enabled() && owner.memtag_shadow.tag_data()==nullptr);
    owner.init_by_page_count(32);
    CHECK(owner.enable_memtag(mt::address_width::i32,false)==mt::storage_error::none);
    auto* alias=&owner;
    auto view=owner.memtag_shadow.template view<std::uint32_t>(owner.memory_length);
    CHECK(view.store(view.insert(0,3),32));
    Owner other(1); other.init_by_page_count(32);
    CHECK(other.enable_memtag(mt::address_width::i32,false)==mt::storage_error::none);
    CHECK(other.memtag_shadow.tag_data()!=owner.memtag_shadow.tag_data());
    CHECK(!other.memtag_shadow.template view<std::uint32_t>(32).check(view.insert(0,3),0,32));
    auto* native=owner.memory_begin;
    auto* shadow=owner.memtag_shadow.tag_data();
    inject(mt::storage_operation::allocate);
    CHECK(!owner.grow_strictly(32));
    clear_injection();
    CHECK(owner.memory_length==32 && owner.memory_begin==native && owner.memtag_shadow.tag_data()==shadow);
    CHECK(alias->memtag_shadow.template view<std::uint32_t>(32).check(view.insert(0,3),0,32));
    CHECK(owner.grow_strictly(32));
    CHECK(alias->memtag_shadow.template view<std::uint32_t>(64).check(view.insert(0,3),0,32));
    CHECK(alias->memtag_shadow.template view<std::uint32_t>(64).check(32,0,32));
    CHECK(!owner.grow_strictly((std::size_t{1}<<28)-63)); // new length 256 MiB+1
    CHECK(owner.memory_length==64);
    Owner moved(std::move(owner));
    CHECK(!owner.memtag_shadow.enabled() && moved.memtag_shadow.enabled());
    owner=std::move(moved);
    CHECK(!moved.memtag_shadow.enabled() && owner.memtag_shadow.enabled());
    uwvm2::utils::container::vector<Owner> owners;
    owners.push_back(std::move(owner));
    auto* tags=owners.front().memtag_shadow.tag_data();
    owners.reserve(40);
    CHECK(owners.front().memtag_shadow.tag_data()==tags);
    CHECK(owners.front().memtag_shadow.template view<std::uint32_t>(64).check(view.insert(0,3),0,32));
    owners.front().clear();
    CHECK(!owners.front().memtag_shadow.enabled());
    owners.front().init_by_page_count(1);
    CHECK(owners.front().memory_length==1 && !owners.front().memtag_shadow.enabled());
}

#if defined(UWVM_SUPPORT_MMAP)
#if defined(UWVM_CPP_EXCEPTIONS) && !(defined(_WIN32)||defined(__CYGWIN__))
static unsigned data_failure_calls;
static void fail_data_commit(void* address,std::size_t size,int protection) {
    if(data_failure_calls++==0)throw fast_io::error{fast_io::posix_domain_value,ENOMEM};
    fast_io::details::sys_mprotect(address,size,protection);
}
#endif
template<class Address,bool Shared>
void mmap_owner_case() {
    constexpr auto width=sizeof(Address)==4?mt::address_width::i32:mt::address_width::i64;
    constexpr auto status=sizeof(Address)==4?linear::mmap_memory_status_t::wasm32:linear::mmap_memory_status_t::wasm64;
    linear::mmap_memory_t owner(65536,status);
    owner.init_by_page_count(1,4,19);
    CHECK(owner.enable_memtag(width,Shared)==mt::storage_error::none);
    auto* shadow=owner.memtag_shadow.tag_data();
    auto v=owner.memtag_shadow.template view<Address,Shared>(65536);
    CHECK(v.store(v.insert(0,7),16));
    inject(mt::storage_operation::commit);
    CHECK(!owner.grow_strictly(1));
    clear_injection();
    CHECK(owner.get_page_size()==1 && owner.memtag_shadow.tag_data()==shadow);

    CHECK(owner.grow_strictly(1));
    CHECK(owner.get_page_size()==2 && owner.memtag_shadow.tag_data()==shadow);
    CHECK(owner.memtag_shadow.template view<Address,Shared>(131072).check(v.insert(0,7),0,16));
    CHECK(owner.memtag_shadow.template view<Address,Shared>(131072).check(65536,0,65536));
    CHECK(owner.memory_begin[65536]==std::byte{});
    linear::mmap_memory_t moved(std::move(owner));
    CHECK(!owner.memtag_shadow.enabled() && moved.memtag_shadow.tag_data()==shadow);
    owner=std::move(moved);
    CHECK(owner.memtag_shadow.tag_data()==shadow && !moved.memtag_shadow.enabled());
    uwvm2::utils::container::vector<linear::mmap_memory_t> owners;
    owners.push_back(std::move(owner));
    owners.reserve(20);
    CHECK(owners.front().memtag_shadow.tag_data()==shadow);
    CHECK(owners.front().memtag_shadow.template view<Address,Shared>(131072).check(v.insert(0,7),0,16));
    owners.front().clear();
    CHECK(!owners.front().memtag_shadow.enabled());
}
#endif

#if defined(UWVM_SUPPORT_MMAP) && defined(UWVM_CPP_EXCEPTIONS) && !(defined(_WIN32)||defined(__CYGWIN__))
void recoverable_data_failure() {
    linear::mmap_memory_t owner(1,linear::mmap_memory_status_t::wasm32);
    owner.init_by_page_count(65536,262144);
    CHECK(owner.enable_memtag(mt::address_width::i32,false)==mt::storage_error::none);
    auto v=owner.memtag_shadow.view<std::uint32_t>(65536);
    CHECK(v.store(v.insert(0,7),16));
    data_failure_calls=0;linear::details::strict_grow_mprotect_test_hook=fail_data_commit;
    CHECK(!owner.grow_strictly(65536));
    linear::details::strict_grow_mprotect_test_hook=nullptr;
    CHECK(data_failure_calls==2 && owner.get_page_size()==65536);
    CHECK(owner.memtag_shadow.prepared_memory_bytes()==131072);
    CHECK(owner.memtag_shadow.view<std::uint32_t>(65536).check(v.insert(0,7),0,16));
    CHECK(owner.grow_strictly(65536));
    CHECK(owner.get_page_size()==131072);
    CHECK(owner.memtag_shadow.view<std::uint32_t>(131072).check(65536,0,65536));
}
#endif

template<class Owner>
void empty_initial_memory() {
    Owner memory;
    CHECK(memory.enable_memtag(mt::address_width::i32,false)==mt::storage_error::invalid_configuration);
    memory.init_by_page_count(0);
    CHECK(memory.enable_memtag(mt::address_width::i32,false)==mt::storage_error::none);
    CHECK(memory.memtag_shadow.template view<std::uint32_t>(0).check(0,0,0));
    CHECK(memory.grow_strictly(1));
    CHECK(memory.memtag_shadow.template view<std::uint32_t>(65536).check(0,0,65536));
}

template<class Owner>
void concurrent_growth() {
    Owner owner;
    if constexpr(Owner::can_mmap)owner.init_by_page_count(1,16);
    else owner.init_by_page_count(1);
    CHECK(owner.enable_memtag(mt::address_width::i32,true)==mt::storage_error::none);
    std::atomic_bool stop{},started{};
    std::atomic_uint reads{};
    std::thread reader([&]{
        started.store(true,std::memory_order_release);
        do {
            if constexpr(Owner::can_mmap) {
                auto bytes=owner.memory_length_p->load(std::memory_order_acquire);
                auto v=owner.memtag_shadow.template view<std::uint32_t,true>(bytes);
                CHECK(v.check(static_cast<std::uint32_t>(bytes-64),0,64));
            } else {
                linear::memory_operation_guard_t pin(owner.growing_flag_p,owner.active_ops_p);
                auto bytes=owner.memory_length;
                auto v=owner.memtag_shadow.template view<std::uint32_t,true>(bytes);
                CHECK(v.check(static_cast<std::uint32_t>(bytes-64),0,64));
            }
            reads.fetch_add(1,std::memory_order_relaxed);
        } while(!stop.load(std::memory_order_acquire));
    });
    while(!started.load(std::memory_order_acquire))std::this_thread::yield();
    for(unsigned i=0;i!=8;++i)CHECK(owner.grow_strictly(1));
    stop.store(true,std::memory_order_release);
    reader.join();
    CHECK(reads.load()>0 && owner.get_page_size()==9);
}
int main() {
    for(auto backend:{mt::storage_backend::growable,mt::storage_backend::reserved}) {
#if !defined(UWVM_SUPPORT_MMAP)
        if(backend==mt::storage_backend::reserved)continue;
#endif
        storage_case<std::uint32_t,false>(backend);
        storage_case<std::uint32_t,true>(backend);
        storage_case<std::uint64_t,false>(backend);
        storage_case<std::uint64_t,true>(backend);
    }
    mt::shadow_storage failure;
    inject(mt::storage_operation::allocate);
    CHECK(failure.enable(mt::address_width::i32,false,mt::storage_backend::growable,32,64)==mt::storage_error::allocation_failed);
    CHECK(!failure.enabled() && failure.tag_data()==nullptr);
    clear_injection();
#if defined(UWVM_SUPPORT_MMAP)
    inject(mt::storage_operation::reserve);
    CHECK(failure.enable(mt::address_width::i32,true,mt::storage_backend::reserved,32,65536)==mt::storage_error::allocation_failed);
    CHECK(!failure.enabled() && failure.tag_data()==nullptr);
    clear_injection();
#endif
    CHECK(failure.enable(mt::address_width::i32,false,mt::storage_backend::growable,(1ull<<28)+1,1ull<<32)==mt::storage_error::address_not_representable);
    CHECK(!failure.enabled());
    allocator_owner_case<linear::single_thread_allocator_memory_t>();
    allocator_owner_case<linear::allocator_memory_t>();
    concurrent_growth<linear::allocator_memory_t>();
    empty_initial_memory<linear::single_thread_allocator_memory_t>();
    empty_initial_memory<linear::allocator_memory_t>();
#if defined(UWVM_SUPPORT_MMAP)
    mmap_owner_case<std::uint32_t,false>();mmap_owner_case<std::uint32_t,true>();
    mmap_owner_case<std::uint64_t,false>();mmap_owner_case<std::uint64_t,true>();
    concurrent_growth<linear::mmap_memory_t>();
    empty_initial_memory<linear::mmap_memory_t>();
#if defined(UWVM_CPP_EXCEPTIONS) && !(defined(_WIN32)||defined(__CYGWIN__))
    recoverable_data_failure();
#endif
    if constexpr(sizeof(std::size_t)>=8) {
        linear::mmap_memory_t untagged;
        untagged.init_by_page_count(4097);
        CHECK(untagged.get_page_size()==4097 && !untagged.memtag_shadow.enabled());
        CHECK(untagged.enable_memtag(mt::address_width::i32,false)==mt::storage_error::address_not_representable);
        CHECK(untagged.get_page_size()==4097 && !untagged.memtag_shadow.enabled());
    }
#endif
    #if defined(UWVM_SUPPORT_MMAP)
    CHECK(failures>=8);
#else
    CHECK(failures>=3);
#endif
    std::puts("PASS storage lifecycle: widths/shared/isolation/moves/relocation/failures/atomic grow/hot offsets");
}
