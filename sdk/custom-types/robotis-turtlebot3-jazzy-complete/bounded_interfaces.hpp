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
/** Complete custom std_srvs/srv/SetBool client; default policy returns13 from construction. */
class Ros2Std_srvsSrvSetBoolClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Std_srvsSrvSetBool_Request;using Response=Ros2Std_srvsSrvSetBool_Response;
Ros2Std_srvsSrvSetBoolClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Std_srvsSrvSetBoolClient(Ros2Std_srvsSrvSetBoolClient const&)=delete;Ros2Std_srvsSrvSetBoolClient& operator=(Ros2Std_srvsSrvSetBoolClient const&)=delete;Ros2Std_srvsSrvSetBoolClient(Ros2Std_srvsSrvSetBoolClient&&)=delete;
~Ros2Std_srvsSrvSetBoolClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_std_srvs_srv_SetBool_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom std_srvs/srv/SetBool server; advertise/respond stay separate authorities. */
class Ros2Std_srvsSrvSetBoolServer:public ServerOwners {
using Handler=std::function<Ros2Std_srvsSrvSetBool_Response(Ros2Std_srvsSrvSetBool_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Std_srvsSrvSetBool_Request;using Response=Ros2Std_srvsSrvSetBool_Response;
Ros2Std_srvsSrvSetBoolServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_std_srvs_srv_SetBool_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_std_srvs_srv_SetBool_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_std_srvs_srv_SetBool_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_std_srvs_srv_SetBool_respond(&respond_,ticket,&response).error;}
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
/** Complete owned custom turtlebot3_msgs/action/Patrol Action client with four separate roles. */
class Ros2Turtlebot3_msgsActionPatrolClient {roswasmc_context context_;std::array<uint64_t,1024> goals_{};roswasmc_action_bundle bundle_{};
public:
using Goal=Ros2Turtlebot3_msgsActionPatrol_Goal;using Result=Ros2Turtlebot3_msgsActionPatrol_Result;using Feedback=Ros2Turtlebot3_msgsActionPatrol_Feedback;
Ros2Turtlebot3_msgsActionPatrolClient(roswasmc_context const& context,roswasmc_action_aliases const& aliases,size_t slots=8):context_{context.backend} {roswasmc_action_bundle_init(&context_,&bundle_,&aliases,goals_.data(),slots);}
Ros2Turtlebot3_msgsActionPatrolClient(Ros2Turtlebot3_msgsActionPatrolClient const&)=delete;Ros2Turtlebot3_msgsActionPatrolClient& operator=(Ros2Turtlebot3_msgsActionPatrolClient const&)=delete;Ros2Turtlebot3_msgsActionPatrolClient(Ros2Turtlebot3_msgsActionPatrolClient&&)=delete;
~Ros2Turtlebot3_msgsActionPatrolClient(){if(bundle_.active){std::array<uint32_t,5> statuses{};roswasmc_action_bundle_close_checked(&bundle_,0,statuses.data(),statuses.size());}}
uint32_t error() const noexcept{return bundle_.error?bundle_.error:bundle_.active?0:9;}bool is_open() const noexcept{return bundle_.active;}bool owns(uint64_t id)const{return roswasmc_action_bundle_owns(&bundle_,id);}
roswasmc_result send_goal(Goal const& value,uint64_t& id){return roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_send(&bundle_,&value,&id);}
roswasmc_result feedback(uint64_t id,Feedback& value){return roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_feedback(&bundle_,id,&value);}
roswasmc_result result(uint64_t id,uint8_t& kind,Result& value){return roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_result(&bundle_,id,&kind,&value);}
roswasmc_result cancel_own(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_own(&bundle_,id,&accepted);}
roswasmc_result cancel_any(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_any(&bundle_,id,&accepted);}
roswasmc_result cancel_all(int& accepted){return roswasmc_action_bundle_cancel_all(&bundle_,&accepted);}
roswasmc_cleanup_report close_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_close_checked(&bundle_,primary,statuses.data(),statuses.size());}
roswasmc_cleanup_report revoke_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_revoke_checked(&bundle_,primary,statuses.data(),statuses.size());}
};
/** Typed custom turtlebot3_msgs/action/Patrol server with explicit feedback/terminal component codecs. */
class Ros2Turtlebot3_msgsActionPatrolServer:public ServerOwners {
public:
using Goal=Ros2Turtlebot3_msgsActionPatrol_Goal;using Result=Ros2Turtlebot3_msgsActionPatrol_Result;using Feedback=Ros2Turtlebot3_msgsActionPatrol_Feedback;
/** Handler may choose separate feedback/result effects; kind must be2..4 when terminal. */
struct Replies {Feedback feedback{};Result result{};uint8_t kind{2};bool send_feedback{true},send_result{true};};
private:using Handler=std::function<Replies(uint64_t,Goal const&)>;std::shared_ptr<Handler> handler_;
public:
Ros2Turtlebot3_msgsActionPatrolServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,true} {}
void on_goal(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& id,Goal& goal){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_poll(&advertise_,&id,&goal);}
roswasmc_result respond_feedback(uint64_t id,Feedback const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_respond_feedback(&respond_,id,&value);}
roswasmc_result respond_result(uint64_t id,uint8_t kind,Result const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_respond_result(&respond_,id,kind,&value);}
/** One pass, at most one feedback plus one terminal response; never implicit replay. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t id{};Goal goal{};auto r=roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_poll(&advertise_,&id,&goal);if(r.error)return r.error;auto replies=(*handler)(id,goal);if(replies.send_result&&(replies.kind<2||replies.kind>4||!roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Result_valid(&replies.result)))return 22;if(replies.send_feedback&&!roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Feedback_valid(&replies.feedback))return 22;if(replies.send_feedback){r=roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_respond_feedback(&respond_,id,&replies.feedback);if(r.error)return r.error;}return replies.send_result?roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_respond_result(&respond_,id,replies.kind,&replies.result).error:0;}
};
/** Complete custom turtlebot3_msgs/srv/Dqn client; default policy returns13 from construction. */
class Ros2Turtlebot3_msgsSrvDqnClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Turtlebot3_msgsSrvDqn_Request;using Response=Ros2Turtlebot3_msgsSrvDqn_Response;
Ros2Turtlebot3_msgsSrvDqnClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Turtlebot3_msgsSrvDqnClient(Ros2Turtlebot3_msgsSrvDqnClient const&)=delete;Ros2Turtlebot3_msgsSrvDqnClient& operator=(Ros2Turtlebot3_msgsSrvDqnClient const&)=delete;Ros2Turtlebot3_msgsSrvDqnClient(Ros2Turtlebot3_msgsSrvDqnClient&&)=delete;
~Ros2Turtlebot3_msgsSrvDqnClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom turtlebot3_msgs/srv/Dqn server; advertise/respond stay separate authorities. */
class Ros2Turtlebot3_msgsSrvDqnServer:public ServerOwners {
using Handler=std::function<Ros2Turtlebot3_msgsSrvDqn_Response(Ros2Turtlebot3_msgsSrvDqn_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Turtlebot3_msgsSrvDqn_Request;using Response=Ros2Turtlebot3_msgsSrvDqn_Response;
Ros2Turtlebot3_msgsSrvDqnServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_respond(&respond_,ticket,&response).error;}
};
/** Complete custom turtlebot3_msgs/srv/Goal client; default policy returns13 from construction. */
class Ros2Turtlebot3_msgsSrvGoalClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Turtlebot3_msgsSrvGoal_Request;using Response=Ros2Turtlebot3_msgsSrvGoal_Response;
Ros2Turtlebot3_msgsSrvGoalClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Turtlebot3_msgsSrvGoalClient(Ros2Turtlebot3_msgsSrvGoalClient const&)=delete;Ros2Turtlebot3_msgsSrvGoalClient& operator=(Ros2Turtlebot3_msgsSrvGoalClient const&)=delete;Ros2Turtlebot3_msgsSrvGoalClient(Ros2Turtlebot3_msgsSrvGoalClient&&)=delete;
~Ros2Turtlebot3_msgsSrvGoalClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom turtlebot3_msgs/srv/Goal server; advertise/respond stay separate authorities. */
class Ros2Turtlebot3_msgsSrvGoalServer:public ServerOwners {
using Handler=std::function<Ros2Turtlebot3_msgsSrvGoal_Response(Ros2Turtlebot3_msgsSrvGoal_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Turtlebot3_msgsSrvGoal_Request;using Response=Ros2Turtlebot3_msgsSrvGoal_Response;
Ros2Turtlebot3_msgsSrvGoalServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_respond(&respond_,ticket,&response).error;}
};
/** Complete custom turtlebot3_msgs/srv/Sound client; default policy returns13 from construction. */
class Ros2Turtlebot3_msgsSrvSoundClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Turtlebot3_msgsSrvSound_Request;using Response=Ros2Turtlebot3_msgsSrvSound_Response;
Ros2Turtlebot3_msgsSrvSoundClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Turtlebot3_msgsSrvSoundClient(Ros2Turtlebot3_msgsSrvSoundClient const&)=delete;Ros2Turtlebot3_msgsSrvSoundClient& operator=(Ros2Turtlebot3_msgsSrvSoundClient const&)=delete;Ros2Turtlebot3_msgsSrvSoundClient(Ros2Turtlebot3_msgsSrvSoundClient&&)=delete;
~Ros2Turtlebot3_msgsSrvSoundClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom turtlebot3_msgs/srv/Sound server; advertise/respond stay separate authorities. */
class Ros2Turtlebot3_msgsSrvSoundServer:public ServerOwners {
using Handler=std::function<Ros2Turtlebot3_msgsSrvSound_Response(Ros2Turtlebot3_msgsSrvSound_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Turtlebot3_msgsSrvSound_Request;using Response=Ros2Turtlebot3_msgsSrvSound_Response;
Ros2Turtlebot3_msgsSrvSoundServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_respond(&respond_,ticket,&response).error;}
};
}
