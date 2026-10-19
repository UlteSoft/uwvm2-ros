/**
 * @file ros1_service_transport.cc
 * @brief Real TCPROS tests of the deadline-bounded official ROS1 service codec.
 *
 * This is a native transport fixture, not a Guest authorization verdict. The
 * parent supplies an owned Master at numeric127.0.0.1:11385, proves the stopped
 * ARM image and sends I. A raw fault peer is forked before ros::init; positive
 * payloads use official generated serializers/deserializers. Negative headers
 * and bodies are deliberately corrupted after serialization. The peer sends
 * no IPC or capability identity and accepts only one exact request per call.
 * No service retry, DNS resolver, detached task or unrestricted network exists.
 */
#include "../../gateways/ros1/bounded_service.h"
#include <std_srvs/Empty.h>
#include <std_srvs/Trigger.h>
#include <std_srvs/SetBool.h>
#include <atomic>
#include <chrono>
#include <csignal>
#include <cstdlib>
#include <new>
#include <stdexcept>
#include <string_view>
#include <sys/mman.h>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>

__attribute__((constructor)) static void service_preinit_stop()
{ if(::raise(SIGSTOP) != 0) { __builtin_trap(); } }

namespace bounded_service_test
{
    namespace gw=uwvm2::ros_gateway; namespace ipc=uwvm2::imported::ros::ipc;
    using clock=std::chrono::steady_clock;
    std::string_view phase{"input"},site{"contract"};
    void require(bool ok,std::string_view fixed="contract") {if(!ok){site=fixed;throw std::runtime_error("fixed bounded service refusal");}}
    template<typename T> bool append(gw::native_io::fixed_text<2048>& text,T const& value)
    {if constexpr(std::is_integral_v<T>){return text.number(value);}else{return text.append(std::string_view{value});}}
    template<typename... T> void emit(T const&... values)
    {gw::native_io::fixed_text<2048> text;require((append(text,values)&&...)&&text.append("\n"),"output");require(gw::native_io::write_all(1,text.view()),"output");}
    struct counts {std::atomic<unsigned> accepted{0},requests{0},closed{0};};
    struct descriptor {int fd{-1};~descriptor(){if(fd>=0)::close(fd);}};
    bool exact(int fd,void* data,std::size_t size,bool writing,ipc::deadline until)
    {
        // Basic control descriptors are pipes, whereas native protocol peers
        // use nonblocking TCP sockets. read/write supports both; IPC's socket-
        // specific transfer helper must not be applied to a stdin pipe.
        auto* bytes=static_cast<std::uint8_t*>(data);
        while(size)
        {
            auto left=std::chrono::duration_cast<std::chrono::milliseconds>(until-clock::now()).count();if(left<=0)return false;
            pollfd ready{fd,static_cast<short>(writing?POLLOUT:POLLIN),0};int result=::poll(&ready,1,static_cast<int>(std::min<std::int64_t>(left,INT_MAX)));
            if(result<0&&errno==EINTR)continue;if(result<=0||(ready.revents&(POLLERR|POLLNVAL)))return false;
            auto count=writing ? ::write(fd,bytes,size) : ::read(fd,bytes,size);
            if(count<0&&(errno==EINTR||errno==EAGAIN||errno==EWOULDBLOCK))continue;if(count<=0)return false;
            bytes+=count;size-=static_cast<std::size_t>(count);
        }
        return true;
    }
    std::vector<std::uint8_t> take(int fd,std::size_t maximum,ipc::deadline until)
    {
        std::array<std::uint8_t,4> length{};require(exact(fd,length.data(),4,false,until),"peer_length");
        auto size=ipc::get(length.data(),4);require(size<=maximum,"peer_bound");std::vector<std::uint8_t> body(static_cast<std::size_t>(size));
        require(exact(fd,body.data(),body.size(),false,until),"peer_body");return body;
    }
    void send_record(int fd,std::vector<std::uint8_t> const& body,ipc::deadline until)
    {
        std::vector<std::uint8_t> length;ipc::put(length,body.size(),4);
        require(exact(fd,length.data(),4,true,until)&&exact(fd,const_cast<std::uint8_t*>(body.data()),body.size(),true,until),"peer_write");
    }
    template<typename Native> std::vector<std::uint8_t> serialized(Native const& native)
    {
        auto size=::ros::serialization::serializationLength(native);std::vector<std::uint8_t> bytes;bytes.reserve(std::max<unsigned>(1,size));bytes.resize(size);
        ::ros::serialization::OStream output{bytes.data(),size};::ros::serialization::serialize(output,native);return bytes;
    }
    void peer(int listener,int command,counts& observed)
    {
        auto until=clock::now()+std::chrono::seconds{90};
        for(;;)
        {
            char mode{};require(exact(command,&mode,1,false,until),"peer_command");if(mode=='Q')return;
            pollfd ready{listener,POLLIN,0};require(::poll(&ready,1,5000)>0&&(ready.revents&POLLIN),"peer_accept");
            descriptor socket{::accept4(listener,nullptr,nullptr,SOCK_CLOEXEC|SOCK_NONBLOCK)};require(socket.fd>=0,"peer_accept");++observed.accepted;
            auto request_header=take(socket.fd,8192,until);require(!request_header.empty(),"peer_header");
            std::vector<std::uint8_t> header;
            bool empty=mode=='e',boolean=mode=='b';char const* md5=empty?ros::service_traits::MD5Sum<std_srvs::Empty>::value():boolean?ros::service_traits::MD5Sum<std_srvs::SetBool>::value():ros::service_traits::MD5Sum<std_srvs::Trigger>::value();
            char const* type=empty?ros::service_traits::DataType<std_srvs::Empty>::value():boolean?ros::service_traits::DataType<std_srvs::SetBool>::value():ros::service_traits::DataType<std_srvs::Trigger>::value();
            gw::ros1_service::header_field(header,"callerid","/remaining_service/peer");
            gw::ros1_service::header_field(header,"md5sum",mode=='m'?"00000000000000000000000000000000":md5);
            gw::ros1_service::header_field(header,"type",mode=='h'?"std_srvs/Foreign":type);send_record(socket.fd,header,until);
            if(mode=='m'||mode=='h')
            {
                // Refused identity must close before a request reaches native.
                char byte{};pollfd closed{socket.fd,POLLIN|POLLHUP,0};require(::poll(&closed,1,3000)>0,"header_close");
                require(::recv(socket.fd,&byte,1,0)==0,"header_request_sent");++observed.closed;continue;
            }
            auto body=take(socket.fd,64,until);
            if(boolean){require(body.size()==1&&body[0]==1,"request_bool");std_srvs::SetBool::Request native;ros::serialization::IStream input{body.data(),1};ros::serialization::deserialize(input,native);require(native.data&&input.getLength()==0,"official_request");}
            else require(body.empty(),"empty_request");
            ++observed.requests;
            if(mode=='t'){std::this_thread::sleep_for(std::chrono::milliseconds{450});++observed.closed;continue;}
            if(mode=='c'){++observed.closed;continue;}
            std::vector<std::uint8_t> response;
            if(empty){std_srvs::Empty::Response value;response=serialized(value);}
            else if(boolean){std_srvs::SetBool::Response value;value.success=true;value.message="bounded-service-ok";response=serialized(value);}
            else{std_srvs::Trigger::Response value;value.success=true;value.message="bounded-service-ok";response=serialized(value);}
            if(mode=='u'){response={1,1,0,0,0,255};}
            if(mode=='n'){response[0]=2;}
            if(mode=='r'){response.push_back(0);}
            std::vector<std::uint8_t> prefix{1};ipc::put(prefix,mode=='l'?1024:response.size(),4);
            require(exact(socket.fd,prefix.data(),prefix.size(),true,until),"response_prefix");
            if(mode!='l')require(exact(socket.fd,response.data(),response.size(),true,until),"response_write");
            // The server must observe the client's owned FD close after either
            // success or refusal. This checks cleanup even for valid responses.
            pollfd closed{socket.fd,POLLIN|POLLHUP,0};require(::poll(&closed,1,3000)>0,"response_close");
            char byte{};require(::recv(socket.fd,&byte,1,0)==0,"duplicate_request");++observed.closed;
        }
    }
    struct child_owner
    {
        pid_t pid{-1};bool reaped{};
        ~child_owner(){if(pid>0&&!reaped){::kill(pid,SIGKILL);int status{};while(::waitpid(pid,&status,0)<0&&errno==EINTR){}}}
        void finish(){int status{};require(::waitpid(pid,&status,0)==pid&&WIFEXITED(status)&&WEXITSTATUS(status)==0,"peer_exit");reaped=true;}
    };
    void registration(std::string const& uri)
    {
        XmlRpc::XmlRpcValue args,result;args[0]="/remaining_service/client";args[1]="/remaining_service/call";args[2]=uri;args[3]="http://127.0.0.1:1";
        gw::ros1_resources::bounded_xmlrpc master{"127.0.0.1",11385,4096};require(master.execute_bounded("registerService",args,result,2000)==0,"register_service");
        require(result.getType()==XmlRpc::XmlRpcValue::TypeArray&&result.size()==3&&static_cast<int>(result[0])==1,"register_response");
    }
    template<typename Service> uwvm2::imported::ros::capability_spec capability(unsigned timeout=1000)
    {
        uwvm2::imported::ros::capability_spec cap;cap.alias="client";cap.operation="service.call";cap.resource="/remaining_service/call";
        cap.type=ros::service_traits::DataType<Service>::value();cap.type_identity=ros::service_traits::MD5Sum<Service>::value();
        cap.limits={{"timeout-ms",std::to_string(timeout)},{"max-request-bytes","64"},{"max-response-bytes","64"},
            {"max-inflight","1"},{"max-rate-hz","10"},{"burst","16"},{"max-bytes-per-second","4096"},{"validator","none"},
            {"audit","allow-and-deny"},{"audit-max-rate-hz","10"},{"audit-burst","16"},{"audit-max-records","100"},{"audit-max-bytes","16384"}};
        return cap;
    }
    bool body_valid(std::vector<std::uint8_t> const& body)
    {return body.size()>=5&&body[0]<=1&&ipc::get(body.data()+1,4)==body.size()-5&&gw::valid_utf8(body.data()+5,body.size()-5);}
}

