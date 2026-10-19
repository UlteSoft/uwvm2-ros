/** @file
 * @brief Actual production event codec/credit queue contract executable.
 * Compile C++17 against src/. This fixture uses no ROS, Guest or permissions;
 * it tests the pure queue boundary, not authenticated Native delivery. The
 * acceptance runner separately exercises all seven kinds through real owners.
 */
#include <uwvm2/imported/ros/ipc/event_queue.h>
#include <cassert>
#include <unistd.h>
#include <atomic>
#include <thread>
namespace ipc=uwvm2::imported::ros::ipc;
static ipc::event_header binding(){ipc::event_header value{};value.backend=2;value.source_handle=0x123400000056ull;value.instance=41;value.session=93;return value;}
static ipc::event_record topic(unsigned value){return {ipc::event_kind::topic_message,0,0,0,{std::uint8_t(value),0,0,0}};}
int main(){
    // The Worker and Native owner use this same no-allocation consumer key.
    // Aliases are deliberately absent: they must not duplicate a native queue.
    auto result_mask=ipc::event_bit(ipc::event_kind::action_result);
    assert(ipc::event_action_consumer_conflict("action.receive-result","/a","pkg/A",99,"action.receive-result","/a","pkg/A",99,result_mask));
    assert(!ipc::event_action_consumer_conflict("action.receive-feedback","/a","pkg/A",99,"action.receive-result","/a","pkg/A",99,result_mask));
    assert(!ipc::event_action_consumer_conflict("action.receive-result","/b","pkg/A",99,"action.receive-result","/a","pkg/A",99,result_mask));
    assert(!ipc::event_action_consumer_conflict("action.receive-result","/a","pkg/B",99,"action.receive-result","/a","pkg/A",99,result_mask));
    assert(!ipc::event_action_consumer_conflict("action.receive-result","/a","pkg/A",100,"action.receive-result","/a","pkg/A",99,result_mask));
    assert(!ipc::event_action_consumer_conflict("action.receive-result","/a","pkg/A",99,"action.receive-result","/a","pkg/A",0,ipc::event_bit(ipc::event_kind::resource_error)));
    // Both memory ranges are separately checked by the Host before this
    // overflow-safe predicate. Adjacency and capacity0 remain valid; touching
    // any byte of the u64 length slot is refused before any native peek/copy.
    assert(!ipc::event_output_overlaps_length(16,8,24)&&!ipc::event_output_overlaps_length(32,8,24));
    assert(ipc::event_output_overlaps_length(16,9,24)&&ipc::event_output_overlaps_length(31,8,24));
    assert(ipc::event_output_overlaps_length(24,1,24)&&!ipc::event_output_overlaps_length(24,0,24));
    assert(!ipc::event_output_overlaps_length(UINT64_MAX-7,7,UINT64_MAX-15));
    assert(ipc::event_output_overlaps_length(UINT64_MAX-7,7,UINT64_MAX-14));
    std::uint32_t mask{};assert(ipc::parse_event_kinds("topic-message,resource-error,resource-closed",mask));
    auto previous=mask;assert(!ipc::parse_event_kinds("resource-closed,topic-message",mask)&&mask==previous);assert(!ipc::parse_event_kinds("topic-message,topic-message",mask));
    ipc::event_stream queue{{3,196,68,1,68,ipc::event_overflow::backpressure},binding(),previous};assert(queue.configured());
    assert(queue.enqueue(topic(1))==0);assert(queue.enqueue(topic(2))==0);assert(queue.enqueue(topic(3))==11);
    auto stats=queue.statistics();assert(stats.depth==1&&stats.pending_depth==1&&stats.enqueued==2&&stats.backpressured==2&&stats.admission_refused==1);
    auto small=queue.poll(67);assert(small.error==90&&small.payload.size()==8&&ipc::get(small.payload.data(),8)==68);assert(queue.statistics().dequeued==0);
    auto first=queue.poll(68);ipc::event_header header{};assert(first.error==0&&ipc::decode_event(first.payload.data(),first.payload.size(),header)&&header.sequence==1&&first.payload[64]==1);
    assert(queue.statistics().pending_depth==0);auto second=queue.poll(68);assert(second.error==0&&second.payload[64]==2);assert(queue.poll(68).error==11);
    // Every kind uses the same complete header; ticket kinds retain unchanged
    // canonical ID8 and reject an independently altered header correlation.
    for(unsigned kind=1;kind<=7;++kind){
        auto h=binding();h.kind=static_cast<ipc::event_kind>(kind);h.sequence=8;
        std::vector<std::uint8_t> body;
        if(kind>=2&&kind<=5){h.correlation=27;if(kind!=3)ipc::put(body,27,8);}
        if(kind==6)h.error=110;if(kind==7)h.flags=1;
        h.payload_bytes=body.size();std::vector<std::uint8_t> record(64+body.size());assert(ipc::encode_event_header(h,record.data(),record.size()));
        std::copy(body.begin(),body.end(),record.begin()+64);assert(ipc::decode_event(record.data(),record.size(),header));
        if(kind==2||kind==4||kind==5){record[24]=28;assert(!ipc::decode_event(record.data(),record.size(),header));}
    }
    // Async reservations cannot be stolen by drop-oldest topic admission. The
    // already allocated credit entry becomes a durable completion without a
    // completion-side queue allocation. Duplicate completion is17; owned ID9.
    auto all=std::uint32_t(127);ipc::event_stream credits{{4,512,128,0,0,ipc::event_overflow::drop_oldest},binding(),all};
    assert(credits.reserve(7,128,32)==0&&credits.reserve(8,128,32)==0&&credits.reserve(9,128,32)==11);
    assert(credits.complete(7,{ipc::event_kind::service_response,0,0,7,{1,0,0,0,0}})==0);
    assert(credits.complete(7,{ipc::event_kind::service_response,0,0,7,{}})==17);
    assert(credits.enqueue(topic(4))==11);auto complete=credits.poll(128);assert(complete.error==0&&ipc::decode_event(complete.payload.data(),complete.payload.size(),header)&&header.correlation==7);
    assert(credits.complete(19,{ipc::event_kind::service_response,0,0,19,{}})==9);
    credits.seal(125,true);assert(credits.complete(8,{ipc::event_kind::service_response,0,0,8,{}})==125);
    auto error=credits.poll(128),closed=credits.poll(128);assert(error.error==0&&closed.error==0);
    assert(ipc::decode_event(error.payload.data(),error.payload.size(),header)&&header.kind==ipc::event_kind::resource_error&&header.error==125&&header.flags==3);
    assert(ipc::decode_event(closed.payload.data(),closed.payload.size(),header)&&header.kind==ipc::event_kind::resource_closed&&header.payload_bytes==0);
    assert(credits.statistics().reserved_depth==0&&credits.statistics().pending_depth==0&&credits.readiness().admission_slots==0);
    ipc::event_stream denied{{3,196,68,0,0,ipc::event_overflow::reject},binding(),ipc::event_bit(ipc::event_kind::topic_message)};denied.seal(125);assert(denied.poll(68).error==11);
    // The retained peek slot is the credit. Arrival overflow cannot replace it,
    // a wrong ticket cannot consume it, and a second exact commit cannot replay.
    ipc::event_stream pinned{{3,196,68,0,0,ipc::event_overflow::drop_oldest},binding(),previous};
    assert(pinned.enqueue(topic(4))==0);auto peek=pinned.peek(68);assert(peek.error==0);
    assert(pinned.enqueue(topic(5))==11);assert(pinned.peek(68).payload==peek.payload);
    assert(pinned.peek(67).error==90&&pinned.peek(68).payload==peek.payload);
    assert(pinned.commit(2)==22&&pinned.statistics().dequeued==0);assert(pinned.commit(1)==0&&pinned.commit(1)==9);
    assert(pinned.enqueue(topic(6))==0&&pinned.peek(68).error==0);pinned.seal(125);
    // Seal cancels the old pin. Its old sequence cannot consume the new metadata
    // before a new peek explicitly reserves that body-free retirement record.
    assert(pinned.commit(2)==9);assert(pinned.peek(68).error==0&&pinned.commit(2)==22&&pinned.commit(3)==0);
    ipc::event_stream durable{{3,512,128,0,0,ipc::event_overflow::reject},binding(),all};
    assert(durable.reserve(91,128,32)==0&&durable.complete(91,{ipc::event_kind::service_response,0,0,91,{1,0,0,0,0}})==0);
    auto held=durable.peek(128);assert(held.error==0&&durable.peek(128).payload==held.payload&&durable.statistics().completed==1&&durable.statistics().dequeued==0);
    assert(durable.commit(1)==0&&durable.statistics().dequeued==1&&durable.statistics().reserved_depth==0);
    // The real stream admission callback previews both credits without debit.
    // Source or observer EAGAIN leaves the same reserved completion/sequence;
    // a wrong commit must not even invoke admission. Only one successful commit
    // debits both, with no replay after the entry has retired.
    ipc::event_stream atomic{{3,512,128,0,0,ipc::event_overflow::reject},binding(),all};
    assert(atomic.reserve(13,128,32)==0&&atomic.complete(13,{ipc::event_kind::service_response,0,0,13,{1,0,0,0,0}})==0);
    unsigned source_bytes=0,observer_bytes=69,admissions=0,debits=0;
    auto admit=[&](ipc::event_header const& h,bool debit)->std::uint32_t{++admissions;assert(h.kind==ipc::event_kind::service_response&&h.correlation==13&&h.payload_bytes==5);if(source_bytes<5||observer_bytes<69)return 11;if(debit){source_bytes-=5;observer_bytes-=69;++debits;}return 0;};
    assert(atomic.peek_if(128,[&](auto const& h){return admit(h,false);}).error==11&&observer_bytes==69&&debits==0);
    source_bytes=5;observer_bytes=0;assert(atomic.peek_if(128,[&](auto const& h){return admit(h,false);}).error==11&&source_bytes==5);
    observer_bytes=69;auto exact=atomic.peek_if(128,[&](auto const& h){return admit(h,false);});assert(!exact.error&&source_bytes==5&&observer_bytes==69);
    auto previous_admissions=admissions;assert(atomic.commit_if(2,[&](auto const& h){return admit(h,true);})==22&&admissions==previous_admissions&&debits==0);
    source_bytes=0;assert(atomic.commit_if(1,[&](auto const& h){return admit(h,true);})==11&&observer_bytes==69&&atomic.peek(128).payload==exact.payload);
    source_bytes=5;assert(atomic.commit_if(1,[&](auto const& h){return admit(h,true);})==0&&debits==1&&source_bytes==0&&observer_bytes==0);
    previous_admissions=admissions;assert(atomic.commit_if(1,[&](auto const& h){return admit(h,true);})==9&&admissions==previous_admissions&&atomic.poll(128).error==11);
    // Metadata selection never confers the separately omitted payload bit.
    ipc::event_stream metadata{{3,192,64,0,0,ipc::event_overflow::reject},binding(),ipc::event_bit(ipc::event_kind::resource_error)|ipc::event_bit(ipc::event_kind::resource_closed)};
    assert(metadata.enqueue(topic(9))==13&&metadata.statistics().depth==0);
    assert(metadata.enqueue({ipc::event_kind::resource_error,ipc::event_external_effects_may_remain,5,0,{}})==0);
    assert(metadata.enqueue({ipc::event_kind::resource_error,0,22,0,{}})==0&&metadata.statistics().depth==1);
    auto fixed=metadata.peek(64);assert(!fixed.error&&ipc::decode_event(fixed.payload.data(),fixed.payload.size(),header)&&header.error==22&&header.correlation==0);
    assert(metadata.enqueue({ipc::event_kind::resource_error,2,110,0,{}})==11&&metadata.peek(64).payload==fixed.payload);
    assert(metadata.commit(1)==0);metadata.seal(0);auto normal_close=metadata.poll(64);assert(!normal_close.error&&ipc::decode_event(normal_close.payload.data(),normal_close.payload.size(),header)&&header.kind==ipc::event_kind::resource_closed&&header.error==0);
    // Concurrent native callbacks cannot create a second pending record or
    // overflow the explicit slot/byte grants. This is trusted-source concurrency.
    ipc::event_stream concurrent{{3,196,68,1,68,ipc::event_overflow::backpressure},binding(),previous};
    std::thread producer1([&]{for(unsigned i=0;i<100;++i)(void)concurrent.enqueue(topic(1));});
    std::thread producer2([&]{for(unsigned i=0;i<100;++i)(void)concurrent.enqueue(topic(2));});producer1.join();producer2.join();
    stats=concurrent.statistics();assert(stats.depth<=1&&stats.bytes<=68&&stats.pending_depth<=1&&stats.pending_bytes<=68&&stats.enqueued==2&&stats.admission_refused==198);
    // One fixed bounded write: vendored fast_io requires newer C++ than this
    // production C++17 boundary. No formatting/scanning or payload reflection.
    static constexpr char marker[]="PASS_UNIFIED_QUEUE\n";assert(::write(1,marker,sizeof(marker)-1)==ssize_t(sizeof(marker)-1));

}
