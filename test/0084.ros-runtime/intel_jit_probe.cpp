/** @file
 * @brief Controlled executable-code probe of Intel's official JIT Profiling API.
 * @details This is NOT a VM or a guest DWARF mapping qualification. x86-64 code
 * is emitted into RW memory, changed to RX, registered before execution, and
 * unregistered before unmapping. Its manually verified source spans refer only
 * to the accompanying controlled fixture. Requires the existing 64 GiB gate.
 */
#include <uwvm2/runtime/lib/uwvm_runtime_intel_jit_profile.h>
#include <array>
#include <chrono>
#include <cstring>
#include <iostream>
#include <sys/mman.h>
#include <unistd.h>
namespace profile = uwvm2::runtime::lib::details::intel_jit_profile;
static void require(bool okay, char const* message) { if(!okay) { std::cerr << message << '\n'; std::exit(1); } }
int main(int argc, char** argv)
{
    require(argc == 4, "Usage: intel-jit-probe off|profile SOURCE_LABEL SECONDS");
    bool const enabled{std::string_view{argv[1]} == "profile"};
    require(enabled || std::string_view{argv[1]} == "off", "unknown mode");
    double seconds{std::stod(argv[3])}; require(seconds > 0.0 && seconds <= 20.0, "bounded duration");
    constexpr std::array<unsigned char, 29u> bytes{
        0x31,0xc0,0x48,0x85,0xff,0x74,0x15,
        0x48,0x69,0xc0,0x0d,0x66,0x19,0x00,
        0x48,0x05,0x5f,0xf3,0x6e,0x3c,
        0x48,0x31,0xf8,0x48,0xff,0xcf,0x75,0xeb,0xc3};
    constexpr std::array lines{
        profile::line_boundary{2u,5u}, profile::line_boundary{7u,6u},
        profile::line_boundary{14u,8u}, profile::line_boundary{20u,9u},
        profile::line_boundary{23u,10u}, profile::line_boundary{26u,11u},
        profile::line_boundary{28u,6u}, profile::line_boundary{29u,13u}};
    long const page_size{::sysconf(_SC_PAGESIZE)}; require(page_size > 0, "page size");
    auto* code{::mmap(nullptr, static_cast<std::size_t>(page_size), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)};
    require(code != MAP_FAILED, "mmap failed");
    std::memcpy(code, bytes.data(), bytes.size());
    require(::mprotect(code, static_cast<std::size_t>(page_size), PROT_READ | PROT_EXEC) == 0, "W^X transition failed");
    using function = std::uint64_t(*)(std::uint64_t);
    function const kernel{reinterpret_cast<function>(code)};
    std::uint64_t reference{};
    for(std::uint64_t n{17u}; n != 0u; --n) { reference = (reference * 1664525u + 1013904223u) ^ n; }
    profile::registry registry{enabled};
    require(!enabled || registry.state() == profile::activation::ready, "official collector unavailable");
    std::uint64_t checksum{}, calls{};
    for(unsigned generation{}; generation != 2u; ++generation)
    {
        profile::method_view method{reinterpret_cast<std::uintptr_t>(code), bytes.size(),
            generation == 0u ? "uwvm.profile_probe.mix" : "uwvm.profile_probe.reused", "uwvm-profile-probe", argv[2], lines};
        auto const status{registry.report(generation, method)};
        require(status == (enabled ? profile::registration_status::reported : profile::registration_status::disabled), "load notification rejected");
        require(kernel(17u) == reference && kernel(0u) == 0u, "generated kernel semantics");
        auto const begin{std::chrono::steady_clock::now()};
        do { checksum += kernel(10000000u); ++calls; }
        while(std::chrono::duration<double>(std::chrono::steady_clock::now() - begin).count() < seconds / 2.0);
        registry.retire(generation); // completed synchronous execution precedes unload
        require(registry.size() == 0u, "retirement incomplete");
    }
    require(::munmap(code, static_cast<std::size_t>(page_size)) == 0, "unmap after final unload");
    std::cout << "PASS controlled JIT probe enabled=" << enabled << " generations=2 calls=" << calls << " checksum=" << checksum << '\n';
}
