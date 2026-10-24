// Source contracts only. Called by an actual owned-Source Runtime harness.
// No generated signature, fake provider, ready flag or Native success model.
#include <array>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <uwvm2/uwvm/runtime/storage/full.h>
#include <uwvm2/uwvm/imported/ros/host.h>
namespace uwvm_ros_debug_contract
{
    namespace full=::uwvm2::uwvm::runtime::full;
    namespace ros=::uwvm2::uwvm::imported::ros;
    using source=full::full_source_instance;
    using module=::uwvm2::uwvm::runtime::storage::wasm_module_storage_t;
    struct report {unsigned attempted{},failed{};};
    template<class T>concept can_issue=requires(T const& value){ros::append_sealed_ros_builtin(value);};
#if defined(UWVM_IMPORT_ROS2)
    struct namespace_imitation
    {
        inline static constexpr auto module_name=ros::ros2_local_imported_module_t::module_name;
        using local_function_tuple=ros::ros2_local_imported_module_t::local_function_tuple;
    };
    static_assert(can_issue<ros::ros2_local_imported_module_t>);
    static_assert(!can_issue<namespace_imitation>); // Even the exact copied tuple/name cannot mint provenance.
#endif
    static_assert(!::std::is_default_constructible_v<full::builtin_ros_loader_identity>);
    static_assert(!::std::is_copy_constructible_v<full::builtin_ros_loader_identity>);
    static_assert(::std::is_trivially_copyable_v<full::builtin_ros_function_data>);
    static bool same(full::builtin_ros_function_data const& a,full::builtin_ros_function_data const& b) noexcept
    {return a.kind==b.kind&&a.function_index==b.function_index&&a.interface_sha256==b.interface_sha256&&
        a.parameter_count==b.parameter_count&&a.result_count==b.result_count&&a.parameters==b.parameters&&a.results==b.results;}
    // The external real harness must retain its genuine closed-host/all-stop/
    // publication/generation owner across this call. A shared Source pointer
    // alone is NOT that admission and this test entry is not a Runtime API.
    report actual_ros_fixture(source::owner const& original,::std::size_t id,::std::uint_least64_t epoch,
        module const* actual,::std::size_t actual_ros_import) noexcept
    {
        report r{};auto check=[&](bool condition){++r.attempted;if(!condition)++r.failed;};
        check(source::has_canonical_owner(original));if(!original||!actual)return r;
        check(original->actual_debug_memory_provider_census(id,epoch,actual));
        // The unchanged strict old queries must NOT be widened by ROS support.
        check(!original->actual_no_unadapted_native_memory_provider(id,epoch,actual));
        check(original->actual_cache_native_provider_census()==source::cache_native_provider_census::unsafe);
        full::builtin_ros_function_data positive{};
        bool admitted=original->actual_builtin_ros_function(id,epoch,actual,actual_ros_import,positive);check(admitted);
        if(admitted)
        {
            check(positive.parameter_count<=positive.parameters.size()&&positive.result_count<=positive.results.size());
            if(positive.parameter_count>positive.parameters.size()||positive.result_count>positive.results.size())return r;
            for(unsigned i{};i!=positive.parameter_count;++i)check(positive.parameters[i]==0x7fu||positive.parameters[i]==0x7eu);
            for(unsigned i{};i!=positive.result_count;++i)check(positive.results[i]==0x7fu||positive.results[i]==0x7eu);
        }
        full::builtin_ros_function_data sentinel{};sentinel.function_index=UINT64_MAX;sentinel.parameter_count=987u;
        auto out=sentinel;
        check(!original->actual_builtin_ros_function(id,0u,actual,actual_ros_import,out)&&same(out,sentinel));
        check(!original->actual_builtin_ros_function(id,epoch,nullptr,actual_ros_import,out)&&same(out,sentinel));
        check(!original->actual_builtin_ros_function(id,epoch,actual,(::std::numeric_limits<::std::size_t>::max)(),out)&&same(out,sentinel));
        check(!original->actual_debug_memory_provider_census(id,0u,actual));
        check(!original->actual_debug_memory_provider_census(id,epoch,nullptr));
        // Every actually imported ROS leaf must map to a real finite tuple.
        auto const& imports=actual->imported_function_vec_storage;
        unsigned ros_count{};
        for(::std::size_t i{};i!=imports.size();++i)
        {
            full::builtin_ros_function_data data{};
            if(original->actual_builtin_ros_function(id,epoch,actual,i,data)){++ros_count;check(data.function_index<64u);}
        }
        check(ros_count!=0u);return r;
    }
    report actual_refused_fixture(source::owner const& original,::std::size_t id,
        ::std::uint_least64_t epoch,module const* actual) noexcept
    {
        // Separate genuine fixtures: unknown concrete provider, preloaded DL,
        // weak symbol, changed final vector, stale generation or actual shared
        // threads provider. Setup/retirement belongs to the actual Runtime.
        report r{1u,0u};if(!original||original->actual_debug_memory_provider_census(id,epoch,actual))r.failed=1u;return r;
    }
}
