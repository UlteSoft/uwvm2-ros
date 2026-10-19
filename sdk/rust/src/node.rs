//! Familiar application objects over explicitly granted capability aliases.
//!
//! A Node label is local to the guest. Resource creation opens an existing host
//! grant, never a native ROS endpoint chosen by the guest. Polling executes all
//! callbacks on the guest thread. Service/action advertise and respond aliases
//! remain separate, and clocks, TF, graph, logging and parameters have no implicit
//! rights. These types are a ROS-style guest API, not binary/source rclcpp compatibility.
use crate::{codec, codes, Backend, Capability, Error, Operation};

/// Local application context with an explicitly selected backend and label.
pub struct Node<'a> {
    backend: Backend,
    name: &'a str,
    closed: core::cell::Cell<bool>,
    #[cfg(feature = "alloc")]
    pub(crate) registry: Option<alloc::rc::Rc<crate::session::Registry>>,
    #[cfg(feature = "alloc")]
    owned: core::cell::RefCell<alloc::vec::Vec<alloc::rc::Rc<crate::session::HandleState>>>,
}
impl<'a> Node<'a> {
    /// Create a local label; this does not open a native node or confer authority.
    pub fn new(backend: Backend, name: &'a str) -> Self {
        Self { backend, name, closed: core::cell::Cell::new(false), #[cfg(feature = "alloc")] registry: None, #[cfg(feature = "alloc")] owned: core::cell::RefCell::new(alloc::vec::Vec::new()) }
    }
    fn open_capability(&self, alias: &str) -> Result<Capability, Error> {
        if self.closed.get() { return Err(Error::new(codes::BAD_HANDLE)); }
        #[cfg(feature = "alloc")]
        {
            let mut entries=self.owned.try_borrow_mut().map_err(|_| Error::new(codes::CONFLICT))?;
            entries.retain(|state| state.handle.get().is_some());
            if entries.len()>=1024 { return Err(Error::new(codes::TOO_LARGE)); }
            drop(entries); // No registry borrow is held across the host import.
            let mut capability=match &self.registry { Some(registry)=>registry.open(self.backend,alias)?, None=>Capability::open(self.backend,alias)? };
            let state=match capability.shared_state() {
                Some(state)=>state,
                None=>alloc::rc::Rc::new(crate::session::HandleState { backend:self.backend, handle:core::cell::Cell::new(Some(capability.raw_handle()?)), callback_busy:core::cell::Cell::new(false) }),
            };
            capability.attach(alloc::rc::Rc::clone(&state)); self.owned.borrow_mut().push(state); Ok(capability)
        }
        #[cfg(not(feature = "alloc"))]
        Capability::open(self.backend, alias)
    }
    /// Return the local diagnostic label.
    pub fn name(&self) -> &str {
        self.name
    }
    /// Close this organizer once and attempt its live handles with alloc enabled.
    /// Inspect every result. Retained typed owners then return9; Session siblings
    /// remain independently open. The no-alloc Node owns no registry: close only
    /// disables its factories and resource owners must be closed individually.
    /// With alloc, an active server callback returns17 before any retirement;
    /// this organizer and all its owners remain available for later cleanup.
    #[cfg(feature = "alloc")]
    pub fn close(&self) -> alloc::vec::Vec<Result<(),Error>> {
        if self.closed.get() { return alloc::vec![Err(Error::new(codes::BAD_HANDLE))]; }
        let mut entries=match self.owned.try_borrow_mut() { Ok(entries)=>entries, Err(_)=>return alloc::vec![Err(Error::new(codes::CONFLICT))] };
        if let Err(error)=crate::session::states_idle(&entries) { return alloc::vec![Err(error)]; }
        self.closed.set(true);
        let results=crate::session::close_states(&entries); entries.clear(); results
    }
    /// Disable factories locally once without alloc. Close each typed resource
    /// separately; this allocation-free organizer retains no handle registry.
    #[cfg(not(feature = "alloc"))]
    pub fn close(&self) -> Result<(),Error> { if self.closed.replace(true) { Err(Error::new(codes::BAD_HANDLE)) } else { Ok(()) } }
    /// Open a topic-publish alias whose native type and QoS are fixed by host policy.
    pub fn publisher(&self, alias: &str) -> Result<Publisher, Error> {
        Ok(Publisher(self.open_capability(alias)?))
    }
    /// Open and activate a topic-subscribe alias; Drop closes it on activation error.
    pub fn subscription(&self, alias: &str) -> Result<Subscription, Error> {
        let c = self.open_capability(alias)?;
        c.subscribe()?;
        Ok(Subscription(c))
    }
    /// Open an independently granted bounded service-call alias.
    pub fn client(&self, alias: &str) -> Result<Client, Error> {
        Ok(Client(self.open_capability(alias)?))
    }
    /// Open separate advertise/respond grants and activate the service event queue.
    pub fn server(&self, advertise_alias: &str, respond_alias: &str) -> Result<Server, Error> {
        let events = self.open_capability(advertise_alias)?;
        let respond = self.open_capability(respond_alias)?;
        events.advertise_service()?;
        Ok(Server { events, respond, spinning: core::cell::Cell::new(false) })
    }
    /// Open an action operation alias. Each send/receive/cancel method needs its
    /// matching alias; create multiple ActionClients when using multiple operations.
    pub fn action_client(&self, alias: &str) -> Result<ActionClient, Error> {
        Ok(ActionClient(self.open_capability(alias)?))
    }
    /// Open independent Action advertise/respond grants, activating the goal queue.
    pub fn action_server(
        &self,
        advertise_alias: &str,
        respond_alias: &str,
    ) -> Result<ActionServer, Error> {
        let events = self.open_capability(advertise_alias)?;
        let respond = self.open_capability(respond_alias)?;
        events.request(Operation::ActionAdvertise, &[], &mut [])?;
        Ok(ActionServer { events, respond, spinning: core::cell::Cell::new(false) })
    }
    /// Open one exact parameter operation grant; it does not grant other methods.
    pub fn parameters(&self, alias: &str) -> Result<Parameters, Error> {
        Ok(Parameters(self.open_capability(alias)?))
    }
    /// Open one bounded graph operation grant; snapshots are canonical Strings.
    pub fn graph(&self, alias: &str) -> Result<Graph, Error> {
        Ok(Graph(self.open_capability(alias)?))
    }
    /// Open one clock-domain grant; reading a different clock needs another alias.
    pub fn clock(&self, alias: &str) -> Result<Clock, Error> {
        Ok(Clock(self.open_capability(alias)?))
    }
    /// Open an independently granted log-writing capability.
    pub fn logger(&self, alias: &str) -> Result<Logger, Error> {
        Ok(Logger(self.open_capability(alias)?))
    }
    /// Open a selected TF lookup/broadcast alias with exact configured frame pair.
    pub fn tf(&self, alias: &str) -> Result<Tf, Error> {
        Ok(Tf(self.open_capability(alias)?))
    }
    /// Open a ROS 2 lifecycle get/change alias; configured transition cannot be overridden.
    pub fn lifecycle(&self, alias: &str) -> Result<Lifecycle, Error> {
        Ok(Lifecycle(self.open_capability(alias)?))
    }
}
/// An owned publisher over one explicit topic-publish grant.
pub struct Publisher(Capability);
impl Publisher {
    /// Publish exact canonical bytes, retaining native quotas and type checks.
    pub fn publish(&self, bytes: &[u8]) -> Result<(), Error> {
        self.0.publish(bytes)
    }
    /// Encode and publish a canonical String using caller-provided bounded scratch.
    pub fn publish_string(&self, text: &str, scratch: &mut [u8]) -> Result<(), Error> {
        let n = codec::encode_string(text, scratch)?;
        self.publish(&scratch[..n])
    }
    /// Encode and publish one canonical Bool.
    pub fn publish_bool(&self, value: bool) -> Result<(), Error> {
        self.publish(&[u8::from(value)])
    }
    /// Encode and publish a finite Twist with no allocation.
    pub fn publish_twist(&self, value: [f64; 6]) -> Result<(), Error> {
        let mut bytes = [0; 48];
        codec::encode_twist(value, &mut bytes)?;
        self.publish(&bytes)
    }
}
/// An activated event queue. Callback scheduling is always guest-driven polling.
pub struct Subscription(Capability);
impl Subscription {
    /// Poll one event; return AGAIN when empty. No implicit retry or sleep is performed.
    pub fn poll(&self, buffer: &mut [u8]) -> Result<usize, Error> {
        self.0.poll(buffer)
    }
    /// Poll and decode one canonical String, borrowing the caller's buffer.
    pub fn poll_string<'a>(&self, buffer: &'a mut [u8]) -> Result<&'a str, Error> {
        let n = self.poll(buffer)?;
        codec::decode_string(&buffer[..n])
    }
    /// Build a Subscription from an already activated service/action/parameter queue.
    /// The existing capability must authorize event_poll; no operation is changed.
    pub fn from_activated(capability: Capability) -> Self {
        Self(capability)
    }
}
/// Bounded synchronous service client, using a separately configured deadline.
pub struct Client(Capability);
impl Client {
    /// Call with canonical request bytes and an explicit response capacity.
    pub fn call(&self, request: &[u8], response: &mut [u8]) -> Result<usize, Error> {
        self.0.request(Operation::ServiceCall, request, response)
    }
    /// Call a Trigger capability with its empty canonical request.
    pub fn trigger<'a>(&self, response: &'a mut [u8]) -> Result<codec::ServiceResponse<'a>, Error> {
        let n = self.call(&[], response)?;
        codec::decode_service_response(&response[..n])
    }
    /// Call a SetBool capability with exactly one canonical byte.
    pub fn set_bool<'a>(
        &self,
        value: bool,
        response: &'a mut [u8],
    ) -> Result<codec::ServiceResponse<'a>, Error> {
        let n = self.call(&[u8::from(value)], response)?;
        codec::decode_service_response(&response[..n])
    }
}
/// Service server with independent event and response authority.
pub struct Server {
    events: Capability,
    respond: Capability,
    spinning: core::cell::Cell<bool>,
}
impl Server {
    /// Poll one validated request event, whose prefix is a Gateway-issued request ID.
    pub fn poll(&self, out: &mut [u8]) -> Result<usize, Error> {
        crate::callback::idle(&self.spinning)?;
        self.events.poll(out)
    }
    /// Respond using an encoded owned request ID and the selected response shape.
    pub fn respond(&self, reply: &[u8]) -> Result<(), Error> {
        self.respond
            .request(Operation::ServiceRespond, reply, &mut [])
            .map(|_| ())
    }
    /// Encode and send a Trigger/SetBool reply for an ID decoded from this queue.
    pub fn respond_bool(
        &self,
        id: codec::RequestId,
        success: bool,
        message: &str,
        scratch: &mut [u8],
    ) -> Result<(), Error> {
        let n =
            codec::encode_service_reply(id, &codec::ServiceResponse { success, message }, scratch)?;
        self.respond(&scratch[..n])
    }
}
/// One explicit Action operation capability. Ownership and pending-goal quotas
/// remain authoritative at the Gateway; GoalIds are never fabricated here.
pub struct ActionClient(Capability);
impl ActionClient {
    /// Send a Fibonacci goal and decode its Gateway-issued ID.
    pub fn send_fibonacci_goal(&self, order: i32) -> Result<codec::GoalId, Error> {
        let mut input = [0; 4];
        codec::encode_fibonacci_goal(order, &mut input)?;
        let mut output = [0; 8];
        let n = self
            .0
            .request(Operation::ActionSendGoal, &input, &mut output)?;
        codec::decode_goal_id(&output[..n])
    }
    /// Request own-goal cancellation. This needs its own cancel-own grant.
    pub fn cancel_own(&self, goal: codec::GoalId) -> Result<bool, Error> {
        let mut response = [0; 1];
        let n = self
            .0
            .request(Operation::ActionCancelOwnGoal, &goal.bytes(), &mut response)?;
        codec::decode_bool(&response[..n])
    }
    /// Cancel a known goal using independently granted cancel-any authority.
    pub fn cancel_any(&self, goal: codec::GoalId) -> Result<bool, Error> {
        let mut response = [0; 1];
        let n = self
            .0
            .request(Operation::ActionCancelAnyGoal, &goal.bytes(), &mut response)?;
        codec::decode_bool(&response[..n])
    }
    /// Request cancellation of all eligible goals using the separately granted
    /// cancel-any capability. Only this method encodes goal zero; it does not
    /// fabricate a GoalId or open this stronger authority automatically.
    pub fn cancel_all(&self) -> Result<bool, Error> {
        let mut response = [0; 1];
        let n = self
            .0
            .request(Operation::ActionCancelAnyGoal, &[0u8; 8], &mut response)?;
        codec::decode_bool(&response[..n])
    }
    /// Receive one Fibonacci feedback envelope for an owned ID.
    pub fn feedback<'a>(
        &self,
        goal: codec::GoalId,
        out: &'a mut [u8],
    ) -> Result<codec::FibonacciSequence<'a>, Error> {
        let n = self
            .0
            .request(Operation::ActionReceiveFeedback, &goal.bytes(), out)?;
        let value = codec::decode_fibonacci_feedback(&out[..n])?;
        if value.goal.raw() != goal.raw() { return Err(Error::new(codes::INVALID)); }
        Ok(value)
    }
    /// Receive one terminal Fibonacci result for an owned ID.
    pub fn result<'a>(
        &self,
        goal: codec::GoalId,
        out: &'a mut [u8],
    ) -> Result<codec::FibonacciSequence<'a>, Error> {
        let n = self
            .0
            .request(Operation::ActionReceiveResult, &goal.bytes(), out)?;
        let value = codec::decode_fibonacci_result(&out[..n])?;
        if value.goal.raw() != goal.raw() { return Err(Error::new(codes::INVALID)); }
        Ok(value)
    }
}
/// Action server with independently granted goal-event and response authority.
pub struct ActionServer {
    events: Capability,
    respond: Capability,
    spinning: core::cell::Cell<bool>,
}
impl ActionServer {
    /// Poll and decode a Gateway-issued Fibonacci goal event.
    pub fn poll_goal(&self) -> Result<(codec::GoalId, i32), Error> {
        crate::callback::idle(&self.spinning)?;
        self.poll_goal_unchecked()
    }
    fn poll_goal_unchecked(&self) -> Result<(codec::GoalId, i32), Error> {
        let mut bytes = [0; 12];
        let n = self.events.poll(&mut bytes)?;
        codec::decode_fibonacci_goal_event(&bytes[..n])
    }
    /// Respond with feedback1, success2, abort3 or cancellation4. The native
    /// state machine independently refuses cancellation4 for a noncancelling goal.
    pub fn respond_fibonacci(
        &self,
        id: codec::GoalId,
        kind: u8,
        values: &[i32],
        scratch: &mut [u8],
    ) -> Result<(), Error> {
        let n = codec::encode_fibonacci_reply(id, kind, values, scratch)?;
        self.respond
            .request(Operation::ActionRespond, &scratch[..n], &mut [])
            .map(|_| ())
    }
}
/// Parameter methods over one operation-specific capability. Matching canonical
/// scalar/array adapters are documented in typed.rs and the language guide.
pub struct Parameters(Capability);
/// Bounded canonical String graph snapshots, filtered by the host policy.
pub struct Graph(Capability);
/// Exact authorized clock domain, preserving u64 nanoseconds without floats.
pub struct Clock(Capability);
/// Canonical String log writing under an independently configured level/rate grant.
pub struct Logger(Capability);
/// TF lookup/broadcast using exact configured frame pairs and finite Transform values.
pub struct Tf(Capability);
/// ROS 2 lifecycle methods with transitions fixed in the explicit host grant.
pub struct Lifecycle(Capability);
macro_rules! resource_requests {
    ($ty:ident, $( $name:ident => $op:ident ),* $(,)?) => {
        impl $ty {
            $( #[doc = concat!("Call `", stringify!($name), "` with canonical bytes and explicit output capacity. The alias must grant this exact operation.")]
            pub fn $name(&self, input: &[u8], output: &mut [u8]) -> Result<usize, Error> { self.0.request(Operation::$op, input, output) } )*
        }
    };
}
resource_requests!(Parameters, get => ParameterGet, set => ParameterSet, delete => ParameterDelete, exists => ParameterExists, list => ParameterList, search => ParameterSearch, describe => ParameterDescribe, declare => ParameterDeclare, undeclare => ParameterUndeclare);
resource_requests!(Graph, inspect_topic => GraphInspectTopic, inspect_node => GraphInspectNode, inspect_service => GraphInspectService, inspect_action => GraphInspectAction, list_topics => GraphListTopics, list_nodes => GraphListNodes, list_services => GraphListServices);
resource_requests!(Tf, lookup => TfLookup, broadcast => TfBroadcast);
resource_requests!(Lifecycle, get_state => LifecycleGetState, change_state => LifecycleChangeState);
impl Parameters {
    /// Subscribe to independently granted ROS 2 parameter events and retain this
    /// handle as a pollable queue. Scalar get/set grants do not satisfy this call.
    pub fn subscribe_events(self) -> Result<Subscription, Error> {
        self.0.subscribe_parameter_events()?;
        Ok(Subscription(self.0))
    }
    /// Read and decode a canonical String parameter without allocating.
    pub fn get_string<'a>(&self, out: &'a mut [u8]) -> Result<&'a str, Error> {
        let n = self.get(&[], out)?;
        codec::decode_string(&out[..n])
    }
    /// Encode and write a String parameter using caller-bounded scratch.
    pub fn set_string(&self, value: &str, scratch: &mut [u8]) -> Result<(), Error> {
        let n = codec::encode_string(value, scratch)?;
        self.set(&scratch[..n], &mut []).map(|_| ())
    }
    /// Read one canonical Bool parameter with no String or numeric coercion.
    pub fn get_bool(&self) -> Result<bool, Error> {
        let mut b = [0; 1];
        let n = self.get(&[], &mut b)?;
        codec::decode_bool(&b[..n])
    }
    /// Write one canonical Bool parameter using an independently granted set alias.
    pub fn set_bool(&self, value: bool) -> Result<(), Error> {
        self.set(&[u8::from(value)], &mut []).map(|_| ())
    }
}
impl Graph {
    /// Read a graph snapshot selected by op, accepting only graph operations.
    pub fn snapshot<'a>(&self, op: Operation, out: &'a mut [u8]) -> Result<&'a str, Error> {
        if !matches!(
            op,
            Operation::GraphInspectTopic
                | Operation::GraphInspectNode
                | Operation::GraphInspectService
                | Operation::GraphInspectAction
                | Operation::GraphListTopics
                | Operation::GraphListNodes
                | Operation::GraphListServices
        ) {
            return Err(Error::new(codes::INVALID));
        }
        let n = self.0.request(op, &[], out)?;
        codec::decode_string(&out[..n])
    }
}
impl Clock {
    /// Read exact nanoseconds in one authorized clock domain.
    pub fn read(&self, op: Operation) -> Result<u64, Error> {
        if !matches!(
            op,
            Operation::TimeReadRos
                | Operation::TimeReadWall
                | Operation::TimeReadSystem
                | Operation::TimeReadSteady
        ) {
            return Err(Error::new(codes::INVALID));
        }
        let mut bytes = [0; 8];
        let n = self.0.request(op, &[], &mut bytes)?;
        codec::decode_time(&bytes[..n])
    }
}
impl Logger {
    /// Encode and write one UTF-8 log message; the configured log level is immutable.
    pub fn write(&self, text: &str, scratch: &mut [u8]) -> Result<(), Error> {
        let n = codec::encode_string(text, scratch)?;
        self.0.log(&scratch[..n])
    }
}

