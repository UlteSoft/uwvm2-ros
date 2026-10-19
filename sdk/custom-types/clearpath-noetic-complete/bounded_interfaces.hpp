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
/** Complete owned custom clearpath_camera_msgs/InspectPOI Action client with four separate roles. */
class Ros1Clearpath_camera_msgsInspectPOIClient {roswasmc_context context_;std::array<uint64_t,1024> goals_{};roswasmc_action_bundle bundle_{};
public:
using Goal=Ros1Clearpath_camera_msgsInspectPOIGoal;using Result=Ros1Clearpath_camera_msgsInspectPOIResult;using Feedback=Ros1Clearpath_camera_msgsInspectPOIFeedback;
Ros1Clearpath_camera_msgsInspectPOIClient(roswasmc_context const& context,roswasmc_action_aliases const& aliases,size_t slots=8):context_{context.backend} {roswasmc_action_bundle_init(&context_,&bundle_,&aliases,goals_.data(),slots);}
Ros1Clearpath_camera_msgsInspectPOIClient(Ros1Clearpath_camera_msgsInspectPOIClient const&)=delete;Ros1Clearpath_camera_msgsInspectPOIClient& operator=(Ros1Clearpath_camera_msgsInspectPOIClient const&)=delete;Ros1Clearpath_camera_msgsInspectPOIClient(Ros1Clearpath_camera_msgsInspectPOIClient&&)=delete;
~Ros1Clearpath_camera_msgsInspectPOIClient(){if(bundle_.active){std::array<uint32_t,5> statuses{};roswasmc_action_bundle_close_checked(&bundle_,0,statuses.data(),statuses.size());}}
uint32_t error() const noexcept{return bundle_.error?bundle_.error:bundle_.active?0:9;}bool is_open() const noexcept{return bundle_.active;}bool owns(uint64_t id)const{return roswasmc_action_bundle_owns(&bundle_,id);}
roswasmc_result send_goal(Goal const& value,uint64_t& id){return roswasmc_bounded_1_clearpath_camera_msgs_InspectPOI_send(&bundle_,&value,&id);}
roswasmc_result feedback(uint64_t id,Feedback& value){return roswasmc_bounded_1_clearpath_camera_msgs_InspectPOI_feedback(&bundle_,id,&value);}
roswasmc_result result(uint64_t id,uint8_t& kind,Result& value){return roswasmc_bounded_1_clearpath_camera_msgs_InspectPOI_result(&bundle_,id,&kind,&value);}
roswasmc_result cancel_own(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_own(&bundle_,id,&accepted);}
roswasmc_result cancel_any(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_any(&bundle_,id,&accepted);}
roswasmc_result cancel_all(int& accepted){return roswasmc_action_bundle_cancel_all(&bundle_,&accepted);}
roswasmc_cleanup_report close_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_close_checked(&bundle_,primary,statuses.data(),statuses.size());}
roswasmc_cleanup_report revoke_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_revoke_checked(&bundle_,primary,statuses.data(),statuses.size());}
};
/** Typed custom clearpath_camera_msgs/InspectPOI server with explicit feedback/terminal component codecs. */
class Ros1Clearpath_camera_msgsInspectPOIServer:public ServerOwners {
public:
using Goal=Ros1Clearpath_camera_msgsInspectPOIGoal;using Result=Ros1Clearpath_camera_msgsInspectPOIResult;using Feedback=Ros1Clearpath_camera_msgsInspectPOIFeedback;
/** Handler may choose separate feedback/result effects; kind must be2..4 when terminal. */
struct Replies {Feedback feedback{};Result result{};uint8_t kind{2};bool send_feedback{true},send_result{true};};
private:using Handler=std::function<Replies(uint64_t,Goal const&)>;std::shared_ptr<Handler> handler_;
public:
Ros1Clearpath_camera_msgsInspectPOIServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,true} {}
void on_goal(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& id,Goal& goal){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_camera_msgs_InspectPOI_poll(&advertise_,&id,&goal);}
roswasmc_result respond_feedback(uint64_t id,Feedback const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_camera_msgs_InspectPOI_respond_feedback(&respond_,id,&value);}
roswasmc_result respond_result(uint64_t id,uint8_t kind,Result const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_camera_msgs_InspectPOI_respond_result(&respond_,id,kind,&value);}
/** One pass, at most one feedback plus one terminal response; never implicit replay. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t id{};Goal goal{};auto r=roswasmc_bounded_1_clearpath_camera_msgs_InspectPOI_poll(&advertise_,&id,&goal);if(r.error)return r.error;auto replies=(*handler)(id,goal);if(replies.send_result&&(replies.kind<2||replies.kind>4||!roswasmc_bounded_1_clearpath_camera_msgs_InspectPOIResult_valid(&replies.result)))return 22;if(replies.send_feedback&&!roswasmc_bounded_1_clearpath_camera_msgs_InspectPOIFeedback_valid(&replies.feedback))return 22;if(replies.send_feedback){r=roswasmc_bounded_1_clearpath_camera_msgs_InspectPOI_respond_feedback(&respond_,id,&replies.feedback);if(r.error)return r.error;}return replies.send_result?roswasmc_bounded_1_clearpath_camera_msgs_InspectPOI_respond_result(&respond_,id,replies.kind,&replies.result).error:0;}
};
/** Complete owned custom clearpath_dock_msgs/LocalDock Action client with four separate roles. */
class Ros1Clearpath_dock_msgsLocalDockClient {roswasmc_context context_;std::array<uint64_t,1024> goals_{};roswasmc_action_bundle bundle_{};
public:
using Goal=Ros1Clearpath_dock_msgsLocalDockGoal;using Result=Ros1Clearpath_dock_msgsLocalDockResult;using Feedback=Ros1Clearpath_dock_msgsLocalDockFeedback;
Ros1Clearpath_dock_msgsLocalDockClient(roswasmc_context const& context,roswasmc_action_aliases const& aliases,size_t slots=8):context_{context.backend} {roswasmc_action_bundle_init(&context_,&bundle_,&aliases,goals_.data(),slots);}
Ros1Clearpath_dock_msgsLocalDockClient(Ros1Clearpath_dock_msgsLocalDockClient const&)=delete;Ros1Clearpath_dock_msgsLocalDockClient& operator=(Ros1Clearpath_dock_msgsLocalDockClient const&)=delete;Ros1Clearpath_dock_msgsLocalDockClient(Ros1Clearpath_dock_msgsLocalDockClient&&)=delete;
~Ros1Clearpath_dock_msgsLocalDockClient(){if(bundle_.active){std::array<uint32_t,5> statuses{};roswasmc_action_bundle_close_checked(&bundle_,0,statuses.data(),statuses.size());}}
uint32_t error() const noexcept{return bundle_.error?bundle_.error:bundle_.active?0:9;}bool is_open() const noexcept{return bundle_.active;}bool owns(uint64_t id)const{return roswasmc_action_bundle_owns(&bundle_,id);}
roswasmc_result send_goal(Goal const& value,uint64_t& id){return roswasmc_bounded_1_clearpath_dock_msgs_LocalDock_send(&bundle_,&value,&id);}
roswasmc_result feedback(uint64_t id,Feedback& value){return roswasmc_bounded_1_clearpath_dock_msgs_LocalDock_feedback(&bundle_,id,&value);}
roswasmc_result result(uint64_t id,uint8_t& kind,Result& value){return roswasmc_bounded_1_clearpath_dock_msgs_LocalDock_result(&bundle_,id,&kind,&value);}
roswasmc_result cancel_own(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_own(&bundle_,id,&accepted);}
roswasmc_result cancel_any(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_any(&bundle_,id,&accepted);}
roswasmc_result cancel_all(int& accepted){return roswasmc_action_bundle_cancel_all(&bundle_,&accepted);}
roswasmc_cleanup_report close_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_close_checked(&bundle_,primary,statuses.data(),statuses.size());}
roswasmc_cleanup_report revoke_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_revoke_checked(&bundle_,primary,statuses.data(),statuses.size());}
};
/** Typed custom clearpath_dock_msgs/LocalDock server with explicit feedback/terminal component codecs. */
class Ros1Clearpath_dock_msgsLocalDockServer:public ServerOwners {
public:
using Goal=Ros1Clearpath_dock_msgsLocalDockGoal;using Result=Ros1Clearpath_dock_msgsLocalDockResult;using Feedback=Ros1Clearpath_dock_msgsLocalDockFeedback;
/** Handler may choose separate feedback/result effects; kind must be2..4 when terminal. */
struct Replies {Feedback feedback{};Result result{};uint8_t kind{2};bool send_feedback{true},send_result{true};};
private:using Handler=std::function<Replies(uint64_t,Goal const&)>;std::shared_ptr<Handler> handler_;
public:
Ros1Clearpath_dock_msgsLocalDockServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,true} {}
void on_goal(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& id,Goal& goal){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_dock_msgs_LocalDock_poll(&advertise_,&id,&goal);}
roswasmc_result respond_feedback(uint64_t id,Feedback const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_dock_msgs_LocalDock_respond_feedback(&respond_,id,&value);}
roswasmc_result respond_result(uint64_t id,uint8_t kind,Result const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_dock_msgs_LocalDock_respond_result(&respond_,id,kind,&value);}
/** One pass, at most one feedback plus one terminal response; never implicit replay. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t id{};Goal goal{};auto r=roswasmc_bounded_1_clearpath_dock_msgs_LocalDock_poll(&advertise_,&id,&goal);if(r.error)return r.error;auto replies=(*handler)(id,goal);if(replies.send_result&&(replies.kind<2||replies.kind>4||!roswasmc_bounded_1_clearpath_dock_msgs_LocalDockResult_valid(&replies.result)))return 22;if(replies.send_feedback&&!roswasmc_bounded_1_clearpath_dock_msgs_LocalDockFeedback_valid(&replies.feedback))return 22;if(replies.send_feedback){r=roswasmc_bounded_1_clearpath_dock_msgs_LocalDock_respond_feedback(&respond_,id,&replies.feedback);if(r.error)return r.error;}return replies.send_result?roswasmc_bounded_1_clearpath_dock_msgs_LocalDock_respond_result(&respond_,id,replies.kind,&replies.result).error:0;}
};
/** Complete owned custom clearpath_dock_msgs/LocalUndock Action client with four separate roles. */
class Ros1Clearpath_dock_msgsLocalUndockClient {roswasmc_context context_;std::array<uint64_t,1024> goals_{};roswasmc_action_bundle bundle_{};
public:
using Goal=Ros1Clearpath_dock_msgsLocalUndockGoal;using Result=Ros1Clearpath_dock_msgsLocalUndockResult;using Feedback=Ros1Clearpath_dock_msgsLocalUndockFeedback;
Ros1Clearpath_dock_msgsLocalUndockClient(roswasmc_context const& context,roswasmc_action_aliases const& aliases,size_t slots=8):context_{context.backend} {roswasmc_action_bundle_init(&context_,&bundle_,&aliases,goals_.data(),slots);}
Ros1Clearpath_dock_msgsLocalUndockClient(Ros1Clearpath_dock_msgsLocalUndockClient const&)=delete;Ros1Clearpath_dock_msgsLocalUndockClient& operator=(Ros1Clearpath_dock_msgsLocalUndockClient const&)=delete;Ros1Clearpath_dock_msgsLocalUndockClient(Ros1Clearpath_dock_msgsLocalUndockClient&&)=delete;
~Ros1Clearpath_dock_msgsLocalUndockClient(){if(bundle_.active){std::array<uint32_t,5> statuses{};roswasmc_action_bundle_close_checked(&bundle_,0,statuses.data(),statuses.size());}}
uint32_t error() const noexcept{return bundle_.error?bundle_.error:bundle_.active?0:9;}bool is_open() const noexcept{return bundle_.active;}bool owns(uint64_t id)const{return roswasmc_action_bundle_owns(&bundle_,id);}
roswasmc_result send_goal(Goal const& value,uint64_t& id){return roswasmc_bounded_1_clearpath_dock_msgs_LocalUndock_send(&bundle_,&value,&id);}
roswasmc_result feedback(uint64_t id,Feedback& value){return roswasmc_bounded_1_clearpath_dock_msgs_LocalUndock_feedback(&bundle_,id,&value);}
roswasmc_result result(uint64_t id,uint8_t& kind,Result& value){return roswasmc_bounded_1_clearpath_dock_msgs_LocalUndock_result(&bundle_,id,&kind,&value);}
roswasmc_result cancel_own(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_own(&bundle_,id,&accepted);}
roswasmc_result cancel_any(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_any(&bundle_,id,&accepted);}
roswasmc_result cancel_all(int& accepted){return roswasmc_action_bundle_cancel_all(&bundle_,&accepted);}
roswasmc_cleanup_report close_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_close_checked(&bundle_,primary,statuses.data(),statuses.size());}
roswasmc_cleanup_report revoke_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_revoke_checked(&bundle_,primary,statuses.data(),statuses.size());}
};
/** Typed custom clearpath_dock_msgs/LocalUndock server with explicit feedback/terminal component codecs. */
class Ros1Clearpath_dock_msgsLocalUndockServer:public ServerOwners {
public:
using Goal=Ros1Clearpath_dock_msgsLocalUndockGoal;using Result=Ros1Clearpath_dock_msgsLocalUndockResult;using Feedback=Ros1Clearpath_dock_msgsLocalUndockFeedback;
/** Handler may choose separate feedback/result effects; kind must be2..4 when terminal. */
struct Replies {Feedback feedback{};Result result{};uint8_t kind{2};bool send_feedback{true},send_result{true};};
private:using Handler=std::function<Replies(uint64_t,Goal const&)>;std::shared_ptr<Handler> handler_;
public:
Ros1Clearpath_dock_msgsLocalUndockServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,true} {}
void on_goal(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& id,Goal& goal){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_dock_msgs_LocalUndock_poll(&advertise_,&id,&goal);}
roswasmc_result respond_feedback(uint64_t id,Feedback const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_dock_msgs_LocalUndock_respond_feedback(&respond_,id,&value);}
roswasmc_result respond_result(uint64_t id,uint8_t kind,Result const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_dock_msgs_LocalUndock_respond_result(&respond_,id,kind,&value);}
/** One pass, at most one feedback plus one terminal response; never implicit replay. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t id{};Goal goal{};auto r=roswasmc_bounded_1_clearpath_dock_msgs_LocalUndock_poll(&advertise_,&id,&goal);if(r.error)return r.error;auto replies=(*handler)(id,goal);if(replies.send_result&&(replies.kind<2||replies.kind>4||!roswasmc_bounded_1_clearpath_dock_msgs_LocalUndockResult_valid(&replies.result)))return 22;if(replies.send_feedback&&!roswasmc_bounded_1_clearpath_dock_msgs_LocalUndockFeedback_valid(&replies.feedback))return 22;if(replies.send_feedback){r=roswasmc_bounded_1_clearpath_dock_msgs_LocalUndock_respond_feedback(&respond_,id,&replies.feedback);if(r.error)return r.error;}return replies.send_result?roswasmc_bounded_1_clearpath_dock_msgs_LocalUndock_respond_result(&respond_,id,replies.kind,&replies.result).error:0;}
};
/** Complete owned custom clearpath_dock_msgs/NetworkDock Action client with four separate roles. */
class Ros1Clearpath_dock_msgsNetworkDockClient {roswasmc_context context_;std::array<uint64_t,1024> goals_{};roswasmc_action_bundle bundle_{};
public:
using Goal=Ros1Clearpath_dock_msgsNetworkDockGoal;using Result=Ros1Clearpath_dock_msgsNetworkDockResult;using Feedback=Ros1Clearpath_dock_msgsNetworkDockFeedback;
Ros1Clearpath_dock_msgsNetworkDockClient(roswasmc_context const& context,roswasmc_action_aliases const& aliases,size_t slots=8):context_{context.backend} {roswasmc_action_bundle_init(&context_,&bundle_,&aliases,goals_.data(),slots);}
Ros1Clearpath_dock_msgsNetworkDockClient(Ros1Clearpath_dock_msgsNetworkDockClient const&)=delete;Ros1Clearpath_dock_msgsNetworkDockClient& operator=(Ros1Clearpath_dock_msgsNetworkDockClient const&)=delete;Ros1Clearpath_dock_msgsNetworkDockClient(Ros1Clearpath_dock_msgsNetworkDockClient&&)=delete;
~Ros1Clearpath_dock_msgsNetworkDockClient(){if(bundle_.active){std::array<uint32_t,5> statuses{};roswasmc_action_bundle_close_checked(&bundle_,0,statuses.data(),statuses.size());}}
uint32_t error() const noexcept{return bundle_.error?bundle_.error:bundle_.active?0:9;}bool is_open() const noexcept{return bundle_.active;}bool owns(uint64_t id)const{return roswasmc_action_bundle_owns(&bundle_,id);}
roswasmc_result send_goal(Goal const& value,uint64_t& id){return roswasmc_bounded_1_clearpath_dock_msgs_NetworkDock_send(&bundle_,&value,&id);}
roswasmc_result feedback(uint64_t id,Feedback& value){return roswasmc_bounded_1_clearpath_dock_msgs_NetworkDock_feedback(&bundle_,id,&value);}
roswasmc_result result(uint64_t id,uint8_t& kind,Result& value){return roswasmc_bounded_1_clearpath_dock_msgs_NetworkDock_result(&bundle_,id,&kind,&value);}
roswasmc_result cancel_own(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_own(&bundle_,id,&accepted);}
roswasmc_result cancel_any(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_any(&bundle_,id,&accepted);}
roswasmc_result cancel_all(int& accepted){return roswasmc_action_bundle_cancel_all(&bundle_,&accepted);}
roswasmc_cleanup_report close_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_close_checked(&bundle_,primary,statuses.data(),statuses.size());}
roswasmc_cleanup_report revoke_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_revoke_checked(&bundle_,primary,statuses.data(),statuses.size());}
};
/** Typed custom clearpath_dock_msgs/NetworkDock server with explicit feedback/terminal component codecs. */
class Ros1Clearpath_dock_msgsNetworkDockServer:public ServerOwners {
public:
using Goal=Ros1Clearpath_dock_msgsNetworkDockGoal;using Result=Ros1Clearpath_dock_msgsNetworkDockResult;using Feedback=Ros1Clearpath_dock_msgsNetworkDockFeedback;
/** Handler may choose separate feedback/result effects; kind must be2..4 when terminal. */
struct Replies {Feedback feedback{};Result result{};uint8_t kind{2};bool send_feedback{true},send_result{true};};
private:using Handler=std::function<Replies(uint64_t,Goal const&)>;std::shared_ptr<Handler> handler_;
public:
Ros1Clearpath_dock_msgsNetworkDockServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,true} {}
void on_goal(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& id,Goal& goal){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_dock_msgs_NetworkDock_poll(&advertise_,&id,&goal);}
roswasmc_result respond_feedback(uint64_t id,Feedback const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_dock_msgs_NetworkDock_respond_feedback(&respond_,id,&value);}
roswasmc_result respond_result(uint64_t id,uint8_t kind,Result const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_dock_msgs_NetworkDock_respond_result(&respond_,id,kind,&value);}
/** One pass, at most one feedback plus one terminal response; never implicit replay. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t id{};Goal goal{};auto r=roswasmc_bounded_1_clearpath_dock_msgs_NetworkDock_poll(&advertise_,&id,&goal);if(r.error)return r.error;auto replies=(*handler)(id,goal);if(replies.send_result&&(replies.kind<2||replies.kind>4||!roswasmc_bounded_1_clearpath_dock_msgs_NetworkDockResult_valid(&replies.result)))return 22;if(replies.send_feedback&&!roswasmc_bounded_1_clearpath_dock_msgs_NetworkDockFeedback_valid(&replies.feedback))return 22;if(replies.send_feedback){r=roswasmc_bounded_1_clearpath_dock_msgs_NetworkDock_respond_feedback(&respond_,id,&replies.feedback);if(r.error)return r.error;}return replies.send_result?roswasmc_bounded_1_clearpath_dock_msgs_NetworkDock_respond_result(&respond_,id,replies.kind,&replies.result).error:0;}
};
/** Complete owned custom clearpath_dock_msgs/RadiusDock Action client with four separate roles. */
class Ros1Clearpath_dock_msgsRadiusDockClient {roswasmc_context context_;std::array<uint64_t,1024> goals_{};roswasmc_action_bundle bundle_{};
public:
using Goal=Ros1Clearpath_dock_msgsRadiusDockGoal;using Result=Ros1Clearpath_dock_msgsRadiusDockResult;using Feedback=Ros1Clearpath_dock_msgsRadiusDockFeedback;
Ros1Clearpath_dock_msgsRadiusDockClient(roswasmc_context const& context,roswasmc_action_aliases const& aliases,size_t slots=8):context_{context.backend} {roswasmc_action_bundle_init(&context_,&bundle_,&aliases,goals_.data(),slots);}
Ros1Clearpath_dock_msgsRadiusDockClient(Ros1Clearpath_dock_msgsRadiusDockClient const&)=delete;Ros1Clearpath_dock_msgsRadiusDockClient& operator=(Ros1Clearpath_dock_msgsRadiusDockClient const&)=delete;Ros1Clearpath_dock_msgsRadiusDockClient(Ros1Clearpath_dock_msgsRadiusDockClient&&)=delete;
~Ros1Clearpath_dock_msgsRadiusDockClient(){if(bundle_.active){std::array<uint32_t,5> statuses{};roswasmc_action_bundle_close_checked(&bundle_,0,statuses.data(),statuses.size());}}
uint32_t error() const noexcept{return bundle_.error?bundle_.error:bundle_.active?0:9;}bool is_open() const noexcept{return bundle_.active;}bool owns(uint64_t id)const{return roswasmc_action_bundle_owns(&bundle_,id);}
roswasmc_result send_goal(Goal const& value,uint64_t& id){return roswasmc_bounded_1_clearpath_dock_msgs_RadiusDock_send(&bundle_,&value,&id);}
roswasmc_result feedback(uint64_t id,Feedback& value){return roswasmc_bounded_1_clearpath_dock_msgs_RadiusDock_feedback(&bundle_,id,&value);}
roswasmc_result result(uint64_t id,uint8_t& kind,Result& value){return roswasmc_bounded_1_clearpath_dock_msgs_RadiusDock_result(&bundle_,id,&kind,&value);}
roswasmc_result cancel_own(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_own(&bundle_,id,&accepted);}
roswasmc_result cancel_any(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_any(&bundle_,id,&accepted);}
roswasmc_result cancel_all(int& accepted){return roswasmc_action_bundle_cancel_all(&bundle_,&accepted);}
roswasmc_cleanup_report close_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_close_checked(&bundle_,primary,statuses.data(),statuses.size());}
roswasmc_cleanup_report revoke_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_revoke_checked(&bundle_,primary,statuses.data(),statuses.size());}
};
/** Typed custom clearpath_dock_msgs/RadiusDock server with explicit feedback/terminal component codecs. */
class Ros1Clearpath_dock_msgsRadiusDockServer:public ServerOwners {
public:
using Goal=Ros1Clearpath_dock_msgsRadiusDockGoal;using Result=Ros1Clearpath_dock_msgsRadiusDockResult;using Feedback=Ros1Clearpath_dock_msgsRadiusDockFeedback;
/** Handler may choose separate feedback/result effects; kind must be2..4 when terminal. */
struct Replies {Feedback feedback{};Result result{};uint8_t kind{2};bool send_feedback{true},send_result{true};};
private:using Handler=std::function<Replies(uint64_t,Goal const&)>;std::shared_ptr<Handler> handler_;
public:
Ros1Clearpath_dock_msgsRadiusDockServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,true} {}
void on_goal(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& id,Goal& goal){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_dock_msgs_RadiusDock_poll(&advertise_,&id,&goal);}
roswasmc_result respond_feedback(uint64_t id,Feedback const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_dock_msgs_RadiusDock_respond_feedback(&respond_,id,&value);}
roswasmc_result respond_result(uint64_t id,uint8_t kind,Result const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_dock_msgs_RadiusDock_respond_result(&respond_,id,kind,&value);}
/** One pass, at most one feedback plus one terminal response; never implicit replay. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t id{};Goal goal{};auto r=roswasmc_bounded_1_clearpath_dock_msgs_RadiusDock_poll(&advertise_,&id,&goal);if(r.error)return r.error;auto replies=(*handler)(id,goal);if(replies.send_result&&(replies.kind<2||replies.kind>4||!roswasmc_bounded_1_clearpath_dock_msgs_RadiusDockResult_valid(&replies.result)))return 22;if(replies.send_feedback&&!roswasmc_bounded_1_clearpath_dock_msgs_RadiusDockFeedback_valid(&replies.feedback))return 22;if(replies.send_feedback){r=roswasmc_bounded_1_clearpath_dock_msgs_RadiusDock_respond_feedback(&respond_,id,&replies.feedback);if(r.error)return r.error;}return replies.send_result?roswasmc_bounded_1_clearpath_dock_msgs_RadiusDock_respond_result(&respond_,id,replies.kind,&replies.result).error:0;}
};
/** Complete owned custom clearpath_localization_msgs/SurveyBaseStation Action client with four separate roles. */
class Ros1Clearpath_localization_msgsSurveyBaseStationClient {roswasmc_context context_;std::array<uint64_t,1024> goals_{};roswasmc_action_bundle bundle_{};
public:
using Goal=Ros1Clearpath_localization_msgsSurveyBaseStationGoal;using Result=Ros1Clearpath_localization_msgsSurveyBaseStationResult;using Feedback=Ros1Clearpath_localization_msgsSurveyBaseStationFeedback;
Ros1Clearpath_localization_msgsSurveyBaseStationClient(roswasmc_context const& context,roswasmc_action_aliases const& aliases,size_t slots=8):context_{context.backend} {roswasmc_action_bundle_init(&context_,&bundle_,&aliases,goals_.data(),slots);}
Ros1Clearpath_localization_msgsSurveyBaseStationClient(Ros1Clearpath_localization_msgsSurveyBaseStationClient const&)=delete;Ros1Clearpath_localization_msgsSurveyBaseStationClient& operator=(Ros1Clearpath_localization_msgsSurveyBaseStationClient const&)=delete;Ros1Clearpath_localization_msgsSurveyBaseStationClient(Ros1Clearpath_localization_msgsSurveyBaseStationClient&&)=delete;
~Ros1Clearpath_localization_msgsSurveyBaseStationClient(){if(bundle_.active){std::array<uint32_t,5> statuses{};roswasmc_action_bundle_close_checked(&bundle_,0,statuses.data(),statuses.size());}}
uint32_t error() const noexcept{return bundle_.error?bundle_.error:bundle_.active?0:9;}bool is_open() const noexcept{return bundle_.active;}bool owns(uint64_t id)const{return roswasmc_action_bundle_owns(&bundle_,id);}
roswasmc_result send_goal(Goal const& value,uint64_t& id){return roswasmc_bounded_1_clearpath_localization_msgs_SurveyBaseStation_send(&bundle_,&value,&id);}
roswasmc_result feedback(uint64_t id,Feedback& value){return roswasmc_bounded_1_clearpath_localization_msgs_SurveyBaseStation_feedback(&bundle_,id,&value);}
roswasmc_result result(uint64_t id,uint8_t& kind,Result& value){return roswasmc_bounded_1_clearpath_localization_msgs_SurveyBaseStation_result(&bundle_,id,&kind,&value);}
roswasmc_result cancel_own(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_own(&bundle_,id,&accepted);}
roswasmc_result cancel_any(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_any(&bundle_,id,&accepted);}
roswasmc_result cancel_all(int& accepted){return roswasmc_action_bundle_cancel_all(&bundle_,&accepted);}
roswasmc_cleanup_report close_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_close_checked(&bundle_,primary,statuses.data(),statuses.size());}
roswasmc_cleanup_report revoke_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_revoke_checked(&bundle_,primary,statuses.data(),statuses.size());}
};
/** Typed custom clearpath_localization_msgs/SurveyBaseStation server with explicit feedback/terminal component codecs. */
class Ros1Clearpath_localization_msgsSurveyBaseStationServer:public ServerOwners {
public:
using Goal=Ros1Clearpath_localization_msgsSurveyBaseStationGoal;using Result=Ros1Clearpath_localization_msgsSurveyBaseStationResult;using Feedback=Ros1Clearpath_localization_msgsSurveyBaseStationFeedback;
/** Handler may choose separate feedback/result effects; kind must be2..4 when terminal. */
struct Replies {Feedback feedback{};Result result{};uint8_t kind{2};bool send_feedback{true},send_result{true};};
private:using Handler=std::function<Replies(uint64_t,Goal const&)>;std::shared_ptr<Handler> handler_;
public:
Ros1Clearpath_localization_msgsSurveyBaseStationServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,true} {}
void on_goal(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& id,Goal& goal){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_localization_msgs_SurveyBaseStation_poll(&advertise_,&id,&goal);}
roswasmc_result respond_feedback(uint64_t id,Feedback const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_localization_msgs_SurveyBaseStation_respond_feedback(&respond_,id,&value);}
roswasmc_result respond_result(uint64_t id,uint8_t kind,Result const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_localization_msgs_SurveyBaseStation_respond_result(&respond_,id,kind,&value);}
/** One pass, at most one feedback plus one terminal response; never implicit replay. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t id{};Goal goal{};auto r=roswasmc_bounded_1_clearpath_localization_msgs_SurveyBaseStation_poll(&advertise_,&id,&goal);if(r.error)return r.error;auto replies=(*handler)(id,goal);if(replies.send_result&&(replies.kind<2||replies.kind>4||!roswasmc_bounded_1_clearpath_localization_msgs_SurveyBaseStationResult_valid(&replies.result)))return 22;if(replies.send_feedback&&!roswasmc_bounded_1_clearpath_localization_msgs_SurveyBaseStationFeedback_valid(&replies.feedback))return 22;if(replies.send_feedback){r=roswasmc_bounded_1_clearpath_localization_msgs_SurveyBaseStation_respond_feedback(&respond_,id,&replies.feedback);if(r.error)return r.error;}return replies.send_result?roswasmc_bounded_1_clearpath_localization_msgs_SurveyBaseStation_respond_result(&respond_,id,replies.kind,&replies.result).error:0;}
};
/** Complete owned custom clearpath_mission_scheduler_msgs/RunScheduleByUuid Action client with four separate roles. */
class Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidClient {roswasmc_context context_;std::array<uint64_t,1024> goals_{};roswasmc_action_bundle bundle_{};
public:
using Goal=Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidGoal;using Result=Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidResult;using Feedback=Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidFeedback;
Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidClient(roswasmc_context const& context,roswasmc_action_aliases const& aliases,size_t slots=8):context_{context.backend} {roswasmc_action_bundle_init(&context_,&bundle_,&aliases,goals_.data(),slots);}
Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidClient(Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidClient const&)=delete;Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidClient& operator=(Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidClient const&)=delete;Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidClient(Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidClient&&)=delete;
~Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidClient(){if(bundle_.active){std::array<uint32_t,5> statuses{};roswasmc_action_bundle_close_checked(&bundle_,0,statuses.data(),statuses.size());}}
uint32_t error() const noexcept{return bundle_.error?bundle_.error:bundle_.active?0:9;}bool is_open() const noexcept{return bundle_.active;}bool owns(uint64_t id)const{return roswasmc_action_bundle_owns(&bundle_,id);}
roswasmc_result send_goal(Goal const& value,uint64_t& id){return roswasmc_bounded_1_clearpath_mission_scheduler_msgs_RunScheduleByUuid_send(&bundle_,&value,&id);}
roswasmc_result feedback(uint64_t id,Feedback& value){return roswasmc_bounded_1_clearpath_mission_scheduler_msgs_RunScheduleByUuid_feedback(&bundle_,id,&value);}
roswasmc_result result(uint64_t id,uint8_t& kind,Result& value){return roswasmc_bounded_1_clearpath_mission_scheduler_msgs_RunScheduleByUuid_result(&bundle_,id,&kind,&value);}
roswasmc_result cancel_own(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_own(&bundle_,id,&accepted);}
roswasmc_result cancel_any(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_any(&bundle_,id,&accepted);}
roswasmc_result cancel_all(int& accepted){return roswasmc_action_bundle_cancel_all(&bundle_,&accepted);}
roswasmc_cleanup_report close_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_close_checked(&bundle_,primary,statuses.data(),statuses.size());}
roswasmc_cleanup_report revoke_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_revoke_checked(&bundle_,primary,statuses.data(),statuses.size());}
};
/** Typed custom clearpath_mission_scheduler_msgs/RunScheduleByUuid server with explicit feedback/terminal component codecs. */
class Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidServer:public ServerOwners {
public:
using Goal=Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidGoal;using Result=Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidResult;using Feedback=Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidFeedback;
/** Handler may choose separate feedback/result effects; kind must be2..4 when terminal. */
struct Replies {Feedback feedback{};Result result{};uint8_t kind{2};bool send_feedback{true},send_result{true};};
private:using Handler=std::function<Replies(uint64_t,Goal const&)>;std::shared_ptr<Handler> handler_;
public:
Ros1Clearpath_mission_scheduler_msgsRunScheduleByUuidServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,true} {}
void on_goal(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& id,Goal& goal){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_scheduler_msgs_RunScheduleByUuid_poll(&advertise_,&id,&goal);}
roswasmc_result respond_feedback(uint64_t id,Feedback const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_scheduler_msgs_RunScheduleByUuid_respond_feedback(&respond_,id,&value);}
roswasmc_result respond_result(uint64_t id,uint8_t kind,Result const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_scheduler_msgs_RunScheduleByUuid_respond_result(&respond_,id,kind,&value);}
/** One pass, at most one feedback plus one terminal response; never implicit replay. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t id{};Goal goal{};auto r=roswasmc_bounded_1_clearpath_mission_scheduler_msgs_RunScheduleByUuid_poll(&advertise_,&id,&goal);if(r.error)return r.error;auto replies=(*handler)(id,goal);if(replies.send_result&&(replies.kind<2||replies.kind>4||!roswasmc_bounded_1_clearpath_mission_scheduler_msgs_RunScheduleByUuidResult_valid(&replies.result)))return 22;if(replies.send_feedback&&!roswasmc_bounded_1_clearpath_mission_scheduler_msgs_RunScheduleByUuidFeedback_valid(&replies.feedback))return 22;if(replies.send_feedback){r=roswasmc_bounded_1_clearpath_mission_scheduler_msgs_RunScheduleByUuid_respond_feedback(&respond_,id,&replies.feedback);if(r.error)return r.error;}return replies.send_result?roswasmc_bounded_1_clearpath_mission_scheduler_msgs_RunScheduleByUuid_respond_result(&respond_,id,replies.kind,&replies.result).error:0;}
};
/** Complete owned custom clearpath_navigation_msgs/ExecuteMissionByUuid Action client with four separate roles. */
class Ros1Clearpath_navigation_msgsExecuteMissionByUuidClient {roswasmc_context context_;std::array<uint64_t,1024> goals_{};roswasmc_action_bundle bundle_{};
public:
using Goal=Ros1Clearpath_navigation_msgsExecuteMissionByUuidGoal;using Result=Ros1Clearpath_navigation_msgsExecuteMissionByUuidResult;using Feedback=Ros1Clearpath_navigation_msgsExecuteMissionByUuidFeedback;
Ros1Clearpath_navigation_msgsExecuteMissionByUuidClient(roswasmc_context const& context,roswasmc_action_aliases const& aliases,size_t slots=8):context_{context.backend} {roswasmc_action_bundle_init(&context_,&bundle_,&aliases,goals_.data(),slots);}
Ros1Clearpath_navigation_msgsExecuteMissionByUuidClient(Ros1Clearpath_navigation_msgsExecuteMissionByUuidClient const&)=delete;Ros1Clearpath_navigation_msgsExecuteMissionByUuidClient& operator=(Ros1Clearpath_navigation_msgsExecuteMissionByUuidClient const&)=delete;Ros1Clearpath_navigation_msgsExecuteMissionByUuidClient(Ros1Clearpath_navigation_msgsExecuteMissionByUuidClient&&)=delete;
~Ros1Clearpath_navigation_msgsExecuteMissionByUuidClient(){if(bundle_.active){std::array<uint32_t,5> statuses{};roswasmc_action_bundle_close_checked(&bundle_,0,statuses.data(),statuses.size());}}
uint32_t error() const noexcept{return bundle_.error?bundle_.error:bundle_.active?0:9;}bool is_open() const noexcept{return bundle_.active;}bool owns(uint64_t id)const{return roswasmc_action_bundle_owns(&bundle_,id);}
roswasmc_result send_goal(Goal const& value,uint64_t& id){return roswasmc_bounded_1_clearpath_navigation_msgs_ExecuteMissionByUuid_send(&bundle_,&value,&id);}
roswasmc_result feedback(uint64_t id,Feedback& value){return roswasmc_bounded_1_clearpath_navigation_msgs_ExecuteMissionByUuid_feedback(&bundle_,id,&value);}
roswasmc_result result(uint64_t id,uint8_t& kind,Result& value){return roswasmc_bounded_1_clearpath_navigation_msgs_ExecuteMissionByUuid_result(&bundle_,id,&kind,&value);}
roswasmc_result cancel_own(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_own(&bundle_,id,&accepted);}
roswasmc_result cancel_any(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_any(&bundle_,id,&accepted);}
roswasmc_result cancel_all(int& accepted){return roswasmc_action_bundle_cancel_all(&bundle_,&accepted);}
roswasmc_cleanup_report close_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_close_checked(&bundle_,primary,statuses.data(),statuses.size());}
roswasmc_cleanup_report revoke_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_revoke_checked(&bundle_,primary,statuses.data(),statuses.size());}
};
/** Typed custom clearpath_navigation_msgs/ExecuteMissionByUuid server with explicit feedback/terminal component codecs. */
class Ros1Clearpath_navigation_msgsExecuteMissionByUuidServer:public ServerOwners {
public:
using Goal=Ros1Clearpath_navigation_msgsExecuteMissionByUuidGoal;using Result=Ros1Clearpath_navigation_msgsExecuteMissionByUuidResult;using Feedback=Ros1Clearpath_navigation_msgsExecuteMissionByUuidFeedback;
/** Handler may choose separate feedback/result effects; kind must be2..4 when terminal. */
struct Replies {Feedback feedback{};Result result{};uint8_t kind{2};bool send_feedback{true},send_result{true};};
private:using Handler=std::function<Replies(uint64_t,Goal const&)>;std::shared_ptr<Handler> handler_;
public:
Ros1Clearpath_navigation_msgsExecuteMissionByUuidServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,true} {}
void on_goal(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& id,Goal& goal){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_navigation_msgs_ExecuteMissionByUuid_poll(&advertise_,&id,&goal);}
roswasmc_result respond_feedback(uint64_t id,Feedback const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_navigation_msgs_ExecuteMissionByUuid_respond_feedback(&respond_,id,&value);}
roswasmc_result respond_result(uint64_t id,uint8_t kind,Result const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_navigation_msgs_ExecuteMissionByUuid_respond_result(&respond_,id,kind,&value);}
/** One pass, at most one feedback plus one terminal response; never implicit replay. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t id{};Goal goal{};auto r=roswasmc_bounded_1_clearpath_navigation_msgs_ExecuteMissionByUuid_poll(&advertise_,&id,&goal);if(r.error)return r.error;auto replies=(*handler)(id,goal);if(replies.send_result&&(replies.kind<2||replies.kind>4||!roswasmc_bounded_1_clearpath_navigation_msgs_ExecuteMissionByUuidResult_valid(&replies.result)))return 22;if(replies.send_feedback&&!roswasmc_bounded_1_clearpath_navigation_msgs_ExecuteMissionByUuidFeedback_valid(&replies.feedback))return 22;if(replies.send_feedback){r=roswasmc_bounded_1_clearpath_navigation_msgs_ExecuteMissionByUuid_respond_feedback(&respond_,id,&replies.feedback);if(r.error)return r.error;}return replies.send_result?roswasmc_bounded_1_clearpath_navigation_msgs_ExecuteMissionByUuid_respond_result(&respond_,id,replies.kind,&replies.result).error:0;}
};
/** Complete owned custom clearpath_navigation_msgs/ExecuteNetworkGoTo Action client with four separate roles. */
class Ros1Clearpath_navigation_msgsExecuteNetworkGoToClient {roswasmc_context context_;std::array<uint64_t,1024> goals_{};roswasmc_action_bundle bundle_{};
public:
using Goal=Ros1Clearpath_navigation_msgsExecuteNetworkGoToGoal;using Result=Ros1Clearpath_navigation_msgsExecuteNetworkGoToResult;using Feedback=Ros1Clearpath_navigation_msgsExecuteNetworkGoToFeedback;
Ros1Clearpath_navigation_msgsExecuteNetworkGoToClient(roswasmc_context const& context,roswasmc_action_aliases const& aliases,size_t slots=8):context_{context.backend} {roswasmc_action_bundle_init(&context_,&bundle_,&aliases,goals_.data(),slots);}
Ros1Clearpath_navigation_msgsExecuteNetworkGoToClient(Ros1Clearpath_navigation_msgsExecuteNetworkGoToClient const&)=delete;Ros1Clearpath_navigation_msgsExecuteNetworkGoToClient& operator=(Ros1Clearpath_navigation_msgsExecuteNetworkGoToClient const&)=delete;Ros1Clearpath_navigation_msgsExecuteNetworkGoToClient(Ros1Clearpath_navigation_msgsExecuteNetworkGoToClient&&)=delete;
~Ros1Clearpath_navigation_msgsExecuteNetworkGoToClient(){if(bundle_.active){std::array<uint32_t,5> statuses{};roswasmc_action_bundle_close_checked(&bundle_,0,statuses.data(),statuses.size());}}
uint32_t error() const noexcept{return bundle_.error?bundle_.error:bundle_.active?0:9;}bool is_open() const noexcept{return bundle_.active;}bool owns(uint64_t id)const{return roswasmc_action_bundle_owns(&bundle_,id);}
roswasmc_result send_goal(Goal const& value,uint64_t& id){return roswasmc_bounded_1_clearpath_navigation_msgs_ExecuteNetworkGoTo_send(&bundle_,&value,&id);}
roswasmc_result feedback(uint64_t id,Feedback& value){return roswasmc_bounded_1_clearpath_navigation_msgs_ExecuteNetworkGoTo_feedback(&bundle_,id,&value);}
roswasmc_result result(uint64_t id,uint8_t& kind,Result& value){return roswasmc_bounded_1_clearpath_navigation_msgs_ExecuteNetworkGoTo_result(&bundle_,id,&kind,&value);}
roswasmc_result cancel_own(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_own(&bundle_,id,&accepted);}
roswasmc_result cancel_any(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_any(&bundle_,id,&accepted);}
roswasmc_result cancel_all(int& accepted){return roswasmc_action_bundle_cancel_all(&bundle_,&accepted);}
roswasmc_cleanup_report close_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_close_checked(&bundle_,primary,statuses.data(),statuses.size());}
roswasmc_cleanup_report revoke_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_revoke_checked(&bundle_,primary,statuses.data(),statuses.size());}
};
/** Typed custom clearpath_navigation_msgs/ExecuteNetworkGoTo server with explicit feedback/terminal component codecs. */
class Ros1Clearpath_navigation_msgsExecuteNetworkGoToServer:public ServerOwners {
public:
using Goal=Ros1Clearpath_navigation_msgsExecuteNetworkGoToGoal;using Result=Ros1Clearpath_navigation_msgsExecuteNetworkGoToResult;using Feedback=Ros1Clearpath_navigation_msgsExecuteNetworkGoToFeedback;
/** Handler may choose separate feedback/result effects; kind must be2..4 when terminal. */
struct Replies {Feedback feedback{};Result result{};uint8_t kind{2};bool send_feedback{true},send_result{true};};
private:using Handler=std::function<Replies(uint64_t,Goal const&)>;std::shared_ptr<Handler> handler_;
public:
Ros1Clearpath_navigation_msgsExecuteNetworkGoToServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,true} {}
void on_goal(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& id,Goal& goal){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_navigation_msgs_ExecuteNetworkGoTo_poll(&advertise_,&id,&goal);}
roswasmc_result respond_feedback(uint64_t id,Feedback const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_navigation_msgs_ExecuteNetworkGoTo_respond_feedback(&respond_,id,&value);}
roswasmc_result respond_result(uint64_t id,uint8_t kind,Result const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_navigation_msgs_ExecuteNetworkGoTo_respond_result(&respond_,id,kind,&value);}
/** One pass, at most one feedback plus one terminal response; never implicit replay. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t id{};Goal goal{};auto r=roswasmc_bounded_1_clearpath_navigation_msgs_ExecuteNetworkGoTo_poll(&advertise_,&id,&goal);if(r.error)return r.error;auto replies=(*handler)(id,goal);if(replies.send_result&&(replies.kind<2||replies.kind>4||!roswasmc_bounded_1_clearpath_navigation_msgs_ExecuteNetworkGoToResult_valid(&replies.result)))return 22;if(replies.send_feedback&&!roswasmc_bounded_1_clearpath_navigation_msgs_ExecuteNetworkGoToFeedback_valid(&replies.feedback))return 22;if(replies.send_feedback){r=roswasmc_bounded_1_clearpath_navigation_msgs_ExecuteNetworkGoTo_respond_feedback(&respond_,id,&replies.feedback);if(r.error)return r.error;}return replies.send_result?roswasmc_bounded_1_clearpath_navigation_msgs_ExecuteNetworkGoTo_respond_result(&respond_,id,replies.kind,&replies.result).error:0;}
};
/** Complete owned custom clearpath_navigation_msgs/ExecuteNetworkMissionByUuid Action client with four separate roles. */
class Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidClient {roswasmc_context context_;std::array<uint64_t,1024> goals_{};roswasmc_action_bundle bundle_{};
public:
using Goal=Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidGoal;using Result=Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidResult;using Feedback=Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidFeedback;
Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidClient(roswasmc_context const& context,roswasmc_action_aliases const& aliases,size_t slots=8):context_{context.backend} {roswasmc_action_bundle_init(&context_,&bundle_,&aliases,goals_.data(),slots);}
Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidClient(Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidClient const&)=delete;Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidClient& operator=(Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidClient const&)=delete;Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidClient(Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidClient&&)=delete;
~Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidClient(){if(bundle_.active){std::array<uint32_t,5> statuses{};roswasmc_action_bundle_close_checked(&bundle_,0,statuses.data(),statuses.size());}}
uint32_t error() const noexcept{return bundle_.error?bundle_.error:bundle_.active?0:9;}bool is_open() const noexcept{return bundle_.active;}bool owns(uint64_t id)const{return roswasmc_action_bundle_owns(&bundle_,id);}
roswasmc_result send_goal(Goal const& value,uint64_t& id){return roswasmc_bounded_1_clearpath_navigation_msgs_ExecuteNetworkMissionByUuid_send(&bundle_,&value,&id);}
roswasmc_result feedback(uint64_t id,Feedback& value){return roswasmc_bounded_1_clearpath_navigation_msgs_ExecuteNetworkMissionByUuid_feedback(&bundle_,id,&value);}
roswasmc_result result(uint64_t id,uint8_t& kind,Result& value){return roswasmc_bounded_1_clearpath_navigation_msgs_ExecuteNetworkMissionByUuid_result(&bundle_,id,&kind,&value);}
roswasmc_result cancel_own(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_own(&bundle_,id,&accepted);}
roswasmc_result cancel_any(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_any(&bundle_,id,&accepted);}
roswasmc_result cancel_all(int& accepted){return roswasmc_action_bundle_cancel_all(&bundle_,&accepted);}
roswasmc_cleanup_report close_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_close_checked(&bundle_,primary,statuses.data(),statuses.size());}
roswasmc_cleanup_report revoke_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_revoke_checked(&bundle_,primary,statuses.data(),statuses.size());}
};
/** Typed custom clearpath_navigation_msgs/ExecuteNetworkMissionByUuid server with explicit feedback/terminal component codecs. */
class Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidServer:public ServerOwners {
public:
using Goal=Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidGoal;using Result=Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidResult;using Feedback=Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidFeedback;
/** Handler may choose separate feedback/result effects; kind must be2..4 when terminal. */
struct Replies {Feedback feedback{};Result result{};uint8_t kind{2};bool send_feedback{true},send_result{true};};
private:using Handler=std::function<Replies(uint64_t,Goal const&)>;std::shared_ptr<Handler> handler_;
public:
Ros1Clearpath_navigation_msgsExecuteNetworkMissionByUuidServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,true} {}
void on_goal(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& id,Goal& goal){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_navigation_msgs_ExecuteNetworkMissionByUuid_poll(&advertise_,&id,&goal);}
roswasmc_result respond_feedback(uint64_t id,Feedback const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_navigation_msgs_ExecuteNetworkMissionByUuid_respond_feedback(&respond_,id,&value);}
roswasmc_result respond_result(uint64_t id,uint8_t kind,Result const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_navigation_msgs_ExecuteNetworkMissionByUuid_respond_result(&respond_,id,kind,&value);}
/** One pass, at most one feedback plus one terminal response; never implicit replay. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t id{};Goal goal{};auto r=roswasmc_bounded_1_clearpath_navigation_msgs_ExecuteNetworkMissionByUuid_poll(&advertise_,&id,&goal);if(r.error)return r.error;auto replies=(*handler)(id,goal);if(replies.send_result&&(replies.kind<2||replies.kind>4||!roswasmc_bounded_1_clearpath_navigation_msgs_ExecuteNetworkMissionByUuidResult_valid(&replies.result)))return 22;if(replies.send_feedback&&!roswasmc_bounded_1_clearpath_navigation_msgs_ExecuteNetworkMissionByUuidFeedback_valid(&replies.feedback))return 22;if(replies.send_feedback){r=roswasmc_bounded_1_clearpath_navigation_msgs_ExecuteNetworkMissionByUuid_respond_feedback(&respond_,id,&replies.feedback);if(r.error)return r.error;}return replies.send_result?roswasmc_bounded_1_clearpath_navigation_msgs_ExecuteNetworkMissionByUuid_respond_result(&respond_,id,replies.kind,&replies.result).error:0;}
};
/** Complete owned custom clearpath_navigation_msgs/GetWaypointPath Action client with four separate roles. */
class Ros1Clearpath_navigation_msgsGetWaypointPathClient {roswasmc_context context_;std::array<uint64_t,1024> goals_{};roswasmc_action_bundle bundle_{};
public:
using Goal=Ros1Clearpath_navigation_msgsGetWaypointPathGoal;using Result=Ros1Clearpath_navigation_msgsGetWaypointPathResult;using Feedback=Ros1Clearpath_navigation_msgsGetWaypointPathFeedback;
Ros1Clearpath_navigation_msgsGetWaypointPathClient(roswasmc_context const& context,roswasmc_action_aliases const& aliases,size_t slots=8):context_{context.backend} {roswasmc_action_bundle_init(&context_,&bundle_,&aliases,goals_.data(),slots);}
Ros1Clearpath_navigation_msgsGetWaypointPathClient(Ros1Clearpath_navigation_msgsGetWaypointPathClient const&)=delete;Ros1Clearpath_navigation_msgsGetWaypointPathClient& operator=(Ros1Clearpath_navigation_msgsGetWaypointPathClient const&)=delete;Ros1Clearpath_navigation_msgsGetWaypointPathClient(Ros1Clearpath_navigation_msgsGetWaypointPathClient&&)=delete;
~Ros1Clearpath_navigation_msgsGetWaypointPathClient(){if(bundle_.active){std::array<uint32_t,5> statuses{};roswasmc_action_bundle_close_checked(&bundle_,0,statuses.data(),statuses.size());}}
uint32_t error() const noexcept{return bundle_.error?bundle_.error:bundle_.active?0:9;}bool is_open() const noexcept{return bundle_.active;}bool owns(uint64_t id)const{return roswasmc_action_bundle_owns(&bundle_,id);}
roswasmc_result send_goal(Goal const& value,uint64_t& id){return roswasmc_bounded_1_clearpath_navigation_msgs_GetWaypointPath_send(&bundle_,&value,&id);}
roswasmc_result feedback(uint64_t id,Feedback& value){return roswasmc_bounded_1_clearpath_navigation_msgs_GetWaypointPath_feedback(&bundle_,id,&value);}
roswasmc_result result(uint64_t id,uint8_t& kind,Result& value){return roswasmc_bounded_1_clearpath_navigation_msgs_GetWaypointPath_result(&bundle_,id,&kind,&value);}
roswasmc_result cancel_own(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_own(&bundle_,id,&accepted);}
roswasmc_result cancel_any(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_any(&bundle_,id,&accepted);}
roswasmc_result cancel_all(int& accepted){return roswasmc_action_bundle_cancel_all(&bundle_,&accepted);}
roswasmc_cleanup_report close_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_close_checked(&bundle_,primary,statuses.data(),statuses.size());}
roswasmc_cleanup_report revoke_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_revoke_checked(&bundle_,primary,statuses.data(),statuses.size());}
};
/** Typed custom clearpath_navigation_msgs/GetWaypointPath server with explicit feedback/terminal component codecs. */
class Ros1Clearpath_navigation_msgsGetWaypointPathServer:public ServerOwners {
public:
using Goal=Ros1Clearpath_navigation_msgsGetWaypointPathGoal;using Result=Ros1Clearpath_navigation_msgsGetWaypointPathResult;using Feedback=Ros1Clearpath_navigation_msgsGetWaypointPathFeedback;
/** Handler may choose separate feedback/result effects; kind must be2..4 when terminal. */
struct Replies {Feedback feedback{};Result result{};uint8_t kind{2};bool send_feedback{true},send_result{true};};
private:using Handler=std::function<Replies(uint64_t,Goal const&)>;std::shared_ptr<Handler> handler_;
public:
Ros1Clearpath_navigation_msgsGetWaypointPathServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,true} {}
void on_goal(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& id,Goal& goal){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_navigation_msgs_GetWaypointPath_poll(&advertise_,&id,&goal);}
roswasmc_result respond_feedback(uint64_t id,Feedback const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_navigation_msgs_GetWaypointPath_respond_feedback(&respond_,id,&value);}
roswasmc_result respond_result(uint64_t id,uint8_t kind,Result const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_navigation_msgs_GetWaypointPath_respond_result(&respond_,id,kind,&value);}
/** One pass, at most one feedback plus one terminal response; never implicit replay. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t id{};Goal goal{};auto r=roswasmc_bounded_1_clearpath_navigation_msgs_GetWaypointPath_poll(&advertise_,&id,&goal);if(r.error)return r.error;auto replies=(*handler)(id,goal);if(replies.send_result&&(replies.kind<2||replies.kind>4||!roswasmc_bounded_1_clearpath_navigation_msgs_GetWaypointPathResult_valid(&replies.result)))return 22;if(replies.send_feedback&&!roswasmc_bounded_1_clearpath_navigation_msgs_GetWaypointPathFeedback_valid(&replies.feedback))return 22;if(replies.send_feedback){r=roswasmc_bounded_1_clearpath_navigation_msgs_GetWaypointPath_respond_feedback(&respond_,id,&replies.feedback);if(r.error)return r.error;}return replies.send_result?roswasmc_bounded_1_clearpath_navigation_msgs_GetWaypointPath_respond_result(&respond_,id,replies.kind,&replies.result).error:0;}
};
/** Complete owned custom clearpath_navigation_msgs/Mission Action client with four separate roles. */
class Ros1Clearpath_navigation_msgsMissionClient {roswasmc_context context_;std::array<uint64_t,1024> goals_{};roswasmc_action_bundle bundle_{};
public:
using Goal=Ros1Clearpath_navigation_msgsMissionGoal;using Result=Ros1Clearpath_navigation_msgsMissionResult;using Feedback=Ros1Clearpath_navigation_msgsMissionFeedback;
Ros1Clearpath_navigation_msgsMissionClient(roswasmc_context const& context,roswasmc_action_aliases const& aliases,size_t slots=8):context_{context.backend} {roswasmc_action_bundle_init(&context_,&bundle_,&aliases,goals_.data(),slots);}
Ros1Clearpath_navigation_msgsMissionClient(Ros1Clearpath_navigation_msgsMissionClient const&)=delete;Ros1Clearpath_navigation_msgsMissionClient& operator=(Ros1Clearpath_navigation_msgsMissionClient const&)=delete;Ros1Clearpath_navigation_msgsMissionClient(Ros1Clearpath_navigation_msgsMissionClient&&)=delete;
~Ros1Clearpath_navigation_msgsMissionClient(){if(bundle_.active){std::array<uint32_t,5> statuses{};roswasmc_action_bundle_close_checked(&bundle_,0,statuses.data(),statuses.size());}}
uint32_t error() const noexcept{return bundle_.error?bundle_.error:bundle_.active?0:9;}bool is_open() const noexcept{return bundle_.active;}bool owns(uint64_t id)const{return roswasmc_action_bundle_owns(&bundle_,id);}
roswasmc_result send_goal(Goal const& value,uint64_t& id){return roswasmc_bounded_1_clearpath_navigation_msgs_Mission_send(&bundle_,&value,&id);}
roswasmc_result feedback(uint64_t id,Feedback& value){return roswasmc_bounded_1_clearpath_navigation_msgs_Mission_feedback(&bundle_,id,&value);}
roswasmc_result result(uint64_t id,uint8_t& kind,Result& value){return roswasmc_bounded_1_clearpath_navigation_msgs_Mission_result(&bundle_,id,&kind,&value);}
roswasmc_result cancel_own(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_own(&bundle_,id,&accepted);}
roswasmc_result cancel_any(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_any(&bundle_,id,&accepted);}
roswasmc_result cancel_all(int& accepted){return roswasmc_action_bundle_cancel_all(&bundle_,&accepted);}
roswasmc_cleanup_report close_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_close_checked(&bundle_,primary,statuses.data(),statuses.size());}
roswasmc_cleanup_report revoke_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_revoke_checked(&bundle_,primary,statuses.data(),statuses.size());}
};
/** Typed custom clearpath_navigation_msgs/Mission server with explicit feedback/terminal component codecs. */
class Ros1Clearpath_navigation_msgsMissionServer:public ServerOwners {
public:
using Goal=Ros1Clearpath_navigation_msgsMissionGoal;using Result=Ros1Clearpath_navigation_msgsMissionResult;using Feedback=Ros1Clearpath_navigation_msgsMissionFeedback;
/** Handler may choose separate feedback/result effects; kind must be2..4 when terminal. */
struct Replies {Feedback feedback{};Result result{};uint8_t kind{2};bool send_feedback{true},send_result{true};};
private:using Handler=std::function<Replies(uint64_t,Goal const&)>;std::shared_ptr<Handler> handler_;
public:
Ros1Clearpath_navigation_msgsMissionServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,true} {}
void on_goal(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& id,Goal& goal){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_navigation_msgs_Mission_poll(&advertise_,&id,&goal);}
roswasmc_result respond_feedback(uint64_t id,Feedback const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_navigation_msgs_Mission_respond_feedback(&respond_,id,&value);}
roswasmc_result respond_result(uint64_t id,uint8_t kind,Result const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_navigation_msgs_Mission_respond_result(&respond_,id,kind,&value);}
/** One pass, at most one feedback plus one terminal response; never implicit replay. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t id{};Goal goal{};auto r=roswasmc_bounded_1_clearpath_navigation_msgs_Mission_poll(&advertise_,&id,&goal);if(r.error)return r.error;auto replies=(*handler)(id,goal);if(replies.send_result&&(replies.kind<2||replies.kind>4||!roswasmc_bounded_1_clearpath_navigation_msgs_MissionResult_valid(&replies.result)))return 22;if(replies.send_feedback&&!roswasmc_bounded_1_clearpath_navigation_msgs_MissionFeedback_valid(&replies.feedback))return 22;if(replies.send_feedback){r=roswasmc_bounded_1_clearpath_navigation_msgs_Mission_respond_feedback(&respond_,id,&replies.feedback);if(r.error)return r.error;}return replies.send_result?roswasmc_bounded_1_clearpath_navigation_msgs_Mission_respond_result(&respond_,id,replies.kind,&replies.result).error:0;}
};
/** Complete owned custom clearpath_navigation_msgs/NetworkGoToPOI Action client with four separate roles. */
class Ros1Clearpath_navigation_msgsNetworkGoToPOIClient {roswasmc_context context_;std::array<uint64_t,1024> goals_{};roswasmc_action_bundle bundle_{};
public:
using Goal=Ros1Clearpath_navigation_msgsNetworkGoToPOIGoal;using Result=Ros1Clearpath_navigation_msgsNetworkGoToPOIResult;using Feedback=Ros1Clearpath_navigation_msgsNetworkGoToPOIFeedback;
Ros1Clearpath_navigation_msgsNetworkGoToPOIClient(roswasmc_context const& context,roswasmc_action_aliases const& aliases,size_t slots=8):context_{context.backend} {roswasmc_action_bundle_init(&context_,&bundle_,&aliases,goals_.data(),slots);}
Ros1Clearpath_navigation_msgsNetworkGoToPOIClient(Ros1Clearpath_navigation_msgsNetworkGoToPOIClient const&)=delete;Ros1Clearpath_navigation_msgsNetworkGoToPOIClient& operator=(Ros1Clearpath_navigation_msgsNetworkGoToPOIClient const&)=delete;Ros1Clearpath_navigation_msgsNetworkGoToPOIClient(Ros1Clearpath_navigation_msgsNetworkGoToPOIClient&&)=delete;
~Ros1Clearpath_navigation_msgsNetworkGoToPOIClient(){if(bundle_.active){std::array<uint32_t,5> statuses{};roswasmc_action_bundle_close_checked(&bundle_,0,statuses.data(),statuses.size());}}
uint32_t error() const noexcept{return bundle_.error?bundle_.error:bundle_.active?0:9;}bool is_open() const noexcept{return bundle_.active;}bool owns(uint64_t id)const{return roswasmc_action_bundle_owns(&bundle_,id);}
roswasmc_result send_goal(Goal const& value,uint64_t& id){return roswasmc_bounded_1_clearpath_navigation_msgs_NetworkGoToPOI_send(&bundle_,&value,&id);}
roswasmc_result feedback(uint64_t id,Feedback& value){return roswasmc_bounded_1_clearpath_navigation_msgs_NetworkGoToPOI_feedback(&bundle_,id,&value);}
roswasmc_result result(uint64_t id,uint8_t& kind,Result& value){return roswasmc_bounded_1_clearpath_navigation_msgs_NetworkGoToPOI_result(&bundle_,id,&kind,&value);}
roswasmc_result cancel_own(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_own(&bundle_,id,&accepted);}
roswasmc_result cancel_any(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_any(&bundle_,id,&accepted);}
roswasmc_result cancel_all(int& accepted){return roswasmc_action_bundle_cancel_all(&bundle_,&accepted);}
roswasmc_cleanup_report close_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_close_checked(&bundle_,primary,statuses.data(),statuses.size());}
roswasmc_cleanup_report revoke_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_revoke_checked(&bundle_,primary,statuses.data(),statuses.size());}
};
/** Typed custom clearpath_navigation_msgs/NetworkGoToPOI server with explicit feedback/terminal component codecs. */
class Ros1Clearpath_navigation_msgsNetworkGoToPOIServer:public ServerOwners {
public:
using Goal=Ros1Clearpath_navigation_msgsNetworkGoToPOIGoal;using Result=Ros1Clearpath_navigation_msgsNetworkGoToPOIResult;using Feedback=Ros1Clearpath_navigation_msgsNetworkGoToPOIFeedback;
/** Handler may choose separate feedback/result effects; kind must be2..4 when terminal. */
struct Replies {Feedback feedback{};Result result{};uint8_t kind{2};bool send_feedback{true},send_result{true};};
private:using Handler=std::function<Replies(uint64_t,Goal const&)>;std::shared_ptr<Handler> handler_;
public:
Ros1Clearpath_navigation_msgsNetworkGoToPOIServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,true} {}
void on_goal(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& id,Goal& goal){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_navigation_msgs_NetworkGoToPOI_poll(&advertise_,&id,&goal);}
roswasmc_result respond_feedback(uint64_t id,Feedback const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_navigation_msgs_NetworkGoToPOI_respond_feedback(&respond_,id,&value);}
roswasmc_result respond_result(uint64_t id,uint8_t kind,Result const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_navigation_msgs_NetworkGoToPOI_respond_result(&respond_,id,kind,&value);}
/** One pass, at most one feedback plus one terminal response; never implicit replay. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t id{};Goal goal{};auto r=roswasmc_bounded_1_clearpath_navigation_msgs_NetworkGoToPOI_poll(&advertise_,&id,&goal);if(r.error)return r.error;auto replies=(*handler)(id,goal);if(replies.send_result&&(replies.kind<2||replies.kind>4||!roswasmc_bounded_1_clearpath_navigation_msgs_NetworkGoToPOIResult_valid(&replies.result)))return 22;if(replies.send_feedback&&!roswasmc_bounded_1_clearpath_navigation_msgs_NetworkGoToPOIFeedback_valid(&replies.feedback))return 22;if(replies.send_feedback){r=roswasmc_bounded_1_clearpath_navigation_msgs_NetworkGoToPOI_respond_feedback(&respond_,id,&replies.feedback);if(r.error)return r.error;}return replies.send_result?roswasmc_bounded_1_clearpath_navigation_msgs_NetworkGoToPOI_respond_result(&respond_,id,replies.kind,&replies.result).error:0;}
};
/** Complete owned custom clearpath_navigation_msgs/RecordPath Action client with four separate roles. */
class Ros1Clearpath_navigation_msgsRecordPathClient {roswasmc_context context_;std::array<uint64_t,1024> goals_{};roswasmc_action_bundle bundle_{};
public:
using Goal=Ros1Clearpath_navigation_msgsRecordPathGoal;using Result=Ros1Clearpath_navigation_msgsRecordPathResult;using Feedback=Ros1Clearpath_navigation_msgsRecordPathFeedback;
Ros1Clearpath_navigation_msgsRecordPathClient(roswasmc_context const& context,roswasmc_action_aliases const& aliases,size_t slots=8):context_{context.backend} {roswasmc_action_bundle_init(&context_,&bundle_,&aliases,goals_.data(),slots);}
Ros1Clearpath_navigation_msgsRecordPathClient(Ros1Clearpath_navigation_msgsRecordPathClient const&)=delete;Ros1Clearpath_navigation_msgsRecordPathClient& operator=(Ros1Clearpath_navigation_msgsRecordPathClient const&)=delete;Ros1Clearpath_navigation_msgsRecordPathClient(Ros1Clearpath_navigation_msgsRecordPathClient&&)=delete;
~Ros1Clearpath_navigation_msgsRecordPathClient(){if(bundle_.active){std::array<uint32_t,5> statuses{};roswasmc_action_bundle_close_checked(&bundle_,0,statuses.data(),statuses.size());}}
uint32_t error() const noexcept{return bundle_.error?bundle_.error:bundle_.active?0:9;}bool is_open() const noexcept{return bundle_.active;}bool owns(uint64_t id)const{return roswasmc_action_bundle_owns(&bundle_,id);}
roswasmc_result send_goal(Goal const& value,uint64_t& id){return roswasmc_bounded_1_clearpath_navigation_msgs_RecordPath_send(&bundle_,&value,&id);}
roswasmc_result feedback(uint64_t id,Feedback& value){return roswasmc_bounded_1_clearpath_navigation_msgs_RecordPath_feedback(&bundle_,id,&value);}
roswasmc_result result(uint64_t id,uint8_t& kind,Result& value){return roswasmc_bounded_1_clearpath_navigation_msgs_RecordPath_result(&bundle_,id,&kind,&value);}
roswasmc_result cancel_own(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_own(&bundle_,id,&accepted);}
roswasmc_result cancel_any(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_any(&bundle_,id,&accepted);}
roswasmc_result cancel_all(int& accepted){return roswasmc_action_bundle_cancel_all(&bundle_,&accepted);}
roswasmc_cleanup_report close_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_close_checked(&bundle_,primary,statuses.data(),statuses.size());}
roswasmc_cleanup_report revoke_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_revoke_checked(&bundle_,primary,statuses.data(),statuses.size());}
};
/** Typed custom clearpath_navigation_msgs/RecordPath server with explicit feedback/terminal component codecs. */
class Ros1Clearpath_navigation_msgsRecordPathServer:public ServerOwners {
public:
using Goal=Ros1Clearpath_navigation_msgsRecordPathGoal;using Result=Ros1Clearpath_navigation_msgsRecordPathResult;using Feedback=Ros1Clearpath_navigation_msgsRecordPathFeedback;
/** Handler may choose separate feedback/result effects; kind must be2..4 when terminal. */
struct Replies {Feedback feedback{};Result result{};uint8_t kind{2};bool send_feedback{true},send_result{true};};
private:using Handler=std::function<Replies(uint64_t,Goal const&)>;std::shared_ptr<Handler> handler_;
public:
Ros1Clearpath_navigation_msgsRecordPathServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,true} {}
void on_goal(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& id,Goal& goal){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_navigation_msgs_RecordPath_poll(&advertise_,&id,&goal);}
roswasmc_result respond_feedback(uint64_t id,Feedback const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_navigation_msgs_RecordPath_respond_feedback(&respond_,id,&value);}
roswasmc_result respond_result(uint64_t id,uint8_t kind,Result const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_navigation_msgs_RecordPath_respond_result(&respond_,id,kind,&value);}
/** One pass, at most one feedback plus one terminal response; never implicit replay. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t id{};Goal goal{};auto r=roswasmc_bounded_1_clearpath_navigation_msgs_RecordPath_poll(&advertise_,&id,&goal);if(r.error)return r.error;auto replies=(*handler)(id,goal);if(replies.send_result&&(replies.kind<2||replies.kind>4||!roswasmc_bounded_1_clearpath_navigation_msgs_RecordPathResult_valid(&replies.result)))return 22;if(replies.send_feedback&&!roswasmc_bounded_1_clearpath_navigation_msgs_RecordPathFeedback_valid(&replies.feedback))return 22;if(replies.send_feedback){r=roswasmc_bounded_1_clearpath_navigation_msgs_RecordPath_respond_feedback(&respond_,id,&replies.feedback);if(r.error)return r.error;}return replies.send_result?roswasmc_bounded_1_clearpath_navigation_msgs_RecordPath_respond_result(&respond_,id,replies.kind,&replies.result).error:0;}
};
/** Complete owned custom clearpath_navigation_msgs/RunNetworkGoToPlanner Action client with four separate roles. */
class Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerClient {roswasmc_context context_;std::array<uint64_t,1024> goals_{};roswasmc_action_bundle bundle_{};
public:
using Goal=Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerGoal;using Result=Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerResult;using Feedback=Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerFeedback;
Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerClient(roswasmc_context const& context,roswasmc_action_aliases const& aliases,size_t slots=8):context_{context.backend} {roswasmc_action_bundle_init(&context_,&bundle_,&aliases,goals_.data(),slots);}
Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerClient(Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerClient const&)=delete;Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerClient& operator=(Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerClient const&)=delete;Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerClient(Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerClient&&)=delete;
~Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerClient(){if(bundle_.active){std::array<uint32_t,5> statuses{};roswasmc_action_bundle_close_checked(&bundle_,0,statuses.data(),statuses.size());}}
uint32_t error() const noexcept{return bundle_.error?bundle_.error:bundle_.active?0:9;}bool is_open() const noexcept{return bundle_.active;}bool owns(uint64_t id)const{return roswasmc_action_bundle_owns(&bundle_,id);}
roswasmc_result send_goal(Goal const& value,uint64_t& id){return roswasmc_bounded_1_clearpath_navigation_msgs_RunNetworkGoToPlanner_send(&bundle_,&value,&id);}
roswasmc_result feedback(uint64_t id,Feedback& value){return roswasmc_bounded_1_clearpath_navigation_msgs_RunNetworkGoToPlanner_feedback(&bundle_,id,&value);}
roswasmc_result result(uint64_t id,uint8_t& kind,Result& value){return roswasmc_bounded_1_clearpath_navigation_msgs_RunNetworkGoToPlanner_result(&bundle_,id,&kind,&value);}
roswasmc_result cancel_own(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_own(&bundle_,id,&accepted);}
roswasmc_result cancel_any(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_any(&bundle_,id,&accepted);}
roswasmc_result cancel_all(int& accepted){return roswasmc_action_bundle_cancel_all(&bundle_,&accepted);}
roswasmc_cleanup_report close_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_close_checked(&bundle_,primary,statuses.data(),statuses.size());}
roswasmc_cleanup_report revoke_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_revoke_checked(&bundle_,primary,statuses.data(),statuses.size());}
};
/** Typed custom clearpath_navigation_msgs/RunNetworkGoToPlanner server with explicit feedback/terminal component codecs. */
class Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerServer:public ServerOwners {
public:
using Goal=Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerGoal;using Result=Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerResult;using Feedback=Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerFeedback;
/** Handler may choose separate feedback/result effects; kind must be2..4 when terminal. */
struct Replies {Feedback feedback{};Result result{};uint8_t kind{2};bool send_feedback{true},send_result{true};};
private:using Handler=std::function<Replies(uint64_t,Goal const&)>;std::shared_ptr<Handler> handler_;
public:
Ros1Clearpath_navigation_msgsRunNetworkGoToPlannerServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,true} {}
void on_goal(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& id,Goal& goal){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_navigation_msgs_RunNetworkGoToPlanner_poll(&advertise_,&id,&goal);}
roswasmc_result respond_feedback(uint64_t id,Feedback const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_navigation_msgs_RunNetworkGoToPlanner_respond_feedback(&respond_,id,&value);}
roswasmc_result respond_result(uint64_t id,uint8_t kind,Result const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_navigation_msgs_RunNetworkGoToPlanner_respond_result(&respond_,id,kind,&value);}
/** One pass, at most one feedback plus one terminal response; never implicit replay. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t id{};Goal goal{};auto r=roswasmc_bounded_1_clearpath_navigation_msgs_RunNetworkGoToPlanner_poll(&advertise_,&id,&goal);if(r.error)return r.error;auto replies=(*handler)(id,goal);if(replies.send_result&&(replies.kind<2||replies.kind>4||!roswasmc_bounded_1_clearpath_navigation_msgs_RunNetworkGoToPlannerResult_valid(&replies.result)))return 22;if(replies.send_feedback&&!roswasmc_bounded_1_clearpath_navigation_msgs_RunNetworkGoToPlannerFeedback_valid(&replies.feedback))return 22;if(replies.send_feedback){r=roswasmc_bounded_1_clearpath_navigation_msgs_RunNetworkGoToPlanner_respond_feedback(&respond_,id,&replies.feedback);if(r.error)return r.error;}return replies.send_result?roswasmc_bounded_1_clearpath_navigation_msgs_RunNetworkGoToPlanner_respond_result(&respond_,id,replies.kind,&replies.result).error:0;}
};
/** Complete owned custom clearpath_navigation_msgs/RunNetworkMissionPreviewer Action client with four separate roles. */
class Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerClient {roswasmc_context context_;std::array<uint64_t,1024> goals_{};roswasmc_action_bundle bundle_{};
public:
using Goal=Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerGoal;using Result=Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerResult;using Feedback=Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerFeedback;
Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerClient(roswasmc_context const& context,roswasmc_action_aliases const& aliases,size_t slots=8):context_{context.backend} {roswasmc_action_bundle_init(&context_,&bundle_,&aliases,goals_.data(),slots);}
Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerClient(Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerClient const&)=delete;Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerClient& operator=(Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerClient const&)=delete;Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerClient(Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerClient&&)=delete;
~Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerClient(){if(bundle_.active){std::array<uint32_t,5> statuses{};roswasmc_action_bundle_close_checked(&bundle_,0,statuses.data(),statuses.size());}}
uint32_t error() const noexcept{return bundle_.error?bundle_.error:bundle_.active?0:9;}bool is_open() const noexcept{return bundle_.active;}bool owns(uint64_t id)const{return roswasmc_action_bundle_owns(&bundle_,id);}
roswasmc_result send_goal(Goal const& value,uint64_t& id){return roswasmc_bounded_1_clearpath_navigation_msgs_RunNetworkMissionPreviewer_send(&bundle_,&value,&id);}
roswasmc_result feedback(uint64_t id,Feedback& value){return roswasmc_bounded_1_clearpath_navigation_msgs_RunNetworkMissionPreviewer_feedback(&bundle_,id,&value);}
roswasmc_result result(uint64_t id,uint8_t& kind,Result& value){return roswasmc_bounded_1_clearpath_navigation_msgs_RunNetworkMissionPreviewer_result(&bundle_,id,&kind,&value);}
roswasmc_result cancel_own(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_own(&bundle_,id,&accepted);}
roswasmc_result cancel_any(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_any(&bundle_,id,&accepted);}
roswasmc_result cancel_all(int& accepted){return roswasmc_action_bundle_cancel_all(&bundle_,&accepted);}
roswasmc_cleanup_report close_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_close_checked(&bundle_,primary,statuses.data(),statuses.size());}
roswasmc_cleanup_report revoke_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_revoke_checked(&bundle_,primary,statuses.data(),statuses.size());}
};
/** Typed custom clearpath_navigation_msgs/RunNetworkMissionPreviewer server with explicit feedback/terminal component codecs. */
class Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerServer:public ServerOwners {
public:
using Goal=Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerGoal;using Result=Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerResult;using Feedback=Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerFeedback;
/** Handler may choose separate feedback/result effects; kind must be2..4 when terminal. */
struct Replies {Feedback feedback{};Result result{};uint8_t kind{2};bool send_feedback{true},send_result{true};};
private:using Handler=std::function<Replies(uint64_t,Goal const&)>;std::shared_ptr<Handler> handler_;
public:
Ros1Clearpath_navigation_msgsRunNetworkMissionPreviewerServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,true} {}
void on_goal(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& id,Goal& goal){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_navigation_msgs_RunNetworkMissionPreviewer_poll(&advertise_,&id,&goal);}
roswasmc_result respond_feedback(uint64_t id,Feedback const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_navigation_msgs_RunNetworkMissionPreviewer_respond_feedback(&respond_,id,&value);}
roswasmc_result respond_result(uint64_t id,uint8_t kind,Result const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_navigation_msgs_RunNetworkMissionPreviewer_respond_result(&respond_,id,kind,&value);}
/** One pass, at most one feedback plus one terminal response; never implicit replay. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t id{};Goal goal{};auto r=roswasmc_bounded_1_clearpath_navigation_msgs_RunNetworkMissionPreviewer_poll(&advertise_,&id,&goal);if(r.error)return r.error;auto replies=(*handler)(id,goal);if(replies.send_result&&(replies.kind<2||replies.kind>4||!roswasmc_bounded_1_clearpath_navigation_msgs_RunNetworkMissionPreviewerResult_valid(&replies.result)))return 22;if(replies.send_feedback&&!roswasmc_bounded_1_clearpath_navigation_msgs_RunNetworkMissionPreviewerFeedback_valid(&replies.feedback))return 22;if(replies.send_feedback){r=roswasmc_bounded_1_clearpath_navigation_msgs_RunNetworkMissionPreviewer_respond_feedback(&respond_,id,&replies.feedback);if(r.error)return r.error;}return replies.send_result?roswasmc_bounded_1_clearpath_navigation_msgs_RunNetworkMissionPreviewer_respond_result(&respond_,id,replies.kind,&replies.result).error:0;}
};
/** Complete owned custom clearpath_navigation_msgs/SmoothPath Action client with four separate roles. */
class Ros1Clearpath_navigation_msgsSmoothPathClient {roswasmc_context context_;std::array<uint64_t,1024> goals_{};roswasmc_action_bundle bundle_{};
public:
using Goal=Ros1Clearpath_navigation_msgsSmoothPathGoal;using Result=Ros1Clearpath_navigation_msgsSmoothPathResult;using Feedback=Ros1Clearpath_navigation_msgsSmoothPathFeedback;
Ros1Clearpath_navigation_msgsSmoothPathClient(roswasmc_context const& context,roswasmc_action_aliases const& aliases,size_t slots=8):context_{context.backend} {roswasmc_action_bundle_init(&context_,&bundle_,&aliases,goals_.data(),slots);}
Ros1Clearpath_navigation_msgsSmoothPathClient(Ros1Clearpath_navigation_msgsSmoothPathClient const&)=delete;Ros1Clearpath_navigation_msgsSmoothPathClient& operator=(Ros1Clearpath_navigation_msgsSmoothPathClient const&)=delete;Ros1Clearpath_navigation_msgsSmoothPathClient(Ros1Clearpath_navigation_msgsSmoothPathClient&&)=delete;
~Ros1Clearpath_navigation_msgsSmoothPathClient(){if(bundle_.active){std::array<uint32_t,5> statuses{};roswasmc_action_bundle_close_checked(&bundle_,0,statuses.data(),statuses.size());}}
uint32_t error() const noexcept{return bundle_.error?bundle_.error:bundle_.active?0:9;}bool is_open() const noexcept{return bundle_.active;}bool owns(uint64_t id)const{return roswasmc_action_bundle_owns(&bundle_,id);}
roswasmc_result send_goal(Goal const& value,uint64_t& id){return roswasmc_bounded_1_clearpath_navigation_msgs_SmoothPath_send(&bundle_,&value,&id);}
roswasmc_result feedback(uint64_t id,Feedback& value){return roswasmc_bounded_1_clearpath_navigation_msgs_SmoothPath_feedback(&bundle_,id,&value);}
roswasmc_result result(uint64_t id,uint8_t& kind,Result& value){return roswasmc_bounded_1_clearpath_navigation_msgs_SmoothPath_result(&bundle_,id,&kind,&value);}
roswasmc_result cancel_own(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_own(&bundle_,id,&accepted);}
roswasmc_result cancel_any(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_any(&bundle_,id,&accepted);}
roswasmc_result cancel_all(int& accepted){return roswasmc_action_bundle_cancel_all(&bundle_,&accepted);}
roswasmc_cleanup_report close_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_close_checked(&bundle_,primary,statuses.data(),statuses.size());}
roswasmc_cleanup_report revoke_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_revoke_checked(&bundle_,primary,statuses.data(),statuses.size());}
};
/** Typed custom clearpath_navigation_msgs/SmoothPath server with explicit feedback/terminal component codecs. */
class Ros1Clearpath_navigation_msgsSmoothPathServer:public ServerOwners {
public:
using Goal=Ros1Clearpath_navigation_msgsSmoothPathGoal;using Result=Ros1Clearpath_navigation_msgsSmoothPathResult;using Feedback=Ros1Clearpath_navigation_msgsSmoothPathFeedback;
/** Handler may choose separate feedback/result effects; kind must be2..4 when terminal. */
struct Replies {Feedback feedback{};Result result{};uint8_t kind{2};bool send_feedback{true},send_result{true};};
private:using Handler=std::function<Replies(uint64_t,Goal const&)>;std::shared_ptr<Handler> handler_;
public:
Ros1Clearpath_navigation_msgsSmoothPathServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,true} {}
void on_goal(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& id,Goal& goal){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_navigation_msgs_SmoothPath_poll(&advertise_,&id,&goal);}
roswasmc_result respond_feedback(uint64_t id,Feedback const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_navigation_msgs_SmoothPath_respond_feedback(&respond_,id,&value);}
roswasmc_result respond_result(uint64_t id,uint8_t kind,Result const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_navigation_msgs_SmoothPath_respond_result(&respond_,id,kind,&value);}
/** One pass, at most one feedback plus one terminal response; never implicit replay. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t id{};Goal goal{};auto r=roswasmc_bounded_1_clearpath_navigation_msgs_SmoothPath_poll(&advertise_,&id,&goal);if(r.error)return r.error;auto replies=(*handler)(id,goal);if(replies.send_result&&(replies.kind<2||replies.kind>4||!roswasmc_bounded_1_clearpath_navigation_msgs_SmoothPathResult_valid(&replies.result)))return 22;if(replies.send_feedback&&!roswasmc_bounded_1_clearpath_navigation_msgs_SmoothPathFeedback_valid(&replies.feedback))return 22;if(replies.send_feedback){r=roswasmc_bounded_1_clearpath_navigation_msgs_SmoothPath_respond_feedback(&respond_,id,&replies.feedback);if(r.error)return r.error;}return replies.send_result?roswasmc_bounded_1_clearpath_navigation_msgs_SmoothPath_respond_result(&respond_,id,replies.kind,&replies.result).error:0;}
};
/** Complete owned custom clearpath_navigation_msgs/UITask Action client with four separate roles. */
class Ros1Clearpath_navigation_msgsUITaskClient {roswasmc_context context_;std::array<uint64_t,1024> goals_{};roswasmc_action_bundle bundle_{};
public:
using Goal=Ros1Clearpath_navigation_msgsUITaskGoal;using Result=Ros1Clearpath_navigation_msgsUITaskResult;using Feedback=Ros1Clearpath_navigation_msgsUITaskFeedback;
Ros1Clearpath_navigation_msgsUITaskClient(roswasmc_context const& context,roswasmc_action_aliases const& aliases,size_t slots=8):context_{context.backend} {roswasmc_action_bundle_init(&context_,&bundle_,&aliases,goals_.data(),slots);}
Ros1Clearpath_navigation_msgsUITaskClient(Ros1Clearpath_navigation_msgsUITaskClient const&)=delete;Ros1Clearpath_navigation_msgsUITaskClient& operator=(Ros1Clearpath_navigation_msgsUITaskClient const&)=delete;Ros1Clearpath_navigation_msgsUITaskClient(Ros1Clearpath_navigation_msgsUITaskClient&&)=delete;
~Ros1Clearpath_navigation_msgsUITaskClient(){if(bundle_.active){std::array<uint32_t,5> statuses{};roswasmc_action_bundle_close_checked(&bundle_,0,statuses.data(),statuses.size());}}
uint32_t error() const noexcept{return bundle_.error?bundle_.error:bundle_.active?0:9;}bool is_open() const noexcept{return bundle_.active;}bool owns(uint64_t id)const{return roswasmc_action_bundle_owns(&bundle_,id);}
roswasmc_result send_goal(Goal const& value,uint64_t& id){return roswasmc_bounded_1_clearpath_navigation_msgs_UITask_send(&bundle_,&value,&id);}
roswasmc_result feedback(uint64_t id,Feedback& value){return roswasmc_bounded_1_clearpath_navigation_msgs_UITask_feedback(&bundle_,id,&value);}
roswasmc_result result(uint64_t id,uint8_t& kind,Result& value){return roswasmc_bounded_1_clearpath_navigation_msgs_UITask_result(&bundle_,id,&kind,&value);}
roswasmc_result cancel_own(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_own(&bundle_,id,&accepted);}
roswasmc_result cancel_any(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_any(&bundle_,id,&accepted);}
roswasmc_result cancel_all(int& accepted){return roswasmc_action_bundle_cancel_all(&bundle_,&accepted);}
roswasmc_cleanup_report close_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_close_checked(&bundle_,primary,statuses.data(),statuses.size());}
roswasmc_cleanup_report revoke_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_revoke_checked(&bundle_,primary,statuses.data(),statuses.size());}
};
/** Typed custom clearpath_navigation_msgs/UITask server with explicit feedback/terminal component codecs. */
class Ros1Clearpath_navigation_msgsUITaskServer:public ServerOwners {
public:
using Goal=Ros1Clearpath_navigation_msgsUITaskGoal;using Result=Ros1Clearpath_navigation_msgsUITaskResult;using Feedback=Ros1Clearpath_navigation_msgsUITaskFeedback;
/** Handler may choose separate feedback/result effects; kind must be2..4 when terminal. */
struct Replies {Feedback feedback{};Result result{};uint8_t kind{2};bool send_feedback{true},send_result{true};};
private:using Handler=std::function<Replies(uint64_t,Goal const&)>;std::shared_ptr<Handler> handler_;
public:
Ros1Clearpath_navigation_msgsUITaskServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,true} {}
void on_goal(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& id,Goal& goal){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_navigation_msgs_UITask_poll(&advertise_,&id,&goal);}
roswasmc_result respond_feedback(uint64_t id,Feedback const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_navigation_msgs_UITask_respond_feedback(&respond_,id,&value);}
roswasmc_result respond_result(uint64_t id,uint8_t kind,Result const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_navigation_msgs_UITask_respond_result(&respond_,id,kind,&value);}
/** One pass, at most one feedback plus one terminal response; never implicit replay. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t id{};Goal goal{};auto r=roswasmc_bounded_1_clearpath_navigation_msgs_UITask_poll(&advertise_,&id,&goal);if(r.error)return r.error;auto replies=(*handler)(id,goal);if(replies.send_result&&(replies.kind<2||replies.kind>4||!roswasmc_bounded_1_clearpath_navigation_msgs_UITaskResult_valid(&replies.result)))return 22;if(replies.send_feedback&&!roswasmc_bounded_1_clearpath_navigation_msgs_UITaskFeedback_valid(&replies.feedback))return 22;if(replies.send_feedback){r=roswasmc_bounded_1_clearpath_navigation_msgs_UITask_respond_feedback(&respond_,id,&replies.feedback);if(r.error)return r.error;}return replies.send_result?roswasmc_bounded_1_clearpath_navigation_msgs_UITask_respond_result(&respond_,id,replies.kind,&replies.result).error:0;}
};
/** Complete custom clearpath_camera_msgs/q62AreaZoom client; default policy returns13 from construction. */
class Ros1Clearpath_camera_msgsQ62AreaZoomClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_camera_msgsQ62AreaZoomRequest;using Response=Ros1Clearpath_camera_msgsQ62AreaZoomResponse;
Ros1Clearpath_camera_msgsQ62AreaZoomClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_camera_msgsQ62AreaZoomClient(Ros1Clearpath_camera_msgsQ62AreaZoomClient const&)=delete;Ros1Clearpath_camera_msgsQ62AreaZoomClient& operator=(Ros1Clearpath_camera_msgsQ62AreaZoomClient const&)=delete;Ros1Clearpath_camera_msgsQ62AreaZoomClient(Ros1Clearpath_camera_msgsQ62AreaZoomClient&&)=delete;
~Ros1Clearpath_camera_msgsQ62AreaZoomClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_camera_msgs_q62AreaZoom_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_camera_msgs/q62AreaZoom server; advertise/respond stay separate authorities. */
class Ros1Clearpath_camera_msgsQ62AreaZoomServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_camera_msgsQ62AreaZoomResponse(Ros1Clearpath_camera_msgsQ62AreaZoomRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_camera_msgsQ62AreaZoomRequest;using Response=Ros1Clearpath_camera_msgsQ62AreaZoomResponse;
Ros1Clearpath_camera_msgsQ62AreaZoomServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_camera_msgs_q62AreaZoom_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_camera_msgs_q62AreaZoom_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_camera_msgs_q62AreaZoom_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_camera_msgs_q62AreaZoom_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_control_msgs/SetControlMode client; default policy returns13 from construction. */
class Ros1Clearpath_control_msgsSetControlModeClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_control_msgsSetControlModeRequest;using Response=Ros1Clearpath_control_msgsSetControlModeResponse;
Ros1Clearpath_control_msgsSetControlModeClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_control_msgsSetControlModeClient(Ros1Clearpath_control_msgsSetControlModeClient const&)=delete;Ros1Clearpath_control_msgsSetControlModeClient& operator=(Ros1Clearpath_control_msgsSetControlModeClient const&)=delete;Ros1Clearpath_control_msgsSetControlModeClient(Ros1Clearpath_control_msgsSetControlModeClient&&)=delete;
~Ros1Clearpath_control_msgsSetControlModeClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_control_msgs_SetControlMode_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_control_msgs/SetControlMode server; advertise/respond stay separate authorities. */
class Ros1Clearpath_control_msgsSetControlModeServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_control_msgsSetControlModeResponse(Ros1Clearpath_control_msgsSetControlModeRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_control_msgsSetControlModeRequest;using Response=Ros1Clearpath_control_msgsSetControlModeResponse;
Ros1Clearpath_control_msgsSetControlModeServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_control_msgs_SetControlMode_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_control_msgs_SetControlMode_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_control_msgs_SetControlMode_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_control_msgs_SetControlMode_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_dock_msgs/AddDock client; default policy returns13 from construction. */
class Ros1Clearpath_dock_msgsAddDockClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_dock_msgsAddDockRequest;using Response=Ros1Clearpath_dock_msgsAddDockResponse;
Ros1Clearpath_dock_msgsAddDockClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_dock_msgsAddDockClient(Ros1Clearpath_dock_msgsAddDockClient const&)=delete;Ros1Clearpath_dock_msgsAddDockClient& operator=(Ros1Clearpath_dock_msgsAddDockClient const&)=delete;Ros1Clearpath_dock_msgsAddDockClient(Ros1Clearpath_dock_msgsAddDockClient&&)=delete;
~Ros1Clearpath_dock_msgsAddDockClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_dock_msgs_AddDock_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_dock_msgs/AddDock server; advertise/respond stay separate authorities. */
class Ros1Clearpath_dock_msgsAddDockServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_dock_msgsAddDockResponse(Ros1Clearpath_dock_msgsAddDockRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_dock_msgsAddDockRequest;using Response=Ros1Clearpath_dock_msgsAddDockResponse;
Ros1Clearpath_dock_msgsAddDockServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_dock_msgs_AddDock_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_dock_msgs_AddDock_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_dock_msgs_AddDock_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_dock_msgs_AddDock_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_dock_msgs/ExportData client; default policy returns13 from construction. */
class Ros1Clearpath_dock_msgsExportDataClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_dock_msgsExportDataRequest;using Response=Ros1Clearpath_dock_msgsExportDataResponse;
Ros1Clearpath_dock_msgsExportDataClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_dock_msgsExportDataClient(Ros1Clearpath_dock_msgsExportDataClient const&)=delete;Ros1Clearpath_dock_msgsExportDataClient& operator=(Ros1Clearpath_dock_msgsExportDataClient const&)=delete;Ros1Clearpath_dock_msgsExportDataClient(Ros1Clearpath_dock_msgsExportDataClient&&)=delete;
~Ros1Clearpath_dock_msgsExportDataClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_dock_msgs_ExportData_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_dock_msgs/ExportData server; advertise/respond stay separate authorities. */
class Ros1Clearpath_dock_msgsExportDataServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_dock_msgsExportDataResponse(Ros1Clearpath_dock_msgsExportDataRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_dock_msgsExportDataRequest;using Response=Ros1Clearpath_dock_msgsExportDataResponse;
Ros1Clearpath_dock_msgsExportDataServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_dock_msgs_ExportData_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_dock_msgs_ExportData_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_dock_msgs_ExportData_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_dock_msgs_ExportData_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_dock_msgs/GetDockByUuid client; default policy returns13 from construction. */
class Ros1Clearpath_dock_msgsGetDockByUuidClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_dock_msgsGetDockByUuidRequest;using Response=Ros1Clearpath_dock_msgsGetDockByUuidResponse;
Ros1Clearpath_dock_msgsGetDockByUuidClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_dock_msgsGetDockByUuidClient(Ros1Clearpath_dock_msgsGetDockByUuidClient const&)=delete;Ros1Clearpath_dock_msgsGetDockByUuidClient& operator=(Ros1Clearpath_dock_msgsGetDockByUuidClient const&)=delete;Ros1Clearpath_dock_msgsGetDockByUuidClient(Ros1Clearpath_dock_msgsGetDockByUuidClient&&)=delete;
~Ros1Clearpath_dock_msgsGetDockByUuidClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_dock_msgs_GetDockByUuid_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_dock_msgs/GetDockByUuid server; advertise/respond stay separate authorities. */
class Ros1Clearpath_dock_msgsGetDockByUuidServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_dock_msgsGetDockByUuidResponse(Ros1Clearpath_dock_msgsGetDockByUuidRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_dock_msgsGetDockByUuidRequest;using Response=Ros1Clearpath_dock_msgsGetDockByUuidResponse;
Ros1Clearpath_dock_msgsGetDockByUuidServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_dock_msgs_GetDockByUuid_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_dock_msgs_GetDockByUuid_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_dock_msgs_GetDockByUuid_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_dock_msgs_GetDockByUuid_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_dock_msgs/ImportData client; default policy returns13 from construction. */
class Ros1Clearpath_dock_msgsImportDataClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_dock_msgsImportDataRequest;using Response=Ros1Clearpath_dock_msgsImportDataResponse;
Ros1Clearpath_dock_msgsImportDataClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_dock_msgsImportDataClient(Ros1Clearpath_dock_msgsImportDataClient const&)=delete;Ros1Clearpath_dock_msgsImportDataClient& operator=(Ros1Clearpath_dock_msgsImportDataClient const&)=delete;Ros1Clearpath_dock_msgsImportDataClient(Ros1Clearpath_dock_msgsImportDataClient&&)=delete;
~Ros1Clearpath_dock_msgsImportDataClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_dock_msgs_ImportData_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_dock_msgs/ImportData server; advertise/respond stay separate authorities. */
class Ros1Clearpath_dock_msgsImportDataServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_dock_msgsImportDataResponse(Ros1Clearpath_dock_msgsImportDataRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_dock_msgsImportDataRequest;using Response=Ros1Clearpath_dock_msgsImportDataResponse;
Ros1Clearpath_dock_msgsImportDataServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_dock_msgs_ImportData_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_dock_msgs_ImportData_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_dock_msgs_ImportData_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_dock_msgs_ImportData_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_dock_msgs/RemoveDock client; default policy returns13 from construction. */
class Ros1Clearpath_dock_msgsRemoveDockClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_dock_msgsRemoveDockRequest;using Response=Ros1Clearpath_dock_msgsRemoveDockResponse;
Ros1Clearpath_dock_msgsRemoveDockClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_dock_msgsRemoveDockClient(Ros1Clearpath_dock_msgsRemoveDockClient const&)=delete;Ros1Clearpath_dock_msgsRemoveDockClient& operator=(Ros1Clearpath_dock_msgsRemoveDockClient const&)=delete;Ros1Clearpath_dock_msgsRemoveDockClient(Ros1Clearpath_dock_msgsRemoveDockClient&&)=delete;
~Ros1Clearpath_dock_msgsRemoveDockClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_dock_msgs_RemoveDock_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_dock_msgs/RemoveDock server; advertise/respond stay separate authorities. */
class Ros1Clearpath_dock_msgsRemoveDockServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_dock_msgsRemoveDockResponse(Ros1Clearpath_dock_msgsRemoveDockRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_dock_msgsRemoveDockRequest;using Response=Ros1Clearpath_dock_msgsRemoveDockResponse;
Ros1Clearpath_dock_msgsRemoveDockServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_dock_msgs_RemoveDock_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_dock_msgs_RemoveDock_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_dock_msgs_RemoveDock_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_dock_msgs_RemoveDock_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_dock_msgs/SetDockLocationById client; default policy returns13 from construction. */
class Ros1Clearpath_dock_msgsSetDockLocationByIdClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_dock_msgsSetDockLocationByIdRequest;using Response=Ros1Clearpath_dock_msgsSetDockLocationByIdResponse;
Ros1Clearpath_dock_msgsSetDockLocationByIdClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_dock_msgsSetDockLocationByIdClient(Ros1Clearpath_dock_msgsSetDockLocationByIdClient const&)=delete;Ros1Clearpath_dock_msgsSetDockLocationByIdClient& operator=(Ros1Clearpath_dock_msgsSetDockLocationByIdClient const&)=delete;Ros1Clearpath_dock_msgsSetDockLocationByIdClient(Ros1Clearpath_dock_msgsSetDockLocationByIdClient&&)=delete;
~Ros1Clearpath_dock_msgsSetDockLocationByIdClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_dock_msgs_SetDockLocationById_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_dock_msgs/SetDockLocationById server; advertise/respond stay separate authorities. */
class Ros1Clearpath_dock_msgsSetDockLocationByIdServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_dock_msgsSetDockLocationByIdResponse(Ros1Clearpath_dock_msgsSetDockLocationByIdRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_dock_msgsSetDockLocationByIdRequest;using Response=Ros1Clearpath_dock_msgsSetDockLocationByIdResponse;
Ros1Clearpath_dock_msgsSetDockLocationByIdServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_dock_msgs_SetDockLocationById_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_dock_msgs_SetDockLocationById_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_dock_msgs_SetDockLocationById_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_dock_msgs_SetDockLocationById_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_dock_msgs/SetDockLocationByName client; default policy returns13 from construction. */
class Ros1Clearpath_dock_msgsSetDockLocationByNameClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_dock_msgsSetDockLocationByNameRequest;using Response=Ros1Clearpath_dock_msgsSetDockLocationByNameResponse;
Ros1Clearpath_dock_msgsSetDockLocationByNameClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_dock_msgsSetDockLocationByNameClient(Ros1Clearpath_dock_msgsSetDockLocationByNameClient const&)=delete;Ros1Clearpath_dock_msgsSetDockLocationByNameClient& operator=(Ros1Clearpath_dock_msgsSetDockLocationByNameClient const&)=delete;Ros1Clearpath_dock_msgsSetDockLocationByNameClient(Ros1Clearpath_dock_msgsSetDockLocationByNameClient&&)=delete;
~Ros1Clearpath_dock_msgsSetDockLocationByNameClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_dock_msgs_SetDockLocationByName_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_dock_msgs/SetDockLocationByName server; advertise/respond stay separate authorities. */
class Ros1Clearpath_dock_msgsSetDockLocationByNameServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_dock_msgsSetDockLocationByNameResponse(Ros1Clearpath_dock_msgsSetDockLocationByNameRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_dock_msgsSetDockLocationByNameRequest;using Response=Ros1Clearpath_dock_msgsSetDockLocationByNameResponse;
Ros1Clearpath_dock_msgsSetDockLocationByNameServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_dock_msgs_SetDockLocationByName_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_dock_msgs_SetDockLocationByName_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_dock_msgs_SetDockLocationByName_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_dock_msgs_SetDockLocationByName_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_localization_msgs/ConvertCartesianToLatLon client; default policy returns13 from construction. */
class Ros1Clearpath_localization_msgsConvertCartesianToLatLonClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_localization_msgsConvertCartesianToLatLonRequest;using Response=Ros1Clearpath_localization_msgsConvertCartesianToLatLonResponse;
Ros1Clearpath_localization_msgsConvertCartesianToLatLonClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_localization_msgsConvertCartesianToLatLonClient(Ros1Clearpath_localization_msgsConvertCartesianToLatLonClient const&)=delete;Ros1Clearpath_localization_msgsConvertCartesianToLatLonClient& operator=(Ros1Clearpath_localization_msgsConvertCartesianToLatLonClient const&)=delete;Ros1Clearpath_localization_msgsConvertCartesianToLatLonClient(Ros1Clearpath_localization_msgsConvertCartesianToLatLonClient&&)=delete;
~Ros1Clearpath_localization_msgsConvertCartesianToLatLonClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_localization_msgs_ConvertCartesianToLatLon_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_localization_msgs/ConvertCartesianToLatLon server; advertise/respond stay separate authorities. */
class Ros1Clearpath_localization_msgsConvertCartesianToLatLonServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_localization_msgsConvertCartesianToLatLonResponse(Ros1Clearpath_localization_msgsConvertCartesianToLatLonRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_localization_msgsConvertCartesianToLatLonRequest;using Response=Ros1Clearpath_localization_msgsConvertCartesianToLatLonResponse;
Ros1Clearpath_localization_msgsConvertCartesianToLatLonServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_localization_msgs_ConvertCartesianToLatLon_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_localization_msgs_ConvertCartesianToLatLon_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_localization_msgs_ConvertCartesianToLatLon_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_localization_msgs_ConvertCartesianToLatLon_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_localization_msgs/ConvertCartesianToLatLonArray client; default policy returns13 from construction. */
class Ros1Clearpath_localization_msgsConvertCartesianToLatLonArrayClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_localization_msgsConvertCartesianToLatLonArrayRequest;using Response=Ros1Clearpath_localization_msgsConvertCartesianToLatLonArrayResponse;
Ros1Clearpath_localization_msgsConvertCartesianToLatLonArrayClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_localization_msgsConvertCartesianToLatLonArrayClient(Ros1Clearpath_localization_msgsConvertCartesianToLatLonArrayClient const&)=delete;Ros1Clearpath_localization_msgsConvertCartesianToLatLonArrayClient& operator=(Ros1Clearpath_localization_msgsConvertCartesianToLatLonArrayClient const&)=delete;Ros1Clearpath_localization_msgsConvertCartesianToLatLonArrayClient(Ros1Clearpath_localization_msgsConvertCartesianToLatLonArrayClient&&)=delete;
~Ros1Clearpath_localization_msgsConvertCartesianToLatLonArrayClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_localization_msgs_ConvertCartesianToLatLonArray_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_localization_msgs/ConvertCartesianToLatLonArray server; advertise/respond stay separate authorities. */
class Ros1Clearpath_localization_msgsConvertCartesianToLatLonArrayServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_localization_msgsConvertCartesianToLatLonArrayResponse(Ros1Clearpath_localization_msgsConvertCartesianToLatLonArrayRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_localization_msgsConvertCartesianToLatLonArrayRequest;using Response=Ros1Clearpath_localization_msgsConvertCartesianToLatLonArrayResponse;
Ros1Clearpath_localization_msgsConvertCartesianToLatLonArrayServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_localization_msgs_ConvertCartesianToLatLonArray_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_localization_msgs_ConvertCartesianToLatLonArray_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_localization_msgs_ConvertCartesianToLatLonArray_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_localization_msgs_ConvertCartesianToLatLonArray_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_localization_msgs/ConvertLatLonToCartesian client; default policy returns13 from construction. */
class Ros1Clearpath_localization_msgsConvertLatLonToCartesianClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_localization_msgsConvertLatLonToCartesianRequest;using Response=Ros1Clearpath_localization_msgsConvertLatLonToCartesianResponse;
Ros1Clearpath_localization_msgsConvertLatLonToCartesianClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_localization_msgsConvertLatLonToCartesianClient(Ros1Clearpath_localization_msgsConvertLatLonToCartesianClient const&)=delete;Ros1Clearpath_localization_msgsConvertLatLonToCartesianClient& operator=(Ros1Clearpath_localization_msgsConvertLatLonToCartesianClient const&)=delete;Ros1Clearpath_localization_msgsConvertLatLonToCartesianClient(Ros1Clearpath_localization_msgsConvertLatLonToCartesianClient&&)=delete;
~Ros1Clearpath_localization_msgsConvertLatLonToCartesianClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_localization_msgs_ConvertLatLonToCartesian_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_localization_msgs/ConvertLatLonToCartesian server; advertise/respond stay separate authorities. */
class Ros1Clearpath_localization_msgsConvertLatLonToCartesianServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_localization_msgsConvertLatLonToCartesianResponse(Ros1Clearpath_localization_msgsConvertLatLonToCartesianRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_localization_msgsConvertLatLonToCartesianRequest;using Response=Ros1Clearpath_localization_msgsConvertLatLonToCartesianResponse;
Ros1Clearpath_localization_msgsConvertLatLonToCartesianServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_localization_msgs_ConvertLatLonToCartesian_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_localization_msgs_ConvertLatLonToCartesian_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_localization_msgs_ConvertLatLonToCartesian_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_localization_msgs_ConvertLatLonToCartesian_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_localization_msgs/ConvertLatLonToCartesianArray client; default policy returns13 from construction. */
class Ros1Clearpath_localization_msgsConvertLatLonToCartesianArrayClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_localization_msgsConvertLatLonToCartesianArrayRequest;using Response=Ros1Clearpath_localization_msgsConvertLatLonToCartesianArrayResponse;
Ros1Clearpath_localization_msgsConvertLatLonToCartesianArrayClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_localization_msgsConvertLatLonToCartesianArrayClient(Ros1Clearpath_localization_msgsConvertLatLonToCartesianArrayClient const&)=delete;Ros1Clearpath_localization_msgsConvertLatLonToCartesianArrayClient& operator=(Ros1Clearpath_localization_msgsConvertLatLonToCartesianArrayClient const&)=delete;Ros1Clearpath_localization_msgsConvertLatLonToCartesianArrayClient(Ros1Clearpath_localization_msgsConvertLatLonToCartesianArrayClient&&)=delete;
~Ros1Clearpath_localization_msgsConvertLatLonToCartesianArrayClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_localization_msgs_ConvertLatLonToCartesianArray_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_localization_msgs/ConvertLatLonToCartesianArray server; advertise/respond stay separate authorities. */
class Ros1Clearpath_localization_msgsConvertLatLonToCartesianArrayServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_localization_msgsConvertLatLonToCartesianArrayResponse(Ros1Clearpath_localization_msgsConvertLatLonToCartesianArrayRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_localization_msgsConvertLatLonToCartesianArrayRequest;using Response=Ros1Clearpath_localization_msgsConvertLatLonToCartesianArrayResponse;
Ros1Clearpath_localization_msgsConvertLatLonToCartesianArrayServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_localization_msgs_ConvertLatLonToCartesianArray_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_localization_msgs_ConvertLatLonToCartesianArray_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_localization_msgs_ConvertLatLonToCartesianArray_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_localization_msgs_ConvertLatLonToCartesianArray_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_localization_msgs/ResetLocalization client; default policy returns13 from construction. */
class Ros1Clearpath_localization_msgsResetLocalizationClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_localization_msgsResetLocalizationRequest;using Response=Ros1Clearpath_localization_msgsResetLocalizationResponse;
Ros1Clearpath_localization_msgsResetLocalizationClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_localization_msgsResetLocalizationClient(Ros1Clearpath_localization_msgsResetLocalizationClient const&)=delete;Ros1Clearpath_localization_msgsResetLocalizationClient& operator=(Ros1Clearpath_localization_msgsResetLocalizationClient const&)=delete;Ros1Clearpath_localization_msgsResetLocalizationClient(Ros1Clearpath_localization_msgsResetLocalizationClient&&)=delete;
~Ros1Clearpath_localization_msgsResetLocalizationClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_localization_msgs_ResetLocalization_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_localization_msgs/ResetLocalization server; advertise/respond stay separate authorities. */
class Ros1Clearpath_localization_msgsResetLocalizationServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_localization_msgsResetLocalizationResponse(Ros1Clearpath_localization_msgsResetLocalizationRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_localization_msgsResetLocalizationRequest;using Response=Ros1Clearpath_localization_msgsResetLocalizationResponse;
Ros1Clearpath_localization_msgsResetLocalizationServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_localization_msgs_ResetLocalization_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_localization_msgs_ResetLocalization_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_localization_msgs_ResetLocalization_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_localization_msgs_ResetLocalization_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_localization_msgs/SetDatum client; default policy returns13 from construction. */
class Ros1Clearpath_localization_msgsSetDatumClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_localization_msgsSetDatumRequest;using Response=Ros1Clearpath_localization_msgsSetDatumResponse;
Ros1Clearpath_localization_msgsSetDatumClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_localization_msgsSetDatumClient(Ros1Clearpath_localization_msgsSetDatumClient const&)=delete;Ros1Clearpath_localization_msgsSetDatumClient& operator=(Ros1Clearpath_localization_msgsSetDatumClient const&)=delete;Ros1Clearpath_localization_msgsSetDatumClient(Ros1Clearpath_localization_msgsSetDatumClient&&)=delete;
~Ros1Clearpath_localization_msgsSetDatumClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_localization_msgs_SetDatum_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_localization_msgs/SetDatum server; advertise/respond stay separate authorities. */
class Ros1Clearpath_localization_msgsSetDatumServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_localization_msgsSetDatumResponse(Ros1Clearpath_localization_msgsSetDatumRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_localization_msgsSetDatumRequest;using Response=Ros1Clearpath_localization_msgsSetDatumResponse;
Ros1Clearpath_localization_msgsSetDatumServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_localization_msgs_SetDatum_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_localization_msgs_SetDatum_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_localization_msgs_SetDatum_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_localization_msgs_SetDatum_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_logger_msgs/DeleteLog client; default policy returns13 from construction. */
class Ros1Clearpath_logger_msgsDeleteLogClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_logger_msgsDeleteLogRequest;using Response=Ros1Clearpath_logger_msgsDeleteLogResponse;
Ros1Clearpath_logger_msgsDeleteLogClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_logger_msgsDeleteLogClient(Ros1Clearpath_logger_msgsDeleteLogClient const&)=delete;Ros1Clearpath_logger_msgsDeleteLogClient& operator=(Ros1Clearpath_logger_msgsDeleteLogClient const&)=delete;Ros1Clearpath_logger_msgsDeleteLogClient(Ros1Clearpath_logger_msgsDeleteLogClient&&)=delete;
~Ros1Clearpath_logger_msgsDeleteLogClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_logger_msgs_DeleteLog_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_logger_msgs/DeleteLog server; advertise/respond stay separate authorities. */
class Ros1Clearpath_logger_msgsDeleteLogServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_logger_msgsDeleteLogResponse(Ros1Clearpath_logger_msgsDeleteLogRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_logger_msgsDeleteLogRequest;using Response=Ros1Clearpath_logger_msgsDeleteLogResponse;
Ros1Clearpath_logger_msgsDeleteLogServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_logger_msgs_DeleteLog_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_logger_msgs_DeleteLog_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_logger_msgs_DeleteLog_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_logger_msgs_DeleteLog_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_logger_msgs/DownloadLog client; default policy returns13 from construction. */
class Ros1Clearpath_logger_msgsDownloadLogClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_logger_msgsDownloadLogRequest;using Response=Ros1Clearpath_logger_msgsDownloadLogResponse;
Ros1Clearpath_logger_msgsDownloadLogClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_logger_msgsDownloadLogClient(Ros1Clearpath_logger_msgsDownloadLogClient const&)=delete;Ros1Clearpath_logger_msgsDownloadLogClient& operator=(Ros1Clearpath_logger_msgsDownloadLogClient const&)=delete;Ros1Clearpath_logger_msgsDownloadLogClient(Ros1Clearpath_logger_msgsDownloadLogClient&&)=delete;
~Ros1Clearpath_logger_msgsDownloadLogClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_logger_msgs_DownloadLog_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_logger_msgs/DownloadLog server; advertise/respond stay separate authorities. */
class Ros1Clearpath_logger_msgsDownloadLogServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_logger_msgsDownloadLogResponse(Ros1Clearpath_logger_msgsDownloadLogRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_logger_msgsDownloadLogRequest;using Response=Ros1Clearpath_logger_msgsDownloadLogResponse;
Ros1Clearpath_logger_msgsDownloadLogServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_logger_msgs_DownloadLog_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_logger_msgs_DownloadLog_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_logger_msgs_DownloadLog_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_logger_msgs_DownloadLog_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_logger_msgs/GetAllLogs client; default policy returns13 from construction. */
class Ros1Clearpath_logger_msgsGetAllLogsClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_logger_msgsGetAllLogsRequest;using Response=Ros1Clearpath_logger_msgsGetAllLogsResponse;
Ros1Clearpath_logger_msgsGetAllLogsClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_logger_msgsGetAllLogsClient(Ros1Clearpath_logger_msgsGetAllLogsClient const&)=delete;Ros1Clearpath_logger_msgsGetAllLogsClient& operator=(Ros1Clearpath_logger_msgsGetAllLogsClient const&)=delete;Ros1Clearpath_logger_msgsGetAllLogsClient(Ros1Clearpath_logger_msgsGetAllLogsClient&&)=delete;
~Ros1Clearpath_logger_msgsGetAllLogsClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_logger_msgs_GetAllLogs_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_logger_msgs/GetAllLogs server; advertise/respond stay separate authorities. */
class Ros1Clearpath_logger_msgsGetAllLogsServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_logger_msgsGetAllLogsResponse(Ros1Clearpath_logger_msgsGetAllLogsRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_logger_msgsGetAllLogsRequest;using Response=Ros1Clearpath_logger_msgsGetAllLogsResponse;
Ros1Clearpath_logger_msgsGetAllLogsServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_logger_msgs_GetAllLogs_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_logger_msgs_GetAllLogs_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_logger_msgs_GetAllLogs_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_logger_msgs_GetAllLogs_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_logger_msgs/RecordCustomEvent client; default policy returns13 from construction. */
class Ros1Clearpath_logger_msgsRecordCustomEventClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_logger_msgsRecordCustomEventRequest;using Response=Ros1Clearpath_logger_msgsRecordCustomEventResponse;
Ros1Clearpath_logger_msgsRecordCustomEventClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_logger_msgsRecordCustomEventClient(Ros1Clearpath_logger_msgsRecordCustomEventClient const&)=delete;Ros1Clearpath_logger_msgsRecordCustomEventClient& operator=(Ros1Clearpath_logger_msgsRecordCustomEventClient const&)=delete;Ros1Clearpath_logger_msgsRecordCustomEventClient(Ros1Clearpath_logger_msgsRecordCustomEventClient&&)=delete;
~Ros1Clearpath_logger_msgsRecordCustomEventClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_logger_msgs_RecordCustomEvent_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_logger_msgs/RecordCustomEvent server; advertise/respond stay separate authorities. */
class Ros1Clearpath_logger_msgsRecordCustomEventServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_logger_msgsRecordCustomEventResponse(Ros1Clearpath_logger_msgsRecordCustomEventRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_logger_msgsRecordCustomEventRequest;using Response=Ros1Clearpath_logger_msgsRecordCustomEventResponse;
Ros1Clearpath_logger_msgsRecordCustomEventServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_logger_msgs_RecordCustomEvent_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_logger_msgs_RecordCustomEvent_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_logger_msgs_RecordCustomEvent_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_logger_msgs_RecordCustomEvent_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_logger_msgs/RecordErrorEvent client; default policy returns13 from construction. */
class Ros1Clearpath_logger_msgsRecordErrorEventClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_logger_msgsRecordErrorEventRequest;using Response=Ros1Clearpath_logger_msgsRecordErrorEventResponse;
Ros1Clearpath_logger_msgsRecordErrorEventClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_logger_msgsRecordErrorEventClient(Ros1Clearpath_logger_msgsRecordErrorEventClient const&)=delete;Ros1Clearpath_logger_msgsRecordErrorEventClient& operator=(Ros1Clearpath_logger_msgsRecordErrorEventClient const&)=delete;Ros1Clearpath_logger_msgsRecordErrorEventClient(Ros1Clearpath_logger_msgsRecordErrorEventClient&&)=delete;
~Ros1Clearpath_logger_msgsRecordErrorEventClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_logger_msgs_RecordErrorEvent_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_logger_msgs/RecordErrorEvent server; advertise/respond stay separate authorities. */
class Ros1Clearpath_logger_msgsRecordErrorEventServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_logger_msgsRecordErrorEventResponse(Ros1Clearpath_logger_msgsRecordErrorEventRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_logger_msgsRecordErrorEventRequest;using Response=Ros1Clearpath_logger_msgsRecordErrorEventResponse;
Ros1Clearpath_logger_msgsRecordErrorEventServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_logger_msgs_RecordErrorEvent_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_logger_msgs_RecordErrorEvent_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_logger_msgs_RecordErrorEvent_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_logger_msgs_RecordErrorEvent_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_logger_msgs/RecordEvent client; default policy returns13 from construction. */
class Ros1Clearpath_logger_msgsRecordEventClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_logger_msgsRecordEventRequest;using Response=Ros1Clearpath_logger_msgsRecordEventResponse;
Ros1Clearpath_logger_msgsRecordEventClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_logger_msgsRecordEventClient(Ros1Clearpath_logger_msgsRecordEventClient const&)=delete;Ros1Clearpath_logger_msgsRecordEventClient& operator=(Ros1Clearpath_logger_msgsRecordEventClient const&)=delete;Ros1Clearpath_logger_msgsRecordEventClient(Ros1Clearpath_logger_msgsRecordEventClient&&)=delete;
~Ros1Clearpath_logger_msgsRecordEventClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_logger_msgs_RecordEvent_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_logger_msgs/RecordEvent server; advertise/respond stay separate authorities. */
class Ros1Clearpath_logger_msgsRecordEventServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_logger_msgsRecordEventResponse(Ros1Clearpath_logger_msgsRecordEventRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_logger_msgsRecordEventRequest;using Response=Ros1Clearpath_logger_msgsRecordEventResponse;
Ros1Clearpath_logger_msgsRecordEventServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_logger_msgs_RecordEvent_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_logger_msgs_RecordEvent_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_logger_msgs_RecordEvent_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_logger_msgs_RecordEvent_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_logger_msgs/RecordLocationEvent client; default policy returns13 from construction. */
class Ros1Clearpath_logger_msgsRecordLocationEventClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_logger_msgsRecordLocationEventRequest;using Response=Ros1Clearpath_logger_msgsRecordLocationEventResponse;
Ros1Clearpath_logger_msgsRecordLocationEventClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_logger_msgsRecordLocationEventClient(Ros1Clearpath_logger_msgsRecordLocationEventClient const&)=delete;Ros1Clearpath_logger_msgsRecordLocationEventClient& operator=(Ros1Clearpath_logger_msgsRecordLocationEventClient const&)=delete;Ros1Clearpath_logger_msgsRecordLocationEventClient(Ros1Clearpath_logger_msgsRecordLocationEventClient&&)=delete;
~Ros1Clearpath_logger_msgsRecordLocationEventClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_logger_msgs_RecordLocationEvent_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_logger_msgs/RecordLocationEvent server; advertise/respond stay separate authorities. */
class Ros1Clearpath_logger_msgsRecordLocationEventServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_logger_msgsRecordLocationEventResponse(Ros1Clearpath_logger_msgsRecordLocationEventRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_logger_msgsRecordLocationEventRequest;using Response=Ros1Clearpath_logger_msgsRecordLocationEventResponse;
Ros1Clearpath_logger_msgsRecordLocationEventServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_logger_msgs_RecordLocationEvent_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_logger_msgs_RecordLocationEvent_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_logger_msgs_RecordLocationEvent_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_logger_msgs_RecordLocationEvent_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_logger_msgs/RecordMediaEvent client; default policy returns13 from construction. */
class Ros1Clearpath_logger_msgsRecordMediaEventClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_logger_msgsRecordMediaEventRequest;using Response=Ros1Clearpath_logger_msgsRecordMediaEventResponse;
Ros1Clearpath_logger_msgsRecordMediaEventClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_logger_msgsRecordMediaEventClient(Ros1Clearpath_logger_msgsRecordMediaEventClient const&)=delete;Ros1Clearpath_logger_msgsRecordMediaEventClient& operator=(Ros1Clearpath_logger_msgsRecordMediaEventClient const&)=delete;Ros1Clearpath_logger_msgsRecordMediaEventClient(Ros1Clearpath_logger_msgsRecordMediaEventClient&&)=delete;
~Ros1Clearpath_logger_msgsRecordMediaEventClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_logger_msgs_RecordMediaEvent_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_logger_msgs/RecordMediaEvent server; advertise/respond stay separate authorities. */
class Ros1Clearpath_logger_msgsRecordMediaEventServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_logger_msgsRecordMediaEventResponse(Ros1Clearpath_logger_msgsRecordMediaEventRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_logger_msgsRecordMediaEventRequest;using Response=Ros1Clearpath_logger_msgsRecordMediaEventResponse;
Ros1Clearpath_logger_msgsRecordMediaEventServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_logger_msgs_RecordMediaEvent_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_logger_msgs_RecordMediaEvent_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_logger_msgs_RecordMediaEvent_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_logger_msgs_RecordMediaEvent_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_logger_msgs/RecordPowerEvent client; default policy returns13 from construction. */
class Ros1Clearpath_logger_msgsRecordPowerEventClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_logger_msgsRecordPowerEventRequest;using Response=Ros1Clearpath_logger_msgsRecordPowerEventResponse;
Ros1Clearpath_logger_msgsRecordPowerEventClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_logger_msgsRecordPowerEventClient(Ros1Clearpath_logger_msgsRecordPowerEventClient const&)=delete;Ros1Clearpath_logger_msgsRecordPowerEventClient& operator=(Ros1Clearpath_logger_msgsRecordPowerEventClient const&)=delete;Ros1Clearpath_logger_msgsRecordPowerEventClient(Ros1Clearpath_logger_msgsRecordPowerEventClient&&)=delete;
~Ros1Clearpath_logger_msgsRecordPowerEventClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_logger_msgs_RecordPowerEvent_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_logger_msgs/RecordPowerEvent server; advertise/respond stay separate authorities. */
class Ros1Clearpath_logger_msgsRecordPowerEventServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_logger_msgsRecordPowerEventResponse(Ros1Clearpath_logger_msgsRecordPowerEventRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_logger_msgsRecordPowerEventRequest;using Response=Ros1Clearpath_logger_msgsRecordPowerEventResponse;
Ros1Clearpath_logger_msgsRecordPowerEventServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_logger_msgs_RecordPowerEvent_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_logger_msgs_RecordPowerEvent_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_logger_msgs_RecordPowerEvent_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_logger_msgs_RecordPowerEvent_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_logger_msgs/StartRecording client; default policy returns13 from construction. */
class Ros1Clearpath_logger_msgsStartRecordingClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_logger_msgsStartRecordingRequest;using Response=Ros1Clearpath_logger_msgsStartRecordingResponse;
Ros1Clearpath_logger_msgsStartRecordingClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_logger_msgsStartRecordingClient(Ros1Clearpath_logger_msgsStartRecordingClient const&)=delete;Ros1Clearpath_logger_msgsStartRecordingClient& operator=(Ros1Clearpath_logger_msgsStartRecordingClient const&)=delete;Ros1Clearpath_logger_msgsStartRecordingClient(Ros1Clearpath_logger_msgsStartRecordingClient&&)=delete;
~Ros1Clearpath_logger_msgsStartRecordingClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_logger_msgs_StartRecording_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_logger_msgs/StartRecording server; advertise/respond stay separate authorities. */
class Ros1Clearpath_logger_msgsStartRecordingServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_logger_msgsStartRecordingResponse(Ros1Clearpath_logger_msgsStartRecordingRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_logger_msgsStartRecordingRequest;using Response=Ros1Clearpath_logger_msgsStartRecordingResponse;
Ros1Clearpath_logger_msgsStartRecordingServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_logger_msgs_StartRecording_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_logger_msgs_StartRecording_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_logger_msgs_StartRecording_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_logger_msgs_StartRecording_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_logger_msgs/StopRecording client; default policy returns13 from construction. */
class Ros1Clearpath_logger_msgsStopRecordingClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_logger_msgsStopRecordingRequest;using Response=Ros1Clearpath_logger_msgsStopRecordingResponse;
Ros1Clearpath_logger_msgsStopRecordingClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_logger_msgsStopRecordingClient(Ros1Clearpath_logger_msgsStopRecordingClient const&)=delete;Ros1Clearpath_logger_msgsStopRecordingClient& operator=(Ros1Clearpath_logger_msgsStopRecordingClient const&)=delete;Ros1Clearpath_logger_msgsStopRecordingClient(Ros1Clearpath_logger_msgsStopRecordingClient&&)=delete;
~Ros1Clearpath_logger_msgsStopRecordingClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_logger_msgs_StopRecording_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_logger_msgs/StopRecording server; advertise/respond stay separate authorities. */
class Ros1Clearpath_logger_msgsStopRecordingServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_logger_msgsStopRecordingResponse(Ros1Clearpath_logger_msgsStopRecordingRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_logger_msgsStopRecordingRequest;using Response=Ros1Clearpath_logger_msgsStopRecordingResponse;
Ros1Clearpath_logger_msgsStopRecordingServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_logger_msgs_StopRecording_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_logger_msgs_StopRecording_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_logger_msgs_StopRecording_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_logger_msgs_StopRecording_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_manager_msgs/AddNetworkEdge client; default policy returns13 from construction. */
class Ros1Clearpath_mission_manager_msgsAddNetworkEdgeClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_manager_msgsAddNetworkEdgeRequest;using Response=Ros1Clearpath_mission_manager_msgsAddNetworkEdgeResponse;
Ros1Clearpath_mission_manager_msgsAddNetworkEdgeClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_manager_msgsAddNetworkEdgeClient(Ros1Clearpath_mission_manager_msgsAddNetworkEdgeClient const&)=delete;Ros1Clearpath_mission_manager_msgsAddNetworkEdgeClient& operator=(Ros1Clearpath_mission_manager_msgsAddNetworkEdgeClient const&)=delete;Ros1Clearpath_mission_manager_msgsAddNetworkEdgeClient(Ros1Clearpath_mission_manager_msgsAddNetworkEdgeClient&&)=delete;
~Ros1Clearpath_mission_manager_msgsAddNetworkEdgeClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_AddNetworkEdge_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_manager_msgs/AddNetworkEdge server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_manager_msgsAddNetworkEdgeServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_manager_msgsAddNetworkEdgeResponse(Ros1Clearpath_mission_manager_msgsAddNetworkEdgeRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_manager_msgsAddNetworkEdgeRequest;using Response=Ros1Clearpath_mission_manager_msgsAddNetworkEdgeResponse;
Ros1Clearpath_mission_manager_msgsAddNetworkEdgeServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_manager_msgs_AddNetworkEdge_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_AddNetworkEdge_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_manager_msgs_AddNetworkEdge_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_manager_msgs_AddNetworkEdge_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_manager_msgs/AddNetworkPath client; default policy returns13 from construction. */
class Ros1Clearpath_mission_manager_msgsAddNetworkPathClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_manager_msgsAddNetworkPathRequest;using Response=Ros1Clearpath_mission_manager_msgsAddNetworkPathResponse;
Ros1Clearpath_mission_manager_msgsAddNetworkPathClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_manager_msgsAddNetworkPathClient(Ros1Clearpath_mission_manager_msgsAddNetworkPathClient const&)=delete;Ros1Clearpath_mission_manager_msgsAddNetworkPathClient& operator=(Ros1Clearpath_mission_manager_msgsAddNetworkPathClient const&)=delete;Ros1Clearpath_mission_manager_msgsAddNetworkPathClient(Ros1Clearpath_mission_manager_msgsAddNetworkPathClient&&)=delete;
~Ros1Clearpath_mission_manager_msgsAddNetworkPathClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_AddNetworkPath_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_manager_msgs/AddNetworkPath server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_manager_msgsAddNetworkPathServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_manager_msgsAddNetworkPathResponse(Ros1Clearpath_mission_manager_msgsAddNetworkPathRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_manager_msgsAddNetworkPathRequest;using Response=Ros1Clearpath_mission_manager_msgsAddNetworkPathResponse;
Ros1Clearpath_mission_manager_msgsAddNetworkPathServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_manager_msgs_AddNetworkPath_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_AddNetworkPath_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_manager_msgs_AddNetworkPath_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_manager_msgs_AddNetworkPath_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_manager_msgs/AddNetworkPoint client; default policy returns13 from construction. */
class Ros1Clearpath_mission_manager_msgsAddNetworkPointClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_manager_msgsAddNetworkPointRequest;using Response=Ros1Clearpath_mission_manager_msgsAddNetworkPointResponse;
Ros1Clearpath_mission_manager_msgsAddNetworkPointClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_manager_msgsAddNetworkPointClient(Ros1Clearpath_mission_manager_msgsAddNetworkPointClient const&)=delete;Ros1Clearpath_mission_manager_msgsAddNetworkPointClient& operator=(Ros1Clearpath_mission_manager_msgsAddNetworkPointClient const&)=delete;Ros1Clearpath_mission_manager_msgsAddNetworkPointClient(Ros1Clearpath_mission_manager_msgsAddNetworkPointClient&&)=delete;
~Ros1Clearpath_mission_manager_msgsAddNetworkPointClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_AddNetworkPoint_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_manager_msgs/AddNetworkPoint server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_manager_msgsAddNetworkPointServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_manager_msgsAddNetworkPointResponse(Ros1Clearpath_mission_manager_msgsAddNetworkPointRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_manager_msgsAddNetworkPointRequest;using Response=Ros1Clearpath_mission_manager_msgsAddNetworkPointResponse;
Ros1Clearpath_mission_manager_msgsAddNetworkPointServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_manager_msgs_AddNetworkPoint_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_AddNetworkPoint_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_manager_msgs_AddNetworkPoint_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_manager_msgs_AddNetworkPoint_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_manager_msgs/AddRemoveById client; default policy returns13 from construction. */
class Ros1Clearpath_mission_manager_msgsAddRemoveByIdClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_manager_msgsAddRemoveByIdRequest;using Response=Ros1Clearpath_mission_manager_msgsAddRemoveByIdResponse;
Ros1Clearpath_mission_manager_msgsAddRemoveByIdClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_manager_msgsAddRemoveByIdClient(Ros1Clearpath_mission_manager_msgsAddRemoveByIdClient const&)=delete;Ros1Clearpath_mission_manager_msgsAddRemoveByIdClient& operator=(Ros1Clearpath_mission_manager_msgsAddRemoveByIdClient const&)=delete;Ros1Clearpath_mission_manager_msgsAddRemoveByIdClient(Ros1Clearpath_mission_manager_msgsAddRemoveByIdClient&&)=delete;
~Ros1Clearpath_mission_manager_msgsAddRemoveByIdClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_AddRemoveById_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_manager_msgs/AddRemoveById server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_manager_msgsAddRemoveByIdServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_manager_msgsAddRemoveByIdResponse(Ros1Clearpath_mission_manager_msgsAddRemoveByIdRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_manager_msgsAddRemoveByIdRequest;using Response=Ros1Clearpath_mission_manager_msgsAddRemoveByIdResponse;
Ros1Clearpath_mission_manager_msgsAddRemoveByIdServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_manager_msgs_AddRemoveById_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_AddRemoveById_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_manager_msgs_AddRemoveById_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_manager_msgs_AddRemoveById_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_manager_msgs/AddRemoveTag client; default policy returns13 from construction. */
class Ros1Clearpath_mission_manager_msgsAddRemoveTagClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_manager_msgsAddRemoveTagRequest;using Response=Ros1Clearpath_mission_manager_msgsAddRemoveTagResponse;
Ros1Clearpath_mission_manager_msgsAddRemoveTagClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_manager_msgsAddRemoveTagClient(Ros1Clearpath_mission_manager_msgsAddRemoveTagClient const&)=delete;Ros1Clearpath_mission_manager_msgsAddRemoveTagClient& operator=(Ros1Clearpath_mission_manager_msgsAddRemoveTagClient const&)=delete;Ros1Clearpath_mission_manager_msgsAddRemoveTagClient(Ros1Clearpath_mission_manager_msgsAddRemoveTagClient&&)=delete;
~Ros1Clearpath_mission_manager_msgsAddRemoveTagClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_AddRemoveTag_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_manager_msgs/AddRemoveTag server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_manager_msgsAddRemoveTagServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_manager_msgsAddRemoveTagResponse(Ros1Clearpath_mission_manager_msgsAddRemoveTagRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_manager_msgsAddRemoveTagRequest;using Response=Ros1Clearpath_mission_manager_msgsAddRemoveTagResponse;
Ros1Clearpath_mission_manager_msgsAddRemoveTagServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_manager_msgs_AddRemoveTag_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_AddRemoveTag_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_manager_msgs_AddRemoveTag_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_manager_msgs_AddRemoveTag_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_manager_msgs/CloneMission client; default policy returns13 from construction. */
class Ros1Clearpath_mission_manager_msgsCloneMissionClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_manager_msgsCloneMissionRequest;using Response=Ros1Clearpath_mission_manager_msgsCloneMissionResponse;
Ros1Clearpath_mission_manager_msgsCloneMissionClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_manager_msgsCloneMissionClient(Ros1Clearpath_mission_manager_msgsCloneMissionClient const&)=delete;Ros1Clearpath_mission_manager_msgsCloneMissionClient& operator=(Ros1Clearpath_mission_manager_msgsCloneMissionClient const&)=delete;Ros1Clearpath_mission_manager_msgsCloneMissionClient(Ros1Clearpath_mission_manager_msgsCloneMissionClient&&)=delete;
~Ros1Clearpath_mission_manager_msgsCloneMissionClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_CloneMission_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_manager_msgs/CloneMission server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_manager_msgsCloneMissionServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_manager_msgsCloneMissionResponse(Ros1Clearpath_mission_manager_msgsCloneMissionRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_manager_msgsCloneMissionRequest;using Response=Ros1Clearpath_mission_manager_msgsCloneMissionResponse;
Ros1Clearpath_mission_manager_msgsCloneMissionServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_manager_msgs_CloneMission_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_CloneMission_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_manager_msgs_CloneMission_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_manager_msgs_CloneMission_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_manager_msgs/CloneNetworkMap client; default policy returns13 from construction. */
class Ros1Clearpath_mission_manager_msgsCloneNetworkMapClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_manager_msgsCloneNetworkMapRequest;using Response=Ros1Clearpath_mission_manager_msgsCloneNetworkMapResponse;
Ros1Clearpath_mission_manager_msgsCloneNetworkMapClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_manager_msgsCloneNetworkMapClient(Ros1Clearpath_mission_manager_msgsCloneNetworkMapClient const&)=delete;Ros1Clearpath_mission_manager_msgsCloneNetworkMapClient& operator=(Ros1Clearpath_mission_manager_msgsCloneNetworkMapClient const&)=delete;Ros1Clearpath_mission_manager_msgsCloneNetworkMapClient(Ros1Clearpath_mission_manager_msgsCloneNetworkMapClient&&)=delete;
~Ros1Clearpath_mission_manager_msgsCloneNetworkMapClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_CloneNetworkMap_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_manager_msgs/CloneNetworkMap server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_manager_msgsCloneNetworkMapServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_manager_msgsCloneNetworkMapResponse(Ros1Clearpath_mission_manager_msgsCloneNetworkMapRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_manager_msgsCloneNetworkMapRequest;using Response=Ros1Clearpath_mission_manager_msgsCloneNetworkMapResponse;
Ros1Clearpath_mission_manager_msgsCloneNetworkMapServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_manager_msgs_CloneNetworkMap_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_CloneNetworkMap_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_manager_msgs_CloneNetworkMap_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_manager_msgs_CloneNetworkMap_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_manager_msgs/CloneNetworkMission client; default policy returns13 from construction. */
class Ros1Clearpath_mission_manager_msgsCloneNetworkMissionClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_manager_msgsCloneNetworkMissionRequest;using Response=Ros1Clearpath_mission_manager_msgsCloneNetworkMissionResponse;
Ros1Clearpath_mission_manager_msgsCloneNetworkMissionClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_manager_msgsCloneNetworkMissionClient(Ros1Clearpath_mission_manager_msgsCloneNetworkMissionClient const&)=delete;Ros1Clearpath_mission_manager_msgsCloneNetworkMissionClient& operator=(Ros1Clearpath_mission_manager_msgsCloneNetworkMissionClient const&)=delete;Ros1Clearpath_mission_manager_msgsCloneNetworkMissionClient(Ros1Clearpath_mission_manager_msgsCloneNetworkMissionClient&&)=delete;
~Ros1Clearpath_mission_manager_msgsCloneNetworkMissionClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_CloneNetworkMission_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_manager_msgs/CloneNetworkMission server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_manager_msgsCloneNetworkMissionServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_manager_msgsCloneNetworkMissionResponse(Ros1Clearpath_mission_manager_msgsCloneNetworkMissionRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_manager_msgsCloneNetworkMissionRequest;using Response=Ros1Clearpath_mission_manager_msgsCloneNetworkMissionResponse;
Ros1Clearpath_mission_manager_msgsCloneNetworkMissionServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_manager_msgs_CloneNetworkMission_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_CloneNetworkMission_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_manager_msgs_CloneNetworkMission_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_manager_msgs_CloneNetworkMission_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_manager_msgs/ClonePointOfInterest client; default policy returns13 from construction. */
class Ros1Clearpath_mission_manager_msgsClonePointOfInterestClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_manager_msgsClonePointOfInterestRequest;using Response=Ros1Clearpath_mission_manager_msgsClonePointOfInterestResponse;
Ros1Clearpath_mission_manager_msgsClonePointOfInterestClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_manager_msgsClonePointOfInterestClient(Ros1Clearpath_mission_manager_msgsClonePointOfInterestClient const&)=delete;Ros1Clearpath_mission_manager_msgsClonePointOfInterestClient& operator=(Ros1Clearpath_mission_manager_msgsClonePointOfInterestClient const&)=delete;Ros1Clearpath_mission_manager_msgsClonePointOfInterestClient(Ros1Clearpath_mission_manager_msgsClonePointOfInterestClient&&)=delete;
~Ros1Clearpath_mission_manager_msgsClonePointOfInterestClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_ClonePointOfInterest_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_manager_msgs/ClonePointOfInterest server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_manager_msgsClonePointOfInterestServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_manager_msgsClonePointOfInterestResponse(Ros1Clearpath_mission_manager_msgsClonePointOfInterestRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_manager_msgsClonePointOfInterestRequest;using Response=Ros1Clearpath_mission_manager_msgsClonePointOfInterestResponse;
Ros1Clearpath_mission_manager_msgsClonePointOfInterestServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_manager_msgs_ClonePointOfInterest_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_ClonePointOfInterest_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_manager_msgs_ClonePointOfInterest_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_manager_msgs_ClonePointOfInterest_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_manager_msgs/CreateMission client; default policy returns13 from construction. */
class Ros1Clearpath_mission_manager_msgsCreateMissionClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_manager_msgsCreateMissionRequest;using Response=Ros1Clearpath_mission_manager_msgsCreateMissionResponse;
Ros1Clearpath_mission_manager_msgsCreateMissionClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_manager_msgsCreateMissionClient(Ros1Clearpath_mission_manager_msgsCreateMissionClient const&)=delete;Ros1Clearpath_mission_manager_msgsCreateMissionClient& operator=(Ros1Clearpath_mission_manager_msgsCreateMissionClient const&)=delete;Ros1Clearpath_mission_manager_msgsCreateMissionClient(Ros1Clearpath_mission_manager_msgsCreateMissionClient&&)=delete;
~Ros1Clearpath_mission_manager_msgsCreateMissionClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_CreateMission_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_manager_msgs/CreateMission server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_manager_msgsCreateMissionServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_manager_msgsCreateMissionResponse(Ros1Clearpath_mission_manager_msgsCreateMissionRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_manager_msgsCreateMissionRequest;using Response=Ros1Clearpath_mission_manager_msgsCreateMissionResponse;
Ros1Clearpath_mission_manager_msgsCreateMissionServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_manager_msgs_CreateMission_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_CreateMission_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_manager_msgs_CreateMission_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_manager_msgs_CreateMission_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_manager_msgs/CreateNetworkMap client; default policy returns13 from construction. */
class Ros1Clearpath_mission_manager_msgsCreateNetworkMapClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_manager_msgsCreateNetworkMapRequest;using Response=Ros1Clearpath_mission_manager_msgsCreateNetworkMapResponse;
Ros1Clearpath_mission_manager_msgsCreateNetworkMapClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_manager_msgsCreateNetworkMapClient(Ros1Clearpath_mission_manager_msgsCreateNetworkMapClient const&)=delete;Ros1Clearpath_mission_manager_msgsCreateNetworkMapClient& operator=(Ros1Clearpath_mission_manager_msgsCreateNetworkMapClient const&)=delete;Ros1Clearpath_mission_manager_msgsCreateNetworkMapClient(Ros1Clearpath_mission_manager_msgsCreateNetworkMapClient&&)=delete;
~Ros1Clearpath_mission_manager_msgsCreateNetworkMapClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_CreateNetworkMap_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_manager_msgs/CreateNetworkMap server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_manager_msgsCreateNetworkMapServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_manager_msgsCreateNetworkMapResponse(Ros1Clearpath_mission_manager_msgsCreateNetworkMapRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_manager_msgsCreateNetworkMapRequest;using Response=Ros1Clearpath_mission_manager_msgsCreateNetworkMapResponse;
Ros1Clearpath_mission_manager_msgsCreateNetworkMapServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_manager_msgs_CreateNetworkMap_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_CreateNetworkMap_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_manager_msgs_CreateNetworkMap_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_manager_msgs_CreateNetworkMap_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_manager_msgs/CreateNetworkMission client; default policy returns13 from construction. */
class Ros1Clearpath_mission_manager_msgsCreateNetworkMissionClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_manager_msgsCreateNetworkMissionRequest;using Response=Ros1Clearpath_mission_manager_msgsCreateNetworkMissionResponse;
Ros1Clearpath_mission_manager_msgsCreateNetworkMissionClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_manager_msgsCreateNetworkMissionClient(Ros1Clearpath_mission_manager_msgsCreateNetworkMissionClient const&)=delete;Ros1Clearpath_mission_manager_msgsCreateNetworkMissionClient& operator=(Ros1Clearpath_mission_manager_msgsCreateNetworkMissionClient const&)=delete;Ros1Clearpath_mission_manager_msgsCreateNetworkMissionClient(Ros1Clearpath_mission_manager_msgsCreateNetworkMissionClient&&)=delete;
~Ros1Clearpath_mission_manager_msgsCreateNetworkMissionClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_CreateNetworkMission_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_manager_msgs/CreateNetworkMission server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_manager_msgsCreateNetworkMissionServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_manager_msgsCreateNetworkMissionResponse(Ros1Clearpath_mission_manager_msgsCreateNetworkMissionRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_manager_msgsCreateNetworkMissionRequest;using Response=Ros1Clearpath_mission_manager_msgsCreateNetworkMissionResponse;
Ros1Clearpath_mission_manager_msgsCreateNetworkMissionServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_manager_msgs_CreateNetworkMission_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_CreateNetworkMission_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_manager_msgs_CreateNetworkMission_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_manager_msgs_CreateNetworkMission_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_manager_msgs/CreatePointOfInterest client; default policy returns13 from construction. */
class Ros1Clearpath_mission_manager_msgsCreatePointOfInterestClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_manager_msgsCreatePointOfInterestRequest;using Response=Ros1Clearpath_mission_manager_msgsCreatePointOfInterestResponse;
Ros1Clearpath_mission_manager_msgsCreatePointOfInterestClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_manager_msgsCreatePointOfInterestClient(Ros1Clearpath_mission_manager_msgsCreatePointOfInterestClient const&)=delete;Ros1Clearpath_mission_manager_msgsCreatePointOfInterestClient& operator=(Ros1Clearpath_mission_manager_msgsCreatePointOfInterestClient const&)=delete;Ros1Clearpath_mission_manager_msgsCreatePointOfInterestClient(Ros1Clearpath_mission_manager_msgsCreatePointOfInterestClient&&)=delete;
~Ros1Clearpath_mission_manager_msgsCreatePointOfInterestClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_CreatePointOfInterest_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_manager_msgs/CreatePointOfInterest server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_manager_msgsCreatePointOfInterestServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_manager_msgsCreatePointOfInterestResponse(Ros1Clearpath_mission_manager_msgsCreatePointOfInterestRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_manager_msgsCreatePointOfInterestRequest;using Response=Ros1Clearpath_mission_manager_msgsCreatePointOfInterestResponse;
Ros1Clearpath_mission_manager_msgsCreatePointOfInterestServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_manager_msgs_CreatePointOfInterest_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_CreatePointOfInterest_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_manager_msgs_CreatePointOfInterest_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_manager_msgs_CreatePointOfInterest_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_manager_msgs/CreateTask client; default policy returns13 from construction. */
class Ros1Clearpath_mission_manager_msgsCreateTaskClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_manager_msgsCreateTaskRequest;using Response=Ros1Clearpath_mission_manager_msgsCreateTaskResponse;
Ros1Clearpath_mission_manager_msgsCreateTaskClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_manager_msgsCreateTaskClient(Ros1Clearpath_mission_manager_msgsCreateTaskClient const&)=delete;Ros1Clearpath_mission_manager_msgsCreateTaskClient& operator=(Ros1Clearpath_mission_manager_msgsCreateTaskClient const&)=delete;Ros1Clearpath_mission_manager_msgsCreateTaskClient(Ros1Clearpath_mission_manager_msgsCreateTaskClient&&)=delete;
~Ros1Clearpath_mission_manager_msgsCreateTaskClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_CreateTask_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_manager_msgs/CreateTask server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_manager_msgsCreateTaskServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_manager_msgsCreateTaskResponse(Ros1Clearpath_mission_manager_msgsCreateTaskRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_manager_msgsCreateTaskRequest;using Response=Ros1Clearpath_mission_manager_msgsCreateTaskResponse;
Ros1Clearpath_mission_manager_msgsCreateTaskServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_manager_msgs_CreateTask_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_CreateTask_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_manager_msgs_CreateTask_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_manager_msgs_CreateTask_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_manager_msgs/CreateWaypoint client; default policy returns13 from construction. */
class Ros1Clearpath_mission_manager_msgsCreateWaypointClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_manager_msgsCreateWaypointRequest;using Response=Ros1Clearpath_mission_manager_msgsCreateWaypointResponse;
Ros1Clearpath_mission_manager_msgsCreateWaypointClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_manager_msgsCreateWaypointClient(Ros1Clearpath_mission_manager_msgsCreateWaypointClient const&)=delete;Ros1Clearpath_mission_manager_msgsCreateWaypointClient& operator=(Ros1Clearpath_mission_manager_msgsCreateWaypointClient const&)=delete;Ros1Clearpath_mission_manager_msgsCreateWaypointClient(Ros1Clearpath_mission_manager_msgsCreateWaypointClient&&)=delete;
~Ros1Clearpath_mission_manager_msgsCreateWaypointClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_CreateWaypoint_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_manager_msgs/CreateWaypoint server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_manager_msgsCreateWaypointServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_manager_msgsCreateWaypointResponse(Ros1Clearpath_mission_manager_msgsCreateWaypointRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_manager_msgsCreateWaypointRequest;using Response=Ros1Clearpath_mission_manager_msgsCreateWaypointResponse;
Ros1Clearpath_mission_manager_msgsCreateWaypointServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_manager_msgs_CreateWaypoint_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_CreateWaypoint_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_manager_msgs_CreateWaypoint_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_manager_msgs_CreateWaypoint_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_manager_msgs/CreateWaypointFromPoi client; default policy returns13 from construction. */
class Ros1Clearpath_mission_manager_msgsCreateWaypointFromPoiClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_manager_msgsCreateWaypointFromPoiRequest;using Response=Ros1Clearpath_mission_manager_msgsCreateWaypointFromPoiResponse;
Ros1Clearpath_mission_manager_msgsCreateWaypointFromPoiClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_manager_msgsCreateWaypointFromPoiClient(Ros1Clearpath_mission_manager_msgsCreateWaypointFromPoiClient const&)=delete;Ros1Clearpath_mission_manager_msgsCreateWaypointFromPoiClient& operator=(Ros1Clearpath_mission_manager_msgsCreateWaypointFromPoiClient const&)=delete;Ros1Clearpath_mission_manager_msgsCreateWaypointFromPoiClient(Ros1Clearpath_mission_manager_msgsCreateWaypointFromPoiClient&&)=delete;
~Ros1Clearpath_mission_manager_msgsCreateWaypointFromPoiClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_CreateWaypointFromPoi_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_manager_msgs/CreateWaypointFromPoi server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_manager_msgsCreateWaypointFromPoiServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_manager_msgsCreateWaypointFromPoiResponse(Ros1Clearpath_mission_manager_msgsCreateWaypointFromPoiRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_manager_msgsCreateWaypointFromPoiRequest;using Response=Ros1Clearpath_mission_manager_msgsCreateWaypointFromPoiResponse;
Ros1Clearpath_mission_manager_msgsCreateWaypointFromPoiServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_manager_msgs_CreateWaypointFromPoi_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_CreateWaypointFromPoi_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_manager_msgs_CreateWaypointFromPoi_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_manager_msgs_CreateWaypointFromPoi_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_manager_msgs/DeleteById client; default policy returns13 from construction. */
class Ros1Clearpath_mission_manager_msgsDeleteByIdClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_manager_msgsDeleteByIdRequest;using Response=Ros1Clearpath_mission_manager_msgsDeleteByIdResponse;
Ros1Clearpath_mission_manager_msgsDeleteByIdClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_manager_msgsDeleteByIdClient(Ros1Clearpath_mission_manager_msgsDeleteByIdClient const&)=delete;Ros1Clearpath_mission_manager_msgsDeleteByIdClient& operator=(Ros1Clearpath_mission_manager_msgsDeleteByIdClient const&)=delete;Ros1Clearpath_mission_manager_msgsDeleteByIdClient(Ros1Clearpath_mission_manager_msgsDeleteByIdClient&&)=delete;
~Ros1Clearpath_mission_manager_msgsDeleteByIdClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_DeleteById_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_manager_msgs/DeleteById server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_manager_msgsDeleteByIdServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_manager_msgsDeleteByIdResponse(Ros1Clearpath_mission_manager_msgsDeleteByIdRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_manager_msgsDeleteByIdRequest;using Response=Ros1Clearpath_mission_manager_msgsDeleteByIdResponse;
Ros1Clearpath_mission_manager_msgsDeleteByIdServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_manager_msgs_DeleteById_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_DeleteById_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_manager_msgs_DeleteById_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_manager_msgs_DeleteById_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_manager_msgs/DeleteEverything client; default policy returns13 from construction. */
class Ros1Clearpath_mission_manager_msgsDeleteEverythingClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_manager_msgsDeleteEverythingRequest;using Response=Ros1Clearpath_mission_manager_msgsDeleteEverythingResponse;
Ros1Clearpath_mission_manager_msgsDeleteEverythingClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_manager_msgsDeleteEverythingClient(Ros1Clearpath_mission_manager_msgsDeleteEverythingClient const&)=delete;Ros1Clearpath_mission_manager_msgsDeleteEverythingClient& operator=(Ros1Clearpath_mission_manager_msgsDeleteEverythingClient const&)=delete;Ros1Clearpath_mission_manager_msgsDeleteEverythingClient(Ros1Clearpath_mission_manager_msgsDeleteEverythingClient&&)=delete;
~Ros1Clearpath_mission_manager_msgsDeleteEverythingClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_DeleteEverything_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_manager_msgs/DeleteEverything server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_manager_msgsDeleteEverythingServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_manager_msgsDeleteEverythingResponse(Ros1Clearpath_mission_manager_msgsDeleteEverythingRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_manager_msgsDeleteEverythingRequest;using Response=Ros1Clearpath_mission_manager_msgsDeleteEverythingResponse;
Ros1Clearpath_mission_manager_msgsDeleteEverythingServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_manager_msgs_DeleteEverything_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_DeleteEverything_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_manager_msgs_DeleteEverything_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_manager_msgs_DeleteEverything_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_manager_msgs/DeleteMultiple client; default policy returns13 from construction. */
class Ros1Clearpath_mission_manager_msgsDeleteMultipleClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_manager_msgsDeleteMultipleRequest;using Response=Ros1Clearpath_mission_manager_msgsDeleteMultipleResponse;
Ros1Clearpath_mission_manager_msgsDeleteMultipleClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_manager_msgsDeleteMultipleClient(Ros1Clearpath_mission_manager_msgsDeleteMultipleClient const&)=delete;Ros1Clearpath_mission_manager_msgsDeleteMultipleClient& operator=(Ros1Clearpath_mission_manager_msgsDeleteMultipleClient const&)=delete;Ros1Clearpath_mission_manager_msgsDeleteMultipleClient(Ros1Clearpath_mission_manager_msgsDeleteMultipleClient&&)=delete;
~Ros1Clearpath_mission_manager_msgsDeleteMultipleClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_DeleteMultiple_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_manager_msgs/DeleteMultiple server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_manager_msgsDeleteMultipleServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_manager_msgsDeleteMultipleResponse(Ros1Clearpath_mission_manager_msgsDeleteMultipleRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_manager_msgsDeleteMultipleRequest;using Response=Ros1Clearpath_mission_manager_msgsDeleteMultipleResponse;
Ros1Clearpath_mission_manager_msgsDeleteMultipleServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_manager_msgs_DeleteMultiple_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_DeleteMultiple_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_manager_msgs_DeleteMultiple_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_manager_msgs_DeleteMultiple_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_manager_msgs/ExportData client; default policy returns13 from construction. */
class Ros1Clearpath_mission_manager_msgsExportDataClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_manager_msgsExportDataRequest;using Response=Ros1Clearpath_mission_manager_msgsExportDataResponse;
Ros1Clearpath_mission_manager_msgsExportDataClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_manager_msgsExportDataClient(Ros1Clearpath_mission_manager_msgsExportDataClient const&)=delete;Ros1Clearpath_mission_manager_msgsExportDataClient& operator=(Ros1Clearpath_mission_manager_msgsExportDataClient const&)=delete;Ros1Clearpath_mission_manager_msgsExportDataClient(Ros1Clearpath_mission_manager_msgsExportDataClient&&)=delete;
~Ros1Clearpath_mission_manager_msgsExportDataClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_ExportData_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_manager_msgs/ExportData server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_manager_msgsExportDataServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_manager_msgsExportDataResponse(Ros1Clearpath_mission_manager_msgsExportDataRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_manager_msgsExportDataRequest;using Response=Ros1Clearpath_mission_manager_msgsExportDataResponse;
Ros1Clearpath_mission_manager_msgsExportDataServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_manager_msgs_ExportData_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_ExportData_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_manager_msgs_ExportData_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_manager_msgs_ExportData_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_manager_msgs/GetAllMissions client; default policy returns13 from construction. */
class Ros1Clearpath_mission_manager_msgsGetAllMissionsClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_manager_msgsGetAllMissionsRequest;using Response=Ros1Clearpath_mission_manager_msgsGetAllMissionsResponse;
Ros1Clearpath_mission_manager_msgsGetAllMissionsClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_manager_msgsGetAllMissionsClient(Ros1Clearpath_mission_manager_msgsGetAllMissionsClient const&)=delete;Ros1Clearpath_mission_manager_msgsGetAllMissionsClient& operator=(Ros1Clearpath_mission_manager_msgsGetAllMissionsClient const&)=delete;Ros1Clearpath_mission_manager_msgsGetAllMissionsClient(Ros1Clearpath_mission_manager_msgsGetAllMissionsClient&&)=delete;
~Ros1Clearpath_mission_manager_msgsGetAllMissionsClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_GetAllMissions_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_manager_msgs/GetAllMissions server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_manager_msgsGetAllMissionsServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_manager_msgsGetAllMissionsResponse(Ros1Clearpath_mission_manager_msgsGetAllMissionsRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_manager_msgsGetAllMissionsRequest;using Response=Ros1Clearpath_mission_manager_msgsGetAllMissionsResponse;
Ros1Clearpath_mission_manager_msgsGetAllMissionsServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_manager_msgs_GetAllMissions_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_GetAllMissions_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_manager_msgs_GetAllMissions_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_manager_msgs_GetAllMissions_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_manager_msgs/GetAllNetworkMaps client; default policy returns13 from construction. */
class Ros1Clearpath_mission_manager_msgsGetAllNetworkMapsClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_manager_msgsGetAllNetworkMapsRequest;using Response=Ros1Clearpath_mission_manager_msgsGetAllNetworkMapsResponse;
Ros1Clearpath_mission_manager_msgsGetAllNetworkMapsClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_manager_msgsGetAllNetworkMapsClient(Ros1Clearpath_mission_manager_msgsGetAllNetworkMapsClient const&)=delete;Ros1Clearpath_mission_manager_msgsGetAllNetworkMapsClient& operator=(Ros1Clearpath_mission_manager_msgsGetAllNetworkMapsClient const&)=delete;Ros1Clearpath_mission_manager_msgsGetAllNetworkMapsClient(Ros1Clearpath_mission_manager_msgsGetAllNetworkMapsClient&&)=delete;
~Ros1Clearpath_mission_manager_msgsGetAllNetworkMapsClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_GetAllNetworkMaps_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_manager_msgs/GetAllNetworkMaps server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_manager_msgsGetAllNetworkMapsServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_manager_msgsGetAllNetworkMapsResponse(Ros1Clearpath_mission_manager_msgsGetAllNetworkMapsRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_manager_msgsGetAllNetworkMapsRequest;using Response=Ros1Clearpath_mission_manager_msgsGetAllNetworkMapsResponse;
Ros1Clearpath_mission_manager_msgsGetAllNetworkMapsServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_manager_msgs_GetAllNetworkMaps_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_GetAllNetworkMaps_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_manager_msgs_GetAllNetworkMaps_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_manager_msgs_GetAllNetworkMaps_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_manager_msgs/GetAllNetworkMissions client; default policy returns13 from construction. */
class Ros1Clearpath_mission_manager_msgsGetAllNetworkMissionsClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_manager_msgsGetAllNetworkMissionsRequest;using Response=Ros1Clearpath_mission_manager_msgsGetAllNetworkMissionsResponse;
Ros1Clearpath_mission_manager_msgsGetAllNetworkMissionsClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_manager_msgsGetAllNetworkMissionsClient(Ros1Clearpath_mission_manager_msgsGetAllNetworkMissionsClient const&)=delete;Ros1Clearpath_mission_manager_msgsGetAllNetworkMissionsClient& operator=(Ros1Clearpath_mission_manager_msgsGetAllNetworkMissionsClient const&)=delete;Ros1Clearpath_mission_manager_msgsGetAllNetworkMissionsClient(Ros1Clearpath_mission_manager_msgsGetAllNetworkMissionsClient&&)=delete;
~Ros1Clearpath_mission_manager_msgsGetAllNetworkMissionsClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_GetAllNetworkMissions_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_manager_msgs/GetAllNetworkMissions server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_manager_msgsGetAllNetworkMissionsServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_manager_msgsGetAllNetworkMissionsResponse(Ros1Clearpath_mission_manager_msgsGetAllNetworkMissionsRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_manager_msgsGetAllNetworkMissionsRequest;using Response=Ros1Clearpath_mission_manager_msgsGetAllNetworkMissionsResponse;
Ros1Clearpath_mission_manager_msgsGetAllNetworkMissionsServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_manager_msgs_GetAllNetworkMissions_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_GetAllNetworkMissions_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_manager_msgs_GetAllNetworkMissions_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_manager_msgs_GetAllNetworkMissions_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_manager_msgs/GetAllPointsOfInterest client; default policy returns13 from construction. */
class Ros1Clearpath_mission_manager_msgsGetAllPointsOfInterestClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_manager_msgsGetAllPointsOfInterestRequest;using Response=Ros1Clearpath_mission_manager_msgsGetAllPointsOfInterestResponse;
Ros1Clearpath_mission_manager_msgsGetAllPointsOfInterestClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_manager_msgsGetAllPointsOfInterestClient(Ros1Clearpath_mission_manager_msgsGetAllPointsOfInterestClient const&)=delete;Ros1Clearpath_mission_manager_msgsGetAllPointsOfInterestClient& operator=(Ros1Clearpath_mission_manager_msgsGetAllPointsOfInterestClient const&)=delete;Ros1Clearpath_mission_manager_msgsGetAllPointsOfInterestClient(Ros1Clearpath_mission_manager_msgsGetAllPointsOfInterestClient&&)=delete;
~Ros1Clearpath_mission_manager_msgsGetAllPointsOfInterestClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_GetAllPointsOfInterest_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_manager_msgs/GetAllPointsOfInterest server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_manager_msgsGetAllPointsOfInterestServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_manager_msgsGetAllPointsOfInterestResponse(Ros1Clearpath_mission_manager_msgsGetAllPointsOfInterestRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_manager_msgsGetAllPointsOfInterestRequest;using Response=Ros1Clearpath_mission_manager_msgsGetAllPointsOfInterestResponse;
Ros1Clearpath_mission_manager_msgsGetAllPointsOfInterestServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_manager_msgs_GetAllPointsOfInterest_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_GetAllPointsOfInterest_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_manager_msgs_GetAllPointsOfInterest_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_manager_msgs_GetAllPointsOfInterest_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_manager_msgs/GetAllTasks client; default policy returns13 from construction. */
class Ros1Clearpath_mission_manager_msgsGetAllTasksClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_manager_msgsGetAllTasksRequest;using Response=Ros1Clearpath_mission_manager_msgsGetAllTasksResponse;
Ros1Clearpath_mission_manager_msgsGetAllTasksClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_manager_msgsGetAllTasksClient(Ros1Clearpath_mission_manager_msgsGetAllTasksClient const&)=delete;Ros1Clearpath_mission_manager_msgsGetAllTasksClient& operator=(Ros1Clearpath_mission_manager_msgsGetAllTasksClient const&)=delete;Ros1Clearpath_mission_manager_msgsGetAllTasksClient(Ros1Clearpath_mission_manager_msgsGetAllTasksClient&&)=delete;
~Ros1Clearpath_mission_manager_msgsGetAllTasksClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_GetAllTasks_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_manager_msgs/GetAllTasks server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_manager_msgsGetAllTasksServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_manager_msgsGetAllTasksResponse(Ros1Clearpath_mission_manager_msgsGetAllTasksRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_manager_msgsGetAllTasksRequest;using Response=Ros1Clearpath_mission_manager_msgsGetAllTasksResponse;
Ros1Clearpath_mission_manager_msgsGetAllTasksServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_manager_msgs_GetAllTasks_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_GetAllTasks_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_manager_msgs_GetAllTasks_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_manager_msgs_GetAllTasks_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_manager_msgs/GetAllWaypoints client; default policy returns13 from construction. */
class Ros1Clearpath_mission_manager_msgsGetAllWaypointsClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_manager_msgsGetAllWaypointsRequest;using Response=Ros1Clearpath_mission_manager_msgsGetAllWaypointsResponse;
Ros1Clearpath_mission_manager_msgsGetAllWaypointsClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_manager_msgsGetAllWaypointsClient(Ros1Clearpath_mission_manager_msgsGetAllWaypointsClient const&)=delete;Ros1Clearpath_mission_manager_msgsGetAllWaypointsClient& operator=(Ros1Clearpath_mission_manager_msgsGetAllWaypointsClient const&)=delete;Ros1Clearpath_mission_manager_msgsGetAllWaypointsClient(Ros1Clearpath_mission_manager_msgsGetAllWaypointsClient&&)=delete;
~Ros1Clearpath_mission_manager_msgsGetAllWaypointsClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_GetAllWaypoints_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_manager_msgs/GetAllWaypoints server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_manager_msgsGetAllWaypointsServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_manager_msgsGetAllWaypointsResponse(Ros1Clearpath_mission_manager_msgsGetAllWaypointsRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_manager_msgsGetAllWaypointsRequest;using Response=Ros1Clearpath_mission_manager_msgsGetAllWaypointsResponse;
Ros1Clearpath_mission_manager_msgsGetAllWaypointsServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_manager_msgs_GetAllWaypoints_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_GetAllWaypoints_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_manager_msgs_GetAllWaypoints_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_manager_msgs_GetAllWaypoints_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_manager_msgs/GetEverything client; default policy returns13 from construction. */
class Ros1Clearpath_mission_manager_msgsGetEverythingClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_manager_msgsGetEverythingRequest;using Response=Ros1Clearpath_mission_manager_msgsGetEverythingResponse;
Ros1Clearpath_mission_manager_msgsGetEverythingClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_manager_msgsGetEverythingClient(Ros1Clearpath_mission_manager_msgsGetEverythingClient const&)=delete;Ros1Clearpath_mission_manager_msgsGetEverythingClient& operator=(Ros1Clearpath_mission_manager_msgsGetEverythingClient const&)=delete;Ros1Clearpath_mission_manager_msgsGetEverythingClient(Ros1Clearpath_mission_manager_msgsGetEverythingClient&&)=delete;
~Ros1Clearpath_mission_manager_msgsGetEverythingClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_GetEverything_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_manager_msgs/GetEverything server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_manager_msgsGetEverythingServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_manager_msgsGetEverythingResponse(Ros1Clearpath_mission_manager_msgsGetEverythingRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_manager_msgsGetEverythingRequest;using Response=Ros1Clearpath_mission_manager_msgsGetEverythingResponse;
Ros1Clearpath_mission_manager_msgsGetEverythingServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_manager_msgs_GetEverything_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_GetEverything_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_manager_msgs_GetEverything_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_manager_msgs_GetEverything_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_manager_msgs/GetMission client; default policy returns13 from construction. */
class Ros1Clearpath_mission_manager_msgsGetMissionClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_manager_msgsGetMissionRequest;using Response=Ros1Clearpath_mission_manager_msgsGetMissionResponse;
Ros1Clearpath_mission_manager_msgsGetMissionClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_manager_msgsGetMissionClient(Ros1Clearpath_mission_manager_msgsGetMissionClient const&)=delete;Ros1Clearpath_mission_manager_msgsGetMissionClient& operator=(Ros1Clearpath_mission_manager_msgsGetMissionClient const&)=delete;Ros1Clearpath_mission_manager_msgsGetMissionClient(Ros1Clearpath_mission_manager_msgsGetMissionClient&&)=delete;
~Ros1Clearpath_mission_manager_msgsGetMissionClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_GetMission_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_manager_msgs/GetMission server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_manager_msgsGetMissionServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_manager_msgsGetMissionResponse(Ros1Clearpath_mission_manager_msgsGetMissionRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_manager_msgsGetMissionRequest;using Response=Ros1Clearpath_mission_manager_msgsGetMissionResponse;
Ros1Clearpath_mission_manager_msgsGetMissionServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_manager_msgs_GetMission_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_GetMission_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_manager_msgs_GetMission_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_manager_msgs_GetMission_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_manager_msgs/GetNetworkMap client; default policy returns13 from construction. */
class Ros1Clearpath_mission_manager_msgsGetNetworkMapClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_manager_msgsGetNetworkMapRequest;using Response=Ros1Clearpath_mission_manager_msgsGetNetworkMapResponse;
Ros1Clearpath_mission_manager_msgsGetNetworkMapClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_manager_msgsGetNetworkMapClient(Ros1Clearpath_mission_manager_msgsGetNetworkMapClient const&)=delete;Ros1Clearpath_mission_manager_msgsGetNetworkMapClient& operator=(Ros1Clearpath_mission_manager_msgsGetNetworkMapClient const&)=delete;Ros1Clearpath_mission_manager_msgsGetNetworkMapClient(Ros1Clearpath_mission_manager_msgsGetNetworkMapClient&&)=delete;
~Ros1Clearpath_mission_manager_msgsGetNetworkMapClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_GetNetworkMap_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_manager_msgs/GetNetworkMap server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_manager_msgsGetNetworkMapServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_manager_msgsGetNetworkMapResponse(Ros1Clearpath_mission_manager_msgsGetNetworkMapRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_manager_msgsGetNetworkMapRequest;using Response=Ros1Clearpath_mission_manager_msgsGetNetworkMapResponse;
Ros1Clearpath_mission_manager_msgsGetNetworkMapServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_manager_msgs_GetNetworkMap_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_GetNetworkMap_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_manager_msgs_GetNetworkMap_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_manager_msgs_GetNetworkMap_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_manager_msgs/GetNetworkMission client; default policy returns13 from construction. */
class Ros1Clearpath_mission_manager_msgsGetNetworkMissionClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_manager_msgsGetNetworkMissionRequest;using Response=Ros1Clearpath_mission_manager_msgsGetNetworkMissionResponse;
Ros1Clearpath_mission_manager_msgsGetNetworkMissionClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_manager_msgsGetNetworkMissionClient(Ros1Clearpath_mission_manager_msgsGetNetworkMissionClient const&)=delete;Ros1Clearpath_mission_manager_msgsGetNetworkMissionClient& operator=(Ros1Clearpath_mission_manager_msgsGetNetworkMissionClient const&)=delete;Ros1Clearpath_mission_manager_msgsGetNetworkMissionClient(Ros1Clearpath_mission_manager_msgsGetNetworkMissionClient&&)=delete;
~Ros1Clearpath_mission_manager_msgsGetNetworkMissionClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_GetNetworkMission_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_manager_msgs/GetNetworkMission server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_manager_msgsGetNetworkMissionServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_manager_msgsGetNetworkMissionResponse(Ros1Clearpath_mission_manager_msgsGetNetworkMissionRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_manager_msgsGetNetworkMissionRequest;using Response=Ros1Clearpath_mission_manager_msgsGetNetworkMissionResponse;
Ros1Clearpath_mission_manager_msgsGetNetworkMissionServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_manager_msgs_GetNetworkMission_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_GetNetworkMission_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_manager_msgs_GetNetworkMission_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_manager_msgs_GetNetworkMission_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_manager_msgs/GetPointOfInterest client; default policy returns13 from construction. */
class Ros1Clearpath_mission_manager_msgsGetPointOfInterestClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_manager_msgsGetPointOfInterestRequest;using Response=Ros1Clearpath_mission_manager_msgsGetPointOfInterestResponse;
Ros1Clearpath_mission_manager_msgsGetPointOfInterestClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_manager_msgsGetPointOfInterestClient(Ros1Clearpath_mission_manager_msgsGetPointOfInterestClient const&)=delete;Ros1Clearpath_mission_manager_msgsGetPointOfInterestClient& operator=(Ros1Clearpath_mission_manager_msgsGetPointOfInterestClient const&)=delete;Ros1Clearpath_mission_manager_msgsGetPointOfInterestClient(Ros1Clearpath_mission_manager_msgsGetPointOfInterestClient&&)=delete;
~Ros1Clearpath_mission_manager_msgsGetPointOfInterestClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_GetPointOfInterest_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_manager_msgs/GetPointOfInterest server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_manager_msgsGetPointOfInterestServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_manager_msgsGetPointOfInterestResponse(Ros1Clearpath_mission_manager_msgsGetPointOfInterestRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_manager_msgsGetPointOfInterestRequest;using Response=Ros1Clearpath_mission_manager_msgsGetPointOfInterestResponse;
Ros1Clearpath_mission_manager_msgsGetPointOfInterestServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_manager_msgs_GetPointOfInterest_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_GetPointOfInterest_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_manager_msgs_GetPointOfInterest_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_manager_msgs_GetPointOfInterest_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_manager_msgs/GetTask client; default policy returns13 from construction. */
class Ros1Clearpath_mission_manager_msgsGetTaskClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_manager_msgsGetTaskRequest;using Response=Ros1Clearpath_mission_manager_msgsGetTaskResponse;
Ros1Clearpath_mission_manager_msgsGetTaskClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_manager_msgsGetTaskClient(Ros1Clearpath_mission_manager_msgsGetTaskClient const&)=delete;Ros1Clearpath_mission_manager_msgsGetTaskClient& operator=(Ros1Clearpath_mission_manager_msgsGetTaskClient const&)=delete;Ros1Clearpath_mission_manager_msgsGetTaskClient(Ros1Clearpath_mission_manager_msgsGetTaskClient&&)=delete;
~Ros1Clearpath_mission_manager_msgsGetTaskClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_GetTask_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_manager_msgs/GetTask server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_manager_msgsGetTaskServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_manager_msgsGetTaskResponse(Ros1Clearpath_mission_manager_msgsGetTaskRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_manager_msgsGetTaskRequest;using Response=Ros1Clearpath_mission_manager_msgsGetTaskResponse;
Ros1Clearpath_mission_manager_msgsGetTaskServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_manager_msgs_GetTask_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_GetTask_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_manager_msgs_GetTask_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_manager_msgs_GetTask_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_manager_msgs/GetWaypoint client; default policy returns13 from construction. */
class Ros1Clearpath_mission_manager_msgsGetWaypointClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_manager_msgsGetWaypointRequest;using Response=Ros1Clearpath_mission_manager_msgsGetWaypointResponse;
Ros1Clearpath_mission_manager_msgsGetWaypointClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_manager_msgsGetWaypointClient(Ros1Clearpath_mission_manager_msgsGetWaypointClient const&)=delete;Ros1Clearpath_mission_manager_msgsGetWaypointClient& operator=(Ros1Clearpath_mission_manager_msgsGetWaypointClient const&)=delete;Ros1Clearpath_mission_manager_msgsGetWaypointClient(Ros1Clearpath_mission_manager_msgsGetWaypointClient&&)=delete;
~Ros1Clearpath_mission_manager_msgsGetWaypointClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_GetWaypoint_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_manager_msgs/GetWaypoint server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_manager_msgsGetWaypointServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_manager_msgsGetWaypointResponse(Ros1Clearpath_mission_manager_msgsGetWaypointRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_manager_msgsGetWaypointRequest;using Response=Ros1Clearpath_mission_manager_msgsGetWaypointResponse;
Ros1Clearpath_mission_manager_msgsGetWaypointServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_manager_msgs_GetWaypoint_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_GetWaypoint_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_manager_msgs_GetWaypoint_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_manager_msgs_GetWaypoint_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_manager_msgs/ImportData client; default policy returns13 from construction. */
class Ros1Clearpath_mission_manager_msgsImportDataClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_manager_msgsImportDataRequest;using Response=Ros1Clearpath_mission_manager_msgsImportDataResponse;
Ros1Clearpath_mission_manager_msgsImportDataClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_manager_msgsImportDataClient(Ros1Clearpath_mission_manager_msgsImportDataClient const&)=delete;Ros1Clearpath_mission_manager_msgsImportDataClient& operator=(Ros1Clearpath_mission_manager_msgsImportDataClient const&)=delete;Ros1Clearpath_mission_manager_msgsImportDataClient(Ros1Clearpath_mission_manager_msgsImportDataClient&&)=delete;
~Ros1Clearpath_mission_manager_msgsImportDataClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_ImportData_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_manager_msgs/ImportData server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_manager_msgsImportDataServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_manager_msgsImportDataResponse(Ros1Clearpath_mission_manager_msgsImportDataRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_manager_msgsImportDataRequest;using Response=Ros1Clearpath_mission_manager_msgsImportDataResponse;
Ros1Clearpath_mission_manager_msgsImportDataServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_manager_msgs_ImportData_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_ImportData_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_manager_msgs_ImportData_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_manager_msgs_ImportData_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_manager_msgs/ImportMission client; default policy returns13 from construction. */
class Ros1Clearpath_mission_manager_msgsImportMissionClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_manager_msgsImportMissionRequest;using Response=Ros1Clearpath_mission_manager_msgsImportMissionResponse;
Ros1Clearpath_mission_manager_msgsImportMissionClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_manager_msgsImportMissionClient(Ros1Clearpath_mission_manager_msgsImportMissionClient const&)=delete;Ros1Clearpath_mission_manager_msgsImportMissionClient& operator=(Ros1Clearpath_mission_manager_msgsImportMissionClient const&)=delete;Ros1Clearpath_mission_manager_msgsImportMissionClient(Ros1Clearpath_mission_manager_msgsImportMissionClient&&)=delete;
~Ros1Clearpath_mission_manager_msgsImportMissionClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_ImportMission_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_manager_msgs/ImportMission server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_manager_msgsImportMissionServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_manager_msgsImportMissionResponse(Ros1Clearpath_mission_manager_msgsImportMissionRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_manager_msgsImportMissionRequest;using Response=Ros1Clearpath_mission_manager_msgsImportMissionResponse;
Ros1Clearpath_mission_manager_msgsImportMissionServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_manager_msgs_ImportMission_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_ImportMission_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_manager_msgs_ImportMission_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_manager_msgs_ImportMission_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_manager_msgs/ImportNetworkMap client; default policy returns13 from construction. */
class Ros1Clearpath_mission_manager_msgsImportNetworkMapClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_manager_msgsImportNetworkMapRequest;using Response=Ros1Clearpath_mission_manager_msgsImportNetworkMapResponse;
Ros1Clearpath_mission_manager_msgsImportNetworkMapClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_manager_msgsImportNetworkMapClient(Ros1Clearpath_mission_manager_msgsImportNetworkMapClient const&)=delete;Ros1Clearpath_mission_manager_msgsImportNetworkMapClient& operator=(Ros1Clearpath_mission_manager_msgsImportNetworkMapClient const&)=delete;Ros1Clearpath_mission_manager_msgsImportNetworkMapClient(Ros1Clearpath_mission_manager_msgsImportNetworkMapClient&&)=delete;
~Ros1Clearpath_mission_manager_msgsImportNetworkMapClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_ImportNetworkMap_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_manager_msgs/ImportNetworkMap server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_manager_msgsImportNetworkMapServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_manager_msgsImportNetworkMapResponse(Ros1Clearpath_mission_manager_msgsImportNetworkMapRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_manager_msgsImportNetworkMapRequest;using Response=Ros1Clearpath_mission_manager_msgsImportNetworkMapResponse;
Ros1Clearpath_mission_manager_msgsImportNetworkMapServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_manager_msgs_ImportNetworkMap_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_ImportNetworkMap_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_manager_msgs_ImportNetworkMap_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_manager_msgs_ImportNetworkMap_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_manager_msgs/ImportNetworkMission client; default policy returns13 from construction. */
class Ros1Clearpath_mission_manager_msgsImportNetworkMissionClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_manager_msgsImportNetworkMissionRequest;using Response=Ros1Clearpath_mission_manager_msgsImportNetworkMissionResponse;
Ros1Clearpath_mission_manager_msgsImportNetworkMissionClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_manager_msgsImportNetworkMissionClient(Ros1Clearpath_mission_manager_msgsImportNetworkMissionClient const&)=delete;Ros1Clearpath_mission_manager_msgsImportNetworkMissionClient& operator=(Ros1Clearpath_mission_manager_msgsImportNetworkMissionClient const&)=delete;Ros1Clearpath_mission_manager_msgsImportNetworkMissionClient(Ros1Clearpath_mission_manager_msgsImportNetworkMissionClient&&)=delete;
~Ros1Clearpath_mission_manager_msgsImportNetworkMissionClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_ImportNetworkMission_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_manager_msgs/ImportNetworkMission server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_manager_msgsImportNetworkMissionServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_manager_msgsImportNetworkMissionResponse(Ros1Clearpath_mission_manager_msgsImportNetworkMissionRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_manager_msgsImportNetworkMissionRequest;using Response=Ros1Clearpath_mission_manager_msgsImportNetworkMissionResponse;
Ros1Clearpath_mission_manager_msgsImportNetworkMissionServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_manager_msgs_ImportNetworkMission_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_ImportNetworkMission_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_manager_msgs_ImportNetworkMission_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_manager_msgs_ImportNetworkMission_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_manager_msgs/SplitNetworkEdge client; default policy returns13 from construction. */
class Ros1Clearpath_mission_manager_msgsSplitNetworkEdgeClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_manager_msgsSplitNetworkEdgeRequest;using Response=Ros1Clearpath_mission_manager_msgsSplitNetworkEdgeResponse;
Ros1Clearpath_mission_manager_msgsSplitNetworkEdgeClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_manager_msgsSplitNetworkEdgeClient(Ros1Clearpath_mission_manager_msgsSplitNetworkEdgeClient const&)=delete;Ros1Clearpath_mission_manager_msgsSplitNetworkEdgeClient& operator=(Ros1Clearpath_mission_manager_msgsSplitNetworkEdgeClient const&)=delete;Ros1Clearpath_mission_manager_msgsSplitNetworkEdgeClient(Ros1Clearpath_mission_manager_msgsSplitNetworkEdgeClient&&)=delete;
~Ros1Clearpath_mission_manager_msgsSplitNetworkEdgeClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_SplitNetworkEdge_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_manager_msgs/SplitNetworkEdge server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_manager_msgsSplitNetworkEdgeServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_manager_msgsSplitNetworkEdgeResponse(Ros1Clearpath_mission_manager_msgsSplitNetworkEdgeRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_manager_msgsSplitNetworkEdgeRequest;using Response=Ros1Clearpath_mission_manager_msgsSplitNetworkEdgeResponse;
Ros1Clearpath_mission_manager_msgsSplitNetworkEdgeServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_manager_msgs_SplitNetworkEdge_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_SplitNetworkEdge_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_manager_msgs_SplitNetworkEdge_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_manager_msgs_SplitNetworkEdge_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_manager_msgs/UpdateMission client; default policy returns13 from construction. */
class Ros1Clearpath_mission_manager_msgsUpdateMissionClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_manager_msgsUpdateMissionRequest;using Response=Ros1Clearpath_mission_manager_msgsUpdateMissionResponse;
Ros1Clearpath_mission_manager_msgsUpdateMissionClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_manager_msgsUpdateMissionClient(Ros1Clearpath_mission_manager_msgsUpdateMissionClient const&)=delete;Ros1Clearpath_mission_manager_msgsUpdateMissionClient& operator=(Ros1Clearpath_mission_manager_msgsUpdateMissionClient const&)=delete;Ros1Clearpath_mission_manager_msgsUpdateMissionClient(Ros1Clearpath_mission_manager_msgsUpdateMissionClient&&)=delete;
~Ros1Clearpath_mission_manager_msgsUpdateMissionClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_UpdateMission_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_manager_msgs/UpdateMission server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_manager_msgsUpdateMissionServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_manager_msgsUpdateMissionResponse(Ros1Clearpath_mission_manager_msgsUpdateMissionRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_manager_msgsUpdateMissionRequest;using Response=Ros1Clearpath_mission_manager_msgsUpdateMissionResponse;
Ros1Clearpath_mission_manager_msgsUpdateMissionServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_manager_msgs_UpdateMission_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_UpdateMission_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_manager_msgs_UpdateMission_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_manager_msgs_UpdateMission_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_manager_msgs/UpdateNetworkEdge client; default policy returns13 from construction. */
class Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgeClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgeRequest;using Response=Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgeResponse;
Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgeClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgeClient(Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgeClient const&)=delete;Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgeClient& operator=(Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgeClient const&)=delete;Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgeClient(Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgeClient&&)=delete;
~Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgeClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_UpdateNetworkEdge_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_manager_msgs/UpdateNetworkEdge server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgeServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgeResponse(Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgeRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgeRequest;using Response=Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgeResponse;
Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgeServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_manager_msgs_UpdateNetworkEdge_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_UpdateNetworkEdge_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_manager_msgs_UpdateNetworkEdge_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_manager_msgs_UpdateNetworkEdge_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_manager_msgs/UpdateNetworkEdges client; default policy returns13 from construction. */
class Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgesClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgesRequest;using Response=Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgesResponse;
Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgesClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgesClient(Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgesClient const&)=delete;Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgesClient& operator=(Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgesClient const&)=delete;Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgesClient(Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgesClient&&)=delete;
~Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgesClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_UpdateNetworkEdges_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_manager_msgs/UpdateNetworkEdges server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgesServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgesResponse(Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgesRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgesRequest;using Response=Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgesResponse;
Ros1Clearpath_mission_manager_msgsUpdateNetworkEdgesServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_manager_msgs_UpdateNetworkEdges_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_UpdateNetworkEdges_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_manager_msgs_UpdateNetworkEdges_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_manager_msgs_UpdateNetworkEdges_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_manager_msgs/UpdateNetworkMap client; default policy returns13 from construction. */
class Ros1Clearpath_mission_manager_msgsUpdateNetworkMapClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_manager_msgsUpdateNetworkMapRequest;using Response=Ros1Clearpath_mission_manager_msgsUpdateNetworkMapResponse;
Ros1Clearpath_mission_manager_msgsUpdateNetworkMapClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_manager_msgsUpdateNetworkMapClient(Ros1Clearpath_mission_manager_msgsUpdateNetworkMapClient const&)=delete;Ros1Clearpath_mission_manager_msgsUpdateNetworkMapClient& operator=(Ros1Clearpath_mission_manager_msgsUpdateNetworkMapClient const&)=delete;Ros1Clearpath_mission_manager_msgsUpdateNetworkMapClient(Ros1Clearpath_mission_manager_msgsUpdateNetworkMapClient&&)=delete;
~Ros1Clearpath_mission_manager_msgsUpdateNetworkMapClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_UpdateNetworkMap_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_manager_msgs/UpdateNetworkMap server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_manager_msgsUpdateNetworkMapServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_manager_msgsUpdateNetworkMapResponse(Ros1Clearpath_mission_manager_msgsUpdateNetworkMapRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_manager_msgsUpdateNetworkMapRequest;using Response=Ros1Clearpath_mission_manager_msgsUpdateNetworkMapResponse;
Ros1Clearpath_mission_manager_msgsUpdateNetworkMapServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_manager_msgs_UpdateNetworkMap_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_UpdateNetworkMap_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_manager_msgs_UpdateNetworkMap_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_manager_msgs_UpdateNetworkMap_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_manager_msgs/UpdateNetworkMission client; default policy returns13 from construction. */
class Ros1Clearpath_mission_manager_msgsUpdateNetworkMissionClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_manager_msgsUpdateNetworkMissionRequest;using Response=Ros1Clearpath_mission_manager_msgsUpdateNetworkMissionResponse;
Ros1Clearpath_mission_manager_msgsUpdateNetworkMissionClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_manager_msgsUpdateNetworkMissionClient(Ros1Clearpath_mission_manager_msgsUpdateNetworkMissionClient const&)=delete;Ros1Clearpath_mission_manager_msgsUpdateNetworkMissionClient& operator=(Ros1Clearpath_mission_manager_msgsUpdateNetworkMissionClient const&)=delete;Ros1Clearpath_mission_manager_msgsUpdateNetworkMissionClient(Ros1Clearpath_mission_manager_msgsUpdateNetworkMissionClient&&)=delete;
~Ros1Clearpath_mission_manager_msgsUpdateNetworkMissionClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_UpdateNetworkMission_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_manager_msgs/UpdateNetworkMission server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_manager_msgsUpdateNetworkMissionServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_manager_msgsUpdateNetworkMissionResponse(Ros1Clearpath_mission_manager_msgsUpdateNetworkMissionRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_manager_msgsUpdateNetworkMissionRequest;using Response=Ros1Clearpath_mission_manager_msgsUpdateNetworkMissionResponse;
Ros1Clearpath_mission_manager_msgsUpdateNetworkMissionServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_manager_msgs_UpdateNetworkMission_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_UpdateNetworkMission_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_manager_msgs_UpdateNetworkMission_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_manager_msgs_UpdateNetworkMission_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_manager_msgs/UpdateNetworkPoint client; default policy returns13 from construction. */
class Ros1Clearpath_mission_manager_msgsUpdateNetworkPointClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_manager_msgsUpdateNetworkPointRequest;using Response=Ros1Clearpath_mission_manager_msgsUpdateNetworkPointResponse;
Ros1Clearpath_mission_manager_msgsUpdateNetworkPointClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_manager_msgsUpdateNetworkPointClient(Ros1Clearpath_mission_manager_msgsUpdateNetworkPointClient const&)=delete;Ros1Clearpath_mission_manager_msgsUpdateNetworkPointClient& operator=(Ros1Clearpath_mission_manager_msgsUpdateNetworkPointClient const&)=delete;Ros1Clearpath_mission_manager_msgsUpdateNetworkPointClient(Ros1Clearpath_mission_manager_msgsUpdateNetworkPointClient&&)=delete;
~Ros1Clearpath_mission_manager_msgsUpdateNetworkPointClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_UpdateNetworkPoint_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_manager_msgs/UpdateNetworkPoint server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_manager_msgsUpdateNetworkPointServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_manager_msgsUpdateNetworkPointResponse(Ros1Clearpath_mission_manager_msgsUpdateNetworkPointRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_manager_msgsUpdateNetworkPointRequest;using Response=Ros1Clearpath_mission_manager_msgsUpdateNetworkPointResponse;
Ros1Clearpath_mission_manager_msgsUpdateNetworkPointServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_manager_msgs_UpdateNetworkPoint_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_UpdateNetworkPoint_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_manager_msgs_UpdateNetworkPoint_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_manager_msgs_UpdateNetworkPoint_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_manager_msgs/UpdateNetworkPoints client; default policy returns13 from construction. */
class Ros1Clearpath_mission_manager_msgsUpdateNetworkPointsClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_manager_msgsUpdateNetworkPointsRequest;using Response=Ros1Clearpath_mission_manager_msgsUpdateNetworkPointsResponse;
Ros1Clearpath_mission_manager_msgsUpdateNetworkPointsClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_manager_msgsUpdateNetworkPointsClient(Ros1Clearpath_mission_manager_msgsUpdateNetworkPointsClient const&)=delete;Ros1Clearpath_mission_manager_msgsUpdateNetworkPointsClient& operator=(Ros1Clearpath_mission_manager_msgsUpdateNetworkPointsClient const&)=delete;Ros1Clearpath_mission_manager_msgsUpdateNetworkPointsClient(Ros1Clearpath_mission_manager_msgsUpdateNetworkPointsClient&&)=delete;
~Ros1Clearpath_mission_manager_msgsUpdateNetworkPointsClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_UpdateNetworkPoints_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_manager_msgs/UpdateNetworkPoints server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_manager_msgsUpdateNetworkPointsServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_manager_msgsUpdateNetworkPointsResponse(Ros1Clearpath_mission_manager_msgsUpdateNetworkPointsRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_manager_msgsUpdateNetworkPointsRequest;using Response=Ros1Clearpath_mission_manager_msgsUpdateNetworkPointsResponse;
Ros1Clearpath_mission_manager_msgsUpdateNetworkPointsServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_manager_msgs_UpdateNetworkPoints_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_UpdateNetworkPoints_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_manager_msgs_UpdateNetworkPoints_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_manager_msgs_UpdateNetworkPoints_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_manager_msgs/UpdatePointOfInterest client; default policy returns13 from construction. */
class Ros1Clearpath_mission_manager_msgsUpdatePointOfInterestClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_manager_msgsUpdatePointOfInterestRequest;using Response=Ros1Clearpath_mission_manager_msgsUpdatePointOfInterestResponse;
Ros1Clearpath_mission_manager_msgsUpdatePointOfInterestClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_manager_msgsUpdatePointOfInterestClient(Ros1Clearpath_mission_manager_msgsUpdatePointOfInterestClient const&)=delete;Ros1Clearpath_mission_manager_msgsUpdatePointOfInterestClient& operator=(Ros1Clearpath_mission_manager_msgsUpdatePointOfInterestClient const&)=delete;Ros1Clearpath_mission_manager_msgsUpdatePointOfInterestClient(Ros1Clearpath_mission_manager_msgsUpdatePointOfInterestClient&&)=delete;
~Ros1Clearpath_mission_manager_msgsUpdatePointOfInterestClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_UpdatePointOfInterest_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_manager_msgs/UpdatePointOfInterest server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_manager_msgsUpdatePointOfInterestServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_manager_msgsUpdatePointOfInterestResponse(Ros1Clearpath_mission_manager_msgsUpdatePointOfInterestRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_manager_msgsUpdatePointOfInterestRequest;using Response=Ros1Clearpath_mission_manager_msgsUpdatePointOfInterestResponse;
Ros1Clearpath_mission_manager_msgsUpdatePointOfInterestServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_manager_msgs_UpdatePointOfInterest_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_UpdatePointOfInterest_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_manager_msgs_UpdatePointOfInterest_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_manager_msgs_UpdatePointOfInterest_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_manager_msgs/UpdateTask client; default policy returns13 from construction. */
class Ros1Clearpath_mission_manager_msgsUpdateTaskClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_manager_msgsUpdateTaskRequest;using Response=Ros1Clearpath_mission_manager_msgsUpdateTaskResponse;
Ros1Clearpath_mission_manager_msgsUpdateTaskClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_manager_msgsUpdateTaskClient(Ros1Clearpath_mission_manager_msgsUpdateTaskClient const&)=delete;Ros1Clearpath_mission_manager_msgsUpdateTaskClient& operator=(Ros1Clearpath_mission_manager_msgsUpdateTaskClient const&)=delete;Ros1Clearpath_mission_manager_msgsUpdateTaskClient(Ros1Clearpath_mission_manager_msgsUpdateTaskClient&&)=delete;
~Ros1Clearpath_mission_manager_msgsUpdateTaskClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_UpdateTask_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_manager_msgs/UpdateTask server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_manager_msgsUpdateTaskServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_manager_msgsUpdateTaskResponse(Ros1Clearpath_mission_manager_msgsUpdateTaskRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_manager_msgsUpdateTaskRequest;using Response=Ros1Clearpath_mission_manager_msgsUpdateTaskResponse;
Ros1Clearpath_mission_manager_msgsUpdateTaskServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_manager_msgs_UpdateTask_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_UpdateTask_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_manager_msgs_UpdateTask_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_manager_msgs_UpdateTask_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_manager_msgs/UpdateWaypoint client; default policy returns13 from construction. */
class Ros1Clearpath_mission_manager_msgsUpdateWaypointClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_manager_msgsUpdateWaypointRequest;using Response=Ros1Clearpath_mission_manager_msgsUpdateWaypointResponse;
Ros1Clearpath_mission_manager_msgsUpdateWaypointClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_manager_msgsUpdateWaypointClient(Ros1Clearpath_mission_manager_msgsUpdateWaypointClient const&)=delete;Ros1Clearpath_mission_manager_msgsUpdateWaypointClient& operator=(Ros1Clearpath_mission_manager_msgsUpdateWaypointClient const&)=delete;Ros1Clearpath_mission_manager_msgsUpdateWaypointClient(Ros1Clearpath_mission_manager_msgsUpdateWaypointClient&&)=delete;
~Ros1Clearpath_mission_manager_msgsUpdateWaypointClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_UpdateWaypoint_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_manager_msgs/UpdateWaypoint server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_manager_msgsUpdateWaypointServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_manager_msgsUpdateWaypointResponse(Ros1Clearpath_mission_manager_msgsUpdateWaypointRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_manager_msgsUpdateWaypointRequest;using Response=Ros1Clearpath_mission_manager_msgsUpdateWaypointResponse;
Ros1Clearpath_mission_manager_msgsUpdateWaypointServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_manager_msgs_UpdateWaypoint_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_UpdateWaypoint_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_manager_msgs_UpdateWaypoint_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_manager_msgs_UpdateWaypoint_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_manager_msgs/ValidateNetworkMap client; default policy returns13 from construction. */
class Ros1Clearpath_mission_manager_msgsValidateNetworkMapClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_manager_msgsValidateNetworkMapRequest;using Response=Ros1Clearpath_mission_manager_msgsValidateNetworkMapResponse;
Ros1Clearpath_mission_manager_msgsValidateNetworkMapClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_manager_msgsValidateNetworkMapClient(Ros1Clearpath_mission_manager_msgsValidateNetworkMapClient const&)=delete;Ros1Clearpath_mission_manager_msgsValidateNetworkMapClient& operator=(Ros1Clearpath_mission_manager_msgsValidateNetworkMapClient const&)=delete;Ros1Clearpath_mission_manager_msgsValidateNetworkMapClient(Ros1Clearpath_mission_manager_msgsValidateNetworkMapClient&&)=delete;
~Ros1Clearpath_mission_manager_msgsValidateNetworkMapClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_ValidateNetworkMap_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_manager_msgs/ValidateNetworkMap server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_manager_msgsValidateNetworkMapServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_manager_msgsValidateNetworkMapResponse(Ros1Clearpath_mission_manager_msgsValidateNetworkMapRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_manager_msgsValidateNetworkMapRequest;using Response=Ros1Clearpath_mission_manager_msgsValidateNetworkMapResponse;
Ros1Clearpath_mission_manager_msgsValidateNetworkMapServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_manager_msgs_ValidateNetworkMap_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_manager_msgs_ValidateNetworkMap_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_manager_msgs_ValidateNetworkMap_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_manager_msgs_ValidateNetworkMap_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_scheduler_msgs/CloneSchedule client; default policy returns13 from construction. */
class Ros1Clearpath_mission_scheduler_msgsCloneScheduleClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_scheduler_msgsCloneScheduleRequest;using Response=Ros1Clearpath_mission_scheduler_msgsCloneScheduleResponse;
Ros1Clearpath_mission_scheduler_msgsCloneScheduleClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_scheduler_msgsCloneScheduleClient(Ros1Clearpath_mission_scheduler_msgsCloneScheduleClient const&)=delete;Ros1Clearpath_mission_scheduler_msgsCloneScheduleClient& operator=(Ros1Clearpath_mission_scheduler_msgsCloneScheduleClient const&)=delete;Ros1Clearpath_mission_scheduler_msgsCloneScheduleClient(Ros1Clearpath_mission_scheduler_msgsCloneScheduleClient&&)=delete;
~Ros1Clearpath_mission_scheduler_msgsCloneScheduleClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_scheduler_msgs_CloneSchedule_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_scheduler_msgs/CloneSchedule server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_scheduler_msgsCloneScheduleServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_scheduler_msgsCloneScheduleResponse(Ros1Clearpath_mission_scheduler_msgsCloneScheduleRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_scheduler_msgsCloneScheduleRequest;using Response=Ros1Clearpath_mission_scheduler_msgsCloneScheduleResponse;
Ros1Clearpath_mission_scheduler_msgsCloneScheduleServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_scheduler_msgs_CloneSchedule_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_scheduler_msgs_CloneSchedule_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_scheduler_msgs_CloneSchedule_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_scheduler_msgs_CloneSchedule_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_scheduler_msgs/CreateSchedule client; default policy returns13 from construction. */
class Ros1Clearpath_mission_scheduler_msgsCreateScheduleClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_scheduler_msgsCreateScheduleRequest;using Response=Ros1Clearpath_mission_scheduler_msgsCreateScheduleResponse;
Ros1Clearpath_mission_scheduler_msgsCreateScheduleClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_scheduler_msgsCreateScheduleClient(Ros1Clearpath_mission_scheduler_msgsCreateScheduleClient const&)=delete;Ros1Clearpath_mission_scheduler_msgsCreateScheduleClient& operator=(Ros1Clearpath_mission_scheduler_msgsCreateScheduleClient const&)=delete;Ros1Clearpath_mission_scheduler_msgsCreateScheduleClient(Ros1Clearpath_mission_scheduler_msgsCreateScheduleClient&&)=delete;
~Ros1Clearpath_mission_scheduler_msgsCreateScheduleClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_scheduler_msgs_CreateSchedule_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_scheduler_msgs/CreateSchedule server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_scheduler_msgsCreateScheduleServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_scheduler_msgsCreateScheduleResponse(Ros1Clearpath_mission_scheduler_msgsCreateScheduleRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_scheduler_msgsCreateScheduleRequest;using Response=Ros1Clearpath_mission_scheduler_msgsCreateScheduleResponse;
Ros1Clearpath_mission_scheduler_msgsCreateScheduleServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_scheduler_msgs_CreateSchedule_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_scheduler_msgs_CreateSchedule_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_scheduler_msgs_CreateSchedule_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_scheduler_msgs_CreateSchedule_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_scheduler_msgs/EnableSchedule client; default policy returns13 from construction. */
class Ros1Clearpath_mission_scheduler_msgsEnableScheduleClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_scheduler_msgsEnableScheduleRequest;using Response=Ros1Clearpath_mission_scheduler_msgsEnableScheduleResponse;
Ros1Clearpath_mission_scheduler_msgsEnableScheduleClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_scheduler_msgsEnableScheduleClient(Ros1Clearpath_mission_scheduler_msgsEnableScheduleClient const&)=delete;Ros1Clearpath_mission_scheduler_msgsEnableScheduleClient& operator=(Ros1Clearpath_mission_scheduler_msgsEnableScheduleClient const&)=delete;Ros1Clearpath_mission_scheduler_msgsEnableScheduleClient(Ros1Clearpath_mission_scheduler_msgsEnableScheduleClient&&)=delete;
~Ros1Clearpath_mission_scheduler_msgsEnableScheduleClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_scheduler_msgs_EnableSchedule_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_scheduler_msgs/EnableSchedule server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_scheduler_msgsEnableScheduleServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_scheduler_msgsEnableScheduleResponse(Ros1Clearpath_mission_scheduler_msgsEnableScheduleRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_scheduler_msgsEnableScheduleRequest;using Response=Ros1Clearpath_mission_scheduler_msgsEnableScheduleResponse;
Ros1Clearpath_mission_scheduler_msgsEnableScheduleServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_scheduler_msgs_EnableSchedule_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_scheduler_msgs_EnableSchedule_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_scheduler_msgs_EnableSchedule_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_scheduler_msgs_EnableSchedule_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_scheduler_msgs/ExportData client; default policy returns13 from construction. */
class Ros1Clearpath_mission_scheduler_msgsExportDataClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_scheduler_msgsExportDataRequest;using Response=Ros1Clearpath_mission_scheduler_msgsExportDataResponse;
Ros1Clearpath_mission_scheduler_msgsExportDataClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_scheduler_msgsExportDataClient(Ros1Clearpath_mission_scheduler_msgsExportDataClient const&)=delete;Ros1Clearpath_mission_scheduler_msgsExportDataClient& operator=(Ros1Clearpath_mission_scheduler_msgsExportDataClient const&)=delete;Ros1Clearpath_mission_scheduler_msgsExportDataClient(Ros1Clearpath_mission_scheduler_msgsExportDataClient&&)=delete;
~Ros1Clearpath_mission_scheduler_msgsExportDataClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_scheduler_msgs_ExportData_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_scheduler_msgs/ExportData server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_scheduler_msgsExportDataServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_scheduler_msgsExportDataResponse(Ros1Clearpath_mission_scheduler_msgsExportDataRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_scheduler_msgsExportDataRequest;using Response=Ros1Clearpath_mission_scheduler_msgsExportDataResponse;
Ros1Clearpath_mission_scheduler_msgsExportDataServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_scheduler_msgs_ExportData_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_scheduler_msgs_ExportData_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_scheduler_msgs_ExportData_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_scheduler_msgs_ExportData_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_scheduler_msgs/GetAllSchedules client; default policy returns13 from construction. */
class Ros1Clearpath_mission_scheduler_msgsGetAllSchedulesClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_scheduler_msgsGetAllSchedulesRequest;using Response=Ros1Clearpath_mission_scheduler_msgsGetAllSchedulesResponse;
Ros1Clearpath_mission_scheduler_msgsGetAllSchedulesClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_scheduler_msgsGetAllSchedulesClient(Ros1Clearpath_mission_scheduler_msgsGetAllSchedulesClient const&)=delete;Ros1Clearpath_mission_scheduler_msgsGetAllSchedulesClient& operator=(Ros1Clearpath_mission_scheduler_msgsGetAllSchedulesClient const&)=delete;Ros1Clearpath_mission_scheduler_msgsGetAllSchedulesClient(Ros1Clearpath_mission_scheduler_msgsGetAllSchedulesClient&&)=delete;
~Ros1Clearpath_mission_scheduler_msgsGetAllSchedulesClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_scheduler_msgs_GetAllSchedules_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_scheduler_msgs/GetAllSchedules server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_scheduler_msgsGetAllSchedulesServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_scheduler_msgsGetAllSchedulesResponse(Ros1Clearpath_mission_scheduler_msgsGetAllSchedulesRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_scheduler_msgsGetAllSchedulesRequest;using Response=Ros1Clearpath_mission_scheduler_msgsGetAllSchedulesResponse;
Ros1Clearpath_mission_scheduler_msgsGetAllSchedulesServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_scheduler_msgs_GetAllSchedules_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_scheduler_msgs_GetAllSchedules_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_scheduler_msgs_GetAllSchedules_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_scheduler_msgs_GetAllSchedules_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_scheduler_msgs/GetNextSchedule client; default policy returns13 from construction. */
class Ros1Clearpath_mission_scheduler_msgsGetNextScheduleClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_scheduler_msgsGetNextScheduleRequest;using Response=Ros1Clearpath_mission_scheduler_msgsGetNextScheduleResponse;
Ros1Clearpath_mission_scheduler_msgsGetNextScheduleClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_scheduler_msgsGetNextScheduleClient(Ros1Clearpath_mission_scheduler_msgsGetNextScheduleClient const&)=delete;Ros1Clearpath_mission_scheduler_msgsGetNextScheduleClient& operator=(Ros1Clearpath_mission_scheduler_msgsGetNextScheduleClient const&)=delete;Ros1Clearpath_mission_scheduler_msgsGetNextScheduleClient(Ros1Clearpath_mission_scheduler_msgsGetNextScheduleClient&&)=delete;
~Ros1Clearpath_mission_scheduler_msgsGetNextScheduleClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_scheduler_msgs_GetNextSchedule_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_scheduler_msgs/GetNextSchedule server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_scheduler_msgsGetNextScheduleServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_scheduler_msgsGetNextScheduleResponse(Ros1Clearpath_mission_scheduler_msgsGetNextScheduleRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_scheduler_msgsGetNextScheduleRequest;using Response=Ros1Clearpath_mission_scheduler_msgsGetNextScheduleResponse;
Ros1Clearpath_mission_scheduler_msgsGetNextScheduleServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_scheduler_msgs_GetNextSchedule_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_scheduler_msgs_GetNextSchedule_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_scheduler_msgs_GetNextSchedule_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_scheduler_msgs_GetNextSchedule_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_scheduler_msgs/GetSchedule client; default policy returns13 from construction. */
class Ros1Clearpath_mission_scheduler_msgsGetScheduleClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_scheduler_msgsGetScheduleRequest;using Response=Ros1Clearpath_mission_scheduler_msgsGetScheduleResponse;
Ros1Clearpath_mission_scheduler_msgsGetScheduleClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_scheduler_msgsGetScheduleClient(Ros1Clearpath_mission_scheduler_msgsGetScheduleClient const&)=delete;Ros1Clearpath_mission_scheduler_msgsGetScheduleClient& operator=(Ros1Clearpath_mission_scheduler_msgsGetScheduleClient const&)=delete;Ros1Clearpath_mission_scheduler_msgsGetScheduleClient(Ros1Clearpath_mission_scheduler_msgsGetScheduleClient&&)=delete;
~Ros1Clearpath_mission_scheduler_msgsGetScheduleClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_scheduler_msgs_GetSchedule_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_scheduler_msgs/GetSchedule server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_scheduler_msgsGetScheduleServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_scheduler_msgsGetScheduleResponse(Ros1Clearpath_mission_scheduler_msgsGetScheduleRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_scheduler_msgsGetScheduleRequest;using Response=Ros1Clearpath_mission_scheduler_msgsGetScheduleResponse;
Ros1Clearpath_mission_scheduler_msgsGetScheduleServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_scheduler_msgs_GetSchedule_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_scheduler_msgs_GetSchedule_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_scheduler_msgs_GetSchedule_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_scheduler_msgs_GetSchedule_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_scheduler_msgs/ImportData client; default policy returns13 from construction. */
class Ros1Clearpath_mission_scheduler_msgsImportDataClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_scheduler_msgsImportDataRequest;using Response=Ros1Clearpath_mission_scheduler_msgsImportDataResponse;
Ros1Clearpath_mission_scheduler_msgsImportDataClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_scheduler_msgsImportDataClient(Ros1Clearpath_mission_scheduler_msgsImportDataClient const&)=delete;Ros1Clearpath_mission_scheduler_msgsImportDataClient& operator=(Ros1Clearpath_mission_scheduler_msgsImportDataClient const&)=delete;Ros1Clearpath_mission_scheduler_msgsImportDataClient(Ros1Clearpath_mission_scheduler_msgsImportDataClient&&)=delete;
~Ros1Clearpath_mission_scheduler_msgsImportDataClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_scheduler_msgs_ImportData_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_scheduler_msgs/ImportData server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_scheduler_msgsImportDataServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_scheduler_msgsImportDataResponse(Ros1Clearpath_mission_scheduler_msgsImportDataRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_scheduler_msgsImportDataRequest;using Response=Ros1Clearpath_mission_scheduler_msgsImportDataResponse;
Ros1Clearpath_mission_scheduler_msgsImportDataServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_scheduler_msgs_ImportData_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_scheduler_msgs_ImportData_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_scheduler_msgs_ImportData_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_scheduler_msgs_ImportData_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_mission_scheduler_msgs/UpdateSchedule client; default policy returns13 from construction. */
class Ros1Clearpath_mission_scheduler_msgsUpdateScheduleClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_mission_scheduler_msgsUpdateScheduleRequest;using Response=Ros1Clearpath_mission_scheduler_msgsUpdateScheduleResponse;
Ros1Clearpath_mission_scheduler_msgsUpdateScheduleClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_mission_scheduler_msgsUpdateScheduleClient(Ros1Clearpath_mission_scheduler_msgsUpdateScheduleClient const&)=delete;Ros1Clearpath_mission_scheduler_msgsUpdateScheduleClient& operator=(Ros1Clearpath_mission_scheduler_msgsUpdateScheduleClient const&)=delete;Ros1Clearpath_mission_scheduler_msgsUpdateScheduleClient(Ros1Clearpath_mission_scheduler_msgsUpdateScheduleClient&&)=delete;
~Ros1Clearpath_mission_scheduler_msgsUpdateScheduleClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_scheduler_msgs_UpdateSchedule_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_mission_scheduler_msgs/UpdateSchedule server; advertise/respond stay separate authorities. */
class Ros1Clearpath_mission_scheduler_msgsUpdateScheduleServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_mission_scheduler_msgsUpdateScheduleResponse(Ros1Clearpath_mission_scheduler_msgsUpdateScheduleRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_mission_scheduler_msgsUpdateScheduleRequest;using Response=Ros1Clearpath_mission_scheduler_msgsUpdateScheduleResponse;
Ros1Clearpath_mission_scheduler_msgsUpdateScheduleServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_mission_scheduler_msgs_UpdateSchedule_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_mission_scheduler_msgs_UpdateSchedule_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_mission_scheduler_msgs_UpdateSchedule_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_mission_scheduler_msgs_UpdateSchedule_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_navigation_msgs/AutonomyTaskSrv client; default policy returns13 from construction. */
class Ros1Clearpath_navigation_msgsAutonomyTaskSrvClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_navigation_msgsAutonomyTaskSrvRequest;using Response=Ros1Clearpath_navigation_msgsAutonomyTaskSrvResponse;
Ros1Clearpath_navigation_msgsAutonomyTaskSrvClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_navigation_msgsAutonomyTaskSrvClient(Ros1Clearpath_navigation_msgsAutonomyTaskSrvClient const&)=delete;Ros1Clearpath_navigation_msgsAutonomyTaskSrvClient& operator=(Ros1Clearpath_navigation_msgsAutonomyTaskSrvClient const&)=delete;Ros1Clearpath_navigation_msgsAutonomyTaskSrvClient(Ros1Clearpath_navigation_msgsAutonomyTaskSrvClient&&)=delete;
~Ros1Clearpath_navigation_msgsAutonomyTaskSrvClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_navigation_msgs_AutonomyTaskSrv_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_navigation_msgs/AutonomyTaskSrv server; advertise/respond stay separate authorities. */
class Ros1Clearpath_navigation_msgsAutonomyTaskSrvServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_navigation_msgsAutonomyTaskSrvResponse(Ros1Clearpath_navigation_msgsAutonomyTaskSrvRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_navigation_msgsAutonomyTaskSrvRequest;using Response=Ros1Clearpath_navigation_msgsAutonomyTaskSrvResponse;
Ros1Clearpath_navigation_msgsAutonomyTaskSrvServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_navigation_msgs_AutonomyTaskSrv_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_navigation_msgs_AutonomyTaskSrv_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_navigation_msgs_AutonomyTaskSrv_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_navigation_msgs_AutonomyTaskSrv_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_navigation_msgs/SetDelayCompensation client; default policy returns13 from construction. */
class Ros1Clearpath_navigation_msgsSetDelayCompensationClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_navigation_msgsSetDelayCompensationRequest;using Response=Ros1Clearpath_navigation_msgsSetDelayCompensationResponse;
Ros1Clearpath_navigation_msgsSetDelayCompensationClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_navigation_msgsSetDelayCompensationClient(Ros1Clearpath_navigation_msgsSetDelayCompensationClient const&)=delete;Ros1Clearpath_navigation_msgsSetDelayCompensationClient& operator=(Ros1Clearpath_navigation_msgsSetDelayCompensationClient const&)=delete;Ros1Clearpath_navigation_msgsSetDelayCompensationClient(Ros1Clearpath_navigation_msgsSetDelayCompensationClient&&)=delete;
~Ros1Clearpath_navigation_msgsSetDelayCompensationClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_navigation_msgs_SetDelayCompensation_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_navigation_msgs/SetDelayCompensation server; advertise/respond stay separate authorities. */
class Ros1Clearpath_navigation_msgsSetDelayCompensationServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_navigation_msgsSetDelayCompensationResponse(Ros1Clearpath_navigation_msgsSetDelayCompensationRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_navigation_msgsSetDelayCompensationRequest;using Response=Ros1Clearpath_navigation_msgsSetDelayCompensationResponse;
Ros1Clearpath_navigation_msgsSetDelayCompensationServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_navigation_msgs_SetDelayCompensation_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_navigation_msgs_SetDelayCompensation_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_navigation_msgs_SetDelayCompensation_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_navigation_msgs_SetDelayCompensation_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_navigation_msgs/SetPathShifter client; default policy returns13 from construction. */
class Ros1Clearpath_navigation_msgsSetPathShifterClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_navigation_msgsSetPathShifterRequest;using Response=Ros1Clearpath_navigation_msgsSetPathShifterResponse;
Ros1Clearpath_navigation_msgsSetPathShifterClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_navigation_msgsSetPathShifterClient(Ros1Clearpath_navigation_msgsSetPathShifterClient const&)=delete;Ros1Clearpath_navigation_msgsSetPathShifterClient& operator=(Ros1Clearpath_navigation_msgsSetPathShifterClient const&)=delete;Ros1Clearpath_navigation_msgsSetPathShifterClient(Ros1Clearpath_navigation_msgsSetPathShifterClient&&)=delete;
~Ros1Clearpath_navigation_msgsSetPathShifterClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_navigation_msgs_SetPathShifter_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_navigation_msgs/SetPathShifter server; advertise/respond stay separate authorities. */
class Ros1Clearpath_navigation_msgsSetPathShifterServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_navigation_msgsSetPathShifterResponse(Ros1Clearpath_navigation_msgsSetPathShifterRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_navigation_msgsSetPathShifterRequest;using Response=Ros1Clearpath_navigation_msgsSetPathShifterResponse;
Ros1Clearpath_navigation_msgsSetPathShifterServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_navigation_msgs_SetPathShifter_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_navigation_msgs_SetPathShifter_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_navigation_msgs_SetPathShifter_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_navigation_msgs_SetPathShifter_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_navigation_msgs/SetPathSmoother client; default policy returns13 from construction. */
class Ros1Clearpath_navigation_msgsSetPathSmootherClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_navigation_msgsSetPathSmootherRequest;using Response=Ros1Clearpath_navigation_msgsSetPathSmootherResponse;
Ros1Clearpath_navigation_msgsSetPathSmootherClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_navigation_msgsSetPathSmootherClient(Ros1Clearpath_navigation_msgsSetPathSmootherClient const&)=delete;Ros1Clearpath_navigation_msgsSetPathSmootherClient& operator=(Ros1Clearpath_navigation_msgsSetPathSmootherClient const&)=delete;Ros1Clearpath_navigation_msgsSetPathSmootherClient(Ros1Clearpath_navigation_msgsSetPathSmootherClient&&)=delete;
~Ros1Clearpath_navigation_msgsSetPathSmootherClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_navigation_msgs_SetPathSmoother_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_navigation_msgs/SetPathSmoother server; advertise/respond stay separate authorities. */
class Ros1Clearpath_navigation_msgsSetPathSmootherServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_navigation_msgsSetPathSmootherResponse(Ros1Clearpath_navigation_msgsSetPathSmootherRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_navigation_msgsSetPathSmootherRequest;using Response=Ros1Clearpath_navigation_msgsSetPathSmootherResponse;
Ros1Clearpath_navigation_msgsSetPathSmootherServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_navigation_msgs_SetPathSmoother_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_navigation_msgs_SetPathSmoother_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_navigation_msgs_SetPathSmoother_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_navigation_msgs_SetPathSmoother_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_navigation_msgs/SetStopDistance client; default policy returns13 from construction. */
class Ros1Clearpath_navigation_msgsSetStopDistanceClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_navigation_msgsSetStopDistanceRequest;using Response=Ros1Clearpath_navigation_msgsSetStopDistanceResponse;
Ros1Clearpath_navigation_msgsSetStopDistanceClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_navigation_msgsSetStopDistanceClient(Ros1Clearpath_navigation_msgsSetStopDistanceClient const&)=delete;Ros1Clearpath_navigation_msgsSetStopDistanceClient& operator=(Ros1Clearpath_navigation_msgsSetStopDistanceClient const&)=delete;Ros1Clearpath_navigation_msgsSetStopDistanceClient(Ros1Clearpath_navigation_msgsSetStopDistanceClient&&)=delete;
~Ros1Clearpath_navigation_msgsSetStopDistanceClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_navigation_msgs_SetStopDistance_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_navigation_msgs/SetStopDistance server; advertise/respond stay separate authorities. */
class Ros1Clearpath_navigation_msgsSetStopDistanceServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_navigation_msgsSetStopDistanceResponse(Ros1Clearpath_navigation_msgsSetStopDistanceRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_navigation_msgsSetStopDistanceRequest;using Response=Ros1Clearpath_navigation_msgsSetStopDistanceResponse;
Ros1Clearpath_navigation_msgsSetStopDistanceServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_navigation_msgs_SetStopDistance_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_navigation_msgs_SetStopDistance_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_navigation_msgs_SetStopDistance_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_navigation_msgs_SetStopDistance_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_safety_msgs/AddCommunicationWatchdog client; default policy returns13 from construction. */
class Ros1Clearpath_safety_msgsAddCommunicationWatchdogClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_safety_msgsAddCommunicationWatchdogRequest;using Response=Ros1Clearpath_safety_msgsAddCommunicationWatchdogResponse;
Ros1Clearpath_safety_msgsAddCommunicationWatchdogClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_safety_msgsAddCommunicationWatchdogClient(Ros1Clearpath_safety_msgsAddCommunicationWatchdogClient const&)=delete;Ros1Clearpath_safety_msgsAddCommunicationWatchdogClient& operator=(Ros1Clearpath_safety_msgsAddCommunicationWatchdogClient const&)=delete;Ros1Clearpath_safety_msgsAddCommunicationWatchdogClient(Ros1Clearpath_safety_msgsAddCommunicationWatchdogClient&&)=delete;
~Ros1Clearpath_safety_msgsAddCommunicationWatchdogClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_safety_msgs_AddCommunicationWatchdog_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_safety_msgs/AddCommunicationWatchdog server; advertise/respond stay separate authorities. */
class Ros1Clearpath_safety_msgsAddCommunicationWatchdogServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_safety_msgsAddCommunicationWatchdogResponse(Ros1Clearpath_safety_msgsAddCommunicationWatchdogRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_safety_msgsAddCommunicationWatchdogRequest;using Response=Ros1Clearpath_safety_msgsAddCommunicationWatchdogResponse;
Ros1Clearpath_safety_msgsAddCommunicationWatchdogServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_safety_msgs_AddCommunicationWatchdog_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_safety_msgs_AddCommunicationWatchdog_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_safety_msgs_AddCommunicationWatchdog_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_safety_msgs_AddCommunicationWatchdog_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_safety_msgs/AddHeartbeatWatchdog client; default policy returns13 from construction. */
class Ros1Clearpath_safety_msgsAddHeartbeatWatchdogClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_safety_msgsAddHeartbeatWatchdogRequest;using Response=Ros1Clearpath_safety_msgsAddHeartbeatWatchdogResponse;
Ros1Clearpath_safety_msgsAddHeartbeatWatchdogClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_safety_msgsAddHeartbeatWatchdogClient(Ros1Clearpath_safety_msgsAddHeartbeatWatchdogClient const&)=delete;Ros1Clearpath_safety_msgsAddHeartbeatWatchdogClient& operator=(Ros1Clearpath_safety_msgsAddHeartbeatWatchdogClient const&)=delete;Ros1Clearpath_safety_msgsAddHeartbeatWatchdogClient(Ros1Clearpath_safety_msgsAddHeartbeatWatchdogClient&&)=delete;
~Ros1Clearpath_safety_msgsAddHeartbeatWatchdogClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_safety_msgs_AddHeartbeatWatchdog_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_safety_msgs/AddHeartbeatWatchdog server; advertise/respond stay separate authorities. */
class Ros1Clearpath_safety_msgsAddHeartbeatWatchdogServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_safety_msgsAddHeartbeatWatchdogResponse(Ros1Clearpath_safety_msgsAddHeartbeatWatchdogRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_safety_msgsAddHeartbeatWatchdogRequest;using Response=Ros1Clearpath_safety_msgsAddHeartbeatWatchdogResponse;
Ros1Clearpath_safety_msgsAddHeartbeatWatchdogServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_safety_msgs_AddHeartbeatWatchdog_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_safety_msgs_AddHeartbeatWatchdog_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_safety_msgs_AddHeartbeatWatchdog_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_safety_msgs_AddHeartbeatWatchdog_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_safety_msgs/AddInclinationWatchdog client; default policy returns13 from construction. */
class Ros1Clearpath_safety_msgsAddInclinationWatchdogClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_safety_msgsAddInclinationWatchdogRequest;using Response=Ros1Clearpath_safety_msgsAddInclinationWatchdogResponse;
Ros1Clearpath_safety_msgsAddInclinationWatchdogClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_safety_msgsAddInclinationWatchdogClient(Ros1Clearpath_safety_msgsAddInclinationWatchdogClient const&)=delete;Ros1Clearpath_safety_msgsAddInclinationWatchdogClient& operator=(Ros1Clearpath_safety_msgsAddInclinationWatchdogClient const&)=delete;Ros1Clearpath_safety_msgsAddInclinationWatchdogClient(Ros1Clearpath_safety_msgsAddInclinationWatchdogClient&&)=delete;
~Ros1Clearpath_safety_msgsAddInclinationWatchdogClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_safety_msgs_AddInclinationWatchdog_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_safety_msgs/AddInclinationWatchdog server; advertise/respond stay separate authorities. */
class Ros1Clearpath_safety_msgsAddInclinationWatchdogServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_safety_msgsAddInclinationWatchdogResponse(Ros1Clearpath_safety_msgsAddInclinationWatchdogRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_safety_msgsAddInclinationWatchdogRequest;using Response=Ros1Clearpath_safety_msgsAddInclinationWatchdogResponse;
Ros1Clearpath_safety_msgsAddInclinationWatchdogServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_safety_msgs_AddInclinationWatchdog_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_safety_msgs_AddInclinationWatchdog_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_safety_msgs_AddInclinationWatchdog_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_safety_msgs_AddInclinationWatchdog_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_safety_msgs/AddNodeStatusWatchdog client; default policy returns13 from construction. */
class Ros1Clearpath_safety_msgsAddNodeStatusWatchdogClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_safety_msgsAddNodeStatusWatchdogRequest;using Response=Ros1Clearpath_safety_msgsAddNodeStatusWatchdogResponse;
Ros1Clearpath_safety_msgsAddNodeStatusWatchdogClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_safety_msgsAddNodeStatusWatchdogClient(Ros1Clearpath_safety_msgsAddNodeStatusWatchdogClient const&)=delete;Ros1Clearpath_safety_msgsAddNodeStatusWatchdogClient& operator=(Ros1Clearpath_safety_msgsAddNodeStatusWatchdogClient const&)=delete;Ros1Clearpath_safety_msgsAddNodeStatusWatchdogClient(Ros1Clearpath_safety_msgsAddNodeStatusWatchdogClient&&)=delete;
~Ros1Clearpath_safety_msgsAddNodeStatusWatchdogClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_safety_msgs_AddNodeStatusWatchdog_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_safety_msgs/AddNodeStatusWatchdog server; advertise/respond stay separate authorities. */
class Ros1Clearpath_safety_msgsAddNodeStatusWatchdogServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_safety_msgsAddNodeStatusWatchdogResponse(Ros1Clearpath_safety_msgsAddNodeStatusWatchdogRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_safety_msgsAddNodeStatusWatchdogRequest;using Response=Ros1Clearpath_safety_msgsAddNodeStatusWatchdogResponse;
Ros1Clearpath_safety_msgsAddNodeStatusWatchdogServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_safety_msgs_AddNodeStatusWatchdog_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_safety_msgs_AddNodeStatusWatchdog_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_safety_msgs_AddNodeStatusWatchdog_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_safety_msgs_AddNodeStatusWatchdog_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_safety_msgs/AddNodeletStatusWatchdog client; default policy returns13 from construction. */
class Ros1Clearpath_safety_msgsAddNodeletStatusWatchdogClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_safety_msgsAddNodeletStatusWatchdogRequest;using Response=Ros1Clearpath_safety_msgsAddNodeletStatusWatchdogResponse;
Ros1Clearpath_safety_msgsAddNodeletStatusWatchdogClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_safety_msgsAddNodeletStatusWatchdogClient(Ros1Clearpath_safety_msgsAddNodeletStatusWatchdogClient const&)=delete;Ros1Clearpath_safety_msgsAddNodeletStatusWatchdogClient& operator=(Ros1Clearpath_safety_msgsAddNodeletStatusWatchdogClient const&)=delete;Ros1Clearpath_safety_msgsAddNodeletStatusWatchdogClient(Ros1Clearpath_safety_msgsAddNodeletStatusWatchdogClient&&)=delete;
~Ros1Clearpath_safety_msgsAddNodeletStatusWatchdogClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_safety_msgs_AddNodeletStatusWatchdog_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_safety_msgs/AddNodeletStatusWatchdog server; advertise/respond stay separate authorities. */
class Ros1Clearpath_safety_msgsAddNodeletStatusWatchdogServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_safety_msgsAddNodeletStatusWatchdogResponse(Ros1Clearpath_safety_msgsAddNodeletStatusWatchdogRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_safety_msgsAddNodeletStatusWatchdogRequest;using Response=Ros1Clearpath_safety_msgsAddNodeletStatusWatchdogResponse;
Ros1Clearpath_safety_msgsAddNodeletStatusWatchdogServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_safety_msgs_AddNodeletStatusWatchdog_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_safety_msgs_AddNodeletStatusWatchdog_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_safety_msgs_AddNodeletStatusWatchdog_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_safety_msgs_AddNodeletStatusWatchdog_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_safety_msgs/AddNumPointsWatchdog client; default policy returns13 from construction. */
class Ros1Clearpath_safety_msgsAddNumPointsWatchdogClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_safety_msgsAddNumPointsWatchdogRequest;using Response=Ros1Clearpath_safety_msgsAddNumPointsWatchdogResponse;
Ros1Clearpath_safety_msgsAddNumPointsWatchdogClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_safety_msgsAddNumPointsWatchdogClient(Ros1Clearpath_safety_msgsAddNumPointsWatchdogClient const&)=delete;Ros1Clearpath_safety_msgsAddNumPointsWatchdogClient& operator=(Ros1Clearpath_safety_msgsAddNumPointsWatchdogClient const&)=delete;Ros1Clearpath_safety_msgsAddNumPointsWatchdogClient(Ros1Clearpath_safety_msgsAddNumPointsWatchdogClient&&)=delete;
~Ros1Clearpath_safety_msgsAddNumPointsWatchdogClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_safety_msgs_AddNumPointsWatchdog_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_safety_msgs/AddNumPointsWatchdog server; advertise/respond stay separate authorities. */
class Ros1Clearpath_safety_msgsAddNumPointsWatchdogServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_safety_msgsAddNumPointsWatchdogResponse(Ros1Clearpath_safety_msgsAddNumPointsWatchdogRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_safety_msgsAddNumPointsWatchdogRequest;using Response=Ros1Clearpath_safety_msgsAddNumPointsWatchdogResponse;
Ros1Clearpath_safety_msgsAddNumPointsWatchdogServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_safety_msgs_AddNumPointsWatchdog_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_safety_msgs_AddNumPointsWatchdog_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_safety_msgs_AddNumPointsWatchdog_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_safety_msgs_AddNumPointsWatchdog_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_safety_msgs/AddOdomCovarianceWatchdog client; default policy returns13 from construction. */
class Ros1Clearpath_safety_msgsAddOdomCovarianceWatchdogClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_safety_msgsAddOdomCovarianceWatchdogRequest;using Response=Ros1Clearpath_safety_msgsAddOdomCovarianceWatchdogResponse;
Ros1Clearpath_safety_msgsAddOdomCovarianceWatchdogClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_safety_msgsAddOdomCovarianceWatchdogClient(Ros1Clearpath_safety_msgsAddOdomCovarianceWatchdogClient const&)=delete;Ros1Clearpath_safety_msgsAddOdomCovarianceWatchdogClient& operator=(Ros1Clearpath_safety_msgsAddOdomCovarianceWatchdogClient const&)=delete;Ros1Clearpath_safety_msgsAddOdomCovarianceWatchdogClient(Ros1Clearpath_safety_msgsAddOdomCovarianceWatchdogClient&&)=delete;
~Ros1Clearpath_safety_msgsAddOdomCovarianceWatchdogClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_safety_msgs_AddOdomCovarianceWatchdog_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_safety_msgs/AddOdomCovarianceWatchdog server; advertise/respond stay separate authorities. */
class Ros1Clearpath_safety_msgsAddOdomCovarianceWatchdogServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_safety_msgsAddOdomCovarianceWatchdogResponse(Ros1Clearpath_safety_msgsAddOdomCovarianceWatchdogRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_safety_msgsAddOdomCovarianceWatchdogRequest;using Response=Ros1Clearpath_safety_msgsAddOdomCovarianceWatchdogResponse;
Ros1Clearpath_safety_msgsAddOdomCovarianceWatchdogServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_safety_msgs_AddOdomCovarianceWatchdog_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_safety_msgs_AddOdomCovarianceWatchdog_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_safety_msgs_AddOdomCovarianceWatchdog_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_safety_msgs_AddOdomCovarianceWatchdog_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_safety_msgs/AddRtkFixWatchdog client; default policy returns13 from construction. */
class Ros1Clearpath_safety_msgsAddRtkFixWatchdogClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_safety_msgsAddRtkFixWatchdogRequest;using Response=Ros1Clearpath_safety_msgsAddRtkFixWatchdogResponse;
Ros1Clearpath_safety_msgsAddRtkFixWatchdogClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_safety_msgsAddRtkFixWatchdogClient(Ros1Clearpath_safety_msgsAddRtkFixWatchdogClient const&)=delete;Ros1Clearpath_safety_msgsAddRtkFixWatchdogClient& operator=(Ros1Clearpath_safety_msgsAddRtkFixWatchdogClient const&)=delete;Ros1Clearpath_safety_msgsAddRtkFixWatchdogClient(Ros1Clearpath_safety_msgsAddRtkFixWatchdogClient&&)=delete;
~Ros1Clearpath_safety_msgsAddRtkFixWatchdogClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_safety_msgs_AddRtkFixWatchdog_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_safety_msgs/AddRtkFixWatchdog server; advertise/respond stay separate authorities. */
class Ros1Clearpath_safety_msgsAddRtkFixWatchdogServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_safety_msgsAddRtkFixWatchdogResponse(Ros1Clearpath_safety_msgsAddRtkFixWatchdogRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_safety_msgsAddRtkFixWatchdogRequest;using Response=Ros1Clearpath_safety_msgsAddRtkFixWatchdogResponse;
Ros1Clearpath_safety_msgsAddRtkFixWatchdogServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_safety_msgs_AddRtkFixWatchdog_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_safety_msgs_AddRtkFixWatchdog_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_safety_msgs_AddRtkFixWatchdog_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_safety_msgs_AddRtkFixWatchdog_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_safety_msgs/AddTopicDataWatchdog client; default policy returns13 from construction. */
class Ros1Clearpath_safety_msgsAddTopicDataWatchdogClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_safety_msgsAddTopicDataWatchdogRequest;using Response=Ros1Clearpath_safety_msgsAddTopicDataWatchdogResponse;
Ros1Clearpath_safety_msgsAddTopicDataWatchdogClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_safety_msgsAddTopicDataWatchdogClient(Ros1Clearpath_safety_msgsAddTopicDataWatchdogClient const&)=delete;Ros1Clearpath_safety_msgsAddTopicDataWatchdogClient& operator=(Ros1Clearpath_safety_msgsAddTopicDataWatchdogClient const&)=delete;Ros1Clearpath_safety_msgsAddTopicDataWatchdogClient(Ros1Clearpath_safety_msgsAddTopicDataWatchdogClient&&)=delete;
~Ros1Clearpath_safety_msgsAddTopicDataWatchdogClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_safety_msgs_AddTopicDataWatchdog_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_safety_msgs/AddTopicDataWatchdog server; advertise/respond stay separate authorities. */
class Ros1Clearpath_safety_msgsAddTopicDataWatchdogServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_safety_msgsAddTopicDataWatchdogResponse(Ros1Clearpath_safety_msgsAddTopicDataWatchdogRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_safety_msgsAddTopicDataWatchdogRequest;using Response=Ros1Clearpath_safety_msgsAddTopicDataWatchdogResponse;
Ros1Clearpath_safety_msgsAddTopicDataWatchdogServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_safety_msgs_AddTopicDataWatchdog_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_safety_msgs_AddTopicDataWatchdog_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_safety_msgs_AddTopicDataWatchdog_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_safety_msgs_AddTopicDataWatchdog_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_safety_msgs/RemoveWatchdog client; default policy returns13 from construction. */
class Ros1Clearpath_safety_msgsRemoveWatchdogClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_safety_msgsRemoveWatchdogRequest;using Response=Ros1Clearpath_safety_msgsRemoveWatchdogResponse;
Ros1Clearpath_safety_msgsRemoveWatchdogClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_safety_msgsRemoveWatchdogClient(Ros1Clearpath_safety_msgsRemoveWatchdogClient const&)=delete;Ros1Clearpath_safety_msgsRemoveWatchdogClient& operator=(Ros1Clearpath_safety_msgsRemoveWatchdogClient const&)=delete;Ros1Clearpath_safety_msgsRemoveWatchdogClient(Ros1Clearpath_safety_msgsRemoveWatchdogClient&&)=delete;
~Ros1Clearpath_safety_msgsRemoveWatchdogClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_safety_msgs_RemoveWatchdog_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_safety_msgs/RemoveWatchdog server; advertise/respond stay separate authorities. */
class Ros1Clearpath_safety_msgsRemoveWatchdogServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_safety_msgsRemoveWatchdogResponse(Ros1Clearpath_safety_msgsRemoveWatchdogRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_safety_msgsRemoveWatchdogRequest;using Response=Ros1Clearpath_safety_msgsRemoveWatchdogResponse;
Ros1Clearpath_safety_msgsRemoveWatchdogServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_safety_msgs_RemoveWatchdog_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_safety_msgs_RemoveWatchdog_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_safety_msgs_RemoveWatchdog_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_safety_msgs_RemoveWatchdog_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_safety_msgs/UpdateCommunicationWatchdog client; default policy returns13 from construction. */
class Ros1Clearpath_safety_msgsUpdateCommunicationWatchdogClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_safety_msgsUpdateCommunicationWatchdogRequest;using Response=Ros1Clearpath_safety_msgsUpdateCommunicationWatchdogResponse;
Ros1Clearpath_safety_msgsUpdateCommunicationWatchdogClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_safety_msgsUpdateCommunicationWatchdogClient(Ros1Clearpath_safety_msgsUpdateCommunicationWatchdogClient const&)=delete;Ros1Clearpath_safety_msgsUpdateCommunicationWatchdogClient& operator=(Ros1Clearpath_safety_msgsUpdateCommunicationWatchdogClient const&)=delete;Ros1Clearpath_safety_msgsUpdateCommunicationWatchdogClient(Ros1Clearpath_safety_msgsUpdateCommunicationWatchdogClient&&)=delete;
~Ros1Clearpath_safety_msgsUpdateCommunicationWatchdogClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_safety_msgs_UpdateCommunicationWatchdog_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_safety_msgs/UpdateCommunicationWatchdog server; advertise/respond stay separate authorities. */
class Ros1Clearpath_safety_msgsUpdateCommunicationWatchdogServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_safety_msgsUpdateCommunicationWatchdogResponse(Ros1Clearpath_safety_msgsUpdateCommunicationWatchdogRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_safety_msgsUpdateCommunicationWatchdogRequest;using Response=Ros1Clearpath_safety_msgsUpdateCommunicationWatchdogResponse;
Ros1Clearpath_safety_msgsUpdateCommunicationWatchdogServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_safety_msgs_UpdateCommunicationWatchdog_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_safety_msgs_UpdateCommunicationWatchdog_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_safety_msgs_UpdateCommunicationWatchdog_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_safety_msgs_UpdateCommunicationWatchdog_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_safety_msgs/UpdateHeartbeatWatchdog client; default policy returns13 from construction. */
class Ros1Clearpath_safety_msgsUpdateHeartbeatWatchdogClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_safety_msgsUpdateHeartbeatWatchdogRequest;using Response=Ros1Clearpath_safety_msgsUpdateHeartbeatWatchdogResponse;
Ros1Clearpath_safety_msgsUpdateHeartbeatWatchdogClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_safety_msgsUpdateHeartbeatWatchdogClient(Ros1Clearpath_safety_msgsUpdateHeartbeatWatchdogClient const&)=delete;Ros1Clearpath_safety_msgsUpdateHeartbeatWatchdogClient& operator=(Ros1Clearpath_safety_msgsUpdateHeartbeatWatchdogClient const&)=delete;Ros1Clearpath_safety_msgsUpdateHeartbeatWatchdogClient(Ros1Clearpath_safety_msgsUpdateHeartbeatWatchdogClient&&)=delete;
~Ros1Clearpath_safety_msgsUpdateHeartbeatWatchdogClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_safety_msgs_UpdateHeartbeatWatchdog_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_safety_msgs/UpdateHeartbeatWatchdog server; advertise/respond stay separate authorities. */
class Ros1Clearpath_safety_msgsUpdateHeartbeatWatchdogServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_safety_msgsUpdateHeartbeatWatchdogResponse(Ros1Clearpath_safety_msgsUpdateHeartbeatWatchdogRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_safety_msgsUpdateHeartbeatWatchdogRequest;using Response=Ros1Clearpath_safety_msgsUpdateHeartbeatWatchdogResponse;
Ros1Clearpath_safety_msgsUpdateHeartbeatWatchdogServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_safety_msgs_UpdateHeartbeatWatchdog_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_safety_msgs_UpdateHeartbeatWatchdog_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_safety_msgs_UpdateHeartbeatWatchdog_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_safety_msgs_UpdateHeartbeatWatchdog_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_safety_msgs/UpdateInclinationWatchdog client; default policy returns13 from construction. */
class Ros1Clearpath_safety_msgsUpdateInclinationWatchdogClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_safety_msgsUpdateInclinationWatchdogRequest;using Response=Ros1Clearpath_safety_msgsUpdateInclinationWatchdogResponse;
Ros1Clearpath_safety_msgsUpdateInclinationWatchdogClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_safety_msgsUpdateInclinationWatchdogClient(Ros1Clearpath_safety_msgsUpdateInclinationWatchdogClient const&)=delete;Ros1Clearpath_safety_msgsUpdateInclinationWatchdogClient& operator=(Ros1Clearpath_safety_msgsUpdateInclinationWatchdogClient const&)=delete;Ros1Clearpath_safety_msgsUpdateInclinationWatchdogClient(Ros1Clearpath_safety_msgsUpdateInclinationWatchdogClient&&)=delete;
~Ros1Clearpath_safety_msgsUpdateInclinationWatchdogClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_safety_msgs_UpdateInclinationWatchdog_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_safety_msgs/UpdateInclinationWatchdog server; advertise/respond stay separate authorities. */
class Ros1Clearpath_safety_msgsUpdateInclinationWatchdogServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_safety_msgsUpdateInclinationWatchdogResponse(Ros1Clearpath_safety_msgsUpdateInclinationWatchdogRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_safety_msgsUpdateInclinationWatchdogRequest;using Response=Ros1Clearpath_safety_msgsUpdateInclinationWatchdogResponse;
Ros1Clearpath_safety_msgsUpdateInclinationWatchdogServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_safety_msgs_UpdateInclinationWatchdog_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_safety_msgs_UpdateInclinationWatchdog_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_safety_msgs_UpdateInclinationWatchdog_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_safety_msgs_UpdateInclinationWatchdog_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_safety_msgs/UpdateNodeStatusWatchdog client; default policy returns13 from construction. */
class Ros1Clearpath_safety_msgsUpdateNodeStatusWatchdogClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_safety_msgsUpdateNodeStatusWatchdogRequest;using Response=Ros1Clearpath_safety_msgsUpdateNodeStatusWatchdogResponse;
Ros1Clearpath_safety_msgsUpdateNodeStatusWatchdogClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_safety_msgsUpdateNodeStatusWatchdogClient(Ros1Clearpath_safety_msgsUpdateNodeStatusWatchdogClient const&)=delete;Ros1Clearpath_safety_msgsUpdateNodeStatusWatchdogClient& operator=(Ros1Clearpath_safety_msgsUpdateNodeStatusWatchdogClient const&)=delete;Ros1Clearpath_safety_msgsUpdateNodeStatusWatchdogClient(Ros1Clearpath_safety_msgsUpdateNodeStatusWatchdogClient&&)=delete;
~Ros1Clearpath_safety_msgsUpdateNodeStatusWatchdogClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_safety_msgs_UpdateNodeStatusWatchdog_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_safety_msgs/UpdateNodeStatusWatchdog server; advertise/respond stay separate authorities. */
class Ros1Clearpath_safety_msgsUpdateNodeStatusWatchdogServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_safety_msgsUpdateNodeStatusWatchdogResponse(Ros1Clearpath_safety_msgsUpdateNodeStatusWatchdogRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_safety_msgsUpdateNodeStatusWatchdogRequest;using Response=Ros1Clearpath_safety_msgsUpdateNodeStatusWatchdogResponse;
Ros1Clearpath_safety_msgsUpdateNodeStatusWatchdogServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_safety_msgs_UpdateNodeStatusWatchdog_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_safety_msgs_UpdateNodeStatusWatchdog_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_safety_msgs_UpdateNodeStatusWatchdog_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_safety_msgs_UpdateNodeStatusWatchdog_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_safety_msgs/UpdateNodeletStatusWatchdog client; default policy returns13 from construction. */
class Ros1Clearpath_safety_msgsUpdateNodeletStatusWatchdogClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_safety_msgsUpdateNodeletStatusWatchdogRequest;using Response=Ros1Clearpath_safety_msgsUpdateNodeletStatusWatchdogResponse;
Ros1Clearpath_safety_msgsUpdateNodeletStatusWatchdogClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_safety_msgsUpdateNodeletStatusWatchdogClient(Ros1Clearpath_safety_msgsUpdateNodeletStatusWatchdogClient const&)=delete;Ros1Clearpath_safety_msgsUpdateNodeletStatusWatchdogClient& operator=(Ros1Clearpath_safety_msgsUpdateNodeletStatusWatchdogClient const&)=delete;Ros1Clearpath_safety_msgsUpdateNodeletStatusWatchdogClient(Ros1Clearpath_safety_msgsUpdateNodeletStatusWatchdogClient&&)=delete;
~Ros1Clearpath_safety_msgsUpdateNodeletStatusWatchdogClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_safety_msgs_UpdateNodeletStatusWatchdog_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_safety_msgs/UpdateNodeletStatusWatchdog server; advertise/respond stay separate authorities. */
class Ros1Clearpath_safety_msgsUpdateNodeletStatusWatchdogServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_safety_msgsUpdateNodeletStatusWatchdogResponse(Ros1Clearpath_safety_msgsUpdateNodeletStatusWatchdogRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_safety_msgsUpdateNodeletStatusWatchdogRequest;using Response=Ros1Clearpath_safety_msgsUpdateNodeletStatusWatchdogResponse;
Ros1Clearpath_safety_msgsUpdateNodeletStatusWatchdogServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_safety_msgs_UpdateNodeletStatusWatchdog_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_safety_msgs_UpdateNodeletStatusWatchdog_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_safety_msgs_UpdateNodeletStatusWatchdog_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_safety_msgs_UpdateNodeletStatusWatchdog_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_safety_msgs/UpdateNumPointsWatchdog client; default policy returns13 from construction. */
class Ros1Clearpath_safety_msgsUpdateNumPointsWatchdogClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_safety_msgsUpdateNumPointsWatchdogRequest;using Response=Ros1Clearpath_safety_msgsUpdateNumPointsWatchdogResponse;
Ros1Clearpath_safety_msgsUpdateNumPointsWatchdogClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_safety_msgsUpdateNumPointsWatchdogClient(Ros1Clearpath_safety_msgsUpdateNumPointsWatchdogClient const&)=delete;Ros1Clearpath_safety_msgsUpdateNumPointsWatchdogClient& operator=(Ros1Clearpath_safety_msgsUpdateNumPointsWatchdogClient const&)=delete;Ros1Clearpath_safety_msgsUpdateNumPointsWatchdogClient(Ros1Clearpath_safety_msgsUpdateNumPointsWatchdogClient&&)=delete;
~Ros1Clearpath_safety_msgsUpdateNumPointsWatchdogClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_safety_msgs_UpdateNumPointsWatchdog_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_safety_msgs/UpdateNumPointsWatchdog server; advertise/respond stay separate authorities. */
class Ros1Clearpath_safety_msgsUpdateNumPointsWatchdogServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_safety_msgsUpdateNumPointsWatchdogResponse(Ros1Clearpath_safety_msgsUpdateNumPointsWatchdogRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_safety_msgsUpdateNumPointsWatchdogRequest;using Response=Ros1Clearpath_safety_msgsUpdateNumPointsWatchdogResponse;
Ros1Clearpath_safety_msgsUpdateNumPointsWatchdogServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_safety_msgs_UpdateNumPointsWatchdog_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_safety_msgs_UpdateNumPointsWatchdog_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_safety_msgs_UpdateNumPointsWatchdog_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_safety_msgs_UpdateNumPointsWatchdog_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_safety_msgs/UpdateOdomCovarianceWatchdog client; default policy returns13 from construction. */
class Ros1Clearpath_safety_msgsUpdateOdomCovarianceWatchdogClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_safety_msgsUpdateOdomCovarianceWatchdogRequest;using Response=Ros1Clearpath_safety_msgsUpdateOdomCovarianceWatchdogResponse;
Ros1Clearpath_safety_msgsUpdateOdomCovarianceWatchdogClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_safety_msgsUpdateOdomCovarianceWatchdogClient(Ros1Clearpath_safety_msgsUpdateOdomCovarianceWatchdogClient const&)=delete;Ros1Clearpath_safety_msgsUpdateOdomCovarianceWatchdogClient& operator=(Ros1Clearpath_safety_msgsUpdateOdomCovarianceWatchdogClient const&)=delete;Ros1Clearpath_safety_msgsUpdateOdomCovarianceWatchdogClient(Ros1Clearpath_safety_msgsUpdateOdomCovarianceWatchdogClient&&)=delete;
~Ros1Clearpath_safety_msgsUpdateOdomCovarianceWatchdogClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_safety_msgs_UpdateOdomCovarianceWatchdog_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_safety_msgs/UpdateOdomCovarianceWatchdog server; advertise/respond stay separate authorities. */
class Ros1Clearpath_safety_msgsUpdateOdomCovarianceWatchdogServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_safety_msgsUpdateOdomCovarianceWatchdogResponse(Ros1Clearpath_safety_msgsUpdateOdomCovarianceWatchdogRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_safety_msgsUpdateOdomCovarianceWatchdogRequest;using Response=Ros1Clearpath_safety_msgsUpdateOdomCovarianceWatchdogResponse;
Ros1Clearpath_safety_msgsUpdateOdomCovarianceWatchdogServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_safety_msgs_UpdateOdomCovarianceWatchdog_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_safety_msgs_UpdateOdomCovarianceWatchdog_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_safety_msgs_UpdateOdomCovarianceWatchdog_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_safety_msgs_UpdateOdomCovarianceWatchdog_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_safety_msgs/UpdateRtkFixWatchdog client; default policy returns13 from construction. */
class Ros1Clearpath_safety_msgsUpdateRtkFixWatchdogClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_safety_msgsUpdateRtkFixWatchdogRequest;using Response=Ros1Clearpath_safety_msgsUpdateRtkFixWatchdogResponse;
Ros1Clearpath_safety_msgsUpdateRtkFixWatchdogClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_safety_msgsUpdateRtkFixWatchdogClient(Ros1Clearpath_safety_msgsUpdateRtkFixWatchdogClient const&)=delete;Ros1Clearpath_safety_msgsUpdateRtkFixWatchdogClient& operator=(Ros1Clearpath_safety_msgsUpdateRtkFixWatchdogClient const&)=delete;Ros1Clearpath_safety_msgsUpdateRtkFixWatchdogClient(Ros1Clearpath_safety_msgsUpdateRtkFixWatchdogClient&&)=delete;
~Ros1Clearpath_safety_msgsUpdateRtkFixWatchdogClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_safety_msgs_UpdateRtkFixWatchdog_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_safety_msgs/UpdateRtkFixWatchdog server; advertise/respond stay separate authorities. */
class Ros1Clearpath_safety_msgsUpdateRtkFixWatchdogServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_safety_msgsUpdateRtkFixWatchdogResponse(Ros1Clearpath_safety_msgsUpdateRtkFixWatchdogRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_safety_msgsUpdateRtkFixWatchdogRequest;using Response=Ros1Clearpath_safety_msgsUpdateRtkFixWatchdogResponse;
Ros1Clearpath_safety_msgsUpdateRtkFixWatchdogServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_safety_msgs_UpdateRtkFixWatchdog_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_safety_msgs_UpdateRtkFixWatchdog_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_safety_msgs_UpdateRtkFixWatchdog_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_safety_msgs_UpdateRtkFixWatchdog_respond(&respond_,ticket,&response).error;}
};
/** Complete custom clearpath_safety_msgs/UpdateTopicDataWatchdog client; default policy returns13 from construction. */
class Ros1Clearpath_safety_msgsUpdateTopicDataWatchdogClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Clearpath_safety_msgsUpdateTopicDataWatchdogRequest;using Response=Ros1Clearpath_safety_msgsUpdateTopicDataWatchdogResponse;
Ros1Clearpath_safety_msgsUpdateTopicDataWatchdogClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Clearpath_safety_msgsUpdateTopicDataWatchdogClient(Ros1Clearpath_safety_msgsUpdateTopicDataWatchdogClient const&)=delete;Ros1Clearpath_safety_msgsUpdateTopicDataWatchdogClient& operator=(Ros1Clearpath_safety_msgsUpdateTopicDataWatchdogClient const&)=delete;Ros1Clearpath_safety_msgsUpdateTopicDataWatchdogClient(Ros1Clearpath_safety_msgsUpdateTopicDataWatchdogClient&&)=delete;
~Ros1Clearpath_safety_msgsUpdateTopicDataWatchdogClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_safety_msgs_UpdateTopicDataWatchdog_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom clearpath_safety_msgs/UpdateTopicDataWatchdog server; advertise/respond stay separate authorities. */
class Ros1Clearpath_safety_msgsUpdateTopicDataWatchdogServer:public ServerOwners {
using Handler=std::function<Ros1Clearpath_safety_msgsUpdateTopicDataWatchdogResponse(Ros1Clearpath_safety_msgsUpdateTopicDataWatchdogRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Clearpath_safety_msgsUpdateTopicDataWatchdogRequest;using Response=Ros1Clearpath_safety_msgsUpdateTopicDataWatchdogResponse;
Ros1Clearpath_safety_msgsUpdateTopicDataWatchdogServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_clearpath_safety_msgs_UpdateTopicDataWatchdog_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_clearpath_safety_msgs_UpdateTopicDataWatchdog_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_clearpath_safety_msgs_UpdateTopicDataWatchdog_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_clearpath_safety_msgs_UpdateTopicDataWatchdog_respond(&respond_,ticket,&response).error;}
};
}
