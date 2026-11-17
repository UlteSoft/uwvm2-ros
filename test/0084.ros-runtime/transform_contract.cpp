/** @file @brief Canonical TF framing, exact time and bounded request rejection. */
#include <uwvm2/imported/ros/scene/transform_contract.h>
#include <cassert>
#include <iostream>
namespace c=uwvm2::imported::ros::transform_contract;
namespace s=uwvm2::imported::ros::scene;
static void query(s::wire::writer& out,bool zero=true)
{
    assert(out.text("map")&&out.boolean(zero));if(zero)assert(out.u64(1)&&out.u64(0)&&out.i64(0));
    assert(out.text("sensor")&&out.boolean(false)&&out.boolean(false));
}
static c::transform value()
{c::transform t;assert(t.parent.assign("map")&&t.child.assign("sensor"));t.time={1,0,-10};t.pose.translation={1,2,3};return t;}
static auto request(c::operation action)
{
    std::array<std::uint8_t,4096> bytes{};s::wire::writer out{bytes};
    auto key=[&]{assert(out.u64(2)&&out.u64(1));};
    switch(action)
    {
    case c::operation::open:
        {std::array<std::uint8_t,32> digest{};assert(out.text("frames")&&out.blob(digest)&&out.u64(1)&&out.u64(3));break;}
    case c::operation::publish:key();assert(out.u64(2)&&out.u64(0)&&out.u64(1)&&c::write_transform(out,value()));break;
    case c::operation::lookup:case c::operation::available:assert(out.u64(2));query(out);break;
    case c::operation::velocity:
        assert(out.u64(2)&&out.text("sensor")&&out.text("map")&&out.text("base")&&c::write_vector(out,{1,2,3})&&
               out.text("base")&&c::write_stamp(out,{1,0,0})&&out.u64(1000000000));break;
    case c::operation::submit:key();assert(out.u64(2));query(out);assert(out.u64(10));break;
    case c::operation::take:case c::operation::cancel:assert(out.u64(2)&&out.u64(4));break;
    case c::operation::clear:key();assert(out.u64(2)&&out.u64(1));break;
    }
    return uwvm2::utils::container::std_vector<std::uint8_t>{out.written().begin(),out.written().end()};
}
static void complete_and_truncation()
{
    for(auto const& operation:c::operations)
    {
        auto bytes=request(operation.id);c::request output;
        assert(c::decode(operation.id,bytes,output)&&bytes.size()<=c::request_bound(operation.id,1));
        output.buffer=99;
        for(std::size_t length{};length<bytes.size();++length)
            assert(!c::decode(operation.id,{bytes.data(),length},output)&&output.buffer==99);
        bytes.push_back(0);assert(!c::decode(operation.id,bytes,output)&&output.buffer==99);
    }
    c::request output;assert(!c::decode(static_cast<c::operation>(255),{},output));
}
static void exact_time()
{
    c::request decoded;auto bytes=request(c::operation::lookup);
    assert(c::decode(c::operation::lookup,bytes,decoded)&&decoded.lookup.target_stamped&&decoded.lookup.target_time.nanoseconds==0&&
        decoded.lookup.target_time.epoch==0&&!decoded.lookup.source_stamped);
    std::array<std::uint8_t,4096> storage{};s::wire::writer out{storage};assert(out.u64(2));query(out,false);
    assert(c::decode(c::operation::lookup,out.written(),decoded)&&!decoded.lookup.target_stamped);
    bytes=request(c::operation::publish);assert(c::decode(c::operation::publish,bytes,decoded)&&decoded.transforms[0].time.nanoseconds==-10);
    assert(decoded.transforms[0].time.epoch==0); // Initial steady/system epoch is zero.
}
static void finite_and_closed_names()
{
    auto t=value();std::array<std::uint8_t,4096> bytes{};s::wire::writer out{bytes};
    assert(out.u64(2)&&out.u64(1)&&out.u64(2)&&out.u64(0)&&out.u64(1));
    auto prefix=out.size();assert(c::write_transform(out,t));c::request decoded;
    assert(c::decode(c::operation::publish,out.written(),decoded));
    t.parent=t.child;out=s::wire::writer{bytes};assert(out.u64(2)&&out.u64(1)&&out.u64(2)&&out.u64(0)&&out.u64(1)&&c::write_transform(out,t));
    assert(!c::decode(c::operation::publish,out.written(),decoded));
    t=value();t.pose.rotation.x=std::numeric_limits<double>::infinity();out=s::wire::writer{bytes};assert(!c::write_transform(out,t));
    out=s::wire::writer{bytes};assert(out.u64(2)&&out.u64(1)&&out.u64(2)&&out.u64(0)&&out.u64(129));
    assert(!c::decode(c::operation::publish,out.written(),decoded));
    out=s::wire::writer{bytes};assert(out.u64(2)&&out.u64(1)&&out.u64(2)&&out.u64(0)&&out.u64(1)&&out.text(std::string_view{"a\0b",3}));
    assert(out.size()>prefix&&!c::decode(c::operation::publish,out.written(),decoded));
}
static void maximal_extents()
{
    c::transform t=value();std::array<char,255> text{};text.fill('a');assert(t.parent.assign({text.data(),text.size()}));
    text[0]='b';assert(t.child.assign({text.data(),text.size()}));
    std::array<std::uint8_t,81920> bytes{};s::wire::writer out{bytes};assert(out.u64(2)&&out.u64(1)&&out.u64(2)&&out.u64(0)&&out.u64(c::maximum_batch));
    for(std::size_t n{};n<c::maximum_batch;++n)assert(c::write_transform(out,t));
    c::request decoded;assert(out.size()<=c::request_bound(c::operation::publish,c::maximum_batch)&&c::decode(c::operation::publish,out.written(),decoded)&&decoded.count==c::maximum_batch);
    assert(c::request_bound(c::operation::publish,SIZE_MAX)==c::request_bound(c::operation::publish,c::maximum_batch));
}
int main(){complete_and_truncation();exact_time();finite_and_closed_names();maximal_extents();std::cout<<"passed 4 transform contract security groups\n";}
