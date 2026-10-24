#!/usr/bin/env python3
"""Review source and independent local-event contracts without native execution.

This script compiles no language, invokes no executable, creates no ROS context
and does not instantiate Wasm. Its wire encoder and authorization/lifetime model
are independent Python oracles. Actual C++/IPC/ROS/ARM results require separate
Linux 64-GiB cgroup fixture reports. A fresh output path is mandatory.
"""
import argparse
import dataclasses
import gc
import hashlib
import json
import math
from pathlib import Path
import struct
import sys
import weakref

sys.dont_write_bytecode = True
ROOT = Path(__file__).resolve().parents[5]
CHECKS = []


def check(condition, name):
    if not condition:
        raise AssertionError(name)
    CHECKS.append(name)


def text(value):
    encoded = value.encode('utf-8')
    return struct.pack('<I', len(encoded)) + encoded


def wire(tag, kind, node, name, value):
    """Literal ABI table: version2, tags0..9 and empty deletion payload."""
    payload = b''
    if kind != 3:
        if tag == 1:
            payload = bytes([int(value)])
        elif tag == 2:
            payload = struct.pack('<q', value)
        elif tag == 3:
            payload = struct.pack('<d', value)
        elif tag == 4:
            payload = text(value)
        elif tag in (5, 6):
            payload = struct.pack('<I', len(value)) + bytes(value)
        elif tag in (7, 8):
            fmt = '<q' if tag == 7 else '<d'
            payload = struct.pack('<I', len(value)) + b''.join(struct.pack(fmt, item) for item in value)
        elif tag == 9:
            payload = struct.pack('<I', len(value)) + b''.join(text(item) for item in value)
        else:
            raise ValueError('unsupported non-deletion tag')
    return struct.pack('<IIBBB', 123, 456, kind, 2, tag) + text(node) + text(name) + struct.pack('<I', len(payload)) + payload


@dataclasses.dataclass
class Grant:
    node: str
    name: str
    tag: int
    maximum: int = 4096
    elements: int = 8
    low: float = -1e20
    high: float = 1e20
    depth: int = 32
    queue_bytes: int = 65536
    overflow: str = 'reject'
    burst: int = 32


class Node:
    def __init__(self, name, native_publication=False):
        self.name, self.native_publication = name, native_publication


class Queue:
    def __init__(self, grant):
        self.grant, self.active, self.events = grant, True, []
        self.tokens = grant.burst

    def close(self):
        self.active = False
        self.events.clear()

    def deliver(self, node, name, tag, kind, value):
        c = self.grant
        if not self.active or c.node != node or c.name != name:
            return
        if c.tag == 0:
            if kind != 3:
                return
            selected = 0
        else:
            if c.tag != tag:
                return
            selected = tag
            if tag >= 5 and len(value) > c.elements:
                return
            numbers = value if tag in (7, 8) else [value] if tag in (2, 3) else []
            if any(not math.isfinite(number) or number < c.low or number > c.high for number in numbers):
                return
            if tag == 9 and any('\0' in item for item in value):
                return
        packet = wire(selected, kind, node, name, value)
        if len(packet) > c.maximum or not self.tokens:
            return
        self.tokens -= 1
        while len(self.events) >= c.depth or sum(map(len, self.events)) + len(packet) > c.queue_bytes:
            if c.overflow != 'drop-oldest' or not self.events:
                return
            self.events.pop(0)
        self.events.append(packet)


class Registry:
    """Small fixed test ceiling; all retained identities are weak references."""
    def __init__(self, maximum=32):
        self.maximum, self.routes = maximum, []

    def prune(self):
        self.routes = [(n, q) for n, q in self.routes if n() is not None and q() is not None and q().active]

    def activate(self, node, queue, verified=True):
        if not verified:
            return False
        self.prune()
        if node.native_publication or node.name != queue.grant.node:
            return True
        if any(n() is node and q() is queue for n, q in self.routes):
            return True
        if len(self.routes) >= self.maximum:
            return False
        self.routes.append((weakref.ref(node), weakref.ref(queue)))
        return True

    def committed(self, node, name, tag, kind, value):
        self.prune()
        for n, q in self.routes:
            if n() is node:
                q().deliver(node.name, name, tag, kind, value)


