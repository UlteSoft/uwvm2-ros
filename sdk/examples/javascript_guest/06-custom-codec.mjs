/** Lesson06: real generated custom record codec in the embedded JS engine.
 * The exact existing ROS1 Packet profile is an example type, not arbitrary IDL
 * compatibility. Its native adapter/identity and packet-pub grant must be
 * separately installed for sendPacket(). The default main performs no ROS IO.
 */
import { createGuestSession, Node } from '../../javascript/guest.mjs';
import { Ros1Example_interfaces_customPacketCodec as codec,
  Ros2Example_interfaces_customFixedSampleCodec as sampleCodec } from '../../custom-types/generated-r3/fixed_records.mjs';
import { check, marker, status, clean } from './support.mjs';
import { customPublish } from './config.mjs';
const packet = () => ({ sequence: 7, active: true, measurement: 1.25, tag: [65,0,0,0], samples: [-3,0,0] });
const sample = () => ({ sequence: 8, enabled: true, temperature: 1.5, acceleration: [0.5,1,-2],
  tag: [65,0,0,0,0,0,0,0], thresholds: [-3,4] });
export function sendPacket(session = createGuestSession()) {
  check(session.version === 1, 'ROS1 example profile');
  const node = new Node(session, 'guest-custom');
  try { status(node.createPublisher('packet-pub', codec).publish(packet())); }
  finally { clean(node.close()); }
  return 0;
}
export function sendSample(session = createGuestSession()) {
  check(session.version === 2, 'ROS2 example profile');
  const node = new Node(session, 'guest-custom-ros2');
  try { status(node.createPublisher('sample-pub', sampleCodec).publish(sample())); }
  finally { clean(node.close()); }
  return 0;
}
export function main() {
  const wire = codec.encode(packet()), value = codec.decode(wire);
  check(wire.length === 23 && value.sequence === 7 && value.samples[0] === -3, 'Complete generated record');
  const sampleWire = sampleCodec.encode(sample()), sampleValue = sampleCodec.decode(sampleWire);
  check(sampleWire.length === 45 && sampleValue.sequence === 8 && sampleValue.thresholds[1] === 4, 'Complete generated ROS2 record');
  if (customPublish) {
    const session = createGuestSession();
    if (session.version === 1) sendPacket(session); else sendSample(session);
  }
  marker('PASS_JS_GUEST_CUSTOM_CODEC'); return 0;
}
