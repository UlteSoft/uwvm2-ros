/** Explicit T1 native SDK counterpart to the NEW bridge package, not an
 * arbitrary native ABI exposed to Guest. The SDK's process-wide DDS factory
 * lives only in this separately owned process. Ending the finite lease closes
 * transport owners; it does not claim to cancel motion or restore robot mode.
 */
#include "sdk_conversion.hpp"
#include "report.hpp"
#include <booster/robot/channel/channel_factory.hpp>
#include <booster/robot/channel/channel_publisher.hpp>
#include <booster/robot/channel/channel_subscriber.hpp>
#include <rclcpp/rclcpp.hpp>
#include <chrono>
#include <cstdlib>
#include <memory>
#include <mutex>
#include <string>
#ifndef UWVM_BOOSTER_SDK_BRIDGE_ENABLE
#define UWVM_BOOSTER_SDK_BRIDGE_ENABLE 0
#endif
static_assert(UWVM_BOOSTER_SDK_BRIDGE_ENABLE==0||UWVM_BOOSTER_SDK_BRIDGE_ENABLE==1);
namespace {
using Clock=std::chrono::steady_clock;
struct State {
 std::mutex mutex;bool live{},poisoned{},have_state{};
 std::uint32_t primary{},close_reader{},close_writer{},attempts{},limit{};
 bool reader_attempted{},writer_attempted{};std::uint32_t shutdown_error{};Clock::time_point until{},last_state{};
 rclcpp::Publisher<uwvm_booster::State>::SharedPtr output;
 void fail(std::uint32_t e)noexcept{poisoned=true;if(!primary)primary=e;}
};
bool decimal(char const* p,std::uint32_t max,std::uint32_t& out)noexcept{
 if(!p||!*p)return false;std::uint32_t n{};
 for(;*p;++p){if(*p<'0'||*p>'9')return false;auto d=static_cast<unsigned>(*p-'0');if(n>(max-d)/10u)return false;n=n*10u+d;}
 if(!n||n>max)return false;out=n;return true;
}
}
int main(int argc,char** argv){
 if(!UWVM_BOOSTER_SDK_BRIDGE_ENABLE)return 13;
 // Exact model + explicit native SDK domain/interface + finite budget.
 if(argc!=6||std::string{argv[1]}!="T1"||!argv[3][0])return 22;
 std::uint32_t encoded_domain{},window{},limit{};
 // Domain is encoded plus1 so genuine domain0 is explicitly selectable.
 if(!decimal(argv[2],233,encoded_domain)||!decimal(argv[4],2000,window)||!decimal(argv[5],32,limit))return 22;
 auto state=std::make_shared<State>();state->limit=limit;
 booster::robot::ChannelPublisher<uwvm_booster::NativeCommand> publisher{booster::robot::b1::kTopicJointCtrl,true};
 booster::robot::ChannelSubscriberOptions subscriber_options;
 subscriber_options.reliable=true;subscriber_options.executor_options.queue_capacity=1;
 subscriber_options.executor_options.overflow_policy=booster::common::DdsExecutorOverflowPolicy::kLatestOnly;
 booster::robot::ChannelSubscriber<uwvm_booster::NativeState> subscriber{booster::robot::b1::kTopicLowState,
  [state](void const* p){std::lock_guard lock{state->mutex};if(!state->live||state->poisoned)return;
   try{if(!p){state->fail(22);return;}auto const& value=*static_cast<uwvm_booster::NativeState const*>(p);
    uwvm_booster::State converted{};auto e=uwvm_booster::copy(value,converted);
    if(e||converted.motor_state_serial.size()!=23){state->fail(e?e:22);return;}
    state->output->publish(converted);state->last_state=Clock::now();state->have_state=true;
   }catch(std::bad_alloc const&){state->fail(12);}catch(...){state->fail(5);}},subscriber_options};
 bool initialized{},reader_started{},writer_started{},close_known=true,unresolved_native_create{};
 rclcpp::Subscription<uwvm_booster::Command>::SharedPtr input;
 auto close=[&]()noexcept{
  {std::lock_guard lock{state->mutex};state->live=false;}
  // Called outside the native callback and outside its mutex. Record attempt
  // before entering vendor cleanup; never retry an unknown completion.
  if(reader_started&&!state->reader_attempted){state->reader_attempted=true;try{subscriber.CloseChannel();}catch(...){state->close_reader=5;close_known=false;}}
  if(writer_started&&!state->writer_attempted){state->writer_attempted=true;try{publisher.CloseChannel();}catch(...){state->close_writer=5;close_known=false;}}
 };
 try{
  rclcpp::init(0,nullptr);initialized=true;auto node=std::make_shared<rclcpp::Node>("uwvm_booster_sdk_bridge");
  state->output=node->create_publisher<uwvm_booster::State>("/uwvm_booster_sdk_bridge/T1/state",rclcpp::QoS(1).reliable());
  unresolved_native_create=true;booster::robot::ChannelFactory::Instance()->Init(static_cast<std::int32_t>(encoded_domain-1),argv[3]);unresolved_native_create=false;
  // InitChannel has no result. Retain the owner even when it throws after a
  // partial create, so both real CloseChannel paths are still attempted once.
  writer_started=true;unresolved_native_create=true;publisher.InitChannel();unresolved_native_create=false;reader_started=true;unresolved_native_create=true;subscriber.InitChannel();unresolved_native_create=false;
  input=node->create_subscription<uwvm_booster::Command>("/uwvm_booster_sdk_bridge/T1/command",rclcpp::QoS(1).reliable(),
   [&](uwvm_booster::Command::ConstSharedPtr value){std::lock_guard lock{state->mutex};if(!state->live||state->poisoned)return;
    auto now=Clock::now();if(!value||!state->have_state||now-state->last_state>std::chrono::milliseconds{100}||now>=state->until||state->attempts>=state->limit){state->fail(110);return;}
    uwvm_booster::NativeCommand command;auto e=uwvm_booster::copy(*value,command);
    if(e||command.motor_cmd().size()!=23){state->fail(e?e:22);return;}
    if(Clock::now()>=state->until){state->fail(110);return;}
    ++state->attempts;try{if(!publisher.Write(&command))state->fail(107);}catch(...){state->fail(5);}
   });
  {std::lock_guard lock{state->mutex};state->until=Clock::now()+std::chrono::milliseconds{window};state->live=true;}
  rclcpp::executors::SingleThreadedExecutor executor;executor.add_node(node);
  for(;;){{std::lock_guard lock{state->mutex};if(state->poisoned||Clock::now()>=state->until||state->attempts>=limit)break;}if(!rclcpp::ok())break;executor.spin_once(std::chrono::milliseconds{10});}
  {std::lock_guard lock{state->mutex};if(!state->primary&&state->attempts<limit)state->fail(rclcpp::ok()?110:125);}
  executor.remove_node(node);input.reset();close();
 }catch(std::bad_alloc const&){{std::lock_guard lock{state->mutex};state->fail(12);}input.reset();close();}
 catch(...){{std::lock_guard lock{state->mutex};state->fail(5);}input.reset();close();}
 if(initialized)try{rclcpp::shutdown();}catch(...){state->shutdown_error=5;}
 auto result=state->primary?state->primary:state->close_reader?state->close_reader:state->close_writer?state->close_writer:state->shutdown_error;
 auto diagnostic=humanoid_native_report::emit("BOOSTER_NATIVE primary=",state->primary," close_reader=",state->close_reader," close_writer=",state->close_writer," shutdown=",state->shutdown_error," reader_attempted=",static_cast<unsigned>(state->reader_attempted)," writer_attempted=",static_cast<unsigned>(state->writer_attempted)," send_attempts=",state->attempts," unresolved_create=",static_cast<unsigned>(unresolved_native_create));
 if(!result)result=diagnostic;
 if(unresolved_native_create)close_known=false;
 // Do not destroy callback backing storage after an unknown vendor close/create.
 if(!close_known)std::_Exit(static_cast<int>(result?result:5));
 return static_cast<int>(result);
}
