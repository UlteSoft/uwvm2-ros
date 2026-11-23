// Regression for non-divisible native-object partitions and size_t overflow.
#include <cstddef>
#include <limits>
#include <cstdlib>
#include <initializer_list>
#include <uwvm2/runtime/lib/uwvm_runtime_llvm_partition_layout.h>
static void require(bool good) { if(!good) { ::std::abort(); } }
int main()
{
    using llvm_jit_partitioning::make_layout;
    static_assert(make_layout(10u, 8u).functions_per_task == 2u);
    static_assert(make_layout(10u, 8u).task_count == 5u);
    static_assert(make_layout(14u, 8u).task_count == 7u);
    static_assert(make_layout(0u, 8u).task_count == 0u);
    static_assert(make_layout(8u, 0u).task_count == 0u);
    for(::std::size_t count{1u}; count != 130u; ++count)
    {
        for(::std::size_t requested{1u}; requested != 34u; ++requested)
        {
            auto const p{make_layout(count, requested)};
            require(p.task_count >= 1u && p.task_count <= requested && p.task_count <= count);
            require((count - 1u) / p.functions_per_task == p.task_count - 1u);
            ::std::size_t visited{};
            for(::std::size_t part{}; part != p.task_count; ++part)
            {
                auto const begin{part * p.functions_per_task};
                require(begin < count && begin == visited);
                while(visited < count && visited / p.functions_per_task == part) { ++visited; }
                require(visited > begin);
            }
            require(visited == count);
        }
    }
    auto constexpr maximum{::std::numeric_limits<::std::size_t>::max()};
    for(auto count : {maximum, maximum - 1u, maximum / 2u + 1u})
    {
        for(auto requested : {::std::size_t{1u}, ::std::size_t{8u}, maximum - 1u, maximum})
        {
            auto const p{make_layout(count, requested)};
            require(p.functions_per_task != 0u && p.task_count != 0u);
            require(p.task_count <= requested && p.task_count <= count);
            require((count - 1u) / p.functions_per_task == p.task_count - 1u);
        }
    }
}
