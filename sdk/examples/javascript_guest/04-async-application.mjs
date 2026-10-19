/** Lesson04: copied Promise-facing resources with the engine's finite job pump.
 * No Node streams/timers are used. Awaiting an operation does not transform a
 * native blocking call into asynchronous ROS execution. Existing actual
 * service_submit/event APIs retain their separate admission semantics.
 */
import { createAsyncGuestSession } from '../../javascript/guest.mjs';
import { createApplication, closeApplication } from '../javascript/06-async-application.mjs';
import { aliases, actions, passes, gated } from './config.mjs';
import { perform } from './actions.mjs';
import { marker, clean, check } from './support.mjs';
export async function main() {
  const session = createAsyncGuestSession(), application = await createApplication(session, aliases, {
    service: async () => ({ success: true, message: 'actual async guest JS handler' }),
    action: async () => ({ kind: 3, values: [0, 1, 1, 2, 3] }),
    timerReady: async () => true, timer: async () => {}
  });
  try { await perform(application, actions, passes, gated); }
  finally {
    const result = await closeApplication(application);
    check(result.clean, 'Complete asynchronous cleanup');
  }
  await session.shutdown(); marker('PASS_JS_GUEST_ASYNC_APPLICATION'); return 0;
}
