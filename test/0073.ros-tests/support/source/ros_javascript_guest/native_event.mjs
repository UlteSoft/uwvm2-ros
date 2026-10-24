/** Real topic-message and resource-closed events inside embedded guest JS.
 * Build this entry separately for ROS1 and ROS2 using event_manifest.py and
 * the existing pinned embedded-engine producer. It is an additional positive
 * fixture; the original lesson08 keeps its narrower ownership-only behavior.
 *
 * Explicit aliases source/observer are separate Runtime capabilities. The
 * owner waits for independent native discovery, commands that peer to publish
 * exactly once, then supplies P and at most200 R bytes per event phase. Guest
 * code supplies neither a native participant nor a clock/deadline. A successful
 * publish return, copied model, Node engine or empty application cannot pass.
 */
import { createGuestSession, Status, StringMessage } from '../../sdk/origin/javascript/guest.mjs';
import { EventBinding, EventKind, decodeEventReadiness } from '../../sdk/origin/javascript/events.mjs';
import { check, status, marker, gate } from '../../sdk/origin/examples/javascript_guest/support.mjs';

export function main() {
  const session = createGuestSession();
  let source, observer;
  try {
    source = session.open('source');
    observer = session.open('observer');
    status(session.raw.topic_subscribe(source.handle));
    status(observer.watchEvents(source));
    const binding = new EventBinding(session.version, source.handle);
    const initial = observer.pollEventV1();
    check(initial.status === Status.AGAIN, 'No event before independent native publication');
    marker('JS_NATIVE_EVENTS_READY'); gate('P');
    let delivered = false;
    for (let attempt = 0; attempt < 200; ++attempt) {
      gate('R');
      const readiness = observer.eventReadiness(); status(readiness);
      const counters = decodeEventReadiness(readiness.bytes);
      check(counters.nextEventBytes <= 4194304n, 'Bounded readiness snapshot');
      const result = observer.pollEventV1();
      if (result.status === Status.AGAIN) { marker('JS_NATIVE_EVENTS_AGAIN'); continue; }
      status(result);
      const event = binding.decode(result.bytes);
      check(event.kind === EventKind.TOPIC_MESSAGE && event.error === 0 && event.flags === 0 &&
        StringMessage.decode(event.body) === 'roswasm-js-event', 'Exact native topic event');
      delivered = true; marker('JS_NATIVE_EVENTS_DATA'); break;
    }
    check(delivered, 'Independent native topic event was not delivered');
    status(source.close());
    let closed = false;
    for (let attempt = 0; attempt < 200; ++attempt) {
      gate('R'); const result = observer.pollEventV1();
      if (result.status === Status.AGAIN) { marker('JS_NATIVE_EVENTS_AGAIN'); continue; }
      status(result); const event = binding.decode(result.bytes);
      check(event.kind === EventKind.RESOURCE_CLOSED && event.body.length === 0 &&
        (event.flags & 1) !== 0, 'Exact source retirement event');
      closed = true; break;
    }
    check(closed, 'Resource-closed event missing');
    status(observer.close());
    marker('PASS_JS_NATIVE_EVENTS'); return 0;
  } finally {
    for (const capability of [source, observer]) if (capability && !capability.closed) status(capability.close());
  }
}
