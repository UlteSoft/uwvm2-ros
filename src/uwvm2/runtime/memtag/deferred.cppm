module;
#include <array>
#include <atomic>
#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <memory>
#include <mutex>
#include <thread>
#include <utility>
#include <vector>
#include <uwvm2/utils/macro/push_macros.h>
export module uwvm2.runtime.memtag.deferred;
import uwvm2.extension.memtag.execution_policy;
import uwvm2.extension.memtag.shadow;
import uwvm2.extension.memtag.hardware_mte;
import uwvm2.runtime.diagnostic.crash_policy;
import uwvm2.runtime.memtag.report;
#define UWVM_MODULE
#define UWVM_MODULE_EXPORT export
#include "deferred.h"
#include <uwvm2/utils/macro/pop_macros.h>
