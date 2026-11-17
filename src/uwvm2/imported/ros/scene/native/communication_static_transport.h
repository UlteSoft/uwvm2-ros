/** @file
 * @brief Public FastDDS custom transport over four sealed-direction broker pipes.
 * @details No socket operation is implemented by this transport. All receiver
 * pumps are created during trusted prepare and wait behind an activation gate.
 * The sandbox attaches already-approved anonymous pipes only after sealing all
 * threads. Startup discovery is retained in finite reserved queues. After
 * activation every packet is checked against the same participant/direction
 * plan as the host broker. A partial write failure fences the transport because
 * the next frame boundary cannot be safely recovered. Native FastDDS container
 * ABI allocations remain in the isolated worker; all owned backing here uses
 * the recoverable fast_io allocator.
 */
#pragma once
#include "external_process_only.h"
#include "communication_network.h"
#include "../communication_transport_broker.h"
#include <fastdds/rtps/transport/TransportInterface.h>
#include <condition_variable>

namespace uwvm2::imported::ros::scene::communication::ros2
{
#if defined(__linux__)
    namespace pipe_transport=::uwvm2::imported::ros::scene::communication_transport;
    namespace public_rtps=::eprosima::fastdds::rtps;
    namespace old_rtps=::eprosima::fastrtps::rtps;
    inline public_rtps::Locator loopback_locator(std::uint16_t port)
    {public_rtps::Locator value;value.kind=LOCATOR_KIND_UDPv4;value.port=port;std::memset(value.address,0,sizeof(value.address));value.address[12]=127;value.address[15]=1;return value;}
    inline bool loopback_address(public_rtps::Locator const& value) noexcept
    {auto fixed=loopback_locator(static_cast<std::uint16_t>(value.port));return value.kind==LOCATOR_KIND_UDPv4&&value.port<=65535&&std::memcmp(value.address,fixed.address,16)==0;}

