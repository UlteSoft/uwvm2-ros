#include "action_cleanup.h"
#include "native_io.h"
#include <array>
#include <atomic>
#include <csignal>
#include <functional>
#include <stdexcept>
#include <thread>
#include <vector>
namespace a=uwvm2::ros_gateway::ros2_action;
namespace n=uwvm2::ros_gateway::native_io;
namespace uwvm2::ros_gateway::ros2_action
{
    struct goal_state
    {
        std::atomic<unsigned>* destroyed;
        std::function<void()> on_destroy;
        explicit goal_state(std::atomic<unsigned>& count):destroyed{&count} {}
        ~goal_state() { if(on_destroy) { on_destroy(); }++*destroyed; }
    };
}
void check(bool value,char const* message) { if(!value) { throw std::runtime_error(message); } }
int main()
{
    ::raise(SIGSTOP);
    unsigned groups{};
    try
    {
        {
            std::atomic<unsigned> deaths{};
            a::cleanup_pool pool{0};auto g{std::make_shared<a::goal_state>(deaths)};std::weak_ptr<a::goal_state> weak{g};
            check(!pool.track(1,g)&&pool.count()==0,"C0 rejects all");
            g.reset();check(weak.expired()&&deaths==1,"C0 does not retain rejected reference");++groups;
        }
        {
            std::atomic<unsigned> deaths{};
            a::cleanup_pool pool{1};auto g{std::make_shared<a::goal_state>(deaths)},other{std::make_shared<a::goal_state>(deaths)};
            std::weak_ptr<a::goal_state> weak{g},next{other};check(pool.track(1,g),"C1 first track");
            g.reset();check(!weak.expired()&&pool.count()==1&&deaths==0,"strong retention");check(!pool.track(2,other)&&pool.count()==1,"C1 refuses second");
            pool.release(1);check(weak.expired()&&deaths==1&&pool.count()==0,"release frees retained reference");
            check(pool.track(2,other),"released capacity reusable");other.reset();pool.release(2);check(next.expired()&&deaths==2,"reuse released");++groups;
        }
        {
            std::atomic<unsigned> deaths{};
            a::cleanup_pool pool{2};auto g{std::make_shared<a::goal_state>(deaths)},other{std::make_shared<a::goal_state>(deaths)};
            std::weak_ptr<a::goal_state> weak{g},rejected{other};check(pool.track(7,g),"duplicate first");
            check(!pool.track(7,g)&&!pool.track(7,other)&&pool.count()==1,"duplicate ID never replaces");g.reset();other.reset();
            check(!weak.expired()&&rejected.expired()&&deaths==1,"duplicate kept original only");pool.release(7);check(weak.expired()&&deaths==2,"duplicate release");++groups;
        }
        {
            std::atomic<unsigned> deaths{};
            a::cleanup_pool pool{8};auto g{std::make_shared<a::goal_state>(deaths)};
            check(!pool.track(0,g)&&!pool.track(9,{}),"zero/null rejected");pool.release(0);pool.release(99);check(pool.count()==0,"unknown release is no-op");++groups;
        }
        {
            std::atomic<unsigned> deaths{},callbacks{};auto pool{std::make_shared<a::cleanup_pool>(1)};std::weak_ptr<a::cleanup_pool> weak{pool};
            auto g{std::make_shared<a::goal_state>(deaths)};g->on_destroy=[weak,&callbacks] {
                auto p{weak.lock()};if(p&&p->count()==0) { p->release(999);++callbacks; }
            };
            check(pool->track(11,g),"reentrant destructor first");g.reset();pool->release(11);
            check(deaths==1&&callbacks==1&&pool->count()==0,"final reference released outside mutex");++groups;
        }
        {
            std::atomic<unsigned> deaths{},won{};std::atomic<bool> go{},bad{};
            a::cleanup_pool pool{4};std::array<std::shared_ptr<a::goal_state>,16> goals;std::array<std::weak_ptr<a::goal_state>,16> weak;
            for(unsigned i{};i<16;++i) { goals[i]=std::make_shared<a::goal_state>(deaths);weak[i]=goals[i]; }
            std::vector<std::thread> threads;
            for(unsigned i{};i<16;++i) { threads.emplace_back([&,i] { while(!go.load()) { std::this_thread::yield(); }if(pool.track(100+i,goals[i])) { ++won; }if(pool.count()>4) { bad=true; } }); }
            go=true;for(auto& t:threads) { t.join(); }check(won==4&&pool.count()==4&&!bad,"concurrent capacity exact4");goals.fill({});
            check(deaths==12,"only four accepted references retained");threads.clear();
            for(unsigned i{};i<16;++i) { threads.emplace_back([&,i] { pool.release(100+i);pool.release(100+i); }); }
            for(auto& t:threads) { t.join(); }check(pool.count()==0&&deaths==16,"concurrent/duplicate release exactly once");
            for(auto const& value:weak) { check(value.expired(),"all concurrent goal references expired"); }++groups;
        }
        {
            std::atomic<unsigned> deaths{},tracked{};std::atomic<bool> bad{};
            a::cleanup_pool pool{4};std::vector<std::thread> threads;
            for(unsigned t{};t<8;++t) { threads.emplace_back([&,t] {
                for(unsigned i{};i<64;++i)
                {
                    auto g{std::make_shared<a::goal_state>(deaths)};auto id{1000+t*64+i};
                    if(pool.track(id,g)) { ++tracked;if(pool.count()>4) { bad=true; }pool.release(id); }
                }
            }); }
            for(auto& t:threads) { t.join(); }check(!bad&&tracked>0&&pool.count()==0&&deaths==512,"bounded concurrent refill and drain");++groups;
        }
        {
            std::atomic<unsigned> deaths{};auto pool{std::make_shared<a::cleanup_pool>(1)};auto g{std::make_shared<a::goal_state>(deaths)};
            std::weak_ptr<a::cleanup_pool> weak_pool{pool};std::weak_ptr<a::goal_state> weak_goal{g};check(pool->track(20,g),"destructor track");g.reset();pool.reset();
            check(weak_pool.expired()&&weak_goal.expired()&&deaths==1,"pool destruction releases state without self cycle");++groups;
        }
        check(groups==8,"all eight groups");
        n::fixed_text<256> output;check(output.append("PASS action-cleanup unit groups=8 retained_capacity_release_duplicate_concurrency_destructor\n"),"bounded output");
        return n::write_all(STDOUT_FILENO,output.view())?0:2;
    }
    catch(std::exception const& error)
    {
        n::fixed_text<256> output;output.append("FAIL action-cleanup unit ");output.append(error.what());output.append("\n");n::write_all(STDERR_FILENO,output.view());return 1;
    }
}
