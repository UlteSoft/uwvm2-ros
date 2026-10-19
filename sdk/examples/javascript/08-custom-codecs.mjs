/**
 * @file 08-custom-codecs.mjs
 * @brief Apply a generated bounded custom-topic codec to either transport.
 *
 * The generated catalog supplies the official ROS type/native identity and
 * canonical codec identity. The trusted Runtime/Gateway must register the exact
 * generated descriptor and typed native adapter; a codec alone grants nothing.
 * Custom services/Actions are outside the current native registry increment.
 */

/** @param {Node} node Synchronous local organizer.
 * @param {object} codec Selected generated fixed-record encode/decode codec.
 * @param {object} aliases Explicit publish/subscribe grant aliases.
 * @param {function} onMessage Local callback receiving the decoded record.
 */
export function customTopics(node,codec,aliases,onMessage) {
  const publisher=node.createPublisher(aliases.publish,codec);
  try {return {publisher,subscription:node.createSubscription(aliases.subscribe,codec,onMessage)};}
  catch(error) {try {error.cleanup=publisher.close();}catch(cleanup){error.cleanup=cleanup;}throw error;}
}
/** Asynchronous counterpart. Results are copied before the callback runs;
 * every opened handle has one close attempt on partial factory failure.
 */
export async function customTopicsAsync(node,codec,aliases,onMessage) {
  const publisher=await node.createPublisher(aliases.publish,codec);
  try {return {publisher,subscription:await node.createSubscription(aliases.subscribe,codec,onMessage)};}
  catch(error) {try {error.cleanup=await publisher.close();}catch(cleanup){error.cleanup=cleanup;}throw error;}
}
