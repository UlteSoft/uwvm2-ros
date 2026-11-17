/** @file @brief Independent native ROS1/Fast-CDR layout and round-trip oracle for bounded schemas. */
#include <uwvm2/imported/ros/scene/bounded_message.h>
#include <ros/serialization.h>
#include <fastcdr/Cdr.h>
#include <fastcdr/FastBuffer.h>
#include <array>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <chrono>
#include <fcntl.h>
#include <unistd.h>
namespace b=uwvm2::imported::ros::scene::bounded_message;
namespace wire=uwvm2::imported::ros::scene::wire;
namespace native=eprosima::fastcdr;
template<typename T>using vector=uwvm2::utils::container::std_vector<T>;
using text=uwvm2::utils::container::std_string;
struct sample
{
    bool flag{},tail{};std::int16_t counter{};std::uint64_t ticks{};
    vector<std::array<double,3>> points; text label; vector<std::uint8_t> image;
    friend bool operator==(sample const&,sample const&)=default;
};
static vector<std::uint8_t> read(char const* directory,char const* name)
{
    text path{directory};path+='/';path+=name;int fd=::open(path.c_str(),O_RDONLY|O_CLOEXEC);assert(fd>=0);
    vector<std::uint8_t> bytes;std::array<std::uint8_t,4096> data;
    for(;;){auto size=::read(fd,data.data(),data.size());assert(size>=0);if(!size)break;bytes.insert(bytes.end(),data.begin(),data.begin()+size);assert(bytes.size()<65536);}assert(!::close(fd));return bytes;
}
static sample make(unsigned index)
{
    sample value;value.flag=index&1;value.tail=index&2;value.counter=static_cast<std::int16_t>(index)-64;value.ticks=UINT64_C(0xfedcba9876543210)+index;
    value.points.resize(index%5);for(std::size_t i{};i<value.points.size();++i)value.points[i]={double(index)*.125,double(i)*-.25,-0.0};
    value.label=index%2?"robot":"\xe6\x9c\xba\xe5\x99\xa8\xe4\xba\xba";
    value.image.resize(index);for(std::size_t i{};i<value.image.size();++i)value.image[i]=static_cast<std::uint8_t>(i+index);
    return value;
}
static std::size_t cdr_write(sample const& value,std::span<std::uint8_t> output,bool little)
{
    native::FastBuffer buffer{reinterpret_cast<char*>(output.data()),output.size()};
    native::Cdr stream{buffer,little?native::Cdr::LITTLE_ENDIANNESS:native::Cdr::BIG_ENDIANNESS,native::XCDRv1};
    stream.serialize_encapsulation();stream<<value.flag<<value.counter<<value.ticks;
    stream<<static_cast<std::uint32_t>(value.points.size());for(auto const& point:value.points)for(double field:point)stream<<field;
    stream.serialize(value.label.c_str());stream<<static_cast<std::uint32_t>(value.image.size());stream.serialize_array(value.image.data(),value.image.size());stream<<value.tail;
    return stream.get_serialized_data_length();
}
static sample cdr_read(std::span<std::uint8_t> input)
{
    native::FastBuffer buffer{reinterpret_cast<char*>(input.data()),input.size()};native::Cdr stream{buffer};stream.read_encapsulation();
    sample value;stream>>value.flag>>value.counter>>value.ticks;std::uint32_t count{};stream>>count;assert(count<=4);value.points.resize(count);
    for(auto& point:value.points)for(auto& field:point){stream>>field;assert(std::isfinite(field));}
    // The independent native decoder owns its temporary string. Production VM
    // validation and conversion use no native object allocations.
    std::string label;stream>>label;assert(label.size()<=32);value.label.assign(label.begin(),label.end());
    stream>>count;assert(count<=4096);value.image.resize(count);stream.deserialize_array(value.image.data(),count);stream>>value.tail;
    assert(stream.get_serialized_data_length()==input.size());return value;
}
static std::size_t ros1_write(sample const& value,std::span<std::uint8_t> output)
{
    ros::serialization::OStream stream{output.data(),static_cast<std::uint32_t>(output.size())};
    stream.next(value.flag);stream.next(value.counter);stream.next(value.ticks);stream.next(static_cast<std::uint32_t>(value.points.size()));
    for(auto const& point:value.points)for(auto field:point)stream.next(field);
    stream.next(value.label);stream.next(value.image);stream.next(value.tail);return output.size()-stream.getLength();
}
static sample ros1_read(std::span<std::uint8_t> input)
{
    ros::serialization::IStream stream{input.data(),static_cast<std::uint32_t>(input.size())};sample value;
    stream.next(value.flag);stream.next(value.counter);stream.next(value.ticks);std::uint32_t count{};stream.next(count);assert(count<=4);value.points.resize(count);
    for(auto& point:value.points)for(auto& field:point)stream.next(field);
    stream.next(value.label);stream.next(value.image);stream.next(value.tail);assert(!stream.getLength());return value;
}
int main(int argc,char** argv)
{
    assert(argc==2);b::schema cdr,packed,wide;
    assert(cdr.parse(read(argv[1],"cdr.bin"))==b::status::ok&&packed.parse(read(argv[1],"ros1.bin"))==b::status::ok&&wide.parse(read(argv[1],"wide.bin"))==b::status::ok);
    std::array<std::uint8_t,65536> native_cdr{},native_packed{},converted{};
    std::size_t cases{};
    for(unsigned index=0;index<257;++index)for(bool little:{false,true})
    {
        auto value=make(index);auto cdr_size=cdr_write(value,native_cdr,little),ros_size=ros1_write(value,native_packed);
        assert(cdr.validate({native_cdr.data(),cdr_size})&&packed.validate({native_packed.data(),ros_size}));
        std::size_t size{};assert(cdr.convert({native_cdr.data(),cdr_size},b::layout::ros1,true,converted,size)==b::status::ok&&size==ros_size);
        assert(std::equal(converted.begin(),converted.begin()+size,native_packed.begin())&&ros1_read({converted.data(),size})==value);
        std::fill(converted.begin(),converted.end(),0);
        assert(packed.convert({native_packed.data(),ros_size},b::layout::cdr,little,converted,size)==b::status::ok&&size==cdr_size);
        assert(std::equal(converted.begin(),converted.begin()+size,native_cdr.begin())&&cdr_read({converted.data(),size})==value);++cases;
        std::fill(native_cdr.begin(),native_cdr.end(),0);
    }
    for(bool little:{false,true})
    {
        std::array<wchar_t,4> text{0x41,0xd83e,0xdd16,0};native::FastBuffer buffer{reinterpret_cast<char*>(native_cdr.data()),native_cdr.size()};
        native::Cdr stream{buffer,little?native::Cdr::LITTLE_ENDIANNESS:native::Cdr::BIG_ENDIANNESS,native::XCDRv1};stream.serialize_encapsulation();stream.serialize(static_cast<wchar_t const*>(text.data()));
        auto size=stream.get_serialized_data_length();assert(wide.validate({native_cdr.data(),size}));std::size_t written{};
        assert(wide.convert({native_cdr.data(),size},b::layout::cdr,!little,converted,written)==b::status::ok&&written==size);
        native::FastBuffer input{reinterpret_cast<char*>(converted.data()),written};native::Cdr decoded{input};decoded.read_encapsulation();std::wstring value;decoded.deserialize(value);
        assert(value.size()==3&&std::equal(value.begin(),value.end(),text.begin()));++cases;
    }
    std::printf("PASS native bounded-message oracle: %zu ROS1/Fast-CDR nested and UTF-16 cases\n",cases);
}
