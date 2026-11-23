#include <uwvm2/runtime/wasm_threads/execution_journal.h>
#include <uwvm2/runtime/wasm_threads/wait.h>
#include <atomic>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <thread>
namespace t=uwvm2::runtime::wasm_threads;
#define CHECK(x) do{if(!(x)){std::fprintf(stderr,"line %d %s\n",__LINE__,#x);std::abort();}}while(false)
static t::journal_event event(t::journal_kind kind,std::uint64_t worker,std::uint64_t a=0)
{t::journal_event e;e.kind=kind;e.worker=worker;e.a=a;return e;}
static auto word(std::uint32_t value)
{std::array<std::byte,4> b{};for(unsigned n{};n<4;++n)b[n]=static_cast<std::byte>(value>>(8*n));return b;}
int main()
{
    t::threaded_execution_history history;history.terminal=true;
    auto origin=event(t::journal_kind::origin,1);origin.input.resize(64);
    auto born=event(t::journal_kind::spawn,1);auto tid=word(1);born.output.assign(tid.begin(),tid.end());
    auto denied=event(t::journal_kind::spawn,1);auto minus=word(UINT32_MAX);denied.output.assign(minus.begin(),minus.end());
    history.events={origin,event(t::journal_kind::enter,1),born,event(t::journal_kind::enter,2,1),
        event(t::journal_kind::instruction,1),event(t::journal_kind::instruction,2),denied,
        event(t::journal_kind::leave,2,1),event(t::journal_kind::leave,1)};
    CHECK(t::valid_execution_history(history));
    for(unsigned repeat{};repeat<100;++repeat)
    {
        t::threaded_execution_journal j{history};CHECK(j.origin(origin.input));
        t::thread_journal_context main{},child{};main.journal=&j;main.worker=1;child.journal=&j;child.worker=2;
        CHECK(j.select(main,history.events[1]));CHECK(j.select(main,history.events[2],false));CHECK(j.verify(main,tid));j.release(main);
        std::atomic<bool> owns{},finished{},timed_out{};
        std::thread watchdog([&]{auto until=std::chrono::steady_clock::now()+std::chrono::seconds{2};
            while(!finished.load()&&std::chrono::steady_clock::now()<until)std::this_thread::sleep_for(std::chrono::milliseconds{1});
            if(!finished.load()){timed_out=true;j.cancel();}});
        std::thread worker([&]{CHECK(j.select(child,history.events[3]));j.release(child);
            while(!owns.load())std::this_thread::yield();
            CHECK(j.select(child,history.events[5]));j.release(child);
            CHECK(j.select(child,history.events[7]));j.release(child);});
        CHECK(j.select(main,history.events[4]));owns=true;
        // Reproduces the real nested spawn deadlock: the opcode owns a turn,
        // but the next recorded event belongs to another genuine native worker.
        CHECK(j.select(main,history.events[6],false));CHECK(j.verify(main,minus));j.release(main);
        CHECK(j.select(main,history.events[8]));j.release(main);worker.join();finished=true;watchdog.join();
        CHECK(!timed_out&&j.observe().cursor==history.events.size());
    }
    // A failure callback can legitimately re-enter a wait owner which observes
    // the journal. It must run after the journal mutex has been released.
    struct failure {t::threaded_execution_journal* journal;bool called;};
    t::threaded_execution_journal bad{history};auto owner=std::make_shared<failure>();owner->journal=&bad;
    CHECK(bad.bind_failure_owner(owner,[](void* p) noexcept{auto& f=*static_cast<failure*>(p);f.called=f.journal->observe().status==t::journal_status::diverged;}));
    CHECK(bad.origin(origin.input));t::thread_journal_context m{};m.worker=1;m.journal=&bad;
    auto invalid=history.events[1];invalid.c=77;CHECK(!bad.select(m,invalid));CHECK(owner->called);
    // Hold real native cancellation in its notification callback. This is the
    // dangerous interval after the journal changed state but before the Group
    // stop token can be published. An import must still choose virtual replay.
    for(bool cancel:{false,true})
    {
        t::threaded_execution_journal intent{history};CHECK(intent.replaying());
        struct pending_failure { std::atomic<bool> entered{},release{}; };
        auto pending=std::make_shared<pending_failure>();
        CHECK(intent.bind_failure_owner(pending,[](void* p) noexcept
        {
            auto& value=*static_cast<pending_failure*>(p);value.entered=true;
            while(!value.release.load())std::this_thread::yield();
        }));
        std::thread failing([&]{if(cancel)intent.cancel();else intent.refuse();});
        while(!pending->entered.load())std::this_thread::yield();
        unsigned live_provider_calls{};
        if(!intent.replaying())++live_provider_calls;
        t::thread_journal_context interrupted{};interrupted.journal=&intent;
        CHECK(!interrupted.cancellation.stop_requested());
        CHECK(t::interrupted_journal_wait(interrupted).status==t::wait_status::cancelled);
        pending->release=true;failing.join();
        CHECK(live_provider_calls==0&&intent.replaying());
    }
    // Native host retention also covers waiting to admit a call. Before
    // host_begin no external result is unknown; after it, the original owner
    // is mandatory until the matching host_end has actually been captured.
    {
        t::threaded_execution_journal j;CHECK(j.origin(origin.input));
        t::thread_journal_context worker{};worker.journal=&j;worker.worker=1;
        CHECK(j.select(worker,event(t::journal_kind::enter,1)));j.release(worker);
        t::threaded_execution_history before,unknown,after;
        CHECK(j.capture(before,false,true)&&!before.retained_native);
        CHECK(!j.has_unresolved_native_imports());
        CHECK(j.select(worker,event(t::journal_kind::host_begin,1)));j.release(worker);
        CHECK(!j.capture(unknown,false,false));
        CHECK(j.capture(unknown,false,true)&&unknown.retained_native);
        CHECK(j.has_unresolved_native_imports());
        CHECK(j.select(worker,event(t::journal_kind::host_end,1)));j.release(worker);
        CHECK(j.capture(after,false,true)&&!after.retained_native);
        CHECK(!j.has_unresolved_native_imports());
        CHECK(t::valid_execution_history(before)&&t::valid_execution_history(unknown)&&t::valid_execution_history(after));
    }
    t::threaded_execution_journal recording;recording.cancel();CHECK(!recording.replaying());
    // A recorded allocation failure remains -1 even when a replay allocator
    // would now succeed; positive growth still requires real allocation.
    t::threaded_execution_history growing;growing.terminal=true;
    auto grow_word=[](std::uint64_t value){std::array<std::byte,8> out{};for(unsigned i{};i<8;++i)out[i]=static_cast<std::byte>(value>>(i*8u));return out;};
    auto grow_fail=grow_word(UINT64_MAX),grow_success=grow_word(1);
    auto fail_grow=event(t::journal_kind::grow,1,2);fail_grow.b=1;fail_grow.c=65536*4;fail_grow.output.assign(grow_fail.begin(),grow_fail.end());
    auto ok_grow=fail_grow;ok_grow.output.assign(grow_success.begin(),grow_success.end());
    growing.events={origin,event(t::journal_kind::enter,1),fail_grow,ok_grow,event(t::journal_kind::leave,1)};
    CHECK(t::valid_execution_history(growing));t::threaded_execution_journal growth{growing};CHECK(growth.origin(origin.input));
    t::thread_journal_context g{};g.worker=1;g.journal=&growth;t::thread_journal_scope selected{g};CHECK(growth.select(g,growing.events[1]));
    unsigned allocations{};auto allocate=[&]() noexcept{++allocations;return 1u;};
    CHECK(t::journal_memory_grow(2,1,65536*4,allocate)==UINT64_MAX&&allocations==0);
    CHECK(t::journal_memory_grow(2,1,65536*4,allocate)==1&&allocations==1);
    CHECK(growth.select(g,growing.events[4]));growth.release(g);
    auto trapped=growing;trapped.events.back().kind=t::journal_kind::trap;trapped.events.back().a=121;CHECK(t::valid_execution_history(trapped));
    // Grow32 callbacks may return an i32/u32 host carrier; serialization must
    // widen before shifting. Memory64 deltas/results preserve their upper bits.
    {
        t::threaded_execution_history h;h.terminal=true;
        auto wide_grow=event(t::journal_kind::grow,1,UINT64_C(0x100000003));
        wide_grow.b=UINT64_C(0x100000001);wide_grow.c=UINT64_C(0x800000000000);
        auto value=grow_word(wide_grow.b);wide_grow.output.assign(value.begin(),value.end());
        h.events={origin,event(t::journal_kind::enter,1),wide_grow,event(t::journal_kind::leave,1)};
        CHECK(t::valid_execution_history(h));t::threaded_execution_journal j{h};CHECK(j.origin(origin.input));
        t::thread_journal_context c{};c.worker=1;c.journal=&j;t::thread_journal_scope scope{c};CHECK(j.select(c,h.events[1]));
        CHECK(t::journal_memory_grow(wide_grow.a,wide_grow.b,wide_grow.c,[&]() noexcept{return wide_grow.b;})==wide_grow.b);
        CHECK(j.select(c,h.events[3]));j.release(c);
    }
    {
        auto h=history;h.events[0].output={std::byte{1},std::byte{15},std::byte{200}};
        CHECK(t::valid_execution_history(h));t::threaded_execution_journal j{h};
        std::array<std::byte,3> colors{};CHECK(j.origin(origin.input,colors));
        CHECK(colors[0]==std::byte{1}&&colors[1]==std::byte{15}&&colors[2]==std::byte{200});
        t::threaded_execution_journal bad_size{h};std::array<std::byte,2> too_short{std::byte{8},std::byte{9}};
        CHECK(!bad_size.origin(origin.input,too_short)&&too_short[0]==std::byte{8}&&too_short[1]==std::byte{9});
        t::threaded_execution_journal bad_source{h};auto wrong=origin.input;wrong[0]=std::byte{1};
        CHECK(!bad_source.origin(wrong,colors));
    }
    // Table64 allocation results must not narrow to the native pointer width.
    // A recorded denial is virtual; a recorded success still needs real storage.
    {
        auto wide=[](std::uint64_t value){std::array<std::byte,8> out{};
            for(unsigned n{};n<8;++n)out[n]=static_cast<std::byte>(value>>(8*n));return out;};
        auto deny=event(t::journal_kind::table_grow,1,7);deny.b=UINT64_C(0x100000002);deny.c=UINT64_C(0x100000001);
        auto bytes=wide(UINT64_MAX);deny.output.assign(bytes.begin(),bytes.end());
        auto success=deny;bytes=wide(deny.c);success.output.assign(bytes.begin(),bytes.end());
        t::threaded_execution_history tables;tables.terminal=true;
        tables.events={origin,event(t::journal_kind::enter,1),deny,success,event(t::journal_kind::leave,1)};
        CHECK(t::valid_execution_history(tables));
        auto corrupt=tables;corrupt.events[3].c++;CHECK(!t::valid_execution_history(corrupt));
        for(bool fail_now:{false,true})
        {
            t::threaded_execution_journal j{tables};CHECK(j.origin(origin.input));
            t::thread_journal_context worker{};worker.worker=1;worker.journal=&j;t::thread_journal_scope scope{worker};
            CHECK(j.select(worker,tables.events[1]));unsigned calls{};
            auto actual=[&]()noexcept->std::uint64_t{++calls;return fail_now?UINT64_MAX:deny.c;};
            CHECK(t::journal_table_grow(7,deny.b,deny.c,actual)==UINT64_MAX&&calls==0);
            CHECK(t::journal_table_grow(7,deny.b,deny.c,actual)==(fail_now?UINT64_MAX:deny.c)&&calls==1);
            if(fail_now)CHECK(j.observe().status==t::journal_status::diverged);
            else {CHECK(j.select(worker,tables.events[4]));j.release(worker);}
        }
    }
    std::puts("PASS 100 native scheduler handoffs, reentrant cancellation, recorded allocation outcomes and terminal traps");
}
