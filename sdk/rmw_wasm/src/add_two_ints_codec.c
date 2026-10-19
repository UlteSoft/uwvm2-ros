/* SPDX-License-Identifier: Apache-2.0
 * Concrete complete-object adapter for the ORIGINAL generated AddTwoInts C
 * Request/Response and complete modern Service support. No replacement native
 * struct, RIHS computation or payload-only service identity is supplied here.
 * Expected identities MUST come from independent actual selected SDK export.
 */
#include <uwvm_rmw_c/provider.h>
#include <example_interfaces/srv/add_two_ints.h>
#include <example_interfaces/srv/detail/add_two_ints__struct.h>
#include <rosidl_typesupport_introspection_c/identifier.h>
#include <rosidl_typesupport_introspection_c/message_introspection.h>
#include <rosidl_typesupport_introspection_c/service_introspection.h>
#include <rosidl_typesupport_introspection_c/field_types.h>
#include <rosidl_runtime_c/type_hash.h>
#include <stddef.h>
#include <string.h>
static int hash_equal(rosidl_type_hash_t const*h,uint8_t const*expected){
 return h&&h->version==1&&!memcmp(h->value,expected,32);
}
static rosidl_typesupport_introspection_c__MessageMembers const*members(rosidl_message_type_support_t const*s){
 if(!s||!s->func||!s->get_type_hash_func||!s->get_type_description_func||!s->get_type_description_sources_func||
  !s->get_type_description_func(s)||!s->get_type_description_sources_func(s))return NULL;
 rosidl_message_type_support_t const*t=s->func(s,rosidl_typesupport_introspection_c__identifier);
 if(!t||!t->typesupport_identifier||strcmp(t->typesupport_identifier,rosidl_typesupport_introspection_c__identifier)||!t->data)return NULL;
 return t->data;
}
static int scalar(rosidl_typesupport_introspection_c__MessageMember const*f,char const*name,size_t offset){
 return f&&f->name_&&!strcmp(f->name_,name)&&f->type_id_==rosidl_typesupport_introspection_c__ROS_TYPE_INT64&&
  !f->string_upper_bound_&&!f->members_&&!f->is_key_&&!f->is_array_&&!f->array_size_&&!f->is_upper_bound_&&
  f->offset_==offset&&!f->default_value_&&!f->size_function&&!f->get_const_function&&!f->get_function&&
  !f->fetch_function&&!f->assign_function&&!f->resize_function;
}
static uint32_t validate_message(uwvm_rmw_codec const*c,void const*supplied){
 rosidl_message_type_support_t const*s=supplied;
 if(!c||!s||!s->get_type_hash_func||!hash_equal(s->get_type_hash_func(s),c->native_rihs01))return 22;
 rosidl_typesupport_introspection_c__MessageMembers const*m=members(s);
 if(!m||!m->message_namespace_||strcmp(m->message_namespace_,"example_interfaces__srv")||!m->message_name_||
  !m->members_||!m->init_function||!m->fini_function||m->has_any_key_member_)return 22;
 if(!strcmp(c->native_type,"example_interfaces/srv/AddTwoInts_Request")){
  if(strcmp(m->message_name_,"AddTwoInts_Request")||m->member_count_!=2||m->size_of_!=sizeof(example_interfaces__srv__AddTwoInts_Request)||
   !scalar(m->members_,"a",offsetof(example_interfaces__srv__AddTwoInts_Request,a))||
   !scalar(m->members_+1,"b",offsetof(example_interfaces__srv__AddTwoInts_Request,b)))return 22;
 }else if(!strcmp(c->native_type,"example_interfaces/srv/AddTwoInts_Response")){
  if(strcmp(m->message_name_,"AddTwoInts_Response")||m->member_count_!=1||m->size_of_!=sizeof(example_interfaces__srv__AddTwoInts_Response)||
   !scalar(m->members_,"sum",offsetof(example_interfaces__srv__AddTwoInts_Response,sum)))return 22;
 }else return 22;
 return 0;
}
static uint32_t validate_service(uwvm_rmw_codec const*c,void const*supplied){
 rosidl_service_type_support_t const*s=supplied;
 if(!c||!s||!s->func||!s->request_typesupport||!s->response_typesupport||!s->event_typesupport||
  !s->event_message_create_handle_function||!s->event_message_destroy_handle_function||
  !s->get_type_hash_func||!s->get_type_description_func||!s->get_type_description_sources_func||
  !hash_equal(s->get_type_hash_func(s),c->native_rihs01)||!s->get_type_description_func(s)||!s->get_type_description_sources_func(s)||
  c->request->validate(c->request,s->request_typesupport)||c->response->validate(c->response,s->response_typesupport))return 22;
 rosidl_service_type_support_t const*t=s->func(s,rosidl_typesupport_introspection_c__identifier);
 if(!t||!t->typesupport_identifier||strcmp(t->typesupport_identifier,rosidl_typesupport_introspection_c__identifier)||!t->data)return 22;
 rosidl_typesupport_introspection_c__ServiceMembers const*m=t->data;
 rosidl_typesupport_introspection_c__MessageMembers const*r=members(s->request_typesupport),*v=members(s->response_typesupport),*event=members(s->event_typesupport);
 if(!m->service_namespace_||strcmp(m->service_namespace_,"example_interfaces__srv")||!m->service_name_||strcmp(m->service_name_,"AddTwoInts")||
  !r||!v||!event||m->request_members_!=r||m->response_members_!=v||m->event_members_!=event||
  !event->message_namespace_||strcmp(event->message_namespace_,"example_interfaces__srv")||
  !event->message_name_||strcmp(event->message_name_,"AddTwoInts_Event")||event->member_count_!=3||!event->members_||
  !event->init_function||!event->fini_function||event->has_any_key_member_)return 22;
 /* The independently exported Service RIHS above covers the complete actual
  * Request/Response/Event/dependency graph. No request/response-only substitute
  * for the Service root or legacy automatic downgrade is accepted. */
 return 0;
}
static void write64(unsigned char*p,int64_t v){uint64_t u;memcpy(&u,&v,8);for(unsigned i=0;i<8;++i)p[i]=(unsigned char)(u>>(8*i));}
static int64_t read64(unsigned char const*p){uint64_t u=0;int64_t v;for(unsigned i=0;i<8;++i)u|=(uint64_t)p[i]<<(8*i);memcpy(&v,&u,8);return v;}
static uint32_t request_encode(void const*object,void*bytes,size_t capacity,size_t*length){
 if(!object||!length||(capacity&&!bytes))return 22;if(capacity<16)return 90;
 example_interfaces__srv__AddTwoInts_Request const*r=object;unsigned char*p=bytes;write64(p,r->a);write64(p+8,r->b);*length=16;return 0;
}
static uint32_t request_decode(void const*bytes,size_t length,void*object){
 if(!bytes||!object||length!=16)return 22;unsigned char const*p=bytes;
 example_interfaces__srv__AddTwoInts_Request next={0};next.a=read64(p);next.b=read64(p+8);*(example_interfaces__srv__AddTwoInts_Request*)object=next;return 0;
}
static uint32_t response_encode(void const*object,void*bytes,size_t capacity,size_t*length){
 if(!object||!length||(capacity&&!bytes))return 22;if(capacity<8)return 90;
 write64(bytes,((example_interfaces__srv__AddTwoInts_Response const*)object)->sum);*length=8;return 0;
}
static uint32_t response_decode(void const*bytes,size_t length,void*object){
 if(!bytes||!object||length!=8)return 22;example_interfaces__srv__AddTwoInts_Response next={0};next.sum=read64(bytes);*(example_interfaces__srv__AddTwoInts_Response*)object=next;return 0;
}
uint32_t uwvm_rmw_c_add_two_ints_codecs_init(uwvm_rmw_codec output[3],uint8_t const expected[3][32]){
 if(!output||!expected)return 22;
 for(size_t i=0;i<3;++i){unsigned nonzero=0;for(size_t j=0;j<32;++j)nonzero|=expected[i][j];if(!nonzero)return 22;}
 rosidl_message_type_support_t const*request=ROSIDL_GET_MSG_TYPE_SUPPORT(example_interfaces,srv,AddTwoInts_Request);
 rosidl_message_type_support_t const*response=ROSIDL_GET_MSG_TYPE_SUPPORT(example_interfaces,srv,AddTwoInts_Response);
 rosidl_service_type_support_t const*root=ROSIDL_GET_SRV_TYPE_SUPPORT(example_interfaces,srv,AddTwoInts);
 uwvm_rmw_codec next[3]={
  {1,UWVM_RMW_MESSAGE_CODEC,"example_interfaces/srv/AddTwoInts_Request",request,{0},16,validate_message,request_encode,request_decode,NULL,NULL},
  {1,UWVM_RMW_MESSAGE_CODEC,"example_interfaces/srv/AddTwoInts_Response",response,{0},8,validate_message,response_encode,response_decode,NULL,NULL},
  {1,UWVM_RMW_SERVICE_CODEC,"example_interfaces/srv/AddTwoInts",root,{0},16,validate_service,NULL,NULL,NULL,NULL}};
 for(size_t i=0;i<3;++i)memcpy(next[i].native_rihs01,expected[i],32);next[2].request=next;next[2].response=next+1;
 for(size_t i=0;i<3;++i){uint32_t raw=next[i].validate(next+i,next[i].generated_support);if(raw)return raw;}
 /* Copy only after the COMPLETE graph is checked. Fix component pointers to
  * caller-owned stable storage, never the temporary stack registry. */
 memcpy(output,next,sizeof next);output[2].request=output;output[2].response=output+1;return 0;
}
