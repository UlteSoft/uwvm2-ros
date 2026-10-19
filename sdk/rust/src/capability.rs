//! Owned, synchronous memory-zero calls; use raw imports for other memories.
use crate::{codes, raw};

/// A host error plus an optional *observed* output length on TOO_LARGE.
/// The sentinel distinguishes a written slot from a refusal before any write.
/// No automatic retry is attempted: the event or operation may already be consumed.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct Error {
    /// Original unsigned ABI code, including unknown future values.
    pub code: u32,
    /// Length written to the slot on error 90, if the host actually wrote it.
    pub required: Option<u64>,
}
impl Error {
    /// Construct an error with no asserted output length.
    pub const fn new(code: u32) -> Self {
        Self {
            code,
            required: None,
        }
    }
}

/// A selected import backend. Enabling only one feature excludes the other's declarations.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum Backend {
    /// ROS 1; its handles cannot be used in ROS 2.
    #[cfg(feature = "ros1")]
    Ros1,
    /// ROS 2; its handles cannot be used in ROS 1.
    #[cfg(feature = "ros2")]
    Ros2,
}

/// The complete request/response operation set; backend-specific cases return 95.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum Operation {
    /// Request/response form of `service_call`; use a separately granted alias.
    ServiceCall,
    /// Request/response form of `service_respond`; use a separately granted alias.
    ServiceRespond,
    /// Request/response form of `action_send_goal`; use a separately granted alias.
    ActionSendGoal,
    /// Request/response form of `action_cancel_own_goal`; use a separately granted alias.
    ActionCancelOwnGoal,
    /// Request/response form of `action_cancel_any_goal`; use a separately granted alias.
    ActionCancelAnyGoal,
    /// Request/response form of `action_receive_feedback`; use a separately granted alias.
    ActionReceiveFeedback,
    /// Request/response form of `action_receive_result`; use a separately granted alias.
    ActionReceiveResult,
    /// Request/response form of `action_advertise`; use a separately granted alias.
    ActionAdvertise,
    /// Request/response form of `action_respond`; use a separately granted alias.
    ActionRespond,
    /// Request/response form of `tf_lookup`; use a separately granted alias.
    TfLookup,
    /// Request/response form of `tf_broadcast`; use a separately granted alias.
    TfBroadcast,
    /// Request/response form of `parameter_get`; use a separately granted alias.
    ParameterGet,
    /// Request/response form of `parameter_set`; use a separately granted alias.
    ParameterSet,
    /// Request/response form of `parameter_delete`; use a separately granted alias.
    ParameterDelete,
    /// Request/response form of `parameter_exists`; use a separately granted alias.
    ParameterExists,
    /// Request/response form of `parameter_list`; use a separately granted alias.
    ParameterList,
    /// Request/response form of `parameter_search`; use a separately granted alias.
    ParameterSearch,
    /// Request/response form of `graph_inspect_topic`; use a separately granted alias.
    GraphInspectTopic,
    /// Request/response form of `graph_inspect_node`; use a separately granted alias.
    GraphInspectNode,
    /// Request/response form of `graph_inspect_service`; use a separately granted alias.
    GraphInspectService,
    /// Request/response form of `graph_list_topics`; use a separately granted alias.
    GraphListTopics,
    /// Request/response form of `graph_list_nodes`; use a separately granted alias.
    GraphListNodes,
    /// Request/response form of `graph_list_services`; use a separately granted alias.
    GraphListServices,
    /// Request/response form of `time_read_ros`; use a separately granted alias.
    TimeReadRos,
    /// Request/response form of `time_read_wall`; use a separately granted alias.
    TimeReadWall,
    /// Request/response form of `parameter_describe`; use a separately granted alias.
    ParameterDescribe,
    /// Request/response form of `parameter_declare`; use a separately granted alias.
    ParameterDeclare,
    /// Request/response form of `parameter_undeclare`; use a separately granted alias.
    ParameterUndeclare,
    /// Request/response form of `graph_inspect_action`; use a separately granted alias.
    GraphInspectAction,
    /// Request/response form of `lifecycle_get_state`; use a separately granted alias.
    LifecycleGetState,
    /// Request/response form of `lifecycle_change_state`; use a separately granted alias.
    LifecycleChangeState,
    /// Request/response form of `time_read_system`; use a separately granted alias.
    TimeReadSystem,
    /// Request/response form of `time_read_steady`; use a separately granted alias.
    TimeReadSteady,
}

