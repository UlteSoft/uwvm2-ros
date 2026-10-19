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
/** Complete owned custom franka_msgs/action/ErrorRecovery Action client with four separate roles. */
class Ros2Franka_msgsActionErrorRecoveryClient {roswasmc_context context_;std::array<uint64_t,1024> goals_{};roswasmc_action_bundle bundle_{};
public:
using Goal=Ros2Franka_msgsActionErrorRecovery_Goal;using Result=Ros2Franka_msgsActionErrorRecovery_Result;using Feedback=Ros2Franka_msgsActionErrorRecovery_Feedback;
Ros2Franka_msgsActionErrorRecoveryClient(roswasmc_context const& context,roswasmc_action_aliases const& aliases,size_t slots=8):context_{context.backend} {roswasmc_action_bundle_init(&context_,&bundle_,&aliases,goals_.data(),slots);}
Ros2Franka_msgsActionErrorRecoveryClient(Ros2Franka_msgsActionErrorRecoveryClient const&)=delete;Ros2Franka_msgsActionErrorRecoveryClient& operator=(Ros2Franka_msgsActionErrorRecoveryClient const&)=delete;Ros2Franka_msgsActionErrorRecoveryClient(Ros2Franka_msgsActionErrorRecoveryClient&&)=delete;
~Ros2Franka_msgsActionErrorRecoveryClient(){if(bundle_.active){std::array<uint32_t,5> statuses{};roswasmc_action_bundle_close_checked(&bundle_,0,statuses.data(),statuses.size());}}
uint32_t error() const noexcept{return bundle_.error?bundle_.error:bundle_.active?0:9;}bool is_open() const noexcept{return bundle_.active;}bool owns(uint64_t id)const{return roswasmc_action_bundle_owns(&bundle_,id);}
roswasmc_result send_goal(Goal const& value,uint64_t& id){return roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_send(&bundle_,&value,&id);}
roswasmc_result feedback(uint64_t id,Feedback& value){return roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_feedback(&bundle_,id,&value);}
roswasmc_result result(uint64_t id,uint8_t& kind,Result& value){return roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_result(&bundle_,id,&kind,&value);}
roswasmc_result cancel_own(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_own(&bundle_,id,&accepted);}
roswasmc_result cancel_any(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_any(&bundle_,id,&accepted);}
roswasmc_result cancel_all(int& accepted){return roswasmc_action_bundle_cancel_all(&bundle_,&accepted);}
roswasmc_cleanup_report close_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_close_checked(&bundle_,primary,statuses.data(),statuses.size());}
roswasmc_cleanup_report revoke_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_revoke_checked(&bundle_,primary,statuses.data(),statuses.size());}
};
/** Typed custom franka_msgs/action/ErrorRecovery server with explicit feedback/terminal component codecs. */
class Ros2Franka_msgsActionErrorRecoveryServer:public ServerOwners {
public:
using Goal=Ros2Franka_msgsActionErrorRecovery_Goal;using Result=Ros2Franka_msgsActionErrorRecovery_Result;using Feedback=Ros2Franka_msgsActionErrorRecovery_Feedback;
/** Handler may choose separate feedback/result effects; kind must be2..4 when terminal. */
struct Replies {Feedback feedback{};Result result{};uint8_t kind{2};bool send_feedback{true},send_result{true};};
private:using Handler=std::function<Replies(uint64_t,Goal const&)>;std::shared_ptr<Handler> handler_;
public:
Ros2Franka_msgsActionErrorRecoveryServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,true} {}
void on_goal(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& id,Goal& goal){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_poll(&advertise_,&id,&goal);}
roswasmc_result respond_feedback(uint64_t id,Feedback const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_respond_feedback(&respond_,id,&value);}
roswasmc_result respond_result(uint64_t id,uint8_t kind,Result const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_respond_result(&respond_,id,kind,&value);}
/** One pass, at most one feedback plus one terminal response; never implicit replay. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t id{};Goal goal{};auto r=roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_poll(&advertise_,&id,&goal);if(r.error)return r.error;auto replies=(*handler)(id,goal);if(replies.send_result&&(replies.kind<2||replies.kind>4||!roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Result_valid(&replies.result)))return 22;if(replies.send_feedback&&!roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Feedback_valid(&replies.feedback))return 22;if(replies.send_feedback){r=roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_respond_feedback(&respond_,id,&replies.feedback);if(r.error)return r.error;}return replies.send_result?roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_respond_result(&respond_,id,replies.kind,&replies.result).error:0;}
};
/** Complete owned custom franka_msgs/action/Grasp Action client with four separate roles. */
class Ros2Franka_msgsActionGraspClient {roswasmc_context context_;std::array<uint64_t,1024> goals_{};roswasmc_action_bundle bundle_{};
public:
using Goal=Ros2Franka_msgsActionGrasp_Goal;using Result=Ros2Franka_msgsActionGrasp_Result;using Feedback=Ros2Franka_msgsActionGrasp_Feedback;
Ros2Franka_msgsActionGraspClient(roswasmc_context const& context,roswasmc_action_aliases const& aliases,size_t slots=8):context_{context.backend} {roswasmc_action_bundle_init(&context_,&bundle_,&aliases,goals_.data(),slots);}
Ros2Franka_msgsActionGraspClient(Ros2Franka_msgsActionGraspClient const&)=delete;Ros2Franka_msgsActionGraspClient& operator=(Ros2Franka_msgsActionGraspClient const&)=delete;Ros2Franka_msgsActionGraspClient(Ros2Franka_msgsActionGraspClient&&)=delete;
~Ros2Franka_msgsActionGraspClient(){if(bundle_.active){std::array<uint32_t,5> statuses{};roswasmc_action_bundle_close_checked(&bundle_,0,statuses.data(),statuses.size());}}
uint32_t error() const noexcept{return bundle_.error?bundle_.error:bundle_.active?0:9;}bool is_open() const noexcept{return bundle_.active;}bool owns(uint64_t id)const{return roswasmc_action_bundle_owns(&bundle_,id);}
roswasmc_result send_goal(Goal const& value,uint64_t& id){return roswasmc_bounded_2_franka_msgs_action_Grasp_send(&bundle_,&value,&id);}
roswasmc_result feedback(uint64_t id,Feedback& value){return roswasmc_bounded_2_franka_msgs_action_Grasp_feedback(&bundle_,id,&value);}
roswasmc_result result(uint64_t id,uint8_t& kind,Result& value){return roswasmc_bounded_2_franka_msgs_action_Grasp_result(&bundle_,id,&kind,&value);}
roswasmc_result cancel_own(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_own(&bundle_,id,&accepted);}
roswasmc_result cancel_any(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_any(&bundle_,id,&accepted);}
roswasmc_result cancel_all(int& accepted){return roswasmc_action_bundle_cancel_all(&bundle_,&accepted);}
roswasmc_cleanup_report close_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_close_checked(&bundle_,primary,statuses.data(),statuses.size());}
roswasmc_cleanup_report revoke_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_revoke_checked(&bundle_,primary,statuses.data(),statuses.size());}
};
/** Typed custom franka_msgs/action/Grasp server with explicit feedback/terminal component codecs. */
class Ros2Franka_msgsActionGraspServer:public ServerOwners {
public:
using Goal=Ros2Franka_msgsActionGrasp_Goal;using Result=Ros2Franka_msgsActionGrasp_Result;using Feedback=Ros2Franka_msgsActionGrasp_Feedback;
/** Handler may choose separate feedback/result effects; kind must be2..4 when terminal. */
struct Replies {Feedback feedback{};Result result{};uint8_t kind{2};bool send_feedback{true},send_result{true};};
private:using Handler=std::function<Replies(uint64_t,Goal const&)>;std::shared_ptr<Handler> handler_;
public:
Ros2Franka_msgsActionGraspServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,true} {}
void on_goal(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& id,Goal& goal){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_franka_msgs_action_Grasp_poll(&advertise_,&id,&goal);}
roswasmc_result respond_feedback(uint64_t id,Feedback const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_franka_msgs_action_Grasp_respond_feedback(&respond_,id,&value);}
roswasmc_result respond_result(uint64_t id,uint8_t kind,Result const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_franka_msgs_action_Grasp_respond_result(&respond_,id,kind,&value);}
/** One pass, at most one feedback plus one terminal response; never implicit replay. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t id{};Goal goal{};auto r=roswasmc_bounded_2_franka_msgs_action_Grasp_poll(&advertise_,&id,&goal);if(r.error)return r.error;auto replies=(*handler)(id,goal);if(replies.send_result&&(replies.kind<2||replies.kind>4||!roswasmc_bounded_2_franka_msgs_action_Grasp_Result_valid(&replies.result)))return 22;if(replies.send_feedback&&!roswasmc_bounded_2_franka_msgs_action_Grasp_Feedback_valid(&replies.feedback))return 22;if(replies.send_feedback){r=roswasmc_bounded_2_franka_msgs_action_Grasp_respond_feedback(&respond_,id,&replies.feedback);if(r.error)return r.error;}return replies.send_result?roswasmc_bounded_2_franka_msgs_action_Grasp_respond_result(&respond_,id,replies.kind,&replies.result).error:0;}
};
/** Complete owned custom franka_msgs/action/Homing Action client with four separate roles. */
class Ros2Franka_msgsActionHomingClient {roswasmc_context context_;std::array<uint64_t,1024> goals_{};roswasmc_action_bundle bundle_{};
public:
using Goal=Ros2Franka_msgsActionHoming_Goal;using Result=Ros2Franka_msgsActionHoming_Result;using Feedback=Ros2Franka_msgsActionHoming_Feedback;
Ros2Franka_msgsActionHomingClient(roswasmc_context const& context,roswasmc_action_aliases const& aliases,size_t slots=8):context_{context.backend} {roswasmc_action_bundle_init(&context_,&bundle_,&aliases,goals_.data(),slots);}
Ros2Franka_msgsActionHomingClient(Ros2Franka_msgsActionHomingClient const&)=delete;Ros2Franka_msgsActionHomingClient& operator=(Ros2Franka_msgsActionHomingClient const&)=delete;Ros2Franka_msgsActionHomingClient(Ros2Franka_msgsActionHomingClient&&)=delete;
~Ros2Franka_msgsActionHomingClient(){if(bundle_.active){std::array<uint32_t,5> statuses{};roswasmc_action_bundle_close_checked(&bundle_,0,statuses.data(),statuses.size());}}
uint32_t error() const noexcept{return bundle_.error?bundle_.error:bundle_.active?0:9;}bool is_open() const noexcept{return bundle_.active;}bool owns(uint64_t id)const{return roswasmc_action_bundle_owns(&bundle_,id);}
roswasmc_result send_goal(Goal const& value,uint64_t& id){return roswasmc_bounded_2_franka_msgs_action_Homing_send(&bundle_,&value,&id);}
roswasmc_result feedback(uint64_t id,Feedback& value){return roswasmc_bounded_2_franka_msgs_action_Homing_feedback(&bundle_,id,&value);}
roswasmc_result result(uint64_t id,uint8_t& kind,Result& value){return roswasmc_bounded_2_franka_msgs_action_Homing_result(&bundle_,id,&kind,&value);}
roswasmc_result cancel_own(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_own(&bundle_,id,&accepted);}
roswasmc_result cancel_any(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_any(&bundle_,id,&accepted);}
roswasmc_result cancel_all(int& accepted){return roswasmc_action_bundle_cancel_all(&bundle_,&accepted);}
roswasmc_cleanup_report close_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_close_checked(&bundle_,primary,statuses.data(),statuses.size());}
roswasmc_cleanup_report revoke_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_revoke_checked(&bundle_,primary,statuses.data(),statuses.size());}
};
/** Typed custom franka_msgs/action/Homing server with explicit feedback/terminal component codecs. */
class Ros2Franka_msgsActionHomingServer:public ServerOwners {
public:
using Goal=Ros2Franka_msgsActionHoming_Goal;using Result=Ros2Franka_msgsActionHoming_Result;using Feedback=Ros2Franka_msgsActionHoming_Feedback;
/** Handler may choose separate feedback/result effects; kind must be2..4 when terminal. */
struct Replies {Feedback feedback{};Result result{};uint8_t kind{2};bool send_feedback{true},send_result{true};};
private:using Handler=std::function<Replies(uint64_t,Goal const&)>;std::shared_ptr<Handler> handler_;
public:
Ros2Franka_msgsActionHomingServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,true} {}
void on_goal(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& id,Goal& goal){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_franka_msgs_action_Homing_poll(&advertise_,&id,&goal);}
roswasmc_result respond_feedback(uint64_t id,Feedback const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_franka_msgs_action_Homing_respond_feedback(&respond_,id,&value);}
roswasmc_result respond_result(uint64_t id,uint8_t kind,Result const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_franka_msgs_action_Homing_respond_result(&respond_,id,kind,&value);}
/** One pass, at most one feedback plus one terminal response; never implicit replay. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t id{};Goal goal{};auto r=roswasmc_bounded_2_franka_msgs_action_Homing_poll(&advertise_,&id,&goal);if(r.error)return r.error;auto replies=(*handler)(id,goal);if(replies.send_result&&(replies.kind<2||replies.kind>4||!roswasmc_bounded_2_franka_msgs_action_Homing_Result_valid(&replies.result)))return 22;if(replies.send_feedback&&!roswasmc_bounded_2_franka_msgs_action_Homing_Feedback_valid(&replies.feedback))return 22;if(replies.send_feedback){r=roswasmc_bounded_2_franka_msgs_action_Homing_respond_feedback(&respond_,id,&replies.feedback);if(r.error)return r.error;}return replies.send_result?roswasmc_bounded_2_franka_msgs_action_Homing_respond_result(&respond_,id,replies.kind,&replies.result).error:0;}
};
/** Complete owned custom franka_msgs/action/Move Action client with four separate roles. */
class Ros2Franka_msgsActionMoveClient {roswasmc_context context_;std::array<uint64_t,1024> goals_{};roswasmc_action_bundle bundle_{};
public:
using Goal=Ros2Franka_msgsActionMove_Goal;using Result=Ros2Franka_msgsActionMove_Result;using Feedback=Ros2Franka_msgsActionMove_Feedback;
Ros2Franka_msgsActionMoveClient(roswasmc_context const& context,roswasmc_action_aliases const& aliases,size_t slots=8):context_{context.backend} {roswasmc_action_bundle_init(&context_,&bundle_,&aliases,goals_.data(),slots);}
Ros2Franka_msgsActionMoveClient(Ros2Franka_msgsActionMoveClient const&)=delete;Ros2Franka_msgsActionMoveClient& operator=(Ros2Franka_msgsActionMoveClient const&)=delete;Ros2Franka_msgsActionMoveClient(Ros2Franka_msgsActionMoveClient&&)=delete;
~Ros2Franka_msgsActionMoveClient(){if(bundle_.active){std::array<uint32_t,5> statuses{};roswasmc_action_bundle_close_checked(&bundle_,0,statuses.data(),statuses.size());}}
uint32_t error() const noexcept{return bundle_.error?bundle_.error:bundle_.active?0:9;}bool is_open() const noexcept{return bundle_.active;}bool owns(uint64_t id)const{return roswasmc_action_bundle_owns(&bundle_,id);}
roswasmc_result send_goal(Goal const& value,uint64_t& id){return roswasmc_bounded_2_franka_msgs_action_Move_send(&bundle_,&value,&id);}
roswasmc_result feedback(uint64_t id,Feedback& value){return roswasmc_bounded_2_franka_msgs_action_Move_feedback(&bundle_,id,&value);}
roswasmc_result result(uint64_t id,uint8_t& kind,Result& value){return roswasmc_bounded_2_franka_msgs_action_Move_result(&bundle_,id,&kind,&value);}
roswasmc_result cancel_own(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_own(&bundle_,id,&accepted);}
roswasmc_result cancel_any(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_any(&bundle_,id,&accepted);}
roswasmc_result cancel_all(int& accepted){return roswasmc_action_bundle_cancel_all(&bundle_,&accepted);}
roswasmc_cleanup_report close_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_close_checked(&bundle_,primary,statuses.data(),statuses.size());}
roswasmc_cleanup_report revoke_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_revoke_checked(&bundle_,primary,statuses.data(),statuses.size());}
};
/** Typed custom franka_msgs/action/Move server with explicit feedback/terminal component codecs. */
class Ros2Franka_msgsActionMoveServer:public ServerOwners {
public:
using Goal=Ros2Franka_msgsActionMove_Goal;using Result=Ros2Franka_msgsActionMove_Result;using Feedback=Ros2Franka_msgsActionMove_Feedback;
/** Handler may choose separate feedback/result effects; kind must be2..4 when terminal. */
struct Replies {Feedback feedback{};Result result{};uint8_t kind{2};bool send_feedback{true},send_result{true};};
private:using Handler=std::function<Replies(uint64_t,Goal const&)>;std::shared_ptr<Handler> handler_;
public:
Ros2Franka_msgsActionMoveServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,true} {}
void on_goal(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& id,Goal& goal){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_franka_msgs_action_Move_poll(&advertise_,&id,&goal);}
roswasmc_result respond_feedback(uint64_t id,Feedback const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_franka_msgs_action_Move_respond_feedback(&respond_,id,&value);}
roswasmc_result respond_result(uint64_t id,uint8_t kind,Result const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_franka_msgs_action_Move_respond_result(&respond_,id,kind,&value);}
/** One pass, at most one feedback plus one terminal response; never implicit replay. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t id{};Goal goal{};auto r=roswasmc_bounded_2_franka_msgs_action_Move_poll(&advertise_,&id,&goal);if(r.error)return r.error;auto replies=(*handler)(id,goal);if(replies.send_result&&(replies.kind<2||replies.kind>4||!roswasmc_bounded_2_franka_msgs_action_Move_Result_valid(&replies.result)))return 22;if(replies.send_feedback&&!roswasmc_bounded_2_franka_msgs_action_Move_Feedback_valid(&replies.feedback))return 22;if(replies.send_feedback){r=roswasmc_bounded_2_franka_msgs_action_Move_respond_feedback(&respond_,id,&replies.feedback);if(r.error)return r.error;}return replies.send_result?roswasmc_bounded_2_franka_msgs_action_Move_respond_result(&respond_,id,replies.kind,&replies.result).error:0;}
};
/** Complete owned custom franka_msgs/action/PTPMotion Action client with four separate roles. */
class Ros2Franka_msgsActionPTPMotionClient {roswasmc_context context_;std::array<uint64_t,1024> goals_{};roswasmc_action_bundle bundle_{};
public:
using Goal=Ros2Franka_msgsActionPTPMotion_Goal;using Result=Ros2Franka_msgsActionPTPMotion_Result;using Feedback=Ros2Franka_msgsActionPTPMotion_Feedback;
Ros2Franka_msgsActionPTPMotionClient(roswasmc_context const& context,roswasmc_action_aliases const& aliases,size_t slots=8):context_{context.backend} {roswasmc_action_bundle_init(&context_,&bundle_,&aliases,goals_.data(),slots);}
Ros2Franka_msgsActionPTPMotionClient(Ros2Franka_msgsActionPTPMotionClient const&)=delete;Ros2Franka_msgsActionPTPMotionClient& operator=(Ros2Franka_msgsActionPTPMotionClient const&)=delete;Ros2Franka_msgsActionPTPMotionClient(Ros2Franka_msgsActionPTPMotionClient&&)=delete;
~Ros2Franka_msgsActionPTPMotionClient(){if(bundle_.active){std::array<uint32_t,5> statuses{};roswasmc_action_bundle_close_checked(&bundle_,0,statuses.data(),statuses.size());}}
uint32_t error() const noexcept{return bundle_.error?bundle_.error:bundle_.active?0:9;}bool is_open() const noexcept{return bundle_.active;}bool owns(uint64_t id)const{return roswasmc_action_bundle_owns(&bundle_,id);}
roswasmc_result send_goal(Goal const& value,uint64_t& id){return roswasmc_bounded_2_franka_msgs_action_PTPMotion_send(&bundle_,&value,&id);}
roswasmc_result feedback(uint64_t id,Feedback& value){return roswasmc_bounded_2_franka_msgs_action_PTPMotion_feedback(&bundle_,id,&value);}
roswasmc_result result(uint64_t id,uint8_t& kind,Result& value){return roswasmc_bounded_2_franka_msgs_action_PTPMotion_result(&bundle_,id,&kind,&value);}
roswasmc_result cancel_own(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_own(&bundle_,id,&accepted);}
roswasmc_result cancel_any(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_any(&bundle_,id,&accepted);}
roswasmc_result cancel_all(int& accepted){return roswasmc_action_bundle_cancel_all(&bundle_,&accepted);}
roswasmc_cleanup_report close_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_close_checked(&bundle_,primary,statuses.data(),statuses.size());}
roswasmc_cleanup_report revoke_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_revoke_checked(&bundle_,primary,statuses.data(),statuses.size());}
};
/** Typed custom franka_msgs/action/PTPMotion server with explicit feedback/terminal component codecs. */
class Ros2Franka_msgsActionPTPMotionServer:public ServerOwners {
public:
using Goal=Ros2Franka_msgsActionPTPMotion_Goal;using Result=Ros2Franka_msgsActionPTPMotion_Result;using Feedback=Ros2Franka_msgsActionPTPMotion_Feedback;
/** Handler may choose separate feedback/result effects; kind must be2..4 when terminal. */
struct Replies {Feedback feedback{};Result result{};uint8_t kind{2};bool send_feedback{true},send_result{true};};
private:using Handler=std::function<Replies(uint64_t,Goal const&)>;std::shared_ptr<Handler> handler_;
public:
Ros2Franka_msgsActionPTPMotionServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,true} {}
void on_goal(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& id,Goal& goal){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_franka_msgs_action_PTPMotion_poll(&advertise_,&id,&goal);}
roswasmc_result respond_feedback(uint64_t id,Feedback const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_franka_msgs_action_PTPMotion_respond_feedback(&respond_,id,&value);}
roswasmc_result respond_result(uint64_t id,uint8_t kind,Result const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_franka_msgs_action_PTPMotion_respond_result(&respond_,id,kind,&value);}
/** One pass, at most one feedback plus one terminal response; never implicit replay. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t id{};Goal goal{};auto r=roswasmc_bounded_2_franka_msgs_action_PTPMotion_poll(&advertise_,&id,&goal);if(r.error)return r.error;auto replies=(*handler)(id,goal);if(replies.send_result&&(replies.kind<2||replies.kind>4||!roswasmc_bounded_2_franka_msgs_action_PTPMotion_Result_valid(&replies.result)))return 22;if(replies.send_feedback&&!roswasmc_bounded_2_franka_msgs_action_PTPMotion_Feedback_valid(&replies.feedback))return 22;if(replies.send_feedback){r=roswasmc_bounded_2_franka_msgs_action_PTPMotion_respond_feedback(&respond_,id,&replies.feedback);if(r.error)return r.error;}return replies.send_result?roswasmc_bounded_2_franka_msgs_action_PTPMotion_respond_result(&respond_,id,replies.kind,&replies.result).error:0;}
};
/** Complete custom franka_msgs/srv/SetCartesianStiffness client; default policy returns13 from construction. */
class Ros2Franka_msgsSrvSetCartesianStiffnessClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Franka_msgsSrvSetCartesianStiffness_Request;using Response=Ros2Franka_msgsSrvSetCartesianStiffness_Response;
Ros2Franka_msgsSrvSetCartesianStiffnessClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Franka_msgsSrvSetCartesianStiffnessClient(Ros2Franka_msgsSrvSetCartesianStiffnessClient const&)=delete;Ros2Franka_msgsSrvSetCartesianStiffnessClient& operator=(Ros2Franka_msgsSrvSetCartesianStiffnessClient const&)=delete;Ros2Franka_msgsSrvSetCartesianStiffnessClient(Ros2Franka_msgsSrvSetCartesianStiffnessClient&&)=delete;
~Ros2Franka_msgsSrvSetCartesianStiffnessClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom franka_msgs/srv/SetCartesianStiffness server; advertise/respond stay separate authorities. */
class Ros2Franka_msgsSrvSetCartesianStiffnessServer:public ServerOwners {
using Handler=std::function<Ros2Franka_msgsSrvSetCartesianStiffness_Response(Ros2Franka_msgsSrvSetCartesianStiffness_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Franka_msgsSrvSetCartesianStiffness_Request;using Response=Ros2Franka_msgsSrvSetCartesianStiffness_Response;
Ros2Franka_msgsSrvSetCartesianStiffnessServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_respond(&respond_,ticket,&response).error;}
};
/** Complete custom franka_msgs/srv/SetForceTorqueCollisionBehavior client; default policy returns13 from construction. */
class Ros2Franka_msgsSrvSetForceTorqueCollisionBehaviorClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Request;using Response=Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Response;
Ros2Franka_msgsSrvSetForceTorqueCollisionBehaviorClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Franka_msgsSrvSetForceTorqueCollisionBehaviorClient(Ros2Franka_msgsSrvSetForceTorqueCollisionBehaviorClient const&)=delete;Ros2Franka_msgsSrvSetForceTorqueCollisionBehaviorClient& operator=(Ros2Franka_msgsSrvSetForceTorqueCollisionBehaviorClient const&)=delete;Ros2Franka_msgsSrvSetForceTorqueCollisionBehaviorClient(Ros2Franka_msgsSrvSetForceTorqueCollisionBehaviorClient&&)=delete;
~Ros2Franka_msgsSrvSetForceTorqueCollisionBehaviorClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom franka_msgs/srv/SetForceTorqueCollisionBehavior server; advertise/respond stay separate authorities. */
class Ros2Franka_msgsSrvSetForceTorqueCollisionBehaviorServer:public ServerOwners {
using Handler=std::function<Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Response(Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Request;using Response=Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Response;
Ros2Franka_msgsSrvSetForceTorqueCollisionBehaviorServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_respond(&respond_,ticket,&response).error;}
};
/** Complete custom franka_msgs/srv/SetFullCollisionBehavior client; default policy returns13 from construction. */
class Ros2Franka_msgsSrvSetFullCollisionBehaviorClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Franka_msgsSrvSetFullCollisionBehavior_Request;using Response=Ros2Franka_msgsSrvSetFullCollisionBehavior_Response;
Ros2Franka_msgsSrvSetFullCollisionBehaviorClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Franka_msgsSrvSetFullCollisionBehaviorClient(Ros2Franka_msgsSrvSetFullCollisionBehaviorClient const&)=delete;Ros2Franka_msgsSrvSetFullCollisionBehaviorClient& operator=(Ros2Franka_msgsSrvSetFullCollisionBehaviorClient const&)=delete;Ros2Franka_msgsSrvSetFullCollisionBehaviorClient(Ros2Franka_msgsSrvSetFullCollisionBehaviorClient&&)=delete;
~Ros2Franka_msgsSrvSetFullCollisionBehaviorClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom franka_msgs/srv/SetFullCollisionBehavior server; advertise/respond stay separate authorities. */
class Ros2Franka_msgsSrvSetFullCollisionBehaviorServer:public ServerOwners {
using Handler=std::function<Ros2Franka_msgsSrvSetFullCollisionBehavior_Response(Ros2Franka_msgsSrvSetFullCollisionBehavior_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Franka_msgsSrvSetFullCollisionBehavior_Request;using Response=Ros2Franka_msgsSrvSetFullCollisionBehavior_Response;
Ros2Franka_msgsSrvSetFullCollisionBehaviorServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_respond(&respond_,ticket,&response).error;}
};
/** Complete custom franka_msgs/srv/SetJointStiffness client; default policy returns13 from construction. */
class Ros2Franka_msgsSrvSetJointStiffnessClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Franka_msgsSrvSetJointStiffness_Request;using Response=Ros2Franka_msgsSrvSetJointStiffness_Response;
Ros2Franka_msgsSrvSetJointStiffnessClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Franka_msgsSrvSetJointStiffnessClient(Ros2Franka_msgsSrvSetJointStiffnessClient const&)=delete;Ros2Franka_msgsSrvSetJointStiffnessClient& operator=(Ros2Franka_msgsSrvSetJointStiffnessClient const&)=delete;Ros2Franka_msgsSrvSetJointStiffnessClient(Ros2Franka_msgsSrvSetJointStiffnessClient&&)=delete;
~Ros2Franka_msgsSrvSetJointStiffnessClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom franka_msgs/srv/SetJointStiffness server; advertise/respond stay separate authorities. */
class Ros2Franka_msgsSrvSetJointStiffnessServer:public ServerOwners {
using Handler=std::function<Ros2Franka_msgsSrvSetJointStiffness_Response(Ros2Franka_msgsSrvSetJointStiffness_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Franka_msgsSrvSetJointStiffness_Request;using Response=Ros2Franka_msgsSrvSetJointStiffness_Response;
Ros2Franka_msgsSrvSetJointStiffnessServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_respond(&respond_,ticket,&response).error;}
};
/** Complete custom franka_msgs/srv/SetLoad client; default policy returns13 from construction. */
class Ros2Franka_msgsSrvSetLoadClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Franka_msgsSrvSetLoad_Request;using Response=Ros2Franka_msgsSrvSetLoad_Response;
Ros2Franka_msgsSrvSetLoadClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Franka_msgsSrvSetLoadClient(Ros2Franka_msgsSrvSetLoadClient const&)=delete;Ros2Franka_msgsSrvSetLoadClient& operator=(Ros2Franka_msgsSrvSetLoadClient const&)=delete;Ros2Franka_msgsSrvSetLoadClient(Ros2Franka_msgsSrvSetLoadClient&&)=delete;
~Ros2Franka_msgsSrvSetLoadClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_franka_msgs_srv_SetLoad_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom franka_msgs/srv/SetLoad server; advertise/respond stay separate authorities. */
class Ros2Franka_msgsSrvSetLoadServer:public ServerOwners {
using Handler=std::function<Ros2Franka_msgsSrvSetLoad_Response(Ros2Franka_msgsSrvSetLoad_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Franka_msgsSrvSetLoad_Request;using Response=Ros2Franka_msgsSrvSetLoad_Response;
Ros2Franka_msgsSrvSetLoadServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_franka_msgs_srv_SetLoad_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_franka_msgs_srv_SetLoad_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_franka_msgs_srv_SetLoad_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_franka_msgs_srv_SetLoad_respond(&respond_,ticket,&response).error;}
};
/** Complete custom franka_msgs/srv/SetStiffnessFrame client; default policy returns13 from construction. */
class Ros2Franka_msgsSrvSetStiffnessFrameClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Franka_msgsSrvSetStiffnessFrame_Request;using Response=Ros2Franka_msgsSrvSetStiffnessFrame_Response;
Ros2Franka_msgsSrvSetStiffnessFrameClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Franka_msgsSrvSetStiffnessFrameClient(Ros2Franka_msgsSrvSetStiffnessFrameClient const&)=delete;Ros2Franka_msgsSrvSetStiffnessFrameClient& operator=(Ros2Franka_msgsSrvSetStiffnessFrameClient const&)=delete;Ros2Franka_msgsSrvSetStiffnessFrameClient(Ros2Franka_msgsSrvSetStiffnessFrameClient&&)=delete;
~Ros2Franka_msgsSrvSetStiffnessFrameClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom franka_msgs/srv/SetStiffnessFrame server; advertise/respond stay separate authorities. */
class Ros2Franka_msgsSrvSetStiffnessFrameServer:public ServerOwners {
using Handler=std::function<Ros2Franka_msgsSrvSetStiffnessFrame_Response(Ros2Franka_msgsSrvSetStiffnessFrame_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Franka_msgsSrvSetStiffnessFrame_Request;using Response=Ros2Franka_msgsSrvSetStiffnessFrame_Response;
Ros2Franka_msgsSrvSetStiffnessFrameServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_respond(&respond_,ticket,&response).error;}
};
/** Complete custom franka_msgs/srv/SetTCPFrame client; default policy returns13 from construction. */
class Ros2Franka_msgsSrvSetTCPFrameClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Franka_msgsSrvSetTCPFrame_Request;using Response=Ros2Franka_msgsSrvSetTCPFrame_Response;
Ros2Franka_msgsSrvSetTCPFrameClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Franka_msgsSrvSetTCPFrameClient(Ros2Franka_msgsSrvSetTCPFrameClient const&)=delete;Ros2Franka_msgsSrvSetTCPFrameClient& operator=(Ros2Franka_msgsSrvSetTCPFrameClient const&)=delete;Ros2Franka_msgsSrvSetTCPFrameClient(Ros2Franka_msgsSrvSetTCPFrameClient&&)=delete;
~Ros2Franka_msgsSrvSetTCPFrameClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom franka_msgs/srv/SetTCPFrame server; advertise/respond stay separate authorities. */
class Ros2Franka_msgsSrvSetTCPFrameServer:public ServerOwners {
using Handler=std::function<Ros2Franka_msgsSrvSetTCPFrame_Response(Ros2Franka_msgsSrvSetTCPFrame_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Franka_msgsSrvSetTCPFrame_Request;using Response=Ros2Franka_msgsSrvSetTCPFrame_Response;
Ros2Franka_msgsSrvSetTCPFrameServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_respond(&respond_,ticket,&response).error;}
};
/** Complete owned custom franka_spine_msgs/action/MoveAbsolute Action client with four separate roles. */
class Ros2Franka_spine_msgsActionMoveAbsoluteClient {roswasmc_context context_;std::array<uint64_t,1024> goals_{};roswasmc_action_bundle bundle_{};
public:
using Goal=Ros2Franka_spine_msgsActionMoveAbsolute_Goal;using Result=Ros2Franka_spine_msgsActionMoveAbsolute_Result;using Feedback=Ros2Franka_spine_msgsActionMoveAbsolute_Feedback;
Ros2Franka_spine_msgsActionMoveAbsoluteClient(roswasmc_context const& context,roswasmc_action_aliases const& aliases,size_t slots=8):context_{context.backend} {roswasmc_action_bundle_init(&context_,&bundle_,&aliases,goals_.data(),slots);}
Ros2Franka_spine_msgsActionMoveAbsoluteClient(Ros2Franka_spine_msgsActionMoveAbsoluteClient const&)=delete;Ros2Franka_spine_msgsActionMoveAbsoluteClient& operator=(Ros2Franka_spine_msgsActionMoveAbsoluteClient const&)=delete;Ros2Franka_spine_msgsActionMoveAbsoluteClient(Ros2Franka_spine_msgsActionMoveAbsoluteClient&&)=delete;
~Ros2Franka_spine_msgsActionMoveAbsoluteClient(){if(bundle_.active){std::array<uint32_t,5> statuses{};roswasmc_action_bundle_close_checked(&bundle_,0,statuses.data(),statuses.size());}}
uint32_t error() const noexcept{return bundle_.error?bundle_.error:bundle_.active?0:9;}bool is_open() const noexcept{return bundle_.active;}bool owns(uint64_t id)const{return roswasmc_action_bundle_owns(&bundle_,id);}
roswasmc_result send_goal(Goal const& value,uint64_t& id){return roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_send(&bundle_,&value,&id);}
roswasmc_result feedback(uint64_t id,Feedback& value){return roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_feedback(&bundle_,id,&value);}
roswasmc_result result(uint64_t id,uint8_t& kind,Result& value){return roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_result(&bundle_,id,&kind,&value);}
roswasmc_result cancel_own(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_own(&bundle_,id,&accepted);}
roswasmc_result cancel_any(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_any(&bundle_,id,&accepted);}
roswasmc_result cancel_all(int& accepted){return roswasmc_action_bundle_cancel_all(&bundle_,&accepted);}
roswasmc_cleanup_report close_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_close_checked(&bundle_,primary,statuses.data(),statuses.size());}
roswasmc_cleanup_report revoke_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_revoke_checked(&bundle_,primary,statuses.data(),statuses.size());}
};
/** Typed custom franka_spine_msgs/action/MoveAbsolute server with explicit feedback/terminal component codecs. */
class Ros2Franka_spine_msgsActionMoveAbsoluteServer:public ServerOwners {
public:
using Goal=Ros2Franka_spine_msgsActionMoveAbsolute_Goal;using Result=Ros2Franka_spine_msgsActionMoveAbsolute_Result;using Feedback=Ros2Franka_spine_msgsActionMoveAbsolute_Feedback;
/** Handler may choose separate feedback/result effects; kind must be2..4 when terminal. */
struct Replies {Feedback feedback{};Result result{};uint8_t kind{2};bool send_feedback{true},send_result{true};};
private:using Handler=std::function<Replies(uint64_t,Goal const&)>;std::shared_ptr<Handler> handler_;
public:
Ros2Franka_spine_msgsActionMoveAbsoluteServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,true} {}
void on_goal(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& id,Goal& goal){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_poll(&advertise_,&id,&goal);}
roswasmc_result respond_feedback(uint64_t id,Feedback const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_respond_feedback(&respond_,id,&value);}
roswasmc_result respond_result(uint64_t id,uint8_t kind,Result const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_respond_result(&respond_,id,kind,&value);}
/** One pass, at most one feedback plus one terminal response; never implicit replay. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t id{};Goal goal{};auto r=roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_poll(&advertise_,&id,&goal);if(r.error)return r.error;auto replies=(*handler)(id,goal);if(replies.send_result&&(replies.kind<2||replies.kind>4||!roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Result_valid(&replies.result)))return 22;if(replies.send_feedback&&!roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Feedback_valid(&replies.feedback))return 22;if(replies.send_feedback){r=roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_respond_feedback(&respond_,id,&replies.feedback);if(r.error)return r.error;}return replies.send_result?roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_respond_result(&respond_,id,replies.kind,&replies.result).error:0;}
};
/** Complete custom franka_spine_msgs/srv/FaultReset client; default policy returns13 from construction. */
class Ros2Franka_spine_msgsSrvFaultResetClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Franka_spine_msgsSrvFaultReset_Request;using Response=Ros2Franka_spine_msgsSrvFaultReset_Response;
Ros2Franka_spine_msgsSrvFaultResetClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Franka_spine_msgsSrvFaultResetClient(Ros2Franka_spine_msgsSrvFaultResetClient const&)=delete;Ros2Franka_spine_msgsSrvFaultResetClient& operator=(Ros2Franka_spine_msgsSrvFaultResetClient const&)=delete;Ros2Franka_spine_msgsSrvFaultResetClient(Ros2Franka_spine_msgsSrvFaultResetClient&&)=delete;
~Ros2Franka_spine_msgsSrvFaultResetClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom franka_spine_msgs/srv/FaultReset server; advertise/respond stay separate authorities. */
class Ros2Franka_spine_msgsSrvFaultResetServer:public ServerOwners {
using Handler=std::function<Ros2Franka_spine_msgsSrvFaultReset_Response(Ros2Franka_spine_msgsSrvFaultReset_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Franka_spine_msgsSrvFaultReset_Request;using Response=Ros2Franka_spine_msgsSrvFaultReset_Response;
Ros2Franka_spine_msgsSrvFaultResetServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_respond(&respond_,ticket,&response).error;}
};
/** Complete custom franka_spine_msgs/srv/GetParameters client; default policy returns13 from construction. */
class Ros2Franka_spine_msgsSrvGetParametersClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Franka_spine_msgsSrvGetParameters_Request;using Response=Ros2Franka_spine_msgsSrvGetParameters_Response;
Ros2Franka_spine_msgsSrvGetParametersClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Franka_spine_msgsSrvGetParametersClient(Ros2Franka_spine_msgsSrvGetParametersClient const&)=delete;Ros2Franka_spine_msgsSrvGetParametersClient& operator=(Ros2Franka_spine_msgsSrvGetParametersClient const&)=delete;Ros2Franka_spine_msgsSrvGetParametersClient(Ros2Franka_spine_msgsSrvGetParametersClient&&)=delete;
~Ros2Franka_spine_msgsSrvGetParametersClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom franka_spine_msgs/srv/GetParameters server; advertise/respond stay separate authorities. */
class Ros2Franka_spine_msgsSrvGetParametersServer:public ServerOwners {
using Handler=std::function<Ros2Franka_spine_msgsSrvGetParameters_Response(Ros2Franka_spine_msgsSrvGetParameters_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Franka_spine_msgsSrvGetParameters_Request;using Response=Ros2Franka_spine_msgsSrvGetParameters_Response;
Ros2Franka_spine_msgsSrvGetParametersServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_respond(&respond_,ticket,&response).error;}
};
/** Complete custom franka_spine_msgs/srv/GetPosition client; default policy returns13 from construction. */
class Ros2Franka_spine_msgsSrvGetPositionClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Franka_spine_msgsSrvGetPosition_Request;using Response=Ros2Franka_spine_msgsSrvGetPosition_Response;
Ros2Franka_spine_msgsSrvGetPositionClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Franka_spine_msgsSrvGetPositionClient(Ros2Franka_spine_msgsSrvGetPositionClient const&)=delete;Ros2Franka_spine_msgsSrvGetPositionClient& operator=(Ros2Franka_spine_msgsSrvGetPositionClient const&)=delete;Ros2Franka_spine_msgsSrvGetPositionClient(Ros2Franka_spine_msgsSrvGetPositionClient&&)=delete;
~Ros2Franka_spine_msgsSrvGetPositionClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom franka_spine_msgs/srv/GetPosition server; advertise/respond stay separate authorities. */
class Ros2Franka_spine_msgsSrvGetPositionServer:public ServerOwners {
using Handler=std::function<Ros2Franka_spine_msgsSrvGetPosition_Response(Ros2Franka_spine_msgsSrvGetPosition_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Franka_spine_msgsSrvGetPosition_Request;using Response=Ros2Franka_spine_msgsSrvGetPosition_Response;
Ros2Franka_spine_msgsSrvGetPositionServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_respond(&respond_,ticket,&response).error;}
};
/** Complete custom franka_spine_msgs/srv/GetSpineState client; default policy returns13 from construction. */
class Ros2Franka_spine_msgsSrvGetSpineStateClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Franka_spine_msgsSrvGetSpineState_Request;using Response=Ros2Franka_spine_msgsSrvGetSpineState_Response;
Ros2Franka_spine_msgsSrvGetSpineStateClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Franka_spine_msgsSrvGetSpineStateClient(Ros2Franka_spine_msgsSrvGetSpineStateClient const&)=delete;Ros2Franka_spine_msgsSrvGetSpineStateClient& operator=(Ros2Franka_spine_msgsSrvGetSpineStateClient const&)=delete;Ros2Franka_spine_msgsSrvGetSpineStateClient(Ros2Franka_spine_msgsSrvGetSpineStateClient&&)=delete;
~Ros2Franka_spine_msgsSrvGetSpineStateClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom franka_spine_msgs/srv/GetSpineState server; advertise/respond stay separate authorities. */
class Ros2Franka_spine_msgsSrvGetSpineStateServer:public ServerOwners {
using Handler=std::function<Ros2Franka_spine_msgsSrvGetSpineState_Response(Ros2Franka_spine_msgsSrvGetSpineState_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Franka_spine_msgsSrvGetSpineState_Request;using Response=Ros2Franka_spine_msgsSrvGetSpineState_Response;
Ros2Franka_spine_msgsSrvGetSpineStateServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_respond(&respond_,ticket,&response).error;}
};
/** Complete custom franka_spine_msgs/srv/Halt client; default policy returns13 from construction. */
class Ros2Franka_spine_msgsSrvHaltClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Franka_spine_msgsSrvHalt_Request;using Response=Ros2Franka_spine_msgsSrvHalt_Response;
Ros2Franka_spine_msgsSrvHaltClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Franka_spine_msgsSrvHaltClient(Ros2Franka_spine_msgsSrvHaltClient const&)=delete;Ros2Franka_spine_msgsSrvHaltClient& operator=(Ros2Franka_spine_msgsSrvHaltClient const&)=delete;Ros2Franka_spine_msgsSrvHaltClient(Ros2Franka_spine_msgsSrvHaltClient&&)=delete;
~Ros2Franka_spine_msgsSrvHaltClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_franka_spine_msgs_srv_Halt_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom franka_spine_msgs/srv/Halt server; advertise/respond stay separate authorities. */
class Ros2Franka_spine_msgsSrvHaltServer:public ServerOwners {
using Handler=std::function<Ros2Franka_spine_msgsSrvHalt_Response(Ros2Franka_spine_msgsSrvHalt_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Franka_spine_msgsSrvHalt_Request;using Response=Ros2Franka_spine_msgsSrvHalt_Response;
Ros2Franka_spine_msgsSrvHaltServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_franka_spine_msgs_srv_Halt_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_franka_spine_msgs_srv_Halt_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_franka_spine_msgs_srv_Halt_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_franka_spine_msgs_srv_Halt_respond(&respond_,ticket,&response).error;}
};
/** Complete custom franka_spine_msgs/srv/SwitchOff client; default policy returns13 from construction. */
class Ros2Franka_spine_msgsSrvSwitchOffClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Franka_spine_msgsSrvSwitchOff_Request;using Response=Ros2Franka_spine_msgsSrvSwitchOff_Response;
Ros2Franka_spine_msgsSrvSwitchOffClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Franka_spine_msgsSrvSwitchOffClient(Ros2Franka_spine_msgsSrvSwitchOffClient const&)=delete;Ros2Franka_spine_msgsSrvSwitchOffClient& operator=(Ros2Franka_spine_msgsSrvSwitchOffClient const&)=delete;Ros2Franka_spine_msgsSrvSwitchOffClient(Ros2Franka_spine_msgsSrvSwitchOffClient&&)=delete;
~Ros2Franka_spine_msgsSrvSwitchOffClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom franka_spine_msgs/srv/SwitchOff server; advertise/respond stay separate authorities. */
class Ros2Franka_spine_msgsSrvSwitchOffServer:public ServerOwners {
using Handler=std::function<Ros2Franka_spine_msgsSrvSwitchOff_Response(Ros2Franka_spine_msgsSrvSwitchOff_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Franka_spine_msgsSrvSwitchOff_Request;using Response=Ros2Franka_spine_msgsSrvSwitchOff_Response;
Ros2Franka_spine_msgsSrvSwitchOffServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_respond(&respond_,ticket,&response).error;}
};
/** Complete custom franka_spine_msgs/srv/SwitchOn client; default policy returns13 from construction. */
class Ros2Franka_spine_msgsSrvSwitchOnClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Franka_spine_msgsSrvSwitchOn_Request;using Response=Ros2Franka_spine_msgsSrvSwitchOn_Response;
Ros2Franka_spine_msgsSrvSwitchOnClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Franka_spine_msgsSrvSwitchOnClient(Ros2Franka_spine_msgsSrvSwitchOnClient const&)=delete;Ros2Franka_spine_msgsSrvSwitchOnClient& operator=(Ros2Franka_spine_msgsSrvSwitchOnClient const&)=delete;Ros2Franka_spine_msgsSrvSwitchOnClient(Ros2Franka_spine_msgsSrvSwitchOnClient&&)=delete;
~Ros2Franka_spine_msgsSrvSwitchOnClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom franka_spine_msgs/srv/SwitchOn server; advertise/respond stay separate authorities. */
class Ros2Franka_spine_msgsSrvSwitchOnServer:public ServerOwners {
using Handler=std::function<Ros2Franka_spine_msgsSrvSwitchOn_Response(Ros2Franka_spine_msgsSrvSwitchOn_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Franka_spine_msgsSrvSwitchOn_Request;using Response=Ros2Franka_spine_msgsSrvSwitchOn_Response;
Ros2Franka_spine_msgsSrvSwitchOnServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_respond(&respond_,ticket,&response).error;}
};
}
