// One backend per package. Session/Node labels are local bookkeeping only;
// policy, resource, native identity, QoS and every quota remain host-selected.
// No debug/update/policy import, native callback, timer or hidden background work.

pub(all) enum Operation {
  TopicPublish
  TopicSubscribe
  EventPoll
  EventWatch
  EventPollV1
  EventReady
  ServiceSubmit
  ServiceCall
  ServiceAdvertise
  ServiceRespond
  ActionSendGoal
  ActionCancelOwnGoal
  ActionCancelAnyGoal
  ActionReceiveFeedback
  ActionReceiveResult
  ActionAdvertise
  ActionRespond
  TfLookup
  TfBroadcast
  ParameterGet
  ParameterSet
  ParameterDelete
  ParameterExists
  ParameterList
  ParameterSearch
  GraphInspectTopic
  GraphInspectNode
  GraphInspectService
  GraphListTopics
  GraphListNodes
  GraphListServices
  TimeReadRos
  TimeReadWall
  LogWrite
} derive(Eq)

pub struct Session {
  mut closed : Bool
  mut leased : Bool
  mut retiring : Bool
  resources : Array[Resource]
  mut last_cleanup : CleanupReport?
  cleanup_reports : Array[CleanupReport]
}

pub struct Node {
  session : Session
  label : String
}

pub struct Resource {
  session : Session
  operation : Operation
  alias : String
  mut handle : UInt64
  mut closed : Bool
  mut revoked : Bool
  // These scalar slots exist BEFORE capability_open. They retain the exact
  // last attempted close/revoke even if constructing a later report traps.
  mut cleanup_attempted : Bool
  mut cleanup_completed : Bool
  mut cleanup_code : Int
  mut cleanup_kind : Int
}

/// Reports are separate from authority-bearing owner state. All outcome slots
/// are allocated before a retirement effect. code retains the setup primary;
/// every close failure remains visible independently in outcomes, in reverse
/// acquisition order. attempted=false is an already-retired/pending local slot.
pub(all) struct CleanupOutcome {
  alias : String
  mut handle : UInt64
  mut attempted : Bool
  mut previous : Bool
  mut completed : Bool
  mut code : Int
}

pub(all) struct CleanupReport {
  mut started : Bool
  mut code : Int
  outcomes : Array[CleanupOutcome]
}

struct PreparedOpen {
  owner : Resource
  name : Bytes
}

pub fn new_session() -> Result[Session, Int] {
  if raw_abi_version() != 1 { return Err(95) }
  if !@linear.memory_available() { return Err(22) }
  Ok({ closed: false, leased: false, retiring: false,
       resources: [], last_cleanup: None, cleanup_reports: [] })
}

pub fn Session::node(self : Session, label : String) -> Result[Node, Int] {
  if self.closed { return Err(9) }
  if label.length() == 0 || label.length() > 255 { return Err(22) }
  Ok({ session: self, label })
}

fn Node::prepare_open(self : Node, alias : String, operation : Operation) -> Result[PreparedOpen, Int] {
  if self.session.closed { return Err(9) }
  if self.session.leased || self.session.retiring { return Err(17) }
  // Bound the complete ownership ledger, not only the currently live subset.
  // Failed/pending records remain observable; no successful owner is discarded.
  if self.session.resources.length() >= 1024 { return Err(11) }
  let name = match @wire.bounded_utf8(alias, 255) { Ok(value) => value; Err(code) => return Err(code) }
  if name.length() == 0 || name.length() > 255 { return Err(22) }
  for byte in name { if byte.to_int() == 0 { return Err(22) } }
  for owner in self.session.resources {
    if owner.alias == alias && owner.revoked { return Err(13) }
  }
  let owner : Resource = {
    session: self.session, operation, alias, handle: 0UL, closed: true,
    revoked: false, cleanup_attempted: false, cleanup_completed: false,
    cleanup_code: 0, cleanup_kind: 0,
  }
  let prepared : PreparedOpen = { owner, name }
  let result = Ok(prepared)
  // Reserve this local owner before entering a host effect. Allocation failure
  // here cannot leak a host handle because no raw open has occurred yet.
  self.session.resources.push(owner)
  result
}

