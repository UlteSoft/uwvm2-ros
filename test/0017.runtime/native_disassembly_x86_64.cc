// LLVM MC component over explicit copied DATA; no runtime ownership proof.
#include <uwvm2/uwvm/debugger/native_disassembly_window.h>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <span>
#include <sys/mman.h>
#include <unistd.h>
#define CHECK(condition) do { if(!(condition)) { ::std::fprintf(stderr, "FAIL %d: %s\n", __LINE__, #condition); ::std::abort(); } } while(false)

int main()
{
#if defined(__linux__) && defined(__x86_64__) && defined(UWVM_USE_LLVM_JIT)
    namespace dis = ::uwvm2::uwvm::debugger::native_disassembly;
    auto const page{static_cast<::std::size_t>(::sysconf(_SC_PAGESIZE))};
    CHECK(page >= 4096u);
    void* const mapping{::mmap(nullptr, page * 2u, PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)};
    CHECK(mapping != MAP_FAILED);
    auto* const end{static_cast<::std::uint8_t*>(mapping) + page};
    // Copied addq $1, %rax; nop DATA. The last byte borders an inaccessible
    // page, so a 15-byte read from the one-byte suffix would overrun the span.
    ::std::uint8_t const code[]{0x48u, 0x83u, 0xc0u, 0x01u, 0x90u};
    ::std::memcpy(end - sizeof(code), code, sizeof(code));
    CHECK(::mprotect(end, page, PROT_NONE) == 0);
    CHECK(::mprotect(mapping, page, PROT_READ) == 0);
    auto const copied{::std::span<::std::uint8_t const>{end - sizeof(code), sizeof(code)}};
    // Display coordinates deliberately lie in PROT_NONE, independently of
    // the readable copied span. Neither decoder may dereference this PC.
    auto const pc{reinterpret_cast<::std::uintptr_t>(end)};
    auto const add{dis::decode_copied(pc, copied)};
    CHECK(add && add.size == 4u && add.pc == pc);
    CHECK(::std::strstr(add.text.data(), "add") != nullptr);
    for(::std::size_t index{}; index != 4u; ++index) { CHECK(add.bytes[index] == code[index]); }
    auto const nop{dis::decode_copied(pc + 4u, copied.subspan(4u))};
    CHECK(nop && nop.pc == pc + 4u && nop.size == 1u && nop.bytes[0] == 0x90u);
    CHECK(::std::strstr(nop.text.data(), "nop") != nullptr);
    CHECK(!dis::decode_copied(pc, copied.first(2u)));

    // Owner/stop coordinates are checked by the copied-range policy, not by
    // interpreting them as pointers or reviving the removed raw-address API.
    dis::decoder context{};
    CHECK(context);
    auto const range{dis::decode_window_with(context, copied, pc, pc + 4u, 0, -1, 2u)};
    CHECK(range.available && range.count == 2u);
    CHECK(range.instructions[0u].pc == pc && range.instructions[0u].size == 4u);
    CHECK(range.instructions[1u].pc == pc + 4u && range.instructions[1u].size == 1u);
    CHECK(!dis::decode_window_with(context, copied, pc, pc - 1u, 0, 0, 1u).available);
    CHECK(!dis::decode_window_with(context, copied, pc, pc + copied.size(), 0, 0, 1u).available);
    CHECK(!dis::decode_window_with(context, copied.first(2u), pc, pc, 0, 0, 1u).available);
    CHECK(::munmap(mapping, page * 2u) == 0);
    ::std::puts("PASS copied LLVM X86 disassembly and range at guard page");
#endif
}
