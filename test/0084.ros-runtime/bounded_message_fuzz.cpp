/** @file @brief Coverage-guided schema/message parser and conversion atomicity invariants. */
#include <uwvm2/imported/ros/scene/bounded_message.h>
#include <cassert>
namespace b=uwvm2::imported::ros::scene::bounded_message;
extern "C" int LLVMFuzzerTestOneInput(std::uint8_t const* data,std::size_t size)
{
    if(size<4)return 0;
    std::uint32_t extent{};for(unsigned i{};i<4;++i)extent|=std::uint32_t(data[i])<<(i*8);
    if(extent>size-4)return 0;
    b::schema schema;
    if(schema.parse({data+4,extent})!=b::status::ok)return 0;
    std::span<std::uint8_t const> input{data+4+extent,size-4-extent};
    uwvm2::utils::container::std_vector<std::uint8_t> output(input.size()+16,0xa5);
    auto accepted=schema.validate(input);std::size_t required=SIZE_MAX;
    auto status=schema.convert(input,schema.encoding(),false,output,required);
    if(accepted)
    {
        assert(status==b::status::ok&&required==input.size());
        assert(schema.validate({output.data(),required}));
        assert(std::all_of(output.begin()+required,output.end(),[](auto value){return value==0xa5;}));
    }
    else
    {
        assert(status!=b::status::ok&&required==SIZE_MAX);
        assert(std::all_of(output.begin(),output.end(),[](auto value){return value==0xa5;}));
    }
    return 0;
}
