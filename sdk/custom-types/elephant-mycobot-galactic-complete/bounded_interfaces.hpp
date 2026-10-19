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
/** Complete custom mecharm_interfaces/srv/GetAngles client; default policy returns13 from construction. */
class Ros2Mecharm_interfacesSrvGetAnglesClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Mecharm_interfacesSrvGetAngles_Request;using Response=Ros2Mecharm_interfacesSrvGetAngles_Response;
Ros2Mecharm_interfacesSrvGetAnglesClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Mecharm_interfacesSrvGetAnglesClient(Ros2Mecharm_interfacesSrvGetAnglesClient const&)=delete;Ros2Mecharm_interfacesSrvGetAnglesClient& operator=(Ros2Mecharm_interfacesSrvGetAnglesClient const&)=delete;Ros2Mecharm_interfacesSrvGetAnglesClient(Ros2Mecharm_interfacesSrvGetAnglesClient&&)=delete;
~Ros2Mecharm_interfacesSrvGetAnglesClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mecharm_interfaces/srv/GetAngles server; advertise/respond stay separate authorities. */
class Ros2Mecharm_interfacesSrvGetAnglesServer:public ServerOwners {
using Handler=std::function<Ros2Mecharm_interfacesSrvGetAngles_Response(Ros2Mecharm_interfacesSrvGetAngles_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Mecharm_interfacesSrvGetAngles_Request;using Response=Ros2Mecharm_interfacesSrvGetAngles_Response;
Ros2Mecharm_interfacesSrvGetAnglesServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_respond(&respond_,ticket,&response).error;}
};
/** Complete custom mecharm_interfaces/srv/GetCoords client; default policy returns13 from construction. */
class Ros2Mecharm_interfacesSrvGetCoordsClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Mecharm_interfacesSrvGetCoords_Request;using Response=Ros2Mecharm_interfacesSrvGetCoords_Response;
Ros2Mecharm_interfacesSrvGetCoordsClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Mecharm_interfacesSrvGetCoordsClient(Ros2Mecharm_interfacesSrvGetCoordsClient const&)=delete;Ros2Mecharm_interfacesSrvGetCoordsClient& operator=(Ros2Mecharm_interfacesSrvGetCoordsClient const&)=delete;Ros2Mecharm_interfacesSrvGetCoordsClient(Ros2Mecharm_interfacesSrvGetCoordsClient&&)=delete;
~Ros2Mecharm_interfacesSrvGetCoordsClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mecharm_interfaces/srv/GetCoords server; advertise/respond stay separate authorities. */
class Ros2Mecharm_interfacesSrvGetCoordsServer:public ServerOwners {
using Handler=std::function<Ros2Mecharm_interfacesSrvGetCoords_Response(Ros2Mecharm_interfacesSrvGetCoords_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Mecharm_interfacesSrvGetCoords_Request;using Response=Ros2Mecharm_interfacesSrvGetCoords_Response;
Ros2Mecharm_interfacesSrvGetCoordsServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_respond(&respond_,ticket,&response).error;}
};
/** Complete custom mecharm_interfaces/srv/GripperStatus client; default policy returns13 from construction. */
class Ros2Mecharm_interfacesSrvGripperStatusClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Mecharm_interfacesSrvGripperStatus_Request;using Response=Ros2Mecharm_interfacesSrvGripperStatus_Response;
Ros2Mecharm_interfacesSrvGripperStatusClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Mecharm_interfacesSrvGripperStatusClient(Ros2Mecharm_interfacesSrvGripperStatusClient const&)=delete;Ros2Mecharm_interfacesSrvGripperStatusClient& operator=(Ros2Mecharm_interfacesSrvGripperStatusClient const&)=delete;Ros2Mecharm_interfacesSrvGripperStatusClient(Ros2Mecharm_interfacesSrvGripperStatusClient&&)=delete;
~Ros2Mecharm_interfacesSrvGripperStatusClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mecharm_interfaces/srv/GripperStatus server; advertise/respond stay separate authorities. */
class Ros2Mecharm_interfacesSrvGripperStatusServer:public ServerOwners {
using Handler=std::function<Ros2Mecharm_interfacesSrvGripperStatus_Response(Ros2Mecharm_interfacesSrvGripperStatus_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Mecharm_interfacesSrvGripperStatus_Request;using Response=Ros2Mecharm_interfacesSrvGripperStatus_Response;
Ros2Mecharm_interfacesSrvGripperStatusServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_respond(&respond_,ticket,&response).error;}
};
/** Complete custom mecharm_interfaces/srv/PumpStatus client; default policy returns13 from construction. */
class Ros2Mecharm_interfacesSrvPumpStatusClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Mecharm_interfacesSrvPumpStatus_Request;using Response=Ros2Mecharm_interfacesSrvPumpStatus_Response;
Ros2Mecharm_interfacesSrvPumpStatusClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Mecharm_interfacesSrvPumpStatusClient(Ros2Mecharm_interfacesSrvPumpStatusClient const&)=delete;Ros2Mecharm_interfacesSrvPumpStatusClient& operator=(Ros2Mecharm_interfacesSrvPumpStatusClient const&)=delete;Ros2Mecharm_interfacesSrvPumpStatusClient(Ros2Mecharm_interfacesSrvPumpStatusClient&&)=delete;
~Ros2Mecharm_interfacesSrvPumpStatusClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mecharm_interfaces/srv/PumpStatus server; advertise/respond stay separate authorities. */
class Ros2Mecharm_interfacesSrvPumpStatusServer:public ServerOwners {
using Handler=std::function<Ros2Mecharm_interfacesSrvPumpStatus_Response(Ros2Mecharm_interfacesSrvPumpStatus_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Mecharm_interfacesSrvPumpStatus_Request;using Response=Ros2Mecharm_interfacesSrvPumpStatus_Response;
Ros2Mecharm_interfacesSrvPumpStatusServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_respond(&respond_,ticket,&response).error;}
};
/** Complete custom mecharm_interfaces/srv/SetAngles client; default policy returns13 from construction. */
class Ros2Mecharm_interfacesSrvSetAnglesClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Mecharm_interfacesSrvSetAngles_Request;using Response=Ros2Mecharm_interfacesSrvSetAngles_Response;
Ros2Mecharm_interfacesSrvSetAnglesClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Mecharm_interfacesSrvSetAnglesClient(Ros2Mecharm_interfacesSrvSetAnglesClient const&)=delete;Ros2Mecharm_interfacesSrvSetAnglesClient& operator=(Ros2Mecharm_interfacesSrvSetAnglesClient const&)=delete;Ros2Mecharm_interfacesSrvSetAnglesClient(Ros2Mecharm_interfacesSrvSetAnglesClient&&)=delete;
~Ros2Mecharm_interfacesSrvSetAnglesClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mecharm_interfaces/srv/SetAngles server; advertise/respond stay separate authorities. */
class Ros2Mecharm_interfacesSrvSetAnglesServer:public ServerOwners {
using Handler=std::function<Ros2Mecharm_interfacesSrvSetAngles_Response(Ros2Mecharm_interfacesSrvSetAngles_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Mecharm_interfacesSrvSetAngles_Request;using Response=Ros2Mecharm_interfacesSrvSetAngles_Response;
Ros2Mecharm_interfacesSrvSetAnglesServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_respond(&respond_,ticket,&response).error;}
};
/** Complete custom mecharm_interfaces/srv/SetCoords client; default policy returns13 from construction. */
class Ros2Mecharm_interfacesSrvSetCoordsClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Mecharm_interfacesSrvSetCoords_Request;using Response=Ros2Mecharm_interfacesSrvSetCoords_Response;
Ros2Mecharm_interfacesSrvSetCoordsClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Mecharm_interfacesSrvSetCoordsClient(Ros2Mecharm_interfacesSrvSetCoordsClient const&)=delete;Ros2Mecharm_interfacesSrvSetCoordsClient& operator=(Ros2Mecharm_interfacesSrvSetCoordsClient const&)=delete;Ros2Mecharm_interfacesSrvSetCoordsClient(Ros2Mecharm_interfacesSrvSetCoordsClient&&)=delete;
~Ros2Mecharm_interfacesSrvSetCoordsClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mecharm_interfaces/srv/SetCoords server; advertise/respond stay separate authorities. */
class Ros2Mecharm_interfacesSrvSetCoordsServer:public ServerOwners {
using Handler=std::function<Ros2Mecharm_interfacesSrvSetCoords_Response(Ros2Mecharm_interfacesSrvSetCoords_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Mecharm_interfacesSrvSetCoords_Request;using Response=Ros2Mecharm_interfacesSrvSetCoords_Response;
Ros2Mecharm_interfacesSrvSetCoordsServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_respond(&respond_,ticket,&response).error;}
};
/** Complete custom mycobot_interfaces/srv/GetAngles client; default policy returns13 from construction. */
class Ros2Mycobot_interfacesSrvGetAnglesClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Mycobot_interfacesSrvGetAngles_Request;using Response=Ros2Mycobot_interfacesSrvGetAngles_Response;
Ros2Mycobot_interfacesSrvGetAnglesClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Mycobot_interfacesSrvGetAnglesClient(Ros2Mycobot_interfacesSrvGetAnglesClient const&)=delete;Ros2Mycobot_interfacesSrvGetAnglesClient& operator=(Ros2Mycobot_interfacesSrvGetAnglesClient const&)=delete;Ros2Mycobot_interfacesSrvGetAnglesClient(Ros2Mycobot_interfacesSrvGetAnglesClient&&)=delete;
~Ros2Mycobot_interfacesSrvGetAnglesClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mycobot_interfaces/srv/GetAngles server; advertise/respond stay separate authorities. */
class Ros2Mycobot_interfacesSrvGetAnglesServer:public ServerOwners {
using Handler=std::function<Ros2Mycobot_interfacesSrvGetAngles_Response(Ros2Mycobot_interfacesSrvGetAngles_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Mycobot_interfacesSrvGetAngles_Request;using Response=Ros2Mycobot_interfacesSrvGetAngles_Response;
Ros2Mycobot_interfacesSrvGetAnglesServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_respond(&respond_,ticket,&response).error;}
};
/** Complete custom mycobot_interfaces/srv/GetCoords client; default policy returns13 from construction. */
class Ros2Mycobot_interfacesSrvGetCoordsClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Mycobot_interfacesSrvGetCoords_Request;using Response=Ros2Mycobot_interfacesSrvGetCoords_Response;
Ros2Mycobot_interfacesSrvGetCoordsClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Mycobot_interfacesSrvGetCoordsClient(Ros2Mycobot_interfacesSrvGetCoordsClient const&)=delete;Ros2Mycobot_interfacesSrvGetCoordsClient& operator=(Ros2Mycobot_interfacesSrvGetCoordsClient const&)=delete;Ros2Mycobot_interfacesSrvGetCoordsClient(Ros2Mycobot_interfacesSrvGetCoordsClient&&)=delete;
~Ros2Mycobot_interfacesSrvGetCoordsClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mycobot_interfaces/srv/GetCoords server; advertise/respond stay separate authorities. */
class Ros2Mycobot_interfacesSrvGetCoordsServer:public ServerOwners {
using Handler=std::function<Ros2Mycobot_interfacesSrvGetCoords_Response(Ros2Mycobot_interfacesSrvGetCoords_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Mycobot_interfacesSrvGetCoords_Request;using Response=Ros2Mycobot_interfacesSrvGetCoords_Response;
Ros2Mycobot_interfacesSrvGetCoordsServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_respond(&respond_,ticket,&response).error;}
};
/** Complete custom mycobot_interfaces/srv/GripperStatus client; default policy returns13 from construction. */
class Ros2Mycobot_interfacesSrvGripperStatusClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Mycobot_interfacesSrvGripperStatus_Request;using Response=Ros2Mycobot_interfacesSrvGripperStatus_Response;
Ros2Mycobot_interfacesSrvGripperStatusClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Mycobot_interfacesSrvGripperStatusClient(Ros2Mycobot_interfacesSrvGripperStatusClient const&)=delete;Ros2Mycobot_interfacesSrvGripperStatusClient& operator=(Ros2Mycobot_interfacesSrvGripperStatusClient const&)=delete;Ros2Mycobot_interfacesSrvGripperStatusClient(Ros2Mycobot_interfacesSrvGripperStatusClient&&)=delete;
~Ros2Mycobot_interfacesSrvGripperStatusClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mycobot_interfaces/srv/GripperStatus server; advertise/respond stay separate authorities. */
class Ros2Mycobot_interfacesSrvGripperStatusServer:public ServerOwners {
using Handler=std::function<Ros2Mycobot_interfacesSrvGripperStatus_Response(Ros2Mycobot_interfacesSrvGripperStatus_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Mycobot_interfacesSrvGripperStatus_Request;using Response=Ros2Mycobot_interfacesSrvGripperStatus_Response;
Ros2Mycobot_interfacesSrvGripperStatusServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_respond(&respond_,ticket,&response).error;}
};
/** Complete custom mycobot_interfaces/srv/PumpStatus client; default policy returns13 from construction. */
class Ros2Mycobot_interfacesSrvPumpStatusClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Mycobot_interfacesSrvPumpStatus_Request;using Response=Ros2Mycobot_interfacesSrvPumpStatus_Response;
Ros2Mycobot_interfacesSrvPumpStatusClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Mycobot_interfacesSrvPumpStatusClient(Ros2Mycobot_interfacesSrvPumpStatusClient const&)=delete;Ros2Mycobot_interfacesSrvPumpStatusClient& operator=(Ros2Mycobot_interfacesSrvPumpStatusClient const&)=delete;Ros2Mycobot_interfacesSrvPumpStatusClient(Ros2Mycobot_interfacesSrvPumpStatusClient&&)=delete;
~Ros2Mycobot_interfacesSrvPumpStatusClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mycobot_interfaces/srv/PumpStatus server; advertise/respond stay separate authorities. */
class Ros2Mycobot_interfacesSrvPumpStatusServer:public ServerOwners {
using Handler=std::function<Ros2Mycobot_interfacesSrvPumpStatus_Response(Ros2Mycobot_interfacesSrvPumpStatus_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Mycobot_interfacesSrvPumpStatus_Request;using Response=Ros2Mycobot_interfacesSrvPumpStatus_Response;
Ros2Mycobot_interfacesSrvPumpStatusServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_respond(&respond_,ticket,&response).error;}
};
/** Complete custom mycobot_interfaces/srv/SetAngles client; default policy returns13 from construction. */
class Ros2Mycobot_interfacesSrvSetAnglesClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Mycobot_interfacesSrvSetAngles_Request;using Response=Ros2Mycobot_interfacesSrvSetAngles_Response;
Ros2Mycobot_interfacesSrvSetAnglesClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Mycobot_interfacesSrvSetAnglesClient(Ros2Mycobot_interfacesSrvSetAnglesClient const&)=delete;Ros2Mycobot_interfacesSrvSetAnglesClient& operator=(Ros2Mycobot_interfacesSrvSetAnglesClient const&)=delete;Ros2Mycobot_interfacesSrvSetAnglesClient(Ros2Mycobot_interfacesSrvSetAnglesClient&&)=delete;
~Ros2Mycobot_interfacesSrvSetAnglesClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mycobot_interfaces/srv/SetAngles server; advertise/respond stay separate authorities. */
class Ros2Mycobot_interfacesSrvSetAnglesServer:public ServerOwners {
using Handler=std::function<Ros2Mycobot_interfacesSrvSetAngles_Response(Ros2Mycobot_interfacesSrvSetAngles_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Mycobot_interfacesSrvSetAngles_Request;using Response=Ros2Mycobot_interfacesSrvSetAngles_Response;
Ros2Mycobot_interfacesSrvSetAnglesServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_respond(&respond_,ticket,&response).error;}
};
/** Complete custom mycobot_interfaces/srv/SetCoords client; default policy returns13 from construction. */
class Ros2Mycobot_interfacesSrvSetCoordsClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Mycobot_interfacesSrvSetCoords_Request;using Response=Ros2Mycobot_interfacesSrvSetCoords_Response;
Ros2Mycobot_interfacesSrvSetCoordsClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Mycobot_interfacesSrvSetCoordsClient(Ros2Mycobot_interfacesSrvSetCoordsClient const&)=delete;Ros2Mycobot_interfacesSrvSetCoordsClient& operator=(Ros2Mycobot_interfacesSrvSetCoordsClient const&)=delete;Ros2Mycobot_interfacesSrvSetCoordsClient(Ros2Mycobot_interfacesSrvSetCoordsClient&&)=delete;
~Ros2Mycobot_interfacesSrvSetCoordsClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mycobot_interfaces/srv/SetCoords server; advertise/respond stay separate authorities. */
class Ros2Mycobot_interfacesSrvSetCoordsServer:public ServerOwners {
using Handler=std::function<Ros2Mycobot_interfacesSrvSetCoords_Response(Ros2Mycobot_interfacesSrvSetCoords_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Mycobot_interfacesSrvSetCoords_Request;using Response=Ros2Mycobot_interfacesSrvSetCoords_Response;
Ros2Mycobot_interfacesSrvSetCoordsServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_respond(&respond_,ticket,&response).error;}
};
/** Complete custom mycobot_pro450_interfaces/srv/GetAngles client; default policy returns13 from construction. */
class Ros2Mycobot_pro450_interfacesSrvGetAnglesClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Mycobot_pro450_interfacesSrvGetAngles_Request;using Response=Ros2Mycobot_pro450_interfacesSrvGetAngles_Response;
Ros2Mycobot_pro450_interfacesSrvGetAnglesClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Mycobot_pro450_interfacesSrvGetAnglesClient(Ros2Mycobot_pro450_interfacesSrvGetAnglesClient const&)=delete;Ros2Mycobot_pro450_interfacesSrvGetAnglesClient& operator=(Ros2Mycobot_pro450_interfacesSrvGetAnglesClient const&)=delete;Ros2Mycobot_pro450_interfacesSrvGetAnglesClient(Ros2Mycobot_pro450_interfacesSrvGetAnglesClient&&)=delete;
~Ros2Mycobot_pro450_interfacesSrvGetAnglesClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mycobot_pro450_interfaces/srv/GetAngles server; advertise/respond stay separate authorities. */
class Ros2Mycobot_pro450_interfacesSrvGetAnglesServer:public ServerOwners {
using Handler=std::function<Ros2Mycobot_pro450_interfacesSrvGetAngles_Response(Ros2Mycobot_pro450_interfacesSrvGetAngles_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Mycobot_pro450_interfacesSrvGetAngles_Request;using Response=Ros2Mycobot_pro450_interfacesSrvGetAngles_Response;
Ros2Mycobot_pro450_interfacesSrvGetAnglesServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_respond(&respond_,ticket,&response).error;}
};
/** Complete custom mycobot_pro450_interfaces/srv/GetCoords client; default policy returns13 from construction. */
class Ros2Mycobot_pro450_interfacesSrvGetCoordsClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Mycobot_pro450_interfacesSrvGetCoords_Request;using Response=Ros2Mycobot_pro450_interfacesSrvGetCoords_Response;
Ros2Mycobot_pro450_interfacesSrvGetCoordsClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Mycobot_pro450_interfacesSrvGetCoordsClient(Ros2Mycobot_pro450_interfacesSrvGetCoordsClient const&)=delete;Ros2Mycobot_pro450_interfacesSrvGetCoordsClient& operator=(Ros2Mycobot_pro450_interfacesSrvGetCoordsClient const&)=delete;Ros2Mycobot_pro450_interfacesSrvGetCoordsClient(Ros2Mycobot_pro450_interfacesSrvGetCoordsClient&&)=delete;
~Ros2Mycobot_pro450_interfacesSrvGetCoordsClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mycobot_pro450_interfaces/srv/GetCoords server; advertise/respond stay separate authorities. */
class Ros2Mycobot_pro450_interfacesSrvGetCoordsServer:public ServerOwners {
using Handler=std::function<Ros2Mycobot_pro450_interfacesSrvGetCoords_Response(Ros2Mycobot_pro450_interfacesSrvGetCoords_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Mycobot_pro450_interfacesSrvGetCoords_Request;using Response=Ros2Mycobot_pro450_interfacesSrvGetCoords_Response;
Ros2Mycobot_pro450_interfacesSrvGetCoordsServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_respond(&respond_,ticket,&response).error;}
};
/** Complete custom mycobot_pro450_interfaces/srv/GetGripperValue client; default policy returns13 from construction. */
class Ros2Mycobot_pro450_interfacesSrvGetGripperValueClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Request;using Response=Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Response;
Ros2Mycobot_pro450_interfacesSrvGetGripperValueClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Mycobot_pro450_interfacesSrvGetGripperValueClient(Ros2Mycobot_pro450_interfacesSrvGetGripperValueClient const&)=delete;Ros2Mycobot_pro450_interfacesSrvGetGripperValueClient& operator=(Ros2Mycobot_pro450_interfacesSrvGetGripperValueClient const&)=delete;Ros2Mycobot_pro450_interfacesSrvGetGripperValueClient(Ros2Mycobot_pro450_interfacesSrvGetGripperValueClient&&)=delete;
~Ros2Mycobot_pro450_interfacesSrvGetGripperValueClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mycobot_pro450_interfaces/srv/GetGripperValue server; advertise/respond stay separate authorities. */
class Ros2Mycobot_pro450_interfacesSrvGetGripperValueServer:public ServerOwners {
using Handler=std::function<Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Response(Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Request;using Response=Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Response;
Ros2Mycobot_pro450_interfacesSrvGetGripperValueServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_respond(&respond_,ticket,&response).error;}
};
/** Complete custom mycobot_pro450_interfaces/srv/GripperStatus client; default policy returns13 from construction. */
class Ros2Mycobot_pro450_interfacesSrvGripperStatusClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Mycobot_pro450_interfacesSrvGripperStatus_Request;using Response=Ros2Mycobot_pro450_interfacesSrvGripperStatus_Response;
Ros2Mycobot_pro450_interfacesSrvGripperStatusClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Mycobot_pro450_interfacesSrvGripperStatusClient(Ros2Mycobot_pro450_interfacesSrvGripperStatusClient const&)=delete;Ros2Mycobot_pro450_interfacesSrvGripperStatusClient& operator=(Ros2Mycobot_pro450_interfacesSrvGripperStatusClient const&)=delete;Ros2Mycobot_pro450_interfacesSrvGripperStatusClient(Ros2Mycobot_pro450_interfacesSrvGripperStatusClient&&)=delete;
~Ros2Mycobot_pro450_interfacesSrvGripperStatusClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mycobot_pro450_interfaces/srv/GripperStatus server; advertise/respond stay separate authorities. */
class Ros2Mycobot_pro450_interfacesSrvGripperStatusServer:public ServerOwners {
using Handler=std::function<Ros2Mycobot_pro450_interfacesSrvGripperStatus_Response(Ros2Mycobot_pro450_interfacesSrvGripperStatus_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Mycobot_pro450_interfacesSrvGripperStatus_Request;using Response=Ros2Mycobot_pro450_interfacesSrvGripperStatus_Response;
Ros2Mycobot_pro450_interfacesSrvGripperStatusServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_respond(&respond_,ticket,&response).error;}
};
/** Complete custom mycobot_pro450_interfaces/srv/SetAngles client; default policy returns13 from construction. */
class Ros2Mycobot_pro450_interfacesSrvSetAnglesClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Mycobot_pro450_interfacesSrvSetAngles_Request;using Response=Ros2Mycobot_pro450_interfacesSrvSetAngles_Response;
Ros2Mycobot_pro450_interfacesSrvSetAnglesClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Mycobot_pro450_interfacesSrvSetAnglesClient(Ros2Mycobot_pro450_interfacesSrvSetAnglesClient const&)=delete;Ros2Mycobot_pro450_interfacesSrvSetAnglesClient& operator=(Ros2Mycobot_pro450_interfacesSrvSetAnglesClient const&)=delete;Ros2Mycobot_pro450_interfacesSrvSetAnglesClient(Ros2Mycobot_pro450_interfacesSrvSetAnglesClient&&)=delete;
~Ros2Mycobot_pro450_interfacesSrvSetAnglesClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mycobot_pro450_interfaces/srv/SetAngles server; advertise/respond stay separate authorities. */
class Ros2Mycobot_pro450_interfacesSrvSetAnglesServer:public ServerOwners {
using Handler=std::function<Ros2Mycobot_pro450_interfacesSrvSetAngles_Response(Ros2Mycobot_pro450_interfacesSrvSetAngles_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Mycobot_pro450_interfacesSrvSetAngles_Request;using Response=Ros2Mycobot_pro450_interfacesSrvSetAngles_Response;
Ros2Mycobot_pro450_interfacesSrvSetAnglesServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_respond(&respond_,ticket,&response).error;}
};
/** Complete custom mycobot_pro450_interfaces/srv/SetCoords client; default policy returns13 from construction. */
class Ros2Mycobot_pro450_interfacesSrvSetCoordsClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Mycobot_pro450_interfacesSrvSetCoords_Request;using Response=Ros2Mycobot_pro450_interfacesSrvSetCoords_Response;
Ros2Mycobot_pro450_interfacesSrvSetCoordsClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Mycobot_pro450_interfacesSrvSetCoordsClient(Ros2Mycobot_pro450_interfacesSrvSetCoordsClient const&)=delete;Ros2Mycobot_pro450_interfacesSrvSetCoordsClient& operator=(Ros2Mycobot_pro450_interfacesSrvSetCoordsClient const&)=delete;Ros2Mycobot_pro450_interfacesSrvSetCoordsClient(Ros2Mycobot_pro450_interfacesSrvSetCoordsClient&&)=delete;
~Ros2Mycobot_pro450_interfacesSrvSetCoordsClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mycobot_pro450_interfaces/srv/SetCoords server; advertise/respond stay separate authorities. */
class Ros2Mycobot_pro450_interfacesSrvSetCoordsServer:public ServerOwners {
using Handler=std::function<Ros2Mycobot_pro450_interfacesSrvSetCoords_Response(Ros2Mycobot_pro450_interfacesSrvSetCoords_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Mycobot_pro450_interfacesSrvSetCoords_Request;using Response=Ros2Mycobot_pro450_interfacesSrvSetCoords_Response;
Ros2Mycobot_pro450_interfacesSrvSetCoordsServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_respond(&respond_,ticket,&response).error;}
};
/** Complete custom mypalletizer_interfaces/srv/GetAngles client; default policy returns13 from construction. */
class Ros2Mypalletizer_interfacesSrvGetAnglesClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Mypalletizer_interfacesSrvGetAngles_Request;using Response=Ros2Mypalletizer_interfacesSrvGetAngles_Response;
Ros2Mypalletizer_interfacesSrvGetAnglesClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Mypalletizer_interfacesSrvGetAnglesClient(Ros2Mypalletizer_interfacesSrvGetAnglesClient const&)=delete;Ros2Mypalletizer_interfacesSrvGetAnglesClient& operator=(Ros2Mypalletizer_interfacesSrvGetAnglesClient const&)=delete;Ros2Mypalletizer_interfacesSrvGetAnglesClient(Ros2Mypalletizer_interfacesSrvGetAnglesClient&&)=delete;
~Ros2Mypalletizer_interfacesSrvGetAnglesClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mypalletizer_interfaces/srv/GetAngles server; advertise/respond stay separate authorities. */
class Ros2Mypalletizer_interfacesSrvGetAnglesServer:public ServerOwners {
using Handler=std::function<Ros2Mypalletizer_interfacesSrvGetAngles_Response(Ros2Mypalletizer_interfacesSrvGetAngles_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Mypalletizer_interfacesSrvGetAngles_Request;using Response=Ros2Mypalletizer_interfacesSrvGetAngles_Response;
Ros2Mypalletizer_interfacesSrvGetAnglesServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_respond(&respond_,ticket,&response).error;}
};
/** Complete custom mypalletizer_interfaces/srv/GetCoords client; default policy returns13 from construction. */
class Ros2Mypalletizer_interfacesSrvGetCoordsClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Mypalletizer_interfacesSrvGetCoords_Request;using Response=Ros2Mypalletizer_interfacesSrvGetCoords_Response;
Ros2Mypalletizer_interfacesSrvGetCoordsClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Mypalletizer_interfacesSrvGetCoordsClient(Ros2Mypalletizer_interfacesSrvGetCoordsClient const&)=delete;Ros2Mypalletizer_interfacesSrvGetCoordsClient& operator=(Ros2Mypalletizer_interfacesSrvGetCoordsClient const&)=delete;Ros2Mypalletizer_interfacesSrvGetCoordsClient(Ros2Mypalletizer_interfacesSrvGetCoordsClient&&)=delete;
~Ros2Mypalletizer_interfacesSrvGetCoordsClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mypalletizer_interfaces/srv/GetCoords server; advertise/respond stay separate authorities. */
class Ros2Mypalletizer_interfacesSrvGetCoordsServer:public ServerOwners {
using Handler=std::function<Ros2Mypalletizer_interfacesSrvGetCoords_Response(Ros2Mypalletizer_interfacesSrvGetCoords_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Mypalletizer_interfacesSrvGetCoords_Request;using Response=Ros2Mypalletizer_interfacesSrvGetCoords_Response;
Ros2Mypalletizer_interfacesSrvGetCoordsServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_respond(&respond_,ticket,&response).error;}
};
/** Complete custom mypalletizer_interfaces/srv/GripperStatus client; default policy returns13 from construction. */
class Ros2Mypalletizer_interfacesSrvGripperStatusClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Mypalletizer_interfacesSrvGripperStatus_Request;using Response=Ros2Mypalletizer_interfacesSrvGripperStatus_Response;
Ros2Mypalletizer_interfacesSrvGripperStatusClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Mypalletizer_interfacesSrvGripperStatusClient(Ros2Mypalletizer_interfacesSrvGripperStatusClient const&)=delete;Ros2Mypalletizer_interfacesSrvGripperStatusClient& operator=(Ros2Mypalletizer_interfacesSrvGripperStatusClient const&)=delete;Ros2Mypalletizer_interfacesSrvGripperStatusClient(Ros2Mypalletizer_interfacesSrvGripperStatusClient&&)=delete;
~Ros2Mypalletizer_interfacesSrvGripperStatusClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mypalletizer_interfaces/srv/GripperStatus server; advertise/respond stay separate authorities. */
class Ros2Mypalletizer_interfacesSrvGripperStatusServer:public ServerOwners {
using Handler=std::function<Ros2Mypalletizer_interfacesSrvGripperStatus_Response(Ros2Mypalletizer_interfacesSrvGripperStatus_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Mypalletizer_interfacesSrvGripperStatus_Request;using Response=Ros2Mypalletizer_interfacesSrvGripperStatus_Response;
Ros2Mypalletizer_interfacesSrvGripperStatusServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_respond(&respond_,ticket,&response).error;}
};
/** Complete custom mypalletizer_interfaces/srv/PumpStatus client; default policy returns13 from construction. */
class Ros2Mypalletizer_interfacesSrvPumpStatusClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Mypalletizer_interfacesSrvPumpStatus_Request;using Response=Ros2Mypalletizer_interfacesSrvPumpStatus_Response;
Ros2Mypalletizer_interfacesSrvPumpStatusClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Mypalletizer_interfacesSrvPumpStatusClient(Ros2Mypalletizer_interfacesSrvPumpStatusClient const&)=delete;Ros2Mypalletizer_interfacesSrvPumpStatusClient& operator=(Ros2Mypalletizer_interfacesSrvPumpStatusClient const&)=delete;Ros2Mypalletizer_interfacesSrvPumpStatusClient(Ros2Mypalletizer_interfacesSrvPumpStatusClient&&)=delete;
~Ros2Mypalletizer_interfacesSrvPumpStatusClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mypalletizer_interfaces/srv/PumpStatus server; advertise/respond stay separate authorities. */
class Ros2Mypalletizer_interfacesSrvPumpStatusServer:public ServerOwners {
using Handler=std::function<Ros2Mypalletizer_interfacesSrvPumpStatus_Response(Ros2Mypalletizer_interfacesSrvPumpStatus_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Mypalletizer_interfacesSrvPumpStatus_Request;using Response=Ros2Mypalletizer_interfacesSrvPumpStatus_Response;
Ros2Mypalletizer_interfacesSrvPumpStatusServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_respond(&respond_,ticket,&response).error;}
};
/** Complete custom mypalletizer_interfaces/srv/SetAngles client; default policy returns13 from construction. */
class Ros2Mypalletizer_interfacesSrvSetAnglesClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Mypalletizer_interfacesSrvSetAngles_Request;using Response=Ros2Mypalletizer_interfacesSrvSetAngles_Response;
Ros2Mypalletizer_interfacesSrvSetAnglesClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Mypalletizer_interfacesSrvSetAnglesClient(Ros2Mypalletizer_interfacesSrvSetAnglesClient const&)=delete;Ros2Mypalletizer_interfacesSrvSetAnglesClient& operator=(Ros2Mypalletizer_interfacesSrvSetAnglesClient const&)=delete;Ros2Mypalletizer_interfacesSrvSetAnglesClient(Ros2Mypalletizer_interfacesSrvSetAnglesClient&&)=delete;
~Ros2Mypalletizer_interfacesSrvSetAnglesClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mypalletizer_interfaces/srv/SetAngles server; advertise/respond stay separate authorities. */
class Ros2Mypalletizer_interfacesSrvSetAnglesServer:public ServerOwners {
using Handler=std::function<Ros2Mypalletizer_interfacesSrvSetAngles_Response(Ros2Mypalletizer_interfacesSrvSetAngles_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Mypalletizer_interfacesSrvSetAngles_Request;using Response=Ros2Mypalletizer_interfacesSrvSetAngles_Response;
Ros2Mypalletizer_interfacesSrvSetAnglesServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_respond(&respond_,ticket,&response).error;}
};
/** Complete custom mypalletizer_interfaces/srv/SetCoords client; default policy returns13 from construction. */
class Ros2Mypalletizer_interfacesSrvSetCoordsClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Mypalletizer_interfacesSrvSetCoords_Request;using Response=Ros2Mypalletizer_interfacesSrvSetCoords_Response;
Ros2Mypalletizer_interfacesSrvSetCoordsClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Mypalletizer_interfacesSrvSetCoordsClient(Ros2Mypalletizer_interfacesSrvSetCoordsClient const&)=delete;Ros2Mypalletizer_interfacesSrvSetCoordsClient& operator=(Ros2Mypalletizer_interfacesSrvSetCoordsClient const&)=delete;Ros2Mypalletizer_interfacesSrvSetCoordsClient(Ros2Mypalletizer_interfacesSrvSetCoordsClient&&)=delete;
~Ros2Mypalletizer_interfacesSrvSetCoordsClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom mypalletizer_interfaces/srv/SetCoords server; advertise/respond stay separate authorities. */
class Ros2Mypalletizer_interfacesSrvSetCoordsServer:public ServerOwners {
using Handler=std::function<Ros2Mypalletizer_interfacesSrvSetCoords_Response(Ros2Mypalletizer_interfacesSrvSetCoords_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Mypalletizer_interfacesSrvSetCoords_Request;using Response=Ros2Mypalletizer_interfacesSrvSetCoords_Response;
Ros2Mypalletizer_interfacesSrvSetCoordsServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_respond(&respond_,ticket,&response).error;}
};
}
