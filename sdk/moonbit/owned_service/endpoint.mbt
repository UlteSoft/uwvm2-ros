// Concrete same-Wasm retained ROS2 endpoint. Native/raw owners are independent.
// No HostTransport, implicit policy, background work, automatic retry or native
// callback exists. Allocation traps are terminal; actual Runtime must retire
// the Instance. A retained attempted frame never becomes a false cleanupPASS.

struct Identity { marker : Int }

pub struct Request {
  identity : Identity
  index : Int
  mut ticket : UInt64
  // 0unused,1prepared,2admitted,3abandoned,4finished,5unknown,6responding.
  mut state : Int
  mut code : Int
  mut attempt : @linear.ReservedReply?
  mut information : Delivery?
}

pub fn Request::ticket(self : Request) -> UInt64 { self.ticket }
pub fn Request::state(self : Request) -> Int { self.state }
pub fn Request::code(self : Request) -> Int { self.code }
pub fn Request::attempt(self : Request) -> @linear.ReservedReply? { self.attempt }
pub fn Request::information(self : Request) -> Delivery? { self.information }

pub struct Endpoint {
  identity : Identity
  owners : @ros2.OwnedOwners
  expected_qos : Array[Bytes]
  receipt : EndpointReceipt
  retire_frame : @linear.ReservedReply
  maximum_body : Int
  packet_capacity : Int
  rows : Array[Request]
  tickets : Array[UInt64]
  slots : Array[Int]
  native_slots : Array[Int]
  mut used_rows : Int
  // 0prepared,1live,2unknown,3closed. Never reset/regrant/reuse history.
  mut state : Int
  mut busy : Bool
  mut last_committed : UInt64
  mut last_nonce : UInt64
  mut last_code : Int
  mut last_take_idle : Bool
  mut last_attempt : @linear.ReservedReply?
  mut native_retire_attempted : Bool
  mut native_retire_code : Int
  mut close_code : Int
}

/// Caller supplies actual separately opened owners, both exact expected QoS80
/// rows, its byte quota and a finite local lifetime-history budget <=65536.
/// This budget is SDK bookkeeping, never a claimed native client/service quota.
/// Preparing this object creates no native entity; activate() is explicit.
pub fn prepare_endpoint(
  owners : @ros2.OwnedOwners, expected_qos : Array[Bytes],
  maximum_body : Int, history_budget : Int,
) -> Result[Endpoint, Int] {
  if raw_abi_version() != 1 { return Err(95) }
  if maximum_body < 0 || maximum_body > MAX_BODY || history_budget < 1 || history_budget > 65536 ||
      expected_qos.length() != 2 { return Err(22) }
  for qos in expected_qos { if !expected_qos_valid(qos) { return Err(22) } }
  let activation = match @linear.reserve_reply(256) { Ok(value) => value; Err(code) => return Err(code) }
  let retire_frame = match @linear.reserve_reply(0) { Ok(value) => value; Err(code) => return Err(code) }
  let identity : Identity = { marker: 1 }
  let rows = Array::makei(history_budget, fn(index) {
    { identity, index, ticket: 0UL, state: 0, code: 0, attempt: None, information: None }
  })
  let mut table_size = 2
  while table_size < history_budget * 2 { table_size = table_size * 2 }
  Ok({ identity, owners, expected_qos: [expected_qos[0].copy(), expected_qos[1].copy()],
    receipt: prepare_receipt(activation, owners.source_handle(), owners.role()), retire_frame,
    maximum_body, packet_capacity: 352 + maximum_body, rows,
    tickets: Array::makei(table_size, fn(_) { 0UL }), slots: Array::makei(table_size, fn(_) { -1 }),
    native_slots: Array::makei(table_size, fn(_) { -1 }),
    used_rows: 0, state: 0, busy: false, last_committed: 0UL, last_nonce: 0UL,
    last_code: 0, last_take_idle: false, last_attempt: None, native_retire_attempted: false, native_retire_code: 0, close_code: 0 })
}

