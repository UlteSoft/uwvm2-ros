/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)            *
 * Licensed under the APL-2.0 License (see LICENSE file).      *
 *************************************************************/
#pragma once
#include <uwvm2/utils/container/standard.h>
#include "uwvm_runtime_intel_jit_profile.h"
#if defined(UWVM_RUNTIME_INTEL_JIT_PROFILE) && UWVM_RUNTIME_INTEL_JIT_PROFILE == 1 && defined(UWVM_RUNTIME_LLVM_JIT)
# include <set>
# include <memory>
# include <llvm/ExecutionEngine/ExecutionEngine.h>
# include <llvm/ExecutionEngine/JITEventListener.h>
# include <llvm/ExecutionEngine/RuntimeDyld.h>
# include <llvm/Object/ObjectFile.h>
# include <llvm/Object/SymbolSize.h>
# include <llvm/Support/Error.h>

namespace uwvm2::runtime::lib::details::intel_jit_profile
{
    /** @brief Independently optional listener for actual loaded text symbols.
     * @details This helper is deliberately not wired into the VM yet. It emits
     * symbol identities only, never fabricated source rows. Object-loaded
     * callbacks precede MCJIT relocation: they only capture metadata. The actual
     * owner must call publish_finalized() after successful finalizeObject and
     * memory permission checks, before execution. Constructor opt-in
     * is host-owned and default off. The borrowed engine must outlive attached
     * listener state. Destroy this owner after execution drain and before code
     * arena retirement. For a retained arena, call detach_keep_code BEFORE the
     * temporary loader dies and retain this owner until that arena is drained.
     *
     * Only ordinary text ST_Function ranges are accepted. Descriptor ABIs and
     * non-text symbols are skipped explicitly; no arbitrary pointer is read to
     * find an entry. Object corruption/limits omit the object's profiling data
     * and set observation_failed; they never grant execution/read authority.
     */
    class listener final : public ::llvm::JITEventListener
    {
        registry registrations_;
        ::llvm::ExecutionEngine* engine_{};
        ::uwvm2::utils::container::std_string module_{};
        ::uwvm2::utils::container::std_set<::std::uint64_t> objects_{};
        struct pending_method
        {::std::uintptr_t address{};::std::size_t size{};::uwvm2::utils::container::std_string name;};
        ::uwvm2::utils::container::std_map<::std::uint64_t,::uwvm2::utils::container::std_vector<pending_method>> pending_{};
        ::std::size_t pending_methods_{};
        bool failed_{};
        static constexpr ::std::size_t maximum_object_bytes{64u * 1024u * 1024u};
        static constexpr ::std::size_t maximum_objects{4096u};
        void discard(::std::uint64_t key) noexcept
        {
            auto found=pending_.find(key);
            if(found!=pending_.end()){pending_methods_-=found->second.size();pending_.erase(found);}
            registrations_.retire(key);objects_.erase(key);
        }
        void fail(::std::uint64_t key) noexcept { discard(key); failed_ = true; }
    public:
        listener(::llvm::ExecutionEngine& engine, bool trusted_enable = false, ::std::string_view module = {})
            : registrations_{trusted_enable}
        {
            if(registrations_.state() != activation::ready) { return; }
            if(!valid_label(module, false)) { failed_ = true; return; }
            module_.assign(module);
            engine_ = &engine;
            engine_->RegisterJITEventListener(this);
        }
        ~listener() override { detach_keep_code(); }
        listener(listener const&) = delete;
        listener& operator=(listener const&) = delete;
        [[nodiscard]] activation state() const noexcept { return registrations_.state(); }
        [[nodiscard]] bool observation_failed() const noexcept { return failed_; }
        [[nodiscard]] ::std::size_t methods() const noexcept { return registrations_.size(); }
        [[nodiscard]] ::std::size_t pending_methods() const noexcept { return pending_methods_; }

        /** @brief Report real code only after owner-verified relocation and permission finalization.
         * @details This trusted call must precede execution. It deliberately
         * does not trigger compilation, finalize code or infer readiness from
         * an address. The owner additionally checks its memory manager's result.
         * Partial or throwing SDK publication retires that object's records;
         * observation failure never changes runtime execution authority.
         */
        bool publish_finalized() noexcept
        {
            if(!registrations_.owner_process())
            { return registrations_.state() == activation::disabled; }
            if(registrations_.state()!=activation::ready)return true;
            if(engine_==nullptr||engine_->hasError()){failed_=true;return false;}
            for(auto it=pending_.begin();it!=pending_.end();)
            {
                auto current=it++;auto key=current->first;bool good=true;
                try
                {
                    for(auto const& method:current->second)
                        if(registrations_.report(key,{method.address,method.size,method.name,module_})!=registration_status::reported)
                        {good=false;break;}
                }
                catch(...){good=false;}
                if(!good){fail(key);continue;}
                pending_methods_-=current->second.size();pending_.erase(current);
            }
            return !failed_;
        }

