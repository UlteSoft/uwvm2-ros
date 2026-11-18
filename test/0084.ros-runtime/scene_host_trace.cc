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
    assert(journal::scene_loss_counts().total()==1 && journal::records.size()==1);
    for(std::size_t i=1;i<=journal::scene_record_limit;++i) {observe();}
    assert(journal::records.size()==journal::scene_record_limit && journal::scene_loss_counts().total()==2);
    assert(journal::scene_loss_counts().busy==1 && journal::scene_loss_counts().capacity==1);
    journal::clear();
    observe();
    assert(journal::records.size()==1 && journal::records.front()->id==journal::scene_record_limit+1);
    assert(journal::scene_loss_counts().total()==2);
    journal::clear();
    journal::observe_scene_result("uwvm_ros_v2","capability.open",13,"main",UINT64_MAX,100,175);
    assert(journal::records.back()->timed && journal::records.back()->duration==75);
    journal::observe_scene_result("uwvm_ros_v2","capability.open",0,"other",5);
    journal::observe_scene_result("uwvm_ros_v2","abi",2,"",9);
    assert(journal::records.back()->generation==0);
    std::string caller(255,'a');
    caller.back()=static_cast<char>(0xff);
    journal::observe_scene_result("uwvm_ros_v2","abi",2,caller,UINT64_MAX);
    assert(journal::records.back()->generation==UINT64_MAX &&
           std::string_view{journal::records.back()->module_name}==caller);
    caller.push_back('b');
    journal::observe_scene_result("uwvm_ros_v2","abi",2,caller,UINT64_MAX);
    assert(journal::records.back()->generation==0 && journal::records.back()->module_name.empty());
    journal::observe_scene_result("uwvm_ros_v2","abi",2,"unverified",0);
    assert(journal::records.back()->generation==0 && journal::records.back()->module_name.empty());
    assert(trace::parse("trace host read family=rosv2 module=main function=capability.open namespace=uwvm_ros_v2 result=13 count=1",request));
    auto page=trace::apply(request);
    assert(page.find("module-name=\"main\"")!=page.npos && page.find("result=13 ")!=page.npos &&
           page.find("generation=18446744073709551615")!=page.npos && page.find("more=0")!=page.npos);
    assert(trace::parse("trace host read family=rosv2 result=0 module=main",request));
    assert(trace::apply(request).find("host-call id=")==page.npos);
    for(std::string_view text:{"trace host read family=ros1 result=1","trace host read family=rosv2 result=4294967296",
                              "trace host on family=rosv2 function=abi","trace host read family=rosv2 module=bad/",
                              "trace host read family=rosv2 function=abi function=abi","trace host read family=rosv2 alias=signal"})
    {assert(!trace::parse(::fast_io::string_view{text.data(),text.size()},request));}
    std::string too_long(256,'x');
    journal::observe_scene_result("uwvm_ros_v2",too_long,13);
    assert(journal::scene_loss_counts().labels==1);
    auto const saved_sequence=journal::sequence;
    journal::sequence=UINT64_MAX;
    journal::observe_scene_result("uwvm_ros_v2","abi",2);
    assert(journal::scene_loss_counts().sequence==1);
    journal::sequence=saved_sequence;
    journal::scene_losses overflow{UINT64_MAX-1,0,1,1,0};
    assert(overflow.total()==UINT64_MAX);
}
