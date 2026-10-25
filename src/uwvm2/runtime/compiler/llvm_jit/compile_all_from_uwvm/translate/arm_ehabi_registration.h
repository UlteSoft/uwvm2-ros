/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)           *
 * Licensed under the APL-2.0 License (see LICENSE file).     *
 *************************************************************/
#pragma once

// GNU/Linux ARM EHABI's native address-sized lookup ABI. This registry belongs only
// to live SectionMemoryManager allocations. It is not a debugger stack/memory
// interface, and no caller-supplied address can register a table.
#if defined(UWVM2_ENABLE_LINUX_ARM_EHABI_PRODUCT) && UWVM2_ENABLE_LINUX_ARM_EHABI_PRODUCT == 1 && \
    defined(__linux__) && defined(__arm__) && !defined(__ARM_DWARF_EH__)
# include <features.h>
# if defined(__GLIBC__)
#  include <climits>
#  include <cstdint>
#  include <cstring>
#  include <limits>
#  include <mutex>
#  include <new>
#  include <fast_io.h>
#  include <vector>
#  include <dlfcn.h>
#  if defined(UWVM_RUNTIME_LLVM_JIT) || defined(UWVM_USE_LLVM_JIT) || defined(UWVM_USE_DEFAULT_JIT)
// Parse the real LLVM ELF object declarations before glibc link.h publishes
// its unscoped ELF macros. Loading these later breaks both the ELF enums and
// ELFObjectFile templates during a genuine ARM EHABI + registered-CFI build.
// Keep all platform macros intact; standalone registry tests need no LLVM.
#   include <llvm/Object/ELFObjectFile.h>
#  endif
#  include <link.h>
#  include <unwind.h>

extern "C" ::std::uint32_t uwvm_libunwind_arm_exidx_lookup_abi_v1() noexcept __attribute__((weak));

// Exact paired ARM EHABI entry points. Static libunwind need not publish
// these in the executable's dynamic symbol table; the owned loader binds them.
extern "C" _Unwind_Reason_Code __aeabi_unwind_cpp_pr0(_Unwind_State, _Unwind_Control_Block*, _Unwind_Context*);
extern "C" _Unwind_Reason_Code __aeabi_unwind_cpp_pr1(_Unwind_State, _Unwind_Control_Block*, _Unwind_Context*);
extern "C" _Unwind_Reason_Code __aeabi_unwind_cpp_pr2(_Unwind_State, _Unwind_Control_Block*, _Unwind_Context*);