/// Single-step polling without allocation. It never blocks waiting for a new
/// event, invokes no foreign callback, and executes callback only after success.
pub fn spin_once(
    subscription: &Subscription,
    scratch: &mut [u8],
    callback: impl FnOnce(&[u8]),
) -> Result<bool, Error> {
    match subscription.poll(scratch) {
        Ok(n) => {
            callback(&scratch[..n]);
            Ok(true)
        }
        Err(error) if error.code == codes::AGAIN => Ok(false),
        Err(error) => Err(error),
    }
}

#[cfg(feature = "alloc")]
use alloc::{boxed::Box, vec::Vec};
#[cfg(feature = "alloc")]
struct Entry {
    subscription: Subscription,
    callback: Box<dyn FnMut(&[u8])>,
}
/// Registering polling executor, enabled with the alloc feature. It polls each
/// queue at most once per spin, bounds its scratch capacity, and dispatches on
/// the guest execution thread. There is no automatic idle loop or retry on90.
#[cfg(feature = "alloc")]
pub struct Executor {
    entries: Vec<Entry>,
    scratch: Vec<u8>,
    spinning: core::cell::Cell<bool>,
    closed: bool,
}
#[cfg(feature = "alloc")]
impl Executor {
    /// Allocate one fixed scratch buffer chosen by the application. This is a
    /// guest capacity choice; it does not increase the host's configured quotas.
    pub fn new(capacity: usize) -> Self {
        Self {
            entries: Vec::new(),
            scratch: alloc::vec![0; capacity],
            spinning: core::cell::Cell::new(false),
            closed: false,
        }
    }
    /// Check the application's scratch capacity before allocating. This bound
    /// is SDK-local and does not increase any native queue or byte quota.
    pub fn try_new(capacity: usize) -> Result<Self, Error> {
        if capacity > crate::typed::MAX_BYTES { return Err(Error::new(codes::TOO_LARGE)); }
        Ok(Self::new(capacity))
    }
    /// Remove a registration and return its unique owned subscription. The
    /// caller can inspect explicit close; removal itself makes no native call.
    pub fn remove(&mut self, index: usize) -> Result<Subscription, Error> {
        if self.closed { return Err(Error::new(codes::BAD_HANDLE)); }
        crate::callback::idle(&self.spinning)?;
        if index >= self.entries.len() { return Err(Error::new(codes::INVALID)); }
        Ok(self.entries.remove(index).subscription)
    }
    /// Close every registered queue once, attempting all and preserving the
    /// first error. Entries are then cleared; later spins return BadHandle9.
    pub fn close(&mut self) -> Result<(), Error> {
        if self.closed { return Err(Error::new(codes::BAD_HANDLE)); }
        crate::callback::idle(&self.spinning)?;
        self.closed = true;
        let mut first = None;
        for entry in &mut self.entries {
            if entry.subscription.is_open() {
                if let Err(error) = entry.subscription.close() { if first.is_none() { first = Some(error); } }
            }
        }
        self.entries.clear(); match first { Some(error) => Err(error), None => Ok(()) }
    }
    /// Register a typed copied event decoder. Decode failures are sent to the
    /// handler as Err and never become a partially initialized value. Use the
    /// polling status from spin_once to distinguish idle native queues.
    pub fn add_typed<T: crate::typed::Canonical + 'static>(
        &mut self, subscription: Subscription, mut callback: impl FnMut(Result<T, Error>) + 'static,
    ) -> Result<(), Error> {
        self.add_subscription(subscription, move |bytes| callback(T::decode(bytes)))
    }
    /// Transfer one subscription into the executor with its guest-thread callback.
    /// Closed/busy registration returns9/17 before mutation. A rejected moved
    /// subscription is dropped once; preserve the returned initialization error.
    pub fn add_subscription(
        &mut self,
        subscription: Subscription,
        callback: impl FnMut(&[u8]) + 'static,
    ) -> Result<(), Error> {
        if self.closed { return Err(Error::new(codes::BAD_HANDLE)); }
        crate::callback::idle(&self.spinning)?;
        self.entries.push(Entry {
            subscription,
            callback: Box::new(callback),
        });
        Ok(())
    }
    /// Poll every registered queue once; return the delivered callback count.
    /// Non-AGAIN errors are reported immediately without automatic retries.
    pub fn spin_once(&mut self) -> Result<usize, Error> {
        if self.closed { return Err(Error::new(codes::BAD_HANDLE)); }
        if self.spinning.replace(true) { return Err(Error::new(codes::CONFLICT)); }
        struct Reset<'a>(&'a core::cell::Cell<bool>);
        impl Drop for Reset<'_> { fn drop(&mut self) { self.0.set(false); } }
        let _reset = Reset(&self.spinning);
        let mut count = 0;
        for entry in &mut self.entries {
            match entry.subscription.poll(&mut self.scratch) {
                Ok(n) => {
                    (entry.callback)(&self.scratch[..n]);
                    count += 1;
                }
                Err(error) if error.code == codes::AGAIN => {}
                Err(error) => return Err(error),
            }
        }
        Ok(count)
    }
}

