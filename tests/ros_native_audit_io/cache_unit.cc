/** @file C++17 immutable native audit cache contract.
 * This unit calls the real audit function with entries in a validated explicit
 * configuration. It creates no adapter, ROS node, Guest or IPC connection.
 * The ARM unit runner supplies its pre-main evidence barrier. Audit output is
 * directed to an owned /dev/null descriptor; actual pipe/socket behavior stays
 * in the separate historical writer tests. Token expectations use generous
 * bursts and never depend on an exact wall-clock interval or token fraction.
 */
#include "../../gateways/common/server.h"
#include <atomic>
#include <cstdlib>
#include <new>
namespace g=uwvm2::ros_gateway;
namespace io=g::native_io;
namespace {
std::atomic<bool> measured{false};
std::atomic<std::uint64_t> allocations{0};
unsigned checks{};
void observe_new() noexcept { if(measured.load(std::memory_order_relaxed)) { allocations.fetch_add(1,std::memory_order_relaxed); } }
void require(bool value,char const* message) { ++checks;if(!value) { throw std::runtime_error(message); } }
/** Redirect only this fixture's stderr; retain and restore its borrowed FD. */
struct quiet_audit
{
    io::owned_fd previous{::fcntl(STDERR_FILENO,F_DUPFD_CLOEXEC,3)};
    quiet_audit()
    {
        io::owned_fd sink{::open("/dev/null",O_WRONLY|O_CLOEXEC)};
        if(previous.get()<0||sink.get()<0||::dup2(sink.get(),STDERR_FILENO)<0) { throw std::runtime_error("fixture audit sink unavailable"); }
    }
    ~quiet_audit() { if(previous.get()>=0) { (void)::dup2(previous.get(),STDERR_FILENO); } }
};
g::ros::capability_spec make_grant(std::string alias,std::uint64_t records,std::uint64_t bytes,std::string mode="allow-and-deny")
{
    auto resource=std::string{"/audit_cache/"}+alias;
    g::ros::capability_spec c{std::move(alias),"time.read-wall",std::move(resource),"builtin/time-v1","builtin:time-nanoseconds-v1",{}};
    c.limits={{"max-rate-hz","1000000"},{"burst","1048576"},{"max-inflight","1"},{"max-response-bytes","1024"},
        {"max-bytes-per-second","1048576"},{"audit",std::move(mode)},{"audit-max-rate-hz","0.25"},
        {"audit-burst","1048576"},{"audit-max-records",std::to_string(records)},{"audit-max-bytes",std::to_string(bytes)}};
    g::validate(g::ros::version::ros1,c);return c;
}
g::configuration configuration()
{
    g::configuration c;c.policy.ros_version=g::ros::version::ros1;c.policy.enabled=true;
    c.timeout_ms=1000;c.max_sessions=1;c.socket="/audit_cache/test.sock";
    c.worker_uid=1000;c.worker_pid=12345;c.audit_records_per_second=1000000;c.audit_max_records=1048576;c.audit_max_bytes=67108864;
    auto add=[&](g::ros::capability_spec value){auto alias=value.alias;require(c.policy.capabilities.emplace(alias,std::move(value)).second,"fixture aliases must be unique");};
    add(make_grant("clock",3,1048576));add(make_grant("second",5,1048576));
    add(make_grant("bytes",1000,1024));add(make_grant("deny",3,1048576,"deny-only"));return c;
}
/** All request strings and map entries are prepared before measuring new.
 * This records standard scalar/array/aligned C++ new calls, not kernel/C heap
 * allocation or a latency result. Initialization is deliberately unmeasured. */
void hot_audit(g::configuration const& cfg,g::ipc::request const& request,g::ros::capability_spec const* cap,
               g::audit_limiter& global,g::audit_limiter& local)
{
    auto before_records=global.total_records,before_bytes=global.total_bytes,before_local=local.total_records,before_local_bytes=local.total_bytes;
    allocations.store(0,std::memory_order_relaxed);measured.store(true,std::memory_order_relaxed);
    try { g::audit(77,request,cap,13,global,local,cfg); }
    catch(...) { measured.store(false,std::memory_order_relaxed);throw; }
    measured.store(false,std::memory_order_relaxed);
    require(!allocations.load(std::memory_order_relaxed),"initialized audit hot path allocated through C++ new");
    auto charged=global.total_records-before_records;
    require(charged<=1&&local.total_records>=before_local&&local.total_records-before_local==charged,"global/grant record accounting diverged");
    require(global.total_bytes>=before_bytes&&local.total_bytes>=before_local_bytes&&
            global.total_bytes-before_bytes==local.total_bytes-before_local_bytes,"global/grant byte accounting diverged");
    if(charged) { require(local.total_bytes>before_local_bytes,"admitted complete record must consume positive bytes"); }
}
template<typename Function> void refused(Function fn,char const* message)
{
    bool rejected{};try { fn(); }catch(std::runtime_error const&) { rejected=true; }
    require(rejected,message);
}
}
void* operator new(std::size_t count) { observe_new();if(auto p=std::malloc(count?count:1)) { return p; }throw std::bad_alloc{}; }
void* operator new[](std::size_t count) { return ::operator new(count); }
void operator delete(void* p) noexcept { std::free(p); }
void operator delete[](void* p) noexcept { std::free(p); }
void operator delete(void* p,std::size_t) noexcept { std::free(p); }
void operator delete[](void* p,std::size_t) noexcept { std::free(p); }
void* operator new(std::size_t count,std::align_val_t alignment)
{
    observe_new();void* p{};if(!::posix_memalign(&p,static_cast<std::size_t>(alignment),count?count:1)) { return p; }throw std::bad_alloc{};
}
void* operator new[](std::size_t count,std::align_val_t alignment) { return ::operator new(count,alignment); }
void operator delete(void* p,std::align_val_t) noexcept { std::free(p); }
void operator delete[](void* p,std::align_val_t) noexcept { std::free(p); }
void operator delete(void* p,std::size_t,std::align_val_t) noexcept { std::free(p); }
void operator delete[](void* p,std::size_t,std::align_val_t) noexcept { std::free(p); }
void* operator new(std::size_t n,std::nothrow_t const&) noexcept { try { return ::operator new(n); }catch(...) { return nullptr; } }
void* operator new[](std::size_t n,std::nothrow_t const&) noexcept { try { return ::operator new[](n); }catch(...) { return nullptr; } }
void* operator new(std::size_t n,std::align_val_t a,std::nothrow_t const&) noexcept { try { return ::operator new(n,a); }catch(...) { return nullptr; } }
void* operator new[](std::size_t n,std::align_val_t a,std::nothrow_t const&) noexcept { try { return ::operator new[](n,a); }catch(...) { return nullptr; } }
void operator delete(void* p,std::nothrow_t const&) noexcept { std::free(p); }
void operator delete[](void* p,std::nothrow_t const&) noexcept { std::free(p); }
void operator delete(void* p,std::align_val_t,std::nothrow_t const&) noexcept { std::free(p); }
void operator delete[](void* p,std::align_val_t,std::nothrow_t const&) noexcept { std::free(p); }
int main()
{
    try
    {
        auto storage=configuration();g::configuration const& cfg=storage;quiet_audit quiet;
        auto clock=&cfg.policy.capabilities.at("clock"),second=&cfg.policy.capabilities.at("second"),bytes=&cfg.policy.capabilities.at("bytes"),deny=&cfg.policy.capabilities.at("deny");
        g::ipc::request read{"time.read-wall","clock",{}},close{"capability.close","clock",{}},reopen=read,revoke{"capability.revoke","clock",{}};
        g::audit_limiter global,clock_logs,second_logs,byte_logs,deny_logs;
        g::audit(77,read,clock,13,global,clock_logs,cfg);
        require(clock_logs.initialized&&clock_logs.configured_owner==&cfg&&clock_logs.configured_grant==clock,"cache must bind original configured entry");
        require(clock_logs.audit_burst==1048576&&clock_logs.audit_rate==0.25&&clock_logs.audit_record_limit==3&&clock_logs.audit_byte_limit==1048576,"all four explicit numbers must be cached exactly");
        require(clock_logs.total_records==1&&clock_logs.total_bytes>0&&global.total_records==1,"initial audit must consume both quotas");
        hot_audit(cfg,close,clock,global,clock_logs);hot_audit(cfg,reopen,clock,global,clock_logs);
        require(clock_logs.total_records==3,"dynamic close/reopen metadata must preserve lifetime record quota");
        auto prior_bytes=clock_logs.total_bytes,prior_global=global.total_records,prior_suppressed=global.suppressed;
        hot_audit(cfg,revoke,clock,global,clock_logs);
        require(clock_logs.total_records==3&&clock_logs.total_bytes==prior_bytes&&global.total_records==prior_global&&global.suppressed==prior_suppressed+1,"exhausted record quota must refuse after reopen");
        g::ipc::request other{"time.read-wall","second",{}};g::audit(77,other,second,13,global,second_logs,cfg);
        require(second_logs.initialized&&second_logs.configured_grant==second&&second_logs.total_records==1&&clock_logs.total_records==3,"second alias must own independent cache and quotas");
        hot_audit(cfg,other,second,global,second_logs);
        require(second_logs.total_records==2&&clock_logs.total_records==3,"second alias cannot refill another alias's quota");
        g::ipc::request byte_read{"time.read-wall","bytes",{}},byte_close{"capability.close","bytes",{}};
        g::audit(77,byte_read,bytes,13,global,byte_logs,cfg);
        require(byte_logs.initialized&&byte_logs.total_records==1&&byte_logs.total_bytes>0,"byte quota scenario needs an actual admitted record");
        for(unsigned i{};i<32;++i) { hot_audit(cfg,i%2?byte_read:byte_close,bytes,global,byte_logs); }
        require(byte_logs.audit_byte_limit==1024&&byte_logs.total_bytes<=1024&&byte_logs.total_records>1&&byte_logs.total_records<33,"byte lifetime quota must persist across dynamic metadata");
        require(byte_logs.audit_record_limit==1000&&byte_logs.tokens>=1,"byte refusals must be independent of record/token exhaustion");
        g::ipc::request skipped{"time.read-wall","deny",{}};
        auto skipped_global=global.total_records;g::audit(77,skipped,deny,0,global,deny_logs,cfg);
        require(!deny_logs.initialized&&!deny_logs.configured_owner&&!deny_logs.configured_grant&&global.total_records==skipped_global,"deny-only success must skip before cache initialization");
        auto capped=cfg;capped.audit_max_records=1;g::audit_limiter capped_global,capped_grant;capped_global.total_records=1;
        auto capped_clock=&capped.policy.capabilities.at("clock");g::audit(77,read,capped_clock,13,capped_global,capped_grant,capped);
        require(!capped_grant.initialized&&!capped_grant.configured_owner&&capped_global.suppressed==1,"global refusal must precede cache initialization");
        auto foreign=*clock;g::audit_limiter foreign_global,foreign_local;
        refused([&]{g::audit(77,read,&foreign,13,foreign_global,foreign_local,cfg);},"foreign copied cap must be refused");
        require(!foreign_local.initialized&&!foreign_local.configured_owner&&!foreign_global.total_records,"foreign cap cannot publish cache state or consume record quota");
        auto replacement=cfg;auto replacement_clock=&replacement.policy.capabilities.at("clock");auto retained_count=clock_logs.total_records,retained_bytes=clock_logs.total_bytes;
        refused([&]{g::audit(77,read,replacement_clock,13,foreign_global,clock_logs,replacement);},"initialized limiter must reject changed configuration/cap binding");
        refused([&]{g::audit(77,other,second,13,foreign_global,clock_logs,cfg);},"initialized limiter must reject second configured cap");
        require(clock_logs.configured_owner==&cfg&&clock_logs.configured_grant==clock&&clock_logs.total_records==retained_count&&clock_logs.total_bytes==retained_bytes,"refused rebinding must preserve original cache and lifetime counters");
        io::fixed_text<192> result;
        if(!result.append("PASS C++17 native audit numeric cache checks=")||!result.number(checks)||!result.append(" cached_aliases=3 hot_cpp_allocations=0\n")||!io::write_all(STDOUT_FILENO,result.view())) { return 2; }
        return 0;
    }
    catch(std::exception const& error)
    {
        measured.store(false,std::memory_order_relaxed);io::fixed_text<512> line;
        if(line.append("FAIL C++17 native audit numeric cache: ")&&line.append(error.what())&&line.append("\n")) { (void)io::write_all(STDOUT_FILENO,line.view()); }
        return 1;
    }
}
