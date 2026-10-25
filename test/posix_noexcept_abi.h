/**
 * @file posix_noexcept_abi.h
 * @brief Provide nonthrowing, native POSIX calls for process and unwind tests.
 * @details These wrappers call the platform's own declarations, preserving its
 * off_t, time, signal and pthread ABI. They neither call the VM implementation
 * nor translate errno. POSIX failures remain observable return values.
 */
#pragma once
#if defined(__unix__) || defined(__APPLE__)
# include <pthread.h>
# include <sched.h>
# include <signal.h>
# include <sys/mman.h>
# include <sys/resource.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

namespace uwvm2test::posix_abi
{
    inline int pipe_noexcept(int* descriptors) noexcept { return ::pipe(descriptors); }

    inline pid_t fork_noexcept() noexcept { return ::fork(); }

    inline int close_noexcept(int descriptor) noexcept { return ::close(descriptor); }

    inline int dup2_noexcept(int source, int destination) noexcept { return ::dup2(source, destination); }

    inline ssize_t read_noexcept(int descriptor, void* bytes, size_t size) noexcept { return ::read(descriptor, bytes, size); }

    inline ssize_t write_noexcept(int descriptor, void const* bytes, size_t size) noexcept { return ::write(descriptor, bytes, size); }

    [[noreturn]] inline void _exit_noexcept(int status) noexcept { ::_exit(status); }

    inline pid_t waitpid_noexcept(pid_t process, int* status, int options) noexcept { return ::waitpid(process, status, options); }

    inline int sigemptyset_noexcept(sigset_t* set) noexcept { return ::sigemptyset(set); }

    inline int sigaction_noexcept(int number, struct sigaction const* action, struct sigaction* previous) noexcept
    { return ::sigaction(number, action, previous); }

    inline int raise_noexcept(int number) noexcept { return ::raise(number); }

    inline int sigaltstack_noexcept(stack_t const* stack, stack_t* previous) noexcept { return ::sigaltstack(stack, previous); }

    inline long sysconf_noexcept(int selector) noexcept { return ::sysconf(selector); }

    inline void* mmap_noexcept(void* address, size_t size, int protection, int flags, int descriptor, off_t offset) noexcept
    { return ::mmap(address, size, protection, flags, descriptor, offset); }

    inline int mprotect_noexcept(void* address, size_t size, int protection) noexcept { return ::mprotect(address, size, protection); }

    inline int munmap_noexcept(void* address, size_t size) noexcept { return ::munmap(address, size); }

    inline int getrlimit_noexcept(decltype(RLIMIT_CORE) resource, struct rlimit* limits) noexcept { return ::getrlimit(resource, limits); }

    inline int setrlimit_noexcept(decltype(RLIMIT_CORE) resource, struct rlimit const* limits) noexcept { return ::setrlimit(resource, limits); }

    inline int sched_yield_noexcept() noexcept { return ::sched_yield(); }

    inline pthread_t pthread_self_noexcept() noexcept { return ::pthread_self(); }

    inline int pthread_key_create_noexcept(pthread_key_t* key, void (*destroy)(void*)) noexcept { return ::pthread_key_create(key, destroy); }

    inline int pthread_key_delete_noexcept(pthread_key_t key) noexcept { return ::pthread_key_delete(key); }

    inline int pthread_setspecific_noexcept(pthread_key_t key, void const* value) noexcept { return ::pthread_setspecific(key, value); }

    inline int pthread_attr_init_noexcept(pthread_attr_t* attributes) noexcept { return ::pthread_attr_init(attributes); }

    inline int pthread_attr_destroy_noexcept(pthread_attr_t* attributes) noexcept { return ::pthread_attr_destroy(attributes); }

    inline int pthread_attr_setstack_noexcept(pthread_attr_t* attributes, void* address, size_t size) noexcept
    { return ::pthread_attr_setstack(attributes, address, size); }

    inline int pthread_attr_setstacksize_noexcept(pthread_attr_t* attributes, size_t size) noexcept { return ::pthread_attr_setstacksize(attributes, size); }

    inline int pthread_attr_getstack_noexcept(pthread_attr_t const* attributes, void** address, size_t* size) noexcept
    { return ::pthread_attr_getstack(attributes, address, size); }

    inline int pthread_create_noexcept(pthread_t* thread, pthread_attr_t const* attributes, void* (*entry)(void*), void* argument) noexcept
    { return ::pthread_create(thread, attributes, entry, argument); }

    inline int pthread_join_noexcept(pthread_t thread, void** result) noexcept { return ::pthread_join(thread, result); }
# if defined(__linux__)
    inline int mincore_noexcept(void* address, size_t size, unsigned char* resident) noexcept { return ::mincore(address, size, resident); }

    inline int pthread_getattr_np_noexcept(pthread_t thread, pthread_attr_t* attributes) noexcept { return ::pthread_getattr_np(thread, attributes); }
# endif
}  // namespace uwvm2test::posix_abi
#endif
