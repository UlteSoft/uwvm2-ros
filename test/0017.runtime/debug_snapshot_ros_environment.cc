#include <uwvm2/runtime/diagnostic/snapshot_ros.h>
#include <uwvm2/imported/ros/ipc/event_queue.h>
#include <uwvm2/imported/ros/common/checkpoint_restore.h>
#include <cstdio>
#include <cstdlib>
namespace wire=uwvm2::runtime::diagnostic::snapshot_ros;
namespace ipc=uwvm2::imported::ros::ipc;
static unsigned checks{};
static void check(bool value,unsigned line){++checks;if(!value){std::fprintf(stderr,"ROS snapshot failure at %u\n",line);std::exit(1);}}
#define CHECK(...) check(bool((__VA_ARGS__)),__LINE__)
static bool write(void* ctx,std::span<std::byte const> data) noexcept
{try{auto& out=*static_cast<std::vector<std::byte>*>(ctx);out.insert(out.end(),data.begin(),data.end());return true;}catch(...){return false;}}
static std::vector<std::byte> encoded(wire::environment const& env)
{std::vector<std::byte> out;std::uint64_t size{};CHECK(wire::encode(env,&out,write,size));CHECK(size==out.size());return out;}
int main(int argc,char** argv)
{
    for(unsigned backend=1;backend<=2;++backend)
    {
        ipc::event_header binding{};binding.backend=backend;binding.source_handle=0x100000002u;
        binding.instance=712;binding.session=0xfedcba9876543210u; // MUST NOT be serialized.
        ipc::event_stream queue{{},binding,127};CHECK(queue.configured());
        ipc::event_record topic{};topic.kind=ipc::event_kind::topic_message;topic.payload={1,2,3,4};
        CHECK(queue.enqueue(topic)==0);CHECK(queue.reserve(41,128,32)==0);CHECK(queue.reserve(42,128,32)==0);
        ipc::event_record response{};response.kind=ipc::event_kind::service_response;response.correlation=41;response.payload={7,8};
        CHECK(queue.complete(41,response)==0);
        auto pinned=queue.peek(1024);CHECK(pinned.error==0);
        wire::environment env{};env.recording[0]=std::byte{1};env.backend=backend;
        wire::data::group group{};group.scope=2;group.name="robot-group";
        group.enabled=group.registered=group.alive=group.session_present=group.connected=true;
        group.next_service_request=43;group.session_label=1;
        group.members={{0,"robot",true,false},{1,"library",true,false}};
        wire::data::capability observer{};observer.alias="observer";observer.operation="event.observe";
        wire::data::capability source{};source.alias="source";source.operation="topic.subscribe";
        group.capabilities={observer,source};group.handles={{1,1,"observer",true,false},{2,1,"source",true,false}};
        wire::data::observation watch{};watch.handle=0x100000001u;watch.source_handle=binding.source_handle;
        watch.observer_alias="observer";watch.source_alias="source";watch.current_session=true;watch.requests={41,42};
        CHECK(queue.copy_checkpoint(watch.local));CHECK(watch.local.state[10]==1);CHECK(watch.local.events.size()==3);
        CHECK(watch.local.events[1].present && !watch.local.events[2].present);
        group.observations.push_back(watch);env.groups.push_back(group);CHECK(wire::valid(env));
        auto bytes=encoded(env);wire::environment decoded{};CHECK(wire::decode(bytes,decoded));CHECK(encoded(decoded)==bytes);
        std::array<std::byte,8> secret{};for(unsigned i{};i<8;++i)secret[i]=std::byte(binding.session>>(8*i));
        CHECK(std::search(bytes.begin(),bytes.end(),secret.begin(),secret.end())==bytes.end());
        auto again=queue.peek(1024);CHECK(again.error==0 && again.payload==pinned.payload);CHECK(queue.commit(1)==0);
        CHECK(decoded.groups[0].observations[0].local.events[0].payload.size()==4);
        auto restored=queue.prepare_checkpoint(watch.local);CHECK(restored);
        auto replayed=restored->peek(1024);CHECK(replayed.error==0 && replayed.payload==pinned.payload);
        CHECK(restored->binding().session==binding.session);CHECK(restored->commit(1)==0);
        auto current=queue.peek(1024);CHECK(current.error==0 && current.payload!=pinned.payload);
        auto invalid=watch.local;invalid.state[0]=3;CHECK(!queue.prepare_checkpoint(invalid));
        invalid=watch.local;invalid.state[8]++;CHECK(!queue.prepare_checkpoint(invalid));
        invalid=watch.local;invalid.state[9]++;CHECK(!queue.prepare_checkpoint(invalid));
        invalid=watch.local;invalid.state[10]=2;invalid.state[11]=999;CHECK(!queue.prepare_checkpoint(invalid));
        invalid=watch.local;invalid.events[1].correlation++;CHECK(!queue.prepare_checkpoint(invalid));
        invalid=watch.local;invalid.events[1].request=40;CHECK(!queue.prepare_checkpoint(invalid));
        invalid=watch.local;invalid.events[0].flags=128;CHECK(!queue.prepare_checkpoint(invalid));
        invalid=watch.local;invalid.events[0].owned_bytes=0;CHECK(!queue.prepare_checkpoint(invalid));
        invalid=watch.local;invalid.events.push_back(invalid.events.back());CHECK(!queue.prepare_checkpoint(invalid));
        CHECK(queue.commit(2)==0);CHECK(!queue.prepare_checkpoint(watch.local)); // delivered native response cannot revive
        namespace cp=uwvm2::imported::ros::checkpoint;
        CHECK(cp::recorded_group_matches_current(group,group));
        auto changed=group;changed.capabilities[0].accounting[3]++;CHECK(cp::recorded_group_matches_current(group,changed));
        changed=group;changed.capabilities[0].revoked=true;CHECK(!cp::recorded_group_matches_current(group,changed));
        changed=group;changed.handles[0].active=false;CHECK(!cp::recorded_group_matches_current(group,changed));
        changed=group;changed.members[0].name="foreign";CHECK(!cp::recorded_group_matches_current(group,changed));
        changed=group;changed.scope=0;CHECK(!cp::recorded_group_matches_current(group,changed));
        changed=group;changed.observations[0].native_sequence++;CHECK(!cp::recorded_group_matches_current(group,changed));
        changed=group;changed.observations[0].requests.clear();CHECK(!cp::recorded_group_matches_current(group,changed));
        changed=group;changed.next_service_request++;CHECK(cp::recorded_group_matches_current(group,changed));
        changed=group;changed.next_service_request--;CHECK(!cp::recorded_group_matches_current(group,changed));
        for(std::size_t n{};n<bytes.size();++n)
        {wire::environment previous=env;CHECK(!wire::decode(std::span(bytes).first(n),previous));CHECK(previous.backend==backend && previous.groups.size()==1);}
        auto excess=bytes;excess.push_back(std::byte{});CHECK(!wire::decode(excess,decoded));
        auto forged=bytes;for(unsigned i{};i<8;++i)forged[33+i]=std::byte{255};CHECK(!wire::decode(forged,decoded));
        auto bad=env;bad.groups.push_back(group);CHECK(!wire::valid(bad));
        bad=env;bad.groups[0].members.push_back(group.members[0]);CHECK(!wire::valid(bad));
        bad=env;bad.groups[0].handles[0].alias="foreign";CHECK(!wire::valid(bad));
        bad=env;bad.groups[0].observations[0].local.events[0].location=8;CHECK(!wire::valid(bad));
        auto row=wire::archive::record{backend==1?wire::archive::component::ros1:wire::archive::component::ros2,
            wire::archive::completeness::partial,0,2,bytes};
        CHECK(!wire::decode(wire::archive::visibility::diagnostic,row,decoded));
        CHECK(wire::decode(wire::archive::visibility::full,row,decoded));row.capture=wire::archive::completeness::complete;
        CHECK(!wire::decode(wire::archive::visibility::full,row,decoded));
        if(argc>1 && backend==1)
        {auto f=std::fopen(argv[1],"wb");CHECK(f);CHECK(std::fwrite(bytes.data(),1,bytes.size(),f)==bytes.size());CHECK(std::fclose(f)==0);}
    }
    std::printf("ROS v1 detached environment: %u checks passed\n",checks);
}
