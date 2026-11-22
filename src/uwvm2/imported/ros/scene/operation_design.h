/** @file
 * @brief Complete design metadata; this table registers no guest imports.
 * @details Generated from the canonical catalogue and operational design.
 * Availability records registration only, not native or safety qualification.
 * No dynamic storage, policy mutation or provider activation occurs here.
 */
#pragma once
#include <array>
#include <string_view>

namespace uwvm2::imported::ros::scene::operation_design
{
    /** @brief Reviewed wire/authority/commit contract for one named operation. */
    struct descriptor
    {std::string_view name,behavior,meaning;unsigned primary_domain;bool registered;};
    inline constexpr std::array operations{
        /** @brief abi: discovery.
         * @details ABI discovery returns the constant major version 2 to a bound caller by default. The host may deny the call (13) or hide the import. Reading the version does not enroll a caller, prepare a provider or consume a resource grant.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"abi","discovery","ABI discovery returns the constant major version 2 to a bound caller by default. The host may deny the call (13) or hide the import. Reading the version does not enroll a caller, prepare a provider or consume a resource grant.",1,true},
        /** @brief capability.open: capability acquisition.
         * @details Exact existing alias; no grant creation or upgrade; undelivered handle rolled back.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"capability.open","capability_acquisition","Exact existing alias; no grant creation or upgrade; undelivered handle rolled back.",1,true},
        /** @brief capability.close: capability retirement.
         * @details Close capability handle without resetting budgets or resource lifetime.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"capability.close","capability_retirement","Close capability handle without resetting budgets or resource lifetime.",1,true},
        /** @brief guard.create: owned creation.
         * @details Create owned guard, rollback if handle delivery fails.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"guard.create","owned_creation","Create owned guard, rollback if handle delivery fails.",2,true},
        /** @brief guard.trigger: local mutation.
         * @details Increment guard generation under exact owner grant.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"guard.trigger","local_mutation","Increment guard generation under exact owner grant.",2,true},
        /** @brief guard.take: consume after copy.
         * @details Consume pending count only after checked delivery.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"guard.take","consume_after_copy","Consume pending count only after checked delivery.",2,true},
        /** @brief clock.create: owned creation.
         * @details Kind must match exact clock purpose.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"clock.create","owned_creation","Kind must match exact clock purpose.",10,true},
        /** @brief clock.read: owned observation.
         * @details Sample owned clock; unstarted time remains flagged.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"clock.read","owned_observation","Sample owned clock; unstarted time remains flagged.",10,true},
        /** @brief clock.update: local mutation.
         * @details ROS clock source must independently permit clock.read with matching resource and binding.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"clock.update","local_mutation","ROS clock source must independently permit clock.read with matching resource and binding.",10,true},
        /** @brief timer.create: owned creation.
         * @details Clock dependency needs independent read grant.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"timer.create","owned_creation","Clock dependency needs independent read grant.",2,true},
        /** @brief timer.reset: local mutation.
         * @details Overflow-checked schedule; zero period is one-shot.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"timer.reset","local_mutation","Overflow-checked schedule; zero period is one-shot.",2,true},
        /** @brief timer.cancel: local mutation.
         * @details Disarm owned timer and notify waiters.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"timer.cancel","local_mutation","Disarm owned timer and notify waiters.",2,true},
        /** @brief timer.take: consume after copy.
         * @details Coalesced occurrence record consumed only after delivery.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"timer.take","consume_after_copy","Coalesced occurrence record consumed only after delivery.",2,true},
        /** @brief wait.create: owned creation.
         * @details Create empty owned waitset.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"wait.create","owned_creation","Create empty owned waitset.",2,true},
        /** @brief wait.add: local mutation.
         * @details Source requires independent resource.observe; bounded membership.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"wait.add","local_mutation","Source requires independent resource.observe; bounded membership.",2,true},
        /** @brief wait.remove: local mutation.
         * @details Detach exact source from owned waitset.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"wait.remove","local_mutation","Detach exact source from owned waitset.",2,true},
        /** @brief wait.acknowledge: local mutation.
         * @details Acknowledge source sequence; readiness is not data consumption.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"wait.acknowledge","local_mutation","Acknowledge source sequence; readiness is not data consumption.",2,true},
        /** @brief wait.poll: bounded wait.
         * @details Nonconsuming bounded readiness; infinite wait explicit; authority revalidated after wake.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"wait.poll","bounded_wait","Nonconsuming bounded readiness; infinite wait explicit; authority revalidated after wake.",2,true},
        /** @brief resource.close: owned close.
         * @details Close an owned local scheduling, lifecycle, sealed-buffer, model or transform resource according to its cleanup contract. Retained borrows or cleanup debt retain their capacity until actual retirement; closure does not grant another purpose. Sensor/proposal and recording/replay handles route to their owning dispatchers; retained borrows/pins still prevent premature retirement.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"resource.close","owned_close","Close an owned local scheduling, lifecycle, sealed-buffer, model or transform resource according to its cleanup contract. Retained borrows or cleanup debt retain their capacity until actual retirement; closure does not grant another purpose. Sensor/proposal and recording/replay handles route to their owning dispatchers; retained borrows/pins still prevent premature retirement.",6,true},
        /** @brief context.open: owned creation.
         * @details Create an independently owned context only after complete provider/security closure validation.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"context.open","owned_creation","Create an independently owned context only after complete provider/security closure validation.",5,true},
        /** @brief context.describe: owned observation.
         * @details Sanitized context identity, supported features and shutdown state. Provider feature reporting distinguishes configured, native-attested supported and currently available states; missing/failed native feature probes are not affirmative support.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"context.describe","owned_observation","Sanitized context identity, supported features and shutdown state. Provider feature reporting distinguishes configured, native-attested supported and currently available states; missing/failed native feature probes are not affirmative support.",5,true},
        /** @brief context.shutdown: keyed effect.
         * @details Cancel owned work and wake waits; never process exit.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"context.shutdown","keyed_effect","Cancel owned work and wake waits; never process exit.",5,true},
        /** @brief node.open: asynchronous creation.
         * @details Resolve immutable node/name/remapping/profile under exact parent context grant. Creation is asynchronous: explicit monotonically increasing key and finite absolute steady deadline; response is a request receipt, not an endpoint. communication.open.take retrieves a separately published typed handle.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"node.open","asynchronous_creation","Resolve immutable node/name/remapping/profile under exact parent context grant. Creation is asynchronous: explicit monotonically increasing key and finite absolute steady deadline; response is a request receipt, not an endpoint. communication.open.take retrieves a separately published typed handle.",5,true},
        /** @brief node.describe: owned observation.
         * @details Only owned resolved identity and enabled features.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"node.describe","owned_observation","Only owned resolved identity and enabled features.",5,true},
        /** @brief type.describe: owned observation.
         * @details Bounded immutable approved schema dependency closure; type names never load code.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"type.describe","owned_observation","Bounded immutable approved schema dependency closure; type names never load code.",5,true},
        /** @brief topic.open: asynchronous creation.
         * @details Create exactly one publisher or subscription role. Creation is asynchronous: explicit monotonically increasing key and finite absolute steady deadline; response is a request receipt, not an endpoint. communication.open.take retrieves a separately published typed handle.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"topic.open","asynchronous_creation","Create exactly one publisher or subscription role. Creation is asynchronous: explicit monotonically increasing key and finite absolute steady deadline; response is a request receipt, not an endpoint. communication.open.take retrieves a separately published typed handle.",5,true},
        /** @brief topic.publish: keyed effect.
         * @details Reserve and validate complete message before acceptance; acceptance is not remote delivery.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"topic.publish","keyed_effect","Reserve and validate complete message before acceptance; acceptance is not remote delivery.",5,true},
        /** @brief topic.take: consume after copy.
         * @details Receive one source record without consuming on invalid/short output.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"topic.take","consume_after_copy","Receive one source record without consuming on invalid/short output.",5,true},
        /** @brief topic.describe: owned observation.
         * @details Owned node, resolved endpoint/type/direction and profile, with optional cached actual provider incarnation and native identity. Metadata is copied from the same retained endpoint; no middleware call or discovery occurs during description. Unknown provider generation and GID remain absent. This grants no publication or subscription authority.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"topic.describe","owned_observation","Owned node, resolved endpoint/type/direction and profile, with optional cached actual provider incarnation and native identity. Metadata is copied from the same retained endpoint; no middleware call or discovery occurs during description. Unknown provider generation and GID remain absent. This grants no publication or subscription authority.",5,true},
        /** @brief qos.describe: owned observation.
         * @details Requested/effective/supported settings; unavailable values remain absent.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"qos.describe","owned_observation","Requested/effective/supported settings; unavailable values remain absent.",5,true},
        /** @brief qos.liveliness.assert: keyed effect.
         * @details Only publisher role and qualified native manual-liveliness support.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"qos.liveliness.assert","keyed_effect","Only publisher role and qualified native manual-liveliness support.",5,true},
        /** @brief qos.acknowledgements.wait: bounded wait.
         * @details Wait for defined native acknowledgement horizon; unsupported is not success.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"qos.acknowledgements.wait","bounded_wait","Wait for defined native acknowledgement horizon; unsupported is not success.",5,true},
        /** @brief service.open: asynchronous creation.
         * @details Fix client/server role and finite request/response capacities. Creation is asynchronous: explicit monotonically increasing key and finite absolute steady deadline; response is a request receipt, not an endpoint. communication.open.take retrieves a separately published typed handle.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"service.open","asynchronous_creation","Fix client/server role and finite request/response capacities. Creation is asynchronous: explicit monotonically increasing key and finite absolute steady deadline; response is a request receipt, not an endpoint. communication.open.take retrieves a separately published typed handle.",5,true},
        /** @brief service.submit: keyed effect.
         * @details Owned native-correlated request; remote outcome can remain unknown after timeout.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"service.submit","keyed_effect","Owned native-correlated request; remote outcome can remain unknown after timeout.",5,true},
        /** @brief service.take: consume after copy.
         * @details Role-specific request/response atomically delivered with correlation.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"service.take","consume_after_copy","Role-specific request/response atomically delivered with correlation.",5,true},
        /** @brief service.respond: keyed effect.
         * @details Respond once to an exact live owned received request.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"service.respond","keyed_effect","Respond once to an exact live owned received request.",5,true},
        /** @brief service.available: owned observation.
         * @details Observe native readiness on the exact live client. Direct client grants supply source_capability zero. A service-observer grant requires its separately authorized registered source service.submit capability. Only the observer is charged. Either authorization may prevent result delivery.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"service.available","owned_observation","Observe native readiness on the exact live client. Direct client grants supply source_capability zero. A service-observer grant requires its separately authorized registered source service.submit capability. Only the observer is charged. Either authorization may prevent result delivery.",5,true},
        /** @brief service.abandon: owned cancellation.
         * @details Retire local wait/response interest; never claim remote cancellation.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"service.abandon","owned_cancellation","Retire local wait/response interest; never claim remote cancellation.",5,true},
        /** @brief action.open: asynchronous creation.
         * @details Fix action role, goal/feedback/result types and retention. Creation is asynchronous: explicit monotonically increasing key and finite absolute steady deadline; response is a request receipt, not an endpoint. communication.open.take retrieves a separately published typed handle.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"action.open","asynchronous_creation","Fix action role, goal/feedback/result types and retention. Creation is asynchronous: explicit monotonically increasing key and finite absolute steady deadline; response is a request receipt, not an endpoint. communication.open.take retrieves a separately published typed handle.",5,true},
        /** @brief action.submit: keyed effect.
         * @details Goal UUID is correlated with owned goal resource; not a transferable authority token.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"action.submit","keyed_effect","Goal UUID is correlated with owned goal resource; not a transferable authority token.",5,true},
        /** @brief action.take: consume after copy.
         * @details Goal proposal/status/feedback/result/cancel request are distinct closed event kinds. uwvm/action/Event body follows action_event; outer correlation is request, body goal/UUID remain distinct.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"action.take","consume_after_copy","Goal proposal/status/feedback/result/cancel request are distinct closed event kinds. uwvm/action/Event body follows action_event; outer correlation is request, body goal/UUID remain distinct.",5,true},
        /** @brief action.respond: keyed effect.
         * @details Legal transition and exact secondary action checked; accepted cancel is not terminal.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"action.respond","keyed_effect","Legal transition and exact secondary action checked; accepted cancel is not terminal.",5,true},
        /** @brief action.cancel: keyed effect.
         * @details Cancellation proposal for owned goal; broader cancel set requires separate explicit scope.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"action.cancel","keyed_effect","Cancellation proposal for owned goal; broader cancel set requires separate explicit scope.",5,true},
        /** @brief action.describe: owned observation.
         * @details Sanitized endpoint/owned-goal state and retention; no unrelated UUID discovery. uwvm/action/State body follows action_endpoint_state.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"action.describe","owned_observation","Sanitized endpoint/owned-goal state and retention; no unrelated UUID discovery. uwvm/action/State body follows action_endpoint_state.",5,true},
        /** @brief parameter.open: owned creation.
         * @details Bind exact parameter keys, value algebra, descriptors and role.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"parameter.open","owned_creation","Bind exact parameter keys, value algebra, descriptors and role.",5,true},
        /** @brief parameter.declare: keyed effect.
         * @details Declaration respects override/dynamic/read-only policy without ambient YAML access.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"parameter.declare","keyed_effect","Declaration respects override/dynamic/read-only policy without ambient YAML access.",5,true},
        /** @brief parameter.undeclare: keyed effect.
         * @details Explicit deletion distinct from not-set/uninitialized value.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"parameter.undeclare","keyed_effect","Explicit deletion distinct from not-set/uninitialized value.",5,true},
        /** @brief parameter.get: owned observation.
         * @details Bounded batch; missing/uninitialized/denied are not silently conflated.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"parameter.get","owned_observation","Bounded batch; missing/uninitialized/denied are not silently conflated.",5,true},
        /** @brief parameter.describe: owned observation.
         * @details Only granted descriptor metadata.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"parameter.describe","owned_observation","Only granted descriptor metadata.",5,true},
        /** @brief parameter.list: owned observation.
         * @details Bounded filtered key listing; counts and cursors obey same visibility. Name entries use fixed message type_identity ros.parameter.name and encoding canonical.text; each body is exactly one canonical wire text.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"parameter.list","owned_observation","Bounded filtered key listing; counts and cursors obey same visibility. Name entries use fixed message type_identity ros.parameter.name and encoding canonical.text; each body is exactly one canonical wire text.",5,true},
        /** @brief parameter.set: keyed effect.
         * @details Per-item writes; report every committed/rejected/unknown decision.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"parameter.set","keyed_effect","Per-item writes; report every committed/rejected/unknown decision.",5,true},
        /** @brief parameter.set.atomic: keyed effect.
         * @details All or none only within qualified provider atomic domain; do not fake cross-node CAS. A ROS1 provider must separately declare atomic upsert of permitted undeclared keys and subtree replacement semantics; ROS2 declaration rules remain distinct. Native bridging cannot silently emulate atomic upsert with a racy get/declare/set sequence.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"parameter.set.atomic","keyed_effect","All or none only within qualified provider atomic domain; do not fake cross-node CAS. A ROS1 provider must separately declare atomic upsert of permitted undeclared keys and subtree replacement semantics; ROS2 declaration rules remain distinct. Native bridging cannot silently emulate atomic upsert with a racy get/declare/set sequence.",5,true},
        /** @brief parameter.take: consume after copy.
         * @details Deliver validation or committed event; no native callback into guest.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"parameter.take","consume_after_copy","Deliver validation or committed event; no native callback into guest.",5,true},
        /** @brief parameter.respond: callback response.
         * @details Current validator generation/deadline; transformed keys and values revalidated; no recursive mutation.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"parameter.respond","callback_response","Current validator generation/deadline; transformed keys and values revalidated; no recursive mutation.",5,true},
        /** @brief lifecycle.open: owned creation.
         * @details Open one explicitly permitted client=1/server=2 role in a deployed local-lifecycle alias; no native node or actuator authority. Opening never resets state or history.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"lifecycle.open","owned_creation","Open one explicitly permitted client=1/server=2 role in a deployed local-lifecycle alias; no native node or actuator authority. Opening never resets state or history.",6,true},
        /** @brief lifecycle.state: owned observation.
         * @details Observe owned cached logical state and enforce an expired finite deadline on access; no physical or autonomous stop guarantee.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"lifecycle.state","owned_observation","Observe owned cached logical state and enforce an expired finite deadline on access; no physical or autonomous stop guarantee.",6,true},
        /** @brief lifecycle.transitions: owned observation.
         * @details Return only currently legal and deployment-permitted transition bits; no transition authority is granted.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"lifecycle.transitions","owned_observation","Return only currently legal and deployment-permitted transition bits; no transition authority is granted.",6,true},
        /** @brief lifecycle.submit: lifecycle submission.
         * @details Exactly ordered successful-submission keys with original receipt replay, finite absolute steady deadline, expected generation and one pending callback. Rejected submissions do not consume key.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"lifecycle.submit","lifecycle_submission","Exactly ordered successful-submission keys with original receipt replay, finite absolute steady deadline, expected generation and one pending callback. Rejected submissions do not consume key.",6,true},
        /** @brief lifecycle.take: consume after copy.
         * @details Reserve one callback, release provider lock before checked delivery, then revalidate owner/request/generation/deadline before marking delivered. Failed delivery is not consumed.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"lifecycle.take","consume_after_copy","Reserve one callback, release provider lock before checked delivery, then revalidate owner/request/generation/deadline before marking delivered. Failed delivery is not consumed.",6,true},
        /** @brief lifecycle.respond: callback response.
         * @details Resolve only an exactly delivered matching request/generation using ROS outcome97/98/99. Preserve last exact response receipt; error queues error-processing callback. No native callback execution.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"lifecycle.respond","callback_response","Resolve only an exactly delivered matching request/generation using ROS outcome97/98/99. Preserve last exact response receipt; error queues error-processing callback. No native callback execution.",6,true},
        /** @brief transform.open: owned creation.
         * @details Bound frame set/cache/source/clock-epoch policy.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"transform.open","owned_creation","Bound frame set/cache/source/clock-epoch policy.",10,true},
        /** @brief transform.publish: keyed effect.
         * @details Authorize every parent/child and source; nonzero expected_generation fences the whole batch. Actual static replacement requires a matching nonzero generation and exact transform.static.replace action; zero permits only additions or unchanged static replay.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"transform.publish","keyed_effect","Authorize every parent/child and source; nonzero expected_generation fences the whole batch. Actual static replacement requires a matching nonzero generation and exact transform.static.replace action; zero permits only additions or unchanged static replay.",10,true},
        /** @brief transform.lookup: owned observation.
         * @details Enforce all query-path frames, epochs and cache bounds.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"transform.lookup","owned_observation","Enforce all query-path frames, epochs and cache bounds.",10,true},
        /** @brief transform.available: owned observation.
         * @details Same visibility boundary as lookup; no hidden frame existence leak.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"transform.available","owned_observation","Same visibility boundary as lookup; no hidden frame existence leak.",10,true},
        /** @brief transform.velocity: owned observation.
         * @details Advanced time/reference-point operation with all frame/window checks.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"transform.velocity","owned_observation","Advanced time/reference-point operation with all frame/window checks.",10,true},
        /** @brief transform.submit: keyed effect.
         * @details Owned asynchronous wait binds exact clock epoch and query.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"transform.submit","keyed_effect","Owned asynchronous wait binds exact clock epoch and query.",10,true},
        /** @brief transform.take: consume after copy.
         * @details Nonconsuming failed copy of one owned query result.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"transform.take","consume_after_copy","Nonconsuming failed copy of one owned query result.",10,true},
        /** @brief transform.cancel: owned cancellation.
         * @details Cancel owned query and wake waiters.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"transform.cancel","owned_cancellation","Cancel owned query and wake waiters.",10,true},
        /** @brief transform.clear: keyed effect.
         * @details Explicit history mutation and pending query invalidation; static policy separate.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"transform.clear","keyed_effect","Explicit history mutation and pending query invalidation; static policy separate.",10,true},
        /** @brief graph.snapshot: owned observation.
         * @details Exact filtered discovery through an explicitly owned observer node and context; observer node profile and actual parent context must match, with counted attachment capacity and complete origin authority.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"graph.snapshot","owned_observation","Exact filtered discovery through an explicitly owned observer node and context; observer node profile and actual parent context must match, with counted attachment capacity and complete origin authority.",5,true},
        /** @brief graph.watch: owned creation.
         * @details Exact filtered discovery through an explicitly owned observer node and context; observer node profile and actual parent context must match, with counted attachment capacity and complete origin authority.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"graph.watch","owned_creation","Exact filtered discovery through an explicitly owned observer node and context; observer node profile and actual parent context must match, with counted attachment capacity and complete origin authority.",5,true},
        /** @brief event.stream.open: owned creation.
         * @details Authorize each observed source and field policy; source references use capability/resource pair.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"event.stream.open","owned_creation","Authorize each observed source and field policy; source references use capability/resource pair.",24,true},
        /** @brief event.stream.poll: consume after copy.
         * @details Copy complete records before consumption; explicit loss and generation; bounded steady deadline.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"event.stream.poll","consume_after_copy","Copy complete records before consumption; explicit loss and generation; bounded steady deadline.",24,true},
        /** @brief event.stream.close: owned close.
         * @details Detach exact stream and wake waiters without closing sources.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"event.stream.close","owned_close","Detach exact stream and wake waiters without closing sources.",24,true},
        /** @brief buffer.create: owned creation.
         * @details Reserve bounded data/workspace before publication.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"buffer.create","owned_creation","Reserve bounded data/workspace before publication.",3,true},
        /** @brief buffer.write: local mutation.
         * @details Contiguous bounded write only before seal; no native pointer.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"buffer.write","local_mutation","Contiguous bounded write only before seal; no native pointer.",3,true},
        /** @brief buffer.seal: local mutation.
         * @details Immutable publication with digest and registered layout.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"buffer.seal","local_mutation","Immutable publication with digest and registered layout.",3,true},
        /** @brief buffer.read: owned observation.
         * @details Exact bounded read; no host descriptor export.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"buffer.read","owned_observation","Exact bounded read; no host descriptor export.",3,true},
        /** @brief buffer.describe: owned observation.
         * @details Untrusted origin cannot be relabelled as a trusted sensor.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"buffer.describe","owned_observation","Untrusted origin cannot be relabelled as a trusted sensor.",3,true},
        /** @brief artifact.open: owned creation.
         * @details Exact positive deployment generation and 32 raw SHA256 bytes; sealed-input binding only. Roll back unpublished handles on failed delivery. No output-store read binding.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"artifact.open","owned_creation","Exact positive deployment generation and 32 raw SHA256 bytes; sealed-input binding only. Roll back unpublished handles on failed delivery. No output-store read binding.",3,true},
        /** @brief artifact.read: owned observation.
         * @details Bounded requested bytes from immutable handle and offset; raw output, EOF short read, no cursor. Exact read action; requested bytes cannot exceed configured transfer or output capacity. Published volatile cache objects are also readable under independent cache read authority; staging stays hidden.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"artifact.read","owned_observation","Bounded requested bytes from immutable handle and offset; raw output, EOF short read, no cursor. Exact read action; requested bytes cannot exceed configured transfer or output capacity. Published volatile cache objects are also readable under independent cache read authority; staging stays hidden.",3,true},
        /** @brief artifact.describe: owned observation.
         * @details 64-byte approved metadata: four LE words then raw32 SHA256. Purpose codes1 package,2 configuration,3 calibration,4 map,5 model-weights. No source path or FD.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"artifact.describe","owned_observation","64-byte approved metadata: four LE words then raw32 SHA256. Purpose codes1 package,2 configuration,3 calibration,4 map,5 model-weights. No source path or FD.",3,true},
        /** @brief artifact.begin: cache transaction.
         * @details Explicit cache/volatile-output only. Per-alias next-key ACK and exact last-request replay; full begin allocation remains charged until abort/remove. Publication is immutable session data, never durable. Status and receipt queries do not acknowledge. No host path or descriptor.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"artifact.begin","cache_transaction","Explicit cache/volatile-output only. Per-alias next-key ACK and exact last-request replay; full begin allocation remains charged until abort/remove. Publication is immutable session data, never durable. Status and receipt queries do not acknowledge. No host path or descriptor.",3,true},
        /** @brief artifact.append: cache transaction.
         * @details Explicit cache/volatile-output only. Per-alias next-key ACK and exact last-request replay; full begin allocation remains charged until abort/remove. Publication is immutable session data, never durable. Status and receipt queries do not acknowledge. No host path or descriptor.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"artifact.append","cache_transaction","Explicit cache/volatile-output only. Per-alias next-key ACK and exact last-request replay; full begin allocation remains charged until abort/remove. Publication is immutable session data, never durable. Status and receipt queries do not acknowledge. No host path or descriptor.",3,true},
        /** @brief artifact.commit: cache transaction.
         * @details Explicit cache/volatile-output only. Per-alias next-key ACK and exact last-request replay; full begin allocation remains charged until abort/remove. Publication is immutable session data, never durable. Status and receipt queries do not acknowledge. No host path or descriptor.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"artifact.commit","cache_transaction","Explicit cache/volatile-output only. Per-alias next-key ACK and exact last-request replay; full begin allocation remains charged until abort/remove. Publication is immutable session data, never durable. Status and receipt queries do not acknowledge. No host path or descriptor.",3,true},
        /** @brief artifact.abort: cache transaction.
         * @details Explicit cache/volatile-output only. Per-alias next-key ACK and exact last-request replay; full begin allocation remains charged until abort/remove. Publication is immutable session data, never durable. Status and receipt queries do not acknowledge. No host path or descriptor.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"artifact.abort","cache_transaction","Explicit cache/volatile-output only. Per-alias next-key ACK and exact last-request replay; full begin allocation remains charged until abort/remove. Publication is immutable session data, never durable. Status and receipt queries do not acknowledge. No host path or descriptor.",3,true},
        /** @brief artifact.remove: cache transaction.
         * @details Explicit cache/volatile-output only. Per-alias next-key ACK and exact last-request replay; full begin allocation remains charged until abort/remove. Publication is immutable session data, never durable. Status and receipt queries do not acknowledge. No host path or descriptor.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"artifact.remove","cache_transaction","Explicit cache/volatile-output only. Per-alias next-key ACK and exact last-request replay; full begin allocation remains charged until abort/remove. Publication is immutable session data, never durable. Status and receipt queries do not acknowledge. No host path or descriptor.",3,true},
        /** @brief sensor.open: owned creation.
         * @details Ingress-only approved source/transport/schema/queue; no send/listen authority.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"sensor.open","owned_creation","Ingress-only approved source/transport/schema/queue; no send/listen authority.",8,true},
        /** @brief sensor.take: consume after copy.
         * @details Validated complete frame with timestamp origin and discontinuity. Capacity for the largest complete frame is checked before native read. The local capture ordinal and optional actual native sequence are distinct. Failed guest copy retains the frame for retry.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"sensor.take","consume_after_copy","Validated complete frame with timestamp origin and discontinuity. Capacity for the largest complete frame is checked before native read. The local capture ordinal and optional actual native sequence are distinct. Failed guest copy retains the frame for retry.",8,true},
        /** @brief sensor.describe: owned observation.
         * @details Bound source identity, loss and negotiated capabilities; no credentials.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"sensor.describe","owned_observation","Bound source identity, loss and negotiated capabilities; no credentials.",8,true},
        /** @brief device.open: robot provider.
         * @details Approved bus/protocol/command envelope, independent watchdog and finite capacity.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"device.open","robot_provider","Approved bus/protocol/command envelope, independent watchdog and finite capacity.",7,true},
        /** @brief device.submit: robot provider.
         * @details Full command bounds/freshness/units/frame before hardware effect.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"device.submit","robot_provider","Full command bounds/freshness/units/frame before hardware effect.",7,true},
        /** @brief device.result: robot provider.
         * @details Distinguish accepted, observed execution, partial and unknown effect.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"device.result","robot_provider","Distinguish accepted, observed execution, partial and unknown effect.",7,true},
        /** @brief device.stop: robot provider.
         * @details Requires explicit stop authority; trusted watchdog independent.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"device.stop","robot_provider","Requires explicit stop authority; trusted watchdog independent.",7,true},
        /** @brief device.describe: robot provider.
         * @details Actual provider/firmware support, state and watchdog contract.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"device.describe","robot_provider","Actual provider/firmware support, state and watchdog contract.",7,true},
        /** @brief control.open: robot provider.
         * @details Reserve complete continuous cycle and safe-state contract.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"control.open","robot_provider","Reserve complete continuous cycle and safe-state contract.",14,true},
        /** @brief control.sample: robot provider.
         * @details A bounded input/read phase tied to write deadline and sequence.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"control.sample","robot_provider","A bounded input/read phase tied to write deadline and sequence.",14,true},
        /** @brief control.submit: robot provider.
         * @details Check same cycle/sequence/deadline; no attenuation skips an admitted phase.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"control.submit","robot_provider","Check same cycle/sequence/deadline; no attenuation skips an admitted phase.",14,true},
        /** @brief control.result: robot provider.
         * @details Report actual execution, missed phase and watchdog outcome.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"control.result","robot_provider","Report actual execution, missed phase and watchdog outcome.",14,true},
        /** @brief control.stop: robot provider.
         * @details Drain or qualified safe stop; revoke never disables watchdog.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"control.stop","robot_provider","Drain or qualified safe stop; revoke never disables watchdog.",14,true},
        /** @brief media.open: robot provider.
         * @details Separate codec/ingress/egress/viewer and listener profile grants.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"media.open","robot_provider","Separate codec/ingress/egress/viewer and listener profile grants.",8,true},
        /** @brief media.submit: robot provider.
         * @details Bound decoded expansion, encoded output and egress destination.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"media.submit","robot_provider","Bound decoded expansion, encoded output and egress destination.",8,true},
        /** @brief media.take: robot provider.
         * @details Complete bounded encoded/decoded frame under profile.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"media.take","robot_provider","Complete bounded encoded/decoded frame under profile.",8,true},
        /** @brief media.close: robot provider.
         * @details Only owned stream/viewer; no unrelated listener shutdown.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"media.close","robot_provider","Only owned stream/viewer; no unrelated listener shutdown.",8,true},
        /** @brief recording.open: owned creation.
         * @details Private finite volatile MCAP store with exact deployment channel/buffer profiles, caller incarnation ownership, atomic source read/export authority and retained pinned objects. Writer mutations use next-key exact recovery; resource.close releases writer or an unpinned checkpoint. Checkpoint is complete volatile acceptance, not durable storage.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"recording.open","owned_creation","Private finite volatile MCAP store with exact deployment channel/buffer profiles, caller incarnation ownership, atomic source read/export authority and retained pinned objects. Writer mutations use next-key exact recovery; resource.close releases writer or an unpinned checkpoint. Checkpoint is complete volatile acceptance, not durable storage.",25,true},
        /** @brief recording.append: recording transaction.
         * @details Private finite volatile MCAP store with exact deployment channel/buffer profiles, caller incarnation ownership, atomic source read/export authority and retained pinned objects. Writer mutations use next-key exact recovery; resource.close releases writer or an unpinned checkpoint. Checkpoint is complete volatile acceptance, not durable storage.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"recording.append","recording_transaction","Private finite volatile MCAP store with exact deployment channel/buffer profiles, caller incarnation ownership, atomic source read/export authority and retained pinned objects. Writer mutations use next-key exact recovery; resource.close releases writer or an unpinned checkpoint. Checkpoint is complete volatile acceptance, not durable storage.",25,true},
        /** @brief recording.checkpoint: recording transaction.
         * @details Private finite volatile MCAP store with exact deployment channel/buffer profiles, caller incarnation ownership, atomic source read/export authority and retained pinned objects. Writer mutations use next-key exact recovery; resource.close releases writer or an unpinned checkpoint. Checkpoint is complete volatile acceptance, not durable storage.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"recording.checkpoint","recording_transaction","Private finite volatile MCAP store with exact deployment channel/buffer profiles, caller incarnation ownership, atomic source read/export authority and retained pinned objects. Writer mutations use next-key exact recovery; resource.close releases writer or an unpinned checkpoint. Checkpoint is complete volatile acceptance, not durable storage.",25,true},
        /** @brief recording.stop: recording transaction.
         * @details Private finite volatile MCAP store with exact deployment channel/buffer profiles, caller incarnation ownership, atomic source read/export authority and retained pinned objects. Writer mutations use next-key exact recovery; resource.close releases writer or an unpinned checkpoint. Checkpoint is complete volatile acceptance, not durable storage.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"recording.stop","recording_transaction","Private finite volatile MCAP store with exact deployment channel/buffer profiles, caller incarnation ownership, atomic source read/export authority and retained pinned objects. Writer mutations use next-key exact recovery; resource.close releases writer or an unpinned checkpoint. Checkpoint is complete volatile acceptance, not durable storage.",25,true},
        /** @brief replay.open: owned creation.
         * @details Private finite volatile MCAP store with exact deployment channel/buffer profiles, caller incarnation ownership, atomic source read/export authority and retained pinned objects. Replay uses an explicitly granted fixed source and exact object digests; it returns bytes only and never publishes or changes clocks.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"replay.open","owned_creation","Private finite volatile MCAP store with exact deployment channel/buffer profiles, caller incarnation ownership, atomic source read/export authority and retained pinned objects. Replay uses an explicitly granted fixed source and exact object digests; it returns bytes only and never publishes or changes clocks.",25,true},
        /** @brief replay.read: consume after copy.
         * @details Private finite volatile MCAP store with exact deployment channel/buffer profiles, caller incarnation ownership, atomic source read/export authority and retained pinned objects. Replay uses an explicitly granted fixed source and exact object digests; it returns bytes only and never publishes or changes clocks.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"replay.read","consume_after_copy","Private finite volatile MCAP store with exact deployment channel/buffer profiles, caller incarnation ownership, atomic source read/export authority and retained pinned objects. Replay uses an explicitly granted fixed source and exact object digests; it returns bytes only and never publishes or changes clocks.",25,true},
        /** @brief replay.seek: local mutation.
         * @details Private finite volatile MCAP store with exact deployment channel/buffer profiles, caller incarnation ownership, atomic source read/export authority and retained pinned objects. Replay uses an explicitly granted fixed source and exact object digests; it returns bytes only and never publishes or changes clocks.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"replay.seek","local_mutation","Private finite volatile MCAP store with exact deployment channel/buffer profiles, caller incarnation ownership, atomic source read/export authority and retained pinned objects. Replay uses an explicitly granted fixed source and exact object digests; it returns bytes only and never publishes or changes clocks.",25,true},
        /** @brief model.open: owned creation.
         * @details Exact model/provider/task identities and reserved session capacity.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"model.open","owned_creation","Exact model/provider/task identities and reserved session capacity.",18,true},
        /** @brief model.submit: keyed effect.
         * @details Pin immutable inputs, data-egress grants and full task limits before work.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"model.submit","keyed_effect","Pin immutable inputs, data-egress grants and full task limits before work.",18,true},
        /** @brief model.take: consume after copy.
         * @details Streaming/terminal output explicitly distinguished; nonconsuming failed copy.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"model.take","consume_after_copy","Streaming/terminal output explicitly distinguished; nonconsuming failed copy.",18,true},
        /** @brief model.cancel: owned cancellation.
         * @details Cancel request acknowledgement distinct from actual provider termination.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"model.cancel","owned_cancellation","Cancel request acknowledgement distinct from actual provider termination.",18,true},
        /** @brief model.describe: owned observation.
         * @details Actual schema/placement support; no credentials/prompts/payload.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"model.describe","owned_observation","Actual schema/placement support; no credentials/prompts/payload.",18,true},
        /** @brief proposal.read: owned observation.
         * @details Read inert validated model proposal and trusted provenance.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"proposal.read","owned_observation","Read inert validated model proposal and trusted provenance.",18,true},
        /** @brief proposal.accept: proposal pipeline.
         * @details Independent arbiter and exact current target action/argument constraints; cannot broaden rights.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"proposal.accept","proposal_pipeline","Independent arbiter and exact current target action/argument constraints; cannot broaden rights.",18,true},
        /** @brief bridge.create: robot provider.
         * @details Both endpoint directions/types/conversion/QoS preflight independently.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"bridge.create","robot_provider","Both endpoint directions/types/conversion/QoS preflight independently.",5,true},
        /** @brief bridge.start: robot provider.
         * @details Revalidate both grants and generation; no unchecked forwarding thread.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"bridge.start","robot_provider","Revalidate both grants and generation; no unchecked forwarding thread.",5,true},
        /** @brief bridge.stop: robot provider.
         * @details Stop admitted direction, drain/cancel declared in profile.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"bridge.stop","robot_provider","Stop admitted direction, drain/cancel declared in profile.",5,true},
        /** @brief bridge.describe: robot provider.
         * @details Conversion/loss/QoS degradation/loop/correlation state only within scope.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"bridge.describe","robot_provider","Conversion/loss/QoS degradation/loop/correlation state only within scope.",5,true},
        /** @brief log.emit: bounded logging.
         * @details Independent log grant, bounded private content and declared attenuation; no format-string execution.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"log.emit","bounded_logging","Independent log grant, bounded private content and declared attenuation; no format-string execution.",24,true},
        /** @brief diagnostic.read: owned observation.
         * @details Bounded metadata counters, denial/loss/timeout/provider states; content inspection separate.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"diagnostic.read","owned_observation","Bounded metadata counters, denial/loss/timeout/provider states; content inspection separate.",24,true},
        /** @brief request.describe: owned observation.
         * @details No extra native effect; requires origin grant and request ownership.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"request.describe","owned_observation","No extra native effect; requires origin grant and request ownership.",2,true},
        /** @brief request.release: terminal release.
         * @details Only terminal receipt retirement; unresolved effect/cleanup debt not forgiven.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"request.release","terminal_release","Only terminal receipt retirement; unresolved effect/cleanup debt not forgiven.",2,true},
        /** @brief artifact.status: owned observation.
         * @details Explicit cache/volatile-output only. Per-alias next-key ACK and exact last-request replay; full begin allocation remains charged until abort/remove. Publication is immutable session data, never durable. Status and receipt queries do not acknowledge. No host path or descriptor.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"artifact.status","owned_observation","Explicit cache/volatile-output only. Per-alias next-key ACK and exact last-request replay; full begin allocation remains charged until abort/remove. Publication is immutable session data, never durable. Status and receipt queries do not acknowledge. No host path or descriptor.",3,true},
        /** @brief artifact.receipt: owned observation.
         * @details Explicit cache/volatile-output only. Per-alias next-key ACK and exact last-request replay; full begin allocation remains charged until abort/remove. Publication is immutable session data, never durable. Status and receipt queries do not acknowledge. No host path or descriptor.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"artifact.receipt","owned_observation","Explicit cache/volatile-output only. Per-alias next-key ACK and exact last-request replay; full begin allocation remains charged until abort/remove. Publication is immutable session data, never durable. Status and receipt queries do not acknowledge. No host path or descriptor.",3,true},
        /** @brief artifact.publish: cache transaction.
         * @details Publish one complete bounded volatile object using the existing exact replay stream; all publish/begin/append/commit rights and exposures required; scratch capacity reserved before carrier copying.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"artifact.publish","cache_transaction","Publish one complete bounded volatile object using the existing exact replay stream; all publish/begin/append/commit rights and exposures required; scratch capacity reserved before carrier copying.",3,true},
        /** @brief proposal.capture: proposal pipeline.
         * @details Explicit finite capture of actual typed source observations and ordered sealed buffers; exact per-capability key replay. Closing capture releases only its own pins. No model execution or target effect.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"proposal.capture","proposal_pipeline","Explicit finite capture of actual typed source observations and ordered sealed buffers; exact per-capability key replay. Closing capture releases only its own pins. No model execution or target effect.",18,true},
        /** @brief proposal.publish: proposal pipeline.
         * @details Authenticate complete successful terminal output from the actual model request against original captured input IDs, SHA and order. Independently authorize publication; model bytes cannot issue grants or execute tools.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"proposal.publish","proposal_pipeline","Authenticate complete successful terminal output from the actual model request against original captured input IDs, SHA and order. Independently authorize publication; model bytes cannot issue grants or execute tools.",18,true},
        /** @brief communication.open.take: consume after copy.
         * @details Independent exact origin action and complete caller/incarnation checks. Take publishes a typed endpoint only after fresh origin admission; cancelled/unknown work retains native cleanup debt and never refunds creation work.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"communication.open.take","consume_after_copy","Independent exact origin action and complete caller/incarnation checks. Take publishes a typed endpoint only after fresh origin admission; cancelled/unknown work retains native cleanup debt and never refunds creation work.",5,true},
        /** @brief communication.open.cancel: owned cancellation.
         * @details Independent exact origin action and complete caller/incarnation checks. Take publishes a typed endpoint only after fresh origin admission; cancelled/unknown work retains native cleanup debt and never refunds creation work.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"communication.open.cancel","owned_cancellation","Independent exact origin action and complete caller/incarnation checks. Take publishes a typed endpoint only after fresh origin admission; cancelled/unknown work retains native cleanup debt and never refunds creation work.",5,true},
        /** @brief capability.revoke: capability retirement.
         * @details Permanently revoke exactly the carrier capability grant. The target must equal the carrier capability, the exact grant must contain capability.revoke and the import must be exposed. One pre-reserved receipt per grant retains the nonzero retry key, exact caller incarnation and target. Atomically fence admissions before nonblocking provider retirement notifications. Retirement state 1 means fenced with notification pending; 2 means local notifications returned, never proof of native process exit, remote cancellation or physical stop. Exact retry can recover failed output after revocation without repeating notifications or allocating. Closing the handle or retiring the caller removes recovery access. No traffic quota may prevent this independently authorized authority reduction; short output and cancellation reject before fencing. Reopening cannot restore the grant; only trusted deployment creates new authority.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"capability.revoke","capability_retirement","Permanently revoke exactly the carrier capability grant. The target must equal the carrier capability, the exact grant must contain capability.revoke and the import must be exposed. One pre-reserved receipt per grant retains the nonzero retry key, exact caller incarnation and target. Atomically fence admissions before nonblocking provider retirement notifications. Retirement state 1 means fenced with notification pending; 2 means local notifications returned, never proof of native process exit, remote cancellation or physical stop. Exact retry can recover failed output after revocation without repeating notifications or allocating. Closing the handle or retiring the caller removes recovery access. No traffic quota may prevent this independently authorized authority reduction; short output and cancellation reject before fencing. Reopening cannot restore the grant; only trusted deployment creates new authority.",1,true},
        /** @brief topic.reserve: receive reservation.
         * @details Explicit two-phase receive: independently authorize the exact consumer endpoint, retain the original native record and deliver a bounded detached copy without acknowledgement. Same-consumer retry returns the retained record/nonce; concurrent consumers cannot steal it. Both node/context/schema origins and native metadata remain attached.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"topic.reserve","receive_reservation","Explicit two-phase receive: independently authorize the exact consumer endpoint, retain the original native record and deliver a bounded detached copy without acknowledgement. Same-consumer retry returns the retained record/nonce; concurrent consumers cannot steal it. Both node/context/schema origins and native metadata remain attached.",5,true},
        /** @brief topic.acknowledge: receive acknowledgement.
         * @details Consume exactly the previously reserved record after guest processing. Exact replay is idempotent; stale generation, wrong consumer, altered nonce or expired reservation is refused. Lost native ACK is outcome_unknown and never triggers blind re-consumption. Bounded receipt survives failed guest delivery.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"topic.acknowledge","receive_acknowledgement","Consume exactly the previously reserved record after guest processing. Exact replay is idempotent; stale generation, wrong consumer, altered nonce or expired reservation is refused. Lost native ACK is outcome_unknown and never triggers blind re-consumption. Bounded receipt survives failed guest delivery.",5,true},
        /** @brief topic.release: receive release.
         * @details Cancel exactly the owned borrow while retaining the original record. This is not service-request cancellation or endpoint close. Cleanup uses pre-reserved capacity and is not throttled by creation/traffic quotas; uncertain native cancellation retains debt until trusted reconciliation.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"topic.release","receive_release","Cancel exactly the owned borrow while retaining the original record. This is not service-request cancellation or endpoint close. Cleanup uses pre-reserved capacity and is not throttled by creation/traffic quotas; uncertain native cancellation retains debt until trusted reconciliation.",5,true},
        /** @brief service.reserve: receive reservation.
         * @details Explicit two-phase receive: independently authorize the exact consumer endpoint, retain the original native record and deliver a bounded detached copy without acknowledgement. Same-consumer retry returns the retained record/nonce; concurrent consumers cannot steal it. Both node/context/schema origins and native metadata remain attached.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"service.reserve","receive_reservation","Explicit two-phase receive: independently authorize the exact consumer endpoint, retain the original native record and deliver a bounded detached copy without acknowledgement. Same-consumer retry returns the retained record/nonce; concurrent consumers cannot steal it. Both node/context/schema origins and native metadata remain attached.",5,true},
        /** @brief service.acknowledge: receive acknowledgement.
         * @details Consume exactly the previously reserved record after guest processing. Exact replay is idempotent; stale generation, wrong consumer, altered nonce or expired reservation is refused. Lost native ACK is outcome_unknown and never triggers blind re-consumption. Bounded receipt survives failed guest delivery.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"service.acknowledge","receive_acknowledgement","Consume exactly the previously reserved record after guest processing. Exact replay is idempotent; stale generation, wrong consumer, altered nonce or expired reservation is refused. Lost native ACK is outcome_unknown and never triggers blind re-consumption. Bounded receipt survives failed guest delivery.",5,true},
        /** @brief service.release: receive release.
         * @details Cancel exactly the owned borrow while retaining the original record. This is not service-request cancellation or endpoint close. Cleanup uses pre-reserved capacity and is not throttled by creation/traffic quotas; uncertain native cancellation retains debt until trusted reconciliation.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"service.release","receive_release","Cancel exactly the owned borrow while retaining the original record. This is not service-request cancellation or endpoint close. Cleanup uses pre-reserved capacity and is not throttled by creation/traffic quotas; uncertain native cancellation retains debt until trusted reconciliation.",5,true},
        /** @brief parameter.exists: owned observation.
         * @details Authorize the exact resolved key before existence lookup. Read-only existence authority reveals neither value nor descriptor and is independent of get/list. Missing, uninitialized and denied are distinct; denied never returns false as if absent.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"parameter.exists","owned_observation","Authorize the exact resolved key before existence lookup. Read-only existence authority reveals neither value nor descriptor and is independent of get/list. Missing, uninitialized and denied are distinct; denied never returns false as if absent.",5,true},
        /** @brief parameter.search: owned observation.
         * @details ROS1 ancestor search within a deployment-fixed canonical namespace root and finite candidate-count bound, including the starting namespace. Authorize each exact candidate key before existence lookup; return denied at the first unauthorized candidate. Exhaustion returns too_large; only reaching the root with no match returns not-found. One coherent local store snapshot supplies a revision even for negative results. No ambient namespace, filesystem path or unrestricted discovery. This local provider does not impersonate a native parameter server.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"parameter.search","owned_observation","ROS1 ancestor search within a deployment-fixed canonical namespace root and finite candidate-count bound, including the starting namespace. Authorize each exact candidate key before existence lookup; return denied at the first unauthorized candidate. Exhaustion returns too_large; only reaching the root with no match returns not-found. One coherent local store snapshot supplies a revision even for negative results. No ambient namespace, filesystem path or unrestricted discovery. This local provider does not impersonate a native parameter server.",5,true},
        /** @brief action.cancel.set: keyed effect.
         * @details Separately authorized finite explicit cancellation of owned goals. Whole-set identity validation precedes native effects, each candidate has independent authority admission and outcome evidence, and exact retry never reissues a candidate. Unknown stops further attempts; no implicit wildcard or foreign-principal authority.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"action.cancel.set","keyed_effect","Separately authorized finite explicit cancellation of owned goals. Whole-set identity validation precedes native effects, each candidate has independent authority admission and outcome evidence, and exact retry never reissues a candidate. Unknown stops further attempts; no implicit wildcard or foreign-principal authority.",5,true},
        /** @brief service.describe: owned observation.
         * @details Inspect the exact retained native client/server endpoint and each request/response QoS leg. Support/absence is explicit; profile configuration never stands in for actual getter evidence. Observation grants no submission, response, borrow or discovery authority.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"service.describe","owned_observation","Inspect the exact retained native client/server endpoint and each request/response QoS leg. Support/absence is explicit; profile configuration never stands in for actual getter evidence. Observation grants no submission, response, borrow or discovery authority.",5,true},
        /** @brief permission.report: owned observation.
         * @details Self observation defaults to allowed only after explicit ROSv2 binding. Family and exact function denials can only reduce observation; any exact denial refuses the full aggregate to prevent bypass. Explicit hidden removes the import. Returns own identity, exact import exposure, configured/live ROSv2 permissions and own group name without other members. WASI descriptor rows are initial approved rights; ROSv1 rows are configured policy, not a complete live handle snapshot. Carrier capability must be zero. Exact immediate caller identity comes from the runtime. Observation never freezes deployment, enrolls a caller, opens a capability, replenishes or consumes execution quotas, starts a provider, invokes an export, reads foreign memory, or changes another policy. A denial writes neither result nor length. Owned temporary buffers are securely overwritten.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"permission.report","owned_observation","Self observation defaults to allowed only after explicit ROSv2 binding. Family and exact function denials can only reduce observation; any exact denial refuses the full aggregate to prevent bypass. Explicit hidden removes the import. Returns own identity, exact import exposure, configured/live ROSv2 permissions and own group name without other members. WASI descriptor rows are initial approved rights; ROSv1 rows are configured policy, not a complete live handle snapshot. Carrier capability must be zero. Exact immediate caller identity comes from the runtime. Observation never freezes deployment, enrolls a caller, opens a capability, replenishes or consumes execution quotas, starts a provider, invokes an export, reads foreign memory, or changes another policy. A denial writes neither result nor length. Owned temporary buffers are securely overwritten. ",1,true},
        /** @brief module.describe: owned observation.
         * @details Return the exact registered target name and kind: main-wasm, preload-wasm or preload-dl. Default authority is zero: require explicit import exposure and exact module/metadata grant action. The trusted grant fixes the target; guests cannot supply arbitrary target names or wildcards. Carrier capability must be zero. Exact immediate caller identity comes from the runtime. Observation never freezes deployment, enrolls a caller, opens a capability, replenishes or consumes execution quotas, starts a provider, invokes an export, reads foreign memory, or changes another policy. A denial writes neither result nor length. Owned temporary buffers are securely overwritten.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"module.describe","owned_observation","Return the exact registered target name and kind: main-wasm, preload-wasm or preload-dl. Default authority is zero: require explicit import exposure and exact module/metadata grant action. The trusted grant fixes the target; guests cannot supply arbitrary target names or wildcards. Carrier capability must be zero. Exact immediate caller identity comes from the runtime. Observation never freezes deployment, enrolls a caller, opens a capability, replenishes or consumes execution quotas, starts a provider, invokes an export, reads foreign memory, or changes another policy. A denial writes neither result nor length. Owned temporary buffers are securely overwritten. ",1,true},
        /** @brief module.imports: owned observation.
         * @details Return the exact Wasm import module/name pairs; native preloaddl platform imports are unsupported. Default authority is zero: require explicit import exposure and exact module/metadata grant action. The trusted grant fixes the target; guests cannot supply arbitrary target names or wildcards. Carrier capability must be zero. Exact immediate caller identity comes from the runtime. Observation never freezes deployment, enrolls a caller, opens a capability, replenishes or consumes execution quotas, starts a provider, invokes an export, reads foreign memory, or changes another policy. A denial writes neither result nor length. Owned temporary buffers are securely overwritten.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"module.imports","owned_observation","Return the exact Wasm import module/name pairs; native preloaddl platform imports are unsupported. Default authority is zero: require explicit import exposure and exact module/metadata grant action. The trusted grant fixes the target; guests cannot supply arbitrary target names or wildcards. Carrier capability must be zero. Exact immediate caller identity comes from the runtime. Observation never freezes deployment, enrolls a caller, opens a capability, replenishes or consumes execution quotas, starts a provider, invokes an export, reads foreign memory, or changes another policy. A denial writes neither result nor length. Owned temporary buffers are securely overwritten. ",1,true},
        /** @brief module.exports: owned observation.
         * @details Return only export names already recorded in the module registry. Do not resolve or call a symbol. Default authority is zero: require explicit import exposure and exact module/metadata grant action. The trusted grant fixes the target; guests cannot supply arbitrary target names or wildcards. Carrier capability must be zero. Exact immediate caller identity comes from the runtime. Observation never freezes deployment, enrolls a caller, opens a capability, replenishes or consumes execution quotas, starts a provider, invokes an export, reads foreign memory, or changes another policy. A denial writes neither result nor length. Owned temporary buffers are securely overwritten.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"module.exports","owned_observation","Return only export names already recorded in the module registry. Do not resolve or call a symbol. Default authority is zero: require explicit import exposure and exact module/metadata grant action. The trusted grant fixes the target; guests cannot supply arbitrary target names or wildcards. Carrier capability must be zero. Exact immediate caller identity comes from the runtime. Observation never freezes deployment, enrolls a caller, opens a capability, replenishes or consumes execution quotas, starts a provider, invokes an export, reads foreign memory, or changes another policy. A denial writes neither result nor length. Owned temporary buffers are securely overwritten. ",1,true},
        /** @brief module.group: owned observation.
         * @details Return the target ROSv2 group name and its members only under this independent action. Default authority is zero: require explicit import exposure and exact module/metadata grant action. The trusted grant fixes the target; guests cannot supply arbitrary target names or wildcards. Carrier capability must be zero. Exact immediate caller identity comes from the runtime. Observation never freezes deployment, enrolls a caller, opens a capability, replenishes or consumes execution quotas, starts a provider, invokes an export, reads foreign memory, or changes another policy. A denial writes neither result nor length. Owned temporary buffers are securely overwritten.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"module.group","owned_observation","Return the target ROSv2 group name and its members only under this independent action. Default authority is zero: require explicit import exposure and exact module/metadata grant action. The trusted grant fixes the target; guests cannot supply arbitrary target names or wildcards. Carrier capability must be zero. Exact immediate caller identity comes from the runtime. Observation never freezes deployment, enrolls a caller, opens a capability, replenishes or consumes execution quotas, starts a provider, invokes an export, reads foreign memory, or changes another policy. A denial writes neither result nor length. Owned temporary buffers are securely overwritten. ",1,true},
        /** @brief module.permission.report: owned observation.
         * @details Return permissions of the exact registered target for the requested family. The viewer observation policy also applies, including exact-function and aggregate denials. Default authority is zero: require explicit import exposure and exact module/metadata grant action. The trusted grant fixes the target; guests cannot supply arbitrary target names or wildcards. Carrier capability must be zero. Exact immediate caller identity comes from the runtime. Observation never freezes deployment, enrolls a caller, opens a capability, replenishes or consumes execution quotas, starts a provider, invokes an export, reads foreign memory, or changes another policy. A denial writes neither result nor length. Owned temporary buffers are securely overwritten.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"module.permission.report","owned_observation","Return permissions of the exact registered target for the requested family. The viewer observation policy also applies, including exact-function and aggregate denials. Default authority is zero: require explicit import exposure and exact module/metadata grant action. The trusted grant fixes the target; guests cannot supply arbitrary target names or wildcards. Carrier capability must be zero. Exact immediate caller identity comes from the runtime. Observation never freezes deployment, enrolls a caller, opens a capability, replenishes or consumes execution quotas, starts a provider, invokes an export, reads foreign memory, or changes another policy. A denial writes neither result nor length. Owned temporary buffers are securely overwritten. ",1,true},
        /** @brief module.importers: owned observation.
         * @details Return direct registered Wasm importers of the exact target, as importer module/symbol rows. Require an independent module/metadata grant and exact exposure even when the target is the caller. Count all visited modules and imports, including nonmatches, against the finite scan budget. Reexports are not expanded and native dynamic-linker dependencies are not inspected. An explicit grant may also name a registered builtin module. No unrelated peers or their remaining imports are disclosed. Carrier capability must be zero. Exact immediate caller identity comes from the runtime. Observation never freezes deployment, enrolls a caller, opens a capability, replenishes or consumes execution quotas, starts a provider, invokes an export, reads foreign memory, or changes another policy. A denial writes neither result nor length. Owned temporary buffers are securely overwritten.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"module.importers","owned_observation","Return direct registered Wasm importers of the exact target, as importer module/symbol rows. Require an independent module/metadata grant and exact exposure even when the target is the caller. Count all visited modules and imports, including nonmatches, against the finite scan budget. Reexports are not expanded and native dynamic-linker dependencies are not inspected. An explicit grant may also name a registered builtin module. No unrelated peers or their remaining imports are disclosed. Carrier capability must be zero. Exact immediate caller identity comes from the runtime. Observation never freezes deployment, enrolls a caller, opens a capability, replenishes or consumes execution quotas, starts a provider, invokes an export, reads foreign memory, or changes another policy. A denial writes neither result nor length. Owned temporary buffers are securely overwritten. ",1,true},
        /** @brief vm.describe: owned observation.
         * @details Return only closed VM build-support flags, selected compiler, compile-thread ceiling and counts of registered declaration kinds. These are not live thread counts or per-module execution permissions. Require independent vm/runtime/metadata authority and exact exposure. Self permission.report and module grants cannot authorize VM observation. No module names, paths, process IDs, addresses, source bytes or credentials are returned. Carrier capability must be zero. Exact immediate caller identity comes from the runtime. Observation never freezes deployment, enrolls a caller, opens a capability, replenishes or consumes execution quotas, starts a provider, invokes an export, reads foreign memory, or changes another policy. A denial writes neither result nor length. Owned temporary buffers are securely overwritten.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"vm.describe","owned_observation","Return only closed VM build-support flags, selected compiler, compile-thread ceiling and counts of registered declaration kinds. These are not live thread counts or per-module execution permissions. Require independent vm/runtime/metadata authority and exact exposure. Self permission.report and module grants cannot authorize VM observation. No module names, paths, process IDs, addresses, source bytes or credentials are returned. Carrier capability must be zero. Exact immediate caller identity comes from the runtime. Observation never freezes deployment, enrolls a caller, opens a capability, replenishes or consumes execution quotas, starts a provider, invokes an export, reads foreign memory, or changes another policy. A denial writes neither result nor length. Owned temporary buffers are securely overwritten. ",1,true},
        /** @brief module.call.submit: owned creation.
         * @details Require registered server incarnation, fixed caller role and token=0. Reserve a free slot, copy bounded request, return nonzero token. Failed delivery discards the unpublished request. Trusted startup route fixes exact endpoint names, semantic operation, opaque schema identities, finite byte/slot/reservation/deadline ceilings. No arbitrary export invocation. Inputs and registered delivery regions must use owned, unexported shared memory; imported memory, exported memory, nonshared heaps, active threaded host calls and resumable/replay compilation profiles are refused. A live debugger value-observation profile is allowed; it does not grant recording, restoration or message authority. Detached scene queries never copy payloads; separately authorized debugger memory reads remain subject to debugger disclosure policy. Input carrier is copied with byte atomics and zeroed before dispatch by default. Trusted startup bool clear_after_copy defaults to true; false retains sender input bytes, including failure cleanup, without weakening authorization, bounds, tags or broker/delivery erasure. Guest requests cannot change this frozen per-route policy. Retained spans cannot overlap across outstanding calls/routes. Full buffer capacity is overwritten on consume, response, acknowledge, cancel, observed expiry, revoke and retirement. Failed erasure quarantines the slot. Expiry is lazy, not a hard timer; independent guest copies are outside this erasure contract.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"module.call.submit","owned_creation","Require registered server incarnation, fixed caller role and token=0. Reserve a free slot, copy bounded request, return nonzero token. Failed delivery discards the unpublished request. Trusted startup route fixes exact endpoint names, semantic operation, opaque schema identities, finite byte/slot/reservation/deadline ceilings. No arbitrary export invocation. Inputs and registered delivery regions must use owned, unexported shared memory; imported memory, exported memory, nonshared heaps, active threaded host calls and resumable/replay compilation profiles are refused. A live debugger value-observation profile is allowed; it does not grant recording, restoration or message authority. Detached scene queries never copy payloads; separately authorized debugger memory reads remain subject to debugger disclosure policy. Input carrier is copied with byte atomics and zeroed before dispatch by default. Trusted startup bool clear_after_copy defaults to true; false retains sender input bytes, including failure cleanup, without weakening authorization, bounds, tags or broker/delivery erasure. Guest requests cannot change this frozen per-route policy. Retained spans cannot overlap across outstanding calls/routes. Full buffer capacity is overwritten on consume, response, acknowledge, cancel, observed expiry, revoke and retirement. Failed erasure quarantines the slot. Expiry is lazy, not a hard timer; independent guest copies are outside this erasure contract.",1,true},
        /** @brief module.call.take: consume after copy.
         * @details Require exact server role, token=0 and empty payload. First authorized take pins server incarnation even when returning again. Copy queued request once into a registered erasure region; preserve request on failed delivery. Trusted startup route fixes exact endpoint names, semantic operation, opaque schema identities, finite byte/slot/reservation/deadline ceilings. No arbitrary export invocation. Inputs and registered delivery regions must use owned, unexported shared memory; imported memory, exported memory, nonshared heaps, active threaded host calls and resumable/replay compilation profiles are refused. A live debugger value-observation profile is allowed; it does not grant recording, restoration or message authority. Detached scene queries never copy payloads; separately authorized debugger memory reads remain subject to debugger disclosure policy. Input carrier is copied with byte atomics and zeroed before dispatch by default. Trusted startup bool clear_after_copy defaults to true; false retains sender input bytes, including failure cleanup, without weakening authorization, bounds, tags or broker/delivery erasure. Guest requests cannot change this frozen per-route policy. Retained spans cannot overlap across outstanding calls/routes. Full buffer capacity is overwritten on consume, response, acknowledge, cancel, observed expiry, revoke and retirement. Failed erasure quarantines the slot. Expiry is lazy, not a hard timer; independent guest copies are outside this erasure contract.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"module.call.take","consume_after_copy","Require exact server role, token=0 and empty payload. First authorized take pins server incarnation even when returning again. Copy queued request once into a registered erasure region; preserve request on failed delivery. Trusted startup route fixes exact endpoint names, semantic operation, opaque schema identities, finite byte/slot/reservation/deadline ceilings. No arbitrary export invocation. Inputs and registered delivery regions must use owned, unexported shared memory; imported memory, exported memory, nonshared heaps, active threaded host calls and resumable/replay compilation profiles are refused. A live debugger value-observation profile is allowed; it does not grant recording, restoration or message authority. Detached scene queries never copy payloads; separately authorized debugger memory reads remain subject to debugger disclosure policy. Input carrier is copied with byte atomics and zeroed before dispatch by default. Trusted startup bool clear_after_copy defaults to true; false retains sender input bytes, including failure cleanup, without weakening authorization, bounds, tags or broker/delivery erasure. Guest requests cannot change this frozen per-route policy. Retained spans cannot overlap across outstanding calls/routes. Full buffer capacity is overwritten on consume, response, acknowledge, cancel, observed expiry, revoke and retirement. Failed erasure quarantines the slot. Expiry is lazy, not a hard timer; independent guest copies are outside this erasure contract.",1,true},
        /** @brief module.call.respond: callback response.
         * @details Require received request token and exact server incarnation. Copy bounded response and overwrite server delivery region before committing responded state. Lost output does not resend or discard the committed response. Trusted startup route fixes exact endpoint names, semantic operation, opaque schema identities, finite byte/slot/reservation/deadline ceilings. No arbitrary export invocation. Inputs and registered delivery regions must use owned, unexported shared memory; imported memory, exported memory, nonshared heaps, active threaded host calls and resumable/replay compilation profiles are refused. A live debugger value-observation profile is allowed; it does not grant recording, restoration or message authority. Detached scene queries never copy payloads; separately authorized debugger memory reads remain subject to debugger disclosure policy. Input carrier is copied with byte atomics and zeroed before dispatch by default. Trusted startup bool clear_after_copy defaults to true; false retains sender input bytes, including failure cleanup, without weakening authorization, bounds, tags or broker/delivery erasure. Guest requests cannot change this frozen per-route policy. Retained spans cannot overlap across outstanding calls/routes. Full buffer capacity is overwritten on consume, response, acknowledge, cancel, observed expiry, revoke and retirement. Failed erasure quarantines the slot. Expiry is lazy, not a hard timer; independent guest copies are outside this erasure contract.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"module.call.respond","callback_response","Require received request token and exact server incarnation. Copy bounded response and overwrite server delivery region before committing responded state. Lost output does not resend or discard the committed response. Trusted startup route fixes exact endpoint names, semantic operation, opaque schema identities, finite byte/slot/reservation/deadline ceilings. No arbitrary export invocation. Inputs and registered delivery regions must use owned, unexported shared memory; imported memory, exported memory, nonshared heaps, active threaded host calls and resumable/replay compilation profiles are refused. A live debugger value-observation profile is allowed; it does not grant recording, restoration or message authority. Detached scene queries never copy payloads; separately authorized debugger memory reads remain subject to debugger disclosure policy. Input carrier is copied with byte atomics and zeroed before dispatch by default. Trusted startup bool clear_after_copy defaults to true; false retains sender input bytes, including failure cleanup, without weakening authorization, bounds, tags or broker/delivery erasure. Guest requests cannot change this frozen per-route policy. Retained spans cannot overlap across outstanding calls/routes. Full buffer capacity is overwritten on consume, response, acknowledge, cancel, observed expiry, revoke and retirement. Failed erasure quarantines the slot. Expiry is lazy, not a hard timer; independent guest copies are outside this erasure contract.",1,true},
        /** @brief module.call.result: consume after copy.
         * @details Require exact original client token and empty payload. Copy committed response once and retain client erasure region until acknowledge. Pending returns again; duplicate consumption conflicts. Trusted startup route fixes exact endpoint names, semantic operation, opaque schema identities, finite byte/slot/reservation/deadline ceilings. No arbitrary export invocation. Inputs and registered delivery regions must use owned, unexported shared memory; imported memory, exported memory, nonshared heaps, active threaded host calls and resumable/replay compilation profiles are refused. A live debugger value-observation profile is allowed; it does not grant recording, restoration or message authority. Detached scene queries never copy payloads; separately authorized debugger memory reads remain subject to debugger disclosure policy. Input carrier is copied with byte atomics and zeroed before dispatch by default. Trusted startup bool clear_after_copy defaults to true; false retains sender input bytes, including failure cleanup, without weakening authorization, bounds, tags or broker/delivery erasure. Guest requests cannot change this frozen per-route policy. Retained spans cannot overlap across outstanding calls/routes. Full buffer capacity is overwritten on consume, response, acknowledge, cancel, observed expiry, revoke and retirement. Failed erasure quarantines the slot. Expiry is lazy, not a hard timer; independent guest copies are outside this erasure contract.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"module.call.result","consume_after_copy","Require exact original client token and empty payload. Copy committed response once and retain client erasure region until acknowledge. Pending returns again; duplicate consumption conflicts. Trusted startup route fixes exact endpoint names, semantic operation, opaque schema identities, finite byte/slot/reservation/deadline ceilings. No arbitrary export invocation. Inputs and registered delivery regions must use owned, unexported shared memory; imported memory, exported memory, nonshared heaps, active threaded host calls and resumable/replay compilation profiles are refused. A live debugger value-observation profile is allowed; it does not grant recording, restoration or message authority. Detached scene queries never copy payloads; separately authorized debugger memory reads remain subject to debugger disclosure policy. Input carrier is copied with byte atomics and zeroed before dispatch by default. Trusted startup bool clear_after_copy defaults to true; false retains sender input bytes, including failure cleanup, without weakening authorization, bounds, tags or broker/delivery erasure. Guest requests cannot change this frozen per-route policy. Retained spans cannot overlap across outstanding calls/routes. Full buffer capacity is overwritten on consume, response, acknowledge, cancel, observed expiry, revoke and retirement. Failed erasure quarantines the slot. Expiry is lazy, not a hard timer; independent guest copies are outside this erasure contract.",1,true},
        /** @brief module.call.acknowledge: receive acknowledgement.
         * @details Require delivered result and empty payload. Overwrite every retained region and host capacity before reusing slot. Exact retry with unchanged retained token carries no new data. Cleanup bypasses traffic quotas but requires authority. Trusted startup route fixes exact endpoint names, semantic operation, opaque schema identities, finite byte/slot/reservation/deadline ceilings. No arbitrary export invocation. Inputs and registered delivery regions must use owned, unexported shared memory; imported memory, exported memory, nonshared heaps, active threaded host calls and resumable/replay compilation profiles are refused. A live debugger value-observation profile is allowed; it does not grant recording, restoration or message authority. Detached scene queries never copy payloads; separately authorized debugger memory reads remain subject to debugger disclosure policy. Input carrier is copied with byte atomics and zeroed before dispatch by default. Trusted startup bool clear_after_copy defaults to true; false retains sender input bytes, including failure cleanup, without weakening authorization, bounds, tags or broker/delivery erasure. Guest requests cannot change this frozen per-route policy. Retained spans cannot overlap across outstanding calls/routes. Full buffer capacity is overwritten on consume, response, acknowledge, cancel, observed expiry, revoke and retirement. Failed erasure quarantines the slot. Expiry is lazy, not a hard timer; independent guest copies are outside this erasure contract.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"module.call.acknowledge","receive_acknowledgement","Require delivered result and empty payload. Overwrite every retained region and host capacity before reusing slot. Exact retry with unchanged retained token carries no new data. Cleanup bypasses traffic quotas but requires authority. Trusted startup route fixes exact endpoint names, semantic operation, opaque schema identities, finite byte/slot/reservation/deadline ceilings. No arbitrary export invocation. Inputs and registered delivery regions must use owned, unexported shared memory; imported memory, exported memory, nonshared heaps, active threaded host calls and resumable/replay compilation profiles are refused. A live debugger value-observation profile is allowed; it does not grant recording, restoration or message authority. Detached scene queries never copy payloads; separately authorized debugger memory reads remain subject to debugger disclosure policy. Input carrier is copied with byte atomics and zeroed before dispatch by default. Trusted startup bool clear_after_copy defaults to true; false retains sender input bytes, including failure cleanup, without weakening authorization, bounds, tags or broker/delivery erasure. Guest requests cannot change this frozen per-route policy. Retained spans cannot overlap across outstanding calls/routes. Full buffer capacity is overwritten on consume, response, acknowledge, cancel, observed expiry, revoke and retirement. Failed erasure quarantines the slot. Expiry is lazy, not a hard timer; independent guest copies are outside this erasure contract.",1,true},
        /** @brief module.call.cancel: owned cancellation.
         * @details Either registered endpoint may cancel its exact token with empty payload. Overwrite both delivery regions and host capacity. No foreign policy mutation; cleanup bypasses traffic quotas but requires authority. Trusted startup route fixes exact endpoint names, semantic operation, opaque schema identities, finite byte/slot/reservation/deadline ceilings. No arbitrary export invocation. Inputs and registered delivery regions must use owned, unexported shared memory; imported memory, exported memory, nonshared heaps, active threaded host calls and resumable/replay compilation profiles are refused. A live debugger value-observation profile is allowed; it does not grant recording, restoration or message authority. Detached scene queries never copy payloads; separately authorized debugger memory reads remain subject to debugger disclosure policy. Input carrier is copied with byte atomics and zeroed before dispatch by default. Trusted startup bool clear_after_copy defaults to true; false retains sender input bytes, including failure cleanup, without weakening authorization, bounds, tags or broker/delivery erasure. Guest requests cannot change this frozen per-route policy. Retained spans cannot overlap across outstanding calls/routes. Full buffer capacity is overwritten on consume, response, acknowledge, cancel, observed expiry, revoke and retirement. Failed erasure quarantines the slot. Expiry is lazy, not a hard timer; independent guest copies are outside this erasure contract.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"module.call.cancel","owned_cancellation","Either registered endpoint may cancel its exact token with empty payload. Overwrite both delivery regions and host capacity. No foreign policy mutation; cleanup bypasses traffic quotas but requires authority. Trusted startup route fixes exact endpoint names, semantic operation, opaque schema identities, finite byte/slot/reservation/deadline ceilings. No arbitrary export invocation. Inputs and registered delivery regions must use owned, unexported shared memory; imported memory, exported memory, nonshared heaps, active threaded host calls and resumable/replay compilation profiles are refused. A live debugger value-observation profile is allowed; it does not grant recording, restoration or message authority. Detached scene queries never copy payloads; separately authorized debugger memory reads remain subject to debugger disclosure policy. Input carrier is copied with byte atomics and zeroed before dispatch by default. Trusted startup bool clear_after_copy defaults to true; false retains sender input bytes, including failure cleanup, without weakening authorization, bounds, tags or broker/delivery erasure. Guest requests cannot change this frozen per-route policy. Retained spans cannot overlap across outstanding calls/routes. Full buffer capacity is overwritten on consume, response, acknowledge, cancel, observed expiry, revoke and retirement. Failed erasure quarantines the slot. Expiry is lazy, not a hard timer; independent guest copies are outside this erasure contract.",1,true},
        /** @brief driver.connect: local mutation.
         * @details Perform one nonblocking handshake on a registered inherited channel. Readiness grants no actuation authority; repeated handshake polling never launches code.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"driver.connect","local_mutation","Perform one nonblocking handshake on a registered inherited channel. Readiness grants no actuation authority; repeated handshake polling never launches code.",6,true},
        /** @brief driver.submit: keyed effect.
         * @details Submit one bounded fixed-contract request. Exact in-flight retry returns its receipt without resending. Changed bytes or deadline conflict; submission is local transport acceptance, never physical completion.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"driver.submit","keyed_effect","Submit one bounded fixed-contract request. Exact in-flight retry returns its receipt without resending. Changed bytes or deadline conflict; submission is local transport acceptance, never physical completion.",6,true},
        /** @brief driver.result: consume after copy.
         * @details Receive an exact sequence and incarnation-bound result. Failed guest delivery retains bytes. Timeout after send fences the session with effect_unknown; never retry a non-idempotent operation automatically.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"driver.result","consume_after_copy","Receive an exact sequence and incarnation-bound result. Failed guest delivery retains bytes. Timeout after send fences the session with effect_unknown; never retry a non-idempotent operation automatically.",6,true},
        /** @brief driver.describe: owned observation.
         * @details Read detached session metadata only; never pump packets, infer physical safety, restart a driver or replenish authority.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"driver.describe","owned_observation","Read detached session metadata only; never pump packets, infer physical safety, restart a driver or replenish authority.",6,true},
        /** @brief driver.disconnect: owned close.
         * @details Release this connection and erase retained buffers. Best-effort release and local descriptor closure never terminate a shared driver. Independent receiver expiry remains mandatory.
         * Apply the complete origin, bounded delivery and recovery contract;
         * this descriptor neither exposes the operation nor grants authority.
         */
        descriptor{"driver.disconnect","owned_close","Release this connection and erase retained buffers. Best-effort release and local descriptor closure never terminate a shared driver. Independent receiver expiry remains mandatory.",6,true},
    };
}
