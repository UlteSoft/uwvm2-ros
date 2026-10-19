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
/** Complete custom mybuddy_communication/GetAngles client; default policy returns13 from construction. */
class Ros1Mybuddy_communicationGetAnglesClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Mybuddy_communicationGetAnglesRequest;using Response=Ros1Mybuddy_communicationGetAnglesResponse;
Ros1Mybuddy_communicationGetAnglesClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Mybuddy_communicationGetAnglesClient(Ros1Mybuddy_communicationGetAnglesClient const&)=delete;Ros1Mybuddy_communicationGetAnglesClient& operator=(Ros1Mybuddy_communicationGetAnglesClient const&)=delete;Ros1Mybuddy_communicationGetAnglesClient(Ros1Mybuddy_communicationGetAnglesClient&&)=delete;
~Ros1Mybuddy_communicationGetAnglesClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mybuddy_communication_GetAngles_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mybuddy_communication/GetAngles server; advertise/respond stay separate authorities. */
class Ros1Mybuddy_communicationGetAnglesServer:public ServerOwners {
using Handler=std::function<Ros1Mybuddy_communicationGetAnglesResponse(Ros1Mybuddy_communicationGetAnglesRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Mybuddy_communicationGetAnglesRequest;using Response=Ros1Mybuddy_communicationGetAnglesResponse;
Ros1Mybuddy_communicationGetAnglesServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_mybuddy_communication_GetAngles_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mybuddy_communication_GetAngles_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_mybuddy_communication_GetAngles_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_mybuddy_communication_GetAngles_respond(&respond_,ticket,&response).error;}
};
/** Complete custom mybuddy_communication/GetCoords client; default policy returns13 from construction. */
class Ros1Mybuddy_communicationGetCoordsClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Mybuddy_communicationGetCoordsRequest;using Response=Ros1Mybuddy_communicationGetCoordsResponse;
Ros1Mybuddy_communicationGetCoordsClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Mybuddy_communicationGetCoordsClient(Ros1Mybuddy_communicationGetCoordsClient const&)=delete;Ros1Mybuddy_communicationGetCoordsClient& operator=(Ros1Mybuddy_communicationGetCoordsClient const&)=delete;Ros1Mybuddy_communicationGetCoordsClient(Ros1Mybuddy_communicationGetCoordsClient&&)=delete;
~Ros1Mybuddy_communicationGetCoordsClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mybuddy_communication_GetCoords_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mybuddy_communication/GetCoords server; advertise/respond stay separate authorities. */
class Ros1Mybuddy_communicationGetCoordsServer:public ServerOwners {
using Handler=std::function<Ros1Mybuddy_communicationGetCoordsResponse(Ros1Mybuddy_communicationGetCoordsRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Mybuddy_communicationGetCoordsRequest;using Response=Ros1Mybuddy_communicationGetCoordsResponse;
Ros1Mybuddy_communicationGetCoordsServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_mybuddy_communication_GetCoords_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mybuddy_communication_GetCoords_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_mybuddy_communication_GetCoords_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_mybuddy_communication_GetCoords_respond(&respond_,ticket,&response).error;}
};
/** Complete custom mybuddy_communication/GripperStatus client; default policy returns13 from construction. */
class Ros1Mybuddy_communicationGripperStatusClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Mybuddy_communicationGripperStatusRequest;using Response=Ros1Mybuddy_communicationGripperStatusResponse;
Ros1Mybuddy_communicationGripperStatusClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Mybuddy_communicationGripperStatusClient(Ros1Mybuddy_communicationGripperStatusClient const&)=delete;Ros1Mybuddy_communicationGripperStatusClient& operator=(Ros1Mybuddy_communicationGripperStatusClient const&)=delete;Ros1Mybuddy_communicationGripperStatusClient(Ros1Mybuddy_communicationGripperStatusClient&&)=delete;
~Ros1Mybuddy_communicationGripperStatusClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mybuddy_communication_GripperStatus_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mybuddy_communication/GripperStatus server; advertise/respond stay separate authorities. */
class Ros1Mybuddy_communicationGripperStatusServer:public ServerOwners {
using Handler=std::function<Ros1Mybuddy_communicationGripperStatusResponse(Ros1Mybuddy_communicationGripperStatusRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Mybuddy_communicationGripperStatusRequest;using Response=Ros1Mybuddy_communicationGripperStatusResponse;
Ros1Mybuddy_communicationGripperStatusServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_mybuddy_communication_GripperStatus_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mybuddy_communication_GripperStatus_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_mybuddy_communication_GripperStatus_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_mybuddy_communication_GripperStatus_respond(&respond_,ticket,&response).error;}
};
/** Complete custom mybuddy_communication/PumpStatus client; default policy returns13 from construction. */
class Ros1Mybuddy_communicationPumpStatusClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Mybuddy_communicationPumpStatusRequest;using Response=Ros1Mybuddy_communicationPumpStatusResponse;
Ros1Mybuddy_communicationPumpStatusClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Mybuddy_communicationPumpStatusClient(Ros1Mybuddy_communicationPumpStatusClient const&)=delete;Ros1Mybuddy_communicationPumpStatusClient& operator=(Ros1Mybuddy_communicationPumpStatusClient const&)=delete;Ros1Mybuddy_communicationPumpStatusClient(Ros1Mybuddy_communicationPumpStatusClient&&)=delete;
~Ros1Mybuddy_communicationPumpStatusClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mybuddy_communication_PumpStatus_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mybuddy_communication/PumpStatus server; advertise/respond stay separate authorities. */
class Ros1Mybuddy_communicationPumpStatusServer:public ServerOwners {
using Handler=std::function<Ros1Mybuddy_communicationPumpStatusResponse(Ros1Mybuddy_communicationPumpStatusRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Mybuddy_communicationPumpStatusRequest;using Response=Ros1Mybuddy_communicationPumpStatusResponse;
Ros1Mybuddy_communicationPumpStatusServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_mybuddy_communication_PumpStatus_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mybuddy_communication_PumpStatus_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_mybuddy_communication_PumpStatus_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_mybuddy_communication_PumpStatus_respond(&respond_,ticket,&response).error;}
};
/** Complete custom mybuddy_communication/SetAngles client; default policy returns13 from construction. */
class Ros1Mybuddy_communicationSetAnglesClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Mybuddy_communicationSetAnglesRequest;using Response=Ros1Mybuddy_communicationSetAnglesResponse;
Ros1Mybuddy_communicationSetAnglesClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Mybuddy_communicationSetAnglesClient(Ros1Mybuddy_communicationSetAnglesClient const&)=delete;Ros1Mybuddy_communicationSetAnglesClient& operator=(Ros1Mybuddy_communicationSetAnglesClient const&)=delete;Ros1Mybuddy_communicationSetAnglesClient(Ros1Mybuddy_communicationSetAnglesClient&&)=delete;
~Ros1Mybuddy_communicationSetAnglesClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mybuddy_communication_SetAngles_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mybuddy_communication/SetAngles server; advertise/respond stay separate authorities. */
class Ros1Mybuddy_communicationSetAnglesServer:public ServerOwners {
using Handler=std::function<Ros1Mybuddy_communicationSetAnglesResponse(Ros1Mybuddy_communicationSetAnglesRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Mybuddy_communicationSetAnglesRequest;using Response=Ros1Mybuddy_communicationSetAnglesResponse;
Ros1Mybuddy_communicationSetAnglesServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_mybuddy_communication_SetAngles_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mybuddy_communication_SetAngles_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_mybuddy_communication_SetAngles_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_mybuddy_communication_SetAngles_respond(&respond_,ticket,&response).error;}
};
/** Complete custom mybuddy_communication/SetCoords client; default policy returns13 from construction. */
class Ros1Mybuddy_communicationSetCoordsClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Mybuddy_communicationSetCoordsRequest;using Response=Ros1Mybuddy_communicationSetCoordsResponse;
Ros1Mybuddy_communicationSetCoordsClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Mybuddy_communicationSetCoordsClient(Ros1Mybuddy_communicationSetCoordsClient const&)=delete;Ros1Mybuddy_communicationSetCoordsClient& operator=(Ros1Mybuddy_communicationSetCoordsClient const&)=delete;Ros1Mybuddy_communicationSetCoordsClient(Ros1Mybuddy_communicationSetCoordsClient&&)=delete;
~Ros1Mybuddy_communicationSetCoordsClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mybuddy_communication_SetCoords_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mybuddy_communication/SetCoords server; advertise/respond stay separate authorities. */
class Ros1Mybuddy_communicationSetCoordsServer:public ServerOwners {
using Handler=std::function<Ros1Mybuddy_communicationSetCoordsResponse(Ros1Mybuddy_communicationSetCoordsRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Mybuddy_communicationSetCoordsRequest;using Response=Ros1Mybuddy_communicationSetCoordsResponse;
Ros1Mybuddy_communicationSetCoordsServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_mybuddy_communication_SetCoords_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mybuddy_communication_SetCoords_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_mybuddy_communication_SetCoords_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_mybuddy_communication_SetCoords_respond(&respond_,ticket,&response).error;}
};
/** Complete custom mecharm_communication/GetAngles client; default policy returns13 from construction. */
class Ros1Mecharm_communicationGetAnglesClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Mecharm_communicationGetAnglesRequest;using Response=Ros1Mecharm_communicationGetAnglesResponse;
Ros1Mecharm_communicationGetAnglesClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Mecharm_communicationGetAnglesClient(Ros1Mecharm_communicationGetAnglesClient const&)=delete;Ros1Mecharm_communicationGetAnglesClient& operator=(Ros1Mecharm_communicationGetAnglesClient const&)=delete;Ros1Mecharm_communicationGetAnglesClient(Ros1Mecharm_communicationGetAnglesClient&&)=delete;
~Ros1Mecharm_communicationGetAnglesClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mecharm_communication_GetAngles_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mecharm_communication/GetAngles server; advertise/respond stay separate authorities. */
class Ros1Mecharm_communicationGetAnglesServer:public ServerOwners {
using Handler=std::function<Ros1Mecharm_communicationGetAnglesResponse(Ros1Mecharm_communicationGetAnglesRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Mecharm_communicationGetAnglesRequest;using Response=Ros1Mecharm_communicationGetAnglesResponse;
Ros1Mecharm_communicationGetAnglesServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_mecharm_communication_GetAngles_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mecharm_communication_GetAngles_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_mecharm_communication_GetAngles_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_mecharm_communication_GetAngles_respond(&respond_,ticket,&response).error;}
};
/** Complete custom mecharm_communication/GetCoords client; default policy returns13 from construction. */
class Ros1Mecharm_communicationGetCoordsClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Mecharm_communicationGetCoordsRequest;using Response=Ros1Mecharm_communicationGetCoordsResponse;
Ros1Mecharm_communicationGetCoordsClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Mecharm_communicationGetCoordsClient(Ros1Mecharm_communicationGetCoordsClient const&)=delete;Ros1Mecharm_communicationGetCoordsClient& operator=(Ros1Mecharm_communicationGetCoordsClient const&)=delete;Ros1Mecharm_communicationGetCoordsClient(Ros1Mecharm_communicationGetCoordsClient&&)=delete;
~Ros1Mecharm_communicationGetCoordsClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mecharm_communication_GetCoords_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mecharm_communication/GetCoords server; advertise/respond stay separate authorities. */
class Ros1Mecharm_communicationGetCoordsServer:public ServerOwners {
using Handler=std::function<Ros1Mecharm_communicationGetCoordsResponse(Ros1Mecharm_communicationGetCoordsRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Mecharm_communicationGetCoordsRequest;using Response=Ros1Mecharm_communicationGetCoordsResponse;
Ros1Mecharm_communicationGetCoordsServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_mecharm_communication_GetCoords_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mecharm_communication_GetCoords_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_mecharm_communication_GetCoords_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_mecharm_communication_GetCoords_respond(&respond_,ticket,&response).error;}
};
/** Complete custom mecharm_communication/GripperStatus client; default policy returns13 from construction. */
class Ros1Mecharm_communicationGripperStatusClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Mecharm_communicationGripperStatusRequest;using Response=Ros1Mecharm_communicationGripperStatusResponse;
Ros1Mecharm_communicationGripperStatusClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Mecharm_communicationGripperStatusClient(Ros1Mecharm_communicationGripperStatusClient const&)=delete;Ros1Mecharm_communicationGripperStatusClient& operator=(Ros1Mecharm_communicationGripperStatusClient const&)=delete;Ros1Mecharm_communicationGripperStatusClient(Ros1Mecharm_communicationGripperStatusClient&&)=delete;
~Ros1Mecharm_communicationGripperStatusClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mecharm_communication_GripperStatus_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mecharm_communication/GripperStatus server; advertise/respond stay separate authorities. */
class Ros1Mecharm_communicationGripperStatusServer:public ServerOwners {
using Handler=std::function<Ros1Mecharm_communicationGripperStatusResponse(Ros1Mecharm_communicationGripperStatusRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Mecharm_communicationGripperStatusRequest;using Response=Ros1Mecharm_communicationGripperStatusResponse;
Ros1Mecharm_communicationGripperStatusServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_mecharm_communication_GripperStatus_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mecharm_communication_GripperStatus_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_mecharm_communication_GripperStatus_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_mecharm_communication_GripperStatus_respond(&respond_,ticket,&response).error;}
};
/** Complete custom mecharm_communication/PumpStatus client; default policy returns13 from construction. */
class Ros1Mecharm_communicationPumpStatusClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Mecharm_communicationPumpStatusRequest;using Response=Ros1Mecharm_communicationPumpStatusResponse;
Ros1Mecharm_communicationPumpStatusClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Mecharm_communicationPumpStatusClient(Ros1Mecharm_communicationPumpStatusClient const&)=delete;Ros1Mecharm_communicationPumpStatusClient& operator=(Ros1Mecharm_communicationPumpStatusClient const&)=delete;Ros1Mecharm_communicationPumpStatusClient(Ros1Mecharm_communicationPumpStatusClient&&)=delete;
~Ros1Mecharm_communicationPumpStatusClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mecharm_communication_PumpStatus_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mecharm_communication/PumpStatus server; advertise/respond stay separate authorities. */
class Ros1Mecharm_communicationPumpStatusServer:public ServerOwners {
using Handler=std::function<Ros1Mecharm_communicationPumpStatusResponse(Ros1Mecharm_communicationPumpStatusRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Mecharm_communicationPumpStatusRequest;using Response=Ros1Mecharm_communicationPumpStatusResponse;
Ros1Mecharm_communicationPumpStatusServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_mecharm_communication_PumpStatus_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mecharm_communication_PumpStatus_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_mecharm_communication_PumpStatus_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_mecharm_communication_PumpStatus_respond(&respond_,ticket,&response).error;}
};
/** Complete custom mecharm_communication/SetAngles client; default policy returns13 from construction. */
class Ros1Mecharm_communicationSetAnglesClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Mecharm_communicationSetAnglesRequest;using Response=Ros1Mecharm_communicationSetAnglesResponse;
Ros1Mecharm_communicationSetAnglesClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Mecharm_communicationSetAnglesClient(Ros1Mecharm_communicationSetAnglesClient const&)=delete;Ros1Mecharm_communicationSetAnglesClient& operator=(Ros1Mecharm_communicationSetAnglesClient const&)=delete;Ros1Mecharm_communicationSetAnglesClient(Ros1Mecharm_communicationSetAnglesClient&&)=delete;
~Ros1Mecharm_communicationSetAnglesClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mecharm_communication_SetAngles_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mecharm_communication/SetAngles server; advertise/respond stay separate authorities. */
class Ros1Mecharm_communicationSetAnglesServer:public ServerOwners {
using Handler=std::function<Ros1Mecharm_communicationSetAnglesResponse(Ros1Mecharm_communicationSetAnglesRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Mecharm_communicationSetAnglesRequest;using Response=Ros1Mecharm_communicationSetAnglesResponse;
Ros1Mecharm_communicationSetAnglesServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_mecharm_communication_SetAngles_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mecharm_communication_SetAngles_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_mecharm_communication_SetAngles_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_mecharm_communication_SetAngles_respond(&respond_,ticket,&response).error;}
};
/** Complete custom mecharm_communication/SetCoords client; default policy returns13 from construction. */
class Ros1Mecharm_communicationSetCoordsClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Mecharm_communicationSetCoordsRequest;using Response=Ros1Mecharm_communicationSetCoordsResponse;
Ros1Mecharm_communicationSetCoordsClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Mecharm_communicationSetCoordsClient(Ros1Mecharm_communicationSetCoordsClient const&)=delete;Ros1Mecharm_communicationSetCoordsClient& operator=(Ros1Mecharm_communicationSetCoordsClient const&)=delete;Ros1Mecharm_communicationSetCoordsClient(Ros1Mecharm_communicationSetCoordsClient&&)=delete;
~Ros1Mecharm_communicationSetCoordsClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mecharm_communication_SetCoords_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mecharm_communication/SetCoords server; advertise/respond stay separate authorities. */
class Ros1Mecharm_communicationSetCoordsServer:public ServerOwners {
using Handler=std::function<Ros1Mecharm_communicationSetCoordsResponse(Ros1Mecharm_communicationSetCoordsRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Mecharm_communicationSetCoordsRequest;using Response=Ros1Mecharm_communicationSetCoordsResponse;
Ros1Mecharm_communicationSetCoordsServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_mecharm_communication_SetCoords_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mecharm_communication_SetCoords_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_mecharm_communication_SetCoords_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_mecharm_communication_SetCoords_respond(&respond_,ticket,&response).error;}
};
/** Complete custom myarm_communication/GetAngles client; default policy returns13 from construction. */
class Ros1Myarm_communicationGetAnglesClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Myarm_communicationGetAnglesRequest;using Response=Ros1Myarm_communicationGetAnglesResponse;
Ros1Myarm_communicationGetAnglesClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Myarm_communicationGetAnglesClient(Ros1Myarm_communicationGetAnglesClient const&)=delete;Ros1Myarm_communicationGetAnglesClient& operator=(Ros1Myarm_communicationGetAnglesClient const&)=delete;Ros1Myarm_communicationGetAnglesClient(Ros1Myarm_communicationGetAnglesClient&&)=delete;
~Ros1Myarm_communicationGetAnglesClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_myarm_communication_GetAngles_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom myarm_communication/GetAngles server; advertise/respond stay separate authorities. */
class Ros1Myarm_communicationGetAnglesServer:public ServerOwners {
using Handler=std::function<Ros1Myarm_communicationGetAnglesResponse(Ros1Myarm_communicationGetAnglesRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Myarm_communicationGetAnglesRequest;using Response=Ros1Myarm_communicationGetAnglesResponse;
Ros1Myarm_communicationGetAnglesServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_myarm_communication_GetAngles_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_myarm_communication_GetAngles_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_myarm_communication_GetAngles_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_myarm_communication_GetAngles_respond(&respond_,ticket,&response).error;}
};
/** Complete custom myarm_communication/GetCoords client; default policy returns13 from construction. */
class Ros1Myarm_communicationGetCoordsClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Myarm_communicationGetCoordsRequest;using Response=Ros1Myarm_communicationGetCoordsResponse;
Ros1Myarm_communicationGetCoordsClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Myarm_communicationGetCoordsClient(Ros1Myarm_communicationGetCoordsClient const&)=delete;Ros1Myarm_communicationGetCoordsClient& operator=(Ros1Myarm_communicationGetCoordsClient const&)=delete;Ros1Myarm_communicationGetCoordsClient(Ros1Myarm_communicationGetCoordsClient&&)=delete;
~Ros1Myarm_communicationGetCoordsClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_myarm_communication_GetCoords_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom myarm_communication/GetCoords server; advertise/respond stay separate authorities. */
class Ros1Myarm_communicationGetCoordsServer:public ServerOwners {
using Handler=std::function<Ros1Myarm_communicationGetCoordsResponse(Ros1Myarm_communicationGetCoordsRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Myarm_communicationGetCoordsRequest;using Response=Ros1Myarm_communicationGetCoordsResponse;
Ros1Myarm_communicationGetCoordsServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_myarm_communication_GetCoords_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_myarm_communication_GetCoords_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_myarm_communication_GetCoords_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_myarm_communication_GetCoords_respond(&respond_,ticket,&response).error;}
};
/** Complete custom myarm_communication/GripperStatus client; default policy returns13 from construction. */
class Ros1Myarm_communicationGripperStatusClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Myarm_communicationGripperStatusRequest;using Response=Ros1Myarm_communicationGripperStatusResponse;
Ros1Myarm_communicationGripperStatusClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Myarm_communicationGripperStatusClient(Ros1Myarm_communicationGripperStatusClient const&)=delete;Ros1Myarm_communicationGripperStatusClient& operator=(Ros1Myarm_communicationGripperStatusClient const&)=delete;Ros1Myarm_communicationGripperStatusClient(Ros1Myarm_communicationGripperStatusClient&&)=delete;
~Ros1Myarm_communicationGripperStatusClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_myarm_communication_GripperStatus_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom myarm_communication/GripperStatus server; advertise/respond stay separate authorities. */
class Ros1Myarm_communicationGripperStatusServer:public ServerOwners {
using Handler=std::function<Ros1Myarm_communicationGripperStatusResponse(Ros1Myarm_communicationGripperStatusRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Myarm_communicationGripperStatusRequest;using Response=Ros1Myarm_communicationGripperStatusResponse;
Ros1Myarm_communicationGripperStatusServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_myarm_communication_GripperStatus_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_myarm_communication_GripperStatus_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_myarm_communication_GripperStatus_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_myarm_communication_GripperStatus_respond(&respond_,ticket,&response).error;}
};
/** Complete custom myarm_communication/PumpStatus client; default policy returns13 from construction. */
class Ros1Myarm_communicationPumpStatusClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Myarm_communicationPumpStatusRequest;using Response=Ros1Myarm_communicationPumpStatusResponse;
Ros1Myarm_communicationPumpStatusClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Myarm_communicationPumpStatusClient(Ros1Myarm_communicationPumpStatusClient const&)=delete;Ros1Myarm_communicationPumpStatusClient& operator=(Ros1Myarm_communicationPumpStatusClient const&)=delete;Ros1Myarm_communicationPumpStatusClient(Ros1Myarm_communicationPumpStatusClient&&)=delete;
~Ros1Myarm_communicationPumpStatusClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_myarm_communication_PumpStatus_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom myarm_communication/PumpStatus server; advertise/respond stay separate authorities. */
class Ros1Myarm_communicationPumpStatusServer:public ServerOwners {
using Handler=std::function<Ros1Myarm_communicationPumpStatusResponse(Ros1Myarm_communicationPumpStatusRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Myarm_communicationPumpStatusRequest;using Response=Ros1Myarm_communicationPumpStatusResponse;
Ros1Myarm_communicationPumpStatusServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_myarm_communication_PumpStatus_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_myarm_communication_PumpStatus_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_myarm_communication_PumpStatus_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_myarm_communication_PumpStatus_respond(&respond_,ticket,&response).error;}
};
/** Complete custom myarm_communication/SetAngles client; default policy returns13 from construction. */
class Ros1Myarm_communicationSetAnglesClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Myarm_communicationSetAnglesRequest;using Response=Ros1Myarm_communicationSetAnglesResponse;
Ros1Myarm_communicationSetAnglesClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Myarm_communicationSetAnglesClient(Ros1Myarm_communicationSetAnglesClient const&)=delete;Ros1Myarm_communicationSetAnglesClient& operator=(Ros1Myarm_communicationSetAnglesClient const&)=delete;Ros1Myarm_communicationSetAnglesClient(Ros1Myarm_communicationSetAnglesClient&&)=delete;
~Ros1Myarm_communicationSetAnglesClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_myarm_communication_SetAngles_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom myarm_communication/SetAngles server; advertise/respond stay separate authorities. */
class Ros1Myarm_communicationSetAnglesServer:public ServerOwners {
using Handler=std::function<Ros1Myarm_communicationSetAnglesResponse(Ros1Myarm_communicationSetAnglesRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Myarm_communicationSetAnglesRequest;using Response=Ros1Myarm_communicationSetAnglesResponse;
Ros1Myarm_communicationSetAnglesServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_myarm_communication_SetAngles_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_myarm_communication_SetAngles_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_myarm_communication_SetAngles_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_myarm_communication_SetAngles_respond(&respond_,ticket,&response).error;}
};
/** Complete custom myarm_communication/SetCoords client; default policy returns13 from construction. */
class Ros1Myarm_communicationSetCoordsClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Myarm_communicationSetCoordsRequest;using Response=Ros1Myarm_communicationSetCoordsResponse;
Ros1Myarm_communicationSetCoordsClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Myarm_communicationSetCoordsClient(Ros1Myarm_communicationSetCoordsClient const&)=delete;Ros1Myarm_communicationSetCoordsClient& operator=(Ros1Myarm_communicationSetCoordsClient const&)=delete;Ros1Myarm_communicationSetCoordsClient(Ros1Myarm_communicationSetCoordsClient&&)=delete;
~Ros1Myarm_communicationSetCoordsClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_myarm_communication_SetCoords_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom myarm_communication/SetCoords server; advertise/respond stay separate authorities. */
class Ros1Myarm_communicationSetCoordsServer:public ServerOwners {
using Handler=std::function<Ros1Myarm_communicationSetCoordsResponse(Ros1Myarm_communicationSetCoordsRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Myarm_communicationSetCoordsRequest;using Response=Ros1Myarm_communicationSetCoordsResponse;
Ros1Myarm_communicationSetCoordsServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_myarm_communication_SetCoords_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_myarm_communication_SetCoords_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_myarm_communication_SetCoords_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_myarm_communication_SetCoords_respond(&respond_,ticket,&response).error;}
};
/** Complete custom mycobot_320_communication/GetAngles client; default policy returns13 from construction. */
class Ros1Mycobot_320_communicationGetAnglesClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Mycobot_320_communicationGetAnglesRequest;using Response=Ros1Mycobot_320_communicationGetAnglesResponse;
Ros1Mycobot_320_communicationGetAnglesClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Mycobot_320_communicationGetAnglesClient(Ros1Mycobot_320_communicationGetAnglesClient const&)=delete;Ros1Mycobot_320_communicationGetAnglesClient& operator=(Ros1Mycobot_320_communicationGetAnglesClient const&)=delete;Ros1Mycobot_320_communicationGetAnglesClient(Ros1Mycobot_320_communicationGetAnglesClient&&)=delete;
~Ros1Mycobot_320_communicationGetAnglesClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mycobot_320_communication_GetAngles_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mycobot_320_communication/GetAngles server; advertise/respond stay separate authorities. */
class Ros1Mycobot_320_communicationGetAnglesServer:public ServerOwners {
using Handler=std::function<Ros1Mycobot_320_communicationGetAnglesResponse(Ros1Mycobot_320_communicationGetAnglesRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Mycobot_320_communicationGetAnglesRequest;using Response=Ros1Mycobot_320_communicationGetAnglesResponse;
Ros1Mycobot_320_communicationGetAnglesServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_mycobot_320_communication_GetAngles_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mycobot_320_communication_GetAngles_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_mycobot_320_communication_GetAngles_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_mycobot_320_communication_GetAngles_respond(&respond_,ticket,&response).error;}
};
/** Complete custom mycobot_320_communication/GetCoords client; default policy returns13 from construction. */
class Ros1Mycobot_320_communicationGetCoordsClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Mycobot_320_communicationGetCoordsRequest;using Response=Ros1Mycobot_320_communicationGetCoordsResponse;
Ros1Mycobot_320_communicationGetCoordsClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Mycobot_320_communicationGetCoordsClient(Ros1Mycobot_320_communicationGetCoordsClient const&)=delete;Ros1Mycobot_320_communicationGetCoordsClient& operator=(Ros1Mycobot_320_communicationGetCoordsClient const&)=delete;Ros1Mycobot_320_communicationGetCoordsClient(Ros1Mycobot_320_communicationGetCoordsClient&&)=delete;
~Ros1Mycobot_320_communicationGetCoordsClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mycobot_320_communication_GetCoords_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mycobot_320_communication/GetCoords server; advertise/respond stay separate authorities. */
class Ros1Mycobot_320_communicationGetCoordsServer:public ServerOwners {
using Handler=std::function<Ros1Mycobot_320_communicationGetCoordsResponse(Ros1Mycobot_320_communicationGetCoordsRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Mycobot_320_communicationGetCoordsRequest;using Response=Ros1Mycobot_320_communicationGetCoordsResponse;
Ros1Mycobot_320_communicationGetCoordsServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_mycobot_320_communication_GetCoords_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mycobot_320_communication_GetCoords_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_mycobot_320_communication_GetCoords_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_mycobot_320_communication_GetCoords_respond(&respond_,ticket,&response).error;}
};
/** Complete custom mycobot_320_communication/GripperStatus client; default policy returns13 from construction. */
class Ros1Mycobot_320_communicationGripperStatusClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Mycobot_320_communicationGripperStatusRequest;using Response=Ros1Mycobot_320_communicationGripperStatusResponse;
Ros1Mycobot_320_communicationGripperStatusClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Mycobot_320_communicationGripperStatusClient(Ros1Mycobot_320_communicationGripperStatusClient const&)=delete;Ros1Mycobot_320_communicationGripperStatusClient& operator=(Ros1Mycobot_320_communicationGripperStatusClient const&)=delete;Ros1Mycobot_320_communicationGripperStatusClient(Ros1Mycobot_320_communicationGripperStatusClient&&)=delete;
~Ros1Mycobot_320_communicationGripperStatusClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mycobot_320_communication_GripperStatus_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mycobot_320_communication/GripperStatus server; advertise/respond stay separate authorities. */
class Ros1Mycobot_320_communicationGripperStatusServer:public ServerOwners {
using Handler=std::function<Ros1Mycobot_320_communicationGripperStatusResponse(Ros1Mycobot_320_communicationGripperStatusRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Mycobot_320_communicationGripperStatusRequest;using Response=Ros1Mycobot_320_communicationGripperStatusResponse;
Ros1Mycobot_320_communicationGripperStatusServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_mycobot_320_communication_GripperStatus_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mycobot_320_communication_GripperStatus_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_mycobot_320_communication_GripperStatus_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_mycobot_320_communication_GripperStatus_respond(&respond_,ticket,&response).error;}
};
/** Complete custom mycobot_320_communication/PumpStatus client; default policy returns13 from construction. */
class Ros1Mycobot_320_communicationPumpStatusClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Mycobot_320_communicationPumpStatusRequest;using Response=Ros1Mycobot_320_communicationPumpStatusResponse;
Ros1Mycobot_320_communicationPumpStatusClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Mycobot_320_communicationPumpStatusClient(Ros1Mycobot_320_communicationPumpStatusClient const&)=delete;Ros1Mycobot_320_communicationPumpStatusClient& operator=(Ros1Mycobot_320_communicationPumpStatusClient const&)=delete;Ros1Mycobot_320_communicationPumpStatusClient(Ros1Mycobot_320_communicationPumpStatusClient&&)=delete;
~Ros1Mycobot_320_communicationPumpStatusClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mycobot_320_communication_PumpStatus_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mycobot_320_communication/PumpStatus server; advertise/respond stay separate authorities. */
class Ros1Mycobot_320_communicationPumpStatusServer:public ServerOwners {
using Handler=std::function<Ros1Mycobot_320_communicationPumpStatusResponse(Ros1Mycobot_320_communicationPumpStatusRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Mycobot_320_communicationPumpStatusRequest;using Response=Ros1Mycobot_320_communicationPumpStatusResponse;
Ros1Mycobot_320_communicationPumpStatusServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_mycobot_320_communication_PumpStatus_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mycobot_320_communication_PumpStatus_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_mycobot_320_communication_PumpStatus_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_mycobot_320_communication_PumpStatus_respond(&respond_,ticket,&response).error;}
};
/** Complete custom mycobot_320_communication/SetAngles client; default policy returns13 from construction. */
class Ros1Mycobot_320_communicationSetAnglesClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Mycobot_320_communicationSetAnglesRequest;using Response=Ros1Mycobot_320_communicationSetAnglesResponse;
Ros1Mycobot_320_communicationSetAnglesClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Mycobot_320_communicationSetAnglesClient(Ros1Mycobot_320_communicationSetAnglesClient const&)=delete;Ros1Mycobot_320_communicationSetAnglesClient& operator=(Ros1Mycobot_320_communicationSetAnglesClient const&)=delete;Ros1Mycobot_320_communicationSetAnglesClient(Ros1Mycobot_320_communicationSetAnglesClient&&)=delete;
~Ros1Mycobot_320_communicationSetAnglesClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mycobot_320_communication_SetAngles_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mycobot_320_communication/SetAngles server; advertise/respond stay separate authorities. */
class Ros1Mycobot_320_communicationSetAnglesServer:public ServerOwners {
using Handler=std::function<Ros1Mycobot_320_communicationSetAnglesResponse(Ros1Mycobot_320_communicationSetAnglesRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Mycobot_320_communicationSetAnglesRequest;using Response=Ros1Mycobot_320_communicationSetAnglesResponse;
Ros1Mycobot_320_communicationSetAnglesServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_mycobot_320_communication_SetAngles_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mycobot_320_communication_SetAngles_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_mycobot_320_communication_SetAngles_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_mycobot_320_communication_SetAngles_respond(&respond_,ticket,&response).error;}
};
/** Complete custom mycobot_320_communication/SetCoords client; default policy returns13 from construction. */
class Ros1Mycobot_320_communicationSetCoordsClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Mycobot_320_communicationSetCoordsRequest;using Response=Ros1Mycobot_320_communicationSetCoordsResponse;
Ros1Mycobot_320_communicationSetCoordsClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Mycobot_320_communicationSetCoordsClient(Ros1Mycobot_320_communicationSetCoordsClient const&)=delete;Ros1Mycobot_320_communicationSetCoordsClient& operator=(Ros1Mycobot_320_communicationSetCoordsClient const&)=delete;Ros1Mycobot_320_communicationSetCoordsClient(Ros1Mycobot_320_communicationSetCoordsClient&&)=delete;
~Ros1Mycobot_320_communicationSetCoordsClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mycobot_320_communication_SetCoords_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mycobot_320_communication/SetCoords server; advertise/respond stay separate authorities. */
class Ros1Mycobot_320_communicationSetCoordsServer:public ServerOwners {
using Handler=std::function<Ros1Mycobot_320_communicationSetCoordsResponse(Ros1Mycobot_320_communicationSetCoordsRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Mycobot_320_communicationSetCoordsRequest;using Response=Ros1Mycobot_320_communicationSetCoordsResponse;
Ros1Mycobot_320_communicationSetCoordsServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_mycobot_320_communication_SetCoords_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_mycobot_320_communication_SetCoords_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_mycobot_320_communication_SetCoords_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_mycobot_320_communication_SetCoords_respond(&respond_,ticket,&response).error;}
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
/** Complete custom ultraarm_communication/GetAngles client; default policy returns13 from construction. */
class Ros1Ultraarm_communicationGetAnglesClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Ultraarm_communicationGetAnglesRequest;using Response=Ros1Ultraarm_communicationGetAnglesResponse;
Ros1Ultraarm_communicationGetAnglesClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Ultraarm_communicationGetAnglesClient(Ros1Ultraarm_communicationGetAnglesClient const&)=delete;Ros1Ultraarm_communicationGetAnglesClient& operator=(Ros1Ultraarm_communicationGetAnglesClient const&)=delete;Ros1Ultraarm_communicationGetAnglesClient(Ros1Ultraarm_communicationGetAnglesClient&&)=delete;
~Ros1Ultraarm_communicationGetAnglesClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_ultraarm_communication_GetAngles_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ultraarm_communication/GetAngles server; advertise/respond stay separate authorities. */
class Ros1Ultraarm_communicationGetAnglesServer:public ServerOwners {
using Handler=std::function<Ros1Ultraarm_communicationGetAnglesResponse(Ros1Ultraarm_communicationGetAnglesRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Ultraarm_communicationGetAnglesRequest;using Response=Ros1Ultraarm_communicationGetAnglesResponse;
Ros1Ultraarm_communicationGetAnglesServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_ultraarm_communication_GetAngles_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_ultraarm_communication_GetAngles_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_ultraarm_communication_GetAngles_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_ultraarm_communication_GetAngles_respond(&respond_,ticket,&response).error;}
};
/** Complete custom ultraarm_communication/GetCoords client; default policy returns13 from construction. */
class Ros1Ultraarm_communicationGetCoordsClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Ultraarm_communicationGetCoordsRequest;using Response=Ros1Ultraarm_communicationGetCoordsResponse;
Ros1Ultraarm_communicationGetCoordsClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Ultraarm_communicationGetCoordsClient(Ros1Ultraarm_communicationGetCoordsClient const&)=delete;Ros1Ultraarm_communicationGetCoordsClient& operator=(Ros1Ultraarm_communicationGetCoordsClient const&)=delete;Ros1Ultraarm_communicationGetCoordsClient(Ros1Ultraarm_communicationGetCoordsClient&&)=delete;
~Ros1Ultraarm_communicationGetCoordsClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_ultraarm_communication_GetCoords_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ultraarm_communication/GetCoords server; advertise/respond stay separate authorities. */
class Ros1Ultraarm_communicationGetCoordsServer:public ServerOwners {
using Handler=std::function<Ros1Ultraarm_communicationGetCoordsResponse(Ros1Ultraarm_communicationGetCoordsRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Ultraarm_communicationGetCoordsRequest;using Response=Ros1Ultraarm_communicationGetCoordsResponse;
Ros1Ultraarm_communicationGetCoordsServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_ultraarm_communication_GetCoords_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_ultraarm_communication_GetCoords_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_ultraarm_communication_GetCoords_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_ultraarm_communication_GetCoords_respond(&respond_,ticket,&response).error;}
};
/** Complete custom ultraarm_communication/GripperStatus client; default policy returns13 from construction. */
class Ros1Ultraarm_communicationGripperStatusClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Ultraarm_communicationGripperStatusRequest;using Response=Ros1Ultraarm_communicationGripperStatusResponse;
Ros1Ultraarm_communicationGripperStatusClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Ultraarm_communicationGripperStatusClient(Ros1Ultraarm_communicationGripperStatusClient const&)=delete;Ros1Ultraarm_communicationGripperStatusClient& operator=(Ros1Ultraarm_communicationGripperStatusClient const&)=delete;Ros1Ultraarm_communicationGripperStatusClient(Ros1Ultraarm_communicationGripperStatusClient&&)=delete;
~Ros1Ultraarm_communicationGripperStatusClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_ultraarm_communication_GripperStatus_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ultraarm_communication/GripperStatus server; advertise/respond stay separate authorities. */
class Ros1Ultraarm_communicationGripperStatusServer:public ServerOwners {
using Handler=std::function<Ros1Ultraarm_communicationGripperStatusResponse(Ros1Ultraarm_communicationGripperStatusRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Ultraarm_communicationGripperStatusRequest;using Response=Ros1Ultraarm_communicationGripperStatusResponse;
Ros1Ultraarm_communicationGripperStatusServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_ultraarm_communication_GripperStatus_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_ultraarm_communication_GripperStatus_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_ultraarm_communication_GripperStatus_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_ultraarm_communication_GripperStatus_respond(&respond_,ticket,&response).error;}
};
/** Complete custom ultraarm_communication/PumpStatus client; default policy returns13 from construction. */
class Ros1Ultraarm_communicationPumpStatusClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Ultraarm_communicationPumpStatusRequest;using Response=Ros1Ultraarm_communicationPumpStatusResponse;
Ros1Ultraarm_communicationPumpStatusClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Ultraarm_communicationPumpStatusClient(Ros1Ultraarm_communicationPumpStatusClient const&)=delete;Ros1Ultraarm_communicationPumpStatusClient& operator=(Ros1Ultraarm_communicationPumpStatusClient const&)=delete;Ros1Ultraarm_communicationPumpStatusClient(Ros1Ultraarm_communicationPumpStatusClient&&)=delete;
~Ros1Ultraarm_communicationPumpStatusClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_ultraarm_communication_PumpStatus_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ultraarm_communication/PumpStatus server; advertise/respond stay separate authorities. */
class Ros1Ultraarm_communicationPumpStatusServer:public ServerOwners {
using Handler=std::function<Ros1Ultraarm_communicationPumpStatusResponse(Ros1Ultraarm_communicationPumpStatusRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Ultraarm_communicationPumpStatusRequest;using Response=Ros1Ultraarm_communicationPumpStatusResponse;
Ros1Ultraarm_communicationPumpStatusServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_ultraarm_communication_PumpStatus_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_ultraarm_communication_PumpStatus_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_ultraarm_communication_PumpStatus_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_ultraarm_communication_PumpStatus_respond(&respond_,ticket,&response).error;}
};
/** Complete custom ultraarm_communication/SetAngles client; default policy returns13 from construction. */
class Ros1Ultraarm_communicationSetAnglesClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Ultraarm_communicationSetAnglesRequest;using Response=Ros1Ultraarm_communicationSetAnglesResponse;
Ros1Ultraarm_communicationSetAnglesClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Ultraarm_communicationSetAnglesClient(Ros1Ultraarm_communicationSetAnglesClient const&)=delete;Ros1Ultraarm_communicationSetAnglesClient& operator=(Ros1Ultraarm_communicationSetAnglesClient const&)=delete;Ros1Ultraarm_communicationSetAnglesClient(Ros1Ultraarm_communicationSetAnglesClient&&)=delete;
~Ros1Ultraarm_communicationSetAnglesClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_ultraarm_communication_SetAngles_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ultraarm_communication/SetAngles server; advertise/respond stay separate authorities. */
class Ros1Ultraarm_communicationSetAnglesServer:public ServerOwners {
using Handler=std::function<Ros1Ultraarm_communicationSetAnglesResponse(Ros1Ultraarm_communicationSetAnglesRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Ultraarm_communicationSetAnglesRequest;using Response=Ros1Ultraarm_communicationSetAnglesResponse;
Ros1Ultraarm_communicationSetAnglesServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_ultraarm_communication_SetAngles_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_ultraarm_communication_SetAngles_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_ultraarm_communication_SetAngles_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_ultraarm_communication_SetAngles_respond(&respond_,ticket,&response).error;}
};
/** Complete custom ultraarm_communication/SetCoords client; default policy returns13 from construction. */
class Ros1Ultraarm_communicationSetCoordsClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Ultraarm_communicationSetCoordsRequest;using Response=Ros1Ultraarm_communicationSetCoordsResponse;
Ros1Ultraarm_communicationSetCoordsClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Ultraarm_communicationSetCoordsClient(Ros1Ultraarm_communicationSetCoordsClient const&)=delete;Ros1Ultraarm_communicationSetCoordsClient& operator=(Ros1Ultraarm_communicationSetCoordsClient const&)=delete;Ros1Ultraarm_communicationSetCoordsClient(Ros1Ultraarm_communicationSetCoordsClient&&)=delete;
~Ros1Ultraarm_communicationSetCoordsClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_ultraarm_communication_SetCoords_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ultraarm_communication/SetCoords server; advertise/respond stay separate authorities. */
class Ros1Ultraarm_communicationSetCoordsServer:public ServerOwners {
using Handler=std::function<Ros1Ultraarm_communicationSetCoordsResponse(Ros1Ultraarm_communicationSetCoordsRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Ultraarm_communicationSetCoordsRequest;using Response=Ros1Ultraarm_communicationSetCoordsResponse;
Ros1Ultraarm_communicationSetCoordsServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_ultraarm_communication_SetCoords_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_ultraarm_communication_SetCoords_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_ultraarm_communication_SetCoords_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_ultraarm_communication_SetCoords_respond(&respond_,ticket,&response).error;}
};
}
