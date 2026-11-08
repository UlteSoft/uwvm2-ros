module;
# include <atomic>
# include <cerrno>
# include <cstddef>
# include <cstdint>
# include <span>
# include <string>
# include <string_view>
# include <utility>
# if defined(__linux__)
#  include <fcntl.h>
#  include <sys/stat.h>
#  include <unistd.h>
# endif

export module uwvm2.runtime.diagnostic.crash_file;
#define UWVM_MODULE
#define UWVM_MODULE_EXPORT export
#include "crash_file.h"
