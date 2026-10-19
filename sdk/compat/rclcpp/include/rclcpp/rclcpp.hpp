/** Finite SOURCE adapter for two exact official Jazzy minimal String examples.
 * No native threads, rclcpp C++ binary ABI, generic RMW, hidden grant or plugin.
 * Unsupported APIs are absent; unsupported runtime input fails explicitly.
 * All actual topic/time/log operations use the existing checked ROS Wasm SDK.
 */
#pragma once
#include <roswasm_rclcpp/selection.hpp>
#include <std_msgs/msg/string.hpp>
#include <chrono>
#include <array>
#include <fast_io.h> // Explicit current vendored C++20 SDK dependency.
#include <cstdlib>
#include <functional>
#include <limits>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>
namespace roswasmcpp {
template<> struct Codec<::std_msgs::msg::String> {
    static bool encode(::std_msgs::msg::String const& value,wire::bytes& out){return Codec<msg::String>::encode(msg::String{value.data},out);}
    static bool decode(std::span<unsigned char const> bytes,::std_msgs::msg::String& out){msg::String next;if(!Codec<msg::String>::decode(bytes,next))return false;out.data=std::move(next.data);return true;}
};
}
namespace rclcpp {
namespace exceptions {
class RCLError:public std::runtime_error {
public:std::uint32_t code;
    explicit RCLError(std::uint32_t value):std::runtime_error("finite rclcpp adapter operation refused"),code{value}{}
};
}
namespace detail {
struct Core;
struct State {
    roswasm_rclcpp::Selection const* selection{};std::unique_ptr<roswasmcpp::Context> context;
    std::uint32_t last_error{},cleanup_error{};std::size_t nodes{};bool initialized{},spinning{},service_dispatching{};
    // Fixed weak registry only for the explicit Service profile. A callback
    // lease snapshots strong facade owners before it consumes any event.
    std::array<std::weak_ptr<Core>,16> service_cores{};
};
inline State& state(){static State value;return value;}
[[noreturn]] inline void fail(std::uint32_t code){
    state().last_error=code?code:22;
#if defined(__cpp_exceptions)
    throw exceptions::RCLError{state().last_error};
#else
    // Original uncaught-error main cannot continue. The external owner must
    // inspect this error and prove Host/module resource drain after a trap.
    std::abort();
#endif
}
inline void checked(std::uint32_t code){if(code)fail(code);}
inline void cleanup(std::uint32_t code)noexcept{if(code&&!state().cleanup_error)state().cleanup_error=code;}
inline bool text(std::string_view value,std::size_t maximum){return !value.empty()&&value.size()<=maximum&&value.find('\0')==value.npos;}
inline void running(){if(!state().initialized||!state().context||!state().context->running())fail(125);}
struct Entity {
    virtual std::uint32_t poll(std::uint64_t)=0;virtual std::uint32_t close()noexcept=0;
    virtual ~Entity()=default;
};
struct Core {
    std::string node;roswasm_rclcpp::NodeBinding binding;roswasmc_context context_;
    std::vector<std::weak_ptr<Entity>> entities;std::unique_ptr<roswasmcpp::Clock> clock;
    std::unique_ptr<roswasmcpp::Logger> logger;bool dispatching{},closed{};
    Core(std::string_view name,roswasm_rclcpp::NodeBinding row):node{name},binding{row},context_{&state().context->backend()}{if(state().nodes>=16)fail(90);++state().nodes;}
    roswasmc_context const& context()const{return context_;}
    std::uint64_t now(){
        running();if(!clock){if(state().service_dispatching)fail(17);if(!text(binding.steady_clock_alias,64))fail(13);clock=std::make_unique<roswasmcpp::Clock>(context(),binding.steady_clock_alias,ROSWASMC_TIME_READ_STEADY);checked(clock->error());}
        std::uint64_t value{};checked(clock->now(value));return value;
    }
    void prepare_factory(){if(dispatching||closed||state().service_dispatching)fail(17);entities.reserve(64);std::erase_if(entities,[](auto const& value){return value.expired();});if(entities.size()>=64)fail(90);}
    void add(std::shared_ptr<Entity>const& owner){if(dispatching||closed)fail(17);entities.emplace_back(owner);}
    void close()noexcept{
        if(closed)return;if(state().service_dispatching){cleanup(17);return;}closed=true;
        for(auto i=entities.rbegin();i!=entities.rend();++i)if(auto owner=i->lock())cleanup(owner->close());
        entities.clear();if(logger){if(logger->is_open())cleanup(logger->close());logger.reset();}if(clock){if(clock->is_open())cleanup(clock->close());clock.reset();}
    }
    ~Core(){close();--state().nodes;}
};
inline roswasm_rclcpp::TopicBinding const& topic(Core& core,std::string_view name,std::size_t depth,roswasm_rclcpp::Role role){
    running();core.prepare_factory();roswasm_rclcpp::TopicBinding const* selected{};
    for(auto const& row:state().selection->topics)if(row.node==core.node&&row.topic==name&&row.role==role){if(selected)fail(22);selected=&row;}
    if(!selected)fail(13);if(selected->keep_last_depth!=depth)fail(95);return *selected;
}
}
/** Native rclcpp accepts many options. This finite profile accepts only the
 * unchanged example's argc0/1, never silently ignores ROS remaps/extra options.
 */
inline void init(int argc,char const*const* argv){
    auto& s=detail::state();if(s.initialized||s.nodes||s.spinning)detail::fail(17);
    if(argc<0||argc>1||(argc&&(!argv||!argv[0])))detail::fail(22);
    auto selected=roswasm_rclcpp::selected_configuration();if(!selected)detail::fail(13);
    if(!selected->backend||selected->backend->version!=2||!selected->backend->abi_version||
       !selected->step||!selected->maximum_spin_steps||selected->maximum_spin_steps>4096||
       selected->topics.size()>32||selected->nodes.empty()||selected->nodes.size()>16)detail::fail(22);
    for(std::size_t i{};i!=selected->nodes.size();++i){auto const& n=selected->nodes[i];
        if(!detail::text(n.node,128)||(!n.steady_clock_alias.empty()&&!detail::text(n.steady_clock_alias,64))||(!n.info_logger_alias.empty()&&!detail::text(n.info_logger_alias,64)))detail::fail(22);
        for(std::size_t j{};j!=i;++j)if(selected->nodes[j].node==n.node)detail::fail(22);
    }
    for(std::size_t i{};i!=selected->topics.size();++i){auto const& r=selected->topics[i];
        if(!detail::text(r.node,128)||!detail::text(r.topic,255)||!detail::text(r.alias,64)||
           !r.keep_last_depth||r.keep_last_depth>4096||r.maximum_wire_bytes<4||r.maximum_wire_bytes>32768||
           (r.role!=roswasm_rclcpp::Role::publish&&r.role!=roswasm_rclcpp::Role::subscribe))detail::fail(22);
        bool found{};for(auto const& n:selected->nodes)found=found||r.node==n.node;if(!found)detail::fail(22);
        for(std::size_t j{};j!=i;++j){auto const& p=selected->topics[j];if(p.node==r.node&&p.topic==r.topic&&p.role==r.role)detail::fail(22);}
    }
    if(selected->backend->abi_version()!=1)detail::fail(95);
    s.selection=selected;s.context=std::make_unique<roswasmcpp::Context>(*selected->backend);s.last_error=0;s.cleanup_error=0;s.initialized=true;
}
inline void init(int argc,char**argv){init(argc,const_cast<char const*const*>(argv));}
inline bool ok(){auto const&s=detail::state();return s.initialized&&s.context&&s.context->running();}
inline void shutdown(){auto&s=detail::state();if(s.service_dispatching)detail::fail(17);if(s.context)s.context->shutdown();s.initialized=false;}
class Logger {
    std::weak_ptr<detail::Core> core_;
public:
    explicit Logger(std::shared_ptr<detail::Core>const& core):core_{core}{}
    void info(char const* format,std::int64_t a,std::int64_t b)const;
    void info(char const* format,std::int64_t a,std::int64_t b,std::int64_t c)const;
    void info(char const* literal)const;
    void error(char const* literal)const;

