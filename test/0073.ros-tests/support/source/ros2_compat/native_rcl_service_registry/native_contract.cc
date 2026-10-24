/* SPDX-License-Identifier: Apache-2.0 */
/** Real selected-SDK witness SOURCE, UNEXECUTED. Capability/handle values in
 * this component are explicit logical fixture data, not Host admission. The
 * Registry identities are read from an actual separately admitted export file;
 * no published test hashes are a default, and no ROS type/function is faked.
 * External Root must own native domain/isolation/physical memory/deadline and
 * process retirement before allowing stdin G. This is not a Guest/ROS ABI test.
 */
#include "src/ros/gateway/ros2/native_rcl_service_registry.h"
#include <fast_io.h>
#include <thread>
using namespace uwvm2::ros_gateway;
namespace impl=native_rcl_service_registry;
namespace tx=impl::transport;
namespace wire=impl::wire;
namespace md=impl::metadata;
static unsigned checks{},failures{};
#define CHECK(v) do{++checks;if(!(v)){++failures;fast_io::io::println(fast_io::err(),"FAIL_NATIVE_SERVICE_REGISTRY line=",unsigned(__LINE__));}}while(0)
#define REQUIRE(v) do{bool const ok=bool(v);CHECK(ok);if(!ok)return false;}while(0)
static bool identities(char const* path,uwvm_rmw_cpp::NativeIdentities& out)
{
    // Exact bounded DATA, not a candidate/generator invocation. The caller's
    // owned admission must pin this ordinary file independently of the binary.
    fast_io::native_file in{fast_io::mnp::os_c_str(path),fast_io::open_mode::in};
    std::array<char,2049> data{};auto current=data.data();auto const limit=data.data()+data.size();
    // Exactly one extra byte distinguishes a valid <=2048-byte export from a
    // longer file. Short reads are continued; no token parsing or file-sized
    // allocation occurs. The borrowed text remains inside this array's lifetime.
    while(current!=limit){auto next=fast_io::operations::read_some(in,current,limit);
        if(next==current)break;current=next;}
    auto n=static_cast<std::size_t>(current-data.data());if(!n||n>2048)return false;
    constexpr std::array<std::string_view,11> names{"string","time","event_info","empty_request","empty_response",
        "empty_event","add_request","add_response","add_event","empty_service","add_service"};
    static_assert(names.size()==static_cast<std::size_t>(uwvm_rmw_cpp::Identity::count));
    uwvm_rmw_cpp::NativeIdentities next{};std::string_view text{data.data(),static_cast<std::size_t>(n)};
    auto nibble=[](char c)->int{return c>='0'&&c<='9'?c-'0':c>='a'&&c<='f'?c-'a'+10:-1;};
    for(std::size_t i=0;i<names.size();++i)
    {
        auto end=text.find('\n');if(end==text.npos)return false;auto line=text.substr(0,end);text.remove_prefix(end+1);
        if(line.size()!=names[i].size()+1+64||line.substr(0,names[i].size())!=names[i]||line[names[i].size()]!=' ')return false;
        auto hash=line.substr(names[i].size()+1);bool nonzero{};
        for(std::size_t j=0;j<32;++j){auto a=nibble(hash[2*j]),b=nibble(hash[2*j+1]);if(a<0||b<0)return false;next.hashes[i][j]=static_cast<std::uint8_t>((a<<4)|b);nonzero|=next.hashes[i][j]!=0;}
        if(!nonzero)return false;
    }
    if(!text.empty())return false;out=next;return true;
}
static ros::capability_spec capability(std::string alias,std::string op,std::string type,std::string hash)
{
    ros::capability_spec c{};c.alias=std::move(alias);c.operation=std::move(op);c.resource="/uwvm_native_service_registry_contract";
    c.type=std::move(type);c.type_identity=std::move(hash);
    for(auto const& p:std::initializer_list<std::pair<std::string,std::string>>{
        {"validator","none"},{"max-request-bytes","16"},{"max-response-bytes","8"},{"max-inflight","1"},{"timeout-ms","5000"},
        {"qos.history","keep-last"},{"qos.depth","1"},{"qos.reliability","reliable"},{"qos.durability","volatile"},
        {"qos.deadline","infinite"},{"qos.lifespan","infinite"},{"qos.liveliness","automatic"},{"qos.liveliness-lease-duration","infinite"}})c.limits.emplace(p);
    return c;
}
static impl::admitted pair(ros::capability_spec const& a,ros::capability_spec const& b,wire::binding const& peer)
{impl::admitted v{};v.source=&a;v.paired=&b;v.paired_owner=peer;return v;}
static tx::control control(wire::binding const& owner,wire::binding const& peer,wire::role role)
{tx::control c{};c.owner=owner;c.peer=peer.source;c.kind=role;return c;}
static ipc::call_result invoke(impl::registry& registry,impl::admitted const& a,tx::control const& c,
    ipc::deadline end,std::uint8_t const* bytes=nullptr,std::size_t length=0)
{return registry.execute(a,c,bytes,length,end);}
static ipc::call_result wait_record(impl::registry& registry,impl::admitted const& a,tx::control const& c,ipc::deadline end)
{
    // This is an external witness driver, NOT the availability implementation.
    // Each iteration makes a genuine bounded single-poll call; its actual wall
    // deadline, not iteration count, decides failure. Positive available stays95.
    for(;;){auto r=invoke(registry,a,c,end);if(r.error!=11)return r;
        if(std::chrono::steady_clock::now()>=end)return {110,{}};
        std::this_thread::sleep_for(std::chrono::milliseconds(1));}
}
template<class Service>static bool route(rclcpp::Node::SharedPtr const& node,
    std::shared_ptr<uwvm_rmw_cpp::Registry const> const& codecs,std::uint64_t first)
{
    auto root=codecs->codec(native_rcl_service::family_traits<Service>::codec);if(!root)return false;
    auto type=std::string{root->native_type},hash=native_rcl_service::identity_text(root->native_rihs01);
    auto call=capability("client","service.call",type,hash),advertise=capability("server","service.advertise",type,hash),respond=capability("respond","service.respond",type,hash);
    advertise.limits.emplace("max-event-queue-depth","4");advertise.limits.emplace("max-event-queue-bytes","1024");
    auto observer=ros::capability_spec{};observer.alias="responses";observer.operation="event.observe";observer.resource=call.resource;
    observer.type=ipc::event_type;observer.type_identity=ipc::event_type_identity;
    observer.limits={{"event-source-alias",call.alias},{"event-kinds","service-response,resource-error,resource-closed"},
        {"max-event-bytes","80"},{"max-event-queue-depth","4"},{"max-event-queue-bytes","1024"}};
    wire::binding c{2,first,2,3,4},o=c,s=c,r=c,v=c;o.source+=1;s.source+=2;r.source+=3;v.source+=4;
    auto ca=pair(call,observer,o),sa=pair(advertise,respond,r);
    impl::registry registry{node,codecs,{8,2}};
    struct terminal_observer
    {
        impl::registry& value;std::uint64_t client,server;
        ~terminal_observer()
        {
            value.clear();
            for(auto source:{client,server})
            {
                impl::diagnostics d{};native_rcl_service::diagnostic n{};
                auto status=value.observe_after_quiescence(source,d,n);
                fast_io::io::println(fast_io::out(),"ACTUAL_NATIVE_REGISTRY_RETIRE source=",source," observation_status=",status,
                    " primary=",d.first_error," commit_attempted=",d.commit_attempted," commit=",d.last_commit_error,
                    " cancel_attempted=",d.cancel_attempted," cancel=",d.last_cancel_error," retire_attempted=",d.retire_attempted,
                    " retire=",d.last_retire_error," native_initialized=",n.native_initialized," fini_attempted=",n.fini_attempted,
                    " fini_returned=",n.fini_returned," fini_result=",n.fini_result);
                if(!status)for(unsigned i=0;i<static_cast<unsigned>(native_rcl_service::operation::count);++i)
                    fast_io::io::println(fast_io::out(),"ACTUAL_NATIVE_CALL source=",source," operation=",i,
                        " calls=",n.native[i].calls," last_result=",n.native[i].last_native_result);
            }
        }
    } terminal{registry,c.source,s.source};
    auto cp=control(c,o,wire::role::client),sp=control(s,r,wire::role::service);
    auto end=ipc::after(5000);auto created=invoke(registry,sa,sp,end);tx::control echo{};wire::packet sr{},cr{};
    REQUIRE(created.error==0);if(created.error)return false;
    REQUIRE(tx::associated_endpoint(created.payload.data(),created.payload.size(),sp,echo,sr));sp.incarnation=sr.incarnation;
    created=invoke(registry,ca,cp,end);REQUIRE(created.error==0);if(created.error)return false;
    REQUIRE(tx::associated_endpoint(created.payload.data(),created.payload.size(),cp,echo,cr));cp.incarnation=cr.incarnation;
    REQUIRE(cr.profiles==2&&sr.profiles==2&&cr.gid_size==16);
    // Actual source lifecycle tests: pre-create explicit canonical constraint is
    // refused, never discarded. This is not production profile admission proof.
    auto unavailable=call;unavailable.limits.emplace("adapter-codec-identity","unproven");
    std::shared_ptr<impl::endpoint> absent;wire::packet untouched{};untouched.incarnation=73;
    REQUIRE(impl::create_fixed(node,codecs,unavailable,c,77,2,{},absent,untouched)==95&&!absent&&untouched.incarnation==73);
    REQUIRE(!native_rcl_service::selected(unavailable,*root,hash));
    REQUIRE(native_rcl_service::selected(call,*root,hash)); // actual unchanged nonexplicit path
    auto availability=capability("available","service.available",type,hash);availability.limits.erase("validator");
    availability.limits["service-source-alias"]=call.alias;availability.limits["max-inflight"]="1";
    availability.limits["max-request-bytes"]="64";availability.limits["max-response-bytes"]="64";
    availability.limits["max-wait-ns"]="1000000";availability.limits["timeout-ms"]="3";
    availability.limits["max-native-clients"]="1";availability.limits["max-binding-history"]="8";
    auto aa=ca;aa.availability=&availability;aa.availability_owner=v;
    auto query=cp;query.operation=tx::command::available;query.peer=v.source;
    auto qr=invoke(registry,aa,query,end);REQUIRE(qr.error==0&&tx::associated_ack(qr.payload.data(),qr.payload.size(),query,echo)&&echo.value<=1);
    impl::diagnostics fact{};native_rcl_service::diagnostic before{},after{};REQUIRE(registry.observe_after_quiescence(c.source,fact,before)==0);
    query.wait_ns=1;qr=invoke(registry,aa,query,end);REQUIRE(qr.error==95&&qr.payload.empty());
    REQUIRE(registry.observe_after_quiescence(c.source,fact,after)==0&&before.native[static_cast<unsigned>(native_rcl_service::operation::available)].calls==after.native[static_cast<unsigned>(native_rcl_service::operation::available)].calls);
    typename Service::Request request{};typename Service::Response response{};
    if constexpr(std::is_same_v<Service,example_interfaces::srv::AddTwoInts>){request.a=-7;request.b=49;response.sum=42;}
    std::array<std::uint8_t,16> body{};std::size_t size=SIZE_MAX;
    REQUIRE(root->request->encode(&request,body.data(),root->request->maximum_wire_bytes,&size)==0);
    auto submit=cp;submit.operation=tx::command::submit;submit.body_bytes=size;
    auto sent=invoke(registry,ca,submit,end,body.data(),size);REQUIRE(sent.error==0&&tx::associated_ack(sent.payload.data(),sent.payload.size(),submit,echo));
    if(sent.error)return false;auto client_ticket=echo.ticket;
    auto peek=sp;peek.operation=tx::command::peek;peek.capacity=4096;peek.writer_bytes=16;
    auto received=wait_record(registry,sa,peek,end);REQUIRE(received.error==0);if(received.error)return false;
    tx::control borrowed{};md::header request_info{};
    REQUIRE(tx::associated_delivery(received.payload.data(),received.payload.size(),peek,borrowed,request_info)&&md::complete(request_info));
    typename Service::Request decoded{};REQUIRE(root->request->decode(received.payload.data()+tx::minimum_delivery_bytes,
        received.payload.size()-tx::minimum_delivery_bytes,&decoded)==0);
    if constexpr(std::is_same_v<Service,example_interfaces::srv::AddTwoInts>)REQUIRE(decoded.a==-7&&decoded.b==49);
    auto ack=sp;ack.operation=tx::command::cancel;ack.sequence=borrowed.sequence;ack.reservation=borrowed.reservation;
    auto canceled=invoke(registry,sa,ack,end);REQUIRE(canceled.error==0);
    received=invoke(registry,sa,peek,end);tx::control again{};md::header request_info2{};
    REQUIRE(!received.error&&tx::associated_delivery(received.payload.data(),received.payload.size(),peek,again,request_info2)&&again.sequence==borrowed.sequence&&again.reservation!=borrowed.reservation);
    auto old=ack;old.operation=tx::command::commit;REQUIRE(invoke(registry,sa,old,end).error==13);REQUIRE(invoke(registry,sa,ack,end).error==13);
    ack.operation=tx::command::commit;ack.reservation=again.reservation;REQUIRE(invoke(registry,sa,ack,end).error==0);
    size=SIZE_MAX;REQUIRE(root->response->encode(&response,body.data(),root->response->maximum_wire_bytes,&size)==0);
    auto answer=sp;answer.operation=tx::command::respond;answer.ticket=again.ticket;answer.body_bytes=size;
    REQUIRE(invoke(registry,sa,answer,end,body.data(),size).error==0);
    peek=cp;peek.operation=tx::command::peek;peek.capacity=4096;peek.writer_bytes=16;
    received=wait_record(registry,ca,peek,end);REQUIRE(received.error==0);if(received.error)return false;
    md::header response_info{};REQUIRE(tx::associated_delivery(received.payload.data(),received.payload.size(),peek,borrowed,response_info)&&md::complete(response_info));
    REQUIRE(response_info.correlation==client_ticket&&response_info.request_sequence==request_info.request_sequence&&response_info.writer==request_info.writer);
    typename Service::Response decoded_response{};
    REQUIRE(root->response->decode(received.payload.data()+tx::minimum_delivery_bytes,received.payload.size()-tx::minimum_delivery_bytes,&decoded_response)==0);
    if constexpr(std::is_same_v<Service,example_interfaces::srv::AddTwoInts>)REQUIRE(decoded_response.sum==42);
    else REQUIRE(received.payload.size()==tx::minimum_delivery_bytes);
    ack=cp;ack.operation=tx::command::commit;ack.sequence=borrowed.sequence;ack.reservation=borrowed.reservation;
    REQUIRE(invoke(registry,ca,ack,end).error==0);
    // Close the independently granted observer, then the responder. Both really
    // retire their paired Native object; no allocation or current session lookup.
    registry.close_owner(o,o.source,observer.alias);registry.close_owner(r,r.source,respond.alias);
    native_rcl_service::diagnostic client{},server{};
    REQUIRE(registry.observe_after_quiescence(c.source,fact,client)==0&&fact.retire_attempted&&fact.last_retire_error==0);
    REQUIRE(registry.observe_after_quiescence(s.source,fact,server)==0&&fact.retire_attempted&&fact.last_retire_error==0);
    REQUIRE(client.fini_attempted&&client.fini_returned&&client.fini_result==RCL_RET_OK&&client.native[static_cast<unsigned>(native_rcl_service::operation::fini)].calls==1);
    REQUIRE(server.fini_attempted&&server.fini_returned&&server.fini_result==RCL_RET_OK&&server.native[static_cast<unsigned>(native_rcl_service::operation::fini)].calls==1);
    auto stale=cp;stale.operation=tx::command::inspect;REQUIRE(invoke(registry,ca,stale,end).error==125);return true;
}
int main(int argc,char** argv)
{
    if(argc!=3||std::string_view{argv[1]}!="--owned-native-registry")return 13;
#if !UWVM_NATIVE_RCL_SERVICE_API || !UWVM_NATIVE_RCL_AVAILABILITY_API || !UWVM_ROS2_HAVE_COMPLETE_RMW_QOS
    (void)argv;return 95;
#else
    try{
        uwvm_rmw_cpp::NativeIdentities actual{};if(!identities(argv[2],actual))return 22;
        auto codecs=std::make_shared<uwvm_rmw_cpp::Registry const>(actual,128);if(codecs->error())return codecs->error();
        fast_io::io::println(fast_io::out(),"READY_ACTUAL_NATIVE_REGISTRY_WARMED_REQUIRES_OWNED_G");
        char go{};auto read_end=fast_io::operations::read_some(fast_io::posix_stdin(),std::addressof(go),std::addressof(go)+1);
        if(read_end==std::addressof(go)||go!='G')return 13;
        rclcpp::init(argc,argv);
        struct shutdown{~shutdown(){if(rclcpp::ok())rclcpp::shutdown();}} cleanup;
        {auto node=std::make_shared<rclcpp::Node>("uwvm_native_service_registry_contract"); // actual original NodeOptions defaults
            CHECK(route<std_srvs::srv::Empty>(node,codecs,UINT64_C(0x100000001)));
            CHECK(route<example_interfaces::srv::AddTwoInts>(node,codecs,UINT64_C(0x100000011)));}
        fast_io::io::println(fast_io::out(),fast_io::mnp::strvw(std::string_view{failures?"FAIL":"PASS",4}),"_NATIVE_SERVICE_REGISTRY_COMPONENT families=2 checks=",checks," failures=",failures);
        return failures?1:0;
    }catch(...){return 5;}
#endif
}
