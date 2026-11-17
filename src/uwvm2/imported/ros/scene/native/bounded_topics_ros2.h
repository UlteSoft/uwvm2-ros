/** @file
 * @brief Generic generated ROS topic adapters with exact prepared schema approval.
 * @details A trusted native build instantiates its finite set of generated C++
 * types and independently pins each upstream type hash. Preparation is inert:
 * it validates metadata, retains immutable schemas and allocates recoverably,
 * but creates no ROS context, node, topic, thread or transport. Factories are
 * passed only to the isolated worker's native provider. Runtime publication and
 * delivery still use the ordinary complete-origin authority and queue machinery.
 * There is no name-based dynamic loading or public unchecked serialization API.
 */
#pragma once
#include "external_process_only.h"
#include "bounded_type_support.h"
#include "communication_ros2.h"

namespace uwvm2::imported::ros::scene::communication::ros2::bounded_types
{
    /** @brief Carry validated CDR directly through generated typed ROS endpoints.
     * @details Native type/hash approval is identical to the object codec, but
     * steady-state traffic uses rclcpp's serialized publish/subscription API.
     * It avoids constructing and copying generated message objects merely to
     * transmit already serialized data. The input remains immutable until the
     * synchronous publish call returns. Receive queues copy only the validated
     * canonical extent. RMW buffers and middleware caches remain isolated native
     * storage; neither zero-copy nor bounded middleware allocation is claimed.
     */
    template<class Native>class serialized_codec
    {
        codec<Native> checked_;
    public:
        inline static constexpr bool serialized_wire=true;
        static error prepare(type_plan const& schema,rosidl_type_hash_t const& expected,std::uint64_t work,serialized_codec& output)noexcept
        {return codec<Native>::prepare(schema,expected,work,output.checked_);}
        bool native_extent(wire::bytes input,std::size_t& size)const noexcept{return checked_.native_extent(input,size);}
        template<class Publisher>native_result publish(Publisher& publisher,wire::bytes input,std::uint64_t deadline)const noexcept
        {
            if(!checked_.canonical(input))return {error::invalid,effect::none};
            if(deadline<=now())return {error::timeout,effect::none};
            fixed_buffer ownership;rcl_serialized_message_t view{};
            view.buffer=const_cast<std::uint8_t*>(input.data());view.buffer_length=view.buffer_capacity=input.size();view.allocator=ownership.allocator();
            try
            {
                publisher.publish(view);
                if(ownership.refused)return {error::io,effect::unknown};
                return {deadline<=now()?error::timeout:error::ok,effect::locally_accepted};
            }
            catch(...){return {error::io,effect::unknown};}
        }
    };
    template<class Native>class topic_factory final:public bounded_topic_factory
    {
        type_plan schema_;
        serialized_codec<Native> codec_;
    public:
        topic_factory(type_plan schema,serialized_codec<Native> prepared):schema_(std::move(schema)),codec_(std::move(prepared)){}
        bool matches(type_plan const& candidate)const noexcept override
        {
            return candidate.selected==schema_.selected&&candidate.type_identity==schema_.type_identity&&candidate.encoding==schema_.encoding&&
                candidate.validate==schema_.validate&&candidate.descriptor==schema_.descriptor;
        }
        error create(topic_plan const& plan,std::shared_ptr<node> parent,handle id,
                     std::shared_ptr<receive_queue> const& queue,std::shared_ptr<native_topic>& output)const noexcept override
        {
            if(!parent||!parent->parent||!parent->parent->live()||plan.type!=schema_.selected||!plan.max_message_bytes||
                plan.max_message_bytes>schema_.validate.bounded()->bounds().message_bytes)return error::denied;
            return topic<Native,serialized_codec<Native>>::create_bound(plan,std::move(parent),id,queue,codec_,output);
        }
    };
    /** @brief Prepare one exact-schema factory; an existing output survives failure.
     * @details Expected type hash and comparison budget come from trusted native
     * deployment, never from guest pointers. This reserves owned schema/factory
     * state with fast_io but does not admit an endpoint or grant an operation.
     */
    template<class Native>error prepare_topic(type_plan const& schema,rosidl_type_hash_t const& expected,std::uint64_t work,
                                             std::shared_ptr<bounded_topic_factory>& output)noexcept
    {
        serialized_codec<Native> prepared;auto status=serialized_codec<Native>::prepare(schema,expected,work,prepared);if(status!=error::ok)return status;
        try
        {auto result=::uwvm2::utils::container::make_shared<topic_factory<Native>>(schema,std::move(prepared));output=std::move(result);return error::ok;}
        catch(std::bad_alloc const&){return error::no_memory;}catch(...){return error::io;}
    }
}
