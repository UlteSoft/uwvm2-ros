/* SPDX-License-Identifier: Apache-2.0 */
#include "uwvm_rmw/internal.h"
#include <string.h>
#include <rmw/validate_full_topic_name.h>
#include <rmw/validate_node_name.h>
#include <rmw/validate_namespace.h>
#include <rmw/error_handling.h>
#include <rmw/time.h>
#include <roswasm/codec.h>
/* B owns these two counters together with context.c. No profile swap is allowed
 * while any options/context can retain the old immutable borrowed selection. */
uwvm_rmw_profile const * uwvm_rmw_selected_profile;
size_t uwvm_rmw_live_options,uwvm_rmw_live_contexts;
static int text(char const * value) {
  size_t n=0;if(!value)return 0;
  while(n<=UWVM_RMW_MAX_NAME_BYTES&&value[n])++n;
  return n&&n<=UWVM_RMW_MAX_NAME_BYTES&&roswasmc_utf8((roswasmc_string){value,n},0);
}
int uwvm_rmw_qos_equal(rmw_qos_profile_t const * a,rmw_qos_profile_t const * b) {
  return a&&b&&a->history==b->history&&a->depth==b->depth&&
    a->reliability==b->reliability&&a->durability==b->durability&&
    rmw_time_equal(a->deadline,b->deadline)&&
    rmw_time_equal(a->lifespan,b->lifespan)&&a->liveliness==b->liveliness&&
    rmw_time_equal(a->liveliness_lease_duration,b->liveliness_lease_duration)&&
    a->avoid_ros_namespace_conventions==b->avoid_ros_namespace_conventions;
}
static int valid_qos(rmw_qos_profile_t const * q) {
  /* Unknown enums are not QoS wildcards. Original rmw_time_t permits
   * nonnormalized durations and saturating infinite representations; compare
   * them through the actual linked rmw_time_equal, never a local redefinition.
   * SYSTEM_DEFAULT requests match explicit rows; effective policies must be
   * genuine separately queried concrete values. No duration normalization is
   * imposed here and no native/Host limit or authority is relaxed. */
  return q->history>=RMW_QOS_POLICY_HISTORY_SYSTEM_DEFAULT&&q->history<RMW_QOS_POLICY_HISTORY_UNKNOWN&&
    q->reliability>=RMW_QOS_POLICY_RELIABILITY_SYSTEM_DEFAULT&&q->reliability<RMW_QOS_POLICY_RELIABILITY_UNKNOWN&&
    q->durability>=RMW_QOS_POLICY_DURABILITY_SYSTEM_DEFAULT&&q->durability<RMW_QOS_POLICY_DURABILITY_UNKNOWN&&
    q->liveliness>=RMW_QOS_POLICY_LIVELINESS_SYSTEM_DEFAULT&&q->liveliness<RMW_QOS_POLICY_LIVELINESS_UNKNOWN;
}
int uwvm_rmw_qos_concrete(rmw_qos_profile_t const * q) {
  return q&&valid_qos(q)&&q->history!=RMW_QOS_POLICY_HISTORY_SYSTEM_DEFAULT&&
    (q->history!=RMW_QOS_POLICY_HISTORY_KEEP_LAST||q->depth)&&
    q->reliability!=RMW_QOS_POLICY_RELIABILITY_SYSTEM_DEFAULT&&
    q->durability!=RMW_QOS_POLICY_DURABILITY_SYSTEM_DEFAULT&&
    q->liveliness!=RMW_QOS_POLICY_LIVELINESS_SYSTEM_DEFAULT;
}
rmw_ret_t uwvm_rmw_install_profile(uwvm_rmw_profile const * p) {
 UWVM_RMW_ENTRY(RMW_RET_ERROR);
  if(uwvm_rmw_live_options||uwvm_rmw_live_contexts) {
    RMW_SET_ERROR_MSG("RMW profile is borrowed by live options/context");return RMW_RET_ERROR;
  }
  if(!p){uwvm_rmw_selected_profile=NULL;return RMW_RET_OK;}
  if(p->version!=1||!p->backend||p->backend->version!=2||!p->backend->abi_version||
    !p->backend->open||!p->backend->close||p->node_count>UWVM_RMW_MAX_NODES||
    p->endpoint_count>UWVM_RMW_RESOURCE_LIMIT||p->codec_count>UWVM_RMW_RESOURCE_LIMIT||
    (!p->nodes&&p->node_count)||(!p->endpoints&&p->endpoint_count)||(!p->codecs&&p->codec_count))goto bad;
#if UWVM_RMW_ENABLE_THREADED_PROFILE
  /* Finite first threaded profile requires the genuine registry/pin-aware
   * local graph provider. An arbitrary callback table is not a concurrency
   * attestation and cannot silently escape phase-one shutdown notification. */
  if(p->node_count&&!uwvm_rmw_c_graph_provider_matches(&p->graph)) {
    return uwvm_rmw_error(NULL,95,"threaded nodes require the actual pin-aware C graph provider");
  }
#endif
  for(size_t i=0;i<p->node_count;++i) {
    int result;size_t bad_index;
    if(!text(p->nodes[i].name)||!text(p->nodes[i].namespace_)||
      rmw_validate_node_name(p->nodes[i].name,&result,&bad_index)!=RMW_RET_OK||result!=RMW_NODE_NAME_VALID||
      rmw_validate_namespace(p->nodes[i].namespace_,&result,&bad_index)!=RMW_RET_OK||result!=RMW_NAMESPACE_VALID)goto bad;
    for(size_t j=0;j<i;++j)if(!strcmp(p->nodes[j].name,p->nodes[i].name)&&
      !strcmp(p->nodes[j].namespace_,p->nodes[i].namespace_))goto bad;
  }
  for(size_t i=0;i<p->codec_count;++i) {
    uwvm_rmw_codec const * c=p->codecs+i;unsigned nonzero=0;
    for(size_t j=0;j<32;++j)nonzero|=c->native_rihs01[j];
    if(c->version!=1||!text(c->native_type)||!c->generated_support||!nonzero||
      c->maximum_wire_bytes>UWVM_RMW_MAX_CANONICAL_BYTES||
      !c->validate||
      (c->kind!=UWVM_RMW_MESSAGE_CODEC&&c->kind!=UWVM_RMW_SERVICE_CODEC))goto bad;
    if(c->kind==UWVM_RMW_MESSAGE_CODEC) {
      if(!c->encode||!c->decode||c->request||c->response)goto bad;
    } else {
      if(c->encode||c->decode||!c->request||!c->response||c->request==c->response)goto bad;
      size_t request=0,response=0;
      while(request<p->codec_count&&p->codecs+request!=c->request)++request;
      while(response<p->codec_count&&p->codecs+response!=c->response)++response;
      if(request==p->codec_count||response==p->codec_count||
        c->request->kind!=UWVM_RMW_MESSAGE_CODEC||c->response->kind!=UWVM_RMW_MESSAGE_CODEC||
        !c->request->native_type||!c->response->native_type||
        c->maximum_wire_bytes<c->request->maximum_wire_bytes||c->maximum_wire_bytes<c->response->maximum_wire_bytes)goto bad;
      size_t n=strlen(c->native_type);
      if(strlen(c->request->native_type)!=n+8||memcmp(c->request->native_type,c->native_type,n)||strcmp(c->request->native_type+n,"_Request")||
        strlen(c->response->native_type)!=n+9||memcmp(c->response->native_type,c->native_type,n)||strcmp(c->response->native_type+n,"_Response"))goto bad;
    }
    for(size_t j=0;j<i;++j)if(!strcmp(c->native_type,p->codecs[j].native_type))goto bad;
  }
  for(size_t i=0;i<p->codec_count;++i)if(p->codecs[i].validate(p->codecs+i,p->codecs[i].generated_support))goto bad;
  for(size_t i=0;i<p->endpoint_count;++i) {
    uwvm_rmw_endpoint_selection const * e=p->endpoints+i;int result;size_t bad_index;
    size_t expected=(e->kind==UWVM_RMW_CLIENT||e->kind==UWVM_RMW_SERVICE)?2:1;
    if(e->node_index>=p->node_count||e->codec_index>=p->codec_count||
      e->kind<UWVM_RMW_PUBLISHER||e->kind>UWVM_RMW_SERVICE||
      !text(e->resolved_name)||rmw_validate_full_topic_name(e->resolved_name,&result,&bad_index)!=RMW_RET_OK||
      result!=RMW_TOPIC_VALID||e->alias_count!=expected||!valid_qos(&e->requested_qos)||!valid_qos(&e->effective_qos)||
      e->effective_qos.history==RMW_QOS_POLICY_HISTORY_SYSTEM_DEFAULT||
      e->effective_qos.reliability==RMW_QOS_POLICY_RELIABILITY_SYSTEM_DEFAULT||
      e->effective_qos.durability==RMW_QOS_POLICY_DURABILITY_SYSTEM_DEFAULT||
      e->effective_qos.liveliness==RMW_QOS_POLICY_LIVELINESS_SYSTEM_DEFAULT)goto bad;
    if((e->kind<=UWVM_RMW_SUBSCRIPTION)!=(p->codecs[e->codec_index].kind==UWVM_RMW_MESSAGE_CODEC))goto bad;
    for(size_t k=0;k<e->alias_count;++k) {
      if(!text(e->aliases[k]))goto bad;
      for(size_t j=0;j<=i;++j)for(size_t z=0;z<p->endpoints[j].alias_count;++z)
        if((j<i||z<k)&&!strcmp(e->aliases[k],p->endpoints[j].aliases[z]))goto bad;
    }
    /* Query grants are independent, explicit and globally alias-distinct.
     * They are not extra endpoints or authority inferred from service.call. */
    if(e->availability_alias) {
      if(e->kind!=UWVM_RMW_CLIENT||!text(e->availability_alias))goto bad;
      for(size_t j=0;j<=i;++j) {
        uwvm_rmw_endpoint_selection const * other=p->endpoints+j;
        for(size_t z=0;z<other->alias_count;++z)
          if(!strcmp(e->availability_alias,other->aliases[z]))goto bad;
        if(j<i&&other->availability_alias&&!strcmp(e->availability_alias,other->availability_alias))goto bad;
      }
    }
    /* A later normal alias must also not reuse an earlier query grant. */
    for(size_t j=0;j<i;++j)if(p->endpoints[j].availability_alias)
      for(size_t k=0;k<e->alias_count;++k)
        if(!strcmp(e->aliases[k],p->endpoints[j].availability_alias))goto bad;
    for(size_t j=0;j<i;++j)if(e->kind==p->endpoints[j].kind&&e->node_index==p->endpoints[j].node_index&&
      !strcmp(e->resolved_name,p->endpoints[j].resolved_name)&&
      uwvm_rmw_qos_equal(&e->requested_qos,&p->endpoints[j].requested_qos))goto bad;
  }
  uwvm_rmw_selected_profile=p;return RMW_RET_OK;
bad:RMW_SET_ERROR_MSG("invalid complete explicit RMW resource/type/QoS selection");return RMW_RET_INVALID_ARGUMENT;
}
uwvm_rmw_endpoint_selection const * uwvm_rmw_select(uwvm_rmw_node * n,
  enum uwvm_rmw_endpoint_kind kind,char const * name,rmw_qos_profile_t const * qos,void const * support) {
  if(!n||!name||!qos||!support)return NULL;
  uwvm_rmw_profile const * p=n->context->profile;
  for(size_t i=0;i<p->endpoint_count;++i) {
    uwvm_rmw_endpoint_selection const * e=p->endpoints+i;
    if(e->kind==kind&&e->node_index==n->selection_index&&!strcmp(e->resolved_name,name)&&
      uwvm_rmw_qos_equal(qos,&e->requested_qos)&&!p->codecs[e->codec_index].validate(p->codecs+e->codec_index,support))return e;
  }
  return NULL;
}
