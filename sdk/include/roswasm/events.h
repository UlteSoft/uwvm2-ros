/** @file
 * @brief Additive, allocation-free unified event ABI v1 for C11/C++ guests.
 * An event.observe grant never replaces the independent source grant. The
 * extension dispatch table is explicit and leaves the original C backend ABI
 * unchanged. Raw bodies retain their native canonical codec and owned ID8.
 */
#ifndef ROSWASMC_EVENTS_H
#define ROSWASMC_EVENTS_H
#include "roswasmc.h"
/** @brief Wire event kinds. The values are stable across both ROS backends. */
typedef enum roswasmc_event_kind { ROSWASMC_TOPIC_MESSAGE=1, ROSWASMC_SERVICE_REQUEST=2,
    ROSWASMC_SERVICE_RESPONSE=3, ROSWASMC_ACTION_FEEDBACK=4, ROSWASMC_ACTION_RESULT=5,
    ROSWASMC_RESOURCE_ERROR=6, ROSWASMC_RESOURCE_CLOSED=7 } roswasmc_event_kind;
/** @brief Decoded metadata; this native struct is never a serialized object. */
typedef struct roswasmc_event_header {
    uint16_t kind;uint8_t backend,flags;uint32_t error;
    uint64_t source,correlation,sequence,instance,session,payload_bytes;
} roswasmc_event_header;
/** @brief Explicit dispatch for the additive imports; no default aliases/rights. */
typedef struct roswasmc_event_backend {
    uint32_t version;
    uint32_t (*watch)(uint64_t,uint64_t,uint64_t);
    uint32_t (*poll)(uint64_t,uint32_t,uint64_t,uint64_t,uint64_t);
    uint32_t (*ready)(uint64_t,uint32_t,uint64_t);
    uint32_t (*submit)(uint64_t,uint64_t,uint32_t,uint64_t,uint64_t,uint64_t);
} roswasmc_event_backend;
/** @brief Four full-width readiness counters; a snapshot grants no credit. */
typedef struct roswasmc_event_readiness { uint64_t ready_events,next_event_bytes,admission_slots,admission_bytes; } roswasmc_event_readiness;
/** @brief Load an unaligned fixed-width little-endian scalar. */
static inline uint64_t roswasmc_event_integer(unsigned char const* p,unsigned n) { uint64_t v=0;for(unsigned i=0;i<n;++i) v|=(uint64_t)p[i]<<(8*i);return v; }
/** @brief Transactionally decode one complete record, including ticket checks.
 * Header64B: UWRE/u32,version/u16,kind/u16,backend/u8,flags/u8,reserved/u16,
 * error/u32,source/correlation/sequence/instance/session/bodyBytes (six u64LE).
 * Flags1=source retired,2=admitted external effects may remain,4=incoming Action,
 * 8=parameter projection. These flags never confer another operation's rights.
 * Body strings use the source codec's UTF-8 encoding, never C string addresses.
 */