fn PreparedOpen::acquire(self : PreparedOpen) -> Int {
  let owner = self.owner
  // Both closures are constructed before admit_u64 performs its one import.
  @linear.admit_u64(self.name, false, fn(a, b, _) {
    raw_capability_open(0, a, self.name.length().to_uint64(), b)
  }, fn(value) {
    owner.handle = value
    owner.closed = false
    0
  })
}

/// All wrapper/list/success allocation is complete before the real open. The
/// successful path commits only scalars and returns the preconstructed Result.
pub fn Node::open(self : Node, alias : String, operation : Operation) -> Result[Resource, Int] {
  let prepared = match self.prepare_open(alias, operation) {
    Ok(value) => value; Err(code) => return Err(code)
  }
  let success = Ok(prepared.owner)
  let code = prepared.acquire()
  if code != 0 { Err(code) } else { success }
}

pub fn Resource::raw_handle(self : Resource) -> Result[UInt64, Int] {
  if self.closed || self.session.closed { Err(9) } else { Ok(self.handle) }
}

fn Resource::retire(self : Resource) -> Int {
  if self.closed { return 9 }
  self.closed = true
  self.cleanup_attempted = true
  self.cleanup_completed = false
  self.cleanup_kind = 1
  let code = raw_capability_close(self.handle)
  self.cleanup_code = code
  self.cleanup_completed = true
  code
}

/// Reject the entire busy session BEFORE mutation. An executor lease covers all
/// decode/callback code; close cannot retire a sibling behind that callback.
pub fn Resource::close(self : Resource) -> Int {
  if self.session.leased || self.session.retiring { return 17 }
  self.retire()
}

fn Resource::invalidate_alias(self : Resource) -> Unit {
  for owner in self.session.resources {
    if owner.alias == self.alias { owner.closed = true; owner.revoked = true }
  }
}

pub fn Resource::revoke(self : Resource) -> Int {
  if self.session.leased || self.session.retiring { return 17 }
  if self.closed || self.session.closed { return 9 }
  self.invalidate_alias()
  self.cleanup_attempted = true
  self.cleanup_completed = false
  self.cleanup_kind = 2
  let code = raw_capability_revoke(self.handle)
  self.cleanup_code = code
  self.cleanup_completed = true
  code
}

fn reserve_cleanup(work : Array[Resource], primary : Int) -> CleanupReport {
  let outcomes : Array[CleanupOutcome] = []
  for i = work.length() - 1; i >= 0; i = i - 1 {
    let owner = work[i]
    outcomes.push({ alias: owner.alias, handle: owner.handle,
      attempted: false, previous: false, completed: false, code: 0 })
  }
  { started: false, code: primary, outcomes }
}

fn Session::retire_reserved(self : Session, work : Array[Resource], report : CleanupReport, primary : Int) -> Unit {
  report.code = primary
  report.started = true
  // The Some/report/array/rows were all reserved before any acquired handle is
  // retired. Scalar writes below retain every returned status; no ignore(close).
  self.retiring = true
  for n = 0; n < work.length(); n = n + 1 {
    let owner = work[work.length() - 1 - n]
    let outcome = report.outcomes[n]
    outcome.handle = owner.handle
    if !owner.closed {
      outcome.attempted = true
      let code = owner.retire()
      outcome.code = code
      outcome.completed = true
      if report.code == 0 && code != 0 { report.code = code }
    } else {
      // Retain earlier manual/factory errors without replaying an import.
      outcome.previous = owner.cleanup_attempted
      outcome.completed = !owner.cleanup_attempted || owner.cleanup_completed
      outcome.code = if owner.cleanup_attempted { owner.cleanup_code } else { 0 }
      if report.code == 0 && outcome.code != 0 { report.code = outcome.code }
    }
  }
  self.retiring = false
}

