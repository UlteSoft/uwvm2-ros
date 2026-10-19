/** Authored SDK-free wire/queue contracts. No fake ROS entity is used and no
 * native transport credit follows from running these local data contracts. */
#ifdef NDEBUG
#error "Queue contracts require active assertions"
#endif
#include "../../../gateways/ros2/native_rmw_queue.h"
#include <cstdlib>
#include <cstring>
#include <atomic>
#include <thread>
namespace q=uwvm2::ros_gateway::native_rmw_queue;
namespace w=uwvm2::imported::ros::ipc::rmw_endpoint;
namespace m=uwvm2::imported::ros::ipc::rmw_metadata;
static void check(bool value){if(!value){std::abort();}}
static w::binding owner{2,UINT64_C(0xfeedbeef81234567),19,23,29};
static m::header header(bool complete=true)
{
    m::header h{};h.owner=owner;h.incarnation=31;h.sequence=999;
    if(complete){h.flags=m::source_time|m::received_time|m::publisher_gid;h.gid_size=16;h.source_timestamp=-17;h.received_timestamp=0;}
    return h; // Legal all-zero wire data; this model is not native attestation.
}
static std::shared_ptr<q::bounded_queue> make(std::size_t records=2,q::overflow policy=q::overflow::reject,
    std::size_t bytes=1024,std::size_t body=32)
{std::shared_ptr<q::bounded_queue> x;check(q::bounded_queue::create({records,bytes,body,policy},owner,31,x)==0);check(x->activate()==0);return x;}
static std::vector<std::uint8_t> body(unsigned n){return {static_cast<std::uint8_t>(n),0,0xff};}
static void options_and_factory_output()
{
    auto x{make()};auto sentinel{x};check(q::bounded_queue::create({0,1024,32},owner,31,x)==22&&x==sentinel);
    check(q::bounded_queue::create({1,191,0},owner,31,x)==22&&x==sentinel);
    check(q::bounded_queue::create({65537,1024,0},owner,31,x)==22&&x==sentinel);
    auto bad{owner};bad.source=17;check(q::bounded_queue::create({1,1024,32},bad,31,x)==22&&x==sentinel);
}
static void same_record_zero_gid_and_signed_timestamp()
{
    auto x{make()};auto bytes{body(7)};check(x->enqueue(header(),bytes)==0&&bytes.empty());q::snapshot s;
    check(x->peek(owner,31,16,s)==0&&s.sequence==1&&s.size==195);m::header h{};
    check(m::decode(s.bytes.get(),192,3,h)&&m::complete(h)&&h.sequence==1&&h.source_timestamp==-17&&h.received_timestamp==0);
    check(h.gid_size==16&&w::zero(h.gid.data(),32)&&s.bytes[192]==7&&s.bytes[193]==0&&s.bytes[194]==0xff);
    check(x->commit(owner,31,s.sequence,s.reservation)==0&&x->observe().records==0);
}
static void missing_metadata_is_not_consumed()
{
    auto x{make()};auto bytes{body(1)};check(x->enqueue(header(false),bytes)==0);
    q::snapshot s;s.sequence=777;s.size=4;s.bytes=std::make_unique<std::uint8_t[]>(4);s.bytes[0]=73;auto* original{s.bytes.get()};
    check(x->peek(owner,31,16,s)==95&&s.sequence==777&&s.size==4&&s.bytes.get()==original&&s.bytes[0]==73);
    auto observation{x->observe()};check(observation.records==1&&!observation.reserved&&observation.consumed==0&&observation.metadata_unavailable==1&&observation.first_error==95);
}
static void explicit_capture_error_is_not_consumed()
{auto x{make()};auto bytes{body(1)};check(x->enqueue(header(),bytes,95)==0);q::snapshot s;check(x->peek(owner,31,16,s)==95&&x->observe().records==1&&!x->observe().reserved);}
static void incompatible_gid_extent_is_atomic()
{
    auto x{make()};auto bytes{body(1)};check(x->enqueue(header(),bytes)==0);q::snapshot s;s.sequence=79;
    for(auto width:{0u,24u,33u}){check(x->peek(owner,31,width,s)==95&&s.sequence==79&&!s.bytes&&!x->observe().reserved);}
    check(x->peek(owner,31,16,s)==0&&x->cancel(owner,31,s.sequence,s.reservation)==0);
}
static void every_binding_component_and_incarnation()
{
    auto x{make()};auto bytes{body(1)};check(x->enqueue(header(),bytes)==0);q::snapshot s;s.sequence=59;
    for(unsigned part{};part!=5;++part)
    {
        auto bad{owner};if(part==0){bad.backend=1;}if(part==1){++bad.source;}if(part==2){++bad.instance;}
        if(part==3){++bad.generation;}if(part==4){++bad.session;}
        check(x->peek(bad,31,16,s)==13&&s.sequence==59&&x->commit(bad,31,1,1)==13&&x->cancel(bad,31,1,1)==13);
    }
    check(x->peek(owner,32,16,s)==13&&s.sequence==59&&x->observe().records==1);
}
static void reservation_blocks_eviction_and_other_consumer()
{
    auto x{make(1,q::overflow::drop_oldest)};auto a{body(1)};check(x->enqueue(header(),a)==0);q::snapshot s,t;t.sequence=71;
    check(x->peek(owner,31,16,s)==0&&x->peek(owner,31,16,t)==17&&t.sequence==71);
    auto b{body(2)};check(x->enqueue(header(),b)==90&&!b.empty());auto observation{x->observe()};
    check(observation.last_sequence==1&&observation.drop_oldest==0&&observation.rejected==1);
    check(x->commit(owner,31,s.sequence,s.reservation)==0);
}
static void cancel_preserves_and_commit_is_one_shot()
{
    auto x{make()};auto bytes{body(1)};check(x->enqueue(header(),bytes)==0);q::snapshot a,b;
    check(x->peek(owner,31,16,a)==0&&x->commit(owner,31,2,a.reservation)==13&&x->cancel(owner,31,2,a.reservation)==13);
    check(x->cancel(owner,31,1,a.reservation)==0&&x->observe().records==1&&x->commit(owner,31,1,a.reservation)==13);
    check(x->peek(owner,31,16,b)==0&&a.sequence==b.sequence&&std::memcmp(a.bytes.get(),b.bytes.get(),a.size)==0);
    check(x->commit(owner,31,1,b.reservation)==0&&x->commit(owner,31,1,b.reservation)==13&&x->observe().consumed==1);
}
static void drop_oldest_gaps_are_accounted()
{
    auto x{make(1,q::overflow::drop_oldest)};auto a{body(1)},b{body(2)};
    check(x->enqueue(header(),a)==0&&x->enqueue(header(),b)==0);q::snapshot s;check(x->peek(owner,31,16,s)==0);
    auto observation{x->observe()};check(s.sequence==2&&s.bytes[192]==2&&observation.accepted==2&&observation.drop_oldest==1&&observation.consumed==0);
}
static void drop_newest_keeps_original_and_incoming()
{
    auto x{make(1,q::overflow::drop_newest)};auto a{body(1)},b{body(2)};
    check(x->enqueue(header(),a)==0&&x->enqueue(header(),b)==90&&b==body(2));q::snapshot s;
    check(x->peek(owner,31,16,s)==0&&s.sequence==1&&s.bytes[192]==1&&x->observe().drop_newest==1);
}
static void reject_does_not_advance_sequence()
{
    auto x{make(1)};auto a{body(1)},b{body(2)};check(x->enqueue(header(),a)==0&&x->enqueue(header(),b)==90);
    q::snapshot s;check(x->peek(owner,31,16,s)==0&&x->commit(owner,31,1,s.reservation)==0&&x->enqueue(header(),b)==0);
    check(x->peek(owner,31,16,s)==0&&s.sequence==2&&x->observe().rejected==1);
}
static void capacity_and_header_are_charged()
{
    auto x{make(4,q::overflow::reject,400,16)};std::vector<std::uint8_t> a(1);a.reserve(16);
    auto actual_capacity{a.capacity()};check(x->enqueue(header(),a)==0);
    check(x->observe().bytes==192+actual_capacity);auto b{body(2)};check(x->enqueue(header(),b)==90);
    q::snapshot s;check(x->peek(owner,31,16,s)==0&&x->commit(owner,31,s.sequence,s.reservation)==0&&x->observe().bytes==0);
}
static void invalid_header_never_evicts()
{
    auto x{make(1,q::overflow::drop_oldest)};auto a{body(1)},b{body(2)};check(x->enqueue(header(),a)==0);
    auto invalid{header()};invalid.flags|=128;check(x->enqueue(invalid,b)==22&&b==body(2));q::snapshot s;
    check(x->peek(owner,31,16,s)==0&&s.sequence==1&&s.bytes[192]==1&&x->observe().drop_oldest==0);
}
static void retirement_closes_ingress_and_consumer()
{
    auto x{make()};auto a{body(1)};check(x->enqueue(header(),a)==0);q::snapshot s;check(x->peek(owner,31,16,s)==0);
    x->retire();auto observation{x->observe()};check(observation.retired&&!observation.active&&!observation.reserved&&observation.retired_records==1&&observation.bytes==0);
    auto b{body(2)};check(x->enqueue(header(),b)==125&&b==body(2)&&x->peek(owner,31,16,s)==125&&x->commit(owner,31,1,s.reservation)==125&&x->activate()==125);
    x->retire();check(x->observe().retired_records==1);
}
static void empty_canonical_body_is_a_real_record()
{
    auto x{make(1,q::overflow::reject,192,0)};std::vector<std::uint8_t> empty;check(x->enqueue(header(),empty)==0);
    q::snapshot s;check(x->peek(owner,31,16,s)==0&&s.size==192&&x->observe().bytes==192);
    m::header h{};check(m::decode(s.bytes.get(),192,0,h)&&h.payload_bytes==0&&x->commit(owner,31,1,s.reservation)==0);
}
static void before_activation_is_not_business()
{
    std::shared_ptr<q::bounded_queue> x;check(q::bounded_queue::create({1,1024,32},owner,31,x)==0);auto b{body(1)};
    check(x->enqueue(header(),b)==11&&b==body(1)&&x->observe().accepted==0&&x->observe().before_activation==1);
    check(x->activate()==0&&x->enqueue(header(),b)==0&&x->observe().last_sequence==1);
}
static void concurrent_retire_closes_actual_queue_ingress()
{
    auto x{make(4,q::overflow::drop_oldest)};std::atomic<bool> start{false};
    std::thread producer{[&]()
    {
        while(!start.load(std::memory_order_acquire)){std::this_thread::yield();}
        for(unsigned n{};n!=100;++n){auto bytes{body(n)};auto code{x->enqueue(header(),bytes)};check(code==0||code==125);}
    }};
    start.store(true,std::memory_order_release);x->retire();auto at_retire{x->observe()};producer.join();auto after{x->observe()};
    check(after.retired&&after.records==0&&after.bytes==0&&after.accepted==at_retire.accepted&&after.last_sequence==at_retire.last_sequence);
    check(after.accepted==after.drop_oldest+after.retired_records&&after.consumed==0);
}
static void cancelled_attempt_cannot_commit_reborrowed_record()
{
    auto x{make()};auto bytes{body(9)};check(x->enqueue(header(),bytes)==0);
    q::snapshot a,b;check(x->peek(owner,31,16,a)==0&&a.sequence==1&&a.reservation==1);
    check(x->cancel(owner,31,a.sequence,a.reservation)==0&&x->observe().last_reservation==1);
    check(x->peek(owner,31,16,b)==0&&b.sequence==a.sequence&&b.reservation==2&&b.reservation!=a.reservation);
    auto before{x->observe()};check(x->commit(owner,31,a.sequence,a.reservation)==13);
    auto after{x->observe()};check(after.reserved&&after.records==before.records&&after.bytes==before.bytes&&after.consumed==0&&after.last_sequence==1&&after.last_reservation==2);
    check(x->commit(owner,31,b.sequence,0)==13&&x->commit(owner,31,b.sequence,b.reservation)==0&&x->observe().consumed==1);
}
static void cancelled_attempt_cannot_cancel_reborrowed_record()
{
    auto x{make()};auto bytes{body(10)};check(x->enqueue(header(),bytes)==0);q::snapshot sentinel;sentinel.sequence=77;sentinel.reservation=88;
    // An incompatible ABI does not allocate a token or change the caller's
    // previous snapshot, even though a complete record is waiting at front.
    check(x->peek(owner,31,24,sentinel)==95&&sentinel.sequence==77&&sentinel.reservation==88&&x->observe().last_reservation==0);
    check(x->peek(owner,31,16,sentinel,194)==90&&sentinel.sequence==77&&sentinel.reservation==88&&x->observe().last_reservation==0&&x->observe().records==1);
    q::snapshot a,b,c;check(x->peek(owner,31,16,a)==0&&x->cancel(owner,31,a.sequence,a.reservation)==0);
    check(x->peek(owner,31,16,b)==0&&b.sequence==a.sequence&&b.reservation>a.reservation);
    check(x->cancel(owner,31,a.sequence,a.reservation)==13&&x->observe().reserved&&x->observe().records==1);
    check(x->cancel(owner,31,b.sequence,0)==13&&x->cancel(owner,31,b.sequence,b.reservation)==0);
    check(x->peek(owner,31,16,c)==0&&c.sequence==a.sequence&&c.reservation>b.reservation);
    check(x->commit(owner,31,b.sequence,b.reservation)==13&&x->cancel(owner,31,b.sequence,b.reservation)==13);
    check(x->commit(owner,31,c.sequence,c.reservation)==0&&x->observe().consumed==1);
    auto incomplete{make()};auto missing{body(11)};check(incomplete->enqueue(header(false),missing)==0);
    check(incomplete->peek(owner,31,16,sentinel)==95&&sentinel.sequence==77&&sentinel.reservation==88&&incomplete->observe().last_reservation==0);
}
int main()
{
    options_and_factory_output();same_record_zero_gid_and_signed_timestamp();missing_metadata_is_not_consumed();
    explicit_capture_error_is_not_consumed();incompatible_gid_extent_is_atomic();every_binding_component_and_incarnation();
    reservation_blocks_eviction_and_other_consumer();cancel_preserves_and_commit_is_one_shot();drop_oldest_gaps_are_accounted();
    drop_newest_keeps_original_and_incoming();reject_does_not_advance_sequence();capacity_and_header_are_charged();
    invalid_header_never_evicts();retirement_closes_ingress_and_consumer();empty_canonical_body_is_a_real_record();before_activation_is_not_business();
    concurrent_retire_closes_actual_queue_ingress();
    cancelled_attempt_cannot_commit_reborrowed_record();cancelled_attempt_cannot_cancel_reborrowed_record();
    return 0;
}
