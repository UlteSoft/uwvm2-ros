/** @file
 * @brief Metadata-only projection of already registered modules.
 * @details A trusted grant selects the exact registry key. No symbol lookup,
 * loader, native callback, export invocation, memory read, debugger control or
 * policy mutation is reachable here. Native dl imports are unsupported: the
 * registry records its Wasm-facing exports, not the platform dynamic linker.
 */
#pragma once
#ifndef UWVM_MODULE
#include <uwvm2/uwvm/wasm/storage/all_module.h>
#include <uwvm2/imported/ros/scene/permission_report.h>
#endif
namespace uwvm2::uwvm::imported::scene::module_observer
{
    namespace report=::uwvm2::imported::ros::scene::permission_report;
    using error=::uwvm2::imported::ros::scene::error;
    template<typename Text>inline std::string_view text(Text const& value)noexcept
    {return {reinterpret_cast<char const*>(value.data()),value.size()};}
    /** @brief Visit validated declarations only; never traverse native linkage. */
    template<typename Module,typename Visit>inline error imports(Module const& module,Visit&& visit)
    {
        using kind=::uwvm2::uwvm::wasm::type::module_type_t;
        if(module.type!=kind::exec_wasm&&module.type!=kind::preloaded_wasm)return error::unsupported;
        auto const* file=module.module_storage_ptr.wf;if(!file||file->binfmt_ver!=1u)return error::unsupported;
        auto const& contents=file->wasm_module_storage.wasm_binfmt_ver1_storage;
        auto get_imports=[&]<::uwvm2::parser::wasm::concepts::wasm_feature... Fs>(::uwvm2::utils::container::tuple<Fs...>)->decltype(auto)
        {return ::uwvm2::parser::wasm::concepts::operation::get_first_type_in_tuple<
            ::uwvm2::parser::wasm::standard::wasm1::features::import_section_storage_t<Fs...>>(contents.sections);};
        for(auto const& item:get_imports(::uwvm2::uwvm::wasm::feature::all_features).imports)
            if(!visit(item))return error::too_large;
        return error::success;
    }
    /** @brief Enumerate direct Wasm declarations importing an authorized target.
     * @details This independently authorized operation reveals peer names even
     * when the target is the caller itself. Self permission.report never calls
     * it. A row contains importer name, declared symbol, zero and the literal
     * declared-import annotation. Reexports are not recursively expanded; dl
     * platform dependencies are not inspected. The active VM execution lease
     * keeps the registered declaration world alive during the synchronous call.
     * Count every visited module and import, including nonmatches, so an empty
     * result cannot cause an unbounded scan. Failure publishes no partial page.
     */
    inline error importers(std::string_view target,report::page& output)
    {
        namespace storage=::uwvm2::uwvm::wasm::storage;
        using kind=::uwvm2::uwvm::wasm::type::module_type_t;
        std::size_t work{};
        for(auto const& entry:storage::all_module)
        {
            if(work==report::maximum_scan)return error::too_large;
            ++work;
            if(entry.second.type!=kind::exec_wasm&&entry.second.type!=kind::preloaded_wasm)continue;
            auto result=imports(entry.second,[&](auto const& item)
            {
                if(work==report::maximum_scan)return false;
                ++work;
                return text(item.module_name)!=target||output.add(text(entry.first),text(item.extern_name),0,"declared-import");
            });
            if(result!=error::success)return result;
        }
        return output.finish(true,true)?error::success:error::too_large;
    }
    /** @brief Identify an already loaded module; never disclose peer metadata. */
    inline error identity(std::string_view target,report::page& output)
    {
        namespace storage=::uwvm2::uwvm::wasm::storage;
        using kind=::uwvm2::uwvm::wasm::type::module_type_t;
        ::uwvm2::utils::container::u8string_view name{reinterpret_cast<char8_t const*>(target.data()),target.size()};
        auto it=storage::all_module.find(name);if(it==storage::all_module.end())return error::bad_handle;
        if(it->second.type==kind::local_import)return error::unsupported;
        auto label=it->second.type==kind::exec_wasm?"main-wasm":it->second.type==kind::preloaded_wasm?"preload-wasm":"preload-dl";
        return output.add("module","name",0,target)&&output.add("module","kind",0,label)&&
            output.add("module","query-only",1)?error::success:error::too_large;
    }
    inline error read(std::string_view target,report::module_operation operation,report::page& output)
    {
        namespace storage=::uwvm2::uwvm::wasm::storage;
        using kind=::uwvm2::uwvm::wasm::type::module_type_t;
        ::uwvm2::utils::container::u8string_view name{reinterpret_cast<char8_t const*>(target.data()),target.size()};
        auto it=storage::all_module.find(name);if(it==storage::all_module.end())return error::bad_handle;
        auto const& module=it->second;
        if(operation==report::module_operation::importers)return importers(target,output);
        if(module.type==kind::local_import)return error::unsupported;
        if(operation==report::module_operation::describe)
        {
            auto status=identity(target,output);if(status!=error::success)return status;
            return output.finish(true,true)?error::success:error::too_large;
        }
        if(operation==report::module_operation::exports)
        {
            auto exports=storage::all_module_export.find(name);
            if(exports!=storage::all_module_export.end())for(auto const& entry:exports->second)
                if(!output.add("export",text(entry.first),0))return error::too_large;
            return output.finish(true,true)?error::success:error::too_large;
        }
        if(operation!=report::module_operation::imports)return error::unsupported;
        auto result=imports(module,[&](auto const& item){return output.add(text(item.module_name),text(item.extern_name),0,"import");});
        if(result!=error::success)return result;
        return output.finish(true,true)?error::success:error::too_large;
    }
}