def source_checks():
    production = (ROOT/'src/ros/gateway/ros2/resources.h').read_text()
    main = (ROOT/'src/ros/gateway/ros2/main.cc').read_text()
    native = (ROOT/'test/0073.ros-tests/support/source/ros_local_parameter_events/native_ros2.cc').read_text()
    contract = (ROOT/'test/0073.ros-tests/support/source/ros_local_parameter_events/contract_ros2.cc').read_text()
    common = (ROOT/'test/0073.ros-tests/support/source/ros_local_parameter_events/common.h').read_text()
    check('start_parameter_event_publisher(false)' in main, 'production native publisher remains disabled')
    check('maximum_routes{65536}' in production, 'explicit weak-route memory ceiling')
    check('std::weak_ptr<::rclcpp::Node> node;std::weak_ptr<event_queue> queue' in production, 'registry owns only weak identities')
    activation = production[production.index('inline ipc::call_result subscribe_events'):production.index('inline ipc::call_result string_result')]
    check(activation.index('ros2_qos::equal') < activation.index('register_local_parameter_events'), 'full actual QoS verification precedes local activation')
    check('if(!verified->load(std::memory_order_acquire)) { return; }' in activation and activation.index('ros2_qos::equal') < activation.index('verified->store(true'), 'native DDS callback is gated until complete verified activation')
    check('queue->capability.limits!=cap.limits' in activation and 'queue->capability.type_identity!=cap.type_identity' in activation, 'activation checks exact queue grant')
    check('subscription.reset();' in activation and 'trigger_notify_guard_condition()' in activation and 'return refuse(95)' in activation and 'return refuse(90)' in activation, 'refused native subscription wakes executor and retains original error')
    check('catch(...) { discard();throw; }' in activation, 'QoS or registry exception clears native owner before original exception propagates')
    check('registry.prune();' in production and '!active_parameter_event_queue(*queue)' in production, 'inactive and expired routes pruned')
    check('start_parameter_event_publisher()' in production, 'automatic native publication excludes memory route')
    check('void notify_local_parameter_event' in production and 'value) noexcept' in production, 'post-commit dispatch cannot throw into RPC')
    check('state=owned' in production and 'std::this_thread::get_id()!=state->writer' in production, 'snapshot state owned and caller-thread isolated')
    check('snapshot_native_parameter(value.get_parameter_value(),state->value)' in production and 'state->ready=false' in production, 'snapshot failure drops event')
    check('remove_post_set_parameters_callback(callback.get())' in production, 'failed mutation removes temporary callback entry')
    check('values.size()>ipc::parameter_max_elements' in production and 'text.size()>ipc::parameter_wire_limit-4' in production, 'snapshot bounds precede active-field copy')
    check('auto const& declared{node->declare_parameter' not in production, 'no borrowed map-owned declaration result retained')
    projection = production[production.index('inline std::uint32_t project_event_value(ros::capability_spec const& cap,::rclcpp::ParameterValue'):production.index('inline std::uint8_t event_native_type')]
    check('to_value_msg()' not in projection and 'native.get<::rclcpp::ParameterType::PARAMETER_BOOL>()' in projection, 'native scalar projection never copies inactive generated fields')
    for operation, commit, kind in [('param.declare','node->declare_parameter',1),('param.set','node->set_parameter',2),('param.undeclare','node->undeclare_parameter',3)]:
        anchor = production.index('if(cap.operation=="'+operation+'")', production.index('if(remote==node->get_fully_qualified_name())'))
        fragment = production[anchor:]
        check(fragment.index(commit) < fragment.index('notify_local_parameter_event(node,name,'+str(kind)), operation+' emits after native commit')
    check('committed.owned->ready' in production, 'no requested-value fallback for missing actual snapshot')
    for token in ['bool','int64','float64','string','byte-array','bool-array','int64-array','float64-array','string-array']:
        check('"'+token+'"' in common, 'independent native fixture inventory '+token)
    for token in ['declaration event carries actual native override','committed native set still emits exactly once after RPC timeout','other native writer is neither captured','set event snapshots actual native pre-set value','oversized actual String is refused','copies only active Bool']:
        check(token in contract, 'prepared actual native contract '+token)
    check('one mutation produces precisely one local event' in common and 'same shared-Node mutation has identical bytes' in native, 'independent complete wire and cross-session oracle prepared')
    check('writer_ids()==writers' in native and '!native.gateway_events' in native, 'prepared actual native GID and traffic observations')


