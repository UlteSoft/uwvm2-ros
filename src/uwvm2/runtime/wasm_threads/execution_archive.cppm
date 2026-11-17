module;
#include <uwvm2/utils/container/standard.h>
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
#include <utility>
#include <vector>
#include <fast_io.h>
#include <fast_io_crypto.h>
export module uwvm2.runtime.wasm_threads:execution_archive;
import :execution_journal;
import uwvm2.runtime.diagnostic.snapshot_archive;
import uwvm2.runtime.diagnostic.snapshot_wasip1;
import uwvm2.runtime.diagnostic.snapshot_ros;
#define UWVM_MODULE
export { extern "C++" {
#include "execution_archive.h"
} }
