/** @file
 * @brief Actual filtered ROS2 discovery using a counted minimal-node attachment.
 * @details The local discovery cache may change during a scan; output is an
 * observed revisioned cache view, not an atomic distributed truth. Only exact
 * approved node names and selected endpoint/type fields leave the provider.
 * Per-node endpoint result arrays use an explicit fast_io allocation budget.
 * Jazzy rcl_get_node_names ignores its allocator argument, so node-name arrays
 * and middleware internal allocations still require the isolated worker budget. Domain and node names
 * never establish peer identity, especially in the unauthenticated local profile.
 */
#pragma once
#include "external_process_only.h"
#include "communication_ros2.h"
#include "../communication_graph.h"
#include "communication_allocator.h"
#include <rcl/graph.h>
#include <rcutils/types/string_array.h>

namespace uwvm2::imported::ros::scene::communication::ros2
{
    class graph_source final : public graph::source
    {
        node_lease attachment;
        std::size_t workspace_bytes{};
        struct names
        {
            rcutils_string_array_t value{rcutils_get_zero_initialized_string_array()};
            ~names(){if(value.data){auto status=rcutils_string_array_fini(&value);static_cast<void>(status);}}
        };
        struct endpoints
        {
            rcl_names_and_types_t value{rcl_get_zero_initialized_names_and_types()};
            ~endpoints(){if(value.names.data||value.types){auto status=rcl_names_and_types_fini(&value);static_cast<void>(status);}}
        };

    public:
        /** @brief Require a finite trusted result-allocation budget; zero stays denied. */
        explicit graph_source(node_lease selected,std::size_t query_workspace_bytes=0) : attachment(std::move(selected)),workspace_bytes(query_workspace_bytes) {}

        bool live() const noexcept override { return attachment.live(); }

        handle context() const noexcept override { return attachment.context(); }

        owner authority() const noexcept override { return attachment.authority(); }

        error capture(graph::plan const& plan, ::uwvm2::utils::container::std_vector<graph::entity>& output) noexcept override
        {
            if(!workspace_bytes)return error::denied;
            if(!attachment.live() || plan.context != attachment.context()) { return error::denied; }
            auto selected = dynamic_cast<node*>(attachment.get());
            if(!selected || !selected->value || !selected->parent->live()) { return error::cancelled; }
            try
            {
                // Public C calls avoid a forced std::string/map ABI. Endpoint
                // arrays honor this bounded allocator. Jazzy node enumeration
                // explicitly ignores it; that external allocation stays within
                // the separately mandatory native worker memory boundary.
                std::lock_guard query_lock{selected->graph_queries};
                query_allocator workspace{workspace_bytes};auto allocator=workspace.native();
                names discovered_names,discovered_spaces;
                auto native_node=selected->value->base->get_rcl_node_handle();
                auto native_result=[&](rcl_ret_t result)
                {if(result==RCL_RET_OK)return error::ok;rcl_reset_error();if(workspace.exhausted())return error::too_large;return result==RCL_RET_BAD_ALLOC?error::no_memory:error::again;};
                auto node_status=native_result(rcl_get_node_names(native_node,allocator,&discovered_names.value,&discovered_spaces.value));
                if(node_status!=error::ok)return node_status;
                if(discovered_names.value.size!=discovered_spaces.value.size)return error::io;
                std::size_t bytes{};
                auto append = [&](graph::kind kind, auto const& name, std::string_view endpoint, std::string_view type) -> error
                {
                    if(name.size() > 255 || endpoint.size() > 255 || type.size() > 255) { return error::too_large; }
                    auto amount = std::size_t{32} + name.size() + endpoint.size() + type.size();
                    if(output.size() == plan.max_rows || amount > plan.max_bytes - bytes) { return error::too_large; }
                    graph::entity row;
                    row.type = kind;
                    row.node.assign(name.data(), name.size());
                    row.endpoint.assign(endpoint);
                    row.message_type.assign(type);
                    output.push_back(std::move(row));
                    bytes += amount;
                    return error::ok;
                };
                for(auto const& allowed: plan.nodes)
                {
                    auto slash = allowed.rfind('/');
                    std::string_view name{allowed.data() + slash + 1, allowed.size() - slash - 1};
                    std::string_view space = slash ? std::string_view{allowed.data(), slash} : std::string_view{"/"};
                    bool exists{};
                    for(std::size_t i{};i<discovered_names.value.size;++i)
                    {
                        auto actual_name=discovered_names.value.data[i],actual_space=discovered_spaces.value.data[i];
                        if(!actual_name||!actual_space)return error::io;
                        if(actual_name == name && actual_space == space)
                        {
                            exists = true;
                            break;
                        }
                    }
                    if(!exists) { continue; }
                    if(plan.fields & 1)
                    {
                        auto result = append(graph::kind::node, allowed, {}, {});
                        if(result != error::ok) { return result; }
                    }
                    std::array<char,256> native_name{},native_namespace{};
                    std::copy(name.begin(),name.end(),native_name.begin());std::copy(space.begin(),space.end(),native_namespace.begin());
                    auto query = [&](graph::kind kind)->error
                    {
                        endpoints found;rcl_ret_t result{};
                        switch(kind)
                        {
                        case graph::kind::publisher:result=rcl_get_publisher_names_and_types_by_node(native_node,&allocator,false,native_name.data(),native_namespace.data(),&found.value);break;
                        case graph::kind::subscription:result=rcl_get_subscriber_names_and_types_by_node(native_node,&allocator,false,native_name.data(),native_namespace.data(),&found.value);break;
                        case graph::kind::service:result=rcl_get_service_names_and_types_by_node(native_node,&allocator,native_name.data(),native_namespace.data(),&found.value);break;
                        case graph::kind::client:result=rcl_get_client_names_and_types_by_node(native_node,&allocator,native_name.data(),native_namespace.data(),&found.value);break;
                        default:return error::invalid;
                        }
                        auto status=native_result(result);if(status!=error::ok)return status;
                        for(std::size_t i{};i<found.value.names.size;++i)
                        {
                            if(!found.value.names.data[i])return error::io;
                            std::string_view endpoint{found.value.names.data[i]};
                            if(!(plan.fields&32))
                            {auto added=append(kind,allowed,endpoint,{});if(added!=error::ok)return added;}
                            else
                            {
                                if(!found.value.types)return error::io;
                                auto const& types=found.value.types[i];
                                for(std::size_t j{};j<types.size;++j)
                                {if(!types.data[j])return error::io;auto added=append(kind,allowed,endpoint,std::string_view{types.data[j]});if(added!=error::ok)return added;}
                            }
                        }
                        return error::ok;
                    };
                    for(auto kind:{graph::kind::publisher,graph::kind::subscription,graph::kind::service,graph::kind::client})
                    {if(plan.fields&static_cast<std::uint64_t>(kind)){auto result=query(kind);if(result!=error::ok)return result;}}
                }
                std::sort(output.begin(),
                          output.end(),
                          [](auto const& a, auto const& b)
                          {
                              if(a.node != b.node) { return a.node < b.node; }
                              if(a.type != b.type) { return a.type < b.type; }
                              if(a.endpoint != b.endpoint) { return a.endpoint < b.endpoint; }
                              return a.message_type < b.message_type;
                          });
                if(!attachment.live() || !selected->parent->live()) { return error::cancelled; }
                return error::ok;
            }
            catch(std::bad_alloc const&)
            {
                return error::no_memory;
            }
            catch(...)
            {
                return error::again;
            }  // native graph may change during a multi-query scan
        }
    };
}  // namespace uwvm2::imported::ros::scene::communication::ros2