macro_rules! backend_call {
    ($backend:expr, $name:ident $(, $arg:expr)*) => {
        match $backend {
            #[cfg(feature = "ros1")]
            Backend::Ros1 => unsafe { raw::ros1::$name($($arg),*) },
            #[cfg(feature = "ros2")]
            Backend::Ros2 => unsafe { raw::ros2::$name($($arg),*) },
        }
    };
}
impl Backend {
    /// Read ABI version without opening a capability or adding authority.
    /// This is the original import result; unsupported pointer targets return95
    /// before a foreign import is attempted by the memory-zero convenience API.
    pub fn abi_version(self) -> u32 {
        if !cfg!(all(target_arch = "wasm32", target_pointer_width = "32")) {
            return codes::UNSUPPORTED;
        }
        backend_call!(self, abi_version)
    }
    #[cfg(feature = "alloc")]
    pub(crate) fn close_handle(self, handle: u64) -> Result<(), Error> {
        status(backend_call!(self, capability_close, handle))
    }
}

fn offset<T>(value: *const T) -> Result<u64, Error> {
    if !cfg!(all(target_arch = "wasm32", target_pointer_width = "32")) {
        return Err(Error::new(codes::UNSUPPORTED));
    }
    Ok(value as usize as u64)
}
fn status(code: u32) -> Result<(), Error> {
    if code == 0 {
        Ok(())
    } else {
        Err(Error::new(code))
    }
}
fn invoke(backend: Backend, op: Operation, h: u64, a: u64, n: u64, b: u64, c: u64, z: u64) -> u32 {
    match backend {
        #[cfg(feature = "ros1")]
        Backend::Ros1 => match op {
            Operation::ServiceCall => unsafe { raw::ros1::service_call(h, 0, a, n, b, c, z) },
            Operation::ServiceRespond => unsafe { raw::ros1::service_respond(h, 0, a, n, b, c, z) },
            Operation::ActionSendGoal => unsafe {
                raw::ros1::action_send_goal(h, 0, a, n, b, c, z)
            },
            Operation::ActionCancelOwnGoal => unsafe {
                raw::ros1::action_cancel_own_goal(h, 0, a, n, b, c, z)
            },
            Operation::ActionCancelAnyGoal => unsafe {
                raw::ros1::action_cancel_any_goal(h, 0, a, n, b, c, z)
            },
            Operation::ActionReceiveFeedback => unsafe {
                raw::ros1::action_receive_feedback(h, 0, a, n, b, c, z)
            },
            Operation::ActionReceiveResult => unsafe {
                raw::ros1::action_receive_result(h, 0, a, n, b, c, z)
            },
            Operation::ActionAdvertise => unsafe {
                raw::ros1::action_advertise(h, 0, a, n, b, c, z)
            },
            Operation::ActionRespond => unsafe { raw::ros1::action_respond(h, 0, a, n, b, c, z) },
            Operation::TfLookup => unsafe { raw::ros1::tf_lookup(h, 0, a, n, b, c, z) },
            Operation::TfBroadcast => unsafe { raw::ros1::tf_broadcast(h, 0, a, n, b, c, z) },
            Operation::ParameterGet => unsafe { raw::ros1::parameter_get(h, 0, a, n, b, c, z) },
            Operation::ParameterSet => unsafe { raw::ros1::parameter_set(h, 0, a, n, b, c, z) },
            Operation::ParameterDelete => unsafe {
                raw::ros1::parameter_delete(h, 0, a, n, b, c, z)
            },
            Operation::ParameterExists => unsafe {
                raw::ros1::parameter_exists(h, 0, a, n, b, c, z)
            },
            Operation::ParameterList => unsafe { raw::ros1::parameter_list(h, 0, a, n, b, c, z) },
            Operation::ParameterSearch => unsafe {
                raw::ros1::parameter_search(h, 0, a, n, b, c, z)
            },
            Operation::GraphInspectTopic => unsafe {
                raw::ros1::graph_inspect_topic(h, 0, a, n, b, c, z)
            },
            Operation::GraphInspectNode => unsafe {
                raw::ros1::graph_inspect_node(h, 0, a, n, b, c, z)
            },
            Operation::GraphInspectService => unsafe {
                raw::ros1::graph_inspect_service(h, 0, a, n, b, c, z)
            },
            Operation::GraphListTopics => unsafe {
                raw::ros1::graph_list_topics(h, 0, a, n, b, c, z)
            },
            Operation::GraphListNodes => unsafe {
                raw::ros1::graph_list_nodes(h, 0, a, n, b, c, z)
            },
            Operation::GraphListServices => unsafe {
                raw::ros1::graph_list_services(h, 0, a, n, b, c, z)
            },
            Operation::TimeReadRos => unsafe { raw::ros1::time_read_ros(h, 0, a, n, b, c, z) },
            Operation::TimeReadWall => unsafe { raw::ros1::time_read_wall(h, 0, a, n, b, c, z) },
            #[allow(unreachable_patterns)]
            _ => codes::UNSUPPORTED,
        },
        #[cfg(feature = "ros2")]
        Backend::Ros2 => match op {
            Operation::ServiceCall => unsafe { raw::ros2::service_call(h, 0, a, n, b, c, z) },
            Operation::ServiceRespond => unsafe { raw::ros2::service_respond(h, 0, a, n, b, c, z) },
            Operation::ActionSendGoal => unsafe {
                raw::ros2::action_send_goal(h, 0, a, n, b, c, z)
            },
            Operation::ActionCancelOwnGoal => unsafe {
                raw::ros2::action_cancel_own_goal(h, 0, a, n, b, c, z)
            },
            Operation::ActionCancelAnyGoal => unsafe {
                raw::ros2::action_cancel_any_goal(h, 0, a, n, b, c, z)
            },
            Operation::ActionReceiveFeedback => unsafe {
                raw::ros2::action_receive_feedback(h, 0, a, n, b, c, z)
            },
            Operation::ActionReceiveResult => unsafe {
                raw::ros2::action_receive_result(h, 0, a, n, b, c, z)
            },
            Operation::ActionAdvertise => unsafe {
                raw::ros2::action_advertise(h, 0, a, n, b, c, z)
            },
            Operation::ActionRespond => unsafe { raw::ros2::action_respond(h, 0, a, n, b, c, z) },
            Operation::TfLookup => unsafe { raw::ros2::tf_lookup(h, 0, a, n, b, c, z) },
            Operation::TfBroadcast => unsafe { raw::ros2::tf_broadcast(h, 0, a, n, b, c, z) },
            Operation::ParameterGet => unsafe { raw::ros2::parameter_get(h, 0, a, n, b, c, z) },
            Operation::ParameterSet => unsafe { raw::ros2::parameter_set(h, 0, a, n, b, c, z) },
            Operation::ParameterList => unsafe { raw::ros2::parameter_list(h, 0, a, n, b, c, z) },
            Operation::ParameterDescribe => unsafe {
                raw::ros2::parameter_describe(h, 0, a, n, b, c, z)
            },
            Operation::ParameterDeclare => unsafe {
                raw::ros2::parameter_declare(h, 0, a, n, b, c, z)
            },
            Operation::ParameterUndeclare => unsafe {
                raw::ros2::parameter_undeclare(h, 0, a, n, b, c, z)
            },
            Operation::GraphInspectTopic => unsafe {
                raw::ros2::graph_inspect_topic(h, 0, a, n, b, c, z)
            },
            Operation::GraphInspectNode => unsafe {
                raw::ros2::graph_inspect_node(h, 0, a, n, b, c, z)
            },
            Operation::GraphInspectService => unsafe {
                raw::ros2::graph_inspect_service(h, 0, a, n, b, c, z)
            },
            Operation::GraphInspectAction => unsafe {
                raw::ros2::graph_inspect_action(h, 0, a, n, b, c, z)
            },
            Operation::GraphListTopics => unsafe {
                raw::ros2::graph_list_topics(h, 0, a, n, b, c, z)
            },
            Operation::GraphListNodes => unsafe {
                raw::ros2::graph_list_nodes(h, 0, a, n, b, c, z)
            },
            Operation::GraphListServices => unsafe {
                raw::ros2::graph_list_services(h, 0, a, n, b, c, z)
            },
            Operation::LifecycleGetState => unsafe {
                raw::ros2::lifecycle_get_state(h, 0, a, n, b, c, z)
            },
            Operation::LifecycleChangeState => unsafe {
                raw::ros2::lifecycle_change_state(h, 0, a, n, b, c, z)
            },
            Operation::TimeReadRos => unsafe { raw::ros2::time_read_ros(h, 0, a, n, b, c, z) },
            Operation::TimeReadSystem => unsafe {
                raw::ros2::time_read_system(h, 0, a, n, b, c, z)
            },
            Operation::TimeReadSteady => unsafe {
                raw::ros2::time_read_steady(h, 0, a, n, b, c, z)
            },
            #[allow(unreachable_patterns)]
            _ => codes::UNSUPPORTED,
        },
    }
}

