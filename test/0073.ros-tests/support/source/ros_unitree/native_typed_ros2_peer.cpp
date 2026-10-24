/** @file Independent native ROS2 witness for the fourteen typed Guest examples.
 *
 * This process uses the actual generated GO/HG/API ROS messages and rclcpp.
 * It is a SIM-only fixture: it never opens LowCmd, a firmware service, an
 * action, a physical interface, or a robot address. Root must first qualify
 * the compiler, complete -MD/link closure, native identities, loaded files,
 * isolated loopback Cyclone profile and original 64 GiB cgroup. Merely building
 * this file proves no DDS delivery or absence of native effects.
 *
 * stdin starts with 16 bytes: "UTN1", familyLE32 (1 GO,2 HG,3 API),
 * positiveLE32 (0 denied,1 explicit positive), reservedLE32=0. Request/API ids
 * are exactly1. Further control bytes are M (match), Q (two-second denied
 * quiet window), P (publish/independently receive LowState), A (receive Request,
 * publish/independently receive Response), Z (wait for Guest endpoint removal
 * and observe two seconds of quiet), F (finish). Each command is single-use.
 * Parent must keep the process held until its actual mapped files are pinned.
 *
 * Native self-observation uses real subscriptions with intra-process delivery
 * disabled. Full generated message equality is checked before emitting scalar
 * observations. That is an independent native transport witness; the Guest
 * must separately return its copied observations for Root to compare. The
 * deliberately selected CRC is an observed message field, not a claim of
 * official CRC authentication or physics. The separate oracle tests cover CRC.
 *
 * All records fit4096 bytes and use decimal integer words. Under C++20 the
 * formatter is fast_io; C++17 uses the existing locale-free fixed_text shim
 * because the vendored fast_io interface requires concepts. Nonblocking pipes,
 * finite poll/spin budgets and owned parent cleanup bound control I/O. No
 * background executor, timer, callback thread or implicit retry is introduced.
 */
#include <rclcpp/rclcpp.hpp>
#include <unitree_go/msg/low_state.hpp>
#include <unitree_hg/msg/low_state.hpp>
#include <unitree_api/msg/request.hpp>
#include <unitree_api/msg/response.hpp>
#include <src/ros/gateway/common/native_io.h>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <csignal>
#include <cerrno>
#include <fcntl.h>
#include <memory>
#include <poll.h>
#include <string>
#include <string_view>
#include <type_traits>
#include <unistd.h>
#if __cplusplus >= 202002L && defined(__cpp_concepts)
#include <fast_io.h>
#include <fast_io_unit/string.h>
#endif

