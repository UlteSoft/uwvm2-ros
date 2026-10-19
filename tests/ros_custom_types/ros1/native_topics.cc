/*************************************************************
 * Prepared actual ROS1 fixed custom topic binding fixture.  *
 * Copyright (c) 2025-present UlteSoft. APL-2.0 License.      *
 *************************************************************/
#include "../../../gateways/ros1/custom_topics.h"
#include <uwvm_custom_ros1_fixture/Packet.h>
#include <array>
#include <algorithm>
#include <atomic>
#include <cerrno>
#include <csignal>
#include <chrono>
#include <cstring>
#include <limits>
#include <stdexcept>
#include <thread>
#include <type_traits>
#include <unistd.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <ros/subscribe_options.h>
#include <ros/xmlrpc_manager.h>
#include <xmlrpcpp/XmlRpcClient.h>

namespace custom = uwvm2::ros_gateway::ros1_custom;
namespace gateway = uwvm2::ros_gateway;
namespace core = uwvm2::imported::ros;
namespace ipc = core::ipc;

namespace
{
    void require(bool condition, char const* reason)
    { if(!condition) { throw std::runtime_error(reason); } }

    /** Emit one bounded failure record without allocating in either process. */
    template<std::size_t N>
    bool exception_record(char const (&prefix)[N],std::exception const& error) noexcept
    {
        std::array<char,N-1+1024+1> record{};std::memcpy(record.data(),prefix,N-1);
        char const* reason{error.what()};std::size_t const length{reason?::strnlen(reason,1024):0};
        if(length)std::memcpy(record.data()+N-1,reason,length);
        record[N-1+length]='\n';return gateway::native_io::write_audit_text(std::string_view{record.data(),N-1+length+1});
    }

    /** Construct an explicit topic grant; provider registration supplies no grant. */
    template<typename Codec>
    core::capability_spec grant(char const* alias, char const* operation, std::string const& resource)
    {
        core::capability_spec cap{alias, operation, resource, std::string{Codec::type}, std::string{Codec::native_identity}, {}};
        cap.limits = {{"adapter-codec-identity", std::string{Codec::codec_identity}},
            {"max-message-bytes", "23"}, {"max-rate-hz", "1000"}, {"burst", "4"},
            {"max-bytes-per-second", "23000"}, {"max-queue-bytes", "184"},
            {"queue-depth", "8"}, {"transport", "tcpros"}, {"overflow-policy", "reject"},
            {"validator", "none"}, {"audit", "allow-and-deny"}, {"audit-max-rate-hz", "100"},
            {"audit-burst", "10"}, {"audit-max-records", "1000"}, {"audit-max-bytes", "65536"},
            {std::string{operation} == "topic.publish" ? "max-publishers" : "max-subscribers", "1"}};
        if(std::string{operation} == "topic.publish") { cap.limits.emplace("latch", "false"); }
        return cap;
    }

    template<typename Native, typename Codec>
    void preflight()
    {
        static_assert(Codec::wire_bytes == 23, "Fixture package and canonical layout must agree");
        auto cap{grant<Codec>("publish", "topic.publish", "/uwvm_custom_fixed/native_packet")};
        require(custom::binding<Native, Codec>::known(cap), "official native name/MD5, descriptor and codec must agree before ros::init");
        auto const native_definition{::ros::message_traits::Definition<Native>::value()};
        require(native_definition && custom::matches_definition(Codec::type, std::string_view{native_definition},
            Codec::definitions, Codec::definition_count), "complete official native field and dependency definition must match the generated schema");
        std::string changed_definition{native_definition};
        auto const field{changed_definition.find("bool active")};
        require(field != std::string::npos, "official fixture definition must contain its native bool field");
        changed_definition.replace(field, sizeof("bool active") - 1, "uint8 active");
        require(!custom::matches_definition(Codec::type, changed_definition, Codec::definitions, Codec::definition_count),
            "equal native storage for bool/uint8 must not hide a different official schema");
        auto missing{cap}; missing.limits.erase("adapter-codec-identity");
        require(!custom::binding<Native, Codec>::known(missing), "missing explicit codec identity must be denied");
        auto wrong{cap}; wrong.limits["adapter-codec-identity"] = "wrong";
        require(!custom::binding<Native, Codec>::known(wrong), "wrong explicit codec identity must be denied");
        wrong = cap; wrong.type_identity = "00000000000000000000000000000000";
        require(!custom::binding<Native, Codec>::known(wrong), "wrong official native identity must be denied");
        wrong = cap; wrong.operation = "service.call";
        require(!custom::binding<Native, Codec>::known(wrong), "custom topic cannot authorize a native Service");
        wrong = cap; wrong.operation = "action.send-goal";
        require(!custom::binding<Native, Codec>::known(wrong), "custom topic cannot authorize a native Action");
    }