#[cfg(feature = "alloc")]
impl Publisher {
    /// Encode and publish one owned builtin Canonical value, preserving the
    /// exact selected adapter. This allocation does not enlarge a host quota.
    pub fn publish_value<T: crate::typed::Canonical>(&self, value: &T) -> Result<(), Error> {
        self.publish(&value.encode()?)
    }
}
#[cfg(feature = "alloc")]
impl Subscription {
    /// Poll and decode an owned builtin value using an application-chosen buffer.
    /// Failed decoding returns an error rather than delivering a partial value.
    pub fn poll_value<T: crate::typed::Canonical>(&self, out: &mut [u8]) -> Result<T, Error> {
        let n = self.poll(out)?;
        T::decode(&out[..n])
    }
}
#[cfg(feature = "alloc")]
impl Parameters {
    /// Read a scalar/array with its exact configured adapter; i32 and i64 never
    /// coerce each other, and event grants remain independent.
    pub fn get_value(
        &self,
        kind: crate::typed::ParameterKind,
        out: &mut [u8],
    ) -> Result<crate::typed::ParameterValue, Error> {
        let n = self.get(&[], out)?;
        crate::typed::ParameterValue::decode(kind, &out[..n])
    }
    /// Encode and set one selected scalar/array value under a matching grant.
    pub fn set_value(&self, value: &crate::typed::ParameterValue) -> Result<(), Error> {
        self.set(&value.encode()?, &mut []).map(|_| ())
    }
    /// Declare one selected value on an explicitly named ROS2 Gateway node.
    /// The current adapter does not support remote native declaration.
    pub fn declare_value(&self, value: &crate::typed::ParameterValue) -> Result<(), Error> {
        self.declare(&value.encode()?, &mut []).map(|_| ())
    }
}
#[cfg(feature = "alloc")]
impl Tf {
    /// Read the exact authorized Transform at u64 nanoseconds (zero=latest).
    /// /tf_static and dynamic history semantics remain host-controlled.
    pub fn lookup_transform(
        &self,
        time: u64,
        out: &mut [u8],
    ) -> Result<crate::typed::Transform, Error> {
        let n = self.lookup(&time.to_le_bytes(), out)?;
        <crate::typed::Transform as crate::typed::Canonical>::decode(&out[..n])
    }
    /// Broadcast one canonical finite Transform, with independently granted TF
    /// publication authority and exact configured frame pair.
    pub fn broadcast_transform(&self, value: &crate::typed::Transform) -> Result<(), Error> {
        let bytes = <crate::typed::Transform as crate::typed::Canonical>::encode(value)?;
        self.broadcast(&bytes, &mut []).map(|_| ())
    }
}
#[cfg(feature = "alloc")]
impl Lifecycle {
    /// Read the native lifecycle state under a separate get-state capability.
    pub fn state(&self, out: &mut [u8]) -> Result<crate::typed::LifecycleState, Error> {
        let n = self.get_state(&[], out)?;
        <crate::typed::LifecycleState as crate::typed::Canonical>::decode(&out[..n])
    }
    /// Request the single transition configured in this capability; the guest
    /// provides an empty request and cannot choose a different transition.
    pub fn change(&self) -> Result<bool, Error> {
        let mut out = [0; 1];
        let n = self.change_state(&[], &mut out)?;
        codec::decode_bool(&out[..n])
    }
}

