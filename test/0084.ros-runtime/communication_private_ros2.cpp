/** @file @brief Actual ROS2 middleware between two sealed workers over private pipes.
 * @details Both workers load the pinned official Jazzy/FastDDS native closure.
 * No UDP peer is used. The host link admits only the two verified namespace-init
 * pidfds and exact sealed profiles, then forwards fixed discovery/user frames.
 */
#define UWVM_COMMUNICATION_RPC_FIXTURE_ONLY
#include "communication_worker_ros2.cpp"
#include <uwvm2/imported/ros/scene/communication_private_link.h>
#include "communication_external_fixture.h"
struct sealed_peer
{
    x::manifest manifest;
    uwvm2::utils::container::std_vector<x::descriptor> files;
    cfg::specification definition;
    uwvm2::utils::container::std_unique_ptr<external_fixture::owner> deployment;
    std::shared_ptr<s::communication_attachment::connection> child;
    channel::channel connection;
    std::uint64_t diagnostic_bytes{};
    void parse(int argc,char** argv,int& at)
    {
        assert(at+7<=argc);std::uint64_t images{},threads{},request{},response{},count{};
        assert(decimal(argv[at++],images)&&decimal(argv[at++],threads)&&decimal(argv[at++],request)&&decimal(argv[at++],response));
        auto launcher=argv[at++],digest=argv[at++];assert(add(launcher,digest,"/worker/launcher",x::image_role::runtime,manifest.launcher,files));
        assert(decimal(argv[at++],count)&&count<=256&&count<=(static_cast<unsigned>(argc-at))/4);
        for(std::uint64_t i{};i<count;++i)
        {
            std::string_view role{argv[at++]};auto destination=argv[at++],path=argv[at++],sha=argv[at++];x::image_spec image;
            auto kind=role=="bootstrap"?x::image_role::bootstrap:role=="runtime"?x::image_role::runtime:x::image_role::configuration;
            assert(add(path,sha,destination,kind,image,files));
            if(image.destination.view()=="/worker/profile")
            {manifest.bound.profile_bytes=image.bytes;manifest.configuration.sha256=image.sha256;bytes data(image.bytes);assert(::pread(image.source,data.data(),data.size(),0)==static_cast<ssize_t>(data.size()));assert(cfg::decode(data,16u*1024u*1024u,definition)==s::error::success);}
            manifest.files.emplace_back(std::move(image));
        }
        assert(manifest.configuration.identity.assign("qualified-sealed-peer")&&manifest.ros_prefix.assign("/opt/ros/jazzy"));auto profile=manifest.bound.profile_bytes;
        manifest.bound={images,2ull*1024*1024*1024,30,4096,manifest.files.size()+64,65536,0,request,response,65536,10000000000ull,1000000,1024u*1024u,profile,threads};
        deployment=uwvm2::utils::container::make_unique<external_fixture::owner>(manifest);
        child=uwvm2::utils::container::make_shared<s::communication_attachment::connection>();assert(child->prepare(deployment->attachment)==error::success);
    }
    void drain()
    {
        std::array<char,4096> text{};auto n=::read(child->stderr_fd(),text.data(),text.size());
        if(n>0){diagnostic_bytes+=n;assert(diagnostic_bytes<manifest.bound.stderr_bytes);assert(::write(2,text.data(),n)==n);}
    }
    void wait(bool attached=false)
    {
        auto until=c::now()+10000000000ull;auto status=error::again;
        while(status==error::again&&c::now()<until)
        {status=attached?child->activate_channel():child->begin();drain();if(status==error::again)::poll(nullptr,0,1);}
        assert(status==error::success);
    }
    void channel_ready()
    {assert(connection.prepare({child->input_fd(),child->output_fd(),child->stderr_fd(),child->process_fd(),child->session_until(),child->stop_flag()},
        {definition.request_bytes,definition.response_bytes,definition.response_bytes*4,65536-diagnostic_bytes,5000000000ull,1000000},definition.network.generation)==c::error::ok);}
    channel::reply call(rpc::operation action,std::string_view name,wire::bytes body={})
    {
        std::uint64_t binding{};for(std::size_t i{};i<definition.definitions.size();++i)if(cfg::profile_of(definition.definitions[i]).identity==name)binding=i+1;assert(binding);
        channel::reply answer;auto status=connection.exchange(action,binding,body,answer);
        if(status!=c::error::ok|| (answer.status!=c::error::ok&&answer.status!=c::error::again))std::fprintf(stderr,"sealed RPC operation=%lu binding=%.*s transport=%u status=%u effect=%u\n",static_cast<unsigned long>(action),static_cast<int>(name.size()),name.data(),unsigned(status),unsigned(answer.status),unsigned(answer.observed));
        assert(status==c::error::ok);return answer;
    }
    bool graph_has(std::string_view expected)
    {
        auto result=call(rpc::operation::graph_capture,"graph");assert(result.status==c::error::ok);wire::reader in{result.bytes()};std::uint64_t rows{};assert(in.u64(rows));bool present{};
        for(std::uint64_t i{};i<rows;++i){std::uint64_t kind{};std::string_view node,endpoint,type;assert(in.u64(kind)&&in.text(node)&&in.text(endpoint)&&in.text(type));if(node==expected)present=true;}
        assert(in.done());return present;
    }
    void stop()
    {
        connection.fence();child->request_stop();
        // The test deployment owns termination. Keep borrowed relay descriptors
        // alive until the link drains; session loss is not physical cleanup.
        assert(::kill(deployment->pid,0)==0);
    }
};
struct exercise
{
    bool string_received{},bool_received{},service_received{},service_served{},action_received{},action_served{},service_sent{},action_sent{};
    unsigned ticks{};
    bool complete()const noexcept{return string_received&&bool_received&&service_received&&service_served&&action_received&&action_served;}
    template<typename Peer> void tick(Peer& peer)
    {
        auto call=[&](rpc::operation op,std::string_view name,wire::bytes body={}){return peer.call(op,name,body);};
        auto deadline=[] {return c::now()+900000000ull;};
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
    }
};
#ifndef UWVM_COMMUNICATION_PRIVATE_FIXTURE_ONLY
/** @brief Exercise distinct reserve/release/ack RPCs on actual native records. */
static bytes retained_delivery(sealed_peer& peer,std::string_view name,bool service)
{
    auto reserve=service?rpc::operation::service_reserve:rpc::operation::topic_reserve;
    auto release=service?rpc::operation::service_release:rpc::operation::topic_release;
    auto acknowledge=service?rpc::operation::service_acknowledge:rpc::operation::topic_acknowledge;
    auto take=service?rpc::operation::service_take:rpc::operation::topic_take;
    auto until=c::now()+5000000000ull;channel::reply first;bytes input;
    do
    {
        input=encoded([&](auto& out){return out.u64(77)&&out.u64(c::now()+900000000ull);});
        first=peer.call(reserve,name,input);assert(first.status==c::error::ok||first.status==c::error::again);
        if(first.status==c::error::again)::poll(nullptr,0,1);
        assert(c::now()<until);
    }while(first.status==c::error::again);
    std::uint64_t endpoint{},generation{},sequence{},nonce{},deadline{};
    wire::reader header{first.bytes()};assert(header.u64(endpoint)&&endpoint&&header.u64(generation)&&generation==peer.definition.network.generation&&
        header.u64(sequence)&&sequence&&header.u64(nonce)&&nonce&&header.u64(deadline)&&deadline);
    auto duplicate=peer.call(reserve,name,input);assert(duplicate.status==c::error::ok&&std::ranges::equal(first.bytes(),duplicate.bytes()));
    bytes original{first.bytes().begin(),first.bytes().end()};first={};duplicate={};
    assert(peer.call(take,name).status==c::error::again);
    auto other=encoded([&](auto& out){return out.u64(78)&&out.u64(deadline);});assert(peer.call(reserve,name,other).status==c::error::busy);
    auto settlement=[&](std::uint64_t key,std::uint64_t value){return encoded([&](auto& out){return out.u64(77)&&out.u64(key)&&out.u64(sequence)&&out.u64(value);});};
    assert(peer.call(acknowledge,name,settlement(1,nonce+1)).status==c::error::denied);
    auto released=peer.call(release,name,settlement(1,nonce));assert(released.status==c::error::ok);
    auto released_again=peer.call(release,name,settlement(1,nonce));assert(released_again.status==c::error::ok&&std::ranges::equal(released.bytes(),released_again.bytes()));
    released={};released_again={};
    auto retained=peer.call(reserve,name,input);assert(retained.status==c::error::ok&&std::ranges::equal(wire::bytes{original}.subspan(40),retained.bytes().subspan(40)));
    wire::reader again{retained.bytes()};std::uint64_t ignored{},fresh{};
    assert(again.u64(ignored)&&again.u64(ignored)&&again.u64(ignored)&&again.u64(fresh)&&fresh!=nonce);
    assert(peer.call(acknowledge,name,settlement(2,nonce)).status==c::error::denied);nonce=fresh;
    auto accepted=peer.call(acknowledge,name,settlement(2,nonce));assert(accepted.status==c::error::ok&&accepted.observed==c::effect::locally_accepted);
    auto accepted_again=peer.call(acknowledge,name,settlement(2,nonce));assert(accepted_again.status==c::error::ok&&std::ranges::equal(accepted.bytes(),accepted_again.bytes()));
    assert(peer.call(release,name,settlement(2,nonce)).status==c::error::stale);
    return {retained.bytes().begin()+40,retained.bytes().end()};
}
static void native_reservation_flow(sealed_peer& sending,sealed_peer& receiving)
{
    auto deadline=[] {return c::now()+900000000ull;};
    auto text=encoded([](auto& out){return out.text("reserved route");});
    auto message=encoded([&](auto& out){return out.blob(text)&&out.u64(deadline());});
    assert(sending.call(rpc::operation::topic_publish,"publisher",message).status==c::error::ok);
    auto topic=retained_delivery(receiving,"subscription",false);assert(delivery(topic).kind=="topic.message");
    std::array<std::uint8_t,1> flag{1};auto submit=encoded([&](auto& out){return out.u64(0)&&out.blob(flag)&&out.u64(deadline());});
    assert(sending.call(rpc::operation::service_submit,"service-client",submit).status==c::error::ok);
    auto original=retained_delivery(receiving,"service-server",true);auto request=delivery(original);assert(request.kind=="service.request"&&request.correlation);
    auto response=encoded([&](auto& out){return out.encoded(flag)&&out.text("reserved service");});
    auto answer=encoded([&](auto& out){return out.u64(request.correlation)&&out.blob(response)&&out.u64(deadline());});
    assert(receiving.call(rpc::operation::service_respond,"service-server",answer).status==c::error::ok);
    auto returned=retained_delivery(sending,"service-client",true);auto result=delivery(returned);assert(result.kind=="service.response");
    wire::reader decoded{result.payload.body.subspan(1)};std::string_view value;assert(decoded.text(value)&&value=="reserved service"&&decoded.done());
}
int main(int argc,char** argv)
{
    ::signal(SIGPIPE,SIG_IGN);try
    {
        std::array<sealed_peer,2> peers;int at=1;for(auto& peer:peers)peer.parse(argc,argv,at);assert(at==argc);
        // External test owners have already provisioned their native processes.
        for(auto& peer:peers)peer.wait();
        t::private_link link;
        assert(link.prepare({peers[0].definition.network,peers[1].definition.network},{peers[0].child,peers[1].child},{peers[0].manifest.configuration,peers[1].manifest.configuration}));
        for(std::size_t side{};side<2;++side)
        {
            peers[side].wait(true);peers[side].channel_ready();
        }
        assert(peers[0].graph_has("/scene/robot")&&!peers[0].graph_has("/scene/peer"));
        assert(peers[1].graph_has("/scene/peer")&&!peers[1].graph_has("/scene/robot"));
        assert(!link.inspect().active&&link.activate());
        auto visible_until=c::now()+5000000000ull;bool left{},right{};
        while(c::now()<visible_until&&!(left&&right))
        {left=peers[0].graph_has("/scene/peer");right=peers[1].graph_has("/scene/robot");if(!(left&&right))::poll(nullptr,0,10);}
        assert(left&&right);
        for(auto& peer:peers)
        {
            for(auto name:{"publisher","subscription","bool-publisher","bool-subscription"})assert(peer.call(rpc::operation::topic_open,name).status==c::error::ok);
            for(auto name:{"service-client","service-server"})assert(peer.call(rpc::operation::service_open,name).status==c::error::ok);
            for(auto name:{"action-client","action-server"})assert(peer.call(rpc::operation::action_open,name).status==c::error::ok);
        }
        std::array<exercise,2> tests;auto until=c::now()+12000000000ull;
        while(c::now()<until&&!(tests[0].complete()&&tests[1].complete()))
        {for(std::size_t i{};i<2;++i)tests[i].tick(peers[i]);::poll(nullptr,0,2);}
        for(std::size_t i{};i<2;++i)
        {auto const& test=tests[i];std::fprintf(stderr,"sealed peer %zu flags=%d%d%d%d%d%d ticks=%u\n",i,test.string_received,test.bool_received,test.service_received,test.service_served,test.action_received,test.action_served,test.ticks);assert(test.complete());}
        native_reservation_flow(peers[0],peers[1]);native_reservation_flow(peers[1],peers[0]);
        std::puts("PASS actual native receive reservations: topic and service both directions, original record retained, changed consumer/nonce refused, release/reborrow and exact acknowledgement replay");
        auto final=link.inspect();assert(!final.fenced&&!final.uncertain);for(auto count:final.forwarded)assert(count);
        peers[1].stop();until=c::now()+1000000000ull;while(!link.inspect().fenced&&c::now()<until)::poll(nullptr,0,1);assert(link.inspect().fenced);peers[0].stop();
        std::puts("PASS actual sealed peer ROS2: native String/Bool, SetBool, LookupTransform both directions, graph visible only after bilateral attachment, private pipes, external session fencing; deployment-owned process survival");return 0;
    }
    catch(std::exception const& failure){std::fprintf(stderr,"sealed-pair fixture: %s\n",failure.what());return 1;}
}
#endif
