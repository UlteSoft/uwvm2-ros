// C++17 regression for the exact contract used by the native ROS1 Gateway.
// Compile and execute only in the qualified Linux 64 GiB cgroup. No ROS
// middleware is needed; this exercises real Gateway validation and the shared
// candidate helper, including the absence of implicit parent-search authority.
#include "../../gateways/common/config.h"
#include <vector>
namespace core=uwvm2::imported::ros;
namespace gateway=uwvm2::ros_gateway;
int main()
{
    try
    {
        core::capability_spec cap{"search","param.search","/scope/child/key","builtin/string-v1","builtin:param-string-v1",{}};
        cap.limits={{"allowed-name-prefix","/scope"},{"max-parameter-bytes","1024"},{"timeout-ms","2000"},{"max-inflight","1"},{"max-rate-hz","100"},{"burst","10"},{"max-bytes-per-second","65536"},{"validator","none"},{"audit","allow-and-deny"},{"audit-max-rate-hz","100"},{"audit-burst","10"},{"audit-max-records","10000"},{"audit-max-bytes","1048576"}};
        auto check=[](bool value,char const* message) { if(!value) { throw std::runtime_error(message); } };
        gateway::validate(core::version::ros1,cap);
        check(core::parameter_search_name_allowed(cap,cap.resource),"initial exact candidate missing");
        check(!core::parameter_search_name_allowed(cap,"/scope/key"),"broad prefix must not enable parents");
        cap.limits["search-parent-namespaces"]="true";gateway::validate(core::version::ros1,cap);
        check(core::parameter_search_name_allowed(cap,"/scope/key"),"explicit permitted parent missing");
        check(!core::parameter_search_name_allowed(cap,"/key"),"root outside prefix must remain denied");
        check(!core::parameter_search_name_allowed(cap,"/scope_bad/key"),"neighboring namespace must remain denied");
        std::string candidate{cap.resource};std::vector<std::string> queried;
        while(core::parameter_search_name_allowed(cap,candidate))
        { queried.push_back(candidate);if(!core::parameter_search_next_candidate(candidate)) { break; } }
        check(queried==std::vector<std::string>{"/scope/child/key","/scope/key"},"prefix must stop the walk before an outside query");
        cap.limits["search-parent-namespaces"]="false";gateway::validate(core::version::ros1,cap);
        check(!core::parameter_search_name_allowed(cap,"/scope/key"),"false must retain exact behavior");
        cap.limits["search-parent-namespaces"]="1";
        bool rejected{};try { gateway::validate(core::version::ros1,cap); }catch(std::exception const&) { rejected=true; }
        check(rejected,"invalid parent-search spelling accepted");
        cap.resource="/";cap.limits["allowed-name-prefix"]="/";cap.limits["search-parent-namespaces"]="true";
        check(core::parameter_search_name_allowed(cap,"/"),"root exact helper membership differs");
        check(!core::parameter_search_name_allowed(cap,"/key"),"root resource must not create descendants");
        candidate="/";check(!core::parameter_search_next_candidate(candidate)&&candidate=="/","root candidate has no successor");
        rejected=false;try { gateway::validate(core::version::ros1,cap); }catch(std::exception const&) { rejected=true; }
        check(rejected,"existing non-root search grant validation must not be widened");
        gateway::native_io::write_all(STDOUT_FILENO,"PASS C++17 Gateway explicit parent-search contract\n");return 0;
    }
    catch(std::exception const& error)
    { gateway::native_io::write_all(STDERR_FILENO,std::string{"FAIL "}+error.what()+"\n");return 1; }
}
