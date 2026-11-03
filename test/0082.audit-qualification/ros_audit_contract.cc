// Actual bounded retention and detached ROS trace primitives. Native ROS
// policy/import integration remains a separate Runtime/Gateway qualification.
#include <uwvm2/imported/ros/common/audit.h>
#include <uwvm2/uwvm/debugger/ros_state.h>
#include <uwvm2/utils/container/impl.h>
#include <cstdio>
#include <cstdlib>
#include <csignal>
#include <new>
#include <string>
namespace ros=uwvm2::imported::ros;
namespace d=ros::debug;
namespace render=uwvm2::uwvm::debugger::ros_state;
static bool fail_allocations{};static unsigned checks{};
void* operator new(std::size_t n) {if(!fail_allocations) {if(auto p=std::malloc(n?n:1)) {return p;}}throw std::bad_alloc{};}
void* operator new[](std::size_t n) {return ::operator new(n);}
void operator delete(void* p) noexcept {std::free(p);}void operator delete[](void* p) noexcept {std::free(p);}
void operator delete(void* p,std::size_t) noexcept {std::free(p);}void operator delete[](void* p,std::size_t) noexcept {std::free(p);}
static void check(bool good,char const* why)
{++checks;if(!good) {std::fprintf(stderr,"FAIL ROS audit: %s\n",why);std::exit(1);}}
static ros::capability_spec cap()
{ros::capability_spec c{};c.limits={{"audit-max-rate-hz","0.000001"},{"audit-burst","8"},{"audit-max-records","2"},{"audit-max-bytes","6"}};return c;}
int main()
{
    if(std::getenv("UWVM_AUDIT_HOLD")) {std::raise(SIGSTOP);}
    ros::bounded_audit audit{};check(!audit.record("a")&&audit.last_status()==ros::audit_record_status::unconfigured,"unconfigured retention fails explicitly");
    auto config=cap();check(bool(audit.configure(config)),"explicit valid quotas");
    check(audit.record("abc")&&audit.record("def")&&audit.record("ghi"),"bounded entries retained");
    check(audit.retained_records()==2&&audit.retained_bytes()==6&&audit.statistics().overwritten==1,"count/byte overflow has a distinct counter");
    check(!audit.record("1234567")&&audit.last_status()==ros::audit_record_status::oversized&&audit.statistics().oversized==1,"oversized record has a distinct reason");
    auto invalid=config;invalid.limits["audit-max-records"]="1";invalid.limits["audit-max-bytes"]="invalid";
    check(!audit.configure(invalid)&&audit.retained_records()==2&&audit.retained_bytes()==6,"reconfiguration failure preserves old quotas and records");
    check(audit.record("x")&&audit.retained_records()==2&&audit.retained_bytes()==4,"old quotas remain usable after invalid reconfiguration");
    std::vector<std::string> records{"sentinel"};std::uint64_t dropped=991;bool failed{};
    fail_allocations=true;try {audit.drain(records,dropped);}catch(std::bad_alloc const&) {failed=true;}fail_allocations=false;
    check(failed&&records==std::vector<std::string>{"sentinel"}&&dropped==991&&audit.retained_records()==2&&audit.statistics().drain_failed==1,
        "failed drain preserves retained bytes and caller output");
    audit.drain(records,dropped);check(records==std::vector<std::string>{"ghi","x"}&&dropped==3,"trusted retry drains the complete surviving history with cumulative loss");
    config.limits["audit-burst"]="3";check(bool(audit.configure(config)),"reset quotas");
    check(audit.record("a")&&audit.record("b")&&audit.record("c")&&!audit.record("d")&&
        audit.last_status()==ros::audit_record_status::rate_limited&&audit.statistics().rate_limited==1,"rate suppression is distinct from retention overflow");
    config.limits["audit-max-records"]="4096";config.limits["audit-max-bytes"]="65536";config.limits["audit-burst"]="1000";check(bool(audit.configure(config)),"allocation fixture quotas");
    fail_allocations=true;bool allocation_failure{};
    for(unsigned i{};i<64;++i) {if(!audit.record("a")) {allocation_failure=audit.last_status()==ros::audit_record_status::allocation_failed;break;}}
    fail_allocations=false;check(allocation_failure&&audit.statistics().allocation_failed==1,"retention allocation loss is explicit and does not terminate");
    d::trace_store store{};d::trace_record call{};call.backend=2;call.instance=7;call.generation=3;
    call.module.assign("module");call.alias.assign("publisher");call.function.assign("publish");call.operation.assign("topic.publish");call.resource.assign("/safe_topic");
    for(unsigned i{};i<258;++i) {store.append(call);}
    d::request query{};query.selected=d::selection::trace;query.count=16;d::view view{};store.read(query,view);
    check(view.gap&&view.oldest==3&&view.overwritten==2&&view.trace_size==16,"initial ROS read reports overwritten history even with after=0");
    auto next=view.next;query.after=next;query.function.assign("absent");store.read(query,view);
    check(view.trace_size==0&&view.next==next,"empty filtered page preserves the exclusive cursor");
    d::trace_store outcomes{};call.error=13;outcomes.append(call);call.error=0;outcomes.append(call);call.result_known=false;outcomes.append(call);
    query={};query.selected=d::selection::trace;query.error_filter=true;query.error=0;query.count=16;outcomes.read(query,view);
    check(view.trace_size==1&&view.trace[0].result_known&&view.trace[0].error==0,"success filter excludes unobserved normal returns");
    query.error=13;outcomes.read(query,view);uwvm2::utils::container::string text{};
    auto sink=uwvm2::utils::container::string_ref_uwvm{std::addressof(text)};render::print(sink,view,0);
    check(std::string_view{text.data(),text.size()}.find("reason=permission_denied")!=std::string_view::npos,"stable permission denial reason rendered");
    call.resource.assign("/safe?token=PRIVATE_SECRET");view.trace[0]=call;view.trace[0].result_known=true;text.clear();render::print(sink,view,0);
    check(std::string_view{text.data(),text.size()}.find("PRIVATE_SECRET")==std::string_view::npos&&
        std::string_view{text.data(),text.size()}.find("resource=<redacted>")!=std::string_view::npos,"secret-bearing resource never reaches renderer");
    view={};view.result=d::status::ok;view.selected=d::selection::trace;view.trace_size=16;
    std::string hostile(255,'\x1b');
    for(auto& item:view.trace)
    {item.sequence=item.monotonic_ns=item.instance=item.generation=UINT64_MAX;item.backend=2;item.error=UINT32_MAX;
     item.module.assign(hostile);item.alias.assign(hostile);item.source_alias.assign(hostile);item.function.assign(hostile.substr(0,64));item.operation.assign(hostile.substr(0,64));
     item.client_session_label=item.client_features=UINT64_MAX;item.client_features_observed=true;item.resource.assign('/'+std::string(63,'a'));}
    text.clear();render::print(sink,view,UINT64_MAX);check(text.size()<=65536,"maximum escaped ROS trace page fits authenticated reply cap");
    check(std::string_view{text.data(),text.size()}.find('\x1b')==std::string_view::npos,"untrusted text cannot inject terminal control bytes");
    std::printf("PASS_ROS_AUDIT_CONTRACT checks=%u maximum_reply_bytes=%zu\n",checks,text.size());
}
