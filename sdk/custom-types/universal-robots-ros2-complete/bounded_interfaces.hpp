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
/** Complete owned custom ur_dashboard_msgs/action/SetMode Action client with four separate roles. */
class Ros2Ur_dashboard_msgsActionSetModeClient {roswasmc_context context_;std::array<uint64_t,1024> goals_{};roswasmc_action_bundle bundle_{};
public:
using Goal=Ros2Ur_dashboard_msgsActionSetMode_Goal;using Result=Ros2Ur_dashboard_msgsActionSetMode_Result;using Feedback=Ros2Ur_dashboard_msgsActionSetMode_Feedback;
Ros2Ur_dashboard_msgsActionSetModeClient(roswasmc_context const& context,roswasmc_action_aliases const& aliases,size_t slots=8):context_{context.backend} {roswasmc_action_bundle_init(&context_,&bundle_,&aliases,goals_.data(),slots);}
Ros2Ur_dashboard_msgsActionSetModeClient(Ros2Ur_dashboard_msgsActionSetModeClient const&)=delete;Ros2Ur_dashboard_msgsActionSetModeClient& operator=(Ros2Ur_dashboard_msgsActionSetModeClient const&)=delete;Ros2Ur_dashboard_msgsActionSetModeClient(Ros2Ur_dashboard_msgsActionSetModeClient&&)=delete;
~Ros2Ur_dashboard_msgsActionSetModeClient(){if(bundle_.active){std::array<uint32_t,5> statuses{};roswasmc_action_bundle_close_checked(&bundle_,0,statuses.data(),statuses.size());}}
uint32_t error() const noexcept{return bundle_.error?bundle_.error:bundle_.active?0:9;}bool is_open() const noexcept{return bundle_.active;}bool owns(uint64_t id)const{return roswasmc_action_bundle_owns(&bundle_,id);}
roswasmc_result send_goal(Goal const& value,uint64_t& id){return roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_send(&bundle_,&value,&id);}
roswasmc_result feedback(uint64_t id,Feedback& value){return roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_feedback(&bundle_,id,&value);}
roswasmc_result result(uint64_t id,uint8_t& kind,Result& value){return roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_result(&bundle_,id,&kind,&value);}
roswasmc_result cancel_own(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_own(&bundle_,id,&accepted);}
roswasmc_result cancel_any(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_any(&bundle_,id,&accepted);}
roswasmc_result cancel_all(int& accepted){return roswasmc_action_bundle_cancel_all(&bundle_,&accepted);}
roswasmc_cleanup_report close_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_close_checked(&bundle_,primary,statuses.data(),statuses.size());}
roswasmc_cleanup_report revoke_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_revoke_checked(&bundle_,primary,statuses.data(),statuses.size());}
};
/** Typed custom ur_dashboard_msgs/action/SetMode server with explicit feedback/terminal component codecs. */
class Ros2Ur_dashboard_msgsActionSetModeServer:public ServerOwners {
public:
using Goal=Ros2Ur_dashboard_msgsActionSetMode_Goal;using Result=Ros2Ur_dashboard_msgsActionSetMode_Result;using Feedback=Ros2Ur_dashboard_msgsActionSetMode_Feedback;
/** Handler may choose separate feedback/result effects; kind must be2..4 when terminal. */
struct Replies {Feedback feedback{};Result result{};uint8_t kind{2};bool send_feedback{true},send_result{true};};
private:using Handler=std::function<Replies(uint64_t,Goal const&)>;std::shared_ptr<Handler> handler_;
public:
Ros2Ur_dashboard_msgsActionSetModeServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,true} {}
void on_goal(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& id,Goal& goal){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_poll(&advertise_,&id,&goal);}
roswasmc_result respond_feedback(uint64_t id,Feedback const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_respond_feedback(&respond_,id,&value);}
roswasmc_result respond_result(uint64_t id,uint8_t kind,Result const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_respond_result(&respond_,id,kind,&value);}
/** One pass, at most one feedback plus one terminal response; never implicit replay. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t id{};Goal goal{};auto r=roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_poll(&advertise_,&id,&goal);if(r.error)return r.error;auto replies=(*handler)(id,goal);if(replies.send_result&&(replies.kind<2||replies.kind>4||!roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Result_valid(&replies.result)))return 22;if(replies.send_feedback&&!roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Feedback_valid(&replies.feedback))return 22;if(replies.send_feedback){r=roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_respond_feedback(&respond_,id,&replies.feedback);if(r.error)return r.error;}return replies.send_result?roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_respond_result(&respond_,id,replies.kind,&replies.result).error:0;}
};
/** Complete custom ur_dashboard_msgs/srv/AddToLog client; default policy returns13 from construction. */
class Ros2Ur_dashboard_msgsSrvAddToLogClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Ur_dashboard_msgsSrvAddToLog_Request;using Response=Ros2Ur_dashboard_msgsSrvAddToLog_Response;
Ros2Ur_dashboard_msgsSrvAddToLogClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Ur_dashboard_msgsSrvAddToLogClient(Ros2Ur_dashboard_msgsSrvAddToLogClient const&)=delete;Ros2Ur_dashboard_msgsSrvAddToLogClient& operator=(Ros2Ur_dashboard_msgsSrvAddToLogClient const&)=delete;Ros2Ur_dashboard_msgsSrvAddToLogClient(Ros2Ur_dashboard_msgsSrvAddToLogClient&&)=delete;
~Ros2Ur_dashboard_msgsSrvAddToLogClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ur_dashboard_msgs/srv/AddToLog server; advertise/respond stay separate authorities. */
class Ros2Ur_dashboard_msgsSrvAddToLogServer:public ServerOwners {
using Handler=std::function<Ros2Ur_dashboard_msgsSrvAddToLog_Response(Ros2Ur_dashboard_msgsSrvAddToLog_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Ur_dashboard_msgsSrvAddToLog_Request;using Response=Ros2Ur_dashboard_msgsSrvAddToLog_Response;
Ros2Ur_dashboard_msgsSrvAddToLogServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_respond(&respond_,ticket,&response).error;}
};
/** Complete custom ur_dashboard_msgs/srv/DownloadProgram client; default policy returns13 from construction. */
class Ros2Ur_dashboard_msgsSrvDownloadProgramClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Ur_dashboard_msgsSrvDownloadProgram_Request;using Response=Ros2Ur_dashboard_msgsSrvDownloadProgram_Response;
Ros2Ur_dashboard_msgsSrvDownloadProgramClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Ur_dashboard_msgsSrvDownloadProgramClient(Ros2Ur_dashboard_msgsSrvDownloadProgramClient const&)=delete;Ros2Ur_dashboard_msgsSrvDownloadProgramClient& operator=(Ros2Ur_dashboard_msgsSrvDownloadProgramClient const&)=delete;Ros2Ur_dashboard_msgsSrvDownloadProgramClient(Ros2Ur_dashboard_msgsSrvDownloadProgramClient&&)=delete;
~Ros2Ur_dashboard_msgsSrvDownloadProgramClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ur_dashboard_msgs/srv/DownloadProgram server; advertise/respond stay separate authorities. */
class Ros2Ur_dashboard_msgsSrvDownloadProgramServer:public ServerOwners {
using Handler=std::function<Ros2Ur_dashboard_msgsSrvDownloadProgram_Response(Ros2Ur_dashboard_msgsSrvDownloadProgram_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Ur_dashboard_msgsSrvDownloadProgram_Request;using Response=Ros2Ur_dashboard_msgsSrvDownloadProgram_Response;
Ros2Ur_dashboard_msgsSrvDownloadProgramServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_respond(&respond_,ticket,&response).error;}
};
/** Complete custom ur_dashboard_msgs/srv/DownloadSupportFile client; default policy returns13 from construction. */
class Ros2Ur_dashboard_msgsSrvDownloadSupportFileClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Request;using Response=Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Response;
Ros2Ur_dashboard_msgsSrvDownloadSupportFileClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Ur_dashboard_msgsSrvDownloadSupportFileClient(Ros2Ur_dashboard_msgsSrvDownloadSupportFileClient const&)=delete;Ros2Ur_dashboard_msgsSrvDownloadSupportFileClient& operator=(Ros2Ur_dashboard_msgsSrvDownloadSupportFileClient const&)=delete;Ros2Ur_dashboard_msgsSrvDownloadSupportFileClient(Ros2Ur_dashboard_msgsSrvDownloadSupportFileClient&&)=delete;
~Ros2Ur_dashboard_msgsSrvDownloadSupportFileClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ur_dashboard_msgs/srv/DownloadSupportFile server; advertise/respond stay separate authorities. */
class Ros2Ur_dashboard_msgsSrvDownloadSupportFileServer:public ServerOwners {
using Handler=std::function<Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Response(Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Request;using Response=Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Response;
Ros2Ur_dashboard_msgsSrvDownloadSupportFileServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_respond(&respond_,ticket,&response).error;}
};
/** Complete custom ur_dashboard_msgs/srv/GenerateFlightReport client; default policy returns13 from construction. */
class Ros2Ur_dashboard_msgsSrvGenerateFlightReportClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Request;using Response=Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Response;
Ros2Ur_dashboard_msgsSrvGenerateFlightReportClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Ur_dashboard_msgsSrvGenerateFlightReportClient(Ros2Ur_dashboard_msgsSrvGenerateFlightReportClient const&)=delete;Ros2Ur_dashboard_msgsSrvGenerateFlightReportClient& operator=(Ros2Ur_dashboard_msgsSrvGenerateFlightReportClient const&)=delete;Ros2Ur_dashboard_msgsSrvGenerateFlightReportClient(Ros2Ur_dashboard_msgsSrvGenerateFlightReportClient&&)=delete;
~Ros2Ur_dashboard_msgsSrvGenerateFlightReportClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ur_dashboard_msgs/srv/GenerateFlightReport server; advertise/respond stay separate authorities. */
class Ros2Ur_dashboard_msgsSrvGenerateFlightReportServer:public ServerOwners {
using Handler=std::function<Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Response(Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Request;using Response=Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Response;
Ros2Ur_dashboard_msgsSrvGenerateFlightReportServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_respond(&respond_,ticket,&response).error;}
};
/** Complete custom ur_dashboard_msgs/srv/GenerateSupportFile client; default policy returns13 from construction. */
class Ros2Ur_dashboard_msgsSrvGenerateSupportFileClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Request;using Response=Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Response;
Ros2Ur_dashboard_msgsSrvGenerateSupportFileClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Ur_dashboard_msgsSrvGenerateSupportFileClient(Ros2Ur_dashboard_msgsSrvGenerateSupportFileClient const&)=delete;Ros2Ur_dashboard_msgsSrvGenerateSupportFileClient& operator=(Ros2Ur_dashboard_msgsSrvGenerateSupportFileClient const&)=delete;Ros2Ur_dashboard_msgsSrvGenerateSupportFileClient(Ros2Ur_dashboard_msgsSrvGenerateSupportFileClient&&)=delete;
~Ros2Ur_dashboard_msgsSrvGenerateSupportFileClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ur_dashboard_msgs/srv/GenerateSupportFile server; advertise/respond stay separate authorities. */
class Ros2Ur_dashboard_msgsSrvGenerateSupportFileServer:public ServerOwners {
using Handler=std::function<Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Response(Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Request;using Response=Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Response;
Ros2Ur_dashboard_msgsSrvGenerateSupportFileServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_respond(&respond_,ticket,&response).error;}
};
/** Complete custom ur_dashboard_msgs/srv/GetLoadedProgram client; default policy returns13 from construction. */
class Ros2Ur_dashboard_msgsSrvGetLoadedProgramClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Request;using Response=Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Response;
Ros2Ur_dashboard_msgsSrvGetLoadedProgramClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Ur_dashboard_msgsSrvGetLoadedProgramClient(Ros2Ur_dashboard_msgsSrvGetLoadedProgramClient const&)=delete;Ros2Ur_dashboard_msgsSrvGetLoadedProgramClient& operator=(Ros2Ur_dashboard_msgsSrvGetLoadedProgramClient const&)=delete;Ros2Ur_dashboard_msgsSrvGetLoadedProgramClient(Ros2Ur_dashboard_msgsSrvGetLoadedProgramClient&&)=delete;
~Ros2Ur_dashboard_msgsSrvGetLoadedProgramClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ur_dashboard_msgs/srv/GetLoadedProgram server; advertise/respond stay separate authorities. */
class Ros2Ur_dashboard_msgsSrvGetLoadedProgramServer:public ServerOwners {
using Handler=std::function<Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Response(Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Request;using Response=Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Response;
Ros2Ur_dashboard_msgsSrvGetLoadedProgramServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_respond(&respond_,ticket,&response).error;}
};
/** Complete custom ur_dashboard_msgs/srv/GetOperationalMode client; default policy returns13 from construction. */
class Ros2Ur_dashboard_msgsSrvGetOperationalModeClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Ur_dashboard_msgsSrvGetOperationalMode_Request;using Response=Ros2Ur_dashboard_msgsSrvGetOperationalMode_Response;
Ros2Ur_dashboard_msgsSrvGetOperationalModeClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Ur_dashboard_msgsSrvGetOperationalModeClient(Ros2Ur_dashboard_msgsSrvGetOperationalModeClient const&)=delete;Ros2Ur_dashboard_msgsSrvGetOperationalModeClient& operator=(Ros2Ur_dashboard_msgsSrvGetOperationalModeClient const&)=delete;Ros2Ur_dashboard_msgsSrvGetOperationalModeClient(Ros2Ur_dashboard_msgsSrvGetOperationalModeClient&&)=delete;
~Ros2Ur_dashboard_msgsSrvGetOperationalModeClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ur_dashboard_msgs/srv/GetOperationalMode server; advertise/respond stay separate authorities. */
class Ros2Ur_dashboard_msgsSrvGetOperationalModeServer:public ServerOwners {
using Handler=std::function<Ros2Ur_dashboard_msgsSrvGetOperationalMode_Response(Ros2Ur_dashboard_msgsSrvGetOperationalMode_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Ur_dashboard_msgsSrvGetOperationalMode_Request;using Response=Ros2Ur_dashboard_msgsSrvGetOperationalMode_Response;
Ros2Ur_dashboard_msgsSrvGetOperationalModeServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_respond(&respond_,ticket,&response).error;}
};
/** Complete custom ur_dashboard_msgs/srv/GetPolyScopeVersion client; default policy returns13 from construction. */
class Ros2Ur_dashboard_msgsSrvGetPolyScopeVersionClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Request;using Response=Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Response;
Ros2Ur_dashboard_msgsSrvGetPolyScopeVersionClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Ur_dashboard_msgsSrvGetPolyScopeVersionClient(Ros2Ur_dashboard_msgsSrvGetPolyScopeVersionClient const&)=delete;Ros2Ur_dashboard_msgsSrvGetPolyScopeVersionClient& operator=(Ros2Ur_dashboard_msgsSrvGetPolyScopeVersionClient const&)=delete;Ros2Ur_dashboard_msgsSrvGetPolyScopeVersionClient(Ros2Ur_dashboard_msgsSrvGetPolyScopeVersionClient&&)=delete;
~Ros2Ur_dashboard_msgsSrvGetPolyScopeVersionClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ur_dashboard_msgs/srv/GetPolyScopeVersion server; advertise/respond stay separate authorities. */
class Ros2Ur_dashboard_msgsSrvGetPolyScopeVersionServer:public ServerOwners {
using Handler=std::function<Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Response(Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Request;using Response=Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Response;
Ros2Ur_dashboard_msgsSrvGetPolyScopeVersionServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_respond(&respond_,ticket,&response).error;}
};
/** Complete custom ur_dashboard_msgs/srv/GetProgramState client; default policy returns13 from construction. */
class Ros2Ur_dashboard_msgsSrvGetProgramStateClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Ur_dashboard_msgsSrvGetProgramState_Request;using Response=Ros2Ur_dashboard_msgsSrvGetProgramState_Response;
Ros2Ur_dashboard_msgsSrvGetProgramStateClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Ur_dashboard_msgsSrvGetProgramStateClient(Ros2Ur_dashboard_msgsSrvGetProgramStateClient const&)=delete;Ros2Ur_dashboard_msgsSrvGetProgramStateClient& operator=(Ros2Ur_dashboard_msgsSrvGetProgramStateClient const&)=delete;Ros2Ur_dashboard_msgsSrvGetProgramStateClient(Ros2Ur_dashboard_msgsSrvGetProgramStateClient&&)=delete;
~Ros2Ur_dashboard_msgsSrvGetProgramStateClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ur_dashboard_msgs/srv/GetProgramState server; advertise/respond stay separate authorities. */
class Ros2Ur_dashboard_msgsSrvGetProgramStateServer:public ServerOwners {
using Handler=std::function<Ros2Ur_dashboard_msgsSrvGetProgramState_Response(Ros2Ur_dashboard_msgsSrvGetProgramState_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Ur_dashboard_msgsSrvGetProgramState_Request;using Response=Ros2Ur_dashboard_msgsSrvGetProgramState_Response;
Ros2Ur_dashboard_msgsSrvGetProgramStateServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_respond(&respond_,ticket,&response).error;}
};
/** Complete custom ur_dashboard_msgs/srv/GetPrograms client; default policy returns13 from construction. */
class Ros2Ur_dashboard_msgsSrvGetProgramsClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Ur_dashboard_msgsSrvGetPrograms_Request;using Response=Ros2Ur_dashboard_msgsSrvGetPrograms_Response;
Ros2Ur_dashboard_msgsSrvGetProgramsClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Ur_dashboard_msgsSrvGetProgramsClient(Ros2Ur_dashboard_msgsSrvGetProgramsClient const&)=delete;Ros2Ur_dashboard_msgsSrvGetProgramsClient& operator=(Ros2Ur_dashboard_msgsSrvGetProgramsClient const&)=delete;Ros2Ur_dashboard_msgsSrvGetProgramsClient(Ros2Ur_dashboard_msgsSrvGetProgramsClient&&)=delete;
~Ros2Ur_dashboard_msgsSrvGetProgramsClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ur_dashboard_msgs/srv/GetPrograms server; advertise/respond stay separate authorities. */
class Ros2Ur_dashboard_msgsSrvGetProgramsServer:public ServerOwners {
using Handler=std::function<Ros2Ur_dashboard_msgsSrvGetPrograms_Response(Ros2Ur_dashboard_msgsSrvGetPrograms_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Ur_dashboard_msgsSrvGetPrograms_Request;using Response=Ros2Ur_dashboard_msgsSrvGetPrograms_Response;
Ros2Ur_dashboard_msgsSrvGetProgramsServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_respond(&respond_,ticket,&response).error;}
};
/** Complete custom ur_dashboard_msgs/srv/GetRobotMode client; default policy returns13 from construction. */
class Ros2Ur_dashboard_msgsSrvGetRobotModeClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Ur_dashboard_msgsSrvGetRobotMode_Request;using Response=Ros2Ur_dashboard_msgsSrvGetRobotMode_Response;
Ros2Ur_dashboard_msgsSrvGetRobotModeClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Ur_dashboard_msgsSrvGetRobotModeClient(Ros2Ur_dashboard_msgsSrvGetRobotModeClient const&)=delete;Ros2Ur_dashboard_msgsSrvGetRobotModeClient& operator=(Ros2Ur_dashboard_msgsSrvGetRobotModeClient const&)=delete;Ros2Ur_dashboard_msgsSrvGetRobotModeClient(Ros2Ur_dashboard_msgsSrvGetRobotModeClient&&)=delete;
~Ros2Ur_dashboard_msgsSrvGetRobotModeClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ur_dashboard_msgs/srv/GetRobotMode server; advertise/respond stay separate authorities. */
class Ros2Ur_dashboard_msgsSrvGetRobotModeServer:public ServerOwners {
using Handler=std::function<Ros2Ur_dashboard_msgsSrvGetRobotMode_Response(Ros2Ur_dashboard_msgsSrvGetRobotMode_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Ur_dashboard_msgsSrvGetRobotMode_Request;using Response=Ros2Ur_dashboard_msgsSrvGetRobotMode_Response;
Ros2Ur_dashboard_msgsSrvGetRobotModeServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_respond(&respond_,ticket,&response).error;}
};
/** Complete custom ur_dashboard_msgs/srv/GetRobotModel client; default policy returns13 from construction. */
class Ros2Ur_dashboard_msgsSrvGetRobotModelClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Ur_dashboard_msgsSrvGetRobotModel_Request;using Response=Ros2Ur_dashboard_msgsSrvGetRobotModel_Response;
Ros2Ur_dashboard_msgsSrvGetRobotModelClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Ur_dashboard_msgsSrvGetRobotModelClient(Ros2Ur_dashboard_msgsSrvGetRobotModelClient const&)=delete;Ros2Ur_dashboard_msgsSrvGetRobotModelClient& operator=(Ros2Ur_dashboard_msgsSrvGetRobotModelClient const&)=delete;Ros2Ur_dashboard_msgsSrvGetRobotModelClient(Ros2Ur_dashboard_msgsSrvGetRobotModelClient&&)=delete;
~Ros2Ur_dashboard_msgsSrvGetRobotModelClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ur_dashboard_msgs/srv/GetRobotModel server; advertise/respond stay separate authorities. */
class Ros2Ur_dashboard_msgsSrvGetRobotModelServer:public ServerOwners {
using Handler=std::function<Ros2Ur_dashboard_msgsSrvGetRobotModel_Response(Ros2Ur_dashboard_msgsSrvGetRobotModel_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Ur_dashboard_msgsSrvGetRobotModel_Request;using Response=Ros2Ur_dashboard_msgsSrvGetRobotModel_Response;
Ros2Ur_dashboard_msgsSrvGetRobotModelServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_respond(&respond_,ticket,&response).error;}
};
/** Complete custom ur_dashboard_msgs/srv/GetSafetyMode client; default policy returns13 from construction. */
class Ros2Ur_dashboard_msgsSrvGetSafetyModeClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Ur_dashboard_msgsSrvGetSafetyMode_Request;using Response=Ros2Ur_dashboard_msgsSrvGetSafetyMode_Response;
Ros2Ur_dashboard_msgsSrvGetSafetyModeClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Ur_dashboard_msgsSrvGetSafetyModeClient(Ros2Ur_dashboard_msgsSrvGetSafetyModeClient const&)=delete;Ros2Ur_dashboard_msgsSrvGetSafetyModeClient& operator=(Ros2Ur_dashboard_msgsSrvGetSafetyModeClient const&)=delete;Ros2Ur_dashboard_msgsSrvGetSafetyModeClient(Ros2Ur_dashboard_msgsSrvGetSafetyModeClient&&)=delete;
~Ros2Ur_dashboard_msgsSrvGetSafetyModeClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ur_dashboard_msgs/srv/GetSafetyMode server; advertise/respond stay separate authorities. */
class Ros2Ur_dashboard_msgsSrvGetSafetyModeServer:public ServerOwners {
using Handler=std::function<Ros2Ur_dashboard_msgsSrvGetSafetyMode_Response(Ros2Ur_dashboard_msgsSrvGetSafetyMode_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Ur_dashboard_msgsSrvGetSafetyMode_Request;using Response=Ros2Ur_dashboard_msgsSrvGetSafetyMode_Response;
Ros2Ur_dashboard_msgsSrvGetSafetyModeServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_respond(&respond_,ticket,&response).error;}
};
/** Complete custom ur_dashboard_msgs/srv/GetSafetyStatus client; default policy returns13 from construction. */
class Ros2Ur_dashboard_msgsSrvGetSafetyStatusClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Request;using Response=Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Response;
Ros2Ur_dashboard_msgsSrvGetSafetyStatusClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Ur_dashboard_msgsSrvGetSafetyStatusClient(Ros2Ur_dashboard_msgsSrvGetSafetyStatusClient const&)=delete;Ros2Ur_dashboard_msgsSrvGetSafetyStatusClient& operator=(Ros2Ur_dashboard_msgsSrvGetSafetyStatusClient const&)=delete;Ros2Ur_dashboard_msgsSrvGetSafetyStatusClient(Ros2Ur_dashboard_msgsSrvGetSafetyStatusClient&&)=delete;
~Ros2Ur_dashboard_msgsSrvGetSafetyStatusClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ur_dashboard_msgs/srv/GetSafetyStatus server; advertise/respond stay separate authorities. */
class Ros2Ur_dashboard_msgsSrvGetSafetyStatusServer:public ServerOwners {
using Handler=std::function<Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Response(Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Request;using Response=Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Response;
Ros2Ur_dashboard_msgsSrvGetSafetyStatusServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_respond(&respond_,ticket,&response).error;}
};
/** Complete custom ur_dashboard_msgs/srv/GetSerialNumber client; default policy returns13 from construction. */
class Ros2Ur_dashboard_msgsSrvGetSerialNumberClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Ur_dashboard_msgsSrvGetSerialNumber_Request;using Response=Ros2Ur_dashboard_msgsSrvGetSerialNumber_Response;
Ros2Ur_dashboard_msgsSrvGetSerialNumberClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Ur_dashboard_msgsSrvGetSerialNumberClient(Ros2Ur_dashboard_msgsSrvGetSerialNumberClient const&)=delete;Ros2Ur_dashboard_msgsSrvGetSerialNumberClient& operator=(Ros2Ur_dashboard_msgsSrvGetSerialNumberClient const&)=delete;Ros2Ur_dashboard_msgsSrvGetSerialNumberClient(Ros2Ur_dashboard_msgsSrvGetSerialNumberClient&&)=delete;
~Ros2Ur_dashboard_msgsSrvGetSerialNumberClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ur_dashboard_msgs/srv/GetSerialNumber server; advertise/respond stay separate authorities. */
class Ros2Ur_dashboard_msgsSrvGetSerialNumberServer:public ServerOwners {
using Handler=std::function<Ros2Ur_dashboard_msgsSrvGetSerialNumber_Response(Ros2Ur_dashboard_msgsSrvGetSerialNumber_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Ur_dashboard_msgsSrvGetSerialNumber_Request;using Response=Ros2Ur_dashboard_msgsSrvGetSerialNumber_Response;
Ros2Ur_dashboard_msgsSrvGetSerialNumberServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_respond(&respond_,ticket,&response).error;}
};
/** Complete custom ur_dashboard_msgs/srv/GetUserRole client; default policy returns13 from construction. */
class Ros2Ur_dashboard_msgsSrvGetUserRoleClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Ur_dashboard_msgsSrvGetUserRole_Request;using Response=Ros2Ur_dashboard_msgsSrvGetUserRole_Response;
Ros2Ur_dashboard_msgsSrvGetUserRoleClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Ur_dashboard_msgsSrvGetUserRoleClient(Ros2Ur_dashboard_msgsSrvGetUserRoleClient const&)=delete;Ros2Ur_dashboard_msgsSrvGetUserRoleClient& operator=(Ros2Ur_dashboard_msgsSrvGetUserRoleClient const&)=delete;Ros2Ur_dashboard_msgsSrvGetUserRoleClient(Ros2Ur_dashboard_msgsSrvGetUserRoleClient&&)=delete;
~Ros2Ur_dashboard_msgsSrvGetUserRoleClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ur_dashboard_msgs/srv/GetUserRole server; advertise/respond stay separate authorities. */
class Ros2Ur_dashboard_msgsSrvGetUserRoleServer:public ServerOwners {
using Handler=std::function<Ros2Ur_dashboard_msgsSrvGetUserRole_Response(Ros2Ur_dashboard_msgsSrvGetUserRole_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Ur_dashboard_msgsSrvGetUserRole_Request;using Response=Ros2Ur_dashboard_msgsSrvGetUserRole_Response;
Ros2Ur_dashboard_msgsSrvGetUserRoleServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_respond(&respond_,ticket,&response).error;}
};
/** Complete custom ur_dashboard_msgs/srv/IsInRemoteControl client; default policy returns13 from construction. */
class Ros2Ur_dashboard_msgsSrvIsInRemoteControlClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Request;using Response=Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Response;
Ros2Ur_dashboard_msgsSrvIsInRemoteControlClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Ur_dashboard_msgsSrvIsInRemoteControlClient(Ros2Ur_dashboard_msgsSrvIsInRemoteControlClient const&)=delete;Ros2Ur_dashboard_msgsSrvIsInRemoteControlClient& operator=(Ros2Ur_dashboard_msgsSrvIsInRemoteControlClient const&)=delete;Ros2Ur_dashboard_msgsSrvIsInRemoteControlClient(Ros2Ur_dashboard_msgsSrvIsInRemoteControlClient&&)=delete;
~Ros2Ur_dashboard_msgsSrvIsInRemoteControlClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ur_dashboard_msgs/srv/IsInRemoteControl server; advertise/respond stay separate authorities. */
class Ros2Ur_dashboard_msgsSrvIsInRemoteControlServer:public ServerOwners {
using Handler=std::function<Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Response(Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Request;using Response=Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Response;
Ros2Ur_dashboard_msgsSrvIsInRemoteControlServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_respond(&respond_,ticket,&response).error;}
};
/** Complete custom ur_dashboard_msgs/srv/IsProgramRunning client; default policy returns13 from construction. */
class Ros2Ur_dashboard_msgsSrvIsProgramRunningClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Ur_dashboard_msgsSrvIsProgramRunning_Request;using Response=Ros2Ur_dashboard_msgsSrvIsProgramRunning_Response;
Ros2Ur_dashboard_msgsSrvIsProgramRunningClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Ur_dashboard_msgsSrvIsProgramRunningClient(Ros2Ur_dashboard_msgsSrvIsProgramRunningClient const&)=delete;Ros2Ur_dashboard_msgsSrvIsProgramRunningClient& operator=(Ros2Ur_dashboard_msgsSrvIsProgramRunningClient const&)=delete;Ros2Ur_dashboard_msgsSrvIsProgramRunningClient(Ros2Ur_dashboard_msgsSrvIsProgramRunningClient&&)=delete;
~Ros2Ur_dashboard_msgsSrvIsProgramRunningClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ur_dashboard_msgs/srv/IsProgramRunning server; advertise/respond stay separate authorities. */
class Ros2Ur_dashboard_msgsSrvIsProgramRunningServer:public ServerOwners {
using Handler=std::function<Ros2Ur_dashboard_msgsSrvIsProgramRunning_Response(Ros2Ur_dashboard_msgsSrvIsProgramRunning_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Ur_dashboard_msgsSrvIsProgramRunning_Request;using Response=Ros2Ur_dashboard_msgsSrvIsProgramRunning_Response;
Ros2Ur_dashboard_msgsSrvIsProgramRunningServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_respond(&respond_,ticket,&response).error;}
};
/** Complete custom ur_dashboard_msgs/srv/IsProgramSaved client; default policy returns13 from construction. */
class Ros2Ur_dashboard_msgsSrvIsProgramSavedClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Ur_dashboard_msgsSrvIsProgramSaved_Request;using Response=Ros2Ur_dashboard_msgsSrvIsProgramSaved_Response;
Ros2Ur_dashboard_msgsSrvIsProgramSavedClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Ur_dashboard_msgsSrvIsProgramSavedClient(Ros2Ur_dashboard_msgsSrvIsProgramSavedClient const&)=delete;Ros2Ur_dashboard_msgsSrvIsProgramSavedClient& operator=(Ros2Ur_dashboard_msgsSrvIsProgramSavedClient const&)=delete;Ros2Ur_dashboard_msgsSrvIsProgramSavedClient(Ros2Ur_dashboard_msgsSrvIsProgramSavedClient&&)=delete;
~Ros2Ur_dashboard_msgsSrvIsProgramSavedClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ur_dashboard_msgs/srv/IsProgramSaved server; advertise/respond stay separate authorities. */
class Ros2Ur_dashboard_msgsSrvIsProgramSavedServer:public ServerOwners {
using Handler=std::function<Ros2Ur_dashboard_msgsSrvIsProgramSaved_Response(Ros2Ur_dashboard_msgsSrvIsProgramSaved_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Ur_dashboard_msgsSrvIsProgramSaved_Request;using Response=Ros2Ur_dashboard_msgsSrvIsProgramSaved_Response;
Ros2Ur_dashboard_msgsSrvIsProgramSavedServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_respond(&respond_,ticket,&response).error;}
};
/** Complete custom ur_dashboard_msgs/srv/Load client; default policy returns13 from construction. */
class Ros2Ur_dashboard_msgsSrvLoadClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Ur_dashboard_msgsSrvLoad_Request;using Response=Ros2Ur_dashboard_msgsSrvLoad_Response;
Ros2Ur_dashboard_msgsSrvLoadClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Ur_dashboard_msgsSrvLoadClient(Ros2Ur_dashboard_msgsSrvLoadClient const&)=delete;Ros2Ur_dashboard_msgsSrvLoadClient& operator=(Ros2Ur_dashboard_msgsSrvLoadClient const&)=delete;Ros2Ur_dashboard_msgsSrvLoadClient(Ros2Ur_dashboard_msgsSrvLoadClient&&)=delete;
~Ros2Ur_dashboard_msgsSrvLoadClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ur_dashboard_msgs/srv/Load server; advertise/respond stay separate authorities. */
class Ros2Ur_dashboard_msgsSrvLoadServer:public ServerOwners {
using Handler=std::function<Ros2Ur_dashboard_msgsSrvLoad_Response(Ros2Ur_dashboard_msgsSrvLoad_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Ur_dashboard_msgsSrvLoad_Request;using Response=Ros2Ur_dashboard_msgsSrvLoad_Response;
Ros2Ur_dashboard_msgsSrvLoadServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_respond(&respond_,ticket,&response).error;}
};
/** Complete custom ur_dashboard_msgs/srv/Popup client; default policy returns13 from construction. */
class Ros2Ur_dashboard_msgsSrvPopupClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Ur_dashboard_msgsSrvPopup_Request;using Response=Ros2Ur_dashboard_msgsSrvPopup_Response;
Ros2Ur_dashboard_msgsSrvPopupClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Ur_dashboard_msgsSrvPopupClient(Ros2Ur_dashboard_msgsSrvPopupClient const&)=delete;Ros2Ur_dashboard_msgsSrvPopupClient& operator=(Ros2Ur_dashboard_msgsSrvPopupClient const&)=delete;Ros2Ur_dashboard_msgsSrvPopupClient(Ros2Ur_dashboard_msgsSrvPopupClient&&)=delete;
~Ros2Ur_dashboard_msgsSrvPopupClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ur_dashboard_msgs/srv/Popup server; advertise/respond stay separate authorities. */
class Ros2Ur_dashboard_msgsSrvPopupServer:public ServerOwners {
using Handler=std::function<Ros2Ur_dashboard_msgsSrvPopup_Response(Ros2Ur_dashboard_msgsSrvPopup_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Ur_dashboard_msgsSrvPopup_Request;using Response=Ros2Ur_dashboard_msgsSrvPopup_Response;
Ros2Ur_dashboard_msgsSrvPopupServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_respond(&respond_,ticket,&response).error;}
};
/** Complete custom ur_dashboard_msgs/srv/RawRequest client; default policy returns13 from construction. */
class Ros2Ur_dashboard_msgsSrvRawRequestClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Ur_dashboard_msgsSrvRawRequest_Request;using Response=Ros2Ur_dashboard_msgsSrvRawRequest_Response;
Ros2Ur_dashboard_msgsSrvRawRequestClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Ur_dashboard_msgsSrvRawRequestClient(Ros2Ur_dashboard_msgsSrvRawRequestClient const&)=delete;Ros2Ur_dashboard_msgsSrvRawRequestClient& operator=(Ros2Ur_dashboard_msgsSrvRawRequestClient const&)=delete;Ros2Ur_dashboard_msgsSrvRawRequestClient(Ros2Ur_dashboard_msgsSrvRawRequestClient&&)=delete;
~Ros2Ur_dashboard_msgsSrvRawRequestClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ur_dashboard_msgs/srv/RawRequest server; advertise/respond stay separate authorities. */
class Ros2Ur_dashboard_msgsSrvRawRequestServer:public ServerOwners {
using Handler=std::function<Ros2Ur_dashboard_msgsSrvRawRequest_Response(Ros2Ur_dashboard_msgsSrvRawRequest_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Ur_dashboard_msgsSrvRawRequest_Request;using Response=Ros2Ur_dashboard_msgsSrvRawRequest_Response;
Ros2Ur_dashboard_msgsSrvRawRequestServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_respond(&respond_,ticket,&response).error;}
};
/** Complete custom ur_dashboard_msgs/srv/SetOperationalMode client; default policy returns13 from construction. */
class Ros2Ur_dashboard_msgsSrvSetOperationalModeClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Ur_dashboard_msgsSrvSetOperationalMode_Request;using Response=Ros2Ur_dashboard_msgsSrvSetOperationalMode_Response;
Ros2Ur_dashboard_msgsSrvSetOperationalModeClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Ur_dashboard_msgsSrvSetOperationalModeClient(Ros2Ur_dashboard_msgsSrvSetOperationalModeClient const&)=delete;Ros2Ur_dashboard_msgsSrvSetOperationalModeClient& operator=(Ros2Ur_dashboard_msgsSrvSetOperationalModeClient const&)=delete;Ros2Ur_dashboard_msgsSrvSetOperationalModeClient(Ros2Ur_dashboard_msgsSrvSetOperationalModeClient&&)=delete;
~Ros2Ur_dashboard_msgsSrvSetOperationalModeClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ur_dashboard_msgs/srv/SetOperationalMode server; advertise/respond stay separate authorities. */
class Ros2Ur_dashboard_msgsSrvSetOperationalModeServer:public ServerOwners {
using Handler=std::function<Ros2Ur_dashboard_msgsSrvSetOperationalMode_Response(Ros2Ur_dashboard_msgsSrvSetOperationalMode_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Ur_dashboard_msgsSrvSetOperationalMode_Request;using Response=Ros2Ur_dashboard_msgsSrvSetOperationalMode_Response;
Ros2Ur_dashboard_msgsSrvSetOperationalModeServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_respond(&respond_,ticket,&response).error;}
};
/** Complete custom ur_dashboard_msgs/srv/SetUserRole client; default policy returns13 from construction. */
class Ros2Ur_dashboard_msgsSrvSetUserRoleClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Ur_dashboard_msgsSrvSetUserRole_Request;using Response=Ros2Ur_dashboard_msgsSrvSetUserRole_Response;
Ros2Ur_dashboard_msgsSrvSetUserRoleClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Ur_dashboard_msgsSrvSetUserRoleClient(Ros2Ur_dashboard_msgsSrvSetUserRoleClient const&)=delete;Ros2Ur_dashboard_msgsSrvSetUserRoleClient& operator=(Ros2Ur_dashboard_msgsSrvSetUserRoleClient const&)=delete;Ros2Ur_dashboard_msgsSrvSetUserRoleClient(Ros2Ur_dashboard_msgsSrvSetUserRoleClient&&)=delete;
~Ros2Ur_dashboard_msgsSrvSetUserRoleClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ur_dashboard_msgs/srv/SetUserRole server; advertise/respond stay separate authorities. */
class Ros2Ur_dashboard_msgsSrvSetUserRoleServer:public ServerOwners {
using Handler=std::function<Ros2Ur_dashboard_msgsSrvSetUserRole_Response(Ros2Ur_dashboard_msgsSrvSetUserRole_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Ur_dashboard_msgsSrvSetUserRole_Request;using Response=Ros2Ur_dashboard_msgsSrvSetUserRole_Response;
Ros2Ur_dashboard_msgsSrvSetUserRoleServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_respond(&respond_,ticket,&response).error;}
};
/** Complete custom ur_dashboard_msgs/srv/UploadProgram client; default policy returns13 from construction. */
class Ros2Ur_dashboard_msgsSrvUploadProgramClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Ur_dashboard_msgsSrvUploadProgram_Request;using Response=Ros2Ur_dashboard_msgsSrvUploadProgram_Response;
Ros2Ur_dashboard_msgsSrvUploadProgramClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Ur_dashboard_msgsSrvUploadProgramClient(Ros2Ur_dashboard_msgsSrvUploadProgramClient const&)=delete;Ros2Ur_dashboard_msgsSrvUploadProgramClient& operator=(Ros2Ur_dashboard_msgsSrvUploadProgramClient const&)=delete;Ros2Ur_dashboard_msgsSrvUploadProgramClient(Ros2Ur_dashboard_msgsSrvUploadProgramClient&&)=delete;
~Ros2Ur_dashboard_msgsSrvUploadProgramClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ur_dashboard_msgs/srv/UploadProgram server; advertise/respond stay separate authorities. */
class Ros2Ur_dashboard_msgsSrvUploadProgramServer:public ServerOwners {
using Handler=std::function<Ros2Ur_dashboard_msgsSrvUploadProgram_Response(Ros2Ur_dashboard_msgsSrvUploadProgram_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Ur_dashboard_msgsSrvUploadProgram_Request;using Response=Ros2Ur_dashboard_msgsSrvUploadProgram_Response;
Ros2Ur_dashboard_msgsSrvUploadProgramServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_respond(&respond_,ticket,&response).error;}
};
/** Complete owned custom ur_msgs/action/FollowJointTrajectoryUntil Action client with four separate roles. */
class Ros2Ur_msgsActionFollowJointTrajectoryUntilClient {roswasmc_context context_;std::array<uint64_t,1024> goals_{};roswasmc_action_bundle bundle_{};
public:
using Goal=Ros2Ur_msgsActionFollowJointTrajectoryUntil_Goal;using Result=Ros2Ur_msgsActionFollowJointTrajectoryUntil_Result;using Feedback=Ros2Ur_msgsActionFollowJointTrajectoryUntil_Feedback;
Ros2Ur_msgsActionFollowJointTrajectoryUntilClient(roswasmc_context const& context,roswasmc_action_aliases const& aliases,size_t slots=8):context_{context.backend} {roswasmc_action_bundle_init(&context_,&bundle_,&aliases,goals_.data(),slots);}
Ros2Ur_msgsActionFollowJointTrajectoryUntilClient(Ros2Ur_msgsActionFollowJointTrajectoryUntilClient const&)=delete;Ros2Ur_msgsActionFollowJointTrajectoryUntilClient& operator=(Ros2Ur_msgsActionFollowJointTrajectoryUntilClient const&)=delete;Ros2Ur_msgsActionFollowJointTrajectoryUntilClient(Ros2Ur_msgsActionFollowJointTrajectoryUntilClient&&)=delete;
~Ros2Ur_msgsActionFollowJointTrajectoryUntilClient(){if(bundle_.active){std::array<uint32_t,5> statuses{};roswasmc_action_bundle_close_checked(&bundle_,0,statuses.data(),statuses.size());}}
uint32_t error() const noexcept{return bundle_.error?bundle_.error:bundle_.active?0:9;}bool is_open() const noexcept{return bundle_.active;}bool owns(uint64_t id)const{return roswasmc_action_bundle_owns(&bundle_,id);}
roswasmc_result send_goal(Goal const& value,uint64_t& id){return roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_send(&bundle_,&value,&id);}
roswasmc_result feedback(uint64_t id,Feedback& value){return roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_feedback(&bundle_,id,&value);}
roswasmc_result result(uint64_t id,uint8_t& kind,Result& value){return roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_result(&bundle_,id,&kind,&value);}
roswasmc_result cancel_own(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_own(&bundle_,id,&accepted);}
roswasmc_result cancel_any(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_any(&bundle_,id,&accepted);}
roswasmc_result cancel_all(int& accepted){return roswasmc_action_bundle_cancel_all(&bundle_,&accepted);}
roswasmc_cleanup_report close_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_close_checked(&bundle_,primary,statuses.data(),statuses.size());}
roswasmc_cleanup_report revoke_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_revoke_checked(&bundle_,primary,statuses.data(),statuses.size());}
};
/** Typed custom ur_msgs/action/FollowJointTrajectoryUntil server with explicit feedback/terminal component codecs. */
class Ros2Ur_msgsActionFollowJointTrajectoryUntilServer:public ServerOwners {
public:
using Goal=Ros2Ur_msgsActionFollowJointTrajectoryUntil_Goal;using Result=Ros2Ur_msgsActionFollowJointTrajectoryUntil_Result;using Feedback=Ros2Ur_msgsActionFollowJointTrajectoryUntil_Feedback;
/** Handler may choose separate feedback/result effects; kind must be2..4 when terminal. */
struct Replies {Feedback feedback{};Result result{};uint8_t kind{2};bool send_feedback{true},send_result{true};};
private:using Handler=std::function<Replies(uint64_t,Goal const&)>;std::shared_ptr<Handler> handler_;
public:
Ros2Ur_msgsActionFollowJointTrajectoryUntilServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,true} {}
void on_goal(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& id,Goal& goal){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_poll(&advertise_,&id,&goal);}
roswasmc_result respond_feedback(uint64_t id,Feedback const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_respond_feedback(&respond_,id,&value);}
roswasmc_result respond_result(uint64_t id,uint8_t kind,Result const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_respond_result(&respond_,id,kind,&value);}
/** One pass, at most one feedback plus one terminal response; never implicit replay. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t id{};Goal goal{};auto r=roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_poll(&advertise_,&id,&goal);if(r.error)return r.error;auto replies=(*handler)(id,goal);if(replies.send_result&&(replies.kind<2||replies.kind>4||!roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Result_valid(&replies.result)))return 22;if(replies.send_feedback&&!roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Feedback_valid(&replies.feedback))return 22;if(replies.send_feedback){r=roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_respond_feedback(&respond_,id,&replies.feedback);if(r.error)return r.error;}return replies.send_result?roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_respond_result(&respond_,id,replies.kind,&replies.result).error:0;}
};
/** Complete owned custom ur_msgs/action/SendScript Action client with four separate roles. */
class Ros2Ur_msgsActionSendScriptClient {roswasmc_context context_;std::array<uint64_t,1024> goals_{};roswasmc_action_bundle bundle_{};
public:
using Goal=Ros2Ur_msgsActionSendScript_Goal;using Result=Ros2Ur_msgsActionSendScript_Result;using Feedback=Ros2Ur_msgsActionSendScript_Feedback;
Ros2Ur_msgsActionSendScriptClient(roswasmc_context const& context,roswasmc_action_aliases const& aliases,size_t slots=8):context_{context.backend} {roswasmc_action_bundle_init(&context_,&bundle_,&aliases,goals_.data(),slots);}
Ros2Ur_msgsActionSendScriptClient(Ros2Ur_msgsActionSendScriptClient const&)=delete;Ros2Ur_msgsActionSendScriptClient& operator=(Ros2Ur_msgsActionSendScriptClient const&)=delete;Ros2Ur_msgsActionSendScriptClient(Ros2Ur_msgsActionSendScriptClient&&)=delete;
~Ros2Ur_msgsActionSendScriptClient(){if(bundle_.active){std::array<uint32_t,5> statuses{};roswasmc_action_bundle_close_checked(&bundle_,0,statuses.data(),statuses.size());}}
uint32_t error() const noexcept{return bundle_.error?bundle_.error:bundle_.active?0:9;}bool is_open() const noexcept{return bundle_.active;}bool owns(uint64_t id)const{return roswasmc_action_bundle_owns(&bundle_,id);}
roswasmc_result send_goal(Goal const& value,uint64_t& id){return roswasmc_bounded_2_ur_msgs_action_SendScript_send(&bundle_,&value,&id);}
roswasmc_result feedback(uint64_t id,Feedback& value){return roswasmc_bounded_2_ur_msgs_action_SendScript_feedback(&bundle_,id,&value);}
roswasmc_result result(uint64_t id,uint8_t& kind,Result& value){return roswasmc_bounded_2_ur_msgs_action_SendScript_result(&bundle_,id,&kind,&value);}
roswasmc_result cancel_own(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_own(&bundle_,id,&accepted);}
roswasmc_result cancel_any(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_any(&bundle_,id,&accepted);}
roswasmc_result cancel_all(int& accepted){return roswasmc_action_bundle_cancel_all(&bundle_,&accepted);}
roswasmc_cleanup_report close_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_close_checked(&bundle_,primary,statuses.data(),statuses.size());}
roswasmc_cleanup_report revoke_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_revoke_checked(&bundle_,primary,statuses.data(),statuses.size());}
};
/** Typed custom ur_msgs/action/SendScript server with explicit feedback/terminal component codecs. */
class Ros2Ur_msgsActionSendScriptServer:public ServerOwners {
public:
using Goal=Ros2Ur_msgsActionSendScript_Goal;using Result=Ros2Ur_msgsActionSendScript_Result;using Feedback=Ros2Ur_msgsActionSendScript_Feedback;
/** Handler may choose separate feedback/result effects; kind must be2..4 when terminal. */
struct Replies {Feedback feedback{};Result result{};uint8_t kind{2};bool send_feedback{true},send_result{true};};
private:using Handler=std::function<Replies(uint64_t,Goal const&)>;std::shared_ptr<Handler> handler_;
public:
Ros2Ur_msgsActionSendScriptServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,true} {}
void on_goal(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& id,Goal& goal){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_ur_msgs_action_SendScript_poll(&advertise_,&id,&goal);}
roswasmc_result respond_feedback(uint64_t id,Feedback const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_msgs_action_SendScript_respond_feedback(&respond_,id,&value);}
roswasmc_result respond_result(uint64_t id,uint8_t kind,Result const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_msgs_action_SendScript_respond_result(&respond_,id,kind,&value);}
/** One pass, at most one feedback plus one terminal response; never implicit replay. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t id{};Goal goal{};auto r=roswasmc_bounded_2_ur_msgs_action_SendScript_poll(&advertise_,&id,&goal);if(r.error)return r.error;auto replies=(*handler)(id,goal);if(replies.send_result&&(replies.kind<2||replies.kind>4||!roswasmc_bounded_2_ur_msgs_action_SendScript_Result_valid(&replies.result)))return 22;if(replies.send_feedback&&!roswasmc_bounded_2_ur_msgs_action_SendScript_Feedback_valid(&replies.feedback))return 22;if(replies.send_feedback){r=roswasmc_bounded_2_ur_msgs_action_SendScript_respond_feedback(&respond_,id,&replies.feedback);if(r.error)return r.error;}return replies.send_result?roswasmc_bounded_2_ur_msgs_action_SendScript_respond_result(&respond_,id,replies.kind,&replies.result).error:0;}
};
/** Complete owned custom ur_msgs/action/ToolContact Action client with four separate roles. */
class Ros2Ur_msgsActionToolContactClient {roswasmc_context context_;std::array<uint64_t,1024> goals_{};roswasmc_action_bundle bundle_{};
public:
using Goal=Ros2Ur_msgsActionToolContact_Goal;using Result=Ros2Ur_msgsActionToolContact_Result;using Feedback=Ros2Ur_msgsActionToolContact_Feedback;
Ros2Ur_msgsActionToolContactClient(roswasmc_context const& context,roswasmc_action_aliases const& aliases,size_t slots=8):context_{context.backend} {roswasmc_action_bundle_init(&context_,&bundle_,&aliases,goals_.data(),slots);}
Ros2Ur_msgsActionToolContactClient(Ros2Ur_msgsActionToolContactClient const&)=delete;Ros2Ur_msgsActionToolContactClient& operator=(Ros2Ur_msgsActionToolContactClient const&)=delete;Ros2Ur_msgsActionToolContactClient(Ros2Ur_msgsActionToolContactClient&&)=delete;
~Ros2Ur_msgsActionToolContactClient(){if(bundle_.active){std::array<uint32_t,5> statuses{};roswasmc_action_bundle_close_checked(&bundle_,0,statuses.data(),statuses.size());}}
uint32_t error() const noexcept{return bundle_.error?bundle_.error:bundle_.active?0:9;}bool is_open() const noexcept{return bundle_.active;}bool owns(uint64_t id)const{return roswasmc_action_bundle_owns(&bundle_,id);}
roswasmc_result send_goal(Goal const& value,uint64_t& id){return roswasmc_bounded_2_ur_msgs_action_ToolContact_send(&bundle_,&value,&id);}
roswasmc_result feedback(uint64_t id,Feedback& value){return roswasmc_bounded_2_ur_msgs_action_ToolContact_feedback(&bundle_,id,&value);}
roswasmc_result result(uint64_t id,uint8_t& kind,Result& value){return roswasmc_bounded_2_ur_msgs_action_ToolContact_result(&bundle_,id,&kind,&value);}
roswasmc_result cancel_own(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_own(&bundle_,id,&accepted);}
roswasmc_result cancel_any(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_any(&bundle_,id,&accepted);}
roswasmc_result cancel_all(int& accepted){return roswasmc_action_bundle_cancel_all(&bundle_,&accepted);}
roswasmc_cleanup_report close_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_close_checked(&bundle_,primary,statuses.data(),statuses.size());}
roswasmc_cleanup_report revoke_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_revoke_checked(&bundle_,primary,statuses.data(),statuses.size());}
};
/** Typed custom ur_msgs/action/ToolContact server with explicit feedback/terminal component codecs. */
class Ros2Ur_msgsActionToolContactServer:public ServerOwners {
public:
using Goal=Ros2Ur_msgsActionToolContact_Goal;using Result=Ros2Ur_msgsActionToolContact_Result;using Feedback=Ros2Ur_msgsActionToolContact_Feedback;
/** Handler may choose separate feedback/result effects; kind must be2..4 when terminal. */
struct Replies {Feedback feedback{};Result result{};uint8_t kind{2};bool send_feedback{true},send_result{true};};
private:using Handler=std::function<Replies(uint64_t,Goal const&)>;std::shared_ptr<Handler> handler_;
public:
Ros2Ur_msgsActionToolContactServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,true} {}
void on_goal(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& id,Goal& goal){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_ur_msgs_action_ToolContact_poll(&advertise_,&id,&goal);}
roswasmc_result respond_feedback(uint64_t id,Feedback const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_msgs_action_ToolContact_respond_feedback(&respond_,id,&value);}
roswasmc_result respond_result(uint64_t id,uint8_t kind,Result const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_msgs_action_ToolContact_respond_result(&respond_,id,kind,&value);}
/** One pass, at most one feedback plus one terminal response; never implicit replay. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t id{};Goal goal{};auto r=roswasmc_bounded_2_ur_msgs_action_ToolContact_poll(&advertise_,&id,&goal);if(r.error)return r.error;auto replies=(*handler)(id,goal);if(replies.send_result&&(replies.kind<2||replies.kind>4||!roswasmc_bounded_2_ur_msgs_action_ToolContact_Result_valid(&replies.result)))return 22;if(replies.send_feedback&&!roswasmc_bounded_2_ur_msgs_action_ToolContact_Feedback_valid(&replies.feedback))return 22;if(replies.send_feedback){r=roswasmc_bounded_2_ur_msgs_action_ToolContact_respond_feedback(&respond_,id,&replies.feedback);if(r.error)return r.error;}return replies.send_result?roswasmc_bounded_2_ur_msgs_action_ToolContact_respond_result(&respond_,id,replies.kind,&replies.result).error:0;}
};
/** Complete custom ur_msgs/srv/GetRobotSoftwareVersion client; default policy returns13 from construction. */
class Ros2Ur_msgsSrvGetRobotSoftwareVersionClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Ur_msgsSrvGetRobotSoftwareVersion_Request;using Response=Ros2Ur_msgsSrvGetRobotSoftwareVersion_Response;
Ros2Ur_msgsSrvGetRobotSoftwareVersionClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Ur_msgsSrvGetRobotSoftwareVersionClient(Ros2Ur_msgsSrvGetRobotSoftwareVersionClient const&)=delete;Ros2Ur_msgsSrvGetRobotSoftwareVersionClient& operator=(Ros2Ur_msgsSrvGetRobotSoftwareVersionClient const&)=delete;Ros2Ur_msgsSrvGetRobotSoftwareVersionClient(Ros2Ur_msgsSrvGetRobotSoftwareVersionClient&&)=delete;
~Ros2Ur_msgsSrvGetRobotSoftwareVersionClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ur_msgs/srv/GetRobotSoftwareVersion server; advertise/respond stay separate authorities. */
class Ros2Ur_msgsSrvGetRobotSoftwareVersionServer:public ServerOwners {
using Handler=std::function<Ros2Ur_msgsSrvGetRobotSoftwareVersion_Response(Ros2Ur_msgsSrvGetRobotSoftwareVersion_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Ur_msgsSrvGetRobotSoftwareVersion_Request;using Response=Ros2Ur_msgsSrvGetRobotSoftwareVersion_Response;
Ros2Ur_msgsSrvGetRobotSoftwareVersionServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_respond(&respond_,ticket,&response).error;}
};
/** Complete custom ur_msgs/srv/SetAnalogOutput client; default policy returns13 from construction. */
class Ros2Ur_msgsSrvSetAnalogOutputClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Ur_msgsSrvSetAnalogOutput_Request;using Response=Ros2Ur_msgsSrvSetAnalogOutput_Response;
Ros2Ur_msgsSrvSetAnalogOutputClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Ur_msgsSrvSetAnalogOutputClient(Ros2Ur_msgsSrvSetAnalogOutputClient const&)=delete;Ros2Ur_msgsSrvSetAnalogOutputClient& operator=(Ros2Ur_msgsSrvSetAnalogOutputClient const&)=delete;Ros2Ur_msgsSrvSetAnalogOutputClient(Ros2Ur_msgsSrvSetAnalogOutputClient&&)=delete;
~Ros2Ur_msgsSrvSetAnalogOutputClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ur_msgs/srv/SetAnalogOutput server; advertise/respond stay separate authorities. */
class Ros2Ur_msgsSrvSetAnalogOutputServer:public ServerOwners {
using Handler=std::function<Ros2Ur_msgsSrvSetAnalogOutput_Response(Ros2Ur_msgsSrvSetAnalogOutput_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Ur_msgsSrvSetAnalogOutput_Request;using Response=Ros2Ur_msgsSrvSetAnalogOutput_Response;
Ros2Ur_msgsSrvSetAnalogOutputServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_respond(&respond_,ticket,&response).error;}
};
/** Complete custom ur_msgs/srv/SetForceMode client; default policy returns13 from construction. */
class Ros2Ur_msgsSrvSetForceModeClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Ur_msgsSrvSetForceMode_Request;using Response=Ros2Ur_msgsSrvSetForceMode_Response;
Ros2Ur_msgsSrvSetForceModeClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Ur_msgsSrvSetForceModeClient(Ros2Ur_msgsSrvSetForceModeClient const&)=delete;Ros2Ur_msgsSrvSetForceModeClient& operator=(Ros2Ur_msgsSrvSetForceModeClient const&)=delete;Ros2Ur_msgsSrvSetForceModeClient(Ros2Ur_msgsSrvSetForceModeClient&&)=delete;
~Ros2Ur_msgsSrvSetForceModeClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_msgs_srv_SetForceMode_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ur_msgs/srv/SetForceMode server; advertise/respond stay separate authorities. */
class Ros2Ur_msgsSrvSetForceModeServer:public ServerOwners {
using Handler=std::function<Ros2Ur_msgsSrvSetForceMode_Response(Ros2Ur_msgsSrvSetForceMode_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Ur_msgsSrvSetForceMode_Request;using Response=Ros2Ur_msgsSrvSetForceMode_Response;
Ros2Ur_msgsSrvSetForceModeServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_ur_msgs_srv_SetForceMode_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_msgs_srv_SetForceMode_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_ur_msgs_srv_SetForceMode_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_ur_msgs_srv_SetForceMode_respond(&respond_,ticket,&response).error;}
};
/** Complete custom ur_msgs/srv/SetFrictionModelParameters client; default policy returns13 from construction. */
class Ros2Ur_msgsSrvSetFrictionModelParametersClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Ur_msgsSrvSetFrictionModelParameters_Request;using Response=Ros2Ur_msgsSrvSetFrictionModelParameters_Response;
Ros2Ur_msgsSrvSetFrictionModelParametersClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Ur_msgsSrvSetFrictionModelParametersClient(Ros2Ur_msgsSrvSetFrictionModelParametersClient const&)=delete;Ros2Ur_msgsSrvSetFrictionModelParametersClient& operator=(Ros2Ur_msgsSrvSetFrictionModelParametersClient const&)=delete;Ros2Ur_msgsSrvSetFrictionModelParametersClient(Ros2Ur_msgsSrvSetFrictionModelParametersClient&&)=delete;
~Ros2Ur_msgsSrvSetFrictionModelParametersClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ur_msgs/srv/SetFrictionModelParameters server; advertise/respond stay separate authorities. */
class Ros2Ur_msgsSrvSetFrictionModelParametersServer:public ServerOwners {
using Handler=std::function<Ros2Ur_msgsSrvSetFrictionModelParameters_Response(Ros2Ur_msgsSrvSetFrictionModelParameters_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Ur_msgsSrvSetFrictionModelParameters_Request;using Response=Ros2Ur_msgsSrvSetFrictionModelParameters_Response;
Ros2Ur_msgsSrvSetFrictionModelParametersServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_respond(&respond_,ticket,&response).error;}
};
/** Complete custom ur_msgs/srv/SetGravity client; default policy returns13 from construction. */
class Ros2Ur_msgsSrvSetGravityClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Ur_msgsSrvSetGravity_Request;using Response=Ros2Ur_msgsSrvSetGravity_Response;
Ros2Ur_msgsSrvSetGravityClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Ur_msgsSrvSetGravityClient(Ros2Ur_msgsSrvSetGravityClient const&)=delete;Ros2Ur_msgsSrvSetGravityClient& operator=(Ros2Ur_msgsSrvSetGravityClient const&)=delete;Ros2Ur_msgsSrvSetGravityClient(Ros2Ur_msgsSrvSetGravityClient&&)=delete;
~Ros2Ur_msgsSrvSetGravityClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_msgs_srv_SetGravity_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ur_msgs/srv/SetGravity server; advertise/respond stay separate authorities. */
class Ros2Ur_msgsSrvSetGravityServer:public ServerOwners {
using Handler=std::function<Ros2Ur_msgsSrvSetGravity_Response(Ros2Ur_msgsSrvSetGravity_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Ur_msgsSrvSetGravity_Request;using Response=Ros2Ur_msgsSrvSetGravity_Response;
Ros2Ur_msgsSrvSetGravityServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_ur_msgs_srv_SetGravity_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_msgs_srv_SetGravity_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_ur_msgs_srv_SetGravity_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_ur_msgs_srv_SetGravity_respond(&respond_,ticket,&response).error;}
};
/** Complete custom ur_msgs/srv/SetIO client; default policy returns13 from construction. */
class Ros2Ur_msgsSrvSetIOClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Ur_msgsSrvSetIO_Request;using Response=Ros2Ur_msgsSrvSetIO_Response;
Ros2Ur_msgsSrvSetIOClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Ur_msgsSrvSetIOClient(Ros2Ur_msgsSrvSetIOClient const&)=delete;Ros2Ur_msgsSrvSetIOClient& operator=(Ros2Ur_msgsSrvSetIOClient const&)=delete;Ros2Ur_msgsSrvSetIOClient(Ros2Ur_msgsSrvSetIOClient&&)=delete;
~Ros2Ur_msgsSrvSetIOClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_msgs_srv_SetIO_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ur_msgs/srv/SetIO server; advertise/respond stay separate authorities. */
class Ros2Ur_msgsSrvSetIOServer:public ServerOwners {
using Handler=std::function<Ros2Ur_msgsSrvSetIO_Response(Ros2Ur_msgsSrvSetIO_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Ur_msgsSrvSetIO_Request;using Response=Ros2Ur_msgsSrvSetIO_Response;
Ros2Ur_msgsSrvSetIOServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_ur_msgs_srv_SetIO_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_msgs_srv_SetIO_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_ur_msgs_srv_SetIO_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_ur_msgs_srv_SetIO_respond(&respond_,ticket,&response).error;}
};
/** Complete custom ur_msgs/srv/SetPayload client; default policy returns13 from construction. */
class Ros2Ur_msgsSrvSetPayloadClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Ur_msgsSrvSetPayload_Request;using Response=Ros2Ur_msgsSrvSetPayload_Response;
Ros2Ur_msgsSrvSetPayloadClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Ur_msgsSrvSetPayloadClient(Ros2Ur_msgsSrvSetPayloadClient const&)=delete;Ros2Ur_msgsSrvSetPayloadClient& operator=(Ros2Ur_msgsSrvSetPayloadClient const&)=delete;Ros2Ur_msgsSrvSetPayloadClient(Ros2Ur_msgsSrvSetPayloadClient&&)=delete;
~Ros2Ur_msgsSrvSetPayloadClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_msgs_srv_SetPayload_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ur_msgs/srv/SetPayload server; advertise/respond stay separate authorities. */
class Ros2Ur_msgsSrvSetPayloadServer:public ServerOwners {
using Handler=std::function<Ros2Ur_msgsSrvSetPayload_Response(Ros2Ur_msgsSrvSetPayload_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Ur_msgsSrvSetPayload_Request;using Response=Ros2Ur_msgsSrvSetPayload_Response;
Ros2Ur_msgsSrvSetPayloadServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_ur_msgs_srv_SetPayload_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_msgs_srv_SetPayload_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_ur_msgs_srv_SetPayload_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_ur_msgs_srv_SetPayload_respond(&respond_,ticket,&response).error;}
};
/** Complete custom ur_msgs/srv/SetSpeedSliderFraction client; default policy returns13 from construction. */
class Ros2Ur_msgsSrvSetSpeedSliderFractionClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Ur_msgsSrvSetSpeedSliderFraction_Request;using Response=Ros2Ur_msgsSrvSetSpeedSliderFraction_Response;
Ros2Ur_msgsSrvSetSpeedSliderFractionClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Ur_msgsSrvSetSpeedSliderFractionClient(Ros2Ur_msgsSrvSetSpeedSliderFractionClient const&)=delete;Ros2Ur_msgsSrvSetSpeedSliderFractionClient& operator=(Ros2Ur_msgsSrvSetSpeedSliderFractionClient const&)=delete;Ros2Ur_msgsSrvSetSpeedSliderFractionClient(Ros2Ur_msgsSrvSetSpeedSliderFractionClient&&)=delete;
~Ros2Ur_msgsSrvSetSpeedSliderFractionClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom ur_msgs/srv/SetSpeedSliderFraction server; advertise/respond stay separate authorities. */
class Ros2Ur_msgsSrvSetSpeedSliderFractionServer:public ServerOwners {
using Handler=std::function<Ros2Ur_msgsSrvSetSpeedSliderFraction_Response(Ros2Ur_msgsSrvSetSpeedSliderFraction_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Ur_msgsSrvSetSpeedSliderFraction_Request;using Response=Ros2Ur_msgsSrvSetSpeedSliderFraction_Response;
Ros2Ur_msgsSrvSetSpeedSliderFractionServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_respond(&respond_,ticket,&response).error;}
};
}