/// Return all original manual/factory close and revoke statuses, without
/// substituting a summary for the per-owner outcome. kind1=close, kind2=revoke.
pub(all) struct Retirement {
  alias : String
  handle : UInt64
  kind : Int
  attempted : Bool
  completed : Bool
  code : Int
}

pub fn Session::retirements(self : Session) -> Array[Retirement] {
  self.resources.map(fn(owner) {
    { alias: owner.alias, handle: owner.handle, kind: owner.cleanup_kind,
      attempted: owner.cleanup_attempted, completed: owner.cleanup_completed,
      code: owner.cleanup_code }
  })
}

pub fn Session::last_cleanup(self : Session) -> CleanupReport? { self.last_cleanup }

/// Reserved reports are distinguishable by started=false. Copying this array
/// is a read-only observation step, never part of cleanup/admission itself.
pub fn Session::cleanup_history(self : Session) -> Array[CleanupReport] {
  self.cleanup_reports.copy()
}

/// Reject a busy group before changing ANY owner/session state. Otherwise close
/// every remaining owner once, in reverse order, retaining ALL failures. The
/// resources ledger remains available after shutdown; no finalizer hides it.
pub fn Session::shutdown(self : Session) -> CleanupReport {
  if self.leased || self.retiring { return { started: false, code: 17, outcomes: [] } }
  if self.closed { return { started: false, code: 9, outcomes: [] } }
  let report = reserve_cleanup(self.resources, 0)
  self.cleanup_reports.push(report)
  // Reserve the optional report wrapper before the first cleanup effect too.
  self.last_cleanup = Some(report)
  self.closed = true
  self.retire_reserved(self.resources, report, 0)
  report
}
/// Exact RPC family selection; there is no string-to-import fallback. Capacity
/// is explicit, bounds checked before the import, with no retry on90 or110.
pub fn Resource::request(self : Resource, input : Bytes, capacity : Int) -> @linear.Reply {
  let h = match self.raw_handle() {
    Ok(value) => value
    Err(code) => return @linear.failure(code)
  }
  @linear.perform(input, capacity, false, fn(a, b, z) {
    match self.operation {
    ServiceCall => raw_service_call(h, 0, a, input.length().to_uint64(), b, capacity.to_uint64(), z)
    ServiceRespond => raw_service_respond(h, 0, a, input.length().to_uint64(), b, capacity.to_uint64(), z)
    ActionSendGoal => raw_action_send_goal(h, 0, a, input.length().to_uint64(), b, capacity.to_uint64(), z)
    ActionCancelOwnGoal => raw_action_cancel_own_goal(h, 0, a, input.length().to_uint64(), b, capacity.to_uint64(), z)
    ActionCancelAnyGoal => raw_action_cancel_any_goal(h, 0, a, input.length().to_uint64(), b, capacity.to_uint64(), z)
    ActionReceiveFeedback => raw_action_receive_feedback(h, 0, a, input.length().to_uint64(), b, capacity.to_uint64(), z)
    ActionReceiveResult => raw_action_receive_result(h, 0, a, input.length().to_uint64(), b, capacity.to_uint64(), z)
    ActionAdvertise => raw_action_advertise(h, 0, a, input.length().to_uint64(), b, capacity.to_uint64(), z)
    ActionRespond => raw_action_respond(h, 0, a, input.length().to_uint64(), b, capacity.to_uint64(), z)
    TfLookup => raw_tf_lookup(h, 0, a, input.length().to_uint64(), b, capacity.to_uint64(), z)
    TfBroadcast => raw_tf_broadcast(h, 0, a, input.length().to_uint64(), b, capacity.to_uint64(), z)
    ParameterGet => raw_parameter_get(h, 0, a, input.length().to_uint64(), b, capacity.to_uint64(), z)
    ParameterSet => raw_parameter_set(h, 0, a, input.length().to_uint64(), b, capacity.to_uint64(), z)
    ParameterDelete => raw_parameter_delete(h, 0, a, input.length().to_uint64(), b, capacity.to_uint64(), z)
    ParameterExists => raw_parameter_exists(h, 0, a, input.length().to_uint64(), b, capacity.to_uint64(), z)
    ParameterList => raw_parameter_list(h, 0, a, input.length().to_uint64(), b, capacity.to_uint64(), z)
    ParameterSearch => raw_parameter_search(h, 0, a, input.length().to_uint64(), b, capacity.to_uint64(), z)
    GraphInspectTopic => raw_graph_inspect_topic(h, 0, a, input.length().to_uint64(), b, capacity.to_uint64(), z)
    GraphInspectNode => raw_graph_inspect_node(h, 0, a, input.length().to_uint64(), b, capacity.to_uint64(), z)
    GraphInspectService => raw_graph_inspect_service(h, 0, a, input.length().to_uint64(), b, capacity.to_uint64(), z)
    GraphListTopics => raw_graph_list_topics(h, 0, a, input.length().to_uint64(), b, capacity.to_uint64(), z)
    GraphListNodes => raw_graph_list_nodes(h, 0, a, input.length().to_uint64(), b, capacity.to_uint64(), z)
    GraphListServices => raw_graph_list_services(h, 0, a, input.length().to_uint64(), b, capacity.to_uint64(), z)
    TimeReadRos => raw_time_read_ros(h, 0, a, input.length().to_uint64(), b, capacity.to_uint64(), z)
    TimeReadWall => raw_time_read_wall(h, 0, a, input.length().to_uint64(), b, capacity.to_uint64(), z)
      _ => 95
    }
  })
}

