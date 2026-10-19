// Concrete owner group for the separately opted-in owned_service package.
// This file imports no optional namespace. Original81/client catalogs stay exact.
// Session owns real capabilities; local roles/labels/receipts mint no grants.

pub struct OwnedOwners {
  session : Session
  owners : Array[Resource]
  source : Resource
  peer : Resource
  availability : Resource?
  role : Int
  cleanup : CleanupReport
  cleanup_reserved : CleanupReport?
  mut closed : Bool
  mut closing : Bool
}

pub fn OwnedOwners::role(self : OwnedOwners) -> Int { self.role }
pub fn OwnedOwners::source_handle(self : OwnedOwners) -> UInt64 { self.source.handle }
pub fn OwnedOwners::peer_handle(self : OwnedOwners) -> UInt64 { self.peer.handle }
pub fn OwnedOwners::availability_handle(self : OwnedOwners) -> UInt64 {
  match self.availability { Some(owner) => owner.handle; None => 0UL }
}
pub fn OwnedOwners::cleanup(self : OwnedOwners) -> CleanupReport { self.cleanup }

/// Real same-Session ownership preflight before any mutation. This is a local
/// guest-thread lease; the Host independently enforces Instance/grant/backend/
/// operation/session/generation on every raw call. No native callback enters it.
pub fn OwnedOwners::enter(self : OwnedOwners) -> Int {
  if self.closing || self.session.leased || self.session.retiring { return 17 }
  if self.closed || self.session.closed { return 9 }
  for owner in self.owners {
    if owner.closed || !physical_equal(owner.session, self.session) ||
        (owner.handle & 4294967295UL) == 0UL || (owner.handle >> 32) == 0UL { return 9 }
  }
  self.session.leased = true
  0
}

/// Call exactly once after a successful enter and all decode/ACK work. No
/// user callback is accepted here. A terminal Guest trap uses Runtime cleanup.
pub fn OwnedOwners::leave(self : OwnedOwners) -> Unit { self.session.leased = false }

/// Cleanup needs a busy preflight, NOT an all-owners-still-live precondition.
/// An externally closed/revoked first owner must not suppress other raw closes.
pub fn OwnedOwners::begin_close(self : OwnedOwners) -> Int {
  if self.closing || self.session.leased || self.session.retiring { return 17 }
  self.session.leased = true
  0
}

/// Retire every independently acquired raw owner in reverse order, once.
/// Endpoint code must end its Session lease before calling this, while keeping
/// its separate endpoint closing guard set through every cleanup outcome.
/// The report and Option ledger were reserved before the first real open.
pub fn OwnedOwners::close(self : OwnedOwners, primary : Int) -> Int {
  if self.closing || self.session.leased || self.session.retiring { return 17 }
  if self.closed { return self.cleanup.code }
  self.closing = true
  self.closed = true
  self.session.last_cleanup = self.cleanup_reserved
  self.session.retire_reserved(self.owners, self.cleanup, primary)
  self.closing = false
  self.cleanup.code
}

fn Node::owned_roles(
  self : Node, source_alias : String, peer_alias : String,
  available_alias : String?, role : Int,
) -> Result[OwnedOwners, Int] {
  if role != 3 && role != 4 { return Err(22) }
  let aliases = [source_alias, peer_alias]
  match available_alias { Some(value) => aliases.push(value); None => () }
  if !distinct_aliases(aliases) { return Err(22) }
  if role == 4 && aliases.length() != 2 { return Err(22) }
  // Validate every exact UTF8 alias before reserving/opening any owner.
  for alias in aliases {
    let bytes = match @wire.bounded_utf8(alias, 255) { Ok(value) => value; Err(code) => return Err(code) }
    if bytes.length() == 0 { return Err(22) }
    for byte in bytes { if byte.to_int() == 0 { return Err(22) } }
  }
  let source = match self.prepare_open(source_alias, if role == 3 { ServiceCall } else { ServiceAdvertise }) {
    Ok(value) => value; Err(code) => return Err(code)
  }
  let peer = match self.prepare_open(peer_alias, if role == 3 { EventPollV1 } else { ServiceRespond }) {
    Ok(value) => value; Err(code) => return Err(code)
  }
  let pending = [source, peer]
  let work = [source.owner, peer.owner]
  let mut availability : Resource? = None
  match available_alias {
    Some(alias) => {
      let entry = match self.prepare_open(alias, ServiceAvailable) { Ok(value) => value; Err(code) => return Err(code) }
      availability = Some(entry.owner)
      pending.push(entry)
      work.push(entry.owner)
    }
    None => ()
  }
  let cleanup = reserve_cleanup(work, 0)
  let cleanup_reserved = Some(cleanup)
  let owners : OwnedOwners = { session: self.session, owners: work,
    source: source.owner, peer: peer.owner, availability, role, cleanup,
    cleanup_reserved, closed: false, closing: false }
  let success = Ok(owners)
  self.session.cleanup_reports.push(cleanup)
  // No wrapper/list/result allocation follows the first successful open.
  let mut code = 0
  for entry in pending {
    code = entry.acquire()
    if code != 0 { break }
  }
  if code != 0 {
    self.session.last_cleanup = cleanup_reserved
    self.session.retire_reserved(work, cleanup, code)
    owners.closed = true
    return Err(code)
  }
  success
}

/// Open exactly the independently granted caller-selected client roles.
/// Optional availability is a distinct alias; no probe client/graph fallback.
/// Native activation is still a separate one-shot optional raw12 operation.
pub fn Node::owned_client_owners(
  self : Node, call_alias : String, observe_alias : String, available_alias : String?,
) -> Result[OwnedOwners, Int] {
  self.owned_roles(call_alias, observe_alias, available_alias, 3)
}

/// Open only caller-selected advertise/respond grants, without native activation.
pub fn Node::owned_service_owners(
  self : Node, advertise_alias : String, respond_alias : String,
) -> Result[OwnedOwners, Int] {
  self.owned_roles(advertise_alias, respond_alias, None, 4)
}
