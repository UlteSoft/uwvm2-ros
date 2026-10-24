/**
 * @file service_client_pool.cc
 * @brief Verify session client ownership independently of ROS discovery timing.
 * @details Exercise reuse, exact contract separation, concurrent acquisition,
 * alias retirement and replacement isolation with observable native owners.
 */
#include <gateway/common/service_client_pool.h>
#include <atomic>
#include <cassert>
#include <memory>
#include <thread>
#include <vector>

namespace
{
    struct client
    {
        inline static std::atomic<unsigned> alive{};

        client() { ++alive; }

        ~client() { --alive; }
    };
}  // namespace

int main()
{
    using namespace uwvm2::ros_gateway;
    using capability = uwvm2::imported::ros::capability_spec;
    std::atomic<unsigned> created{}, notifications{};
    {
        service_client_pool unused{[&] { ++notifications; }};
    }
    assert(notifications == 0u);
    capability contract{"speed", "service.call", "/robot/speed", "std_srvs/srv/Trigger", "type-v1", {{"timeout-ms", "100"}}};
    auto create{[&]
                {
                    ++created;
                    return std::make_shared<client>();
                }};
    {
        service_client_pool pool{[&] { ++notifications; }};
        auto first{pool.acquire<client>(contract, contract.resource, create)};
        assert(first);
        std::vector<std::thread> callers;
        for(unsigned index{}; index != 8u; ++index)
        {
            callers.emplace_back(
                [&]
                {
                    for(unsigned attempt{}; attempt != 100u; ++attempt) { assert(pool.acquire<client>(contract, contract.resource, create) == first); }
                });
        }
        for(auto& caller: callers) { caller.join(); }
        assert(created == 1u);

        auto changed{contract};
        changed.limits["timeout-ms"] = "200";
        assert(!pool.acquire<client>(changed, changed.resource, create));
        assert(created == 1u);

        pool.close(contract.alias);
        assert(notifications == 1u && client::alive == 1u);
        auto second{pool.acquire<client>(contract, contract.resource, create)};
        assert(second != first && created == 2u);
        pool.discard(contract.alias, first);
        assert(pool.acquire<client>(contract, contract.resource, create) == second);
        first.reset();
        assert(client::alive == 1u);
        pool.discard(contract.alias, second);
        second.reset();
        assert(client::alive == 0u);

        assert(!pool.acquire<client>(contract, contract.resource, [] { return std::shared_ptr<client>{}; }));
        assert(pool.acquire<client>(contract, contract.resource, create));
    }
    assert(client::alive == 0u && created == 3u);
}