fn Endpoint::enter(self : Endpoint, live : Bool) -> Int {
  if self.busy || !@linear.arena_idle() { return 17 }
  if self.state == 3 { return 9 }
  if self.state == 2 { return 107 }
  if live && self.state != 1 { return 9 }
  let code = self.owners.enter()
  if code != 0 { return code }
  self.busy = true
  0
}

fn Endpoint::leave(self : Endpoint, code : Int) -> Int {
  self.last_code = code
  self.owners.leave()
  self.busy = false
  code
}

fn Endpoint::unknown(self : Endpoint, code : Int) -> Int {
  self.state = 2
  self.leave(if code == 0 { 107 } else { code })
}

fn Endpoint::find(self : Endpoint, ticket : UInt64) -> Int {
  if ticket == 0UL { return -1 }
  let mask = (self.tickets.length() - 1).to_uint64()
  let mut at = ((ticket ^ (ticket >> 32)) & mask).to_int()
  for _index = 0; _index < self.tickets.length(); _index = _index + 1 {
    if self.tickets[at] == 0UL { return -1 }
    if self.tickets[at] == ticket { return self.slots[at] }
    at = (at + 1) & (self.tickets.length() - 1)
  }
  -1
}

fn Endpoint::register(self : Endpoint, row : Request, ticket : UInt64) -> Bool {
  if ticket == 0UL || self.find(ticket) >= 0 { return false }
  let mask = (self.tickets.length() - 1).to_uint64()
  let mut at = ((ticket ^ (ticket >> 32)) & mask).to_int()
  for _index = 0; _index < self.tickets.length(); _index = _index + 1 {
    if self.tickets[at] == 0UL {
      self.tickets[at] = ticket
      self.slots[at] = row.index
      row.ticket = ticket
      return true
    }
    at = (at + 1) & (self.tickets.length() - 1)
  }
  false
}

fn same_native_id(left : Delivery, right : Delivery) -> Bool {
  if left.native_sequence != right.native_sequence || left.writer.length() != 16 || right.writer.length() != 16 { return false }
  for index = 0; index < 16; index = index + 1 {
    if left.writer[index] != right.writer[index] { return false }
  }
  true
}

fn Endpoint::native_slot(self : Endpoint, value : Delivery) -> Int {
  let mut hash = value.native_sequence.reinterpret_as_uint64()
  for byte in value.writer { hash = (hash ^ byte.to_int().to_uint64()) * 1099511628211UL }
  let mut at = (hash & (self.native_slots.length() - 1).to_uint64()).to_int()
  for _index = 0; _index < self.native_slots.length(); _index = _index + 1 {
    let row = self.native_slots[at]
    if row < 0 { return at }
    match self.rows[row].information {
      Some(previous) => if same_native_id(previous, value) { return -1 }
      None => return -1
    }
    at = (at + 1) & (self.native_slots.length() - 1)
  }
  -1
}

/// Activation is a single attempt. Receipt and actual call witness already
/// exist; complete numeric admission afterwards allocates no managed wrapper.
/// Failure does not silently close/lose diagnostics; caller must inspect/close.
pub fn Endpoint::activate(self : Endpoint) -> Int {
  if self.state != 0 { return if self.busy { 17 } else { 9 } }
  let attempt = Some(self.receipt.frame)
  let code = self.enter(false)
  if code != 0 { return code }
  self.last_attempt = attempt
  let code = self.receipt.frame.invoke(b"", fn(_, output) {
    if self.owners.role() == 3 {
      raw_client_activate(self.owners.source_handle(), self.owners.peer_handle(), 0, output)
    } else {
      raw_service_activate(self.owners.source_handle(), self.owners.peer_handle(), 0, output)
    }
  })
  if code != 0 { return self.unknown(code) }
  let code = admit_receipt(self.receipt, 1, self.expected_qos)
  if code != 0 { return self.unknown(code) }
  self.state = 1
  self.leave(0)
}

