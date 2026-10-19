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
/** Complete custom mycobot_communication/GetAngles client; default policy returns13 from construction. */
class Ros1Mycobot_communicationGetAnglesClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Mycobot_communicationGetAnglesRequest;using Response=Ros1Mycobot_communicationGetAnglesResponse;
Ros1Mycobot_communicationGetAnglesClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Mycobot_communicationGetAnglesClient(Ros1Mycobot_communicationGetAnglesClient const&)=delete;Ros1Mycobot_communicationGetAnglesClient& operator=(Ros1Mycobot_communicationGetAnglesClient const&)=delete;Ros1Mycobot_communicationGetAnglesClient(Ros1Mycobot_communicationGetAnglesClient&&)=delete;
~Ros1Mycobot_communicationGetAnglesClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mycobot_communication_GetAngles_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mycobot_communication/GetAngles server; advertise/respond stay separate authorities. */
class Ros1Mycobot_communicationGetAnglesServer:public ServerOwners {
using Handler=std::function<Ros1Mycobot_communicationGetAnglesResponse(Ros1Mycobot_communicationGetAnglesRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Mycobot_communicationGetAnglesRequest;using Response=Ros1Mycobot_communicationGetAnglesResponse;
Ros1Mycobot_communicationGetAnglesServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_mycobot_communication_GetAngles_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mycobot_communication_GetAngles_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_mycobot_communication_GetAngles_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_mycobot_communication_GetAngles_respond(&respond_,ticket,&response).error;}
};
/** Complete custom mycobot_communication/GetCoords client; default policy returns13 from construction. */
class Ros1Mycobot_communicationGetCoordsClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Mycobot_communicationGetCoordsRequest;using Response=Ros1Mycobot_communicationGetCoordsResponse;
Ros1Mycobot_communicationGetCoordsClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Mycobot_communicationGetCoordsClient(Ros1Mycobot_communicationGetCoordsClient const&)=delete;Ros1Mycobot_communicationGetCoordsClient& operator=(Ros1Mycobot_communicationGetCoordsClient const&)=delete;Ros1Mycobot_communicationGetCoordsClient(Ros1Mycobot_communicationGetCoordsClient&&)=delete;
~Ros1Mycobot_communicationGetCoordsClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mycobot_communication_GetCoords_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mycobot_communication/GetCoords server; advertise/respond stay separate authorities. */
class Ros1Mycobot_communicationGetCoordsServer:public ServerOwners {
using Handler=std::function<Ros1Mycobot_communicationGetCoordsResponse(Ros1Mycobot_communicationGetCoordsRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Mycobot_communicationGetCoordsRequest;using Response=Ros1Mycobot_communicationGetCoordsResponse;
Ros1Mycobot_communicationGetCoordsServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_mycobot_communication_GetCoords_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mycobot_communication_GetCoords_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_mycobot_communication_GetCoords_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_mycobot_communication_GetCoords_respond(&respond_,ticket,&response).error;}
};
/** Complete custom mycobot_communication/GripperStatus client; default policy returns13 from construction. */
class Ros1Mycobot_communicationGripperStatusClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Mycobot_communicationGripperStatusRequest;using Response=Ros1Mycobot_communicationGripperStatusResponse;
Ros1Mycobot_communicationGripperStatusClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Mycobot_communicationGripperStatusClient(Ros1Mycobot_communicationGripperStatusClient const&)=delete;Ros1Mycobot_communicationGripperStatusClient& operator=(Ros1Mycobot_communicationGripperStatusClient const&)=delete;Ros1Mycobot_communicationGripperStatusClient(Ros1Mycobot_communicationGripperStatusClient&&)=delete;
~Ros1Mycobot_communicationGripperStatusClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mycobot_communication_GripperStatus_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mycobot_communication/GripperStatus server; advertise/respond stay separate authorities. */
class Ros1Mycobot_communicationGripperStatusServer:public ServerOwners {
using Handler=std::function<Ros1Mycobot_communicationGripperStatusResponse(Ros1Mycobot_communicationGripperStatusRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Mycobot_communicationGripperStatusRequest;using Response=Ros1Mycobot_communicationGripperStatusResponse;
Ros1Mycobot_communicationGripperStatusServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_mycobot_communication_GripperStatus_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mycobot_communication_GripperStatus_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_mycobot_communication_GripperStatus_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_mycobot_communication_GripperStatus_respond(&respond_,ticket,&response).error;}
};
/** Complete custom mycobot_communication/PumpStatus client; default policy returns13 from construction. */
class Ros1Mycobot_communicationPumpStatusClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Mycobot_communicationPumpStatusRequest;using Response=Ros1Mycobot_communicationPumpStatusResponse;
Ros1Mycobot_communicationPumpStatusClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Mycobot_communicationPumpStatusClient(Ros1Mycobot_communicationPumpStatusClient const&)=delete;Ros1Mycobot_communicationPumpStatusClient& operator=(Ros1Mycobot_communicationPumpStatusClient const&)=delete;Ros1Mycobot_communicationPumpStatusClient(Ros1Mycobot_communicationPumpStatusClient&&)=delete;
~Ros1Mycobot_communicationPumpStatusClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mycobot_communication_PumpStatus_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mycobot_communication/PumpStatus server; advertise/respond stay separate authorities. */
class Ros1Mycobot_communicationPumpStatusServer:public ServerOwners {
using Handler=std::function<Ros1Mycobot_communicationPumpStatusResponse(Ros1Mycobot_communicationPumpStatusRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Mycobot_communicationPumpStatusRequest;using Response=Ros1Mycobot_communicationPumpStatusResponse;
Ros1Mycobot_communicationPumpStatusServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_mycobot_communication_PumpStatus_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mycobot_communication_PumpStatus_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_mycobot_communication_PumpStatus_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_mycobot_communication_PumpStatus_respond(&respond_,ticket,&response).error;}
};
/** Complete custom mycobot_communication/SetAngles client; default policy returns13 from construction. */
class Ros1Mycobot_communicationSetAnglesClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Mycobot_communicationSetAnglesRequest;using Response=Ros1Mycobot_communicationSetAnglesResponse;
Ros1Mycobot_communicationSetAnglesClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Mycobot_communicationSetAnglesClient(Ros1Mycobot_communicationSetAnglesClient const&)=delete;Ros1Mycobot_communicationSetAnglesClient& operator=(Ros1Mycobot_communicationSetAnglesClient const&)=delete;Ros1Mycobot_communicationSetAnglesClient(Ros1Mycobot_communicationSetAnglesClient&&)=delete;
~Ros1Mycobot_communicationSetAnglesClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mycobot_communication_SetAngles_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mycobot_communication/SetAngles server; advertise/respond stay separate authorities. */
class Ros1Mycobot_communicationSetAnglesServer:public ServerOwners {
using Handler=std::function<Ros1Mycobot_communicationSetAnglesResponse(Ros1Mycobot_communicationSetAnglesRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Mycobot_communicationSetAnglesRequest;using Response=Ros1Mycobot_communicationSetAnglesResponse;
Ros1Mycobot_communicationSetAnglesServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_mycobot_communication_SetAngles_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mycobot_communication_SetAngles_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_mycobot_communication_SetAngles_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_mycobot_communication_SetAngles_respond(&respond_,ticket,&response).error;}
};
/** Complete custom mycobot_communication/SetCoords client; default policy returns13 from construction. */
class Ros1Mycobot_communicationSetCoordsClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Mycobot_communicationSetCoordsRequest;using Response=Ros1Mycobot_communicationSetCoordsResponse;
Ros1Mycobot_communicationSetCoordsClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Mycobot_communicationSetCoordsClient(Ros1Mycobot_communicationSetCoordsClient const&)=delete;Ros1Mycobot_communicationSetCoordsClient& operator=(Ros1Mycobot_communicationSetCoordsClient const&)=delete;Ros1Mycobot_communicationSetCoordsClient(Ros1Mycobot_communicationSetCoordsClient&&)=delete;
~Ros1Mycobot_communicationSetCoordsClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mycobot_communication_SetCoords_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mycobot_communication/SetCoords server; advertise/respond stay separate authorities. */
class Ros1Mycobot_communicationSetCoordsServer:public ServerOwners {
using Handler=std::function<Ros1Mycobot_communicationSetCoordsResponse(Ros1Mycobot_communicationSetCoordsRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Mycobot_communicationSetCoordsRequest;using Response=Ros1Mycobot_communicationSetCoordsResponse;
Ros1Mycobot_communicationSetCoordsServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_mycobot_communication_SetCoords_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mycobot_communication_SetCoords_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_mycobot_communication_SetCoords_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_mycobot_communication_SetCoords_respond(&respond_,ticket,&response).error;}
};
/** Complete custom mypalletizer_communication/GetAngles client; default policy returns13 from construction. */
class Ros1Mypalletizer_communicationGetAnglesClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Mypalletizer_communicationGetAnglesRequest;using Response=Ros1Mypalletizer_communicationGetAnglesResponse;
Ros1Mypalletizer_communicationGetAnglesClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Mypalletizer_communicationGetAnglesClient(Ros1Mypalletizer_communicationGetAnglesClient const&)=delete;Ros1Mypalletizer_communicationGetAnglesClient& operator=(Ros1Mypalletizer_communicationGetAnglesClient const&)=delete;Ros1Mypalletizer_communicationGetAnglesClient(Ros1Mypalletizer_communicationGetAnglesClient&&)=delete;
~Ros1Mypalletizer_communicationGetAnglesClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mypalletizer_communication_GetAngles_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mypalletizer_communication/GetAngles server; advertise/respond stay separate authorities. */
class Ros1Mypalletizer_communicationGetAnglesServer:public ServerOwners {
using Handler=std::function<Ros1Mypalletizer_communicationGetAnglesResponse(Ros1Mypalletizer_communicationGetAnglesRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Mypalletizer_communicationGetAnglesRequest;using Response=Ros1Mypalletizer_communicationGetAnglesResponse;
Ros1Mypalletizer_communicationGetAnglesServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_mypalletizer_communication_GetAngles_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mypalletizer_communication_GetAngles_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_mypalletizer_communication_GetAngles_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_mypalletizer_communication_GetAngles_respond(&respond_,ticket,&response).error;}
};
/** Complete custom mypalletizer_communication/GetCoords client; default policy returns13 from construction. */
class Ros1Mypalletizer_communicationGetCoordsClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Mypalletizer_communicationGetCoordsRequest;using Response=Ros1Mypalletizer_communicationGetCoordsResponse;
Ros1Mypalletizer_communicationGetCoordsClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Mypalletizer_communicationGetCoordsClient(Ros1Mypalletizer_communicationGetCoordsClient const&)=delete;Ros1Mypalletizer_communicationGetCoordsClient& operator=(Ros1Mypalletizer_communicationGetCoordsClient const&)=delete;Ros1Mypalletizer_communicationGetCoordsClient(Ros1Mypalletizer_communicationGetCoordsClient&&)=delete;
~Ros1Mypalletizer_communicationGetCoordsClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mypalletizer_communication_GetCoords_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mypalletizer_communication/GetCoords server; advertise/respond stay separate authorities. */
class Ros1Mypalletizer_communicationGetCoordsServer:public ServerOwners {
using Handler=std::function<Ros1Mypalletizer_communicationGetCoordsResponse(Ros1Mypalletizer_communicationGetCoordsRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Mypalletizer_communicationGetCoordsRequest;using Response=Ros1Mypalletizer_communicationGetCoordsResponse;
Ros1Mypalletizer_communicationGetCoordsServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_mypalletizer_communication_GetCoords_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mypalletizer_communication_GetCoords_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_mypalletizer_communication_GetCoords_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_mypalletizer_communication_GetCoords_respond(&respond_,ticket,&response).error;}
};
/** Complete custom mypalletizer_communication/GripperStatus client; default policy returns13 from construction. */
class Ros1Mypalletizer_communicationGripperStatusClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Mypalletizer_communicationGripperStatusRequest;using Response=Ros1Mypalletizer_communicationGripperStatusResponse;
Ros1Mypalletizer_communicationGripperStatusClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Mypalletizer_communicationGripperStatusClient(Ros1Mypalletizer_communicationGripperStatusClient const&)=delete;Ros1Mypalletizer_communicationGripperStatusClient& operator=(Ros1Mypalletizer_communicationGripperStatusClient const&)=delete;Ros1Mypalletizer_communicationGripperStatusClient(Ros1Mypalletizer_communicationGripperStatusClient&&)=delete;
~Ros1Mypalletizer_communicationGripperStatusClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mypalletizer_communication_GripperStatus_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mypalletizer_communication/GripperStatus server; advertise/respond stay separate authorities. */
class Ros1Mypalletizer_communicationGripperStatusServer:public ServerOwners {
using Handler=std::function<Ros1Mypalletizer_communicationGripperStatusResponse(Ros1Mypalletizer_communicationGripperStatusRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Mypalletizer_communicationGripperStatusRequest;using Response=Ros1Mypalletizer_communicationGripperStatusResponse;
Ros1Mypalletizer_communicationGripperStatusServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_mypalletizer_communication_GripperStatus_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mypalletizer_communication_GripperStatus_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_mypalletizer_communication_GripperStatus_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_mypalletizer_communication_GripperStatus_respond(&respond_,ticket,&response).error;}
};
/** Complete custom mypalletizer_communication/PumpStatus client; default policy returns13 from construction. */
class Ros1Mypalletizer_communicationPumpStatusClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Mypalletizer_communicationPumpStatusRequest;using Response=Ros1Mypalletizer_communicationPumpStatusResponse;
Ros1Mypalletizer_communicationPumpStatusClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Mypalletizer_communicationPumpStatusClient(Ros1Mypalletizer_communicationPumpStatusClient const&)=delete;Ros1Mypalletizer_communicationPumpStatusClient& operator=(Ros1Mypalletizer_communicationPumpStatusClient const&)=delete;Ros1Mypalletizer_communicationPumpStatusClient(Ros1Mypalletizer_communicationPumpStatusClient&&)=delete;
~Ros1Mypalletizer_communicationPumpStatusClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mypalletizer_communication_PumpStatus_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mypalletizer_communication/PumpStatus server; advertise/respond stay separate authorities. */
class Ros1Mypalletizer_communicationPumpStatusServer:public ServerOwners {
using Handler=std::function<Ros1Mypalletizer_communicationPumpStatusResponse(Ros1Mypalletizer_communicationPumpStatusRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Mypalletizer_communicationPumpStatusRequest;using Response=Ros1Mypalletizer_communicationPumpStatusResponse;
Ros1Mypalletizer_communicationPumpStatusServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_mypalletizer_communication_PumpStatus_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mypalletizer_communication_PumpStatus_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_mypalletizer_communication_PumpStatus_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_mypalletizer_communication_PumpStatus_respond(&respond_,ticket,&response).error;}
};
/** Complete custom mypalletizer_communication/SetAngles client; default policy returns13 from construction. */
class Ros1Mypalletizer_communicationSetAnglesClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Mypalletizer_communicationSetAnglesRequest;using Response=Ros1Mypalletizer_communicationSetAnglesResponse;
Ros1Mypalletizer_communicationSetAnglesClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Mypalletizer_communicationSetAnglesClient(Ros1Mypalletizer_communicationSetAnglesClient const&)=delete;Ros1Mypalletizer_communicationSetAnglesClient& operator=(Ros1Mypalletizer_communicationSetAnglesClient const&)=delete;Ros1Mypalletizer_communicationSetAnglesClient(Ros1Mypalletizer_communicationSetAnglesClient&&)=delete;
~Ros1Mypalletizer_communicationSetAnglesClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mypalletizer_communication_SetAngles_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mypalletizer_communication/SetAngles server; advertise/respond stay separate authorities. */
class Ros1Mypalletizer_communicationSetAnglesServer:public ServerOwners {
using Handler=std::function<Ros1Mypalletizer_communicationSetAnglesResponse(Ros1Mypalletizer_communicationSetAnglesRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Mypalletizer_communicationSetAnglesRequest;using Response=Ros1Mypalletizer_communicationSetAnglesResponse;
Ros1Mypalletizer_communicationSetAnglesServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_mypalletizer_communication_SetAngles_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mypalletizer_communication_SetAngles_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_mypalletizer_communication_SetAngles_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_mypalletizer_communication_SetAngles_respond(&respond_,ticket,&response).error;}
};
/** Complete custom mypalletizer_communication/SetCoords client; default policy returns13 from construction. */
class Ros1Mypalletizer_communicationSetCoordsClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Mypalletizer_communicationSetCoordsRequest;using Response=Ros1Mypalletizer_communicationSetCoordsResponse;
Ros1Mypalletizer_communicationSetCoordsClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Mypalletizer_communicationSetCoordsClient(Ros1Mypalletizer_communicationSetCoordsClient const&)=delete;Ros1Mypalletizer_communicationSetCoordsClient& operator=(Ros1Mypalletizer_communicationSetCoordsClient const&)=delete;Ros1Mypalletizer_communicationSetCoordsClient(Ros1Mypalletizer_communicationSetCoordsClient&&)=delete;
~Ros1Mypalletizer_communicationSetCoordsClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mypalletizer_communication_SetCoords_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mypalletizer_communication/SetCoords server; advertise/respond stay separate authorities. */
class Ros1Mypalletizer_communicationSetCoordsServer:public ServerOwners {
using Handler=std::function<Ros1Mypalletizer_communicationSetCoordsResponse(Ros1Mypalletizer_communicationSetCoordsRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Mypalletizer_communicationSetCoordsRequest;using Response=Ros1Mypalletizer_communicationSetCoordsResponse;
Ros1Mypalletizer_communicationSetCoordsServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_mypalletizer_communication_SetCoords_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mypalletizer_communication_SetCoords_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_mypalletizer_communication_SetCoords_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_mypalletizer_communication_SetCoords_respond(&respond_,ticket,&response).error;}
};
}
