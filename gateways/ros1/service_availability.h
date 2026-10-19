/*************************************************************
 * UWVM2 bounded typed ROS1 Service metadata probe. APL-2.0. *
 *************************************************************/
#pragma once
#include "bounded_service.h"
#include <uwvm2/imported/ros/common/service_availability_history.h>
#include <algorithm>
#include <array>
#include <charconv>
#include <chrono>
#include <cstdint>
#include <string>
#include <string_view>
#include <thread>
#include <vector>
#include <stdexcept>
#include <system_error>

namespace uwvm2::ros_gateway::ros1_service_availability
{
    namespace wire=::uwvm2::imported::ros::ipc::client_availability;
    struct result {std::uint32_t error{};std::uint64_t value{};};
    inline bool space(char c) noexcept{return c==' '||c=='\t'||c=='\r'||c=='\n';}
    inline void skip(std::string const& s,std::size_t& p) noexcept
    {while(p<s.size()&&space(s[p])){++p;}}
    inline bool tag(std::string const& s,std::size_t& p,std::string_view text) noexcept
    {skip(s,p);if(s.compare(p,text.size(),text)!=0){return false;}p+=text.size();return true;}
    inline bool xml_result(std::string const& body,XmlRpc::XmlRpcValue& value)
    {
        // Bound native XML recursion before its real fromXml parser. DTD,
        // comments, processing instructions and embedded NUL are outside this
        // finite metadata response contract; no external entity is resolved.
        if(body.empty()||body.size()>4096||body.find('\0')!=body.npos){return false;}
        std::size_t p{};skip(body,p);
        if(body.compare(p,5,"<?xml")==0)
        {auto end{body.find("?>",p+5)};if(end==body.npos||end-p>256){return false;}p=end+2;}
        unsigned depth{};
        for(auto n=p;(n=body.find('<',n))!=body.npos;)
        {
            auto end{body.find('>',n+1)};if(end==body.npos||n+1==body.size()||body[n+1]=='!'||body[n+1]=='?'){return false;}
            if(body[n+1]=='/'){if(!depth){return false;}--depth;}
            else if(body[end-1]!='/'){if(++depth>32){return false;}}
            n=end+1;
        }
        if(depth||!tag(body,p,"<methodResponse>")||!tag(body,p,"<params>")||!tag(body,p,"<param>")){return false;}
        int offset{static_cast<int>(p)};
        if(!value.fromXml(body,&offset)||offset<0||static_cast<std::size_t>(offset)>body.size()){return false;}
        p=static_cast<std::size_t>(offset);
        if(!tag(body,p,"</param>")||!tag(body,p,"</params>")||!tag(body,p,"</methodResponse>")){return false;}
        skip(body,p);return p==body.size();
    }
    inline std::uint32_t lookup_result(std::string const& response,bool& found,std::string& uri)
    {
        found=false;uri.clear();XmlRpc::XmlRpcValue result;
        if(!xml_result(response,result)||result.getType()!=XmlRpc::XmlRpcValue::TypeArray||result.size()!=3||
            result[0].getType()!=XmlRpc::XmlRpcValue::TypeInt||result[1].getType()!=XmlRpc::XmlRpcValue::TypeString||
            result[2].getType()!=XmlRpc::XmlRpcValue::TypeString){return 22;}
        auto status{static_cast<int>(result[0])};auto const& message{static_cast<std::string&>(result[1])};
        auto const& address{static_cast<std::string&>(result[2])};
        if(status==1){if(address.empty()){return 22;}uri=address;found=true;return 0;}
        // This exact negative response is the real rosmaster lookupService
        // absence branch. Other caller errors/faults and internal status0 are
        // NOT a completed false readiness observation. Alternative Master
        // implementations must supply an independently reviewed equivalent.
        if(status==-1&&message=="no provider"&&address.empty()){return 0;}
        return status==0?5u:22u;
    }
    inline std::string master_host()
    {
        auto host{::ros::master::getHost()};
        if(host.size()>2&&host.front()=='['&&host.back()==']'){host=host.substr(1,host.size()-2);}
        return host;
    }
    inline void decimal(std::string& out,std::uint64_t value)
    {
        std::array<char,24> storage{};auto result{std::to_chars(storage.data(),storage.data()+storage.size(),value)};
        if(result.ec!=std::errc{}){throw std::runtime_error("bounded decimal representation failed");}
        out.append(storage.data(),static_cast<std::size_t>(result.ptr-storage.data()));
    }
    inline bool receive_header(int fd,ipc::deadline until,std::string& received,std::size_t& body_start,std::size_t& body_length)
    {
        std::array<char,1024> chunk{};
        for(;;)
        {
            auto end{received.find("\r\n\r\n")};
            if(end!=received.npos){body_start=end+4;break;}
            if(received.size()>=8192){errno=EMSGSIZE;return false;}
            auto now{std::chrono::steady_clock::now()};if(now>=until){errno=ETIMEDOUT;return false;}
            auto left{std::chrono::duration_cast<std::chrono::milliseconds>(until-now).count()};
            pollfd ready{fd,POLLIN,0};auto rc{::poll(&ready,1,static_cast<int>(std::max<std::int64_t>(1,std::min<std::int64_t>(left,INT_MAX))))};
            if(rc<0&&errno==EINTR){continue;}if(rc<=0){if(!rc){errno=ETIMEDOUT;}return false;}
            auto count{::recv(fd,chunk.data(),chunk.size(),MSG_DONTWAIT)};
            if(count<0&&(errno==EINTR||errno==EAGAIN||errno==EWOULDBLOCK)){continue;}
            if(count<=0){errno=ECONNRESET;return false;}
            received.append(chunk.data(),static_cast<std::size_t>(count));
            if(received.size()>8192+4096){errno=EMSGSIZE;return false;}
        }
        if(body_start>8192){errno=EMSGSIZE;return false;}
        auto first{received.find("\r\n")};
        if(first==received.npos||first>=body_start||
            (received.compare(0,13,"HTTP/1.0 200 ")!=0&&received.compare(0,13,"HTTP/1.1 200 ")!=0)){errno=EPROTO;return false;}
        bool length_found{};unsigned fields{};std::size_t position{first+2};
        while(position<body_start-2)
        {
            auto end{received.find("\r\n",position)},colon{received.find(':',position)};
            if(end==received.npos||end>=body_start||colon==received.npos||colon>=end||colon==position||++fields>64){errno=EPROTO;return false;}
            std::string name{received.substr(position,colon-position)};
            for(char& c:name){if(c>='A'&&c<='Z'){c=static_cast<char>(c-'A'+'a');}if(c<=' '||c>126){errno=EPROTO;return false;}}
            auto begin{colon+1};while(begin<end&&(received[begin]==' '||received[begin]=='\t')){++begin;}
            auto finish{end};while(finish>begin&&(received[finish-1]==' '||received[finish-1]=='\t')){--finish;}
            if(name=="transfer-encoding"){errno=EPROTO;return false;}
            if(name=="content-length")
            {
                if(length_found||begin==finish){errno=EPROTO;return false;}
                std::uint64_t value{};auto parsed{std::from_chars(received.data()+begin,received.data()+finish,value)};
                if(parsed.ec!=std::errc{}||parsed.ptr!=received.data()+finish||!value||value>4096){errno=EPROTO;return false;}
                body_length=static_cast<std::size_t>(value);length_found=true;
            }
            position=end+2;
        }
        if(!length_found||received.size()-body_start>body_length){errno=EPROTO;return false;}
        return true;
    }
    inline std::uint32_t lookup(std::string const& resource,ipc::deadline until,bool& found,std::string& uri)
    {
        found=false;uri.clear();sockaddr_storage address{};socklen_t address_size{};
        auto port{::ros::master::getPort()};auto host{master_host()};
        if(!port||port>65535||!ros1_service::numeric_address(host,static_cast<std::uint16_t>(port),address,address_size)){return 95;}
        // Native XML value formatting only; NEVER call executeNonBlock. Its
        // setupConnection calls XmlRpcSocket::connect and can resolve DNS before
        // a dispatcher deadline. Our socket goes directly to the numeric addr.
        XmlRpc::XmlRpcValue caller{::ros::this_node::getName()},service{resource};
        std::string body{"<?xml version=\"1.0\"?><methodCall><methodName>lookupService</methodName><params><param>"};
        body+=caller.toXml();body+="</param><param>";body+=service.toXml();body+="</param></params></methodCall>";
        if(body.size()>8192){return 90;}
        std::string request;request.reserve(body.size()+256);
        request="POST / HTTP/1.0\r\nHost: ";
        if(address.ss_family==AF_INET6){request+='[';request+=host;request+=']';}else{request+=host;}
        request+=':';decimal(request,port);
        request+="\r\nConnection: close\r\nContent-Type: text/xml\r\nContent-Length: ";decimal(request,body.size());request+="\r\n\r\n";request+=body;
        native_io::owned_fd fd{::socket(address.ss_family,SOCK_STREAM|SOCK_CLOEXEC|SOCK_NONBLOCK,0)};
        if(fd.get()<0||!ros1_service::connect_socket(fd.get(),address,address_size,until)){return ros1_service::failure(until);}
        if(!ipc::transfer(fd.get(),request.data(),request.size(),true,until)){return ros1_service::failure(until);}
        std::string received;received.reserve(8192+4096);std::size_t body_start{},body_length{};
        if(!receive_header(fd.get(),until,received,body_start,body_length))
        {return errno==EMSGSIZE?90u:errno==EPROTO?22u:ros1_service::failure(until);}
        std::string response{received.substr(body_start)};auto old{response.size()};response.resize(body_length);
        if(!ipc::transfer(fd.get(),response.data()+old,body_length-old,false,until)){return ros1_service::failure(until);}
        if(std::chrono::steady_clock::now()>=until){return 110;}
        auto parsed{lookup_result(response,found,uri)};
        return std::chrono::steady_clock::now()>=until?110u:parsed;
    }
    template<class Service> result one(ros::capability_spec const& source,ipc::deadline until)
    {
        auto md5{::ros::service_traits::MD5Sum<Service>::value()},type{::ros::service_traits::DataType<Service>::value()};
        if(source.operation!="service.call"||source.type!=type||source.type_identity!=md5){return {13,0};}
        bool found{};std::string uri;auto error{lookup(source.resource,until,found,uri)};
        if(error||!found){return {error,0};}
        sockaddr_storage address{};socklen_t address_size{};
        if(!ros1_service::endpoint(uri,address,address_size)){return {95,0};}
        native_io::owned_fd fd{::socket(address.ss_family,SOCK_STREAM|SOCK_CLOEXEC|SOCK_NONBLOCK,0)};
        if(fd.get()<0){return {ros1_service::failure(until),0};}
        if(!ros1_service::connect_socket(fd.get(),address,address_size,until))
        {return errno==ECONNREFUSED?result{0,0}:result{ros1_service::failure(until),0};}
        std::vector<std::uint8_t> header;header.reserve(2048);
        ros1_service::header_field(header,"callerid",::ros::this_node::getName());
        ros1_service::header_field(header,"service",source.resource);ros1_service::header_field(header,"md5sum",md5);
        ros1_service::header_field(header,"persistent","0");
        if(header.size()>8192){return {90,0};}
        std::array<std::uint8_t,4> length{};wire::put(length.data(),header.size(),4);
        if(!ipc::transfer(fd.get(),length.data(),4,true,until)||!ipc::transfer(fd.get(),header.data(),header.size(),true,until)||
            !ipc::transfer(fd.get(),length.data(),4,false,until)){return {ros1_service::failure(until),0};}
        auto size{wire::get(length.data(),4)};if(!size||size>8192){return {90,0};}header.resize(static_cast<std::size_t>(size));
        if(!ipc::transfer(fd.get(),header.data(),header.size(),false,until)){return {ros1_service::failure(until),0};}
        if(!ros1_service::response_header(header,md5,type)){return {13,0};}
        // Return/RAII close BEFORE ever writing a service request length/body.
        // Real roscpp invokes processRequest only after that missing body arrives.
        return std::chrono::steady_clock::now()>=until?result{110,0}:result{0,1};
    }
    template<class Service,class Qualifier>
    result observe(ros::capability_spec const& availability,ros::capability_spec const& source,
                   wire::packet const& request,std::uint32_t timeout_ms,Qualifier qualify)
    {
        if(!wire::valid_request(request)||request.binding.backend!=1||request.operation==wire::command::retire||
            !ros::service_availability_pair(ros::version::ros1,availability,source)){return {22,0};}
        if(!qualify(source)){return {13,0};}
        sockaddr_storage master{};socklen_t size{};auto port{::ros::master::getPort()};
        if(!port||port>65535||!ros1_service::numeric_address(master_host(),static_cast<std::uint16_t>(port),master,size)){return {95,0};}
        if(request.operation==wire::command::features){return {0,wire::typed_query|wire::finite_wait|wire::typed_md5_header};}
        if(!timeout_ms||request.value>limit(availability,"max-wait-ns")){return {22,0};}
        auto started{std::chrono::steady_clock::now()},until{started+std::chrono::milliseconds(std::min<std::uint64_t>(timeout_ms,limit(availability,"timeout-ms")))};
        auto logical{started+std::chrono::nanoseconds(request.value)};
        for(;;)
        {
            auto observed{one<Service>(source,until)};if(observed.error||observed.value){return observed;}
            auto now{std::chrono::steady_clock::now()};if(now>=until){return {110,0};}
            if(!request.value||now>=logical){return {0,0};}
            std::this_thread::sleep_until(std::min(logical,std::min(until,now+std::chrono::milliseconds(200))));
        }
    }
}
