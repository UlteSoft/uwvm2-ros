// Licensed under the APL-2.0 License (see LICENSE file).
#include <cassert>
#include <cstdio>
#include <thread>
#include <uwvm2/uwvm/debugger/host_trace.h>

namespace trace = uwvm2::uwvm::debugger::host_trace;
namespace journal = uwvm2::utils::debug::host_calls;

int main()
{
    trace::request request;
    assert(trace::parse("trace host read family=rosv2",request) && request.channels==8);
    assert(!trace::parse("trace host read family=unknown",request));
    auto observe=[] {journal::observe_scene_result("uwvm_ros_v2","abi",2);};
    observe();
    assert(journal::records.empty());
    journal::enabled.store(8);
    observe();
    assert(journal::records.size()==1 && !journal::active);
    auto report=trace::apply(request);
    std::fwrite(report.data(),1,report.size(),stdout);
    auto const& row=*journal::records.front();
    assert(row.family==journal::channel::rosv2 && row.returned && row.has_result && row.result==2);
    assert(row.count==0 && row.segments.empty() && row.module_name.empty() && !row.timed);
    {
        std::lock_guard lock{journal::mutex};
        std::thread peer{observe};
        peer.join();
    }
    assert(journal::scene_dropped.load()==1 && journal::records.size()==1);
    for(std::size_t i=1;i<=journal::scene_record_limit;++i) {observe();}
    assert(journal::records.size()==journal::scene_record_limit && journal::scene_dropped.load()==2);
    journal::clear();
    observe();
    assert(journal::records.size()==1 && journal::records.front()->id==journal::scene_record_limit+1);
    assert(journal::scene_dropped.load()==2);
}
