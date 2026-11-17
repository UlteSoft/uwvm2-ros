/** @file @brief Native ROS1 serializers and Fast-CDR bytes against the compiled recording registry. */
#include <uwvm2/imported/ros/scene/recording_codecs.h>
#include <ros/serialization.h>
#include <fastcdr/Cdr.h>
#include <fastcdr/FastBuffer.h>
#include "../0017.runtime/fast_io_allocation_probe.h"
#include <cassert>
#include <iostream>
namespace s=uwvm2::imported::ros::scene;
namespace c=s::recording::codecs;
namespace native=eprosima::fastcdr;
static void assert_closed(s::wire::bytes complete,bool(*validate)(s::wire::bytes)noexcept)
{
    auto before=fast_io_allocation_probe::failures;fast_io_allocation_probe::armed=true;
    assert(validate(complete));for(std::size_t n{};n<complete.size();++n)assert(!validate(complete.first(n)));
    std::array<std::uint8_t,4096> extended{};assert(complete.size()<extended.size());
    std::copy(complete.begin(),complete.end(),extended.begin());assert(!validate({extended.data(),complete.size()+1}));
    fast_io_allocation_probe::armed=false;assert(fast_io_allocation_probe::failures==before);
}
template<typename T>static void ros1(T const& value,bool(*validate)(s::wire::bytes)noexcept)
{
    std::array<std::uint8_t,4096> storage{};auto size=ros::serialization::serializationLength(value);assert(size<=storage.size());
    ros::serialization::OStream stream{storage.data(),size};ros::serialization::serialize(stream,value);assert(!stream.getLength());
    assert_closed({storage.data(),size},validate);
    ros::serialization::IStream input{storage.data(),size};T copied;ros::serialization::deserialize(input,copied);assert(copied==value&&!input.getLength());
}
template<typename T>static void ros2(T const& value,bool(*validate)(s::wire::bytes)noexcept)
{
    for(auto endian:{native::Cdr::BIG_ENDIANNESS,native::Cdr::LITTLE_ENDIANNESS})
    {
        std::array<char,4096> storage{};native::FastBuffer buffer{storage.data(),storage.size()};native::Cdr stream{buffer,endian,native::DDS_CDR};
        stream.serialize_encapsulation();stream.serialize(value);auto size=stream.get_serialized_data_length();
        assert_closed({reinterpret_cast<std::uint8_t const*>(storage.data()),size},validate);
        native::FastBuffer read_buffer{storage.data(),size};native::Cdr input{read_buffer};input.read_encapsulation();T copied;input.deserialize(copied);assert(copied==value);
        auto bytes=s::wire::bytes{reinterpret_cast<std::uint8_t const*>(storage.data()),size};
        storage[3]=1;assert(!validate(bytes));storage[3]=0;storage[1]=6;assert(!validate(bytes));
    }
}
static void booleans()
{
    for(bool value:{false,true}){ros1(value,c::ros1_boolean);ros2(value,c::ros2_boolean);}
    std::array<std::uint8_t,5> bad{0,1,0,0,2};assert(!c::ros2_boolean(bad)&&!c::ros1_boolean({bad.data()+4,1}));
}
static void strings()
{
    // Native libraries own test-only std::string objects; VM validation itself does not allocate.
    std::array values{std::string{},std::string{"robot"},std::string{"\xe6\x9c\xba\xe5\x99\xa8\xe4\xba\xba"},
                      std::string{"a\0b",3},std::string(511,'x')};
    for(auto const& value:values)
    {
        ros1(value,c::ros1_string);
        if(value.find('\0')==value.npos)ros2(value,c::ros2_string);
        else
        {
            std::array<char,32> storage{};native::FastBuffer buffer{storage.data(),storage.size()};native::Cdr stream{buffer,native::Cdr::LITTLE_ENDIANNESS,native::DDS_CDR};
            stream.serialize_encapsulation();bool rejected{};
            try{stream.serialize(value);}catch(native::exception::BadParamException const&){rejected=true;}assert(rejected);
            std::array<std::uint8_t,12> embedded{0,1,0,0,4,0,0,0,'a',0,'b',0};assert(!c::ros2_string(embedded));
        }
    }
    std::array<std::uint8_t,10> cdr{0,1,0,0,2,0,0,0,'a',0};assert(c::ros2_string(cdr));
    cdr.back()='b';assert(!c::ros2_string(cdr));cdr.back()=0;cdr[4]=0;assert(!c::ros2_string(cdr));
    cdr[4]=255;cdr[5]=cdr[6]=cdr[7]=255;assert(!c::ros2_string(cdr));
    std::array<std::uint8_t,4> overflow{255,255,255,255};assert(!c::ros1_string(overflow));
}
static void registry_identity()
{
    auto registry=c::registry();assert(registry.size()==4);
    for(std::size_t i{};i<registry.size();++i)
    {
        auto const& value=registry[i];assert(value.validate&&s::identifier(value.name));
        assert(value.definition_sha256==s::buffers::hash(s::recording::mcap::raw(i%2?"string data\n":"bool data\n")));
        assert(value.ros_profile==(i<2?"ros1":"ros2")&&value.message_encoding==(i<2?"ros1":"cdr"));
    }
}
int main(){booleans();strings();registry_identity();std::cout<<"passed 3 native recording codec groups\n";}
