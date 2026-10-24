/** @file Native model/codec contracts. This does not count as actual Guest/ROS execution. */
#include <platform/ros/cpp/roswasm/roswasm.hpp>
#include <platform/ros/cpp/roswasm/service_client.hpp>
#include <platform/ros/cpp/roswasm/library_profile.hpp>
#include "../../c/mock_backend.h"
#include <cassert>
#include <stdexcept>
using namespace roswasmcpp;
static uint32_t increment(void* user){++*static_cast<unsigned*>(user);return 110;}
static uint32_t throw_call(void*){throw std::runtime_error("expected callback throw");}
struct TypedCall:DeferredCall<int64_t> {unsigned calls;TypedCall():calls(0){}DeferredResult<int64_t> run(){++calls;return DeferredResult<int64_t>(0,INT64_MIN);} };
static void subscription_callback(void* user,msg::String const& value){assert(value.data=="event");++*static_cast<unsigned*>(user);}
int main(){
    assert(library_profile()==ROSWASM_CPP_STANDARD);
    char payload[32];size_t used=99;assert(!library_encode_string("value",5,payload,sizeof(payload),used)&&used==9);
    used=99;assert(library_encode_string("value",5,payload,2,used)==22&&used==99);
    Context context(*model_backend());Node node(context,"stationary");
    Publisher<msg::String> pub;assert(!node.create_publisher(pub,"allowed"));
    msg::String value;value.data="complete";assert(!pub.publish(value)&&model_publish_calls==1);
    Publisher<msg::String> denied;assert(node.create_publisher(denied,"missing")==13);assert(denied.publish(value)==13&&model_publish_calls==1);
    assert(pub.open(node.context(),"allowed")==17);
    wire::bytes bytes;assert(Codec<msg::String>::encode(value,bytes));msg::String next;
    assert(Codec<msg::String>::decode(bytes,next)&&next.data==value.data);bytes.pop_back();next.data="preserved";
    assert(!Codec<msg::String>::decode(bytes,next)&&next.data=="preserved");
    value.data.assign(1,static_cast<char>(0xc0));assert(!Codec<msg::String>::encode(value,bytes));
    msg::Image image;image.sequence=0xfedcba98u;image.seconds=42;image.nanoseconds=27;image.width=2;image.height=1;image.step=6;
    image.frame_id="camera";image.encoding="rgb8";image.bigendian=true;image.data.assign(6,0xa5);
    assert(Codec<msg::Image>::encode(image,bytes));msg::Image restored;assert(Codec<msg::Image>::decode(bytes,restored));
    assert(restored.sequence==image.sequence&&restored.frame_id==image.frame_id&&restored.encoding==image.encoding&&restored.data==image.data&&restored.bigendian&&restored.step==6);
    msg::Transform transform;assert(transform.quaternion[3]==1);transform.sequence=12;transform.parent_frame="parent";transform.child_frame="child";
    transform.translation[0]=1.25;transform.translation[1]=-2.5;assert(Codec<msg::Transform>::encode(transform,bytes));msg::Transform t;
    assert(Codec<msg::Transform>::decode(bytes,t)&&t.translation[1]==-2.5&&t.quaternion[3]==1&&t.parent_frame=="parent");
    int64_t signed_value=INT64_MIN,signed_out=0;bytes=ParameterCodec<int64_t>::encode(signed_value);assert(ParameterCodec<int64_t>::decode(bytes,signed_out)&&signed_out==signed_value);
    std::vector<int64_t> integers;integers.push_back(INT64_MIN);integers.push_back(INT64_MAX);integers.push_back(-1);
    bytes=ParameterCodec<std::vector<int64_t> >::encode(integers);std::vector<int64_t> actual;
    assert(ParameterCodec<std::vector<int64_t> >::decode(bytes,actual)&&actual==integers);bytes.push_back(0);assert(!ParameterCodec<std::vector<int64_t> >::decode(bytes,actual)&&actual==integers);
    BooleanArray booleans;booleans.values.push_back(2);assert(ParameterCodec<BooleanArray>::encode(booleans).empty());
    msg::TypedParameterEvent event;event.seconds=7;event.nanoseconds=8;event.kind=1;event.node="/source";event.name="p.array";
    for(int tag=1;tag<=9;++tag){event.tag=static_cast<msg::ParameterTag::Value>(tag);event.integer_value=INT64_MIN;event.string_value="text";
        event.byte_array_value.assign(3,255);event.bool_array_value.assign(3,1);event.integer_array_value=integers;event.double_array_value.assign(3,-1.5);event.string_array_value.assign(3,"three");
        assert(Codec<msg::TypedParameterEvent>::encode(event,bytes));msg::TypedParameterEvent copy;
        assert(Codec<msg::TypedParameterEvent>::decode(bytes,copy)&&copy.tag==event.tag&&copy.node==event.node&&copy.name==event.name);
        wire::bytes reencoded;assert(Codec<msg::TypedParameterEvent>::encode(copy,reencoded)&&reencoded==bytes);}
    event.kind=3;event.tag=msg::ParameterTag::deletion;assert(Codec<msg::TypedParameterEvent>::encode(event,bytes));
    msg::TypedParameterEvent deleted;assert(Codec<msg::TypedParameterEvent>::decode(bytes,deleted)&&deleted.kind==3&&deleted.tag==msg::ParameterTag::deletion);
    wire::service_response response;response.success=true;response.message="complete";assert(Codec<wire::service_response>::encode(response,bytes));
    assert(Codec<wire::service_response>::decode(bytes,response)&&response.success&&response.message=="complete");
    Subscription<msg::String> subscription(node.context(),"allowed",128);msg::String incoming;incoming.data="event";assert(Codec<msg::String>::encode(incoming,bytes));
    std::memcpy(model_payload,buffer_data(bytes),bytes.size());model_payload_size=bytes.size();unsigned received=0;
#if ROSWASM_CPP_LEGACY
    subscription.on_message(subscription_callback,&received);
#else
    subscription.on_message([&received](msg::String const& v){assert(v.data=="event");++received;});
#endif
    assert(!subscription.poll_once()&&received==1);
    Executor executor(context,4);roswasmc_future raw;roswasmc_future_init(&raw);unsigned calls=0;size_t completed=0;
    assert(!executor.post(raw,increment,&calls)&&calls==0);assert(!executor.spin_some(completed)&&completed==1&&calls==1&&roswasmc_future_take(&raw)==110);
    TypedCall typed;assert(!executor.post(typed));assert(!executor.spin_some(completed)&&typed.calls==1);int64_t result=0;
    assert(!typed.future.take(result)&&result==INT64_MIN&&typed.future.take(result)==9);
    roswasmc_future throwing;roswasmc_future_init(&throwing);roswasmc_callback_group group;roswasmc_callback_group_init(&group,1);
    assert(!executor.post(throwing,throw_call,0,0,&group));bool caught=false;
    try{executor.spin_some(completed);}catch(std::runtime_error const&){caught=true;}
    assert(caught&&group.active==0&&throwing.state==ROSWASMC_FUTURE_CANCELLED&&!throwing.queued);
    assert(!executor.spin_some(completed));
    Capability cap(node.context(),"allowed");model_error=110;assert(cap.close()==110&&!cap.handle());unsigned cleanup=model_close_calls;model_error=0;
    assert(cap.close()==9&&model_close_calls==cleanup);
    context.shutdown();Publisher<msg::String> stopped;assert(node.create_publisher(stopped,"allowed")!=0);assert(executor.spin_some(completed)==125);
    return 0;
}
