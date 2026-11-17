/** @file @brief Like-for-like actual RMW object round trips with bounded validation overhead. */
#include <uwvm2/imported/ros/scene/native/bounded_type_support.h>
#include <std_msgs/msg/float64_multi_array.hpp>
#include <cassert>
#include <chrono>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
namespace s=uwvm2::imported::ros::scene;
namespace c=s::communication;
namespace b=c::ros2::bounded_types;
using Native=std_msgs::msg::Float64MultiArray;
using bytes=uwvm2::utils::container::std_vector<std::uint8_t>;
static bytes read(char const* directory,char const* suffix)
{
    uwvm2::utils::container::std_string path{directory};path+='/';path+=suffix;
    int fd=::open(path.c_str(),O_RDONLY|O_CLOEXEC);assert(fd>=0);bytes result;std::array<std::uint8_t,4096> data;
    for(;;){auto count=::read(fd,data.data(),data.size());assert(count>=0);if(!count)break;result.insert(result.end(),data.begin(),data.begin()+count);}assert(::close(fd)==0);return result;
}
static std::uint64_t nanos()noexcept
{return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();}
[[gnu::noinline]] std::uint64_t native_roundtrip(s::wire::bytes input,std::span<std::uint8_t> output,rosidl_message_type_support_t const* support,bool finite)
{
    Native value;b::fixed_buffer input_owner,output_owner;rmw_serialized_message_t in{},out{};
    in.buffer=const_cast<std::uint8_t*>(input.data());in.buffer_length=in.buffer_capacity=input.size();in.allocator=input_owner.allocator();
    out.buffer=output.data();out.buffer_capacity=output.size();out.allocator=output_owner.allocator();
    assert(rmw_deserialize(&in,support,&value)==RMW_RET_OK&&!input_owner.refused);
    if(finite)for(double number:value.data)assert((std::bit_cast<std::uint64_t>(number)&0x7ff0000000000000ull)!=0x7ff0000000000000ull);
    assert(rmw_serialize(&value,support,&out)==RMW_RET_OK&&!output_owner.refused);
    return out.buffer_length+output[4]+output[input.size()-1];
}
[[gnu::noinline]] std::uint64_t bounded_roundtrip(s::wire::bytes input,std::span<std::uint8_t> output,b::codec<Native> const& codec)
{
    Native value;std::size_t size{};assert(codec.decode(input,value));assert(codec.encode(value,output,size));
    return size+output[4]+output[input.size()-1];
}
int main(int argc,char** argv)
{
    assert(argc==3);auto description=read(argv[1],"schema.bin"),digest=read(argv[1],"hash.bin");assert(digest.size()==33);
    c::type_plan plan;plan.type_identity="std_msgs/msg/Float64MultiArray";plan.encoding="cdr";plan.descriptor=description;
    assert(plan.validate.assign(description)==s::bounded_message::status::ok);rosidl_type_hash_t hash{};hash.version=digest[0];std::copy(digest.begin()+1,digest.end(),hash.value);
    b::codec<Native> codec;assert(b::codec<Native>::prepare(plan,hash,65536,codec)==c::error::ok);auto support=rosidl_typesupport_cpp::get_message_type_support_handle<Native>();
    bool profile=std::string_view{argv[2]}=="profile";assert(profile||std::string_view{argv[2]}=="measure");
    for(std::size_t count:{6u,1024u,65536u})
    {
        if(profile&&count!=1024)continue;
        Native initial;initial.data.resize(count);for(std::size_t i{};i<count;++i)initial.data[i]=static_cast<double>(i)+0.25;
        bytes input(2u*1024u*1024u,0),output(input.size(),0);std::size_t size{};assert(codec.encode(initial,input,size));input.resize(size);
        assert(native_roundtrip(input,output,support,false)==bounded_roundtrip(input,output,codec));
        if(profile)
        {
            std::uint64_t sum{},end=nanos()+8000000000ull;do{for(unsigned i{};i<4096;++i)sum+=bounded_roundtrip(input,output,codec);}while(nanos()<end);
            std::printf("profile %lu\n",static_cast<unsigned long>(sum));continue;
        }
        auto iterations=count==65536?128u:4096u;
        for(unsigned sample{};sample<31;++sample)for(unsigned step{};step<3;++step)
        {
            auto method=(sample+step)%3;std::uint64_t sum{},start=nanos();
            for(unsigned i{};i<iterations;++i)sum+=method==2?bounded_roundtrip(input,output,codec):native_roundtrip(input,output,support,method==1);
            auto elapsed=nanos()-start;std::printf("%zu %u %u %lu %u %lu\n",count,method,sample,static_cast<unsigned long>(elapsed),iterations,static_cast<unsigned long>(sum));
        }
    }
}