extern "C" ::std::uintptr_t __gnu_Unwind_Find_exidx(::std::uintptr_t, int*) noexcept;
namespace uwvm2::runtime::compiler::llvm_jit::details
{
    class arm_ehabi_registration_owner;
    struct arm_ehabi_owned_range { ::std::uintptr_t begin{}, size{}; };
    struct arm_ehabi_owned_table
    {
        ::std::uint32_t const* entries{};
        unsigned count{};
        // Each entry is bounded by the next start in this table and its own
        // actual code allocation. Never let the final entry cover the VM.
        ::std::vector<arm_ehabi_owned_range> functions{};
    };
    class arm_ehabi_registry
    {
        friend class arm_ehabi_registration_owner;
        struct owner_tables { void const* owner{}; ::std::vector<arm_ehabi_owned_table> tables{}; };
        ::std::mutex mutex_{};
        ::std::vector<owner_tables> owners_{};
        bool publish(void const* owner, ::std::vector<arm_ehabi_owned_table> tables)
        {
            ::std::lock_guard lock{mutex_};

            for(auto const& table: tables)
            {
                for(auto const& range: table.functions)
                {
                    for(auto const& old: owners_)
                    { if(old.owner == owner) { continue; } for(auto const& prior: old.tables) { for(auto const& other: prior.functions)
                      { if(range.begin < other.begin + other.size && other.begin < range.begin + range.size) { return false; } } } }
                }
            }
            for(auto& old: owners_) { if(old.owner == owner) { old.tables = ::std::move(tables); return true; } }
            owners_.push_back({owner, ::std::move(tables)}); return true;
        }
        void retire(void const* owner) noexcept
        {
            ::std::lock_guard lock{mutex_};
            for(auto i{owners_.begin()}; i != owners_.end(); ++i)
            { if(i->owner == owner) { owners_.erase(i); return; } }
        }
    public:
        // The caller is the native unwinder, with the execution owner retained
        // until its chain has drained. The lookup copies no native frame bytes.
        [[nodiscard]] ::std::uintptr_t find(::std::uintptr_t pc, int& count) noexcept
        {
            ::std::lock_guard lock{mutex_};
            for(auto const& owner: owners_)
            { for(auto const& table: owner.tables) { for(auto const& range: table.functions)
              { if(pc >= range.begin && pc - range.begin < range.size)
                { count = static_cast<int>(table.count); return reinterpret_cast<::std::uintptr_t>(table.entries); } } } }
            return 0u;
        }
    };
    [[nodiscard]] inline arm_ehabi_registry& get_arm_ehabi_registry() noexcept
    {
        // Process-lifetime registry: destruction of another static owner must
        // still be able to remove its tables before freeing JIT allocations.
        static auto* const value{new(::std::nothrow) arm_ehabi_registry};
        if(value == nullptr) { ::fast_io::fast_terminate(); } return *value;
    }
    // A private base of the real memory manager, so ordinary headers and
    // module partitions share one global registry and lookup hook. There is no
    // forward declaration of a module-owned manager in the global fragment.
    class arm_ehabi_registration_owner
    {
    protected:
        [[nodiscard]] bool publish_arm_ehabi_tables(::std::vector<arm_ehabi_owned_table> tables)
        { return get_arm_ehabi_registry().publish(this, ::std::move(tables)); }
        void retire_arm_ehabi_tables() noexcept { get_arm_ehabi_registry().retire(this); }
    };
    [[nodiscard]] inline bool arm_ehabi_lookup_is_exported() noexcept
    {
        return ::uwvm_libunwind_arm_exidx_lookup_abi_v1 != nullptr &&
            ::uwvm_libunwind_arm_exidx_lookup_abi_v1() == 1u &&
            ::dlsym(RTLD_DEFAULT, "__gnu_Unwind_Find_exidx") ==
            reinterpret_cast<void*>(reinterpret_cast<::std::uintptr_t>(&::__gnu_Unwind_Find_exidx));
    }
    [[nodiscard]] inline ::std::uintptr_t arm_ehabi_prel31(::std::uint32_t const* word) noexcept
    {
        ::std::uint32_t value{}; ::std::memcpy(&value, word, sizeof(value));
        auto const signed_offset{static_cast<::std::int32_t>(value << 1u) >> 1u};
        // EHABI specifies modular 32-bit PREL31 arithmetic. No result is read
        // until an exact retained code/data allocation contains it.
        return reinterpret_cast<::std::uintptr_t>(word) + static_cast<::std::uintptr_t>(signed_offset);
    }
}
// Executable/shared-product links must export this exact symbol. A missing
// export fails finalization; enabling a macro never substitutes for the hook.
extern "C" __attribute__((used, visibility("default"))) inline ::std::uintptr_t
__gnu_Unwind_Find_exidx(::std::uintptr_t pc, int* count) noexcept
{
    if(count == nullptr) { return 0u; }
    *count = 0;
    if(auto const table{::uwvm2::runtime::compiler::llvm_jit::details::get_arm_ehabi_registry().find(pc, *count)}; table != 0u)
    { return table; }
    // Match glibc's static-image lookup for every non-JIT PC. Do not recurse
    // through this interposed symbol or substitute a JIT table for VM code.
    struct static_lookup
    {
        ::std::uintptr_t pc{}, table{};
        int entries{};
    } lookup{pc};
    // dl_iterate_phdr is available on the selected older ROS libc ABIs too.
    // A static table must belong to the same loader-owned executable image.
    ::dl_iterate_phdr(+[](::dl_phdr_info* image, ::std::size_t, void* state) noexcept
    {
        auto& result{*static_cast<static_lookup*>(state)};
        bool contains{};
        for(::std::size_t index{}; index != image->dlpi_phnum; ++index)
        {
            auto const& segment{image->dlpi_phdr[index]};
            auto const start{image->dlpi_addr + segment.p_vaddr};
            if(segment.p_type == PT_LOAD && (segment.p_flags & PF_X) != 0 &&
               result.pc >= start && result.pc - start < segment.p_memsz)
            { contains = true; break; }
        }
        if(!contains) { return 0; }
        for(::std::size_t index{}; index != image->dlpi_phnum; ++index)
        {
            auto const& segment{image->dlpi_phdr[index]};
            if(segment.p_type != PT_ARM_EXIDX) { continue; }
            if(segment.p_memsz == 0u || segment.p_memsz % 8u != 0u ||
               segment.p_memsz / 8u > static_cast<::std::size_t>((::std::numeric_limits<int>::max)()))
            { return 1; }
            auto const start{image->dlpi_addr + segment.p_vaddr};
            for(::std::size_t allocation{}; allocation != image->dlpi_phnum; ++allocation)
            {
                auto const& readable{image->dlpi_phdr[allocation]};
                auto const begin{image->dlpi_addr + readable.p_vaddr};
                if(readable.p_type == PT_LOAD && (readable.p_flags & PF_R) != 0 &&
                   start >= begin && start - begin <= readable.p_memsz &&
                   segment.p_memsz <= readable.p_memsz - (start - begin))
                {
                    result.table = start;
                    result.entries = static_cast<int>(segment.p_memsz / 8u);
                    return 1;
                }
            }
            return 1;
        }
        return 1;
    }, &lookup);
    *count = lookup.entries;
    return lookup.table;
}
# endif
#endif
