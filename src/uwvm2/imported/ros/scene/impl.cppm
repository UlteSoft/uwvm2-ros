/** @file
 * @brief Sole named-module owner of scene policy, records and provider models.
 * @details Importing the model registers no native provider or grant. Shared
 * retirement and debugger hooks retain explicit global C++ attachment so
 * textual runtime glue and named-module consumers refer to the same hooks.
 */
module;
#include "attachment_tls_headers.h"
// Shared allocator/callback types have global attachment across header/module consumers.
#include <uwvm2/utils/container/standard.h>
#include <uwvm2/utils/container/shared.h>
#include <uwvm2/utils/container/unique.h>
#include <uwvm2/utils/container/function_ref.h>
#include <algorithm>
#include <array>
#include <atomic>
#include <bit>
#include <cerrno>
#include <charconv>
#include <chrono>
#include <cmath>
#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <exception>
#include <functional>
#include <initializer_list>
#include <limits>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <set>
#include <span>
#include <stdexcept>
#include <stop_token>
#include <string>
#include <string_view>
#include <thread>
#include <utility>
#include <variant>
#include <vector>
#if defined(__linux__)
#include <arpa/inet.h>
#include <dirent.h>
#include <fcntl.h>
#include <linux/fs.h>
#include <linux/memfd.h>
#include <linux/mempolicy.h>
#include <linux/audit.h>
#include <linux/filter.h>
#include <linux/sched.h>
#include <linux/seccomp.h>
#include <poll.h>
// Some libc wrappers re-enter <sys/poll.h> without a guard of their own.
#include <sys/poll.h>
#include <signal.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/prctl.h>
#include <sys/eventfd.h>
#include <sys/ioctl.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>
#endif

export module uwvm2.imported.ros.scene;
import fast_io_crypto;
#define UWVM_MODULE
// glibc's unguarded poll.h forwards to the guarded sys/poll.h already
// included in the global fragment. Repeated forwarding introduces no named
// declarations. Clang nevertheless diagnoses the include spelling itself.
#if defined(__clang__) && __has_warning("-Winclude-angled-in-module-purview")
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winclude-angled-in-module-purview"
#endif
export {
#include "erased_buffer.h"
#include "deployment.h"
#include "storage.h"
#include "lifecycle.h"
}
#if defined(__clang__) && __has_warning("-Winclude-angled-in-module-purview")
#pragma clang diagnostic pop
#endif