def model_checks():
    node = Node('/local_parameter_events/gateway')
    samples = {1:True,2:9007199254740993,3:1.25,4:'\ufeff中',5:[0,127,128,255],6:[0,1,0],7:[-7,9007199254740993],8:[-0.0,1.25],9:['','中']}
    for tag, value in samples.items():
        registry = Registry();a = Queue(Grant(node.name,'value',tag));b = Queue(Grant(node.name,'value',tag));deleted = Queue(Grant(node.name,'value',0))
        registry.committed(node,'value',tag,1,value)
        check(not a.events and not b.events, 'grant presence alone is inactive tag'+str(tag))
        check(not registry.activate(node,a,False) and not registry.routes, 'failed QoS cannot activate tag'+str(tag))
        for queue in (a,b,deleted):
            check(registry.activate(node,queue), 'explicit independent queue activation tag'+str(tag))
        check(registry.activate(node,a) and len(registry.routes)==3, 'same queue identity cannot duplicate tag'+str(tag))
        for kind in (1,2,3):
            registry.committed(node,'value',tag,kind,value)
            check(a.events[-1]==b.events[-1]==wire(tag,kind,node.name,'value',value), 'two-session whole golden tag'+str(tag)+' kind'+str(kind))
            if kind != 3:
                check(not deleted.events, 'tag0 excludes nondeletion tag'+str(tag)+' kind'+str(kind))
        check(deleted.events==[wire(0,3,node.name,'value',None)], 'deletion-only omits prior metadata tag'+str(tag))
        a.close();registry.committed(node,'value',tag,2,value)
        check(not a.events and len(b.events)==4 and len(registry.routes)==2, 'retired owner does not retire sibling tag'+str(tag))
    registry = Registry(2);a = Queue(Grant(node.name,'value',1));b = Queue(Grant(node.name,'value',1));c = Queue(Grant(node.name,'value',1))
    check(registry.activate(node,a) and registry.activate(node,b) and not registry.activate(node,c), 'registry hard limit refuses extra route')
    a.close();check(registry.activate(node,c) and len(registry.routes)==2, 'inactive lifetime recycles route capacity')
    weak = weakref.ref(b);del b;gc.collect();registry.prune();check(weak() is None and len(registry.routes)==1, 'registry cannot extend queue lifetime')
    publisher = Node(node.name,True);check(registry.activate(publisher,Queue(Grant(node.name,'value',1))) and len(registry.routes)==1, 'native publication adds no duplicate route')
    other = Queue(Grant('/different','value',1));check(registry.activate(node,other) and len(registry.routes)==1, 'other Node scope adds no local route')
    weak_node = weakref.ref(node);del node;gc.collect();registry.prune();check(weak_node() is None and not registry.routes, 'registry cannot extend Node lifetime')
    node = Node('/test/node')
    negatives = [Grant('/other','value',2),Grant(node.name,'other',2),Grant(node.name,'value',1),Grant(node.name,'value',2,maximum=1),Grant(node.name,'value',2,low=0,high=0)]
    for index, grant in enumerate(negatives):
        queue = Queue(grant);queue.deliver(node.name,'value',2,2,7);check(not queue.events, 'independent negative node/name/type/bytes/bounds '+str(index))
    for tag, value in [(5,[0,1]),(6,[0,1]),(7,[0,1]),(8,[0.,1.]),(9,['a','b'])]:
        queue = Queue(Grant(node.name,'value',tag,elements=1));queue.deliver(node.name,'value',tag,3,value);check(not queue.events,'deleted prior element quota remains enforced tag'+str(tag))
    queue = Queue(Grant(node.name,'value',3));queue.deliver(node.name,'value',3,3,float('nan'));check(not queue.events,'deleted prior nonfinite value rejected')
    for policy, retained in [('drop-oldest',[5,6]),('drop-newest',[1,2]),('reject',[1,2])]:
        size = len(wire(2,2,node.name,'value',1));queue = Queue(Grant(node.name,'value',2,depth=2,queue_bytes=2*size,overflow=policy))
        for value in range(1,7):
            queue.deliver(node.name,'value',2,2,value)
        check(queue.events==[wire(2,2,node.name,'value',value) for value in retained], 'independent depth/byte overflow '+policy)
    queue = Queue(Grant(node.name,'value',1,burst=1));queue.deliver(node.name,'value',1,2,True);queue.deliver(node.name,'value',1,2,False)
    check(len(queue.events)==1,'rate token admission shares one event budget')


def main():
    parser = argparse.ArgumentParser(description=__doc__);parser.add_argument('--output',type=Path,required=True);args = parser.parse_args()
    source_checks();model_checks()
    paths = [ROOT/'src/ros/gateway/ros2/resources.h', ROOT/'src/ros/gateway/ros2/main.cc'] + sorted((ROOT/'test/0073.ros-tests/support/source/ros_local_parameter_events').glob('*'))
    inputs = {str(path.relative_to(ROOT)):{'bytes':path.stat().st_size,'sha256':hashlib.sha256(path.read_bytes()).hexdigest()} for path in paths if path.is_file()}
    report = {'status':'SOURCE_AND_MODEL_PASS','scope':'Python source predicates and independent authority/lifetime/wire models only','checks':len(CHECKS),'predicates':CHECKS,'inputs':inputs,
              'native_cpp_compilation':'NOT_PERFORMED','actual_gateway_ipc':'NOT_PERFORMED','actual_ros_execution':'NOT_PERFORMED','actual_arm_execution':'NOT_PERFORMED','original_task_complete':False}
    with args.output.open('x') as stream:
        json.dump(report,stream,indent=2,sort_keys=True);stream.write('\n')
    print(json.dumps({'status':report['status'],'checks':len(CHECKS),'output':str(args.output)},sort_keys=True))


if __name__ == '__main__':
    main()
