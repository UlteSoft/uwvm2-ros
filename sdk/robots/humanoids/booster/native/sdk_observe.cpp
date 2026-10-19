/** Explicit read-only native SDK observation, separate from command transport.
 * Actual robot-info/version and mode values are returned by genuine RPC calls.
 * They are runtime observations, not cryptographic firmware attestation. No
 * ChangeMode, movement, stop or operation-cancel method is called. Default0 has
 * zero network effects. Root owns whole-process deadline/library/SDK closure.
 */
#include <booster/robot/b1/b1_loco_api.hpp>
#include <booster/robot/rpc/rpc_client.hpp>
#include "tests/ros_custom_types/bounded-v2/native_protocol.h"
#include "report.hpp"
#include <chrono>
#include <cstdlib>
#include <string>
#ifndef UWVM_BOOSTER_SDK_OBSERVE_ENABLE
#define UWVM_BOOSTER_SDK_OBSERVE_ENABLE 0
#endif
static_assert(UWVM_BOOSTER_SDK_OBSERVE_ENABLE==0||UWVM_BOOSTER_SDK_OBSERVE_ENABLE==1);
int main(int argc,char** argv){
 if(!UWVM_BOOSTER_SDK_OBSERVE_ENABLE)return 13;
 // domain-plus-one, interface, explicit RPC robot suffix ("-" selects default),
 // exact expected model, exact firmware version, exact expected mode0..4.
 if(argc!=7)return 22;
 std::string d{argv[1]},iface{argv[2]},suffix{argv[3]},model{argv[4]},version{argv[5]},mode{argv[6]};
 if(d.empty()||d.size()>3||iface.empty()||suffix.empty()||model.empty()||version.empty()||mode.size()!=1||mode[0]<'0'||mode[0]>'4')return 22;
 unsigned domain{};for(char c:d){if(c<'0'||c>'9')return 22;domain=domain*10u+static_cast<unsigned>(c-'0');}if(!domain||domain>233)return 22;
 if(iface.size()>128||suffix.size()>128||model.size()>128||version.size()>128)return 90;
 for(auto const* value:{&iface,&suffix,&model,&version})for(unsigned char c:*value)if(c<0x20||c>0x7e)return 22;
 using Clock=std::chrono::steady_clock;auto until=Clock::now()+std::chrono::milliseconds{2000};
 auto remaining=[&]()->std::int64_t{auto n=std::chrono::duration_cast<std::chrono::milliseconds>(until-Clock::now()).count();return n>0?(n>1000?1000:n):0;};
 booster::robot::RpcClient rpc;bool attempted{},stop_attempted{},unresolved_native_create{};std::uint32_t primary{},cleanup{};
 auto close=[&]()noexcept{if(attempted&&!stop_attempted){stop_attempted=true;try{rpc.Stop();}catch(...){cleanup=5;}}};
 try{
  unresolved_native_create=true;booster::robot::ChannelFactory::Instance()->Init(static_cast<std::int32_t>(domain-1),iface);unresolved_native_create=false;
  attempted=true;unresolved_native_create=true;rpc.Init(suffix=="-"?"rt/LocoApiTopic":"rt/LocoApiTopic/"+suffix);unresolved_native_create=false;
  auto wait=remaining();if(!wait||!rpc.WaitForService(wait,true)){primary=110;}
  auto query=[&](booster::robot::b1::LocoApiId id)->booster::robot::Response{
   auto left=remaining();if(!left){primary=110;throw std::runtime_error{"observation deadline before send"};}
   booster::robot::Request req{booster::robot::RequestHeader{static_cast<std::int64_t>(id)},""};
   // Exactly one native send per query. A timeout/result is never replayed.
   auto response=rpc.SendApiRequest(req,left);
   if(!remaining()){primary=110;throw std::runtime_error{"observation deadline after send"};}
   if(response.GetHeader().GetStatus()!=0||response.GetBody().size()>4096)throw std::runtime_error{"native response refused"};
   return response;
  };
  if(!primary){
   auto actual=query(booster::robot::b1::LocoApiId::kGetRobotInfo);auto text=actual.GetBody();auto json=nlohmann::json::parse(text);
   booster::robot::b1::GetRobotInfoResponse info;info.FromJson(json);
   if(info.model_!=model||info.version_!=version)primary=13;
   // Retain the complete native response, including all seven original fields.
   bounded_native_io::emit(STDOUT_FILENO,text);bounded_native_io::emit(STDOUT_FILENO,"\n");
   if(!primary){auto actual_mode=query(booster::robot::b1::LocoApiId::kGetMode);auto body=actual_mode.GetBody();auto object=nlohmann::json::parse(body);booster::robot::b1::GetModeResponse value;value.FromJson(object);
    if(static_cast<int>(value.mode_)!=mode[0]-'0')primary=13;
    bounded_native_io::emit(STDOUT_FILENO,body);bounded_native_io::emit(STDOUT_FILENO,"\n");}
  }
 }catch(std::bad_alloc const&){primary=12;}catch(...){if(!primary)primary=5;}
 close();auto output=humanoid_native_report::emit("BOOSTER_OBSERVE primary=",primary," stop=",cleanup," stop_attempted=",static_cast<unsigned>(stop_attempted));
 auto status=primary?primary:cleanup?cleanup:output;
 if(cleanup||unresolved_native_create)std::_Exit(static_cast<int>(status));
 return static_cast<int>(status);
}