/// Query the SAME retained incarnation and compare every receipt byte except
/// the declared inspect operation. No probe client or type/QoS fallback.
pub fn Endpoint::inspect(self : Endpoint) -> Int {
  let frame = match @linear.reserve_reply(256) { Ok(value) => value; Err(code) => return code }
  let receipt = prepare_receipt(frame, self.receipt.source, self.receipt.role)
  let attempt = Some(frame)
  let code = self.enter(true)
  if code != 0 { return code }
  self.last_attempt = attempt
  let code = frame.invoke(b"", fn(_, output) {
    raw_endpoint_inspect(self.receipt.source, self.receipt.incarnation, 0, output)
  })
  if code != 0 { return self.unknown(code) }
  let code = admit_receipt(receipt, 2, self.expected_qos)
  if code != 0 || !same_receipt(self.receipt, receipt) { return self.unknown(if code == 0 { 17 } else { code }) }
  self.leave(0)
}

/// Typed wrappers must completely encode/validate the actual request before
/// calling this byte-level operation. No partial legacy String projection.
pub fn Endpoint::submit(self : Endpoint, request : Bytes) -> Result[Request, Int] {
  if self.owners.role() != 3 || request.length() > self.maximum_body { return Err(22) }
  if self.used_rows >= self.rows.length() { return Err(11) }
  let frame = match @linear.reserve_reply(8) { Ok(value) => value; Err(code) => return Err(code) }
  let row = self.rows[self.used_rows]
  let success = Ok(row)
  let attempt = Some(frame)
  let code = self.enter(true)
  if code != 0 { return Err(code) }
  self.used_rows = self.used_rows + 1
  row.state = 1
  row.attempt = attempt
  self.last_attempt = attempt
  let code = frame.invoke(request, fn(input, output) {
    raw_client_submit(self.receipt.source, self.receipt.incarnation, 0, input, request.length().to_uint64(), output)
  })
  if code != 0 {
    row.state = 5
    row.code = code
    return Err(self.unknown(code))
  }
  if !self.register(row, word(frame, 0, 8)) {
    row.state = 5
    row.code = 107
    return Err(self.unknown(107))
  }
  row.state = 2
  row.code = 0
  let _code = self.leave(0)
  success
}

/// Local abandon ONCE only from ADMITTED. ABANDONED remains a valid state for
/// one later FULL metadata/body decode and disposal, never native DDS cancel.
pub fn Endpoint::abandon(self : Endpoint, row : Request) -> Int {
  if self.owners.role() != 3 || !physical_equal(self.identity, row.identity) || row.state != 2 { return 9 }
  let frame = match @linear.reserve_reply(0) { Ok(value) => value; Err(code) => return code }
  let attempt = Some(frame)
  let code = self.enter(true)
  if code != 0 { return code }
  row.state = 3
  row.attempt = attempt
  self.last_attempt = attempt
  let code = frame.invoke(b"", fn(_, _) { raw_client_abandon(self.receipt.source, self.receipt.incarnation, row.ticket) })
  row.code = code
  if code != 0 { row.state = 5; return self.unknown(code) }
  self.leave(0)
}