    class static_pipe_state
    {
        struct channel
        {
            static_pipe_state* parent{};std::size_t index{};pthread_t thread{};bool started{};
            std::mutex send_mutex;std::recursive_mutex receiver_mutex;
            public_rtps::TransportReceiverInterface* receiver{};
            pipe_transport::packet_queue startup;
            ::uwvm2::utils::container::std_vector<std::uint8_t> input,output;
            std::uint64_t outgoing{1},incoming{1};int rx{-1},tx{-1};
        };
        std::array<channel,2> channels;
        std::mutex gate_mutex;std::condition_variable gate;
        std::atomic<bool> attached{},stopped{},failed{};
        bool initialized{};::pid_t process{::getpid()};
        bool transfer(int fd,std::span<std::uint8_t> bytes,bool sending,std::chrono::steady_clock::time_point deadline) noexcept
        {
            std::size_t at{};
            while(at<bytes.size()&&!stopped.load(std::memory_order_acquire))
            {
                auto n=sending ? ::write(fd,bytes.data()+at,bytes.size()-at) : ::read(fd,bytes.data()+at,bytes.size()-at);
                if(n>0){at+=static_cast<std::size_t>(n);continue;}
                if(!n)return false;
                if(errno==EINTR)continue;
                if(errno!=EAGAIN&&errno!=EWOULDBLOCK)return false;
                auto now=std::chrono::steady_clock::now();if(now>=deadline)return false;
                auto delay=std::chrono::duration_cast<std::chrono::milliseconds>(deadline-now).count();
                pollfd wait{fd,static_cast<short>(sending?POLLOUT:POLLIN),0};
                if(::poll(&wait,1,static_cast<int>(std::min<std::int64_t>(20,std::max<std::int64_t>(1,delay))))<0&&errno!=EINTR)return false;
                if(wait.revents&(POLLERR|POLLHUP|POLLNVAL))return false;
            }
            return at==bytes.size();
        }
        bool send_locked(channel& c,wire::bytes data,std::chrono::steady_clock::time_point deadline) noexcept
        {
            if(c.outgoing==UINT64_MAX)return false;
            if(!pipe_transport::encode_header({c.output.data(),pipe_transport::header_bytes},{approved.generation,c.outgoing,data.size()}))return false;
            std::memcpy(c.output.data()+pipe_transport::header_bytes,data.data(),data.size());
            if(!transfer(c.tx,{c.output.data(),pipe_transport::header_bytes+data.size()},true,deadline))return false;
            ++c.outgoing;return true;
        }
        static void* entry(void* pointer) noexcept
        {auto& c=*static_cast<channel*>(pointer);c.parent->receive(c);return nullptr;}
        void receive(channel& c) noexcept
        {
            sigset_t mask;::sigemptyset(&mask);::sigaddset(&mask,SIGPIPE);::pthread_sigmask(SIG_BLOCK,&mask,nullptr);
            {std::unique_lock lock{gate_mutex};gate.wait(lock,[&]{return attached.load()||stopped.load();});}
            if(stopped.load())return;
            {
                std::lock_guard lock{c.send_mutex};
                while(!c.startup.empty())
                {
                    if(!send_locked(c,c.startup.payload(),std::chrono::steady_clock::now()+std::chrono::nanoseconds{approved.frame_deadline_ns})){fence();return;}
                    c.startup.pop();
                }
            }
            while(!stopped.load(std::memory_order_acquire))
            {
                pollfd wait{c.rx,POLLIN,0};auto result=::poll(&wait,1,20);
                if(result<0){if(errno==EINTR)continue;fence();return;}
                if(wait.revents&(POLLERR|POLLHUP|POLLNVAL)){fence();return;}if(!(wait.revents&POLLIN))continue;
                auto deadline=std::chrono::steady_clock::now()+std::chrono::nanoseconds{approved.frame_deadline_ns};
                if(!transfer(c.rx,{c.input.data(),pipe_transport::header_bytes},false,deadline)){fence();return;}
                pipe_transport::frame_header header;
                if(!pipe_transport::decode_header({c.input.data(),pipe_transport::header_bytes},approved,c.incoming,header)||c.incoming==UINT64_MAX){fence();return;}
                auto size=static_cast<std::size_t>(header.length);
                if(!transfer(c.rx,{c.input.data()+pipe_transport::header_bytes,size},false,deadline)){fence();return;}
                wire::bytes payload{c.input.data()+pipe_transport::header_bytes,size};
                auto direction=c.index?communication_worker::pipe_role::user_rx:communication_worker::pipe_role::discovery_rx;
                if(!pipe_transport::packet_allowed(approved,direction,payload)){fence();return;}++c.incoming;
                std::lock_guard lock{c.receiver_mutex};
                if(c.receiver)
                {
                    try{c.receiver->OnDataReceived(payload.data(),static_cast<std::uint32_t>(size),loopback_locator(c.index?approved.local_user:approved.local_discovery),loopback_locator(approved.peer_source));}
                    catch(...){fence();return;}
                }
            }
        }
    public:
        pipe_transport::plan const approved;
        explicit static_pipe_state(pipe_transport::plan value):approved(value){}
        ~static_pipe_state(){stop();}
        static_pipe_state(static_pipe_state const&)=delete;
        bool prepare() noexcept
        {
            if(process!=::getpid()||initialized||!approved.valid())return false;
            try
            {
                for(std::size_t i{};i<2;++i)
                {
                    auto& c=channels[i];c.parent=this;c.index=i;
                    c.input.resize(approved.max_datagram_bytes+pipe_transport::header_bytes);c.output.resize(c.input.size());
                    if(approved.allowed[i*2+1]&&!c.startup.initialize(static_cast<std::size_t>(approved.queues[i*2+1].packets),approved.max_datagram_bytes))return false;
                }
                for(auto& c:channels){if(::pthread_create(&c.thread,nullptr,entry,&c)){stop();return false;}c.started=true;}
                initialized=true;return true;
            }
            catch(...){stop();return false;}
        }
        bool activate(std::span<communication_worker::role_fd const> fds) noexcept
        {
            if(process!=::getpid()||!initialized||attached.load()||stopped.load()||fds.size()!=4)return false;
            std::array<int,4> values{-1,-1,-1,-1};
            for(auto const& fd:fds){auto i=pipe_transport::index(fd.role);if(i>=4||fd.descriptor<0||values[i]>=0)return false;values[i]=fd.descriptor;}
            for(std::size_t i{};i<4;++i)for(std::size_t j{};j<i;++j)if(values[i]==values[j])return false;
            {std::lock_guard lock{gate_mutex};for(std::size_t i{};i<2;++i){channels[i].rx=values[i*2];channels[i].tx=values[i*2+1];}attached.store(true,std::memory_order_release);}
            gate.notify_all();return true;
        }
        void fence() noexcept{failed.store(true);stopped.store(true);gate.notify_all();}
        bool healthy() const noexcept{return process==::getpid()&&!failed.load()&&!stopped.load();}
        void stop() noexcept
        {
            stopped.store(true);gate.notify_all();if(process!=::getpid())return;
            for(auto& c:channels)if(c.started){::pthread_join(c.thread,nullptr);c.started=false;}
        }
        bool bind(std::size_t route,public_rtps::TransportReceiverInterface* receiver) noexcept
        {if(route>=2||!receiver||!healthy())return false;auto& c=channels[route];std::lock_guard lock{c.receiver_mutex};if(c.receiver&&c.receiver!=receiver)return false;c.receiver=receiver;return true;}
        bool unbind(std::size_t route) noexcept
        {if(route>=2)return false;auto& c=channels[route];std::lock_guard lock{c.receiver_mutex};bool present=c.receiver!=nullptr;c.receiver=nullptr;return present;}
        bool bound(std::size_t route) noexcept
        {if(route>=2)return false;std::lock_guard lock{channels[route].receiver_mutex};return channels[route].receiver!=nullptr;}
        bool send(std::size_t route,wire::bytes payload,std::chrono::steady_clock::time_point deadline) noexcept
        {
            if(route>=2||!healthy())return false;auto direction=route?communication_worker::pipe_role::user_tx:communication_worker::pipe_role::discovery_tx;
            if(!pipe_transport::packet_allowed(approved,direction,payload))return false;
            auto& c=channels[route];std::lock_guard lock{c.send_mutex};
            if(!attached.load(std::memory_order_acquire))
            {if(!c.startup.push(approved.generation,1,payload)){fence();return false;}return true;}
            // Preserve queued startup ordering if an existing DDS thread wins the gate race.
            while(!c.startup.empty())
            {if(!send_locked(c,c.startup.payload(),deadline)){fence();return false;}c.startup.pop();}
            if(!send_locked(c,payload,deadline)){fence();return false;}return true;
        }
    };

