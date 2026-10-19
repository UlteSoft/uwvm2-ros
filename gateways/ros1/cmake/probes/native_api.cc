// This is a compile/link contract, not a native behavior test or permission
// grant. It deliberately references the public callback, traits and XML-RPC
// APIs used by the adapter instead of selecting a distro-name threshold.
#include <ros/ros.h>
#include <ros/subscribe_options.h>
#include <ros/serialization.h>
#include <ros/service_traits.h>
#include <std_msgs/String.h>
#include <std_srvs/Trigger.h>
#include <std_srvs/SetBool.h>
#include <sensor_msgs/Image.h>
#include <geometry_msgs/TransformStamped.h>
#include <tf2_msgs/TFMessage.h>
#include <actionlib_tutorials/FibonacciAction.h>
#include <actionlib_msgs/GoalStatusArray.h>
#include <actionlib_msgs/GoalID.h>
#include "compat.h"
#include "tf_compat.h"

class xmlrpc_contract final:public XmlRpc::XmlRpcClient
{
    bool readHeader() override { return XmlRpc::XmlRpcClient::readHeader(); }
    bool readResponse() override { return XmlRpc::XmlRpcClient::readResponse(); }
    bool parseResponse(XmlRpc::XmlRpcValue& value) override
    { return XmlRpc::XmlRpcClient::parseResponse(value); }
public:
    xmlrpc_contract():XmlRpc::XmlRpcClient{"127.0.0.1",1,"/"} {}
    bool complete(XmlRpc::XmlRpcValue& value)
    {
        _contentLength=0;_header.clear();_response.clear();_eof=false;
        _connectionState=IDLE;
        return executeNonBlock("metadata",value)&&executeCheckDone(value)&&!isFault();
    }
};
int main()
{
    using message=std_msgs::String;
    boost::function<void(message::ConstPtr const&)> callback{
        [](message::ConstPtr const&) {}};
    ::ros::SubscribeOptions options;
    options.initByFullCallbackType<message::ConstPtr const&>("/compile_only",1,callback);
    options.allow_concurrent_callbacks=false;
    options.tracked_object=::ros::VoidConstPtr{};
    options.transport_hints=::ros::TransportHints{}.tcp();
    std_srvs::Trigger trigger;
    std_srvs::SetBool set_bool;
    trigger.response.success=1;set_bool.request.data=1;
    auto const bytes=::ros::serialization::serializationLength(set_bool.request);
    auto const trigger_type=::ros::service_traits::DataType<std_srvs::Trigger>::value();
    auto const bool_md5=::ros::service_traits::MD5Sum<std_srvs::SetBool>::value();
    auto const action_md5=::ros::message_traits::MD5Sum<actionlib_tutorials::FibonacciAction>::value();
    xmlrpc_contract client;XmlRpc::XmlRpcValue value;
    return bytes==1&&trigger_type&&bool_md5&&action_md5&&client.complete(value)?0:1;
}
