module;
#include <atomic>
#include <memory>
#if defined(__linux__)
#include <cerrno>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#include <uwvm2/utils/macro/push_macros.h>
export module uwvm2.uwvm.cmdline.params:runtime_memtag_mode;
import fast_io;
import uwvm2.utils.container;
import uwvm2.utils.cmdline;
import uwvm2.uwvm.io;
import uwvm2.extension.memtag.execution_policy;
#define UWVM_MODULE
#define UWVM_MODULE_EXPORT export
#include "runtime_memtag_mode.h"
