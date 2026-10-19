/* SPDX-License-Identifier: Apache-2.0
 * Complete selected modern C graph, actual generated objects and real C
 * introspection. This is authored SOURCE, not generated/compiled/qualified.
 */
#include <uwvm_rmw/c_codecs.h>
#include <uwvm_rmw/contracts.h>
#include "uwvm_rmw_c_codecs_features.h"
#include <string.h>
#include <stddef.h>
#include <stdint.h>
struct uwvm_rmw_c_codecs_registry {
 struct uwvm_rmw_c_codecs_registry const*self;
 rcutils_allocator_t allocator;uwvm_rmw_c_native_identities selected;
 size_t string_bound;bool ready,exported;
 rosidl_message_type_support_t const*generic[9],*native[9];
 rosidl_service_type_support_t const*services[2],*native_services[2];
 uwvm_rmw_codec codecs[UWVM_RMW_C_CODEC_COUNT];
};
#if UWVM_RMW_C_CODECS_COMPLETE_API
#include <std_msgs/msg/string.h>
#include <std_srvs/srv/empty.h>
#include <example_interfaces/srv/add_two_ints.h>
#include <service_msgs/msg/service_event_info.h>
#include <builtin_interfaces/msg/time.h>
#include <rosidl_runtime_c/string_functions.h>
#include <rosidl_runtime_c/type_hash.h>
#include <rosidl_runtime_c/type_description/type_description__struct.h>
#include <rosidl_runtime_c/type_description/field_type__struct.h>
#include <rosidl_typesupport_introspection_c/identifier.h>
#include <rosidl_typesupport_introspection_c/message_introspection.h>
#include <rosidl_typesupport_introspection_c/service_introspection.h>
#include <rosidl_typesupport_introspection_c/field_types.h>
#include <roswasm/codec.h>
typedef std_msgs__msg__String String;
typedef builtin_interfaces__msg__Time Time;
typedef service_msgs__msg__ServiceEventInfo Info;
typedef std_srvs__srv__Empty_Request EmptyRequest;
typedef std_srvs__srv__Empty_Response EmptyResponse;
typedef std_srvs__srv__Empty_Event EmptyEvent;
typedef example_interfaces__srv__AddTwoInts_Request AddRequest;
typedef example_interfaces__srv__AddTwoInts_Response AddResponse;
typedef example_interfaces__srv__AddTwoInts_Event AddEvent;
enum{identities=11,messages=9,descriptors=7,none=11};
typedef struct {char const*name;uint8_t description_type,native_type;size_t offset,capacity,nested;bool array,upper;} Field;
typedef struct {char const*name,*ns,*local;size_t size,count;Field fields[4];uint16_t references;} Spec;
#define TD_nested rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE
#define TD_u8 rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT8
#define TD_i32 rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_INT32
#define TD_u32 rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT32
#define TD_i64 rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_INT64
#define TD_text rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_STRING
#define TD_bytes16 rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT8_ARRAY
#define TD_bounded rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE_BOUNDED_SEQUENCE
/* offsetof/sizeof refer only to the actual included ORIGINAL C definitions. */
static Spec const specs[identities]={

 {"std_msgs/msg/String","std_msgs__msg","String",sizeof(String),1,
  {{"data",TD_text,rosidl_typesupport_introspection_c__ROS_TYPE_STRING,offsetof(String,data),0,none,false,false}},0},
 {"builtin_interfaces/msg/Time","builtin_interfaces__msg","Time",sizeof(Time),2,
  {{"sec",TD_i32,rosidl_typesupport_introspection_c__ROS_TYPE_INT32,offsetof(Time,sec),0,none,false,false},
    {"nanosec",TD_u32,rosidl_typesupport_introspection_c__ROS_TYPE_UINT32,offsetof(Time,nanosec),0,none,false,false}},0},
 {"service_msgs/msg/ServiceEventInfo","service_msgs__msg","ServiceEventInfo",sizeof(Info),4,
  {{"event_type",TD_u8,rosidl_typesupport_introspection_c__ROS_TYPE_UINT8,offsetof(Info,event_type),0,none,false,false},
    {"stamp",TD_nested,rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,offsetof(Info,stamp),0,1,false,false},
    {"client_gid",TD_bytes16,rosidl_typesupport_introspection_c__ROS_TYPE_UINT8,offsetof(Info,client_gid),16,none,true,false},
    {"sequence_number",TD_i64,rosidl_typesupport_introspection_c__ROS_TYPE_INT64,offsetof(Info,sequence_number),0,none,false,false}},1u<<1},
 {"std_srvs/srv/Empty_Request","std_srvs__srv","Empty_Request",sizeof(EmptyRequest),1,
  {{"structure_needs_at_least_one_member",TD_u8,rosidl_typesupport_introspection_c__ROS_TYPE_UINT8,offsetof(EmptyRequest,structure_needs_at_least_one_member),0,none,false,false}},0},
 {"std_srvs/srv/Empty_Response","std_srvs__srv","Empty_Response",sizeof(EmptyResponse),1,
  {{"structure_needs_at_least_one_member",TD_u8,rosidl_typesupport_introspection_c__ROS_TYPE_UINT8,offsetof(EmptyResponse,structure_needs_at_least_one_member),0,none,false,false}},0},
 {"std_srvs/srv/Empty_Event","std_srvs__srv","Empty_Event",sizeof(EmptyEvent),3,
  {{"info",TD_nested,rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,offsetof(EmptyEvent,info),0,2,false,false},
    {"request",TD_bounded,rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,offsetof(EmptyEvent,request),1,3,true,true},
    {"response",TD_bounded,rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,offsetof(EmptyEvent,response),1,4,true,true}},(1u<<1)|(1u<<2)|(1u<<3)|(1u<<4)},
 {"example_interfaces/srv/AddTwoInts_Request","example_interfaces__srv","AddTwoInts_Request",sizeof(AddRequest),2,
  {{"a",TD_i64,rosidl_typesupport_introspection_c__ROS_TYPE_INT64,offsetof(AddRequest,a),0,none,false,false},
    {"b",TD_i64,rosidl_typesupport_introspection_c__ROS_TYPE_INT64,offsetof(AddRequest,b),0,none,false,false}},0},
 {"example_interfaces/srv/AddTwoInts_Response","example_interfaces__srv","AddTwoInts_Response",sizeof(AddResponse),1,
  {{"sum",TD_i64,rosidl_typesupport_introspection_c__ROS_TYPE_INT64,offsetof(AddResponse,sum),0,none,false,false}},0},
 {"example_interfaces/srv/AddTwoInts_Event","example_interfaces__srv","AddTwoInts_Event",sizeof(AddEvent),3,
  {{"info",TD_nested,rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,offsetof(AddEvent,info),0,2,false,false},
    {"request",TD_bounded,rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,offsetof(AddEvent,request),1,6,true,true},
    {"response",TD_bounded,rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,offsetof(AddEvent,response),1,7,true,true}},(1u<<1)|(1u<<2)|(1u<<6)|(1u<<7)},
 {"std_srvs/srv/Empty","std_srvs__srv","Empty",0,3,
  {{"request_message",TD_nested,0,0,0,3,false,false},{"response_message",TD_nested,0,0,0,4,false,false},
    {"event_message",TD_nested,0,0,0,5,false,false}},(1u<<1)|(1u<<2)|(1u<<3)|(1u<<4)|(1u<<5)},
 {"example_interfaces/srv/AddTwoInts","example_interfaces__srv","AddTwoInts",0,3,
  {{"request_message",TD_nested,0,0,0,6,false,false},{"response_message",TD_nested,0,0,0,7,false,false},
    {"event_message",TD_nested,0,0,0,8,false,false}},(1u<<1)|(1u<<2)|(1u<<6)|(1u<<7)|(1u<<8)}
};
_Static_assert(sizeof(int64_t)==8&&sizeof(uint32_t)==4,"complete selected scalar widths");
_Static_assert(service_msgs__msg__ServiceEventInfo__REQUEST_SENT==0&&service_msgs__msg__ServiceEventInfo__REQUEST_RECEIVED==1&&
 service_msgs__msg__ServiceEventInfo__RESPONSE_SENT==2&&service_msgs__msg__ServiceEventInfo__RESPONSE_RECEIVED==3,"actual event constants");
