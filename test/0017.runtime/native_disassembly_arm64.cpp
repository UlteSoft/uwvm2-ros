// LLVM MC component over explicit copied DATA, not a runtime ownership proof.
// The copied span ends exactly where an inaccessible virtual-memory page starts.
#if !defined(__APPLE__) || !defined(__aarch64__)
# error This test only runs on macOS ARM64.
#endif
#include <TargetConditionals.h>
#if !TARGET_OS_OSX
# error This test only runs on macOS ARM64.
#endif
#ifndef UWVM_USE_LLVM_JIT
# define UWVM_USE_LLVM_JIT
#endif
#include <uwvm2/uwvm/debugger/native_disassembly_window.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <span>

namespace disassembly = ::uwvm2::uwvm::debugger::native_disassembly;

int main()
{
    auto const page_length{::sysconf(_SC_PAGESIZE)};
    if(page_length < 4096) { return 1; }
    auto const page_size{static_cast<::std::size_t>(page_length)};
    auto* const pages{static_cast<::std::uint8_t*>(::mmap(nullptr, page_size * 2u,
        PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0))};
    if(pages == MAP_FAILED) { return 2; }
    if(::mprotect(pages + page_size, page_size, PROT_NONE) != 0) { return 3; }
    // AArch64 NOP (0xd503201f), the last complete instruction before the
    // inaccessible page. The decoder receives only these four copied bytes.
    ::std::uint8_t constexpr nop[]{0x1f, 0x20, 0x03, 0xd5};
    auto* const instruction{pages + page_size - sizeof(nop)};
    ::std::memcpy(instruction, nop, sizeof(nop));
    if(::mprotect(pages, page_size, PROT_READ) != 0) { return 4; }
    auto const copied{::std::span<::std::uint8_t const>{instruction, sizeof(nop)}};
    // The aligned display PC lies in PROT_NONE; only copied is readable.
    auto const pc{reinterpret_cast<::std::uintptr_t>(pages + page_size)};
    auto const decoded{disassembly::decode_copied(pc, copied)};
    if(!decoded || decoded.size != 4u || decoded.pc != pc ||
       ::std::memcmp(decoded.bytes.data(), nop, sizeof(nop)) != 0 ||
       ::std::strstr(decoded.text.data(), "nop") == nullptr) { return 5; }
    if(disassembly::decode_copied(pc, copied.first(3u)) ||
       disassembly::decode_copied(pc + 1u, copied)) { return 6; }

    disassembly::decoder context{};
    if(!context) { return 7; }
    auto const range{disassembly::decode_window_with(context, copied, pc, pc, 0, 0, 1u)};
    if(!range.available || range.count != 1u || !range.instructions[0u] ||
       range.instructions[0u].pc != pc || range.instructions[0u].size != 4u ||
       ::std::memcmp(range.instructions[0u].bytes.data(), nop, sizeof(nop)) != 0 ||
       ::std::strstr(range.instructions[0u].text.data(), "nop") == nullptr) { return 8; }
    if(disassembly::decode_window_with(context, copied.first(3u), pc, pc, 0, 0, 1u).available ||
       disassembly::decode_window_with(context, copied, pc, pc - 4u, 0, 0, 1u).available ||
       disassembly::decode_window_with(context, copied, pc, pc + 4u, 0, 0, 1u).available) { return 9; }
    if(::munmap(pages, page_size * 2u) != 0) { return 10; }
    ::std::puts("PASS ARM64 copied LLVM MC disassembly and range at guard page");
}
