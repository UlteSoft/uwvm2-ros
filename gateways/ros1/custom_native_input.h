/*************************************************************
 * UWVM2 pre-deserialization custom ROS1 guards. APL-2.0.    *
 *************************************************************/
#pragma once
#include "../common/server.h"
#include <ros/subscribe_options.h>
#include <ros/advertise_service_options.h>
#include <boost/make_shared.hpp>
#include <functional>
#include <limits>
#include <stdexcept>
namespace uwvm2::ros_gateway::ros1_custom_input
{
    using validator=std::function<bool(std::uint8_t const*,std::size_t)>;
    /** Guard before the official helper creates/resizes typed native fields.
     * Noetic's helper SPI is version-specific and must be compiled/qualified
     * against the selected SDK. Incoming middleware transport-buffer allocations
     * precede this SPI and are outside canonical queue accounting; deployment
     * cgroup/RMW/network budgets remain independent required host constraints.
     * Validated ROS1 finite field bytes are still officially deserialized and
     * re-encoded by the generated native codec, never forwarded to a Guest. */
    class subscription final:public ::ros::SubscriptionCallbackHelper
    {
        ::ros::SubscriptionCallbackHelperPtr delegate_;validator validate_;
    public:
        subscription(::ros::SubscriptionCallbackHelperPtr delegate,validator validate):delegate_{std::move(delegate)},validate_{std::move(validate)} {if(!delegate_||!validate_) {throw std::invalid_argument{"custom native input requires an official helper and validator"};}}
        ::ros::VoidConstPtr deserialize(::ros::SubscriptionCallbackHelperDeserializeParams const& params) override
        {
            if(!delegate_||!validate_||!validate_(params.buffer,params.length)) {return {};}
            try {return delegate_->deserialize(params);}catch(std::exception const&) {return {};}
        }
        void call(::ros::SubscriptionCallbackHelperCallParams& params) override {delegate_->call(params);}
        std::type_info const& getTypeInfo() override {return delegate_->getTypeInfo();}
        bool isConst() override {return delegate_->isConst();}bool hasHeader() override {return delegate_->hasHeader();}
    };
    template<class Native,class Callback,class Validator>
    ::ros::Subscriber subscribe(::ros::NodeHandle& node,std::string const& topic,std::uint32_t depth,Callback const& callback,Validator validate)
    {
        ::ros::SubscribeOptions options;options.init<Native>(topic,depth,callback);options.transport_hints=::ros::TransportHints{}.tcp();
        options.helper=boost::make_shared<subscription>(options.helper,validator{std::move(validate)});return node.subscribe(options);
    }
    /** Validate the full serialized service request before allocating native
     * Request/Response objects. Integer address subtraction avoids forming an
     * unchecked pointer range around corrupted helper metadata. */
    class service final:public ::ros::ServiceCallbackHelper
    {
        ::ros::ServiceCallbackHelperPtr delegate_;validator validate_;
    public:
        service(::ros::ServiceCallbackHelperPtr delegate,validator validate):delegate_{std::move(delegate)},validate_{std::move(validate)} {if(!delegate_||!validate_) {throw std::invalid_argument{"custom native input requires an official helper and validator"};}}
        bool call(::ros::ServiceCallbackHelperCallParams& params) override
        {
            auto& request{params.request};auto base{reinterpret_cast<std::uintptr_t>(request.buf.get())},start{reinterpret_cast<std::uintptr_t>(request.message_start)};
            if(!delegate_||!validate_||start<base||start-base>request.num_bytes||request.num_bytes-(start-base)>UINT32_MAX) {return false;}
            auto size{request.num_bytes-(start-base)};
            if((size&&!request.message_start)||!validate_(request.message_start,size)) {return false;}
            // A genuinely empty request still needs a real zero-range address
            // for the official serializer's pointer arithmetic.
            if(!request.buf.get()) {if(request.num_bytes||request.message_start) {return false;}request.buf.reset(new std::uint8_t[1]);request.message_start=request.buf.get();}
            try {return delegate_->call(params);}catch(std::exception const&) {return false;}
        }
    };
    /** Skip only the fixed official Actionlib wrapper. Header strings, status
     * text and native GoalID are separately bounded before any typed allocation.
     * The remaining complete Goal/Feedback/Result component uses its registry
     * grammar. These native wrappers are not canonical Runtime ID envelopes. */
    inline bool action_body(std::uint8_t const* bytes,std::size_t size,bool status,
        ipc::custom_topic_descriptor const& component,std::size_t maximum)
    {
        if(!bytes||maximum>32768||size>maximum+800||size<16||ipc::get(bytes+8,4)>=1000000000) {return false;}std::size_t at{12};
        auto text=[&](bool required){if(size-at<4) {return false;}auto n{ipc::get(bytes+at,4)};at+=4;if(n>256||n>size-at||(required&&!n)||!ipc::custom_wire_utf8(bytes+at,n)) {return false;}at+=n;return true;};
        if(!text(false)||size-at<8||ipc::get(bytes+at+4,4)>=1000000000) {return false;}at+=8;if(!text(true)) {return false;}
        if(status) {if(size-at<1||bytes[at]>9) {return false;}++at;if(!text(false)) {return false;}}
        return size-at<=maximum&&ipc::validate_custom_topic(component,bytes+at,size-at);
    }
    /** The independent cancellation wrapper is stamp8 plus one bounded native
     * GoalID string. Empty IDs retain official cancel-all/timestamp semantics;
     * permission and current owned-goal selection remain adapter decisions. */
    inline bool cancel_body(std::uint8_t const* bytes,std::size_t size)
    {if(!bytes||size<12||size>268||ipc::get(bytes+4,4)>=1000000000) {return false;}auto n{ipc::get(bytes+8,4)};return n==size-12&&n<=256&&ipc::custom_wire_utf8(bytes+12,n);}
}
