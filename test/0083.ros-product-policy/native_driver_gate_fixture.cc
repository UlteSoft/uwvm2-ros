// Uses the actual public stable table, including every wasm32/wasm64 entry.
// The runner generates driver_denied_calls.inc from the signature catalogue.
#include <uwvm2/uwvm/wasm/type/wasip1_api.h>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <type_traits>
#include <fcntl.h>
#include <unistd.h>

namespace api = uwvm2::uwvm::wasm::type;
static api::uwvm_wasip1_host_api_v1 const* table;
extern "C" void uwvm_set_wasip1_host_api_v1(api::uwvm_wasip1_host_api_v1 const* value) noexcept
{
    table = value;
    if(auto const* path = std::getenv("UWVM_DRIVER_GATE_TRACE"))
    {
        int const fd = ::open(path, O_WRONLY | O_CREAT | O_APPEND | O_CLOEXEC, 0600);
        if(fd >= 0) { (void)::write(fd, value ? "S1\n" : "S0\n", 3); ::close(fd); }
    }
}
template<class Result, class... Args> static bool denied(Result (*function)(Args...))
{
    if(!function) return false;
    if constexpr(std::is_void_v<Result>) { function(Args{}...); return true; }
    else return function(Args{}...) == static_cast<Result>(76);
}
static void put(std::byte* result, std::uint32_t value)
{
    std::memcpy(result, &value, 4);
    if(auto const* path = std::getenv("UWVM_DRIVER_GATE_TRACE"))
    {
        int const fd = ::open(path, O_WRONLY | O_CREAT | O_APPEND | O_CLOEXEC, 0600);
        if(fd >= 0)
        {
            char text[32]; int const size = std::snprintf(text, sizeof(text), "%u\n", value);
            if(size > 0) (void)::write(fd, text, static_cast<std::size_t>(size));
            ::close(fd);
        }
    }
}
static void api_present(std::byte* result, std::byte*) { put(result, table != nullptr); }
static void all_denied(std::byte* result, std::byte*)
{
    if(!table) { put(result, 999); return; }
    unsigned index{};
#define CHECK(name) do { ++index; if(!denied(table->name)) { put(result, index); return; } } while(false)
#include "driver_denied_calls.inc"
#undef CHECK
    put(result, index == 92u ? 0u : 998u);
}
static void args_probe(std::byte* result, std::byte*)
{ put(result, table ? static_cast<std::uint32_t>(table->args_sizes_get(0, 4)) : 999u); }
static void args64_probe(std::byte* result, std::byte*)
{ put(result, table ? static_cast<std::uint32_t>(table->args_sizes_get_wasm64(8, 16)) : 999u); }
static void denied_exits(std::byte* result, std::byte*)
{
    if(!table) { put(result, 999); return; }
    table->proc_exit(static_cast<uwvm2::imported::wasi::wasip1::abi::exitcode_t>(79));
    table->proc_exit_wasm64(static_cast<uwvm2::imported::wasi::wasip1::abi::exitcode_wasm64_t>(79));
    put(result, 0);
}
static void effect(char value)
{
    if(auto const* path = std::getenv("UWVM_DRIVER_EFFECT_MARKER"))
    {
        int const fd = ::open(path, O_WRONLY | O_CREAT | O_APPEND | O_CLOEXEC, 0600);
        if(fd >= 0) { (void)::write(fd, &value, 1); ::close(fd); }
    }
}
template<bool Wide> static void write_pair(std::byte* result, std::byte*)
{
    if(!table) { put(result, 999); return; }
    // The enclosing native callback owns a separate observable side effect.
    // Only a single provider may be retried: the sequence must stay exactly BE.
    effect('B');
    std::uint32_t first{}, second{};
    if constexpr(Wide)
    {
        first = static_cast<std::uint32_t>(table->fd_write_wasm64(1, 96, 1, 112));
        second = static_cast<std::uint32_t>(table->fd_write_wasm64(1, 96, 1, 120));
    }
    else
    {
        first = static_cast<std::uint32_t>(table->fd_write(1, 64, 1, 80));
        second = static_cast<std::uint32_t>(table->fd_write(1, 64, 1, 84));
    }
    effect('E');
    put(result, first * 256u + second);
}
static void complete(std::byte* result, std::byte*)
{
    // An early native proc_exit(0) is not a passing denial test. The guest must
    // return from all calls and explicitly invoke this final witness.
    bool written{};
    if(auto const* path = std::getenv("UWVM_DRIVER_GATE_MARKER"))
    {
        int const fd = ::open(path, O_WRONLY | O_CREAT | O_APPEND | O_CLOEXEC, 0600);
        if(fd >= 0) { written = ::write(fd, "1", 1) == 1; ::close(fd); }
    }
    put(result, written ? 0u : 1u);
}
extern "C"
{
    struct module_name { char const* name; std::size_t size; };
    struct function {
        char const* name; std::size_t name_size;
        std::uint_least8_t const* parameters; std::size_t parameter_count;
        std::uint_least8_t const* results; std::size_t result_count;
        void (*invoke)(std::byte*, std::byte*);
    };
    struct functions { function const* begin; std::size_t size; };
    module_name uwvm_get_module_name() { return {"driver", 6}; }
    functions uwvm_function()
    {
        static std::uint_least8_t const result[] = {0x7f};
        static function const entries[] = {
            {"api_present", 11, nullptr, 0, result, 1, api_present},
            {"all_denied", 10, nullptr, 0, result, 1, all_denied},
            {"args_probe", 10, nullptr, 0, result, 1, args_probe},
            {"args64_probe", 12, nullptr, 0, result, 1, args64_probe},
            {"denied_exits", 12, nullptr, 0, result, 1, denied_exits},
            {"write_pair", 10, nullptr, 0, result, 1, write_pair<false>},
            {"write_pair64", 12, nullptr, 0, result, 1, write_pair<true>},
            {"complete", 8, nullptr, 0, result, 1, complete}
        };
        return {entries, sizeof(entries) / sizeof(entries[0])};
    }
}
