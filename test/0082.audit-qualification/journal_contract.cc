// Detached diagnostic journal semantics; this test supplies no VM authority.
#include <uwvm2/uwvm/debugger/wasip1_calls.h>
#include <uwvm2/utils/container/impl.h>
#include <cstdio>
#include <cstdlib>
#include <csignal>
#include <future>
#include <thread>
#include <string_view>
namespace calls=uwvm2::uwvm::debugger::wasip1_calls;
namespace ws=uwvm2::uwvm::debugger::wasip1_state;
static unsigned checks{};
static void check(bool good,char const* why)
{++checks;if(!good) {std::fprintf(stderr,"FAIL audit journal: %s\n",why);std::exit(1);}}
static calls::view read()
{ws::request r{};r.operation=ws::action::trace_read;r.count=64;return calls::apply(r);}
int main()
{
    if(std::getenv("UWVM_AUDIT_HOLD")) {std::raise(SIGSTOP);}
    ws::request r{};r.operation=ws::action::trace_enable;(void)calls::apply(r);
    calls::record row{};row.count=4;row.widths.fill(32);row.arguments[0]=3;row.environment=calls::new_environment();
    calls::observe_thread(4,2);auto token=calls::enter(u8"fd_write",row);
    {calls::call_scope scope{token};calls::observe_io(9,0,false);calls::observe_io(9,5,true);}
    r.operation=ws::action::trace_disable;(void)calls::apply(r);calls::leave(token,true,0);
    auto page=read();check(page.records.size()==2&&page.records[1].returned&&page.records[1].call==token.call,
        "disable admits the return of an already observed call");
    check(page.records[1].requested_bytes==9&&page.records[1].transferred_bytes==5&&page.records[1].transferred_bytes_known,
        "scoped numeric metadata follows the observed pair");
    auto sequence=page.newest;check(calls::enter(u8"fd_write",row).call==0&&read().newest==sequence,"disabled entry produces no journal work");
    r.operation=ws::action::trace_enable;r.name=u8"fd_write";(void)calls::apply(r);
    check(calls::enter(u8"fd_read",row).call==0,"function filter is exact");
    r.name.clear();(void)calls::apply(r);auto invalid=read().dropped_invalid;
    check(calls::enter(u8"fd_write\n",row).call==0&&read().dropped_invalid==invalid+1,"invalid metadata loss is explicit");
    auto live=calls::enter(u8"fd_read",row);std::promise<void> locked,release;auto released=release.get_future();
    std::thread owner([&]{std::lock_guard guard{calls::mutex};locked.set_value();released.wait();});locked.get_future().wait();
    auto blocked=calls::enter(u8"fd_write",row);calls::leave(live,true,8);release.set_value();owner.join();
    page=read();check(blocked.call==0&&page.dropped_busy==2,"entry and return contention drop metadata without waiting for the owner");
    auto pending=calls::enter(u8"fd_write",row);r.operation=ws::action::trace_clear;(void)calls::apply(r);calls::leave(pending,true,0);
    page=read();check(page.records.empty()&&page.retired_inflight==1&&page.cleared_through>=pending.call&&page.gap,
        "clear explicitly retires outstanding pair and exposes a cleared cursor gap");
    r.operation=ws::action::trace_enable;(void)calls::apply(r);
    for(unsigned i{};i<260;++i) {auto call=calls::enter(u8"fd_write",row);calls::leave(call,true,0);}
    page=read();check(page.records.size()==64&&page.overwritten==8&&page.gap&&page.remaining==448,"bounded journal exposes overwrite and pagination");
    auto next=page.next;r={};r.operation=ws::action::trace_read;r.first=next;r.count=64;auto second=calls::apply(r);
    check(second.records.front().sequence==next+1,"exclusive cursor does not duplicate records");
    calls::view maximum{};maximum.enabled=true;maximum.filter_size=63;maximum.filter.fill('a');
    maximum.overwritten=maximum.oldest=maximum.newest=maximum.next=maximum.remaining=UINT64_MAX;
    maximum.dropped_busy=maximum.dropped_sequence=maximum.dropped_invalid=maximum.retired_inflight=maximum.cleared_through=maximum.epoch=UINT64_MAX;
    calls::record extreme{};extreme.name_size=63;extreme.name.fill('a');extreme.count=16;extreme.arguments.fill(UINT64_MAX);extreme.widths.fill(64);
    extreme.sequence=extreme.call=extreme.participant=extreme.module=extreme.session=extreme.environment=extreme.started_ns=UINT64_MAX;extreme.context_available=true;
    for(unsigned i{};i<64;++i) {maximum.records.push_back(extreme);}
    uwvm2::utils::container::string text{};calls::print(uwvm2::utils::container::string_ref_uwvm{std::addressof(text)},maximum);
    check(text.size()<=65536,"maximum ABI argument page fits the real reply budget");
    for(auto& item:maximum.records)
    {item.returned=item.has_errno=item.duration_known=item.descriptor_created=item.requested_bytes_known=item.transferred_bytes_known=true;
     item.result=item.duration_ns=item.created_descriptor=item.requested_bytes=item.transferred_bytes=UINT64_MAX;}
    text.clear();calls::print(uwvm2::utils::container::string_ref_uwvm{std::addressof(text)},maximum);
    check(text.size()<=65536,"maximum numeric return page fits the real reply budget");
    // Counter state is advanced only in this isolated DATA executable.
    {std::lock_guard lock{calls::mutex};calls::sequence=UINT64_MAX-1;}
    auto final=calls::enter(u8"fd_write",row);calls::leave(final,true,0);page=read();
    check(final.call==UINT64_MAX&&page.sequence_exhausted&&page.dropped_sequence==1&&!page.enabled,"sequence exhaustion is distinct from contention");
    r={};r.operation=ws::action::trace_enable;(void)calls::apply(r);check(!read().enabled,"enable cannot hide sequence exhaustion");
    calls::dropped_busy.store(UINT64_MAX-1);calls::increment(calls::dropped_busy);calls::increment(calls::dropped_busy);
    check(read().dropped_busy==UINT64_MAX,"loss counters saturate without wrapping");
    std::printf("PASS_AUDIT_JOURNAL checks=%u\n",checks);
}