pub fn Resource::write(self : Resource, input : Bytes) -> Int {
  let h = match self.raw_handle() { Ok(value) => value; Err(code) => return code }
  @linear.write(input, fn(a) {
    match self.operation {
    TopicPublish => raw_topic_publish(h, 0, a, input.length().to_uint64())
    LogWrite => raw_log_write(h, 0, a, input.length().to_uint64())
      _ => 95
    }
  })
}

/// Activate only the operation explicitly selected for this local owner.
pub fn Resource::activate(self : Resource) -> Int {
  if self.session.leased || self.session.retiring { return 17 }
  let h = match self.raw_handle() { Ok(value) => value; Err(code) => return code }
  match self.operation {
    TopicSubscribe => raw_topic_subscribe(h)
    ServiceAdvertise => raw_service_advertise(h)
    _ => 95
  }
}

/// Only queue-owning resources may use legacy event_poll. Code11 is returned
/// as idle; a callback's own code11 remains an application error, not an idle poll.
fn Resource::poll_leased(self : Resource, capacity : Int) -> @linear.Reply {
  match self.operation {
    TopicSubscribe | ServiceAdvertise | ActionAdvertise => ()
    _ => return @linear.failure(22)
  }
  let h = match self.raw_handle() { Ok(value) => value; Err(code) => return @linear.failure(code) }
  @linear.perform(b"", capacity, false, fn(_, b, z) { raw_event_poll(h, 0, b, capacity.to_uint64(), z) })
}

/// Unified observer and source are separate handles/grants. Watching creates no
/// aliases, source activation or payload permission; goal correlation stays u64.
pub fn Resource::watch(self : Resource, source : Resource, correlation : UInt64) -> Int {
  if self.operation != EventWatch { return 22 }
  let observer = match self.raw_handle() { Ok(value) => value; Err(code) => return code }
  let owner = match source.raw_handle() { Ok(value) => value; Err(code) => return code }
  raw_event_watch(observer, owner, correlation)
}

pub fn Resource::poll_v1(self : Resource, capacity : Int) -> @linear.Reply {
  if self.operation != EventPollV1 { return @linear.failure(22) }
  let h = match self.raw_handle() { Ok(value) => value; Err(code) => return @linear.failure(code) }
  @linear.perform(b"", capacity, false, fn(_, b, z) { raw_event_poll_v1(h, 0, b, capacity.to_uint64(), z) })
}

