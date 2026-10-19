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
/** Complete owned custom franka_gripper/Grasp Action client with four separate roles. */
class Ros1Franka_gripperGraspClient {roswasmc_context context_;std::array<uint64_t,1024> goals_{};roswasmc_action_bundle bundle_{};
public:
using Goal=Ros1Franka_gripperGraspGoal;using Result=Ros1Franka_gripperGraspResult;using Feedback=Ros1Franka_gripperGraspFeedback;
Ros1Franka_gripperGraspClient(roswasmc_context const& context,roswasmc_action_aliases const& aliases,size_t slots=8):context_{context.backend} {roswasmc_action_bundle_init(&context_,&bundle_,&aliases,goals_.data(),slots);}
Ros1Franka_gripperGraspClient(Ros1Franka_gripperGraspClient const&)=delete;Ros1Franka_gripperGraspClient& operator=(Ros1Franka_gripperGraspClient const&)=delete;Ros1Franka_gripperGraspClient(Ros1Franka_gripperGraspClient&&)=delete;
~Ros1Franka_gripperGraspClient(){if(bundle_.active){std::array<uint32_t,5> statuses{};roswasmc_action_bundle_close_checked(&bundle_,0,statuses.data(),statuses.size());}}
uint32_t error() const noexcept{return bundle_.error?bundle_.error:bundle_.active?0:9;}bool is_open() const noexcept{return bundle_.active;}bool owns(uint64_t id)const{return roswasmc_action_bundle_owns(&bundle_,id);}
roswasmc_result send_goal(Goal const& value,uint64_t& id){return roswasmc_bounded_1_franka_gripper_Grasp_send(&bundle_,&value,&id);}
roswasmc_result feedback(uint64_t id,Feedback& value){return roswasmc_bounded_1_franka_gripper_Grasp_feedback(&bundle_,id,&value);}
roswasmc_result result(uint64_t id,uint8_t& kind,Result& value){return roswasmc_bounded_1_franka_gripper_Grasp_result(&bundle_,id,&kind,&value);}
roswasmc_result cancel_own(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_own(&bundle_,id,&accepted);}
roswasmc_result cancel_any(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_any(&bundle_,id,&accepted);}
roswasmc_result cancel_all(int& accepted){return roswasmc_action_bundle_cancel_all(&bundle_,&accepted);}
roswasmc_cleanup_report close_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_close_checked(&bundle_,primary,statuses.data(),statuses.size());}
roswasmc_cleanup_report revoke_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_revoke_checked(&bundle_,primary,statuses.data(),statuses.size());}
};
/** Typed custom franka_gripper/Grasp server with explicit feedback/terminal component codecs. */
class Ros1Franka_gripperGraspServer:public ServerOwners {
public:
using Goal=Ros1Franka_gripperGraspGoal;using Result=Ros1Franka_gripperGraspResult;using Feedback=Ros1Franka_gripperGraspFeedback;
/** Handler may choose separate feedback/result effects; kind must be2..4 when terminal. */
struct Replies {Feedback feedback{};Result result{};uint8_t kind{2};bool send_feedback{true},send_result{true};};
private:using Handler=std::function<Replies(uint64_t,Goal const&)>;std::shared_ptr<Handler> handler_;
public:
Ros1Franka_gripperGraspServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,true} {}
void on_goal(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& id,Goal& goal){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_franka_gripper_Grasp_poll(&advertise_,&id,&goal);}
roswasmc_result respond_feedback(uint64_t id,Feedback const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_franka_gripper_Grasp_respond_feedback(&respond_,id,&value);}
roswasmc_result respond_result(uint64_t id,uint8_t kind,Result const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_franka_gripper_Grasp_respond_result(&respond_,id,kind,&value);}
/** One pass, at most one feedback plus one terminal response; never implicit replay. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t id{};Goal goal{};auto r=roswasmc_bounded_1_franka_gripper_Grasp_poll(&advertise_,&id,&goal);if(r.error)return r.error;auto replies=(*handler)(id,goal);if(replies.send_result&&(replies.kind<2||replies.kind>4||!roswasmc_bounded_1_franka_gripper_GraspResult_valid(&replies.result)))return 22;if(replies.send_feedback&&!roswasmc_bounded_1_franka_gripper_GraspFeedback_valid(&replies.feedback))return 22;if(replies.send_feedback){r=roswasmc_bounded_1_franka_gripper_Grasp_respond_feedback(&respond_,id,&replies.feedback);if(r.error)return r.error;}return replies.send_result?roswasmc_bounded_1_franka_gripper_Grasp_respond_result(&respond_,id,replies.kind,&replies.result).error:0;}
};
/** Complete owned custom franka_gripper/Homing Action client with four separate roles. */
class Ros1Franka_gripperHomingClient {roswasmc_context context_;std::array<uint64_t,1024> goals_{};roswasmc_action_bundle bundle_{};
public:
using Goal=Ros1Franka_gripperHomingGoal;using Result=Ros1Franka_gripperHomingResult;using Feedback=Ros1Franka_gripperHomingFeedback;
Ros1Franka_gripperHomingClient(roswasmc_context const& context,roswasmc_action_aliases const& aliases,size_t slots=8):context_{context.backend} {roswasmc_action_bundle_init(&context_,&bundle_,&aliases,goals_.data(),slots);}
Ros1Franka_gripperHomingClient(Ros1Franka_gripperHomingClient const&)=delete;Ros1Franka_gripperHomingClient& operator=(Ros1Franka_gripperHomingClient const&)=delete;Ros1Franka_gripperHomingClient(Ros1Franka_gripperHomingClient&&)=delete;
~Ros1Franka_gripperHomingClient(){if(bundle_.active){std::array<uint32_t,5> statuses{};roswasmc_action_bundle_close_checked(&bundle_,0,statuses.data(),statuses.size());}}
uint32_t error() const noexcept{return bundle_.error?bundle_.error:bundle_.active?0:9;}bool is_open() const noexcept{return bundle_.active;}bool owns(uint64_t id)const{return roswasmc_action_bundle_owns(&bundle_,id);}
roswasmc_result send_goal(Goal const& value,uint64_t& id){return roswasmc_bounded_1_franka_gripper_Homing_send(&bundle_,&value,&id);}
roswasmc_result feedback(uint64_t id,Feedback& value){return roswasmc_bounded_1_franka_gripper_Homing_feedback(&bundle_,id,&value);}
roswasmc_result result(uint64_t id,uint8_t& kind,Result& value){return roswasmc_bounded_1_franka_gripper_Homing_result(&bundle_,id,&kind,&value);}
roswasmc_result cancel_own(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_own(&bundle_,id,&accepted);}
roswasmc_result cancel_any(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_any(&bundle_,id,&accepted);}
roswasmc_result cancel_all(int& accepted){return roswasmc_action_bundle_cancel_all(&bundle_,&accepted);}
roswasmc_cleanup_report close_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_close_checked(&bundle_,primary,statuses.data(),statuses.size());}
roswasmc_cleanup_report revoke_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_revoke_checked(&bundle_,primary,statuses.data(),statuses.size());}
};
/** Typed custom franka_gripper/Homing server with explicit feedback/terminal component codecs. */
class Ros1Franka_gripperHomingServer:public ServerOwners {
public:
using Goal=Ros1Franka_gripperHomingGoal;using Result=Ros1Franka_gripperHomingResult;using Feedback=Ros1Franka_gripperHomingFeedback;
/** Handler may choose separate feedback/result effects; kind must be2..4 when terminal. */
struct Replies {Feedback feedback{};Result result{};uint8_t kind{2};bool send_feedback{true},send_result{true};};
private:using Handler=std::function<Replies(uint64_t,Goal const&)>;std::shared_ptr<Handler> handler_;
public:
Ros1Franka_gripperHomingServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,true} {}
void on_goal(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& id,Goal& goal){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_franka_gripper_Homing_poll(&advertise_,&id,&goal);}
roswasmc_result respond_feedback(uint64_t id,Feedback const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_franka_gripper_Homing_respond_feedback(&respond_,id,&value);}
roswasmc_result respond_result(uint64_t id,uint8_t kind,Result const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_franka_gripper_Homing_respond_result(&respond_,id,kind,&value);}
/** One pass, at most one feedback plus one terminal response; never implicit replay. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t id{};Goal goal{};auto r=roswasmc_bounded_1_franka_gripper_Homing_poll(&advertise_,&id,&goal);if(r.error)return r.error;auto replies=(*handler)(id,goal);if(replies.send_result&&(replies.kind<2||replies.kind>4||!roswasmc_bounded_1_franka_gripper_HomingResult_valid(&replies.result)))return 22;if(replies.send_feedback&&!roswasmc_bounded_1_franka_gripper_HomingFeedback_valid(&replies.feedback))return 22;if(replies.send_feedback){r=roswasmc_bounded_1_franka_gripper_Homing_respond_feedback(&respond_,id,&replies.feedback);if(r.error)return r.error;}return replies.send_result?roswasmc_bounded_1_franka_gripper_Homing_respond_result(&respond_,id,replies.kind,&replies.result).error:0;}
};
/** Complete owned custom franka_gripper/Move Action client with four separate roles. */
class Ros1Franka_gripperMoveClient {roswasmc_context context_;std::array<uint64_t,1024> goals_{};roswasmc_action_bundle bundle_{};
public:
using Goal=Ros1Franka_gripperMoveGoal;using Result=Ros1Franka_gripperMoveResult;using Feedback=Ros1Franka_gripperMoveFeedback;
Ros1Franka_gripperMoveClient(roswasmc_context const& context,roswasmc_action_aliases const& aliases,size_t slots=8):context_{context.backend} {roswasmc_action_bundle_init(&context_,&bundle_,&aliases,goals_.data(),slots);}
Ros1Franka_gripperMoveClient(Ros1Franka_gripperMoveClient const&)=delete;Ros1Franka_gripperMoveClient& operator=(Ros1Franka_gripperMoveClient const&)=delete;Ros1Franka_gripperMoveClient(Ros1Franka_gripperMoveClient&&)=delete;
~Ros1Franka_gripperMoveClient(){if(bundle_.active){std::array<uint32_t,5> statuses{};roswasmc_action_bundle_close_checked(&bundle_,0,statuses.data(),statuses.size());}}
uint32_t error() const noexcept{return bundle_.error?bundle_.error:bundle_.active?0:9;}bool is_open() const noexcept{return bundle_.active;}bool owns(uint64_t id)const{return roswasmc_action_bundle_owns(&bundle_,id);}
roswasmc_result send_goal(Goal const& value,uint64_t& id){return roswasmc_bounded_1_franka_gripper_Move_send(&bundle_,&value,&id);}
roswasmc_result feedback(uint64_t id,Feedback& value){return roswasmc_bounded_1_franka_gripper_Move_feedback(&bundle_,id,&value);}
roswasmc_result result(uint64_t id,uint8_t& kind,Result& value){return roswasmc_bounded_1_franka_gripper_Move_result(&bundle_,id,&kind,&value);}
roswasmc_result cancel_own(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_own(&bundle_,id,&accepted);}
roswasmc_result cancel_any(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_any(&bundle_,id,&accepted);}
roswasmc_result cancel_all(int& accepted){return roswasmc_action_bundle_cancel_all(&bundle_,&accepted);}
roswasmc_cleanup_report close_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_close_checked(&bundle_,primary,statuses.data(),statuses.size());}
roswasmc_cleanup_report revoke_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_revoke_checked(&bundle_,primary,statuses.data(),statuses.size());}
};
/** Typed custom franka_gripper/Move server with explicit feedback/terminal component codecs. */
class Ros1Franka_gripperMoveServer:public ServerOwners {
public:
using Goal=Ros1Franka_gripperMoveGoal;using Result=Ros1Franka_gripperMoveResult;using Feedback=Ros1Franka_gripperMoveFeedback;
/** Handler may choose separate feedback/result effects; kind must be2..4 when terminal. */
struct Replies {Feedback feedback{};Result result{};uint8_t kind{2};bool send_feedback{true},send_result{true};};
private:using Handler=std::function<Replies(uint64_t,Goal const&)>;std::shared_ptr<Handler> handler_;
public:
Ros1Franka_gripperMoveServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,true} {}
void on_goal(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& id,Goal& goal){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_franka_gripper_Move_poll(&advertise_,&id,&goal);}
roswasmc_result respond_feedback(uint64_t id,Feedback const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_franka_gripper_Move_respond_feedback(&respond_,id,&value);}
roswasmc_result respond_result(uint64_t id,uint8_t kind,Result const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_franka_gripper_Move_respond_result(&respond_,id,kind,&value);}
/** One pass, at most one feedback plus one terminal response; never implicit replay. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t id{};Goal goal{};auto r=roswasmc_bounded_1_franka_gripper_Move_poll(&advertise_,&id,&goal);if(r.error)return r.error;auto replies=(*handler)(id,goal);if(replies.send_result&&(replies.kind<2||replies.kind>4||!roswasmc_bounded_1_franka_gripper_MoveResult_valid(&replies.result)))return 22;if(replies.send_feedback&&!roswasmc_bounded_1_franka_gripper_MoveFeedback_valid(&replies.feedback))return 22;if(replies.send_feedback){r=roswasmc_bounded_1_franka_gripper_Move_respond_feedback(&respond_,id,&replies.feedback);if(r.error)return r.error;}return replies.send_result?roswasmc_bounded_1_franka_gripper_Move_respond_result(&respond_,id,replies.kind,&replies.result).error:0;}
};
/** Complete owned custom franka_gripper/Stop Action client with four separate roles. */
class Ros1Franka_gripperStopClient {roswasmc_context context_;std::array<uint64_t,1024> goals_{};roswasmc_action_bundle bundle_{};
public:
using Goal=Ros1Franka_gripperStopGoal;using Result=Ros1Franka_gripperStopResult;using Feedback=Ros1Franka_gripperStopFeedback;
Ros1Franka_gripperStopClient(roswasmc_context const& context,roswasmc_action_aliases const& aliases,size_t slots=8):context_{context.backend} {roswasmc_action_bundle_init(&context_,&bundle_,&aliases,goals_.data(),slots);}
Ros1Franka_gripperStopClient(Ros1Franka_gripperStopClient const&)=delete;Ros1Franka_gripperStopClient& operator=(Ros1Franka_gripperStopClient const&)=delete;Ros1Franka_gripperStopClient(Ros1Franka_gripperStopClient&&)=delete;
~Ros1Franka_gripperStopClient(){if(bundle_.active){std::array<uint32_t,5> statuses{};roswasmc_action_bundle_close_checked(&bundle_,0,statuses.data(),statuses.size());}}
uint32_t error() const noexcept{return bundle_.error?bundle_.error:bundle_.active?0:9;}bool is_open() const noexcept{return bundle_.active;}bool owns(uint64_t id)const{return roswasmc_action_bundle_owns(&bundle_,id);}
roswasmc_result send_goal(Goal const& value,uint64_t& id){return roswasmc_bounded_1_franka_gripper_Stop_send(&bundle_,&value,&id);}
roswasmc_result feedback(uint64_t id,Feedback& value){return roswasmc_bounded_1_franka_gripper_Stop_feedback(&bundle_,id,&value);}
roswasmc_result result(uint64_t id,uint8_t& kind,Result& value){return roswasmc_bounded_1_franka_gripper_Stop_result(&bundle_,id,&kind,&value);}
roswasmc_result cancel_own(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_own(&bundle_,id,&accepted);}
roswasmc_result cancel_any(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_any(&bundle_,id,&accepted);}
roswasmc_result cancel_all(int& accepted){return roswasmc_action_bundle_cancel_all(&bundle_,&accepted);}
roswasmc_cleanup_report close_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_close_checked(&bundle_,primary,statuses.data(),statuses.size());}
roswasmc_cleanup_report revoke_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_revoke_checked(&bundle_,primary,statuses.data(),statuses.size());}
};
/** Typed custom franka_gripper/Stop server with explicit feedback/terminal component codecs. */
class Ros1Franka_gripperStopServer:public ServerOwners {
public:
using Goal=Ros1Franka_gripperStopGoal;using Result=Ros1Franka_gripperStopResult;using Feedback=Ros1Franka_gripperStopFeedback;
/** Handler may choose separate feedback/result effects; kind must be2..4 when terminal. */
struct Replies {Feedback feedback{};Result result{};uint8_t kind{2};bool send_feedback{true},send_result{true};};
private:using Handler=std::function<Replies(uint64_t,Goal const&)>;std::shared_ptr<Handler> handler_;
public:
Ros1Franka_gripperStopServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,true} {}
void on_goal(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& id,Goal& goal){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_franka_gripper_Stop_poll(&advertise_,&id,&goal);}
roswasmc_result respond_feedback(uint64_t id,Feedback const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_franka_gripper_Stop_respond_feedback(&respond_,id,&value);}
roswasmc_result respond_result(uint64_t id,uint8_t kind,Result const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_franka_gripper_Stop_respond_result(&respond_,id,kind,&value);}
/** One pass, at most one feedback plus one terminal response; never implicit replay. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t id{};Goal goal{};auto r=roswasmc_bounded_1_franka_gripper_Stop_poll(&advertise_,&id,&goal);if(r.error)return r.error;auto replies=(*handler)(id,goal);if(replies.send_result&&(replies.kind<2||replies.kind>4||!roswasmc_bounded_1_franka_gripper_StopResult_valid(&replies.result)))return 22;if(replies.send_feedback&&!roswasmc_bounded_1_franka_gripper_StopFeedback_valid(&replies.feedback))return 22;if(replies.send_feedback){r=roswasmc_bounded_1_franka_gripper_Stop_respond_feedback(&respond_,id,&replies.feedback);if(r.error)return r.error;}return replies.send_result?roswasmc_bounded_1_franka_gripper_Stop_respond_result(&respond_,id,replies.kind,&replies.result).error:0;}
};
/** Complete owned custom franka_msgs/ErrorRecovery Action client with four separate roles. */
class Ros1Franka_msgsErrorRecoveryClient {roswasmc_context context_;std::array<uint64_t,1024> goals_{};roswasmc_action_bundle bundle_{};
public:
using Goal=Ros1Franka_msgsErrorRecoveryGoal;using Result=Ros1Franka_msgsErrorRecoveryResult;using Feedback=Ros1Franka_msgsErrorRecoveryFeedback;
Ros1Franka_msgsErrorRecoveryClient(roswasmc_context const& context,roswasmc_action_aliases const& aliases,size_t slots=8):context_{context.backend} {roswasmc_action_bundle_init(&context_,&bundle_,&aliases,goals_.data(),slots);}
Ros1Franka_msgsErrorRecoveryClient(Ros1Franka_msgsErrorRecoveryClient const&)=delete;Ros1Franka_msgsErrorRecoveryClient& operator=(Ros1Franka_msgsErrorRecoveryClient const&)=delete;Ros1Franka_msgsErrorRecoveryClient(Ros1Franka_msgsErrorRecoveryClient&&)=delete;
~Ros1Franka_msgsErrorRecoveryClient(){if(bundle_.active){std::array<uint32_t,5> statuses{};roswasmc_action_bundle_close_checked(&bundle_,0,statuses.data(),statuses.size());}}
uint32_t error() const noexcept{return bundle_.error?bundle_.error:bundle_.active?0:9;}bool is_open() const noexcept{return bundle_.active;}bool owns(uint64_t id)const{return roswasmc_action_bundle_owns(&bundle_,id);}
roswasmc_result send_goal(Goal const& value,uint64_t& id){return roswasmc_bounded_1_franka_msgs_ErrorRecovery_send(&bundle_,&value,&id);}
roswasmc_result feedback(uint64_t id,Feedback& value){return roswasmc_bounded_1_franka_msgs_ErrorRecovery_feedback(&bundle_,id,&value);}
roswasmc_result result(uint64_t id,uint8_t& kind,Result& value){return roswasmc_bounded_1_franka_msgs_ErrorRecovery_result(&bundle_,id,&kind,&value);}
roswasmc_result cancel_own(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_own(&bundle_,id,&accepted);}
roswasmc_result cancel_any(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_any(&bundle_,id,&accepted);}
roswasmc_result cancel_all(int& accepted){return roswasmc_action_bundle_cancel_all(&bundle_,&accepted);}
roswasmc_cleanup_report close_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_close_checked(&bundle_,primary,statuses.data(),statuses.size());}
roswasmc_cleanup_report revoke_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_revoke_checked(&bundle_,primary,statuses.data(),statuses.size());}
};
/** Typed custom franka_msgs/ErrorRecovery server with explicit feedback/terminal component codecs. */
class Ros1Franka_msgsErrorRecoveryServer:public ServerOwners {
public:
using Goal=Ros1Franka_msgsErrorRecoveryGoal;using Result=Ros1Franka_msgsErrorRecoveryResult;using Feedback=Ros1Franka_msgsErrorRecoveryFeedback;
/** Handler may choose separate feedback/result effects; kind must be2..4 when terminal. */
struct Replies {Feedback feedback{};Result result{};uint8_t kind{2};bool send_feedback{true},send_result{true};};
private:using Handler=std::function<Replies(uint64_t,Goal const&)>;std::shared_ptr<Handler> handler_;
public:
Ros1Franka_msgsErrorRecoveryServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,true} {}
void on_goal(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& id,Goal& goal){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_franka_msgs_ErrorRecovery_poll(&advertise_,&id,&goal);}
roswasmc_result respond_feedback(uint64_t id,Feedback const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_franka_msgs_ErrorRecovery_respond_feedback(&respond_,id,&value);}
roswasmc_result respond_result(uint64_t id,uint8_t kind,Result const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_franka_msgs_ErrorRecovery_respond_result(&respond_,id,kind,&value);}
/** One pass, at most one feedback plus one terminal response; never implicit replay. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t id{};Goal goal{};auto r=roswasmc_bounded_1_franka_msgs_ErrorRecovery_poll(&advertise_,&id,&goal);if(r.error)return r.error;auto replies=(*handler)(id,goal);if(replies.send_result&&(replies.kind<2||replies.kind>4||!roswasmc_bounded_1_franka_msgs_ErrorRecoveryResult_valid(&replies.result)))return 22;if(replies.send_feedback&&!roswasmc_bounded_1_franka_msgs_ErrorRecoveryFeedback_valid(&replies.feedback))return 22;if(replies.send_feedback){r=roswasmc_bounded_1_franka_msgs_ErrorRecovery_respond_feedback(&respond_,id,&replies.feedback);if(r.error)return r.error;}return replies.send_result?roswasmc_bounded_1_franka_msgs_ErrorRecovery_respond_result(&respond_,id,replies.kind,&replies.result).error:0;}
};
/** Complete custom franka_msgs/SetCartesianImpedance client; default policy returns13 from construction. */
class Ros1Franka_msgsSetCartesianImpedanceClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Franka_msgsSetCartesianImpedanceRequest;using Response=Ros1Franka_msgsSetCartesianImpedanceResponse;
Ros1Franka_msgsSetCartesianImpedanceClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Franka_msgsSetCartesianImpedanceClient(Ros1Franka_msgsSetCartesianImpedanceClient const&)=delete;Ros1Franka_msgsSetCartesianImpedanceClient& operator=(Ros1Franka_msgsSetCartesianImpedanceClient const&)=delete;Ros1Franka_msgsSetCartesianImpedanceClient(Ros1Franka_msgsSetCartesianImpedanceClient&&)=delete;
~Ros1Franka_msgsSetCartesianImpedanceClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_franka_msgs_SetCartesianImpedance_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom franka_msgs/SetCartesianImpedance server; advertise/respond stay separate authorities. */
class Ros1Franka_msgsSetCartesianImpedanceServer:public ServerOwners {
using Handler=std::function<Ros1Franka_msgsSetCartesianImpedanceResponse(Ros1Franka_msgsSetCartesianImpedanceRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Franka_msgsSetCartesianImpedanceRequest;using Response=Ros1Franka_msgsSetCartesianImpedanceResponse;
Ros1Franka_msgsSetCartesianImpedanceServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_franka_msgs_SetCartesianImpedance_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_franka_msgs_SetCartesianImpedance_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_franka_msgs_SetCartesianImpedance_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_franka_msgs_SetCartesianImpedance_respond(&respond_,ticket,&response).error;}
};
/** Complete custom franka_msgs/SetEEFrame client; default policy returns13 from construction. */
class Ros1Franka_msgsSetEEFrameClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Franka_msgsSetEEFrameRequest;using Response=Ros1Franka_msgsSetEEFrameResponse;
Ros1Franka_msgsSetEEFrameClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Franka_msgsSetEEFrameClient(Ros1Franka_msgsSetEEFrameClient const&)=delete;Ros1Franka_msgsSetEEFrameClient& operator=(Ros1Franka_msgsSetEEFrameClient const&)=delete;Ros1Franka_msgsSetEEFrameClient(Ros1Franka_msgsSetEEFrameClient&&)=delete;
~Ros1Franka_msgsSetEEFrameClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_franka_msgs_SetEEFrame_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom franka_msgs/SetEEFrame server; advertise/respond stay separate authorities. */
class Ros1Franka_msgsSetEEFrameServer:public ServerOwners {
using Handler=std::function<Ros1Franka_msgsSetEEFrameResponse(Ros1Franka_msgsSetEEFrameRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Franka_msgsSetEEFrameRequest;using Response=Ros1Franka_msgsSetEEFrameResponse;
Ros1Franka_msgsSetEEFrameServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_franka_msgs_SetEEFrame_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_franka_msgs_SetEEFrame_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_franka_msgs_SetEEFrame_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_franka_msgs_SetEEFrame_respond(&respond_,ticket,&response).error;}
};
/** Complete custom franka_msgs/SetForceTorqueCollisionBehavior client; default policy returns13 from construction. */
class Ros1Franka_msgsSetForceTorqueCollisionBehaviorClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Franka_msgsSetForceTorqueCollisionBehaviorRequest;using Response=Ros1Franka_msgsSetForceTorqueCollisionBehaviorResponse;
Ros1Franka_msgsSetForceTorqueCollisionBehaviorClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Franka_msgsSetForceTorqueCollisionBehaviorClient(Ros1Franka_msgsSetForceTorqueCollisionBehaviorClient const&)=delete;Ros1Franka_msgsSetForceTorqueCollisionBehaviorClient& operator=(Ros1Franka_msgsSetForceTorqueCollisionBehaviorClient const&)=delete;Ros1Franka_msgsSetForceTorqueCollisionBehaviorClient(Ros1Franka_msgsSetForceTorqueCollisionBehaviorClient&&)=delete;
~Ros1Franka_msgsSetForceTorqueCollisionBehaviorClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehavior_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom franka_msgs/SetForceTorqueCollisionBehavior server; advertise/respond stay separate authorities. */
class Ros1Franka_msgsSetForceTorqueCollisionBehaviorServer:public ServerOwners {
using Handler=std::function<Ros1Franka_msgsSetForceTorqueCollisionBehaviorResponse(Ros1Franka_msgsSetForceTorqueCollisionBehaviorRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Franka_msgsSetForceTorqueCollisionBehaviorRequest;using Response=Ros1Franka_msgsSetForceTorqueCollisionBehaviorResponse;
Ros1Franka_msgsSetForceTorqueCollisionBehaviorServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehavior_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehavior_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehavior_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehavior_respond(&respond_,ticket,&response).error;}
};
/** Complete custom franka_msgs/SetFullCollisionBehavior client; default policy returns13 from construction. */
class Ros1Franka_msgsSetFullCollisionBehaviorClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Franka_msgsSetFullCollisionBehaviorRequest;using Response=Ros1Franka_msgsSetFullCollisionBehaviorResponse;
Ros1Franka_msgsSetFullCollisionBehaviorClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Franka_msgsSetFullCollisionBehaviorClient(Ros1Franka_msgsSetFullCollisionBehaviorClient const&)=delete;Ros1Franka_msgsSetFullCollisionBehaviorClient& operator=(Ros1Franka_msgsSetFullCollisionBehaviorClient const&)=delete;Ros1Franka_msgsSetFullCollisionBehaviorClient(Ros1Franka_msgsSetFullCollisionBehaviorClient&&)=delete;
~Ros1Franka_msgsSetFullCollisionBehaviorClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_franka_msgs_SetFullCollisionBehavior_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom franka_msgs/SetFullCollisionBehavior server; advertise/respond stay separate authorities. */
class Ros1Franka_msgsSetFullCollisionBehaviorServer:public ServerOwners {
using Handler=std::function<Ros1Franka_msgsSetFullCollisionBehaviorResponse(Ros1Franka_msgsSetFullCollisionBehaviorRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Franka_msgsSetFullCollisionBehaviorRequest;using Response=Ros1Franka_msgsSetFullCollisionBehaviorResponse;
Ros1Franka_msgsSetFullCollisionBehaviorServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_franka_msgs_SetFullCollisionBehavior_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_franka_msgs_SetFullCollisionBehavior_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_franka_msgs_SetFullCollisionBehavior_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_franka_msgs_SetFullCollisionBehavior_respond(&respond_,ticket,&response).error;}
};
/** Complete custom franka_msgs/SetJointImpedance client; default policy returns13 from construction. */
class Ros1Franka_msgsSetJointImpedanceClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Franka_msgsSetJointImpedanceRequest;using Response=Ros1Franka_msgsSetJointImpedanceResponse;
Ros1Franka_msgsSetJointImpedanceClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Franka_msgsSetJointImpedanceClient(Ros1Franka_msgsSetJointImpedanceClient const&)=delete;Ros1Franka_msgsSetJointImpedanceClient& operator=(Ros1Franka_msgsSetJointImpedanceClient const&)=delete;Ros1Franka_msgsSetJointImpedanceClient(Ros1Franka_msgsSetJointImpedanceClient&&)=delete;
~Ros1Franka_msgsSetJointImpedanceClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_franka_msgs_SetJointImpedance_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom franka_msgs/SetJointImpedance server; advertise/respond stay separate authorities. */
class Ros1Franka_msgsSetJointImpedanceServer:public ServerOwners {
using Handler=std::function<Ros1Franka_msgsSetJointImpedanceResponse(Ros1Franka_msgsSetJointImpedanceRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Franka_msgsSetJointImpedanceRequest;using Response=Ros1Franka_msgsSetJointImpedanceResponse;
Ros1Franka_msgsSetJointImpedanceServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_franka_msgs_SetJointImpedance_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_franka_msgs_SetJointImpedance_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_franka_msgs_SetJointImpedance_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_franka_msgs_SetJointImpedance_respond(&respond_,ticket,&response).error;}
};
/** Complete custom franka_msgs/SetKFrame client; default policy returns13 from construction. */
class Ros1Franka_msgsSetKFrameClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Franka_msgsSetKFrameRequest;using Response=Ros1Franka_msgsSetKFrameResponse;
Ros1Franka_msgsSetKFrameClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Franka_msgsSetKFrameClient(Ros1Franka_msgsSetKFrameClient const&)=delete;Ros1Franka_msgsSetKFrameClient& operator=(Ros1Franka_msgsSetKFrameClient const&)=delete;Ros1Franka_msgsSetKFrameClient(Ros1Franka_msgsSetKFrameClient&&)=delete;
~Ros1Franka_msgsSetKFrameClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_franka_msgs_SetKFrame_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom franka_msgs/SetKFrame server; advertise/respond stay separate authorities. */
class Ros1Franka_msgsSetKFrameServer:public ServerOwners {
using Handler=std::function<Ros1Franka_msgsSetKFrameResponse(Ros1Franka_msgsSetKFrameRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Franka_msgsSetKFrameRequest;using Response=Ros1Franka_msgsSetKFrameResponse;
Ros1Franka_msgsSetKFrameServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_franka_msgs_SetKFrame_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_franka_msgs_SetKFrame_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_franka_msgs_SetKFrame_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_franka_msgs_SetKFrame_respond(&respond_,ticket,&response).error;}
};
/** Complete custom franka_msgs/SetLoad client; default policy returns13 from construction. */
class Ros1Franka_msgsSetLoadClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Franka_msgsSetLoadRequest;using Response=Ros1Franka_msgsSetLoadResponse;
Ros1Franka_msgsSetLoadClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Franka_msgsSetLoadClient(Ros1Franka_msgsSetLoadClient const&)=delete;Ros1Franka_msgsSetLoadClient& operator=(Ros1Franka_msgsSetLoadClient const&)=delete;Ros1Franka_msgsSetLoadClient(Ros1Franka_msgsSetLoadClient&&)=delete;
~Ros1Franka_msgsSetLoadClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_franka_msgs_SetLoad_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom franka_msgs/SetLoad server; advertise/respond stay separate authorities. */
class Ros1Franka_msgsSetLoadServer:public ServerOwners {
using Handler=std::function<Ros1Franka_msgsSetLoadResponse(Ros1Franka_msgsSetLoadRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Franka_msgsSetLoadRequest;using Response=Ros1Franka_msgsSetLoadResponse;
Ros1Franka_msgsSetLoadServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_franka_msgs_SetLoad_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_franka_msgs_SetLoad_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_franka_msgs_SetLoad_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_franka_msgs_SetLoad_respond(&respond_,ticket,&response).error;}
};
}
