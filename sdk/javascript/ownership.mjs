/**
 * @file ownership.mjs
 * @brief Private local leases for already owned JavaScript capabilities.
 *
 * These weak sets represent caller-side resource lifetimes only. They create no
 * handle, native identity, grant, thread or Runtime operation. A polling lease
 * spans decode/callback/await/response; it prevents retiring that owner through
 * a resource, its Capability, or its Node/Context. A cleanup lease spans every
 * checked cleanup attempt, excluding a new poll while asynchronous cleanup is
 * awaiting another owner. Requests on a polling owner remain available so a
 * server can send its exact-ID response from inside its handler.
 */
const polling = new WeakSet(), retiring = new WeakSet();
/** Retain each supplied owner once; optional absent capabilities are ignored. */
export function distinctOwners(capabilities) { return [...new Set(capabilities.filter(Boolean))]; }
/** Inspect a local borrow without invoking any Runtime import. */
export function ownerPolling(owner) { return polling.has(owner); }
/** Inspect a pending checked cleanup without invoking any Runtime import. */
export function ownerRetiring(owner) { return retiring.has(owner); }
/** Refuse an entire group before its first cleanup or lifetime mutation. */
export function ownersBusy(capabilities) { return distinctOwners(capabilities).some(owner => ownerPolling(owner) || ownerRetiring(owner)); }
/** Acquire one bounded local group. Null means Conflict17; no set is changed.
 * The returned release callable is used exactly once from the owner's finally.
 * Active resources must retain their original owner references for this pass.
 */
function acquire(capabilities, selected) {
  const owners = distinctOwners(capabilities);
  if (owners.some(owner => polling.has(owner) || retiring.has(owner))) return null;
  for (const owner of owners) selected.add(owner);
  return () => { for (const owner of owners) selected.delete(owner); };
}
/** Acquire decode/callback/response ownership; requests remain permitted. */
export function acquirePolling(capabilities) { return acquire(capabilities, polling); }
/** Acquire every cleanup owner up front, retaining the lease across awaits. */
export function acquireRetirement(capabilities) { return acquire(capabilities, retiring); }
