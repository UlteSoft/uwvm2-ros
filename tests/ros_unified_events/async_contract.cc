/** @file
 * @brief Exercise the real owned bounded task pool without ROS/Guest execution.
 * Build C++17 with src/ and pthread. An independently gated fake Native call
 * establishes admission order; no timing guess establishes close correctness.
 */
#include "../../gateways/common/async_service.h"
#include <cassert>
#include <unistd.h>
#include <condition_variable>
#include <atomic>
namespace gateway=uwvm2::ros_gateway;namespace ipc=uwvm2::imported::ros::ipc;
int main(){
    ipc::event_header binding{};binding.backend=2;binding.source_handle=12;binding.instance=32;binding.session=76;
    auto stream=std::make_shared<ipc::event_stream>(ipc::event_stream_settings{4,4096,128,0,0,ipc::event_overflow::reject},binding,ipc::event_bit(ipc::event_kind::service_response)|ipc::event_bit(ipc::event_kind::resource_error)|ipc::event_bit(ipc::event_kind::resource_closed));
    auto source=std::make_shared<gateway::async_service_source>();gateway::async_service_pool pool;
    std::mutex mutex;std::condition_variable changed;bool entered=false,release=false;std::atomic<unsigned> effects{0};
    assert(stream->reserve(1,128,gateway::async_service_pool::accounting_bytes)==0);
    gateway::async_service_pool::task first{source,stream,1,std::chrono::steady_clock::now()+std::chrono::seconds(2),{},[&](auto const&,std::uint32_t timeout){assert(timeout>0);std::unique_lock<std::mutex> lock{mutex};entered=true;changed.notify_all();assert(changed.wait_for(lock,std::chrono::seconds(1),[&]{return release;}));++effects;return ipc::call_result{0,{1,0,0,0,0}};}};
    assert(pool.submit(std::move(first),2)==0);{std::unique_lock<std::mutex> lock{mutex};assert(changed.wait_for(lock,std::chrono::seconds(1),[&]{return entered;}));}
    assert(stream->reserve(2,128,gateway::async_service_pool::accounting_bytes)==0);
    gateway::async_service_pool::task second{source,stream,2,std::chrono::steady_clock::now()+std::chrono::seconds(2),{},[&](auto const&,std::uint32_t){++effects;return ipc::call_result{};}};
    assert(pool.submit(std::move(second),2)==0);
    source->retire();stream->seal(125,true);{std::lock_guard<std::mutex> lock{mutex};release=true;}changed.notify_all();pool.stop();
    assert(effects.load()==1);assert(source->inflight==0);assert(stream->statistics().reserved_depth==0);
    auto error=stream->poll(128),closed=stream->poll(128);ipc::event_header header{};
    assert(error.error==0&&ipc::decode_event(error.payload.data(),error.payload.size(),header)&&header.kind==ipc::event_kind::resource_error);
    assert(closed.error==0&&ipc::decode_event(closed.payload.data(),closed.payload.size(),header)&&header.kind==ipc::event_kind::resource_closed);
    assert(stream->poll(128).error==11);assert(!stream->active());
    // Error metadata distinguishes a call already attempted from expiration
    // before transport admission. This bit is conservative effect disclosure,
    // never a cancellation/rollback promise and never implicit permission.
    for(bool expired:{false,true})
    {
        auto result_stream=std::make_shared<ipc::event_stream>(ipc::event_stream_settings{3,512,128,0,0,ipc::event_overflow::reject},binding,ipc::event_bit(ipc::event_kind::service_response)|ipc::event_bit(ipc::event_kind::resource_error));
        auto result_source=std::make_shared<gateway::async_service_source>();gateway::async_service_pool result_pool;std::atomic<unsigned> attempted{0};
        assert(result_stream->reserve(21,128,gateway::async_service_pool::accounting_bytes)==0);
        auto deadline=std::chrono::steady_clock::now()+(expired?-std::chrono::seconds(1):std::chrono::seconds(2));
        gateway::async_service_pool::task request{result_source,result_stream,21,deadline,{},[&](auto const&,std::uint32_t){++attempted;return ipc::call_result{5,{}};}};
        assert(result_pool.submit(std::move(request),1)==0);
        auto wait_until=std::chrono::steady_clock::now()+std::chrono::seconds(1);
        for(;;){bool finished;{std::lock_guard<std::mutex> guard{result_source->mutex};finished=result_source->inflight==0;}if(finished)break;assert(std::chrono::steady_clock::now()<wait_until);std::this_thread::yield();}
        result_pool.stop();auto record=result_stream->poll(128);assert(!record.error&&ipc::decode_event(record.payload.data(),record.payload.size(),header));
        assert(header.kind==ipc::event_kind::resource_error&&header.correlation==21&&header.payload_bytes==0);
        assert(header.error==(expired?110u:5u)&&header.flags==(expired?0u:ipc::event_external_effects_may_remain)&&attempted.load()==(expired?0u:1u));
        assert(result_stream->poll(128).error==11&&result_stream->statistics().reserved_depth==0);
    }
    // One fixed bounded write: vendored fast_io requires newer C++ than this
    // production C++17 boundary. No formatting/scanning or payload reflection.
    static constexpr char marker[]="PASS_UNIFIED_ASYNC\n";assert(::write(1,marker,sizeof(marker)-1)==ssize_t(sizeof(marker)-1));

}