    /** A single bounded native control channel; this is not Guest/Gateway IPC. */
    enum class peer_command : std::uint8_t { ready=1, valid=2, nonfinite=3, boolean=4, stop=5, outbound=6, success=7 };
    using peer_record=std::array<std::uint8_t,24>;
    using deadline_type=std::chrono::steady_clock::time_point;

    template<typename Predicate>
    void phase_wait(Predicate predicate,char const* reason)
    {
        auto const deadline{std::chrono::steady_clock::now()+std::chrono::seconds(10)};
        bool ready{predicate()};
        while(!ready&&std::chrono::steady_clock::now()<deadline)
        { std::this_thread::sleep_for(std::chrono::milliseconds(10));ready=predicate(); }
        require(ready,reason);
    }

    /** Fixed datagrams preserve message boundaries and never allocate. */
    bool transfer_ready(int fd,short events,deadline_type deadline)
    {
        while(std::chrono::steady_clock::now()<deadline)
        {
            pollfd poller{fd,events,0};int const result{::poll(&poller,1,100)};
            if(result<0&&errno==EINTR) { continue; }
            if(result<0) { return false; }
            if(result&&poller.revents&events) { return true; }
            if(result&&poller.revents&(POLLERR|POLLHUP|POLLNVAL)) { return false; }
        }
        return false;
    }

    bool send_record(int fd,peer_record const& record) noexcept
    {
        auto const deadline{std::chrono::steady_clock::now()+std::chrono::seconds(10)};
        while(transfer_ready(fd,POLLOUT,deadline))
        {
            auto const count{::send(fd,record.data(),record.size(),MSG_NOSIGNAL)};
            if(count<0&&(errno==EINTR||errno==EAGAIN||errno==EWOULDBLOCK)) { continue; }
            return count==static_cast<ssize_t>(record.size());
        }
        return false;
    }

    peer_record receive_record(int fd,unsigned seconds=10)
    {
        auto const deadline{std::chrono::steady_clock::now()+std::chrono::seconds(seconds)};
        peer_record record{};
        while(transfer_ready(fd,POLLIN,deadline))
        {
            auto const count{::recv(fd,record.data(),record.size(),MSG_TRUNC)};
            if(count<0&&(errno==EINTR||errno==EAGAIN||errno==EWOULDBLOCK)) { continue; }
            require(count==static_cast<ssize_t>(record.size()),"native peer control record must have its exact bounded length");
            return record;
        }
        throw std::runtime_error("bounded native peer control receive failed");
    }

    void command(int fd,peer_command operation)
    { peer_record record{};record[0]=static_cast<std::uint8_t>(operation);require(send_record(fd,record),"bounded native peer command send failed"); }

