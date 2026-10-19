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
/** Complete owned custom urinterfaces/action/SetMode Action client with four separate roles. */
class Ros2UrinterfacesActionSetModeClient {roswasmc_context context_;std::array<uint64_t,1024> goals_{};roswasmc_action_bundle bundle_{};
public:
using Goal=Ros2UrinterfacesActionSetMode_Goal;using Result=Ros2UrinterfacesActionSetMode_Result;using Feedback=Ros2UrinterfacesActionSetMode_Feedback;
Ros2UrinterfacesActionSetModeClient(roswasmc_context const& context,roswasmc_action_aliases const& aliases,size_t slots=8):context_{context.backend} {roswasmc_action_bundle_init(&context_,&bundle_,&aliases,goals_.data(),slots);}
Ros2UrinterfacesActionSetModeClient(Ros2UrinterfacesActionSetModeClient const&)=delete;Ros2UrinterfacesActionSetModeClient& operator=(Ros2UrinterfacesActionSetModeClient const&)=delete;Ros2UrinterfacesActionSetModeClient(Ros2UrinterfacesActionSetModeClient&&)=delete;
~Ros2UrinterfacesActionSetModeClient(){if(bundle_.active){std::array<uint32_t,5> statuses{};roswasmc_action_bundle_close_checked(&bundle_,0,statuses.data(),statuses.size());}}
uint32_t error() const noexcept{return bundle_.error?bundle_.error:bundle_.active?0:9;}bool is_open() const noexcept{return bundle_.active;}bool owns(uint64_t id)const{return roswasmc_action_bundle_owns(&bundle_,id);}
roswasmc_result send_goal(Goal const& value,uint64_t& id){return roswasmc_bounded_2_urinterfaces_action_SetMode_send(&bundle_,&value,&id);}
roswasmc_result feedback(uint64_t id,Feedback& value){return roswasmc_bounded_2_urinterfaces_action_SetMode_feedback(&bundle_,id,&value);}
roswasmc_result result(uint64_t id,uint8_t& kind,Result& value){return roswasmc_bounded_2_urinterfaces_action_SetMode_result(&bundle_,id,&kind,&value);}
roswasmc_result cancel_own(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_own(&bundle_,id,&accepted);}
roswasmc_result cancel_any(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_any(&bundle_,id,&accepted);}
roswasmc_result cancel_all(int& accepted){return roswasmc_action_bundle_cancel_all(&bundle_,&accepted);}
roswasmc_cleanup_report close_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_close_checked(&bundle_,primary,statuses.data(),statuses.size());}
roswasmc_cleanup_report revoke_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_revoke_checked(&bundle_,primary,statuses.data(),statuses.size());}
};
/** Typed custom urinterfaces/action/SetMode server with explicit feedback/terminal component codecs. */
class Ros2UrinterfacesActionSetModeServer:public ServerOwners {
public:
using Goal=Ros2UrinterfacesActionSetMode_Goal;using Result=Ros2UrinterfacesActionSetMode_Result;using Feedback=Ros2UrinterfacesActionSetMode_Feedback;
/** Handler may choose separate feedback/result effects; kind must be2..4 when terminal. */
struct Replies {Feedback feedback{};Result result{};uint8_t kind{2};bool send_feedback{true},send_result{true};};
private:using Handler=std::function<Replies(uint64_t,Goal const&)>;std::shared_ptr<Handler> handler_;
public:
Ros2UrinterfacesActionSetModeServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,true} {}
void on_goal(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& id,Goal& goal){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_urinterfaces_action_SetMode_poll(&advertise_,&id,&goal);}
roswasmc_result respond_feedback(uint64_t id,Feedback const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_urinterfaces_action_SetMode_respond_feedback(&respond_,id,&value);}
roswasmc_result respond_result(uint64_t id,uint8_t kind,Result const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_urinterfaces_action_SetMode_respond_result(&respond_,id,kind,&value);}
/** One pass, at most one feedback plus one terminal response; never implicit replay. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t id{};Goal goal{};auto r=roswasmc_bounded_2_urinterfaces_action_SetMode_poll(&advertise_,&id,&goal);if(r.error)return r.error;auto replies=(*handler)(id,goal);if(replies.send_result&&(replies.kind<2||replies.kind>4||!roswasmc_bounded_2_urinterfaces_action_SetMode_Result_valid(&replies.result)))return 22;if(replies.send_feedback&&!roswasmc_bounded_2_urinterfaces_action_SetMode_Feedback_valid(&replies.feedback))return 22;if(replies.send_feedback){r=roswasmc_bounded_2_urinterfaces_action_SetMode_respond_feedback(&respond_,id,&replies.feedback);if(r.error)return r.error;}return replies.send_result?roswasmc_bounded_2_urinterfaces_action_SetMode_respond_result(&respond_,id,replies.kind,&replies.result).error:0;}
};
/** Complete custom urinterfaces/srv/SetAnalogOutput client; default policy returns13 from construction. */
class Ros2UrinterfacesSrvSetAnalogOutputClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2UrinterfacesSrvSetAnalogOutput_Request;using Response=Ros2UrinterfacesSrvSetAnalogOutput_Response;
Ros2UrinterfacesSrvSetAnalogOutputClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2UrinterfacesSrvSetAnalogOutputClient(Ros2UrinterfacesSrvSetAnalogOutputClient const&)=delete;Ros2UrinterfacesSrvSetAnalogOutputClient& operator=(Ros2UrinterfacesSrvSetAnalogOutputClient const&)=delete;Ros2UrinterfacesSrvSetAnalogOutputClient(Ros2UrinterfacesSrvSetAnalogOutputClient&&)=delete;
~Ros2UrinterfacesSrvSetAnalogOutputClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_urinterfaces_srv_SetAnalogOutput_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom urinterfaces/srv/SetAnalogOutput server; advertise/respond stay separate authorities. */
class Ros2UrinterfacesSrvSetAnalogOutputServer:public ServerOwners {
using Handler=std::function<Ros2UrinterfacesSrvSetAnalogOutput_Response(Ros2UrinterfacesSrvSetAnalogOutput_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2UrinterfacesSrvSetAnalogOutput_Request;using Response=Ros2UrinterfacesSrvSetAnalogOutput_Response;
Ros2UrinterfacesSrvSetAnalogOutputServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_urinterfaces_srv_SetAnalogOutput_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_urinterfaces_srv_SetAnalogOutput_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_urinterfaces_srv_SetAnalogOutput_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_urinterfaces_srv_SetAnalogOutput_respond(&respond_,ticket,&response).error;}
};
/** Complete custom urinterfaces/srv/SetDigitalOutput client; default policy returns13 from construction. */
class Ros2UrinterfacesSrvSetDigitalOutputClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2UrinterfacesSrvSetDigitalOutput_Request;using Response=Ros2UrinterfacesSrvSetDigitalOutput_Response;
Ros2UrinterfacesSrvSetDigitalOutputClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2UrinterfacesSrvSetDigitalOutputClient(Ros2UrinterfacesSrvSetDigitalOutputClient const&)=delete;Ros2UrinterfacesSrvSetDigitalOutputClient& operator=(Ros2UrinterfacesSrvSetDigitalOutputClient const&)=delete;Ros2UrinterfacesSrvSetDigitalOutputClient(Ros2UrinterfacesSrvSetDigitalOutputClient&&)=delete;
~Ros2UrinterfacesSrvSetDigitalOutputClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_urinterfaces_srv_SetDigitalOutput_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom urinterfaces/srv/SetDigitalOutput server; advertise/respond stay separate authorities. */
class Ros2UrinterfacesSrvSetDigitalOutputServer:public ServerOwners {
using Handler=std::function<Ros2UrinterfacesSrvSetDigitalOutput_Response(Ros2UrinterfacesSrvSetDigitalOutput_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2UrinterfacesSrvSetDigitalOutput_Request;using Response=Ros2UrinterfacesSrvSetDigitalOutput_Response;
Ros2UrinterfacesSrvSetDigitalOutputServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_urinterfaces_srv_SetDigitalOutput_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_urinterfaces_srv_SetDigitalOutput_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_urinterfaces_srv_SetDigitalOutput_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_urinterfaces_srv_SetDigitalOutput_respond(&respond_,ticket,&response).error;}
};
/** Complete custom urinterfaces/srv/SetSpeedFraction client; default policy returns13 from construction. */
class Ros2UrinterfacesSrvSetSpeedFractionClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2UrinterfacesSrvSetSpeedFraction_Request;using Response=Ros2UrinterfacesSrvSetSpeedFraction_Response;
Ros2UrinterfacesSrvSetSpeedFractionClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2UrinterfacesSrvSetSpeedFractionClient(Ros2UrinterfacesSrvSetSpeedFractionClient const&)=delete;Ros2UrinterfacesSrvSetSpeedFractionClient& operator=(Ros2UrinterfacesSrvSetSpeedFractionClient const&)=delete;Ros2UrinterfacesSrvSetSpeedFractionClient(Ros2UrinterfacesSrvSetSpeedFractionClient&&)=delete;
~Ros2UrinterfacesSrvSetSpeedFractionClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_urinterfaces_srv_SetSpeedFraction_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom urinterfaces/srv/SetSpeedFraction server; advertise/respond stay separate authorities. */
class Ros2UrinterfacesSrvSetSpeedFractionServer:public ServerOwners {
using Handler=std::function<Ros2UrinterfacesSrvSetSpeedFraction_Response(Ros2UrinterfacesSrvSetSpeedFraction_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2UrinterfacesSrvSetSpeedFraction_Request;using Response=Ros2UrinterfacesSrvSetSpeedFraction_Response;
Ros2UrinterfacesSrvSetSpeedFractionServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_urinterfaces_srv_SetSpeedFraction_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_urinterfaces_srv_SetSpeedFraction_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_urinterfaces_srv_SetSpeedFraction_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_urinterfaces_srv_SetSpeedFraction_respond(&respond_,ticket,&response).error;}
};
/** Complete custom urinterfaces/srv/SharedVariablesEditor client; default policy returns13 from construction. */
class Ros2UrinterfacesSrvSharedVariablesEditorClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2UrinterfacesSrvSharedVariablesEditor_Request;using Response=Ros2UrinterfacesSrvSharedVariablesEditor_Response;
Ros2UrinterfacesSrvSharedVariablesEditorClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2UrinterfacesSrvSharedVariablesEditorClient(Ros2UrinterfacesSrvSharedVariablesEditorClient const&)=delete;Ros2UrinterfacesSrvSharedVariablesEditorClient& operator=(Ros2UrinterfacesSrvSharedVariablesEditorClient const&)=delete;Ros2UrinterfacesSrvSharedVariablesEditorClient(Ros2UrinterfacesSrvSharedVariablesEditorClient&&)=delete;
~Ros2UrinterfacesSrvSharedVariablesEditorClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_urinterfaces_srv_SharedVariablesEditor_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom urinterfaces/srv/SharedVariablesEditor server; advertise/respond stay separate authorities. */
class Ros2UrinterfacesSrvSharedVariablesEditorServer:public ServerOwners {
using Handler=std::function<Ros2UrinterfacesSrvSharedVariablesEditor_Response(Ros2UrinterfacesSrvSharedVariablesEditor_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2UrinterfacesSrvSharedVariablesEditor_Request;using Response=Ros2UrinterfacesSrvSharedVariablesEditor_Response;
Ros2UrinterfacesSrvSharedVariablesEditorServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_urinterfaces_srv_SharedVariablesEditor_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_urinterfaces_srv_SharedVariablesEditor_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_urinterfaces_srv_SharedVariablesEditor_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_urinterfaces_srv_SharedVariablesEditor_respond(&respond_,ticket,&response).error;}
};
/** Complete custom urinterfaces/srv/VariablesViewer client; default policy returns13 from construction. */
class Ros2UrinterfacesSrvVariablesViewerClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2UrinterfacesSrvVariablesViewer_Request;using Response=Ros2UrinterfacesSrvVariablesViewer_Response;
Ros2UrinterfacesSrvVariablesViewerClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2UrinterfacesSrvVariablesViewerClient(Ros2UrinterfacesSrvVariablesViewerClient const&)=delete;Ros2UrinterfacesSrvVariablesViewerClient& operator=(Ros2UrinterfacesSrvVariablesViewerClient const&)=delete;Ros2UrinterfacesSrvVariablesViewerClient(Ros2UrinterfacesSrvVariablesViewerClient&&)=delete;
~Ros2UrinterfacesSrvVariablesViewerClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_urinterfaces_srv_VariablesViewer_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom urinterfaces/srv/VariablesViewer server; advertise/respond stay separate authorities. */
class Ros2UrinterfacesSrvVariablesViewerServer:public ServerOwners {
using Handler=std::function<Ros2UrinterfacesSrvVariablesViewer_Response(Ros2UrinterfacesSrvVariablesViewer_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2UrinterfacesSrvVariablesViewer_Request;using Response=Ros2UrinterfacesSrvVariablesViewer_Response;
Ros2UrinterfacesSrvVariablesViewerServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_urinterfaces_srv_VariablesViewer_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_urinterfaces_srv_VariablesViewer_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_urinterfaces_srv_VariablesViewer_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_urinterfaces_srv_VariablesViewer_respond(&respond_,ticket,&response).error;}
};
}
