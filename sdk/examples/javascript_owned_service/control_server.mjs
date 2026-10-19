/**
 * @file control_server.mjs
 * @brief Complete caller-driven GetParameterTypes server application.
 *
 * A real Wasm entry supplies createGuestSession(), the opt-in native builtin,
 * the actual generated record namespace, explicit advertise/respond CLI aliases,
 * two independently observed expected QoS80 rows, and a local type resolver.
 * This application uses only true retained owned-Service imports. Distinct
 * Runtime policy grants, not module names or these functions, permit Native work.
 */
import { OwnedServiceBridge } from '../../javascript/owned_service/bridge.mjs';
import { OwnedServer } from '../../javascript/owned_service/owned.mjs';
import { bindControlServices } from '../../javascript/owned_service/control.mjs';
import { prepareApplication, rejectActivation, failSetup, closeApplication } from './setup.mjs';

export function openParameterTypesServer(session, nativeBinding, records, selection, resolveType) {
  if (typeof resolveType !== 'function') throw new TypeError('Explicit local parameter-type resolver required');
  const definitions = bindControlServices(records), bridge = new OwnedServiceBridge(nativeBinding);
  const app = prepareApplication(2, { resolveType,
    responseCodec: records.Ros2Rcl_interfacesSrvGetParameterTypes_ResponseCodec });
  try {
    app.owners[0] = session.open(selection.advertiseAlias); app.owners[1] = session.open(selection.responderAlias);
    app.endpoint = new OwnedServer(bridge, app.owners[0], app.owners[1], definitions.getParameterTypes,
      { ownership: 'owned', historyEntries: selection.historyEntries, expectedQos: selection.expectedQos });
    const status = app.endpoint.activate();
    if (status !== 0) rejectActivation(app, status);
    return app;
  } catch (error) { return failSetup(app, error); }
}
/** Take one request, then call the resolver after ACK. Callback failure closes
 * the endpoint and is rethrown; it never becomes an empty successful response.
 * The exact unsigned64 Runtime request ticket is used for one response. The
 * Native request-ID metadata is for observation; it is not the local ticket.
 */
export function serveParameterTypesOnce(app) {
  const taken = app.endpoint.take();
  if (taken.status || !taken.delivery || taken.delivery.control) return taken.status || (taken.delivery?.error ?? 11);
  const request = taken.delivery;
  try {
    const response = app.responseCodec.defaultValue();
    response.types = request.value.names.map(name => app.resolveType(name));
    const status = app.endpoint.respond(request.ticket, response);
    if (status !== 0) app.endpoint.close();
    return status;
  } catch (error) { app.endpoint.close(); throw error; }
}
/** Every independent cleanup is attempted even if another owner throws. */
export function closeParameterTypesServer(app) { return closeApplication(app); }
