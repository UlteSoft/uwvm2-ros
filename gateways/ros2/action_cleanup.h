#pragma once
#include <cstddef>
#include <cstdint>
#include <map>
#include <memory>
#include <mutex>

namespace uwvm2::ros_gateway::ros2_action
{
    struct goal_state;
    class cleanup_pool
    {
        std::uint64_t maximum_;
        mutable std::mutex mutex_;
        std::map<std::uint64_t,std::shared_ptr<goal_state>> pending_;
    public:
        explicit cleanup_pool(std::uint64_t maximum):maximum_{maximum} {}
        cleanup_pool(cleanup_pool const&)=delete;
        cleanup_pool& operator=(cleanup_pool const&)=delete;
        bool track(std::uint64_t id,std::shared_ptr<goal_state> const& goal)
        {
            if(!id||!goal) { return false; }
            std::lock_guard<std::mutex> guard{mutex_};
            if(pending_.size()>=maximum_||pending_.find(id)!=pending_.end()) { return false; }
            return pending_.emplace(id,goal).second;
        }
        void release(std::uint64_t id)
        {
            std::shared_ptr<goal_state> retired;
            {
                std::lock_guard<std::mutex> guard{mutex_};
                auto found{pending_.find(id)};
                if(found==pending_.end()) { return; }
                retired=std::move(found->second);
                pending_.erase(found);
            }
            // Native client/state destruction may reenter cleanup callbacks.
            // Keep its final reference release outside the pool mutex.
        }
        std::size_t count() const
        {
            std::lock_guard<std::mutex> guard{mutex_};return pending_.size();
        }
    };
}
