/*************************************************************
 * UWVM2 owned native ROS2 client readiness. APL-2.0.         *
 *************************************************************/
#pragma once
#include "qos.h"
#include <uwvm2/imported/ros/common/service_availability_history.h>
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <thread>
#include <type_traits>
#include <utility>

namespace uwvm2::ros_gateway::ros2_service_availability
{
    namespace wire=::uwvm2::imported::ros::ipc::client_availability;
    using history=::uwvm2::imported::ros::service_availability_history;
    struct result {std::uint32_t error{};std::uint64_t value{};};
    template<class T,class=void> struct supports_query:std::false_type{};
    template<class T> struct supports_query<T,std::void_t<decltype(std::declval<T&>().service_is_ready())>>
        :std::is_convertible<decltype(std::declval<T&>().service_is_ready()),bool>{};

    // One registry per existing authenticated adapter/session, serialized by its
    // request owner. These are EXTRA rcl clients/endpoints under the explicit
    // observation grant, distinct from service.call's ephemeral native client.
    // No request, event subscription, native cancellation or executor wait is
    // issued here. The selected RMW decides its participant/entity mapping; no
    // fixed participant count or cryptographic remote peer identity is claimed.
    class registry
    {
        struct client_base
        {
            wire::context binding{};std::string availability_alias{},source_alias{};
            virtual ~client_base()=default;
            virtual bool ready()=0;
        };
        template<class Service> struct typed_client final:client_base
        {
            std::shared_ptr<rclcpp::Client<Service>> native;
            bool ready() override{return native->service_is_ready();}
        };
        std::map<std::uint64_t,std::unique_ptr<client_base>> clients_;
    public:
        // Whole generation/alias matching and local tombstone retirement happen
        // in the common ledger FIRST. This erases native ownership without any
        // token/bandwidth admission or new allocation. Session destruction also
        // abandons every client after IPC loss; no raw pointer survives it.
        void retire(wire::context const& binding,wire::retirement scope,
                    std::string_view availability_alias,std::string_view source_alias) noexcept
        {
            for(auto i=clients_.begin();i!=clients_.end();)
            {
                auto const& c{*i->second};
                bool selected{scope==wire::retirement::availability_handle?c.binding.availability==binding.availability:
                    scope==wire::retirement::source_handle?c.binding.source==binding.source:
                    scope==wire::retirement::availability_alias?c.availability_alias==availability_alias:
                    scope==wire::retirement::source_alias&&c.source_alias==source_alias};
                if(selected){i=clients_.erase(i);}else{++i;}
            }
        }
        void clear() noexcept{clients_.clear();}

        // Qualifier is the EXISTING real typed builtin/custom native identity
        // check selected by the concrete adapter's finite Service dispatch. It
        // must consume source (the original service.call contract), not a fake
        // generated identity or a Guest name. Both complete actual QoS getters
        // are mandatory; old SDKs without them refuse95 before entity creation.
        template<class Service,class Node,class Qualifier>
        result observe(Node const& node,ros::capability_spec const& availability,
                       ros::capability_spec const& source,wire::packet const& request,
                       std::uint32_t timeout_ms,Qualifier qualify)
        {
            if(!wire::valid_request(request)||request.binding.backend!=2||request.operation==wire::command::retire||
               !ros::service_availability_pair(ros::version::ros2,availability,source)){return {22,0};}
            if(!qualify(source)){return {13,0};}
            if constexpr(!supports_query<rclcpp::Client<Service>>::value||!ros2_qos::supports_client_qos<rclcpp::Client<Service>>::value)
            {return {95,0};}
            else
            {
                if(request.operation==wire::command::features)
                {return {0,wire::typed_query|wire::finite_wait|wire::actual_qos_check};}
                auto maximum{limit(availability,"max-wait-ns")};
                if(request.value>maximum||!timeout_ms){return {22,0};}
                auto started{std::chrono::steady_clock::now()};
                auto until{started+std::chrono::milliseconds(std::min<std::uint64_t>(timeout_ms,limit(availability,"timeout-ms")))};
                auto logical{started+std::chrono::nanoseconds(request.value)};
                client_base* active{};auto found{clients_.find(request.binding.availability)};
                if(found!=clients_.end())
                {
                    active=found->second.get();
                    if(!wire::same(active->binding,request.binding)||active->availability_alias!=availability.alias||active->source_alias!=source.alias){return {13,0};}
                }
                else
                {
                    std::uint64_t count{};
                    for(auto const& row:clients_){if(row.second->availability_alias==availability.alias){++count;}}
                    if(count>=limit(availability,"max-native-clients")){return {11,0};}
                    // Reserve both copied ownership AND the map node before
                    // native creation. No allocation is deferred until after
                    // create_client. Every refusal/exception erases this owned
                    // pending slot and destroys any partially created client.
                    auto pending{std::make_unique<typed_client<Service>>()};
                    pending->binding=request.binding;pending->availability_alias=availability.alias;pending->source_alias=source.alias;
                    auto inserted{clients_.emplace(request.binding.availability,std::move(pending))};
                    if(!inserted.second){return {17,0};}
                    struct creation_transaction
                    {
                        decltype(clients_)& clients;decltype(inserted.first) position;Node const& node;bool committed{};
                        ~creation_transaction() noexcept
                        {
                            if(!committed)
                            {
                                clients.erase(position);
                                // Match ordinary retire/clear: an executor wait
                                // set may still retain a newly created client.
                                // This local transaction cannot outlive observe
                                // or its node argument. No raw native reference
                                // escapes; notification is best-effort cleanup.
                                try{ros2_compat::notify(node);}catch(...){}
                            }
                        }
                    } transaction{clients_,inserted.first,node};
                    auto* owned{static_cast<typed_client<Service>*>(inserted.first->second.get())};
                    auto configured{ros2_qos::configured(source)};
                    owned->native=ros2_compat::create_client<Service>(node,source.resource,configured);
                    if(!owned->native||!ros2_qos::client(*owned->native,configured)){return {95,0};}
                    if(std::chrono::steady_clock::now()>=until){return {110,0};}
                    transaction.committed=true;active=owned;
                }
                // At least one real native query is required, including wait0.
                // A positive logical wait includes client setup time. The
                // separate elapsed-time checks include native setup/query
                // overhead; expiry110 cannot be relabeled false/success. A
                // synchronous RMW call cannot be preempted by this postcheck.
                // Host IPC timeout limits Guest waiting, while a separately
                // pidfd-owned Gateway process deadline bounds a stuck native
                // call/finalizer and must preserve actual retirement evidence.
                for(;;)
                {
                    if(std::chrono::steady_clock::now()>=until){return {110,0};}
                    bool ready{active->ready()};auto now{std::chrono::steady_clock::now()};
                    if(now>=until){return {110,0};}
                    if(ready){return {0,1};}
                    if(!request.value||now>=logical){return {0,0};}
                    auto next{std::min(logical,std::min(until,now+std::chrono::milliseconds(1)))};
                    std::this_thread::sleep_until(next);
                }
            }
        }
    };
}
