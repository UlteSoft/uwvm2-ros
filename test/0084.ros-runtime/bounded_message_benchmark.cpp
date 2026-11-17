/** @file
 * @brief Bounded CDR conversion versus an independently compiled Fast-CDR round trip.
 * @details Both paths consume serialized input and produce serialized output in
 * preallocated private storage. Native checked additionally enforces the same
 * finite floating-point rule; raw native does not. Schema preparation, allocation
 * and fixture construction are outside timing. This measures message processing,
 * not endpoint latency, isolation, physical devices or a complete ROS runtime.
 */
#include <uwvm2/imported/ros/scene/bounded_message.h>
#include <fastcdr/Cdr.h>
#include <fastcdr/FastBuffer.h>
#include <cassert>
#include <bit>
#include <chrono>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
namespace b=uwvm2::imported::ros::scene::bounded_message;
namespace native=eprosima::fastcdr;
template<typename T>using vector=uwvm2::utils::container::std_vector<T>;
using text=uwvm2::utils::container::std_string;
using clock_type=std::chrono::steady_clock;
struct fixture
{
    b::schema schema;vector<std::uint8_t> input,output,octets;vector<double> numbers;
    bool numeric{};std::size_t count{},extent{};
};
static vector<std::uint8_t> read(char const* directory,char const* name)
{
    text path{directory};path+='/';path+=name;int fd=::open(path.c_str(),O_RDONLY|O_CLOEXEC);assert(fd>=0);
    vector<std::uint8_t> bytes;std::array<std::uint8_t,4096> data;
    for(;;){auto size=::read(fd,data.data(),data.size());assert(size>=0);if(!size)break;bytes.insert(bytes.end(),data.begin(),data.begin()+size);assert(bytes.size()<65536);}
    assert(!::close(fd));return bytes;
}
static fixture prepare(char const* directory,bool numeric,std::size_t count)
{
    fixture f;f.numeric=numeric;f.count=count;assert(f.schema.parse(read(directory,numeric?"finite.bin":"octets.bin"))==b::status::ok);
    f.input.resize(16+count*(numeric?8:1));f.output.resize(f.input.size());
    native::FastBuffer buffer{reinterpret_cast<char*>(f.input.data()),f.input.size()};
    native::Cdr stream{buffer,native::Cdr::LITTLE_ENDIANNESS,native::XCDRv1};stream.serialize_encapsulation();stream<<static_cast<std::uint32_t>(count);
    if(numeric){f.numbers.resize(count);for(std::size_t i{};i<count;++i)f.numbers[i]=double(i)*.125-8.;stream.serialize_array(f.numbers.data(),count);}
    else{f.octets.resize(count);for(std::size_t i{};i<count;++i)f.octets[i]=static_cast<std::uint8_t>(i);stream.serialize_array(f.octets.data(),count);}
    f.extent=stream.get_serialized_data_length();f.input.resize(f.extent);assert(f.schema.validate(f.input));return f;
}
__attribute__((noinline)) static std::size_t bounded_roundtrip(fixture& f)
{
    std::size_t required{};auto status=f.schema.convert(f.input,b::layout::cdr,true,f.output,required);
    assert(status==b::status::ok&&required==f.extent);return required;
}
__attribute__((noinline)) static std::size_t native_roundtrip(fixture& f,bool checked)
{
    native::FastBuffer in_buffer{reinterpret_cast<char*>(f.input.data()),f.input.size()};native::Cdr in{in_buffer};in.read_encapsulation();
    std::uint32_t count{};in>>count;assert(count==f.count);
    if(f.numeric)
    {
        in.deserialize_array(f.numbers.data(),count);
        if(checked)for(auto const& value:f.numbers)
            assert((std::bit_cast<std::uint64_t>(value)&UINT64_C(0x7ff0000000000000))!=UINT64_C(0x7ff0000000000000));
    }
    else in.deserialize_array(f.octets.data(),count);
    assert(in.get_serialized_data_length()==f.extent);
    native::FastBuffer out_buffer{reinterpret_cast<char*>(f.output.data()),f.output.size()};
    native::Cdr out{out_buffer,native::Cdr::LITTLE_ENDIANNESS,native::XCDRv1};out.serialize_encapsulation();out<<count;
    if(f.numeric)out.serialize_array(f.numbers.data(),count);else out.serialize_array(f.octets.data(),count);
    return out.get_serialized_data_length();
}
static std::uint64_t batch(fixture& f,unsigned mode,std::size_t iterations,std::uint64_t& checksum)
{
    auto begin=clock_type::now();
    for(std::size_t i{};i<iterations;++i)
    {
        // Keep input and output visible to the compiler without adding a fence
        // instruction or an extra input/output copy to either implementation.
        asm volatile("":::"memory");
        auto size=mode==2?bounded_roundtrip(f):native_roundtrip(f,mode==1);
        checksum+=size+f.output[size-1];asm volatile("":::"memory");
    }
    return std::chrono::duration_cast<std::chrono::nanoseconds>(clock_type::now()-begin).count();
}
int main(int argc,char** argv)
{
    assert(argc==3);std::string_view mode{argv[2]};std::uint64_t checksum{};
    if(mode=="profile")
    {
        auto f=prepare(argv[1],true,1024);auto until=clock_type::now()+std::chrono::seconds{5};std::size_t iterations{};
        do{batch(f,2,256,checksum);iterations+=256;}while(clock_type::now()<until);
        std::printf("profile %zu %llu\n",iterations,static_cast<unsigned long long>(checksum));return 0;
    }
    assert(mode=="measure");
    for(bool numeric:{true,false})for(std::size_t count:numeric?std::array<std::size_t,4>{0,6,1024,65536}:std::array<std::size_t,4>{0,256,65536,1048576})
    {
        auto f=prepare(argv[1],numeric,count);
        for(unsigned method{};method<3;++method)
        {
            auto size=method==2?bounded_roundtrip(f):native_roundtrip(f,method==1);
            assert(size==f.extent&&std::equal(f.input.begin(),f.input.end(),f.output.begin()));
            batch(f,method,32,checksum);
        }
        auto iterations=std::clamp<std::size_t>((1u<<20)/f.extent,8,2048);
        for(unsigned sample{};sample<31;++sample)for(unsigned order{};order<3;++order)
        {
            auto method=sample%2?2-order:order;auto elapsed=batch(f,method,iterations,checksum);
            std::printf("%s %zu %u %u %llu %zu %llu\n",numeric?"finite":"octets",count,method,sample,
                static_cast<unsigned long long>(elapsed),iterations,static_cast<unsigned long long>(checksum));
        }
    }
}