    /** Reap this exact child before its PID can be reused, including exceptions. */
    struct peer_owner
    {
        int fd;pid_t pid;
        peer_owner(int channel,pid_t child):fd{channel},pid{child}{}
        peer_owner(peer_owner const&)=delete;
        peer_owner& operator=(peer_owner const&)=delete;
        ~peer_owner()
        {
            if(fd>=0) { ::close(fd); }
            if(pid>0)
            {
                int status{};auto result{::waitpid(pid,&status,WNOHANG)};
                if(result==0)
                {
                    // An unreaped direct child keeps its PID reserved. Only
                    // this fork result is signalled; no process-group signals.
                    (void)::kill(pid,SIGCONT);(void)::kill(pid,SIGTERM);
                    auto const deadline{std::chrono::steady_clock::now()+std::chrono::seconds(2)};
                    do { result=::waitpid(pid,&status,WNOHANG);if(result!=0)break;std::this_thread::sleep_for(std::chrono::milliseconds(10)); }
                    while(std::chrono::steady_clock::now()<deadline);
                    if(result==0) { (void)::kill(pid,SIGKILL);while(::waitpid(pid,&status,0)<0&&errno==EINTR){} }
                }
            }
        }
        void complete()
        {
            auto const record{receive_record(fd)};
            require(record[0]==static_cast<std::uint8_t>(peer_command::success)&&
                std::all_of(record.begin()+1,record.end(),[](std::uint8_t x){return x==0;}),"native peer must acknowledge all phases before success");
            int status{};pid_t result{};
            phase_wait([&]{result=::waitpid(pid,&status,WNOHANG);return result!=0;},"native peer must terminate and be reaped within its deadline");
            require(result==pid,"native peer success must reap the exact fork child");pid=-1;
            require(WIFEXITED(status)&&WEXITSTATUS(status)==0,"native peer must exit successfully after its exact outbound witness");
        }
    };

    template<typename Native>
    Native fixed_sample()
    {
        Native sample{};sample.sequence=-12345;sample.active=1;sample.measurement=-1.25;
        sample.tag[0]=0;sample.tag[1]=127;sample.tag[2]=128;sample.tag[3]=255;
        sample.samples[0]=-32768;sample.samples[1]=0;sample.samples[2]=32767;
        return sample;
    }

    /** Bind a native message to the opposite process's genuine ROS caller ID. */
    template<typename Native>
    bool from_node(::ros::MessageEvent<Native const> const& event,std::string const& name)
    {
        auto const header{event.getConnectionHeaderPtr()};if(!header)return false;
        auto const caller{header->find("callerid")};return caller!=header->end()&&caller->second==name;
    }

    template<typename Native,typename Callback>
    ::ros::Subscriber witness_subscription(::ros::NodeHandle& node,std::string const& resource,Callback callback)
    {
        using event_type=::ros::MessageEvent<Native const>;
        ::boost::function<void(event_type const&)> function{callback};::ros::SubscribeOptions options;
        options.template initByFullCallbackType<event_type const&>(resource,8,function);
        options.transport_hints=::ros::TransportHints{}.tcp();return node.subscribe(options);
    }

