module;
#include <algorithm>
#include <array>
#include <atomic>
#include <charconv>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <mutex>
#include <string>
#include <string_view>
#include <vector>
#include <fast_io.h>
#include <fast_io_dsal/string_view.h>
export module uwvm2.uwvm.debugger.host_trace;
import fast_io;
import uwvm2.utils.debug.host_calls;
#define UWVM_MODULE
#define UWVM_MODULE_EXPORT export
#include "host_trace.h"
