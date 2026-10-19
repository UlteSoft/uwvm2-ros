/* Copyright (c) 2026 MacroModel contributors. SPDX-License-Identifier: Apache-2.0 */
#include "document.h"
#include "pb_decode.h"
#include <stdalign.h>
#include <string.h>
#include <limits.h>

/* Finite wire work and exact original schema identities are separate from
 * transport quotas. This module only accepts process-static generated rows. */
typedef struct allocation {
    max_align_t alignment;
    size_t size;
    uint32_t live;
} allocation;
typedef struct budget {
    unsigned char *begin;
    size_t capacity, used;
    uint32_t failed;
} budget;
static _Thread_local budget *current_budget;

static bool add(size_t a, size_t b, size_t *out) {
    if (b > SIZE_MAX-a) return false;
    *out=a+b; return true;
}
static bool aligned(size_t n, size_t *out) {
    size_t v; if(!add(n,alignof(max_align_t)-1,&v)) return false;
    *out=v-(v%alignof(max_align_t)); return true;
}
static void *reserve(budget *b,size_t n) {
    size_t start,end;
    if(!aligned(b->used,&start)||!add(start,n,&end)||end>b->capacity) {
        b->failed=90; return NULL;
    }
    b->used=end;
    return b->begin+start;
}
static allocation *owned_allocation(budget *b,void *ptr) {
    uintptr_t start=(uintptr_t)b->begin, p=(uintptr_t)ptr;
    if(p<start||p-start<sizeof(allocation)||p-start>b->used) return NULL;
    size_t offset=(size_t)(p-start)-sizeof(allocation);
    if(offset%alignof(max_align_t)!=0||offset>b->used-sizeof(allocation)) return NULL;
    allocation *a=(allocation *)(b->begin+offset);
    if(!a->live||a->size>b->used-offset-sizeof(allocation)) return NULL;
    return a;
}
void *uwvm_spot_pb_realloc(void *ptr,size_t n) {
    budget *b=current_budget;
    allocation *old=NULL;
    if(!b) return NULL;
    if(ptr) { old=owned_allocation(b,ptr); if(!old){b->failed=22;return NULL;} }
    if(!n) { if(old) old->live=0;return NULL; }
    if(old&&n<=old->size) return ptr;
    size_t total;
    if(!add(sizeof(allocation),n,&total)) {b->failed=90;return NULL;}
    allocation *next=(allocation *)reserve(b,total);
    if(!next) return NULL;
    next->size=n;next->live=1;
    void *data=next+1;
    if(old) {memcpy(data,ptr,old->size);old->live=0;}
    return data;
}
void uwvm_spot_pb_free(void *ptr) {
    if(!ptr) return;
    budget *b=current_budget;
    if(!b) return;
    allocation *a=owned_allocation(b,ptr);
    if(!a) {b->failed=22;return;}
    a->live=0;
}
static bool admitted_type(const uwvm_spot_proto_type *t) {
    for(size_t i=0;i<uwvm_spot_proto_type_count;++i)
        if(t==&uwvm_spot_proto_types[i]) return true;
    return false;
}
uint32_t uwvm_spot_proto_init(uwvm_spot_proto_document *out,void *bank0,void *bank1,size_t capacity) {
    if(!out||!bank0||!bank1||capacity<4096||capacity>UWVM_SPOT_PROTO_BANK_MAX) return 22;
    uintptr_t a=(uintptr_t)bank0,b=(uintptr_t)bank1,o=(uintptr_t)out;
    if(a%alignof(max_align_t)||b%alignof(max_align_t)||capacity>UINTPTR_MAX-a||
       capacity>UINTPTR_MAX-b||sizeof(*out)>UINTPTR_MAX-o) return 22;
    if((a<b+capacity&&b<a+capacity)||(o<a+capacity&&a<o+sizeof(*out))||
       (o<b+capacity&&b<o+sizeof(*out))) return 22;
    uwvm_spot_proto_document next={0};
    next.banks[0]=bank0;next.banks[1]=bank1;next.capacity=capacity;next.initialized=1;
    *out=next;return 0;
}
uint32_t uwvm_spot_proto_decode(uwvm_spot_proto_document *doc,const uwvm_spot_proto_type *type,
                              const void *wire,size_t size) {
    if(!doc||doc->initialized!=1) return 22;
    if(doc->busy||current_budget) return 17;
    if(!admitted_type(type)) return 22;
    uint32_t status=uwvm_spot_proto_preflight(type->schema,wire,size);
    if(status) return status;
    /* Snapshot before touching inactive storage: input can borrow either bank. */
    uint8_t captured[UWVM_SPOT_PROTO_WIRE_MAX];
    if(size) memcpy(captured,wire,size);
    uint32_t target=doc->value?doc->active_bank^1u:0u;
    budget arena={doc->banks[target],doc->capacity,0,0};
    void *root=reserve(&arena,type->object_size);
    uint8_t *saved=(uint8_t *)reserve(&arena,size);
    if(!root||!saved) return 90;
    memset(root,0,type->object_size);
    if(size) memcpy(saved,captured,size);
    doc->busy=1;current_budget=&arena;
    pb_istream_t stream=pb_istream_from_buffer(saved,size);
    bool ok=pb_decode(&stream,type->nanopb,root);
    status=arena.failed?arena.failed:((ok&&stream.bytes_left==0)?0u:22u);
    /* pb_decode already releases its partial object on failure. On success
     * retain all fields in the selected bank; arena release is whole-bank. */
    current_budget=NULL;doc->busy=0;
    if(status) return status;
    doc->value=root;doc->type=type;doc->wire=saved;doc->wire_size=size;
    doc->active_bank=target;doc->used=arena.used;
    return 0;
}
uint32_t uwvm_spot_proto_clear(uwvm_spot_proto_document *doc) {
    if(!doc||doc->initialized!=1) return 22;
    if(doc->busy||current_budget) return 17;
    doc->value=NULL;doc->wire=NULL;doc->wire_size=0;doc->type=NULL;doc->used=0;
    return 0;
}
