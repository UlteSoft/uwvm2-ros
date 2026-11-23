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

            // Shared memory providers are authenticated immutable preload
            // modules. Multiple memories/aliases do not grant code or effects.
            auto const* primary{parent_world->memory(0u, 0u)};
            if(primary == nullptr || primary->memory_type_ptr == nullptr ||
               !details::wasm_memory_is_shared(*primary->memory_type_ptr) ||
               details::wasm_memory_is_address64(*primary->memory_type_ptr) != runtime_memory_is_address64(main, 0u))
            { return threaded_main_active_status::unsupported_shape; }
            bool primary_provider{};
            for(auto& [name,module]:registry)
            {
                static_cast<void>(name);if(&module==&main)continue;
                if(::std::atomic_ref<bool>{module.gc_collection_phase.active_segments_ready}.load(::std::memory_order_acquire))
                    return threaded_main_active_status::already_effectful;
                bool linked_code{};
                for(::std::size_t m{1u};m<parent_world->module_count();++m)
                    if(parent_world->module(m)==&module&&parent_world->original_module(m)==&module){linked_code=true;break;}
                if(linked_code||dormant_function_preload(owner,module))continue;
                threaded_provider_error error{};
                auto provider=ThreadedMemoryProvider::inspect(owner,&module,actual.max_shared_memory_bytes(),error);
                if(!provider||error!=threaded_provider_error::ok)return threaded_main_active_status::unsupported_shape;
                for(auto const& memory:module.local_defined_memory_vec_storage)primary_provider|=&memory==primary;
            }
            if(!primary_provider)return threaded_main_active_status::unsupported_shape;
            // Full Core validation and the same initializer used for fresh
            // children define table/data expression semantics. Authenticate all
            // owners before the first effect; any instantiation trap retains the
            // sticky reset requirement and no native Guest worker is published.
            for(::std::size_t m{};m<parent_world->module_count();++m)
            {
                auto it=registry.find(parent_world->file(m)->module_name);
                if(it==registry.end() || &it->second!=parent_world->module(m) ||
                    &it->second!=parent_world->original_module(m))return threaded_main_active_status::invalid_identity;
            }
            if(stopped())return threaded_main_active_status::stopped;
            native.shared_effects_started_.store(true,::std::memory_order_release);
            for(::std::size_t position{};position<parent_world->module_count();++position)
            {
                auto const m=parent_world->initialization_module_at(position);
                if(m>=parent_world->module_count())return threaded_main_active_status::invalid_identity;
                auto& member=registry.find(parent_world->file(m)->module_name)->second;
                details::initialization_context<details::initialization_purpose::ordinary> context{};
                if(details::apply_wasm1_active_element_and_data_segments_for_module_in_context(
                    context,parent_world->file(m)->module_name,member)!=details::active_segment_result::ok)
                    return threaded_main_active_status::bounds;
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