/// Pure complete generated-codec validation runs BEFORE one ACK. It may only
/// prepare an application-owned decoded value; application work starts after
/// successful return. A decoder11 is an error, never an empty native queue.
pub fn Endpoint::take(self : Endpoint, validate_body : (Bytes) -> Int) -> Result[Delivery, Int] {
  let packet = match @linear.reserve_reply(self.packet_capacity) { Ok(value) => value; Err(code) => return Err(code) }
  let ack = match @linear.reserve_reply(0) { Ok(value) => value; Err(code) => return Err(code) }
  let cancel = match @linear.reserve_reply(0) { Ok(value) => value; Err(code) => return Err(code) }
  let borrow = new_borrow()
  let peek_attempt = Some(packet)
  let ack_attempt = Some(ack)
  let cancel_attempt = Some(cancel)
  let code = self.enter(true)
  if code != 0 { return Err(code) }
  self.last_take_idle = false
  self.last_attempt = peek_attempt
  let code = packet.invoke(b"", fn(_, output) {
    raw_endpoint_peek(self.receipt.source, self.receipt.incarnation, 16, 0, output, self.packet_capacity.to_uint64())
  })
  if code != 0 {
    // Only this actual raw peek result is queue-idle. A decoder/cancel error11
    // must remain an application failure, even though its numeric errno agrees.
    self.last_take_idle = code == 11
    let code = if code == 11 || code == 90 { self.leave(code) } else { self.unknown(code) }
    return Err(code)
  }
  let decoded = decode_delivery(packet, self.receipt, self.owners.peer_handle(), self.maximum_body,
                                self.last_committed, self.last_nonce, borrow)
  let mut failure = 0
  let delivery = match decoded {
    Ok(value) => Some(value)
    Err(code) => { failure = code; None }
  }
  if !borrow.associated { return Err(self.unknown(if failure == 0 { 22 } else { failure })) }
  self.last_nonce = borrow.nonce
  let mut row_index = -1
  let mut native_slot = -1
  match delivery {
    Some(value) => {
      if !value.control {
        // Even abandoned responses receive complete canonical body validation.
        failure = validate_body(value.body)
        if failure == 0 {
          row_index = self.find(borrow.ticket)
          if self.owners.role() == 3 {
            if row_index < 0 || (self.rows[row_index].state != 2 && self.rows[row_index].state != 3) { failure = 9 }
            else { value.abandoned = self.rows[row_index].state == 3 }
          } else {
            if row_index >= 0 { failure = 13 }
            else if self.used_rows >= self.rows.length() { failure = 11 }
            else {
              native_slot = self.native_slot(value)
              if native_slot < 0 { failure = 13 }
            }
          }
        }
      } else if borrow.ticket != 0UL {
        row_index = self.find(borrow.ticket)
        if row_index < 0 ||
            (self.owners.role() == 3 && self.rows[row_index].state != 2 && self.rows[row_index].state != 3) ||
            (self.owners.role() == 4 && self.rows[row_index].state != 2) {
          failure = 9
        } else { value.abandoned = self.rows[row_index].state == 3 }
      }
    }
    None => ()
  }
  if failure != 0 {
    self.last_attempt = cancel_attempt
    let code = cancel.invoke(b"", fn(_, _) {
      raw_endpoint_cancel(self.receipt.source, self.receipt.incarnation, borrow.sequence, borrow.nonce)
    })
    if code != 0 { return Err(self.unknown(code)) }
    return Err(self.leave(failure))
  }
  let value = match delivery { Some(value) => value; None => return Err(self.unknown(22)) }
  let information = Some(value)
  let success = Ok(value)
  if !value.control && self.owners.role() == 4 {
    row_index = self.used_rows
    self.used_rows = self.used_rows + 1
    let row = self.rows[row_index]
    if !self.register(row, borrow.ticket) { return Err(self.unknown(13)) }
    row.state = 1
    row.information = information
    self.native_slots[native_slot] = row_index
  }
  self.last_attempt = ack_attempt
  let code = ack.invoke(b"", fn(_, _) {
    raw_endpoint_commit(self.receipt.source, self.receipt.incarnation, borrow.sequence, borrow.nonce)
  })
  if code != 0 {
    if row_index >= 0 { self.rows[row_index].state = 5; self.rows[row_index].code = code }
    // NEVER cancel/retry/replay an unknown commit.
    return Err(self.unknown(code))
  }
  self.last_committed = borrow.sequence
  if row_index >= 0 {
    let row = self.rows[row_index]
    row.information = information
    row.state = if value.control || self.owners.role() == 3 { 4 } else { 2 }
    row.code = if value.control { value.error.to_int() } else { 0 }
  }
  let _code = self.leave(0)
  // Actual request_error produces kind6/flags0 for only one request. Preserve
  // that row's failure without claiming the whole native source was retired.
  // Source-retired flag1 is the separate terminal endpoint fact.
  if value.control && (value.control_flags & 1) != 0 {
    self.state = 2
    self.last_code = if value.error != 0U { value.error.to_int() } else { 125 }
  }
  success
}

