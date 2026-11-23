// Stable ROS v1 capability environments: actual owners survive a native world
// handoff; file-local numbers cannot create registrations or replenish quotas.
#include <uwvm2/imported/ros/capability/manager.h>
#include <array>
#include <cstdio>
#include <cstdlib>
namespace ros=uwvm2::imported::ros;
static void check(bool value,unsigned line)
{if(!value){std::fprintf(stderr,"ROS checkpoint rebinding failed at %u\n",line);std::exit(1);}}
#define CHECK(x) check(bool(x),__LINE__)
static ros::target_policy policy(ros::version version,ros::scope scope,std::string name)
{
    ros::target_policy value{};value.ros_version=version;value.target_scope=scope;
    value.name=std::move(name);value.enabled=true;
    value.connection={{"gateway-socket","/unused/checkpoint.sock"},{"gateway-uid","1000"},
        {"node-name","checkpoint"},{"namespace","/"}};
    if(version==ros::version::ros1)value.connection.insert({{"master-uri","http://127.0.0.1:11311"},
        {"advertise-host","127.0.0.1"},{"transport","tcpros"}});
    else value.connection.insert({{"domain-id","1"},{"rmw-implementation","rmw_fastrtps_cpp"},
        {"security-mode","disabled-test"},{"keystore","not-applicable"},{"enclave","not-applicable"}});
    ros::capability_spec cap{"talk","topic.publish","/test/data",
        version==ros::version::ros1?"std_msgs/String":"std_msgs/msg/String",{}, {}};
    cap.type_identity=ros::builtin_type_identity(version,cap.type);
    cap.limits={{"max-rate-hz","0.001"},{"burst","2"},{"overflow-policy","reject"},
        {"validator","none"},{"audit","deny-only"}};
    if(version==ros::version::ros1)cap.limits.insert({{"queue-depth","4"},{"transport","tcpros"},{"latch","false"}});
    else cap.limits.insert({{"qos.reliability","reliable"},{"qos.durability","volatile"},
        {"qos.history","keep-last"},{"qos.depth","4"},{"qos.deadline","infinite"},
        {"qos.lifespan","infinite"},{"qos.liveliness","automatic"},{"qos.liveliness-lease-duration","infinite"}});
    value.capabilities.emplace(cap.alias,std::move(cap));return value;
}
int main()
{
    unsigned checks{};
    for(auto version:{ros::version::ros1,ros::version::ros2})
    {
        ros::capability_manager manager;std::array<int,15> objects{};
        auto group=policy(version,ros::scope::group,"members");
        auto single=policy(version,ros::scope::single,"c");
        auto global=policy(version,ros::scope::global,"");
        auto reg=[&](unsigned i,char const* name,ros::target_policy const& target)
        {auto result=manager.register_instance(&objects[i],7,name,version==ros::version::ros1?&target:nullptr,
            version==ros::version::ros2?&target:nullptr);
         if(!result)std::fprintf(stderr,"registration: %s\n",result.detail.c_str());CHECK(result);};
        reg(0,"a",group);reg(1,"b",group);reg(2,"c",single);reg(3,"d",global);reg(4,"e",global);
        std::uint64_t gh{},sh{},ah{};
        CHECK(manager.open(&objects[0],7,version,"talk",gh));
        CHECK(manager.open(&objects[2],7,version,"talk",sh));
        CHECK(manager.open(&objects[3],7,version,"talk",ah));
        ros::authorization lease;
        auto describe=[&](unsigned i,std::uint64_t epoch,std::uint64_t handle)
        {auto r=manager.describe(&objects[i],epoch,version,handle,lease);lease.release();return bool(r);};
        CHECK(describe(1,7,gh));CHECK(!describe(2,7,gh));CHECK(!describe(0,7,sh));
        CHECK(describe(4,7,ah));CHECK(!describe(0,7,ah));
        auto consume=[&](unsigned i,std::uint64_t epoch,std::uint64_t handle)
        {auto r=manager.authorize(&objects[i],epoch,version,handle,"topic.publish",{},lease,false);lease.release();return bool(r);};
        CHECK(consume(0,7,gh));CHECK(consume(1,7,gh));CHECK(!consume(0,7,gh));
        // Snapshot reads must preserve the already consumed quota and the
        // exact group/global ownership. A cross-group source list is refused.
        std::array<ros::checkpoint::source_member,2> source_members{{
            {&objects[0],7,0,"a",false},{&objects[1],7,1,"b",false}}};
        ros::checkpoint::data::group captured{};
        CHECK(manager.copy_checkpoint_backend(source_members,version,captured)==ros::checkpoint::status::ready);
        CHECK(captured.registered && captured.alive && captured.handles.size()==1 && captured.capabilities.size()==1);
        CHECK(captured.handles[0].active && captured.handles[0].slot==static_cast<std::uint32_t>(gh));
        CHECK(captured.capabilities[0].accounting[3]==0); // reading did not refill tokens
        CHECK(!consume(0,7,gh));
        source_members[1]={&objects[2],7,2,"c",false};
        ros::checkpoint::data::group rejected{};
        CHECK(manager.copy_checkpoint_backend(source_members,version,rejected)==ros::checkpoint::status::malformed);
        CHECK(describe(1,7,gh));CHECK(!describe(2,7,gh));
        std::array<ros::checkpoint::member,5> members{};
        std::array<char const*,5> names{"a","b","c","d","e"};
        for(unsigned i{};i<members.size();++i)members[i]={&objects[i],&objects[i+5],7,8,names[i],false};
        auto refused=[&](auto const& input)
        {ros::capability_manager::checkpoint_rebinding bad;
            CHECK(manager.prepare_checkpoint_rebinding(input,bad)!=ros::checkpoint::status::ready);
            CHECK(!bad.current());CHECK(!bad.publish());CHECK(describe(0,7,gh));++checks;};
        refused(std::span(members).first(4));
        auto wrong=members;wrong[1].replacement=wrong[0].replacement;refused(wrong);
        wrong=members;wrong[1].previous=wrong[0].previous;refused(wrong);
        wrong=members;wrong[0].replacement=wrong[4].previous;refused(wrong);
        wrong=members;wrong[0].module="foreign";refused(wrong);
        wrong=members;wrong[0].previous_epoch=6;refused(wrong);
        wrong=members;wrong[0].authority_retired=true;refused(wrong);
        {ros::capability_manager::checkpoint_rebinding cancelled;
         CHECK(manager.prepare_checkpoint_rebinding(members,cancelled)==ros::checkpoint::status::ready);
         CHECK(cancelled.current());} // Dropped preparation must leave the old world intact.
        CHECK(describe(0,7,gh));CHECK(!describe(5,8,gh));
        ros::capability_manager::checkpoint_rebinding first;
        CHECK(manager.prepare_checkpoint_rebinding(members,first)==ros::checkpoint::status::ready);
        CHECK(first.current());CHECK(first.publish());CHECK(!first.current());CHECK(!first.publish());
        CHECK(!describe(0,7,gh));CHECK(!describe(5,7,gh));CHECK(describe(5,8,gh));CHECK(describe(6,8,gh));
        CHECK(!describe(7,8,gh));CHECK(describe(7,8,sh));CHECK(describe(9,8,ah));
        CHECK(!consume(5,8,gh));CHECK(consume(7,8,sh));CHECK(consume(8,8,ah));
        // Stale prepared handoff must not resurrect an intervening retirement.
        for(unsigned i{};i<members.size();++i)members[i]={&objects[i+5],&objects[i+10],8,9,names[i],false};
        ros::capability_manager::checkpoint_rebinding stale;
        CHECK(manager.prepare_checkpoint_rebinding(members,stale)==ros::checkpoint::status::ready);
        CHECK(manager.retire_registered(&objects[5],8));CHECK(!stale.current());CHECK(!stale.publish());
        CHECK(!describe(5,8,gh));CHECK(describe(6,8,gh));
        members[0].authority_retired=true;
        ros::capability_manager::checkpoint_rebinding second;
        CHECK(manager.prepare_checkpoint_rebinding(members,second)==ros::checkpoint::status::ready);
        CHECK(second.publish());CHECK(!describe(10,9,gh));CHECK(describe(11,9,gh));
        CHECK(!consume(11,9,gh));CHECK(describe(12,9,sh));CHECK(describe(14,9,ah));
        ++checks;
    }
    std::printf("ROS v1 current-owner handoff: %u cases passed (both backends)\n",checks);
}
