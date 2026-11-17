/** @file
 * @brief Real concrete scene loader identity beyond the old 64-function limit.
 * @details Exercises actual tuple capture, source membership, exact tail OOM,
 * replacement and detached bounds. Runtime memory functions deliberately refuse
 * calls; this native metadata fixture never invokes a guest or opens a grant.
 * Full owned-source runtime and debugger admission require separate VM tests.
 */
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/uwvm/runtime/macro/push_macros.h>
#include <uwvm2/uwvm/imported/scene/host.h>
#include <cassert>
#include <cstdio>
namespace h=uwvm2::uwvm::imported::scene;
namespace f=uwvm2::uwvm::runtime::full;
namespace w=uwvm2::uwvm::wasm;
namespace a=uwvm2::utils::allocator::fast_io_strict;
static std::size_t reject_size{},rejections{};
extern "C" void* __real_malloc(std::size_t);
extern "C" void* __real_calloc(std::size_t,std::size_t);
extern "C" void* __real_realloc(void*,std::size_t);
static bool refuse(std::size_t size) noexcept
{if(reject_size&&size==reject_size){++rejections;return true;}return false;}
extern "C" void* __wrap_malloc(std::size_t size){return refuse(size)?nullptr:__real_malloc(size);}
extern "C" void* __wrap_calloc(std::size_t count,std::size_t size)
{return count&&size>SIZE_MAX/count?nullptr:refuse(count*size)?nullptr:__real_calloc(count,size);}
extern "C" void* __wrap_realloc(void* p,std::size_t size){return refuse(size)?nullptr:__real_realloc(p,size);}
namespace uwvm2::runtime::lib
{
    bool ros_caller_identity_host_api(ros_caller_identity&,bool) noexcept{return false;}
    bool preload_memory_range_host_api(std::size_t,std::uint_least64_t,std::size_t) noexcept{return false;}
    bool preload_memory_normalize_range_host_api(std::size_t,std::uint_least64_t,std::size_t,std::uint64_t&) noexcept{return false;}
    bool preload_memory_read_host_api(std::size_t,std::uint_least64_t,void*,std::size_t) noexcept{return false;}
    bool preload_memory_write_host_api(std::size_t,std::uint_least64_t,void const*,std::size_t) noexcept{return false;}
}
template<uwvm2::parser::wasm::concepts::wasm_feature... Fs>
static auto const& actual_functions(w::type::feature_list<Fs...>)
{return w::type::details::all_function_information_cache<h::local_imported_module_t::local_function_tuple,Fs...>::function_information;}
int main()
{
    auto const& known=actual_functions(w::type::binfmt_ver1_feature_list_t{});
    constexpr auto scene=static_cast<std::size_t>(f::builtin_ros_kind::scene);
    static_assert(!std::is_default_constructible_v<f::builtin_ros_loader_identity>);
    static_assert(!std::is_copy_constructible_v<f::builtin_ros_loader_identity>);
    assert(known.size()>64);
    auto& providers=w::storage::preload_local_imported;auto& modules=w::storage::all_module;
    assert(providers.empty()&&modules.empty());
    reject_size=a::array_prefix<f::builtin_ros_function_data>+(known.size()-64)*sizeof(f::builtin_ros_function_data);
    assert(!h::sealed_loader_factory::append());reject_size=0;
    assert(rejections==1&&providers.empty()&&!f::builtin_ros_loader_identity::snapshot_current()[scene]);
    assert(h::sealed_loader_factory::append()&&providers.size()==1);
    auto owner=f::builtin_ros_loader_identity::snapshot_current()[scene];assert(owner&&f::builtin_ros_loader_identity::canonical(owner));
    auto* member=std::addressof(providers.index_unchecked(0));
    assert(modules.try_emplace(h::local_imported_module_t::module_name,w::type::all_module_t{
        .module_storage_ptr={.li=member},.type=w::type::module_type_t::local_import}).second);
    assert(owner->current_member()==member&&f::builtin_ros_loader_identity::still_selected(owner));
    f::builtin_ros_function_data first{};assert(owner->copy_function(member,0,first));
    for(std::size_t i{};i<known.size();++i)
    {
        f::builtin_ros_function_data value{};assert(owner->copy_function(member,i,value));
        assert(value.kind==f::builtin_ros_kind::scene&&value.function_index==i&&value.interface_sha256==first.interface_sha256);
        auto count=[](auto range){if(!range.begin){assert(!range.end);return std::size_t{};}return static_cast<std::size_t>(range.end-range.begin);};
        assert(value.parameter_count==count(known[i].function_type.parameter));
        assert(value.result_count==count(known[i].function_type.result));
    }
    f::builtin_ros_function_data sentinel{};sentinel.function_index=UINT64_MAX;
    assert(!owner->copy_function(member,known.size(),sentinel)&&sentinel.function_index==UINT64_MAX);
    assert(!owner->copy_function(member,SIZE_MAX,sentinel)&&sentinel.function_index==UINT64_MAX);
    assert(!owner->copy_function(nullptr,64,sentinel)&&sentinel.function_index==UINT64_MAX);
    // Replacing the real selected provider fences the prior canonical identity.
    modules.clear();assert(h::sealed_loader_factory::append()&&providers.size()==2);
    auto replacement=f::builtin_ros_loader_identity::snapshot_current()[scene];
    assert(replacement&&replacement.get()!=owner.get()&&!f::builtin_ros_loader_identity::still_selected(owner));
    assert(!owner->copy_function(member,64,sentinel)&&sentinel.function_index==UINT64_MAX);
    providers.clear();assert(!replacement->current_member());
    std::printf("PASS %zu actual scene tuple entries, recoverable tail allocation, canonical replacement and bounds\n",known.size());
}
