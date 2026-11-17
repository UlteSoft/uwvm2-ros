/** @file
 * @brief Verify bounded atomic TF history, time domains and recoverable allocation.
 */
#include <uwvm2/imported/ros/scene/transform_cache.h>
#include <cassert>
#include <iostream>
#include <numbers>
#if defined(UWVM_SCENE_NATIVE_TF)
#include <tf2/buffer_core.hpp>
#endif
namespace t=uwvm2::imported::ros::scene::transforms;
namespace c=uwvm2::imported::ros::transform_contract;
using t::error;
static thread_local std::size_t allocations{},fail_after{};
static bool fail() noexcept {++allocations;return fail_after&&!--fail_after;}
extern "C" void* __real_malloc(std::size_t);
extern "C" void* __real_calloc(std::size_t,std::size_t);
extern "C" void* __real_realloc(void*,std::size_t);
extern "C" void* __wrap_malloc(std::size_t n){return fail()?nullptr:__real_malloc(n);}
extern "C" void* __wrap_calloc(std::size_t n,std::size_t size){return fail()?nullptr:__real_calloc(n,size);}
extern "C" void* __wrap_realloc(void* p,std::size_t n){return fail()?nullptr:__real_realloc(p,n);}
static c::name name(std::string_view value){c::name result;assert(result.assign(value));return result;}
static constexpr t::clock_context clock_sample{7,0,10000000000,true};
static t::cache_plan plan()
{
    t::cache_plan p;
    p.frames={name("base"),name("camera"),name("map"),name("odom"),name("tool"),name("world")};p.frame_count=6;
    p.edges={t::edge{5,2,false,true},t::edge{2,3,true,false},t::edge{3,0,true,false},
        t::edge{0,1,false,true},t::edge{0,4,false,true},t::edge{2,0,true,false}};p.edge_count=6;
    p.max_samples=64;p.samples_per_child=16;p.max_batch=16;p.history_ns=10000000000;p.future_ns=0;
    p.geometry={10000,1e-12};p.linear_speed=100;p.angular_speed=10;p.velocity_window_ns=2000000000;
    std::uint64_t bytes{};assert(t::cache::bank_bytes(p,bytes));p.data_bytes=p.workspace_bytes=bytes;return p;
}
static c::transform record(std::string_view parent,std::string_view child,std::int64_t time,double x,bool fixed=false)
{return {name(parent),name(child),{clock_sample.identity,clock_sample.epoch,time},{{x,0,0},{}},fixed};}
static c::query query(std::string_view target,std::string_view source,std::int64_t time,bool exact=true)
{return {name(target),name(source),{},{7,0,time},{7,0,time},exact,exact,false};}
static bool near(double a,double b){return std::abs(a-b)<1e-9;}
static error publish(t::cache& cache,c::transform const& value,t::clock_context clock=clock_sample,bool replace=false)
{return cache.publish({&value,1},clock,replace);}
static void policy_and_clock()
{
    auto good=plan();assert(t::cache::valid(good));t::cache cache;auto bad=good;
    bad.max_samples=0;assert(!t::cache::valid(bad));bad=good;bad.data_bytes--;assert(!t::cache::valid(bad));
    bad=good;bad.workspace_bytes--;assert(!t::cache::valid(bad));bad=good;bad.frames[0]=name("world");assert(!t::cache::valid(bad));
    bad=good;bad.edges[6]={0,2,true,false};bad.edge_count=7;assert(!t::cache::valid(bad));
    bad=good;bad.edges[6]=bad.edges[0];bad.edge_count=7;assert(!t::cache::valid(bad));
    bad=good;bad.frame_count=129;assert(!t::cache::valid(bad));bad=good;bad.max_samples=SIZE_MAX;assert(!t::cache::valid(bad));
    assert(cache.configure(good,{})==error::invalid&&!cache.inspect().generation);
    auto stopped=clock_sample;stopped.started=false;assert(cache.configure(good,stopped)==error::again&&!cache.inspect().generation);
    assert(cache.configure(good,clock_sample)==error::success&&cache.inspect().generation==1);
    assert(cache.configure(good,clock_sample)==error::conflict);
    auto value=record("map","base",0,1);assert(publish(cache,value)==error::success);
    c::transform out=record("world","tool",9,9),sentinel=out;
    auto wrong=clock_sample;wrong.identity=8;assert(cache.lookup(query("map","base",0),wrong,out)==error::denied&&out.pose==sentinel.pose);
    wrong=clock_sample;wrong.epoch=1;assert(cache.lookup(query("map","base",0),wrong,out)==error::conflict);
    assert(cache.lookup(query("map","base",0),stopped,out)==error::again);
    auto generation=cache.inspect().generation;
    assert(cache.clear(wrong,generation+1)==error::conflict&&cache.inspect().samples==1);
    assert(cache.clear(wrong,generation)==error::success&&cache.inspect().samples==0&&cache.inspect().epoch==1);
    assert(publish(cache,value,wrong)==error::conflict);value.time.epoch=1;assert(publish(cache,value,wrong)==error::success);
    value.time.clock=8;assert(publish(cache,value,wrong)==error::denied);
    value.time={7,1,wrong.now+1};assert(publish(cache,value,wrong)==error::invalid);
}
static void batch_atomicity_and_static_authority()
{
    t::cache cache;assert(cache.configure(plan(),clock_sample)==error::success);
    assert(publish(cache,record("map","base",0,1))==error::success);
    auto before=cache.inspect();std::array batch{record("map","base",10,2),record("world","base",10,99)};
    assert(cache.publish(batch,clock_sample,false)==error::denied&&cache.inspect().samples==before.samples&&cache.inspect().generation==before.generation);
    c::transform out;assert(cache.lookup(query("map","base",10),clock_sample,out)==error::again);
    batch[1]=record("map","base",10,3);assert(cache.publish(batch,clock_sample,false)==error::conflict&&cache.inspect().generation==before.generation);
    batch[1]=batch[0];assert(cache.publish(batch,clock_sample,false)==error::success&&cache.inspect().samples==2);
    auto wrong=record("map","base",20,2);wrong.pose.rotation.w=2;before=cache.inspect();assert(publish(cache,wrong)==error::invalid&&cache.inspect().generation==before.generation);
    assert(publish(cache,record("base","camera",0,2,true))==error::success);
    auto fixed=record("base","camera",0,2,true);fixed.pose.rotation.w=-1;assert(publish(cache,fixed)==error::success);
    fixed.pose.translation.x=3;before=cache.inspect();assert(publish(cache,fixed)==error::denied&&cache.inspect().generation==before.generation);
    assert(publish(cache,fixed,clock_sample,true)==error::conflict);
    assert(cache.publish({&fixed,1},clock_sample,true,before.generation+1)==error::conflict);
    assert(cache.publish({&fixed,1},clock_sample,true,before.generation)==error::success);
    assert(cache.lookup(query("base","camera",INT64_MIN),clock_sample,out)==error::success&&out.pose.translation.x==3&&out.is_static);
    assert(publish(cache,record("map","base",0,1,true))==error::denied); // edge is not static-authorized
    assert(cache.lookup(query("base","hidden",0),clock_sample,out)==error::denied);
    assert(cache.lookup(query("base","tool",0),clock_sample,out)==error::again);
}
static void interpolation_latest_and_advanced()
{
    t::cache cache;assert(cache.configure(plan(),clock_sample)==error::success);
    std::array batch{record("world","map",0,100,true),record("map","odom",0,0),record("map","odom",10,10),
        record("odom","base",0,10),record("odom","base",20,30),record("base","camera",0,2,true)};
    assert(cache.publish(batch,clock_sample,false)==error::success);c::transform out;
    assert(cache.lookup(query("map","base",0),clock_sample,out)==error::success&&near(out.pose.translation.x,10)&&out.time.nanoseconds==0);
    assert(cache.lookup(query("map","base",5),clock_sample,out)==error::success&&near(out.pose.translation.x,20));
    assert(cache.lookup(query("map","base",0,false),clock_sample,out)==error::success&&near(out.pose.translation.x,30)&&out.time.nanoseconds==10&&!out.is_static);
    assert(cache.lookup(query("world","camera",5),clock_sample,out)==error::success&&near(out.pose.translation.x,122));
    assert(cache.lookup(query("base","base",INT64_MIN),clock_sample,out)==error::success&&out.pose==t::math::rigid{});
    auto q=query("base","camera",10);q.source_time.nanoseconds=0;
    assert(cache.lookup(q,clock_sample,out)==error::invalid);
    q.has_fixed=true;q.fixed=name("map");assert(cache.lookup(q,clock_sample,out)==error::success&&near(out.pose.translation.x,-18)&&out.time.nanoseconds==10);
    q.target_time.epoch=1;assert(cache.lookup(q,clock_sample,out)==error::conflict);
    q=query("map","base",11);auto before=out;assert(cache.lookup(q,clock_sample,out)==error::again&&out.pose==before.pose);
    // Temporal reparenting is a discrete change at the newer sample.
    assert(publish(cache,record("map","odom",20,20))==error::success);
    assert(publish(cache,record("map","base",30,1))==error::success);
    assert(cache.lookup(query("map","base",25),clock_sample,out)==error::again); // odom lacks t25
    assert(publish(cache,record("map","odom",30,30))==error::success);
    assert(cache.lookup(query("map","base",25),clock_sample,out)==error::success&&near(out.pose.translation.x,55));
    assert(cache.lookup(query("map","base",30),clock_sample,out)==error::success&&near(out.pose.translation.x,1));
}
static void bounded_history_and_order()
{
    auto p=plan();p.history_ns=10;p.max_samples=3;p.samples_per_child=3;p.max_batch=3;
    std::uint64_t bytes{};assert(t::cache::bank_bytes(p,bytes));p.data_bytes=p.workspace_bytes=bytes;
    t::cache cache;assert(cache.configure(p,clock_sample)==error::success);
    assert(publish(cache,record("map","base",0,0))==error::success);
    assert(publish(cache,record("map","base",10,10))==error::success);
    assert(publish(cache,record("map","base",20,20))==error::success&&cache.inspect().samples==3);
    assert(publish(cache,record("map","base",30,30))==error::success&&cache.inspect().samples==3);
    c::transform out;assert(cache.lookup(query("map","base",20),clock_sample,out)==error::success);
    assert(cache.lookup(query("map","base",19),clock_sample,out)==error::again);
    assert(cache.lookup(query("map","base",25),clock_sample,out)==error::success&&near(out.pose.translation.x,25));
    auto before=cache.inspect();assert(publish(cache,record("map","base",29,29))==error::too_large&&cache.inspect().generation==before.generation);
    assert(publish(cache,record("map","base",19,19))==error::again);
    std::array batch{record("map","base",50,50),record("map","base",31,31)};
    assert(cache.publish(batch,clock_sample,false)==error::again&&cache.inspect().generation==before.generation);
    std::swap(batch[0],batch[1]);assert(cache.publish(batch,clock_sample,false)==error::again&&cache.inspect().generation==before.generation);
    assert(cache.clear(clock_sample,before.generation)==error::success);
    std::array negative{record("map","base",-10,-10),record("map","base",0,0)};
    assert(cache.publish(negative,clock_sample,false)==error::success&&cache.lookup(query("map","base",-5),clock_sample,out)==error::success&&near(out.pose.translation.x,-5));
}
static void velocity_complete_window()
{
    t::cache cache;assert(cache.configure(plan(),clock_sample)==error::success);
    auto first=record("map","base",0,0),last=record("map","base",1000000000,2);
    auto angle=std::numbers::pi/2;last.pose.rotation={0,0,std::sin(angle/2),std::cos(angle/2)};
    std::array batch{first,last};assert(cache.publish(batch,clock_sample,false)==error::success);
    c::velocity_query q{name("base"),name("map"),name("map"),name("base"),{},last.time,1000000000};t::math::velocity out;
    assert(cache.velocity(q,clock_sample,out)==error::success&&near(out.linear.x,2)&&near(out.angular.z,angle));
    q.point={1,0,0};assert(cache.velocity(q,clock_sample,out)==error::success&&near(out.linear.x,2-angle)&&near(out.linear.y,0));
    q.reference=name("base");assert(cache.velocity(q,clock_sample,out)==error::success&&near(out.linear.x,0)&&near(out.linear.y,-(2-angle)));
    auto before=out;q.averaging_ns=2000000000;assert(cache.velocity(q,clock_sample,out)==error::again&&out.linear==before.linear);
    q.averaging_ns=2000000001;assert(cache.velocity(q,clock_sample,out)==error::too_large);
    q.averaging_ns=1;q.time.nanoseconds=INT64_MIN;assert(cache.velocity(q,clock_sample,out)==error::invalid);
    q.time=last.time;q.reference=name("secret");assert(cache.velocity(q,clock_sample,out)==error::denied);
}
static void allocator_recovery_and_no_hot_allocation()
{
    for(std::size_t fault=1;fault<=2;++fault)
    {
        t::cache cache;auto p=plan();fail_after=fault;
        assert(cache.configure(p,clock_sample)==error::no_memory&&!fail_after&&!cache.inspect().generation&&!cache.inspect().data_bytes);
        assert(cache.configure(p,clock_sample)==error::success);
    }
    t::cache cache;assert(cache.configure(plan(),clock_sample)==error::success);
    auto a=record("map","base",0,0),b=record("map","base",1000000000,2);std::array batch{a,b};
    auto q=query("map","base",500000000);c::transform out;
    c::velocity_query v{name("base"),name("map"),name("map"),name("base"),{},b.time,1000000000};t::math::velocity velocity;
    auto before=allocations;fail_after=1;
    assert(cache.publish(batch,clock_sample,false)==error::success&&cache.lookup(q,clock_sample,out)==error::success&&cache.velocity(v,clock_sample,velocity)==error::success);
    auto generation=cache.inspect().generation;assert(cache.clear(clock_sample,generation)==error::success);
    assert(allocations==before&&fail_after==1);fail_after=0;
}
#if defined(UWVM_SCENE_NATIVE_TF)
static void actual_native_cache_comparison()
{
    auto p=plan();p.max_samples=128;p.samples_per_child=32;
    std::uint64_t bytes{};assert(t::cache::bank_bytes(p,bytes));p.data_bytes=p.workspace_bytes=bytes;
    t::cache cache;assert(cache.configure(p,clock_sample)==error::success);
    tf2::BufferCore native;
    auto add=[&](c::transform const& value)
    {
        assert(publish(cache,value)==error::success);
        geometry_msgs::msg::TransformStamped message;
        message.header.frame_id=std::string(value.parent.view());message.child_frame_id=std::string(value.child.view());
        message.header.stamp.sec=static_cast<std::int32_t>(value.time.nanoseconds/1000000000);
        message.header.stamp.nanosec=static_cast<std::uint32_t>(value.time.nanoseconds%1000000000);
        auto& pose=message.transform;pose.translation.x=value.pose.translation.x;pose.translation.y=value.pose.translation.y;pose.translation.z=value.pose.translation.z;
        pose.rotation.x=value.pose.rotation.x;pose.rotation.y=value.pose.rotation.y;pose.rotation.z=value.pose.rotation.z;pose.rotation.w=value.pose.rotation.w;
        assert(native.setTransform(message,"pinned-fixture",value.is_static));
    };
    add(record("world","map",1000000000,100,true));add(record("base","camera",1000000000,2,true));
    for(int i=0;i<16;++i)
    {
        auto time=1000000000+static_cast<std::int64_t>(i)*100000000;
        auto first=record("map","odom",time,i*.25),second=record("odom","base",time,2+i*.5);
        double a=i*.1,b=-i*.05;first.pose.rotation={0,0,std::sin(a/2),std::cos(a/2)};second.pose.rotation={0,std::sin(b/2),0,std::cos(b/2)};
        first.pose.translation.y=i*.125;second.pose.translation.z=i*.1;add(first);add(second);
    }
    auto compare=[](c::transform const& value,geometry_msgs::msg::TransformStamped const& reference)
    {
        auto const& p=reference.transform;
        assert(near(value.pose.translation.x,p.translation.x)&&near(value.pose.translation.y,p.translation.y)&&near(value.pose.translation.z,p.translation.z));
        t::math::quaternion q{p.rotation.x,p.rotation.y,p.rotation.z,p.rotation.w};
        for(auto axis:{t::math::vector{1,0,0},t::math::vector{0,1,0},t::math::vector{0,0,1}})
        {
            auto a=t::math::rotate_unit(value.pose.rotation,axis),b=t::math::rotate_unit(q,axis);
            assert(near(a.x,b.x)&&near(a.y,b.y)&&near(a.z,b.z));
        }
    };
    std::uint64_t random=0x123456789abcdef0ULL;auto time=[&]{random=random*6364136223846793005ULL+1;return std::int64_t{1000000000}+static_cast<std::int64_t>(random%1500000001);};
    std::array frames{std::string_view{"world"},std::string_view{"map"},std::string_view{"odom"},std::string_view{"base"},std::string_view{"camera"}};
    for(unsigned i=0;i<4000;++i)
    {
        auto at=time();auto target=frames[i%frames.size()],source=frames[(i/frames.size()+1)%frames.size()];
        c::transform result;assert(cache.lookup(query(target,source,at),clock_sample,result)==error::success);
        compare(result,native.lookupTransform(std::string(target),std::string(source),tf2::TimePoint{std::chrono::nanoseconds{at}}));
    }
    for(unsigned i=0;i<2000;++i)
    {
        auto source_time=time(),target_time=time();auto q=query("base","camera",target_time);q.source_time.nanoseconds=source_time;q.has_fixed=true;q.fixed=name("map");
        c::transform result;assert(cache.lookup(q,clock_sample,result)==error::success);
        compare(result,native.lookupTransform("base",tf2::TimePoint{std::chrono::nanoseconds{target_time}},"camera",tf2::TimePoint{std::chrono::nanoseconds{source_time}},"map"));
    }
    c::transform result;assert(cache.lookup(query("world","camera",0,false),clock_sample,result)==error::success);
    compare(result,native.lookupTransform("world","camera",tf2::TimePointZero));
    std::cout<<"passed 6001 actual native tf2 cache comparisons\n";
}
#endif
int main()
{
    policy_and_clock();batch_atomicity_and_static_authority();interpolation_latest_and_advanced();
    bounded_history_and_order();velocity_complete_window();allocator_recovery_and_no_hot_allocation();
#if defined(UWVM_SCENE_NATIVE_TF)
    actual_native_cache_comparison();
#endif
    std::cout<<"passed 6 transform cache security groups\n";
}