    void info(char const* format,char const* text)const{
        auto core=core_.lock();if(!core||core->closed)detail::fail(9);detail::running();if(!format||!text)detail::fail(22);
        // Real libc formatting follows a finite checked format grammar: one
        // %s and optional %% only. No %n, width, arbitrary varargs or truncation.
        std::size_t size{},conversions{};for(;format[size];++size){if(size>=255)detail::fail(90);if(format[size]=='%'){char next=format[++size];if(next=='s')++conversions;else if(next!='%')detail::fail(95);}}
        if(conversions!=1)detail::fail(95);std::size_t length{};while(text[length]){if(length++>=32764)detail::fail(90);}
        std::size_t expected{};
        for(std::size_t i{};i!=size;++i){std::size_t piece{1};if(format[i]=='%'){char next=format[++i];piece=next=='s'?length:1;}if(piece>4095-expected)detail::fail(90);expected+=piece;}
        // Only memory output: validated literal runs, one complete argument
        // and escaped percent. No varargs/printf parser or truncating buffer.
        std::string rendered;rendered.reserve(expected);::fast_io::ostring_ref_std sink{std::addressof(rendered)};
        std::size_t start{};
        for(std::size_t i{};i!=size;++i){if(format[i]!='%')continue;
            if(i!=start)::fast_io::print(sink,std::string_view{format+start,i-start});
            if(format[++i]=='s')::fast_io::print(sink,std::string_view{text,length});
            else ::fast_io::print(sink,::fast_io::mnp::chvw('%'));start=i+1;
        }
        if(start!=size)::fast_io::print(sink,std::string_view{format+start,size-start});
        if(rendered.size()!=expected)detail::fail(22);
        if(!core->logger){if(detail::state().service_dispatching)detail::fail(17);if(!detail::text(core->binding.info_logger_alias,64))detail::fail(13);core->logger=std::make_unique<roswasmcpp::Logger>(core->context(),core->binding.info_logger_alias);detail::checked(core->logger->error());}
        detail::checked(core->logger->write(rendered));
    }
};
class TimerBase:public detail::Entity {
    std::uint64_t period_,due_,last_;std::function<void()> callback_;bool cancelled_{},busy_{};
public:
    using SharedPtr=std::shared_ptr<TimerBase>;
    TimerBase(std::uint64_t period,std::uint64_t now,std::function<void()> callback):period_{period},due_{now},last_{now},callback_{std::move(callback)}{
        if(!period||!callback_||period>std::numeric_limits<std::uint64_t>::max()-now)detail::fail(22);due_+=period;
    }
    void cancel()noexcept{cancelled_=true;}
    std::uint32_t close()noexcept override{if(detail::state().service_dispatching)return 17;cancel();return 0;}
    std::uint32_t poll(std::uint64_t now)override{
        if(cancelled_)return 11;if(busy_)return 17;if(now<last_)return 22;last_=now;if(now<due_)return 11;
        // Keep the original fixed-period phase and skip missed periods, as
        // actual rcl_timer_call_with_info does; do not drift to now+period.
        auto steps=(now-due_)/period_+1;if(steps>(std::numeric_limits<std::uint64_t>::max()-due_)/period_)return 90;
        due_+=steps*period_;struct Lease{bool& b;~Lease(){b=false;}}lease{busy_};busy_=true;auto callback=callback_;callback();return 0;
    }
};
template<class Message> class Publisher:public detail::Entity {
    static_assert(std::is_same_v<Message,::std_msgs::msg::String>,"This finite source adapter implements complete String only");
    std::shared_ptr<detail::Core> core_;roswasmcpp::Publisher<Message> publisher_;std::size_t maximum_;bool closed_{};
public:
    using SharedPtr=std::shared_ptr<Publisher<Message>>;
    Publisher(std::shared_ptr<detail::Core> core,std::string_view alias,std::size_t maximum):core_{std::move(core)},publisher_{core_->context(),alias},maximum_{maximum}{detail::checked(publisher_.error());}
    void publish(Message const& value){detail::running();if(closed_)detail::fail(9);if(value.data.size()>maximum_-4)detail::fail(90);detail::checked(publisher_.publish(value));}
    std::uint32_t poll(std::uint64_t)override{return 11;}
    std::uint32_t close()noexcept override{if(detail::state().service_dispatching)return 17;if(closed_)return 0;closed_=true;return publisher_.is_open()?publisher_.close():0;}
    ~Publisher(){detail::cleanup(close());}
};
template<class Message> class Subscription:public detail::Entity {
    static_assert(std::is_same_v<Message,::std_msgs::msg::String>,"This finite source adapter implements complete String only");
    std::shared_ptr<detail::Core> core_;roswasmcpp::Subscription<Message> subscription_;bool closed_{};
public:
    using SharedPtr=std::shared_ptr<Subscription<Message>>;
    Subscription(std::shared_ptr<detail::Core> core,std::string_view alias,std::size_t maximum,std::function<void(Message const&)> callback):core_{std::move(core)},subscription_{core_->context(),alias,maximum}{
        detail::checked(subscription_.error());if(!callback)detail::fail(22);subscription_.on_message(std::move(callback));
    }
    std::uint32_t poll(std::uint64_t)override{return closed_?9:subscription_.poll_once();}
    std::uint32_t close()noexcept override{if(detail::state().service_dispatching)return 17;if(closed_)return 0;auto code=subscription_.is_open()?subscription_.close():0;if(code!=17)closed_=true;return code;}
    ~Subscription(){detail::cleanup(close());}
};
template<class ServiceT> class Service;
template<class ServiceT> class Client;
class Node {
    std::shared_ptr<detail::Core> core_;
public:
    using SharedPtr=std::shared_ptr<Node>;
    static SharedPtr make_shared(std::string const& name){return std::make_shared<Node>(name);}
    template<class ServiceT,class Callback> std::shared_ptr<Service<ServiceT>> create_service(std::string_view name,Callback&& callback);
    template<class ServiceT> std::shared_ptr<Client<ServiceT>> create_client(std::string_view name);
    /** Internal source-adapter owner access, never a native node/Guest ABI handle. */
    std::shared_ptr<detail::Core> _service_core()const{return core_;}
    explicit Node(std::string const& name){
        detail::running();if(detail::state().service_dispatching)detail::fail(17);roswasm_rclcpp::NodeBinding const* selected{};
        for(auto const& row:detail::state().selection->nodes)if(row.node==name)selected=&row;
        if(!selected)detail::fail(13);core_=std::make_shared<detail::Core>(name,*selected);
        if(detail::state().selection->services){bool retained{};for(auto& slot:detail::state().service_cores)if(slot.expired()){slot=core_;retained=true;break;}if(!retained)detail::fail(90);}
    }
    virtual ~Node(){core_->close();}
    Node(Node const&)=delete;Node&operator=(Node const&)=delete;
    Logger get_logger()const{return Logger{core_};}
    template<class Message> typename Publisher<Message>::SharedPtr create_publisher(std::string_view topic,std::size_t depth){
        auto const& row=detail::topic(*core_,topic,depth,roswasm_rclcpp::Role::publish);
        auto value=std::make_shared<Publisher<Message>>(core_,row.alias,row.maximum_wire_bytes);core_->add(value);return value;
    }
    template<class Message,class Callback> typename Subscription<Message>::SharedPtr create_subscription(std::string_view topic,std::size_t depth,Callback&& callback){
        auto const& row=detail::topic(*core_,topic,depth,roswasm_rclcpp::Role::subscribe);
        auto value=std::make_shared<Subscription<Message>>(core_,row.alias,row.maximum_wire_bytes,std::function<void(Message const&)>{std::forward<Callback>(callback)});core_->add(value);return value;
    }
    template<class Rep,class Period,class Callback> TimerBase::SharedPtr create_wall_timer(std::chrono::duration<Rep,Period> period,Callback&& callback){
        detail::running();core_->prepare_factory();
        static_assert(std::is_integral_v<Rep>&&sizeof(Rep)<=8&&std::ratio_equal_v<Period,std::milli>,"Finite official fixture profile supports integral milliseconds only");
        auto count=period.count();if(count<=0)detail::fail(22);auto ticks=static_cast<std::uint64_t>(count);
        if(ticks>static_cast<std::uint64_t>(std::numeric_limits<std::int64_t>::max())/1000000)detail::fail(90);auto ns=ticks*1000000;
        auto timer=std::make_shared<TimerBase>(static_cast<std::uint64_t>(ns),core_->now(),std::function<void()>{std::forward<Callback>(callback)});core_->add(timer);return timer;
    }
    void _spin(){
        auto&s=detail::state();detail::running();if(s.spinning||s.service_dispatching||core_->dispatching||core_->closed)detail::fail(17);
        struct Guard{bool& busy;std::shared_ptr<detail::Core> core;~Guard(){busy=false;core->dispatching=false;core->close();}}guard{s.spinning,core_};s.spinning=true;
        for(std::size_t pass{};pass!=s.selection->maximum_spin_steps;++pass){
            if(!ok())return;auto step=s.selection->step(s.selection->step_context);if(step==125){shutdown();return;}detail::checked(step);
            if(!ok())return;auto now=core_->clock?core_->now():0;
            std::vector<std::shared_ptr<detail::Entity>> owners;for(auto const& weak:core_->entities)if(auto owner=weak.lock())owners.push_back(std::move(owner));
            core_->dispatching=true;for(auto const& owner:owners){if(!ok())break;auto code=owner->poll(now);if(code!=11)detail::checked(code);}core_->dispatching=false;
        }
        detail::fail(110); // A finite preparation/test budget is a failure, never fake normal shutdown.
    }
};
template<class NodeT> void spin(std::shared_ptr<NodeT>const& node){static_assert(std::is_base_of_v<Node,NodeT>);if(!node)detail::fail(22);auto retained=node;retained->_spin();detail::checked(detail::state().cleanup_error);}
}
#define RCLCPP_INFO(logger,...) ((logger).info(__VA_ARGS__))
#define RCLCPP_ERROR(logger,...) ((logger).error(__VA_ARGS__))

/** Optional owned-runner observations: local error/cleanup only, no authority. */
extern "C" inline std::uint32_t roswasm_rclcpp_last_error()noexcept{return rclcpp::detail::state().last_error;}
extern "C" inline std::uint32_t roswasm_rclcpp_cleanup_error()noexcept{return rclcpp::detail::state().cleanup_error;}

// Additive service profile; the original topic selection remains absent by default.
#include "service_profile.hpp"
