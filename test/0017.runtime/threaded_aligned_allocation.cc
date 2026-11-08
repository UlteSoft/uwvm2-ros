#include <cstddef>
#include <cstdint>
#include <limits>
#include <uwvm2/uwvm/runtime/initializer/threaded_allocation.h>

using account = ::uwvm2::uwvm::runtime::initializer::threaded_allocation_account;
using compatible = ::fast_io::generic_allocator_adapter<::fast_io::c_malloc_allocator>;

template<unsigned Alignment> struct alignas(Alignment) value
{
    inline static unsigned alive{};
    ::std::uint64_t number{};
    explicit value(::std::uint64_t input) noexcept : number{input} { ++alive; }
    ~value() { --alive; }
};

template<typename T> int exercise()
{
    constexpr auto count{3uz};
    constexpr auto bytes{count * sizeof(T)};
    constexpr auto request{alignof(T) <= compatible::default_alignment ? bytes :
        bytes + alignof(T) + sizeof(void*)};
    auto* ledger{account::create(sizeof(account) + request)};
    if(ledger == nullptr) { return 1; }
    auto* memory{ledger->template try_c_malloc_vector_storage<T>(count)};
    if(memory == nullptr || reinterpret_cast<::std::uintptr_t>(memory) % alignof(T) != 0uz ||
        ledger->requested_bytes() != sizeof(account) + request) { return 2; }
    {
        ::fast_io::containers::vector<T, compatible> actual;
        actual.imp = {memory, memory, memory + count};
        actual.emplace_back(11u);
        actual.emplace_back(29u);
        actual.emplace_back(47u);
        if(T::alive != count || actual[0].number != 11u || actual[1].number != 29u || actual[2].number != 47u) { return 3; }
        if(ledger->template try_c_malloc_vector_storage<T>(1uz) != nullptr ||
            ledger->requested_bytes() != sizeof(account) + request) { return 4; }
    } // Real fast_io typed deallocate_n must recover/free exactly this storage.
    if(T::alive != 0u) { return 5; }
    ledger->release();
    ledger = account::create(sizeof(account) + request - 1uz);
    if(ledger == nullptr || ledger->template try_c_malloc_vector_storage<T>(count) != nullptr ||
        ledger->requested_bytes() != sizeof(account)) { return 6; }
    ledger->release();
    return 0;
}

int main()
{
    auto* ledger{account::create(sizeof(account) + 3uz)};
    if(ledger == nullptr || ledger->try_c_malloc_vector_storage<char>(0uz) != nullptr ||
        ledger->try_c_malloc_vector_storage<value<64>>(SIZE_MAX / sizeof(value<64>) + 1uz) != nullptr ||
        ledger->try_c_malloc_vector_storage<value<64>>(SIZE_MAX / sizeof(value<64>)) != nullptr ||
        ledger->requested_bytes() != sizeof(account)) { return 10; }
    auto* plain{ledger->try_c_malloc_vector_storage<char>(3uz)};
    if(plain == nullptr || ledger->requested_bytes() != sizeof(account) + 3uz) { return 11; }
    ::fast_io::typed_generic_allocator_adapter<compatible, char>::deallocate_n(plain, 3uz);
    ledger->release();
    for(unsigned iteration{}; iteration != 1000u; ++iteration)
    {
        if(auto const error{exercise<value<16>>()}) { return 20 + error; }
        if(auto const error{exercise<value<64>>()}) { return 40 + error; }
    }
}
