/** @file @brief Exact frame/units/finite bounds, whole-vector and FP behavior of scalar commands. */
#include <uwvm2/imported/ros/scene/device_command.h>
#include <cassert>
#include <cfenv>
#include <iostream>
#include <vector>
namespace d=uwvm2::imported::ros::scene::devices;namespace w=uwvm2::imported::ros::scene::wire;using d::error;
static d::scalar_schema policy()
{
    d::scalar_schema value;assert(value.profile.identity.assign("bounded.actuation")&&value.frame.assign("base"));value.profile.sha256.fill(42);value.count=2;
    assert(value.fields[0].name.assign("drive.velocity")&&value.fields[1].name.assign("steering.angle"));value.fields[0].quantity=d::unit::metre_per_second;value.fields[0].minimum=-2;value.fields[0].maximum=2;
    value.fields[1].quantity=d::unit::radian;value.fields[1].minimum=-0.5;value.fields[1].maximum=0.5;assert(d::valid(value));return value;
}
static std::vector<std::uint8_t> wire(d::scalar_schema const& policy,d::scalar_command const& values,std::string_view frame="base")
{
    std::array<std::uint8_t,8192> bytes{};w::writer out{bytes};assert(out.text(frame)&&out.u64(values.count));
    for(std::size_t i{};i<values.count;++i)assert(out.text(policy.fields[i].name.view())&&out.text(d::unit_names[static_cast<std::size_t>(policy.fields[i].quantity)])&&out.f64(values.values[i]));
    return {out.written().begin(),out.written().end()};
}
static void complete_command_and_exact_endpoints()
{
    auto schema=policy();d::scalar_command input;input.count=2;input.values[0]=2;input.values[1]=-0.5;auto body=wire(schema,input);d::scalar_command result;
    assert(d::decode(body,schema,result)==error::success&&result.count==2&&result.values==input.values);
    std::array<std::uint8_t,8192> encoded{};w::writer out{encoded};assert(d::encode(out,schema,input)&&std::equal(out.written().begin(),out.written().end(),body.begin(),body.end()));
    input.values[0]=std::bit_cast<double>(std::bit_cast<std::uint64_t>(2.0)+1);assert(d::decode(wire(schema,input),schema,result)==error::denied&&result.values[0]==2);
    input.values[0]=0;input.values[1]=std::bit_cast<double>(std::bit_cast<std::uint64_t>(-0.5)+1);assert(d::decode(wire(schema,input),schema,result)==error::denied);
}
static void schema_frame_units_and_completeness()
{
    auto schema=policy();d::scalar_command input;input.count=2;auto body=wire(schema,input);d::scalar_command result;result.count=19;
    for(std::size_t n{};n<body.size();++n)assert(d::decode({body.data(),n},schema,result)!=error::success&&result.count==19);
    body.push_back(0);assert(d::decode(body,schema,result)==error::invalid);
    assert(d::decode(wire(schema,input,"map"),schema,result)==error::invalid);
    auto other=schema;other.fields[0].quantity=d::unit::radian_per_second;assert(d::decode(wire(other,input),schema,result)==error::invalid);
    other=schema;std::swap(other.fields[0],other.fields[1]);assert(d::decode(wire(other,input),schema,result)==error::invalid);
    input.count=1;assert(d::decode(wire(schema,input),schema,result)==error::invalid);
    schema.count=0;assert(!d::valid(schema)&&d::decode({},schema,result)==error::denied);
}
static void all_units_and_invalid_policy()
{
    auto schema=policy();schema.count=1;d::scalar_command input;input.count=1;d::scalar_command result;
    for(std::size_t n{};n<d::unit_names.size();++n){schema.fields[0].quantity=static_cast<d::unit>(n);assert(d::decode(wire(schema,input),schema,result)==error::success);}
    schema.fields[0].quantity=static_cast<d::unit>(255);assert(!d::valid(schema));
    schema=policy();schema.fields[1].name=schema.fields[0].name;assert(!d::valid(schema));
    schema=policy();schema.fields[0].minimum=3;assert(!d::valid(schema));
    schema=policy();schema.fields[0].maximum=std::bit_cast<double>(UINT64_C(0x7ff0000000000001));assert(!d::valid(schema));
    schema=policy();schema.frame.size=97;assert(!d::valid(schema));schema=policy();schema.count=33;assert(!d::valid(schema));
}
static void nonfinite_rejection_preserves_fp_environment()
{
    auto schema=policy();d::scalar_command values;values.count=2;d::scalar_command result;
    std::array<std::uint64_t,6> patterns{UINT64_C(0x7ff0000000000000),UINT64_C(0xfff0000000000000),UINT64_C(0x7ff0000000000001),UINT64_C(0xfff0000000000001),UINT64_C(0x7ff8000000000000),UINT64_C(0xfff8000000000000)};
    auto original=std::fegetround();
    for(auto mode:{FE_TONEAREST,FE_DOWNWARD,FE_UPWARD,FE_TOWARDZERO})
    {
        assert(std::fesetround(mode)==0);
        for(auto pattern:patterns)
        {
            values.values[0]=std::bit_cast<double>(pattern);auto body=wire(schema,values);assert(std::feclearexcept(FE_ALL_EXCEPT)==0);assert(std::feraiseexcept(FE_DIVBYZERO)==0);
            auto before=std::fetestexcept(FE_ALL_EXCEPT);assert(d::decode(body,schema,result)==error::invalid);assert(std::fetestexcept(FE_ALL_EXCEPT)==before&&std::fegetround()==mode);
        }
    }
    assert(std::fesetround(original)==0&&std::feclearexcept(FE_ALL_EXCEPT)==0);
}
static void frozen_decoder_matches_reference()
{
    auto schema=policy();d::scalar_command value;value.count=2;value.values[0]=1.25;value.values[1]=-.5;auto encoded=wire(schema,value);
    d::scalar_decoder prepared;d::scalar_command left,right;left.count=right.count=19;
    assert(prepared.decode(encoded,right)==error::denied&&right.count==19);
    assert(prepared.configure(schema)==error::success&&prepared.configure(schema)==error::denied);
    for(std::size_t n{};n<encoded.size();++n)
    {
        left.count=right.count=19;
        assert(d::decode({encoded.data(),n},schema,left)==prepared.decode({encoded.data(),n},right)&&left.count==19&&right.count==19);
        auto mutated=encoded;
        for(auto bit:{1,128})
        {
            mutated[n]=encoded[n]^bit;left.count=right.count=19;
            auto expected=d::decode(mutated,schema,left);auto actual=prepared.decode(mutated,right);assert(expected==actual);
            if(actual==error::success)assert(left.count==right.count&&left.values==right.values);
            else assert(left.count==19&&right.count==19);
        }
    }
    schema.fields[0].minimum=2;schema.fields[0].name.assign("changed");
    assert(prepared.decode(encoded,right)==error::success&&right.values==value.values);
    encoded.push_back(0);assert(prepared.decode(encoded,right)==error::invalid);
}
int main()
{complete_command_and_exact_endpoints();schema_frame_units_and_completeness();all_units_and_invalid_policy();nonfinite_rejection_preserves_fp_environment();frozen_decoder_matches_reference();std::cout<<"passed 5 device command safety groups\n";return 0;}