static inline uint32_t roswasmc_event_decode(void const* bytes,uint64_t length,roswasmc_event_header* out) {
    unsigned char const* p=(unsigned char const*)bytes;roswasmc_event_header h;
    if(!p||!out||length<64||length>4194304u||roswasmc_event_integer(p,4)!=0x45525755u||roswasmc_event_integer(p+4,2)!=1||roswasmc_event_integer(p+10,2)) return UWVM_ROS_INVALID;
    h.kind=(uint16_t)roswasmc_event_integer(p+6,2);h.backend=p[8];h.flags=p[9];h.error=(uint32_t)roswasmc_event_integer(p+12,4);
    h.source=uwvm_ros_load_u64(p+16);h.correlation=uwvm_ros_load_u64(p+24);h.sequence=uwvm_ros_load_u64(p+32);h.instance=uwvm_ros_load_u64(p+40);h.session=uwvm_ros_load_u64(p+48);h.payload_bytes=uwvm_ros_load_u64(p+56);
    if(h.kind<1||h.kind>7||h.backend<1||h.backend>2||!h.source||!h.sequence||!h.instance||!h.session||h.payload_bytes!=length-64) return UWVM_ROS_INVALID;
    if(h.kind>=6) { if(h.payload_bytes||(h.flags&~3u)||(h.kind==6&&!h.error)||(h.kind==7&&!(h.flags&1u))) return UWVM_ROS_INVALID; }
    else {
        if(h.error) return UWVM_ROS_INVALID;
        if(h.kind==1) { if(h.correlation||(h.flags!=0&&h.flags!=8)) return UWVM_ROS_INVALID; }
        else if(!h.correlation||(h.kind==2?(h.flags!=0&&h.flags!=4):h.flags!=0)) return UWVM_ROS_INVALID;
        if((h.kind==2||h.kind==4||h.kind==5)&&(h.payload_bytes<8||uwvm_ros_load_u64(p+64)!=h.correlation)) return UWVM_ROS_INVALID;
    }
    *out=h;return UWVM_ROS_SUCCESS;
}
/** @brief Check explicit backend and current observer owner without imports. */
static inline uint32_t roswasmc_event_owner(roswasmc_event_backend const* api,roswasmc_capability const* cap) {
    if(!roswasmc_is_open(cap)) return UWVM_ROS_BAD_HANDLE;
    return !api||api->version!=cap->context->backend->version?UWVM_ROS_INVALID:UWVM_ROS_SUCCESS;
}
/** @brief Associate already opened observer/source owners once; does not activate
 * a legacy shared consumer. correlation is nonzero only when outgoing Action
 * data is selected; metadata-only watches use zero and do not consume a result.
 * Source and observer must have the same stable context/backend. Duplicate
 * watches or data watches on an activated legacy queue return17. Metadata-only
 * observation may coexist with that queue; no implicit migration occurs.
 */
static inline uint32_t roswasmc_event_watch(roswasmc_event_backend const* api,roswasmc_capability const* observer,roswasmc_capability const* source,uint64_t correlation) {
    uint32_t e=roswasmc_event_owner(api,observer);if(e) return e;e=roswasmc_event_owner(api,source);if(e) return e;
    if(observer->context!=source->context||observer->handle==source->handle) return UWVM_ROS_INVALID;
    return api->watch?api->watch(observer->handle,source->handle,correlation):(uint32_t)UWVM_ROS_UNSUPPORTED;
}
/** @brief Capacity-checked poll. Only this new API retains a record on90.
 * The required-size slot is valid only if the UINT64_MAX sentinel changed.
 * On success the caller owns a complete copy; decode it before using its body.
 * Observer close/revoke still uses ordinary checked capability cleanup.
 */
static inline roswasmc_result roswasmc_event_poll(roswasmc_event_backend const* api,roswasmc_capability const* observer,void* output,uint64_t capacity) {
    roswasmc_result r={roswasmc_event_owner(api,observer),0,0};unsigned char slot[8];if(r.error) return r;
    if((capacity&&!output)||capacity>4194304u) {r.error=UWVM_ROS_INVALID;return r;}
    if(!api->poll) {r.error=UWVM_ROS_UNSUPPORTED;return r;}for(unsigned i=0;i<8;++i)slot[i]=255;
    r.error=api->poll(observer->handle,0,uwvm_ros_address(output),capacity,uwvm_ros_address(slot));
    if((r.error==0||r.error==90)&&uwvm_ros_load_u64(slot)!=UINT64_MAX) {r.has_length=1;r.length=uwvm_ros_load_u64(slot);}
    if(!r.error&&(!r.has_length||r.length>capacity)) r.error=UWVM_ROS_INVALID;return r;
}
/** @brief Return a finite credit snapshot; callers must still handle submit11. */
static inline uint32_t roswasmc_event_ready(roswasmc_event_backend const* api,roswasmc_capability const* observer,roswasmc_event_readiness* output) {
    unsigned char bytes[32]={0};uint32_t e=roswasmc_event_owner(api,observer);if(e)return e;if(!output)return UWVM_ROS_INVALID;if(!api->ready)return UWVM_ROS_UNSUPPORTED;
    e=api->ready(observer->handle,0,uwvm_ros_address(bytes));if(e)return e;
    output->ready_events=uwvm_ros_load_u64(bytes);output->next_event_bytes=uwvm_ros_load_u64(bytes+8);output->admission_slots=uwvm_ros_load_u64(bytes+16);output->admission_bytes=uwvm_ros_load_u64(bytes+24);return 0;
}
/** @brief Submit once; return0 means bounded native admission, not completion.
 * Response and error observations both must be explicit. No guest pointer is
 * retained. Code11 means no admission credit and no native service dispatch.
 * Code107/110 may follow admission; never automatically replay the operation.
 */
