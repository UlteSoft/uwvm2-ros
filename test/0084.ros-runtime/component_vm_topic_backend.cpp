/** @file
 * @brief Real isolated component forwarding one typed native ROS topic record.
 * @details The child receives only two ordered roles and sealed configuration.
 * It publishes the exact borrowed canonical body, acknowledges only the original
 * reservation after known publication acceptance, and then exits. No heap,
 * host capability, path, descriptor or selectable native entry point is used.
 */
#include <uwvm2/imported/ros/scene/component_protocol.h>
#include <uwvm2/imported/ros/scene/robot_provider_response.h>
#include <algorithm>
namespace p=uwvm2::imported::ros::scene::component_protocol;
namespace r=uwvm2::imported::ros::scene::robot_contract;
namespace w=uwvm2::imported::ros::scene::wire;
extern "C" bool uwvm_component_run(p::invocation const& invocation,p::call_function call,void* context)noexcept
{
    if(invocation.count!=2||invocation.configuration.size()!=1||invocation.configuration[0]!=43||
       !invocation.roles[0].publisher||invocation.roles[1].publisher)return false;
    std::array<std::uint8_t,4096> response{};std::size_t size{};
    if(call(context,2,p::action::reserve,{},response,size))return false;
    r::response_decoder in{{response.data(),size},{response.size(),1024,255,0}};
    std::uint64_t endpoint{},generation{},sequence{},nonce{},deadline{},correlation{};r::metadata origin;r::message message;std::string_view kind;
    if(!in.identity(endpoint)||!in.identity(generation)||!in.identity(sequence)||!in.identity(nonce)||!in.deadline(deadline)||
       !in.origin(origin)||!in.number(correlation)||correlation||!in.text(kind)||kind!="topic.message"||!in.body(message)||!in.done()||
       message.type!="std_msgs/msg/String"||message.encoding!="scene-canonical"||message.body.size()>1024)return false;
    std::array<std::uint8_t,1024> retained{};std::copy(message.body.begin(),message.body.end(),retained.begin());auto length=message.body.size();
    std::array<std::uint8_t,24> token;w::writer out{token};out.u64(generation);out.u64(sequence);out.u64(nonce);
    if(call(context,1,p::action::publish,{retained.data(),length},response,size))return false;
    return call(context,2,p::action::acknowledge,token,response,size)==0;
}
