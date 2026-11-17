/** @file
 * @brief Check bounded transform math, analytic velocity and pinned upstream tf2 equivalence.
 */
#include <uwvm2/imported/ros/scene/transform_math.h>
#include <cassert>
#include <iostream>
#include <numbers>
#if defined(UWVM_SCENE_TF2_REFERENCE)
#include <tf2/LinearMath/Transform.hpp>
#endif
namespace m=uwvm2::imported::ros::scene::transforms::math;
static constexpr m::bounds limit{10000,1e-12};
static bool near(double a,double b,double tolerance=1e-10){return std::abs(a-b)<=tolerance;}
static bool near(m::vector a,m::vector b){return near(a.x,b.x)&&near(a.y,b.y)&&near(a.z,b.z);}
static m::quaternion around_z(double angle){return {0,0,std::sin(angle/2),std::cos(angle/2)};}
static void validation()
{
    m::rigid sentinel{{11,12,13},{0,0,0,1}},out=sentinel,value{};
    assert(m::normalize(value,{},out)&&out==m::rigid{});
    for(auto bad:{std::numeric_limits<double>::quiet_NaN(),std::numeric_limits<double>::infinity(),-std::numeric_limits<double>::infinity()})
    {
        out=sentinel;value={};value.translation.x=bad;assert(!m::normalize(value,limit,out)&&out==sentinel);
        value={};value.rotation.w=bad;assert(!m::normalize(value,limit,out)&&out==sentinel);
    }
    value={};value.rotation={0,0,0,0};out=sentinel;assert(!m::normalize(value,limit,out)&&out==sentinel);
    value.rotation.w=2;assert(!m::normalize(value,limit,out));value.rotation.w=1+5e-13;assert(m::normalize(value,limit,out)&&out.rotation.w==1);
    assert(!m::normalize(value,{10000,0},out));value={};value.translation={8000,8000,0};assert(!m::normalize(value,limit,out));
    for(auto bound:{m::bounds{-1,0},m::bounds{1,-1},m::bounds{1,1},m::bounds{INFINITY,0}})assert(!m::valid(bound));
}
static void composition_and_inverse()
{
    m::rigid parent{{1,0,0},around_z(std::numbers::pi/2)},child{{1,0,0},{}},out,back;
    assert(m::compose(parent,child,limit,out)&&near(out.translation,{1,1,0}));
    assert(m::inverse(parent,limit,back)&&m::compose(parent,back,limit,out)&&near(out.translation,{})&&near(out.rotation.w,1));
    assert(m::compose(back,parent,limit,out)&&near(out.translation,{})&&near(out.rotation.w,1));
    m::rigid sentinel{{7,8,9},{}};out=sentinel;
    assert(!m::compose({{9000,0,0},{}},{{9000,0,0},{}},limit,out)&&out==sentinel);
}
static void interpolation()
{
    m::rigid first{},last{{2,4,6},around_z(std::numbers::pi)},out;
    assert(m::interpolate(first,-10,last,10,0,limit,out)&&near(out.translation,{1,2,3}));
    assert(near(m::rotate_unit(out.rotation,{1,0,0}),{0,1,0}));
    auto negative=last;negative.rotation={-last.rotation.x,-last.rotation.y,-last.rotation.z,-last.rotation.w};m::rigid same;
    assert(m::interpolate(first,-10,negative,10,0,limit,same)&&near(m::rotate_unit(same.rotation,{1,0,0}),{0,1,0}));
    assert(m::interpolate(first,INT64_MIN,last,INT64_MAX,0,limit,out)&&near(out.translation,{1,2,3}));
    assert(m::interpolate(first,-10,last,10,-10,limit,out)&&out==first);
    assert(m::interpolate(first,-10,last,10,10,limit,out)&&near(out.translation,last.translation));
    auto before=out;assert(!m::interpolate(first,-10,last,10,11,limit,out)&&out==before);
    assert(!m::interpolate(first,1,last,1,1,limit,out)&&out==before);
    negative=first;negative.rotation.w=-1;
    assert(m::interpolate(first,0,negative,2,1,limit,out)&&near(out.rotation.w,1));
}
static void velocity_and_reference()
{
    m::velocity out;
    assert(m::differentiate({},{{2,0,0},around_z(std::numbers::pi/2)},1000000000,limit,10,10,out));
    assert(near(out.linear,{2,0,0})&&near(out.angular,{0,0,std::numbers::pi/2}));
    auto before=out;assert(!m::differentiate({},{{2,0,0},{}},0,limit,10,10,out)&&near(out.linear,before.linear));
    assert(!m::differentiate({},{{2,0,0},{}},1000000000,limit,1,10,out));
    assert(!m::differentiate({},{{},around_z(std::numbers::pi/2)},1000000000,limit,10,1,out));
    m::velocity observed{{1,0,0},{0,0,2}};
    assert(m::reference(observed,{3,0,0},{1,0,0},{},limit,20,20,out)&&near(out.linear,{1,4,0})&&near(out.angular,{0,0,2}));
    assert(m::reference(observed,{3,0,0},{1,0,0},around_z(std::numbers::pi/2),limit,20,20,out)&&near(out.linear,{-4,1,0}));
    before=out;assert(!m::reference(observed,{3,0,0},{1,0,0},{},limit,3,20,out)&&near(out.linear,before.linear));
}
#if defined(UWVM_SCENE_TF2_REFERENCE)
static tf2::Quaternion native(m::quaternion q){return {q.x,q.y,q.z,q.w};}
static tf2::Transform native(m::rigid t){return tf2::Transform(native(t.rotation),{t.translation.x,t.translation.y,t.translation.z});}
static m::vector vector(tf2::Vector3 const& v){return {v.x(),v.y(),v.z()};}
static void upstream_equivalence()
{
    std::uint64_t random=0x9e3779b97f4a7c15ULL;
    auto number=[&]{random=random*6364136223846793005ULL+1442695040888963407ULL;return (static_cast<double>(random>>11)/9007199254740992.0-.5)*2;};
    auto pose=[&]
    {
        m::vector axis{number(),number(),number()};axis=m::scale(axis,1/m::norm(axis));auto angle=number()*std::numbers::pi;
        m::rigid value{{number()*100,number()*100,number()*100},{axis.x*std::sin(angle/2),axis.y*std::sin(angle/2),axis.z*std::sin(angle/2),std::cos(angle/2)}};
        return value;
    };
    for(unsigned index=0;index!=20000;++index)
    {
        auto a=pose(),b=pose();m::rigid out;auto reference=native(a)*native(b);
        assert(m::compose(a,b,limit,out)&&near(out.translation,vector(reference.getOrigin())));
        for(auto point:{m::vector{1,0,0},m::vector{0,1,0},m::vector{0,0,1}})
            assert(near(m::rotate_unit(out.rotation,point),vector(reference.getBasis()*tf2::Vector3(point.x,point.y,point.z))));
        assert(m::inverse(a,limit,out)&&near(out.translation,vector(native(a).inverse().getOrigin())));
        double t=.5*(number()+1);auto qt=native(a.rotation).slerp(native(b.rotation),t);
        auto ns=static_cast<std::int64_t>(t*1000000000);t=static_cast<double>(ns)/1000000000;
        qt=native(a.rotation).slerp(native(b.rotation),t);
        assert(m::interpolate(a,0,b,1000000000,ns,limit,out));
        assert(near(m::rotate_unit(out.rotation,{1,0,0}),vector(tf2::quatRotate(qt,{1,0,0}))));
    }
}
#endif
int main()
{
    validation();composition_and_inverse();interpolation();velocity_and_reference();
#if defined(UWVM_SCENE_TF2_REFERENCE)
    upstream_equivalence();std::cout<<"passed 20000 upstream tf2 equivalence cases\n";
#endif
    std::cout<<"passed 4 transform math security groups\n";
}