// Every unique typed owner exposes checked lifetime operations. Group bookkeeping
// in an optional Session is shared internally, while the actual opaque capability
// is neither cloned nor fabricated. Default Drop remains a fallback only.
macro_rules! checked_owner {
    ($($ty:ident),* $(,)?) => { $(
        impl $ty {
            /// Retire this resource and return the original close outcome once.
            /// A backend error may follow Runtime retirement; never retry it.
            pub fn close(&mut self) -> Result<(), Error> { self.0.close() }
            /// Revoke this exact alias and retire this wrapper before notification.
            /// Reopening is not recovery; sibling handles remain host-controlled.
            pub fn revoke(&mut self) -> Result<(), Error> { self.0.revoke() }
            /// Borrow the complete opaque u64 for one reviewed raw operation.
            /// Never alter its bits or transfer it across Instances/backends.
            pub fn raw_handle(&self) -> Result<u64, Error> { self.0.raw_handle() }
            /// Observe local ownership without a native query or implicit reopen.
            pub fn is_open(&self) -> bool { self.0.is_open() }
        }
    )* };
}
checked_owner!(Publisher, Subscription, Client, ActionClient, Parameters, Graph, Clock, Logger, Tf, Lifecycle);
macro_rules! checked_server_owner {
    ($($ty:ident),* $(,)?) => { $(
        impl $ty {
            /// Close both independent aliases even when the first close fails.
            /// Return the first failure after attempting both; no replay occurs.
            pub fn close(&mut self) -> Result<(), Error> {
                crate::callback::idle(&self.spinning)?;
                let first = self.events.close(); let second = self.respond.close(); first.and(second)
            }
            /// Revoke both explicitly owned aliases once, attempting both and
            /// preserving the first error. This grants no stronger operations.
            pub fn revoke(&mut self) -> Result<(), Error> {
                crate::callback::idle(&self.spinning)?;
                let first = self.events.revoke(); let second = self.respond.revoke(); first.and(second)
            }
            /// Borrow the advertiser/event and responder IDs in that order.
            /// These remain different operation grants and cannot be swapped.
            pub fn raw_handles(&self) -> Result<(u64, u64), Error> {
                Ok((self.events.raw_handle()?, self.respond.raw_handle()?))
            }
        }
    )* };
}
checked_server_owner!(Server, ActionServer);