namespace unitree_typed_peer {
using Clock=std::chrono::steady_clock;
namespace io=uwvm2::ros_gateway::native_io;
struct Failure { std::uint32_t code; };
void require(bool value,std::uint32_t code=22) { if(!value)throw Failure{code}; }
bool equal(char const* value,std::string_view expected) { return value&&std::string_view{value}==expected; }
std::uint32_t le32(unsigned char const* p) {
    return std::uint32_t(p[0])|(std::uint32_t(p[1])<<8)|(std::uint32_t(p[2])<<16)|(std::uint32_t(p[3])<<24);
}
std::uint32_t bits(float value) { std::uint32_t word;std::memcpy(&word,&value,sizeof word);return word; }

/** The trusted parent supplies and independently pins the loopback XML.
 * This executable has no CLI address or domain override. Native ROS authority
 * is not Guest capability enforcement; both witnesses are needed by the test.
 */
void environment() {
    require(equal(::getenv("ROS_DOMAIN_ID"),"1"));
    require(equal(::getenv("RMW_IMPLEMENTATION"),"rmw_cyclonedds_cpp"));
    auto xml=::getenv("CYCLONEDDS_URI");require(xml&&*xml);
    require(::getenv("ROS_NAMESPACE")==nullptr&&::getenv("LD_PRELOAD")==nullptr);
    require(::signal(SIGPIPE,SIG_IGN)!=SIG_ERR,107);
    for(int fd:{STDIN_FILENO,STDOUT_FILENO}) {
        auto flags=::fcntl(fd,F_GETFL,0);
        require(flags>=0&&::fcntl(fd,F_SETFL,flags|O_NONBLOCK)==0,107);
    }
}

/** Control framing only: reads are finite and cannot allocate from input. */
void input(void* destination,std::size_t bytes) {
    require(bytes>0&&bytes<=16);std::size_t used=0;unsigned attempts=0;
    auto until=Clock::now()+std::chrono::seconds{30};
    while(used<bytes) {
        require(++attempts<=128&&Clock::now()<until,110);
        auto ms=std::chrono::duration_cast<std::chrono::milliseconds>(until-Clock::now()).count();
        pollfd p{STDIN_FILENO,POLLIN,0};auto ready=::poll(&p,1,static_cast<int>(ms>0?ms:0));
        if(ready<0&&errno==EINTR)continue;
        require(ready>=0,107);require(ready>0,110);require(!(p.revents&(POLLERR|POLLNVAL)),107);
        auto count=::read(STDIN_FILENO,static_cast<unsigned char*>(destination)+used,bytes-used);
        if(count<0&&(errno==EAGAIN||errno==EWOULDBLOCK||errno==EINTR))continue;
        require(count>0&&static_cast<std::size_t>(count)<=bytes-used,107);used+=static_cast<std::size_t>(count);
    }
}

class Text {
#if __cplusplus >= 202002L && defined(__cpp_concepts)
    std::string value_;
#else
    io::fixed_text<4096> value_;
    template<class T> void one(T const& value) {
        if constexpr(std::is_integral_v<T>)require(value_.number(value),90);
        else require(value_.append(std::string_view{value}),90);
    }
#endif
public:
    Text() {
#if __cplusplus >= 202002L && defined(__cpp_concepts)
        value_.reserve(2048);
#endif
    }
    template<class... T> void append(T const&... value) {
#if __cplusplus >= 202002L && defined(__cpp_concepts)
        fast_io::ostring_ref_std sink{std::addressof(value_)};fast_io::print(sink,value...);
#else
        (one(value),...);
#endif
    }
    std::string_view view() const {
#if __cplusplus >= 202002L && defined(__cpp_concepts)
        return value_;
#else
        return value_.view();
#endif
    }
};
void output(std::string_view text) {
    require(!text.empty()&&text.size()<=4096,90);auto until=Clock::now()+std::chrono::seconds{1};unsigned attempts=0;
    for(;;) {
        require(++attempts<=128&&Clock::now()<until,110);
        auto ms=std::chrono::duration_cast<std::chrono::milliseconds>(until-Clock::now()).count();
        pollfd p{STDOUT_FILENO,POLLOUT,0};auto ready=::poll(&p,1,static_cast<int>(ms>0?ms:0));
        if(ready<0&&errno==EINTR)continue;
        require(ready>0&&(p.revents&POLLOUT)&&!(p.revents&(POLLERR|POLLHUP|POLLNVAL)),107);
        require(io::write_all(STDOUT_FILENO,text),107);return;
    }
}
template<class... T> void line(T const&... value) { Text text;text.append(value...);output(text.view()); }

/** Expected carrier values are exactly representable binary32 fractions.
 * The complete default-generated value, including every unmodified nested
 * field, remains part of native equality. No hand-written wire codec is used.
 */
template<class State> State selected_state(unsigned count) {
    State value{};require(value.motor_state.size()==count);
    value.tick=42;value.crc=0x13572468;
    for(unsigned i=0;i<count;++i) {
        auto& motor=value.motor_state[i];motor.q=static_cast<float>(i+1)/32;
        motor.dq=-static_cast<float>(i+1)/64;motor.tau_est=static_cast<float>(i+1)/128;
    }
    return value;
}

class Witness {
    std::shared_ptr<rclcpp::Node> node_;
    rclcpp::executors::SingleThreadedExecutor& executor_;
    unsigned family_,positive_,states_=0,requests_=0,responses_=0,published_=0;
    bool matched_=false,quiet_=false,closed_quiet_=false,operation_=false;
    rclcpp::Publisher<unitree_go::msg::LowState>::SharedPtr go_pub_;
    rclcpp::Publisher<unitree_hg::msg::LowState>::SharedPtr hg_pub_;
    rclcpp::Publisher<unitree_api::msg::Response>::SharedPtr response_pub_;
    rclcpp::Subscription<unitree_go::msg::LowState>::SharedPtr go_sub_;
    rclcpp::Subscription<unitree_hg::msg::LowState>::SharedPtr hg_sub_;
    rclcpp::Subscription<unitree_api::msg::Request>::SharedPtr request_sub_;
    rclcpp::Subscription<unitree_api::msg::Response>::SharedPtr response_sub_;
    unitree_go::msg::LowState go_=selected_state<unitree_go::msg::LowState>(20);
    unitree_hg::msg::LowState hg_=selected_state<unitree_hg::msg::LowState>(35);
    unitree_api::msg::Request request_{};
    unitree_api::msg::Response response_{};
    static constexpr char state_topic[]="/uwvm_unitree_typed/lowstate";
    static constexpr char request_topic[]="/uwvm_unitree_typed/request";
    static constexpr char response_topic[]="/uwvm_unitree_typed/response";

