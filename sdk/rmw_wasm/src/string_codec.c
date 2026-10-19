/* SPDX-License-Identifier: Apache-2.0
 * First complete registry entry. These are the ORIGINAL generated std_msgs C
 * object and actual introspection types, never a hand-written substitute. */
#include "uwvm_rmw/codec.h"
#include "uwvm_rmw/contracts.h"
#include <string.h>
#include <stddef.h>
#include <std_msgs/msg/detail/string__struct.h>
#include <rosidl_runtime_c/string_functions.h>
#include <rosidl_runtime_c/type_hash.h>
#include <rosidl_typesupport_introspection_c/message_introspection.h>
#include <rosidl_typesupport_introspection_c/field_types.h>
#include <rosidl_typesupport_introspection_c/identifier.h>
#include <roswasm/codec.h>
static uint32_t validate(uwvm_rmw_codec const * c,void const * supplied) {
  rosidl_message_type_support_t const * s=(rosidl_message_type_support_t const *)supplied;
  if(!c||c->version!=1||c->kind!=UWVM_RMW_MESSAGE_CODEC||!c->native_type||
    strcmp(c->native_type,"std_msgs/msg/String")||c->maximum_wire_bytes<4||
    c->maximum_wire_bytes>UWVM_RMW_MAX_CANONICAL_BYTES||
    !s||!s->func||!s->get_type_hash_func||!s->get_type_description_func||!s->get_type_description_sources_func)return UWVM_ROS_INVALID;
  rosidl_type_hash_t const * hash=s->get_type_hash_func(s);
  if(!hash||hash->version!=1||memcmp(hash->value,c->native_rihs01,32)||
    !s->get_type_description_func(s)||!s->get_type_description_sources_func(s))return UWVM_ROS_INVALID;
  rosidl_message_type_support_t const * ts=s->func(s,rosidl_typesupport_introspection_c__identifier);
  if(!ts||!ts->typesupport_identifier||strcmp(ts->typesupport_identifier,rosidl_typesupport_introspection_c__identifier)||!ts->data)return UWVM_ROS_INVALID;
  rosidl_typesupport_introspection_c__MessageMembers const * m=(rosidl_typesupport_introspection_c__MessageMembers const *)ts->data;
  if(!m->message_namespace_||!m->message_name_||strcmp(m->message_namespace_,"std_msgs__msg")||strcmp(m->message_name_,"String")||
    m->member_count_!=1||m->size_of_!=sizeof(std_msgs__msg__String)||m->has_any_key_member_||!m->members_||!m->init_function||!m->fini_function)return UWVM_ROS_INVALID;
  rosidl_typesupport_introspection_c__MessageMember const * f=m->members_;
  if(!f->name_||strcmp(f->name_,"data")||f->type_id_!=rosidl_typesupport_introspection_c__ROS_TYPE_STRING||
    f->string_upper_bound_||f->members_||f->is_key_||f->is_array_||f->array_size_||f->is_upper_bound_||
    f->offset_!=offsetof(std_msgs__msg__String,data)||f->default_value_||f->size_function||f->get_const_function||
    f->get_function||f->fetch_function||f->assign_function||f->resize_function)return UWVM_ROS_INVALID;
  return 0;
}
static uint32_t encode(void const * object,void * bytes,size_t capacity,size_t * length) {
  std_msgs__msg__String const * s=(std_msgs__msg__String const *)object;
  if(!s||!s->data.data||s->data.size>=s->data.capacity||s->data.data[s->data.size]!='\0')return UWVM_ROS_INVALID;
  return roswasmc_encode_string((roswasmc_string){s->data.data,s->data.size},bytes,capacity,length);
}
static uint32_t decode(void const * bytes,size_t length,void * object) {
  std_msgs__msg__String * s=(std_msgs__msg__String *)object;roswasmc_string text;
  if(!s||!s->data.data||s->data.size>=s->data.capacity||s->data.data[s->data.size]!='\0')return UWVM_ROS_INVALID;
  uint32_t code=roswasmc_decode_string(bytes,length,&text);if(code)return code;
  /* Actual rosidl assignn preserves the old allocation on allocation failure.
   * Full UTF8/extent validation happens before this native-object mutation. */
  return rosidl_runtime_c__String__assignn(&s->data,text.data,text.size)?0:12;
}
uint32_t uwvm_rmw_string_codec_init(uwvm_rmw_codec * output,
  rosidl_message_type_support_t const * support,uint8_t const identity[32],size_t bound) {
  if(!output||!support||!identity||bound<4||bound>UWVM_RMW_MAX_CANONICAL_BYTES)return UWVM_ROS_INVALID;
  unsigned nonzero=0;for(size_t i=0;i<32;++i)nonzero|=identity[i];if(!nonzero)return UWVM_ROS_INVALID;
  uwvm_rmw_codec next={1,UWVM_RMW_MESSAGE_CODEC,"std_msgs/msg/String",support,{0},bound,validate,encode,decode,NULL,NULL};
  memcpy(next.native_rihs01,identity,32);uint32_t code=validate(&next,support);if(code)return code;*output=next;return 0;
}
