// Private cold ownership lookup, defined after the actual world type.
#pragma once
#if defined(UWVM_RUNTIME_LLVM_JIT) && defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD) && defined(UWVM_CPP_EXCEPTIONS) && !defined(UWVM_DISABLE_LOCAL_IMPORTED_WASIP1) && defined(UWVM_IMPORT_WASI_WASIP1)
extern "C++" bool runtime_wasip1_debug_environment::resolve_text_owners(
    ::std::size_t id, wasip1_env_type& environment, strings*& arguments, strings*& variables) noexcept
{
    arguments=nullptr;variables=nullptr;
    namespace storage=::uwvm2::uwvm::imported::wasi::wasip1::storage;
    if(id>=g_runtime.modules.size() || !is_wasip1_import_visible_for_runtime_module_id_slow(id) ||
       ::std::addressof(resolve_wasip1_env_for_runtime_module_id(id))!=::std::addressof(environment)) { return false; }
#if !defined(UWVM_TERMINATE_IMME_WHEN_PARSE)
    if(auto const& world=g_runtime.checkpoint_world;world)
    {
        if(!world->published_ || world->publication_epoch_!=current_runtime_generation() ||
           !::uwvm2::uwvm::runtime::full::selected_full_source_matches_owner(world->prepared_source_) ||
           id>=world->wasip1_modules_.size()) { return false; }
        auto const index=world->wasip1_modules_[id].environment;
        if(index>=world->wasip1_environments_.size()) { return false; }
        auto& owned=*world->wasip1_environments_[index];
        if(::std::addressof(owned.environment)!=::std::addressof(environment)) { return false; }
        arguments=::std::addressof(owned.arguments);variables=::std::addressof(owned.variables);return true;
    }
#endif
    if(::std::addressof(environment)==::std::addressof(storage::default_wasip1_env))
    {
        arguments=::std::addressof(storage::wasip1_argument_storage);
        variables=::std::addressof(storage::wasip1_environment_storage);return true;
    }
    auto const group=find_wasip1_override_for_runtime_module_id_slow(id);
    if(group==nullptr || ::std::addressof(group->env)!=::std::addressof(environment)) { return false; }
    arguments=::std::addressof(group->argument_storage);variables=::std::addressof(group->environment_storage);return true;
}
#endif
