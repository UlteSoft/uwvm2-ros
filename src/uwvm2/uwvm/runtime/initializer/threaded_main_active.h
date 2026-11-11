// Finite, nonallocating actual-parent active-table initialization. SOURCE ONLY.
#pragma once
#ifndef UWVM_MODULE
# include <atomic>
# include <mutex>
# include "threaded_bootstrap.h"
# include <uwvm2/runtime/wasm_threads/thread_group.h>
#endif
#if defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD) && defined(UWVM_CPP_EXCEPTIONS) && defined(__linux__)
namespace uwvm2::uwvm::runtime::initializer
{
    enum class threaded_main_active_status : unsigned char
    { applied, invalid_identity, invalid_reservation, already_effectful, unsupported_shape, bounds, stopped };

    class ThreadedMainActiveSegments final
    {
        friend class ::uwvm2::runtime::lib::runtime_threaded_profile_bridge;
        using admission = ::uwvm2::runtime::wasm_threads::ThreadedSpawnAdmission;
        using group = ::uwvm2::runtime::wasm_threads::ThreadedGroup;
        using world = ThreadedChildWorld;
        using source = ::uwvm2::uwvm::runtime::full::full_source_instance;
        ThreadedMainActiveSegments() = delete;

        // Extra cold preloads may contribute cached code without participating
        // in the child world. Authenticate their actual source and require that
        // instantiation has no effects or authority. This does not permit a
        // unknown native or unvalidated Wasm code into the child graph.
        [[nodiscard]] static bool dormant_function_preload(source::owner const& owner,
            ::uwvm2::uwvm::runtime::storage::wasm_module_storage_t const& module) noexcept
        {
            auto const id{owner->bound_initialized_module_id(&module)};
            auto const* file{owner->bound_initialized_file(id,&module)};
            if(file==nullptr || file==&owner->file() || file->binfmt_ver!=1u || !file->has_owned_source_image()) { return false; }
            if(!module.imported_function_vec_storage.empty() || !module.imported_memory_vec_storage.empty() ||
               !module.imported_table_vec_storage.empty() || !module.imported_global_vec_storage.empty() ||
               !module.imported_tag_vec_storage.empty() || !module.local_defined_memory_vec_storage.empty() ||
               !module.local_defined_table_vec_storage.empty() || !module.local_defined_global_vec_storage.empty() ||
               !module.local_defined_tag_vec_storage.empty() || !module.local_defined_element_vec_storage.empty() ||
               !module.local_defined_data_vec_storage.empty() ||
               module.local_defined_function_vec_storage.size()!=module.local_defined_code_vec_storage.size()) { return false; }
            return [&]<::uwvm2::parser::wasm::concepts::wasm_feature... Fs>(
                ::uwvm2::utils::container::tuple<Fs...>) noexcept
            {
                namespace w1=::uwvm2::parser::wasm::standard::wasm1::features;
                namespace w11=::uwvm2::parser::wasm::standard::wasm1p1::features;
                auto const& sections{file->wasm_module_storage.wasm_binfmt_ver1_storage.sections};
                auto absent{[&]<typename Section>() noexcept
                { return ::uwvm2::parser::wasm::concepts::operation::get_first_type_in_tuple<Section>(sections).sec_span.sec_begin==nullptr; }};
                // Only type/function/code/export/custom declarations can remain.
                return absent.template operator()<w1::import_section_storage_t<Fs...>>() &&
                    absent.template operator()<w1::memory_section_storage_t<Fs...>>() &&
                    absent.template operator()<w1::table_section_storage_t<Fs...>>() &&
                    absent.template operator()<w1::global_section_storage_t<Fs...>>() &&
                    absent.template operator()<w1::start_section_storage_t>() &&
                    absent.template operator()<w1::element_section_storage_t<Fs...>>() &&
                    absent.template operator()<w1::data_section_storage_t<Fs...>>() &&
                    absent.template operator()<w11::tag_section_storage_t<Fs...>>() &&
                    absent.template operator()<w11::data_count_section_storage_t<Fs...>>();
            }(::uwvm2::uwvm::wasm::feature::wasm_binfmt1_features);
        }

