/**
 * @file control.mjs
 * @brief Bind all seven original default-node Service roots to their REAL codecs.
 *
 * Supply the namespace of the explicit records-only producer's
 * bounded_records.mjs. This file does not generate codecs, make up Native hashes,
 * load modules from a path, add permission or route through legacy business RPC.
 * The generator's proof and the actual final Wasm/native validation remain
 * separate. The complete GetTypeDescription response includes raw source bytes.
 */
import { serviceDefinition } from './owned.mjs';
const names = Object.freeze([
  ['describeParameters', 'Ros2Rcl_interfacesSrvDescribeParameters'],
  ['getParameterTypes', 'Ros2Rcl_interfacesSrvGetParameterTypes'],
  ['getParameters', 'Ros2Rcl_interfacesSrvGetParameters'],
  ['listParameters', 'Ros2Rcl_interfacesSrvListParameters'],
  ['setParameters', 'Ros2Rcl_interfacesSrvSetParameters'],
  ['setParametersAtomically', 'Ros2Rcl_interfacesSrvSetParametersAtomically'],
  ['getTypeDescription', 'Ros2Type_description_interfacesSrvGetTypeDescription']
]);
/** Pure, complete binding. Every root identity differs from its component IDs.
 * All names/properties are captured before any endpoint/Native operation.
 */
export function bindControlServices(records) {
  if (!records || typeof records !== 'object') throw new TypeError('Actual complete generated record namespace required');
  const result = {};
  for (const [key, symbol] of names) {
    const identity = records[symbol + 'Identity'];
    const request = records[symbol + '_RequestCodec'], response = records[symbol + '_ResponseCodec'];
    result[key] = serviceDefinition(identity, request, response);
  }
  return Object.freeze(result);
}
