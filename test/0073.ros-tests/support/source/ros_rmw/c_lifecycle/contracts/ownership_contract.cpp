// SPDX-License-Identifier: Apache-2.0
// Authored real-SDK ownership contracts, NOT executed results. The only wrappers
// delegate to actual linked C fini/malloc symbols. No ROS function is replaced
// by a success stub, no Node/TypeSupport/graph/native request is fabricated.
#include <rclcpp/any_service_callback.hpp>
#include <platform/ros2/cpp/uwvm_rmw/c_service_lifecycle.hpp>
#include <type_description_interfaces/srv/get_type_description.hpp>
#include <type_description_interfaces/msg/detail/field__functions.h>
#include <type_description_interfaces/msg/detail/individual_type_description__functions.h>
#include <type_description_interfaces/msg/detail/type_source__functions.h>
#include <type_description_interfaces/msg/detail/key_value__functions.h>
#include <rosidl_runtime_c/string_functions.h>
#include <rcl/node.h>
#include <rcl/error_handling.h>
#include <atomic>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <memory>
#include <new>
#include <stdexcept>
#include <string>
#if RCLCPP_UWVM_CALLBACK_C_LIFECYCLE_HEADER_VERSION != 1
#error "Actual ported original AnyServiceCallback header required"
#endif
using Q=type_description_interfaces__srv__GetTypeDescription_Request;
using S=type_description_interfaces__srv__GetTypeDescription_Response;
using uwvm_rmw::c_lifecycle::make_service_object;
static std::atomic<unsigned> q_fini{0},s_fini{0};
static std::atomic<bool> fail_next_cpp_new{false},fail_next_c_malloc{false};
// This fault concerns the actual C++ shared-owner allocation, before C fields.
void* operator new(std::size_t n){
 if(fail_next_cpp_new.exchange(false))throw std::bad_alloc{};
 if(void*p=std::malloc(n?n:1))return p;
 throw std::bad_alloc{};
}
void operator delete(void*p)noexcept{std::free(p);}
void operator delete(void*p,std::size_t)noexcept{std::free(p);}
extern "C" void* __real_malloc(std::size_t);
extern "C" void* __wrap_malloc(std::size_t n){
 if(fail_next_c_malloc.exchange(false))return nullptr;
 return __real_malloc(n);
}
extern "C" void __real_type_description_interfaces__srv__GetTypeDescription_Request__fini(Q*);
extern "C" void __real_type_description_interfaces__srv__GetTypeDescription_Response__fini(S*);
extern "C" void __wrap_type_description_interfaces__srv__GetTypeDescription_Request__fini(Q*p){
 ++q_fini;__real_type_description_interfaces__srv__GetTypeDescription_Request__fini(p);
}
extern "C" void __wrap_type_description_interfaces__srv__GetTypeDescription_Response__fini(S*p){
 ++s_fini;__real_type_description_interfaces__srv__GetTypeDescription_Response__fini(p);
}
static void require(bool ok){if(!ok)throw std::runtime_error("actual C ownership contract failed");}
static bool zero_string(rosidl_runtime_c__String const&s){return !s.data&&!s.size&&!s.capacity;}
static void assign(rosidl_runtime_c__String&s,char const*text){require(rosidl_runtime_c__String__assign(&s,text));}
static void init_request(Q&q){
 require(zero_string(q.type_name)&&zero_string(q.type_hash));
 require(type_description_interfaces__srv__GetTypeDescription_Request__init(&q));
 assign(q.type_name,"value_only/msg/Complete");
 assign(q.type_hash,"value-only-not-an-attested-native-hash");q.include_type_sources=true;
}
static void init_response(S&s){
 require(zero_string(s.failure_reason)&&!s.type_sources.data&&!s.type_sources.size&&!s.type_sources.capacity);
 // Same initial ownership state as the actual RCL18d handler's one init call.
 require(type_description_interfaces__srv__GetTypeDescription_Response__init(&s));
 s.successful=true;assign(s.failure_reason,"value-only response");
 auto&d=s.type_description.type_description;
 assign(d.type_name,"value_only/msg/Complete");
 type_description_interfaces__msg__Field__Sequence__fini(&d.fields);
 require(type_description_interfaces__msg__Field__Sequence__init(&d.fields,1));
 assign(d.fields.data[0].name,"field");assign(d.fields.data[0].default_value,"0");
 d.fields.data[0].type.type_id=type_description_interfaces__msg__FieldType__FIELD_TYPE_UINT8;
 d.fields.data[0].type.capacity=0;d.fields.data[0].type.string_capacity=0;
 assign(d.fields.data[0].type.nested_type_name,"");
 auto&refs=s.type_description.referenced_type_descriptions;
 type_description_interfaces__msg__IndividualTypeDescription__Sequence__fini(&refs);
 require(type_description_interfaces__msg__IndividualTypeDescription__Sequence__init(&refs,1));
 assign(refs.data[0].type_name,"value_only/msg/Dependency");
 type_description_interfaces__msg__TypeSource__Sequence__fini(&s.type_sources);
 require(type_description_interfaces__msg__TypeSource__Sequence__init(&s.type_sources,1));
 assign(s.type_sources.data[0].type_name,"value_only/msg/Complete");
 assign(s.type_sources.data[0].encoding,"msg");assign(s.type_sources.data[0].raw_file_contents,"uint8 field\n");
 type_description_interfaces__msg__KeyValue__Sequence__fini(&s.extra_information);
 require(type_description_interfaces__msg__KeyValue__Sequence__init(&s.extra_information,1));
 assign(s.extra_information.data[0].key,"ownership");assign(s.extra_information.data[0].value,"native-\xcf\x80");
}
static void zero_request(){auto before=q_fini.load();{auto q=make_service_object<Q>();require(zero_string(q->type_name)&&zero_string(q->type_hash));require(!q->include_type_sources);}require(q_fini==before+1);}
static void zero_response(){auto before=s_fini.load();{auto s=make_service_object<S>();require(zero_string(s->failure_reason));require(zero_string(s->type_description.type_description.type_name));require(!s->type_sources.data&&!s->extra_information.data);}require(s_fini==before+1);}
static void request_late_owner(){auto before=q_fini.load();std::weak_ptr<Q>w;std::shared_ptr<void> erased;{auto q=make_service_object<Q>();init_request(*q);w=q;erased=q;q.reset();require(q_fini==before);auto p=w.lock();require(p&&p->include_type_sources&&p->type_name.size==23);}require(q_fini==before);erased.reset();require(w.expired()&&q_fini==before+1);}
static void response_late_owner(){auto before=s_fini.load();auto s=make_service_object<S>();init_response(*s);std::weak_ptr<S>w=s;std::shared_ptr<void> erased=s;s.reset();require(s_fini==before);auto locked=w.lock();erased.reset();require(locked&&s_fini==before);require(locked->type_description.type_description.fields.size==1&&locked->type_sources.size==1&&locked->extra_information.size==1);locked.reset();require(w.expired()&&s_fini==before+1);}
struct CService{using Request=Q;using Response=S;}; // aliases real C types only
using Header=std::shared_ptr<rmw_request_id_t>;
static void callback_once(){
 auto before=s_fini.load();rclcpp::AnyServiceCallback<CService>cb;
 cb.set(std::function<void(std::shared_ptr<Q>,std::shared_ptr<S>)>([](auto q,auto s){require(q->type_name.size>0);init_response(*s);}));
 auto q=make_service_object<Q>();init_request(*q);auto h=std::make_shared<rmw_request_id_t>();
 auto s=cb.dispatch({},h,q);require(s&&s->successful&&s_fini==before);s.reset();require(s_fini==before+1);
}
static void callback_throws(){
 auto before=s_fini.load();rclcpp::AnyServiceCallback<CService>cb;
 cb.set(std::function<void(std::shared_ptr<Q>,std::shared_ptr<S>)>([](auto,auto s){init_response(*s);throw std::runtime_error("original callback exception");}));
 auto q=make_service_object<Q>();init_request(*q);auto h=std::make_shared<rmw_request_id_t>();
 try{(void)cb.dispatch({},h,q);require(false);}catch(std::runtime_error const&e){require(std::string(e.what())=="original callback exception");}
 require(s_fini==before+1&&q->type_name.data);
}
static void deferred_request(){
 auto bq=q_fini.load(),bs=s_fini.load();std::shared_ptr<Q>late;rclcpp::AnyServiceCallback<CService>cb;
 cb.set(std::function<void(Header,std::shared_ptr<Q>)>([&](auto,auto q){late=std::move(q);}));
 auto q=make_service_object<Q>();init_request(*q);auto h=std::make_shared<rmw_request_id_t>();require(!cb.dispatch({},h,q));q.reset();require(q_fini==bq&&s_fini==bs&&late);late.reset();require(q_fini==bq+1&&s_fini==bs);
}
static void deferred_with_service(){
 auto bq=q_fini.load(),bs=s_fini.load();std::shared_ptr<Q>late;rclcpp::AnyServiceCallback<CService>cb;
 cb.set(std::function<void(std::shared_ptr<rclcpp::Service<CService>>,Header,std::shared_ptr<Q>)>([&](auto service,auto,auto q){require(!service);late=std::move(q);}));
 auto q=make_service_object<Q>();auto h=std::make_shared<rmw_request_id_t>();require(!cb.dispatch({},h,q));q.reset();require(q_fini==bq&&s_fini==bs);late.reset();require(q_fini==bq+1&&s_fini==bs);
}
static void shared_allocation_failure(){
 auto bq=q_fini.load(),bs=s_fini.load();bool caught=false;fail_next_cpp_new=true;
 try{(void)make_service_object<Q>();}catch(std::bad_alloc const&){caught=true;}
 require(caught&&!fail_next_cpp_new&&q_fini==bq);
 caught=false;fail_next_cpp_new=true;try{(void)make_service_object<S>();}catch(std::bad_alloc const&){caught=true;}
 require(caught&&!fail_next_cpp_new&&s_fini==bs);
}
static void genuine_c_init_failure(){
 // This must hit actual static rcutils -> malloc. If an SDK ignores that
 // linkage, the consumed flag check FAILS; there is no simulated true result.
 auto bq=q_fini.load();{auto q=make_service_object<Q>();fail_next_c_malloc=true;bool ok=type_description_interfaces__srv__GetTypeDescription_Request__init(q.get());require(!fail_next_c_malloc&&!ok);}require(q_fini>=bq+1);
 auto bs=s_fini.load();{auto s=make_service_object<S>();fail_next_c_malloc=true;bool ok=type_description_interfaces__srv__GetTypeDescription_Response__init(s.get());require(!fail_next_c_malloc&&!ok);}require(s_fini>=bs+1);
 // The generated init's own rollback may call fini internally. The owner still
 // finalizes its remaining zero-valid state once; no live allocation is retried.
}
static void c_private_commit(){
 auto before=q_fini.load();auto q=make_service_object<Q>();Q next{};init_request(next);
 Q old=*q;*q=next;next=old;type_description_interfaces__srv__GetTypeDescription_Request__fini(&next);
 require(q->type_name.size>0&&q_fini==before+1);q.reset();require(q_fini==before+2);
}
static void cpp_generated_original(){
 using CPP=type_description_interfaces::srv::GetTypeDescription;
 static_assert(!std::is_same<CPP::Request,Q>::value&&!std::is_same<CPP::Response,S>::value,"real C/C++ distinction");
 auto q=make_service_object<CPP::Request>();require(q->include_type_sources);q->type_name="actual original C++ value";
 auto s=make_service_object<CPP::Response>();s->failure_reason="ordinary C++ ownership";
}
struct Generic{static unsigned made,dead;std::string value;Generic():value("ordinary C++"){++made;}~Generic(){++dead;}};
unsigned Generic::made=0;unsigned Generic::dead=0;
static void generic_original(){auto made=Generic::made,dead=Generic::dead;{auto p=make_service_object<Generic>();std::shared_ptr<void>v=p;p.reset();require(Generic::made==made+1&&Generic::dead==dead);}require(Generic::dead==dead+1);}
static void real_handler_preinit_return(){
 auto before=s_fini.load();auto q=make_service_object<Q>();auto s=make_service_object<S>();rmw_request_id_t h{};
 rcl_node_type_description_service_handle_request(nullptr,&h,q.get(),s.get());
 require(zero_string(s->failure_reason));rcl_reset_error();s.reset();require(s_fini==before+1);
}
int main(){
 struct Row{char const*name;void(*run)();};Row rows[]={
  {"zero-request",zero_request},{"zero-response",zero_response},{"request-late-owner",request_late_owner},
  {"response-late-owner",response_late_owner},{"original-callback-once",callback_once},{"callback-throw",callback_throws},
  {"deferred-request",deferred_request},{"deferred-with-service",deferred_with_service},{"shared-allocation-failure",shared_allocation_failure},
  {"genuine-c-init-failure",genuine_c_init_failure},{"private-owned-commit",c_private_commit},{"cpp-generated-original",cpp_generated_original},
  {"generic-original",generic_original},{"real-handler-preinit-return",real_handler_preinit_return}};
 for(auto const&row:rows){try{row.run();}catch(std::exception const&e){std::fprintf(stderr,"FAIL %s: %s\n",row.name,e.what());return 1;}}
 std::puts("PASS_LOCAL_C_LIFECYCLE_CONTRACTS_NOT_NATIVE_TRANSPORT_OR_GUEST_QUALIFICATION");return 0;
}
