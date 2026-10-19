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
/** Complete custom abb_rapid_sm_addin_msgs/srv/GetEGMSettings client; default policy returns13 from construction. */
class Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettingsClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Request;using Response=Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Response;
Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettingsClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettingsClient(Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettingsClient const&)=delete;Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettingsClient& operator=(Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettingsClient const&)=delete;Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettingsClient(Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettingsClient&&)=delete;
~Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettingsClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom abb_rapid_sm_addin_msgs/srv/GetEGMSettings server; advertise/respond stay separate authorities. */
class Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettingsServer:public ServerOwners {
using Handler=std::function<Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Response(Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Request;using Response=Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Response;
Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettingsServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_respond(&respond_,ticket,&response).error;}
};
/** Complete custom abb_rapid_sm_addin_msgs/srv/SetEGMSettings client; default policy returns13 from construction. */
class Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettingsClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Request;using Response=Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Response;
Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettingsClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettingsClient(Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettingsClient const&)=delete;Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettingsClient& operator=(Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettingsClient const&)=delete;Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettingsClient(Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettingsClient&&)=delete;
~Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettingsClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom abb_rapid_sm_addin_msgs/srv/SetEGMSettings server; advertise/respond stay separate authorities. */
class Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettingsServer:public ServerOwners {
using Handler=std::function<Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Response(Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Request;using Response=Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Response;
Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettingsServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_respond(&respond_,ticket,&response).error;}
};
/** Complete custom abb_rapid_sm_addin_msgs/srv/SetRAPIDRoutine client; default policy returns13 from construction. */
class Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutineClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Request;using Response=Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Response;
Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutineClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutineClient(Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutineClient const&)=delete;Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutineClient& operator=(Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutineClient const&)=delete;Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutineClient(Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutineClient&&)=delete;
~Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutineClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom abb_rapid_sm_addin_msgs/srv/SetRAPIDRoutine server; advertise/respond stay separate authorities. */
class Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutineServer:public ServerOwners {
using Handler=std::function<Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Response(Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Request;using Response=Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Response;
Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutineServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_respond(&respond_,ticket,&response).error;}
};
/** Complete custom abb_rapid_sm_addin_msgs/srv/SetSGCommand client; default policy returns13 from construction. */
class Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommandClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Request;using Response=Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Response;
Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommandClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommandClient(Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommandClient const&)=delete;Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommandClient& operator=(Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommandClient const&)=delete;Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommandClient(Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommandClient&&)=delete;
~Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommandClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom abb_rapid_sm_addin_msgs/srv/SetSGCommand server; advertise/respond stay separate authorities. */
class Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommandServer:public ServerOwners {
using Handler=std::function<Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Response(Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Request;using Response=Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Response;
Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommandServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_respond(&respond_,ticket,&response).error;}
};
/** Complete custom abb_robot_msgs/srv/GetFileContents client; default policy returns13 from construction. */
class Ros2Abb_robot_msgsSrvGetFileContentsClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Abb_robot_msgsSrvGetFileContents_Request;using Response=Ros2Abb_robot_msgsSrvGetFileContents_Response;
Ros2Abb_robot_msgsSrvGetFileContentsClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Abb_robot_msgsSrvGetFileContentsClient(Ros2Abb_robot_msgsSrvGetFileContentsClient const&)=delete;Ros2Abb_robot_msgsSrvGetFileContentsClient& operator=(Ros2Abb_robot_msgsSrvGetFileContentsClient const&)=delete;Ros2Abb_robot_msgsSrvGetFileContentsClient(Ros2Abb_robot_msgsSrvGetFileContentsClient&&)=delete;
~Ros2Abb_robot_msgsSrvGetFileContentsClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom abb_robot_msgs/srv/GetFileContents server; advertise/respond stay separate authorities. */
class Ros2Abb_robot_msgsSrvGetFileContentsServer:public ServerOwners {
using Handler=std::function<Ros2Abb_robot_msgsSrvGetFileContents_Response(Ros2Abb_robot_msgsSrvGetFileContents_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Abb_robot_msgsSrvGetFileContents_Request;using Response=Ros2Abb_robot_msgsSrvGetFileContents_Response;
Ros2Abb_robot_msgsSrvGetFileContentsServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_respond(&respond_,ticket,&response).error;}
};
/** Complete custom abb_robot_msgs/srv/GetIOSignal client; default policy returns13 from construction. */
class Ros2Abb_robot_msgsSrvGetIOSignalClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Abb_robot_msgsSrvGetIOSignal_Request;using Response=Ros2Abb_robot_msgsSrvGetIOSignal_Response;
Ros2Abb_robot_msgsSrvGetIOSignalClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Abb_robot_msgsSrvGetIOSignalClient(Ros2Abb_robot_msgsSrvGetIOSignalClient const&)=delete;Ros2Abb_robot_msgsSrvGetIOSignalClient& operator=(Ros2Abb_robot_msgsSrvGetIOSignalClient const&)=delete;Ros2Abb_robot_msgsSrvGetIOSignalClient(Ros2Abb_robot_msgsSrvGetIOSignalClient&&)=delete;
~Ros2Abb_robot_msgsSrvGetIOSignalClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom abb_robot_msgs/srv/GetIOSignal server; advertise/respond stay separate authorities. */
class Ros2Abb_robot_msgsSrvGetIOSignalServer:public ServerOwners {
using Handler=std::function<Ros2Abb_robot_msgsSrvGetIOSignal_Response(Ros2Abb_robot_msgsSrvGetIOSignal_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Abb_robot_msgsSrvGetIOSignal_Request;using Response=Ros2Abb_robot_msgsSrvGetIOSignal_Response;
Ros2Abb_robot_msgsSrvGetIOSignalServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_respond(&respond_,ticket,&response).error;}
};
/** Complete custom abb_robot_msgs/srv/GetRAPIDBool client; default policy returns13 from construction. */
class Ros2Abb_robot_msgsSrvGetRAPIDBoolClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Abb_robot_msgsSrvGetRAPIDBool_Request;using Response=Ros2Abb_robot_msgsSrvGetRAPIDBool_Response;
Ros2Abb_robot_msgsSrvGetRAPIDBoolClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Abb_robot_msgsSrvGetRAPIDBoolClient(Ros2Abb_robot_msgsSrvGetRAPIDBoolClient const&)=delete;Ros2Abb_robot_msgsSrvGetRAPIDBoolClient& operator=(Ros2Abb_robot_msgsSrvGetRAPIDBoolClient const&)=delete;Ros2Abb_robot_msgsSrvGetRAPIDBoolClient(Ros2Abb_robot_msgsSrvGetRAPIDBoolClient&&)=delete;
~Ros2Abb_robot_msgsSrvGetRAPIDBoolClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom abb_robot_msgs/srv/GetRAPIDBool server; advertise/respond stay separate authorities. */
class Ros2Abb_robot_msgsSrvGetRAPIDBoolServer:public ServerOwners {
using Handler=std::function<Ros2Abb_robot_msgsSrvGetRAPIDBool_Response(Ros2Abb_robot_msgsSrvGetRAPIDBool_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Abb_robot_msgsSrvGetRAPIDBool_Request;using Response=Ros2Abb_robot_msgsSrvGetRAPIDBool_Response;
Ros2Abb_robot_msgsSrvGetRAPIDBoolServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_respond(&respond_,ticket,&response).error;}
};
/** Complete custom abb_robot_msgs/srv/GetRAPIDDnum client; default policy returns13 from construction. */
class Ros2Abb_robot_msgsSrvGetRAPIDDnumClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Abb_robot_msgsSrvGetRAPIDDnum_Request;using Response=Ros2Abb_robot_msgsSrvGetRAPIDDnum_Response;
Ros2Abb_robot_msgsSrvGetRAPIDDnumClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Abb_robot_msgsSrvGetRAPIDDnumClient(Ros2Abb_robot_msgsSrvGetRAPIDDnumClient const&)=delete;Ros2Abb_robot_msgsSrvGetRAPIDDnumClient& operator=(Ros2Abb_robot_msgsSrvGetRAPIDDnumClient const&)=delete;Ros2Abb_robot_msgsSrvGetRAPIDDnumClient(Ros2Abb_robot_msgsSrvGetRAPIDDnumClient&&)=delete;
~Ros2Abb_robot_msgsSrvGetRAPIDDnumClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom abb_robot_msgs/srv/GetRAPIDDnum server; advertise/respond stay separate authorities. */
class Ros2Abb_robot_msgsSrvGetRAPIDDnumServer:public ServerOwners {
using Handler=std::function<Ros2Abb_robot_msgsSrvGetRAPIDDnum_Response(Ros2Abb_robot_msgsSrvGetRAPIDDnum_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Abb_robot_msgsSrvGetRAPIDDnum_Request;using Response=Ros2Abb_robot_msgsSrvGetRAPIDDnum_Response;
Ros2Abb_robot_msgsSrvGetRAPIDDnumServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_respond(&respond_,ticket,&response).error;}
};
/** Complete custom abb_robot_msgs/srv/GetRAPIDNum client; default policy returns13 from construction. */
class Ros2Abb_robot_msgsSrvGetRAPIDNumClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Abb_robot_msgsSrvGetRAPIDNum_Request;using Response=Ros2Abb_robot_msgsSrvGetRAPIDNum_Response;
Ros2Abb_robot_msgsSrvGetRAPIDNumClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Abb_robot_msgsSrvGetRAPIDNumClient(Ros2Abb_robot_msgsSrvGetRAPIDNumClient const&)=delete;Ros2Abb_robot_msgsSrvGetRAPIDNumClient& operator=(Ros2Abb_robot_msgsSrvGetRAPIDNumClient const&)=delete;Ros2Abb_robot_msgsSrvGetRAPIDNumClient(Ros2Abb_robot_msgsSrvGetRAPIDNumClient&&)=delete;
~Ros2Abb_robot_msgsSrvGetRAPIDNumClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom abb_robot_msgs/srv/GetRAPIDNum server; advertise/respond stay separate authorities. */
class Ros2Abb_robot_msgsSrvGetRAPIDNumServer:public ServerOwners {
using Handler=std::function<Ros2Abb_robot_msgsSrvGetRAPIDNum_Response(Ros2Abb_robot_msgsSrvGetRAPIDNum_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Abb_robot_msgsSrvGetRAPIDNum_Request;using Response=Ros2Abb_robot_msgsSrvGetRAPIDNum_Response;
Ros2Abb_robot_msgsSrvGetRAPIDNumServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_respond(&respond_,ticket,&response).error;}
};
/** Complete custom abb_robot_msgs/srv/GetRAPIDString client; default policy returns13 from construction. */
class Ros2Abb_robot_msgsSrvGetRAPIDStringClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Abb_robot_msgsSrvGetRAPIDString_Request;using Response=Ros2Abb_robot_msgsSrvGetRAPIDString_Response;
Ros2Abb_robot_msgsSrvGetRAPIDStringClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Abb_robot_msgsSrvGetRAPIDStringClient(Ros2Abb_robot_msgsSrvGetRAPIDStringClient const&)=delete;Ros2Abb_robot_msgsSrvGetRAPIDStringClient& operator=(Ros2Abb_robot_msgsSrvGetRAPIDStringClient const&)=delete;Ros2Abb_robot_msgsSrvGetRAPIDStringClient(Ros2Abb_robot_msgsSrvGetRAPIDStringClient&&)=delete;
~Ros2Abb_robot_msgsSrvGetRAPIDStringClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom abb_robot_msgs/srv/GetRAPIDString server; advertise/respond stay separate authorities. */
class Ros2Abb_robot_msgsSrvGetRAPIDStringServer:public ServerOwners {
using Handler=std::function<Ros2Abb_robot_msgsSrvGetRAPIDString_Response(Ros2Abb_robot_msgsSrvGetRAPIDString_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Abb_robot_msgsSrvGetRAPIDString_Request;using Response=Ros2Abb_robot_msgsSrvGetRAPIDString_Response;
Ros2Abb_robot_msgsSrvGetRAPIDStringServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_respond(&respond_,ticket,&response).error;}
};
/** Complete custom abb_robot_msgs/srv/GetRAPIDSymbol client; default policy returns13 from construction. */
class Ros2Abb_robot_msgsSrvGetRAPIDSymbolClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Request;using Response=Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Response;
Ros2Abb_robot_msgsSrvGetRAPIDSymbolClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Abb_robot_msgsSrvGetRAPIDSymbolClient(Ros2Abb_robot_msgsSrvGetRAPIDSymbolClient const&)=delete;Ros2Abb_robot_msgsSrvGetRAPIDSymbolClient& operator=(Ros2Abb_robot_msgsSrvGetRAPIDSymbolClient const&)=delete;Ros2Abb_robot_msgsSrvGetRAPIDSymbolClient(Ros2Abb_robot_msgsSrvGetRAPIDSymbolClient&&)=delete;
~Ros2Abb_robot_msgsSrvGetRAPIDSymbolClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom abb_robot_msgs/srv/GetRAPIDSymbol server; advertise/respond stay separate authorities. */
class Ros2Abb_robot_msgsSrvGetRAPIDSymbolServer:public ServerOwners {
using Handler=std::function<Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Response(Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Request;using Response=Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Response;
Ros2Abb_robot_msgsSrvGetRAPIDSymbolServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_respond(&respond_,ticket,&response).error;}
};
/** Complete custom abb_robot_msgs/srv/GetRobotControllerDescription client; default policy returns13 from construction. */
class Ros2Abb_robot_msgsSrvGetRobotControllerDescriptionClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Request;using Response=Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Response;
Ros2Abb_robot_msgsSrvGetRobotControllerDescriptionClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Abb_robot_msgsSrvGetRobotControllerDescriptionClient(Ros2Abb_robot_msgsSrvGetRobotControllerDescriptionClient const&)=delete;Ros2Abb_robot_msgsSrvGetRobotControllerDescriptionClient& operator=(Ros2Abb_robot_msgsSrvGetRobotControllerDescriptionClient const&)=delete;Ros2Abb_robot_msgsSrvGetRobotControllerDescriptionClient(Ros2Abb_robot_msgsSrvGetRobotControllerDescriptionClient&&)=delete;
~Ros2Abb_robot_msgsSrvGetRobotControllerDescriptionClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom abb_robot_msgs/srv/GetRobotControllerDescription server; advertise/respond stay separate authorities. */
class Ros2Abb_robot_msgsSrvGetRobotControllerDescriptionServer:public ServerOwners {
using Handler=std::function<Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Response(Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Request;using Response=Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Response;
Ros2Abb_robot_msgsSrvGetRobotControllerDescriptionServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_respond(&respond_,ticket,&response).error;}
};
/** Complete custom abb_robot_msgs/srv/GetSpeedRatio client; default policy returns13 from construction. */
class Ros2Abb_robot_msgsSrvGetSpeedRatioClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Abb_robot_msgsSrvGetSpeedRatio_Request;using Response=Ros2Abb_robot_msgsSrvGetSpeedRatio_Response;
Ros2Abb_robot_msgsSrvGetSpeedRatioClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Abb_robot_msgsSrvGetSpeedRatioClient(Ros2Abb_robot_msgsSrvGetSpeedRatioClient const&)=delete;Ros2Abb_robot_msgsSrvGetSpeedRatioClient& operator=(Ros2Abb_robot_msgsSrvGetSpeedRatioClient const&)=delete;Ros2Abb_robot_msgsSrvGetSpeedRatioClient(Ros2Abb_robot_msgsSrvGetSpeedRatioClient&&)=delete;
~Ros2Abb_robot_msgsSrvGetSpeedRatioClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom abb_robot_msgs/srv/GetSpeedRatio server; advertise/respond stay separate authorities. */
class Ros2Abb_robot_msgsSrvGetSpeedRatioServer:public ServerOwners {
using Handler=std::function<Ros2Abb_robot_msgsSrvGetSpeedRatio_Response(Ros2Abb_robot_msgsSrvGetSpeedRatio_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Abb_robot_msgsSrvGetSpeedRatio_Request;using Response=Ros2Abb_robot_msgsSrvGetSpeedRatio_Response;
Ros2Abb_robot_msgsSrvGetSpeedRatioServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_respond(&respond_,ticket,&response).error;}
};
/** Complete custom abb_robot_msgs/srv/SetFileContents client; default policy returns13 from construction. */
class Ros2Abb_robot_msgsSrvSetFileContentsClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Abb_robot_msgsSrvSetFileContents_Request;using Response=Ros2Abb_robot_msgsSrvSetFileContents_Response;
Ros2Abb_robot_msgsSrvSetFileContentsClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Abb_robot_msgsSrvSetFileContentsClient(Ros2Abb_robot_msgsSrvSetFileContentsClient const&)=delete;Ros2Abb_robot_msgsSrvSetFileContentsClient& operator=(Ros2Abb_robot_msgsSrvSetFileContentsClient const&)=delete;Ros2Abb_robot_msgsSrvSetFileContentsClient(Ros2Abb_robot_msgsSrvSetFileContentsClient&&)=delete;
~Ros2Abb_robot_msgsSrvSetFileContentsClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom abb_robot_msgs/srv/SetFileContents server; advertise/respond stay separate authorities. */
class Ros2Abb_robot_msgsSrvSetFileContentsServer:public ServerOwners {
using Handler=std::function<Ros2Abb_robot_msgsSrvSetFileContents_Response(Ros2Abb_robot_msgsSrvSetFileContents_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Abb_robot_msgsSrvSetFileContents_Request;using Response=Ros2Abb_robot_msgsSrvSetFileContents_Response;
Ros2Abb_robot_msgsSrvSetFileContentsServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_respond(&respond_,ticket,&response).error;}
};
/** Complete custom abb_robot_msgs/srv/SetIOSignal client; default policy returns13 from construction. */
class Ros2Abb_robot_msgsSrvSetIOSignalClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Abb_robot_msgsSrvSetIOSignal_Request;using Response=Ros2Abb_robot_msgsSrvSetIOSignal_Response;
Ros2Abb_robot_msgsSrvSetIOSignalClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Abb_robot_msgsSrvSetIOSignalClient(Ros2Abb_robot_msgsSrvSetIOSignalClient const&)=delete;Ros2Abb_robot_msgsSrvSetIOSignalClient& operator=(Ros2Abb_robot_msgsSrvSetIOSignalClient const&)=delete;Ros2Abb_robot_msgsSrvSetIOSignalClient(Ros2Abb_robot_msgsSrvSetIOSignalClient&&)=delete;
~Ros2Abb_robot_msgsSrvSetIOSignalClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom abb_robot_msgs/srv/SetIOSignal server; advertise/respond stay separate authorities. */
class Ros2Abb_robot_msgsSrvSetIOSignalServer:public ServerOwners {
using Handler=std::function<Ros2Abb_robot_msgsSrvSetIOSignal_Response(Ros2Abb_robot_msgsSrvSetIOSignal_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Abb_robot_msgsSrvSetIOSignal_Request;using Response=Ros2Abb_robot_msgsSrvSetIOSignal_Response;
Ros2Abb_robot_msgsSrvSetIOSignalServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_respond(&respond_,ticket,&response).error;}
};
/** Complete custom abb_robot_msgs/srv/SetRAPIDBool client; default policy returns13 from construction. */
class Ros2Abb_robot_msgsSrvSetRAPIDBoolClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Abb_robot_msgsSrvSetRAPIDBool_Request;using Response=Ros2Abb_robot_msgsSrvSetRAPIDBool_Response;
Ros2Abb_robot_msgsSrvSetRAPIDBoolClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Abb_robot_msgsSrvSetRAPIDBoolClient(Ros2Abb_robot_msgsSrvSetRAPIDBoolClient const&)=delete;Ros2Abb_robot_msgsSrvSetRAPIDBoolClient& operator=(Ros2Abb_robot_msgsSrvSetRAPIDBoolClient const&)=delete;Ros2Abb_robot_msgsSrvSetRAPIDBoolClient(Ros2Abb_robot_msgsSrvSetRAPIDBoolClient&&)=delete;
~Ros2Abb_robot_msgsSrvSetRAPIDBoolClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom abb_robot_msgs/srv/SetRAPIDBool server; advertise/respond stay separate authorities. */
class Ros2Abb_robot_msgsSrvSetRAPIDBoolServer:public ServerOwners {
using Handler=std::function<Ros2Abb_robot_msgsSrvSetRAPIDBool_Response(Ros2Abb_robot_msgsSrvSetRAPIDBool_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Abb_robot_msgsSrvSetRAPIDBool_Request;using Response=Ros2Abb_robot_msgsSrvSetRAPIDBool_Response;
Ros2Abb_robot_msgsSrvSetRAPIDBoolServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_respond(&respond_,ticket,&response).error;}
};
/** Complete custom abb_robot_msgs/srv/SetRAPIDDnum client; default policy returns13 from construction. */
class Ros2Abb_robot_msgsSrvSetRAPIDDnumClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Abb_robot_msgsSrvSetRAPIDDnum_Request;using Response=Ros2Abb_robot_msgsSrvSetRAPIDDnum_Response;
Ros2Abb_robot_msgsSrvSetRAPIDDnumClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Abb_robot_msgsSrvSetRAPIDDnumClient(Ros2Abb_robot_msgsSrvSetRAPIDDnumClient const&)=delete;Ros2Abb_robot_msgsSrvSetRAPIDDnumClient& operator=(Ros2Abb_robot_msgsSrvSetRAPIDDnumClient const&)=delete;Ros2Abb_robot_msgsSrvSetRAPIDDnumClient(Ros2Abb_robot_msgsSrvSetRAPIDDnumClient&&)=delete;
~Ros2Abb_robot_msgsSrvSetRAPIDDnumClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom abb_robot_msgs/srv/SetRAPIDDnum server; advertise/respond stay separate authorities. */
class Ros2Abb_robot_msgsSrvSetRAPIDDnumServer:public ServerOwners {
using Handler=std::function<Ros2Abb_robot_msgsSrvSetRAPIDDnum_Response(Ros2Abb_robot_msgsSrvSetRAPIDDnum_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Abb_robot_msgsSrvSetRAPIDDnum_Request;using Response=Ros2Abb_robot_msgsSrvSetRAPIDDnum_Response;
Ros2Abb_robot_msgsSrvSetRAPIDDnumServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_respond(&respond_,ticket,&response).error;}
};
/** Complete custom abb_robot_msgs/srv/SetRAPIDNum client; default policy returns13 from construction. */
class Ros2Abb_robot_msgsSrvSetRAPIDNumClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Abb_robot_msgsSrvSetRAPIDNum_Request;using Response=Ros2Abb_robot_msgsSrvSetRAPIDNum_Response;
Ros2Abb_robot_msgsSrvSetRAPIDNumClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Abb_robot_msgsSrvSetRAPIDNumClient(Ros2Abb_robot_msgsSrvSetRAPIDNumClient const&)=delete;Ros2Abb_robot_msgsSrvSetRAPIDNumClient& operator=(Ros2Abb_robot_msgsSrvSetRAPIDNumClient const&)=delete;Ros2Abb_robot_msgsSrvSetRAPIDNumClient(Ros2Abb_robot_msgsSrvSetRAPIDNumClient&&)=delete;
~Ros2Abb_robot_msgsSrvSetRAPIDNumClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom abb_robot_msgs/srv/SetRAPIDNum server; advertise/respond stay separate authorities. */
class Ros2Abb_robot_msgsSrvSetRAPIDNumServer:public ServerOwners {
using Handler=std::function<Ros2Abb_robot_msgsSrvSetRAPIDNum_Response(Ros2Abb_robot_msgsSrvSetRAPIDNum_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Abb_robot_msgsSrvSetRAPIDNum_Request;using Response=Ros2Abb_robot_msgsSrvSetRAPIDNum_Response;
Ros2Abb_robot_msgsSrvSetRAPIDNumServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_respond(&respond_,ticket,&response).error;}
};
/** Complete custom abb_robot_msgs/srv/SetRAPIDString client; default policy returns13 from construction. */
class Ros2Abb_robot_msgsSrvSetRAPIDStringClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Abb_robot_msgsSrvSetRAPIDString_Request;using Response=Ros2Abb_robot_msgsSrvSetRAPIDString_Response;
Ros2Abb_robot_msgsSrvSetRAPIDStringClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Abb_robot_msgsSrvSetRAPIDStringClient(Ros2Abb_robot_msgsSrvSetRAPIDStringClient const&)=delete;Ros2Abb_robot_msgsSrvSetRAPIDStringClient& operator=(Ros2Abb_robot_msgsSrvSetRAPIDStringClient const&)=delete;Ros2Abb_robot_msgsSrvSetRAPIDStringClient(Ros2Abb_robot_msgsSrvSetRAPIDStringClient&&)=delete;
~Ros2Abb_robot_msgsSrvSetRAPIDStringClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom abb_robot_msgs/srv/SetRAPIDString server; advertise/respond stay separate authorities. */
class Ros2Abb_robot_msgsSrvSetRAPIDStringServer:public ServerOwners {
using Handler=std::function<Ros2Abb_robot_msgsSrvSetRAPIDString_Response(Ros2Abb_robot_msgsSrvSetRAPIDString_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Abb_robot_msgsSrvSetRAPIDString_Request;using Response=Ros2Abb_robot_msgsSrvSetRAPIDString_Response;
Ros2Abb_robot_msgsSrvSetRAPIDStringServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_respond(&respond_,ticket,&response).error;}
};
/** Complete custom abb_robot_msgs/srv/SetRAPIDSymbol client; default policy returns13 from construction. */
class Ros2Abb_robot_msgsSrvSetRAPIDSymbolClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Request;using Response=Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Response;
Ros2Abb_robot_msgsSrvSetRAPIDSymbolClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Abb_robot_msgsSrvSetRAPIDSymbolClient(Ros2Abb_robot_msgsSrvSetRAPIDSymbolClient const&)=delete;Ros2Abb_robot_msgsSrvSetRAPIDSymbolClient& operator=(Ros2Abb_robot_msgsSrvSetRAPIDSymbolClient const&)=delete;Ros2Abb_robot_msgsSrvSetRAPIDSymbolClient(Ros2Abb_robot_msgsSrvSetRAPIDSymbolClient&&)=delete;
~Ros2Abb_robot_msgsSrvSetRAPIDSymbolClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom abb_robot_msgs/srv/SetRAPIDSymbol server; advertise/respond stay separate authorities. */
class Ros2Abb_robot_msgsSrvSetRAPIDSymbolServer:public ServerOwners {
using Handler=std::function<Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Response(Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Request;using Response=Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Response;
Ros2Abb_robot_msgsSrvSetRAPIDSymbolServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_respond(&respond_,ticket,&response).error;}
};
/** Complete custom abb_robot_msgs/srv/SetSpeedRatio client; default policy returns13 from construction. */
class Ros2Abb_robot_msgsSrvSetSpeedRatioClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Abb_robot_msgsSrvSetSpeedRatio_Request;using Response=Ros2Abb_robot_msgsSrvSetSpeedRatio_Response;
Ros2Abb_robot_msgsSrvSetSpeedRatioClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Abb_robot_msgsSrvSetSpeedRatioClient(Ros2Abb_robot_msgsSrvSetSpeedRatioClient const&)=delete;Ros2Abb_robot_msgsSrvSetSpeedRatioClient& operator=(Ros2Abb_robot_msgsSrvSetSpeedRatioClient const&)=delete;Ros2Abb_robot_msgsSrvSetSpeedRatioClient(Ros2Abb_robot_msgsSrvSetSpeedRatioClient&&)=delete;
~Ros2Abb_robot_msgsSrvSetSpeedRatioClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom abb_robot_msgs/srv/SetSpeedRatio server; advertise/respond stay separate authorities. */
class Ros2Abb_robot_msgsSrvSetSpeedRatioServer:public ServerOwners {
using Handler=std::function<Ros2Abb_robot_msgsSrvSetSpeedRatio_Response(Ros2Abb_robot_msgsSrvSetSpeedRatio_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Abb_robot_msgsSrvSetSpeedRatio_Request;using Response=Ros2Abb_robot_msgsSrvSetSpeedRatio_Response;
Ros2Abb_robot_msgsSrvSetSpeedRatioServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_respond(&respond_,ticket,&response).error;}
};
/** Complete custom abb_robot_msgs/srv/TriggerWithResultCode client; default policy returns13 from construction. */
class Ros2Abb_robot_msgsSrvTriggerWithResultCodeClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Abb_robot_msgsSrvTriggerWithResultCode_Request;using Response=Ros2Abb_robot_msgsSrvTriggerWithResultCode_Response;
Ros2Abb_robot_msgsSrvTriggerWithResultCodeClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Abb_robot_msgsSrvTriggerWithResultCodeClient(Ros2Abb_robot_msgsSrvTriggerWithResultCodeClient const&)=delete;Ros2Abb_robot_msgsSrvTriggerWithResultCodeClient& operator=(Ros2Abb_robot_msgsSrvTriggerWithResultCodeClient const&)=delete;Ros2Abb_robot_msgsSrvTriggerWithResultCodeClient(Ros2Abb_robot_msgsSrvTriggerWithResultCodeClient&&)=delete;
~Ros2Abb_robot_msgsSrvTriggerWithResultCodeClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom abb_robot_msgs/srv/TriggerWithResultCode server; advertise/respond stay separate authorities. */
class Ros2Abb_robot_msgsSrvTriggerWithResultCodeServer:public ServerOwners {
using Handler=std::function<Ros2Abb_robot_msgsSrvTriggerWithResultCode_Response(Ros2Abb_robot_msgsSrvTriggerWithResultCode_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Abb_robot_msgsSrvTriggerWithResultCode_Request;using Response=Ros2Abb_robot_msgsSrvTriggerWithResultCode_Response;
Ros2Abb_robot_msgsSrvTriggerWithResultCodeServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_respond(&respond_,ticket,&response).error;}
};
/** Complete owned custom control_msgs/action/FollowJointTrajectory Action client with four separate roles. */
class Ros2Control_msgsActionFollowJointTrajectoryClient {roswasmc_context context_;std::array<uint64_t,1024> goals_{};roswasmc_action_bundle bundle_{};
public:
using Goal=Ros2Control_msgsActionFollowJointTrajectory_Goal;using Result=Ros2Control_msgsActionFollowJointTrajectory_Result;using Feedback=Ros2Control_msgsActionFollowJointTrajectory_Feedback;
Ros2Control_msgsActionFollowJointTrajectoryClient(roswasmc_context const& context,roswasmc_action_aliases const& aliases,size_t slots=8):context_{context.backend} {roswasmc_action_bundle_init(&context_,&bundle_,&aliases,goals_.data(),slots);}
Ros2Control_msgsActionFollowJointTrajectoryClient(Ros2Control_msgsActionFollowJointTrajectoryClient const&)=delete;Ros2Control_msgsActionFollowJointTrajectoryClient& operator=(Ros2Control_msgsActionFollowJointTrajectoryClient const&)=delete;Ros2Control_msgsActionFollowJointTrajectoryClient(Ros2Control_msgsActionFollowJointTrajectoryClient&&)=delete;
~Ros2Control_msgsActionFollowJointTrajectoryClient(){if(bundle_.active){std::array<uint32_t,5> statuses{};roswasmc_action_bundle_close_checked(&bundle_,0,statuses.data(),statuses.size());}}
uint32_t error() const noexcept{return bundle_.error?bundle_.error:bundle_.active?0:9;}bool is_open() const noexcept{return bundle_.active;}bool owns(uint64_t id)const{return roswasmc_action_bundle_owns(&bundle_,id);}
roswasmc_result send_goal(Goal const& value,uint64_t& id){return roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_send(&bundle_,&value,&id);}
roswasmc_result feedback(uint64_t id,Feedback& value){return roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_feedback(&bundle_,id,&value);}
roswasmc_result result(uint64_t id,uint8_t& kind,Result& value){return roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_result(&bundle_,id,&kind,&value);}
roswasmc_result cancel_own(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_own(&bundle_,id,&accepted);}
roswasmc_result cancel_any(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_any(&bundle_,id,&accepted);}
roswasmc_result cancel_all(int& accepted){return roswasmc_action_bundle_cancel_all(&bundle_,&accepted);}
roswasmc_cleanup_report close_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_close_checked(&bundle_,primary,statuses.data(),statuses.size());}
roswasmc_cleanup_report revoke_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_revoke_checked(&bundle_,primary,statuses.data(),statuses.size());}
};
/** Typed custom control_msgs/action/FollowJointTrajectory server with explicit feedback/terminal component codecs. */
class Ros2Control_msgsActionFollowJointTrajectoryServer:public ServerOwners {
public:
using Goal=Ros2Control_msgsActionFollowJointTrajectory_Goal;using Result=Ros2Control_msgsActionFollowJointTrajectory_Result;using Feedback=Ros2Control_msgsActionFollowJointTrajectory_Feedback;
/** Handler may choose separate feedback/result effects; kind must be2..4 when terminal. */
struct Replies {Feedback feedback{};Result result{};uint8_t kind{2};bool send_feedback{true},send_result{true};};
private:using Handler=std::function<Replies(uint64_t,Goal const&)>;std::shared_ptr<Handler> handler_;
public:
Ros2Control_msgsActionFollowJointTrajectoryServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,true} {}
void on_goal(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& id,Goal& goal){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_poll(&advertise_,&id,&goal);}
roswasmc_result respond_feedback(uint64_t id,Feedback const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_respond_feedback(&respond_,id,&value);}
roswasmc_result respond_result(uint64_t id,uint8_t kind,Result const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_respond_result(&respond_,id,kind,&value);}
/** One pass, at most one feedback plus one terminal response; never implicit replay. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t id{};Goal goal{};auto r=roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_poll(&advertise_,&id,&goal);if(r.error)return r.error;auto replies=(*handler)(id,goal);if(replies.send_result&&(replies.kind<2||replies.kind>4||!roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Result_valid(&replies.result)))return 22;if(replies.send_feedback&&!roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback_valid(&replies.feedback))return 22;if(replies.send_feedback){r=roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_respond_feedback(&respond_,id,&replies.feedback);if(r.error)return r.error;}return replies.send_result?roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_respond_result(&respond_,id,replies.kind,&replies.result).error:0;}
};
/** Complete custom std_srvs/srv/Trigger client; default policy returns13 from construction. */
class Ros2Std_srvsSrvTriggerClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Std_srvsSrvTrigger_Request;using Response=Ros2Std_srvsSrvTrigger_Response;
Ros2Std_srvsSrvTriggerClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Std_srvsSrvTriggerClient(Ros2Std_srvsSrvTriggerClient const&)=delete;Ros2Std_srvsSrvTriggerClient& operator=(Ros2Std_srvsSrvTriggerClient const&)=delete;Ros2Std_srvsSrvTriggerClient(Ros2Std_srvsSrvTriggerClient&&)=delete;
~Ros2Std_srvsSrvTriggerClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_std_srvs_srv_Trigger_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom std_srvs/srv/Trigger server; advertise/respond stay separate authorities. */
class Ros2Std_srvsSrvTriggerServer:public ServerOwners {
using Handler=std::function<Ros2Std_srvsSrvTrigger_Response(Ros2Std_srvsSrvTrigger_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Std_srvsSrvTrigger_Request;using Response=Ros2Std_srvsSrvTrigger_Response;
Ros2Std_srvsSrvTriggerServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_std_srvs_srv_Trigger_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_std_srvs_srv_Trigger_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_std_srvs_srv_Trigger_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_std_srvs_srv_Trigger_respond(&respond_,ticket,&response).error;}
};
}
