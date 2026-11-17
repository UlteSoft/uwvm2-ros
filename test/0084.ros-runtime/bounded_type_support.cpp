/** @file @brief Actual generated ROS metadata and RMW round trips, with fail-closed mutations. */
#include <uwvm2/imported/ros/scene/native/bounded_type_support.h>
#include <cassert>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include "bounded-native-types.inc"
namespace s=uwvm2::imported::ros::scene;
namespace c=s::communication;
namespace b=c::ros2::bounded_types;
namespace n=b::introspection;
using bytes=uwvm2::utils::container::std_vector<std::uint8_t>;
static bytes read(char const* directory,char const* name,char const* suffix)
{
    uwvm2::utils::container::std_string path{directory};path+='/';path+=name;path+=suffix;
    int fd=::open(path.c_str(),O_RDONLY|O_CLOEXEC);assert(fd>=0);bytes result;std::array<std::uint8_t,4096> chunk;
    for(;;){auto count=::read(fd,chunk.data(),chunk.size());assert(count>=0);if(!count)break;result.insert(result.end(),chunk.begin(),chunk.begin()+count);assert(result.size()<=4u*1024u*1024u);}
    assert(::close(fd)==0);return result;
}
static std::size_t cases{},refusals{},roundtrips{};
template<class Native>void run(char const* directory,char const* name)
{
    auto descriptor=read(directory,name,".schema");auto digest=read(directory,name,".hash");assert(digest.size()==33);
    rosidl_type_hash_t hash{};hash.version=digest[0];std::copy(digest.begin()+1,digest.end(),hash.value);
    c::type_plan plan;plan.type_identity=rosidl_generator_traits::name<Native>();plan.encoding="cdr";plan.descriptor=descriptor;
    assert(plan.validate.assign(descriptor)==s::bounded_message::status::ok);
    b::codec<Native> codec;assert(b::codec<Native>::prepare(plan,hash,65536,codec)==c::error::ok);++cases;
    auto metadata=n::get_message_type_support_handle<Native>();auto support=rosidl_typesupport_cpp::get_message_type_support_handle<Native>();
    assert(b::hash_matches(metadata,hash)&&b::hash_matches(support,hash));
    auto corrupted=hash;corrupted.value[0]^=1;assert(b::codec<Native>::prepare(plan,corrupted,65536,codec)==c::error::unsupported);++refusals;
    corrupted={};assert(b::codec<Native>::prepare(plan,corrupted,65536,codec)==c::error::unsupported);++refusals;
    assert(b::codec<Native>::prepare(plan,hash,0,codec)==c::error::unsupported);++refusals;
    assert(b::codec<Native>::prepare(plan,hash,1,codec)==c::error::unsupported);++refusals;
    auto wrong=plan;wrong.type_identity="wrong_msgs/msg/Type";assert(b::codec<Native>::prepare(wrong,hash,65536,codec)==c::error::unsupported);++refusals;
    wrong=plan;wrong.descriptor.push_back(0);assert(b::codec<Native>::prepare(wrong,hash,65536,codec)==c::error::unsupported);++refusals;
    wrong=plan;wrong.validate=+[](s::wire::bytes)noexcept{return true;};assert(b::codec<Native>::prepare(wrong,hash,65536,codec)==c::error::unsupported);++refusals;
    auto native=*b::members(metadata);auto altered=*metadata;altered.data=&native;b::shape_check check;
    native.message_name_="Spoof";assert(check.matches(*plan.validate.bounded(),plan.type_identity,&altered,65536)==c::error::unsupported);++refusals;
    native=*b::members(metadata);
    if(native.member_count_)
    {
        uwvm2::utils::container::std_vector<n::MessageMember> fields(native.members_,native.members_+native.member_count_);native.members_=fields.data();
        fields[0].name_="incorrect_field";assert(check.matches(*plan.validate.bounded(),plan.type_identity,&altered,65536)==c::error::unsupported);++refusals;
        fields[0]=b::members(metadata)->members_[0];fields[0].type_id_=n::ROS_TYPE_LONG_DOUBLE;
        assert(check.matches(*plan.validate.bounded(),plan.type_identity,&altered,65536)==c::error::unsupported);++refusals;
        fields[0]=b::members(metadata)->members_[0];fields[0].is_array_=!fields[0].is_array_;
        assert(check.matches(*plan.validate.bounded(),plan.type_identity,&altered,65536)==c::error::unsupported);++refusals;
        for(std::size_t index{};index<fields.size();++index)
        {
            fields.assign(b::members(metadata)->members_,b::members(metadata)->members_+native.member_count_);native.members_=fields.data();
            auto& field=fields[index];bool changed{};
            if(field.is_array_){field.array_size_=1;field.is_upper_bound_=true;changed=true;}
            else if(field.type_id_==n::ROS_TYPE_STRING||field.type_id_==n::ROS_TYPE_WSTRING){field.string_upper_bound_=1;changed=true;}
            if(changed){assert(check.matches(*plan.validate.bounded(),plan.type_identity,&altered,65536)==c::error::unsupported);++refusals;}
        }
    }
    for(auto suffix:{".little",".big"})
    {
        auto message=read(directory,name,suffix);Native value{},oracle{};b::fixed_buffer borrowed;rmw_serialized_message_t view{};
        view.buffer=message.data();view.buffer_length=view.buffer_capacity=message.size();view.allocator=borrowed.allocator();
        assert(rmw_deserialize(&view,support,&oracle)==RMW_RET_OK&&!borrowed.refused);
        assert(codec.decode(message,value)&&value==oracle);++roundtrips;
        auto previous=value;
        for(std::size_t size{};size<message.size();++size){assert(!codec.decode({message.data(),size},value)&&value==previous);++refusals;}
        auto trailing=message;trailing.push_back(0);assert(!codec.decode(trailing,value)&&value==previous);++refusals;
        bytes encoded(65536,0),reference(65536,0);std::size_t size=SIZE_MAX;
        assert(codec.encode(value,encoded,size)&&size!=SIZE_MAX);Native decoded{};assert(codec.decode({encoded.data(),size},decoded)&&decoded==value);++roundtrips;
        b::fixed_buffer fixed;rmw_serialized_message_t native_wire{};native_wire.buffer=reference.data();native_wire.buffer_capacity=reference.size();native_wire.allocator=fixed.allocator();
        assert(rmw_serialize(&value,support,&native_wire)==RMW_RET_OK&&!fixed.refused&&native_wire.buffer_length>=size&&native_wire.buffer_length-size<=3);
        assert(std::equal(encoded.begin(),encoded.begin()+size,reference.begin()));
        bytes short_buffer(size+16,0xa5);std::size_t unchanged=1234;
        assert(!codec.encode(value,{short_buffer.data(),size-1},unchanged)&&unchanged==1234);
        assert(std::all_of(short_buffer.begin()+size-1,short_buffer.end(),[](auto byte){return byte==0xa5;}));++refusals;
    }
}
int main(int argc,char** argv)
{
    assert(argc==2);auto directory=argv[1];
#define UWVM_BOUNDED_TYPE_INSTANTIATIONS
#include "bounded-native-types.inc"
#undef UWVM_BOUNDED_TYPE_INSTANTIATIONS
    // A sensor's permitted nonfinite values are an explicit schema choice.
    auto descriptor=read(directory,"std_msgs_msg_Float64",".schema");auto digest=read(directory,"std_msgs_msg_Float64",".hash");
    c::type_plan plan;plan.type_identity="std_msgs/msg/Float64";plan.encoding="cdr";plan.descriptor=descriptor;assert(plan.validate.assign(descriptor)==s::bounded_message::status::ok);
    rosidl_type_hash_t hash{};hash.version=digest[0];std::copy(digest.begin()+1,digest.end(),hash.value);b::codec<std_msgs::msg::Float64> finite;
    assert(b::codec<std_msgs::msg::Float64>::prepare(plan,hash,65536,finite)==c::error::ok);
    std_msgs::msg::Float64 value;value.data=std::numeric_limits<double>::infinity();std::array<std::uint8_t,32> output{};std::size_t size=777;
    assert(!finite.encode(value,output,size)&&size==777);++refusals;
    std::printf("PASS bounded native type support: %zu types, %zu roundtrips, %zu refusals\n",cases,roundtrips,refusals);
}
