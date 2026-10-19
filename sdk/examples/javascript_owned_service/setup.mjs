/**
 * @file setup.mjs
 * @brief Preallocated ownership and failure ledger for both complete examples.
 *
 * Prepare this state before the FIRST capability acquisition. A setup failure
 * throws its already allocated carrier, which retains the original primary by
 * reference (including a frozen Error), the application and every independent
 * cleanup outcome. It never needs to mutate or attach properties to the primary.
 * This is local application bookkeeping, not Runtime authority or Native proof.
 */
const applications = new WeakMap();
/** The actual carrier is constructed before opening any alias. Its application
 * survives failed setup, so callers can inspect facts and explicitly retry only
 * cleanup preflights which never entered a genuine SDK close delegate.
 */
export class SetupFailure extends Error {
  constructor(application) {
    super('Owned-Service application setup failed'); this.name = 'OwnedServiceSetupFailure';
    Object.defineProperty(this, 'application', { value: application, enumerable: true });
    this.primary = null; this.cleanupStatus = null; this.cleanupException = null; this.cleanupThrew = false;
  }
}
/** Allocate every application, failure and partial-owner diagnostic slot first.
 * additions contains captured application-specific codecs/handlers, never grants.
 */
export function prepareApplication(ownerCount, additions) {
  if (!Number.isInteger(ownerCount) || ownerCount < 2 || ownerCount > 3) throw new TypeError('Two or three explicit owners required');
  const app = { ...additions, endpoint: null, owners: new Array(ownerCount).fill(null), open: false };
  const state = { closing: false, cleanupStatus: null, cleanupException: null, cleanupThrew: false,
    owners: Array.from({ length: ownerCount }, () => ({ handle: null, code: null, error: null, threw: false,
      attempted: false, externallyClosed: false, retained: false, finished: false })) };
  state.failure = new SetupFailure(app);
  state.activationFailure = new Error('Owned-Service application activation refused');
  state.activationFailure.code = 0;
  applications.set(app, state); return app;
}
/** Use the already allocated activation refusal; no new Error after the import. */
export function rejectActivation(app, code) {
  const state = applications.get(app); state.activationFailure.code = code; throw state.activationFailure;
}
/** Close every acquired owner independently. The full Native endpoint, when it
 * exists, already retains its complete private ledger. Partial acquisition uses
 * these preallocated rows and the genuine SDK's private closed/attempt evidence.
 * Returning0 never hides a recorded thrown cleanup. No job/Promise is pumped.
 */
export function closeApplication(app) {
  const state = applications.get(app); if (!state) throw new TypeError('Actual prepared application required');
  if (state.closing) return 17;
  state.closing = true;
  try {
    app.open = false;
    if (app.endpoint) {
      try { state.cleanupStatus = app.endpoint.close(); }
      catch (error) { state.cleanupException = error; state.cleanupThrew = true; state.cleanupStatus = 22; }
      return state.cleanupStatus;
    }
    let first = 0;
    for (let i = app.owners.length - 1; i >= 0; --i) {
      const cap = app.owners[i], row = state.owners[i];
      if (!cap || row.finished) { if (row.code && !first) first = row.code; if (row.threw && !first) first = 22; continue; }
      row.handle = cap.handle;
      if (cap.closed) { row.externallyClosed = true; row.retained = false; row.finished = true; continue; }
      try { row.code = cap.close(); if (row.code && !first) first = row.code; }
      catch (error) { row.error = error; row.threw = true; if (!first) first = 22; }
      finally { row.attempted = cap.closed; row.retained = !cap.closed; row.finished = cap.closed; }
      if (row.retained && !first) first = row.code || 17;
    }
    state.cleanupStatus = first; return first;
  } finally { state.closing = false; }
}
/** The caller receives the preexisting carrier, not a newly allocated wrapper.
 * app/primary/ledgers remain reachable even when the primary Error is frozen.
 */
export function failSetup(app, primary) {
  const state = applications.get(app), failure = state.failure; failure.primary = primary;
  try { failure.cleanupStatus = closeApplication(app); failure.cleanupException = state.cleanupException; failure.cleanupThrew = state.cleanupThrew; }
  catch (error) {
    state.cleanupException = error; state.cleanupThrew = true; state.cleanupStatus = 22;
    failure.cleanupException = error; failure.cleanupThrew = true; failure.cleanupStatus = 22;
  }
  throw failure;
}
/** Explicit diagnostic copies can be requested by a caller after cleanup. The
 * endpoint reference exposes only its normal facts() API, not private buffers.
 */
export function applicationCleanupFacts(app) {
  const state = applications.get(app); if (!state) throw new TypeError('Actual prepared application required');
  return Object.freeze({ cleanupStatus: state.cleanupStatus, cleanupException: state.cleanupException, cleanupThrew: state.cleanupThrew,
    endpoint: app.endpoint, owners: Object.freeze(state.owners.map(row => Object.freeze({ ...row }))) });
}
