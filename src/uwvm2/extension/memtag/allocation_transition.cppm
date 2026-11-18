module;
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <span>
export module uwvm2.extension.memtag.allocation_transition;
import uwvm2.extension.memtag.allocation_neighborhood;
import uwvm2.extension.memtag.managed_allocations;
import uwvm2.extension.memtag.operations;
import uwvm2.extension.memtag.shadow;
import uwvm2.extension.memtag.instruction;
#define UWVM_MODULE
#define UWVM_MODULE_EXPORT export
#include "allocation_transition.h"
