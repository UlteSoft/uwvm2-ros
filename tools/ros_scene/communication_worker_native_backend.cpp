/** @file
 * @brief Digest-pinned single-node ROS2 worker behind a closed static transport.
 * @details Prepare opens exactly the approved context/node with no external
 * pipes; endpoint creation waits for the separate sealed RPC after attachment.
 * Fixed ROS type-support libraries are eagerly resolved in the private image.
 * Owned state and buffers use nullable/recoverable fast_io allocation. ROS ABI
 * objects remain external-library storage, bounded by the worker process limits.
 */
#include <uwvm2/imported/ros/scene/communication_worker_configuration.h>
#include <uwvm2/imported/ros/scene/communication_worker_protocol.h>
#include <uwvm2/imported/ros/scene/communication_receive_contract.h>
#include <uwvm2/imported/ros/scene/native/communication_static_transport.h>
#include <uwvm2/imported/ros/scene/native/communication_actions_ros2.h>
#include <uwvm2/imported/ros/scene/native/communication_graph_ros2.h>
#include <fastdds/dds/log/Log.hpp>
#include <dirent.h>
#include <dlfcn.h>
#include <charconv>
#include <cstdio>
#include <sys/stat.h>
#if defined(UWVM_ROS_BOUNDED_TOPICS_HEADER)
// This include is a trusted native-build choice, never a manifest/guest path.
#include UWVM_ROS_BOUNDED_TOPICS_HEADER
#endif
namespace s=uwvm2::imported::ros::scene;
namespace c=s::communication;
namespace cfg=s::communication_configuration;
namespace manifest=s::communication_worker_configuration;
namespace rpc=s::communication_worker_protocol;
namespace startup=s::communication_worker;
namespace wire=s::wire;
namespace containers=uwvm2::utils::container;
namespace contract=uwvm2::imported::ros::communication_contract;
namespace action_contract=uwvm2::imported::ros::action_contract;
namespace receive_contract=uwvm2::imported::ros::communication_receive_contract;
namespace
{
    using log=eprosima::fastdds::dds::Log;
    /** @brief Bounded stderr record; no worker log can corrupt stdout RPC. */
    class log_sink final:public eprosima::fastdds::dds::LogConsumer
    {
    public:
        static void operator delete(void* p) noexcept
        {uwvm2::utils::allocator::fast_io_strict::native_strict_typed_global_allocator<log_sink>::deallocate(static_cast<log_sink*>(p));}
        void Consume(log::Entry const& entry) override
        {
            std::array<char,1024> bytes{};constexpr std::string_view prefix="FastDDS: ";std::copy(prefix.begin(),prefix.end(),bytes.begin());
            auto count=std::min(entry.message.size(),bytes.size()-prefix.size()-1);std::memcpy(bytes.data()+prefix.size(),entry.message.data(),count);bytes[prefix.size()+count]='\n';
            // Stderr is a bounded nonblocking parent pipe. Dropped diagnostics do
            // not alter a ROS result or open an unapproved logging destination.
            (void)::write(2,bytes.data(),prefix.size()+count+1);
        }
    };
    struct state
    {
        manifest::specification specification;
        std::shared_ptr<c::ros2::static_network> network;
        std::shared_ptr<c::ros2::provider> provider;
        std::shared_ptr<c::registry> registry;
        containers::std_vector<c::handle> resources;
        // RPC sequencing is connection-wide. Native exact-retry keys are local
        // to each resource and must advance only when that resource commits a
        // receipt; interleaved inspections/takes never consume a mutation key.
        containers::std_vector<std::uint64_t> mutation_sequences;
        containers::std_vector<std::shared_ptr<c::actions::endpoint>> actions;
        containers::std_vector<std::shared_ptr<c::graph::source>> graphs;
        containers::std_vector<std::uint8_t> input;
        rpc::ledger history;
        std::size_t context_index{},node_index{};
        bool attached{};
        c::owner owner(std::size_t index) const noexcept{return {1,index+1,specification.network.generation};}
        c::handle context() const noexcept{return resources[context_index];}
        c::handle node() const noexcept{return resources[node_index];}
        ~state(){for(auto& action:actions)if(action)action->retire();graphs.clear();actions.clear();if(registry)registry->retire();}
    };
    std::shared_ptr<state> current;
    bool prewarm_logs()
    {
        auto p=uwvm2::utils::allocator::fast_io_strict::native_strict_typed_global_allocator<log_sink>::allocate(1);if(!p)return false;
        std::construct_at(p);log::ClearConsumers();log::RegisterConsumer(std::unique_ptr<eprosima::fastdds::dds::LogConsumer>{p});
        log::SetVerbosity(log::Error);log::QueueLog("closed communication worker startup",{__FILE__,__LINE__,__func__,"scene"},log::Error);log::Flush();return true;
    }
    bool preload()
    {
        // These literal SONAMEs refer only to digest-pinned files in the private
        // worker closure. No guest or manifest path reaches dlopen.
        constexpr char const* libraries[]={
            "librmw_fastrtps_cpp.so","libstd_msgs__rosidl_typesupport_fastrtps_cpp.so","libstd_srvs__rosidl_typesupport_fastrtps_cpp.so",
            "libtf2_msgs__rosidl_typesupport_fastrtps_cpp.so","libgeometry_msgs__rosidl_typesupport_fastrtps_cpp.so",
            "libbuiltin_interfaces__rosidl_typesupport_fastrtps_cpp.so","libaction_msgs__rosidl_typesupport_fastrtps_cpp.so",
            "libunique_identifier_msgs__rosidl_typesupport_fastrtps_cpp.so","librosgraph_msgs__rosidl_typesupport_fastrtps_cpp.so",
            "librcl_interfaces__rosidl_typesupport_fastrtps_cpp.so","librmw_dds_common__rosidl_typesupport_fastrtps_cpp.so",
            "librmw_dds_common__rosidl_typesupport_introspection_cpp.so","libstd_msgs__rosidl_typesupport_introspection_cpp.so",
            "libstd_srvs__rosidl_typesupport_introspection_cpp.so","libtf2_msgs__rosidl_typesupport_introspection_cpp.so",
            "libgeometry_msgs__rosidl_typesupport_introspection_cpp.so","libbuiltin_interfaces__rosidl_typesupport_introspection_cpp.so",
            "libaction_msgs__rosidl_typesupport_introspection_cpp.so","libunique_identifier_msgs__rosidl_typesupport_introspection_cpp.so",
            "librosgraph_msgs__rosidl_typesupport_introspection_cpp.so","librcl_interfaces__rosidl_typesupport_introspection_cpp.so",
            "libservice_msgs__rosidl_typesupport_introspection_cpp.so"};
        for(auto name:libraries)if(!::dlopen(name,RTLD_NOW|RTLD_LOCAL)){std::fprintf(stderr,"worker preload failed: %s: %s\n",name,::dlerror());return false;}
#if defined(UWVM_ROS_BOUNDED_TOPICS_HEADER)
        if(!c::ros2::approved_bounded_topics::preload())return false;
#endif
        return true;
    }
    bool notifications(startup::prepared_backend& output)
    {
        auto directory=::opendir("/proc/self/fd");if(!directory)return false;
        bool success=true;auto directory_fd=::dirfd(directory);
        while(auto entry=::readdir(directory))
        {
            std::string_view name{entry->d_name};int fd{};auto parsed=std::from_chars(name.data(),name.data()+name.size(),fd);
            if(parsed.ec!=std::errc{}||parsed.ptr!=name.data()+name.size()||fd<4||fd==directory_fd)continue;
            std::array<char,64> path{};auto length=std::snprintf(path.data(),path.size(),"/proc/self/fd/%d",fd);if(length<=0||static_cast<std::size_t>(length)>=path.size()){success=false;break;}
            std::array<char,128> target{};auto count=::readlink(path.data(),target.data(),target.size());if(count<=0||static_cast<std::size_t>(count)==target.size()){success=false;break;}
            std::string_view link{target.data(),static_cast<std::size_t>(count)};startup::notification_kind kind;
            if(link=="anon_inode:[eventfd]")kind=startup::notification_kind::event;
            else if(link=="anon_inode:[eventpoll]")kind=startup::notification_kind::poll;
            else if(link.starts_with("pipe:["))kind=startup::notification_kind::pipe;
            else{std::fprintf(stderr,"worker undeclared native descriptor %d: %.*s\n",fd,static_cast<int>(link.size()),link.data());success=false;break;}
            auto flags=::fcntl(fd,F_GETFL);if(flags<0||output.notification_count==output.notifications.size()){success=false;break;}
            auto direction=(flags&O_ACCMODE)==O_RDONLY?startup::io_direction::read:(flags&O_ACCMODE)==O_WRONLY?startup::io_direction::write:startup::io_direction::both;
            output.notifications[output.notification_count++]={fd,direction,kind};
        }
        ::closedir(directory);return success;
    }
    bool initialize(state& value)
    {
        containers::std_vector<std::shared_ptr<c::ros2::bounded_topic_factory>> bounded;
#if defined(UWVM_ROS_BOUNDED_TOPICS_HEADER)
        if(c::ros2::approved_bounded_topics::prepare(value.specification,bounded)!=c::error::ok)return false;
#else
        // A schema catalogue alone cannot install native code. Refuse used
        // bounded topic types before creating any ROS context or participant.
        for(auto const& item:value.specification.definitions)if(item.kind==cfg::family::topic)
        {
            auto schema=manifest::find(value.specification,cfg::family::type,std::get<c::topic_plan>(item.policy).type);
            if(schema&&std::get<c::type_plan>(schema->policy).validate.bounded())return false;
        }
#endif
        auto& spec=value.specification;value.network=c::ros2::static_network::install(spec.network);if(!value.network){std::fputs("worker static_network failed\n",stderr);return false;}
        value.provider=containers::make_shared<c::ros2::provider>(c::ros2::builtin_types(),value.network,c::ros2::builtin_services(),+[](std::string_view stage,std::string_view reason)noexcept
        {std::fprintf(stderr,"worker native %.*s: %.*s\n",static_cast<int>(std::min<std::size_t>(stage.size(),32)),stage.data(),static_cast<int>(std::min<std::size_t>(reason.size(),1024)),reason.data());},std::move(bounded));
        c::limits limits{spec.capacity.contexts,spec.capacity.nodes,spec.capacity.topics,spec.capacity.types,spec.capacity.queue_bytes,spec.response_bytes,spec.capacity.services};
        value.registry=containers::make_shared<c::registry>(value.provider,limits);value.resources.resize(spec.definitions.size());value.mutation_sequences.resize(spec.definitions.size());value.actions.resize(spec.definitions.size());value.graphs.resize(spec.definitions.size());value.input.resize(rpc::header_bytes+spec.request_bytes);
        if(!value.history.prepare(spec.network.generation,spec.request_bytes,spec.response_bytes))return false;
        for(std::size_t i{};i<spec.definitions.size();++i)
        {
            auto& definition=spec.definitions[i];auto status=c::error::ok;
            std::visit([&](auto& policy)
            {
                if constexpr(requires{policy.authority;}){policy.authority=value.owner(i);status=value.registry->configure(policy);}
                else policy.policy.authority=value.owner(i);
            },definition.policy);
            if(status!=c::error::ok){std::fprintf(stderr,"worker configure slot=%zu family=%u status=%u\n",i,unsigned(definition.kind),unsigned(status));return false;}
            if(definition.kind==cfg::family::context)value.context_index=i;
            if(definition.kind==cfg::family::node)value.node_index=i;
        }
        auto const& context=std::get<c::context_plan>(spec.definitions[value.context_index].policy);
        auto const& node=std::get<c::node_plan>(spec.definitions[value.node_index].policy);
        auto opened=value.registry->open_context(context.authority,context.selected,value.resources[value.context_index]);
        if(opened!=c::error::ok){std::fprintf(stderr,"worker open context failed status=%u\n",unsigned(opened));return false;}
        opened=value.registry->open_node(node.authority,context.authority,value.context(),node.selected,value.resources[value.node_index]);
        if(opened!=c::error::ok)std::fprintf(stderr,"worker open node failed status=%u\n",unsigned(opened));
        return opened==c::error::ok;
    }
    bool transfer(int fd,std::span<std::uint8_t> bytes,bool output,std::uint64_t timeout,std::uint64_t& deadline,bool idle=false) noexcept
    {
        if(!deadline&&!idle)deadline=c::now()+timeout;std::size_t offset{};
        while(offset<bytes.size())
        {
            auto got=output?::write(fd,bytes.data()+offset,bytes.size()-offset) : ::read(fd,bytes.data()+offset,bytes.size()-offset);
            if(got>0){if(!deadline)deadline=c::now()+timeout;offset+=static_cast<std::size_t>(got);continue;}if(!got)return false;
            if(errno==EINTR)continue;if(errno!=EAGAIN&&errno!=EWOULDBLOCK)return false;
            if(deadline&&c::now()>=deadline)return false;pollfd ready{fd,static_cast<short>(output?POLLOUT:POLLIN),0};if(::poll(&ready,1,20)<0&&errno!=EINTR)return false;
        }
        return true;
    }
    c::type_plan const* schema(state const& value,c::profile const& profile) noexcept
    {auto found=manifest::find(value.specification,cfg::family::type,profile);return found?&std::get<c::type_plan>(found->policy):nullptr;}
    /** @brief Execute only a sealed manifest slot, with all bounds before effects.
     * @details Host authority is enforced before sending this private request;
     * the worker independently enforces the slot's fixed node/schema/direction,
     * quotas and state machine. A slot is never a guest-controlled native handle.
     * Successful takes commit into the exact-retry ledger before pipe delivery.
     */
    rpc::result dispatch(state& value,rpc::request const& request,std::span<std::uint8_t> output)
    {
        if(!request.binding||request.binding>value.specification.definitions.size())return {c::error::denied,c::effect::none,0};
        auto index=static_cast<std::size_t>(request.binding-1);auto const& definition=value.specification.definitions[index];auto owner=value.owner(index);
        auto& id=value.resources[index];wire::writer writer{output};wire::reader input{request.body};auto action=request.action;
        c::request_key key{id,value.mutation_sequences[index]==UINT64_MAX?0:value.mutation_sequences[index]+1};
        auto answer=[&](c::error status,c::effect observed=c::effect::none){return rpc::result{status,observed,status==c::error::ok?writer.size():0};};
        auto invalid=[&]{return answer(c::error::invalid);};
        auto finish=[&](c::error status,c::receipt const& receipt)
        {if(status!=c::error::ok)return answer(status);if(receipt.key==key&&key.sequence)++value.mutation_sequences[index];
         if(!contract::encode_receipt(writer,receipt))return answer(c::error::io,c::effect::unknown);
         // The registry call returned a retained receipt; that does not make
         // the underlying native operation successful. Preserve its exact
         // numeric result even when the recovery body remains available.
         return rpc::result{receipt.status,receipt.observed,writer.size()};};
        bool mutating=action==rpc::operation::topic_publish||action==rpc::operation::topic_liveliness||action==rpc::operation::topic_acknowledged||
            action==rpc::operation::service_submit||action==rpc::operation::service_respond||action==rpc::operation::service_abandon||
            action==rpc::operation::action_submit||action==rpc::operation::action_respond||action==rpc::operation::action_cancel||action==rpc::operation::context_shutdown;
        if(mutating&&output.size()<160)return answer(c::error::too_large);
        if(action==rpc::operation::context_inspect&&definition.kind==cfg::family::context)
        {if(!input.done())return invalid();c::context_info info;auto status=value.registry->describe(owner,id,info);if(status==c::error::ok&&!contract::encode_context(writer,info))status=c::error::too_large;return answer(status);}
        if(action==rpc::operation::node_inspect&&definition.kind==cfg::family::node)
        {if(!input.done())return invalid();c::node_info info;auto status=value.registry->describe(owner,id,info);if(status==c::error::ok&&!contract::encode_node(writer,info))status=c::error::too_large;return answer(status);}
        if(action==rpc::operation::context_shutdown&&definition.kind==cfg::family::context)
        {if(!input.done())return invalid();c::receipt receipt;auto status=value.registry->shutdown(owner,id,key,receipt);return finish(status,receipt);}
        if(action==rpc::operation::resource_close)
        {
            if(!input.done())return invalid();if(output.size()<8)return answer(c::error::too_large);if(!id)return answer(c::error::bad_handle);
            c::error status;
            if(definition.kind==cfg::family::action)
            {status=value.actions[index]?value.actions[index]->close(owner):c::error::bad_handle;if(status==c::error::ok)value.actions[index].reset();}
            else if(definition.kind==cfg::family::graph){value.graphs[index].reset();status=c::error::ok;}
            else status=value.registry->close(owner,id);
            if(status==c::error::ok){writer.u64(id);id=0;value.mutation_sequences[index]=0;}return answer(status);
        }
        bool topic_receive=action==rpc::operation::topic_reserve||action==rpc::operation::topic_acknowledge||action==rpc::operation::topic_release;
        bool service_receive=action==rpc::operation::service_reserve||action==rpc::operation::service_acknowledge||action==rpc::operation::service_release;
        if(topic_receive||service_receive)
        {
            if((topic_receive&&definition.kind!=cfg::family::topic)||(service_receive&&definition.kind!=cfg::family::service))return answer(c::error::denied);
            if(!id)return answer(c::error::bad_handle);
            std::uint64_t consumer_id{};if(!input.u64(consumer_id)||!consumer_id)return invalid();
            // The authenticated host owns this opaque consumer token. It is
            // scoped to this exact worker and binding; no pointer crosses IPC.
            c::delivery_consumer consumer{&value,value.specification.network.generation,consumer_id};
            bool reserve=action==rpc::operation::topic_reserve||action==rpc::operation::service_reserve;
            if(reserve)
            {
                std::uint64_t deadline{};if(!input.u64(deadline)||!input.done())return invalid();
                auto const* type=topic_receive?schema(value,std::get<c::topic_plan>(definition.policy).type):
                    schema(value,std::get<c::service_plan>(definition.policy).direction==c::service_role::client?
                        std::get<c::service_plan>(definition.policy).response_type:std::get<c::service_plan>(definition.policy).request_type);
                if(!type)return answer(c::error::denied);
                auto maximum=topic_receive?std::get<c::topic_plan>(definition.policy).max_message_bytes:
                    std::get<c::service_plan>(definition.policy).direction==c::service_role::client?
                        std::get<c::service_plan>(definition.policy).max_response_bytes:std::get<c::service_plan>(definition.policy).max_request_bytes;
                // Reserve the entire bounded reply before pinning the original
                // row. Fixed overhead includes reference, metadata and framing.
                if(maximum>output.size()||256+type->type_identity.size()+type->encoding.size()>output.size()-maximum)return answer(c::error::too_large);
                if(topic_receive)
                {
                    auto status=value.registry->reserve_topic(owner,id,consumer,deadline,[&](c::delivery_reference reference,std::uint64_t until,c::delivery const& message)
                    {return receive_contract::encode(writer,{id,value.specification.network.generation,reference.sequence,reference.nonce},until,
                        message.information,0,"topic.message",{type->type_identity,type->encoding,message.body});});
                    return answer(status);
                }
                struct target{wire::writer* writer;c::type_plan const* type;std::uint64_t generation;};
                target destination{&writer,type,value.specification.network.generation};
                auto copy=[](void* opaque,c::delivery_reference reference,std::uint64_t until,c::service_delivery const& message)
                {
                    auto& target=*static_cast<struct target*>(opaque);
                    return receive_contract::encode(*target.writer,{reference.source,target.generation,reference.sequence,reference.nonce},until,
                        message.information,message.correlation,message.response?"service.response":"service.request",{target.type->type_identity,target.type->encoding,message.body});
                };
                return answer(value.registry->reserve_service(owner,id,consumer,deadline,&destination,copy));
            }
            std::uint64_t key{},sequence{},nonce{};
            if(!input.u64(key)||!key||!input.u64(sequence)||!sequence||!input.u64(nonce)||!nonce||!input.done())return invalid();
            if(output.size()<160)return answer(c::error::too_large);
            bool acknowledge=action==rpc::operation::topic_acknowledge||action==rpc::operation::service_acknowledge;
            c::delivery_reference reference{id,owner.generation,sequence,nonce};c::delivery_receipt result;
            auto status=topic_receive?value.registry->settle_topic(owner,id,consumer,reference,{id,key},acknowledge,result):
                value.registry->settle_service(owner,id,consumer,reference,{id,key},acknowledge,result);
            if(status!=c::error::ok)return answer(status);
            c::receipt receipt{result.request,value.specification.network.generation,result.key,result.status,result.observed};
            if(!contract::encode_receipt(writer,receipt))return answer(c::error::io,c::effect::unknown);
            return {receipt.status,receipt.observed,writer.size()};
        }
        if(definition.kind==cfg::family::topic)
        {
            auto const& plan=std::get<c::topic_plan>(definition.policy);auto type=schema(value,plan.type);if(!type)return answer(c::error::denied);
            if(action==rpc::operation::topic_open)
            {
                if(!input.done())return invalid();if(output.size()<8)return answer(c::error::too_large);if(id)return answer(c::error::busy);
                auto status=value.registry->open_topic(owner,value.owner(value.node_index),value.node(),plan.selected,plan.resolved_name,type->type_identity,plan.direction,id);
                if(status==c::error::ok)writer.u64(id);return answer(status);
            }
            if(!id)return answer(c::error::bad_handle);
            if(action==rpc::operation::topic_publish)
            {wire::bytes body;std::uint64_t deadline{};if(!input.blob(body)||!input.u64(deadline)||!input.done())return invalid();c::receipt receipt;
             auto status=value.registry->publish(owner,id,key,type->type_identity,type->encoding,body,deadline,receipt);return finish(status,receipt);}
            if(action==rpc::operation::topic_take)
            {
                if(!input.done())return invalid();bool attempted{},encoded{};auto status=value.registry->take(owner,id,[&](c::delivery const& message)
                {attempted=true;encoded=contract::encode_delivery(writer,message.information,0,"topic.message",{type->type_identity,type->encoding,message.body});return encoded;});
                return answer(status==c::error::io&&attempted&&!encoded?c::error::too_large:status);
            }
            if(action==rpc::operation::topic_liveliness)
            {if(!input.done())return invalid();c::receipt receipt;auto status=value.registry->assert_liveliness(owner,id,key,receipt);return finish(status,receipt);}
            if(action==rpc::operation::topic_acknowledged)
            {std::uint64_t deadline{};if(!input.u64(deadline)||!input.done())return invalid();c::receipt receipt;auto status=value.registry->wait_acknowledged(owner,id,deadline,receipt);return finish(status,receipt);}
        }
        if(definition.kind==cfg::family::service)
        {
            auto const& plan=std::get<c::service_plan>(definition.policy);
            if(action==rpc::operation::service_open)
            {
                if(!input.done())return invalid();if(output.size()<8)return answer(c::error::too_large);if(id)return answer(c::error::busy);
                auto status=value.registry->open_service(owner,value.owner(value.node_index),value.node(),plan.selected,plan.resolved_name,plan.service_type,plan.direction,id);
                if(status==c::error::ok)writer.u64(id);return answer(status);
            }
            if(!id)return answer(c::error::bad_handle);
            if(action==rpc::operation::service_submit||action==rpc::operation::service_respond)
            {
                wire::bytes body;std::uint64_t deadline{},correlation{};if(!input.u64(correlation)||!input.blob(body)||!input.u64(deadline)||!input.done())return invalid();
                bool response=action==rpc::operation::service_respond;if(!response&&correlation)return invalid();
                auto type=schema(value,response?plan.response_type:plan.request_type);if(!type)return answer(c::error::denied);c::receipt receipt;
                auto status=value.registry->service_mutate(owner,id,key,correlation,type->type_identity,type->encoding,body,deadline,response,receipt);return finish(status,receipt);
            }
            if(action==rpc::operation::service_take)
            {
                if(!input.done())return invalid();struct target{wire::writer* writer;state const* value;c::service_plan const* plan;bool attempted{},encoded{};};target sink{&writer,&value,&plan};
                auto copy=[](void* opaque,c::service_delivery const& message)
                {auto& into=*static_cast<target*>(opaque);auto type=schema(*into.value,message.response?into.plan->response_type:into.plan->request_type);if(!type)return false;
                 into.attempted=true;into.encoded=contract::encode_delivery(*into.writer,message.information,message.correlation,message.response?"service.response":"service.request",{type->type_identity,type->encoding,message.body});return into.encoded;};
                auto status=value.registry->service_take(owner,id,&sink,copy);return answer(status==c::error::io&&sink.attempted&&!sink.encoded?c::error::too_large:status);
            }
            if(action==rpc::operation::service_available)
            {if(!input.done())return invalid();if(output.size()<8)return answer(c::error::too_large);bool available{};auto status=value.registry->service_available(owner,id,available);if(status==c::error::ok)writer.boolean(available);return answer(status);}
            if(action==rpc::operation::service_abandon)
            {std::uint64_t correlation{};if(!input.u64(correlation)||!input.done())return invalid();c::receipt receipt;auto status=value.registry->service_abandon(owner,id,correlation,receipt);return finish(status,receipt);}
        }
        if((definition.kind==cfg::family::topic&&action==rpc::operation::topic_qos)||(definition.kind==cfg::family::service&&action==rpc::operation::service_qos))
        {
            if(!input.done())return invalid();c::qos quality;std::array<std::uint8_t,32> gid{};std::size_t count{};
            auto status=value.registry->describe_qos(owner,id,quality,gid,count);if(status!=c::error::ok)return answer(status);
            if(!contract::qos_body(writer,quality)||!writer.blob({gid.data(),count}))return answer(c::error::too_large);return answer(status);
        }
        if(definition.kind==cfg::family::action)
        {
            auto const& wrapper=std::get<s::communication_action_dispatch::plan>(definition.policy);auto const& plan=wrapper.policy;
            if(action==rpc::operation::action_open)
            {
                if(!input.done())return invalid();if(output.size()<8)return answer(c::error::too_large);if(id||value.actions[index])return answer(c::error::busy);
                c::node_lease node;auto status=value.registry->attach_node(value.owner(value.node_index),value.node(),wrapper.node_profile,node);if(status!=c::error::ok)return answer(status);
                auto assigned=plan;for(auto type:{&assigned.goal,&assigned.feedback,&assigned.result}){auto trusted=schema(value,type->selected);if(!trusted)return answer(c::error::denied);type->authority=trusted->authority;}
                id=uwvm2::imported::ros::resources::allocate_identity();if(!id)return answer(c::error::too_large);
                status=c::ros2::open_lookup_action(assigned,std::move(node),id,value.actions[index]);
                if(status==c::error::ok)writer.u64(id);else if(!value.actions[index])id=0;
                return answer(status,status!=c::error::ok&&value.actions[index]&&value.actions[index]->cleanup_debt()?c::effect::unknown:c::effect::none);
            }
            auto endpoint=value.actions[index];if(!id||!endpoint)return answer(c::error::bad_handle);
            if(action==rpc::operation::action_submit)
            {wire::bytes identity,body;std::uint64_t deadline{};if(!input.blob(identity)||identity.size()!=16||!input.blob(body)||!input.u64(deadline)||!input.done())return invalid();
             c::actions::uuid uuid;std::copy(identity.begin(),identity.end(),uuid.begin());c::receipt receipt;auto status=endpoint->submit(owner,key,uuid,body,deadline,receipt);return finish(status,receipt);}
            if(action==rpc::operation::action_respond)
            {c::actions::response response;std::uint64_t transition{},deadline{};if(!input.u64(response.request)||!input.u64(response.goal)||!input.u64(transition)||transition<1||transition>8||
                !input.blob(response.body)||!input.boolean(response.accept_cancel)||!input.u64(deadline)||!input.done())return invalid();
             response.change=static_cast<c::actions::transition>(transition);
             c::receipt receipt;auto status=endpoint->respond(owner,key,response,deadline,receipt);return finish(status,receipt);}
            if(action==rpc::operation::action_cancel)
            {std::uint64_t goal{},deadline{};if(!input.u64(goal)||!input.u64(deadline)||!input.done())return invalid();c::receipt receipt;auto status=endpoint->cancel(owner,key,goal,deadline,receipt);return finish(status,receipt);}
            if(action==rpc::operation::action_take)
            {if(!input.done())return invalid();struct target{wire::writer* writer;c::actions::plan const* plan;bool attempted{},encoded{};};target sink{&writer,&plan};
             auto copy=[](void* opaque,c::actions::delivery const& message){auto& into=*static_cast<target*>(opaque);into.attempted=true;into.encoded=action_contract::encode_delivery(*into.writer,message,*into.plan);return into.encoded;};
             auto status=endpoint->take(owner,&sink,copy);return answer(status==c::error::io&&sink.attempted&&!sink.encoded?c::error::too_large:status);}
            if(action==rpc::operation::action_describe)
            {bool selected{};std::uint64_t goal{};if(!input.boolean(selected)||(selected&&!input.u64(goal))||!input.done())return invalid();c::actions::endpoint_info info;std::optional<c::actions::goal_info> detail;
             auto status=endpoint->describe(owner,info);if(status==c::error::ok&&selected){detail.emplace();status=endpoint->describe(owner,goal,*detail);}if(status==c::error::ok&&!action_contract::encode_state(writer,info,detail))status=c::error::too_large;return answer(status);}
        }
        if(definition.kind==cfg::family::graph&&action==rpc::operation::graph_capture)
        {
            if(!input.done())return invalid();auto const& wrapper=std::get<s::communication_graph_dispatch::plan>(definition.policy);
            if(!value.graphs[index])
            {c::node_lease node;auto status=value.registry->attach_node(value.owner(value.node_index),value.node(),wrapper.node_profile,node);if(status!=c::error::ok)return answer(status);
             value.graphs[index]=containers::make_shared<c::ros2::graph_source>(std::move(node),wrapper.policy.max_bytes);id=uwvm2::imported::ros::resources::allocate_identity();if(!id){value.graphs[index].reset();return answer(c::error::too_large);}}
            auto plan=wrapper.policy;plan.context=value.context();containers::std_vector<c::graph::entity> rows;rows.reserve(plan.max_rows);
            auto status=value.graphs[index]->capture(plan,rows);if(status!=c::error::ok)return answer(status);if(!writer.u64(rows.size()))return answer(c::error::too_large);
            for(auto const& row:rows)if(!writer.u64(static_cast<std::uint64_t>(row.type))||!writer.text(row.node)||!writer.text(row.endpoint)||!writer.text(row.message_type))return answer(c::error::too_large);
            return answer(c::error::ok);
        }
        return answer(c::error::unsupported);
    }
}
extern "C" bool uwvm_communication_prepare(wire::bytes input,startup::startup_limits const& limits,startup::prepared_backend& output) noexcept
{
    if(current)return false;
    for(int fd:{0,1,2}){auto flags=::fcntl(fd,F_GETFL);if(flags<0||!(flags&O_NONBLOCK))return false;}
    try
    {
        auto candidate=containers::make_shared<state>();if(manifest::decode(input,16u*1024u*1024u,candidate->specification)!=s::error::success)return false;
        if(limits.request_bytes!=candidate->specification.request_bytes||limits.response_bytes!=candidate->specification.response_bytes||!limits.thread_count)return false;
        if(!prewarm_logs()){std::fputs("worker prewarm logs failed\n",stderr);return false;}
        if(!preload()){std::fputs("worker preload failed\n",stderr);return false;}
        if(!initialize(*candidate)){std::fputs("worker initialize failed\n",stderr);return false;}
        startup::prepared_backend prepared;prepared.context=candidate.get();if(!notifications(prepared)){std::fputs("worker notifications failed\n",stderr);return false;}
        current=std::move(candidate);output=prepared;return true;
    }
    catch(std::exception const& failure){std::fprintf(stderr,"worker prepare exception: %.1024s\n",failure.what());return false;}
    catch(...){std::fputs("worker prepare unknown exception\n",stderr);return false;}
}
extern "C" bool uwvm_communication_activate(void* opaque,std::span<startup::role_fd const> descriptors) noexcept
{
    if(!current||opaque!=current.get()||current->attached||!current->network->activate(descriptors))return false;
    current->attached=true;return true;
}
extern "C" int uwvm_communication_serve(void* opaque) noexcept
{
    if(!current||opaque!=current.get()||!current->attached)return 130;
    auto& value=*current;
    for(;;)
    {
        std::uint64_t read_deadline{},write_deadline{};
        auto header=std::span<std::uint8_t>{value.input}.first(rpc::header_bytes);
        if(!transfer(0,header,false,value.specification.network.frame_deadline_ns,read_deadline,true))return 131;
        std::uint64_t extent{};if(!rpc::request_extent(header,value.specification.network.generation,value.specification.request_bytes,extent))return 132;
        if(!transfer(0,std::span<std::uint8_t>{value.input}.subspan(rpc::header_bytes,extent),false,value.specification.network.frame_deadline_ns,read_deadline))return 133;
        wire::bytes response;if(!value.history.execute({value.input.data(),rpc::header_bytes+extent},[&](auto const& call,auto out){return dispatch(value,call,out);},response))return 134;
        if(!transfer(1,{const_cast<std::uint8_t*>(response.data()),response.size()},true,value.specification.network.frame_deadline_ns,write_deadline))return 135;
    }
}
extern "C" void uwvm_communication_destroy(void* opaque) noexcept
{if(current&&opaque==current.get()){current.reset();log::Flush();log::KillThread();}}