pub fn Resource::ready(self : Resource) -> @linear.Reply {
  if self.operation != EventReady { return @linear.failure(22) }
  let h = match self.raw_handle() { Ok(value) => value; Err(code) => return @linear.failure(code) }
  @linear.perform(b"", 32, true, fn(_, b, _) { raw_event_ready(h, 0, b) })
}

pub fn Resource::submit(self : Resource, observer : Resource, input : Bytes) -> @linear.Reply {
  if self.operation != ServiceSubmit { return @linear.failure(22) }
  let source = match self.raw_handle() { Ok(value) => value; Err(code) => return @linear.failure(code) }
  let events = match observer.raw_handle() { Ok(value) => value; Err(code) => return @linear.failure(code) }
  @linear.perform(input, 8, true, fn(a, b, _) {
    raw_service_submit(source, events, 0, a, input.length().to_uint64(), b)
  })
}



pub fn Resource::poll(self : Resource, capacity : Int) -> @linear.Reply {
  if self.session.leased || self.session.retiring { return @linear.failure(17) }
  self.session.leased = true
  let reply = self.poll_leased(capacity)
  self.session.leased = false
  reply
}

/// Pure local role-shape admission before ANY factory reservation/import.
/// Aliases remain exact host-selected names: no auto suffix/remap/default grant.
fn distinct_aliases(names : Array[String]) -> Bool {
  for i = 0; i < names.length(); i = i + 1 {
    if names[i].length() == 0 { return false }
    for j = 0; j < i; j = j + 1 {
      if names[j] == names[i] { return false }
    }
  }
  true
}

pub fn Node::publisher(self : Node, alias : String) -> Result[Resource, Int] {
  self.open(alias, TopicPublish)
}

pub fn Node::subscription(self : Node, alias : String) -> Result[Resource, Int] {
  let pending = match self.prepare_open(alias, TopicSubscribe) { Ok(value) => value; Err(code) => return Err(code) }
  let success = Ok(pending.owner)
  let work = [pending.owner]
  let cleanup = reserve_cleanup(work, 0)
  let cleanup_reserved = Some(cleanup)
  self.session.cleanup_reports.push(cleanup)
  let code = pending.acquire()
  if code != 0 { return Err(code) }
  let code = pending.owner.activate()
  if code != 0 {
    self.session.last_cleanup = cleanup_reserved
    self.session.retire_reserved(work, cleanup, code)
    return Err(code)
  }
  success
}

pub fn Node::client(self : Node, alias : String) -> Result[Resource, Int] {
  self.open(alias, ServiceCall)
}

pub(all) struct ServiceServer {
  events : Resource
  responder : Resource
}

pub fn Node::service(self : Node, advertise : String, respond : String) -> Result[ServiceServer, Int] {
  if !distinct_aliases([advertise, respond]) { return Err(22) }
  let events = match self.prepare_open(advertise, ServiceAdvertise) { Ok(value) => value; Err(code) => return Err(code) }
  let responder = match self.prepare_open(respond, ServiceRespond) { Ok(value) => value; Err(code) => return Err(code) }
  let work = [events.owner, responder.owner]
  let cleanup = reserve_cleanup(work, 0)
  let cleanup_reserved = Some(cleanup)
  self.session.cleanup_reports.push(cleanup)
  let success : Result[ServiceServer, Int] = Ok({ events: events.owner, responder: responder.owner })
  let mut code = events.acquire()
  if code == 0 { code = responder.acquire() }
  if code == 0 { code = events.owner.activate() }
  if code != 0 {
    self.session.last_cleanup = cleanup_reserved
    self.session.retire_reserved(work, cleanup, code)
    return Err(code)
  }
  success
}

pub fn ServiceServer::respond(self : ServiceServer, id : UInt64, body : Bytes) -> Int {
  let input = match @wire.id_prefix(id, body) { Ok(value) => value; Err(code) => return code }
  self.responder.request(input, 0).code
}

pub struct ActionClient {
  sender : Resource
  feedback : Resource
  result : Resource
  cancel : Resource
  // All tracking slots exist before any actual action admission.
  goals : Array[UInt64]
}

