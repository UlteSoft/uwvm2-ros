module;
#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <mutex>
#include <new>
#include <string>
#include <string_view>
#include <vector>
export module uwvm2.utils.debug.host_calls;
import uwvm2.utils.thread;
#define UWVM_MODULE
#define UWVM_MODULE_EXPORT export
#include "host_calls.h"
