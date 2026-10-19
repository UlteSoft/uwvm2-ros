/* SPDX-License-Identifier: Apache-2.0
 * Actual rmw_node_t objects with mandatory real local graph guards. Node labels
 * are local middleware identity, never a grant for arbitrary native endpoints. */
#include "uwvm_rmw/internal.h"
#include <string.h>
#include <rmw/error_handling.h>
rmw_node_t * rmw_create_node(rmw_context_t * handle,char const * name,char const * namespace_) {
 UWVM_RMW_ENTRY(NULL);
  uwvm_rmw_context * c=uwvm_rmw_context_find(handle,0);
  if(!c||!name||!namespace_){uwvm_rmw_error(c,22,"invalid owned RMW node context/name");return NULL;}
  uwvm_rmw_scope scope;if(uwvm_rmw_scope_enter(c,&scope,0)!=RMW_RET_OK)return NULL;
  size_t selected=0;while(selected<c->profile->node_count&&
    (strcmp(name,c->profile->nodes[selected].name)||strcmp(namespace_,c->profile->nodes[selected].namespace_)))++selected;
  if(selected==c->profile->node_count) {
    uwvm_rmw_error(c,13,"Node is absent from the explicit RMW selection (raw13)");goto fail;
  }
  if(c->node_count==UWVM_RMW_MAX_NODES) {
    uwvm_rmw_error(c,90,"RMW node lifetime capacity exhausted (raw90)");goto fail;
  }
  uwvm_rmw_graph_guard_provider const * p=&c->profile->graph;
  if(p->version!=1||!p->create||!p->destroy||!p->notify) {
    uwvm_rmw_error(c,95,"No genuine Guest-local graph guard provider (raw95)");goto fail;
  }
  /* The stable record is reserved before the provider may allocate anything.
   * The provider promises no unreported partial guard when it returns NULL. */
  uwvm_rmw_node * n=c->nodes+c->node_count++;*n=(uwvm_rmw_node){0};n->context=c;n->selection_index=selected;
  n->public_handle.implementation_identifier=uwvm_rmw_identifier;n->public_handle.data=n;
  n->public_handle.name=c->profile->nodes[selected].name;
  n->public_handle.namespace_=c->profile->nodes[selected].namespace_;n->public_handle.context=handle;
  n->graph=p->create(handle,p->state);
  if(!n->graph){uwvm_rmw_error(c,95,"Guest-local graph guard creation failed");goto fail;}
  if(n->graph->context!=handle||n->graph->implementation_identifier!=uwvm_rmw_identifier) {
    rmw_ret_t cleanup=p->destroy(n->graph,p->state);n->graph=NULL;
    c->diagnostics.graph_close_attempted[c->node_count-1]=1;
    c->diagnostics.graph_close_codes[c->node_count-1]=(uint32_t)cleanup;
    uwvm_rmw_error(c,22,"Graph provider returned a foreign guard");goto fail;
  }
  n->live=1;++c->live_nodes;
  if(p->notify(n->graph,p->state)!=RMW_RET_OK) {
    rmw_ret_t cleanup=p->destroy(n->graph,p->state);n->graph=NULL;n->live=0;--c->live_nodes;
    c->diagnostics.graph_close_attempted[c->node_count-1]=1;
    c->diagnostics.graph_close_codes[c->node_count-1]=(uint32_t)cleanup;
    uwvm_rmw_error(c,5,"Local graph notification failed during node setup");goto fail;
  }
  uwvm_rmw_scope_leave(&scope);return &n->public_handle;
fail:uwvm_rmw_scope_leave(&scope);return NULL;
}
rmw_ret_t rmw_destroy_node(rmw_node_t * handle) {
 UWVM_RMW_ENTRY(RMW_RET_ERROR);
  uwvm_rmw_node * n=uwvm_rmw_node_find(handle,1);
  if(!n){RMW_SET_ERROR_MSG("foreign/retired RMW node");return RMW_RET_INCORRECT_RMW_IMPLEMENTATION;}
  uwvm_rmw_context * c=n->context;uwvm_rmw_scope scope;
  rmw_ret_t code=uwvm_rmw_scope_enter(c,&scope,1);if(code!=RMW_RET_OK)return code;
  if(n->resource_count) {
    uwvm_rmw_scope_leave(&scope);return uwvm_rmw_error(c,17,"RMW node still owns live endpoint objects (raw17)");
  }
  /* Once logical preconditions pass, teardown retires the node even if guard
   * cleanup fails. No callback/close attempt is replayed. These graph slots hold
   * actual RMW return codes, whereas native owner slots hold raw ABI errno. */
  n->live=0;--c->live_nodes;size_t index=(size_t)(n-c->nodes);
  rmw_ret_t cleanup=c->profile->graph.destroy(n->graph,c->profile->graph.state);n->graph=NULL;
  c->diagnostics.graph_close_attempted[index]=1;c->diagnostics.graph_close_codes[index]=(uint32_t)cleanup;
  uwvm_rmw_scope_leave(&scope);
  if(cleanup!=RMW_RET_OK)RMW_SET_ERROR_MSG("RMW node graph guard cleanup failed; outcome retained");
  return cleanup;
}
rmw_guard_condition_t const * rmw_node_get_graph_guard_condition(rmw_node_t const * handle) {
 UWVM_RMW_ENTRY(NULL);
  uwvm_rmw_node * n=uwvm_rmw_node_find(handle,0);
  if(!n){RMW_SET_ERROR_MSG("invalid owned RMW node graph guard");return NULL;}return n->graph;
}
rmw_ret_t uwvm_rmw_notify_local_graph(uwvm_rmw_node * n) {
  if(!n||!n->live||!n->graph)return RMW_RET_INVALID_ARGUMENT;
  return n->context->profile->graph.notify(n->graph,n->context->profile->graph.state);
}
