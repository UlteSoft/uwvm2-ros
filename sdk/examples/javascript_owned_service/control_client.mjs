/**
 * @file control_client.mjs
 * @brief Complete caller-driven GetTypeDescription client application.
 *
 * The Wasm entry imports its explicitly selected generated bounded_records.mjs,
 * createGuestSession(), and `roswasm:rmw_service_native`, then passes them here.
 * Supply independently selected expectedQos (two exact protocol80 rows), three
 * CLI alias names, and a genuine requested type/hash. No directory, environment,
 * network, native ROS library or debug/update interface is opened by this code.
 * The outer caller owns the finite poll schedule and external process deadline.
 */
import { OwnedServiceBridge } from '../../javascript/owned_service/bridge.mjs';
import { OwnedClient } from '../../javascript/owned_service/owned.mjs';
import { bindControlServices } from '../../javascript/owned_service/control.mjs';
import { prepareApplication, rejectActivation, failSetup, closeApplication } from './setup.mjs';

/** Construct all application/cleanup slots before the first capability open.
 * A construction/activation failure closes every acquired owner independently.
 * Explicit owned lifetime transfers those three SDK owners to the endpoint.
 */
export function openDescriptionClient(session, nativeBinding, records, selection) {
  const definitions = bindControlServices(records), bridge = new OwnedServiceBridge(nativeBinding);
  const requestCodec = records.Ros2Type_description_interfacesSrvGetTypeDescription_RequestCodec;
  const app = prepareApplication(3, { requestCodec });
  try {
    app.owners[0] = session.open(selection.callAlias);
    app.owners[1] = session.open(selection.observerAlias);
    app.owners[2] = session.open(selection.availabilityAlias);
    app.endpoint = new OwnedClient(bridge, app.owners[0], app.owners[1], definitions.getTypeDescription,
      { ownership: 'owned', availability: app.owners[2], historyEntries: selection.historyEntries, expectedQos: selection.expectedQos });
    const status = app.endpoint.activate();
    if (status !== 0) rejectActivation(app, status);
    app.open = true; return app;
  } catch (error) { return failSetup(app, error); }
}
/** Real Native availability uses its independently granted query owner. */
export function beginDescription(app, typeName, typeHash, waitNanoseconds) {
  const ready = app.endpoint.available(waitNanoseconds);
  if (ready.status || !ready.available) return { status: ready.status || 11, ticket: null };
  const request = app.requestCodec.defaultValue();
  request.type_name = typeName; request.type_hash = typeHash;
  // Preserve the actual original default, which requests the complete sources.
  if (request.include_type_sources !== true) throw new Error('Unexpected original request default');
  return app.endpoint.submit(request);
}
/** ONE explicit poll; caller schedules another call only under its own deadline.
 * response.type_sources[*].raw_file_contents is an owned Uint8Array. NUL and
 * non-UTF8 bytes are valid only there. The full response is not a two-field RPC.
 * information retains exact signed timestamps/native request sequence and the
 * complete16-byte writer GUID; a receipt GID is never substituted for that GUID.
 */
export function takeDescription(app) { return app.endpoint.take(); }
/** Explicit request abandonment never resubmits or produces a late success. */
export function abandonDescription(app, ticket) { return app.endpoint.abandon(ticket); }
/** Always call from the Wasm entry's finally. Keep facts for every cleanup error.
 * Constructor failure before an endpoint exists still closes partial opens.
 */
export function closeDescriptionClient(app) { return closeApplication(app); }
