#pragma once
#include "../common/server.h"
#include <ros/ros.h>
#include <ros/master.h>
#include "compat.h"
#include <algorithm>
#include <cctype>
#include <set>
#include <uwvm2/imported/ros/ipc/parameter.h>

namespace uwvm2::ros_gateway::ros1_resources
{
    inline bool prefix(std::string const& scope,std::string const& name)
    { return scope=="/" || name==scope || (name.size()>scope.size() && name.compare(0,scope.size(),scope)==0 && name[scope.size()]=='/'); }
    inline bool known(ros::capability_spec const& cap)
    {
        if(cap.operation=="graph.list-topics"||cap.operation=="graph.list-nodes"||cap.operation=="graph.list-services"||cap.operation=="graph.inspect-topic"||cap.operation=="graph.inspect-node"||cap.operation=="graph.inspect-service")
        { return cap.type=="builtin/graph-list-v1"&&cap.type_identity=="builtin:graph-list-v1"; }
        if(cap.operation=="param.get"||cap.operation=="param.set")
        { return ipc::validate_parameter_contract(ros::version::ros1,cap)==0; }
        if(cap.operation=="param.delete"||cap.operation=="param.exists"||cap.operation=="param.search"||cap.operation=="param.list")
        { return cap.type=="builtin/string-v1"&&cap.type_identity=="builtin:param-string-v1"; }
        return false;
    }
    class bounded_xmlrpc final:public XmlRpc::XmlRpcClient
    {
        std::size_t maximum_;
        bool failed_{};
        bool receive(std::string& buffer,std::size_t maximum)
        {
            char chunk[4096]; auto n{::recv(getfd(),chunk,sizeof chunk,MSG_DONTWAIT)};
            if(n<0) { if(errno==EAGAIN||errno==EWOULDBLOCK||errno==EINTR) { return true; } failed_=true; close(); return false; }
            if(!n) { _eof=true; return true; }
            if(static_cast<std::size_t>(n)>maximum-buffer.size()) { failed_=true; close(); return false; }
            buffer.append(chunk,static_cast<std::size_t>(n)); return true;
        }
        bool readHeader() override
        {
            if(!receive(_header,8192)) { return false; }
            auto end{_header.find("\r\n\r\n")};
            if(end==std::string::npos) { if(_eof) { failed_=true; close(); return false; } return true; }
            auto header{_header.substr(0,end)};
            if(header.rfind("HTTP/1.1 200",0)!=0&&header.rfind("HTTP/1.0 200",0)!=0) { failed_=true; close(); return false; }
            std::transform(header.begin(),header.end(),header.begin(),[](unsigned char c) { return static_cast<char>(std::tolower(c)); });
            auto at{header.find("\r\ncontent-length:")};
            if(at==std::string::npos||header.find("\r\ncontent-length:",at+2)!=std::string::npos||header.find("transfer-encoding:")!=std::string::npos)
            { failed_=true; close(); return false; }
            at+=17; while(at<header.size()&&(header[at]==' '||header[at]=='\t')) { ++at; }
            auto finish{header.find("\r\n",at)}; auto text{header.substr(at,finish==std::string::npos?finish:finish-at)};
            std::uint64_t length{};
            try { length=integer(text,maximum_); } catch(...) { failed_=true; close(); return false; }
            if(!length) { failed_=true; close(); return false; }
            _contentLength=static_cast<int>(length); _response=_header.substr(end+4); _header.resize(end+4);
            if(_response.size()>length) { failed_=true; close(); return false; }
            _connectionState=READ_RESPONSE; return true;
        }
        bool readResponse() override
        {
            if(_response.size()<static_cast<std::size_t>(_contentLength) && !receive(_response,static_cast<std::size_t>(_contentLength))) { return false; }
            if(_response.size()!=static_cast<std::size_t>(_contentLength)) { if(_eof) { failed_=true; close(); return false; } return true; }
            _connectionState=IDLE; return false;
        }
        bool parseResponse(XmlRpc::XmlRpcValue& result) override
        {
            // Reject recursive XML bombs before xmlrpcpp's recursive value parser.
            unsigned depth{};
            for(std::size_t p{};(p=_response.find('<',p))!=std::string::npos;)
            {
                auto end{_response.find('>',p)}; if(end==std::string::npos) { return false; }
                if(_response.compare(p,5,"<?xml")==0) { p=end+1; continue; }
                if(p+1==_response.size()||_response[p+1]=='!'||_response[p+1]=='?') { return false; }
                if(_response[p+1]=='/') { if(!depth) { return false; } --depth; }
                else if(_response[end-1]!='/') { if(++depth>32) { return false; } }
                p=end+1;
            }
            return !depth&&XmlRpc::XmlRpcClient::parseResponse(result);
        }
    public:
        bounded_xmlrpc(std::string const& host,int port,std::size_t maximum):XmlRpc::XmlRpcClient(host.c_str(),port,"/"),maximum_{maximum} {}
        std::uint32_t execute_bounded(char const* method,XmlRpc::XmlRpcValue const& params,XmlRpc::XmlRpcValue& value,std::uint32_t timeout)
        {
            auto deadline{std::chrono::steady_clock::now()+std::chrono::milliseconds(timeout)};
            if(!executeNonBlock(method,params)) { return 107; }
            while(std::chrono::steady_clock::now()<deadline)
            {
                _disp.work(0.001);
                if(executeCheckDone(value)) { close(); return failed_?90:!value.valid()||isFault()?107:0; }
            }
            close(); return 110;
        }
    };
    inline ipc::call_result string_result(std::string const& value,std::uint64_t maximum)
    {
        if(!wire_size_fits(maximum,{4,value.size()})) { return {90,{}}; }if(!valid_utf8(reinterpret_cast<std::uint8_t const*>(value.data()),value.size())) { return {22,{}}; }
        std::vector<std::uint8_t> result;result.reserve(4+value.size());ipc::put(result,value.size(),4);result.insert(result.end(),value.begin(),value.end());return {0,std::move(result)};
    }
    inline XmlRpc::XmlRpcValue::Type native_parameter_kind(ipc::parameter_kind kind)
    {
        if(ipc::array_parameter(kind)) { kind=ipc::parameter_element_kind(kind); }
        if(kind==ipc::parameter_kind::string) { return XmlRpc::XmlRpcValue::TypeString; }
        if(kind==ipc::parameter_kind::boolean) { return XmlRpc::XmlRpcValue::TypeBoolean; }
        if(kind==ipc::parameter_kind::int32) { return XmlRpc::XmlRpcValue::TypeInt; }
        if(kind==ipc::parameter_kind::float64) { return XmlRpc::XmlRpcValue::TypeDouble; }
        return XmlRpc::XmlRpcValue::TypeInvalid;
    }
    inline std::uint32_t native_parameter_type(ipc::parameter_kind kind,XmlRpc::XmlRpcValue& native)
    {
        auto expected{native_parameter_kind(kind)};
        if(!ipc::array_parameter(kind)) { return native.getType()==expected?0:95; }
        if(native.getType()!=XmlRpc::XmlRpcValue::TypeArray) { return 95; }
        // XMLRPC arrays have no element-kind metadata. An empty array can be
        // represented by any explicitly selected ROS1 array projection.
        if(native.size()>1048576) { return 90; }
        for(int n{};n<native.size();++n) { if(native[n].getType()!=expected) { return 95; } }
        return 0;
    }
    inline ipc::call_result parameter_result(ros::capability_spec const& cap,XmlRpc::XmlRpcValue& native)
    {
        ipc::parameter_value value;value.type=ipc::parameter_type(ros::version::ros1,cap.type);
        if(ipc::array_parameter(value.type))
        {
            if(native.getType()!=XmlRpc::XmlRpcValue::TypeArray) { return {95,{}}; }
            auto count{static_cast<std::uint64_t>(native.size())},maximum{limit(cap,"max-parameter-bytes")};
            if(count>limit(cap,"max-parameter-elements")||maximum<4) { return {90,{}}; }
            auto error{native_parameter_type(value.type,native)};if(error) { return {error,{}}; }
            auto remaining{maximum-4};
            if(value.type==ipc::parameter_kind::string_array)
            {
                for(int n{};n<native.size();++n)
                {
                    auto const& text{static_cast<std::string&>(native[n])};
                    if(remaining<4||text.size()>remaining-4) { return {90,{}}; }
                    if(!ipc::parameter_array_string_valid(ros::version::ros1,text)) { return {22,{}}; }
                    remaining-=4+text.size();
                }
            }
            else if(count>remaining/ipc::parameter_element_width(value.type)) { return {90,{}}; }
            // Native element kind/count/full encoding size are checked before
            // allocating any bridge-owned value vectors or string copies.
            for(int n{};n<native.size();++n)
            {
                if(value.type==ipc::parameter_kind::boolean_array) { value.bool_array_value.push_back(static_cast<bool>(native[n])?1:0); }
                else if(value.type==ipc::parameter_kind::int32_array) { value.int32_array_value.push_back(static_cast<int>(native[n])); }
                else if(value.type==ipc::parameter_kind::float64_array) { value.double_array_value.push_back(static_cast<double>(native[n])); }
                else { value.string_array_value.push_back(static_cast<std::string&>(native[n])); }
            }
        }
        else if(value.type==ipc::parameter_kind::string)
        {
            if(native.getType()!=XmlRpc::XmlRpcValue::TypeString) { return {95,{}}; }
            auto const& text{static_cast<std::string&>(native)};
            if(!wire_size_fits(limit(cap,"max-parameter-bytes"),{4,text.size()})) { return {90,{}}; }
            value.string_value=text;
        }
        else if(value.type==ipc::parameter_kind::boolean)
        { if(native.getType()!=XmlRpc::XmlRpcValue::TypeBoolean) { return {95,{}}; }value.bool_value=static_cast<bool>(native); }
        else if(value.type==ipc::parameter_kind::int32)
        { if(native.getType()!=XmlRpc::XmlRpcValue::TypeInt) { return {95,{}}; }value.integer_value=static_cast<int>(native); }
        else if(value.type==ipc::parameter_kind::float64)
        { if(native.getType()!=XmlRpc::XmlRpcValue::TypeDouble) { return {95,{}}; }value.double_value=static_cast<double>(native); }
        else { return {95,{}}; }
        std::vector<std::uint8_t> bytes;auto error{ipc::encode_parameter(value,bytes)};
        if(!error) { error=ipc::validate_parameter_value(ros::version::ros1,cap,bytes.data(),bytes.size()); }
        return error?ipc::call_result{error,{}}:ipc::call_result{0,std::move(bytes)};
    }
    inline ipc::call_result call(::ros::NodeHandle&,ros::capability_spec const& cap,std::vector<std::uint8_t> const& request,std::uint32_t timeout)
    {
        if(!known(cap)) { return {95,{}}; }
        bool graph{cap.operation.rfind("graph.",0)==0};
        auto maximum{limit(cap,graph?"max-graph-result-bytes":"max-parameter-bytes")};
        auto bounded_timeout{std::min(timeout,static_cast<std::uint32_t>(limit(cap,"timeout-ms")))};
        auto deadline{std::chrono::steady_clock::now()+std::chrono::milliseconds(bounded_timeout)};
        if(cap.operation=="param.search")
        {
            if(!request.empty()) { return {22,{}}; }
            // No native query is made before the initial resource is allowed.
            // Parent candidates are additional authority only when explicitly
            // enabled; the prefix alone does not widen an older exact grant.
            if(!ros::parameter_search_name_allowed(cap,cap.resource)) { return {13,{}}; }
            std::string candidate{cap.resource};
            for(;;)
            {
                if(!ros::parameter_search_name_allowed(cap,candidate)) { return {2,{}}; }
                auto left{std::chrono::duration_cast<std::chrono::milliseconds>(deadline-std::chrono::steady_clock::now()).count()};
                if(left<=0) { return {110,{}}; }
                XmlRpc::XmlRpcValue search_args,search_response;
                search_args[0]=::ros::this_node::getName();search_args[1]=candidate;
                bounded_xmlrpc search{::ros::master::getHost(),static_cast<int>(::ros::master::getPort()),static_cast<std::size_t>(maximum*6+65536)};
                auto error{search.execute_bounded("hasParam",search_args,search_response,static_cast<std::uint32_t>(left))};
                if(error) { return {error,{}}; }
                // The common deadline also bounds transitions between calls.
                // It cannot interrupt a blocking native XMLRPC operation.
                if(std::chrono::steady_clock::now()>=deadline) { return {110,{}}; }
                if(search_response.getType()!=XmlRpc::XmlRpcValue::TypeArray||search_response.size()!=3||search_response[0].getType()!=XmlRpc::XmlRpcValue::TypeInt||static_cast<int>(search_response[0])!=1) { return {2,{}}; }
                if(search_response[2].getType()!=XmlRpc::XmlRpcValue::TypeBoolean) { return {22,{}}; }
                if(static_cast<bool>(search_response[2])) { return string_result(candidate,maximum); }
                if(!ros::parameter_search_parents_enabled(cap)||!ros::parameter_search_next_candidate(candidate)) { return {2,{}}; }
            }
        }
        XmlRpc::XmlRpcValue args,response; args[0]=::ros::this_node::getName();
        char const* method{};
        if(cap.operation=="param.get") { method="getParam"; args[1]=cap.resource; }
        else if(cap.operation=="param.set")
        {
            ipc::parameter_value value;auto error{ipc::validate_parameter_value(ros::version::ros1,cap,request.data(),request.size(),&value)};if(error) { return {error,{}}; }
            method="setParam";args[1]=cap.resource;
            if(value.type==ipc::parameter_kind::string) { args[2]=value.string_value; }
            else if(value.type==ipc::parameter_kind::boolean) { args[2]=value.bool_value; }
            else if(value.type==ipc::parameter_kind::int32) { args[2]=static_cast<int>(value.integer_value); }
            else if(value.type==ipc::parameter_kind::float64) { args[2]=value.double_value; }
            else if(ipc::array_parameter(value.type))
            {
                auto count{ipc::parameter_array_size(value)};args[2].setSize(static_cast<int>(count));
                for(std::size_t n{};n<count;++n)
                {
                    if(value.type==ipc::parameter_kind::boolean_array) { args[2][static_cast<int>(n)]=value.bool_array_value[n]!=0; }
                    else if(value.type==ipc::parameter_kind::int32_array) { args[2][static_cast<int>(n)]=static_cast<int>(value.int32_array_value[n]); }
                    else if(value.type==ipc::parameter_kind::float64_array) { args[2][static_cast<int>(n)]=value.double_array_value[n]; }
                    else if(value.type==ipc::parameter_kind::string_array) { args[2][static_cast<int>(n)]=value.string_array_value[n]; }
                    else { return {95,{}}; }
                }
            }
            else { return {95,{}}; }
            if(value.type!=ipc::parameter_kind::string)
            {
                // XMLRPC setParam has no conditional type operation. Refuse a
                // currently different native kind before the authorized write.
                XmlRpc::XmlRpcValue current_args,current;current_args[0]=::ros::this_node::getName();current_args[1]=cap.resource;
                bounded_xmlrpc inspect{::ros::master::getHost(),static_cast<int>(::ros::master::getPort()),static_cast<std::size_t>(maximum*6+65536)};
                auto left{std::chrono::duration_cast<std::chrono::milliseconds>(deadline-std::chrono::steady_clock::now()).count()};if(left<=0) { return {110,{}}; }
                error=inspect.execute_bounded("getParam",current_args,current,static_cast<std::uint32_t>(left));if(error) { return {error,{}}; }
                if(current.getType()!=XmlRpc::XmlRpcValue::TypeArray||current.size()!=3||current[0].getType()!=XmlRpc::XmlRpcValue::TypeInt) { return {22,{}}; }
                auto status{static_cast<int>(current[0])};
                if(status==1)
                {
                    error=native_parameter_type(value.type,current[2]);if(error) { return {error,{}}; }
                }
                // A missing parameter may be created by its explicit typed set
                // grant. Existing values are checked by kind only, permitting a
                // bounded finite write to repair an out-of-range native value.
                else if(status!=-1) { return {2,{}}; }
            }
        }
        else if(cap.operation=="param.delete") { method="deleteParam"; args[1]=cap.resource; }
        else if(cap.operation=="param.exists") { method="hasParam"; args[1]=cap.resource; }
        else if(cap.operation=="param.list") { method="getParamNames"; }
        else if(cap.operation=="graph.list-topics"||cap.operation=="graph.inspect-topic") { method="getTopicTypes"; }
        else { method="getSystemState"; }
        if(cap.operation!="param.set"&&!request.empty()) { return {22,{}}; }
        // XML escape growth is bounded by 6x the explicit result limit plus a
        // public 64 KiB envelope ceiling; each recv is bounded before append.
        bounded_xmlrpc client{::ros::master::getHost(),static_cast<int>(::ros::master::getPort()),static_cast<std::size_t>(maximum*6+65536)};
        auto left{std::chrono::duration_cast<std::chrono::milliseconds>(deadline-std::chrono::steady_clock::now()).count()};if(left<=0) { return {110,{}}; }
        auto error{client.execute_bounded(method,args,response,static_cast<std::uint32_t>(left))}; if(error) { return {error,{}}; }
        if(response.getType()!=XmlRpc::XmlRpcValue::TypeArray||response.size()!=3||response[0].getType()!=XmlRpc::XmlRpcValue::TypeInt||static_cast<int>(response[0])!=1) { return {2,{}}; }
        auto& value{response[2]};
        if(cap.operation=="param.set"||cap.operation=="param.delete") { return {}; }
        if(cap.operation=="param.exists")
        {
            if(value.getType()!=XmlRpc::XmlRpcValue::TypeBoolean) { return {22,{}}; }
            bool exists{static_cast<bool>(value)};
            return {0,{static_cast<std::uint8_t>(exists?1:0)}};
        }
        if(cap.operation=="param.get")
        { return parameter_result(cap,value); }
        std::set<std::string> rows;
        auto add=[&](std::string const& name,std::string const& type)
        {
            if(!canonical(name)||!prefix(cap.resource,name)||!prefix(field(cap,"allowed-name-prefix"),name)) { return; }
            if(cap.operation.rfind("graph.inspect-",0)==0&&name!=cap.resource) { return; }
            std::string row{name}; if(!type.empty()) { if(type.find_first_of("\t\r\n")!=std::string::npos) { return; } row+='\t'; row+=type; } row+='\n'; rows.insert(std::move(row));
        };
        if(cap.operation=="param.list")
        { if(value.getType()!=XmlRpc::XmlRpcValue::TypeArray) { return {22,{}}; } for(int i{};i<value.size();++i) { if(value[i].getType()!=XmlRpc::XmlRpcValue::TypeString) { return {22,{}}; } add(static_cast<std::string>(value[i]),""); } }
        else if(cap.operation=="graph.list-topics"||cap.operation=="graph.inspect-topic")
        { if(value.getType()!=XmlRpc::XmlRpcValue::TypeArray) { return {22,{}}; } for(int i{};i<value.size();++i) { auto& row{value[i]}; if(row.getType()!=XmlRpc::XmlRpcValue::TypeArray||row.size()!=2||row[0].getType()!=XmlRpc::XmlRpcValue::TypeString||row[1].getType()!=XmlRpc::XmlRpcValue::TypeString) { return {22,{}}; } add(static_cast<std::string>(row[0]),static_cast<std::string>(row[1])); } }
        else
        {
            if(value.getType()!=XmlRpc::XmlRpcValue::TypeArray||value.size()!=3) { return {22,{}}; }
            for(int category{};category<3;++category)
            {
                if(value[category].getType()!=XmlRpc::XmlRpcValue::TypeArray) { return {22,{}}; }
                for(int i{};i<value[category].size();++i)
                {
                    auto& row{value[category][i]}; if(row.getType()!=XmlRpc::XmlRpcValue::TypeArray||row.size()!=2||row[0].getType()!=XmlRpc::XmlRpcValue::TypeString||row[1].getType()!=XmlRpc::XmlRpcValue::TypeArray) { return {22,{}}; }
                    if((cap.operation=="graph.list-services"||cap.operation=="graph.inspect-service")&&category==2) { add(static_cast<std::string>(row[0]),"not-applicable"); }
                    if(cap.operation=="graph.list-nodes"||cap.operation=="graph.inspect-node") { for(int j{};j<row[1].size();++j) { if(row[1][j].getType()!=XmlRpc::XmlRpcValue::TypeString) { return {22,{}}; } add(static_cast<std::string>(row[1][j]),""); } }
                }
            }
        }
        if(cap.operation.rfind("graph.inspect-",0)==0&&rows.empty()) { return {2,{}}; }
        std::string result; for(auto const& row:rows) { if(row.size()>maximum-std::min<std::uint64_t>(maximum,result.size()+4)) { return {90,{}}; } result+=row; }
        return string_result(result,maximum);
    }
}
