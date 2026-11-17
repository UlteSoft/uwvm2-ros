/** @file
 * @brief Independently encode ROS parameter variants and test bounded semantic parsing.
 */
#include <uwvm2/imported/ros/scene/parameter_values.h>
#include <bit>
#include <cassert>
#include <iostream>
#include <vector>
namespace p=uwvm2::imported::ros::scene::parameter_values;
static void word(std::vector<std::uint8_t>& bytes,std::uint64_t value)
{for(unsigned n{};n<8;++n)bytes.push_back(static_cast<std::uint8_t>(value>>(8*n)));}
static void text(std::vector<std::uint8_t>& bytes,std::string_view value)
{word(bytes,value.size());bytes.insert(bytes.end(),value.begin(),value.end());}
static std::vector<std::uint8_t> scalar(std::uint64_t tag,std::uint64_t value)
{std::vector<std::uint8_t> bytes;word(bytes,tag);word(bytes,value);return bytes;}
static p::limits policy(p::algebra selected=p::algebra::ros2)
{return {4096,1024,1024,8,selected,false};}
static bool valid(std::vector<std::uint8_t> const& bytes,p::limits limits=policy())
{p::scalar out;return p::validate(bytes,limits,out);}
static void separate_algebras()
{
    assert(!valid(scalar(2,1),{}));auto disabled=policy();disabled.selected=p::algebra::disabled;assert(!valid(scalar(2,1),disabled));
    auto ros1=policy(p::algebra::ros1);
    for(std::uint64_t tag{};tag<40;++tag)
    {
        std::vector<std::uint8_t> value;word(value,tag);
        if(tag==1||tag==2||tag==3)word(value,1);
        else if(tag==4||tag==5||tag==6||tag==7||tag==8||tag==9||tag==32||tag==33)word(value,0);
        else if(tag==34)text(value,"20240229T23:59:59");
        assert(valid(value)==(tag<=9));
        assert(valid(value,ros1)==((tag>=1&&tag<=5)||(tag>=32&&tag<=34)));
    }
    for(auto integer:{INT64_MIN,INT64_MAX,std::int64_t{INT32_MIN}-1,std::int64_t{INT32_MAX}+1})
    {auto value=scalar(2,std::bit_cast<std::uint64_t>(integer));assert(valid(value)&&!valid(value,ros1));}
    for(auto integer:{std::int64_t{INT32_MIN},std::int64_t{INT32_MAX},std::int64_t{-1},std::int64_t{0}})
        assert(valid(scalar(2,std::bit_cast<std::uint64_t>(integer)),ros1));
    assert(!valid(scalar(1,2))&&!valid(scalar(1,UINT64_MAX)));
}
static void finite_numeric_policy()
{
    for(auto value:{std::numeric_limits<double>::infinity(),-std::numeric_limits<double>::infinity(),std::numeric_limits<double>::quiet_NaN()})
    {
        auto input=scalar(3,std::bit_cast<std::uint64_t>(value));assert(!valid(input));auto permissive=policy();permissive.nonfinite=true;
        assert(valid(input,permissive));
        auto array=scalar(8,1);word(array,std::bit_cast<std::uint64_t>(value));assert(!valid(array)&&valid(array,permissive));
    }
    p::integer_range all{INT64_MIN,INT64_MAX,UINT64_MAX};assert(all.valid()&&all.contains(INT64_MIN)&&all.contains(INT64_MAX)&&!all.contains(0));
    p::integer_range uneven{2,5,2};assert(uneven.contains(2)&&uneven.contains(4)&&uneven.contains(5)&&!uneven.contains(3));
    p::integer_range single{1,2,5};assert(single.contains(1)&&single.contains(2)&&!single.contains(0));
    p::integer_range reversed{1,-1,0};assert(!reversed.valid()&&!reversed.contains(0));
    p::integer_range full_step{INT64_MIN,INT64_MAX,std::uint64_t{1}<<63};assert(full_step.contains(0)&&!full_step.contains(1));
    p::floating_range floats{2,5,-2,0};assert(floats.valid()&&floats.contains(2)&&floats.contains(4)&&floats.contains(5)&&!floats.contains(3));
    floats.step=0;assert(floats.contains(3.25));floats.absolute_tolerance=100;assert(!floats.contains(5.00001));
    floats.step=std::numeric_limits<double>::quiet_NaN();assert(!floats.valid()&&!floats.contains(2));
    floats={0,1,std::numeric_limits<double>::denorm_min(),0};assert(!floats.contains(std::numeric_limits<double>::infinity()));
    floats={-std::numeric_limits<double>::max(),std::numeric_limits<double>::max(),1,0};assert(floats.valid()&&floats.contains(floats.minimum)&&floats.contains(floats.maximum));
}
static void recursive_global_budgets()
{
    auto limits=policy(p::algebra::ros1);auto input=scalar(32,2);auto child=scalar(32,2);
    auto number=scalar(2,1);child.insert(child.end(),number.begin(),number.end());child.insert(child.end(),number.begin(),number.end());
    input.insert(input.end(),child.begin(),child.end());input.insert(input.end(),child.begin(),child.end());
    limits.nodes=7;limits.elements=6;limits.depth=3;assert(valid(input,limits));
    --limits.nodes;assert(!valid(input,limits));++limits.nodes;--limits.elements;assert(!valid(input,limits));++limits.elements;--limits.depth;assert(!valid(input,limits));
    limits=policy(p::algebra::ros1);auto object=scalar(33,2);text(object,"same");object.insert(object.end(),number.begin(),number.end());
    text(object,"same");object.insert(object.end(),number.begin(),number.end());assert(!valid(object,limits));
    auto excessive=scalar(33,p::maximum_struct_members+1);assert(!valid(excessive,limits));
    auto huge=scalar(32,UINT64_MAX);assert(!valid(huge,limits));
    limits=policy();auto array=scalar(7,2);word(array,1);word(array,2);limits.nodes=2;assert(!valid(array,limits));
    limits.nodes=3;limits.elements=2;assert(valid(array,limits));limits.bytes=array.size()-1;assert(!valid(array,limits));
    limits=policy(p::algebra::ros1);limits.depth=17;assert(!valid(number,limits));
}
static void xml_and_exact_framing()
{
    auto ros1=policy(p::algebra::ros1);auto date=scalar(34,17);std::string_view leap="20240229T23:59:59";
    date.insert(date.end(),leap.begin(),leap.end());assert(valid(date,ros1));
    for(auto invalid:{"20230229T23:59:59","19000229T00:00:00","00000101T00:00:00","20241301T00:00:00","20240101T24:00:00","20240101T23:59:60"})assert(!p::datetime(invalid));
    assert(p::datetime("20000229T00:00:00"));
    std::vector<std::uint8_t> nul;word(nul,4);text(nul,std::string_view{"a\0b",3});assert(valid(nul)&&!valid(nul,ros1));
    std::vector<std::uint8_t> noncharacter;word(noncharacter,4);text(noncharacter,"\xef\xbf\xbe");assert(valid(noncharacter)&&!valid(noncharacter,ros1));
    auto proper=scalar(2,7);p::scalar sentinel;sentinel.integer=99;
    for(std::size_t n{};n<proper.size();++n)assert(!p::validate({proper.data(),n},policy(),sentinel)&&sentinel.integer==99);
    proper.push_back(0);assert(!valid(proper));
    std::uint64_t state=0x1234;for(unsigned iteration{};iteration<12000;++iteration)
    {
        std::vector<std::uint8_t> bytes(iteration%96);
        for(auto& byte:bytes){state^=state<<13;state^=state>>7;state^=state<<17;byte=static_cast<std::uint8_t>(state);}
        p::scalar out;static_cast<void>(p::validate(bytes,iteration%2?policy():ros1,out));
    }
}
int main(){separate_algebras();finite_numeric_policy();recursive_global_budgets();xml_and_exact_framing();std::cout<<"passed 4 parameter value security groups\n";}