    void spin() { executor_.spin_once(std::chrono::milliseconds{1}); }
    /** Endpoint counts are observed on the actual isolated native graph.
     * One native self-observer remains throughout. Guest endpoints are the
     * only allowed count increment, and must disappear after Guest close.
     */
    bool graph(unsigned guest,bool request_active=true) {
        require(node_->count_publishers("/lowcmd")==0&&node_->count_publishers("/rt/lowcmd")==0);
        require(node_->get_service_names_and_types().empty());
        if(family_==3) {
            return node_->count_publishers(request_topic)==(request_active?guest:0)&&node_->count_subscribers(request_topic)==1&&
                node_->count_publishers(response_topic)==1&&node_->count_subscribers(response_topic)==1+guest&&
                response_pub_->get_subscription_count()==1+guest&&
                request_sub_->get_publisher_count()==(request_active?guest:0);
        }
        auto matched=family_==1?go_pub_->get_subscription_count():hg_pub_->get_subscription_count();
        return node_->count_publishers(state_topic)==1&&node_->count_subscribers(state_topic)==1+guest&&matched==1+guest;
    }
    template<class Predicate> void wait(Predicate predicate) {
        auto until=Clock::now()+std::chrono::seconds{10};unsigned polls=0;
        while(!predicate()) { require(++polls<=16384&&Clock::now()<until,110);spin(); }
    }
    /** Every quiet window spans at least two seconds and thirty real spins.
     * Payload counters and native graph are checked on every sample. Parent
     * independently checks Guest results, mapped identities and owned cleanup.
     */
    void quiet_window() {
        auto before=Clock::now();unsigned samples=0;
        auto states=states_,requests=requests_,responses=responses_;
        do {
            require(samples<16384,110);spin();++samples;require(graph(0));
            require(states_==states&&requests_==requests&&responses_==responses);
        } while(Clock::now()-before<std::chrono::seconds{2}||samples<30);
        auto ns=std::chrono::duration_cast<std::chrono::nanoseconds>(Clock::now()-before).count();
        line("NATIVE_TYPED_QUIET ",family_," ",samples," ",ns," ",states_," ",requests_," ",responses_,"\n");
    }
    template<class State> void state_record(State const& value) {
        Text text;text.append("NATIVE_TYPED_STATE ",family_," ",value.tick," ",value.crc," ",value.motor_state.size());
        for(auto const& motor:value.motor_state) {
            require(std::isfinite(motor.q)&&std::isfinite(motor.dq)&&std::isfinite(motor.tau_est));
            text.append(" ",bits(motor.q)," ",bits(motor.dq)," ",bits(motor.tau_est));
        }
        text.append("\n");output(text.view());
    }
public:
    Witness(std::shared_ptr<rclcpp::Node> node,rclcpp::executors::SingleThreadedExecutor& executor,unsigned family,unsigned positive):
        node_{std::move(node)},executor_{executor},family_{family},positive_{positive} {
        request_.header.identity.id=1;request_.header.identity.api_id=1;request_.parameter="{}";
        response_.header.identity.id=1;response_.header.identity.api_id=1;
        response_.header.status.code=0;response_.data="typed-example-ack";
        auto qos=rclcpp::QoS{rclcpp::KeepLast{1}}.reliable().durability_volatile();
        if(family_==1) {
            go_pub_=node_->create_publisher<unitree_go::msg::LowState>(state_topic,qos);
            go_sub_=node_->create_subscription<unitree_go::msg::LowState>(state_topic,qos,[this](unitree_go::msg::LowState::ConstSharedPtr value) {
                require(operation_&&positive_&&value&&states_==0&&*value==go_);++states_;
            });
        } else if(family_==2) {
            hg_pub_=node_->create_publisher<unitree_hg::msg::LowState>(state_topic,qos);
            hg_sub_=node_->create_subscription<unitree_hg::msg::LowState>(state_topic,qos,[this](unitree_hg::msg::LowState::ConstSharedPtr value) {
                require(operation_&&positive_&&value&&states_==0&&*value==hg_);++states_;
            });
        } else {
            response_pub_=node_->create_publisher<unitree_api::msg::Response>(response_topic,qos);
            request_sub_=node_->create_subscription<unitree_api::msg::Request>(request_topic,qos,[this](unitree_api::msg::Request::ConstSharedPtr value) {
                require(matched_&&positive_&&value&&requests_==0&&*value==request_);++requests_;
            });
            response_sub_=node_->create_subscription<unitree_api::msg::Response>(response_topic,qos,[this](unitree_api::msg::Response::ConstSharedPtr value) {
                require(operation_&&positive_&&value&&responses_==0&&*value==response_);++responses_;
            });
        }
    }
    bool command(unsigned char command) {
        switch(command) {
        case 'M':
            // Capability-open is local: the Gateway creates a publisher only
            // on its first real publication. Require the already activated
            // Response subscription now; verify the actual Request publisher
            // only after the Guest has submitted. Waiting for it here would
            // deadlock the finite, once-only publication protocol.
            require(!matched_);wait([&]{return graph(positive_,false);});matched_=true;
            line("NATIVE_TYPED_MATCHED ",family_," ",positive_,"\n");return true;
        case 'Q':
            require(!positive_&&matched_&&!quiet_&&!operation_);quiet_window();quiet_=true;return true;
        case 'P':
            require(positive_&&matched_&&!operation_&&family_!=3);operation_=true;
            if(family_==1)go_pub_->publish(go_);else hg_pub_->publish(hg_);++published_;
            wait([&]{return states_==1;});if(family_==1)state_record(go_);else state_record(hg_);return true;
        case 'A':
            require(positive_&&matched_&&!operation_&&family_==3);operation_=true;
            wait([&]{return requests_==1&&graph(1);});response_pub_->publish(response_);++published_;
            wait([&]{return responses_==1;});line("NATIVE_TYPED_RPC 1 1 0 typed-example-ack\n");return true;
        case 'Z':
            require(positive_&&operation_&&!closed_quiet_);wait([&]{return graph(0);});quiet_window();closed_quiet_=true;return true;
        case 'F':
            require(matched_&&(positive_?closed_quiet_:quiet_));
            require(positive_?(published_==1&&(family_==3?(requests_==1&&responses_==1&&states_==0):(states_==1&&requests_==0&&responses_==0))):
                (published_==0&&states_==0&&requests_==0&&responses_==0));
            line("PASS_NATIVE_TYPED_WITNESS ",family_," ",positive_,"\n");return false;
        default:throw Failure{22};
        }
    }
};
}