    /** Query our live node through its actual XMLRPC interface, with a deadline. */
    void require_tcpros(std::string const& inbound,std::string const& outbound,std::string const& peer_name,
                       std::string const& peer_uri,pid_t peer_pid)
    {
        auto const port{::ros::XMLRPCManager::instance()->getServerPort()};
        require(port&&port<=65535,"native node must expose its genuine XMLRPC endpoint");
        ::XmlRpc::XmlRpcClient client{"127.0.0.1",static_cast<int>(port),"/"};
        ::XmlRpc::XmlRpcValue parameters,result;parameters[0]=::ros::this_node::getName();
        require(client.executeNonBlock("getBusInfo",parameters),"actual native getBusInfo request must start");
        // executeCheckDone observes state only. Advance this standalone
        // client's public dispatcher without blocking before checking it.
        phase_wait([&]{client._disp.work(0.0);return client.executeCheckDone(result);},"actual native getBusInfo must complete within its deadline");
        require(!client.isFault()&&result.getType()==::XmlRpc::XmlRpcValue::TypeArray&&result.size()==3&&
            result[0].getType()==::XmlRpc::XmlRpcValue::TypeInt&&static_cast<int>(result[0])==1&&
            result[2].getType()==::XmlRpc::XmlRpcValue::TypeArray&&result[2].size()<=64,"actual native getBusInfo response must be bounded and successful");
        bool received{},sent{};
        for(int n{};n!=result[2].size();++n)
        {
            auto& row{result[2][n]};if(row.getType()!=::XmlRpc::XmlRpcValue::TypeArray||row.size()<6||row.size()>7)continue;
            if(row[1].getType()!=::XmlRpc::XmlRpcValue::TypeString||row[2].getType()!=::XmlRpc::XmlRpcValue::TypeString||
                row[3].getType()!=::XmlRpc::XmlRpcValue::TypeString||row[4].getType()!=::XmlRpc::XmlRpcValue::TypeString||
                row[5].getType()!=::XmlRpc::XmlRpcValue::TypeBoolean||!static_cast<bool>(row[5])||static_cast<std::string>(row[3])!="TCPROS")continue;
            if(static_cast<std::string>(row[2])=="i"&&static_cast<std::string>(row[4])==inbound&&static_cast<std::string>(row[1])==peer_uri)received=true;
            if(static_cast<std::string>(row[2])=="o"&&static_cast<std::string>(row[4])==outbound&&static_cast<std::string>(row[1])==peer_name)sent=true;
        }
        require(received&&sent,"actual inbound and outbound bus rows must both be connected TCPROS to the fork peer");
        auto const proof{std::string{"TRANSPORT ROS1 inbound=TCPROS outbound=TCPROS child_pid="}+std::to_string(peer_pid)+" parent_pid="+std::to_string(::getpid())+"\n"};
        require(gateway::native_io::write_all(STDOUT_FILENO,proof),"bounded actual TCPROS proof output must succeed");
    }

    template<typename Native,typename Codec>
    void native_peer(int channel,int argc,char** argv,std::string const& base,std::string const& parent_name)
    {
        auto const node_name{std::string{"uwvm_custom_ros1_peer_"}+std::to_string(::getpid())};
        ::ros::init(argc,argv,node_name,::ros::init_options::NoSigintHandler|::ros::init_options::NoRosout);
        require(::ros::this_node::getName()=="/"+node_name,"native peer node name must remain exact");
        ::ros::NodeHandle node;::ros::AsyncSpinner spinner{2};spinner.start();
        auto sender{node.advertise<Native>(base+"/inbound",8,false)};
        auto const sample{fixed_sample<Native>()};std::array<std::uint8_t,23> expected{};
        require(Codec::valid(sample)&&Codec::encode(sample,expected.data(),expected.size()),"peer fixed sample must encode exactly");
        std::atomic<bool> observed{},foreign{},channel_failure{};
        auto witness{witness_subscription<Native>(node,base+"/outbound",[&](::ros::MessageEvent<Native const> const& event)
        {
            if(!from_node(event,parent_name)) { foreign=true;return; }
            auto const message{event.getMessage()};std::array<std::uint8_t,23> actual{};
            if(!message||!Codec::valid(*message)||!Codec::encode(*message,actual.data(),actual.size())||actual!=expected)return;
            if(!observed.exchange(true))
            { peer_record record{};record[0]=static_cast<std::uint8_t>(peer_command::outbound);std::copy(actual.begin(),actual.end(),record.begin()+1);if(!send_record(channel,record))channel_failure=true; }
        })};
        peer_record ready{};ready[0]=static_cast<std::uint8_t>(peer_command::ready);
        std::uint32_t const port{::ros::XMLRPCManager::instance()->getServerPort()};std::memcpy(ready.data()+1,&port,sizeof(port));
        require(send_record(channel,ready),"native peer readiness must enter its bounded channel");
        unsigned phases{};bool finished{};
        for(unsigned n{};n!=4;++n)
        {
            // Thirty seconds allow the parent's bounded witness/publication and
            // live bus-information phases between control commands.
            auto const record{receive_record(channel,30)};
            require(std::all_of(record.begin()+1,record.end(),[](std::uint8_t x){return x==0;}),"native peer commands must contain no undeclared payload");
            peer_command const operation = static_cast<peer_command>(record[0]);
            if(operation==peer_command::stop)
            {require(phases==7&&observed&&!foreign&&!channel_failure,"peer success requires every native transmission and opposite exact-byte witness");finished=true;break;}
            unsigned bit{};Native outgoing{sample};
            if(operation==peer_command::valid)bit=1;
            else if(operation==peer_command::nonfinite){bit=2;outgoing.measurement=std::numeric_limits<double>::quiet_NaN();require(!Codec::valid(outgoing),"peer native NaN must remain invalid");}
            else if(operation==peer_command::boolean){bit=4;outgoing.active=2;require(!Codec::valid(outgoing),"peer native bool byte 2 must remain invalid");}
            else throw std::runtime_error("unknown native peer control operation");
            require(!(phases&bit),"native peer transmission phases cannot be repeated");
            phase_wait([&]{return sender.getNumSubscribers()>=1;},"native peer sender must discover the opposite process subscriber");
            sender.publish(outgoing);phases|=bit;
        }
        require(finished,"native peer must receive explicit successful completion");
        sender.shutdown();witness.shutdown();spinner.stop();::ros::shutdown();
        peer_record success{};success[0]=static_cast<std::uint8_t>(peer_command::success);
        require(send_record(channel,success),"native peer final acknowledgement must succeed");
    }

