/** @file @brief Guarded recoverable fast_io storage for VM-owned supervisor threads. */
#pragma once
#include "../runtime/resources.h"
#include <uwvm2/utils/container/standard.h>
#if defined(__linux__)
#include <pthread.h>
#include <sys/mman.h>
#include <unistd.h>
namespace uwvm2::imported::ros::scene::model_worker
{
    using error=resources::error;
    /** @brief Finite supervisor stack allocated through the recoverable VM allocator.
     * @details Both ends have inaccessible guard pages. The allocation remains
     * owned until pthread_join completes; no detached native thread is used.
     */
    class supervisor_stack
    {
        void* allocation{};
        std::size_t extent{}, page{};
    public:
        supervisor_stack()=default;
        supervisor_stack(supervisor_stack const&)=delete;
        supervisor_stack& operator=(supervisor_stack const&)=delete;
        ~supervisor_stack()
        {
            if(allocation)
            {
                ::mprotect(allocation,page,PROT_READ|PROT_WRITE);
                ::mprotect(static_cast<std::uint8_t*>(allocation)+extent-page,page,PROT_READ|PROT_WRITE);
                ::uwvm2::utils::allocator::fast_io_strict::native_strict_global_allocator::deallocate_aligned(allocation,page);
            }
        }
        error prepare(std::uint64_t bytes) noexcept
        {
            if(allocation || bytes>SIZE_MAX) { return error::invalid; }
            auto actual=::sysconf(_SC_PAGESIZE);
            if(actual<=0 || bytes%static_cast<std::uint64_t>(actual) || bytes>SIZE_MAX-2*static_cast<std::size_t>(actual)) { return error::invalid; }
            page=static_cast<std::size_t>(actual);extent=static_cast<std::size_t>(bytes)+2*page;
            allocation=::uwvm2::utils::allocator::fast_io_strict::native_strict_global_allocator::allocate_aligned(page,extent);
            if(!allocation) { return error::no_memory; }
            return !::mprotect(allocation,page,PROT_NONE) &&
                   !::mprotect(static_cast<std::uint8_t*>(allocation)+extent-page,page,PROT_NONE)?error::success:error::io;
        }
        void* data() const noexcept { return static_cast<std::uint8_t*>(allocation)+page; }
        std::size_t size() const noexcept { return extent-2*page; }
    };

}
#endif