    class static_pipe_transport;
    struct static_pipe_descriptor final:public public_rtps::TransportDescriptorInterface
    {
        std::shared_ptr<static_pipe_state> state;
        explicit static_pipe_descriptor(std::shared_ptr<static_pipe_state> owner):TransportDescriptorInterface(owner->approved.max_datagram_bytes,1),state(std::move(owner)){}
        public_rtps::TransportInterface* create_transport() const override;
        std::uint32_t min_send_buffer_size() const override{return maxMessageSize;}
    };
    class static_pipe_transport final:public public_rtps::TransportInterface
    {
        static_pipe_descriptor config;
        std::shared_ptr<static_pipe_state> state;
        struct sender final:old_rtps::SenderResource
        {
            explicit sender(static_pipe_transport* transport):SenderResource(LOCATOR_KIND_UDPv4)
            {
                send_lambda_=[transport](old_rtps::octet const* data,std::uint32_t size,old_rtps::LocatorsIterator* begin,old_rtps::LocatorsIterator* end,std::chrono::steady_clock::time_point const& deadline)
                {
                    bool result=true;auto& it=*begin;
                    while(it!=*end){auto route=transport->remote_route(*it);if(route<2)result=transport->state->send(route,{data,size},deadline)&&result;else result=false;++it;}return result;
                };
            }
            static void operator delete(void* pointer) noexcept
            {::uwvm2::utils::allocator::fast_io_strict::native_strict_typed_global_allocator<sender>::deallocate(static_cast<sender*>(pointer));}
        };
        std::size_t local_route(public_rtps::Locator const& locator) const noexcept
        {if(!loopback_address(locator))return 2;if(locator.port==state->approved.local_discovery)return 0;if(locator.port==state->approved.local_user)return 1;return 2;}
        std::size_t remote_route(public_rtps::Locator const& locator) const noexcept
        {if(!loopback_address(locator))return 2;if(locator.port==state->approved.peer_discovery)return 0;if(locator.port==state->approved.peer_user)return 1;return 2;}
    public:
        explicit static_pipe_transport(static_pipe_descriptor const& value):TransportInterface(LOCATOR_KIND_UDPv4),config(value),state(value.state){}
        ~static_pipe_transport() override{state->stop();}
        static void operator delete(void* pointer) noexcept
        {::uwvm2::utils::allocator::fast_io_strict::native_strict_typed_global_allocator<static_pipe_transport>::deallocate(static_cast<static_pipe_transport*>(pointer));}
        bool init(old_rtps::PropertyPolicy const*,std::uint32_t const&) override{return state->prepare();}
        bool IsInputChannelOpen(public_rtps::Locator const& l) const override{return state->bound(local_route(l));}
        bool IsLocatorSupported(public_rtps::Locator const& l) const override{return loopback_address(l)&&(local_route(l)<2||remote_route(l)<2);}
        bool is_locator_allowed(public_rtps::Locator const& l) const override{return IsLocatorSupported(l);}
        public_rtps::Locator RemoteToMainLocal(public_rtps::Locator const& l) const override
        {auto route=remote_route(l);return route<2?loopback_locator(route?state->approved.local_user_source:state->approved.local_discovery_source):public_rtps::Locator{};}
        bool transform_remote_locator(public_rtps::Locator const& in,public_rtps::Locator& out) const override
        {if(remote_route(in)>=2)return false;out=in;return true;}
        bool OpenOutputChannel(public_rtps::SendResourceList& list,public_rtps::Locator const& l) override
        {
            if(remote_route(l)>=2&&l!=loopback_locator(state->approved.local_discovery_source)&&l!=loopback_locator(state->approved.local_user_source))return false;
            for(auto const& resource:list)if(dynamic_cast<sender*>(resource.get()))return true;
            try
            {
                auto* resource=::uwvm2::utils::allocator::fast_io_strict::object<sender>([this](sender* raw){return ::new(static_cast<void*>(raw)) sender(this);});
                std::unique_ptr<old_rtps::SenderResource> owned{resource};list.push_back(std::move(owned));return true;
            }
            catch(...){return false;}
        }
        bool OpenInputChannel(public_rtps::Locator const& l,public_rtps::TransportReceiverInterface* r,std::uint32_t) override{return state->bind(local_route(l),r);}
        bool CloseInputChannel(public_rtps::Locator const& l) override{return state->unbind(local_route(l));}
        bool DoInputLocatorsMatch(public_rtps::Locator const& a,public_rtps::Locator const& b) const override{return local_route(a)<2&&a==b;}
        public_rtps::LocatorList NormalizeLocator(public_rtps::Locator const& l) override
        {public_rtps::LocatorList result;if(IsLocatorSupported(l))result.push_back(l);return result;}
        void select_locators(old_rtps::LocatorSelector& selector) const override
        {
            auto& entries=selector.transport_starts();
            for(std::size_t i{};i<entries.size();++i)
            {auto* e=entries[i];if(!e->transport_should_process)continue;bool selected{};for(std::size_t n{};n<e->unicast.size();++n)if(remote_route(e->unicast[n])<2&&!selector.is_selected(e->unicast[n])){e->state.unicast.push_back(n);selected=true;}if(selected)selector.select(i);}
        }
        bool is_local_locator(public_rtps::Locator const& l) const override{return local_route(l)<2;}
        bool is_localhost_allowed() const override{return true;}
        public_rtps::TransportDescriptorInterface* get_configuration() override{return &config;}
        void AddDefaultOutputLocator(public_rtps::LocatorList& list) override{list.push_back(loopback_locator(state->approved.local_discovery_source));}
        bool getDefaultMetatrafficMulticastLocators(public_rtps::LocatorList&,std::uint32_t) const override{return false;}
        bool getDefaultMetatrafficUnicastLocators(public_rtps::LocatorList& list,std::uint32_t port) const override
        {if(port!=state->approved.local_discovery)return false;list.push_back(loopback_locator(state->approved.local_discovery));return true;}
        bool getDefaultUnicastLocators(public_rtps::LocatorList& list,std::uint32_t port) const override
        {if(port!=state->approved.local_user)return false;list.push_back(loopback_locator(state->approved.local_user));return true;}
        bool fillMetatrafficMulticastLocator(public_rtps::Locator&,std::uint32_t) const override{return false;}
        bool fillMetatrafficUnicastLocator(public_rtps::Locator& l,std::uint32_t port) const override
        {if(port!=state->approved.local_discovery)return false;if(!l.port)l=loopback_locator(state->approved.local_discovery);return local_route(l)==0;}
        bool fillUnicastLocator(public_rtps::Locator& l,std::uint32_t port) const override
        {if(port!=state->approved.local_user)return false;if(!l.port)l=loopback_locator(state->approved.local_user);return local_route(l)==1;}
        bool configureInitialPeerLocator(public_rtps::Locator& l,old_rtps::PortParameters const&,std::uint32_t domain,public_rtps::LocatorList& list) const override
        {if(domain!=state->approved.domain||remote_route(l)!=0)return false;list.push_back(l);return true;}
        std::uint32_t max_recv_buffer_size() const override{return state->approved.max_datagram_bytes;}
        void shutdown() override{state->stop();}
    };
    inline public_rtps::TransportInterface* static_pipe_descriptor::create_transport() const
    {
        try{return ::uwvm2::utils::allocator::fast_io_strict::object<static_pipe_transport>([this](static_pipe_transport* raw){return ::new(static_cast<void*>(raw)) static_pipe_transport(*this);});}
        catch(...){return nullptr;}
    }

