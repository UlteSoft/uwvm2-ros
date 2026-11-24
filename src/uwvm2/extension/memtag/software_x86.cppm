module;
#include <bit>
#include <cstddef>
#include <cstdint>
#if defined(__linux__) && (defined(__x86_64__) || defined(__i386__)) && defined(__GNUC__) && __STDC_HOSTED__
# include <cpuid.h>
# include <immintrin.h>
#endif
export module uwvm2.extension.memtag.software_x86;
#define UWVM_MODULE
#define UWVM_MODULE_EXPORT export
#include "software_x86.h"