int main(int argc,char**) {
    using namespace unitree_typed_peer;
    std::shared_ptr<rclcpp::Context> context;std::uint32_t status=0;
    try {
        require(argc==1);environment();unsigned char setup[16];input(setup,sizeof setup);
        auto family=le32(setup+4),positive=le32(setup+8);
        require(!std::memcmp(setup,"UTN1",4)&&family>=1&&family<=3&&positive<=1&&le32(setup+12)==0);
        context=std::make_shared<rclcpp::Context>();rclcpp::InitOptions init;context->init(0,nullptr,init);
        {
            rclcpp::NodeOptions options;options.context(context).use_global_arguments(false).enable_rosout(false);
            options.start_parameter_services(false).start_parameter_event_publisher(false).use_intra_process_comms(false);
            options.parameter_overrides({rclcpp::Parameter("start_type_description_service",false)});
            auto node=std::make_shared<rclcpp::Node>("uwvm_unitree_typed_native_peer","/uwvm_unitree",options);
            rclcpp::ExecutorOptions eo;eo.context=context;rclcpp::executors::SingleThreadedExecutor executor{eo};executor.add_node(node);
            {
                Witness witness{node,executor,family,positive};line("READY_NATIVE_TYPED_V1 ",family," ",positive,"\n");
                for(unsigned commands=0;;++commands) {
                    require(commands<8,110);unsigned char op;input(&op,1);if(!witness.command(op))break;
                }
            }
            executor.remove_node(node);
        }
    } catch(Failure const& failure) { status=failure.code; }
      catch(...) { status=22; }
    if(context&&context->is_valid())try { require(context->shutdown("typed native fixture complete")); }catch(...) { if(!status)status=22; }
    if(status)try { line("FAIL_NATIVE_TYPED_WITNESS ",status,"\n"); }catch(...) {}
    return status?1:0;
}