        // Only the actual Runtime bridge, retaining its admitted startup state,
        // calls this after all backend construction and before native launch.
        // The real Group reservation is checked under its own native mutex.
        [[nodiscard]] static threaded_main_active_status apply(admission const& actual,
            world::owner const& parent_world, group& native, group::SpawnReservation const& reservation) noexcept
        {
            using namespace ::uwvm2::uwvm::runtime::storage;
            auto const retained{actual.retain()};
            auto const native_owner{native.canonical_owner_.lock()};
            if(!retained || retained.get()!=&actual || !actual.owns_actual_generation() || !native_owner ||
               native_owner.get()!=&native || native.admission_.get()!=&actual || !parent_world ||
               &parent_world->admission()!=&actual || parent_world->module(0)!=&actual.source_module() ||
               parent_world->source_owner()!=actual.source_owner()) { return threaded_main_active_status::invalid_identity; }
            auto const code{parent_world->validated_code()};
            if(!code || !code->covers(*parent_world)) { return threaded_main_active_status::invalid_identity; }
            // This runs only on the dedicated compiler worker, outside the
            // Runtime registry/publication and Group locks. Management waiters
            // release this mutex before waiting for worker retirement. Ordinary
            // contention is not an invalid admission: wait for the lock, then
            // recheck the retained reservation and stop state before any effect.
            ::std::unique_lock lock{native.mutex_};
            if(reservation.group_!=&native || reservation.retained_group_.get()!=&native || reservation.position_!=0uz ||
               reservation.tid_!=0u || native.initialization_error_!=0 || native.stop_fd_<0 || !native.accepting_ ||
               native.slot_count_.load(::std::memory_order_acquire)==0uz || native.slots_[0].group!=&native || native.occupied_!=1uz || native.occupied_children_!=0uz ||
               !native.initial_main_reserved_ || native.slots_[0].phase!=group::slot_phase::preparing ||
               native.slots_[0].tid!=0u || !native.slots_[0].initial_main || native.slots_[0].work ||
               native.slots_[0].launch_published) { return threaded_main_active_status::invalid_reservation; }
            if(native.ever_launched_.load(::std::memory_order_acquire) ||
               native.shared_effects_started_.load(::std::memory_order_acquire)) { return threaded_main_active_status::already_effectful; }
            auto stopped{[&]() noexcept { return native.stop_requested_.load(::std::memory_order_acquire); }};
            if(stopped()) { return threaded_main_active_status::stopped; }
            auto const& owner{actual.source_owner()};
            auto const selected{::uwvm2::uwvm::runtime::full::selected_full_source_owner_pin()};
            if(!source::has_canonical_owner(owner) || selected.get()!=owner.get() ||
               selected.owner_before(owner) || owner.owner_before(selected) ||
               owner->actual_validated_file(actual.source_module_id(),actual.source_validation_epoch(),&actual.source_module())!=parent_world->file(0))
            { return threaded_main_active_status::invalid_identity; }
            auto& registry{::uwvm2::uwvm::runtime::storage::active_runtime_registry()};
            if(&registry!=&owner->registry()) { return threaded_main_active_status::invalid_identity; }
            auto const main_member{registry.find(owner->file().module_name)};
            if(main_member==registry.end() || &main_member->second!=&actual.source_module()) { return threaded_main_active_status::invalid_identity; }
            auto& main{main_member->second};
            if(::std::atomic_ref<bool>{main.gc_collection_phase.active_segments_ready}.load(::std::memory_order_acquire))
            { return threaded_main_active_status::already_effectful; }

            // Exactly one source-bound shared-memory leaf participates in this
            // profile. Unused function-only preloads have no effects to replay.
            ::std::optional<ThreadedMemoryProvider> provider;
            for(auto& [name,module] : registry)
            {
                static_cast<void>(name);
                if(&module==&main) { continue; }
                if(::std::atomic_ref<bool>{module.gc_collection_phase.active_segments_ready}.load(::std::memory_order_acquire))
                { return threaded_main_active_status::already_effectful; }
                bool linked_code{};
                for(::std::size_t m{1u};m<parent_world->module_count();++m)
                    if(parent_world->module(m)==&module && parent_world->original_module(m)==&module) { linked_code=true;break; }
                if(linked_code || dormant_function_preload(owner,module)) { continue; }
                if(provider) { return threaded_main_active_status::unsupported_shape; }
                threaded_provider_error error{};
                provider=ThreadedMemoryProvider::inspect(owner,&module,actual.max_shared_memory_bytes(),error);
                if(!provider || error!=threaded_provider_error::ok) { return threaded_main_active_status::unsupported_shape; }
            }
            if(!provider || main.imported_memory_vec_storage.size()!=1uz || !main.local_defined_memory_vec_storage.empty() ||
               !main.imported_table_vec_storage.empty()) { return threaded_main_active_status::unsupported_shape; }
            auto const& imported{main.imported_memory_vec_storage.index_unchecked(0uz)};
            if(imported.link_kind!=imported_memory_storage_t::imported_memory_link_kind::defined ||
               imported.target.defined_ptr!=&provider->memory() || parent_world->shared_memory(0)!=&provider->memory())
            { return threaded_main_active_status::unsupported_shape; }
            auto const provider_member{registry.find(provider->file().module_name)};
            if(provider_member==registry.end() || &provider_member->second!=&provider->module() ||
               ::std::atomic_ref<bool>{provider_member->second.gc_collection_phase.active_segments_ready}.load(::std::memory_order_acquire))
            { return threaded_main_active_status::already_effectful; }
            // No expression-payload materialization, aggregate roots or active
            // shared-data replay is admitted. All rejection happens pre-effect.
            for(::std::size_t m{};m<parent_world->module_count();++m)
            {
                auto it=registry.find(parent_world->file(m)->module_name);
                if(it==registry.end() || &it->second!=parent_world->module(m) ||
                    &it->second!=parent_world->original_module(m))return threaded_main_active_status::invalid_identity;
                auto& member=it->second;
            for(auto const& data:member.local_defined_data_vec_storage)
            {
                if(data.data_type_ptr==nullptr || data.data_type_ptr->storage.segment.active ||
                   data.data.kind!=wasm_data_segment_kind::passive) { return threaded_main_active_status::unsupported_shape; }
            }
            for(auto const& table:member.local_defined_table_vec_storage)
            {
                if(table.table_type_ptr==nullptr || table.owner_module_rt_ptr!=&member ||
                   table.elems.size()>actual.max_table_elements() ||
                   runtime_table_family(table)!=runtime_table_reference_family::function ||
                   static_cast<unsigned>(table.table_type_ptr->reftype)!=0x70u)
                { return threaded_main_active_status::unsupported_shape; }
            }
            ::std::size_t visited{};
            for(auto const& segment:member.local_defined_element_vec_storage)
            {
                if(stopped()) { return threaded_main_active_status::stopped; }
                if(segment.element_type_ptr==nullptr || runtime_element_family(segment,member)!=runtime_table_reference_family::function)
                { return threaded_main_active_status::unsupported_shape; }
                auto const& declaration{segment.element_type_ptr->storage.segment};
                auto const& element{segment.element};
                if(!declaration.vec_expr.empty() || element.funcref_begin || element.funcref_end ||
                   element.externref_begin || element.externref_end || element.gc_ref_begin || element.gc_ref_end)
                { return threaded_main_active_status::unsupported_shape; }
                auto const count{declaration.vec_funcidx.size()};
                auto const* expected_begin{count==0uz?nullptr:declaration.vec_funcidx.data()};
                auto const* expected_end{count==0uz?nullptr:expected_begin+count};
                if(element.funcidx_begin!=expected_begin || element.funcidx_end!=expected_end)
                { return threaded_main_active_status::invalid_identity; }
                for(auto function:declaration.vec_funcidx)
                {
                    if((visited++&1023uz)==0uz && stopped()) { return threaded_main_active_status::stopped; }
                    auto const imports{member.imported_function_vec_storage.size()};
                    if(function<imports || function-imports>=member.local_defined_function_vec_storage.size())
                    { return threaded_main_active_status::unsupported_shape; }
                }
                if(!declaration.active)
                {
                    if(element.kind!=wasm_element_segment_kind::passive) { return threaded_main_active_status::invalid_identity; }
                    continue;
                }
                if(element.kind!=wasm_element_segment_kind::active || wasm_element_segment_is_dropped(element) ||
                   declaration.expr.opcodes.size()!=1uz ||
                   declaration.expr.opcodes.front_unchecked().opcode!=::uwvm2::parser::wasm::standard::wasm1::opcode::op_basic::i32_const ||
                   declaration.table_idx!=element.table_idx || element.table_idx>=member.local_defined_table_vec_storage.size())
                { return threaded_main_active_status::unsupported_shape; }
                auto const offset{static_cast<::std::uint_least32_t>(declaration.expr.opcodes.front_unchecked().storage.i32)};
                auto const& table{member.local_defined_table_vec_storage.index_unchecked(element.table_idx)};
                if(element.offset!=offset || offset>table.elems.size() || count>table.elems.size()-offset)
                { return threaded_main_active_status::bounds; }
            }
            }
            if(stopped()) { return threaded_main_active_status::stopped; }
            // Sticky BEFORE the first actual source-instance write. It is never
            // cleared here, even on cancellation/failure or before native launch.
            native.shared_effects_started_.store(true,::std::memory_order_release);
            for(::std::size_t m{};m<parent_world->module_count();++m)
            {
                // All registry identities and active extents were checked in
                // the first pass, before marking or publishing any effects.
                auto& member=registry.find(parent_world->file(m)->module_name)->second;
            ::std::size_t visited{};
            for(auto& segment:member.local_defined_element_vec_storage)
            {
                auto& element{segment.element};
                if(element.kind!=wasm_element_segment_kind::active) { continue; }
                auto& table{member.local_defined_table_vec_storage.index_unchecked(element.table_idx)};
                auto const& functions{segment.element_type_ptr->storage.segment.vec_funcidx};
                for(::std::size_t i{};i!=functions.size();++i)
                {
                    if((visited++&1023uz)==0uz && stopped()) { return threaded_main_active_status::stopped; }
                    auto const local{functions.index_unchecked(i)-member.imported_function_vec_storage.size()};
                    auto& slot{table.elems.index_unchecked(static_cast<::std::size_t>(element.offset)+i)};
                    slot.storage.defined_ptr=&member.local_defined_function_vec_storage.index_unchecked(local);
                    slot.type=local_defined_table_elem_storage_type_t::func_ref_defined;
                }
                drop_wasm_element_segment_payload(element);
            }
            }
            // Pure preload has no data/table/start effects to execute. Publish
            // actual instance phases only after all main writes/drop complete.
            // The same source registry remained exclusive under Root's retained
            // admission; these pointers were authenticated before the effect.
            for(auto& [name,module] : registry)
            {
                static_cast<void>(name);
                module.gc_collection_phase.publish_active_segments(true);
            }
            return stopped()?threaded_main_active_status::stopped:threaded_main_active_status::applied;
        }
    };
}
#endif
