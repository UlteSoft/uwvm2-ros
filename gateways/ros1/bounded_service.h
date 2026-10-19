/*************************************************************
 * UWVM2 deadline-bounded ROS1 Service transport. APL-2.0.    *
 *************************************************************/
#pragma once
#include "resources.h"
#include <ros/serialization.h>
#include <ros/service_traits.h>
#include <ros/this_node.h>
#include <arpa/inet.h>
#include <array>
#include <climits>

namespace uwvm2::ros_gateway::ros1_service
{
    /**
     * ROS1's ordinary synchronous ServiceClient has no call deadline. Joining a
     * future after shutdown can therefore stall connection teardown. This
     * transport uses the official generated serialization and MD5/type traits,
     * but owns its nonblocking TCPROS socket and one absolute deadline itself.
     * It never starts a detached thread, retries an admitted request, or exposes
     * the Master XML-RPC or raw socket to a Guest.
     *
     * Endpoints must advertise a numeric IPv4/IPv6 address (or localhost). DNS
     * lookup is deliberately excluded from the bounded call: an unresolved
     * hostname returns unsupported before sending a service request. A trusted
     * deployment should select ROS_IP and a numeric ROS_MASTER_URI explicitly.
     * Protocol reference: ROS Noetic service_server_link.cpp and
     * service_client_link.cpp in https://github.com/ros/ros_comm/tree/noetic-devel.
     */
    inline bool numeric_address(std::string const& host,std::uint16_t port,
        sockaddr_storage& storage,socklen_t& size) noexcept
    {
        auto text{host=="localhost"?"127.0.0.1":host.c_str()};
        auto& ipv4{reinterpret_cast<sockaddr_in&>(storage)};
        if(::inet_pton(AF_INET,text,&ipv4.sin_addr)==1)
        { ipv4.sin_family=AF_INET;ipv4.sin_port=htons(port);size=sizeof ipv4;return true; }
        auto& ipv6{reinterpret_cast<sockaddr_in6&>(storage)};
        if(::inet_pton(AF_INET6,text,&ipv6.sin6_addr)==1)
        { ipv6.sin6_family=AF_INET6;ipv6.sin6_port=htons(port);size=sizeof ipv6;return true; }
        return false;
    }
    inline bool endpoint(std::string_view uri,sockaddr_storage& address,socklen_t& size)
    {
        constexpr std::string_view scheme{"rosrpc://"};
        if(uri.substr(0,scheme.size())!=scheme||uri.size()>1024) {return false;}
        uri.remove_prefix(scheme.size());std::string_view host,port;
        if(!uri.empty()&&uri.front()=='[')
        {
            auto closing{uri.find(']')};
            if(closing==std::string_view::npos||closing+2>=uri.size()||uri[closing+1]!=':') {return false;}
            host=uri.substr(1,closing-1);port=uri.substr(closing+2);
        }
        else
        {
            auto colon{uri.rfind(':')};if(colon==std::string_view::npos) {return false;}
            host=uri.substr(0,colon);port=uri.substr(colon+1);
        }
        if(host.empty()||port.empty()) {return false;}
        std::uint64_t number{};
        try {number=integer(port,65535);}catch(...) {return false;}
        return number&&numeric_address(std::string{host},static_cast<std::uint16_t>(number),address,size);
    }
    inline std::uint32_t failure(ipc::deadline until) noexcept
    {return std::chrono::steady_clock::now()>=until||errno==ETIMEDOUT?110u:107u;}
    inline bool connect_socket(int fd,sockaddr_storage const& address,socklen_t size,ipc::deadline until)
    {
        int rc{::connect(fd,reinterpret_cast<sockaddr const*>(&address),size)};
        if(rc==0) {return true;}
        if(errno!=EINPROGRESS&&errno!=EINTR) {return false;}
        for(;;)
        {
            auto left{std::chrono::duration_cast<std::chrono::milliseconds>(until-std::chrono::steady_clock::now()).count()};
            if(left<=0) {errno=ETIMEDOUT;return false;}
            pollfd ready{fd,POLLOUT,0};
            rc=::poll(&ready,1,static_cast<int>(std::min<std::int64_t>(left,INT_MAX)));
            if(rc<0&&errno==EINTR) {continue;}
            if(rc<=0) {if(rc==0) {errno=ETIMEDOUT;}return false;}
            int error{};socklen_t bytes{sizeof error};
            if(::getsockopt(fd,SOL_SOCKET,SO_ERROR,&error,&bytes)||bytes!=sizeof error) {return false;}
            if(error) {errno=error;return false;}return true;
        }
    }
    inline void header_field(std::vector<std::uint8_t>& output,std::string_view key,std::string_view value)
    {
        ipc::put(output,key.size()+1+value.size(),4);
        output.insert(output.end(),key.begin(),key.end());output.push_back('=');
        output.insert(output.end(),value.begin(),value.end());
    }
    inline bool response_header(std::vector<std::uint8_t> const& bytes,std::string_view md5,std::string_view type)
    {
        // Header metadata is capped at sixteen fields. Fixed storage avoids
        // allocating a tree node for each untrusted field in this hot path.
        std::array<std::pair<std::string_view,std::string_view>,16> fields{};
        std::size_t position{},used{};
        while(position<bytes.size())
        {
            if(bytes.size()-position<4||used==fields.size()) {return false;}
            auto length{ipc::get(bytes.data()+position,4)};position+=4;
            if(!length||length>bytes.size()-position) {return false;}
            std::string_view field{reinterpret_cast<char const*>(bytes.data()+position),static_cast<std::size_t>(length)};
            auto equals{field.find('=')};
            if(equals==0||equals==std::string_view::npos) {return false;}
            auto key{field.substr(0,equals)};
            for(std::size_t i{};i!=used;++i) {if(fields[i].first==key) {return false;}}
            fields[used++]={key,field.substr(equals+1)};
            position+=static_cast<std::size_t>(length);
        }
        bool has_md5{},has_type{};
        for(std::size_t i{};i!=used;++i)
        {
            auto const& item{fields[i]};
            if(item.first=="error") {return false;}
            if(item.first=="md5sum") {if(item.second!=md5) {return false;}has_md5=true;}
            if(item.first=="type") {if(item.second!=type) {return false;}has_type=true;}
        }
        return has_md5&&has_type;
    }

