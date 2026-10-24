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
/** Complete custom uwvm_ros_bounded_v2/Echo client; default policy returns13 from construction. */
class Ros1Uwvm_ros_bounded_v2EchoClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Uwvm_ros_bounded_v2EchoRequest;using Response=Ros1Uwvm_ros_bounded_v2EchoResponse;
Ros1Uwvm_ros_bounded_v2EchoClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Uwvm_ros_bounded_v2EchoClient(Ros1Uwvm_ros_bounded_v2EchoClient const&)=delete;Ros1Uwvm_ros_bounded_v2EchoClient& operator=(Ros1Uwvm_ros_bounded_v2EchoClient const&)=delete;Ros1Uwvm_ros_bounded_v2EchoClient(Ros1Uwvm_ros_bounded_v2EchoClient&&)=delete;
~Ros1Uwvm_ros_bounded_v2EchoClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_uwvm_ros_bounded_v2_Echo_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom uwvm_ros_bounded_v2/Echo server; advertise/respond stay separate authorities. */
class Ros1Uwvm_ros_bounded_v2EchoServer:public ServerOwners {
using Handler=std::function<Ros1Uwvm_ros_bounded_v2EchoResponse(Ros1Uwvm_ros_bounded_v2EchoRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Uwvm_ros_bounded_v2EchoRequest;using Response=Ros1Uwvm_ros_bounded_v2EchoResponse;
Ros1Uwvm_ros_bounded_v2EchoServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_uwvm_ros_bounded_v2_Echo_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_uwvm_ros_bounded_v2_Echo_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_uwvm_ros_bounded_v2_Echo_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_uwvm_ros_bounded_v2_Echo_respond(&respond_,ticket,&response).error;}
};
/** Complete custom uwvm_ros_bounded_v2/Empty client; default policy returns13 from construction. */
class Ros1Uwvm_ros_bounded_v2EmptyClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros1Uwvm_ros_bounded_v2EmptyRequest;using Response=Ros1Uwvm_ros_bounded_v2EmptyResponse;
Ros1Uwvm_ros_bounded_v2EmptyClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros1Uwvm_ros_bounded_v2EmptyClient(Ros1Uwvm_ros_bounded_v2EmptyClient const&)=delete;Ros1Uwvm_ros_bounded_v2EmptyClient& operator=(Ros1Uwvm_ros_bounded_v2EmptyClient const&)=delete;Ros1Uwvm_ros_bounded_v2EmptyClient(Ros1Uwvm_ros_bounded_v2EmptyClient&&)=delete;
~Ros1Uwvm_ros_bounded_v2EmptyClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_uwvm_ros_bounded_v2_Empty_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom uwvm_ros_bounded_v2/Empty server; advertise/respond stay separate authorities. */
class Ros1Uwvm_ros_bounded_v2EmptyServer:public ServerOwners {
using Handler=std::function<Ros1Uwvm_ros_bounded_v2EmptyResponse(Ros1Uwvm_ros_bounded_v2EmptyRequest const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros1Uwvm_ros_bounded_v2EmptyRequest;using Response=Ros1Uwvm_ros_bounded_v2EmptyResponse;
Ros1Uwvm_ros_bounded_v2EmptyServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_uwvm_ros_bounded_v2_Empty_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_uwvm_ros_bounded_v2_Empty_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_1_uwvm_ros_bounded_v2_Empty_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_1_uwvm_ros_bounded_v2_Empty_respond(&respond_,ticket,&response).error;}
};
/** Complete owned custom uwvm_ros_bounded_v2/Digest Action client with four separate roles. */
class Ros1Uwvm_ros_bounded_v2DigestClient {roswasmc_context context_;std::array<uint64_t,1024> goals_{};roswasmc_action_bundle bundle_{};
public:
using Goal=Ros1Uwvm_ros_bounded_v2DigestGoal;using Result=Ros1Uwvm_ros_bounded_v2DigestResult;using Feedback=Ros1Uwvm_ros_bounded_v2DigestFeedback;
Ros1Uwvm_ros_bounded_v2DigestClient(roswasmc_context const& context,roswasmc_action_aliases const& aliases,size_t slots=8):context_{context.backend} {roswasmc_action_bundle_init(&context_,&bundle_,&aliases,goals_.data(),slots);}
Ros1Uwvm_ros_bounded_v2DigestClient(Ros1Uwvm_ros_bounded_v2DigestClient const&)=delete;Ros1Uwvm_ros_bounded_v2DigestClient& operator=(Ros1Uwvm_ros_bounded_v2DigestClient const&)=delete;Ros1Uwvm_ros_bounded_v2DigestClient(Ros1Uwvm_ros_bounded_v2DigestClient&&)=delete;
~Ros1Uwvm_ros_bounded_v2DigestClient(){if(bundle_.active){std::array<uint32_t,5> statuses{};roswasmc_action_bundle_close_checked(&bundle_,0,statuses.data(),statuses.size());}}
uint32_t error() const noexcept{return bundle_.error?bundle_.error:bundle_.active?0:9;}bool is_open() const noexcept{return bundle_.active;}bool owns(uint64_t id)const{return roswasmc_action_bundle_owns(&bundle_,id);}
roswasmc_result send_goal(Goal const& value,uint64_t& id){return roswasmc_bounded_1_uwvm_ros_bounded_v2_Digest_send(&bundle_,&value,&id);}
roswasmc_result feedback(uint64_t id,Feedback& value){return roswasmc_bounded_1_uwvm_ros_bounded_v2_Digest_feedback(&bundle_,id,&value);}
roswasmc_result result(uint64_t id,uint8_t& kind,Result& value){return roswasmc_bounded_1_uwvm_ros_bounded_v2_Digest_result(&bundle_,id,&kind,&value);}
roswasmc_result cancel_own(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_own(&bundle_,id,&accepted);}
roswasmc_result cancel_any(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_any(&bundle_,id,&accepted);}
roswasmc_result cancel_all(int& accepted){return roswasmc_action_bundle_cancel_all(&bundle_,&accepted);}
roswasmc_cleanup_report close_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_close_checked(&bundle_,primary,statuses.data(),statuses.size());}
roswasmc_cleanup_report revoke_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_revoke_checked(&bundle_,primary,statuses.data(),statuses.size());}
};
/** Typed custom uwvm_ros_bounded_v2/Digest server with explicit feedback/terminal component codecs. */
class Ros1Uwvm_ros_bounded_v2DigestServer:public ServerOwners {
public:
using Goal=Ros1Uwvm_ros_bounded_v2DigestGoal;using Result=Ros1Uwvm_ros_bounded_v2DigestResult;using Feedback=Ros1Uwvm_ros_bounded_v2DigestFeedback;
/** Handler may choose separate feedback/result effects; kind must be2..4 when terminal. */
struct Replies {Feedback feedback{};Result result{};uint8_t kind{2};bool send_feedback{true},send_result{true};};
private:using Handler=std::function<Replies(uint64_t,Goal const&)>;std::shared_ptr<Handler> handler_;
public:
Ros1Uwvm_ros_bounded_v2DigestServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,true} {}
void on_goal(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& id,Goal& goal){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_1_uwvm_ros_bounded_v2_Digest_poll(&advertise_,&id,&goal);}
roswasmc_result respond_feedback(uint64_t id,Feedback const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_uwvm_ros_bounded_v2_Digest_respond_feedback(&respond_,id,&value);}
roswasmc_result respond_result(uint64_t id,uint8_t kind,Result const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_1_uwvm_ros_bounded_v2_Digest_respond_result(&respond_,id,kind,&value);}
/** One pass, at most one feedback plus one terminal response; never implicit replay. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t id{};Goal goal{};auto r=roswasmc_bounded_1_uwvm_ros_bounded_v2_Digest_poll(&advertise_,&id,&goal);if(r.error)return r.error;auto replies=(*handler)(id,goal);if(replies.send_result&&(replies.kind<2||replies.kind>4||!roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestResult_valid(&replies.result)))return 22;if(replies.send_feedback&&!roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestFeedback_valid(&replies.feedback))return 22;if(replies.send_feedback){r=roswasmc_bounded_1_uwvm_ros_bounded_v2_Digest_respond_feedback(&respond_,id,&replies.feedback);if(r.error)return r.error;}return replies.send_result?roswasmc_bounded_1_uwvm_ros_bounded_v2_Digest_respond_result(&respond_,id,replies.kind,&replies.result).error:0;}
};
/** Complete custom uwvm_ros_bounded_v2/srv/Echo client; default policy returns13 from construction. */
class Ros2Uwvm_ros_bounded_v2SrvEchoClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Uwvm_ros_bounded_v2SrvEcho_Request;using Response=Ros2Uwvm_ros_bounded_v2SrvEcho_Response;
Ros2Uwvm_ros_bounded_v2SrvEchoClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Uwvm_ros_bounded_v2SrvEchoClient(Ros2Uwvm_ros_bounded_v2SrvEchoClient const&)=delete;Ros2Uwvm_ros_bounded_v2SrvEchoClient& operator=(Ros2Uwvm_ros_bounded_v2SrvEchoClient const&)=delete;Ros2Uwvm_ros_bounded_v2SrvEchoClient(Ros2Uwvm_ros_bounded_v2SrvEchoClient&&)=delete;
~Ros2Uwvm_ros_bounded_v2SrvEchoClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom uwvm_ros_bounded_v2/srv/Echo server; advertise/respond stay separate authorities. */
class Ros2Uwvm_ros_bounded_v2SrvEchoServer:public ServerOwners {
using Handler=std::function<Ros2Uwvm_ros_bounded_v2SrvEcho_Response(Ros2Uwvm_ros_bounded_v2SrvEcho_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Uwvm_ros_bounded_v2SrvEcho_Request;using Response=Ros2Uwvm_ros_bounded_v2SrvEcho_Response;
Ros2Uwvm_ros_bounded_v2SrvEchoServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_respond(&respond_,ticket,&response).error;}
};
/** Complete custom uwvm_ros_bounded_v2/srv/Empty client; default policy returns13 from construction. */
class Ros2Uwvm_ros_bounded_v2SrvEmptyClient {roswasmc_context context_;roswasmc_capability cap_{};uint32_t error_{};
public:
using Request=Ros2Uwvm_ros_bounded_v2SrvEmpty_Request;using Response=Ros2Uwvm_ros_bounded_v2SrvEmpty_Response;
Ros2Uwvm_ros_bounded_v2SrvEmptyClient(roswasmc_context const& context,std::string_view selected):context_{context.backend} {roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}
Ros2Uwvm_ros_bounded_v2SrvEmptyClient(Ros2Uwvm_ros_bounded_v2SrvEmptyClient const&)=delete;Ros2Uwvm_ros_bounded_v2SrvEmptyClient& operator=(Ros2Uwvm_ros_bounded_v2SrvEmptyClient const&)=delete;Ros2Uwvm_ros_bounded_v2SrvEmptyClient(Ros2Uwvm_ros_bounded_v2SrvEmptyClient&&)=delete;
~Ros2Uwvm_ros_bounded_v2SrvEmptyClient(){if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}
uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}
roswasmc_result call(Request const& request,Response& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_call(&cap_,&request,&response);}
uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}
};
/** Typed custom uwvm_ros_bounded_v2/srv/Empty server; advertise/respond stay separate authorities. */
class Ros2Uwvm_ros_bounded_v2SrvEmptyServer:public ServerOwners {
using Handler=std::function<Ros2Uwvm_ros_bounded_v2SrvEmpty_Response(Ros2Uwvm_ros_bounded_v2SrvEmpty_Request const&)>;std::shared_ptr<Handler> handler_;
public:
using Request=Ros2Uwvm_ros_bounded_v2SrvEmpty_Request;using Response=Ros2Uwvm_ros_bounded_v2SrvEmpty_Response;
Ros2Uwvm_ros_bounded_v2SrvEmptyServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,false} {}
/** Replacement applies next pass; the same mutable functor object survives current dispatch. */
void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& ticket,Request& request){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_poll(&advertise_,&ticket,&request);}
roswasmc_result respond(uint64_t ticket,Response const& response){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_respond(&respond_,ticket,&response);}
/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t ticket{};Request request{};auto r=roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_respond(&respond_,ticket,&response).error;}
};
/** Complete owned custom uwvm_ros_bounded_v2/action/Digest Action client with four separate roles. */
class Ros2Uwvm_ros_bounded_v2ActionDigestClient {roswasmc_context context_;std::array<uint64_t,1024> goals_{};roswasmc_action_bundle bundle_{};
public:
using Goal=Ros2Uwvm_ros_bounded_v2ActionDigest_Goal;using Result=Ros2Uwvm_ros_bounded_v2ActionDigest_Result;using Feedback=Ros2Uwvm_ros_bounded_v2ActionDigest_Feedback;
Ros2Uwvm_ros_bounded_v2ActionDigestClient(roswasmc_context const& context,roswasmc_action_aliases const& aliases,size_t slots=8):context_{context.backend} {roswasmc_action_bundle_init(&context_,&bundle_,&aliases,goals_.data(),slots);}
Ros2Uwvm_ros_bounded_v2ActionDigestClient(Ros2Uwvm_ros_bounded_v2ActionDigestClient const&)=delete;Ros2Uwvm_ros_bounded_v2ActionDigestClient& operator=(Ros2Uwvm_ros_bounded_v2ActionDigestClient const&)=delete;Ros2Uwvm_ros_bounded_v2ActionDigestClient(Ros2Uwvm_ros_bounded_v2ActionDigestClient&&)=delete;
~Ros2Uwvm_ros_bounded_v2ActionDigestClient(){if(bundle_.active){std::array<uint32_t,5> statuses{};roswasmc_action_bundle_close_checked(&bundle_,0,statuses.data(),statuses.size());}}
uint32_t error() const noexcept{return bundle_.error?bundle_.error:bundle_.active?0:9;}bool is_open() const noexcept{return bundle_.active;}bool owns(uint64_t id)const{return roswasmc_action_bundle_owns(&bundle_,id);}
roswasmc_result send_goal(Goal const& value,uint64_t& id){return roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_send(&bundle_,&value,&id);}
roswasmc_result feedback(uint64_t id,Feedback& value){return roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_feedback(&bundle_,id,&value);}
roswasmc_result result(uint64_t id,uint8_t& kind,Result& value){return roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_result(&bundle_,id,&kind,&value);}
roswasmc_result cancel_own(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_own(&bundle_,id,&accepted);}
roswasmc_result cancel_any(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_any(&bundle_,id,&accepted);}
roswasmc_result cancel_all(int& accepted){return roswasmc_action_bundle_cancel_all(&bundle_,&accepted);}
roswasmc_cleanup_report close_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_close_checked(&bundle_,primary,statuses.data(),statuses.size());}
roswasmc_cleanup_report revoke_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_revoke_checked(&bundle_,primary,statuses.data(),statuses.size());}
};
/** Typed custom uwvm_ros_bounded_v2/action/Digest server with explicit feedback/terminal component codecs. */
class Ros2Uwvm_ros_bounded_v2ActionDigestServer:public ServerOwners {
public:
using Goal=Ros2Uwvm_ros_bounded_v2ActionDigest_Goal;using Result=Ros2Uwvm_ros_bounded_v2ActionDigest_Result;using Feedback=Ros2Uwvm_ros_bounded_v2ActionDigest_Feedback;
/** Handler may choose separate feedback/result effects; kind must be2..4 when terminal. */
struct Replies {Feedback feedback{};Result result{};uint8_t kind{2};bool send_feedback{true},send_result{true};};
private:using Handler=std::function<Replies(uint64_t,Goal const&)>;std::shared_ptr<Handler> handler_;
public:
Ros2Uwvm_ros_bounded_v2ActionDigestServer(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{context,advertise,respond,true} {}
void on_goal(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}
roswasmc_result poll_once(uint64_t& id,Goal& goal){if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{busy_};return roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_poll(&advertise_,&id,&goal);}
roswasmc_result respond_feedback(uint64_t id,Feedback const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_respond_feedback(&respond_,id,&value);}
roswasmc_result respond_result(uint64_t id,uint8_t kind,Result const& value){if(error())return roswasmc_action_bundle_failure(error());return roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_respond_result(&respond_,id,kind,&value);}
/** One pass, at most one feedback plus one terminal response; never implicit replay. */
/** One caller-driven executor pass; Idle11 preserves pending owners. */
uint32_t poll_once(){return process_once();}
uint32_t process_once(){if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{busy_};uint64_t id{};Goal goal{};auto r=roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_poll(&advertise_,&id,&goal);if(r.error)return r.error;auto replies=(*handler)(id,goal);if(replies.send_result&&(replies.kind<2||replies.kind>4||!roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Result_valid(&replies.result)))return 22;if(replies.send_feedback&&!roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Feedback_valid(&replies.feedback))return 22;if(replies.send_feedback){r=roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_respond_feedback(&respond_,id,&replies.feedback);if(r.error)return r.error;}return replies.send_result?roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_respond_result(&respond_,id,replies.kind,&replies.result).error:0;}
};
}