/// A unique owned opaque handle. It is never cloned, synthesized or shared
/// between instances/backends. Drop attempts close; inspect explicit close when
/// its error matters. Neither successful open nor a Node grants additional rights.
pub struct Capability {
    backend: Backend,
    handle: Option<u64>,
    callback_busy: core::cell::Cell<bool>,
    // Optional alloc Session bookkeeping. This is not a cloneable public
    // capability; only the unique wrapper and its owner may retire this state.
    #[cfg(feature = "alloc")]
    group: Option<alloc::rc::Rc<crate::session::HandleState>>,
}
impl Capability {
    /// Open one ASCII capability alias using memory zero and an eight-byte LE slot.
    /// Aliases are configured by the host, not ROS endpoint names.
    pub fn open(backend: Backend, alias: &str) -> Result<Self, Error> {
        if alias.is_empty() || alias.len() > 255 || alias.as_bytes().contains(&0) {
            return Err(Error::new(codes::INVALID));
        }
        let mut encoded = [0u8; 8];
        let a = offset(alias.as_ptr())?;
        let z = offset(encoded.as_mut_ptr())?;
        status(backend_call!(
            backend,
            capability_open,
            0,
            a,
            alias.len() as u64,
            z
        ))?;
        let handle = u64::from_le_bytes(encoded);
        if handle == 0 {
            return Err(Error::new(codes::INVALID));
        }
        Ok(Self {
            backend,
            handle: Some(handle),
            callback_busy: core::cell::Cell::new(false),
            #[cfg(feature = "alloc")]
            group: None,
        })
    }
    /// Return the backend tied to this handle.
    pub fn backend(&self) -> Backend {
        self.backend
    }
    /// Borrow the opaque handle for an explicit raw call. Never edit its bits or transfer it.
    pub fn raw_handle(&self) -> Result<u64, Error> {
        #[cfg(feature = "alloc")]
        if let Some(state) = &self.group {
            return state.handle.get().ok_or(Error::new(codes::BAD_HANDLE));
        }
        self.handle.ok_or(Error::new(codes::BAD_HANDLE))
    }
    /// Whether this wrapper still owns a live handle, including Session closure.
    /// This is local bookkeeping and performs no native RPC or implicit reopen.
    pub fn is_open(&self) -> bool {
        self.raw_handle().is_ok()
    }
    #[cfg(feature = "alloc")]
    pub(crate) fn attach(&mut self, state: alloc::rc::Rc<crate::session::HandleState>) {
        self.group = Some(state);
    }
    #[cfg(feature = "alloc")]
    pub(crate) fn shared_state(&self) -> Option<alloc::rc::Rc<crate::session::HandleState>> { self.group.as_ref().map(alloc::rc::Rc::clone) }
    fn owner_busy(&self) -> &core::cell::Cell<bool> {
        #[cfg(feature = "alloc")]
        if let Some(state)=&self.group { return &state.callback_busy; }
        &self.callback_busy
    }
    pub(crate) fn callback_guard(&self) -> Result<crate::callback::OwnerGuard<'_>,Error> {
        self.raw_handle()?; crate::callback::guard_owner(self.owner_busy())
    }
    fn retire(&mut self) -> Result<u64, Error> {
        self.raw_handle()?;
        crate::callback::idle(self.owner_busy())?;
        let local = self.handle.take().ok_or(Error::new(codes::BAD_HANDLE))?;
        #[cfg(feature = "alloc")]
        if let Some(state) = &self.group {
            return state.handle.take().ok_or(Error::new(codes::BAD_HANDLE));
        }
        Ok(local)
    }
    /// Retire ownership before closing. A transport failure can follow retirement;
    /// the SDK will not reuse or automatically retry this handle. An active
    /// server callback returns Conflict17 before retirement or any close import.
    pub fn close(&mut self) -> Result<(), Error> {
        let h = self.retire()?;
        status(backend_call!(self.backend, capability_close, h))
    }
    /// Revoke the alias and retire this wrapper. A failed transport notification
    /// can follow local host retirement; other handles must observe host errors.
    pub fn revoke(&mut self) -> Result<(), Error> {
        let h = self.retire()?;
        status(backend_call!(self.backend, capability_revoke, h))
    }
    /// Publish canonical bytes. The exact topic/type/QoS/quotas come from the grant.
    pub fn publish(&self, payload: &[u8]) -> Result<(), Error> {
        let h = self.raw_handle()?;
        let a = offset(payload.as_ptr())?;
        status(backend_call!(
            self.backend,
            topic_publish,
            h,
            0,
            a,
            payload.len() as u64
        ))
    }
    /// Activate an explicitly authorized topic queue; event polling is separate.
    pub fn subscribe(&self) -> Result<(), Error> {
        status(backend_call!(
            self.backend,
            topic_subscribe,
            self.raw_handle()?
        ))
    }
    /// Activate an explicitly authorized service request queue.
    pub fn advertise_service(&self) -> Result<(), Error> {
        status(backend_call!(
            self.backend,
            service_advertise,
            self.raw_handle()?
        ))
    }
    /// Activate an independently granted ROS 2 parameter-event queue.
    /// ROS 1 returns UNSUPPORTED without invoking another backend.
    pub fn subscribe_parameter_events(&self) -> Result<(), Error> {
        match self.backend {
            #[cfg(feature = "ros1")]
            Backend::Ros1 => Err(Error::new(codes::UNSUPPORTED)),
            #[cfg(feature = "ros2")]
            Backend::Ros2 => {
                status(unsafe { raw::ros2::parameter_subscribe_events(self.raw_handle()?) })
            }
        }
    }
    /// Poll one canonical event into a caller-bounded buffer. AGAIN is normal.
    /// On TOO_LARGE the host may already have consumed the queued event.
    pub fn poll(&self, output: &mut [u8]) -> Result<usize, Error> {
        let h = self.raw_handle()?;
        let mut length = u64::MAX.to_le_bytes();
        let b = offset(output.as_mut_ptr())?;
        let z = offset(length.as_mut_ptr())?;
        let code = backend_call!(self.backend, event_poll, h, 0, b, output.len() as u64, z);
        finish(code, length, output.len())
    }
    /// Bind this separately granted observer to a live source once. Correlation
    /// is nonzero only for selected outgoing Action data; metadata-only uses zero.
    /// No default alias is used and a metadata watch activates no native reader.
    pub fn watch_events(&self, source:&Capability, correlation:u64)->Result<(),Error> {
        crate::callback::idle(self.owner_busy())?;crate::callback::idle(source.owner_busy())?;
        if self.backend!=source.backend {return Err(Error::new(codes::INVALID));}
        status(backend_call!(self.backend,event_watch,self.raw_handle()?,source.raw_handle()?,correlation))
    }
    /// Poll a complete versioned event. Unlike legacy poll, code90 retains the
    /// event and reports required bytes only when the length slot was written.
    /// Decode the initialized prefix with `events::decode` before using its body.
    pub fn poll_event_v1(&self, output:&mut [u8])->Result<usize,Error> {
        crate::callback::idle(self.owner_busy())?;
        let h=self.raw_handle()?;let mut slot=u64::MAX.to_le_bytes();
        let b=offset(output.as_mut_ptr())?;let z=offset(slot.as_mut_ptr())?;
        finish(backend_call!(self.backend,event_poll_v1,h,0,b,output.len() as u64,z),slot,output.len())
    }
    /// Return a credit snapshot. Admission can still return11 after readiness;
    /// this method does not reserve capacity or create native resources.
    pub fn event_readiness(&self)->Result<crate::events::Readiness,Error> {
        crate::callback::idle(self.owner_busy())?;let mut bytes=[0u8;32];
        let out=offset(bytes.as_mut_ptr())?;status(backend_call!(self.backend,event_ready,self.raw_handle()?,0,out))?;
        crate::events::decode_readiness(&bytes)
    }
    /// Submit a canonical service request through this source and its separately
    /// granted observer. Success is bounded admission, not native completion.
    /// Poll the observer for response/error with the returned owned ID. A timeout
    /// may follow an external effect; this method never retries automatically.
    pub fn submit_service(&self, observer:&Capability, input:&[u8])->Result<u64,Error> {
        crate::callback::idle(self.owner_busy())?;crate::callback::idle(observer.owner_busy())?;
        if self.backend!=observer.backend {return Err(Error::new(codes::INVALID));}
        let mut slot=[0u8;8];let a=offset(input.as_ptr())?;let z=offset(slot.as_mut_ptr())?;
        status(backend_call!(self.backend,service_submit,self.raw_handle()?,observer.raw_handle()?,0,a,input.len() as u64,z))?;
        let id=u64::from_le_bytes(slot);if id==0 {return Err(Error::new(codes::INVALID));}Ok(id)
    }
    /// Invoke a named RPC with disjoint borrowed input/output slices.
    /// The host validates operation identity, payload and quotas independently.
    /// Success returns initialized output bytes; failure does not grant a retry.
    pub fn request(
        &self,
        op: Operation,
        request: &[u8],
        output: &mut [u8],
    ) -> Result<usize, Error> {
        let h = self.raw_handle()?;
        let mut length = u64::MAX.to_le_bytes();
        let a = offset(request.as_ptr())?;
        let b = offset(output.as_mut_ptr())?;
        let z = offset(length.as_mut_ptr())?;
        let code = invoke(
            self.backend,
            op,
            h,
            a,
            request.len() as u64,
            b,
            output.len() as u64,
            z,
        );
        finish(code, length, output.len())
    }
    /// Write a canonical String log payload using an independently granted log alias.
    pub fn log(&self, request: &[u8]) -> Result<(), Error> {
        let h = self.raw_handle()?;
        let a = offset(request.as_ptr())?;
        status(backend_call!(
            self.backend,
            log_write,
            h,
            0,
            a,
            request.len() as u64
        ))
    }
}
impl Drop for Capability {
    fn drop(&mut self) {
        if self.handle.is_some() {
            let _ = self.close();
        }
    }
}
fn finish(code: u32, encoded: [u8; 8], capacity: usize) -> Result<usize, Error> {
    let n = u64::from_le_bytes(encoded);
    if code != 0 {
        return Err(Error {
            code,
            required: if code == codes::TOO_LARGE && n != u64::MAX {
                Some(n)
            } else {
                None
            },
        });
    }
    if n > capacity as u64 {
        return Err(Error::new(codes::INVALID));
    }
    Ok(n as usize)
}

#[cfg(test)]
mod tests {
    use super::{finish, Error};
    use crate::codes;
    #[test]
    fn output_slot_is_observed_not_assumed() {
        assert_eq!(
            finish(codes::TOO_LARGE, u64::MAX.to_le_bytes(), 16),
            Err(Error {
                code: codes::TOO_LARGE,
                required: None
            })
        );
        assert_eq!(
            finish(codes::TOO_LARGE, 32u64.to_le_bytes(), 16),
            Err(Error {
                code: codes::TOO_LARGE,
                required: Some(32)
            })
        );
        assert_eq!(
            finish(codes::DENIED, 32u64.to_le_bytes(), 16),
            Err(Error {
                code: codes::DENIED,
                required: None
            })
        );
        assert_eq!(finish(0, 16u64.to_le_bytes(), 16), Ok(16));
        assert_eq!(
            finish(0, 17u64.to_le_bytes(), 16),
            Err(Error::new(codes::INVALID))
        );
        assert_eq!(
            finish(0, u64::MAX.to_le_bytes(), 16),
            Err(Error::new(codes::INVALID))
        );
        assert_eq!(
            finish(0xffff_ffff, 0u64.to_le_bytes(), 16),
            Err(Error::new(0xffff_ffff))
        );
    }
}
