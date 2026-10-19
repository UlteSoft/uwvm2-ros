//! Caller-driven local scheduling, enabled by `alloc`.
//!
//! This module never imports a ROS function or a clock. A deferred closure can
//! call an existing synchronous Client, but its call still blocks that one
//! caller-driven step until the host's deadline. These Futures are local result
//! slots, not native asynchronous ROS requests. Time passed to `spin_once` must
//! use one consistent monotonic domain selected by the application. Timers do
//! not infer permissions, sleep, create a thread or replay overdue callbacks.
use crate::{codes, Error};
use alloc::{boxed::Box, collections::VecDeque, rc::Rc, vec::Vec};
use core::cell::{Cell, RefCell};

/// A mutually exclusive guest-thread callback group; cloning shares its guard.
#[derive(Clone)]
pub struct CallbackGroup { busy: Rc<Cell<bool>> }
impl Default for CallbackGroup { fn default() -> Self { Self::new() } }
impl CallbackGroup {
    /// Create local state without importing or allocating any native resource.
    pub fn new() -> Self { Self { busy: Rc::new(Cell::new(false)) } }
    /// Invoke once with no borrow held across the application closure.
    /// A nested invocation of this same group reports Conflict17.
    pub fn run<T>(&self, callback: impl FnOnce() -> Result<T, Error>) -> Result<T, Error> {
        if self.busy.replace(true) { return Err(Error::new(codes::CONFLICT)); }
        struct Reset<'a>(&'a Cell<bool>);
        impl Drop for Reset<'_> { fn drop(&mut self) { self.0.set(false); } }
        let _reset = Reset(&self.busy); callback()
    }
}
enum Outcome<T> { Pending, Running, Ready(Result<T, Error>), Taken }
struct ResultSlot<T> { outcome: RefCell<Outcome<T>> }
/// One copied/owned result of a local deferred closure, without an implicit wait.
pub struct Future<T> { slot: Rc<ResultSlot<T>> }
impl<T> Future<T> {
    /// Whether completion or local cancellation has produced a result.
    pub fn is_ready(&self) -> bool { matches!(*self.slot.outcome.borrow(), Outcome::Ready(_)) }
    /// Take the owned result once. Pending is Again11 and a consumed slot is9.
    pub fn try_take(&self) -> Result<T, Error> {
        let mut state = self.slot.outcome.try_borrow_mut().map_err(|_| Error::new(codes::CONFLICT))?;
        match core::mem::replace(&mut *state, Outcome::Taken) {
            Outcome::Pending => { *state = Outcome::Pending; Err(Error::new(codes::AGAIN)) },
            Outcome::Running => { *state = Outcome::Running; Err(Error::new(codes::AGAIN)) },
            Outcome::Ready(result) => result,
            Outcome::Taken => Err(Error::new(codes::BAD_HANDLE)),
        }
    }
    /// Cancel only work that has not started. Its result becomes local125;
    /// this does not revoke an alias or undo a synchronous native effect.
    pub fn cancel(&self) -> Result<(), Error> {
        let mut state = self.slot.outcome.try_borrow_mut().map_err(|_| Error::new(codes::CONFLICT))?;
        if !matches!(*state, Outcome::Pending) { return Err(Error::new(codes::BAD_HANDLE)); }
        *state = Outcome::Ready(Err(Error::new(codes::REVOKED))); Ok(())
    }
}
struct Job { run: Box<dyn FnOnce()>, cancel: Box<dyn FnOnce()> }
struct TimerState { live: Cell<bool>, deadline: Cell<u64>, period: u64 }
struct TimerEntry { state: Rc<TimerState>, group: CallbackGroup, callback: Box<dyn FnMut() -> Result<(), Error>> }
/// Local timer registration with exact u64 nanoseconds and checked cancellation.
pub struct Timer { state: Rc<TimerState> }
impl Timer {
    /// Cancel once before a future pass. No native clock or close is imported.
    pub fn cancel(&self) -> Result<(), Error> {
        if !self.state.live.replace(false) { return Err(Error::new(codes::BAD_HANDLE)); } Ok(())
    }
    /// Whether the local registration is active; no native readiness claim.
    pub fn is_active(&self) -> bool { self.state.live.get() }
}
/// Bounded deferred queue and timers driven by explicit calls to `spin_once`.
/// Context here is local scheduling state, not a native rcl/rclcpp Context.
pub struct Context { maximum: usize, jobs: VecDeque<Job>, timers: Vec<TimerEntry>, closed: bool, spinning: Cell<bool>, last_time: Option<u64> }
impl Context {
    /// Select a combined budget of1..1024 queued jobs and active timers.
    pub fn new(maximum: usize) -> Result<Self, Error> {
        if maximum == 0 || maximum > 1024 { return Err(Error::new(codes::INVALID)); }
        Ok(Self { maximum, jobs: VecDeque::new(), timers: Vec::new(), closed: false, spinning: Cell::new(false), last_time: None })
    }
    fn room(&mut self) -> Result<(), Error> {
        if self.closed { return Err(Error::new(codes::BAD_HANDLE)); }
        if self.spinning.get() { return Err(Error::new(codes::CONFLICT)); }
        self.timers.retain(|entry| entry.state.live.get());
        if self.jobs.len() + self.timers.len() >= self.maximum { return Err(Error::new(codes::TOO_LARGE)); } Ok(())
    }
    /// Queue one owned closure. It runs in a later caller step, never on a thread.
    /// Captured references must be owned/'static; no borrowed event is retained.
    pub fn defer<T: 'static>(&mut self, callback: impl FnOnce() -> Result<T, Error> + 'static) -> Result<Future<T>, Error> {
        self.room()?;
        let slot = Rc::new(ResultSlot { outcome: RefCell::new(Outcome::Pending) });
        let run_slot = Rc::clone(&slot); let cancel_slot = Rc::clone(&slot);
        self.jobs.push_back(Job {
            run: Box::new(move || {
                let pending = { let mut value = run_slot.outcome.borrow_mut(); if matches!(*value, Outcome::Pending) { *value = Outcome::Running; true } else { false } };
                // Release the result-slot borrow before invoking application code.
                if pending { let result = callback(); *run_slot.outcome.borrow_mut() = Outcome::Ready(result); }
            }),
            cancel: Box::new(move || { let mut value = cancel_slot.outcome.borrow_mut(); if matches!(*value, Outcome::Pending) { *value = Outcome::Ready(Err(Error::new(codes::REVOKED))); } }),
        }); Ok(Future { slot })
    }
    /// Register a periodic callback. `first` is an absolute caller-domain time;
    /// period must be positive. An overdue pass invokes at most one callback,
    /// scheduling the next from `now`, not replaying missed ticks.
    pub fn timer(&mut self, first: u64, period: u64, group: CallbackGroup, callback: impl FnMut() -> Result<(), Error> + 'static) -> Result<Timer, Error> {
        self.room()?; if period == 0 { return Err(Error::new(codes::INVALID)); }
        let state = Rc::new(TimerState { live: Cell::new(true), deadline: Cell::new(first), period });
        self.timers.push(TimerEntry { state: Rc::clone(&state), group, callback: Box::new(callback) }); Ok(Timer { state })
    }
    /// Process the queue present at entry and each due timer at most once.
    /// Reject backwards caller time before any task/callback side effect. Returns
    /// the number of consumed jobs and timer callbacks, preserving first error.
    pub fn spin_once(&mut self, now: u64) -> Result<usize, Error> {
        if self.closed { return Err(Error::new(codes::BAD_HANDLE)); }
        if self.spinning.get() { return Err(Error::new(codes::CONFLICT)); }
        if self.last_time.is_some_and(|old| now < old) { return Err(Error::new(codes::INVALID)); }
        self.last_time = Some(now); self.spinning.set(true);
        struct Reset<'a>(&'a Cell<bool>);
        impl Drop for Reset<'_> { fn drop(&mut self) { self.0.set(false); } }
        let _reset = Reset(&self.spinning);
        let count = self.jobs.len(); let mut completed = 0;
        for _ in 0..count { if let Some(job) = self.jobs.pop_front() { (job.run)(); completed += 1; } }
        let mut first = None;
        for entry in &mut self.timers {
            if !entry.state.live.get() || now < entry.state.deadline.get() { continue; }
            match now.checked_add(entry.state.period) {
                None => { entry.state.live.set(false); if first.is_none() { first = Some(Error::new(codes::INVALID)); } },
                Some(next) => {
                    entry.state.deadline.set(next);
                    let callback = &mut entry.callback;
                    if let Err(error) = entry.group.run(|| callback()) { if first.is_none() { first = Some(error); } }
                    completed += 1;
                },
            }
        }
        match first { Some(error) => Err(error), None => Ok(completed) }
    }
    /// Stop locally, cancel queued results with local125 and disable timers.
    /// This imports no ROS close and does not interrupt a started synchronous RPC.
    /// Close a Session separately to retire its native capabilities explicitly.
    pub fn shutdown(&mut self) -> Result<(), Error> {
        if self.closed { return Err(Error::new(codes::BAD_HANDLE)); }
        if self.spinning.get() { return Err(Error::new(codes::CONFLICT)); }
        self.closed = true;
        while let Some(job) = self.jobs.pop_front() { (job.cancel)(); }
        for entry in &self.timers { entry.state.live.set(false); }
        self.timers.clear(); Ok(())
    }
}
impl Drop for Context { fn drop(&mut self) { if !self.closed && !self.spinning.get() { let _ = self.shutdown(); } } }
