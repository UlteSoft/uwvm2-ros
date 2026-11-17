#pragma once
#include <cstdint>
namespace uwvm2::runtime::lib
{
    // Ordinary builtin entry has no private Group/ChildLease. This function
    // diagnoses and returns refusal only; it cannot create native threads.
    extern "C++" ::std::int32_t runtime_threaded_spawn_unavailable_host_api() noexcept;
}