static inline uint32_t roswasmc_service_submit(roswasmc_event_backend const* api,roswasmc_capability const* source,roswasmc_capability const* observer,void const* input,uint64_t length,uint64_t* request_id) {
    unsigned char slot[8]={0};uint32_t e=roswasmc_event_owner(api,observer);if(e)return e;e=roswasmc_event_owner(api,source);if(e)return e;
    if(source->context!=observer->context||!request_id||(length&&!input)||length>4193778u)return UWVM_ROS_INVALID;if(!api->submit)return UWVM_ROS_UNSUPPORTED;
    e=api->submit(source->handle,observer->handle,0,uwvm_ros_address(input),length,uwvm_ros_address(slot));if(e)return e;
    if(!uwvm_ros_load_u64(slot))return UWVM_ROS_INVALID;*request_id=uwvm_ros_load_u64(slot);return 0;
}
/** @brief Local audit binding for exactly one already authorized event watch.
 * Initial delivery must be sequence1; subsequent records must be contiguous.
 * Decode/refusal is transactional: an invalid record changes neither binding
 * nor output. Zero-correlation watches may deliver several service request IDs;
 * nonzero correlation restricts Action data while permitting uncorrelated error
 * metadata. The trusted Runtime still checks actual capabilities and ownership.
 */
typedef struct roswasmc_event_binding {
    uint8_t backend;uint64_t source,correlation,sequence,instance,session;
} roswasmc_event_binding;
/** @brief Initialize only local state. No watch, alias or native resource opens. */
static inline uint32_t roswasmc_event_binding_init(roswasmc_event_binding* binding,uint32_t backend,uint64_t source,uint64_t correlation) {
    if(!binding||backend<1||backend>2||!source)return UWVM_ROS_INVALID;
    {roswasmc_event_binding next={(uint8_t)backend,source,correlation,0,0,0};*binding=next;}return 0;
}
/** @brief Decode and commit the exact source/session/delivery sequence once. */
static inline uint32_t roswasmc_event_binding_decode(roswasmc_event_binding* binding,void const* bytes,uint64_t length,roswasmc_event_header* out) {
    roswasmc_event_header h;uint32_t error;if(!binding||!out)return UWVM_ROS_INVALID;
    error=roswasmc_event_decode(bytes,length,&h);if(error)return error;
    if(h.backend!=binding->backend||h.source!=binding->source||binding->sequence==UINT64_MAX||h.sequence!=binding->sequence+1||
       (binding->instance&&(h.instance!=binding->instance||h.session!=binding->session))||
       (binding->correlation&&h.correlation&&h.correlation!=binding->correlation))return UWVM_ROS_INVALID;
    binding->sequence=h.sequence;binding->instance=h.instance;binding->session=h.session;*out=h;return 0;
}
/** @brief Complete-event callback. Body is borrowed only until normal return.
 * Inspect kind/flags before selecting a service, Action or topic codec. Numeric
 * error/closed metadata is delivered as an event, not as a polling failure.
 * A nonzero application status ends this pass; the copied event is not replayed.
 */
typedef uint32_t (*roswasmc_unified_callback)(void*,roswasmc_event_header const*,unsigned char const*,size_t);
/** @brief Allocation-free caller-owned unified-event executor entity.
 * Borrowed owners, backend, buffer and callback data must outlive registration.
 * The source is NOT subscribed/advertised through a legacy consumer. Construction
 * performs one explicit watch only. Close/revoke here retires only the observer;
 * the source remains independently owned. C callers must not directly retire or
 * mutate borrowed owners/buffer while a callback is active, or longjmp through
 * this dispatch. These local guards never replace Runtime authority checks.
 */
