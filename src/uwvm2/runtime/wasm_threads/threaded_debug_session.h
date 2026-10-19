#pragma once
// Stage A: Host-only bounded metadata snapshots. Not a Guest memory authority,
// pause/step capability, ordinary debugger provider, or an unwind implementation.
#ifndef UWVM_MODULE
#include <cstddef>
#include <cstdint>
#include <memory>
#include <span>
#include <utility>
#include <vector>
#endif
namespace uwvm2::uwvm::runtime::initializer { extern "C++" { class ThreadedChildWorld; } }
#ifndef UWVM_MODULE
namespace uwvm2::uwvm::runtime::full { class full_source_instance; }
#endif
namespace uwvm2::runtime::compiler::uwvm_int::threaded { extern "C++" { class engine; } }
namespace uwvm2::runtime::lib { extern "C++" { class ThreadedLlvmChild; class runtime_threaded_profile_bridge; } }
namespace uwvm2::runtime::wasm_threads
{
    class ThreadedGroup;
    enum class debug_backend : unsigned { interpreter, llvm };
    enum class debug_read_status : unsigned { ok, insufficient_capacity, stale, unavailable };
    enum class debug_worker_state : unsigned { native_entry, body_left };
    struct debug_function_row
    {
        ::std::size_t module{}, function{};
        ::std::uint64_t wasm_begin{}, wasm_end{};
        ::std::uintptr_t native_begin{}, native_bytes{};
        ::std::size_t logical_begin{}, logical_count{}, fixed_native_frame_bytes{};
        bool raw_adapter{}, fixed_native_frame_available{};
    };
    struct debug_logical_pc_row
    { ::std::size_t function{}, instruction{}; ::std::uint64_t wasm_offset{}; };
    struct debug_cfi_row { ::std::uintptr_t address{}; ::std::size_t bytes{}; };
    struct debug_metadata_counts { ::std::size_t functions{}, logical_pcs{}, cfi_ranges{}; };

    // Only an actual compiler can construct this image. It is privately owned
    // by the actual engine; its World pins the canonical Source/admission. Group
    // retains that exact ChildWork/engine/native arena until physical pthread
    // join (including TLS destructors). No shared_ptr<void>/ready-bit provider.
    class code_debug_image final
    {
        friend class ::uwvm2::runtime::compiler::uwvm_int::threaded::engine;
        friend class ::uwvm2::runtime::lib::ThreadedLlvmChild;
        using world_owner=::std::shared_ptr<::uwvm2::uwvm::runtime::initializer::ThreadedChildWorld const>;
        world_owner world_;
        debug_backend backend_;
        ::std::vector<debug_function_row> functions_;
        ::std::vector<debug_logical_pc_row> logical_;
        ::std::vector<debug_cfi_row> cfi_;
        explicit code_debug_image(world_owner const& world,debug_backend backend) noexcept:world_{world},backend_{backend}{}
    public:
        code_debug_image(code_debug_image const&)=delete;
        code_debug_image& operator=(code_debug_image const&)=delete;
        ~code_debug_image()=default;
        [[nodiscard]] auto const& retained_world() const noexcept{return world_;}
        [[nodiscard]] debug_backend backend() const noexcept{return backend_;}
        [[nodiscard]] auto functions() const noexcept{return ::std::span<debug_function_row const>{functions_};}
        [[nodiscard]] auto logical_pcs() const noexcept{return ::std::span<debug_logical_pc_row const>{logical_};}
        [[nodiscard]] auto cfi_ranges() const noexcept{return ::std::span<debug_cfi_row const>{cfi_};}
        [[nodiscard]] debug_metadata_counts counts() const noexcept{return {functions_.size(),logical_.size(),cfi_.size()};}
        [[nodiscard]] ::std::size_t allocated_bytes() const noexcept
        {
            ::std::size_t n{};
            auto add=[&](::std::size_t count,::std::size_t width) noexcept
            {if(n==SIZE_MAX||count>(SIZE_MAX-n)/width){n=SIZE_MAX;}else n+=count*width;};
            add(functions_.capacity(),sizeof(debug_function_row));add(logical_.capacity(),sizeof(debug_logical_pc_row));
            add(cfi_.capacity(),sizeof(debug_cfi_row));return n;
        }
        // LLVM currently supplies function-level relocated extents only; neither
        // that fact nor CFI registration promises opcode/line/local information.
        [[nodiscard]] bool exact_logical_pc_map() const noexcept{return backend_==debug_backend::interpreter;}
    };
    class threaded_debug_worker_ticket final
    {
        friend class ThreadedGroup;
        ::std::weak_ptr<ThreadedGroup> group_;
        ::std::weak_ptr<::uwvm2::uwvm::runtime::initializer::ThreadedChildWorld const> world_;
        ::std::weak_ptr<::uwvm2::uwvm::runtime::full::full_source_instance const> source_;
        code_debug_image const* image_{};
        ::std::size_t position_{};
        ::std::uint64_t incarnation_{}, session_epoch_{}, source_generation_{}, validation_epoch_{};
    public:
        threaded_debug_worker_ticket() noexcept=default;
    };
    struct debug_worker_snapshot
    {
        ::std::uint32_t tid{};
        ::std::uint64_t native_tid{}, slot_incarnation{}, source_generation{}, source_validation_epoch{};
        ::std::size_t slot{}, source_module{};
        debug_worker_state state{};
        debug_backend backend{};
        debug_metadata_counts metadata{};
        bool logical_pc_map_available{}, source_lines_available{}, locals_available{}, native_unwind_implemented{};
    };
    class threaded_debug_session final
    {
        friend class ThreadedGroup;
        ::std::weak_ptr<ThreadedGroup> group_;
        ::std::uint64_t epoch_{};
        threaded_debug_session(::std::weak_ptr<ThreadedGroup> group,::std::uint64_t epoch) noexcept
            :group_{::std::move(group)},epoch_{epoch}{}
    public:
        threaded_debug_session() noexcept=default;
        [[nodiscard]] debug_read_status workers(::std::span<threaded_debug_worker_ticket>,::std::size_t& needed) const noexcept;
        [[nodiscard]] debug_read_status read(threaded_debug_worker_ticket const&,debug_worker_snapshot&,
            ::std::span<debug_function_row>,::std::span<debug_logical_pc_row>,::std::span<debug_cfi_row>) const noexcept;
    };
}