static bool text(rosidl_runtime_c__String const*v,char const*expected){
 size_t n=strlen(expected);if(v->size!=n)return false;
 /* Generated descriptions contain static views with capacity==size. Never read
  * a presumed NUL beyond this metadata's actual meaningful extent. */
 if(!v->data)return n==0&&v->capacity==0;
 return v->capacity>=n&&(!n||!memcmp(v->data,expected,n));
}
static size_t index_name(rosidl_runtime_c__String const*n){for(size_t i=0;i<identities;++i)if(text(n,specs[i].name))return i;return none;}
static bool individual(rosidl_runtime_c__type_description__IndividualTypeDescription const*v,size_t index){
 Spec const*s=specs+index;if(!text(&v->type_name,s->name)||v->fields.size!=s->count||v->fields.capacity<v->fields.size||!v->fields.data)return false;
 for(size_t i=0;i<s->count;++i){rosidl_runtime_c__type_description__Field const*f=v->fields.data+i;Field const*w=s->fields+i;
 if(!text(&f->name,w->name)||f->type.type_id!=w->description_type||f->type.capacity!=w->capacity||f->type.string_capacity||
 !text(&f->type.nested_type_name,w->nested==none?"":specs[w->nested].name)||!text(&f->default_value,""))return false;}return true;
}
static size_t population(uint16_t mask){size_t n=0;for(;mask;mask=(uint16_t)(mask&(mask-1)))++n;return n;}
static bool description(rosidl_runtime_c__type_description__TypeDescription const*v,size_t index){
 if(!v||!individual(&v->type_description,index))return false;
 rosidl_runtime_c__type_description__IndividualTypeDescription__Sequence const*r=&v->referenced_type_descriptions;
 uint16_t mask=specs[index].references,seen=0;if(r->size!=population(mask)||r->capacity<r->size||(!r->data&&r->size))return false;
 for(size_t i=0;i<r->size;++i){size_t at=index_name(&r->data[i].type_name);
 if(at==none||!(mask&(1u<<at))||(seen&(1u<<at))||!individual(r->data+i,at))return false;seen=(uint16_t)(seen|(1u<<at));}return seen==mask;
}
static bool sources(rosidl_runtime_c__type_description__TypeSource__Sequence const*v,size_t index){
 if(!v||!v->size||v->size>identities||v->capacity<v->size||!v->data)return false;
 uint16_t mask=(uint16_t)(specs[index].references|(1u<<index)),seen=0;
 for(size_t i=0;i<v->size;++i){rosidl_runtime_c__type_description__TypeSource const*s=v->data+i;size_t at=index_name(&s->type_name);
 if(at==none||!(mask&(1u<<at))||(seen&(1u<<at)))return false;seen=(uint16_t)(seen|(1u<<at));
 bool implicit=at>=3&&at<=8;if(!text(&s->encoding,implicit?"implicit":at>=messages?"srv":"msg"))return false;
 if(implicit){if(!text(&s->raw_file_contents,""))return false;}
 else if(!s->raw_file_contents.data||!s->raw_file_contents.size||s->raw_file_contents.size>4194304||s->raw_file_contents.capacity<s->raw_file_contents.size)return false;
 }return seen==mask;
}
static bool message_metadata(rosidl_message_type_support_t const*h,uwvm_rmw_c_codecs_registry const*s,size_t i){
 if(!h||!h->get_type_hash_func||!h->get_type_description_func||!h->get_type_description_sources_func)return false;
 rosidl_type_hash_t const*v=h->get_type_hash_func(h);return v&&v->version==1&&!memcmp(v->value,s->selected.hashes[i],32)&&
 description(h->get_type_description_func(h),i)&&sources(h->get_type_description_sources_func(h),i);
}
static bool service_metadata(rosidl_service_type_support_t const*h,uwvm_rmw_c_codecs_registry const*s,size_t i){
 if(!h||!h->get_type_hash_func||!h->get_type_description_func||!h->get_type_description_sources_func)return false;
 rosidl_type_hash_t const*v=h->get_type_hash_func(h);return v&&v->version==1&&!memcmp(v->value,s->selected.hashes[i],32)&&
 description(h->get_type_description_func(h),i)&&sources(h->get_type_description_sources_func(h),i);
}
static bool members(uwvm_rmw_c_codecs_registry const*s,size_t index){
 rosidl_message_type_support_t const*h=s->native[index];Spec const*spec=specs+index;
 if(!h||!h->typesupport_identifier||strcmp(h->typesupport_identifier,rosidl_typesupport_introspection_c__identifier)||!h->data||!h->func)return false;
 rosidl_typesupport_introspection_c__MessageMembers const*m=h->data;
 if(!m->message_namespace_||strcmp(m->message_namespace_,spec->ns)||!m->message_name_||strcmp(m->message_name_,spec->local)||
 m->member_count_!=spec->count||m->size_of_!=spec->size||m->has_any_key_member_||!m->members_||!m->init_function||!m->fini_function)return false;
 for(size_t i=0;i<spec->count;++i){rosidl_typesupport_introspection_c__MessageMember const*f=m->members_+i;Field const*w=spec->fields+i;
 if(!f->name_||strcmp(f->name_,w->name)||f->type_id_!=w->native_type||f->string_upper_bound_||f->is_key_||f->is_array_!=w->array||
 f->array_size_!=w->capacity||f->is_upper_bound_!=w->upper||f->offset_!=w->offset||f->default_value_||f->members_!=(w->nested==none?NULL:s->native[w->nested]))return false;
 if(w->array){if(!f->size_function||!f->get_const_function||!f->get_function||!f->fetch_function||!f->assign_function||((f->resize_function!=NULL)!=w->upper))return false;}
 else if(f->size_function||f->get_const_function||f->get_function||f->fetch_function||f->assign_function||f->resize_function)return false;
 }return true;
}
static bool pair(rosidl_message_type_support_t const*v,rosidl_message_type_support_t const*a,rosidl_message_type_support_t const*b){return v==a||v==b;}
static bool whole_graph(uwvm_rmw_c_codecs_registry const*s){
 /* Both generic and introspection Service handles must point at the ACTUAL
  * generated C event constructors/destructors, not just arbitrary nonnull code. */
 static rosidl_event_message_create_handle_function_function const create_event[2]={
  ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_CREATE_EVENT_MESSAGE_SYMBOL_NAME(rosidl_typesupport_c,std_srvs,srv,Empty),
  ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_CREATE_EVENT_MESSAGE_SYMBOL_NAME(rosidl_typesupport_c,example_interfaces,srv,AddTwoInts)};
 static rosidl_event_message_destroy_handle_function_function const destroy_event[2]={
  ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_DESTROY_EVENT_MESSAGE_SYMBOL_NAME(rosidl_typesupport_c,std_srvs,srv,Empty),
  ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_DESTROY_EVENT_MESSAGE_SYMBOL_NAME(rosidl_typesupport_c,example_interfaces,srv,AddTwoInts)};
 for(size_t i=0;i<messages;++i)if(!members(s,i)||!message_metadata(s->generic[i],s,i)||!message_metadata(s->native[i],s,i))return false;
 for(size_t i=0;i<2;++i){size_t req=i?6:3,resp=i?7:4,event=i?8:5;Spec const*spec=specs+9+i;
 rosidl_service_type_support_t const*h=s->native_services[i],*g=s->services[i];
 if(!h||!h->typesupport_identifier||strcmp(h->typesupport_identifier,rosidl_typesupport_introspection_c__identifier)||!h->data||!h->func||!g)return false;
 rosidl_typesupport_introspection_c__ServiceMembers const*m=h->data;
 if(!pair(g->request_typesupport,s->generic[req],s->native[req])||!pair(g->response_typesupport,s->generic[resp],s->native[resp])||
 !pair(g->event_typesupport,s->generic[event],s->native[event])||!g->event_message_create_handle_function||!g->event_message_destroy_handle_function||
 !m->service_namespace_||strcmp(m->service_namespace_,spec->ns)||!m->service_name_||strcmp(m->service_name_,spec->local)||
 m->request_members_!=s->native[req]->data||m->response_members_!=s->native[resp]->data||m->event_members_!=s->native[event]->data||
 h->request_typesupport!=s->native[req]||h->response_typesupport!=s->native[resp]||h->event_typesupport!=s->native[event]||
 h->event_message_create_handle_function!=create_event[i]||h->event_message_destroy_handle_function!=destroy_event[i]||
 g->event_message_create_handle_function!=create_event[i]||g->event_message_destroy_handle_function!=destroy_event[i]||!service_metadata(g,s,9+i)||!service_metadata(h,s,9+i))return false;
 }return true;
}
static uint32_t validate(uwvm_rmw_codec const*c,void const*supplied){
 if(!c||!c->validation_state)return 22;uwvm_rmw_c_codecs_registry const*s=c->validation_state;
 if(s->self!=s||!s->ready)return 22;size_t slot=descriptors;for(size_t i=0;i<descriptors;++i)if(c==s->codecs+i){slot=i;break;}if(slot==descriptors)return 22;
 static size_t const map[7]={0,3,4,9,6,7,10};size_t id=map[slot];bool root=id>=messages;
 void const*expected=root?(void const*)s->services[id-messages]:s->generic[id];
 void const*direct=root?(void const*)s->native_services[id-messages]:s->native[id];
 size_t bound=slot==0?s->string_bound:(slot==1||slot==2||slot==3)?0:slot==5?8:16;
 if(c->version!=1||c->kind!=(root?UWVM_RMW_SERVICE_CODEC:UWVM_RMW_MESSAGE_CODEC)||!c->native_type||strcmp(c->native_type,specs[id].name)||
 c->generated_support!=expected||(supplied!=expected&&supplied!=direct)||memcmp(c->native_rihs01,s->selected.hashes[id],32)||c->maximum_wire_bytes!=bound)return 22;
 if(root){size_t rq=slot==3?1:4,rs=slot==3?2:5;if(c->encode||c->decode||c->request!=s->codecs+rq||c->response!=s->codecs+rs)return 22;}
 else if(!c->encode||!c->decode||c->request||c->response)return 22;
 return whole_graph(s)?0:22;
}
static bool initialized_string(rosidl_runtime_c__String const*s){return s&&s->data&&s->size<s->capacity&&s->data[s->size]=='\0';}
static uint32_t string_encode(void const*object,void*bytes,size_t cap,size_t*length){
 if(!object||!length||(cap&&!bytes))return 22;String const*s=object;if(!initialized_string(&s->data))return 22;
 if(s->data.size>UWVM_RMW_MAX_CANONICAL_BYTES-4||s->data.size+4>cap)return 90;
 return roswasmc_encode_string((roswasmc_string){s->data.data,s->data.size},bytes,cap,length);
}
static uint32_t string_decode(void const*bytes,size_t length,void*object){
 if(!object||!bytes||length>UWVM_RMW_MAX_CANONICAL_BYTES)return 22;String*s=object;if(!initialized_string(&s->data))return 22;
 roswasmc_string text;uint32_t code=roswasmc_decode_string(bytes,length,&text);if(code)return code;
 /* Decode transactionally into an ORIGINAL rosidl String. Unlike assigning
  * directly into destination, this also supports input aliasing its old data:
  * reallocation cannot invalidate the canonical input while it is copied. */
 rosidl_runtime_c__String next={0};if(!rosidl_runtime_c__String__init(&next))return 12;
 if(!rosidl_runtime_c__String__assignn(&next,text.data,text.size)){rosidl_runtime_c__String__fini(&next);return 12;}
 rosidl_runtime_c__String old=s->data;s->data=next;rosidl_runtime_c__String__fini(&old);return 0;
}
static uint64_t get64(unsigned char const*p){uint64_t v=0;for(unsigned i=0;i<8;++i)v|=(uint64_t)p[i]<<(8*i);return v;}
static void put64(unsigned char*p,int64_t n){uint64_t v;memcpy(&v,&n,8);for(unsigned i=0;i<8;++i)p[i]=(unsigned char)(v>>(8*i));}
static int64_t signed64(unsigned char const*p){uint64_t v=get64(p);int64_t n;memcpy(&n,&v,8);return n;}
static uint32_t request_encode(void const*object,void*bytes,size_t cap,size_t*length){if(!object||!length||(cap&&!bytes))return 22;if(cap<16)return 90;AddRequest const*v=object;put64(bytes,v->a);put64((unsigned char*)bytes+8,v->b);*length=16;return 0;}
static uint32_t request_decode(void const*bytes,size_t length,void*object){if(!bytes||!object||length!=16)return 22;AddRequest next={0};next.a=signed64(bytes);next.b=signed64((unsigned char const*)bytes+8);*(AddRequest*)object=next;return 0;}
static uint32_t response_encode(void const*object,void*bytes,size_t cap,size_t*length){if(!object||!length||(cap&&!bytes))return 22;if(cap<8)return 90;put64(bytes,((AddResponse const*)object)->sum);*length=8;return 0;}
static uint32_t response_decode(void const*bytes,size_t length,void*object){if(!bytes||!object||length!=8)return 22;((AddResponse*)object)->sum=signed64(bytes);return 0;}
static uint32_t empty_request_encode(void const*object,void*bytes,size_t cap,size_t*length){(void)bytes;if(!object||!length||cap||((EmptyRequest const*)object)->structure_needs_at_least_one_member)return 22;*length=0;return 0;}
static uint32_t empty_response_encode(void const*object,void*bytes,size_t cap,size_t*length){(void)bytes;if(!object||!length||cap||((EmptyResponse const*)object)->structure_needs_at_least_one_member)return 22;*length=0;return 0;}
static uint32_t empty_request_decode(void const*bytes,size_t length,void*object){(void)bytes;if(!object||length)return 22;((EmptyRequest*)object)->structure_needs_at_least_one_member=0;return 0;}
static uint32_t empty_response_decode(void const*bytes,size_t length,void*object){(void)bytes;if(!object||length)return 22;((EmptyResponse*)object)->structure_needs_at_least_one_member=0;return 0;}
static bool warm_message(uwvm_rmw_c_codecs_registry*s,size_t i,rosidl_message_type_support_t const*g){
 if(!g||!g->func)return false;rosidl_message_type_support_t const*h=g->func(g,rosidl_typesupport_introspection_c__identifier);if(!h)return false;s->generic[i]=g;s->native[i]=h;return true;
}
static bool warm_service(uwvm_rmw_c_codecs_registry*s,size_t i,rosidl_service_type_support_t const*g){
 if(!g||!g->func)return false;rosidl_service_type_support_t const*h=g->func(g,rosidl_typesupport_introspection_c__identifier);if(!h)return false;s->services[i]=g;s->native_services[i]=h;return true;
}
static uint32_t initialize(uwvm_rmw_c_codecs_registry*s){
#define MSG(i,p,k,n) warm_message(s,i,ROSIDL_GET_MSG_TYPE_SUPPORT(p,k,n))
#define SRV(i,p,k,n) warm_service(s,i,ROSIDL_GET_SRV_TYPE_SUPPORT(p,k,n))
 if(!MSG(0,std_msgs,msg,String)||!MSG(1,builtin_interfaces,msg,Time)||!MSG(2,service_msgs,msg,ServiceEventInfo)||
 !MSG(3,std_srvs,srv,Empty_Request)||!MSG(4,std_srvs,srv,Empty_Response)||!MSG(5,std_srvs,srv,Empty_Event)||
 !MSG(6,example_interfaces,srv,AddTwoInts_Request)||!MSG(7,example_interfaces,srv,AddTwoInts_Response)||!MSG(8,example_interfaces,srv,AddTwoInts_Event)||
 !SRV(0,std_srvs,srv,Empty)||!SRV(1,example_interfaces,srv,AddTwoInts))return 95;
#undef MSG
#undef SRV
 if(!whole_graph(s))return 22;
 static size_t const map[7]={0,3,4,9,6,7,10};
 for(size_t i=0;i<descriptors;++i){uwvm_rmw_codec*c=s->codecs+i;size_t id=map[i];c->version=1;c->kind=id<messages?UWVM_RMW_MESSAGE_CODEC:UWVM_RMW_SERVICE_CODEC;
 c->native_type=specs[id].name;c->generated_support=id<messages?(void const*)s->generic[id]:s->services[id-messages];
 memcpy(c->native_rihs01,s->selected.hashes[id],32);c->maximum_wire_bytes=i==0?s->string_bound:(i==1||i==2||i==3)?0:i==5?8:16;c->validate=validate;c->validation_state=s;}
 s->codecs[0].encode=string_encode;s->codecs[0].decode=string_decode;
 s->codecs[1].encode=empty_request_encode;s->codecs[1].decode=empty_request_decode;s->codecs[2].encode=empty_response_encode;s->codecs[2].decode=empty_response_decode;
 s->codecs[3].request=s->codecs+1;s->codecs[3].response=s->codecs+2;s->codecs[4].encode=request_encode;s->codecs[4].decode=request_decode;
 s->codecs[5].encode=response_encode;s->codecs[5].decode=response_decode;s->codecs[6].request=s->codecs+4;s->codecs[6].response=s->codecs+5;s->ready=true;
 for(size_t i=0;i<descriptors;++i)if(validate(s->codecs+i,s->codecs[i].generated_support)){s->ready=false;return 22;}return 0;
}
#endif
uint32_t uwvm_rmw_c_codecs_create(uwvm_rmw_c_native_identities const*ids,size_t bound,rcutils_allocator_t const*a,uwvm_rmw_c_codecs_registry**output){
 if(!ids||!a||!output||!rcutils_allocator_is_valid(a)||bound<4||bound>UWVM_RMW_MAX_CANONICAL_BYTES)return 22;
 for(size_t i=0;i<UWVM_RMW_C_ID_COUNT;++i){unsigned nonzero=0;for(size_t j=0;j<32;++j)nonzero|=ids->hashes[i][j];if(!nonzero)return 22;}
#if !UWVM_RMW_C_CODECS_COMPLETE_API
 return 95;
#else
 uwvm_rmw_c_codecs_registry*s=a->zero_allocate(1,sizeof*s,a->state);if(!s)return 12;s->allocator=*a;s->selected=*ids;s->string_bound=bound;s->self=s;
 uint32_t code=initialize(s);if(code){a->deallocate(s,a->state);return code;}*output=s;return 0;
#endif
}
static bool valid(uwvm_rmw_c_codecs_registry const*s){return s&&s->self==s&&s->ready;}
uint32_t uwvm_rmw_c_codecs_export(uwvm_rmw_c_codecs_registry*s,uwvm_rmw_codec const**output,size_t*count){
 if(!valid(s)||!output||!count)return 22;s->exported=true;*output=s->codecs;*count=UWVM_RMW_C_CODEC_COUNT;return 0;
}
uint32_t uwvm_rmw_c_codecs_get(uwvm_rmw_c_codecs_registry*s,enum uwvm_rmw_c_codec_index i,uwvm_rmw_codec const**output){
 if(!valid(s)||!output||(unsigned)i>=UWVM_RMW_C_CODEC_COUNT)return 22;s->exported=true;*output=s->codecs+i;return 0;
}
uint32_t uwvm_rmw_c_codecs_observe(uwvm_rmw_c_codecs_registry const*s,bool*exported,size_t*bound){
 if(!valid(s)||!exported||!bound)return 22;*exported=s->exported;*bound=s->string_bound;return 0;
}
uint32_t uwvm_rmw_c_codecs_destroy_unexported(uwvm_rmw_c_codecs_registry**p){
 if(!p||!valid(*p))return 22;uwvm_rmw_c_codecs_registry*s=*p;if(s->exported)return 17;
 rcutils_allocator_t a=s->allocator;s->ready=false;s->self=NULL;a.deallocate(s,a.state);*p=NULL;return 0;
}
