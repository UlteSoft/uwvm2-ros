/** Regression for actual Gateway availability limit admission.
 * Source only, not a native graph-event wait or ROS availability observation.
 * Both real policy validators run on complete independently explicit grants.
 */
#include <uwvm2/imported/ros/policy/schema.h>
#include "../../../../../src/ros/gateway/common/config.h"
#include <cstdlib>
namespace ros=uwvm2::imported::ros;
namespace gateway=uwvm2::ros_gateway;
static void need(bool value){if(!value)std::abort();}
static ros::capability_spec availability()
{
    ros::capability_spec c{"available","service.available","/wait_contract",
        "std_srvs/srv/Empty",{}, {}};
    c.type_identity=ros::builtin_type_identity(ros::version::ros2,c.type);
    for(auto key:ros::operation_schema(ros::version::ros2,c.operation,c.type))c.limits.emplace(key,"1");
    c.limits["service-source-alias"]="source";
    c.limits["timeout-ms"]="2001";c.limits["max-wait-ns"]="2000000000";
    c.limits["max-native-clients"]="1";c.limits["max-binding-history"]="2";
    c.limits["max-inflight"]="1";c.limits["max-request-bytes"]="64";c.limits["max-response-bytes"]="64";
    c.limits["max-bytes-per-second"]="128";c.limits["max-rate-hz"]="1";c.limits["burst"]="1";
    c.limits["audit"]="deny-only";c.limits["audit-max-rate-hz"]="1";c.limits["audit-burst"]="1";
    c.limits["audit-max-records"]="16";c.limits["audit-max-bytes"]="4096";
    c.limits["qos.reliability"]="reliable";c.limits["qos.durability"]="volatile";
    c.limits["qos.history"]="keep-last";c.limits["qos.depth"]="1";c.limits["qos.liveliness"]="automatic";
    for(auto key:{"qos.deadline","qos.lifespan","qos.liveliness-lease-duration"})c.limits[key]="infinite";
    return c;
}
static bool accepted(ros::capability_spec const&c)
{try{gateway::validate(ros::version::ros2,c);return true;}catch(std::runtime_error const&){return false;}}
static bool limited(ros::capability_spec const&c,std::string_view key)
{try{(void)gateway::limit(c,key);return false;}catch(std::runtime_error const&){return true;}}
int main()
{
    // A valid two-second maximum used to pass the dedicated ns parser and then
    // fail the generic1GiB helper. It is a maximum grant, not an actual sleep.
    auto c=availability();need(bool(ros::validate_capability(ros::version::ros2,c))&&accepted(c));
    need(gateway::limit(c,"max-wait-ns")==UINT64_C(2000000000));
    // The largest whole-ms ceiling with genuine1ms transport headroom remains
    // valid. A declared60s wait cannot fit a60s total timeout plus headroom.
    c.limits["max-wait-ns"]="59999000000";c.limits["timeout-ms"]="60000";
    need(bool(ros::validate_capability(ros::version::ros2,c))&&accepted(c));
    need(gateway::limit(c,"max-wait-ns")==UINT64_C(59999000000));
    c.limits["max-wait-ns"]="60000000000";
    need(gateway::limit(c,"max-wait-ns")==UINT64_C(60000000000));
    need(!ros::validate_capability(ros::version::ros2,c)&&!accepted(c));
    // The protocol ceiling, malformed numbers and every other field keep their
    // existing numeric restrictions. No arbitrary generic quota is widened.
    for(auto value:{"60000000001","18446744073709551616","-1","1junk"})
    {c.limits["max-wait-ns"]=value;need(limited(c,"max-wait-ns")&&!accepted(c));}
    c.limits["max-wait-ns"]="2000000000";c.limits["max-response-bytes"]="1073741825";
    need(limited(c,"max-response-bytes"));c.limits["max-response-bytes"]="1073741824";
    need(gateway::limit(c,"max-response-bytes")==UINT64_C(1073741824));
}
