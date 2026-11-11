module;
#include <mutex>
#include <cstdint>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#if defined(__linux__)
#include <cerrno>
#include <unistd.h>
#endif
#include <uwvm2/utils/macro/push_macros.h>
export module uwvm2.runtime.memtag.report;
import uwvm2.extension.memtag.execution_policy;
import uwvm2.extension.memtag.shadow;
import uwvm2.runtime.diagnostic.crash_policy;
#define UWVM_MODULE
#define UWVM_MODULE_EXPORT export
#include "report.h"
#include <uwvm2/utils/macro/pop_macros.h>
