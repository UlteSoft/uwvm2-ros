/**
 * @file queue_ipc_metrics.cc
 * @brief Actual native queue snapshots and separately identified socketpair IPC.
 *
 * CLI: queue|ipc. The controller maps this stopped ARM executable and sends I.
 * Queue measurements use the same production event_stream and legacy source
 * queue implementation, with explicit trusted settings. They grant no Guest
 * observation right and prove neither ROS delivery nor Host authorization.
 * IPC measurements use real owned AF_UNIX socketpairs and the production
 * frame encoder/transfer code, deliberately excluding Gateway authentication,
 * native ROS work and permission checks. Those costs require separate actual
 * Worker/Gateway/native comparisons. No physical ARM performance claim is made.
 */
#include <fast_io.h>
#include "../../gateways/common/server.h"
#include <chrono>
#include <csignal>
#include <stdexcept>
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>

__attribute__((constructor)) static void metrics_preinit_stop()
{if(::raise(SIGSTOP)!=0){__builtin_trap();}}

namespace metrics
{
    namespace ipc=uwvm2::imported::ros::ipc;namespace gw=uwvm2::ros_gateway;
    using clock=std::chrono::steady_clock;
    void require(bool value){if(!value)throw std::runtime_error("fixed queue/IPC metrics refusal");}
    std::uint64_t nanoseconds(clock::duration value){return std::chrono::duration_cast<std::chrono::nanoseconds>(value).count();}
    void input()
    {
        pollfd ready{0,POLLIN,0};require(::poll(&ready,1,20000)>0&&(ready.revents&POLLIN));char byte{};require(::read(0,&byte,1)==1&&byte=='I');
        require(::signal(SIGPIPE,SIG_IGN)!=SIG_ERR);
    }
    void snapshot(std::string_view name,std::string_view step,ipc::event_queue_statistics const& s,bool conservation=true)
    {
        // Keep literal lengths in the views; a raw const-char pointer is not
        // an unambiguous fast_io printable string argument.
        constexpr std::string_view queued_scope{"queued-topic-before-retirement"};
        constexpr std::string_view retirement_scope{"retirement-or-reserved-credit"};
        fast_io::println("{\"kind\":\"actual-native-queue\",\"queue\":\"",name,"\",\"step\":\"",step,
          "\",\"conservation_scope\":\"",conservation?queued_scope:retirement_scope,
          "\",\"depth\":",s.depth,",\"bytes\":",s.bytes,",\"pending_depth\":",s.pending_depth,",\"pending_bytes\":",s.pending_bytes,
          ",\"reserved_depth\":",s.reserved_depth,",\"reserved_bytes\":",s.reserved_bytes,",\"high_water_depth\":",s.high_water_depth,
          ",\"high_water_bytes\":",s.high_water_bytes,",\"enqueued\":",s.enqueued,",\"dequeued\":",s.dequeued,
          ",\"dropped_oldest\":",s.dropped_oldest,",\"dropped_newest\":",s.dropped_newest,",\"rejected\":",s.rejected,
          ",\"backpressured\":",s.backpressured,",\"admission_refused\":",s.admission_refused,",\"completed\":",s.completed,",\"closed\":",s.closed,"}");
    }
    ipc::event_header binding()
    {ipc::event_header value{};value.backend=2;value.source_handle=1;value.instance=2;value.session=3;return value;}
    ipc::event_record topic(unsigned value)
    {ipc::event_record record{};record.kind=ipc::event_kind::topic_message;record.payload=std::vector<std::uint8_t>(16,static_cast<std::uint8_t>(value));return record;}
    void queue()
    {
        unsigned operations{},samples{};
        for(auto policy : {ipc::event_overflow::reject,ipc::event_overflow::drop_oldest,ipc::event_overflow::drop_newest,ipc::event_overflow::backpressure})
        {
            std::string_view name=policy==ipc::event_overflow::reject?"reject":policy==ipc::event_overflow::drop_oldest?"drop-oldest":policy==ipc::event_overflow::drop_newest?"drop-newest":"backpressure";
            ipc::event_stream_settings settings{4,512,128,policy==ipc::event_overflow::backpressure?1u:0u,policy==ipc::event_overflow::backpressure?128u:0u,policy};
            ipc::event_stream stream{settings,binding(),ipc::event_bit(ipc::event_kind::topic_message)|ipc::event_bit(ipc::event_kind::resource_error)|ipc::event_bit(ipc::event_kind::resource_closed)};
            require(stream.configured());snapshot(name,"empty",stream.statistics());++samples;
            require(stream.enqueue(topic(1))==0&&stream.enqueue(topic(2))==0);operations+=2;auto full=stream.statistics();
            require(full.depth==2&&full.high_water_depth==2&&full.bytes>0&&full.enqueued==2);snapshot(name,"full",full);++samples;
            auto overflow=stream.enqueue(topic(3));++operations;
            require(overflow==(policy==ipc::event_overflow::drop_oldest||policy==ipc::event_overflow::backpressure?0u:11u));
            auto after=stream.statistics();require(after.depth==2&&after.pending_depth==(policy==ipc::event_overflow::backpressure?1u:0u));snapshot(name,"overflow",after);++samples;
            if(policy==ipc::event_overflow::backpressure){require(stream.enqueue(topic(4))==11);++operations;auto next=stream.statistics();require(next.pending_depth==1&&next.backpressured==2&&next.admission_refused==1);snapshot(name,"pending-refusal",next);++samples;}
            auto retained=stream.statistics();auto small=stream.poll(79);require(small.error==90&&small.payload.size()==8&&ipc::get(small.payload.data(),8)==80);
            auto unchanged=stream.statistics();require(unchanged.depth==retained.depth&&unchanged.bytes==retained.bytes&&unchanged.dequeued==retained.dequeued);++operations;
            unsigned delivered{};for(;;){auto value=stream.poll(128);++operations;if(value.error==11)break;require(value.error==0&&value.payload.size()==80);ipc::event_header header{};require(ipc::decode_event(value.payload.data(),value.payload.size(),header)&&header.source_handle==1&&header.instance==2&&header.session==3&&header.sequence==delivered+1);++delivered;}
            require(delivered==(policy==ipc::event_overflow::backpressure?3u:2u));snapshot(name,"drained",stream.statistics());++samples;
            require(stream.enqueue(topic(5))==0);++operations;stream.seal(125);auto closed=stream.statistics();require(closed.depth==2&&closed.pending_depth==0&&closed.reserved_depth==0&&closed.closed==1);snapshot(name,"sealed",closed,false);++samples;
            for(auto kind:{ipc::event_kind::resource_error,ipc::event_kind::resource_closed}){auto value=stream.poll(128);ipc::event_header header{};require(value.error==0&&ipc::decode_event(value.payload.data(),value.payload.size(),header)&&header.kind==kind&&header.payload_bytes==0&&(header.flags&ipc::event_source_retired));++operations;}
            require(stream.poll(128).error==11&&stream.enqueue(topic(6))==125);operations+=2;
        }
        // Native async credit: admission precedes any modeled service dispatch.
        // These are real queue storage operations, not a service execution claim.
        ipc::event_stream stream{{4,512,128,0,0,ipc::event_overflow::reject},binding(),127};
        require(stream.reserve(7,96,32)==0);auto reserved=stream.statistics();require(reserved.reserved_depth==1&&reserved.reserved_bytes==128);snapshot("reserved-credit","reserved",reserved,false);++samples;
        ipc::event_record response{ipc::event_kind::service_response,0,0,7,std::vector<std::uint8_t>(8,0)};ipc::event_put(response.payload.data(),7,8);
        require(stream.complete(7,std::move(response))==0);auto complete=stream.statistics();require(complete.reserved_depth==1&&complete.completed==1);snapshot("reserved-credit","completed",complete,false);++samples;
        require(stream.poll(128).error==0&&stream.statistics().reserved_depth==0);operations+=3;
        // The legacy per-source queue is sampled independently of the shared
        // observer stream. Its byte accounting excludes the64B envelope and
        // must never be reported as the observer's occupancy.
        for(std::string_view policy:{"reject","drop-oldest","drop-newest","backpressure"})
        {
            uwvm2::imported::ros::capability_spec cap;cap.alias="sub";cap.operation="topic.subscribe";cap.resource="/metrics/topic";
            cap.type="std_msgs/msg/String";cap.type_identity="RIHS01_df668c740482bbd48fb39d76a70dfd4bd59db1288021743503259e948f6b1a18";
            cap.limits={{"max-message-bytes","16"},{"max-rate-hz","1000000"},{"burst","64"},{"max-bytes-per-second","65536"},
                {"max-queue-bytes","32"},{"qos.depth","2"},{"overflow-policy",std::string{policy}},
                {"max-pending-event-bytes",policy=="backpressure"?"16":"0"},{"validator","none"}};
            gw::event_queue source{cap};auto body=[](){std::vector<std::uint8_t> value(16,'a');ipc::event_put(value.data(),12,4);return value;};
            require(source.enqueue(body())&&source.enqueue(body()));operations+=2;
            auto full=source.statistics();require(full.depth==2&&full.bytes>=32&&full.high_water_depth==2);snapshot("legacy-source",policy,full);++samples;
            auto extra=source.enqueue(body());require(extra==(policy=="drop-oldest"||policy=="backpressure"));++operations;
            auto overflow=source.statistics();require(overflow.depth==2&&overflow.pending_depth==(policy=="backpressure"?1u:0u));snapshot("legacy-source","overflow",overflow);++samples;
            unsigned delivered{};for(;;){auto value=source.poll();++operations;if(value.error==11)break;require(!value.error&&value.payload.size()==16&&value.payload[0]==12);++delivered;}
            require(delivered==(policy=="backpressure"?3u:2u));snapshot("legacy-source","drained",source.statistics());++samples;
            source.deactivate();require(source.poll().error==125&&source.statistics().bytes==0);++operations;
        }
        fast_io::println("PASS_NATIVE_QUEUE_METRICS operations=",operations," samples=",samples," configured_depth=4 configured_bytes=512 reserved_control=2 pending_max=1 actual_occupancy=1");
    }
    struct peer_owner
    {
        pid_t pid{};bool reaped{};~peer_owner(){if(pid>0&&!reaped){::kill(pid,SIGKILL);int status{};while(::waitpid(pid,&status,0)<0&&errno==EINTR){}}}
    };
    std::uint64_t cpu_us(rusage const& usage)
    {return static_cast<std::uint64_t>(usage.ru_utime.tv_sec+usage.ru_stime.tv_sec)*1000000+usage.ru_utime.tv_usec+usage.ru_stime.tv_usec;}
    void isolated_ipc()
    {
        for(unsigned bytes:{64u,4096u})
        {
            int pair[2];require(::socketpair(AF_UNIX,SOCK_STREAM|SOCK_CLOEXEC|SOCK_NONBLOCK,0,pair)==0);peer_owner peer;peer.pid=::fork();require(peer.pid>=0);
            if(peer.pid==0)
            {
                ::close(pair[0]);try{for(unsigned index=1;index<=1000;++index){ipc::frame request;require(ipc::read_frame(pair[1],request,ipc::after(2000))&&request.request_id==index&&request.session_id==7&&request.payload==std::vector<std::uint8_t>(bytes,0x5a));ipc::frame reply=request;reply.type=ipc::kind::response;reply.response_id=index;reply.request_id=0;require(ipc::write_frame(pair[1],reply,ipc::after(2000)));}::close(pair[1]);::_exit(0);}catch(...){::_exit(1);}
            }
            ::close(pair[1]);std::array<std::uint64_t,1000> samples{};rusage before{},after{};require(::getrusage(RUSAGE_SELF,&before)==0);auto began=clock::now();
            for(unsigned index=1;index<=1000;++index)
            {
                ipc::frame request;request.backend=2;request.request_id=index;request.session_id=7;request.timeout_ms=2000;request.payload=std::vector<std::uint8_t>(bytes,0x5a);
                auto start=clock::now();require(ipc::write_frame(pair[0],request,ipc::after(2000)));ipc::frame reply;require(ipc::read_frame(pair[0],reply,ipc::after(2000))&&reply.type==ipc::kind::response&&reply.response_id==index&&reply.request_id==0&&reply.session_id==7&&reply.payload==request.payload);samples[index-1]=nanoseconds(clock::now()-start);
            }
            auto elapsed=nanoseconds(clock::now()-began);require(::getrusage(RUSAGE_SELF,&after)==0);::close(pair[0]);int status{};require(::waitpid(peer.pid,&status,0)==peer.pid&&WIFEXITED(status)&&WEXITSTATUS(status)==0);peer.reaped=true;
            std::sort(samples.begin(),samples.end());
            fast_io::println("{\"kind\":\"isolated-socketpair-frame-ipc\",\"simulation\":\"qemu-aarch64\",\"completed\":1000,\"concurrency\":1,\"payload_bytes\":",bytes,
                ",\"header_bytes\":48,\"elapsed_ns\":",elapsed,",\"latency_ns_p50\":",samples[500],",\"latency_ns_p95\":",samples[950],",\"latency_ns_max\":",samples.back(),
                ",\"parent_cpu_us\":",cpu_us(after)-cpu_us(before),",\"parent_max_rss_kib\":",after.ru_maxrss,",\"voluntary_switches\":",after.ru_nvcsw-before.ru_nvcsw,
                ",\"involuntary_switches\":",after.ru_nivcsw-before.ru_nivcsw,",\"gateway_authentication_measured\":false,\"native_ros_measured\":false,\"permission_check_measured\":false}");
        }
        fast_io::println("PASS_ISOLATED_IPC records=2000 payload_sizes=64,4096 concurrency=1 authentication=not-measured");
    }
}
int main(int argc,char** argv)
{
    try{metrics::require(argc==2);metrics::input();if(std::string_view{argv[1]}=="queue")metrics::queue();else{metrics::require(std::string_view{argv[1]}=="ipc");metrics::isolated_ipc();}return 0;}
    catch(...){fast_io::println(fast_io::err(),"FAIL_NATIVE_QUEUE_IPC_METRICS");return 1;}
}
