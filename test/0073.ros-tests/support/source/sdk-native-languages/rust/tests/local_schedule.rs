//! Native, SDK-local tests with no ROS imports, clock reads or Wasm execution.
//! Run with `--features alloc,ros1` or alloc,ros2 in a separate one-job target.
#![cfg(feature = "alloc")]
use roswasm::{
    codes,
    local::{CallbackGroup, Context},
    Error,
};
use std::{cell::Cell, rc::Rc};

#[test]
fn future_is_one_shot_and_never_runs_inline() {
    let mut context = Context::new(2).unwrap();
    let calls = Rc::new(Cell::new(0));
    let seen = calls.clone();
    let future = context
        .defer(move || {
            seen.set(seen.get() + 1);
            Ok(42u64)
        })
        .unwrap();
    assert_eq!(calls.get(), 0);
    assert_eq!(future.try_take().unwrap_err().code, codes::AGAIN);
    assert_eq!(context.spin_once(0).unwrap(), 1);
    assert!(future.is_ready());
    assert_eq!(future.try_take().unwrap(), 42);
    assert_eq!(calls.get(), 1);
    assert_eq!(future.try_take().unwrap_err().code, codes::BAD_HANDLE);
    assert_eq!(context.spin_once(0).unwrap(), 0);
}
#[test]
fn local_cancellation_does_not_execute_or_revoke_native_authority() {
    let mut context = Context::new(1).unwrap();
    let calls = Rc::new(Cell::new(0));
    let seen = calls.clone();
    let future = context
        .defer(move || {
            seen.set(1);
            Ok(())
        })
        .unwrap();
    future.cancel().unwrap();
    assert_eq!(future.cancel().unwrap_err().code, codes::BAD_HANDLE);
    assert_eq!(context.spin_once(1).unwrap(), 1);
    assert_eq!(calls.get(), 0);
    assert_eq!(future.try_take().unwrap_err().code, codes::REVOKED);
}
#[test]
fn timer_budget_exact_time_and_shutdown_are_checked() {
    assert!(Context::new(0).is_err());
    assert!(Context::new(1025).is_err());
    let mut context = Context::new(1).unwrap();
    let count = Rc::new(Cell::new(0));
    let seen = count.clone();
    let timer = context
        .timer(10, 5, CallbackGroup::new(), move || {
            seen.set(seen.get() + 1);
            Ok(())
        })
        .unwrap();
    assert_eq!(
        context.defer(|| Ok(())).err().unwrap().code,
        codes::TOO_LARGE
    );
    assert_eq!(context.spin_once(9).unwrap(), 0);
    assert_eq!(context.spin_once(10).unwrap(), 1);
    assert_eq!(context.spin_once(100).unwrap(), 1);
    assert_eq!(count.get(), 2); // no catch-up replay
    assert_eq!(context.spin_once(99).unwrap_err().code, codes::INVALID);
    assert_eq!(count.get(), 2);
    context.shutdown().unwrap();
    assert!(!timer.is_active());
    assert_eq!(timer.cancel().unwrap_err().code, codes::BAD_HANDLE);
    assert_eq!(context.spin_once(101).unwrap_err().code, codes::BAD_HANDLE);
    assert_eq!(context.shutdown().unwrap_err().code, codes::BAD_HANDLE);
    assert_eq!(
        context.defer(|| Ok(())).err().unwrap().code,
        codes::BAD_HANDLE
    );
}
#[test]
fn group_refuses_reentry_and_preserves_callback_error() {
    let group = CallbackGroup::new();
    let same = group.clone();
    group
        .run(|| {
            assert_eq!(same.run(|| Ok(())).unwrap_err().code, codes::CONFLICT);
            Ok(())
        })
        .unwrap();
    assert_eq!(
        group
            .run(|| Err::<(), _>(Error::new(codes::DENIED)))
            .unwrap_err()
            .code,
        codes::DENIED
    );
    group.run(|| Ok(())).unwrap();
}
#[test]
fn shutdown_cancels_pending_work_and_timer_overflow_never_dispatches() {
    let mut context = Context::new(2).unwrap();
    let future = context.defer(|| Ok(1)).unwrap();
    context.shutdown().unwrap();
    assert_eq!(future.try_take().unwrap_err().code, codes::REVOKED);
    let mut context = Context::new(1).unwrap();
    let timer = context
        .timer(u64::MAX, 1, CallbackGroup::new(), || {
            panic!("overflow dispatched")
        })
        .unwrap();
    assert_eq!(
        context.spin_once(u64::MAX).unwrap_err().code,
        codes::INVALID
    );
    assert!(!timer.is_active());
}
