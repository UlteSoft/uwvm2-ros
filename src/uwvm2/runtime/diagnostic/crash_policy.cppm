module;
#include <atomic>
#include <cstdint>
#include <cstddef>
#if defined(__linux__)
#include <unistd.h>
#include <signal.h>
#include <cerrno>
#endif
export module uwvm2.runtime.diagnostic.crash_policy;
#define UWVM_MODULE
#define UWVM_MODULE_EXPORT export
#include "crash_policy.h"