impl Server {
    /// Poll one complete Trigger/SetBool request and invoke a guest-local handler.
    /// The handler sees the Gateway-issued request ID and canonical request bytes;
    /// decode the configured request shape before returning a response. Empty
    /// queue returns Ok(false), other errors retain their exact code/length.
    /// Request scratch and reply scratch must be disjoint caller-owned buffers.
    pub fn spin_once(
        &self, input: &mut [u8], output: &mut [u8],
        handler: impl FnOnce(codec::RequestId, &[u8], &mut [u8]) -> Result<usize, Error>,
    ) -> Result<bool, Error> {
        let _events=self.events.callback_guard()?; let _respond=self.respond.callback_guard()?;
        crate::callback::service_once(&self.spinning,input,output,|buffer|self.events.poll(buffer),|wire|self.respond(wire),handler)
    }
}
impl ActionServer {
    /// Dispatch one validated incoming Fibonacci goal to guest application code.
    /// The handler chooses feedback/terminal kind and values; every response is
    /// still bound to the owned incoming ID and exact respond capability. No
    /// native callback or automatic timeout retry is registered.
    pub fn spin_goal_once(
        &self, handler: impl FnOnce(codec::GoalId, i32) -> Result<(), Error>,
    ) -> Result<bool, Error> {
        let _events=self.events.callback_guard()?; let _respond=self.respond.callback_guard()?;
        crate::callback::goal_once(&self.spinning,||self.poll_goal_unchecked(),handler)
    }
}
