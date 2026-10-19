// Detached DATA contract only; live Host imports are verified separately.
#include <uwvm2/imported/ros/common/debug.h>
#include <cstdio>
#include <cstdlib>
namespace d=uwvm2::imported::ros::debug;
static void check(bool v,char const* why){if(!v){std::fprintf(stderr,"FAIL interface DATA: %s\n",why);std::exit(1);}}
int main(){
 d::interface_store store;d::trace_record call{};call.backend=2;call.instance=7;call.generation=3;call.module.assign("main");call.alias.assign("pub");call.operation.assign("topic.publish");call.function.assign("publish");
 call.started_ns=10;call.monotonic_ns=40;call.duration_ns=30;call.duration_known=true;store.append(call);
 call.monotonic_ns=60;call.duration_ns=50;call.error=13;store.append(call);
 d::request q{};q.selected=d::selection::calls;d::view a{},b{};store.read(q,a);store.read(q,b);
 check(a.result==d::status::ok&&a.interface_size==1&&a.total==1,"exact fixed aggregate");auto const& row=a.interfaces[0];
 check(row.calls==2&&row.errors==1&&row.timed==2&&row.total_ns==80&&row.min_ns==30&&row.max_ns==50,"count/error/latency aggregate");
 check(b.interfaces[0].calls==row.calls&&b.interfaces[0].total_ns==row.total_ns&&b.interfaces[0].window_ns>=row.window_ns,"reads do not consume/reset");
 q.function.assign("other");store.read(q,b);check(b.total==0&&b.interface_size==0,"exact function filter");q.function={};
 call.generation=4;store.append(call);q.first=1;store.read(q,b);check(b.total==2&&b.interface_size==1&&b.interfaces[0].generation==4,"replacement generation remains distinct");q.first=0;
 call.function.assign(std::string_view{"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"});store.append(call);store.read(q,b);
 check(b.total==2&&b.dropped_labels==1,"truncated identity cannot merge");call.function.assign("publish");
 for(unsigned i=0;i<127;++i){call.instance=100+i;store.append(call);}store.read(q,b);check(b.total==128&&b.more&&b.dropped_full==1,"fixed 128 slot bound reports overflow");
 q.first=128;store.read(q,b);check(b.total==128&&b.interface_size==0&&!b.more,"empty terminal page");
 d::interface_store overflow;call.instance=7;call.started_ns=0;call.monotonic_ns=UINT64_MAX;call.duration_ns=UINT64_MAX;overflow.append(call);overflow.append(call);q.first=0;overflow.read(q,b);
 check(b.interfaces[0].saturated&&b.interfaces[0].total_ns==UINT64_MAX&&b.interfaces[0].calls==2,"duration sums saturate without wrap");
 d::trace_store trace;trace.append(call);trace.read_calls(q,b);check(b.interface_size==1,"actual trace append feeds interface store");
 q.selected=d::selection::trace;trace.read(q,b);check(b.trace_size==1,"interface snapshot leaves trace ring intact");
 d::call_result pending{},success{},failure{};check(!pending.known,"unreturned result begins unknown");
 check(success.finish(0)==0&&success.known&&!success.error,"normal successful return is explicitly observed");
 check(failure.finish(13)==13&&failure.known&&failure.error==13,"normal denied return is explicitly observed");
#if defined(__cpp_exceptions) || defined(_CPPUNWIND)
 try {throw 17;} catch(int) {}
 check(!pending.known,"exception path does not invent a result");
#endif
 d::trace_store abandoned;call={};call.backend=2;call.alias.assign("publish");call.result_known=false;
 abandoned.append(call);q={};q.selected=d::selection::calls;abandoned.read_calls(q,b);
 check(b.interface_size==1&&b.interfaces[0].calls==1&&b.interfaces[0].incomplete==1&&b.interfaces[0].errors==0,"unknown result is counted separately from known errors");
 q.selected=d::selection::trace;q.error_filter=true;q.error=0;abandoned.read(q,b);
 check(b.trace_size==0&&b.total==0,"success filter excludes unknown result");
 q.error_filter=false;abandoned.read(q,b);check(b.trace_size==1&&!b.trace[0].result_known,"unfiltered trace retains unknown result");
 std::puts("PASS detached ROS interface statistics DATA");}
