module;
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <limits>
#if defined(__linux__) && defined(__aarch64__)
# include <cerrno>
# include <csignal>
# include <pthread.h>
# include <sys/auxv.h>
# include <sys/mman.h>
# include <sys/prctl.h>
#endif
export module uwvm2.extension.memtag.hardware_mte;
import uwvm2.extension.memtag.execution_policy;
#define UWVM_MODULE
#define UWVM_MODULE_EXPORT export
#include "hardware_mte.h"
