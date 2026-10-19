/** Lesson03: the existing full synchronous resource facade inside actual Wasm.
 * Configure explicit aliases/actions in config.mjs. Service and Action handlers
 * are guest JS callbacks; polling/correlation/cleanup use the shared SDK. The
 * empty default plan grants no rights and only executes local application work.
 */
import { createGuestSession } from '../../javascript/guest.mjs';
import { createApplication, closeApplication } from '../javascript/07-sync-application.mjs';
import { aliases, actions, passes, gated } from './config.mjs';
import { perform } from './actions.mjs';
import { marker, clean } from './support.mjs';
export async function main() {
  const application = createApplication(createGuestSession(), aliases, {
    service: () => ({ success: true, message: 'actual guest JS service handler' }),
    action: () => ({ kind: 3, values: [0, 1, 1, 2, 3] }),
    timerReady: () => true, timer: () => {}
  });
  try { await perform(application, actions, passes, gated); }
  finally { clean(closeApplication(application)); }
  marker('PASS_JS_GUEST_APPLICATION'); return 0;
}