pub fn Node::action_client(self : Node, send : String, feedback : String, result : String, cancel : String) -> Result[ActionClient, Int] {
  let names = [send, feedback, result, cancel]
  if !distinct_aliases(names) { return Err(22) }
  let operations = [ActionSendGoal, ActionReceiveFeedback, ActionReceiveResult, ActionCancelOwnGoal]
  let pending : Array[PreparedOpen] = []
  for i = 0; i < 4; i = i + 1 {
    match self.prepare_open(names[i], operations[i]) {
      Ok(value) => pending.push(value)
      Err(code) => return Err(code)
    }
  }
  let work = pending.map(fn(item) { item.owner })
  let cleanup = reserve_cleanup(work, 0)
  let cleanup_reserved = Some(cleanup)
  self.session.cleanup_reports.push(cleanup)
  let success = Ok({ sender: work[0], feedback: work[1], result: work[2],
                     cancel: work[3], goals: Array::make(1024, 0UL) })
  for item in pending {
    let code = item.acquire()
    if code != 0 {
      self.session.last_cleanup = cleanup_reserved
      self.session.retire_reserved(work, cleanup, code)
      return Err(code)
    }
  }
  success
}

pub fn ActionClient::send_goal(self : ActionClient, canonical_goal : Bytes) -> Result[UInt64, Int] {
  let h = match self.sender.raw_handle() { Ok(value) => value; Err(code) => return Err(code) }
  let mut slot = -1
  for i = 0; i < self.goals.length(); i = i + 1 {
    if self.goals[i] == 0UL { slot = i; break }
  }
  if slot < 0 { return Err(11) }
  // Result[UInt64] allocation would occur after admission. Keep the known goal
  // in a reserved scalar slot FIRST, then construct the public result. A later
  // allocation trap cannot lose the actual ID from this Session-owned client.
  let code = @linear.admit_u64(canonical_goal, true, fn(a, b, z) {
    raw_action_send_goal(h, 0, a, canonical_goal.length().to_uint64(), b, 8UL, z)
  }, fn(id) {
    if self.goals.contains(id) { return 22 }
    self.goals[slot] = id
    0
  })
  if code != 0 { Err(code) } else { Ok(self.goals[slot]) }
}

pub fn ActionClient::feedback(self : ActionClient, id : UInt64, capacity : Int) -> @linear.Reply {
  if id == 0UL || !self.goals.contains(id) { return @linear.failure(22) }
  let reply = self.feedback.request(@wire.u64(id), capacity)
  if reply.code == 0 && !@wire.action_envelope(reply.data, id, false) {
    return @linear.failure(22)
  }
  reply
}

pub fn ActionClient::result(self : ActionClient, id : UInt64, capacity : Int) -> @linear.Reply {
  if id == 0UL || !self.goals.contains(id) { return @linear.failure(22) }
  let reply = self.result.request(@wire.u64(id), capacity)
  if reply.code == 0 {
    if !@wire.action_envelope(reply.data, id, true) { return @linear.failure(22) }
    // Validate matching nonzero ID AND terminal kind2..4 before local retirement.
    // Payload-specific validation remains the caller's selected strict codec.
    for i = 0; i < self.goals.length(); i = i + 1 {
      if self.goals[i] == id { self.goals[i] = 0UL }
    }
  }
  reply
}

pub fn ActionClient::cancel_own(self : ActionClient, id : UInt64) -> @linear.Reply {
  if id == 0UL || !self.goals.contains(id) { return @linear.failure(22) }
  // The actual built-in profile returns Bool1, never a zero-capacity response.
  let reply = self.cancel.request(@wire.u64(id), 1)
  if reply.code == 0 {
    match @wire.read_bool(reply.data) { Ok(_) => (); Err(_) => return @linear.failure(22) }
  }
  reply
}

pub(all) struct ActionServer {
  events : Resource
  responder : Resource
}

