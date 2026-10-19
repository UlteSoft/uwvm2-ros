// Copyright 2018 Open Source Robotics Foundation, Inc.
// Target-specific adaptation: Copyright 2026 UWVM contributors.
// Original ros2/rclcpp commit 53fa7aeea86e8ed484f9465fa14059faf13a71f1,
// rclcpp/src/rclcpp/signal_handler.cpp. Public API and default NodeOptions are retained.
// This private WASI source port is not a generic POSIX signal implementation.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "signal_handler.hpp"
#include <uwvm_rmw/wasi_signal.hpp>
#if !defined(__wasi__) || !defined(UWVM_RCLCPP_WASI_SIGNAL_PORT)
#error "This fixed original-rclcpp source port requires explicit real WASI admission"
#endif
#if defined(RCLCPP_HAS_SIGACTION)
#error "The target port must not accidentally compile a sigaction ABI"
#endif

#include <atomic>
#include <csignal>
#include <exception>
#include <mutex>
#include <string>
#include <thread>

// includes for semaphore notification code
#if defined(_WIN32)
#include <windows.h>
#elif defined(__APPLE__)
#include <dispatch/dispatch.h>
#else  // posix
#include <semaphore.h>
#endif

#include "rclcpp/logging.hpp"
#include "rclcpp/utilities.hpp"
#include "rcutils/strerror.h"
#include "rmw/impl/cpp/demangle.hpp"

using rclcpp::SignalHandler;
using rclcpp::SignalHandlerOptions;

namespace {
// Established only by the real original deferred thread entry. Callback/user
// code receives no setter or synthetic "worker" flag.
thread_local SignalHandler const * actual_deferred_signal_worker{};
struct scoped_deferred_signal_worker final {
  SignalHandler const * previous{};
  explicit scoped_deferred_signal_worker(SignalHandler const * value) noexcept
  : previous{actual_deferred_signal_worker} {actual_deferred_signal_worker = value;}
  ~scoped_deferred_signal_worker() {actual_deferred_signal_worker = previous;}
  scoped_deferred_signal_worker(scoped_deferred_signal_worker const &) = delete;
};
}

SignalHandler::signal_handler_type
SignalHandler::set_signal_handler(
  int signal_value,
  const SignalHandler::signal_handler_type & signal_handler)
{
  SignalHandler::signal_handler_type previous{};
  auto code = uwvm_rmw::wasi_signal::exchange_handler(signal_value, signal_handler, &previous);
  if (code != 0) {
    throw std::runtime_error("Explicit WASI signal registration failed: " + std::to_string(code));
  }
  return previous;
}

// Unfortunately macros (or duplicated code) are needed here,
// as the signal handler must be a function pointer.
#if defined(RCLCPP_HAS_SIGACTION)
void
SignalHandler::signal_handler(
  int signum, siginfo_t * siginfo, void * context)
{
  auto & instance = SignalHandler::get_global_signal_handler();

  auto old_signal_handler = instance.get_old_signal_handler(signum);
  if (old_signal_handler.sa_flags & SA_SIGINFO) {
    if (old_signal_handler.sa_sigaction != NULL) {
      old_signal_handler.sa_sigaction(signum, siginfo, context);
    }
  } else {
    if (
      old_signal_handler.sa_handler != NULL &&  /* Is set */
      old_signal_handler.sa_handler != SIG_DFL &&  /* Is not default*/
      old_signal_handler.sa_handler != SIG_IGN)  /* Is not ignored */
    {
      old_signal_handler.sa_handler(signum);
    }
  }
  instance.signal_handler_common();
}
#else
void
SignalHandler::signal_handler(int signum)
{
  auto & instance = SignalHandler::get_global_signal_handler();
  auto old_signal_handler = instance.get_old_signal_handler(signum);
  if (
    SIG_ERR != old_signal_handler && SIG_IGN != old_signal_handler &&
    SIG_DFL != old_signal_handler)
  {
    old_signal_handler(signum);
  }
  instance.signal_handler_common();
}
#endif

rclcpp::Logger &
SignalHandler::get_logger()
{
  return SignalHandler::get_global_signal_handler().logger_;
}

SignalHandler &
SignalHandler::get_global_signal_handler()
{
  static SignalHandler & signal_handler = *new SignalHandler();
  return signal_handler;
}

