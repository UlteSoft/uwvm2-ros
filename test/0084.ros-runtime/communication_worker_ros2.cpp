/** @file @brief Actual sealed ROS worker RPC against a controlled native ROS2 peer.
 * @details Test-only native peer is created by this retained fixture. Passing
 * this does not authenticate arbitrary loopback peers or qualify other codecs.
 */
#define main communication_startup_fixture_entry
#include "communication_worker_startup.cpp"
#undef main
#define UWVM_COMMUNICATION_STATIC_FIXTURE_ONLY
#include "communication_static_ros2.cpp"
#include <uwvm2/imported/ros/scene/communication_worker_configuration.h>
#include <uwvm2/imported/ros/scene/communication_worker_channel.h>
#include <uwvm2/imported/ros/scene/native/communication_action_codecs.h>
namespace cfg=s::communication_worker_configuration;
namespace rpc=s::communication_worker_protocol;
namespace channel=s::communication_worker_channel;
namespace wire=s::wire;
namespace contract=uwvm2::imported::ros::communication_contract;
using bytes=uwvm2::utils::container::std_vector<std::uint8_t>;
static bytes encoded(auto&& operation)
{std::array<std::uint8_t,4096> storage{};wire::writer out{storage};assert(operation(out));return {out.written().begin(),out.written().end()};}
struct message
{std::uint64_t correlation{};std::string_view kind;contract::message_view payload;};
static message delivery(wire::bytes bytes)
{
    wire::reader in{bytes};std::uint64_t ignored{};bool optional{};wire::bytes publisher;message result;
    assert(in.u64(ignored)&&in.u64(ignored)&&in.u64(ignored));
    for(unsigned i{};i<2;++i){assert(in.boolean(optional));if(optional)assert(in.u64(ignored)&&in.u64(ignored)&&in.u64(ignored));}
    assert(in.boolean(optional)&&(!optional||in.blob(publisher))&&in.u64(ignored)&&in.u64(result.correlation)&&in.text(result.kind)&&contract::read_message(in,result.payload)&&in.done());return result;
}
[[maybe_unused]] static char receive_marker(int fd)
{
    char marker{};auto until=c::now()+10000000000ull;
    for(;;){auto got=::read(fd,&marker,1);if(got==1)return marker;assert(got<0&&(errno==EAGAIN||errno==EINTR)&&c::now()<until);::poll(nullptr,0,1);}
}
#ifndef UWVM_COMMUNICATION_RPC_FIXTURE_ONLY
static int run_actual(int argc,char** argv)
{
    assert(argc>=11&&(argc-7)%4==0);x::manifest manifest;uwvm2::utils::container::std_vector<x::descriptor> owned;
    std::uint64_t image_bytes{},threads{},request{},response{};
    assert(decimal(argv[3],image_bytes)&&decimal(argv[4],threads)&&decimal(argv[5],request)&&decimal(argv[6],response));
    assert(add(argv[1],argv[2],"/worker/launcher",x::image_role::runtime,manifest.launcher,owned));
    cfg::specification definition;
    for(int i=7;i<argc;i+=4)
    {
        std::string_view role{argv[i]};auto kind=role=="bootstrap"?x::image_role::bootstrap:role=="runtime"?x::image_role::runtime:x::image_role::configuration;x::image_spec image;
        assert(add(argv[i+2],argv[i+3],argv[i+1],kind,image,owned));
        if(image.destination.view()=="/worker/profile")
        {
            manifest.bound.profile_bytes=image.bytes;manifest.configuration.sha256=image.sha256;bytes input(image.bytes);assert(::pread(image.source,input.data(),input.size(),0)==static_cast<ssize_t>(input.size()));
            assert(cfg::decode(input,16u*1024u*1024u,definition)==s::error::success);
        }
        manifest.files.emplace_back(std::move(image));
    }
    assert(definition.network.domain==policy().domain&&request==definition.request_bytes&&response==definition.response_bytes);
    auto binding=[&](std::string_view name){for(std::size_t i{};i<definition.definitions.size();++i)if(cfg::profile_of(definition.definitions[i]).identity==name)return std::uint64_t{i+1};assert(false);return std::uint64_t{};};
    int completed[2],release[2];assert(!::pipe2(completed,O_NONBLOCK|O_CLOEXEC)&&!::pipe2(release,O_NONBLOCK|O_CLOEXEC));
    auto peer=::fork();assert(peer>=0);if(!peer){::close(completed[0]);::close(release[1]);native_peer(definition.network,completed[1],release[0],true);::_exit(0);}
    ::close(completed[1]);::close(release[0]);assert(receive_marker(completed[0])=='R');
    assert(manifest.configuration.identity.assign("actual-rpc-worker")&&manifest.ros_prefix.assign("/opt/ros/jazzy"));auto profile_bytes=manifest.bound.profile_bytes;
    manifest.bound={image_bytes,2ull*1024*1024*1024,30,4096,manifest.files.size()+64,65536,0,request,response,65536,10000000000ull,1000000,1024u*1024u,profile_bytes,threads};
    x::process child;auto prepared=child.prepare(manifest);std::fprintf(stderr,"worker prepare valid=%d status=%u images=%zu profile=%lu\n",x::valid(manifest),unsigned(prepared),manifest.files.size(),profile_bytes);assert(prepared==error::success);
    auto launched=child.launch();std::fprintf(stderr,"worker launch=%u\n",unsigned(launched));assert(launched==error::success);std::size_t stderr_count{};
    auto drain=[&]{std::array<char,4096> text{};auto n=::read(child.stderr_fd(),text.data(),text.size());if(n>0){stderr_count+=n;assert(stderr_count<65536);assert(::write(2,text.data(),n)==n);}};
    auto wait=[&](unsigned phase)
    {auto until=c::now()+10000000000ull;auto status=error::again;while(status==error::again&&c::now()<until){status=phase?child.readiness(phase==2):child.identify();drain();if(status==error::again)::poll(nullptr,0,1);}assert(status==error::success);};
    wait(0);wait(1);
    auto graph_presence=[&]{char command='g';assert(::write(release[1],&command,1)==1);return receive_marker(completed[0]);};
    assert(graph_presence()=='0'); // Context/node were prepared privately, without a transport.
    t::broker broker;assert(broker.open(definition.network));std::array<w::role_fd,4> pipes;assert(broker.take_worker_pipes(pipes));
    assert(child.activate(pipes)==error::success);for(auto fd:pipes)::close(fd.descriptor);wait(2);assert(graph_presence()=='0');
    assert(broker.activate());auto visible_until=c::now()+5000000000ull;bool visible{};
    while(c::now()<visible_until&&!visible){visible=graph_presence()=='1';drain();if(!visible)::poll(nullptr,0,20);}
    if(!visible){auto state=broker.inspect();std::fprintf(stderr,"graph visibility relay active=%d fenced=%d uncertain=%d forwarded=%lu,%lu,%lu,%lu rejected=%lu,%lu,%lu,%lu\n",state.active,state.fenced,state.uncertain,state.forwarded[0],state.forwarded[1],state.forwarded[2],state.forwarded[3],state.rejected[0],state.rejected[1],state.rejected[2],state.rejected[3]);}
    assert(visible);
    channel::channel connection;assert(connection.prepare({child.input_fd(),child.output_fd(),child.stderr_fd(),child.process_fd()},
        {request,response,response*4,65536-stderr_count,5000000000ull,1000000},definition.network.generation)==c::error::ok);
    auto call=[&](rpc::operation operation,std::string_view name,wire::bytes body=wire::bytes{})
    {
        channel::reply answer;auto status=connection.exchange(operation,binding(name),body,answer);
        if(status!=c::error::ok)std::fprintf(stderr,"RPC transport operation=%lu binding=%.*s status=%u effect=%u\n",static_cast<unsigned long>(operation),static_cast<int>(name.size()),name.data(),unsigned(status),unsigned(answer.observed));
        assert(status==c::error::ok);if(answer.status!=c::error::ok&&answer.status!=c::error::again)std::fprintf(stderr,"RPC answer operation=%lu binding=%.*s status=%u effect=%u\n",static_cast<unsigned long>(operation),static_cast<int>(name.size()),name.data(),unsigned(answer.status),unsigned(answer.observed));return answer;
    };
    assert(call(rpc::operation::context_inspect,"context").status==c::error::ok&&call(rpc::operation::node_inspect,"node").status==c::error::ok);
    assert(call(rpc::operation::topic_open,"context").status==c::error::unsupported);
    for(auto name:{"publisher","subscription","bool-publisher","bool-subscription"})
    {auto result=call(rpc::operation::topic_open,name);if(result.status!=c::error::ok)std::fprintf(stderr,"topic open %s=%u\n",name,unsigned(result.status));assert(result.status==c::error::ok&&result.bytes().size()==8);}
    for(auto name:{"service-client","service-server"})assert(call(rpc::operation::service_open,name).status==c::error::ok);
    for(auto name:{"action-client","action-server"})assert(call(rpc::operation::action_open,name).status==c::error::ok);
    assert(call(rpc::operation::topic_qos,"publisher").status==c::error::ok&&call(rpc::operation::service_qos,"service-client").status==c::error::ok);
    bool string_received{},bool_received{},service_received{},service_served{},action_received{},action_served{},service_sent{},action_sent{},peer_complete{};
    auto deadline=[] {return c::now()+900000000ull;};auto until=c::now()+10000000000ull;unsigned ticks{};
    while(c::now()<until&&!(string_received&&bool_received&&service_received&&service_served&&action_received&&action_served&&peer_complete))
    {
        if(++ticks%10==0)
        {
            auto text=encoded([](auto& out){return out.text("bounded route");});auto body=encoded([&](auto& out){return out.blob(text)&&out.u64(deadline());});assert(call(rpc::operation::topic_publish,"publisher",body).status==c::error::ok);
            bytes boolean{1};body=encoded([&](auto& out){return out.blob(boolean)&&out.u64(deadline());});assert(call(rpc::operation::topic_publish,"bool-publisher",body).status==c::error::ok);
        }
        for(auto name:{"subscription","bool-subscription"})
        {
            auto result=call(rpc::operation::topic_take,name);if(result.status==c::error::again)continue;assert(result.status==c::error::ok);auto got=delivery(result.bytes());wire::reader body{got.payload.body};
            if(name==std::string_view{"subscription"}){std::string_view value;assert(body.text(value)&&body.done()&&value=="bounded route");string_received=true;}
            else{assert(got.payload.body.size()==1&&got.payload.body[0]==1);bool_received=true;}
        }
        if(!service_sent)
        {auto result=call(rpc::operation::service_available,"service-client");assert(result.status==c::error::ok);wire::reader body{result.bytes()};bool available{};assert(body.boolean(available)&&body.done());
         if(available){bytes value{1};auto input=encoded([&](auto& out){return out.u64(0)&&out.blob(value)&&out.u64(deadline());});assert(call(rpc::operation::service_submit,"service-client",input).status==c::error::ok);service_sent=true;}}
        for(auto name:{"service-client","service-server"})
        {
            auto result=call(rpc::operation::service_take,name);if(result.status==c::error::again)continue;assert(result.status==c::error::ok);auto got=delivery(result.bytes());
            if(name==std::string_view{"service-client"}){assert(!got.payload.body.empty()&&got.payload.body[0]==1);wire::reader tail{got.payload.body.subspan(1)};std::string_view message;assert(tail.text(message)&&message=="bounded service"&&tail.done());service_received=true;}
            else{assert(got.payload.body.size()==1&&got.payload.body[0]==1);auto response=encoded([](auto& out){std::array<std::uint8_t,1> flag{1};return out.encoded(flag)&&out.text("bounded service");});auto input=encoded([&](auto& out){return out.u64(got.correlation)&&out.blob(response)&&out.u64(deadline());});assert(call(rpc::operation::service_respond,"service-server",input).status==c::error::ok);service_served=true;}
        }
        if(!action_sent&&service_sent&&ticks>50)
        {
            Action::Goal goal;goal.target_frame="world";goal.source_frame="robot";std::array<std::uint8_t,1024> body{};std::size_t size{};assert(c::ros2::lookup_goal_codec::encode(goal,body,size));std::array<std::uint8_t,16> uuid{};uuid[0]=72;
            auto input=encoded([&](auto& out){return out.blob(uuid)&&out.blob({body.data(),size})&&out.u64(deadline());});assert(call(rpc::operation::action_submit,"action-client",input).status==c::error::ok);action_sent=true;
        }
        for(auto name:{"action-client","action-server"})
        {
            auto result=call(rpc::operation::action_take,name);if(result.status==c::error::again)continue;assert(result.status==c::error::ok);auto got=delivery(result.bytes());wire::reader body{got.payload.body};std::uint64_t goal{},state{};wire::bytes uuid;bool present{};contract::message_view payload;
            assert(body.u64(goal)&&body.blob(uuid)&&uuid.size()==16&&body.u64(state)&&body.boolean(present)&&(!present||contract::read_message(body,payload))&&body.done());
            if(got.kind=="action.result"){Action::Result value;assert(c::ros2::lookup_result_codec::decode(payload.body,value)&&value.transform.child_frame_id=="robot");action_received=true;}
            if(got.kind=="action.goal.proposal")
            {
                Action::Goal proposed;assert(c::ros2::lookup_goal_codec::decode(payload.body,proposed)&&proposed.target_frame=="world");
                for(std::uint64_t transition:{1,3,5})
                {
                    std::array<std::uint8_t,1024> result_body{};std::size_t size{};
                    if(transition==5){Action::Result value;value.transform.header.frame_id="world";value.transform.child_frame_id="robot";value.transform.transform.rotation.w=1;assert(c::ros2::lookup_result_codec::encode(value,result_body,size));}
                    auto input=encoded([&](auto& out){return out.u64(got.correlation)&&out.u64(goal)&&out.u64(transition)&&out.blob({result_body.data(),size})&&out.boolean(false)&&out.u64(deadline());});assert(call(rpc::operation::action_respond,"action-server",input).status==c::error::ok);
                }
                action_served=true;
            }
        }
        if(!peer_complete){char marker{};auto got=::read(completed[0],&marker,1);if(got==1){assert(marker==1);peer_complete=true;}else assert(got<0&&errno==EAGAIN);}
        ::poll(nullptr,0,2);
    }
    std::fprintf(stderr,"worker RPC flags=%d%d%d%d%d%d peer=%d ticks=%u\n",string_received,bool_received,service_received,service_served,action_received,action_served,peer_complete,ticks);
    assert(visible&&string_received&&bool_received&&service_received&&service_served&&action_received&&action_served&&peer_complete);
    assert(call(rpc::operation::graph_capture,"graph").status==c::error::ok);auto snapshot=broker.inspect();assert(!snapshot.fenced&&snapshot.forwarded[0]&&snapshot.forwarded[1]&&snapshot.forwarded[2]&&snapshot.forwarded[3]);
    char done=1;assert(::write(release[1],&done,1)==1);int peer_status{};assert(::waitpid(peer,&peer_status,0)==peer&&WIFEXITED(peer_status)&&!WEXITSTATUS(peer_status));
    connection.fence();broker.revoke();child.request_stop();int child_status{};auto cleaned=error::again;auto cleanup_until=c::now()+5000000000ull;
    while(cleaned==error::again&&c::now()<cleanup_until){cleaned=child.reap(child_status);if(cleaned==error::again)::poll(nullptr,0,1);}assert(cleaned==error::success);
    ::close(completed[0]);::close(release[1]);std::puts("PASS actual sealed ROS2 worker RPC: String/Bool, SetBool, LookupTransform both directions; graph invisible before attachment; bounded static relay; confirmed cleanup");return 0;
}
int main(int argc,char** argv)
{::signal(SIGPIPE,SIG_IGN);try{return run_actual(argc,argv);}catch(std::exception const& error){std::fprintf(stderr,"fixture exception: %s\n",error.what());return 1;}}

#endif
