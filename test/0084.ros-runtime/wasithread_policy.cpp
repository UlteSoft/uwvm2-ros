/** @file @brief I1T authority, aggregate counts, exact f32 scaling and concurrent admission. */
#include <uwvm2/utils/control/wasithread_diagnostics.h>
#include <uwvm2/utils/control/wasithread_system.h>
#include <atomic>
#include <array>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <pthread.h>
#include <sched.h>
#if defined(I1T_ALLOCATION_FAULTS)
static thread_local int fail_after=-1;
extern "C" void* __real_malloc(std::size_t);
extern "C" void* __real_calloc(std::size_t,std::size_t);
static bool fail() noexcept {return fail_after>=0&&fail_after--==0;}
extern "C" void* __wrap_malloc(std::size_t n){return fail()?nullptr:__real_malloc(n);}
extern "C" void* __wrap_calloc(std::size_t n,std::size_t s){return fail()?nullptr:__real_calloc(n,s);}
#endif
namespace t=uwvm2::utils::control::wasithread;
static void grant(t::policy& p,t::scope kind,std::u8string_view name)
{assert(p.enable(kind,name,true));assert(p.expose(kind,name,t::exposure::allow));}
static void authority()
{
    t::policy p;
    assert(!p.visible(u8"main"));
    assert(!p.reserve(u8"main",true).held);
    assert(p.create(t::scope::single,u8"main"));
    assert(!p.create(t::scope::single,u8"main"));
    assert(!p.bind(t::scope::global,{},u8"main"));
    assert(p.enable(t::scope::single,u8"main",true));
    assert(!p.visible(u8"main"));
    assert(p.expose(t::scope::single,u8"main",t::exposure::deny));
    assert(p.visible(u8"main"));
    auto main=p.reserve(u8"main",false);assert(main.held);
    assert(p.reserve(u8"main",true).reason==t::failure::denied);
    assert(p.reserve(u8"other",true).reason==t::failure::disabled);
    assert(!p.enable(t::scope::single,u8"main",false));
    assert(!p.create(t::scope::group,u8"late"));
    assert(!p.constrain(t::scope::global,{},t::limit::threads,20));
    assert(p.inspect(t::scope::global).occupied.mains==1);
}
static void selection_and_inspection()
{
    t::policy p;grant(p,t::scope::global,{});
    assert(p.visible(u8"default"));assert(!p.visible(u8""));
    assert(!p.create(static_cast<t::scope>(99),u8"invalid"));
    assert(!p.expose(t::scope::global,{},static_cast<t::exposure>(99)));
    assert(p.create(t::scope::single,u8"private"));assert(!p.visible(u8"private"));
    assert(p.create(t::scope::group,u8"pair"));assert(p.bind(t::scope::group,u8"pair",u8"a"));
    assert(p.bind(t::scope::group,u8"pair",u8"b"));
    assert(!p.bind(t::scope::group,u8"pair",u8"a"));assert(!p.create(t::scope::single,u8"b"));
    assert(!p.bind(t::scope::group,u8"pair",u8"private"));
    grant(p,t::scope::group,u8"pair");assert(p.visible(u8"a")&&p.visible(u8"b"));
    auto main=p.reserve(u8"default",false);assert(main.held);
    assert(p.reserve(u8"private",true).reason==t::failure::disabled);
    auto a=p.reserve(u8"a",true),b=p.reserve(u8"b",true);assert(a.held&&b.held);
    assert(p.effective(u8"a").occupied.children==2&&p.effective(u8"b").occupied.children==2);
    t::observation out[2];auto first=p.inspect_targets(0,out);
    assert(first.total==3&&first.copied==2&&first.sealed);
    assert(out[0].kind==t::scope::global&&out[0].occupied.children==2);
    assert(out[1].kind==t::scope::group&&out[1].occupied.children==2);
    auto next=p.inspect_targets(2,out);assert(next.copied==1&&out[0].kind==t::scope::single);
    assert(p.inspect_targets(SIZE_MAX,out).copied==0);
    t::membership members[2];auto page=p.inspect_memberships(0,members);
    assert(page.total==3&&page.copied==2&&members[0].target==u8"pair"&&members[1].target==u8"pair");
    assert(p.inspect_memberships(2,members).copied==1&&members[0].kind==t::scope::single);
    assert(!p.reserve(u8"",false).held);
}
static void limits()
{
    for(auto bound:{t::limit::extra_threads,t::limit::threads,t::limit::spawns})
    {
        t::policy p;grant(p,t::scope::global,{});
        assert(p.constrain(t::scope::global,{},bound,bound==t::limit::threads?1:0));
        auto main=p.reserve(u8"main",false);assert(main.held);
        auto denied=p.reserve(u8"main",true);assert(!denied.held);
        assert(p.inspect(t::scope::global).occupied.spawn_admissions==0);
    }
    t::policy p;assert(p.create(t::scope::group,u8"work"));assert(p.bind(t::scope::group,u8"work",u8"a"));
    assert(p.bind(t::scope::group,u8"work",u8"b"));grant(p,t::scope::group,u8"work");
    assert(p.constrain(t::scope::global,{},t::limit::threads,3));
    assert(p.constrain(t::scope::group,u8"work",t::limit::extra_threads,1));
    assert(p.constrain(t::scope::group,u8"work",t::limit::spawns,2));
    auto main=p.reserve(u8"a",false),other=p.reserve(u8"unbound-main",false);assert(main.held&&other.held);
    auto child=p.reserve(u8"b",true);assert(child.held);
    assert(!p.reserve(u8"a",true).held);
    t::policy::reservation moved{std::move(child.held)};assert(!child.held);
    moved.reset();moved.reset(); // rollback/repeated destruction cannot underflow
    auto retry=p.reserve(u8"a",true);assert(retry.held);retry.held.reset();
    assert(p.reserve(u8"b",true).reason==t::failure::spawns);
    assert(p.inspect(t::scope::global).occupied.mains==2);
    assert(p.inspect(t::scope::group,u8"work").occupied.mains==1);
    assert(p.inspect(t::scope::global).occupied.children==0);
    t::policy zero;assert(zero.constrain(t::scope::global,{},t::limit::threads,0));
    assert(!zero.reserve(u8"main",false).held);
}
struct concurrent_case
{
    t::policy policy;std::atomic<unsigned> arrived{},admitted{};std::atomic<bool> release{};
    struct argument{concurrent_case* self;std::u8string_view module;};
    static void* run(void* opaque)
    {
        auto& a=*static_cast<argument*>(opaque);auto& self=*a.self;
        auto child=self.policy.reserve(a.module,true);
        if(child.held)self.admitted.fetch_add(1);
        self.arrived.fetch_add(1);
        while(!self.release.load())sched_yield();
        return nullptr; // RAII disposal represents completion after native join in the bridge
    }
};
static void concurrent()
{
    concurrent_case c;auto& p=c.policy;
    assert(p.create(t::scope::group,u8"a"));assert(p.create(t::scope::group,u8"b"));
    assert(p.bind(t::scope::group,u8"a",u8"ma"));assert(p.bind(t::scope::group,u8"b",u8"mb"));
    grant(p,t::scope::group,u8"a");grant(p,t::scope::group,u8"b");
    assert(p.constrain(t::scope::global,{},t::limit::threads,7));
    assert(p.constrain(t::scope::global,{},t::limit::extra_threads,5));
    assert(p.constrain(t::scope::group,u8"a",t::limit::extra_threads,3));
    assert(p.constrain(t::scope::group,u8"b",t::limit::extra_threads,3));
    auto a=p.reserve(u8"ma",false),b=p.reserve(u8"mb",false);assert(a.held&&b.held);
    pthread_t threads[32]{};concurrent_case::argument args[32]{};
    for(unsigned i=0;i<32;++i){args[i]={&c,i%2?u8"ma":u8"mb"};assert(pthread_create(&threads[i],nullptr,c.run,&args[i])==0);}
    while(c.arrived.load()!=32)sched_yield();
    assert(c.admitted.load()==5);auto total=p.inspect(t::scope::global);
    assert(total.occupied.mains==2&&total.occupied.children==5);
    assert(p.inspect(t::scope::group,u8"a").occupied.children<=3);
    assert(p.inspect(t::scope::group,u8"b").occupied.children<=3);
    c.release.store(true);for(auto thread:threads)assert(pthread_join(thread,nullptr)==0);
    assert(p.inspect(t::scope::global).occupied.children==0);
    assert(p.reserve(u8"ma",true).held);
}
static void ratios()
{
    for(auto text:{u8"0",u8"-0",u8"1",u8"0.5",u8"1e-3",u8"0.000000000000000000000000000000000000000000001"})
    {float value{};assert(t::parse_ratio(text,value));}
    for(auto text:{u8"",u8"NaN",u8"nan",u8"inf",u8"-inf",u8"-0.1",u8"1.01",u8"0.5junk",u8"1e999"})
    {float value=0.7f;assert(!t::parse_ratio(text,value));assert(value==0.7f);}
    std::uint64_t n{};assert(t::scale_limit(11,0.5f,n)&&n==5);
    assert(t::scale_limit(UINT64_MAX-1,1.0f,n)&&n==UINT64_MAX-1);
    assert(t::scale_limit(UINT64_MAX-1,0.5f,n)&&n==(UINT64_MAX-1)/2);
    assert(t::scale_limit(UINT64_MAX-1,0.0f,n)&&n==0);
    assert(!t::scale_limit(UINT64_MAX,1.0f,n));
    std::uint64_t random=0x1020304050607080;
    for(unsigned i=0;i<100000;++i)
    {
        random=random*6364136223846793005ull+1;
        auto bits=static_cast<std::uint32_t>(random%0x3f800001u);auto value=std::bit_cast<float>(bits);
        auto exponent=bits>>23,significand=bits&0x7fffff;unsigned shift=149;
        if(exponent){significand|=0x800000;shift=150-exponent;}
        // Independent binary long multiplication reference: all target ABIs
        // support uint64_t, while the three 32-bit ABIs have no __uint128_t.
        // Accumulate single-bit products into a 96-bit bit vector so the oracle
        // does not duplicate scale_limit's native-word carry implementation.
        std::array<unsigned char,96> product{};
        for(unsigned left=0;left<64;++left)if((random>>left)&1u)
            for(unsigned right=0;right<24;++right)if((significand>>right)&1u)
            {
                unsigned carry=1,position=left+right;
                while(carry)
                {
                    assert(position<product.size());
                    auto sum=static_cast<unsigned>(product[position])+carry;
                    product[position++]=static_cast<unsigned char>(sum&1u);carry=sum>>1;
                }
            }
        std::uint64_t expected{};
        for(unsigned bit=shift;bit<product.size();++bit)
        {
            if(bit-shift>=64){assert(!product[bit]);continue;}
            expected|=static_cast<std::uint64_t>(product[bit])<<(bit-shift);
        }
        assert(t::scale_limit(random,value,n)&&n==expected);
    }
    auto system=t::inspect_system_ceiling();assert(system.available&&system.effective<=system.kernel);
    assert(system.effective<=system.process&&system.effective<=system.cgroup);
    std::printf("system-ceiling=%llu kernel=%llu process=%llu cgroup=%llu\n",
        (unsigned long long)system.effective,(unsigned long long)system.kernel,(unsigned long long)system.process,(unsigned long long)system.cgroup);
}
static void allocations()
{
#if defined(I1T_ALLOCATION_FAULTS)
    for(int after=0;after!=4;++after)
    {
        t::policy p;fail_after=after;bool created=p.create(t::scope::single,u8"main");fail_after=-1;
        assert(!created);assert(p.effective(u8"main").kind==t::scope::global&&!p.visible(u8"main"));assert(p.create(t::scope::single,u8"main"));
    }
#endif
}
int main()
{
    authority();selection_and_inspection();limits();concurrent();ratios();allocations();
    namespace h=uwvm2::utils::control::host_interfaces;
    auto id=h::find(h::family::wasithread,std::string_view{"thread-spawn"});assert(id!=h::invalid);
    assert(h::find(h::family::wasip1,std::string_view{"thread-spawn"})==h::invalid);
    assert(uwvm2::utils::control::host_diagnostics::configure(std::string_view{"all_host_limited_wasithread"},
        uwvm2::utils::control::host_diagnostics::severity::error));
    std::puts("I1T policy: 6 safety groups passed");
}