bool
SignalHandler::install(SignalHandlerOptions signal_handler_options)
{
  // Refuse BEFORE install_mutex: a different thread may own that mutex while
  // physically joining this very receiver/deferred worker.
  if (uwvm_rmw::wasi_signal::in_receiver_thread() || actual_deferred_signal_worker == this) {
    throw std::runtime_error("WASI signal worker cannot reenter install/uninstall: 17");
  }
  std::lock_guard<std::mutex> lock(install_mutex_);
  if (installed_.load()) {
    return false;
  }
  if (signal_handler_options == SignalHandlerOptions::None) {
    installed_.store(true);
    return true;  // An explicit application choice; default All is unchanged.
  }
  uwvm_rmw::wasi_signal::diagnostics facts{};
  auto code = uwvm_rmw::wasi_signal::observe(&facts);
  if (code != 0 || facts.state != uwvm_rmw::wasi_signal::phase::prepared ||
    uwvm_rmw::wasi_signal::terminal_error() != 0)
  {
    throw std::runtime_error("No genuinely prepared private WASI signal channel");
  }
  bool int_registered = false;
  bool term_registered = false;
  bool deferred_created = false;
  signal_handlers_options_.store(signal_handler_options, std::memory_order_release);
  try {
    setup_wait_for_signal();
    signal_received_.store(false);
    if (signal_handler_options == SignalHandlerOptions::SigInt ||
      signal_handler_options == SignalHandlerOptions::All)
    {
      old_sigint_handler_ = set_signal_handler(SIGINT, &this->signal_handler);
      int_registered = true;
    }
    if (signal_handler_options == SignalHandlerOptions::SigTerm ||
      signal_handler_options == SignalHandlerOptions::All)
    {
      old_sigterm_handler_ = set_signal_handler(SIGTERM, &this->signal_handler);
      term_registered = true;
    }
    installed_.store(true);
    signal_handler_thread_ = std::thread(&SignalHandler::deferred_signal_handler, this);
    deferred_created = true;
    unsigned mask = (int_registered ? uwvm_rmw::wasi_signal::interrupt_mask : 0u) |
      (term_registered ? uwvm_rmw::wasi_signal::terminate_mask : 0u);
    code = uwvm_rmw::wasi_signal::activate(mask, &SignalHandler::wasi_platform_failure_wake);
    if (code != 0) {
      throw std::runtime_error("Real WASI signal receiver activation failed: " + std::to_string(code));
    }
  } catch (...) {
    // All original fields/semaphore exist before receiver publication. No
    // handler registration or setup error is silently converted into success.
    auto primary = std::current_exception();
    auto receiver_cleanup = uwvm_rmw::wasi_signal::stop_and_join();
    if (receiver_cleanup != 0) {
      // Unknown live callback ownership cannot permit semaphore destruction.
      // Runtime whole-Group stop/physical retirement is independent and required.
      std::terminate();
    }
    installed_.store(false);
    if (deferred_created) {
      notify_signal_handler();
      try {signal_handler_thread_.join();} catch (...) {std::terminate();}
    }
    if (term_registered) {
      try {set_signal_handler(SIGTERM, old_sigterm_handler_);} catch (...) {std::terminate();}
    }
    if (int_registered) {
      try {set_signal_handler(SIGINT, old_sigint_handler_);} catch (...) {std::terminate();}
    }
    signal_handlers_options_.store(SignalHandlerOptions::None, std::memory_order_release);
    teardown_wait_for_signal();
    std::rethrow_exception(primary);
  }
  RCLCPP_DEBUG(get_logger(), "private WASI signal handler installed");
  return true;
}

