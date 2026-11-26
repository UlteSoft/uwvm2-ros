// Intel MTT admission requires a working CPU/OS checked mapping, allocation
// tags and a fault-delivery backend. The guest field is defined by intel_mtt_layout.
#pragma once
#ifndef UWVM_MODULE
# include "instruction.h"
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::extension::memtag::hardware_mtt
{
    [[nodiscard]] inline constexpr bool supported() noexcept { return false; }
}
