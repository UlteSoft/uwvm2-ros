module;
#include <atomic>
#include <cstdint>
#include <cstddef>
#include <cstdlib>
#if defined(__linux__)
#include <unistd.h>
#include <signal.h>
#include <cerrno>
#include <poll.h>
#include <sys/socket.h>
#endif
export module uwvm2.runtime.diagnostic.crash_policy;
import fast_io;
#define UWVM_MODULE
#define UWVM_MODULE_EXPORT export
#include "crash_policy.h"