    /** Call one official generated service after the Capability Engine admits
     * it. validate_native_body must verify the complete bounded serialized
     * structure before the official deserializer can allocate any fields.
     * The returned native object is converted to the reviewed canonical codec
     * by the caller; native serialization is never sent directly to a Guest. */
    template<typename Service,typename Validator>
    std::uint32_t call(ros::capability_spec const& cap,Service& native,std::uint32_t timeout,Validator validate_native_body)
    {
        auto until{ipc::after(static_cast<std::uint32_t>(std::min<std::uint64_t>(timeout,limit(cap,"timeout-ms"))))};
        auto md5{::ros::service_traits::MD5Sum<Service>::value()};
        auto type{::ros::service_traits::DataType<Service>::value()};
        if(cap.operation!="service.call"||cap.type!=type||cap.type_identity!=md5) {return 13;}
        sockaddr_storage master{};socklen_t master_size{};
        auto master_port{::ros::master::getPort()};
        if(!master_port||master_port>65535||!numeric_address(::ros::master::getHost(),static_cast<std::uint16_t>(master_port),master,master_size)) {return 95;}
        XmlRpc::XmlRpcValue arguments,result;
        arguments[0]=::ros::this_node::getName();arguments[1]=cap.resource;
        // Discovery may wait for an advertised server within the existing
        // timeout. Only lookup metadata is repeated, at most five times per
        // second; an admitted TCPROS request is sent exactly once.
        for(;;)
        {
            auto left{std::chrono::duration_cast<std::chrono::milliseconds>(until-std::chrono::steady_clock::now()).count()};
            if(left<=0) {return 110;}
            ros1_resources::bounded_xmlrpc lookup{::ros::master::getHost(),static_cast<int>(::ros::master::getPort()),4096};
            auto lookup_error{lookup.execute_bounded("lookupService",arguments,result,static_cast<std::uint32_t>(left))};
            if(lookup_error) {return lookup_error;}
            if(std::chrono::steady_clock::now()>=until) {return 110;}
            if(result.getType()!=XmlRpc::XmlRpcValue::TypeArray||result.size()!=3||result[0].getType()!=XmlRpc::XmlRpcValue::TypeInt)
            {return 22;}
            if(static_cast<int>(result[0])==1)
            {if(result[2].getType()!=XmlRpc::XmlRpcValue::TypeString) {return 22;}break;}
            left=std::chrono::duration_cast<std::chrono::milliseconds>(until-std::chrono::steady_clock::now()).count();
            if(left<=0) {return 110;}
            (void)::poll(nullptr,0,static_cast<int>(std::min<std::int64_t>(left,200)));
        }
        auto const& uri{static_cast<std::string&>(result[2])};sockaddr_storage address{};socklen_t address_size{};
        if(!endpoint(uri,address,address_size)) {return 95;}
        native_io::owned_fd socket{::socket(address.ss_family,SOCK_STREAM|SOCK_CLOEXEC|SOCK_NONBLOCK,0)};
        if(socket.get()<0||!connect_socket(socket.get(),address,address_size,until)) {return failure(until);}
        std::vector<std::uint8_t> header;header.reserve(1024);
        header_field(header,"callerid",::ros::this_node::getName());header_field(header,"service",cap.resource);
        header_field(header,"md5sum",md5);header_field(header,"persistent","0");
        if(header.size()>8192) {return 90;}
        std::array<std::uint8_t,4> length{};
        for(unsigned i{};i!=4;++i) {length[i]=static_cast<std::uint8_t>(header.size()>>(8*i));}
        if(!ipc::transfer(socket.get(),length.data(),length.size(),true,until)||
           !ipc::transfer(socket.get(),header.data(),header.size(),true,until)||
           !ipc::transfer(socket.get(),length.data(),length.size(),false,until)) {return failure(until);}
        auto count{ipc::get(length.data(),4)};
        if(!count||count>8192) {return 90;}header.resize(static_cast<std::size_t>(count));
        if(!ipc::transfer(socket.get(),header.data(),header.size(),false,until)) {return failure(until);}
        if(!response_header(header,md5,type)) {return 13;}
        std::vector<std::uint8_t> body;
        try
        {
            auto size{::ros::serialization::serializationLength(native.request)};
            if(size>limit(cap,"max-request-bytes")) {return 90;}
            // Official empty request/response serializers still receive a
            // real buffer address, avoiding null-pointer range arithmetic.
            body.reserve(std::max<std::size_t>(1,size));
            body.resize(size);::ros::serialization::OStream stream{body.data(),size};
            ::ros::serialization::serialize(stream,native.request);
        }
        catch(...) {return 22;}
        for(unsigned i{};i!=4;++i) {length[i]=static_cast<std::uint8_t>(body.size()>>(8*i));}
        if(!ipc::transfer(socket.get(),length.data(),length.size(),true,until)||
           !ipc::transfer(socket.get(),body.data(),body.size(),true,until)) {return failure(until);}
        std::array<std::uint8_t,5> response{};
        if(!ipc::transfer(socket.get(),response.data(),response.size(),false,until)) {return failure(until);}
        count=ipc::get(response.data()+1,4);
        if(response[0]>1) {return 22;}
        if(count>limit(cap,"max-response-bytes")||count>ipc::max_payload_bytes-518) {return 90;}
        body.resize(static_cast<std::size_t>(count));
        if(!ipc::transfer(socket.get(),body.data(),body.size(),false,until)) {return failure(until);}
        if(!response[0]) {return 107;}
        if(!validate_native_body(body)) {return 22;}
        try
        {
            ::ros::serialization::IStream stream{body.data(),static_cast<std::uint32_t>(body.size())};
            ::ros::serialization::deserialize(stream,native.response);
            if(stream.getLength()) {return 22;}
        }
        catch(...) {return 22;}
        return std::chrono::steady_clock::now()<until?0u:110u;
    }
}