/// Find only an already committed genuine owned server request. This returns
/// a local reference; numeric correlation alone is never a bearer permission.
pub fn Endpoint::request(self : Endpoint, ticket : UInt64) -> Result[Request, Int] {
  let index = self.find(ticket)
  if index < 0 || self.owners.role() != 4 || self.rows[index].state != 2 { Err(9) }
  else { Ok(self.rows[index]) }
}

pub fn Endpoint::respond(self : Endpoint, row : Request, response : Bytes) -> Int {
  if self.owners.role() != 4 || !physical_equal(row.identity, self.identity) || row.state != 2 ||
      response.length() > self.maximum_body { return 9 }
  let frame = match @linear.reserve_reply(0) { Ok(value) => value; Err(code) => return code }
  let attempt = Some(frame)
  let code = self.enter(true)
  if code != 0 { return code }
  row.state = 6
  row.attempt = attempt
  self.last_attempt = attempt
  let code = frame.invoke(response, fn(input, _) {
    raw_service_respond(self.receipt.source, self.receipt.incarnation, self.owners.peer_handle(),
                        row.ticket, 0, input, response.length().to_uint64())
  })
  row.code = code
  if code != 0 { row.state = 5; return self.unknown(code) }
  row.state = 4
  self.leave(0)
}

pub fn Endpoint::available(self : Endpoint, wait_ns : UInt64) -> Result[Bool, Int] {
  if self.owners.role() != 3 || self.owners.availability_handle() == 0UL { return Err(13) }
  if wait_ns > MAX_WAIT_NS { return Err(22) }
  let frame = match @linear.reserve_reply(1) { Ok(value) => value; Err(code) => return Err(code) }
  let attempt = Some(frame)
  let yes = Ok(true)
  let no = Ok(false)
  let code = self.enter(true)
  if code != 0 { return Err(code) }
  self.last_attempt = attempt
  let code = frame.invoke(b"", fn(_, output) {
    raw_client_available(self.receipt.source, self.receipt.incarnation, self.owners.availability_handle(), wait_ns, 0, output)
  })
  if code != 0 { return Err(self.leave(code)) }
  let result = frame.byte_at(0)
  if result != 0 && result != 1 { return Err(self.unknown(22)) }
  let _code = self.leave(0)
  if result == 1 { yes } else { no }
}

/// Native retire at most once, THEN independent reverse raw close. The endpoint
/// busy guard spans both, but its Session lease ends before raw owner cleanup.
/// Keep native and every raw cleanup status separately; repeated close replays
/// neither phase. Failed/unknown activation still closes all actual raw owners.
pub fn Endpoint::close(self : Endpoint) -> Int {
  if self.busy || !@linear.arena_idle() { return 17 }
  if self.state == 3 { return self.close_code }
  let code = self.owners.begin_close()
  if code != 0 { return code }
  self.busy = true
  let mut primary = if self.state == 2 { self.last_code } else { 0 }
  self.state = 3
  if self.receipt.valid && !self.native_retire_attempted {
    self.native_retire_attempted = true
    let code = self.retire_frame.invoke(b"", fn(_, _) {
      raw_endpoint_retire(self.receipt.source, self.receipt.incarnation)
    })
    self.native_retire_code = code
    if primary == 0 && code != 0 { primary = code }
  }
  self.owners.leave()
  let code = self.owners.close(primary)
  self.close_code = code
  self.busy = false
  code
}

pub fn Endpoint::last_attempt(self : Endpoint) -> @linear.ReservedReply? { self.last_attempt }
pub fn Endpoint::last_code(self : Endpoint) -> Int { self.last_code }
pub fn Endpoint::last_take_idle(self : Endpoint) -> Bool { self.last_take_idle }
pub fn Endpoint::native_retire_attempted(self : Endpoint) -> Bool { self.native_retire_attempted }
pub fn Endpoint::native_retire_code(self : Endpoint) -> Int { self.native_retire_code }
pub fn Endpoint::cleanup(self : Endpoint) -> @ros2.CleanupReport { self.owners.cleanup() }
pub fn Endpoint::receipt(self : Endpoint) -> EndpointReceipt { self.receipt }
