// Real ROS1 Master/Gateway regression for an explicitly bounded parent search.
// The caller supplies the freshly built Gateway executable as argv[1] and an
// isolated ROS_MASTER_URI. Compilation and execution belong exclusively to the
// qualified Linux 64 GiB, zero-swap cgroup; it must not be run locally.
#include "../ros_resources/common.h"
#include <ros/ros.h>

namespace ros1_search_native
{
    using namespace ros_resources_probe;
    unsigned checks{};
    void check(bool value,char const* detail) { ++checks;require(value,detail); }
    struct parameter_cleanup
    {
        ::ros::NodeHandle& node;std::vector<std::string> names;
        ~parameter_cleanup() { for(auto const& name:names) { node.deleteParam(name); } }
    };
}
int main(int argc,char** argv)
{
    using namespace ros1_search_native;
    try
    {
        require(argc==2,"usage ROS1 search probe freshly-built-gateway");
        ::ros::init(argc,argv,"uwvm_ros1_parent_search_probe",::ros::init_options::NoSigintHandler|::ros::init_options::NoRosout);
        ::ros::NodeHandle node;::ros::AsyncSpinner spinner{2};spinner.start();
        std::string scope{"/uwvm_resources_search"};
        std::string key{"uwvm_search_key_"+std::to_string(::getpid())};
        auto initial{scope+"/ns/child/"+key},parent{scope+"/ns/"+key},grandparent{scope+"/"+key},outside{"/"+key};
        parameter_cleanup cleanup{node,{initial,parent,grandparent,outside}};
        node.setParam(initial,std::string{"exact"});node.setParam(parent,std::string{"nearest"});
        node.setParam(grandparent,std::string{"grandparent"});node.setParam(outside,std::string{"outside-secret"});
        fixture gateway_process{core::version::ros1,argv[1],[&](gateway::configuration& cfg) {
            cfg.policy.capabilities.clear();
            for(auto alias:{"exact","disabled","parents","narrow","neighboring","short"})
            {
                auto cap{resource(core::version::ros1,alias,"param.search",initial)};
                cap.limits["allowed-name-prefix"]=std::string{alias}=="narrow"?scope+"/ns":std::string{alias}=="neighboring"?scope+"_bad":scope;
                if(std::string{alias}=="disabled") { cap.limits["search-parent-namespaces"]="false"; }
                if(std::string{alias}=="parents"||std::string{alias}=="narrow"||std::string{alias}=="neighboring"||std::string{alias}=="short") { cap.limits["search-parent-namespaces"]="true"; }
                if(std::string{alias}=="short") { cap.limits["max-parameter-bytes"]="4"; }
                add(cfg,std::move(cap));
            }
        }};
        ipc::client client;gateway_process.connect(client);
        auto search=[&](char const* alias) { return client.call("param.search",alias,{},2500); };
        check(text(search("exact"))==initial,"legacy exact search finds only initial resource");
        check(text(search("parents"))==initial,"explicit parent search preserves nearest exact match");
        check(search("short").error==90,"search result retains canonical String quota including length");
        node.deleteParam(initial);
        check(search("exact").error==2,"legacy broad prefix does not implicitly grant parents");
        check(search("disabled").error==2,"explicit false retains exact search");
        check(text(search("parents"))==parent,"explicit parent search selects nearest allowed ancestor");
        ::ros::NodeHandle native_start{scope+"/ns/child"};std::string native_name;
        check(native_start.searchParam(key,native_name)&&native_name==parent,"real native single-key search agrees inside scope");
        node.deleteParam(parent);
        check(text(search("parents"))==grandparent,"explicit search reaches next allowed ancestor");
        auto narrow{search("narrow")};check(narrow.error==2&&narrow.payload.empty(),"narrow prefix does not disclose an outside ancestor");
        auto neighboring{search("neighboring")};check(neighboring.error==13&&neighboring.payload.empty(),"segment-neighbor prefix denies initial resource");
        node.deleteParam(grandparent);
        check(native_start.searchParam(key,native_name)&&native_name==outside,"real native unrestricted search can find outside ancestor");
        auto denied_outside{search("parents")};check(denied_outside.error==2&&denied_outside.payload.empty(),"bounded guest search does not expose native outside ancestor");
        // This native IPC client can forge a body that the actual Guest ABI
        // cannot supply. The common Gateway dispatcher admits exactly zero
        // request bytes for param.search, so size admission rejects this frame
        // with project too_large (90) before its empty-request check (22).
        // Require an empty response as well: rejection must disclose no name.
        auto substituted{client.call("param.search","parents",{1},2500)};
        check(substituted.error==90&&substituted.payload.empty(),"forged native IPC search body exceeds fixed zero-byte request quota");
        check(client.call("param.get","parents",{},2500).error==13,"search capability cannot read parameter value");
        ipc::client unbound;gateway_process.connect(unbound,false);
        auto altered{gateway_process.config.policy};altered.capabilities.at("parents").limits["search-parent-namespaces"]="false";
        check(unbound.call("session.bind","target",core::binding_manifest(altered),1000).error==13,"parent authority participates in exact policy binding");unbound.close();
        check(!client.call("capability.revoke","parents",{},1000).error&&search("parents").error==125,"revocation retires parent-search authority");
        client.close();spinner.stop();
        gateway::native_io::fixed_text<160> result;
        result.append("PASS actual ROS1 exact/explicit-parent search and namespace isolation checks=");result.number(checks);result.append("\n");
        gateway::native_io::write_all(STDOUT_FILENO,result.view());return 0;
    }
    catch(std::exception const& error)
    { uwvm2::ros_gateway::native_io::write_all(STDERR_FILENO,std::string{"FAIL "}+error.what()+"\n");return 1; }
}
