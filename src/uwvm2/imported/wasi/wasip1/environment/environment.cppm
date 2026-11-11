/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/

/**
 * @author      MacroModel
 * @version     2.0.0
 * @copyright   APL-2.0 License
 */

/****************************************
 *  _   _ __        ____     __ __  __  *
 * | | | |\ \      / /\ \   / /|  \/  | *
 * | | | | \ \ /\ / /  \ \ / / | |\/| | *
 * | |_| |  \ V  V /    \ V /  | |  | | *
 *  \___/    \_/\_/      \_/   |_|  |_| *
 *                                      *
 ****************************************/

module;
#include <memory>

// std
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <climits>
#include <version>
#include <limits>
#include <concepts>
#include <bit>
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
# include <array>
# include <chrono>
# include <mutex>
# include <condition_variable>
# include <atomic>
# if defined(__linux__)
#  include <cerrno>
#  include <sys/eventfd.h>
#  include <sys/timerfd.h>
#  include <fcntl.h>
#  include <poll.h>
#  include <signal.h>
#  include <sys/socket.h>
#  include <sys/syscall.h>
#  include <unistd.h>
#  include <pthread.h>
# endif
#endif
// macro
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/imported/wasi/wasip1/feature/feature_push_macro.h>

export module uwvm2.imported.wasi.wasip1.environment:environment;

import fast_io;
import uwvm2.utils.debug;
import uwvm2.utils.mutex;
import uwvm2.utils.thread;
import uwvm2.utils.container;
import uwvm2.parser.wasm.standard.wasm1.type;
import uwvm2.object.memory;
import uwvm2.imported.wasi.wasip1.abi;
import uwvm2.imported.wasi.wasip1.fd_manager;
import uwvm2.imported.wasi.wasip1.memory;

#ifndef UWVM_MODULE
# define UWVM_MODULE
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT export
#endif

#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
# include "resource_limits.h"
# include "bounded_stdio.h"
#endif
#include "environment.h"
#include "policy_encoding.h"

