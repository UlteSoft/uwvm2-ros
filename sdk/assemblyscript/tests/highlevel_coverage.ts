/** Prepared semantic fixture making the managed high-level APIs reachable.
 * The Api base returns unsupported and creates no native ROS resources. This
 * fixture is for compiler coverage and bounded codec tests, never a ROS behavior
 * verdict. Actual compilation/execution must occur in the authorized cgroup.
 */
import { Api, Operation } from '../api';
import { Node, Executor, Service, ActionClient, ActionServer, Parameters, Graph,
  Clock, Logger, Tf, Lifecycle } from '../roswasm';
import { StringCodec, BoolCodec, TwistCodec, Twist, ParameterKind, ParameterValue,
  ServiceRequest, ServiceResponse, IncomingGoal, ActionSequence, Transform } from '../codecs';
export { abortGuest } from '../api';

/** Stateless guest-local String callback; no native code receives a function pointer. */
function receiveString(value: string): void {}
/** Stateless guest-local Bool callback for typed generic instantiation coverage. */
function receiveBool(value: bool): void {}
/** Stateless guest-local Twist callback for typed generic instantiation coverage. */
function receiveTwist(value: Twist): void {}
/** Return one owned-ID service response in the prepared fixture. */
function serve(request: ServiceRequest): ServiceResponse { return new ServiceResponse(true, 'ok'); }
/** Return one terminal Action result preserving the incoming owned ID. */
function act(goal: IncomingGoal): ActionSequence { return new ActionSequence(goal.id, 2, [0, 1]); }

/** Make all high-level methods reachable; base Api guarantees no native resource calls. */
export function _start(): void {
  const node = new Node(new Api(2), 'semantic-fixture');
  const stringPublisher = node.createPublisher<string>('string-pub', new StringCodec()); stringPublisher.publish('x'); stringPublisher.close();
  const boolPublisher = node.createPublisher<bool>('bool-pub', new BoolCodec()); boolPublisher.publish(true); boolPublisher.close();
  const twistPublisher = node.createPublisher<Twist>('twist-pub', new TwistCodec()); twistPublisher.publish(new Twist()); twistPublisher.close();
  const executor = new Executor();
  executor.add(node.createSubscription<string>('string-sub', new StringCodec(), receiveString));
  executor.add(node.createSubscription<bool>('bool-sub', new BoolCodec(), receiveBool));
  executor.add(node.createSubscription<Twist>('twist-sub', new TwistCodec(), receiveTwist)); executor.spinOnce();
  const client = node.createClient('call'); client.response(client.trigger()); client.response(client.setBool(true)); client.close();
  const service = new Service(node.open('advertise'), node.open('respond'), serve); service.spinOnce(); service.close();
  const action = new ActionClient(node.open('send'), node.open('cancel'), node.open('feedback'), node.open('result'));
  action.goalId(action.sendGoal(5)); action.cancelOwn(1); action.cancelAny(1); action.cancelAll(); action.feedback(1); action.result(1); action.close();
  const server = new ActionServer(node.open('action-advertise'), node.open('action-respond'), act); server.spinOnce(); server.close();
  const parameter = new Parameters(node.open('get'), Operation.parameter_get, ParameterKind.Int64); parameter.get(); parameter.decode(parameter.getReply()); parameter.readTextReply(); parameter.set(new ParameterValue(ParameterKind.Int64)); parameter.readText(); parameter.subscribeEvents(); parameter.pollEvent(); parameter.close();
  parameter.declareValue(new ParameterValue(ParameterKind.Int64)); parameter.delete(); parameter.undeclare(); parameter.exists(); parameter.list(); parameter.describe(); parameter.search(); parameter.pollEventReply();
  const graph = new Graph(node.open('graph'), Operation.graph_list_topics); graph.text(graph.readText()); graph.close();
  const clock = new Clock(node.open('clock'), Operation.time_read_ros); clock.nanoseconds(clock.now()); clock.close();
  const logger = node.createLogger('logger'); logger.write('x'); logger.close();
  const tf = new Tf(node.open('tf'), Operation.tf_lookup); tf.transform(tf.lookup()); tf.broadcast(new Transform()); tf.close();
  const lifecycle = new Lifecycle(node.open('lifecycle'), Operation.lifecycle_get_state); lifecycle.getState(); lifecycle.changeState(); lifecycle.close();
  lifecycle.getStateReply();
  const resource = node.createResource('resource', Operation.graph_list_services); resource.call(); resource.close();
  const capability = node.open('capability'); capability.isOpen(); capability.revoke(); capability.subscribe(); capability.advertiseService(); capability.subscribeParameterEvents(); capability.poll(); capability.close();
  node.createService('ad2', 'resp2', serve).spinOnce();
  node.createActionClient('send2', 'cancel2', 'feed2', 'result2').sendGoal(1);
  node.createActionServer('aad2', 'aresp2', act).spinOnce();
  node.createParameters('param2', Operation.parameter_get, ParameterKind.String).get();
  node.createGraph('graph2', Operation.graph_list_nodes).readText();
  node.createClock('clock2').now();
  node.createTf('tf2', Operation.tf_lookup).lookup();
  node.createLifecycle('life2', Operation.lifecycle_get_state).getState();
  node.openFor('unsupported', Operation.lifecycle_get_state);
  node.close();
}