bool
SignalHandler::uninstall()
{
  // Refuse BEFORE install_mutex: a different thread may own that mutex while
  // physically joining this very receiver/deferred worker.
  if (uwvm_rmw::wasi_signal::in_receiver_thread() || actual_deferred_signal_worker == this) {
    throw std::runtime_error("WASI signal worker cannot reenter install/uninstall: 17");
  }
  std::lock_guard<std::mutex> lock(install_mutex_);
  if (!installed_.load()) {
    return false;
  }
  // Receiver dispatch closes and genuinely joins BEFORE handlers/old fields or
  // the semaphore can be retired. It cannot race a late host-control callback.
  if (signal_handlers_options_.load(std::memory_order_acquire) != SignalHandlerOptions::None) {
    auto code = uwvm_rmw::wasi_signal::stop_and_join();
    if (code != 0) {
      throw std::runtime_error("WASI signal receiver remains owned: " + std::to_string(code));
    }
  }
  if (signal_handlers_options_.load(std::memory_order_acquire) == SignalHandlerOptions::SigInt ||
    signal_handlers_options_.load(std::memory_order_acquire) == SignalHandlerOptions::All)
  {
    set_signal_handler(SIGINT, old_sigint_handler_);
  }
  if (signal_handlers_options_.load(std::memory_order_acquire) == SignalHandlerOptions::SigTerm ||
    signal_handlers_options_.load(std::memory_order_acquire) == SignalHandlerOptions::All)
  {
    set_signal_handler(SIGTERM, old_sigterm_handler_);
  }
  installed_.store(false);
  signal_handlers_options_.store(SignalHandlerOptions::None, std::memory_order_release);
  notify_signal_handler();
  if (signal_handler_thread_.joinable()) {
    signal_handler_thread_.join();
  }
  teardown_wait_for_signal();
  RCLCPP_DEBUG(get_logger(), "private WASI signal handler uninstalled");
  return true;
}

bool
SignalHandler::is_installed()
{
  return installed_.load();
}

SignalHandler::~SignalHandler()
{
  try {
    uninstall();
  } catch (const std::exception & exc) {
    RCLCPP_ERROR(
      get_logger(),
      "caught %s exception when uninstalling signal handlers in rclcpp::~SignalHandler: %s",
      rmw::impl::cpp::demangle(exc).c_str(), exc.what());
  } catch (...) {
    RCLCPP_ERROR(
      get_logger(),
      "caught unknown exception when uninstalling signal handlers in rclcpp::~SignalHandler");
  }
}

SignalHandler::signal_handler_type
SignalHandler::get_old_signal_handler(int signum)
{
  if (SIGINT == signum) {
    return old_sigint_handler_;
  } else if (SIGTERM == signum) {
    return old_sigterm_handler_;
  }
#if defined(RCLCPP_HAS_SIGACTION)
  SignalHandler::signal_handler_type ret;
  memset(&ret, 0, sizeof(ret));
  sigemptyset(&ret.sa_mask);
  ret.sa_handler = SIG_DFL;
  return ret;
#else
  return SIG_DFL;
#endif
}

void
SignalHandler::signal_handler_common()
{
  auto & instance = SignalHandler::get_global_signal_handler();
  instance.signal_received_.store(true);
  instance.notify_signal_handler();
}

void
SignalHandler::deferred_signal_handler()
{
  scoped_deferred_signal_worker entered{this};
  while (true) {
    if (!is_installed()) {
      break;  // Startup rollback/uninstall must join before retiring semaphore.
    }
    auto platform_error = uwvm_rmw::wasi_signal::terminal_error();
    if (platform_error != 0) {
      // This is a real platform failure, never a fabricated signal. An
      // uncaught worker failure terminates this Guest Group; Root must still
      // physically retire all participants/Host owners and preserve diagnostics.
      throw std::runtime_error("Private WASI signal channel failed: " + std::to_string(platform_error));
    }
    if (signal_received_.exchange(false)) {
      RCLCPP_INFO(SignalHandler::get_logger(), "signal_handler(SIGINT/SIGTERM)");
      RCLCPP_DEBUG(get_logger(), "deferred_signal_handler(): shutting down");
      for (auto context_ptr : rclcpp::get_contexts()) {
        if (context_ptr->get_init_options().shutdown_on_signal) {
          RCLCPP_DEBUG(
            get_logger(),
            "deferred_signal_handler(): "
            "shutting down rclcpp::Context @ %p, because it had shutdown_on_signal == true",
            static_cast<void *>(context_ptr.get()));
          try {
            context_ptr->shutdown("signal handler");
          } catch (const std::exception & exc) {
            // an uncaught exception on this thread would call std::terminate(),
            // taking down the whole process, so log the failure instead
            RCLCPP_ERROR(
              get_logger(),
              "deferred_signal_handler(): failed to shutdown rclcpp::Context @ %p: %s",
              static_cast<void *>(context_ptr.get()), exc.what());
          } catch (...) {
            RCLCPP_ERROR(
              get_logger(),
              "deferred_signal_handler(): failed to shutdown rclcpp::Context @ %p",
              static_cast<void *>(context_ptr.get()));
          }
        }
      }
    }
    if (!is_installed()) {
      RCLCPP_DEBUG(get_logger(), "deferred_signal_handler(): signal handling uninstalled");
      break;
    }
    RCLCPP_DEBUG(
      get_logger(), "deferred_signal_handler(): waiting for SIGINT/SIGTERM or uninstall");
    wait_for_signal();
    RCLCPP_DEBUG(
      get_logger(), "deferred_signal_handler(): woken up due to SIGINT/SIGTERM or uninstall");
  }
}

