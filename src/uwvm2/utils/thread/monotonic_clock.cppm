module;
#include <chrono>
#include <cstdint>
#include <limits>
#if defined(__linux__)
#include <sys/timerfd.h>
#endif
export module uwvm2.utils.thread:monotonic_clock;
export import :monotonic_sample;
#define UWVM_MODULE
#define UWVM_MODULE_EXPORT export
#include "monotonic_clock.h"
