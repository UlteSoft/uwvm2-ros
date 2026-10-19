/** @file
 * @brief An owned, eagerly created native publisher for the original RMW path.
 *
 * This is distinct from the legacy first-publish activation path. Creation
 * returns a real retained publisher, its actual queried QoS and its real GID.
 * Every subsequent publish and retirement addresses this exact object and
 * incarnation. There is no dummy publication, temporary probe or fallback to
 * another alias's lazily created publisher.
 *
 * The authenticated transport must first lease/validate the exact source grant
 * and assign a fresh nonreusing native incarnation. This object performs the
 * complete real generated type/codec association check too. It grants nothing.
 */
#pragma once
#include "../../compat/node.h"
#include "topic_proof.hpp"
#include "native_rmw_metadata.h"
#include <mutex>

namespace uwvm2::ros_gateway::default_control_gateway::publisher_detail
{
    namespace wire=::uwvm2::imported::ros::ipc::rmw_endpoint;
    using endpoint=::uwvm2::ros_gateway::native_rmw_publisher::endpoint;
    template<class Native>class typed_endpoint final:public endpoint
    {
        ::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr node_;
        std::shared_ptr<detail::TopicProof<Native> const> proof_;
        ros::capability_spec capability_;
        wire::packet association_;
        std::shared_ptr<rclcpp::Publisher<Native>> publisher_;
        std::mutex mutex_;
        bool retired_{};
        typed_endpoint(std::shared_ptr<detail::TopicProof<Native> const> proof,::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr node,ros::capability_spec cap,wire::packet association,
            std::shared_ptr<rclcpp::Publisher<Native>> publisher):
            node_{std::move(node)},proof_{std::move(proof)},capability_{std::move(cap)},association_{association},publisher_{std::move(publisher)}{}
        bool owns(wire::binding const& owner,std::uint64_t incarnation) const noexcept
        {return wire::same(owner,association_.owner)&&incarnation==association_.incarnation;}
        void notify() noexcept{try{ros2_compat::notify(node_);}catch(...) {}}
    public:
        /** Factory output is unchanged on every failure. Reserve registry and
         * rollback storage BEFORE calling this factory. The selected Codec is
         * the real trusted generated whole-message codec, not Guest metadata.
         * Generic custom dispatch is a separate trusted compile-time registry.
         */
        static std::uint32_t create(std::shared_ptr<detail::TopicProof<Native> const> const& proof,::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr const& node,ros::capability_spec const& cap,
            wire::binding const& owner,std::uint64_t incarnation,std::shared_ptr<endpoint>& output,
            wire::packet& observed) noexcept
        {

            if(!node||!wire::valid(owner)||!incarnation||cap.operation!="topic.publish"){return 22;}
            if constexpr(!UWVM_ROS2_HAVE_COMPLETE_RMW_QOS||!native_rmw_metadata::has_entity_actual<rclcpp::Publisher<Native>>::value||
                !native_rmw_metadata::has_gid<rclcpp::Publisher<Native>>::value){return 95;}
            else
            {
                try
                {
                    if(!proof||!proof->root()||!proof->selected(cap)){return 13;}
                    auto root=proof->root();auto error=root->validate(root,root->generated_support);if(error)return error;
                    // Allocate every Guest-independent local owner/control block
                    // before native creation. The empty holder is not published.
                    auto holder{std::shared_ptr<typed_endpoint>{new typed_endpoint{proof,node,cap,{},nullptr}}};
                    wire::packet reply{};reply.operation=wire::command::activate;reply.kind=wire::role::publisher;
                    reply.owner=owner;reply.incarnation=incarnation;
                    auto configured{ros2_qos::configured(cap)};
                    auto created{ros2_compat::create_publisher<Native>(node,cap.resource,configured)};
                    if(!created){return 5;}
                    holder->publisher_=std::move(created);
                    auto actual_name{holder->publisher_->get_topic_name()};
                    if(!actual_name||std::string_view{actual_name}!=cap.resource){return 13;}
                    auto code{native_rmw_metadata::publisher(*holder->publisher_,reply)};if(code){return code;}
                    wire::qos requested{};
                    if(!native_rmw_metadata::actual_profile(ros2_compat::profile(configured),requested)||!wire::same(requested,reply.actual[0])){return 95;}
                    holder->association_=reply;observed=reply;output=std::move(holder);return 0;
                }
                catch(std::bad_alloc const&){return 12;}
                catch(...){return 5;}
            }
        }
        ~typed_endpoint() override
        {
            // Shared ownership must keep the endpoint alive through admitted
            // calls. Destruction is final native cleanup, never a business retry.
            publisher_.reset();notify();
        }
        std::uint32_t inspect(wire::binding const& owner,std::uint64_t incarnation,wire::packet& output) override
        {
            std::lock_guard<std::mutex> guard{mutex_};if(!owns(owner,incarnation)){return 13;}
            if(retired_||!publisher_){return 125;}
            auto reply{association_};reply.operation=wire::command::inspect;
            auto code{native_rmw_metadata::publisher(*publisher_,reply)};if(code){return code;}
            // An identity/QoS change is a real refusal, not permission to update
            // the admitted snapshot or route future traffic to another object.
            if(!wire::same(reply.actual[0],association_.actual[0])||reply.gid_size!=association_.gid_size||reply.gid!=association_.gid){return 13;}
            output=reply;return 0;
        }
        std::uint32_t publish(wire::binding const& owner,std::uint64_t incarnation,std::uint8_t const* bytes,std::size_t count) override
        {
            std::lock_guard<std::mutex> guard{mutex_};if(!owns(owner,incarnation)){return 13;}
            if(retired_||!publisher_){return 125;}
            if((!bytes&&count)||count>limit(capability_,"max-message-bytes")){return count>limit(capability_,"max-message-bytes")?90:22;}
            try
            {
                auto error=proof_->validate(bytes,count);if(error)return error;
                Native value{};error=proof_->root()->decode(bytes,count,&value);if(error)return error;
                // Exactly one actual publication on the retained native object.
                // A transport timeout cannot make this safe to replay.
                publisher_->publish(value);return 0;
            }
            catch(std::bad_alloc const&){return 12;}
            catch(...){return 5;}
        }
        std::uint32_t retire(wire::binding const& owner,std::uint64_t incarnation) override
        {
            std::lock_guard<std::mutex> guard{mutex_};if(!owns(owner,incarnation)){return 13;}
            if(retired_){return 125;}retired_=true;publisher_.reset();notify();return 0;
        }
    };
}
