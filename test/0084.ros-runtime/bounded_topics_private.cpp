/** @file
 * @brief Actual generic ROS topics between two sealed, direction-limited workers.
 * @details Uses native generated PoseWithCovariance, Float64MultiArray and
 * JointState factories. The existing closed topic/service/action paths run in
 * the same processes to check codec adaptation. No guest permission is inferred
 * from these supervisor-owned test RPC bindings; VM carrier tests are separate.
 */
#define UWVM_COMMUNICATION_PRIVATE_FIXTURE_ONLY
#include "communication_private_ros2.cpp"
static bytes payload(char const* directory,std::size_t index,char const* suffix)
{
    uwvm2::utils::container::std_string path{directory};path+='/';path+=static_cast<char>('0'+index);path+=suffix;
    int fd=::open(path.c_str(),O_RDONLY|O_CLOEXEC);assert(fd>=0);bytes result;std::array<std::uint8_t,4096> block;
    for(;;){auto count=::read(fd,block.data(),block.size());assert(count>=0);if(!count)break;result.insert(result.end(),block.begin(),block.begin()+count);assert(result.size()<=4096);}
    assert(::close(fd)==0);return result;
}
int main(int argc,char** argv)
{
    ::signal(SIGPIPE,SIG_IGN);assert(argc>2);
    try
    {
        std::array<bytes,3> little,big;for(std::size_t i{};i<3;++i){little[i]=payload(argv[1],i,".little");big[i]=payload(argv[1],i,".big");}
        constexpr std::array<std::string_view,3> publishers{"bounded.0.publisher","bounded.1.publisher","bounded.2.publisher"};
        constexpr std::array<std::string_view,3> subscriptions{"bounded.0.subscription","bounded.1.subscription","bounded.2.subscription"};
        std::array<sealed_peer,2> peers;int at=2;for(auto& peer:peers)peer.parse(argc,argv,at);assert(at==argc);
        // External test owners have already provisioned their native processes.
        for(auto& peer:peers)peer.wait();
        t::private_link link;assert(link.prepare({peers[0].definition.network,peers[1].definition.network},{peers[0].child,peers[1].child},{peers[0].manifest.configuration,peers[1].manifest.configuration}));
        for(std::size_t side{};side<2;++side)
        {
            peers[side].wait(true);peers[side].channel_ready();
        }
        assert(peers[0].graph_has("/scene/robot")&&!peers[0].graph_has("/scene/peer"));
        assert(peers[1].graph_has("/scene/peer")&&!peers[1].graph_has("/scene/robot"));
        for(auto& peer:peers)
        {
            auto body=encoded([&](auto& out){return out.blob(little[0])&&out.u64(c::now()+900000000ull);});
            auto refused=peer.call(rpc::operation::topic_publish,publishers[0],body);assert(refused.status==c::error::bad_handle&&refused.observed==c::effect::none);
        }
        assert(!link.inspect().active&&link.activate());
        auto until=c::now()+5000000000ull;bool left{},right{};
        while(c::now()<until&&!(left&&right))
        {left=peers[0].graph_has("/scene/peer");right=peers[1].graph_has("/scene/robot");if(!(left&&right))::poll(nullptr,0,10);}
        assert(left&&right);
        for(auto& peer:peers)
        {
            for(auto name:{"publisher","subscription","bool-publisher","bool-subscription"})assert(peer.call(rpc::operation::topic_open,name).status==c::error::ok);
            for(auto name:{"service-client","service-server"})assert(peer.call(rpc::operation::service_open,name).status==c::error::ok);
            for(auto name:{"action-client","action-server"})assert(peer.call(rpc::operation::action_open,name).status==c::error::ok);
            for(std::size_t i{};i<3;++i)
            {
                assert(peer.call(rpc::operation::topic_open,publishers[i]).status==c::error::ok);
                assert(peer.call(rpc::operation::topic_open,subscriptions[i]).status==c::error::ok);
                // Trailing bytes and truncation must fail before native publish.
                auto malformed=little[i];malformed.push_back(0);
                for(auto bytes:{wire::bytes{malformed},wire::bytes{little[i]}.first(little[i].size()-1)})
                {auto body=encoded([&](auto& out){return out.blob(bytes)&&out.u64(c::now()+900000000ull);});
                 auto refused=peer.call(rpc::operation::topic_publish,publishers[i],body);assert(refused.status==c::error::invalid&&refused.observed==c::effect::none);}
            }
        }
        std::array<exercise,2> legacy;std::array<std::array<bool,3>,2> delivered{};std::size_t ticks{};
        auto complete=[&]{return legacy[0].complete()&&legacy[1].complete()&&std::ranges::all_of(delivered,[](auto const& rows){return std::ranges::all_of(rows,[](bool v){return v;});});};
        until=c::now()+12000000000ull;
        while(c::now()<until&&!complete())
        {
            ++ticks;
            for(std::size_t side{};side<2;++side)
            {
                auto& peer=peers[side];legacy[side].tick(peer);
                for(std::size_t i{};i<3;++i)
                {
                    if(ticks%10==0)
                    {auto body=encoded([&](auto& out){return out.blob(side?little[i]:big[i])&&out.u64(c::now()+900000000ull);});assert(peer.call(rpc::operation::topic_publish,publishers[i],body).status==c::error::ok);}
                    auto result=peer.call(rpc::operation::topic_take,subscriptions[i]);if(result.status==c::error::again)continue;
                    assert(result.status==c::error::ok);auto got=delivery(result.bytes());assert(got.kind=="topic.message"&&got.payload.encoding=="cdr");
                    // Serialized endpoints retain the sender's explicit CDR
                    // byte order; neither side silently rewrites the payload.
                    assert(std::ranges::equal(got.payload.body,side?big[i]:little[i]));delivered[side][i]=true;
                }
            }
            ::poll(nullptr,0,2);
        }
        assert(complete());
        for(auto& peer:peers)for(auto name:publishers)
        {
            assert(peer.call(rpc::operation::resource_close,name).status==c::error::ok);
            auto body=encoded([&](auto& out){return out.blob(little[0])&&out.u64(c::now()+900000000ull);});
            auto refused=peer.call(rpc::operation::topic_publish,name,body);assert(refused.status==c::error::bad_handle&&refused.observed==c::effect::none);
        }
        auto final=link.inspect();assert(!final.fenced&&!final.uncertain);for(auto count:final.forwarded)assert(count);
        peers[1].stop();until=c::now()+1000000000ull;while(!link.inspect().fenced&&c::now()<until)::poll(nullptr,0,1);assert(link.inspect().fenced);
        peers[0].stop();
        std::puts("PASS actual sealed bounded topics: three generated types, six directions, both byte orders, strict input rejection, legacy topic/service/action parity and pidfd cleanup");return 0;
    }
    catch(std::exception const& failure){std::fprintf(stderr,"bounded native pair: %s\n",failure.what());return 1;}
}