    /**
     * Exercise real roscpp publishers/subscribers through the reusable binding.
     * This fixture intentionally tests that binding, not Wasm caller identity or
     * authenticated IPC. Those remain separate required Worker/Gateway tests.
     * Its PASS marker cannot be used as proof of the complete original G01 task.
     */
    template<typename Native, typename Codec>
    void exercise(::ros::NodeHandle& node,peer_owner& peer,std::string const& base,std::string const& peer_name,std::string const& peer_uri)
    {
        using adapter = custom::binding<Native, Codec>;
        std::map<std::string, ::ros::Publisher> publishers;
        std::map<std::string, ::ros::Subscriber> subscribers;
        custom::topic_store store{node, publishers, subscribers};
        require(publishers.empty() && subscribers.empty(), "loading a provider must not create native entities");
        auto publishing{grant<Codec>("publish", "topic.publish", base+"/outbound")};
        auto subscribing{grant<Codec>("subscribe", "topic.subscribe", base+"/inbound")};
        auto const sample{fixed_sample<Native>()};
        require(Codec::valid(sample), "the fixed native sample must satisfy generated constraints");
        std::vector<std::uint8_t> expected(Codec::wire_bytes);
        require(Codec::encode(sample, expected.data(), expected.size()), "bounded native encode must succeed");
        auto const* descriptor{ipc::find_custom_topic(1U, publishing.type)};
        require(descriptor && ipc::validate_custom_topic(*descriptor, expected.data(), expected.size()), "shared canonical validator must accept the exact generated layout");

        // No publisher is constructed until the complete canonical body passes.
        auto malformed{expected}; malformed[4] = 2;
        require(adapter::publish(store, publishing, malformed).error == 22 && publishers.empty(), "noncanonical bool must be rejected before native publisher creation");
        malformed = expected; malformed.push_back(0);
        require(adapter::publish(store, publishing, malformed).error == 90 && publishers.empty(), "extra byte exceeding the explicit quota must be rejected before native publisher creation");
        auto structural_cap{publishing}; structural_cap.limits["max-message-bytes"] = "24";
        require(adapter::publish(store, structural_cap, malformed).error == 22 && publishers.empty(), "trailing bytes within the explicit quota must fail the exact structural validator before publisher creation");
        malformed = expected;
        for(unsigned n{}; n != 8; ++n) { malformed[5 + n] = static_cast<std::uint8_t>(0x7ff8000000000000ULL >> (8 * n)); }
        require(adapter::publish(store, publishing, malformed).error == 22 && publishers.empty(), "nonfinite canonical fields must be rejected before native publisher creation");

        auto queue{std::make_shared<gateway::event_queue>(subscribing)};
        require(!adapter::subscribe(store, subscribing, queue).error && subscribers.size() == 1, "explicit custom subscription must create one typed native entity");
        std::atomic<unsigned> native_samples{}, invalid_native_samples{};
        std::atomic<bool> foreign_publisher{};
        auto witness{witness_subscription<Native>(node,subscribing.resource,[&](::ros::MessageEvent<Native const> const& event)
        {
            if(!from_node(event,peer_name)) {foreign_publisher=true;return;}
            auto const received{event.getMessage()};if(!received)return;
            if(!Codec::valid(*received)) {++invalid_native_samples;return;}
            std::array<std::uint8_t,23> actual{};
            if(Codec::encode(*received,actual.data(),actual.size())&&std::equal(actual.begin(),actual.end(),expected.begin()))++native_samples;
        })};
        command(peer.fd,peer_command::valid);
        ipc::call_result inbound{11,{}};
        phase_wait([&]{inbound=queue->poll();return inbound.error!=11;},"actual opposite-process native custom message must reach the queue");
        require(!inbound.error&&inbound.payload==expected,"actual native custom message must enter the bounded queue with exact content");
        phase_wait([&]{return native_samples.load()!=0;},"raw native witness must receive exact inbound content from the peer caller ID");
        require(!foreign_publisher,"inbound witness must never accept a different publisher caller ID");

        // A separate native witness confirms the invalid message traversed ROS;
        // the custom adapter must reject it before canonical output allocation.
        Native nonfinite{sample}; nonfinite.measurement = std::numeric_limits<double>::quiet_NaN();
        require(!Codec::valid(nonfinite), "native nonfinite prevalidation must be allocation-free and reject NaN");
        command(peer.fd,peer_command::nonfinite);
        phase_wait([&]{return invalid_native_samples.load()!=0;},"actual opposite-process native witness must observe malformed nonfinite data");
        require(invalid_native_samples != 0, "actual native witness must observe the malformed native field");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        require(queue->poll().error == 11, "invalid actual native data must not reach the custom event queue");

        // Official ROS1 bool members have byte storage. A native value of 2
        // must be rejected, rather than normalized into a canonical true value.
        Native invalid_boolean{sample}; invalid_boolean.active = 2;
        require(!Codec::valid(invalid_boolean), "noncanonical native bool must be rejected before allocation");
        auto invalid_before{invalid_native_samples.load()};command(peer.fd,peer_command::boolean);
        phase_wait([&]{return invalid_native_samples.load()>invalid_before;},"actual opposite-process native witness must observe malformed boolean data");
        require(invalid_native_samples.load() > invalid_before, "actual native witness must observe noncanonical native bool");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        require(queue->poll().error == 11, "noncanonical actual native bool must not reach the custom event queue");

        auto changed_subscription{subscribing}; changed_subscription.limits["max-rate-hz"] = "1";
        auto changed_queue{std::make_shared<gateway::event_queue>(changed_subscription)};
        require(adapter::subscribe(store, changed_subscription, changed_queue).error == 13,
            "existing native subscription cannot silently reuse a changed complete grant");

        // Outbound content has a different topic. Only the fork child's raw
        // subscriber can witness it, so no local callback can satisfy this proof.
        require(!adapter::publish(store,publishing,expected).error,"explicit canonical custom publication must create one native publisher");
        phase_wait([&]{return store.publishers.at(publishing.alias).getNumSubscribers()>=1;},"outbound custom publisher must discover the opposite process witness");
        require(!adapter::publish(store,publishing,expected).error,"explicit canonical custom publication must traverse the established native connection");
        auto const acknowledgement{receive_record(peer.fd)};
        require(acknowledgement[0]==static_cast<std::uint8_t>(peer_command::outbound)&&
            std::equal(acknowledgement.begin()+1,acknowledgement.end(),expected.begin()),"opposite native peer must acknowledge exact outbound canonical bytes");
        require(!foreign_publisher,"all malformed inbound witnesses must retain the opposite publisher caller ID");
        require_tcpros(subscribing.resource,publishing.resource,peer_name,peer_uri,peer.pid);
        auto changed_publication{publishing}; changed_publication.limits["max-rate-hz"] = "1";
        require(adapter::publish(store, changed_publication, expected).error == 13,
            "existing native publisher cannot silently reuse a changed complete grant");
        // The reusable store also deactivates its retained weak queue. Its close
        // operation must remain safe when used directly by a native fixture.
        store.close(subscribing.alias); store.close(publishing.alias);
        require(publishers.empty() && subscribers.empty() && store.retained_grants.empty() &&
            store.subscription_verified.empty() && store.subscription_queues.empty() && queue->poll().error == 125,
            "close must remove native alias resources, grant snapshots, callback gates and retire the queue");
        witness.shutdown();command(peer.fd,peer_command::stop);peer.complete();
    }