    /** @brief Trusted single-domain worker profile installed before ROS startup.
     * @details This is the custom transport half of the sandbox deployment. It
     * must be paired with the independent startup closure and syscall seal;
     * invoking this class in a normal process does not create a sandbox.
     */
    class static_network final:public network_policy
    {
        std::shared_ptr<static_pipe_state> state;std::shared_ptr<static_pipe_descriptor> transport;
        ::pid_t process{::getpid()};bool installed{};
    public:
        explicit static_network(pipe_transport::plan const& plan):state(::uwvm2::utils::container::make_shared<static_pipe_state>(plan)),transport(::uwvm2::utils::container::make_shared<static_pipe_descriptor>(state)){}
        bool valid() const noexcept override
        {
            if(process!=::getpid()||!installed||!state->healthy())return false;
            auto const& p=eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->get_default_participant_qos();
            auto const& w=p.wire_protocol();auto const& a=state->approved;
            return !p.transport().use_builtin_transports&&p.transport().user_transports.size()==1&&p.transport().user_transports[0].get()==transport.get()&&
                w.participant_id==static_cast<std::int32_t>(a.local_participant)&&std::equal(a.local_prefix.begin(),a.local_prefix.end(),w.prefix.value)&&
                w.builtin.metatrafficMulticastLocatorList.empty()&&w.builtin.metatrafficUnicastLocatorList.size()==1&&
                w.builtin.metatrafficUnicastLocatorList.begin()->port==a.local_discovery&&w.builtin.initialPeersList.size()==1&&w.builtin.initialPeersList.begin()->port==a.peer_discovery;
        }
        bool permits_domain(std::uint32_t domain) const noexcept override{return domain==state->approved.domain&&valid();}
        bool activate(std::span<communication_worker::role_fd const> fds) noexcept{return valid()&&state->activate(fds);}
        bool healthy() const noexcept{return state->healthy();}
        static std::shared_ptr<static_network> install(pipe_transport::plan const& approved)
        {
            if(!approved.valid())return {};
            for(auto const& context:rclcpp::get_contexts())if(context&&context->is_valid())return {};
            auto factory=eprosima::fastdds::dds::DomainParticipantFactory::get_instance();
            for(unsigned domain{};domain<=232;++domain)if(!factory->lookup_participants(domain).empty())return {};
            // An inherited transport/security profile is a conflict, never a silent downgrade.
            for(auto key:{"FASTDDS_DEFAULT_PROFILES_FILE","FASTRTPS_DEFAULT_PROFILES_FILE","ROS_STATIC_PEERS","ROS_DISCOVERY_SERVER","ROS_SUPER_CLIENT","FASTDDS_ENVIRONMENT_FILE","FASTDDS_BUILTIN_TRANSPORTS","ROS_SECURITY_ENABLE","ROS_SECURITY_STRATEGY","ROS_SECURITY_KEYSTORE","ROS_SECURITY_ENCLAVE_OVERRIDE","ROS_LOCALHOST_ONLY","RMW_FASTRTPS_USE_QOS_FROM_XML"})if(std::getenv(key))return {};
            for(auto const& pair:std::array<std::pair<char const*,char const*>,4>{{{"RMW_IMPLEMENTATION","rmw_fastrtps_cpp"},{"ROS_AUTOMATIC_DISCOVERY_RANGE","SYSTEM_DEFAULT"},{"SKIP_DEFAULT_XML","1"},{"RMW_FASTRTPS_PUBLICATION_MODE","SYNCHRONOUS"}}})
            {auto old=std::getenv(pair.first);if(old&&std::strcmp(old,pair.second))return {};if(::setenv(pair.first,pair.second,1))return {};}
            auto value=::uwvm2::utils::container::make_shared<static_network>(approved);
            factory->load_profiles();auto qos=factory->get_default_participant_qos();
            qos.transport().use_builtin_transports=false;qos.transport().user_transports.clear();qos.transport().user_transports.push_back(value->transport);
            auto& wire=qos.wire_protocol();wire.participant_id=static_cast<std::int32_t>(approved.local_participant);std::copy(approved.local_prefix.begin(),approved.local_prefix.end(),wire.prefix.value);
            wire.builtin.metatrafficMulticastLocatorList.clear();wire.builtin.metatrafficUnicastLocatorList.clear();wire.builtin.metatrafficUnicastLocatorList.push_back(loopback_locator(approved.local_discovery));
            wire.builtin.initialPeersList.clear();wire.builtin.initialPeersList.push_back(loopback_locator(approved.peer_discovery));
            wire.default_unicast_locator_list.clear();wire.default_unicast_locator_list.push_back(loopback_locator(approved.local_user));wire.default_multicast_locator_list.clear();
            if(factory->set_default_participant_qos(qos)!=eprosima::fastrtps::types::ReturnCode_t::RETCODE_OK)return {};
            value->installed=true;return value->valid()?value:nullptr;
        }
    };
#endif
}
