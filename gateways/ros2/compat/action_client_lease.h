#pragma once
#include <atomic>
#include <memory>
#include <string>
#include <utility>

namespace uwvm2::ros_gateway::ros2_compat
{
    /** Public-interface Action client registration with checked caller options.
     *
     * Older rclcpp_action factories omit their options operand although the
     * public typed Client constructor accepts it. This helper preserves the
     * exact options and public waitables lifetime instead of calling a default
     * factory. It owns no thread/context and does not retain the complete Node.
     * Its duck-typed boundary also permits SDK-free ownership regressions.
     */
    template<class Client,class Node,class Options>
    auto legacy_action_client(Node const& node,std::string const& name,Options const& options)
        -> decltype(Client{node->get_node_base_interface(),node->get_node_graph_interface(),
                          node->get_node_logging_interface(),name,options},
                    node->get_node_waitables_interface()->add_waitable(std::shared_ptr<Client>{},nullptr),
                    node->get_node_waitables_interface()->remove_waitable(std::shared_ptr<Client>{},nullptr),
                    std::shared_ptr<Client>{})
    {
        auto waitables{node->get_node_waitables_interface()};
        using waitables_type=typename decltype(waitables)::element_type;
        struct registration
        {
            std::weak_ptr<waitables_type> owner;
            std::atomic<bool> registered{false};
        };
        auto state{std::make_shared<registration>()};state->owner=waitables;
        auto destroy=[state](Client* value) noexcept
        {
            if(!value) { return; }
            if(state->registered.load(std::memory_order_acquire))
            {
                if(auto owner=state->owner.lock())
                {
                    try
                    {
                        // The native public remove_waitable interface accepts
                        // a shared view. It must not delete this Client while
                        // its original control block is already retiring.
                        auto view=std::shared_ptr<Client>{value,[](Client*) noexcept {}};
                        owner->remove_waitable(view,nullptr);
                    }
                    catch(...) {} // Destruction still releases native storage.
                }
            }
            delete value;
        };
        auto client=std::shared_ptr<Client>{new Client{node->get_node_base_interface(),
            node->get_node_graph_interface(),node->get_node_logging_interface(),name,options},std::move(destroy)};
        try { waitables->add_waitable(client,nullptr); }
        catch(...)
        {
            // A public implementation may append the waitable and then fail
            // while notifying its executor. Roll back that possible partial
            // registration while the actual Client still has a strong owner.
            try { waitables->remove_waitable(client,nullptr); }catch(...) {}
            throw;
        }
        state->registered.store(true,std::memory_order_release);
        return client;
    }
}
