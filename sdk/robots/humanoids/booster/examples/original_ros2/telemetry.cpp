/** Finite caller-paced full-record telemetry using a real generated Codec. */
#include <roswasm/ros2.h>
#include "bounded_records.hpp"
#include "basic_io.hpp"
#include <fast_io.h>
#include <new>
#ifndef UWVM_HUMANOID_POSITIVE
#define UWVM_HUMANOID_POSITIVE 0
#endif
static_assert(UWVM_HUMANOID_POSITIVE==0||UWVM_HUMANOID_POSITIVE==1);
using Message=uwvm_custom::Ros2Booster_interfaceMsgLowState;
int main(){
 if(uwvm_ros2_abi_version()!=1)return 95;roswasmc_context context{roswasmc_ros2_backend()};
 roswasmcpp::Subscription<Message> subscription{context,"booster-lowstate",roswasmcpp::Codec<Message>::max_wire_bytes};
 auto primary=subscription.error();
 if(!UWVM_HUMANOID_POSITIVE){if(primary==13&&!subscription.is_open())return 0;if(subscription.is_open())subscription.close();return 22;}
 if(primary){auto report=fast_io::concat_std("HUMANOID_SETUP primary=",primary," rollback=",subscription.rollback_error(),"\n");(void)humanoid_io::output(report.data(),report.size());return primary;}std::uint32_t count{};
 subscription.on_message([&](Message const& value){
   // Complete decoding/validation precedes this callback. Diagnostics below
   // are not a projected replacement schema or native identity assertion.
   auto encoded=roswasmcpp::wire::bytes{};
   if(!roswasmcpp::Codec<Message>::encode(value,encoded)){primary=22;return;}
   auto text=fast_io::concat_std("HUMANOID_FULL_RECORD bytes=",encoded.size(),"\n");
   primary=humanoid_io::output(text.data(),text.size());if(!primary)++count;
 });
 for(unsigned i{};!primary&&!count&&i<64;++i){char gate{};primary=humanoid_io::input(&gate,1);if(primary||gate!='R'){if(!primary)primary=22;break;}auto polled=subscription.poll_once();if(!primary)primary=polled==11?0:polled;}
 if(!primary&&!count)primary=110;auto cleanup=subscription.close();
 auto text=fast_io::concat_std("HUMANOID_TELEMETRY primary=",primary," close=",cleanup," rollback=",subscription.rollback_error()," records=",count,"\n");
 auto output=humanoid_io::output(text.data(),text.size());return primary?primary:cleanup?cleanup:output;
}
