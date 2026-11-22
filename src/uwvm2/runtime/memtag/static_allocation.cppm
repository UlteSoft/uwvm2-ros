module;
#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
export module uwvm2.runtime.memtag.static_allocation;
import uwvm2.extension.memtag.instruction;
import uwvm2.extension.memtag.allocation_neighborhood;
import uwvm2.extension.memtag.managed_allocations;
import uwvm2.extension.memtag.shadow;
import uwvm2.extension.memtag.storage;
import uwvm2.runtime.memtag.random;
#define UWVM_MODULE
#define UWVM_MODULE_EXPORT export
#include "static_allocation.h"
