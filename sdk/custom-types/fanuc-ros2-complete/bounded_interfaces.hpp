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
/** Complete custom fanuc_msgs/srv/CfgForceSensor client; default policy returns13 from construction. */
class Ros2Fanuc_msgsSrvCfgForceSensorClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Fanuc_msgsSrvCfgForceSensor_Request;using Response=Ros2Fanuc_msgsSrvCfgForceSensor_Response;
Ros2Fanuc_msgsSrvCfgForceSensorClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Fanuc_msgsSrvCfgForceSensorClient(Ros2Fanuc_msgsSrvCfgForceSensorClient const&)=delete;Ros2Fanuc_msgsSrvCfgForceSensorClient& operator=(Ros2Fanuc_msgsSrvCfgForceSensorClient const&)=delete;Ros2Fanuc_msgsSrvCfgForceSensorClient(Ros2Fanuc_msgsSrvCfgForceSensorClient&&)=delete;
~Ros2Fanuc_msgsSrvCfgForceSensorClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom fanuc_msgs/srv/CfgForceSensor server; advertise/respond stay separate authorities. */
class Ros2Fanuc_msgsSrvCfgForceSensorServer:public ServerOwners {
using Handler=std::function<Ros2Fanuc_msgsSrvCfgForceSensor_Response(Ros2Fanuc_msgsSrvCfgForceSensor_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Fanuc_msgsSrvCfgForceSensor_Request;using Response=Ros2Fanuc_msgsSrvCfgForceSensor_Response;
Ros2Fanuc_msgsSrvCfgForceSensorServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_respond(&respond_,ticket,&response).error;}
};
/** Complete custom fanuc_msgs/srv/GetAnalogIO client; default policy returns13 from construction. */
class Ros2Fanuc_msgsSrvGetAnalogIOClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Fanuc_msgsSrvGetAnalogIO_Request;using Response=Ros2Fanuc_msgsSrvGetAnalogIO_Response;
Ros2Fanuc_msgsSrvGetAnalogIOClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Fanuc_msgsSrvGetAnalogIOClient(Ros2Fanuc_msgsSrvGetAnalogIOClient const&)=delete;Ros2Fanuc_msgsSrvGetAnalogIOClient& operator=(Ros2Fanuc_msgsSrvGetAnalogIOClient const&)=delete;Ros2Fanuc_msgsSrvGetAnalogIOClient(Ros2Fanuc_msgsSrvGetAnalogIOClient&&)=delete;
~Ros2Fanuc_msgsSrvGetAnalogIOClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom fanuc_msgs/srv/GetAnalogIO server; advertise/respond stay separate authorities. */
class Ros2Fanuc_msgsSrvGetAnalogIOServer:public ServerOwners {
using Handler=std::function<Ros2Fanuc_msgsSrvGetAnalogIO_Response(Ros2Fanuc_msgsSrvGetAnalogIO_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Fanuc_msgsSrvGetAnalogIO_Request;using Response=Ros2Fanuc_msgsSrvGetAnalogIO_Response;
Ros2Fanuc_msgsSrvGetAnalogIOServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_respond(&respond_,ticket,&response).error;}
};
/** Complete custom fanuc_msgs/srv/GetBoolIO client; default policy returns13 from construction. */
class Ros2Fanuc_msgsSrvGetBoolIOClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Fanuc_msgsSrvGetBoolIO_Request;using Response=Ros2Fanuc_msgsSrvGetBoolIO_Response;
Ros2Fanuc_msgsSrvGetBoolIOClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Fanuc_msgsSrvGetBoolIOClient(Ros2Fanuc_msgsSrvGetBoolIOClient const&)=delete;Ros2Fanuc_msgsSrvGetBoolIOClient& operator=(Ros2Fanuc_msgsSrvGetBoolIOClient const&)=delete;Ros2Fanuc_msgsSrvGetBoolIOClient(Ros2Fanuc_msgsSrvGetBoolIOClient&&)=delete;
~Ros2Fanuc_msgsSrvGetBoolIOClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom fanuc_msgs/srv/GetBoolIO server; advertise/respond stay separate authorities. */
class Ros2Fanuc_msgsSrvGetBoolIOServer:public ServerOwners {
using Handler=std::function<Ros2Fanuc_msgsSrvGetBoolIO_Response(Ros2Fanuc_msgsSrvGetBoolIO_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Fanuc_msgsSrvGetBoolIO_Request;using Response=Ros2Fanuc_msgsSrvGetBoolIO_Response;
Ros2Fanuc_msgsSrvGetBoolIOServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_respond(&respond_,ticket,&response).error;}
};
/** Complete custom fanuc_msgs/srv/GetCartesianPosition client; default policy returns13 from construction. */
class Ros2Fanuc_msgsSrvGetCartesianPositionClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Fanuc_msgsSrvGetCartesianPosition_Request;using Response=Ros2Fanuc_msgsSrvGetCartesianPosition_Response;
Ros2Fanuc_msgsSrvGetCartesianPositionClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Fanuc_msgsSrvGetCartesianPositionClient(Ros2Fanuc_msgsSrvGetCartesianPositionClient const&)=delete;Ros2Fanuc_msgsSrvGetCartesianPositionClient& operator=(Ros2Fanuc_msgsSrvGetCartesianPositionClient const&)=delete;Ros2Fanuc_msgsSrvGetCartesianPositionClient(Ros2Fanuc_msgsSrvGetCartesianPositionClient&&)=delete;
~Ros2Fanuc_msgsSrvGetCartesianPositionClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom fanuc_msgs/srv/GetCartesianPosition server; advertise/respond stay separate authorities. */
class Ros2Fanuc_msgsSrvGetCartesianPositionServer:public ServerOwners {
using Handler=std::function<Ros2Fanuc_msgsSrvGetCartesianPosition_Response(Ros2Fanuc_msgsSrvGetCartesianPosition_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Fanuc_msgsSrvGetCartesianPosition_Request;using Response=Ros2Fanuc_msgsSrvGetCartesianPosition_Response;
Ros2Fanuc_msgsSrvGetCartesianPositionServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_respond(&respond_,ticket,&response).error;}
};
/** Complete custom fanuc_msgs/srv/GetGroupIO client; default policy returns13 from construction. */
class Ros2Fanuc_msgsSrvGetGroupIOClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Fanuc_msgsSrvGetGroupIO_Request;using Response=Ros2Fanuc_msgsSrvGetGroupIO_Response;
Ros2Fanuc_msgsSrvGetGroupIOClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Fanuc_msgsSrvGetGroupIOClient(Ros2Fanuc_msgsSrvGetGroupIOClient const&)=delete;Ros2Fanuc_msgsSrvGetGroupIOClient& operator=(Ros2Fanuc_msgsSrvGetGroupIOClient const&)=delete;Ros2Fanuc_msgsSrvGetGroupIOClient(Ros2Fanuc_msgsSrvGetGroupIOClient&&)=delete;
~Ros2Fanuc_msgsSrvGetGroupIOClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom fanuc_msgs/srv/GetGroupIO server; advertise/respond stay separate authorities. */
class Ros2Fanuc_msgsSrvGetGroupIOServer:public ServerOwners {
using Handler=std::function<Ros2Fanuc_msgsSrvGetGroupIO_Response(Ros2Fanuc_msgsSrvGetGroupIO_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Fanuc_msgsSrvGetGroupIO_Request;using Response=Ros2Fanuc_msgsSrvGetGroupIO_Response;
Ros2Fanuc_msgsSrvGetGroupIOServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_respond(&respond_,ticket,&response).error;}
};
/** Complete custom fanuc_msgs/srv/GetNumReg client; default policy returns13 from construction. */
class Ros2Fanuc_msgsSrvGetNumRegClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Fanuc_msgsSrvGetNumReg_Request;using Response=Ros2Fanuc_msgsSrvGetNumReg_Response;
Ros2Fanuc_msgsSrvGetNumRegClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Fanuc_msgsSrvGetNumRegClient(Ros2Fanuc_msgsSrvGetNumRegClient const&)=delete;Ros2Fanuc_msgsSrvGetNumRegClient& operator=(Ros2Fanuc_msgsSrvGetNumRegClient const&)=delete;Ros2Fanuc_msgsSrvGetNumRegClient(Ros2Fanuc_msgsSrvGetNumRegClient&&)=delete;
~Ros2Fanuc_msgsSrvGetNumRegClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom fanuc_msgs/srv/GetNumReg server; advertise/respond stay separate authorities. */
class Ros2Fanuc_msgsSrvGetNumRegServer:public ServerOwners {
using Handler=std::function<Ros2Fanuc_msgsSrvGetNumReg_Response(Ros2Fanuc_msgsSrvGetNumReg_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Fanuc_msgsSrvGetNumReg_Request;using Response=Ros2Fanuc_msgsSrvGetNumReg_Response;
Ros2Fanuc_msgsSrvGetNumRegServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_respond(&respond_,ticket,&response).error;}
};
/** Complete custom fanuc_msgs/srv/GetPayloadComp client; default policy returns13 from construction. */
class Ros2Fanuc_msgsSrvGetPayloadCompClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Fanuc_msgsSrvGetPayloadComp_Request;using Response=Ros2Fanuc_msgsSrvGetPayloadComp_Response;
Ros2Fanuc_msgsSrvGetPayloadCompClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Fanuc_msgsSrvGetPayloadCompClient(Ros2Fanuc_msgsSrvGetPayloadCompClient const&)=delete;Ros2Fanuc_msgsSrvGetPayloadCompClient& operator=(Ros2Fanuc_msgsSrvGetPayloadCompClient const&)=delete;Ros2Fanuc_msgsSrvGetPayloadCompClient(Ros2Fanuc_msgsSrvGetPayloadCompClient&&)=delete;
~Ros2Fanuc_msgsSrvGetPayloadCompClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom fanuc_msgs/srv/GetPayloadComp server; advertise/respond stay separate authorities. */
class Ros2Fanuc_msgsSrvGetPayloadCompServer:public ServerOwners {
using Handler=std::function<Ros2Fanuc_msgsSrvGetPayloadComp_Response(Ros2Fanuc_msgsSrvGetPayloadComp_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Fanuc_msgsSrvGetPayloadComp_Request;using Response=Ros2Fanuc_msgsSrvGetPayloadComp_Response;
Ros2Fanuc_msgsSrvGetPayloadCompServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_respond(&respond_,ticket,&response).error;}
};
/** Complete custom fanuc_msgs/srv/GetPayloadID client; default policy returns13 from construction. */
class Ros2Fanuc_msgsSrvGetPayloadIDClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Fanuc_msgsSrvGetPayloadID_Request;using Response=Ros2Fanuc_msgsSrvGetPayloadID_Response;
Ros2Fanuc_msgsSrvGetPayloadIDClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Fanuc_msgsSrvGetPayloadIDClient(Ros2Fanuc_msgsSrvGetPayloadIDClient const&)=delete;Ros2Fanuc_msgsSrvGetPayloadIDClient& operator=(Ros2Fanuc_msgsSrvGetPayloadIDClient const&)=delete;Ros2Fanuc_msgsSrvGetPayloadIDClient(Ros2Fanuc_msgsSrvGetPayloadIDClient&&)=delete;
~Ros2Fanuc_msgsSrvGetPayloadIDClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom fanuc_msgs/srv/GetPayloadID server; advertise/respond stay separate authorities. */
class Ros2Fanuc_msgsSrvGetPayloadIDServer:public ServerOwners {
using Handler=std::function<Ros2Fanuc_msgsSrvGetPayloadID_Response(Ros2Fanuc_msgsSrvGetPayloadID_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Fanuc_msgsSrvGetPayloadID_Request;using Response=Ros2Fanuc_msgsSrvGetPayloadID_Response;
Ros2Fanuc_msgsSrvGetPayloadIDServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_respond(&respond_,ticket,&response).error;}
};
/** Complete custom fanuc_msgs/srv/GetPayloadValue client; default policy returns13 from construction. */
class Ros2Fanuc_msgsSrvGetPayloadValueClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Fanuc_msgsSrvGetPayloadValue_Request;using Response=Ros2Fanuc_msgsSrvGetPayloadValue_Response;
Ros2Fanuc_msgsSrvGetPayloadValueClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Fanuc_msgsSrvGetPayloadValueClient(Ros2Fanuc_msgsSrvGetPayloadValueClient const&)=delete;Ros2Fanuc_msgsSrvGetPayloadValueClient& operator=(Ros2Fanuc_msgsSrvGetPayloadValueClient const&)=delete;Ros2Fanuc_msgsSrvGetPayloadValueClient(Ros2Fanuc_msgsSrvGetPayloadValueClient&&)=delete;
~Ros2Fanuc_msgsSrvGetPayloadValueClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom fanuc_msgs/srv/GetPayloadValue server; advertise/respond stay separate authorities. */
class Ros2Fanuc_msgsSrvGetPayloadValueServer:public ServerOwners {
using Handler=std::function<Ros2Fanuc_msgsSrvGetPayloadValue_Response(Ros2Fanuc_msgsSrvGetPayloadValue_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Fanuc_msgsSrvGetPayloadValue_Request;using Response=Ros2Fanuc_msgsSrvGetPayloadValue_Response;
Ros2Fanuc_msgsSrvGetPayloadValueServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_respond(&respond_,ticket,&response).error;}
};
/** Complete custom fanuc_msgs/srv/GetPosReg client; default policy returns13 from construction. */
class Ros2Fanuc_msgsSrvGetPosRegClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Fanuc_msgsSrvGetPosReg_Request;using Response=Ros2Fanuc_msgsSrvGetPosReg_Response;
Ros2Fanuc_msgsSrvGetPosRegClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Fanuc_msgsSrvGetPosRegClient(Ros2Fanuc_msgsSrvGetPosRegClient const&)=delete;Ros2Fanuc_msgsSrvGetPosRegClient& operator=(Ros2Fanuc_msgsSrvGetPosRegClient const&)=delete;Ros2Fanuc_msgsSrvGetPosRegClient(Ros2Fanuc_msgsSrvGetPosRegClient&&)=delete;
~Ros2Fanuc_msgsSrvGetPosRegClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom fanuc_msgs/srv/GetPosReg server; advertise/respond stay separate authorities. */
class Ros2Fanuc_msgsSrvGetPosRegServer:public ServerOwners {
using Handler=std::function<Ros2Fanuc_msgsSrvGetPosReg_Response(Ros2Fanuc_msgsSrvGetPosReg_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Fanuc_msgsSrvGetPosReg_Request;using Response=Ros2Fanuc_msgsSrvGetPosReg_Response;
Ros2Fanuc_msgsSrvGetPosRegServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_respond(&respond_,ticket,&response).error;}
};
/** Complete custom fanuc_msgs/srv/GetTCPSpeed client; default policy returns13 from construction. */
class Ros2Fanuc_msgsSrvGetTCPSpeedClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Fanuc_msgsSrvGetTCPSpeed_Request;using Response=Ros2Fanuc_msgsSrvGetTCPSpeed_Response;
Ros2Fanuc_msgsSrvGetTCPSpeedClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Fanuc_msgsSrvGetTCPSpeedClient(Ros2Fanuc_msgsSrvGetTCPSpeedClient const&)=delete;Ros2Fanuc_msgsSrvGetTCPSpeedClient& operator=(Ros2Fanuc_msgsSrvGetTCPSpeedClient const&)=delete;Ros2Fanuc_msgsSrvGetTCPSpeedClient(Ros2Fanuc_msgsSrvGetTCPSpeedClient&&)=delete;
~Ros2Fanuc_msgsSrvGetTCPSpeedClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom fanuc_msgs/srv/GetTCPSpeed server; advertise/respond stay separate authorities. */
class Ros2Fanuc_msgsSrvGetTCPSpeedServer:public ServerOwners {
using Handler=std::function<Ros2Fanuc_msgsSrvGetTCPSpeed_Response(Ros2Fanuc_msgsSrvGetTCPSpeed_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Fanuc_msgsSrvGetTCPSpeed_Request;using Response=Ros2Fanuc_msgsSrvGetTCPSpeed_Response;
Ros2Fanuc_msgsSrvGetTCPSpeedServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_respond(&respond_,ticket,&response).error;}
};
/** Complete custom fanuc_msgs/srv/GetUFrameData client; default policy returns13 from construction. */
class Ros2Fanuc_msgsSrvGetUFrameDataClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Fanuc_msgsSrvGetUFrameData_Request;using Response=Ros2Fanuc_msgsSrvGetUFrameData_Response;
Ros2Fanuc_msgsSrvGetUFrameDataClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Fanuc_msgsSrvGetUFrameDataClient(Ros2Fanuc_msgsSrvGetUFrameDataClient const&)=delete;Ros2Fanuc_msgsSrvGetUFrameDataClient& operator=(Ros2Fanuc_msgsSrvGetUFrameDataClient const&)=delete;Ros2Fanuc_msgsSrvGetUFrameDataClient(Ros2Fanuc_msgsSrvGetUFrameDataClient&&)=delete;
~Ros2Fanuc_msgsSrvGetUFrameDataClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom fanuc_msgs/srv/GetUFrameData server; advertise/respond stay separate authorities. */
class Ros2Fanuc_msgsSrvGetUFrameDataServer:public ServerOwners {
using Handler=std::function<Ros2Fanuc_msgsSrvGetUFrameData_Response(Ros2Fanuc_msgsSrvGetUFrameData_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Fanuc_msgsSrvGetUFrameData_Request;using Response=Ros2Fanuc_msgsSrvGetUFrameData_Response;
Ros2Fanuc_msgsSrvGetUFrameDataServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_respond(&respond_,ticket,&response).error;}
};
/** Complete custom fanuc_msgs/srv/GetUFrameUTool client; default policy returns13 from construction. */
class Ros2Fanuc_msgsSrvGetUFrameUToolClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Fanuc_msgsSrvGetUFrameUTool_Request;using Response=Ros2Fanuc_msgsSrvGetUFrameUTool_Response;
Ros2Fanuc_msgsSrvGetUFrameUToolClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Fanuc_msgsSrvGetUFrameUToolClient(Ros2Fanuc_msgsSrvGetUFrameUToolClient const&)=delete;Ros2Fanuc_msgsSrvGetUFrameUToolClient& operator=(Ros2Fanuc_msgsSrvGetUFrameUToolClient const&)=delete;Ros2Fanuc_msgsSrvGetUFrameUToolClient(Ros2Fanuc_msgsSrvGetUFrameUToolClient&&)=delete;
~Ros2Fanuc_msgsSrvGetUFrameUToolClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom fanuc_msgs/srv/GetUFrameUTool server; advertise/respond stay separate authorities. */
class Ros2Fanuc_msgsSrvGetUFrameUToolServer:public ServerOwners {
using Handler=std::function<Ros2Fanuc_msgsSrvGetUFrameUTool_Response(Ros2Fanuc_msgsSrvGetUFrameUTool_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Fanuc_msgsSrvGetUFrameUTool_Request;using Response=Ros2Fanuc_msgsSrvGetUFrameUTool_Response;
Ros2Fanuc_msgsSrvGetUFrameUToolServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_respond(&respond_,ticket,&response).error;}
};
/** Complete custom fanuc_msgs/srv/GetUToolData client; default policy returns13 from construction. */
class Ros2Fanuc_msgsSrvGetUToolDataClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Fanuc_msgsSrvGetUToolData_Request;using Response=Ros2Fanuc_msgsSrvGetUToolData_Response;
Ros2Fanuc_msgsSrvGetUToolDataClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Fanuc_msgsSrvGetUToolDataClient(Ros2Fanuc_msgsSrvGetUToolDataClient const&)=delete;Ros2Fanuc_msgsSrvGetUToolDataClient& operator=(Ros2Fanuc_msgsSrvGetUToolDataClient const&)=delete;Ros2Fanuc_msgsSrvGetUToolDataClient(Ros2Fanuc_msgsSrvGetUToolDataClient&&)=delete;
~Ros2Fanuc_msgsSrvGetUToolDataClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom fanuc_msgs/srv/GetUToolData server; advertise/respond stay separate authorities. */
class Ros2Fanuc_msgsSrvGetUToolDataServer:public ServerOwners {
using Handler=std::function<Ros2Fanuc_msgsSrvGetUToolData_Response(Ros2Fanuc_msgsSrvGetUToolData_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Fanuc_msgsSrvGetUToolData_Request;using Response=Ros2Fanuc_msgsSrvGetUToolData_Response;
Ros2Fanuc_msgsSrvGetUToolDataServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_respond(&respond_,ticket,&response).error;}
};
/** Complete custom fanuc_msgs/srv/ReadError client; default policy returns13 from construction. */
class Ros2Fanuc_msgsSrvReadErrorClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Fanuc_msgsSrvReadError_Request;using Response=Ros2Fanuc_msgsSrvReadError_Response;
Ros2Fanuc_msgsSrvReadErrorClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Fanuc_msgsSrvReadErrorClient(Ros2Fanuc_msgsSrvReadErrorClient const&)=delete;Ros2Fanuc_msgsSrvReadErrorClient& operator=(Ros2Fanuc_msgsSrvReadErrorClient const&)=delete;Ros2Fanuc_msgsSrvReadErrorClient(Ros2Fanuc_msgsSrvReadErrorClient&&)=delete;
~Ros2Fanuc_msgsSrvReadErrorClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_ReadError_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom fanuc_msgs/srv/ReadError server; advertise/respond stay separate authorities. */
class Ros2Fanuc_msgsSrvReadErrorServer:public ServerOwners {
using Handler=std::function<Ros2Fanuc_msgsSrvReadError_Response(Ros2Fanuc_msgsSrvReadError_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Fanuc_msgsSrvReadError_Request;using Response=Ros2Fanuc_msgsSrvReadError_Response;
Ros2Fanuc_msgsSrvReadErrorServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_fanuc_msgs_srv_ReadError_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_ReadError_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_fanuc_msgs_srv_ReadError_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_fanuc_msgs_srv_ReadError_respond(&respond_,ticket,&response).error;}
};
/** Complete custom fanuc_msgs/srv/Reset client; default policy returns13 from construction. */
class Ros2Fanuc_msgsSrvResetClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Fanuc_msgsSrvReset_Request;using Response=Ros2Fanuc_msgsSrvReset_Response;
Ros2Fanuc_msgsSrvResetClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Fanuc_msgsSrvResetClient(Ros2Fanuc_msgsSrvResetClient const&)=delete;Ros2Fanuc_msgsSrvResetClient& operator=(Ros2Fanuc_msgsSrvResetClient const&)=delete;Ros2Fanuc_msgsSrvResetClient(Ros2Fanuc_msgsSrvResetClient&&)=delete;
~Ros2Fanuc_msgsSrvResetClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_Reset_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom fanuc_msgs/srv/Reset server; advertise/respond stay separate authorities. */
class Ros2Fanuc_msgsSrvResetServer:public ServerOwners {
using Handler=std::function<Ros2Fanuc_msgsSrvReset_Response(Ros2Fanuc_msgsSrvReset_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Fanuc_msgsSrvReset_Request;using Response=Ros2Fanuc_msgsSrvReset_Response;
Ros2Fanuc_msgsSrvResetServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_fanuc_msgs_srv_Reset_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_Reset_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_fanuc_msgs_srv_Reset_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_fanuc_msgs_srv_Reset_respond(&respond_,ticket,&response).error;}
};
/** Complete custom fanuc_msgs/srv/SetAnalogIO client; default policy returns13 from construction. */
class Ros2Fanuc_msgsSrvSetAnalogIOClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Fanuc_msgsSrvSetAnalogIO_Request;using Response=Ros2Fanuc_msgsSrvSetAnalogIO_Response;
Ros2Fanuc_msgsSrvSetAnalogIOClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Fanuc_msgsSrvSetAnalogIOClient(Ros2Fanuc_msgsSrvSetAnalogIOClient const&)=delete;Ros2Fanuc_msgsSrvSetAnalogIOClient& operator=(Ros2Fanuc_msgsSrvSetAnalogIOClient const&)=delete;Ros2Fanuc_msgsSrvSetAnalogIOClient(Ros2Fanuc_msgsSrvSetAnalogIOClient&&)=delete;
~Ros2Fanuc_msgsSrvSetAnalogIOClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom fanuc_msgs/srv/SetAnalogIO server; advertise/respond stay separate authorities. */
class Ros2Fanuc_msgsSrvSetAnalogIOServer:public ServerOwners {
using Handler=std::function<Ros2Fanuc_msgsSrvSetAnalogIO_Response(Ros2Fanuc_msgsSrvSetAnalogIO_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Fanuc_msgsSrvSetAnalogIO_Request;using Response=Ros2Fanuc_msgsSrvSetAnalogIO_Response;
Ros2Fanuc_msgsSrvSetAnalogIOServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_respond(&respond_,ticket,&response).error;}
};
/** Complete custom fanuc_msgs/srv/SetBoolIO client; default policy returns13 from construction. */
class Ros2Fanuc_msgsSrvSetBoolIOClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Fanuc_msgsSrvSetBoolIO_Request;using Response=Ros2Fanuc_msgsSrvSetBoolIO_Response;
Ros2Fanuc_msgsSrvSetBoolIOClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Fanuc_msgsSrvSetBoolIOClient(Ros2Fanuc_msgsSrvSetBoolIOClient const&)=delete;Ros2Fanuc_msgsSrvSetBoolIOClient& operator=(Ros2Fanuc_msgsSrvSetBoolIOClient const&)=delete;Ros2Fanuc_msgsSrvSetBoolIOClient(Ros2Fanuc_msgsSrvSetBoolIOClient&&)=delete;
~Ros2Fanuc_msgsSrvSetBoolIOClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom fanuc_msgs/srv/SetBoolIO server; advertise/respond stay separate authorities. */
class Ros2Fanuc_msgsSrvSetBoolIOServer:public ServerOwners {
using Handler=std::function<Ros2Fanuc_msgsSrvSetBoolIO_Response(Ros2Fanuc_msgsSrvSetBoolIO_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Fanuc_msgsSrvSetBoolIO_Request;using Response=Ros2Fanuc_msgsSrvSetBoolIO_Response;
Ros2Fanuc_msgsSrvSetBoolIOServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_respond(&respond_,ticket,&response).error;}
};
/** Complete custom fanuc_msgs/srv/SetGenOverride client; default policy returns13 from construction. */
class Ros2Fanuc_msgsSrvSetGenOverrideClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Fanuc_msgsSrvSetGenOverride_Request;using Response=Ros2Fanuc_msgsSrvSetGenOverride_Response;
Ros2Fanuc_msgsSrvSetGenOverrideClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Fanuc_msgsSrvSetGenOverrideClient(Ros2Fanuc_msgsSrvSetGenOverrideClient const&)=delete;Ros2Fanuc_msgsSrvSetGenOverrideClient& operator=(Ros2Fanuc_msgsSrvSetGenOverrideClient const&)=delete;Ros2Fanuc_msgsSrvSetGenOverrideClient(Ros2Fanuc_msgsSrvSetGenOverrideClient&&)=delete;
~Ros2Fanuc_msgsSrvSetGenOverrideClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom fanuc_msgs/srv/SetGenOverride server; advertise/respond stay separate authorities. */
class Ros2Fanuc_msgsSrvSetGenOverrideServer:public ServerOwners {
using Handler=std::function<Ros2Fanuc_msgsSrvSetGenOverride_Response(Ros2Fanuc_msgsSrvSetGenOverride_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Fanuc_msgsSrvSetGenOverride_Request;using Response=Ros2Fanuc_msgsSrvSetGenOverride_Response;
Ros2Fanuc_msgsSrvSetGenOverrideServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_respond(&respond_,ticket,&response).error;}
};
/** Complete custom fanuc_msgs/srv/SetGroupIO client; default policy returns13 from construction. */
class Ros2Fanuc_msgsSrvSetGroupIOClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Fanuc_msgsSrvSetGroupIO_Request;using Response=Ros2Fanuc_msgsSrvSetGroupIO_Response;
Ros2Fanuc_msgsSrvSetGroupIOClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Fanuc_msgsSrvSetGroupIOClient(Ros2Fanuc_msgsSrvSetGroupIOClient const&)=delete;Ros2Fanuc_msgsSrvSetGroupIOClient& operator=(Ros2Fanuc_msgsSrvSetGroupIOClient const&)=delete;Ros2Fanuc_msgsSrvSetGroupIOClient(Ros2Fanuc_msgsSrvSetGroupIOClient&&)=delete;
~Ros2Fanuc_msgsSrvSetGroupIOClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom fanuc_msgs/srv/SetGroupIO server; advertise/respond stay separate authorities. */
class Ros2Fanuc_msgsSrvSetGroupIOServer:public ServerOwners {
using Handler=std::function<Ros2Fanuc_msgsSrvSetGroupIO_Response(Ros2Fanuc_msgsSrvSetGroupIO_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Fanuc_msgsSrvSetGroupIO_Request;using Response=Ros2Fanuc_msgsSrvSetGroupIO_Response;
Ros2Fanuc_msgsSrvSetGroupIOServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_respond(&respond_,ticket,&response).error;}
};
/** Complete custom fanuc_msgs/srv/SetNumReg client; default policy returns13 from construction. */
class Ros2Fanuc_msgsSrvSetNumRegClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Fanuc_msgsSrvSetNumReg_Request;using Response=Ros2Fanuc_msgsSrvSetNumReg_Response;
Ros2Fanuc_msgsSrvSetNumRegClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Fanuc_msgsSrvSetNumRegClient(Ros2Fanuc_msgsSrvSetNumRegClient const&)=delete;Ros2Fanuc_msgsSrvSetNumRegClient& operator=(Ros2Fanuc_msgsSrvSetNumRegClient const&)=delete;Ros2Fanuc_msgsSrvSetNumRegClient(Ros2Fanuc_msgsSrvSetNumRegClient&&)=delete;
~Ros2Fanuc_msgsSrvSetNumRegClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom fanuc_msgs/srv/SetNumReg server; advertise/respond stay separate authorities. */
class Ros2Fanuc_msgsSrvSetNumRegServer:public ServerOwners {
using Handler=std::function<Ros2Fanuc_msgsSrvSetNumReg_Response(Ros2Fanuc_msgsSrvSetNumReg_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Fanuc_msgsSrvSetNumReg_Request;using Response=Ros2Fanuc_msgsSrvSetNumReg_Response;
Ros2Fanuc_msgsSrvSetNumRegServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_respond(&respond_,ticket,&response).error;}
};
/** Complete custom fanuc_msgs/srv/SetPayloadComp client; default policy returns13 from construction. */
class Ros2Fanuc_msgsSrvSetPayloadCompClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Fanuc_msgsSrvSetPayloadComp_Request;using Response=Ros2Fanuc_msgsSrvSetPayloadComp_Response;
Ros2Fanuc_msgsSrvSetPayloadCompClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Fanuc_msgsSrvSetPayloadCompClient(Ros2Fanuc_msgsSrvSetPayloadCompClient const&)=delete;Ros2Fanuc_msgsSrvSetPayloadCompClient& operator=(Ros2Fanuc_msgsSrvSetPayloadCompClient const&)=delete;Ros2Fanuc_msgsSrvSetPayloadCompClient(Ros2Fanuc_msgsSrvSetPayloadCompClient&&)=delete;
~Ros2Fanuc_msgsSrvSetPayloadCompClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom fanuc_msgs/srv/SetPayloadComp server; advertise/respond stay separate authorities. */
class Ros2Fanuc_msgsSrvSetPayloadCompServer:public ServerOwners {
using Handler=std::function<Ros2Fanuc_msgsSrvSetPayloadComp_Response(Ros2Fanuc_msgsSrvSetPayloadComp_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Fanuc_msgsSrvSetPayloadComp_Request;using Response=Ros2Fanuc_msgsSrvSetPayloadComp_Response;
Ros2Fanuc_msgsSrvSetPayloadCompServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_respond(&respond_,ticket,&response).error;}
};
/** Complete custom fanuc_msgs/srv/SetPayloadID client; default policy returns13 from construction. */
class Ros2Fanuc_msgsSrvSetPayloadIDClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Fanuc_msgsSrvSetPayloadID_Request;using Response=Ros2Fanuc_msgsSrvSetPayloadID_Response;
Ros2Fanuc_msgsSrvSetPayloadIDClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Fanuc_msgsSrvSetPayloadIDClient(Ros2Fanuc_msgsSrvSetPayloadIDClient const&)=delete;Ros2Fanuc_msgsSrvSetPayloadIDClient& operator=(Ros2Fanuc_msgsSrvSetPayloadIDClient const&)=delete;Ros2Fanuc_msgsSrvSetPayloadIDClient(Ros2Fanuc_msgsSrvSetPayloadIDClient&&)=delete;
~Ros2Fanuc_msgsSrvSetPayloadIDClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom fanuc_msgs/srv/SetPayloadID server; advertise/respond stay separate authorities. */
class Ros2Fanuc_msgsSrvSetPayloadIDServer:public ServerOwners {
using Handler=std::function<Ros2Fanuc_msgsSrvSetPayloadID_Response(Ros2Fanuc_msgsSrvSetPayloadID_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Fanuc_msgsSrvSetPayloadID_Request;using Response=Ros2Fanuc_msgsSrvSetPayloadID_Response;
Ros2Fanuc_msgsSrvSetPayloadIDServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_respond(&respond_,ticket,&response).error;}
};
/** Complete custom fanuc_msgs/srv/SetPayloadValue client; default policy returns13 from construction. */
class Ros2Fanuc_msgsSrvSetPayloadValueClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Fanuc_msgsSrvSetPayloadValue_Request;using Response=Ros2Fanuc_msgsSrvSetPayloadValue_Response;
Ros2Fanuc_msgsSrvSetPayloadValueClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Fanuc_msgsSrvSetPayloadValueClient(Ros2Fanuc_msgsSrvSetPayloadValueClient const&)=delete;Ros2Fanuc_msgsSrvSetPayloadValueClient& operator=(Ros2Fanuc_msgsSrvSetPayloadValueClient const&)=delete;Ros2Fanuc_msgsSrvSetPayloadValueClient(Ros2Fanuc_msgsSrvSetPayloadValueClient&&)=delete;
~Ros2Fanuc_msgsSrvSetPayloadValueClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom fanuc_msgs/srv/SetPayloadValue server; advertise/respond stay separate authorities. */
class Ros2Fanuc_msgsSrvSetPayloadValueServer:public ServerOwners {
using Handler=std::function<Ros2Fanuc_msgsSrvSetPayloadValue_Response(Ros2Fanuc_msgsSrvSetPayloadValue_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Fanuc_msgsSrvSetPayloadValue_Request;using Response=Ros2Fanuc_msgsSrvSetPayloadValue_Response;
Ros2Fanuc_msgsSrvSetPayloadValueServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_respond(&respond_,ticket,&response).error;}
};
/** Complete custom fanuc_msgs/srv/SetPosReg client; default policy returns13 from construction. */
class Ros2Fanuc_msgsSrvSetPosRegClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Fanuc_msgsSrvSetPosReg_Request;using Response=Ros2Fanuc_msgsSrvSetPosReg_Response;
Ros2Fanuc_msgsSrvSetPosRegClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Fanuc_msgsSrvSetPosRegClient(Ros2Fanuc_msgsSrvSetPosRegClient const&)=delete;Ros2Fanuc_msgsSrvSetPosRegClient& operator=(Ros2Fanuc_msgsSrvSetPosRegClient const&)=delete;Ros2Fanuc_msgsSrvSetPosRegClient(Ros2Fanuc_msgsSrvSetPosRegClient&&)=delete;
~Ros2Fanuc_msgsSrvSetPosRegClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom fanuc_msgs/srv/SetPosReg server; advertise/respond stay separate authorities. */
class Ros2Fanuc_msgsSrvSetPosRegServer:public ServerOwners {
using Handler=std::function<Ros2Fanuc_msgsSrvSetPosReg_Response(Ros2Fanuc_msgsSrvSetPosReg_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Fanuc_msgsSrvSetPosReg_Request;using Response=Ros2Fanuc_msgsSrvSetPosReg_Response;
Ros2Fanuc_msgsSrvSetPosRegServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_respond(&respond_,ticket,&response).error;}
};
/** Complete custom fanuc_msgs/srv/SetUFrameData client; default policy returns13 from construction. */
class Ros2Fanuc_msgsSrvSetUFrameDataClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Fanuc_msgsSrvSetUFrameData_Request;using Response=Ros2Fanuc_msgsSrvSetUFrameData_Response;
Ros2Fanuc_msgsSrvSetUFrameDataClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Fanuc_msgsSrvSetUFrameDataClient(Ros2Fanuc_msgsSrvSetUFrameDataClient const&)=delete;Ros2Fanuc_msgsSrvSetUFrameDataClient& operator=(Ros2Fanuc_msgsSrvSetUFrameDataClient const&)=delete;Ros2Fanuc_msgsSrvSetUFrameDataClient(Ros2Fanuc_msgsSrvSetUFrameDataClient&&)=delete;
~Ros2Fanuc_msgsSrvSetUFrameDataClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom fanuc_msgs/srv/SetUFrameData server; advertise/respond stay separate authorities. */
class Ros2Fanuc_msgsSrvSetUFrameDataServer:public ServerOwners {
using Handler=std::function<Ros2Fanuc_msgsSrvSetUFrameData_Response(Ros2Fanuc_msgsSrvSetUFrameData_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Fanuc_msgsSrvSetUFrameData_Request;using Response=Ros2Fanuc_msgsSrvSetUFrameData_Response;
Ros2Fanuc_msgsSrvSetUFrameDataServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_respond(&respond_,ticket,&response).error;}
};
/** Complete custom fanuc_msgs/srv/SetUFrameUTool client; default policy returns13 from construction. */
class Ros2Fanuc_msgsSrvSetUFrameUToolClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Fanuc_msgsSrvSetUFrameUTool_Request;using Response=Ros2Fanuc_msgsSrvSetUFrameUTool_Response;
Ros2Fanuc_msgsSrvSetUFrameUToolClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Fanuc_msgsSrvSetUFrameUToolClient(Ros2Fanuc_msgsSrvSetUFrameUToolClient const&)=delete;Ros2Fanuc_msgsSrvSetUFrameUToolClient& operator=(Ros2Fanuc_msgsSrvSetUFrameUToolClient const&)=delete;Ros2Fanuc_msgsSrvSetUFrameUToolClient(Ros2Fanuc_msgsSrvSetUFrameUToolClient&&)=delete;
~Ros2Fanuc_msgsSrvSetUFrameUToolClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom fanuc_msgs/srv/SetUFrameUTool server; advertise/respond stay separate authorities. */
class Ros2Fanuc_msgsSrvSetUFrameUToolServer:public ServerOwners {
using Handler=std::function<Ros2Fanuc_msgsSrvSetUFrameUTool_Response(Ros2Fanuc_msgsSrvSetUFrameUTool_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Fanuc_msgsSrvSetUFrameUTool_Request;using Response=Ros2Fanuc_msgsSrvSetUFrameUTool_Response;
Ros2Fanuc_msgsSrvSetUFrameUToolServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_respond(&respond_,ticket,&response).error;}
};
/** Complete custom fanuc_msgs/srv/SetUToolData client; default policy returns13 from construction. */
class Ros2Fanuc_msgsSrvSetUToolDataClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Fanuc_msgsSrvSetUToolData_Request;using Response=Ros2Fanuc_msgsSrvSetUToolData_Response;
Ros2Fanuc_msgsSrvSetUToolDataClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Fanuc_msgsSrvSetUToolDataClient(Ros2Fanuc_msgsSrvSetUToolDataClient const&)=delete;Ros2Fanuc_msgsSrvSetUToolDataClient& operator=(Ros2Fanuc_msgsSrvSetUToolDataClient const&)=delete;Ros2Fanuc_msgsSrvSetUToolDataClient(Ros2Fanuc_msgsSrvSetUToolDataClient&&)=delete;
~Ros2Fanuc_msgsSrvSetUToolDataClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom fanuc_msgs/srv/SetUToolData server; advertise/respond stay separate authorities. */
class Ros2Fanuc_msgsSrvSetUToolDataServer:public ServerOwners {
using Handler=std::function<Ros2Fanuc_msgsSrvSetUToolData_Response(Ros2Fanuc_msgsSrvSetUToolData_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Fanuc_msgsSrvSetUToolData_Request;using Response=Ros2Fanuc_msgsSrvSetUToolData_Response;
Ros2Fanuc_msgsSrvSetUToolDataServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_respond(&respond_,ticket,&response).error;}
};
/** Complete custom fanuc_msgs/srv/SwitchControlState client; default policy returns13 from construction. */
class Ros2Fanuc_msgsSrvSwitchControlStateClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Fanuc_msgsSrvSwitchControlState_Request;using Response=Ros2Fanuc_msgsSrvSwitchControlState_Response;
Ros2Fanuc_msgsSrvSwitchControlStateClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Fanuc_msgsSrvSwitchControlStateClient(Ros2Fanuc_msgsSrvSwitchControlStateClient const&)=delete;Ros2Fanuc_msgsSrvSwitchControlStateClient& operator=(Ros2Fanuc_msgsSrvSwitchControlStateClient const&)=delete;Ros2Fanuc_msgsSrvSwitchControlStateClient(Ros2Fanuc_msgsSrvSwitchControlStateClient&&)=delete;
~Ros2Fanuc_msgsSrvSwitchControlStateClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom fanuc_msgs/srv/SwitchControlState server; advertise/respond stay separate authorities. */
class Ros2Fanuc_msgsSrvSwitchControlStateServer:public ServerOwners {
using Handler=std::function<Ros2Fanuc_msgsSrvSwitchControlState_Response(Ros2Fanuc_msgsSrvSwitchControlState_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Fanuc_msgsSrvSwitchControlState_Request;using Response=Ros2Fanuc_msgsSrvSwitchControlState_Response;
Ros2Fanuc_msgsSrvSwitchControlStateServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_respond(&respond_,ticket,&response).error;}
};
/** Complete custom rmi_msgs/srv/AddCall client; default policy returns13 from construction. */
class Ros2Rmi_msgsSrvAddCallClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Rmi_msgsSrvAddCall_Request;using Response=Ros2Rmi_msgsSrvAddCall_Response;
Ros2Rmi_msgsSrvAddCallClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Rmi_msgsSrvAddCallClient(Ros2Rmi_msgsSrvAddCallClient const&)=delete;Ros2Rmi_msgsSrvAddCallClient& operator=(Ros2Rmi_msgsSrvAddCallClient const&)=delete;Ros2Rmi_msgsSrvAddCallClient(Ros2Rmi_msgsSrvAddCallClient&&)=delete;
~Ros2Rmi_msgsSrvAddCallClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_rmi_msgs_srv_AddCall_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom rmi_msgs/srv/AddCall server; advertise/respond stay separate authorities. */
class Ros2Rmi_msgsSrvAddCallServer:public ServerOwners {
using Handler=std::function<Ros2Rmi_msgsSrvAddCall_Response(Ros2Rmi_msgsSrvAddCall_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Rmi_msgsSrvAddCall_Request;using Response=Ros2Rmi_msgsSrvAddCall_Response;
Ros2Rmi_msgsSrvAddCallServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_rmi_msgs_srv_AddCall_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_rmi_msgs_srv_AddCall_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_rmi_msgs_srv_AddCall_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_rmi_msgs_srv_AddCall_respond(&respond_,ticket,&response).error;}
};
/** Complete custom rmi_msgs/srv/AddMotionInstruction client; default policy returns13 from construction. */
class Ros2Rmi_msgsSrvAddMotionInstructionClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Rmi_msgsSrvAddMotionInstruction_Request;using Response=Ros2Rmi_msgsSrvAddMotionInstruction_Response;
Ros2Rmi_msgsSrvAddMotionInstructionClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Rmi_msgsSrvAddMotionInstructionClient(Ros2Rmi_msgsSrvAddMotionInstructionClient const&)=delete;Ros2Rmi_msgsSrvAddMotionInstructionClient& operator=(Ros2Rmi_msgsSrvAddMotionInstructionClient const&)=delete;Ros2Rmi_msgsSrvAddMotionInstructionClient(Ros2Rmi_msgsSrvAddMotionInstructionClient&&)=delete;
~Ros2Rmi_msgsSrvAddMotionInstructionClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom rmi_msgs/srv/AddMotionInstruction server; advertise/respond stay separate authorities. */
class Ros2Rmi_msgsSrvAddMotionInstructionServer:public ServerOwners {
using Handler=std::function<Ros2Rmi_msgsSrvAddMotionInstruction_Response(Ros2Rmi_msgsSrvAddMotionInstruction_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Rmi_msgsSrvAddMotionInstruction_Request;using Response=Ros2Rmi_msgsSrvAddMotionInstruction_Response;
Ros2Rmi_msgsSrvAddMotionInstructionServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_respond(&respond_,ticket,&response).error;}
};
/** Complete custom rmi_msgs/srv/AddSetPayload client; default policy returns13 from construction. */
class Ros2Rmi_msgsSrvAddSetPayloadClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Rmi_msgsSrvAddSetPayload_Request;using Response=Ros2Rmi_msgsSrvAddSetPayload_Response;
Ros2Rmi_msgsSrvAddSetPayloadClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Rmi_msgsSrvAddSetPayloadClient(Ros2Rmi_msgsSrvAddSetPayloadClient const&)=delete;Ros2Rmi_msgsSrvAddSetPayloadClient& operator=(Ros2Rmi_msgsSrvAddSetPayloadClient const&)=delete;Ros2Rmi_msgsSrvAddSetPayloadClient(Ros2Rmi_msgsSrvAddSetPayloadClient&&)=delete;
~Ros2Rmi_msgsSrvAddSetPayloadClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom rmi_msgs/srv/AddSetPayload server; advertise/respond stay separate authorities. */
class Ros2Rmi_msgsSrvAddSetPayloadServer:public ServerOwners {
using Handler=std::function<Ros2Rmi_msgsSrvAddSetPayload_Response(Ros2Rmi_msgsSrvAddSetPayload_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Rmi_msgsSrvAddSetPayload_Request;using Response=Ros2Rmi_msgsSrvAddSetPayload_Response;
Ros2Rmi_msgsSrvAddSetPayloadServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_respond(&respond_,ticket,&response).error;}
};
/** Complete custom rmi_msgs/srv/AddSetUFrame client; default policy returns13 from construction. */
class Ros2Rmi_msgsSrvAddSetUFrameClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Rmi_msgsSrvAddSetUFrame_Request;using Response=Ros2Rmi_msgsSrvAddSetUFrame_Response;
Ros2Rmi_msgsSrvAddSetUFrameClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Rmi_msgsSrvAddSetUFrameClient(Ros2Rmi_msgsSrvAddSetUFrameClient const&)=delete;Ros2Rmi_msgsSrvAddSetUFrameClient& operator=(Ros2Rmi_msgsSrvAddSetUFrameClient const&)=delete;Ros2Rmi_msgsSrvAddSetUFrameClient(Ros2Rmi_msgsSrvAddSetUFrameClient&&)=delete;
~Ros2Rmi_msgsSrvAddSetUFrameClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom rmi_msgs/srv/AddSetUFrame server; advertise/respond stay separate authorities. */
class Ros2Rmi_msgsSrvAddSetUFrameServer:public ServerOwners {
using Handler=std::function<Ros2Rmi_msgsSrvAddSetUFrame_Response(Ros2Rmi_msgsSrvAddSetUFrame_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Rmi_msgsSrvAddSetUFrame_Request;using Response=Ros2Rmi_msgsSrvAddSetUFrame_Response;
Ros2Rmi_msgsSrvAddSetUFrameServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_respond(&respond_,ticket,&response).error;}
};
/** Complete custom rmi_msgs/srv/AddSetUTool client; default policy returns13 from construction. */
class Ros2Rmi_msgsSrvAddSetUToolClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Rmi_msgsSrvAddSetUTool_Request;using Response=Ros2Rmi_msgsSrvAddSetUTool_Response;
Ros2Rmi_msgsSrvAddSetUToolClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Rmi_msgsSrvAddSetUToolClient(Ros2Rmi_msgsSrvAddSetUToolClient const&)=delete;Ros2Rmi_msgsSrvAddSetUToolClient& operator=(Ros2Rmi_msgsSrvAddSetUToolClient const&)=delete;Ros2Rmi_msgsSrvAddSetUToolClient(Ros2Rmi_msgsSrvAddSetUToolClient&&)=delete;
~Ros2Rmi_msgsSrvAddSetUToolClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom rmi_msgs/srv/AddSetUTool server; advertise/respond stay separate authorities. */
class Ros2Rmi_msgsSrvAddSetUToolServer:public ServerOwners {
using Handler=std::function<Ros2Rmi_msgsSrvAddSetUTool_Response(Ros2Rmi_msgsSrvAddSetUTool_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Rmi_msgsSrvAddSetUTool_Request;using Response=Ros2Rmi_msgsSrvAddSetUTool_Response;
Ros2Rmi_msgsSrvAddSetUToolServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_respond(&respond_,ticket,&response).error;}
};
/** Complete custom rmi_msgs/srv/AddWaitDIN client; default policy returns13 from construction. */
class Ros2Rmi_msgsSrvAddWaitDINClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Rmi_msgsSrvAddWaitDIN_Request;using Response=Ros2Rmi_msgsSrvAddWaitDIN_Response;
Ros2Rmi_msgsSrvAddWaitDINClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Rmi_msgsSrvAddWaitDINClient(Ros2Rmi_msgsSrvAddWaitDINClient const&)=delete;Ros2Rmi_msgsSrvAddWaitDINClient& operator=(Ros2Rmi_msgsSrvAddWaitDINClient const&)=delete;Ros2Rmi_msgsSrvAddWaitDINClient(Ros2Rmi_msgsSrvAddWaitDINClient&&)=delete;
~Ros2Rmi_msgsSrvAddWaitDINClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom rmi_msgs/srv/AddWaitDIN server; advertise/respond stay separate authorities. */
class Ros2Rmi_msgsSrvAddWaitDINServer:public ServerOwners {
using Handler=std::function<Ros2Rmi_msgsSrvAddWaitDIN_Response(Ros2Rmi_msgsSrvAddWaitDIN_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Rmi_msgsSrvAddWaitDIN_Request;using Response=Ros2Rmi_msgsSrvAddWaitDIN_Response;
Ros2Rmi_msgsSrvAddWaitDINServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_respond(&respond_,ticket,&response).error;}
};
/** Complete custom rmi_msgs/srv/AddWaitTime client; default policy returns13 from construction. */
class Ros2Rmi_msgsSrvAddWaitTimeClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Rmi_msgsSrvAddWaitTime_Request;using Response=Ros2Rmi_msgsSrvAddWaitTime_Response;
Ros2Rmi_msgsSrvAddWaitTimeClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Rmi_msgsSrvAddWaitTimeClient(Ros2Rmi_msgsSrvAddWaitTimeClient const&)=delete;Ros2Rmi_msgsSrvAddWaitTimeClient& operator=(Ros2Rmi_msgsSrvAddWaitTimeClient const&)=delete;Ros2Rmi_msgsSrvAddWaitTimeClient(Ros2Rmi_msgsSrvAddWaitTimeClient&&)=delete;
~Ros2Rmi_msgsSrvAddWaitTimeClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom rmi_msgs/srv/AddWaitTime server; advertise/respond stay separate authorities. */
class Ros2Rmi_msgsSrvAddWaitTimeServer:public ServerOwners {
using Handler=std::function<Ros2Rmi_msgsSrvAddWaitTime_Response(Ros2Rmi_msgsSrvAddWaitTime_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Rmi_msgsSrvAddWaitTime_Request;using Response=Ros2Rmi_msgsSrvAddWaitTime_Response;
Ros2Rmi_msgsSrvAddWaitTimeServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_respond(&respond_,ticket,&response).error;}
};
/** Complete custom rmi_msgs/srv/CallCommand client; default policy returns13 from construction. */
class Ros2Rmi_msgsSrvCallCommandClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Rmi_msgsSrvCallCommand_Request;using Response=Ros2Rmi_msgsSrvCallCommand_Response;
Ros2Rmi_msgsSrvCallCommandClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Rmi_msgsSrvCallCommandClient(Ros2Rmi_msgsSrvCallCommandClient const&)=delete;Ros2Rmi_msgsSrvCallCommandClient& operator=(Ros2Rmi_msgsSrvCallCommandClient const&)=delete;Ros2Rmi_msgsSrvCallCommandClient(Ros2Rmi_msgsSrvCallCommandClient&&)=delete;
~Ros2Rmi_msgsSrvCallCommandClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_rmi_msgs_srv_CallCommand_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom rmi_msgs/srv/CallCommand server; advertise/respond stay separate authorities. */
class Ros2Rmi_msgsSrvCallCommandServer:public ServerOwners {
using Handler=std::function<Ros2Rmi_msgsSrvCallCommand_Response(Ros2Rmi_msgsSrvCallCommand_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Rmi_msgsSrvCallCommand_Request;using Response=Ros2Rmi_msgsSrvCallCommand_Response;
Ros2Rmi_msgsSrvCallCommandServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_rmi_msgs_srv_CallCommand_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_rmi_msgs_srv_CallCommand_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_rmi_msgs_srv_CallCommand_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_rmi_msgs_srv_CallCommand_respond(&respond_,ticket,&response).error;}
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
