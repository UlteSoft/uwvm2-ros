/** Lesson 02: real guest JS Node, typed pub/sub and JavaScript receive callback.
 * Explicitly grant aliases pub/sub to separate reviewed String endpoints. The
 * supervisor must independently observe native discovery before sending P,
 * then send at most200 R bytes, one per requested poll. It must independently
 * verify final native bytes/writer and the default-denied companion case.
 * The callback below runs in this Wasm's JS heap, after copied polling returns.
 */
import { createGuestSession, Node, Executor, StringMessage, Status } from '../../javascript/guest.mjs';
import { check, marker, gate, status, clean } from './support.mjs';
export function main() {
  const session = createGuestSession(), node = new Node(session, 'js-guest');
  const executor = new Executor(); let received = 0;
  try {
    const publisher = node.createPublisher('pub', StringMessage);
    const subscription = node.createSubscription('sub', StringMessage, value => {
      check(value === 'roswasm-js-relay-in' && ++received === 1, 'Exact one native reply');
    });
    executor.add(subscription);
    status(publisher.publish('warmup-js-relay')); marker('JS_GUEST_ENDPOINTS_CREATED'); gate('P');
    status(publisher.publish('roswasm-js-relay-out')); marker('JS_GUEST_FINAL_SENT');
    for (let polls = 0; polls < 200 && !received; ++polls) {
      gate('R'); const result = executor.spinOnce();
      check(result.length === 1 && (result[0] === 0 || result[0] === Status.AGAIN), 'One checked poll');
      if (result[0] === Status.AGAIN) marker('JS_GUEST_POLL_AGAIN');
    }
    check(received === 1, 'Bounded native reply missing');
    const old = publisher.capability.handle;
    status(publisher.close()); status(subscription.close());
    check(session.write('topic_publish', old, StringMessage.encode('roswasm-js-relay-out')) === 9, 'Actual stale handle');
    clean(node.close()); marker('PASS_JS_GUEST_DELIVERY'); return 0;
  } finally { executor.close(); clean(node.close()); }
}
