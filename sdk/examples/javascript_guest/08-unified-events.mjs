/** Lesson08: actual source/observer ownership and unified event envelopes.
 * Configure sourceAlias/observerAlias plus independent Runtime grants. The
 * existing implementation checks the copied event's source/session/sequence,
 * closes its source and observes the permitted resource_closed record. No
 * actor, alias, credit or native effect is inferred by this application.
 */
import { createAsyncGuestSession } from '../../javascript/guest.mjs';
import { runUnifiedEvents } from '../javascript/09-unified-events.mjs';
import { sourceAlias, observerAlias } from './config.mjs';
import { status, marker } from './support.mjs';
export async function main() {
  const session = createAsyncGuestSession();
  status(await runUnifiedEvents(session, sourceAlias, observerAlias));
  await session.shutdown(); marker('PASS_JS_GUEST_UNIFIED_EVENTS'); return 0;
}
