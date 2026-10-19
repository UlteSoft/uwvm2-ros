/** @file
 * @brief Prepared pure C++20 tests of typed wrappers and callback/owner lifetimes.
 * The import model is deliberately separate from the native ROS acceptance
 * suite. A successful model run cannot establish DDS or aarch64 behavior.
 */
#include <cassert>
#include <roswasm/roswasmcpp.hpp>
#include "../c/mock_backend.h"
/** @brief Verify typed values, non-replayed errors, moved ownership and bounded executor passes. */
int main() {
    using namespace roswasmcpp;
    Node node{*model_backend(),"model-only"};
    assert(node.label()=="model-only"&&model_publish_calls==0);
    auto denied=node.create_publisher<msg::String>("absent");assert(denied.error()==13);
    auto pub=node.create_publisher<msg::String>("allowed");assert(pub.error()==0);
    assert(pub.publish({"hello"})==0&&model_publish_calls==1);
    auto sub=node.create_subscription<msg::String>("allowed",64);assert(sub.error()==0);
    size_t callbacks{};sub.on_message([&](msg::String const& value){assert(value.data=="event");++callbacks;});
    auto event=wire::string("event");std::copy(event.begin(),event.end(),model_payload);model_payload_size=event.size();
    Executor executor;auto id=executor.add(sub);size_t delivered{};
    model_error=11;assert(executor.spin_some(1,delivered)==0&&delivered==0&&callbacks==0);
    model_error=0;assert(executor.spin_some(1,delivered)==0&&delivered==1&&callbacks==1);
    model_malformed_success=1;assert(executor.spin_some(1,delivered)==22&&delivered==0&&callbacks==1);
    model_malformed_success=2;assert(executor.spin_some(1,delivered)==22&&delivered==0&&callbacks==1);
    model_malformed_success=0;
    executor.remove(id);assert(executor.spin_some(1,delivered)==0&&delivered==0);
    size_t busy_registration{};uint32_t nested_error{};size_t nested_delivered=77;
    sub.on_message([&](msg::String const&){
        ++callbacks;assert(executor.add(sub)==0);executor.remove(busy_registration);executor.clear();
        nested_error=executor.spin_some(1,nested_delivered);
    });
    busy_registration=executor.add(sub);assert(busy_registration!=0);
    assert(executor.spin_some(1,delivered)==0&&delivered==1&&callbacks==2&&nested_error==22&&nested_delivered==77);
    /* Callback removal/clear were safely ignored; apply requested removal after the pass. */
    executor.remove(busy_registration);assert(executor.spin_some(1,delivered)==0&&delivered==0);
    wire::bytes nul_encoded;msg::String nul_string{std::string{"a\0b",3}},nul_decoded;
    assert(Codec<msg::String>::encode(nul_string,nul_encoded)&&nul_encoded==wire::bytes({3,0,0,0,'a',0,'b'}));
    assert(Codec<msg::String>::decode(nul_encoded,nul_decoded)&&nul_decoded.data==nul_string.data);
    msg::String original{"keep"};unsigned char malformed[]={4,0,0,0,'a'};
    assert(!Codec<msg::String>::decode(malformed,original)&&original.data=="keep");
    msg::Transform transform;transform.translation[0]=std::numeric_limits<double>::infinity();
    wire::bytes preserved{99};assert(!Codec<msg::Transform>::encode(transform,preserved)&&preserved==wire::bytes{99});
    auto nonfinite_wire=wire::encode(transform);msg::Transform prior;prior.parent_frame="keep";
    assert(!Codec<msg::Transform>::decode(nonfinite_wire,prior)&&prior.parent_frame=="keep");
    std::vector<int64_t> values{INT64_MIN,INT64_MAX},decoded;
    auto array=ParameterCodec<std::vector<int64_t>>::encode(values);
    assert(ParameterCodec<std::vector<int64_t>>::decode(array,decoded)&&decoded==values);
    {Capability owner{node.context(),"allowed"};auto handle=owner.handle();Capability moved{std::move(owner)};
     assert(owner.handle()==0&&moved.handle()==handle);model_error=107;assert(moved.close()==107&&moved.handle()==0);}
    model_error=0;return 0;
}
