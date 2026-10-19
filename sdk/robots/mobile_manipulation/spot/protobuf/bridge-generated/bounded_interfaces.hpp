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
/** Complete custom uwvm_spot_bridge/srv/Command client; default policy returns13 from construction. */
class Ros2Uwvm_spot_bridgeSrvCommandClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Uwvm_spot_bridgeSrvCommand_Request;using Response=Ros2Uwvm_spot_bridgeSrvCommand_Response;
Ros2Uwvm_spot_bridgeSrvCommandClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Uwvm_spot_bridgeSrvCommandClient(Ros2Uwvm_spot_bridgeSrvCommandClient const&)=delete;Ros2Uwvm_spot_bridgeSrvCommandClient& operator=(Ros2Uwvm_spot_bridgeSrvCommandClient const&)=delete;Ros2Uwvm_spot_bridgeSrvCommandClient(Ros2Uwvm_spot_bridgeSrvCommandClient&&)=delete;
~Ros2Uwvm_spot_bridgeSrvCommandClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom uwvm_spot_bridge/srv/Command server; advertise/respond stay separate authorities. */
class Ros2Uwvm_spot_bridgeSrvCommandServer:public ServerOwners {
using Handler=std::function<Ros2Uwvm_spot_bridgeSrvCommand_Response(Ros2Uwvm_spot_bridgeSrvCommand_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Uwvm_spot_bridgeSrvCommand_Request;using Response=Ros2Uwvm_spot_bridgeSrvCommand_Response;
Ros2Uwvm_spot_bridgeSrvCommandServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_respond(&respond_,ticket,&response).error;}
};
/** Complete custom uwvm_spot_bridge/srv/Feedback client; default policy returns13 from construction. */
class Ros2Uwvm_spot_bridgeSrvFeedbackClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Uwvm_spot_bridgeSrvFeedback_Request;using Response=Ros2Uwvm_spot_bridgeSrvFeedback_Response;
Ros2Uwvm_spot_bridgeSrvFeedbackClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Uwvm_spot_bridgeSrvFeedbackClient(Ros2Uwvm_spot_bridgeSrvFeedbackClient const&)=delete;Ros2Uwvm_spot_bridgeSrvFeedbackClient& operator=(Ros2Uwvm_spot_bridgeSrvFeedbackClient const&)=delete;Ros2Uwvm_spot_bridgeSrvFeedbackClient(Ros2Uwvm_spot_bridgeSrvFeedbackClient&&)=delete;
~Ros2Uwvm_spot_bridgeSrvFeedbackClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom uwvm_spot_bridge/srv/Feedback server; advertise/respond stay separate authorities. */
class Ros2Uwvm_spot_bridgeSrvFeedbackServer:public ServerOwners {
using Handler=std::function<Ros2Uwvm_spot_bridgeSrvFeedback_Response(Ros2Uwvm_spot_bridgeSrvFeedback_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Uwvm_spot_bridgeSrvFeedback_Request;using Response=Ros2Uwvm_spot_bridgeSrvFeedback_Response;
Ros2Uwvm_spot_bridgeSrvFeedbackServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_respond(&respond_,ticket,&response).error;}
};
/** Complete custom uwvm_spot_bridge/srv/Lease client; default policy returns13 from construction. */
class Ros2Uwvm_spot_bridgeSrvLeaseClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Uwvm_spot_bridgeSrvLease_Request;using Response=Ros2Uwvm_spot_bridgeSrvLease_Response;
Ros2Uwvm_spot_bridgeSrvLeaseClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Uwvm_spot_bridgeSrvLeaseClient(Ros2Uwvm_spot_bridgeSrvLeaseClient const&)=delete;Ros2Uwvm_spot_bridgeSrvLeaseClient& operator=(Ros2Uwvm_spot_bridgeSrvLeaseClient const&)=delete;Ros2Uwvm_spot_bridgeSrvLeaseClient(Ros2Uwvm_spot_bridgeSrvLeaseClient&&)=delete;
~Ros2Uwvm_spot_bridgeSrvLeaseClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom uwvm_spot_bridge/srv/Lease server; advertise/respond stay separate authorities. */
class Ros2Uwvm_spot_bridgeSrvLeaseServer:public ServerOwners {
using Handler=std::function<Ros2Uwvm_spot_bridgeSrvLease_Response(Ros2Uwvm_spot_bridgeSrvLease_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Uwvm_spot_bridgeSrvLease_Request;using Response=Ros2Uwvm_spot_bridgeSrvLease_Response;
Ros2Uwvm_spot_bridgeSrvLeaseServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_respond(&respond_,ticket,&response).error;}
};
/** Complete custom uwvm_spot_bridge/srv/State client; default policy returns13 from construction. */
class Ros2Uwvm_spot_bridgeSrvStateClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Uwvm_spot_bridgeSrvState_Request;using Response=Ros2Uwvm_spot_bridgeSrvState_Response;
Ros2Uwvm_spot_bridgeSrvStateClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Uwvm_spot_bridgeSrvStateClient(Ros2Uwvm_spot_bridgeSrvStateClient const&)=delete;Ros2Uwvm_spot_bridgeSrvStateClient& operator=(Ros2Uwvm_spot_bridgeSrvStateClient const&)=delete;Ros2Uwvm_spot_bridgeSrvStateClient(Ros2Uwvm_spot_bridgeSrvStateClient&&)=delete;
~Ros2Uwvm_spot_bridgeSrvStateClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_uwvm_spot_bridge_srv_State_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom uwvm_spot_bridge/srv/State server; advertise/respond stay separate authorities. */
class Ros2Uwvm_spot_bridgeSrvStateServer:public ServerOwners {
using Handler=std::function<Ros2Uwvm_spot_bridgeSrvState_Response(Ros2Uwvm_spot_bridgeSrvState_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Uwvm_spot_bridgeSrvState_Request;using Response=Ros2Uwvm_spot_bridgeSrvState_Response;
Ros2Uwvm_spot_bridgeSrvStateServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_uwvm_spot_bridge_srv_State_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_uwvm_spot_bridge_srv_State_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_uwvm_spot_bridge_srv_State_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_uwvm_spot_bridge_srv_State_respond(&respond_,ticket,&response).error;}
};
/** Complete custom uwvm_spot_bridge/srv/Velocity client; default policy returns13 from construction. */
class Ros2Uwvm_spot_bridgeSrvVelocityClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Uwvm_spot_bridgeSrvVelocity_Request;using Response=Ros2Uwvm_spot_bridgeSrvVelocity_Response;
Ros2Uwvm_spot_bridgeSrvVelocityClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Uwvm_spot_bridgeSrvVelocityClient(Ros2Uwvm_spot_bridgeSrvVelocityClient const&)=delete;Ros2Uwvm_spot_bridgeSrvVelocityClient& operator=(Ros2Uwvm_spot_bridgeSrvVelocityClient const&)=delete;Ros2Uwvm_spot_bridgeSrvVelocityClient(Ros2Uwvm_spot_bridgeSrvVelocityClient&&)=delete;
~Ros2Uwvm_spot_bridgeSrvVelocityClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom uwvm_spot_bridge/srv/Velocity server; advertise/respond stay separate authorities. */
class Ros2Uwvm_spot_bridgeSrvVelocityServer:public ServerOwners {
using Handler=std::function<Ros2Uwvm_spot_bridgeSrvVelocity_Response(Ros2Uwvm_spot_bridgeSrvVelocity_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Uwvm_spot_bridgeSrvVelocity_Request;using Response=Ros2Uwvm_spot_bridgeSrvVelocity_Response;
Ros2Uwvm_spot_bridgeSrvVelocityServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_respond(&respond_,ticket,&response).error;}
};
}
