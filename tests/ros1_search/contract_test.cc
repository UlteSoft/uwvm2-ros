// Prepared Worker and native-Gateway contract regression. Run only inside the
// qualified Linux 64 GiB cgroup. This translation unit requires C++20 for the
// Worker policy API; the shared search helper itself remains C++17 compatible.
#include <uwvm2/imported/ros/policy/policy.h>
#include "../../gateways/common/config.h"
#include <fast_io.h>
#include <vector>

namespace search_contract_test
{
    namespace core=uwvm2::imported::ros;
    unsigned checks{};
    void require(bool success,char const* detail)
    { ++checks;if(!success) { throw std::runtime_error(detail); } }
    core::capability_spec grant(std::string resource="/scope/ns/child/key",std::string prefix="/scope")
    {
        core::capability_spec cap{"search","param.search",std::move(resource),"builtin/string-v1","builtin:param-string-v1",{}};
        cap.limits={{"allowed-name-prefix",std::move(prefix)},{"max-parameter-bytes","1024"},{"timeout-ms","2000"},{"max-inflight","1"},{"max-rate-hz","100"},{"burst","10"},{"max-bytes-per-second","65536"},{"validator","none"},{"audit","allow-and-deny"},{"audit-max-rate-hz","100"},{"audit-burst","10"},{"audit-max-records","10000"},{"audit-max-bytes","1048576"}};
        return cap;
    }
    std::vector<std::byte> wire(std::string_view name)
    {
        std::vector<std::byte> result;
        for(unsigned n{};n!=4;++n) { result.push_back(std::byte((name.size()>>(n*8))&255)); }
        for(unsigned char value:name) { result.push_back(std::byte{value}); }
        return result;
    }
    void response(core::capability_spec const& cap,std::string_view name,core::errc expected)
    {
        auto encoded{wire(name)};
        require(core::validate_response_payload(core::version::ros1,cap,encoded).code==expected,"Worker search response contract differs");
    }
    void schema(core::capability_spec const& cap,bool accepted)
    {
        require(static_cast<bool>(core::validate_capability(core::version::ros1,cap))==accepted,"Worker optional parent-search field validation differs");
        bool gateway_accepts{true};
        try { uwvm2::ros_gateway::validate(core::version::ros1,cap); }
        catch(std::exception const&) { gateway_accepts=false; }
        require(gateway_accepts==accepted,"Gateway optional parent-search field validation differs");
    }
}
int main()
{
    using namespace search_contract_test;
    try
    {
        auto exact{grant()};schema(exact,true);
        require(!core::parameter_search_parents_enabled(exact),"missing field must retain exact behavior");
        response(exact,"/scope/ns/child/key",core::errc::success);
        for(auto name:{"/scope/ns/key","/scope/key","/key","/scope/ns/child/other","/scope/ns/child/key/extra","/scope_bad/key","/scope//key","/scope/ns/../key","key",""})
        { response(exact,name,core::errc::permission_denied); }
        auto disabled{exact};disabled.limits["search-parent-namespaces"]="false";schema(disabled,true);
        response(disabled,"/scope/ns/key",core::errc::permission_denied);
        auto parents{exact};parents.limits["search-parent-namespaces"]="true";schema(parents,true);
        require(core::parameter_search_parents_enabled(parents),"true must explicitly enable parents");
        for(auto name:{"/scope/ns/child/key","/scope/ns/key","/scope/key"}) { response(parents,name,core::errc::success); }
        for(auto name:{"/key","/scope/ns_bad/key","/scope_bad/key","/scope/ns/child/key/extra","/scope/ns/../key","/scope//key","/scope/ns/child/sibling/key","/scope/ns/child/other"})
        { response(parents,name,core::errc::permission_denied); }
        auto root{parents};root.limits["allowed-name-prefix"]="/";response(root,"/key",core::errc::success);
        auto narrow{parents};narrow.limits["allowed-name-prefix"]="/scope/ns";
        response(narrow,"/scope/ns/key",core::errc::success);response(narrow,"/scope/key",core::errc::permission_denied);
        auto neighboring{parents};neighboring.limits["allowed-name-prefix"]="/scope_bad";
        response(neighboring,neighboring.resource,core::errc::permission_denied);
        for(auto value:{"TRUE","1","yes","","false ","true\n"})
        { auto invalid{exact};invalid.limits["search-parent-namespaces"]=value;schema(invalid,false); }
        for(auto operation:{"param.get","param.exists","param.list"})
        {
            auto invalid{parents};invalid.operation=operation;
            if(invalid.operation!="param.list") { invalid.limits.erase("allowed-name-prefix"); }
            schema(invalid,false);invalid.limits.erase("search-parent-namespaces");schema(invalid,true);
        }
        auto ros2{parents};ros2.operation="param.get";ros2.limits.erase("allowed-name-prefix");
        ros2.limits["parameter-node"]="/scope/ns/child";ros2.limits["parameter-name"]="key";
        ros2.limits.insert({{"qos.reliability","reliable"},{"qos.durability","volatile"},{"qos.history","keep-last"},{"qos.depth","8"},{"qos.deadline","infinite"},{"qos.lifespan","infinite"},{"qos.liveliness","automatic"},{"qos.liveliness-lease-duration","infinite"}});
        require(!core::validate_capability(core::version::ros2,ros2),"ROS2 cannot acquire this ROS1 field");
        bool ros2_rejected{};try { uwvm2::ros_gateway::validate(core::version::ros2,ros2); }catch(std::exception const&) { ros2_rejected=true; }
        require(ros2_rejected,"ROS2 Gateway cannot acquire this ROS1 field");ros2.limits.erase("search-parent-namespaces");
        require(static_cast<bool>(core::validate_capability(core::version::ros2,ros2)),"ROS2 calibration without this field must be valid");
        uwvm2::ros_gateway::validate(core::version::ros2,ros2);
        auto bytes{wire(exact.resource)};bytes[0]^=std::byte{1};
        require(core::validate_response_payload(core::version::ros1,exact,bytes).code==core::errc::invalid_argument,"length framing remains validated first");
        auto quota{parents};quota.limits["max-parameter-bytes"]="4";
        response(quota,parents.resource,core::errc::too_large);
        std::string candidate{exact.resource};std::vector<std::string> actual;
        do { actual.push_back(candidate); }while(core::parameter_search_next_candidate(candidate));
        require(actual==std::vector<std::string>{"/scope/ns/child/key","/scope/ns/key","/scope/key","/key"},"fixed-leaf candidate order differs");
        require(!core::parameter_search_next_candidate(candidate)&&candidate=="/key","root candidate must be stable");
        auto root_resource{grant("/","/")};
        require(core::parameter_search_name_allowed(root_resource,"/"),"helper retains exact root membership");
        root_resource.limits["search-parent-namespaces"]="true";
        require(!core::parameter_search_name_allowed(root_resource,"/key"),"root resource has no parent candidates");
        schema(root_resource,false); // Existing configured search grants require a non-root absolute name.
        candidate="/";require(!core::parameter_search_next_candidate(candidate)&&candidate=="/","root namespace has no successor");
        for(auto target:{core::scope::global,core::scope::single,core::scope::group})
        {
            core::policy_store store;std::string name{target==core::scope::global?"":"search-owner"};
            if(target!=core::scope::global) { require(static_cast<bool>(store.create_target(core::version::ros1,target,name)),"create explicit search target"); }
            require(static_cast<bool>(store.grant(core::version::ros1,target,name,exact)),"grant exact search before CLI field");
            require(store.set_capability(core::version::ros1,target,name,"search","search-parent-namespaces","1").code==core::errc::invalid_argument,"CLI rejects invalid boolean before insertion");
            require(static_cast<bool>(store.set_capability(core::version::ros1,target,name,"search","search-parent-namespaces","true")),"CLI accepts explicit parent field on each target scope");
            require(store.set_capability(core::version::ros1,target,name,"search","search-parent-namespaces","false").code==core::errc::conflict,"CLI duplicate parent field remains conflict");
            auto configured{store.find_target(core::version::ros1,target,name)};
            require(configured&&core::parameter_search_parents_enabled(configured->capabilities.at("search")),"CLI parent field stored in exact selected target");
        }
        core::policy_store non_search;auto get{exact};get.operation="param.get";get.limits.erase("allowed-name-prefix");
        require(static_cast<bool>(non_search.grant(core::version::ros1,core::scope::global,"",get)),"CLI non-search calibration grant");
        require(non_search.set_capability(core::version::ros1,core::scope::global,"","search","search-parent-namespaces","true").code==core::errc::invalid_argument,"CLI cannot assign parent field to param.get");
        core::policy_store false_search;
        require(static_cast<bool>(false_search.grant(core::version::ros1,core::scope::global,"",exact)),"CLI false calibration grant");
        require(static_cast<bool>(false_search.set_capability(core::version::ros1,core::scope::global,"","search","search-parent-namespaces","false")),"CLI accepts explicit false");
        require(!core::parameter_search_parents_enabled(false_search.find_target(core::version::ros1,core::scope::global,"")->capabilities.at("search")),"CLI false must retain exact behavior");
        core::policy_store ros2_search;
        require(static_cast<bool>(ros2_search.grant(core::version::ros2,core::scope::global,"",ros2)),"CLI ROS2 get calibration grant");
        require(ros2_search.set_capability(core::version::ros2,core::scope::global,"","search","search-parent-namespaces","true").code==core::errc::invalid_argument,"CLI cannot assign ROS1 parent authority to ROS2");
        require(static_cast<bool>(core::validate_request_payload(core::version::ros1,parents,{})),"search request remains empty");
        std::vector<std::byte> substituted{std::byte{1}};
        require(core::validate_request_payload(core::version::ros1,parents,substituted).code==core::errc::invalid_argument,"guest cannot substitute the fixed key");
        fast_io::print(fast_io::out(),"PASS ROS1 explicit parent-search Worker/Gateway contract checks=",checks,"\n");return 0;
    }
    catch(std::exception const& error)
    { fast_io::print(fast_io::err(),"FAIL ",fast_io::mnp::os_c_str(error.what()),"\n");return 1; }
}
