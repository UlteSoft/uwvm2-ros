//! Bounded local ownership groups for applications using the `alloc` feature.
//!
//! A Session selects one existing import backend and checks ABI version1. It
//! creates no native ROS node, policy, connection credentials or resource grant.
//! Nodes created through this Session retain its local registry; ordinary
//! `Node::new` keeps the allocation-free independent ownership API. The actual
//! Runtime caller is still this Wasm module/Instance, never a Session label.
//!
//! Explicit `close` attempts every live registered handle once and returns every
//! status. Retirement precedes imports, including backend failure. The Session
//! remains closed afterwards and its Nodes cannot silently open/reconnect.
//! Deterministic Drop is only best-effort fallback, not checked cleanup or a
//! guarantee after a Guest trap, abort or process exit. No background thread,
//! timer, default clock, environment, network or automatic retry is introduced.
use crate::{codes, node::Node, Backend, Capability, Error};
use alloc::{rc::Rc, vec::Vec};
use core::cell::{Cell, RefCell};

pub(crate) struct HandleState {
    pub(crate) backend: Backend,
    pub(crate) handle: Cell<Option<u64>>,
    pub(crate) callback_busy: Cell<bool>,
}
pub(crate) struct Registry {
    closed: Cell<bool>,
    maximum: usize,
    entries: RefCell<Vec<Rc<HandleState>>>,
}
impl Registry {
    pub(crate) fn open(&self, backend: Backend, alias: &str) -> Result<Capability, Error> {
        if self.closed.get() { return Err(Error::new(codes::BAD_HANDLE)); }
        let mut entries = self.entries.try_borrow_mut().map_err(|_| Error::new(codes::CONFLICT))?;
        // Closed resources no longer consume this application's local registry
        // budget; retained typed wrappers still see their retired shared state.
        entries.retain(|entry| entry.handle.get().is_some());
        if entries.len() == self.maximum { return Err(Error::new(codes::TOO_LARGE)); }
        let mut capability = Capability::open(backend, alias)?;
        let state = Rc::new(HandleState { backend, handle: Cell::new(Some(capability.raw_handle()?)), callback_busy:Cell::new(false) });
        capability.attach(Rc::clone(&state)); entries.push(state); Ok(capability)
    }
    fn close(&self) -> Vec<Result<(), Error>> {
        // Preflight every shared callback state before closing the group or any
        // owner. Busy17 preserves factories/owners for cleanup after dispatch.
        if self.closed.get() { return alloc::vec![Err(Error::new(codes::BAD_HANDLE))]; }
        let mut entries = match self.entries.try_borrow_mut() {
            Ok(value) => value,
            Err(_) => return alloc::vec![Err(Error::new(codes::CONFLICT))],
        };
        if let Err(error)=states_idle(&entries) { return alloc::vec![Err(error)]; }
        self.closed.set(true);
        let results=close_states(&entries);
        entries.clear(); results
    }
}

pub(crate) fn states_idle(entries: &[Rc<HandleState>])->Result<(),Error> {
    crate::callback::cleanup_idle(entries.iter().map(|state|(state.handle.get().is_some(),&state.callback_busy)))
}
pub(crate) fn close_states(entries: &[Rc<HandleState>]) -> Vec<Result<(),Error>> {
    if let Err(error)=states_idle(entries) { return alloc::vec![Err(error)]; }
    let mut results=Vec::new();
    for state in entries { if let Some(handle)=state.handle.take() { results.push(state.backend.close_handle(handle)); } }
    results
}

/// An explicitly selected backend and bounded group of owned capabilities.
/// This SDK group is local; host policy still authorizes every individual alias.
pub struct Session {
    backend: Backend,
    registry: Rc<Registry>,
}
impl Session {
    /// Check ABI version1 and create a local budget of1..1024 live handles.
    /// The budget never raises native entity/byte/queue/goal/rate quotas. No
    /// allocation or open occurs when the limit or target/ABI is unsupported.
    pub fn new(backend: Backend, maximum_resources: usize) -> Result<Self, Error> {
        if maximum_resources == 0 || maximum_resources > 1024 {
            return Err(Error::new(codes::INVALID));
        }
        if backend.abi_version() != 1 { return Err(Error::new(codes::UNSUPPORTED)); }
        Ok(Self { backend, registry: Rc::new(Registry {
            closed: Cell::new(false), maximum: maximum_resources, entries: RefCell::new(Vec::new()),
        }) })
    }
    /// Return a local named organizer whose factories attach to this group.
    /// Constructing it opens nothing; its backend and native names are immutable
    /// host selections. Nodes may outlive this wrapper but become locally closed.
    pub fn node<'a>(&self, name: &'a str) -> Node<'a> {
        let mut node = Node::new(self.backend, name);
        node.registry = Some(Rc::clone(&self.registry)); node
    }
    /// Return the explicitly chosen backend without importing or opening anything.
    pub fn backend(&self) -> Backend { self.backend }
    /// Whether new group-owned resources are still permitted locally.
    /// This does not imply that the Runtime grants any alias or is connected.
    pub fn is_open(&self) -> bool { !self.registry.closed.get() }
    /// Retire and close every currently live resource, preserving each outcome.
    /// Inspect every returned Result. A timeout may follow native retirement;
    /// no close, write or queue operation is automatically replayed. Repeated
    /// group close reports BadHandle9 without issuing any further imports.
    /// A live server callback reports Conflict17 before any owner retires;
    /// the group remains open and must be closed after the callback returns.
    pub fn close(&self) -> Vec<Result<(), Error>> { self.registry.close() }
}
impl Drop for Session {
    fn drop(&mut self) { if !self.registry.closed.get() { let _ = self.registry.close(); } }
}
