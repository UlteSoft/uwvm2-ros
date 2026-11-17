module;
#include <algorithm>
#include <array>
#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>
#include <memory>
#include <mutex>
#include <span>
#include <utility>
#include <vector>
#include <uwvm2/utils/thread/stop_token.h>
#if defined(__linux__)
#include <unistd.h>
#endif
export module uwvm2.runtime.wasm_threads:execution_journal;
import uwvm2.utils.thread;
#define UWVM_MODULE
export { extern "C++" {
#include "execution_journal.h"
} }
