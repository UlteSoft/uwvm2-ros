#include "../../../../../sdk/origin/common/codec.hpp"
#include "../../../../../sdk/origin/ros1/abi.hpp"
#include "../../../../../sdk/origin/ros2/abi.hpp"
#include "../../../../../sdk/origin/ros1/freestanding.hpp"
#include "../../../../../sdk/origin/ros2/freestanding.hpp"
#include <uwvm2/imported/ros/ipc/image.h>
#include <uwvm2/imported/ros/ipc/transform.h>
#include <uwvm2/imported/ros/ipc/action.h>
#include <uwvm2/imported/ros/ipc/parameter_event.h>
#include <uwvm2/imported/ros/ipc/parameter.h>
#include <cassert>
#include <algorithm>
#include <fast_io.h>

int main()
{
    namespace ipc=uwvm2::imported::ros::ipc;
    for(auto number:{std::numeric_limits<std::int32_t>::min(),std::int32_t{-1},std::int32_t{0},std::numeric_limits<std::int32_t>::max()})
    {
        auto bytes{uwvm_ros::parameter_int32(number)};std::int32_t output{};assert(uwvm_ros::decode_parameter_int32(bytes,output)&&output==number);
        ipc::parameter_value native;assert(!ipc::decode_parameter(ipc::parameter_kind::int32,bytes.data(),bytes.size(),native)&&native.integer_value==number);
        std::vector<std::uint8_t> encoded;assert(!ipc::encode_parameter(native,encoded)&&encoded==bytes);
        bytes.push_back(0);output=42;assert(!uwvm_ros::decode_parameter_int32(bytes,output)&&output==42);
    }
    for(auto number:{std::numeric_limits<std::int64_t>::min(),std::int64_t{-1},std::int64_t{9007199254740993LL},std::numeric_limits<std::int64_t>::max()})
    {
        auto bytes{uwvm_ros::parameter_int64(number)};std::int64_t output{};assert(uwvm_ros::decode_parameter_int64(bytes,output)&&output==number);
        ipc::parameter_value native;assert(!ipc::decode_parameter(ipc::parameter_kind::int64,bytes.data(),bytes.size(),native)&&native.integer_value==number);
        std::vector<std::uint8_t> encoded;assert(!ipc::encode_parameter(native,encoded)&&encoded==bytes);
        bytes.pop_back();output=42;assert(!uwvm_ros::decode_parameter_int64(bytes,output)&&output==42);
    }
    for(auto number:{-0.0,std::nextafter(1.,2.),std::numeric_limits<double>::min(),std::numeric_limits<double>::max()})
    {
        auto bytes{uwvm_ros::parameter_float64(number)};double output{};assert(uwvm_ros::decode_parameter_float64(bytes,output)&&std::bit_cast<std::uint64_t>(output)==std::bit_cast<std::uint64_t>(number));
        ipc::parameter_value native;assert(!ipc::decode_parameter(ipc::parameter_kind::float64,bytes.data(),bytes.size(),native)&&std::bit_cast<std::uint64_t>(native.double_value)==std::bit_cast<std::uint64_t>(number));
        std::vector<std::uint8_t> encoded;assert(!ipc::encode_parameter(native,encoded)&&encoded==bytes);
    }
    for(auto number:{std::numeric_limits<double>::infinity(),-std::numeric_limits<double>::infinity(),std::numeric_limits<double>::quiet_NaN()})
    {
        assert(uwvm_ros::parameter_float64(number).empty());uwvm_ros::bytes bytes;uwvm_ros::append_u64(bytes,std::bit_cast<std::uint64_t>(number));double output{42};
        assert(!uwvm_ros::decode_parameter_float64(bytes,output)&&output==42);ipc::parameter_value native;native.double_value=42;
        assert(ipc::decode_parameter(ipc::parameter_kind::float64,bytes.data(),bytes.size(),native)==22&&native.double_value==42);
    }
    bool boolean{true};assert(!uwvm_ros::decode_parameter_bool(uwvm_ros::bytes{2},boolean)&&boolean);
    unsigned array_cases{};
    auto array_roundtrip=[&](auto const& values,auto encode,auto decode,ipc::parameter_kind type,auto member) {
        uwvm_ros::bytes wire{7};assert(encode(values,wire));
        using vector_type=std::remove_cvref_t<decltype(values)>;
        vector_type output;assert(decode(wire,output)&&output==values);
        ipc::parameter_value native;assert(!ipc::decode_parameter(type,wire.data(),wire.size(),native)&&(native.*member)==values);
        std::vector<std::uint8_t> copied;assert(!ipc::encode_parameter(native,copied)&&copied==wire);
        uwvm_ros::bytes sdk_copy;assert(encode(output,sdk_copy)&&sdk_copy==wire);++array_cases;
        auto malformed=[&](uwvm_ros::bytes const& bytes,unsigned error=22u) {
            output=values;assert(!decode(bytes,output)&&output==values);
            native.type=type;native.*member=values;
            assert(ipc::decode_parameter(type,bytes.data(),bytes.size(),native)==error&&(native.*member)==values);++array_cases;
        };
        auto bad{wire};bad.push_back(0);malformed(bad);bad=wire;bad.pop_back();malformed(bad);
        malformed({255,255,255,255});
        if(!values.empty()) {
            native.*member=values;assert(ipc::decode_parameter(type,wire.data(),wire.size(),native,0)==90&&(native.*member)==values);++array_cases;
        }
    };
    for(auto const& values:{std::vector<std::uint8_t>{},std::vector<std::uint8_t>{0,1}})
        array_roundtrip(values,uwvm_ros::encode_parameter_bool_array,uwvm_ros::decode_parameter_bool_array,ipc::parameter_kind::boolean_array,&ipc::parameter_value::bool_array_value);
    for(auto const& values:{std::vector<std::uint8_t>{},std::vector<std::uint8_t>{0,255}})
        array_roundtrip(values,uwvm_ros::encode_parameter_byte_array,uwvm_ros::decode_parameter_byte_array,ipc::parameter_kind::byte_array,&ipc::parameter_value::byte_array_value);
    for(auto const& values:{std::vector<std::int32_t>{},std::vector<std::int32_t>{std::numeric_limits<std::int32_t>::min(),-7,std::numeric_limits<std::int32_t>::max()}})
        array_roundtrip(values,uwvm_ros::encode_parameter_int32_array,uwvm_ros::decode_parameter_int32_array,ipc::parameter_kind::int32_array,&ipc::parameter_value::int32_array_value);
    for(auto const& values:{std::vector<std::int64_t>{},std::vector<std::int64_t>{std::numeric_limits<std::int64_t>::min(),9007199254740993LL,std::numeric_limits<std::int64_t>::max()}})
        array_roundtrip(values,uwvm_ros::encode_parameter_int64_array,uwvm_ros::decode_parameter_int64_array,ipc::parameter_kind::int64_array,&ipc::parameter_value::int64_array_value);
    for(auto const& values:{std::vector<double>{},std::vector<double>{-0.,std::nextafter(1.,2.),std::numeric_limits<double>::min(),std::numeric_limits<double>::max()}})
        array_roundtrip(values,uwvm_ros::encode_parameter_float64_array,uwvm_ros::decode_parameter_float64_array,ipc::parameter_kind::float64_array,&ipc::parameter_value::double_array_value);
    for(auto const& values:{std::vector<std::string>{},std::vector<std::string>{"","hello<&>","你好\nline",std::string{"\1",1}}})
        array_roundtrip(values,uwvm_ros::encode_parameter_string_array,uwvm_ros::decode_parameter_string_array,ipc::parameter_kind::string_array,&ipc::parameter_value::string_array_value);
    {
        uwvm_ros::bytes sentinel{7};std::vector<std::uint8_t> values{0,2};
        assert(!uwvm_ros::encode_parameter_bool_array(values,sentinel)&&sentinel==uwvm_ros::bytes{7});++array_cases;
        auto wire{uwvm_ros::parameter_byte_array(values)};std::vector<std::uint8_t> output{42};
        assert(!uwvm_ros::decode_parameter_bool_array(wire,output)&&output==std::vector<std::uint8_t>{42});++array_cases;
        values.assign(1048577,0);sentinel={7};
        assert(!uwvm_ros::encode_parameter_byte_array(values,sentinel)&&sentinel==uwvm_ros::bytes{7});++array_cases;
        wire={1,0,16,0};wire.insert(wire.end(),values.begin(),values.end());
        assert(!uwvm_ros::decode_parameter_byte_array(wire,output)&&output==std::vector<std::uint8_t>{42});++array_cases;
    }
    for(auto number:{std::numeric_limits<double>::infinity(),-std::numeric_limits<double>::infinity(),std::numeric_limits<double>::quiet_NaN()}) {
        uwvm_ros::bytes sentinel{7};std::vector<double> values{1.,number};
        assert(!uwvm_ros::encode_parameter_float64_array(values,sentinel)&&sentinel==uwvm_ros::bytes{7});++array_cases;
        auto wire{uwvm_ros::parameter_float64_array(std::vector<double>{1.,2.})};
        auto bits{std::bit_cast<std::uint64_t>(number)};for(unsigned i{};i!=8;++i) wire[12+i]=static_cast<unsigned char>(bits>>(8*i));
        std::vector<double> output{42};assert(!uwvm_ros::decode_parameter_float64_array(wire,output)&&output==std::vector<double>{42});++array_cases;
    }
    for(auto const& text:{std::string{"\0",1},std::string{"\xc0\x80",2},std::string{"\xed\xa0\x80",3},std::string{"\xf4\x90\x80\x80",4},std::string{"\xe4\xbd",2}}) {
        uwvm_ros::bytes sentinel{7};std::vector<std::string> values{text};
        assert(!uwvm_ros::encode_parameter_string_array(values,sentinel)&&sentinel==uwvm_ros::bytes{7});++array_cases;
        uwvm_ros::bytes wire;uwvm_ros::append_u32(wire,1);uwvm_ros::append_string(wire,text);
        std::vector<std::string> output{"sentinel"};assert(!uwvm_ros::decode_parameter_string_array(wire,output)&&output==std::vector<std::string>{"sentinel"});++array_cases;
    }
    assert(ipc::parameter_array_string_valid(uwvm2::imported::ros::version::ros2,std::string{"\1",1}));
    assert(!ipc::parameter_array_string_valid(uwvm2::imported::ros::version::ros1,std::string{"\1",1}));
    fast_io::println("PASS parameter array SDK codec cases ",array_cases);
    auto string=uwvm_ros::string("hello");
    assert((string==uwvm_ros::bytes{5,0,0,0,'h','e','l','l','o'}));
    std::string decoded; assert(uwvm_ros::decode_string(string,decoded)&&decoded=="hello");
    string[0]=255; assert(!uwvm_ros::decode_string(string,decoded));
    auto twist=uwvm_ros::twist({1.25,-2,3.5,0.1,0.2,0.3});
    assert(twist.size()==48&&twist[6]==0xf4&&twist[7]==0x3f);
    std::array<double,6> values{}; assert(uwvm_ros::decode_twist(twist,values)&&values[0]==1.25&&values[1]==-2);
    twist.pop_back(); assert(!uwvm_ros::decode_twist(twist,values));
    uwvm_ros::image guest_image{7,123,456,2,2,6,"camera_frame","rgb8",false,{0,1,2,3,4,5,6,7,8,9,10,11}};
    auto image=uwvm_ros::encode(guest_image); ipc::image gateway_image;
    assert(ipc::decode_image(image,1,gateway_image)&&gateway_image.sequence==7&&gateway_image.data==guest_image.data);
    // Admission borrows this exact packet. Keep it alive while checking that
    // both text and pixels refer to the original storage, rather than a hidden
    // owning copy. Failed length/stride checks must leave the caller's previous
    // view unchanged, so an invalid packet cannot replace an admitted view.
    ipc::image_view borrowed;
    assert(ipc::decode_image_view(image.data(),image.size(),1,borrowed));
    assert(borrowed.sequence==7&&borrowed.encoding=="rgb8"&&borrowed.frame_id=="camera_frame");
    assert(borrowed.data==image.data()+image.size()-guest_image.data.size());
    assert(borrowed.data_size==guest_image.data.size()&&
           std::equal(borrowed.data,borrowed.data+borrowed.data_size,guest_image.data.begin()));
    auto const admitted_view=borrowed;
    auto unchanged_view=[&]() {
        return borrowed.sequence==admitted_view.sequence&&borrowed.seconds==admitted_view.seconds&&
            borrowed.nanoseconds==admitted_view.nanoseconds&&borrowed.height==admitted_view.height&&
            borrowed.width==admitted_view.width&&borrowed.step==admitted_view.step&&
            borrowed.bigendian==admitted_view.bigendian&&borrowed.data==admitted_view.data&&
            borrowed.data_size==admitted_view.data_size&&borrowed.frame_id.data()==admitted_view.frame_id.data()&&
            borrowed.frame_id.size()==admitted_view.frame_id.size()&&borrowed.encoding.data()==admitted_view.encoding.data()&&
            borrowed.encoding.size()==admitted_view.encoding.size();
    };
    auto bad_image=image;
    auto const pixel_offset=static_cast<std::size_t>(borrowed.data-image.data());
    bad_image[pixel_offset-4]=11; // Declared pixel count no longer matches the packet.
    assert(!ipc::decode_image_view(bad_image.data(),bad_image.size(),1,borrowed));
    assert(unchanged_view());
    bad_image=image;
    bad_image[pixel_offset-8]=5; // rgb8 width2 requires at least six bytes per row.
    assert(!ipc::decode_image_view(bad_image.data(),bad_image.size(),1,borrowed));
    assert(unchanged_view());
    assert(ipc::encode_image(gateway_image)==image);
    uwvm_ros::image image_output;assert(uwvm_ros::decode(image,image_output)&&image_output.step==6);
    image.pop_back();assert(!uwvm_ros::decode(image,image_output));
    uwvm_ros::transform guest_transform; guest_transform.seconds=123;guest_transform.nanoseconds=456;
    guest_transform.parent_frame="map";guest_transform.child_frame="base_link";guest_transform.translation={1,2,3};
    auto transform=uwvm_ros::encode(guest_transform);ipc::transform_data gateway_transform;
    assert(ipc::decode_transform(transform,gateway_transform)&&gateway_transform.parent=="map"&&gateway_transform.values[6]==1);
    assert(ipc::encode_transform(gateway_transform)==transform);
    uwvm_ros::service_response service{true,"hello"};auto response=uwvm_ros::encode(service);
    assert((response==uwvm_ros::bytes{1,5,0,0,0,'h','e','l','l','o'}));
    auto reply=uwvm_ros::service_reply(42,service);assert(reply.size()==18&&reply[0]==42&&reply[8]==1);
    uwvm_ros::action_sequence action{42,2,{0,1,1,2,3,5}};
    auto action_bytes=uwvm_ros::encode(action);ipc::action_sequence native_action;
    assert(ipc::decode_action_sequence(action_bytes,true,native_action)&&native_action.goal==42&&native_action.kind==2&&native_action.values==action.values);
    assert(ipc::encode_action_sequence(native_action,true)==action_bytes);
    uwvm_ros::action_sequence guest_action;assert(uwvm_ros::decode(action_bytes,guest_action)&&guest_action.values==action.values);
    action_bytes.pop_back();assert(!uwvm_ros::decode(action_bytes,guest_action));
    assert((uwvm_ros::fibonacci_goal(5)==uwvm_ros::bytes{5,0,0,0}));
    auto event_bytes=ipc::encode_parameter_event({123,456,2,"/node","value","hello"});
    uwvm_ros::parameter_event event;assert(uwvm_ros::decode(event_bytes,event)&&event.node=="/node"&&event.value=="hello"&&event.kind==2);
    event_bytes[8]=4;assert(!uwvm_ros::decode(event_bytes,event));
    unsigned typed_cases{};
    auto typed_roundtrip=[&](uwvm_ros::typed_parameter_event const& value) {
        auto bytes{uwvm_ros::encode(value)};assert(!bytes.empty());
        auto width{value.kind==3?0u:value.tag==uwvm_ros::parameter_event_value_tag::boolean?1u:8u};
        assert(bytes.size()==23+value.node.size()+value.name.size()+width&&bytes[9]==2&&bytes[10]==static_cast<unsigned char>(value.tag));
        ipc::typed_parameter_event native;assert(!ipc::decode_typed_parameter_event(bytes,native));
        assert(native.kind==value.kind&&native.node==value.node&&native.name==value.name&&ipc::parameter_event_tag(native.value.type)==static_cast<unsigned char>(value.tag));
        std::vector<std::uint8_t> native_bytes;assert(!ipc::encode_typed_parameter_event(native,native_bytes)&&native_bytes==bytes);
        uwvm_ros::typed_parameter_event output;assert(uwvm_ros::decode(bytes,output)&&output.kind==value.kind&&output.tag==value.tag&&output.node==value.node&&output.name==value.name);
        if(value.kind!=3) {
            if(value.tag==uwvm_ros::parameter_event_value_tag::boolean) { assert(output.bool_value==value.bool_value&&native.value.bool_value==value.bool_value); }
            else if(value.tag==uwvm_ros::parameter_event_value_tag::integer) { assert(output.integer_value==value.integer_value&&native.value.integer_value==value.integer_value); }
            else { assert(std::bit_cast<std::uint64_t>(output.double_value)==std::bit_cast<std::uint64_t>(value.double_value)&&std::bit_cast<std::uint64_t>(native.value.double_value)==std::bit_cast<std::uint64_t>(value.double_value)); }
        }
        ++typed_cases;
    };
    uwvm_ros::typed_parameter_event typed{123,456,1,"/node","nested.value"};
    for(auto kind:{1,2,3}) {
        typed.kind=static_cast<unsigned char>(kind);typed.tag=uwvm_ros::parameter_event_value_tag::boolean;
        for(bool value:{false,true}) { typed.bool_value=value;typed_roundtrip(typed); }
        typed.tag=uwvm_ros::parameter_event_value_tag::integer;
        for(auto value:{std::numeric_limits<std::int64_t>::min(),std::int64_t{-1},std::int64_t{0},std::int64_t{9007199254740993LL},std::numeric_limits<std::int64_t>::max()}) { typed.integer_value=value;typed_roundtrip(typed); }
        typed.tag=uwvm_ros::parameter_event_value_tag::float64;
        for(auto value:{-0.,0.,std::nextafter(1.,2.),std::numeric_limits<double>::min(),std::numeric_limits<double>::max()}) { typed.double_value=value;typed_roundtrip(typed); }
    }
    typed.kind=2;typed.tag=uwvm_ros::parameter_event_value_tag::boolean;typed.bool_value=true;
    auto good{uwvm_ros::encode(typed)};
    auto typed_invalid=[&](uwvm_ros::bytes const& bytes) {
        uwvm_ros::typed_parameter_event output;output.node="sentinel";output.integer_value=42;
        assert(!uwvm_ros::decode(bytes,output)&&output.node=="sentinel"&&output.integer_value==42);
        ipc::typed_parameter_event native;native.node="sentinel";native.value.integer_value=42;
        assert(ipc::decode_typed_parameter_event(bytes,native)==22&&native.node=="sentinel"&&native.value.integer_value==42);++typed_cases;
    };
    for(auto index:{8u,9u,10u}) { auto bytes{good};bytes[index]=0;typed_invalid(bytes); }
    auto malformed{good};malformed.back()=2;typed_invalid(malformed);malformed=good;malformed.push_back(0);typed_invalid(malformed);malformed=good;malformed.pop_back();typed_invalid(malformed);
    malformed=good;malformed[8]=3;typed_invalid(malformed);malformed=good;malformed[11]=255;malformed[12]=255;malformed[13]=255;malformed[14]=255;typed_invalid(malformed);
    malformed=good;malformed[15]='x';typed_invalid(malformed);malformed=good;malformed[0]=255;malformed[1]=255;malformed[2]=255;malformed[3]=255;typed_invalid(malformed);
    malformed=good;malformed[4]=0;malformed[5]=202;malformed[6]=154;malformed[7]=59;typed_invalid(malformed);
    for(auto number:{std::numeric_limits<double>::infinity(),-std::numeric_limits<double>::infinity(),std::numeric_limits<double>::quiet_NaN()}) {
        typed.tag=uwvm_ros::parameter_event_value_tag::float64;typed.double_value=number;uwvm_ros::bytes sentinel{7};
        assert(!uwvm_ros::encode_typed_parameter_event(typed,sentinel)&&sentinel==uwvm_ros::bytes{7});++typed_cases;
        typed.double_value=1.;malformed=uwvm_ros::encode(typed);auto bits{std::bit_cast<std::uint64_t>(number)};
        for(unsigned i{};i!=8;++i) malformed[malformed.size()-8+i]=static_cast<unsigned char>(bits>>(8*i));typed_invalid(malformed);
    }
    for(auto node:{"/bad//node","/node/","relative","/1node"}) {
        typed.node=node;uwvm_ros::bytes sentinel{7};assert(!uwvm_ros::encode_typed_parameter_event(typed,sentinel)&&sentinel==uwvm_ros::bytes{7});++typed_cases;
    }
    typed.node="/node";
    for(auto name:{"bad..name","name.","1name","name/value"}) {
        typed.name=name;uwvm_ros::bytes sentinel{7};assert(!uwvm_ros::encode_typed_parameter_event(typed,sentinel)&&sentinel==uwvm_ros::bytes{7});++typed_cases;
    }
    auto legacy{ipc::encode_parameter_event({123,456,2,"/node","value","hello"})};uwvm_ros::parameter_event legacy_output;
    assert(uwvm_ros::decode(legacy,legacy_output)&&legacy_output.value=="hello");typed_invalid(legacy);
    fast_io::println("PASS typed ParameterEvent codec cases ",typed_cases);
    fast_io::println("PASS SDK canonical codec interoperability");
}
