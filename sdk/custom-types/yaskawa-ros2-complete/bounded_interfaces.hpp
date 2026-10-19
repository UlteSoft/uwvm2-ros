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
/** Complete custom industrial_msgs/srv/CmdJointTrajectory client; default policy returns13 from construction. */
class Ros2Industrial_msgsSrvCmdJointTrajectoryClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Industrial_msgsSrvCmdJointTrajectory_Request;using Response=Ros2Industrial_msgsSrvCmdJointTrajectory_Response;
Ros2Industrial_msgsSrvCmdJointTrajectoryClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Industrial_msgsSrvCmdJointTrajectoryClient(Ros2Industrial_msgsSrvCmdJointTrajectoryClient const&)=delete;Ros2Industrial_msgsSrvCmdJointTrajectoryClient& operator=(Ros2Industrial_msgsSrvCmdJointTrajectoryClient const&)=delete;Ros2Industrial_msgsSrvCmdJointTrajectoryClient(Ros2Industrial_msgsSrvCmdJointTrajectoryClient&&)=delete;
~Ros2Industrial_msgsSrvCmdJointTrajectoryClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom industrial_msgs/srv/CmdJointTrajectory server; advertise/respond stay separate authorities. */
class Ros2Industrial_msgsSrvCmdJointTrajectoryServer:public ServerOwners {
using Handler=std::function<Ros2Industrial_msgsSrvCmdJointTrajectory_Response(Ros2Industrial_msgsSrvCmdJointTrajectory_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Industrial_msgsSrvCmdJointTrajectory_Request;using Response=Ros2Industrial_msgsSrvCmdJointTrajectory_Response;
Ros2Industrial_msgsSrvCmdJointTrajectoryServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_respond(&respond_,ticket,&response).error;}
};
/** Complete custom industrial_msgs/srv/GetRobotInfo client; default policy returns13 from construction. */
class Ros2Industrial_msgsSrvGetRobotInfoClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Industrial_msgsSrvGetRobotInfo_Request;using Response=Ros2Industrial_msgsSrvGetRobotInfo_Response;
Ros2Industrial_msgsSrvGetRobotInfoClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Industrial_msgsSrvGetRobotInfoClient(Ros2Industrial_msgsSrvGetRobotInfoClient const&)=delete;Ros2Industrial_msgsSrvGetRobotInfoClient& operator=(Ros2Industrial_msgsSrvGetRobotInfoClient const&)=delete;Ros2Industrial_msgsSrvGetRobotInfoClient(Ros2Industrial_msgsSrvGetRobotInfoClient&&)=delete;
~Ros2Industrial_msgsSrvGetRobotInfoClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom industrial_msgs/srv/GetRobotInfo server; advertise/respond stay separate authorities. */
class Ros2Industrial_msgsSrvGetRobotInfoServer:public ServerOwners {
using Handler=std::function<Ros2Industrial_msgsSrvGetRobotInfo_Response(Ros2Industrial_msgsSrvGetRobotInfo_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Industrial_msgsSrvGetRobotInfo_Request;using Response=Ros2Industrial_msgsSrvGetRobotInfo_Response;
Ros2Industrial_msgsSrvGetRobotInfoServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_respond(&respond_,ticket,&response).error;}
};
/** Complete custom industrial_msgs/srv/SetDrivePower client; default policy returns13 from construction. */
class Ros2Industrial_msgsSrvSetDrivePowerClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Industrial_msgsSrvSetDrivePower_Request;using Response=Ros2Industrial_msgsSrvSetDrivePower_Response;
Ros2Industrial_msgsSrvSetDrivePowerClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Industrial_msgsSrvSetDrivePowerClient(Ros2Industrial_msgsSrvSetDrivePowerClient const&)=delete;Ros2Industrial_msgsSrvSetDrivePowerClient& operator=(Ros2Industrial_msgsSrvSetDrivePowerClient const&)=delete;Ros2Industrial_msgsSrvSetDrivePowerClient(Ros2Industrial_msgsSrvSetDrivePowerClient&&)=delete;
~Ros2Industrial_msgsSrvSetDrivePowerClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom industrial_msgs/srv/SetDrivePower server; advertise/respond stay separate authorities. */
class Ros2Industrial_msgsSrvSetDrivePowerServer:public ServerOwners {
using Handler=std::function<Ros2Industrial_msgsSrvSetDrivePower_Response(Ros2Industrial_msgsSrvSetDrivePower_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Industrial_msgsSrvSetDrivePower_Request;using Response=Ros2Industrial_msgsSrvSetDrivePower_Response;
Ros2Industrial_msgsSrvSetDrivePowerServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_respond(&respond_,ticket,&response).error;}
};
/** Complete custom industrial_msgs/srv/SetRemoteLoggerLevel client; default policy returns13 from construction. */
class Ros2Industrial_msgsSrvSetRemoteLoggerLevelClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Request;using Response=Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Response;
Ros2Industrial_msgsSrvSetRemoteLoggerLevelClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Industrial_msgsSrvSetRemoteLoggerLevelClient(Ros2Industrial_msgsSrvSetRemoteLoggerLevelClient const&)=delete;Ros2Industrial_msgsSrvSetRemoteLoggerLevelClient& operator=(Ros2Industrial_msgsSrvSetRemoteLoggerLevelClient const&)=delete;Ros2Industrial_msgsSrvSetRemoteLoggerLevelClient(Ros2Industrial_msgsSrvSetRemoteLoggerLevelClient&&)=delete;
~Ros2Industrial_msgsSrvSetRemoteLoggerLevelClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom industrial_msgs/srv/SetRemoteLoggerLevel server; advertise/respond stay separate authorities. */
class Ros2Industrial_msgsSrvSetRemoteLoggerLevelServer:public ServerOwners {
using Handler=std::function<Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Response(Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Request;using Response=Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Response;
Ros2Industrial_msgsSrvSetRemoteLoggerLevelServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_respond(&respond_,ticket,&response).error;}
};
/** Complete custom industrial_msgs/srv/StartMotion client; default policy returns13 from construction. */
class Ros2Industrial_msgsSrvStartMotionClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Industrial_msgsSrvStartMotion_Request;using Response=Ros2Industrial_msgsSrvStartMotion_Response;
Ros2Industrial_msgsSrvStartMotionClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Industrial_msgsSrvStartMotionClient(Ros2Industrial_msgsSrvStartMotionClient const&)=delete;Ros2Industrial_msgsSrvStartMotionClient& operator=(Ros2Industrial_msgsSrvStartMotionClient const&)=delete;Ros2Industrial_msgsSrvStartMotionClient(Ros2Industrial_msgsSrvStartMotionClient&&)=delete;
~Ros2Industrial_msgsSrvStartMotionClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_industrial_msgs_srv_StartMotion_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom industrial_msgs/srv/StartMotion server; advertise/respond stay separate authorities. */
class Ros2Industrial_msgsSrvStartMotionServer:public ServerOwners {
using Handler=std::function<Ros2Industrial_msgsSrvStartMotion_Response(Ros2Industrial_msgsSrvStartMotion_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Industrial_msgsSrvStartMotion_Request;using Response=Ros2Industrial_msgsSrvStartMotion_Response;
Ros2Industrial_msgsSrvStartMotionServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_industrial_msgs_srv_StartMotion_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_industrial_msgs_srv_StartMotion_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_industrial_msgs_srv_StartMotion_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_industrial_msgs_srv_StartMotion_respond(&respond_,ticket,&response).error;}
};
/** Complete custom industrial_msgs/srv/StopMotion client; default policy returns13 from construction. */
class Ros2Industrial_msgsSrvStopMotionClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Industrial_msgsSrvStopMotion_Request;using Response=Ros2Industrial_msgsSrvStopMotion_Response;
Ros2Industrial_msgsSrvStopMotionClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Industrial_msgsSrvStopMotionClient(Ros2Industrial_msgsSrvStopMotionClient const&)=delete;Ros2Industrial_msgsSrvStopMotionClient& operator=(Ros2Industrial_msgsSrvStopMotionClient const&)=delete;Ros2Industrial_msgsSrvStopMotionClient(Ros2Industrial_msgsSrvStopMotionClient&&)=delete;
~Ros2Industrial_msgsSrvStopMotionClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_industrial_msgs_srv_StopMotion_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom industrial_msgs/srv/StopMotion server; advertise/respond stay separate authorities. */
class Ros2Industrial_msgsSrvStopMotionServer:public ServerOwners {
using Handler=std::function<Ros2Industrial_msgsSrvStopMotion_Response(Ros2Industrial_msgsSrvStopMotion_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Industrial_msgsSrvStopMotion_Request;using Response=Ros2Industrial_msgsSrvStopMotion_Response;
Ros2Industrial_msgsSrvStopMotionServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_industrial_msgs_srv_StopMotion_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_industrial_msgs_srv_StopMotion_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_industrial_msgs_srv_StopMotion_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_industrial_msgs_srv_StopMotion_respond(&respond_,ticket,&response).error;}
};
/** Complete custom motoros2_interfaces/srv/GetActiveAlarmInfo client; default policy returns13 from construction. */
class Ros2Motoros2_interfacesSrvGetActiveAlarmInfoClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Request;using Response=Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Response;
Ros2Motoros2_interfacesSrvGetActiveAlarmInfoClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Motoros2_interfacesSrvGetActiveAlarmInfoClient(Ros2Motoros2_interfacesSrvGetActiveAlarmInfoClient const&)=delete;Ros2Motoros2_interfacesSrvGetActiveAlarmInfoClient& operator=(Ros2Motoros2_interfacesSrvGetActiveAlarmInfoClient const&)=delete;Ros2Motoros2_interfacesSrvGetActiveAlarmInfoClient(Ros2Motoros2_interfacesSrvGetActiveAlarmInfoClient&&)=delete;
~Ros2Motoros2_interfacesSrvGetActiveAlarmInfoClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom motoros2_interfaces/srv/GetActiveAlarmInfo server; advertise/respond stay separate authorities. */
class Ros2Motoros2_interfacesSrvGetActiveAlarmInfoServer:public ServerOwners {
using Handler=std::function<Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Response(Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Request;using Response=Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Response;
Ros2Motoros2_interfacesSrvGetActiveAlarmInfoServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_respond(&respond_,ticket,&response).error;}
};
/** Complete custom motoros2_interfaces/srv/ListInformJobs client; default policy returns13 from construction. */
class Ros2Motoros2_interfacesSrvListInformJobsClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Motoros2_interfacesSrvListInformJobs_Request;using Response=Ros2Motoros2_interfacesSrvListInformJobs_Response;
Ros2Motoros2_interfacesSrvListInformJobsClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Motoros2_interfacesSrvListInformJobsClient(Ros2Motoros2_interfacesSrvListInformJobsClient const&)=delete;Ros2Motoros2_interfacesSrvListInformJobsClient& operator=(Ros2Motoros2_interfacesSrvListInformJobsClient const&)=delete;Ros2Motoros2_interfacesSrvListInformJobsClient(Ros2Motoros2_interfacesSrvListInformJobsClient&&)=delete;
~Ros2Motoros2_interfacesSrvListInformJobsClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom motoros2_interfaces/srv/ListInformJobs server; advertise/respond stay separate authorities. */
class Ros2Motoros2_interfacesSrvListInformJobsServer:public ServerOwners {
using Handler=std::function<Ros2Motoros2_interfacesSrvListInformJobs_Response(Ros2Motoros2_interfacesSrvListInformJobs_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Motoros2_interfacesSrvListInformJobs_Request;using Response=Ros2Motoros2_interfacesSrvListInformJobs_Response;
Ros2Motoros2_interfacesSrvListInformJobsServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_respond(&respond_,ticket,&response).error;}
};
/** Complete custom motoros2_interfaces/srv/QueueTrajPoint client; default policy returns13 from construction. */
class Ros2Motoros2_interfacesSrvQueueTrajPointClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Motoros2_interfacesSrvQueueTrajPoint_Request;using Response=Ros2Motoros2_interfacesSrvQueueTrajPoint_Response;
Ros2Motoros2_interfacesSrvQueueTrajPointClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Motoros2_interfacesSrvQueueTrajPointClient(Ros2Motoros2_interfacesSrvQueueTrajPointClient const&)=delete;Ros2Motoros2_interfacesSrvQueueTrajPointClient& operator=(Ros2Motoros2_interfacesSrvQueueTrajPointClient const&)=delete;Ros2Motoros2_interfacesSrvQueueTrajPointClient(Ros2Motoros2_interfacesSrvQueueTrajPointClient&&)=delete;
~Ros2Motoros2_interfacesSrvQueueTrajPointClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom motoros2_interfaces/srv/QueueTrajPoint server; advertise/respond stay separate authorities. */
class Ros2Motoros2_interfacesSrvQueueTrajPointServer:public ServerOwners {
using Handler=std::function<Ros2Motoros2_interfacesSrvQueueTrajPoint_Response(Ros2Motoros2_interfacesSrvQueueTrajPoint_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Motoros2_interfacesSrvQueueTrajPoint_Request;using Response=Ros2Motoros2_interfacesSrvQueueTrajPoint_Response;
Ros2Motoros2_interfacesSrvQueueTrajPointServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_respond(&respond_,ticket,&response).error;}
};
/** Complete custom motoros2_interfaces/srv/ReadGroupIO client; default policy returns13 from construction. */
class Ros2Motoros2_interfacesSrvReadGroupIOClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Motoros2_interfacesSrvReadGroupIO_Request;using Response=Ros2Motoros2_interfacesSrvReadGroupIO_Response;
Ros2Motoros2_interfacesSrvReadGroupIOClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Motoros2_interfacesSrvReadGroupIOClient(Ros2Motoros2_interfacesSrvReadGroupIOClient const&)=delete;Ros2Motoros2_interfacesSrvReadGroupIOClient& operator=(Ros2Motoros2_interfacesSrvReadGroupIOClient const&)=delete;Ros2Motoros2_interfacesSrvReadGroupIOClient(Ros2Motoros2_interfacesSrvReadGroupIOClient&&)=delete;
~Ros2Motoros2_interfacesSrvReadGroupIOClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom motoros2_interfaces/srv/ReadGroupIO server; advertise/respond stay separate authorities. */
class Ros2Motoros2_interfacesSrvReadGroupIOServer:public ServerOwners {
using Handler=std::function<Ros2Motoros2_interfacesSrvReadGroupIO_Response(Ros2Motoros2_interfacesSrvReadGroupIO_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Motoros2_interfacesSrvReadGroupIO_Request;using Response=Ros2Motoros2_interfacesSrvReadGroupIO_Response;
Ros2Motoros2_interfacesSrvReadGroupIOServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_respond(&respond_,ticket,&response).error;}
};
/** Complete custom motoros2_interfaces/srv/ReadMRegister client; default policy returns13 from construction. */
class Ros2Motoros2_interfacesSrvReadMRegisterClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Motoros2_interfacesSrvReadMRegister_Request;using Response=Ros2Motoros2_interfacesSrvReadMRegister_Response;
Ros2Motoros2_interfacesSrvReadMRegisterClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Motoros2_interfacesSrvReadMRegisterClient(Ros2Motoros2_interfacesSrvReadMRegisterClient const&)=delete;Ros2Motoros2_interfacesSrvReadMRegisterClient& operator=(Ros2Motoros2_interfacesSrvReadMRegisterClient const&)=delete;Ros2Motoros2_interfacesSrvReadMRegisterClient(Ros2Motoros2_interfacesSrvReadMRegisterClient&&)=delete;
~Ros2Motoros2_interfacesSrvReadMRegisterClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom motoros2_interfaces/srv/ReadMRegister server; advertise/respond stay separate authorities. */
class Ros2Motoros2_interfacesSrvReadMRegisterServer:public ServerOwners {
using Handler=std::function<Ros2Motoros2_interfacesSrvReadMRegister_Response(Ros2Motoros2_interfacesSrvReadMRegister_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Motoros2_interfacesSrvReadMRegister_Request;using Response=Ros2Motoros2_interfacesSrvReadMRegister_Response;
Ros2Motoros2_interfacesSrvReadMRegisterServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_respond(&respond_,ticket,&response).error;}
};
/** Complete custom motoros2_interfaces/srv/ReadSingleIO client; default policy returns13 from construction. */
class Ros2Motoros2_interfacesSrvReadSingleIOClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Motoros2_interfacesSrvReadSingleIO_Request;using Response=Ros2Motoros2_interfacesSrvReadSingleIO_Response;
Ros2Motoros2_interfacesSrvReadSingleIOClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Motoros2_interfacesSrvReadSingleIOClient(Ros2Motoros2_interfacesSrvReadSingleIOClient const&)=delete;Ros2Motoros2_interfacesSrvReadSingleIOClient& operator=(Ros2Motoros2_interfacesSrvReadSingleIOClient const&)=delete;Ros2Motoros2_interfacesSrvReadSingleIOClient(Ros2Motoros2_interfacesSrvReadSingleIOClient&&)=delete;
~Ros2Motoros2_interfacesSrvReadSingleIOClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom motoros2_interfaces/srv/ReadSingleIO server; advertise/respond stay separate authorities. */
class Ros2Motoros2_interfacesSrvReadSingleIOServer:public ServerOwners {
using Handler=std::function<Ros2Motoros2_interfacesSrvReadSingleIO_Response(Ros2Motoros2_interfacesSrvReadSingleIO_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Motoros2_interfacesSrvReadSingleIO_Request;using Response=Ros2Motoros2_interfacesSrvReadSingleIO_Response;
Ros2Motoros2_interfacesSrvReadSingleIOServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_respond(&respond_,ticket,&response).error;}
};
/** Complete custom motoros2_interfaces/srv/ResetError client; default policy returns13 from construction. */
class Ros2Motoros2_interfacesSrvResetErrorClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Motoros2_interfacesSrvResetError_Request;using Response=Ros2Motoros2_interfacesSrvResetError_Response;
Ros2Motoros2_interfacesSrvResetErrorClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Motoros2_interfacesSrvResetErrorClient(Ros2Motoros2_interfacesSrvResetErrorClient const&)=delete;Ros2Motoros2_interfacesSrvResetErrorClient& operator=(Ros2Motoros2_interfacesSrvResetErrorClient const&)=delete;Ros2Motoros2_interfacesSrvResetErrorClient(Ros2Motoros2_interfacesSrvResetErrorClient&&)=delete;
~Ros2Motoros2_interfacesSrvResetErrorClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom motoros2_interfaces/srv/ResetError server; advertise/respond stay separate authorities. */
class Ros2Motoros2_interfacesSrvResetErrorServer:public ServerOwners {
using Handler=std::function<Ros2Motoros2_interfacesSrvResetError_Response(Ros2Motoros2_interfacesSrvResetError_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Motoros2_interfacesSrvResetError_Request;using Response=Ros2Motoros2_interfacesSrvResetError_Response;
Ros2Motoros2_interfacesSrvResetErrorServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_respond(&respond_,ticket,&response).error;}
};
/** Complete custom motoros2_interfaces/srv/SelectMotionTool client; default policy returns13 from construction. */
class Ros2Motoros2_interfacesSrvSelectMotionToolClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Motoros2_interfacesSrvSelectMotionTool_Request;using Response=Ros2Motoros2_interfacesSrvSelectMotionTool_Response;
Ros2Motoros2_interfacesSrvSelectMotionToolClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Motoros2_interfacesSrvSelectMotionToolClient(Ros2Motoros2_interfacesSrvSelectMotionToolClient const&)=delete;Ros2Motoros2_interfacesSrvSelectMotionToolClient& operator=(Ros2Motoros2_interfacesSrvSelectMotionToolClient const&)=delete;Ros2Motoros2_interfacesSrvSelectMotionToolClient(Ros2Motoros2_interfacesSrvSelectMotionToolClient&&)=delete;
~Ros2Motoros2_interfacesSrvSelectMotionToolClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom motoros2_interfaces/srv/SelectMotionTool server; advertise/respond stay separate authorities. */
class Ros2Motoros2_interfacesSrvSelectMotionToolServer:public ServerOwners {
using Handler=std::function<Ros2Motoros2_interfacesSrvSelectMotionTool_Response(Ros2Motoros2_interfacesSrvSelectMotionTool_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Motoros2_interfacesSrvSelectMotionTool_Request;using Response=Ros2Motoros2_interfacesSrvSelectMotionTool_Response;
Ros2Motoros2_interfacesSrvSelectMotionToolServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_respond(&respond_,ticket,&response).error;}
};
/** Complete custom motoros2_interfaces/srv/StartPointQueueMode client; default policy returns13 from construction. */
class Ros2Motoros2_interfacesSrvStartPointQueueModeClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Motoros2_interfacesSrvStartPointQueueMode_Request;using Response=Ros2Motoros2_interfacesSrvStartPointQueueMode_Response;
Ros2Motoros2_interfacesSrvStartPointQueueModeClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Motoros2_interfacesSrvStartPointQueueModeClient(Ros2Motoros2_interfacesSrvStartPointQueueModeClient const&)=delete;Ros2Motoros2_interfacesSrvStartPointQueueModeClient& operator=(Ros2Motoros2_interfacesSrvStartPointQueueModeClient const&)=delete;Ros2Motoros2_interfacesSrvStartPointQueueModeClient(Ros2Motoros2_interfacesSrvStartPointQueueModeClient&&)=delete;
~Ros2Motoros2_interfacesSrvStartPointQueueModeClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom motoros2_interfaces/srv/StartPointQueueMode server; advertise/respond stay separate authorities. */
class Ros2Motoros2_interfacesSrvStartPointQueueModeServer:public ServerOwners {
using Handler=std::function<Ros2Motoros2_interfacesSrvStartPointQueueMode_Response(Ros2Motoros2_interfacesSrvStartPointQueueMode_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Motoros2_interfacesSrvStartPointQueueMode_Request;using Response=Ros2Motoros2_interfacesSrvStartPointQueueMode_Response;
Ros2Motoros2_interfacesSrvStartPointQueueModeServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_respond(&respond_,ticket,&response).error;}
};
/** Complete custom motoros2_interfaces/srv/StartTrajMode client; default policy returns13 from construction. */
class Ros2Motoros2_interfacesSrvStartTrajModeClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Motoros2_interfacesSrvStartTrajMode_Request;using Response=Ros2Motoros2_interfacesSrvStartTrajMode_Response;
Ros2Motoros2_interfacesSrvStartTrajModeClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Motoros2_interfacesSrvStartTrajModeClient(Ros2Motoros2_interfacesSrvStartTrajModeClient const&)=delete;Ros2Motoros2_interfacesSrvStartTrajModeClient& operator=(Ros2Motoros2_interfacesSrvStartTrajModeClient const&)=delete;Ros2Motoros2_interfacesSrvStartTrajModeClient(Ros2Motoros2_interfacesSrvStartTrajModeClient&&)=delete;
~Ros2Motoros2_interfacesSrvStartTrajModeClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom motoros2_interfaces/srv/StartTrajMode server; advertise/respond stay separate authorities. */
class Ros2Motoros2_interfacesSrvStartTrajModeServer:public ServerOwners {
using Handler=std::function<Ros2Motoros2_interfacesSrvStartTrajMode_Response(Ros2Motoros2_interfacesSrvStartTrajMode_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Motoros2_interfacesSrvStartTrajMode_Request;using Response=Ros2Motoros2_interfacesSrvStartTrajMode_Response;
Ros2Motoros2_interfacesSrvStartTrajModeServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_respond(&respond_,ticket,&response).error;}
};
/** Complete custom motoros2_interfaces/srv/WriteGroupIO client; default policy returns13 from construction. */
class Ros2Motoros2_interfacesSrvWriteGroupIOClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Motoros2_interfacesSrvWriteGroupIO_Request;using Response=Ros2Motoros2_interfacesSrvWriteGroupIO_Response;
Ros2Motoros2_interfacesSrvWriteGroupIOClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Motoros2_interfacesSrvWriteGroupIOClient(Ros2Motoros2_interfacesSrvWriteGroupIOClient const&)=delete;Ros2Motoros2_interfacesSrvWriteGroupIOClient& operator=(Ros2Motoros2_interfacesSrvWriteGroupIOClient const&)=delete;Ros2Motoros2_interfacesSrvWriteGroupIOClient(Ros2Motoros2_interfacesSrvWriteGroupIOClient&&)=delete;
~Ros2Motoros2_interfacesSrvWriteGroupIOClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom motoros2_interfaces/srv/WriteGroupIO server; advertise/respond stay separate authorities. */
class Ros2Motoros2_interfacesSrvWriteGroupIOServer:public ServerOwners {
using Handler=std::function<Ros2Motoros2_interfacesSrvWriteGroupIO_Response(Ros2Motoros2_interfacesSrvWriteGroupIO_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Motoros2_interfacesSrvWriteGroupIO_Request;using Response=Ros2Motoros2_interfacesSrvWriteGroupIO_Response;
Ros2Motoros2_interfacesSrvWriteGroupIOServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_respond(&respond_,ticket,&response).error;}
};
/** Complete custom motoros2_interfaces/srv/WriteMRegister client; default policy returns13 from construction. */
class Ros2Motoros2_interfacesSrvWriteMRegisterClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Motoros2_interfacesSrvWriteMRegister_Request;using Response=Ros2Motoros2_interfacesSrvWriteMRegister_Response;
Ros2Motoros2_interfacesSrvWriteMRegisterClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Motoros2_interfacesSrvWriteMRegisterClient(Ros2Motoros2_interfacesSrvWriteMRegisterClient const&)=delete;Ros2Motoros2_interfacesSrvWriteMRegisterClient& operator=(Ros2Motoros2_interfacesSrvWriteMRegisterClient const&)=delete;Ros2Motoros2_interfacesSrvWriteMRegisterClient(Ros2Motoros2_interfacesSrvWriteMRegisterClient&&)=delete;
~Ros2Motoros2_interfacesSrvWriteMRegisterClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom motoros2_interfaces/srv/WriteMRegister server; advertise/respond stay separate authorities. */
class Ros2Motoros2_interfacesSrvWriteMRegisterServer:public ServerOwners {
using Handler=std::function<Ros2Motoros2_interfacesSrvWriteMRegister_Response(Ros2Motoros2_interfacesSrvWriteMRegister_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Motoros2_interfacesSrvWriteMRegister_Request;using Response=Ros2Motoros2_interfacesSrvWriteMRegister_Response;
Ros2Motoros2_interfacesSrvWriteMRegisterServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_respond(&respond_,ticket,&response).error;}
};
/** Complete custom motoros2_interfaces/srv/WriteSingleIO client; default policy returns13 from construction. */
class Ros2Motoros2_interfacesSrvWriteSingleIOClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Motoros2_interfacesSrvWriteSingleIO_Request;using Response=Ros2Motoros2_interfacesSrvWriteSingleIO_Response;
Ros2Motoros2_interfacesSrvWriteSingleIOClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Motoros2_interfacesSrvWriteSingleIOClient(Ros2Motoros2_interfacesSrvWriteSingleIOClient const&)=delete;Ros2Motoros2_interfacesSrvWriteSingleIOClient& operator=(Ros2Motoros2_interfacesSrvWriteSingleIOClient const&)=delete;Ros2Motoros2_interfacesSrvWriteSingleIOClient(Ros2Motoros2_interfacesSrvWriteSingleIOClient&&)=delete;
~Ros2Motoros2_interfacesSrvWriteSingleIOClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom motoros2_interfaces/srv/WriteSingleIO server; advertise/respond stay separate authorities. */
class Ros2Motoros2_interfacesSrvWriteSingleIOServer:public ServerOwners {
using Handler=std::function<Ros2Motoros2_interfacesSrvWriteSingleIO_Response(Ros2Motoros2_interfacesSrvWriteSingleIO_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Motoros2_interfacesSrvWriteSingleIO_Request;using Response=Ros2Motoros2_interfacesSrvWriteSingleIO_Response;
Ros2Motoros2_interfacesSrvWriteSingleIOServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_respond(&respond_,ticket,&response).error;}
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
