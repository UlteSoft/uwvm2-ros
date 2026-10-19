// SPDX-License-Identifier: Apache-2.0
// Sole named-module owner; import issues no authority or native effects.
module;
#include <algorithm>
#include <array>
#include <atomic>
#include <bit>
#include <cerrno>
#include <charconv>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <deque>
#include <exception>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <limits>
#include <locale>
#include <map>
#include <memory>
#include <mutex>
#include <new>
#include <optional>
#include <set>
#include <span>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>
#include <vector>
#if defined(__linux__)
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <pthread.h>
#include <signal.h>
#include <sys/random.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <sys/un.h>
#include <unistd.h>
#endif
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/uwvm/runtime/macro/push_macros.h>

export module uwvm2.imported.ros;
import fast_io;

#ifndef UWVM_MODULE
#define UWVM_MODULE
#endif
#undef UWVM_MODULE_EXPORT
#define UWVM_MODULE_EXPORT export
export {
#include "capability/manager.h"
#include "policy/policy.h"
#include "ipc/unix.h"
#include "ipc/event_queue.h"
#include "common/manifest.h"
#include "common/audit_io.h"
#include "common/management.h"
#include "common/service_availability_history.h"
#include "common/rmw_endpoint_history.h"
#include "common/rmw_service_bindings.h"
#include "ipc/rmw_service.h"
#include "ipc/rmw_publish.h"
#include "ipc/rmw_subscription.h"
#include "ipc/cancellation_scope.h"

}