pub fn Node::action_server(self : Node, advertise : String, respond : String) -> Result[ActionServer, Int] {
  if !distinct_aliases([advertise, respond]) { return Err(22) }
  let events = match self.prepare_open(advertise, ActionAdvertise) { Ok(value) => value; Err(code) => return Err(code) }
  let responder = match self.prepare_open(respond, ActionRespond) { Ok(value) => value; Err(code) => return Err(code) }
  let work = [events.owner, responder.owner]
  let cleanup = reserve_cleanup(work, 0)
  let cleanup_reserved = Some(cleanup)
  self.session.cleanup_reports.push(cleanup)
  let success : Result[ActionServer, Int] = Ok({ events: events.owner, responder: responder.owner })
  let mut code = events.acquire()
  if code == 0 { code = responder.acquire() }
  if code == 0 { code = events.owner.request(b"", 0).code }
  if code != 0 {
    self.session.last_cleanup = cleanup_reserved
    self.session.retire_reserved(work, cleanup, code)
    return Err(code)
  }
  success
}

pub fn ActionServer::respond(self : ActionServer, id : UInt64, kind : Int, body : Bytes) -> Int {
  if kind < 1 || kind > 4 { return 22 }
  let payload = Bytes::make(1, kind.to_byte()) + body
  let input = match @wire.id_prefix(id, payload) { Ok(value) => value; Err(code) => return code }
  self.responder.request(input, 0).code
}

/// Finite guest-thread executor. Each pass first validates EVERY source and
/// reserves the distinct Session list, then leases the WHOLE set before polling.
/// Leases remain held while decoding/callbacks run. They block open, close,
/// revoke and shutdown before mutation, including a sibling in another entry.
/// Callbacks are non-raising; a Wasm trap terminates the instance and requires
/// external owner cleanup. This is not a claim of trap/allocator recovery.
pub struct Executor {
  entries : Array[(Resource, (Bytes) -> Int)]
  capacity : Int
  mut busy : Bool
  mut closed : Bool
}

pub fn new_executor(capacity : Int) -> Result[Executor, Int] {
  if capacity < 0 || capacity > 4194304 { return Err(90) }
  Ok({ entries: [], capacity, busy: false, closed: false })
}

pub fn Executor::add(self : Executor, source : Resource, callback : (Bytes) -> Int) -> Int {
  if self.closed { return 9 }
  if self.busy || source.session.leased || source.session.retiring { return 17 }
  if self.entries.length() >= 64 { return 11 }
  match source.operation {
    TopicSubscribe | ServiceAdvertise | ActionAdvertise => ()
    _ => return 22
  }
  match source.raw_handle() { Err(code) => return code; Ok(_) => () }
  self.entries.push((source, callback))
  0
}

pub(all) struct Spin {
  code : Int
  delivered : Int
}

pub fn Executor::spin_once(self : Executor) -> Spin {
  if self.closed { return { code: 9, delivered: 0 } }
  if self.busy { return { code: 17, delivered: 0 } }
  let sessions : Array[Session] = []
  for (source, _) in self.entries {
    if source.session.leased || source.session.retiring { return { code: 17, delivered: 0 } }
    match source.raw_handle() { Err(code) => return { code, delivered: 0 }; Ok(_) => () }
    if !sessions.any(fn(item) { physical_equal(item, source.session) }) {
      sessions.push(source.session)
    }
  }
  // No callback/import/allocation occurs between the complete preflight and
  // acquisition of these scalar guest-thread leases.
  self.busy = true
  for session in sessions { session.leased = true }
  let mut delivered = 0
  let mut failure = 0
  for (source, callback) in self.entries {
    let reply = source.poll_leased(self.capacity)
    if reply.code == 11 { continue }
    if reply.code != 0 { failure = reply.code; break }
    let code = callback(reply.data)
    if code != 0 { failure = code; break }
    delivered = delivered + 1
  }
  for session in sessions { session.leased = false }
  self.busy = false
  { code: failure, delivered }
}

pub fn Executor::close(self : Executor) -> Int {
  if self.closed { return 9 }
  if self.busy { return 17 }
  self.closed = true
  self.entries.clear()
  0
}
