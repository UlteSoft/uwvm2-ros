/** @file
 * @brief Complete caller-driven unified topic/closed event application.
 * Build once per backend using ROSWASM_EXAMPLE_ROS_VERSION=1/2. Host policy must
 * explicitly grant sub (String topic.subscribe) and subEvents (event.observe,
 * event-source-alias=sub, kinds=topic-message,resource-error,resource-closed).
 * See ../../docs/unified-events.md for all quotas and native peer prerequisites.
 * No grants => open13, which is reported by the process status. One initial
 * poll may return11; this example grants no clock/thread and installs no loop.
 */
#include "select_backend.h"
#include <roswasm/events.h>
static roswasmc_event_backend const* events_api(void) {
#if ROSWASM_EXAMPLE_ROS_VERSION == 1
    return roswasmc_ros1_event_backend();
#else
    return roswasmc_ros2_event_backend();
#endif
}
/** One copied header snapshot; body remains borrowed until callback return.
 * Decode String with its canonical codec only for TopicMessage. Incoming service
 * and Action events need their own selected source codec and distinct responder.
 */
static uint32_t receive(void* user,roswasmc_event_header const* header,unsigned char const* body,size_t bytes) {
    (void)body;(void)bytes;*(roswasmc_event_header*)user=*header;return 0;
}
int main(void) {
    roswasmc_context context={ROSWASM_BACKEND()};roswasmc_capability source,observer;
    roswasmc_capability_init(&source,&context);roswasmc_capability_init(&observer,&context);
    uint32_t primary=roswasmc_open(&source,"sub",3),cleanup=0;
    if(!primary)primary=roswasmc_open(&observer,"subEvents",9);
    unsigned char storage[256];roswasmc_event_header header={0};roswasmc_event_readiness ready;
    roswasmc_event_subscription subscription={0};
    if(!primary)primary=roswasmc_event_subscription_init(&subscription,events_api(),&observer,&source,0,
        storage,sizeof storage,receive,&header);
    roswasmc_event_subscription* registrations[]={&subscription};size_t delivered=0;
    if(!primary)primary=roswasmc_event_ready(events_api(),&observer,&ready);
    // The additive executor uses event_poll_v1; legacy roswasmc_spin_some still
    // consumes legacy queues. Idle is skipped, capacity90 is returned unchanged.
    if(!primary)primary=roswasmc_event_spin_some(registrations,1,1,&delivered);
    // Close source first. Permitted metadata remains observable through the
    // independently owned observer; sensitive queued bodies are discarded.
    if(roswasmc_is_open(&source)){uint32_t e=roswasmc_close(&source);if(!cleanup)cleanup=e;}
    if(!primary&&!cleanup){primary=roswasmc_event_subscription_spin(&subscription);if(!primary&&header.kind!=ROSWASMC_RESOURCE_CLOSED)primary=UWVM_ROS_INVALID;}
    if(roswasmc_is_open(&observer)){uint32_t e=roswasmc_close(&observer);if(!cleanup)cleanup=e;}
    return (int)(primary?primary:cleanup);
}
