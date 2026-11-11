#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/runtime/memtag/bridge.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
namespace mt=uwvm2::extension::memtag;
namespace rt=uwvm2::runtime::memtag;
namespace linear=uwvm2::object::memory::linear;
#define CHECK(x) do { if(!(x)) { std::fprintf(stderr,"assertion %u\n",__LINE__);std::abort(); } } while(false)
int main(int argc,char** argv)
{
    CHECK(argc==3);
    int output=::open(argv[2],O_WRONLY|O_CREAT|O_TRUNC|O_CLOEXEC,0600);CHECK(output>=3);
    mt::report_descriptor=output;mt::selected_report_action.store(mt::report_action::silent_log);
    mt::selected_fault_delivery.store(std::strcmp(argv[1],"sync")==0?mt::fault_delivery::synchronous:mt::fault_delivery::software_async);
    mt::asynchronous_worker_count.store(4);
    linear::mmap_memory_t owner{65536,linear::mmap_memory_status_t::wasm32};owner.init_by_page_count(1,2);
    owner.diagnostic_owner_memory_index=17;
    CHECK(owner.enable_memtag(mt::address_width::i32,true)==mt::storage_error::none);
    using view=mt::shadow_view<std::uint32_t,true>;
    auto tags=owner.memtag_shadow.view<std::uint32_t,true>(65536);CHECK(tags.store(view::insert(0,5),65536));
    std::atomic<unsigned> callbacks{};
    std::vector<std::thread> threads;
    for(unsigned worker=0;worker!=8;++worker)threads.emplace_back([&,worker]
    {
        rt::deferred::entry_scope entry;CHECK(entry.enter());rt::deferred::set_origin(worker,3,9);
        {
            rt::deferred::entry_scope nested;CHECK(nested.enter());
            rt::deferred::set_origin(99,88,77);nested.finish();
            CHECK(rt::deferred::origin_module==worker && rt::deferred::origin_function==3 && rt::deferred::origin_offset==9);
        }
        for(unsigned iteration=0;iteration!=1000;++iteration)
        {
            CHECK((rt::with_guest_access<std::uint32_t,true>(owner,view::insert(15,6),0,32,
                [&](std::span<std::byte> bytes) noexcept {CHECK(bytes.size()==32);callbacks.fetch_add(1);} )));
            CHECK((rt::with_checked_access<std::uint32_t,true>(owner,view::insert(32,6),0,1,
                [&](std::span<std::byte>) noexcept {callbacks.fetch_add(1);} )));
        }
        bool called{};auto const bounds=rt::with_guest_access<std::uint32_t,true>(owner,view::insert(65535,6),0,2,
            [&](std::span<std::byte>) noexcept {called=true;});
        CHECK(!bounds && bounds.error==mt::access_error::out_of_bounds && !called);
        entry.finish();
    });
    for(auto& thread:threads)thread.join();
    CHECK(callbacks.load()==16000);CHECK(!uwvm2::runtime::diagnostic::crash::first_fault.load());
    CHECK(::close(output)==0);std::puts("PASS: 32000 lossless reports; callbacks continued; bounds refused");
}
