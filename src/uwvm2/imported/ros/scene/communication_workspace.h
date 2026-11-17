/** @file
 * @brief Finite recoverable call workspace shared by communication dispatchers.
 * @details Capacity is reserved before allocation. Limits are deployment capacity,
 * not a rate limit; zero reserves no capacity. Tokens keep their account alive
 * and release without touching inherited mutexes after fork. All owning memory
 * uses the project's recoverable fast_io allocation adapters.
 */
#pragma once
#include "authority.h"
#include <uwvm2/utils/container/shared.h>
namespace uwvm2::imported::ros::scene
{
    class communication_workspace
    {
        struct account
        {
            std::mutex mutex;
            std::uint64_t maximum{},calls_max{},used{},calls{};
            bool retired{};
#if defined(__linux__)
            ::pid_t process{::getpid()};
#endif
        };
        std::shared_ptr<account> value;
    public:
        class lease
        {
            friend class communication_workspace;
            std::shared_ptr<account> value;
            std::uint64_t bytes{};
        public:
            lease()=default;lease(lease const&)=delete;lease& operator=(lease const&)=delete;
            lease(lease&& other) noexcept:value(std::move(other.value)),bytes(std::exchange(other.bytes,0)){}
            lease& operator=(lease&& other) noexcept
            {if(this!=&other){reset();value=std::move(other.value);bytes=std::exchange(other.bytes,0);}return *this;}
            ~lease(){reset();}
            explicit operator bool() const noexcept{return bool(value);}
            void reset() noexcept
            {
#if defined(__linux__)
                if(value&&value->process!=::getpid()){std::construct_at(std::addressof(value));bytes=0;return;}
#endif
                if(value){std::lock_guard lock{value->mutex};value->used-=bytes;--value->calls;}
                value.reset();bytes=0;
            }
        };
        error configure(std::uint64_t bytes,std::uint64_t calls)
        {
            if(value)return error::conflict;
            if(!bytes||bytes>INT64_MAX||!calls||calls>INT64_MAX)return error::invalid;
            try{auto next=::uwvm2::utils::container::make_shared<account>();next->maximum=bytes;next->calls_max=calls;value=std::move(next);return error::success;}
            catch(std::bad_alloc const&){return error::no_memory;}
        }
        error reserve(std::uint64_t bytes,lease& output)
        {
            if(!value)return error::denied;
#if defined(__linux__)
            if(value->process!=::getpid())return error::denied;
#endif
            if(output)return error::conflict;
            std::lock_guard lock{value->mutex};if(value->retired)return error::cancelled;
            if(bytes>value->maximum-value->used||value->calls>=value->calls_max)return error::again;
            value->used+=bytes;++value->calls;output.value=value;output.bytes=bytes;return error::success;
        }
        error inspect(std::uint64_t& bytes,std::uint64_t& calls) const noexcept
        {
            if(!value){bytes=0;calls=0;return error::success;}
#if defined(__linux__)
            if(value->process!=::getpid())return error::denied;
#endif
            std::unique_lock lock{value->mutex,std::try_to_lock};if(!lock.owns_lock())return error::again;
            bytes=value->used;calls=value->calls;return error::success;
        }
        void retire() noexcept
        {
            if(!value)return;
#if defined(__linux__)
            if(value->process!=::getpid())return;
#endif
            std::lock_guard lock{value->mutex};value->retired=true;
        }
    };
}
