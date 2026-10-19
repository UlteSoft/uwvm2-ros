// SPDX-License-Identifier: Apache-2.0
// Included at TU scope only, after the genuine threaded Host API feature gate.
#if UWVM2_HAS_THREADED_PROFILE_HOST_API
#include <array>
#include <cerrno>
#include <climits>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/syscall.h>
#endif
