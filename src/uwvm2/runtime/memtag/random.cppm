module;
#include <array>
#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <uwvm2/utils/macro/push_macros.h>
#if defined(__linux__)
# include <pthread.h>
# include <uwvm2/utils/control/posix_abi.h>
#endif
export module uwvm2.runtime.memtag.random;
#define UWVM_MODULE
#define UWVM_MODULE_EXPORT export
#include "random.h"
