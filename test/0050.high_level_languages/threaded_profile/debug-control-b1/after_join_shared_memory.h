#pragma once
// UNEXECUTED SOURCE. Read ONLY after actual complete physical join. Resolve
// the real Source-owned imported leaf; no raw external memory/World constructor.
#include <uwvm2/runtime/wasm_threads/wait.h>
namespace b1
{
    inline bool read_after_join(::uwvm2::uwvm::runtime::full::full_source_instance::owner const& retained,
        ::std::uint32_t offset,::std::uint64_t& value) noexcept
    {
        auto const* module{retained?retained->initialized_main_module():nullptr};
        if(!module||module->imported_memory_vec_storage.empty())return false;
        auto const* cursor{::std::addressof(module->imported_memory_vec_storage.index_unchecked(0u))};
        for(::std::size_t depth{};depth<=retained->registry().size();++depth)
        {
            bool imported{};
            for(auto const& [name,native]:retained->registry())
            {
                static_cast<void>(name);
                for(auto const& entry:native.imported_memory_vec_storage)if(::std::addressof(entry)==cursor)imported=true;
            }
            if(!imported)return false;
            using kind=::uwvm2::uwvm::runtime::storage::imported_memory_storage_t::imported_memory_link_kind;
            if(cursor->link_kind==kind::imported){cursor=cursor->target.imported_ptr;continue;}
            if(cursor->link_kind!=kind::defined||!cursor->target.defined_ptr)return false;
            auto const* leaf{cursor->target.defined_ptr};bool owned{};
            for(auto const& [name,native]:retained->registry())
            {
                static_cast<void>(name);
                for(auto const& memory:native.local_defined_memory_vec_storage)if(::std::addressof(memory)==leaf)owned=true;
            }
            if(!owned)return false;
            ::std::size_t length{};
            return ::uwvm2::runtime::wasm_threads::details::access(leaf->memory,offset,4u,length,
                [&](::std::byte* bytes) noexcept
                {value=::uwvm2::runtime::compiler::shared::wasm_threads::atomic_load_le<4u>(bytes);});
        }
        return false;
    }
}
