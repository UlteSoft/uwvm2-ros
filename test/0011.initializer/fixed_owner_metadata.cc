// Check the actual shared-owner allocator rebound to the library control block.
// Optional ELF --wrap=malloc,--wrap=calloc,--wrap=realloc probes host refusal.
#include <uwvm2/uwvm/runtime/initializer/init.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#if defined(UWVM_TEST_WRAP_ALLOC)
# include "../0017.runtime/fast_io_allocation_probe.h"
#endif
#if defined(__linux__)
# include <sys/wait.h>
# include <unistd.h>
#endif

namespace init = uwvm2::uwvm::runtime::initializer::details;
namespace runtime = uwvm2::uwvm::runtime::storage;
namespace budget = uwvm2::utils::allocator::resource;
constexpr auto ordinary = init::initialization_purpose::ordinary;
constexpr auto restore = init::initialization_purpose::unpublished_restore;
constexpr auto pool = static_cast<unsigned>(budget::pool::initializer);
#define CHECK(condition) do { if(!(condition)) { std::fprintf(stderr, "initializer ownership line %d: %s\n", __LINE__, #condition); std::abort(); } } while(false)

template<typename Create>
static void check_owner(Create create)
{
    auto const initial{budget::live_bytes[pool].load()};
    auto owned{create()};
    CHECK(owned);
    auto const bytes{budget::live_bytes[pool].load() - initial};
    CHECK(bytes >= sizeof(*owned));
    std::weak_ptr weak{owned};
    auto alias{owned};
    owned.reset();
    CHECK(budget::live_bytes[pool].load() == initial + bytes);
    alias.reset();
    CHECK(weak.expired() && budget::live_bytes[pool].load() == initial + bytes);
    weak.reset();
    CHECK(budget::live_bytes[pool].load() == initial);

#if defined(__linux__)
    for(bool physical : {false, true})
    {
# if !defined(UWVM_TEST_WRAP_ALLOC)
        if(physical) { continue; }
# endif
        int output[2]{};
        CHECK(::pipe(output) == 0);
        auto child{::fork()};
        CHECK(child >= 0);
        if(child == 0)
        {
            ::close(output[0]);
            CHECK(::dup2(output[1], 2) == 2);
            ::close(output[1]);
# if defined(UWVM_TEST_WRAP_ALLOC)
            if(physical)
            {
                fast_io_allocation_probe::selected_size = bytes;
                fast_io_allocation_probe::once = fast_io_allocation_probe::armed = true;
            }
            else
# endif
            { budget::policies[pool] = {bytes - 1, initial + bytes}; }
            (void)create();
            ::_Exit(1);
        }
        ::close(output[1]);
        char message[512]{};
        auto const length{::read(output[0], message, sizeof(message)-1)};
        ::close(output[0]);
        int status{};
        CHECK(::waitpid(child, &status, 0) == child && WIFEXITED(status) && WEXITSTATUS(status) == 127);
        CHECK(length > 0 && std::strstr(message, "metadata-single-bytes/metadata-live-bytes"));
    }
#endif
}

int main()
{
    auto const initial{budget::live_bytes[pool].load()};
    {
        auto lease = [] { return init::make_initializer_shared<ordinary, runtime::gc_lease_owner>(); };
        check_owner(lease);
        check_owner([] { return init::make_initializer_shared<ordinary, runtime::tag_instance_identity const>(); });
        auto leases{lease()};
        runtime::gc_type::recursive_type_section types{};
        init::initialization_context<ordinary> world{};
        check_owner([&] { return world.make_store(types, leases, nullptr); });
#if defined(__cpp_exceptions)
        auto const before_restore{budget::live_bytes[pool].load()};
        auto const saved{budget::policies[pool]};
        budget::policies[pool].live = before_restore;
        bool refused{};
        try { (void)init::make_initializer_shared<restore, runtime::tag_instance_identity const>(); }
        catch(std::bad_alloc const&) { refused = true; }
        budget::policies[pool] = saved;
        CHECK(refused && budget::live_bytes[pool].load() == before_restore);
        CHECK((init::make_initializer_shared<restore, runtime::tag_instance_identity const>()));
#endif
    }
    CHECK(budget::live_bytes[pool].load() == initial);
    std::puts("PASS fixed initializer shared owners, imported alias and weak lifetime, ordinary and private refusal");
}
