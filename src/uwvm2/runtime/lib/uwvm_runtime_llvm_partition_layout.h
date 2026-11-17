#pragma once

// Included inside runtime::lib; no process state or runtime execution work.
namespace llvm_jit_partitioning
{
    struct layout
    {
        ::std::size_t functions_per_task{};
        ::std::size_t task_count{};
    };

    [[nodiscard]] inline constexpr layout make_layout(::std::size_t function_count,
                                                       ::std::size_t requested_task_count) noexcept
    {
        if(function_count == 0uz || requested_task_count == 0uz) { return {}; }
        if(requested_task_count > function_count) { requested_task_count = function_count; }
        auto const per_task{function_count / requested_task_count +
            static_cast<::std::size_t>(function_count % requested_task_count != 0uz)};
        // ceil(n / ceil(n / requested)) may be SMALLER than requested. Only
        // these nonempty partitions exist; otherwise a tail begin can exceed n.
        // Quotient/remainder arithmetic also avoids n + divisor - 1 overflow.
        return {per_task, function_count / per_task +
            static_cast<::std::size_t>(function_count % per_task != 0uz)};
    }
}
