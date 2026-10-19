/** @file C++20 concrete custom typed RPC owners, independent of builtin factories.
 * Context/goal storage is stable inside a nonmoving owner. Each named role is explicit.
 * Use checked close/revoke before destruction to observe all cleanup errors. Native
 * permission, ticket ownership, deadlines, quotas and result kind remain host checks.
 * This synchronous caller-driven facade creates no thread, clock or implicit retry. */
#pragma once
#include "bounded_interfaces.h"
#include "bounded_records.hpp"
#include <array>
#include <memory>
#include <functional>
#include <string_view>
#include <utility>
namespace uwvm_custom {
/** Borrow the selected immutable backend; no alias or permission is created. */
inline roswasmc_string alias(std::string_view text) {return {text.data(),text.size()};}
/** Private stable two-role ownership; handlers cannot borrow/retire raw capabilities. */
class ServerOwners {protected:roswasmc_context context_;roswasmc_capability advertise_{},respond_{};uint32_t error_{};bool busy_{};roswasmc_cleanup_report rollback_{};
struct Pass {bool& busy;explicit Pass(bool& value):busy{value}{busy=true;}~Pass(){busy=false;}};
ServerOwners(roswasmc_context const& context,std::string_view advertise,std::string_view respond,bool action):context_{context.backend}{
roswasmc_capability_init(&advertise_,&context_);roswasmc_capability_init(&respond_,&context_);
error_=roswasmc_check_operation(&context_,action?ROSWASMC_ACTION_RESPOND:ROSWASMC_SERVICE_RESPOND);
if(!error_&&(!roswasmc_action_bundle_alias(alias(advertise))||!roswasmc_action_bundle_alias(alias(respond))||advertise==respond))error_=22;
if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;
if(!error_)error_=action?roswasmc_check_operation(&context_,ROSWASMC_ACTION_ADVERTISE):(context_.backend->advertise_service?0:95);
if(!error_)error_=roswasmc_open(&advertise_,advertise.data(),advertise.size());if(!error_)error_=roswasmc_open(&respond_,respond.data(),respond.size());
if(!error_&&advertise_.handle==respond_.handle){respond_.handle=0;error_=17;}
if(!error_){if(action){auto r=roswasmc_action_advertise(&advertise_,nullptr,0,nullptr,0);error_=r.error;if(!error_&&(!r.has_length||r.length))error_=22;}else error_=roswasmc_service_advertise(&advertise_);}
if(error_){std::array<uint32_t,2> statuses{};rollback_=is_open()?cleanup(error_,statuses,false):roswasmc_cleanup_init(error_);}}
roswasmc_cleanup_report cleanup(uint32_t primary,std::array<uint32_t,2>& statuses,bool revoke){auto r=roswasmc_cleanup_init(primary);if(busy_){r.cleanup_error=17;return r;}if(!is_open()){r.cleanup_error=9;return r;}roswasmc_capability* caps[]={&advertise_,&respond_};for(size_t i=2;i>0;--i){uint32_t code=0;if(roswasmc_is_open(caps[i-1])){code=revoke?roswasmc_revoke(caps[i-1]):roswasmc_close(caps[i-1]);++r.attempted;if(code){++r.failed;if(!r.cleanup_error)r.cleanup_error=code;}}statuses[i-1]=code;}return r;}
public:ServerOwners(ServerOwners const&)=delete;ServerOwners& operator=(ServerOwners const&)=delete;ServerOwners(ServerOwners&&)=delete;
~ServerOwners(){if(is_open()){std::array<uint32_t,2> statuses{};cleanup(0,statuses,false);}}
/** Construction and rollback errors stay inspectable; no constructor retries an open. */
uint32_t error()const noexcept{return error_?error_:is_open()?0:9;}bool is_open()const noexcept{return roswasmc_is_open(&advertise_)||roswasmc_is_open(&respond_);}roswasmc_cleanup_report construction_cleanup()const{return rollback_;}
/** Preflight both owners before the first close; a callback refuses17 with no imports. */
roswasmc_cleanup_report close_checked(uint32_t primary,std::array<uint32_t,2>& statuses){return cleanup(primary,statuses,false);}
/** Explicit alias revocation; never automatic rollback or destructor behavior. */
roswasmc_cleanup_report revoke_checked(uint32_t primary,std::array<uint32_t,2>& statuses){return cleanup(primary,statuses,true);}
};
/** Complete custom ur_dashboard_msgs/AddToLog client; default policy returns13 from construction. */
class Ros1Ur_dashboard_msgsAddToLogClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Ur_dashboard_msgsAddToLogRequest;using Response=Ros1Ur_dashboard_msgsAddToLogResponse;
Ros1Ur_dashboard_msgsAddToLogClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Ur_dashboard_msgsAddToLogClient(Ros1Ur_dashboard_msgsAddToLogClient const&)=delete;Ros1Ur_dashboard_msgsAddToLogClient& operator=(Ros1Ur_dashboard_msgsAddToLogClient const&)=delete;Ros1Ur_dashboard_msgsAddToLogClient(Ros1Ur_dashboard_msgsAddToLogClient&&)=delete;
~Ros1Ur_dashboard_msgsAddToLogClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_ur_dashboard_msgs_AddToLog_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ur_dashboard_msgs/AddToLog server; advertise/respond stay separate authorities. */
class Ros1Ur_dashboard_msgsAddToLogServer:public ServerOwners {
using Handler=std::function<Ros1Ur_dashboard_msgsAddToLogResponse(Ros1Ur_dashboard_msgsAddToLogRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Ur_dashboard_msgsAddToLogRequest;using Response=Ros1Ur_dashboard_msgsAddToLogResponse;
Ros1Ur_dashboard_msgsAddToLogServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_ur_dashboard_msgs_AddToLog_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_ur_dashboard_msgs_AddToLog_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_ur_dashboard_msgs_AddToLog_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_ur_dashboard_msgs_AddToLog_respond(&respond_,ticket,&response).error;}
};
/** Complete custom ur_dashboard_msgs/GetLoadedProgram client; default policy returns13 from construction. */
class Ros1Ur_dashboard_msgsGetLoadedProgramClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Ur_dashboard_msgsGetLoadedProgramRequest;using Response=Ros1Ur_dashboard_msgsGetLoadedProgramResponse;
Ros1Ur_dashboard_msgsGetLoadedProgramClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Ur_dashboard_msgsGetLoadedProgramClient(Ros1Ur_dashboard_msgsGetLoadedProgramClient const&)=delete;Ros1Ur_dashboard_msgsGetLoadedProgramClient& operator=(Ros1Ur_dashboard_msgsGetLoadedProgramClient const&)=delete;Ros1Ur_dashboard_msgsGetLoadedProgramClient(Ros1Ur_dashboard_msgsGetLoadedProgramClient&&)=delete;
~Ros1Ur_dashboard_msgsGetLoadedProgramClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgram_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ur_dashboard_msgs/GetLoadedProgram server; advertise/respond stay separate authorities. */
class Ros1Ur_dashboard_msgsGetLoadedProgramServer:public ServerOwners {
using Handler=std::function<Ros1Ur_dashboard_msgsGetLoadedProgramResponse(Ros1Ur_dashboard_msgsGetLoadedProgramRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Ur_dashboard_msgsGetLoadedProgramRequest;using Response=Ros1Ur_dashboard_msgsGetLoadedProgramResponse;
Ros1Ur_dashboard_msgsGetLoadedProgramServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgram_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgram_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgram_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgram_respond(&respond_,ticket,&response).error;}
};
/** Complete custom ur_dashboard_msgs/GetProgramState client; default policy returns13 from construction. */
class Ros1Ur_dashboard_msgsGetProgramStateClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Ur_dashboard_msgsGetProgramStateRequest;using Response=Ros1Ur_dashboard_msgsGetProgramStateResponse;
Ros1Ur_dashboard_msgsGetProgramStateClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Ur_dashboard_msgsGetProgramStateClient(Ros1Ur_dashboard_msgsGetProgramStateClient const&)=delete;Ros1Ur_dashboard_msgsGetProgramStateClient& operator=(Ros1Ur_dashboard_msgsGetProgramStateClient const&)=delete;Ros1Ur_dashboard_msgsGetProgramStateClient(Ros1Ur_dashboard_msgsGetProgramStateClient&&)=delete;
~Ros1Ur_dashboard_msgsGetProgramStateClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_ur_dashboard_msgs_GetProgramState_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ur_dashboard_msgs/GetProgramState server; advertise/respond stay separate authorities. */
class Ros1Ur_dashboard_msgsGetProgramStateServer:public ServerOwners {
using Handler=std::function<Ros1Ur_dashboard_msgsGetProgramStateResponse(Ros1Ur_dashboard_msgsGetProgramStateRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Ur_dashboard_msgsGetProgramStateRequest;using Response=Ros1Ur_dashboard_msgsGetProgramStateResponse;
Ros1Ur_dashboard_msgsGetProgramStateServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_ur_dashboard_msgs_GetProgramState_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_ur_dashboard_msgs_GetProgramState_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_ur_dashboard_msgs_GetProgramState_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_ur_dashboard_msgs_GetProgramState_respond(&respond_,ticket,&response).error;}
};
/** Complete custom ur_dashboard_msgs/GetRobotMode client; default policy returns13 from construction. */
class Ros1Ur_dashboard_msgsGetRobotModeClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Ur_dashboard_msgsGetRobotModeRequest;using Response=Ros1Ur_dashboard_msgsGetRobotModeResponse;
Ros1Ur_dashboard_msgsGetRobotModeClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Ur_dashboard_msgsGetRobotModeClient(Ros1Ur_dashboard_msgsGetRobotModeClient const&)=delete;Ros1Ur_dashboard_msgsGetRobotModeClient& operator=(Ros1Ur_dashboard_msgsGetRobotModeClient const&)=delete;Ros1Ur_dashboard_msgsGetRobotModeClient(Ros1Ur_dashboard_msgsGetRobotModeClient&&)=delete;
~Ros1Ur_dashboard_msgsGetRobotModeClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_ur_dashboard_msgs_GetRobotMode_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ur_dashboard_msgs/GetRobotMode server; advertise/respond stay separate authorities. */
class Ros1Ur_dashboard_msgsGetRobotModeServer:public ServerOwners {
using Handler=std::function<Ros1Ur_dashboard_msgsGetRobotModeResponse(Ros1Ur_dashboard_msgsGetRobotModeRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Ur_dashboard_msgsGetRobotModeRequest;using Response=Ros1Ur_dashboard_msgsGetRobotModeResponse;
Ros1Ur_dashboard_msgsGetRobotModeServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_ur_dashboard_msgs_GetRobotMode_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_ur_dashboard_msgs_GetRobotMode_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_ur_dashboard_msgs_GetRobotMode_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_ur_dashboard_msgs_GetRobotMode_respond(&respond_,ticket,&response).error;}
};
/** Complete custom ur_dashboard_msgs/GetSafetyMode client; default policy returns13 from construction. */
class Ros1Ur_dashboard_msgsGetSafetyModeClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Ur_dashboard_msgsGetSafetyModeRequest;using Response=Ros1Ur_dashboard_msgsGetSafetyModeResponse;
Ros1Ur_dashboard_msgsGetSafetyModeClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Ur_dashboard_msgsGetSafetyModeClient(Ros1Ur_dashboard_msgsGetSafetyModeClient const&)=delete;Ros1Ur_dashboard_msgsGetSafetyModeClient& operator=(Ros1Ur_dashboard_msgsGetSafetyModeClient const&)=delete;Ros1Ur_dashboard_msgsGetSafetyModeClient(Ros1Ur_dashboard_msgsGetSafetyModeClient&&)=delete;
~Ros1Ur_dashboard_msgsGetSafetyModeClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyMode_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ur_dashboard_msgs/GetSafetyMode server; advertise/respond stay separate authorities. */
class Ros1Ur_dashboard_msgsGetSafetyModeServer:public ServerOwners {
using Handler=std::function<Ros1Ur_dashboard_msgsGetSafetyModeResponse(Ros1Ur_dashboard_msgsGetSafetyModeRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Ur_dashboard_msgsGetSafetyModeRequest;using Response=Ros1Ur_dashboard_msgsGetSafetyModeResponse;
Ros1Ur_dashboard_msgsGetSafetyModeServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyMode_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyMode_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyMode_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyMode_respond(&respond_,ticket,&response).error;}
};
/** Complete custom ur_dashboard_msgs/IsInRemoteControl client; default policy returns13 from construction. */
class Ros1Ur_dashboard_msgsIsInRemoteControlClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Ur_dashboard_msgsIsInRemoteControlRequest;using Response=Ros1Ur_dashboard_msgsIsInRemoteControlResponse;
Ros1Ur_dashboard_msgsIsInRemoteControlClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Ur_dashboard_msgsIsInRemoteControlClient(Ros1Ur_dashboard_msgsIsInRemoteControlClient const&)=delete;Ros1Ur_dashboard_msgsIsInRemoteControlClient& operator=(Ros1Ur_dashboard_msgsIsInRemoteControlClient const&)=delete;Ros1Ur_dashboard_msgsIsInRemoteControlClient(Ros1Ur_dashboard_msgsIsInRemoteControlClient&&)=delete;
~Ros1Ur_dashboard_msgsIsInRemoteControlClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControl_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ur_dashboard_msgs/IsInRemoteControl server; advertise/respond stay separate authorities. */
class Ros1Ur_dashboard_msgsIsInRemoteControlServer:public ServerOwners {
using Handler=std::function<Ros1Ur_dashboard_msgsIsInRemoteControlResponse(Ros1Ur_dashboard_msgsIsInRemoteControlRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Ur_dashboard_msgsIsInRemoteControlRequest;using Response=Ros1Ur_dashboard_msgsIsInRemoteControlResponse;
Ros1Ur_dashboard_msgsIsInRemoteControlServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControl_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControl_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControl_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControl_respond(&respond_,ticket,&response).error;}
};
/** Complete custom ur_dashboard_msgs/IsProgramRunning client; default policy returns13 from construction. */
class Ros1Ur_dashboard_msgsIsProgramRunningClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Ur_dashboard_msgsIsProgramRunningRequest;using Response=Ros1Ur_dashboard_msgsIsProgramRunningResponse;
Ros1Ur_dashboard_msgsIsProgramRunningClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Ur_dashboard_msgsIsProgramRunningClient(Ros1Ur_dashboard_msgsIsProgramRunningClient const&)=delete;Ros1Ur_dashboard_msgsIsProgramRunningClient& operator=(Ros1Ur_dashboard_msgsIsProgramRunningClient const&)=delete;Ros1Ur_dashboard_msgsIsProgramRunningClient(Ros1Ur_dashboard_msgsIsProgramRunningClient&&)=delete;
~Ros1Ur_dashboard_msgsIsProgramRunningClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunning_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ur_dashboard_msgs/IsProgramRunning server; advertise/respond stay separate authorities. */
class Ros1Ur_dashboard_msgsIsProgramRunningServer:public ServerOwners {
using Handler=std::function<Ros1Ur_dashboard_msgsIsProgramRunningResponse(Ros1Ur_dashboard_msgsIsProgramRunningRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Ur_dashboard_msgsIsProgramRunningRequest;using Response=Ros1Ur_dashboard_msgsIsProgramRunningResponse;
Ros1Ur_dashboard_msgsIsProgramRunningServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunning_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunning_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunning_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunning_respond(&respond_,ticket,&response).error;}
};
/** Complete custom ur_dashboard_msgs/IsProgramSaved client; default policy returns13 from construction. */
class Ros1Ur_dashboard_msgsIsProgramSavedClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Ur_dashboard_msgsIsProgramSavedRequest;using Response=Ros1Ur_dashboard_msgsIsProgramSavedResponse;
Ros1Ur_dashboard_msgsIsProgramSavedClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Ur_dashboard_msgsIsProgramSavedClient(Ros1Ur_dashboard_msgsIsProgramSavedClient const&)=delete;Ros1Ur_dashboard_msgsIsProgramSavedClient& operator=(Ros1Ur_dashboard_msgsIsProgramSavedClient const&)=delete;Ros1Ur_dashboard_msgsIsProgramSavedClient(Ros1Ur_dashboard_msgsIsProgramSavedClient&&)=delete;
~Ros1Ur_dashboard_msgsIsProgramSavedClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSaved_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ur_dashboard_msgs/IsProgramSaved server; advertise/respond stay separate authorities. */
class Ros1Ur_dashboard_msgsIsProgramSavedServer:public ServerOwners {
using Handler=std::function<Ros1Ur_dashboard_msgsIsProgramSavedResponse(Ros1Ur_dashboard_msgsIsProgramSavedRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Ur_dashboard_msgsIsProgramSavedRequest;using Response=Ros1Ur_dashboard_msgsIsProgramSavedResponse;
Ros1Ur_dashboard_msgsIsProgramSavedServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSaved_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSaved_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSaved_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSaved_respond(&respond_,ticket,&response).error;}
};
/** Complete custom ur_dashboard_msgs/Load client; default policy returns13 from construction. */
class Ros1Ur_dashboard_msgsLoadClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Ur_dashboard_msgsLoadRequest;using Response=Ros1Ur_dashboard_msgsLoadResponse;
Ros1Ur_dashboard_msgsLoadClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Ur_dashboard_msgsLoadClient(Ros1Ur_dashboard_msgsLoadClient const&)=delete;Ros1Ur_dashboard_msgsLoadClient& operator=(Ros1Ur_dashboard_msgsLoadClient const&)=delete;Ros1Ur_dashboard_msgsLoadClient(Ros1Ur_dashboard_msgsLoadClient&&)=delete;
~Ros1Ur_dashboard_msgsLoadClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_ur_dashboard_msgs_Load_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ur_dashboard_msgs/Load server; advertise/respond stay separate authorities. */
class Ros1Ur_dashboard_msgsLoadServer:public ServerOwners {
using Handler=std::function<Ros1Ur_dashboard_msgsLoadResponse(Ros1Ur_dashboard_msgsLoadRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Ur_dashboard_msgsLoadRequest;using Response=Ros1Ur_dashboard_msgsLoadResponse;
Ros1Ur_dashboard_msgsLoadServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_ur_dashboard_msgs_Load_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_ur_dashboard_msgs_Load_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_ur_dashboard_msgs_Load_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_ur_dashboard_msgs_Load_respond(&respond_,ticket,&response).error;}
};
/** Complete custom ur_dashboard_msgs/Popup client; default policy returns13 from construction. */
class Ros1Ur_dashboard_msgsPopupClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Ur_dashboard_msgsPopupRequest;using Response=Ros1Ur_dashboard_msgsPopupResponse;
Ros1Ur_dashboard_msgsPopupClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Ur_dashboard_msgsPopupClient(Ros1Ur_dashboard_msgsPopupClient const&)=delete;Ros1Ur_dashboard_msgsPopupClient& operator=(Ros1Ur_dashboard_msgsPopupClient const&)=delete;Ros1Ur_dashboard_msgsPopupClient(Ros1Ur_dashboard_msgsPopupClient&&)=delete;
~Ros1Ur_dashboard_msgsPopupClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_ur_dashboard_msgs_Popup_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ur_dashboard_msgs/Popup server; advertise/respond stay separate authorities. */
class Ros1Ur_dashboard_msgsPopupServer:public ServerOwners {
using Handler=std::function<Ros1Ur_dashboard_msgsPopupResponse(Ros1Ur_dashboard_msgsPopupRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Ur_dashboard_msgsPopupRequest;using Response=Ros1Ur_dashboard_msgsPopupResponse;
Ros1Ur_dashboard_msgsPopupServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_ur_dashboard_msgs_Popup_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_ur_dashboard_msgs_Popup_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_ur_dashboard_msgs_Popup_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_ur_dashboard_msgs_Popup_respond(&respond_,ticket,&response).error;}
};
/** Complete custom ur_dashboard_msgs/RawRequest client; default policy returns13 from construction. */
class Ros1Ur_dashboard_msgsRawRequestClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Ur_dashboard_msgsRawRequestRequest;using Response=Ros1Ur_dashboard_msgsRawRequestResponse;
Ros1Ur_dashboard_msgsRawRequestClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Ur_dashboard_msgsRawRequestClient(Ros1Ur_dashboard_msgsRawRequestClient const&)=delete;Ros1Ur_dashboard_msgsRawRequestClient& operator=(Ros1Ur_dashboard_msgsRawRequestClient const&)=delete;Ros1Ur_dashboard_msgsRawRequestClient(Ros1Ur_dashboard_msgsRawRequestClient&&)=delete;
~Ros1Ur_dashboard_msgsRawRequestClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_ur_dashboard_msgs_RawRequest_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ur_dashboard_msgs/RawRequest server; advertise/respond stay separate authorities. */
class Ros1Ur_dashboard_msgsRawRequestServer:public ServerOwners {
using Handler=std::function<Ros1Ur_dashboard_msgsRawRequestResponse(Ros1Ur_dashboard_msgsRawRequestRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Ur_dashboard_msgsRawRequestRequest;using Response=Ros1Ur_dashboard_msgsRawRequestResponse;
Ros1Ur_dashboard_msgsRawRequestServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_ur_dashboard_msgs_RawRequest_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_ur_dashboard_msgs_RawRequest_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_ur_dashboard_msgs_RawRequest_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_ur_dashboard_msgs_RawRequest_respond(&respond_,ticket,&response).error;}
};
/** Complete owned custom ur_dashboard_msgs/SetMode Action client with four separate roles. */
class Ros1Ur_dashboard_msgsSetModeClient {roswasmc_context context_;std::array<uint64_t,1024> goals_{};roswasmc_action_bundle bundle_{};
public:
using Goal=Ros1Ur_dashboard_msgsSetModeGoal;using Result=Ros1Ur_dashboard_msgsSetModeResult;using Feedback=Ros1Ur_dashboard_msgsSetModeFeedback;
Ros1Ur_dashboard_msgsSetModeClient(roswasmc_context const& context,roswasmc_action_aliases const& aliases,size_t slots=8):context_{context.backend} {roswasmc_action_bundle_init(&context_,&bundle_,&aliases,goals_.data(),slots);}
Ros1Ur_dashboard_msgsSetModeClient(Ros1Ur_dashboard_msgsSetModeClient const&)=delete;Ros1Ur_dashboard_msgsSetModeClient& operator=(Ros1Ur_dashboard_msgsSetModeClient const&)=delete;Ros1Ur_dashboard_msgsSetModeClient(Ros1Ur_dashboard_msgsSetModeClient&&)=delete;
~Ros1Ur_dashboard_msgsSetModeClient(){if(bundle_.active){std::array<uint32_t,5> statuses{};roswasmc_action_bundle_close_checked(&bundle_,0,statuses.data(),statuses.size());}}
uint32_t error() const noexcept{return bundle_.error?bundle_.error:bundle_.active?0:9;}bool is_open() const noexcept{return bundle_.active;}bool owns(uint64_t id)const{return roswasmc_action_bundle_owns(&bundle_,id);}
roswasmc_result send_goal(Goal const& value,uint64_t& id){return roswasmc_bounded_1_ur_dashboard_msgs_SetMode_send(&bundle_,&value,&id);}
roswasmc_result feedback(uint64_t id,Feedback& value){return roswasmc_bounded_1_ur_dashboard_msgs_SetMode_feedback(&bundle_,id,&value);}
roswasmc_result result(uint64_t id,uint8_t& kind,Result& value){return roswasmc_bounded_1_ur_dashboard_msgs_SetMode_result(&bundle_,id,&kind,&value);}
roswasmc_result cancel_own(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_own(&bundle_,id,&accepted);}
roswasmc_result cancel_any(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_any(&bundle_,id,&accepted);}
roswasmc_result cancel_all(int& accepted){return roswasmc_action_bundle_cancel_all(&bundle_,&accepted);}
roswasmc_cleanup_report close_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_close_checked(&bundle_,primary,statuses.data(),statuses.size());}
roswasmc_cleanup_report revoke_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_revoke_checked(&bundle_,primary,statuses.data(),statuses.size());}
};
/** Typed custom ur_dashboard_msgs/SetMode server with explicit feedback/terminal component codecs. */
class Ros1Ur_dashboard_msgsSetModeServer:public ServerOwners {
public:
using Goal=Ros1Ur_dashboard_msgsSetModeGoal;using Result=Ros1Ur_dashboard_msgsSetModeResult;using Feedback=Ros1Ur_dashboard_msgsSetModeFeedback;
/** Handler may choose separate feedback/result effects; kind must be2..4 when terminal. */
struct Replies {Feedback feedback{};Result result{};uint8_t kind{2};bool send_feedback{true},send_result{true};};
private:using Handler=std::function<Replies(uint64_t,Goal const&)>;std::shared_ptr<Handler> handler_;
public:
Ros1Ur_dashboard_msgsSetModeServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,true} {}
void on_goal(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& id,Goal& goal){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_ur_dashboard_msgs_SetMode_poll(&advertise_,&id,&goal);}
roswasmc_result respond_feedback(uint64_t id,Feedback const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_ur_dashboard_msgs_SetMode_respond_feedback(&respond_,id,&value);}
roswasmc_result respond_result(uint64_t id,uint8_t kind,Result const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_ur_dashboard_msgs_SetMode_respond_result(&respond_,id,kind,&value);}
/** One pass, at most one feedback plus one terminal response; never implicit replay. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t id{};Goal goal{};auto r=roswasmc_bounded_1_ur_dashboard_msgs_SetMode_poll(&advertise_,&id,&goal);if(r.error)return r.error;auto replies=(*handler)(id,goal);if(replies.send_result&&(replies.kind<2||replies.kind>4||!roswasmc_bounded_1_ur_dashboard_msgs_SetModeResult_valid(&replies.result)))return 22;if(replies.send_feedback&&!roswasmc_bounded_1_ur_dashboard_msgs_SetModeFeedback_valid(&replies.feedback))return 22;if(replies.send_feedback){r=roswasmc_bounded_1_ur_dashboard_msgs_SetMode_respond_feedback(&respond_,id,&replies.feedback);if(r.error)return r.error;}return replies.send_result?roswasmc_bounded_1_ur_dashboard_msgs_SetMode_respond_result(&respond_,id,replies.kind,&replies.result).error:0;}
};
/** Complete custom ur_msgs/GetRobotSoftwareVersion client; default policy returns13 from construction. */
class Ros1Ur_msgsGetRobotSoftwareVersionClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Ur_msgsGetRobotSoftwareVersionRequest;using Response=Ros1Ur_msgsGetRobotSoftwareVersionResponse;
Ros1Ur_msgsGetRobotSoftwareVersionClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Ur_msgsGetRobotSoftwareVersionClient(Ros1Ur_msgsGetRobotSoftwareVersionClient const&)=delete;Ros1Ur_msgsGetRobotSoftwareVersionClient& operator=(Ros1Ur_msgsGetRobotSoftwareVersionClient const&)=delete;Ros1Ur_msgsGetRobotSoftwareVersionClient(Ros1Ur_msgsGetRobotSoftwareVersionClient&&)=delete;
~Ros1Ur_msgsGetRobotSoftwareVersionClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersion_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ur_msgs/GetRobotSoftwareVersion server; advertise/respond stay separate authorities. */
class Ros1Ur_msgsGetRobotSoftwareVersionServer:public ServerOwners {
using Handler=std::function<Ros1Ur_msgsGetRobotSoftwareVersionResponse(Ros1Ur_msgsGetRobotSoftwareVersionRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Ur_msgsGetRobotSoftwareVersionRequest;using Response=Ros1Ur_msgsGetRobotSoftwareVersionResponse;
Ros1Ur_msgsGetRobotSoftwareVersionServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersion_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersion_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersion_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersion_respond(&respond_,ticket,&response).error;}
};
/** Complete custom ur_msgs/SetAnalogOutput client; default policy returns13 from construction. */
class Ros1Ur_msgsSetAnalogOutputClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Ur_msgsSetAnalogOutputRequest;using Response=Ros1Ur_msgsSetAnalogOutputResponse;
Ros1Ur_msgsSetAnalogOutputClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Ur_msgsSetAnalogOutputClient(Ros1Ur_msgsSetAnalogOutputClient const&)=delete;Ros1Ur_msgsSetAnalogOutputClient& operator=(Ros1Ur_msgsSetAnalogOutputClient const&)=delete;Ros1Ur_msgsSetAnalogOutputClient(Ros1Ur_msgsSetAnalogOutputClient&&)=delete;
~Ros1Ur_msgsSetAnalogOutputClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_ur_msgs_SetAnalogOutput_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ur_msgs/SetAnalogOutput server; advertise/respond stay separate authorities. */
class Ros1Ur_msgsSetAnalogOutputServer:public ServerOwners {
using Handler=std::function<Ros1Ur_msgsSetAnalogOutputResponse(Ros1Ur_msgsSetAnalogOutputRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Ur_msgsSetAnalogOutputRequest;using Response=Ros1Ur_msgsSetAnalogOutputResponse;
Ros1Ur_msgsSetAnalogOutputServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_ur_msgs_SetAnalogOutput_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_ur_msgs_SetAnalogOutput_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_ur_msgs_SetAnalogOutput_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_ur_msgs_SetAnalogOutput_respond(&respond_,ticket,&response).error;}
};
/** Complete custom ur_msgs/SetForceMode client; default policy returns13 from construction. */
class Ros1Ur_msgsSetForceModeClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Ur_msgsSetForceModeRequest;using Response=Ros1Ur_msgsSetForceModeResponse;
Ros1Ur_msgsSetForceModeClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Ur_msgsSetForceModeClient(Ros1Ur_msgsSetForceModeClient const&)=delete;Ros1Ur_msgsSetForceModeClient& operator=(Ros1Ur_msgsSetForceModeClient const&)=delete;Ros1Ur_msgsSetForceModeClient(Ros1Ur_msgsSetForceModeClient&&)=delete;
~Ros1Ur_msgsSetForceModeClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_ur_msgs_SetForceMode_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ur_msgs/SetForceMode server; advertise/respond stay separate authorities. */
class Ros1Ur_msgsSetForceModeServer:public ServerOwners {
using Handler=std::function<Ros1Ur_msgsSetForceModeResponse(Ros1Ur_msgsSetForceModeRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Ur_msgsSetForceModeRequest;using Response=Ros1Ur_msgsSetForceModeResponse;
Ros1Ur_msgsSetForceModeServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_ur_msgs_SetForceMode_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_ur_msgs_SetForceMode_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_ur_msgs_SetForceMode_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_ur_msgs_SetForceMode_respond(&respond_,ticket,&response).error;}
};
/** Complete custom ur_msgs/SetIO client; default policy returns13 from construction. */
class Ros1Ur_msgsSetIOClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Ur_msgsSetIORequest;using Response=Ros1Ur_msgsSetIOResponse;
Ros1Ur_msgsSetIOClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Ur_msgsSetIOClient(Ros1Ur_msgsSetIOClient const&)=delete;Ros1Ur_msgsSetIOClient& operator=(Ros1Ur_msgsSetIOClient const&)=delete;Ros1Ur_msgsSetIOClient(Ros1Ur_msgsSetIOClient&&)=delete;
~Ros1Ur_msgsSetIOClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_ur_msgs_SetIO_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ur_msgs/SetIO server; advertise/respond stay separate authorities. */
class Ros1Ur_msgsSetIOServer:public ServerOwners {
using Handler=std::function<Ros1Ur_msgsSetIOResponse(Ros1Ur_msgsSetIORequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Ur_msgsSetIORequest;using Response=Ros1Ur_msgsSetIOResponse;
Ros1Ur_msgsSetIOServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_ur_msgs_SetIO_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_ur_msgs_SetIO_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_ur_msgs_SetIO_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_ur_msgs_SetIO_respond(&respond_,ticket,&response).error;}
};
/** Complete custom ur_msgs/SetPayload client; default policy returns13 from construction. */
class Ros1Ur_msgsSetPayloadClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Ur_msgsSetPayloadRequest;using Response=Ros1Ur_msgsSetPayloadResponse;
Ros1Ur_msgsSetPayloadClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Ur_msgsSetPayloadClient(Ros1Ur_msgsSetPayloadClient const&)=delete;Ros1Ur_msgsSetPayloadClient& operator=(Ros1Ur_msgsSetPayloadClient const&)=delete;Ros1Ur_msgsSetPayloadClient(Ros1Ur_msgsSetPayloadClient&&)=delete;
~Ros1Ur_msgsSetPayloadClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_ur_msgs_SetPayload_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ur_msgs/SetPayload server; advertise/respond stay separate authorities. */
class Ros1Ur_msgsSetPayloadServer:public ServerOwners {
using Handler=std::function<Ros1Ur_msgsSetPayloadResponse(Ros1Ur_msgsSetPayloadRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Ur_msgsSetPayloadRequest;using Response=Ros1Ur_msgsSetPayloadResponse;
Ros1Ur_msgsSetPayloadServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_ur_msgs_SetPayload_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_ur_msgs_SetPayload_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_ur_msgs_SetPayload_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_ur_msgs_SetPayload_respond(&respond_,ticket,&response).error;}
};
/** Complete custom ur_msgs/SetSpeedSliderFraction client; default policy returns13 from construction. */
class Ros1Ur_msgsSetSpeedSliderFractionClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Ur_msgsSetSpeedSliderFractionRequest;using Response=Ros1Ur_msgsSetSpeedSliderFractionResponse;
Ros1Ur_msgsSetSpeedSliderFractionClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Ur_msgsSetSpeedSliderFractionClient(Ros1Ur_msgsSetSpeedSliderFractionClient const&)=delete;Ros1Ur_msgsSetSpeedSliderFractionClient& operator=(Ros1Ur_msgsSetSpeedSliderFractionClient const&)=delete;Ros1Ur_msgsSetSpeedSliderFractionClient(Ros1Ur_msgsSetSpeedSliderFractionClient&&)=delete;
~Ros1Ur_msgsSetSpeedSliderFractionClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_ur_msgs_SetSpeedSliderFraction_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ur_msgs/SetSpeedSliderFraction server; advertise/respond stay separate authorities. */
class Ros1Ur_msgsSetSpeedSliderFractionServer:public ServerOwners {
using Handler=std::function<Ros1Ur_msgsSetSpeedSliderFractionResponse(Ros1Ur_msgsSetSpeedSliderFractionRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Ur_msgsSetSpeedSliderFractionRequest;using Response=Ros1Ur_msgsSetSpeedSliderFractionResponse;
Ros1Ur_msgsSetSpeedSliderFractionServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_ur_msgs_SetSpeedSliderFraction_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_ur_msgs_SetSpeedSliderFraction_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_ur_msgs_SetSpeedSliderFraction_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_ur_msgs_SetSpeedSliderFraction_respond(&respond_,ticket,&response).error;}
};
}