void
SignalHandler::wasi_platform_failure_wake() noexcept
{
  SignalHandler::get_global_signal_handler().notify_signal_handler();
}

void
SignalHandler::setup_wait_for_signal()
{
#if defined(_WIN32)
  signal_handler_sem_ = CreateSemaphore(
    NULL,  // default security attributes
    0,  // initial semaphore count
    1,  // maximum semaphore count
    NULL);  // unnamed semaphore
  if (NULL == signal_handler_sem_) {
    throw std::runtime_error("CreateSemaphore() failed in setup_wait_for_signal()");
  }
#elif defined(__APPLE__)
  signal_handler_sem_ = dispatch_semaphore_create(0);
#else  // posix
  if (-1 == sem_init(&signal_handler_sem_, 0, 0)) {
    throw std::runtime_error(std::string("sem_init() failed: ") + strerror(errno));
  }
#endif
  wait_for_signal_is_setup_.store(true);
}

void
SignalHandler::teardown_wait_for_signal() noexcept
{
  if (!wait_for_signal_is_setup_.exchange(false)) {
    return;
  }
#if defined(_WIN32)
  CloseHandle(signal_handler_sem_);
#elif defined(__APPLE__)
  dispatch_release(signal_handler_sem_);
#else  // posix
  if (-1 == sem_destroy(&signal_handler_sem_)) {
    RCLCPP_ERROR(get_logger(), "invalid semaphore in teardown_wait_for_signal(): %d", errno);
    // No fake teardown success or retry of unknown semaphore ownership.
    std::terminate();
  }
#endif
}

void
SignalHandler::wait_for_signal()
{
  if (!wait_for_signal_is_setup_.load()) {
    RCLCPP_ERROR(get_logger(), "called wait_for_signal() before setup_wait_for_signal()");
    return;
  }
#if defined(_WIN32)
  DWORD dw_wait_result = WaitForSingleObject(signal_handler_sem_, INFINITE);
  switch (dw_wait_result) {
    case WAIT_ABANDONED:
      RCLCPP_ERROR(
        get_logger(), "WaitForSingleObject() failed in wait_for_signal() with WAIT_ABANDONED: %s",
        GetLastError());
      break;
    case WAIT_OBJECT_0:
      // successful
      break;
    case WAIT_TIMEOUT:
      RCLCPP_ERROR(get_logger(), "WaitForSingleObject() timedout out in wait_for_signal()");
      break;
    case WAIT_FAILED:
      RCLCPP_ERROR(
        get_logger(), "WaitForSingleObject() failed in wait_for_signal(): %s", GetLastError());
      break;
    default:
      RCLCPP_ERROR(
        get_logger(), "WaitForSingleObject() gave unknown return in wait_for_signal(): %s",
        GetLastError());
  }
#elif defined(__APPLE__)
  dispatch_semaphore_wait(signal_handler_sem_, DISPATCH_TIME_FOREVER);
#else  // posix
  int s;
  do {
    s = sem_wait(&signal_handler_sem_);
  } while (-1 == s && EINTR == errno);
  if (-1 == s) {
    throw std::runtime_error(std::string("sem_wait() failed: ") + strerror(errno));
  }
#endif
}

void
SignalHandler::notify_signal_handler() noexcept
{
  if (!wait_for_signal_is_setup_.load()) {
    return;
  }
#if defined(_WIN32)
  if (!ReleaseSemaphore(signal_handler_sem_, 1, NULL)) {
    RCLCPP_ERROR(
      get_logger(), "ReleaseSemaphore() failed in notify_signal_handler(): %s", GetLastError());
  }
#elif defined(__APPLE__)
  dispatch_semaphore_signal(signal_handler_sem_);
#else  // posix
  if (-1 == sem_post(&signal_handler_sem_)) {
    RCLCPP_ERROR(get_logger(), "sem_post failed in notify_signal_handler(): %d", errno);
    // A failed wake cannot be hidden behind a successful receiver/deferred join.
    std::terminate();
  }
#endif
}

rclcpp::SignalHandlerOptions
SignalHandler::get_current_signal_handler_options()
{
  return signal_handlers_options_.load(std::memory_order_acquire);
}