    // Keep the selection in a dependent template. A provider may contain other
    // fixed messages whose members and sizes differ from this specific fixture.
    template<typename Native, typename Codec>
    void select_preflight(bool& selected)
    {
        if constexpr(std::is_same_v<Native, ::uwvm_custom_ros1_fixture::Packet>)
        { preflight<Native, Codec>(); selected = true; }
    }

    template<typename Native, typename Codec>
    void select_exercise(::ros::NodeHandle& node,peer_owner& peer,std::string const& base,std::string const& peer_name,std::string const& peer_uri)
    {
        if constexpr(std::is_same_v<Native, ::uwvm_custom_ros1_fixture::Packet>)
        { exercise<Native, Codec>(node,peer,base,peer_name,peer_uri); }
    }

    template<typename Native,typename Codec>
    void select_peer(int channel,int argc,char** argv,std::string const& base,std::string const& parent_name,bool& selected)
    {
        if constexpr(std::is_same_v<Native,::uwvm_custom_ros1_fixture::Packet>)
        {native_peer<Native,Codec>(channel,argc,argv,base,parent_name);selected=true;}
    }
}

int main(int argc, char** argv)
{
    try
    {
        bool selected{};
#define UWVM_CUSTOM_ROS1_PREFLIGHT(Native, Type, Identity, CodecIdentity, WireBytes, Codec) \
        select_preflight<Native, Codec>(selected);
        UWVM_ROS_CUSTOM_ROS1_TYPES(UWVM_CUSTOM_ROS1_PREFLIGHT)
#undef UWVM_CUSTOM_ROS1_PREFLIGHT
        require(selected, "the generated native provider must register the exact official Packet fixture");
        auto const parent_name{std::string{"uwvm_custom_ros1_parent_"}+std::to_string(::getpid())};
        auto const base{std::string{"/uwvm_custom_fixed/native_packet_"}+std::to_string(::getpid())};
        int channels[2]{};
        require(::socketpair(AF_UNIX,SOCK_SEQPACKET|SOCK_CLOEXEC|SOCK_NONBLOCK,0,channels)==0,"one bounded native peer socketpair must be created");
        // Fork before either side starts any ROS thread or creates a NodeHandle.
        // The controller pins and observes this inherited-cgroup descendant.
        pid_t const child{::fork()};
        if(child<0) {::close(channels[0]);::close(channels[1]);throw std::runtime_error("native peer fork failed");}
        if(child==0)
        {
            ::close(channels[0]);int status=1;
            try
            {
                bool selected_peer{};
#define UWVM_CUSTOM_ROS1_PEER(Native, Type, Identity, CodecIdentity, WireBytes, Codec) \
                select_peer<Native,Codec>(channels[1],argc,argv,base,"/"+parent_name,selected_peer);
                UWVM_ROS_CUSTOM_ROS1_TYPES(UWVM_CUSTOM_ROS1_PEER)
#undef UWVM_CUSTOM_ROS1_PEER
                require(selected_peer,"the generated provider must select the exact native peer fixture");status=0;
            }
            catch(std::exception const& error)
            {
                ::ros::shutdown();
                char const marker[]{"FAIL ROS1 fixed custom topic native peer: "};
                if(!exception_record(marker,error))status=1;
            }
            catch(...) {::ros::shutdown();(void)gateway::native_io::write_audit_text("FAIL ROS1 fixed custom topic native peer\n");}
            ::close(channels[1]);::_exit(status);
        }
        ::close(channels[1]);peer_owner peer{channels[0],child};
        auto const readiness{receive_record(peer.fd)};
        require(readiness[0]==static_cast<std::uint8_t>(peer_command::ready)&&
            std::all_of(readiness.begin()+5,readiness.end(),[](std::uint8_t x){return x==0;}),"fork peer readiness must have its closed fixed layout");
        std::uint32_t peer_port{};std::memcpy(&peer_port,readiness.data()+1,sizeof(peer_port));
        require(peer_port&&peer_port<=65535,"fork peer readiness must bind its actual native XMLRPC port");
        auto const peer_name{std::string{"/uwvm_custom_ros1_peer_"}+std::to_string(child)};
        auto const peer_uri{std::string{"http://127.0.0.1:"}+std::to_string(peer_port)+"/"};
        ::ros::init(argc,argv,parent_name,::ros::init_options::NoSigintHandler|::ros::init_options::NoRosout);
        require(::ros::this_node::getName()=="/"+parent_name,"parent native fixture node name must remain exact");
        // The adapter callback is registered before the raw witness. One
        // callback thread preserves that queue order: observing malformed
        // content in the witness proves the adapter callback has completed.
        ::ros::NodeHandle node;::ros::AsyncSpinner spinner{1};spinner.start();
#define UWVM_CUSTOM_ROS1_EXERCISE(Native, Type, Identity, CodecIdentity, WireBytes, Codec) \
        select_exercise<Native,Codec>(node,peer,base,peer_name,peer_uri);
        UWVM_ROS_CUSTOM_ROS1_TYPES(UWVM_CUSTOM_ROS1_EXERCISE)
#undef UWVM_CUSTOM_ROS1_EXERCISE
        spinner.stop(); ::ros::shutdown();
        // The C++17 native Gateway cannot include the vendored C++20 fast_io.
        // Reuse the native writer for the one small diagnostic marker.
        char const marker[]{"PASS ROS1 fixed custom topic native binding exact_body=23\n"};
        return gateway::native_io::write_all(STDOUT_FILENO, std::string_view{marker, sizeof(marker) - 1}) ? 0 : 1;
    }
    catch(std::exception const& error)
    {
        ::ros::shutdown();
        // Keep the real failed assertion visible without allocating from the
        // exception handler. The existing native writer receives one bounded
        // record, with an explicit final newline, and its result is checked.
        char const prefix[]{"FAIL ROS1 fixed custom topic native binding: "};
        if(!exception_record(prefix,error)) { return 1; }
        return 1;
    }
    catch(...)
    {
        ::ros::shutdown();
        char const marker[]{"FAIL ROS1 fixed custom topic native binding\n"};
        (void)gateway::native_io::write_audit_text(std::string_view{marker, sizeof(marker) - 1});
        return 1;
    }
}
