/** @file
 * @brief A real retained Subscription and its same-callback owned receive queue.
 *
 * This is an independent new native endpoint path. It does not call the legacy
 * listen function, infer an entity from a graph name, or grant a Host resource.
 * The future authenticated transport must supply the exact current leased
 * source binding and a nonreusing native incarnation on every operation.
 */
#pragma once
#include "compat/node.h"
#include "native_rmw_builtin.h"
#include "native_rmw_queue.h"
#if __has_include(<rclcpp/message_info.hpp>)
#include <rclcpp/message_info.hpp>
#define UWVM_NATIVE_SUBSCRIPTION_MESSAGE_INFO 1
#else
#define UWVM_NATIVE_SUBSCRIPTION_MESSAGE_INFO 0
#endif

namespace uwvm2::ros_gateway::native_rmw_subscription
{
    namespace wire=::uwvm2::imported::ros::ipc::rmw_endpoint;
    namespace info=::uwvm2::imported::ros::ipc::rmw_metadata;
    namespace queue=native_rmw_queue;
    class endpoint
    {
    public:
        virtual ~endpoint()=default;
        virtual std::uint32_t inspect(wire::binding const&,std::uint64_t,wire::packet&)=0;
        virtual std::uint32_t peek(wire::binding const&,std::uint64_t,std::size_t,queue::snapshot&,
            std::size_t=::uwvm2::imported::ros::ipc::max_payload_bytes)=0;
        virtual std::uint32_t commit(wire::binding const&,std::uint64_t,std::uint64_t,std::uint64_t)=0;
        virtual std::uint32_t cancel(wire::binding const&,std::uint64_t,std::uint64_t,std::uint64_t)=0;
        virtual std::uint32_t observe(wire::binding const&,std::uint64_t,queue::statistics&)=0;
        virtual std::uint32_t retire(wire::binding const&,std::uint64_t)=0;
    };
    /** D4's builtin publisher contract deliberately accepts only topic.publish.
     * Subscription uses its own exact operation/type/native identity check and
     * reuses only its complete canonical payload validation, including Twist
     * policy bounds. The original D4 helper is not edited or relaxed.
     */
    template<class Codec>struct builtin_contract
    {
        using Native=typename Codec::Native;
        static bool known(ros::capability_spec const& cap)
        {return cap.operation=="topic.subscribe"&&cap.type==Codec::type&&ros2_compat::builtin_message<Native>(cap.type,cap.type_identity);}
        static std::uint64_t minimum(ros::capability_spec const&) noexcept{return Codec::minimum_bytes;}
        static bool validate(ros::capability_spec const& cap,std::uint8_t const* p,std::size_t n)
        {return native_rmw_builtin::contract<Codec>::validate(cap,p,n);}
    };
    template<class Codec,class=void>struct variable_extent:std::false_type{};
    template<class Codec>struct variable_extent<Codec,std::void_t<decltype(Codec::encoded_size(
        std::declval<typename Codec::Native const&>(),std::declval<std::size_t&>()))>>:std::true_type{};
    /** Only the fixed original generated codecs use wire_bytes. Variable
     * generated codecs and all four builtins expose their actual encoded_size.
     * No synthetic native type, field projection or schema identity is added.
     */
    template<class Codec>bool size_of(typename Codec::Native const& value,std::size_t maximum,std::size_t& output)
    {
        // Reject obvious native dynamic extents before a full UTF-8/pixel scan
        // or any canonical copy. Complete codec validation still follows.
        if constexpr(std::is_same<Codec,native_rmw_builtin::string_codec>::value)
        {if(maximum<4||value.data.size()>maximum-4){return false;}}
        if constexpr(std::is_same<Codec,native_rmw_builtin::image_codec>::value)
        {
            if(maximum<37||value.header.frame_id.size()>maximum-37||
                value.encoding.size()>maximum-37-value.header.frame_id.size()||
                value.data.size()>maximum-37-value.header.frame_id.size()-value.encoding.size()){return false;}
        }
        if constexpr(variable_extent<Codec>::value)
        {std::size_t n{};if(!Codec::encoded_size(value,n)||n>maximum){return false;}output=n;return true;}
        else
        {if(Codec::wire_bytes>maximum||!Codec::valid(value)){return false;}output=Codec::wire_bytes;return true;}
    }
    inline std::uint32_t queue_options(ros::capability_spec const& cap,queue::options& output)
    {
        queue::options next{};next.records=static_cast<std::size_t>(limit(cap,"qos.depth"));
        next.bytes=static_cast<std::size_t>(limit(cap,"max-queue-bytes"));
        next.body_bytes=static_cast<std::size_t>(limit(cap,"max-message-bytes"));
        auto const& policy{field(cap,"overflow-policy")};
        if(policy=="backpressure"){return 95;} // No native pause/ack protocol exists here.
        if(policy=="drop-oldest"){next.policy=queue::overflow::drop_oldest;}
        else if(policy=="drop-newest"){next.policy=queue::overflow::drop_newest;}
        else if(policy=="reject"){next.policy=queue::overflow::reject;}
        else{return 22;}
        if(!queue::valid(next)||!limit(cap,"burst")||limit(cap,"burst")>next.records||
            limit(cap,"max-bytes-per-second")<next.body_bytes){return 90;}
        output=next;return 0;
    }
    template<class Native,class Codec,class Contract=native_rmw_publisher::custom_contract<Native,Codec>>
    class typed_endpoint final:public endpoint
    {
        struct callback_state
        {
            ros::capability_spec capability;
            wire::binding owner;
            std::uint64_t incarnation{};
            std::size_t maximum{};
            std::shared_ptr<queue::bounded_queue> records;
            bucket quota;
            std::mutex serial;
            callback_state(ros::capability_spec cap,wire::binding binding,std::uint64_t id,
                std::size_t max,std::shared_ptr<queue::bounded_queue> q):
                capability{std::move(cap)},owner{binding},incarnation{id},maximum{max},records{std::move(q)}
            {quota.refresh(capability);}
            void capture(std::shared_ptr<Native const> const& value,rmw_message_info_t const& native) noexcept
            {
                // An actual executor may dispatch callbacks concurrently. One
                // serialization lock bounds canonical encoding to one local
                // temporary at a time; it never calls an upper Guest callback.
                std::lock_guard<std::mutex> lock{serial};
                if(records->ingress()){return;}
                if(!value){records->callback_error(22);return;}
                try
                {
                    std::size_t n{};if(!size_of<Codec>(*value,maximum,n)){records->callback_error(90);return;}
                    if(!quota.take(capability,n)){records->callback_error(11);return;}
                    std::vector<std::uint8_t> canonical;
                    if(!Codec::encode(*value,canonical)||canonical.size()!=n||!Contract::validate(capability,canonical.data(),canonical.size()))
                    {records->callback_error(22);return;}
                    info::header h{};h.owner=owner;h.incarnation=incarnation;h.payload_bytes=n;
                    // D4's pure native capture validates a header's nonzero
                    // sequence. This local placeholder is not a receipt: the
                    // queue overwrites it with its sole accepted sequence while
                    // holding its commit mutex, before encoding the record.
                    h.sequence=1;auto code{native_rmw_metadata::topic(native,h)};
                    if(code&&code!=95){records->callback_error(code);return;}
                    // On95 topic() leaves h unchanged: absent native bits and
                    // its capture error stay in this same owned queue record.
                    // On old SDKs code may be0 but times absent; peek still95.
                    (void)records->enqueue(h,canonical,code);
                }
                catch(std::bad_alloc const&){records->callback_error(12);}
                catch(...){records->callback_error(5);}
            }
        };
        ::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr node_;
        wire::packet association_;
        std::shared_ptr<rclcpp::Subscription<Native>> subscription_;
        std::shared_ptr<callback_state> callback_;
        std::mutex mutex_;
        bool retired_{};
        typed_endpoint(::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr node,std::shared_ptr<callback_state> state):node_{std::move(node)},callback_{std::move(state)}{}
        bool owns(wire::binding const& owner,std::uint64_t incarnation) const noexcept
        {return wire::same(owner,association_.owner)&&incarnation==association_.incarnation;}
        void notify() noexcept {try{ros2_compat::notify(node_);}catch(...) {}}
        void close() noexcept
        {callback_->records->retire();subscription_.reset();notify();}
    public:
        /** No caller output changes until all real native association/QoS
         * checks pass. The future registry reserves its slot before calling.
         * Original NodeOptions and actual generated native types are unchanged.
         * Missing actual-QoS getter refuses95 before native creation.
         */
        static std::uint32_t create(::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr const& node,ros::capability_spec const& cap,
            wire::binding const& owner,std::uint64_t incarnation,std::shared_ptr<endpoint>& output,
            wire::packet& observed) noexcept
        {
            static_assert(std::is_same<Native,typename Codec::Native>::value,"codec must cover the exact real generated Message");
            if(!node||!wire::valid(owner)||!incarnation||cap.operation!="topic.subscribe"){return 22;}
            if constexpr(!UWVM_ROS2_HAVE_COMPLETE_RMW_QOS||!native_rmw_metadata::has_entity_actual<rclcpp::Subscription<Native>>::value){return 95;}
            else
            {
                try
                {
                    if(!Contract::known(cap)){return 13;}
                    queue::options options{};auto error{queue_options(cap,options)};if(error){return error;}
                    if(Contract::minimum(cap)>options.body_bytes){return 90;}
                    std::shared_ptr<queue::bounded_queue> records;
                    error=queue::bounded_queue::create(options,owner,incarnation,records);if(error){return error;}
                    auto state{std::make_shared<callback_state>(cap,owner,incarnation,options.body_bytes,records)};
                    auto holder{std::shared_ptr<typed_endpoint>{new typed_endpoint{node,state}}};
                    auto configured{ros2_qos::configured(cap)};
                    std::weak_ptr<callback_state> weak{state};
#if UWVM_NATIVE_SUBSCRIPTION_MESSAGE_INFO
                    auto callback=[weak](std::shared_ptr<Native const> value,rclcpp::MessageInfo const& information)
                    {if(auto retained=weak.lock()){retained->capture(value,information.get_rmw_message_info());}};
#else
                    auto callback=[weak](std::shared_ptr<Native const> value,rmw_message_info_t const& information)
                    {if(auto retained=weak.lock()){retained->capture(value,information);}};
#endif
                    // Both signatures are actual original SDK APIs, selected
                    // by the actual original header. No MessageInfo substitute
                    // or reflection of a distribution name is used.
                    auto created{ros2_compat::create_subscription<Native>(node,cap.resource,configured,std::move(callback))};
                    if(!created){return 5;}holder->subscription_=std::move(created);
                    auto actual_name{holder->subscription_->get_topic_name()};
                    if(!actual_name||std::string_view{actual_name}!=cap.resource){return 13;}
                    wire::packet reply{};reply.kind=wire::role::subscription;reply.operation=wire::command::activate;
                    reply.owner=owner;reply.incarnation=incarnation;
                    error=native_rmw_metadata::subscription(*holder->subscription_,reply);if(error){return error;}
                    wire::qos requested{};
                    if(!native_rmw_metadata::actual_profile(ros2_compat::profile(configured),requested)||!wire::same(requested,reply.actual[0])){return 95;}
                    holder->association_=reply;error=records->activate();if(error){return error;}
                    observed=reply;output=std::move(holder);return 0;
                }
                catch(std::bad_alloc const&){return 12;}catch(...){return 5;}
            }
        }
        ~typed_endpoint() override {close();}
        std::uint32_t inspect(wire::binding const& owner,std::uint64_t incarnation,wire::packet& output) override
        {
            std::lock_guard<std::mutex> lock{mutex_};if(!owns(owner,incarnation)){return 13;}
            if(retired_||!subscription_){return 125;}auto reply{association_};reply.operation=wire::command::inspect;
            auto error{native_rmw_metadata::subscription(*subscription_,reply)};if(error){return error;}
            if(!wire::same(reply.actual[0],association_.actual[0])){return 13;}output=reply;return 0;
        }
        std::uint32_t peek(wire::binding const& owner,std::uint64_t incarnation,std::size_t gid_bytes,queue::snapshot& output,
            std::size_t capacity=::uwvm2::imported::ros::ipc::max_payload_bytes) override
        {
            std::lock_guard<std::mutex> lock{mutex_};if(!owns(owner,incarnation)){return 13;}
            if(retired_){return 125;}return callback_->records->peek(owner,incarnation,gid_bytes,output,capacity);
        }
        std::uint32_t commit(wire::binding const& owner,std::uint64_t incarnation,std::uint64_t sequence,std::uint64_t reservation) override
        {
            std::lock_guard<std::mutex> lock{mutex_};if(!owns(owner,incarnation)){return 13;}
            if(retired_){return 125;}return callback_->records->commit(owner,incarnation,sequence,reservation);
        }
        std::uint32_t cancel(wire::binding const& owner,std::uint64_t incarnation,std::uint64_t sequence,std::uint64_t reservation) override
        {
            std::lock_guard<std::mutex> lock{mutex_};if(!owns(owner,incarnation)){return 13;}
            if(retired_){return 125;}return callback_->records->cancel(owner,incarnation,sequence,reservation);
        }
        std::uint32_t observe(wire::binding const& owner,std::uint64_t incarnation,queue::statistics& output) override
        {
            std::lock_guard<std::mutex> lock{mutex_};if(!owns(owner,incarnation)){return 13;}
            output=callback_->records->observe();return 0;
        }
        std::uint32_t retire(wire::binding const& owner,std::uint64_t incarnation) override
        {
            std::lock_guard<std::mutex> lock{mutex_};if(!owns(owner,incarnation)){return 13;}
            if(retired_){return 125;}retired_=true;close();return 0;
        }
    };
    template<class Codec>using builtin=typed_endpoint<typename Codec::Native,Codec,builtin_contract<Codec>>;
}
#undef UWVM_NATIVE_SUBSCRIPTION_MESSAGE_INFO