        /** @brief Detach the temporary loader, retaining real live-code registrations. */
        void detach_keep_code() noexcept
        {
            // A child may inherit a locked engine/SDK. It must not touch either
            // borrow; its process-local owner destruction sends no SDK event.
            if(!registrations_.owner_process()) { engine_ = nullptr; return; }
            if(engine_ != nullptr) { engine_->UnregisterJITEventListener(this); engine_ = nullptr; }
            if(!pending_.empty()){pending_.clear();pending_methods_=0;failed_=true;}
        }
        /** @brief Retire notifications after execution drain, before arena release. */
        void retire_code() noexcept { detach_keep_code(); registrations_.retire_all(); objects_.clear(); }
        void notifyObjectLoaded(ObjectKey key, ::llvm::object::ObjectFile const& object,
                                ::llvm::RuntimeDyld::LoadedObjectInfo const& loaded) override
        {
            if(engine_ == nullptr || registrations_.state() != activation::ready) { return; }
            try
            {
                if(object.getMemoryBufferRef().getBufferSize() > maximum_object_bytes || objects_.size() == maximum_objects ||
                   !objects_.insert(key).second) { fail(key); return; }
                ::std::size_t symbol_count{};
                for([[maybe_unused]] auto const& symbol : object.symbols())
                { if(++symbol_count > maximum_methods) { fail(key); return; } }
                for(auto const& sized : ::llvm::object::computeSymbolSizes(object))
                {
                    auto const& symbol{sized.first};
                    auto type{symbol.getType()};
                    if(!type) { ::llvm::consumeError(type.takeError()); fail(key); return; }
                    if(*type != ::llvm::object::SymbolRef::ST_Function || sized.second == 0u) { continue; }
                    auto section_result{symbol.getSection()};
                    if(!section_result) { ::llvm::consumeError(section_result.takeError()); fail(key); return; }
                    auto const section{*section_result};
                    if(section == object.section_end() || !section->isText()) { continue; }
                    auto address{symbol.getAddress()};
                    if(!address) { ::llvm::consumeError(address.takeError()); fail(key); return; }
                    auto name{symbol.getName()};
                    if(!name) { ::llvm::consumeError(name.takeError()); fail(key); return; }
                    auto const base{section->getAddress()}, size{section->getSize()}, loaded_base{loaded.getSectionLoadAddress(*section)};
                    if(*address < base || *address - base >= size || sized.second > size - (*address - base) ||
                       loaded_base == 0u || loaded_base > UINTPTR_MAX || *address - base > UINTPTR_MAX - loaded_base ||
                       sized.second > UINTPTR_MAX - loaded_base - (*address - base)) { fail(key); return; }
                    method_view view{static_cast<::std::uintptr_t>(loaded_base + *address - base),
                        static_cast<::std::size_t>(sized.second), {name->data(), name->size()}, module_};
                    if(!valid_method(view)||registrations_.size()+pending_methods_==maximum_methods){fail(key);return;}
                    pending_[key].push_back({view.address,view.size,::uwvm2::utils::container::std_string{view.name}});
                    ++pending_methods_;
                }
            }
            catch(...) { fail(key); }
        }
        void notifyFreeingObject(ObjectKey key) override
        { if(registrations_.owner_process()) { discard(key); } }
    };

    /** @brief Recoverable fast_io ownership for an optional code-arena observer.
     * @details Declare this owner after the actual engine owner. Reset it before
     * an explicit engine reset. Allocation failure returns an empty owner and
     * never terminates through the ordinary fast_io allocator. The standard
     * unique_ptr is only a pointer/deleter carrier; its backing allocation and
     * release both use the existing strict fast_io allocator.
     */
    struct listener_deleter
    {
        void operator()(listener* value) const noexcept
        {
            if(value == nullptr) { return; }
            ::std::destroy_at(value);
            ::uwvm2::utils::allocator::fast_io_strict::native_strict_typed_global_allocator<listener>::deallocate_n(value, 1u);
        }
    };
    using listener_owner = ::std::unique_ptr<listener, listener_deleter>;

    [[nodiscard]] inline listener_owner make_listener_owner(
        ::llvm::ExecutionEngine& engine, bool trusted_enable, ::std::string_view module = {}) noexcept
    {
        if(!trusted_enable) { return {}; }
        using allocator = ::uwvm2::utils::allocator::fast_io_strict::native_strict_typed_global_allocator<listener>;
        auto* const storage{allocator::allocate(1u)};
        if(storage == nullptr) { return {}; }
        try { ::std::construct_at(storage, engine, trusted_enable, module); }
        catch(...) { allocator::deallocate_n(storage, 1u); return {}; }
        return listener_owner{storage};
    }
}
#endif
