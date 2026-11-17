"use strict";

// A later user selection wins over a delayed stop reveal. This is UI state,
// never an execution capability: the adapter still validates every location.
const revisions = new WeakMap();
function current(session) {
  const revision = revisions.get(session);
  return () => revisions.get(session) === revision;
}
function begin(session) {
  revisions.set(session, (revisions.get(session) || 0) + 1);
  return current(session);
}
module.exports = {begin, current};
