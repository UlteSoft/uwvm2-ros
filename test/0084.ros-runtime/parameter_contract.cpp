#include <uwvm2/imported/ros/scene/parameter_contract.h>
#include <array>
#include <cassert>
#include <cstdio>

namespace pc=uwvm2::imported::ros::parameter_contract;
namespace pv=uwvm2::imported::ros::scene::parameter_values;
namespace wire=uwvm2::imported::ros::scene::wire;

struct record
{
    std::array<std::uint8_t,8192> storage{};
    wire::writer output{storage};
    wire::bytes bytes() const{return output.written();}
};
static pv::limits policy(pv::algebra algebra=pv::algebra::ros2)
{return {4096,128,128,8,algebra,false};}
static void key(wire::writer& w){assert(w.u64(17)&&w.u64(1)&&w.u64(17));}
static void scalar(wire::writer& w,std::int64_t value=42)
{assert(w.u64(2)&&w.i64(value));}
static void items(wire::writer& w,std::uint64_t count=1)
{assert(w.u64(count));for(std::uint64_t i{};i!=count;++i){assert(w.text("motor.speed"));scalar(w);}}
static void descriptor(wire::writer& w)
{assert(w.text("motor.speed")&&w.text("integer")&&w.text("speed")&&w.text("bounded")&&w.boolean(false)&&w.boolean(false)&&w.boolean(true)&&
        w.u64(1)&&w.i64(INT64_MIN)&&w.i64(INT64_MAX)&&w.u64(UINT64_MAX));}
