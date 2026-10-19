//! Lesson7: Future, Context, Timer and CallbackGroup without implicit authority.
//!
//! Build with alloc. This lesson imports no clock and opens no ROS capability;
//! the literal caller times make the scheduling policy visible. In a real app,
//! pass timestamps from a separately granted monotonic Clock into spin_once.
//! A deferred Client call would remain a synchronous RPC in that later step.
use roswasm::{local::{CallbackGroup, Context}, Error};
fn run() -> Result<(), Error> {
    let mut context = Context::new(2)?;
    let future = context.defer(|| Ok(42u64))?;
    let timer = context.timer(10,5,CallbackGroup::new(),|| { println!("one local timer tick"); Ok(()) })?;
    context.spin_once(0)?; println!("deferred result {}",future.try_take()?);
    context.spin_once(10)?; // one callback, no catch-up loop or sleep
    timer.cancel()?;
    context.shutdown()?; // local scheduling only; close native Session separately
    Ok(())
}
fn main() { if let Err(error) = run() { eprintln!("local scheduling status {}",error.code); std::process::exit(1); } }
