/** Explicit application choices for lessons03/04/08. Edit a copy and list its
 * exact source in the closed build manifest. No environment/JSON/file loader is
 * used at execution. Empty aliases create only local Context/Node/executor.
 *
 * Available alias families match createApplication in the existing SDK lesson:
 * pub,sub,client; service:{advertise,respond}; actionClient:{send,cancelOwn,
 * cancelAny,feedback,result}; actionServer:{advertise,respond}; parameters:
 * {get,set,list,describe,declare,undeclare,delete,exists,search,events};
 * graph:{alias,operation}; clocks:[{alias,operation}]; tf:{lookup,broadcast};
 * lifecycle:{get,change}; logger; resource:{alias,operation}.
 * Runtime CLI grants independently fix resource/type/QoS/quota/operation.
 */
export const aliases = Object.freeze({});
/** Select at most64 explicit method actions. Use BigInt literals for every i64.
 * Examples: {object:'publisher',method:'publish',args:['hello']},
 * {object:'parameters',method:'set',args:[7n]},
 * {object:'client',method:'callDeferred',args:[]},
 * {object:'executor',method:'spinOnce',args:[]}.
 * Never select a mutation merely to discover its behavior on a live robot.
 */
export const actions = Object.freeze([]);
export const passes = 1;
/** Enable explicit stdin gates only when the independent native supervisor is
 * prepared. No guest clock is inferred from this local example configuration.
 */
export const gated = false;
export const sourceAlias = 'source';
export const observerAlias = 'observer';
/** Explicitly select real generated-codec publication for lesson06. The
 * default remains a local wire roundtrip. Native type adapter and alias grants
 * are independent prerequisites; selecting true never provisions either.
 */
export const customPublish = false;