static void accepted(pc::operation action,record& input,pv::limits limits=policy())
{
    pc::request result;assert(pc::decode(action,input.bytes(),limits,result));
    for(std::size_t end{};end<input.bytes().size();++end)
    {pc::request sentinel;sentinel.scope=1234;assert(!pc::decode(action,input.bytes().first(end),limits,sentinel));assert(sentinel.scope==1234);}
    auto size=input.bytes().size();input.storage[size]=0;
    assert(!pc::decode(action,wire::bytes{input.storage.data(),size+1},limits,result));
}
static void canonical_operations()
{
    {record r;std::array<std::uint8_t,32> digest{};assert(r.output.u64(8)&&r.output.u64(9)&&r.output.text("motor.params")&&r.output.blob(digest));accepted(pc::operation::open,r);}
    {record r;key(r.output);descriptor(r.output);scalar(r.output);accepted(pc::operation::declare,r);pc::request q;assert(pc::decode(pc::operation::declare,r.bytes(),policy(),q));
     assert(q.declaration.integers.step==UINT64_MAX&&q.declaration.integers.contains(INT64_MAX)&&q.items[0].scalar.integer==42);}
    {record r;key(r.output);assert(r.output.text("motor.speed")&&r.output.boolean(true)&&r.output.u64(7));accepted(pc::operation::undeclare,r);}
    for(auto action:{pc::operation::get,pc::operation::describe})
    {record r;assert(r.output.u64(17)&&r.output.u64(1)&&r.output.text("motor.speed"));accepted(action,r);}
    {record r;assert(r.output.u64(17)&&r.output.u64(0)&&r.output.u64(0)&&r.output.boolean(false)&&r.output.u64(8));accepted(pc::operation::list,r);}
    for(auto action:{pc::operation::set,pc::operation::set_atomic})
    {record r;key(r.output);assert(r.output.boolean(false));items(r.output);accepted(action,r);}
    for(auto action:{pc::operation::set,pc::operation::set_atomic})
    {record r;key(r.output);assert(r.output.boolean(false)&&r.output.u64(0));accepted(action,r);}
    for(auto action:{pc::operation::get,pc::operation::describe})
    {record r;assert(r.output.u64(17)&&r.output.u64(0));accepted(action,r);}
    {record r;assert(r.output.u64(17));accepted(pc::operation::take,r);}
    {record r;assert(r.output.u64(17)&&r.output.u64(99)&&r.output.boolean(true)&&r.output.boolean(true));items(r.output);assert(r.output.text("approved"));accepted(pc::operation::respond,r);}
    {record r;assert(r.output.u64(17)&&r.output.text("motor.speed"));accepted(pc::operation::exists,r);}
    {record r;assert(r.output.u64(17)&&r.output.text("speed")&&r.output.text("/robot/arm"));accepted(pc::operation::search,r,policy(pv::algebra::ros1));}
}
static void bounded_transactions()
{
    pc::request q;
    {record r;key(r.output);assert(r.output.boolean(false));items(r.output,2);assert(!pc::decode(pc::operation::set,r.bytes(),policy(),q));assert(!pc::decode(pc::operation::set_atomic,r.bytes(),policy(),q));}
    {record r;key(r.output);assert(r.output.boolean(false));items(r.output);auto p=policy();p.nodes=0;assert(!pc::decode(pc::operation::set,r.bytes(),p,q));p=policy();p.bytes=8;assert(!pc::decode(pc::operation::set,r.bytes(),p,q));}
    {record r;key(r.output);assert(r.output.boolean(false)&&r.output.u64(129));assert(!pc::decode(pc::operation::set,r.bytes(),policy(),q));}
    {record r;assert(r.output.u64(17)&&r.output.u64(99)&&r.output.boolean(false)&&r.output.boolean(true));items(r.output);assert(r.output.text("denied"));assert(!pc::decode(pc::operation::respond,r.bytes(),policy(),q));}
    {record r;assert(r.output.u64(17)&&r.output.u64(0)&&r.output.u64(0)&&r.output.boolean(false)&&r.output.u64(0));assert(!pc::decode(pc::operation::list,r.bytes(),policy(),q));}
    {record r;assert(r.output.u64(17)&&r.output.u64(1)&&r.output.text(std::string_view{"bad\0key",7}));assert(!pc::decode(pc::operation::get,r.bytes(),policy(),q));}
}
static void separate_algebras()
{
    pc::request q;
    {record r;key(r.output);assert(r.output.boolean(false)&&r.output.u64(1)&&r.output.text("name")&&r.output.u64(0));assert(pc::decode(pc::operation::set,r.bytes(),policy(),q));assert(!pc::decode(pc::operation::set,r.bytes(),policy(pv::algebra::ros1),q));}
    {record r;key(r.output);assert(r.output.boolean(false)&&r.output.u64(1)&&r.output.text("name"));scalar(r.output,INT64_MAX);assert(pc::decode(pc::operation::set,r.bytes(),policy(),q));assert(!pc::decode(pc::operation::set,r.bytes(),policy(pv::algebra::ros1),q));}
    {record r;key(r.output);assert(r.output.boolean(false)&&r.output.u64(1)&&r.output.text("name")&&r.output.u64(32)&&r.output.u64(1));scalar(r.output);assert(pc::decode(pc::operation::set,r.bytes(),policy(pv::algebra::ros1),q));assert(!pc::decode(pc::operation::set,r.bytes(),policy(),q));}
}
static void descriptor_roundtrip()
{
    record r;descriptor(r.output);wire::reader input{r.bytes()};pc::descriptor value;assert(pc::read_descriptor(input,value)&&input.done());
    record encoded;assert(pc::write_descriptor(encoded.output,value));assert(std::ranges::equal(r.bytes(),encoded.bytes()));
    auto measured=wire::writer::measuring(8192);assert(pc::write_descriptor(measured,value)&&measured.size()==r.bytes().size());
    record wrong;assert(wrong.output.text("motor.speed")&&wrong.output.text("boolean")&&wrong.output.text("")&&wrong.output.text("")&&wrong.output.boolean(false)&&
        wrong.output.boolean(false)&&wrong.output.boolean(true)&&wrong.output.u64(1)&&wrong.output.i64(0)&&wrong.output.i64(8)&&wrong.output.u64(1));
    wire::reader bad{wrong.bytes()};assert(!pc::read_descriptor(bad,value));
}
static void encoded_composition()
{
    std::array<std::uint8_t,16> bytes{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    auto original=bytes;
    wire::writer overlap{std::span<std::uint8_t>{bytes}.subspan(2)};
    assert(overlap.encoded(wire::bytes{bytes}.first(8))&&overlap.size()==8);
    assert(std::equal(original.begin(),original.begin()+8,bytes.begin()+2));
    wire::writer too_short{std::span<std::uint8_t>{bytes}.first(4)};auto before=bytes;
    assert(!too_short.encoded(wire::bytes{original}.first(5))&&bytes==before&&too_short.size()==0&&!too_short.good());
    assert(!too_short.encoded({})&&bytes==before);
    auto measure=wire::writer::measuring(16);assert(measure.encoded(original)&&measure.size()==16&&measure.written().empty());
    assert(measure.encoded({})&&!measure.u64(0)&&measure.size()==16);
}
int main()
{canonical_operations();bounded_transactions();separate_algebras();descriptor_roundtrip();encoded_composition();std::puts("passed 5 parameter contract security groups");}