typedef struct roswasmc_event_subscription {
    roswasmc_event_backend const* api;roswasmc_capability* observer;roswasmc_capability* source;
    unsigned char* buffer;size_t capacity;roswasmc_unified_callback callback;void* user;
    roswasmc_event_binding binding;uint32_t error;int spinning,delivered;
} roswasmc_event_subscription;
/** @brief Bind explicit already-open owners and fixed storage; no hidden opens.
 * A failed watch leaves both owners with the caller for checked cleanup.
 */
static inline uint32_t roswasmc_event_subscription_init(roswasmc_event_subscription* subscription,
    roswasmc_event_backend const* api,roswasmc_capability* observer,roswasmc_capability* source,
    uint64_t correlation,unsigned char* buffer,size_t capacity,roswasmc_unified_callback callback,void* user) {
    uint32_t error;roswasmc_event_subscription next={0};
    if(!subscription)return UWVM_ROS_INVALID;next.error=UWVM_ROS_INVALID;*subscription=next;
    if(!buffer||capacity<64||capacity>4194304u||!callback)return UWVM_ROS_INVALID;
    error=roswasmc_event_owner(api,source);if(error)return error;
    next.api=api;next.observer=observer;next.source=source;next.buffer=buffer;next.capacity=capacity;
    next.callback=callback;next.user=user;next.error=0;next.spinning=next.delivered=0;
    error=roswasmc_event_binding_init(&next.binding,api->version,source->handle,correlation);if(error)return error;
    error=roswasmc_event_watch(api,observer,source,correlation);next.error=error;*subscription=next;return error;
}
/** @brief Copy/validate/dispatch one event;11 is idle,90 retains the native record.
 * Binding is committed before application code, so callback failure cannot cause
 * replay of an already consumed event. Nested dispatch or facade retirement17
 * occurs before polling or buffer mutation. No automatic resize/retry occurs.
 */
static inline uint32_t roswasmc_event_subscription_spin(roswasmc_event_subscription* subscription) {
    roswasmc_result result;roswasmc_event_header header;uint32_t error;
    if(!subscription)return UWVM_ROS_INVALID;if(subscription->spinning)return UWVM_ROS_CONFLICT;
    if(subscription->error)return subscription->error;subscription->delivered=0;subscription->spinning=1;
    result=roswasmc_event_poll(subscription->api,subscription->observer,subscription->buffer,subscription->capacity);
    error=result.error;
    if(!error)error=roswasmc_event_binding_decode(&subscription->binding,subscription->buffer,result.length,&header);
    if(!error){subscription->delivered=1;error=subscription->callback(subscription->user,&header,subscription->buffer+64,(size_t)header.payload_bytes);}
    subscription->spinning=0;return error;
}
/** @brief Poll each selected entity once; maximum counts successful callbacks.
 * Registrations/storage remain borrowed for the entire pass. A callback must
 * defer array mutation/destruction until return. Errors preserve delivered.
 */
static inline uint32_t roswasmc_event_spin_some(roswasmc_event_subscription* const* subscriptions,size_t count,size_t maximum,size_t* delivered) {
    size_t i;uint32_t error;if(!delivered||(!subscriptions&&count))return UWVM_ROS_INVALID;*delivered=0;
    for(i=0;i<count&&*delivered<maximum;++i){error=roswasmc_event_subscription_spin(subscriptions[i]);
        if(error==UWVM_ROS_AGAIN&&!subscriptions[i]->delivered)continue;if(error)return error;++*delivered;}return 0;
}
/** @brief Retire only this explicit observer once; defer during callbacks. */
static inline uint32_t roswasmc_event_subscription_close(roswasmc_event_subscription* subscription) {
    if(!subscription)return UWVM_ROS_INVALID;if(subscription->spinning)return UWVM_ROS_CONFLICT;
    return roswasmc_close(subscription->observer);
}
/** @brief Revoke only this explicit observer alias; no source authority changes. */
static inline uint32_t roswasmc_event_subscription_revoke(roswasmc_event_subscription* subscription) {
    if(!subscription)return UWVM_ROS_INVALID;if(subscription->spinning)return UWVM_ROS_CONFLICT;
    return roswasmc_revoke(subscription->observer);
}
#endif
