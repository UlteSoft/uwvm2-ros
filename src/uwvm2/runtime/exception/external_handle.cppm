module;
#include <uwvm2/utils/container/shared.h>
#include <uwvm2/utils/allocator/fast_io_strict/object.h>
#if defined(UWVM_EXPERIMENTAL_EXTERNAL_EXCEPTION_HANDLES) && UWVM_EXPERIMENTAL_EXTERNAL_EXCEPTION_HANDLES == 1
#include <exception>
#include <memory>
#include <mutex>
#include <utility>
#endif
export module uwvm2.runtime.exception.external_handle;
#if defined(UWVM_EXPERIMENTAL_EXTERNAL_EXCEPTION_HANDLES) && UWVM_EXPERIMENTAL_EXTERNAL_EXCEPTION_HANDLES == 1
import uwvm2.runtime.exception.value;
import uwvm2.runtime.exception.native_roots;
import uwvm2.utils.thread;
#define UWVM_MODULE
#define UWVM_MODULE_EXPORT export
#include "external_handle.h"
#endif
