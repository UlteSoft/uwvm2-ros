# Real Native compile+LINK capability probe source. Not executed by this author.
function(uwvm_signal_cli_native_link_probe)
  if(NOT CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(FATAL_ERROR "The explicit OS producer is a Linux Native CLI profile")
  endif()
  set(_flags "${CMAKE_CXX_FLAGS};${CMAKE_EXE_LINKER_FLAGS};${CMAKE_REQUIRED_FLAGS};${CMAKE_REQUIRED_LINK_OPTIONS};${CMAKE_REQUIRED_LIBRARIES}")
  foreach(_config DEBUG RELEASE RELWITHDEBINFO MINSIZEREL)
    string(APPEND _flags ";${CMAKE_CXX_FLAGS_${_config}};${CMAKE_EXE_LINKER_FLAGS_${_config}}")
  endforeach()
  if(_flags MATCHES "--unresolved-symbols|--allow-undefined|--import-undefined")
    message(FATAL_ERROR "Permissive unresolved-symbol flags cannot qualify this LINK probe")
  endif()
  include(CheckCXXSourceCompiles)
  find_package(Threads REQUIRED)
  set(CMAKE_TRY_COMPILE_TARGET_TYPE EXECUTABLE)
  list(APPEND CMAKE_REQUIRED_LIBRARIES Threads::Threads)
  unset(UWVM_SIGNAL_CLI_NATIVE_LINK CACHE)
  unset(UWVM_SIGNAL_CLI_NATIVE_LINK)
  check_cxx_source_compiles("#include <atomic>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
static_assert(std::atomic<unsigned>::is_always_lock_free);
int main(){
  auto volatile a=&sigaction; auto volatile b=&sigemptyset;
  auto volatile c=&sigaddset; auto volatile d=&pthread_sigmask;
  auto volatile e=&getpid; auto volatile f=&geteuid; auto volatile g=&raise;
  auto volatile h=&fcntl; auto volatile i=&_exit; auto volatile j=&fstat; auto volatile k=&sigismember;
  return !(a&&b&&c&&d&&e&&f&&g&&h&&i&&j&&k);
}" UWVM_SIGNAL_CLI_NATIVE_LINK)
  if(NOT UWVM_SIGNAL_CLI_NATIVE_LINK)
    message(FATAL_ERROR "Actual Native signal/lock-free/flag LINK capability is missing")
  endif()
  # Caller scope is preserved. Transitive flags, actual target MD/link and Native
  # Runtime behavior remain separate gates; this source does not award them.
endfunction()
