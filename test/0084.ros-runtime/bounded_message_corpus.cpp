/** @file @brief Execute real acquired interface descriptions and independently encoded corpus messages. */
#include <uwvm2/imported/ros/scene/bounded_message.h>
#include <cassert>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
namespace b=uwvm2::imported::ros::scene::bounded_message;
using bytes=uwvm2::utils::container::std_vector<std::uint8_t>;
static bool complete(int fd,std::span<std::uint8_t> output,bool eof=false)
{
    std::size_t offset{};
    while(offset<output.size())
    {auto size=::read(fd,output.data()+offset,output.size()-offset);assert(size>=0);if(!size){assert(eof&&!offset);return false;}offset+=static_cast<std::size_t>(size);}
    return true;
}
static bool read(int fd,bytes& out,std::size_t maximum,bool eof=false)
{
    std::array<std::uint8_t,8> header;if(!complete(fd,header,eof))return false;
    std::uint64_t count{};for(unsigned i{};i<8;++i)count|=std::uint64_t(header[i])<<(i*8);assert(count<=maximum);
    out.resize(static_cast<std::size_t>(count));return complete(fd,out);
}
int main(int argc,char** argv)
{
    assert(argc==2);int fd=::open(argv[1],O_RDONLY|O_CLOEXEC);assert(fd>=0);
    bytes name,description,input,target,output;std::uint64_t cases{},total{};
    while(read(fd,name,1024,true))
    {
        assert(read(fd,description,4u*1024u*1024u)&&read(fd,input,16u*1024u*1024u)&&read(fd,target,16u*1024u*1024u));
        b::schema schema;auto prepared=schema.parse(description);
        if(prepared!=b::status::ok||!schema.validate(input))
        {std::fprintf(stderr,"corpus schema/input failure %.*s status=%u\n",int(name.size()),name.data(),unsigned(prepared));return 1;}
        output.assign(target.size()+8,0xa5);std::size_t required=777;
        auto result=schema.convert(input,b::layout::cdr,false,output,required);
        if(result!=b::status::ok||required!=target.size()||!std::equal(target.begin(),target.end(),output.begin()))
        {std::fprintf(stderr,"corpus conversion failure %.*s status=%u\n",int(name.size()),name.data(),unsigned(result));return 1;}
        assert(std::all_of(output.begin()+required,output.end(),[](auto value){return value==0xa5;}));
        if(!input.empty())assert(!schema.validate(std::span<std::uint8_t const>{input}.first(input.size()-1)));
        input.push_back(0);assert(!schema.validate(input));input.pop_back();
        b::schema incomplete;assert(incomplete.parse(std::span<std::uint8_t const>{description}.first(description.size()-1))!=b::status::ok);
        ++cases;total+=input.size();
    }
    assert(!::close(fd));assert(cases);std::printf("PASS acquired ROS shape corpus: %llu cases, %llu message bytes\n",static_cast<unsigned long long>(cases),static_cast<unsigned long long>(total));
}
