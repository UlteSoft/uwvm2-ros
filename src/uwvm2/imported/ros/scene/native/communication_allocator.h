/** @file
 * @brief Recoverable fast_io allocator for bounded public ROS C query results.
 * @details A synchronous query owns this allocator state until all returned C
 * arrays are finalized. Every live allocation includes its aligned accounting
 * header in the explicit capacity. Reallocation failure preserves the previous
 * allocation and charge. Zero capacity grants no allocation. This bounds the
 * public C result allocator, not middleware-internal caches or total worker RSS.
 */
#pragma once
#include "external_process_only.h"
#include <uwvm2/utils/allocator/fast_io_strict/impl.h>
#include <rcutils/allocator.h>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>
namespace uwvm2::imported::ros::scene::communication::ros2
{
    class query_allocator
    {
        using backing=::uwvm2::utils::allocator::fast_io_strict::native_strict_global_allocator;
        struct alignas(std::max_align_t) header{std::size_t bytes;};
        std::size_t maximum_{},used_{};bool exhausted_{};
        static void* allocate(std::size_t bytes,void* context) noexcept
        {
            auto& self=*static_cast<query_allocator*>(context);if(!bytes)bytes=1;
            if(bytes>SIZE_MAX-sizeof(header)||bytes+sizeof(header)>self.maximum_-self.used_){self.exhausted_=true;return nullptr;}
            auto extent=bytes+sizeof(header);auto* allocation=static_cast<header*>(backing::allocate(extent));if(!allocation)return nullptr;
            allocation->bytes=extent;self.used_+=extent;return allocation+1;
        }
        static void deallocate(void* pointer,void* context) noexcept
        {
            if(!pointer)return;
            auto& self=*static_cast<query_allocator*>(context);auto* allocation=static_cast<header*>(pointer)-1;
            self.used_-=allocation->bytes;backing::deallocate(allocation);
        }
        static void* reallocate(void* pointer,std::size_t bytes,void* context) noexcept
        {
            if(!pointer)return allocate(bytes,context);
            if(!bytes){deallocate(pointer,context);return nullptr;}
            auto& self=*static_cast<query_allocator*>(context);auto* old=static_cast<header*>(pointer)-1;auto previous=old->bytes;
            if(bytes>SIZE_MAX-sizeof(header)||bytes+sizeof(header)>self.maximum_-(self.used_-previous)){self.exhausted_=true;return nullptr;}
            auto extent=bytes+sizeof(header);auto* replacement=static_cast<header*>(backing::reallocate(old,extent));if(!replacement)return nullptr;
            replacement->bytes=extent;self.used_=self.used_-previous+extent;return replacement+1;
        }
        static void* zero_allocate(std::size_t count,std::size_t size,void* context) noexcept
        {
            if(count&&size>SIZE_MAX/count){static_cast<query_allocator*>(context)->exhausted_=true;return nullptr;}
            auto bytes=count*size;auto* result=allocate(bytes,context);if(result)std::memset(result,0,bytes);return result;
        }
    public:
        explicit query_allocator(std::size_t maximum):maximum_(maximum){}
        query_allocator(query_allocator const&)=delete;query_allocator& operator=(query_allocator const&)=delete;
        rcutils_allocator_t native() noexcept{return {allocate,deallocate,reallocate,zero_allocate,this};}
        bool exhausted() const noexcept{return exhausted_;}
        std::size_t used() const noexcept{return used_;}
    };
}
