// Sparse Linux reservations coexist with ASan without touching shadow memory
// for the entire inaccessible address window. Only committed bytes are exposed.
#pragma once
#include <cstddef>
#include <cstdint>
#if defined(__linux__)
#include <sys/mman.h>
#include <sys/syscall.h>
#include <unistd.h>
#if defined(__SANITIZE_ADDRESS__)
#define UWVM_ASAN_LINUX_MAPPING 1
#elif defined(__has_feature)
#if __has_feature(address_sanitizer)
#define UWVM_ASAN_LINUX_MAPPING 1
#endif
#endif
#if defined(UWVM_ASAN_LINUX_MAPPING)
#include <sanitizer/asan_interface.h>
#endif
namespace uwvm2::utils::allocator::sanitizer_mapping
{
    [[nodiscard]] inline void* reserve(::std::size_t bytes,int flags) noexcept
    {
#if defined(UWVM_ASAN_LINUX_MAPPING)
        // No fixed address is requested: the kernel cannot overwrite ASan's
        // own shadow or another mapping. mmap2's final argument is zero pages.
#if defined(SYS_mmap2)
        auto const result=::syscall(SYS_mmap2,nullptr,bytes,PROT_NONE,flags,-1,0);
#else
        auto const result=::syscall(SYS_mmap,nullptr,bytes,PROT_NONE,flags,-1,0);
#endif
        return result==-1?MAP_FAILED:reinterpret_cast<void*>(result);
#else
        return ::mmap(nullptr,bytes,PROT_NONE,flags,-1,0);
#endif
    }
    inline void accessible(void* address,::std::size_t bytes) noexcept
    {
#if defined(UWVM_ASAN_LINUX_MAPPING)
        if(bytes)::__asan_unpoison_memory_region(address,bytes);
#else
        static_cast<void>(address);static_cast<void>(bytes);
#endif
    }
    inline int release(void* address,::std::size_t bytes) noexcept
    {
#if defined(UWVM_ASAN_LINUX_MAPPING)
        return static_cast<int>(::syscall(SYS_munmap,address,bytes));
#else
        return ::munmap(address,bytes);
#endif
    }
}
#endif
