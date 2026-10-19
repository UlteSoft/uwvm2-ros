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
/** Complete custom abb_rapid_sm_addin_msgs/GetEGMSettings client; default policy returns13 from construction. */
class Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsRequest;using Response=Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsResponse;
Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsClient(Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsClient const&)=delete;Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsClient& operator=(Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsClient const&)=delete;Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsClient(Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsClient&&)=delete;
~Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettings_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom abb_rapid_sm_addin_msgs/GetEGMSettings server; advertise/respond stay separate authorities. */
class Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsServer:public ServerOwners {
using Handler=std::function<Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsResponse(Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsRequest;using Response=Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsResponse;
Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettings_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettings_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettings_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettings_respond(&respond_,ticket,&response).error;}
};
/** Complete custom abb_rapid_sm_addin_msgs/SetEGMSettings client; default policy returns13 from construction. */
class Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsRequest;using Response=Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsResponse;
Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsClient(Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsClient const&)=delete;Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsClient& operator=(Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsClient const&)=delete;Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsClient(Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsClient&&)=delete;
~Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettings_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom abb_rapid_sm_addin_msgs/SetEGMSettings server; advertise/respond stay separate authorities. */
class Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsServer:public ServerOwners {
using Handler=std::function<Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsResponse(Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsRequest;using Response=Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsResponse;
Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettings_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettings_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettings_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettings_respond(&respond_,ticket,&response).error;}
};
/** Complete custom abb_rapid_sm_addin_msgs/SetRAPIDRoutine client; default policy returns13 from construction. */
class Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineRequest;using Response=Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineResponse;
Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineClient(Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineClient const&)=delete;Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineClient& operator=(Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineClient const&)=delete;Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineClient(Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineClient&&)=delete;
~Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutine_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom abb_rapid_sm_addin_msgs/SetRAPIDRoutine server; advertise/respond stay separate authorities. */
class Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineServer:public ServerOwners {
using Handler=std::function<Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineResponse(Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineRequest;using Response=Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineResponse;
Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutine_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutine_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutine_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutine_respond(&respond_,ticket,&response).error;}
};
/** Complete custom abb_rapid_sm_addin_msgs/SetSGCommand client; default policy returns13 from construction. */
class Ros1Abb_rapid_sm_addin_msgsSetSGCommandClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Abb_rapid_sm_addin_msgsSetSGCommandRequest;using Response=Ros1Abb_rapid_sm_addin_msgsSetSGCommandResponse;
Ros1Abb_rapid_sm_addin_msgsSetSGCommandClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Abb_rapid_sm_addin_msgsSetSGCommandClient(Ros1Abb_rapid_sm_addin_msgsSetSGCommandClient const&)=delete;Ros1Abb_rapid_sm_addin_msgsSetSGCommandClient& operator=(Ros1Abb_rapid_sm_addin_msgsSetSGCommandClient const&)=delete;Ros1Abb_rapid_sm_addin_msgsSetSGCommandClient(Ros1Abb_rapid_sm_addin_msgsSetSGCommandClient&&)=delete;
~Ros1Abb_rapid_sm_addin_msgsSetSGCommandClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommand_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom abb_rapid_sm_addin_msgs/SetSGCommand server; advertise/respond stay separate authorities. */
class Ros1Abb_rapid_sm_addin_msgsSetSGCommandServer:public ServerOwners {
using Handler=std::function<Ros1Abb_rapid_sm_addin_msgsSetSGCommandResponse(Ros1Abb_rapid_sm_addin_msgsSetSGCommandRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Abb_rapid_sm_addin_msgsSetSGCommandRequest;using Response=Ros1Abb_rapid_sm_addin_msgsSetSGCommandResponse;
Ros1Abb_rapid_sm_addin_msgsSetSGCommandServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommand_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommand_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommand_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommand_respond(&respond_,ticket,&response).error;}
};
/** Complete custom abb_robot_msgs/GetFileContents client; default policy returns13 from construction. */
class Ros1Abb_robot_msgsGetFileContentsClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Abb_robot_msgsGetFileContentsRequest;using Response=Ros1Abb_robot_msgsGetFileContentsResponse;
Ros1Abb_robot_msgsGetFileContentsClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Abb_robot_msgsGetFileContentsClient(Ros1Abb_robot_msgsGetFileContentsClient const&)=delete;Ros1Abb_robot_msgsGetFileContentsClient& operator=(Ros1Abb_robot_msgsGetFileContentsClient const&)=delete;Ros1Abb_robot_msgsGetFileContentsClient(Ros1Abb_robot_msgsGetFileContentsClient&&)=delete;
~Ros1Abb_robot_msgsGetFileContentsClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_abb_robot_msgs_GetFileContents_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom abb_robot_msgs/GetFileContents server; advertise/respond stay separate authorities. */
class Ros1Abb_robot_msgsGetFileContentsServer:public ServerOwners {
using Handler=std::function<Ros1Abb_robot_msgsGetFileContentsResponse(Ros1Abb_robot_msgsGetFileContentsRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Abb_robot_msgsGetFileContentsRequest;using Response=Ros1Abb_robot_msgsGetFileContentsResponse;
Ros1Abb_robot_msgsGetFileContentsServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_abb_robot_msgs_GetFileContents_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_abb_robot_msgs_GetFileContents_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_abb_robot_msgs_GetFileContents_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_abb_robot_msgs_GetFileContents_respond(&respond_,ticket,&response).error;}
};
/** Complete custom abb_robot_msgs/GetIOSignal client; default policy returns13 from construction. */
class Ros1Abb_robot_msgsGetIOSignalClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Abb_robot_msgsGetIOSignalRequest;using Response=Ros1Abb_robot_msgsGetIOSignalResponse;
Ros1Abb_robot_msgsGetIOSignalClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Abb_robot_msgsGetIOSignalClient(Ros1Abb_robot_msgsGetIOSignalClient const&)=delete;Ros1Abb_robot_msgsGetIOSignalClient& operator=(Ros1Abb_robot_msgsGetIOSignalClient const&)=delete;Ros1Abb_robot_msgsGetIOSignalClient(Ros1Abb_robot_msgsGetIOSignalClient&&)=delete;
~Ros1Abb_robot_msgsGetIOSignalClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_abb_robot_msgs_GetIOSignal_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom abb_robot_msgs/GetIOSignal server; advertise/respond stay separate authorities. */
class Ros1Abb_robot_msgsGetIOSignalServer:public ServerOwners {
using Handler=std::function<Ros1Abb_robot_msgsGetIOSignalResponse(Ros1Abb_robot_msgsGetIOSignalRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Abb_robot_msgsGetIOSignalRequest;using Response=Ros1Abb_robot_msgsGetIOSignalResponse;
Ros1Abb_robot_msgsGetIOSignalServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_abb_robot_msgs_GetIOSignal_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_abb_robot_msgs_GetIOSignal_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_abb_robot_msgs_GetIOSignal_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_abb_robot_msgs_GetIOSignal_respond(&respond_,ticket,&response).error;}
};
/** Complete custom abb_robot_msgs/GetRAPIDBool client; default policy returns13 from construction. */
class Ros1Abb_robot_msgsGetRAPIDBoolClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Abb_robot_msgsGetRAPIDBoolRequest;using Response=Ros1Abb_robot_msgsGetRAPIDBoolResponse;
Ros1Abb_robot_msgsGetRAPIDBoolClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Abb_robot_msgsGetRAPIDBoolClient(Ros1Abb_robot_msgsGetRAPIDBoolClient const&)=delete;Ros1Abb_robot_msgsGetRAPIDBoolClient& operator=(Ros1Abb_robot_msgsGetRAPIDBoolClient const&)=delete;Ros1Abb_robot_msgsGetRAPIDBoolClient(Ros1Abb_robot_msgsGetRAPIDBoolClient&&)=delete;
~Ros1Abb_robot_msgsGetRAPIDBoolClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBool_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom abb_robot_msgs/GetRAPIDBool server; advertise/respond stay separate authorities. */
class Ros1Abb_robot_msgsGetRAPIDBoolServer:public ServerOwners {
using Handler=std::function<Ros1Abb_robot_msgsGetRAPIDBoolResponse(Ros1Abb_robot_msgsGetRAPIDBoolRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Abb_robot_msgsGetRAPIDBoolRequest;using Response=Ros1Abb_robot_msgsGetRAPIDBoolResponse;
Ros1Abb_robot_msgsGetRAPIDBoolServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBool_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBool_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBool_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBool_respond(&respond_,ticket,&response).error;}
};
/** Complete custom abb_robot_msgs/GetRAPIDDnum client; default policy returns13 from construction. */
class Ros1Abb_robot_msgsGetRAPIDDnumClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Abb_robot_msgsGetRAPIDDnumRequest;using Response=Ros1Abb_robot_msgsGetRAPIDDnumResponse;
Ros1Abb_robot_msgsGetRAPIDDnumClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Abb_robot_msgsGetRAPIDDnumClient(Ros1Abb_robot_msgsGetRAPIDDnumClient const&)=delete;Ros1Abb_robot_msgsGetRAPIDDnumClient& operator=(Ros1Abb_robot_msgsGetRAPIDDnumClient const&)=delete;Ros1Abb_robot_msgsGetRAPIDDnumClient(Ros1Abb_robot_msgsGetRAPIDDnumClient&&)=delete;
~Ros1Abb_robot_msgsGetRAPIDDnumClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnum_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom abb_robot_msgs/GetRAPIDDnum server; advertise/respond stay separate authorities. */
class Ros1Abb_robot_msgsGetRAPIDDnumServer:public ServerOwners {
using Handler=std::function<Ros1Abb_robot_msgsGetRAPIDDnumResponse(Ros1Abb_robot_msgsGetRAPIDDnumRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Abb_robot_msgsGetRAPIDDnumRequest;using Response=Ros1Abb_robot_msgsGetRAPIDDnumResponse;
Ros1Abb_robot_msgsGetRAPIDDnumServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnum_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnum_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnum_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnum_respond(&respond_,ticket,&response).error;}
};
/** Complete custom abb_robot_msgs/GetRAPIDNum client; default policy returns13 from construction. */
class Ros1Abb_robot_msgsGetRAPIDNumClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Abb_robot_msgsGetRAPIDNumRequest;using Response=Ros1Abb_robot_msgsGetRAPIDNumResponse;
Ros1Abb_robot_msgsGetRAPIDNumClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Abb_robot_msgsGetRAPIDNumClient(Ros1Abb_robot_msgsGetRAPIDNumClient const&)=delete;Ros1Abb_robot_msgsGetRAPIDNumClient& operator=(Ros1Abb_robot_msgsGetRAPIDNumClient const&)=delete;Ros1Abb_robot_msgsGetRAPIDNumClient(Ros1Abb_robot_msgsGetRAPIDNumClient&&)=delete;
~Ros1Abb_robot_msgsGetRAPIDNumClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNum_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom abb_robot_msgs/GetRAPIDNum server; advertise/respond stay separate authorities. */
class Ros1Abb_robot_msgsGetRAPIDNumServer:public ServerOwners {
using Handler=std::function<Ros1Abb_robot_msgsGetRAPIDNumResponse(Ros1Abb_robot_msgsGetRAPIDNumRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Abb_robot_msgsGetRAPIDNumRequest;using Response=Ros1Abb_robot_msgsGetRAPIDNumResponse;
Ros1Abb_robot_msgsGetRAPIDNumServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNum_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNum_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNum_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNum_respond(&respond_,ticket,&response).error;}
};
/** Complete custom abb_robot_msgs/GetRAPIDString client; default policy returns13 from construction. */
class Ros1Abb_robot_msgsGetRAPIDStringClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Abb_robot_msgsGetRAPIDStringRequest;using Response=Ros1Abb_robot_msgsGetRAPIDStringResponse;
Ros1Abb_robot_msgsGetRAPIDStringClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Abb_robot_msgsGetRAPIDStringClient(Ros1Abb_robot_msgsGetRAPIDStringClient const&)=delete;Ros1Abb_robot_msgsGetRAPIDStringClient& operator=(Ros1Abb_robot_msgsGetRAPIDStringClient const&)=delete;Ros1Abb_robot_msgsGetRAPIDStringClient(Ros1Abb_robot_msgsGetRAPIDStringClient&&)=delete;
~Ros1Abb_robot_msgsGetRAPIDStringClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_abb_robot_msgs_GetRAPIDString_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom abb_robot_msgs/GetRAPIDString server; advertise/respond stay separate authorities. */
class Ros1Abb_robot_msgsGetRAPIDStringServer:public ServerOwners {
using Handler=std::function<Ros1Abb_robot_msgsGetRAPIDStringResponse(Ros1Abb_robot_msgsGetRAPIDStringRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Abb_robot_msgsGetRAPIDStringRequest;using Response=Ros1Abb_robot_msgsGetRAPIDStringResponse;
Ros1Abb_robot_msgsGetRAPIDStringServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_abb_robot_msgs_GetRAPIDString_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_abb_robot_msgs_GetRAPIDString_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_abb_robot_msgs_GetRAPIDString_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_abb_robot_msgs_GetRAPIDString_respond(&respond_,ticket,&response).error;}
};
/** Complete custom abb_robot_msgs/GetRAPIDSymbol client; default policy returns13 from construction. */
class Ros1Abb_robot_msgsGetRAPIDSymbolClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Abb_robot_msgsGetRAPIDSymbolRequest;using Response=Ros1Abb_robot_msgsGetRAPIDSymbolResponse;
Ros1Abb_robot_msgsGetRAPIDSymbolClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Abb_robot_msgsGetRAPIDSymbolClient(Ros1Abb_robot_msgsGetRAPIDSymbolClient const&)=delete;Ros1Abb_robot_msgsGetRAPIDSymbolClient& operator=(Ros1Abb_robot_msgsGetRAPIDSymbolClient const&)=delete;Ros1Abb_robot_msgsGetRAPIDSymbolClient(Ros1Abb_robot_msgsGetRAPIDSymbolClient&&)=delete;
~Ros1Abb_robot_msgsGetRAPIDSymbolClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbol_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom abb_robot_msgs/GetRAPIDSymbol server; advertise/respond stay separate authorities. */
class Ros1Abb_robot_msgsGetRAPIDSymbolServer:public ServerOwners {
using Handler=std::function<Ros1Abb_robot_msgsGetRAPIDSymbolResponse(Ros1Abb_robot_msgsGetRAPIDSymbolRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Abb_robot_msgsGetRAPIDSymbolRequest;using Response=Ros1Abb_robot_msgsGetRAPIDSymbolResponse;
Ros1Abb_robot_msgsGetRAPIDSymbolServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbol_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbol_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbol_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbol_respond(&respond_,ticket,&response).error;}
};
/** Complete custom abb_robot_msgs/GetRobotControllerDescription client; default policy returns13 from construction. */
class Ros1Abb_robot_msgsGetRobotControllerDescriptionClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Abb_robot_msgsGetRobotControllerDescriptionRequest;using Response=Ros1Abb_robot_msgsGetRobotControllerDescriptionResponse;
Ros1Abb_robot_msgsGetRobotControllerDescriptionClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Abb_robot_msgsGetRobotControllerDescriptionClient(Ros1Abb_robot_msgsGetRobotControllerDescriptionClient const&)=delete;Ros1Abb_robot_msgsGetRobotControllerDescriptionClient& operator=(Ros1Abb_robot_msgsGetRobotControllerDescriptionClient const&)=delete;Ros1Abb_robot_msgsGetRobotControllerDescriptionClient(Ros1Abb_robot_msgsGetRobotControllerDescriptionClient&&)=delete;
~Ros1Abb_robot_msgsGetRobotControllerDescriptionClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescription_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom abb_robot_msgs/GetRobotControllerDescription server; advertise/respond stay separate authorities. */
class Ros1Abb_robot_msgsGetRobotControllerDescriptionServer:public ServerOwners {
using Handler=std::function<Ros1Abb_robot_msgsGetRobotControllerDescriptionResponse(Ros1Abb_robot_msgsGetRobotControllerDescriptionRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Abb_robot_msgsGetRobotControllerDescriptionRequest;using Response=Ros1Abb_robot_msgsGetRobotControllerDescriptionResponse;
Ros1Abb_robot_msgsGetRobotControllerDescriptionServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescription_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescription_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescription_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescription_respond(&respond_,ticket,&response).error;}
};
/** Complete custom abb_robot_msgs/GetSpeedRatio client; default policy returns13 from construction. */
class Ros1Abb_robot_msgsGetSpeedRatioClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Abb_robot_msgsGetSpeedRatioRequest;using Response=Ros1Abb_robot_msgsGetSpeedRatioResponse;
Ros1Abb_robot_msgsGetSpeedRatioClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Abb_robot_msgsGetSpeedRatioClient(Ros1Abb_robot_msgsGetSpeedRatioClient const&)=delete;Ros1Abb_robot_msgsGetSpeedRatioClient& operator=(Ros1Abb_robot_msgsGetSpeedRatioClient const&)=delete;Ros1Abb_robot_msgsGetSpeedRatioClient(Ros1Abb_robot_msgsGetSpeedRatioClient&&)=delete;
~Ros1Abb_robot_msgsGetSpeedRatioClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatio_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom abb_robot_msgs/GetSpeedRatio server; advertise/respond stay separate authorities. */
class Ros1Abb_robot_msgsGetSpeedRatioServer:public ServerOwners {
using Handler=std::function<Ros1Abb_robot_msgsGetSpeedRatioResponse(Ros1Abb_robot_msgsGetSpeedRatioRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Abb_robot_msgsGetSpeedRatioRequest;using Response=Ros1Abb_robot_msgsGetSpeedRatioResponse;
Ros1Abb_robot_msgsGetSpeedRatioServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatio_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatio_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatio_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatio_respond(&respond_,ticket,&response).error;}
};
/** Complete custom abb_robot_msgs/SetFileContents client; default policy returns13 from construction. */
class Ros1Abb_robot_msgsSetFileContentsClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Abb_robot_msgsSetFileContentsRequest;using Response=Ros1Abb_robot_msgsSetFileContentsResponse;
Ros1Abb_robot_msgsSetFileContentsClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Abb_robot_msgsSetFileContentsClient(Ros1Abb_robot_msgsSetFileContentsClient const&)=delete;Ros1Abb_robot_msgsSetFileContentsClient& operator=(Ros1Abb_robot_msgsSetFileContentsClient const&)=delete;Ros1Abb_robot_msgsSetFileContentsClient(Ros1Abb_robot_msgsSetFileContentsClient&&)=delete;
~Ros1Abb_robot_msgsSetFileContentsClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_abb_robot_msgs_SetFileContents_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom abb_robot_msgs/SetFileContents server; advertise/respond stay separate authorities. */
class Ros1Abb_robot_msgsSetFileContentsServer:public ServerOwners {
using Handler=std::function<Ros1Abb_robot_msgsSetFileContentsResponse(Ros1Abb_robot_msgsSetFileContentsRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Abb_robot_msgsSetFileContentsRequest;using Response=Ros1Abb_robot_msgsSetFileContentsResponse;
Ros1Abb_robot_msgsSetFileContentsServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_abb_robot_msgs_SetFileContents_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_abb_robot_msgs_SetFileContents_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_abb_robot_msgs_SetFileContents_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_abb_robot_msgs_SetFileContents_respond(&respond_,ticket,&response).error;}
};
/** Complete custom abb_robot_msgs/SetIOSignal client; default policy returns13 from construction. */
class Ros1Abb_robot_msgsSetIOSignalClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Abb_robot_msgsSetIOSignalRequest;using Response=Ros1Abb_robot_msgsSetIOSignalResponse;
Ros1Abb_robot_msgsSetIOSignalClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Abb_robot_msgsSetIOSignalClient(Ros1Abb_robot_msgsSetIOSignalClient const&)=delete;Ros1Abb_robot_msgsSetIOSignalClient& operator=(Ros1Abb_robot_msgsSetIOSignalClient const&)=delete;Ros1Abb_robot_msgsSetIOSignalClient(Ros1Abb_robot_msgsSetIOSignalClient&&)=delete;
~Ros1Abb_robot_msgsSetIOSignalClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_abb_robot_msgs_SetIOSignal_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom abb_robot_msgs/SetIOSignal server; advertise/respond stay separate authorities. */
class Ros1Abb_robot_msgsSetIOSignalServer:public ServerOwners {
using Handler=std::function<Ros1Abb_robot_msgsSetIOSignalResponse(Ros1Abb_robot_msgsSetIOSignalRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Abb_robot_msgsSetIOSignalRequest;using Response=Ros1Abb_robot_msgsSetIOSignalResponse;
Ros1Abb_robot_msgsSetIOSignalServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_abb_robot_msgs_SetIOSignal_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_abb_robot_msgs_SetIOSignal_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_abb_robot_msgs_SetIOSignal_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_abb_robot_msgs_SetIOSignal_respond(&respond_,ticket,&response).error;}
};
/** Complete custom abb_robot_msgs/SetRAPIDBool client; default policy returns13 from construction. */
class Ros1Abb_robot_msgsSetRAPIDBoolClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Abb_robot_msgsSetRAPIDBoolRequest;using Response=Ros1Abb_robot_msgsSetRAPIDBoolResponse;
Ros1Abb_robot_msgsSetRAPIDBoolClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Abb_robot_msgsSetRAPIDBoolClient(Ros1Abb_robot_msgsSetRAPIDBoolClient const&)=delete;Ros1Abb_robot_msgsSetRAPIDBoolClient& operator=(Ros1Abb_robot_msgsSetRAPIDBoolClient const&)=delete;Ros1Abb_robot_msgsSetRAPIDBoolClient(Ros1Abb_robot_msgsSetRAPIDBoolClient&&)=delete;
~Ros1Abb_robot_msgsSetRAPIDBoolClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBool_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom abb_robot_msgs/SetRAPIDBool server; advertise/respond stay separate authorities. */
class Ros1Abb_robot_msgsSetRAPIDBoolServer:public ServerOwners {
using Handler=std::function<Ros1Abb_robot_msgsSetRAPIDBoolResponse(Ros1Abb_robot_msgsSetRAPIDBoolRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Abb_robot_msgsSetRAPIDBoolRequest;using Response=Ros1Abb_robot_msgsSetRAPIDBoolResponse;
Ros1Abb_robot_msgsSetRAPIDBoolServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBool_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBool_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBool_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBool_respond(&respond_,ticket,&response).error;}
};
/** Complete custom abb_robot_msgs/SetRAPIDDnum client; default policy returns13 from construction. */
class Ros1Abb_robot_msgsSetRAPIDDnumClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Abb_robot_msgsSetRAPIDDnumRequest;using Response=Ros1Abb_robot_msgsSetRAPIDDnumResponse;
Ros1Abb_robot_msgsSetRAPIDDnumClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Abb_robot_msgsSetRAPIDDnumClient(Ros1Abb_robot_msgsSetRAPIDDnumClient const&)=delete;Ros1Abb_robot_msgsSetRAPIDDnumClient& operator=(Ros1Abb_robot_msgsSetRAPIDDnumClient const&)=delete;Ros1Abb_robot_msgsSetRAPIDDnumClient(Ros1Abb_robot_msgsSetRAPIDDnumClient&&)=delete;
~Ros1Abb_robot_msgsSetRAPIDDnumClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnum_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom abb_robot_msgs/SetRAPIDDnum server; advertise/respond stay separate authorities. */
class Ros1Abb_robot_msgsSetRAPIDDnumServer:public ServerOwners {
using Handler=std::function<Ros1Abb_robot_msgsSetRAPIDDnumResponse(Ros1Abb_robot_msgsSetRAPIDDnumRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Abb_robot_msgsSetRAPIDDnumRequest;using Response=Ros1Abb_robot_msgsSetRAPIDDnumResponse;
Ros1Abb_robot_msgsSetRAPIDDnumServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnum_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnum_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnum_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnum_respond(&respond_,ticket,&response).error;}
};
/** Complete custom abb_robot_msgs/SetRAPIDNum client; default policy returns13 from construction. */
class Ros1Abb_robot_msgsSetRAPIDNumClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Abb_robot_msgsSetRAPIDNumRequest;using Response=Ros1Abb_robot_msgsSetRAPIDNumResponse;
Ros1Abb_robot_msgsSetRAPIDNumClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Abb_robot_msgsSetRAPIDNumClient(Ros1Abb_robot_msgsSetRAPIDNumClient const&)=delete;Ros1Abb_robot_msgsSetRAPIDNumClient& operator=(Ros1Abb_robot_msgsSetRAPIDNumClient const&)=delete;Ros1Abb_robot_msgsSetRAPIDNumClient(Ros1Abb_robot_msgsSetRAPIDNumClient&&)=delete;
~Ros1Abb_robot_msgsSetRAPIDNumClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNum_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom abb_robot_msgs/SetRAPIDNum server; advertise/respond stay separate authorities. */
class Ros1Abb_robot_msgsSetRAPIDNumServer:public ServerOwners {
using Handler=std::function<Ros1Abb_robot_msgsSetRAPIDNumResponse(Ros1Abb_robot_msgsSetRAPIDNumRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Abb_robot_msgsSetRAPIDNumRequest;using Response=Ros1Abb_robot_msgsSetRAPIDNumResponse;
Ros1Abb_robot_msgsSetRAPIDNumServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNum_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNum_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNum_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNum_respond(&respond_,ticket,&response).error;}
};
/** Complete custom abb_robot_msgs/SetRAPIDString client; default policy returns13 from construction. */
class Ros1Abb_robot_msgsSetRAPIDStringClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Abb_robot_msgsSetRAPIDStringRequest;using Response=Ros1Abb_robot_msgsSetRAPIDStringResponse;
Ros1Abb_robot_msgsSetRAPIDStringClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Abb_robot_msgsSetRAPIDStringClient(Ros1Abb_robot_msgsSetRAPIDStringClient const&)=delete;Ros1Abb_robot_msgsSetRAPIDStringClient& operator=(Ros1Abb_robot_msgsSetRAPIDStringClient const&)=delete;Ros1Abb_robot_msgsSetRAPIDStringClient(Ros1Abb_robot_msgsSetRAPIDStringClient&&)=delete;
~Ros1Abb_robot_msgsSetRAPIDStringClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_abb_robot_msgs_SetRAPIDString_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom abb_robot_msgs/SetRAPIDString server; advertise/respond stay separate authorities. */
class Ros1Abb_robot_msgsSetRAPIDStringServer:public ServerOwners {
using Handler=std::function<Ros1Abb_robot_msgsSetRAPIDStringResponse(Ros1Abb_robot_msgsSetRAPIDStringRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Abb_robot_msgsSetRAPIDStringRequest;using Response=Ros1Abb_robot_msgsSetRAPIDStringResponse;
Ros1Abb_robot_msgsSetRAPIDStringServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_abb_robot_msgs_SetRAPIDString_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_abb_robot_msgs_SetRAPIDString_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_abb_robot_msgs_SetRAPIDString_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_abb_robot_msgs_SetRAPIDString_respond(&respond_,ticket,&response).error;}
};
/** Complete custom abb_robot_msgs/SetRAPIDSymbol client; default policy returns13 from construction. */
class Ros1Abb_robot_msgsSetRAPIDSymbolClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Abb_robot_msgsSetRAPIDSymbolRequest;using Response=Ros1Abb_robot_msgsSetRAPIDSymbolResponse;
Ros1Abb_robot_msgsSetRAPIDSymbolClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Abb_robot_msgsSetRAPIDSymbolClient(Ros1Abb_robot_msgsSetRAPIDSymbolClient const&)=delete;Ros1Abb_robot_msgsSetRAPIDSymbolClient& operator=(Ros1Abb_robot_msgsSetRAPIDSymbolClient const&)=delete;Ros1Abb_robot_msgsSetRAPIDSymbolClient(Ros1Abb_robot_msgsSetRAPIDSymbolClient&&)=delete;
~Ros1Abb_robot_msgsSetRAPIDSymbolClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbol_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom abb_robot_msgs/SetRAPIDSymbol server; advertise/respond stay separate authorities. */
class Ros1Abb_robot_msgsSetRAPIDSymbolServer:public ServerOwners {
using Handler=std::function<Ros1Abb_robot_msgsSetRAPIDSymbolResponse(Ros1Abb_robot_msgsSetRAPIDSymbolRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Abb_robot_msgsSetRAPIDSymbolRequest;using Response=Ros1Abb_robot_msgsSetRAPIDSymbolResponse;
Ros1Abb_robot_msgsSetRAPIDSymbolServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbol_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbol_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbol_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbol_respond(&respond_,ticket,&response).error;}
};
/** Complete custom abb_robot_msgs/SetSpeedRatio client; default policy returns13 from construction. */
class Ros1Abb_robot_msgsSetSpeedRatioClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Abb_robot_msgsSetSpeedRatioRequest;using Response=Ros1Abb_robot_msgsSetSpeedRatioResponse;
Ros1Abb_robot_msgsSetSpeedRatioClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Abb_robot_msgsSetSpeedRatioClient(Ros1Abb_robot_msgsSetSpeedRatioClient const&)=delete;Ros1Abb_robot_msgsSetSpeedRatioClient& operator=(Ros1Abb_robot_msgsSetSpeedRatioClient const&)=delete;Ros1Abb_robot_msgsSetSpeedRatioClient(Ros1Abb_robot_msgsSetSpeedRatioClient&&)=delete;
~Ros1Abb_robot_msgsSetSpeedRatioClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatio_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom abb_robot_msgs/SetSpeedRatio server; advertise/respond stay separate authorities. */
class Ros1Abb_robot_msgsSetSpeedRatioServer:public ServerOwners {
using Handler=std::function<Ros1Abb_robot_msgsSetSpeedRatioResponse(Ros1Abb_robot_msgsSetSpeedRatioRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Abb_robot_msgsSetSpeedRatioRequest;using Response=Ros1Abb_robot_msgsSetSpeedRatioResponse;
Ros1Abb_robot_msgsSetSpeedRatioServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatio_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatio_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatio_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatio_respond(&respond_,ticket,&response).error;}
};
/** Complete custom abb_robot_msgs/TriggerWithResultCode client; default policy returns13 from construction. */
class Ros1Abb_robot_msgsTriggerWithResultCodeClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Abb_robot_msgsTriggerWithResultCodeRequest;using Response=Ros1Abb_robot_msgsTriggerWithResultCodeResponse;
Ros1Abb_robot_msgsTriggerWithResultCodeClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Abb_robot_msgsTriggerWithResultCodeClient(Ros1Abb_robot_msgsTriggerWithResultCodeClient const&)=delete;Ros1Abb_robot_msgsTriggerWithResultCodeClient& operator=(Ros1Abb_robot_msgsTriggerWithResultCodeClient const&)=delete;Ros1Abb_robot_msgsTriggerWithResultCodeClient(Ros1Abb_robot_msgsTriggerWithResultCodeClient&&)=delete;
~Ros1Abb_robot_msgsTriggerWithResultCodeClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCode_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom abb_robot_msgs/TriggerWithResultCode server; advertise/respond stay separate authorities. */
class Ros1Abb_robot_msgsTriggerWithResultCodeServer:public ServerOwners {
using Handler=std::function<Ros1Abb_robot_msgsTriggerWithResultCodeResponse(Ros1Abb_robot_msgsTriggerWithResultCodeRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Abb_robot_msgsTriggerWithResultCodeRequest;using Response=Ros1Abb_robot_msgsTriggerWithResultCodeResponse;
Ros1Abb_robot_msgsTriggerWithResultCodeServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCode_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCode_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCode_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCode_respond(&respond_,ticket,&response).error;}
};
/** Complete owned custom control_msgs/FollowJointTrajectory Action client with four separate roles. */
class Ros1Control_msgsFollowJointTrajectoryClient {roswasmc_context context_;std::array<uint64_t,1024> goals_{};roswasmc_action_bundle bundle_{};
public:
using Goal=Ros1Control_msgsFollowJointTrajectoryGoal;using Result=Ros1Control_msgsFollowJointTrajectoryResult;using Feedback=Ros1Control_msgsFollowJointTrajectoryFeedback;
Ros1Control_msgsFollowJointTrajectoryClient(roswasmc_context const& context,roswasmc_action_aliases const& aliases,size_t slots=8):context_{context.backend} {roswasmc_action_bundle_init(&context_,&bundle_,&aliases,goals_.data(),slots);}
Ros1Control_msgsFollowJointTrajectoryClient(Ros1Control_msgsFollowJointTrajectoryClient const&)=delete;Ros1Control_msgsFollowJointTrajectoryClient& operator=(Ros1Control_msgsFollowJointTrajectoryClient const&)=delete;Ros1Control_msgsFollowJointTrajectoryClient(Ros1Control_msgsFollowJointTrajectoryClient&&)=delete;
~Ros1Control_msgsFollowJointTrajectoryClient(){if(bundle_.active){std::array<uint32_t,5> statuses{};roswasmc_action_bundle_close_checked(&bundle_,0,statuses.data(),statuses.size());}}
uint32_t error() const noexcept{return bundle_.error?bundle_.error:bundle_.active?0:9;}bool is_open() const noexcept{return bundle_.active;}bool owns(uint64_t id)const{return roswasmc_action_bundle_owns(&bundle_,id);}
roswasmc_result send_goal(Goal const& value,uint64_t& id){return roswasmc_bounded_1_control_msgs_FollowJointTrajectory_send(&bundle_,&value,&id);}
roswasmc_result feedback(uint64_t id,Feedback& value){return roswasmc_bounded_1_control_msgs_FollowJointTrajectory_feedback(&bundle_,id,&value);}
roswasmc_result result(uint64_t id,uint8_t& kind,Result& value){return roswasmc_bounded_1_control_msgs_FollowJointTrajectory_result(&bundle_,id,&kind,&value);}
roswasmc_result cancel_own(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_own(&bundle_,id,&accepted);}
roswasmc_result cancel_any(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_any(&bundle_,id,&accepted);}
roswasmc_result cancel_all(int& accepted){return roswasmc_action_bundle_cancel_all(&bundle_,&accepted);}
roswasmc_cleanup_report close_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_close_checked(&bundle_,primary,statuses.data(),statuses.size());}
roswasmc_cleanup_report revoke_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_revoke_checked(&bundle_,primary,statuses.data(),statuses.size());}
};
/** Typed custom control_msgs/FollowJointTrajectory server with explicit feedback/terminal component codecs. */
class Ros1Control_msgsFollowJointTrajectoryServer:public ServerOwners {
public:
using Goal=Ros1Control_msgsFollowJointTrajectoryGoal;using Result=Ros1Control_msgsFollowJointTrajectoryResult;using Feedback=Ros1Control_msgsFollowJointTrajectoryFeedback;
/** Handler may choose separate feedback/result effects; kind must be2..4 when terminal. */
struct Replies {Feedback feedback{};Result result{};uint8_t kind{2};bool send_feedback{true},send_result{true};};
private:using Handler=std::function<Replies(uint64_t,Goal const&)>;std::shared_ptr<Handler> handler_;
public:
Ros1Control_msgsFollowJointTrajectoryServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,true} {}
void on_goal(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& id,Goal& goal){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_control_msgs_FollowJointTrajectory_poll(&advertise_,&id,&goal);}
roswasmc_result respond_feedback(uint64_t id,Feedback const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_control_msgs_FollowJointTrajectory_respond_feedback(&respond_,id,&value);}
roswasmc_result respond_result(uint64_t id,uint8_t kind,Result const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_control_msgs_FollowJointTrajectory_respond_result(&respond_,id,kind,&value);}
/** One pass, at most one feedback plus one terminal response; never implicit replay. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t id{};Goal goal{};auto r=roswasmc_bounded_1_control_msgs_FollowJointTrajectory_poll(&advertise_,&id,&goal);if(r.error)return r.error;auto replies=(*handler)(id,goal);if(replies.send_result&&(replies.kind<2||replies.kind>4||!roswasmc_bounded_1_control_msgs_FollowJointTrajectoryResult_valid(&replies.result)))return 22;if(replies.send_feedback&&!roswasmc_bounded_1_control_msgs_FollowJointTrajectoryFeedback_valid(&replies.feedback))return 22;if(replies.send_feedback){r=roswasmc_bounded_1_control_msgs_FollowJointTrajectory_respond_feedback(&respond_,id,&replies.feedback);if(r.error)return r.error;}return replies.send_result?roswasmc_bounded_1_control_msgs_FollowJointTrajectory_respond_result(&respond_,id,replies.kind,&replies.result).error:0;}
};
}
