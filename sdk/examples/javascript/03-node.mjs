/** Lesson 3: familiar Node/publisher/subscription/executor concepts.
 * The embedder supplies an authenticated guest bridge and an exclusively reserved
 * scratch region. The CLI must provision greeting-pub and greeting-sub separately.
 * Calling this example without those grants returns/throws status 13 by design.
 */
import { Session, Node, Executor, StringMessage } from '../../javascript/roswasm.mjs';

/** Publish one greeting and poll one subscription event without native callbacks.
 * @param {object} bridge Trusted bridge described in sdk/javascript/README.md.
 * @param {object} scratch Caller-reserved {offset: BigInt, inputCapacity, outputCapacity}.
 * @param {function} receive Guest/embedder-local callback; it runs only during spinOnce.
 * @returns {object} Publish/poll/close statuses. No hidden sleep, clock grant or retry occurs.
 */
export function greetingOnce(bridge, scratch, receive) {
  const session = new Session(2, bridge, scratch), node = new Node(session, 'greeting');
  let result,error;
  try {
    const publisher = node.createPublisher('greeting-pub', StringMessage);
    const subscription = node.createSubscription('greeting-sub', StringMessage, receive);
    const executor = new Executor(); executor.add(subscription);
    result = { publishStatus: publisher.publish('hello from roswasm'), pollStatuses: executor.spinOnce() };
  } catch (failure) { error=failure; }
  let closeStatuses;
  try { closeStatuses=node.close(); } catch (cleanup) { if(!error)throw cleanup;error.cleanupFailures=cleanup; }
  if(error) {if(closeStatuses)error.closeStatuses=closeStatuses;throw error;}
  return {...result,closeStatuses};
}
