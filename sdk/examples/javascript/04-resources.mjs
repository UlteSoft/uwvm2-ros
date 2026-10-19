/** Lesson 4: typed services, parameters, clocks and exact-operation resources.
 * Every alias in this lesson is an example policy name, not an implicit native
 * ROS name. Configure exact resources/types/identities/limits outside the guest.
 */
import { Node, Session, Parameters, Clock, Lifecycle, Tf } from '../../javascript/roswasm.mjs';
import { Parameter } from '../../javascript/codecs.mjs';

/** Use explicitly granted ROS 2 aliases once, retaining all runtime status codes.
 * @param {object} bridge Trusted authority-aware bridge, never a host ROS mock.
 * @param {object} scratch Exclusive caller-reserved scratch region.
 * @returns {object} Service/parameter/clock/lifecycle/TF results; no operation is retried.
 */
export function resourceOnce(bridge, scratch) {
  const node = new Node(new Session(2, bridge, scratch), 'resource-example');
  let result,error;
  try {
    const service = node.createClient('trigger-call');
    const parameters = new Parameters({ get: node.open('counter-get'), set: node.open('counter-set') }, Parameter.Int64);
    const clock = new Clock(node.open('clock-ros'), 'time_read_ros');
    const lifecycle = new Lifecycle(node.open('lifecycle-state'));
    const tf = new Tf(node.open('map-to-base-lookup'));
    result = { service: service.call(), parameter: parameters.get(), time: clock.now(),
      state: lifecycle.getState(), transform: tf.lookup(0n) };
  } catch(failure) {error=failure;}
  let closeStatuses;
  try {closeStatuses=node.close();}catch(cleanup){if(!error)throw cleanup;error.cleanupFailures=cleanup;}
  if(error){if(closeStatuses)error.closeStatuses=closeStatuses;throw error;}
  return {...result,closeStatuses};
}