int main(int argc,char**)
{
    using namespace bounded_service_test;
    try
    {
        require(argc==1&&::getenv("ROS_MASTER_URI")&&std::string_view{::getenv("ROS_MASTER_URI")}=="http://127.0.0.1:11385"
            &&::getenv("ROS_IP")&&std::string_view{::getenv("ROS_IP")}=="127.0.0.1","environment");
        char initial{};require(exact(0,&initial,1,false,ipc::after(20000))&&initial=='I',"initial_gate");
        require(::signal(SIGPIPE,SIG_IGN)!=SIG_ERR,"signal");
        descriptor listener{::socket(AF_INET,SOCK_STREAM|SOCK_CLOEXEC|SOCK_NONBLOCK,0)};require(listener.fd>=0,"listener");
        sockaddr_in address{};address.sin_family=AF_INET;address.sin_addr.s_addr=htonl(INADDR_LOOPBACK);
        require(::bind(listener.fd,reinterpret_cast<sockaddr*>(&address),sizeof address)==0&&::listen(listener.fd,1)==0,"listener");
        socklen_t size=sizeof address;require(::getsockname(listener.fd,reinterpret_cast<sockaddr*>(&address),&size)==0,"listener");
        int control[2];require(::pipe2(control,O_CLOEXEC|O_NONBLOCK)==0,"control");descriptor read_control{control[0]},write_control{control[1]};
        void* shared=::mmap(nullptr,sizeof(counts),PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);require(shared!=MAP_FAILED,"shared_counts");
        auto& observed=*new(shared) counts;child_owner child;child.pid=::fork();require(child.pid>=0,"fork");
        if(child.pid==0){try{::close(write_control.fd);peer(listener.fd,read_control.fd,observed);::_exit(0);}catch(...){::_exit(1);}}
        ::close(read_control.fd);read_control.fd=-1;::close(listener.fd);listener.fd=-1;
        phase="initialization";ros::M_string remappings;remappings["__ns"]="/remaining_service";
        ros::init(remappings,"client",ros::init_options::NoRosout|ros::init_options::NoSigintHandler);ros::NodeHandle node;
        std::string numeric="rosrpc://127.0.0.1:"+std::to_string(ntohs(address.sin_port));registration(numeric);
        emit("READY_BOUNDED_SERVICE pid=",::getpid()," peer_pid=",child.pid," master=11385 transport=tcpros");
        unsigned checks{};
        for(char mode : {'a','e','b','m','h','u','n','r','l','t','c'})
        {
            phase="native-call";unsigned before_requests=observed.requests.load(),before_accepted=observed.accepted.load(),before_closed=observed.closed.load();
            require(exact(write_control.fd,&mode,1,true,ipc::after(1000)),"peer_command");unsigned status{};
            unsigned expected=mode=='m'||mode=='h'?13:mode=='u'||mode=='n'||mode=='r'?22:mode=='l'?90:mode=='t'?110:mode=='c'?107:0;
            if(mode=='e'){std_srvs::Empty service;status=gw::ros1_service::call(capability<std_srvs::Empty>(),service,1000,[](auto const& body){return body.empty();});}
            else if(mode=='b'){std_srvs::SetBool service;service.request.data=true;status=gw::ros1_service::call(capability<std_srvs::SetBool>(),service,1000,body_valid);require(status||service.response.success&&service.response.message=="bounded-service-ok","setbool_response");}
            else{std_srvs::Trigger service;service.response.success=false;service.response.message="protected";auto cap=capability<std_srvs::Trigger>(mode=='t'?200:1000);
                status=gw::ros1_service::call(cap,service,1000,body_valid);require(status?(!service.response.success&&service.response.message=="protected"):(service.response.success&&service.response.message=="bounded-service-ok"),"output_transaction");}
            require(status==expected,"expected_errno");auto until=clock::now()+std::chrono::seconds{3};
            while(observed.closed.load()==before_closed&&clock::now()<until)std::this_thread::sleep_for(std::chrono::milliseconds{2});
            unsigned requests=observed.requests.load()-before_requests;require(observed.accepted.load()-before_accepted==1&&observed.closed.load()-before_closed==1
                &&requests==(mode=='m'||mode=='h'?0U:1U),"request_once_and_close");
            emit("BOUNDED_SERVICE_CASE mode=",std::string_view{&mode,1}," errno=",status," accepted=1 requests=",requests," closed=1 output_preserved=1");++checks;
        }
        phase="numeric-refusal";registration("rosrpc://unresolved.invalid:12345");std_srvs::Trigger dns;
        auto before=observed.accepted.load();require(gw::ros1_service::call(capability<std_srvs::Trigger>(),dns,1000,body_valid)==95&&observed.accepted.load()==before,"dns_unsupported");++checks;
        auto bad=capability<std_srvs::Trigger>();bad.type_identity="00000000000000000000000000000000";
        require(gw::ros1_service::call(bad,dns,1000,body_valid)==13&&observed.accepted.load()==before,"grant_identity");++checks;
        sockaddr_storage value{};socklen_t length{};require(gw::ros1_service::endpoint("rosrpc://localhost:12345",value,length)
            &&gw::ros1_service::endpoint("rosrpc://[::1]:12345",value,length),"numeric_profile");++checks;
        char quit='Q';require(exact(write_control.fd,&quit,1,true,ipc::after(1000)),"peer_quit");child.finish();node.shutdown();ros::shutdown();
        observed.~counts();require(::munmap(shared,sizeof(counts))==0,"shared_cleanup");
        emit("PASS_BOUNDED_SERVICE checks=",checks," positive=3 malformed22=3 quota90=1 header13=2 local13=1 timeout110=1 closed107=1 dns95=1 numeric_profile=1");return 0;
    }
    catch(...){try{emit("FAIL_BOUNDED_SERVICE phase=",phase," site=",site);}catch(...){}return 1;}
}
