module;
#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <cstddef>
#include <cstdlib>
#include <mutex>
export module uwvm2.utils.thread:process_exit;
#ifndef UWVM_MODULE
#define UWVM_MODULE
#endif
#ifndef UWVM_MODULE_EXPORT
#define UWVM_MODULE_EXPORT export
#endif
#include "process_exit.h"
