/** @file @brief Decode independently generated canonical transform definitions and re-encode exact bytes. */
#include <uwvm2/imported/ros/scene/transform_policy.h>
#include <uwvm2/imported/ros/scene/configuration_file.h>
#include <cassert>
#include <charconv>
#include <cstdio>
namespace scene=uwvm2::imported::ros::scene;
namespace transform=scene::transforms;
using E=transform::error;
static void limits(transform::configuration& value)
{
    constexpr std::array<std::uint64_t,13> selected{1024,16,128,1000000000,0,1000000000,8,1000000000,1,1u<<30,1u<<30,1u<<30,65536};
    static_assert(selected.size()==transform::limit_names.size());
    for(std::size_t index{};index<selected.size();++index)assert(value.limit(transform::limit_names[index],selected[index])==E::success);
}
int main(int argc,char** argv)
{
    assert(argc==6);std::string_view path=argv[1],identity=argv[2],hash=argv[3],extent=argv[4],expected=argv[5];
    scene::buffers::profile profile;assert(profile.identity.assign(identity)&&hash.size()==64);
    for(std::size_t index{};index<32;++index)
    {unsigned byte{};auto parsed=std::from_chars(hash.data()+index*2,hash.data()+index*2+2,byte,16);assert(parsed.ec==std::errc{}&&parsed.ptr==hash.data()+index*2+2);profile.sha256[index]=static_cast<std::uint8_t>(byte);}
    std::uint64_t size{};auto parsed=std::from_chars(extent.data(),extent.data()+extent.size(),size);assert(parsed.ec==std::errc{}&&parsed.ptr==extent.data()+extent.size());
    scene::byte_buffer input;assert(scene::configuration_file::read(path,size,profile.sha256,transform::maximum_definition_bytes,input)==E::success);
    transform::configuration policy;assert(policy.profile(profile)==E::success);limits(policy);
    auto defined=policy.definition(input.view());auto frozen=defined==E::success?policy.freeze():defined;
    if(expected=="reject")
    {assert(frozen!=E::success&&!policy.frozen()&&!policy.limits());std::puts("passed invalid canonical transform definition refusal");return 0;}
    assert(expected=="accept"&&frozen==E::success&&policy.frozen()&&policy.limits());
    auto const& value=policy.limits()->session.history;scene::byte_buffer copy;copy.resize_for_overwrite(input.view().size());scene::wire::writer output{copy.writable()};
    assert(output.text(transform::definition_magic)&&output.text(identity)&&output.u64(value.frame_count));
    for(std::size_t index{};index<value.frame_count;++index)assert(output.text(value.frames[index].view()));
    assert(output.u64(value.edge_count));
    for(std::size_t index{};index<value.edge_count;++index)
    {auto const& edge=value.edges[index];assert(output.u64(edge.parent)&&output.u64(edge.child)&&output.boolean(edge.dynamic)&&output.boolean(edge.fixed));}
    assert(output.f64(value.geometry.translation)&&output.f64(value.geometry.quaternion_tolerance)&&output.f64(value.linear_speed)&&output.f64(value.angular_speed));
    assert(output.size()==input.view().size()&&std::equal(copy.view().begin(),copy.view().end(),input.view().begin(),input.view().end()));
    assert(scene::buffers::hash(copy.view())==profile.sha256);
    std::puts("passed exact transform definition cross-language roundtrip");
}
