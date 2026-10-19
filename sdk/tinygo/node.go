//go:build tinygo && (wasm || tinygo.wasm) && ((ros1 && !ros2) || (ros2 && !ros1))

package roswasm

// Node is a local application label over the selected build-tag backend. It is
// not a native ROS node identity and it grants no implicit network/resources.
type Node struct {
	label string
	session *Session
	closed bool
	states []*capabilityState
}

// NewNode creates a local label. ROS backend selection is an explicit build tag.
func NewNode(label string) *Node { return &Node{label: label} }

func (n *Node) open(alias string) (*Capability, error) {
	if n == nil || n.closed { return nil, Error{Code: BadHandle} }
	if n.session != nil {
		if !n.session.IsOpen() { return nil, Error{Code: BadHandle} }
		n.session.states = liveStates(n.session.states)
		if len(n.session.states) >= n.session.maximum { return nil, Error{Code: TooLarge} }
	}
	n.states = liveStates(n.states)
	// The independent Node also has a bounded local bookkeeping budget. Policy
	// limits remain authoritative and may be considerably lower.
	if len(n.states) >= 1024 { return nil, Error{Code: TooLarge} }
	c, err := Open(alias)
	if err != nil { return nil, err }
	n.states = append(n.states, c.state)
	if n.session != nil { n.session.states = append(n.session.states, c.state) }
	return c, nil
}

// Close disables this Node's factories and attempts all its still-live handles.
// Copies of typed wrappers share retirement state. Session.Close skips states
// already retired here; neither layer issues a second native close.
// Busy server17 preflights all owners and preserves this organizer for later
// cleanup, rather than partially closing idle siblings inside the handler.
func (n *Node) Close() []error {
	if n == nil || n.closed { return []error{Error{Code: BadHandle}} }
	if err:=ownersIdle(n.states); err!=nil { return []error{err} }
	n.closed = true
	results := closeStates(n.states)
	n.states = nil
	return results
}

// Name returns the local diagnostic label, never a native identity assertion.
func (n *Node) Name() string { return n.label }

// Publisher owns one explicit topic-publish capability, whose type/QoS is host-selected.
type Publisher struct{ cap *Capability }

// Subscription owns an activated topic/service/action/parameter event queue.
// All delivery is guest-driven polling; no native callback enters Go.
type Subscription struct{ cap *Capability }

// Client owns one bounded synchronous service-call grant with an explicit host deadline.
type Client struct{ cap *Capability }

// Server owns separate service-advertise and service-respond authorities.
type Server struct { events, respond *Capability; state *callbackState }

// ActionClient owns one exact action operation grant. Send/receive/cancel operations
// need separate aliases; create a client object per granted operation.
type ActionClient struct{ cap *Capability }

// ActionServer owns separately granted action-advertise and action-respond handles.
type ActionServer struct { events, respond *Capability; state *callbackState }

// Parameters owns one exact scalar/array/metadata operation grant. Other methods
// can be denied; typed event subscription is a separate capability.
type Parameters struct{ cap *Capability }

// Graph owns one independently granted bounded graph snapshot operation.
type Graph struct{ cap *Capability }

// Clock owns one exact authorized clock domain, preserving uint64 nanoseconds.
type Clock struct{ cap *Capability }

// Logger owns an explicit String log-writing grant with host-fixed level/rate quotas.
type Logger struct{ cap *Capability }

// Tf owns one exact TF lookup/broadcast grant with configured target/source frames.
type Tf struct{ cap *Capability }

// Lifecycle owns one explicit ROS2 get/change grant; transitions are host-fixed.
type Lifecycle struct{ cap *Capability }

// CreatePublisher opens an existing host alias; it does not select a new topic or type.
func (n *Node) CreatePublisher(alias string) (*Publisher, error) {
	c, err := n.open(alias)
	if err != nil {
		return nil, err
	}
	return &Publisher{c}, nil
}

// CreateSubscription opens and activates a separately granted topic queue.
// Any activation error closes the newly opened handle before returning.
func (n *Node) CreateSubscription(alias string) (*Subscription, error) {
	c, err := n.open(alias)
	if err != nil {
		return nil, err
	}
	if err = c.Subscribe(); err != nil {
		_ = c.Close()
		return nil, err
	}
	return &Subscription{c}, nil
}

// CreateClient opens a separately granted service-call alias.
func (n *Node) CreateClient(alias string) (*Client, error) {
	c, err := n.open(alias)
	if err != nil {
		return nil, err
	}
	return &Client{c}, nil
}

// CreateServer opens independent advertise/respond aliases and activates requests.
// Partial construction closes every successful prior open.
func (n *Node) CreateServer(advertiseAlias, respondAlias string) (*Server, error) {
	events, err := n.open(advertiseAlias)
	if err != nil {
		return nil, err
	}
	respond, err := n.open(respondAlias)
	if err != nil {
		_ = events.Close()
		return nil, err
	}
	if err = events.AdvertiseService(); err != nil {
		_ = respond.Close()
		_ = events.Close()
		return nil, err
	}
	state:=&callbackState{}; events.state.callback=state; respond.state.callback=state
	return &Server{events:events, respond:respond, state:state}, nil
}

// CreateActionServer opens independent advertise/respond grants and activates goals.
func (n *Node) CreateActionServer(advertiseAlias, respondAlias string) (*ActionServer, error) {
	events, err := n.open(advertiseAlias)
	if err != nil {
		return nil, err
	}
	respond, err := n.open(respondAlias)
	if err != nil {
		_ = events.Close()
		return nil, err
	}
	if _, err = events.Request(OpActionAdvertise, nil, nil); err != nil {
		_ = respond.Close()
		_ = events.Close()
		return nil, err
	}
	state:=&callbackState{}; events.state.callback=state; respond.state.callback=state
	return &ActionServer{events:events, respond:respond, state:state}, nil
}

// CreateActionClient opens one operation-specific host alias; no sibling methods are implicitly granted.
func (n *Node) CreateActionClient(alias string) (*ActionClient, error) {
	c, err := n.open(alias)
	if err != nil {
		return nil, err
	}
	return &ActionClient{c}, nil
}

// CreateParameters opens one operation-specific host alias; no sibling methods are implicitly granted.
func (n *Node) CreateParameters(alias string) (*Parameters, error) {
	c, err := n.open(alias)
	if err != nil {
		return nil, err
	}
	return &Parameters{c}, nil
}

// CreateGraph opens one operation-specific host alias; no sibling methods are implicitly granted.
func (n *Node) CreateGraph(alias string) (*Graph, error) {
	c, err := n.open(alias)
	if err != nil {
		return nil, err
	}
	return &Graph{c}, nil
}

// CreateClock opens one operation-specific host alias; no sibling methods are implicitly granted.
func (n *Node) CreateClock(alias string) (*Clock, error) {
	c, err := n.open(alias)
	if err != nil {
		return nil, err
	}
	return &Clock{c}, nil
}

// CreateLogger opens one operation-specific host alias; no sibling methods are implicitly granted.
func (n *Node) CreateLogger(alias string) (*Logger, error) {
	c, err := n.open(alias)
	if err != nil {
		return nil, err
	}
	return &Logger{c}, nil
}

// CreateTf opens one operation-specific host alias; no sibling methods are implicitly granted.
func (n *Node) CreateTf(alias string) (*Tf, error) {
	c, err := n.open(alias)
	if err != nil {
		return nil, err
	}
	return &Tf{c}, nil
}

// CreateLifecycle opens one operation-specific host alias; no sibling methods are implicitly granted.
func (n *Node) CreateLifecycle(alias string) (*Lifecycle, error) {
	c, err := n.open(alias)
	if err != nil {
		return nil, err
	}
	return &Lifecycle{c}, nil
}

// Close retires the owned Publisher handle even if transport notification fails.
func (x *Publisher) Close() error { return x.cap.Close() }

// Close retires the owned Subscription handle even if transport notification fails.
func (x *Subscription) Close() error { return x.cap.Close() }

// Close retires the owned Client handle even if transport notification fails.
func (x *Client) Close() error { return x.cap.Close() }

// Close retires the owned ActionClient handle even if transport notification fails.
func (x *ActionClient) Close() error { return x.cap.Close() }

// Close retires the owned Parameters handle even if transport notification fails.
func (x *Parameters) Close() error { return x.cap.Close() }

// Close retires the owned Graph handle even if transport notification fails.
func (x *Graph) Close() error { return x.cap.Close() }

// Close retires the owned Clock handle even if transport notification fails.
func (x *Clock) Close() error { return x.cap.Close() }

// Close retires the owned Logger handle even if transport notification fails.
func (x *Logger) Close() error { return x.cap.Close() }

// Close retires the owned Tf handle even if transport notification fails.
func (x *Tf) Close() error { return x.cap.Close() }

// Close retires the owned Lifecycle handle even if transport notification fails.
func (x *Lifecycle) Close() error { return x.cap.Close() }

// Close retires both server handles; it still closes the second if the first fails.
func (x *Server) Close() error {
	if err:=callbackIdle(x.state); err!=nil { return err }
	first := x.events.Close()
	second := x.respond.Close()
	if first != nil {
		return first
	}
	return second
}

// Close retires both Action server handles, preserving the first reported error.
func (x *ActionServer) Close() error {
	if err:=callbackIdle(x.state); err!=nil { return err }
	first := x.events.Close()
	second := x.respond.Close()
	if first != nil {
		return first
	}
	return second
}

// Publish submits exact canonical bytes under the explicit topic grant.
func (x *Publisher) Publish(b []byte) error { return x.cap.Publish(b) }

// PublishMessage encodes one builtin typed value before invoking Publish.
func (x *Publisher) PublishMessage(v Message) error {
	b, err := v.Encode()
	if err != nil {
		return err
	}
	return x.Publish(b)
}

// PublishString uses caller-bounded scratch to avoid per-message allocation.
func (x *Publisher) PublishString(v string, scratch []byte) error {
	n, err := EncodeString(v, scratch)
	if err != nil {
		return err
	}
	return x.Publish(scratch[:n])
}

// PublishBool writes the exact one-byte Bool shape with no integer coercion.
func (x *Publisher) PublishBool(v bool) error {
	b := []byte{0}
	if v {
		b[0] = 1
	}
	return x.Publish(b)
}

// PublishTwist writes finite linear/angular components; magnitude bounds remain host policy.
func (x *Publisher) PublishTwist(v [6]float64) error {
	var b [48]byte
	_, err := EncodeTwist(v, b[:])
	if err != nil {
		return err
	}
	return x.Publish(b[:])
}

// Poll obtains one canonical event. Again is normal, and no retry on90 is performed.
func (x *Subscription) Poll(out []byte) (uint64, error) { return x.cap.Poll(out) }

// PollString obtains and decodes one complete canonical String event.
func (x *Subscription) PollString(out []byte) (string, error) {
	n, err := x.Poll(out)
	if err != nil {
		return "", err
	}
	return DecodeString(out[:n])
}

// Call invokes the selected service with canonical request bytes and bounded response.
func (x *Client) Call(request, response []byte) (uint64, error) {
	return x.cap.Request(OpServiceCall, request, response)
}

// Trigger uses an empty request and decodes the native Bool+String response.
func (x *Client) Trigger(response []byte) (ServiceResponse, error) {
	n, err := x.Call(nil, response)
	if err != nil {
		return ServiceResponse{}, err
	}
	return DecodeServiceResponse(response[:n])
}

// SetBool uses exactly one canonical Bool request and decodes Bool+String response.
func (x *Client) SetBool(v bool, response []byte) (ServiceResponse, error) {
	b := []byte{0}
	if v {
		b[0] = 1
	}
	n, err := x.Call(b, response)
	if err != nil {
		return ServiceResponse{}, err
	}
	return DecodeServiceResponse(response[:n])
}

// Poll obtains one Gateway-owned service request ID and selected canonical request.
func (x *Server) Poll(out []byte) (uint64, error) { if err:=callbackIdle(x.state); err!=nil { return 0,err }; return x.events.Poll(out) }

// Respond submits an owned request ID plus selected response under the separate grant.
func (x *Server) Respond(reply []byte) error {
	_, err := x.respond.Request(OpServiceRespond, reply, nil)
	return err
}

// RespondBool encodes a Trigger/SetBool reply for a decoded request ID.
func (x *Server) RespondBool(id RequestID, v ServiceResponse, scratch []byte) error {
	n, err := EncodeServiceReply(id, v, scratch)
	if err != nil {
		return err
	}
	return x.Respond(scratch[:n])
}

// SendFibonacciGoal sends order0..46 and decodes the Gateway-issued goal ID.
func (x *ActionClient) SendFibonacciGoal(order int32) (GoalID, error) {
	var a [4]byte
	_, err := EncodeFibonacciGoal(order, a[:])
	if err != nil {
		return GoalID{}, err
	}
	var b [8]byte
	n, err := x.cap.Request(OpActionSendGoal, a[:], b[:])
	if err != nil {
		return GoalID{}, err
	}
	return DecodeGoalID(b[:n])
}

// CancelOwn requests cancellation with an independently granted own-cancel alias.
func (x *ActionClient) CancelOwn(id GoalID) (bool, error) {
	if id.value == 0 {
		return false, invalid()
	}
	a := id.Bytes()
	var b [1]byte
	n, err := x.cap.Request(OpActionCancelOwnGoal, a[:], b[:])
	if err != nil {
		return false, err
	}
	return DecodeBool(b[:n])
}

// CancelAny requests a known goal cancellation using separate cancel-any authority.
func (x *ActionClient) CancelAny(id GoalID) (bool, error) {
	if id.value == 0 {
		return false, invalid()
	}
	a := id.Bytes()
	var b [1]byte
	n, err := x.cap.Request(OpActionCancelAnyGoal, a[:], b[:])
	if err != nil {
		return false, err
	}
	return DecodeBool(b[:n])
}

// CancelAll uses goal zero only for independently granted cancel-any authority.
// This is a stronger operation than canceling a known owned goal; the SDK never
// opens the alias automatically and never constructs a zero GoalID.
func (x *ActionClient) CancelAll() (bool, error) {
	var request [8]byte
	var response [1]byte
	n, err := x.cap.Request(OpActionCancelAnyGoal, request[:], response[:])
	if err != nil {
		return false, err
	}
	return DecodeBool(response[:n])
}

// Feedback receives and decodes one envelope for an owned ID under a receive grant.
func (x *ActionClient) Feedback(id GoalID, out []byte) (FibonacciSequence, error) {
	if id.value == 0 {
		return FibonacciSequence{}, invalid()
	}
	a := id.Bytes()
	n, err := x.cap.Request(OpActionReceiveFeedback, a[:], out)
	if err != nil {
		return FibonacciSequence{}, err
	}
	value, err := DecodeFibonacciFeedback(out[:n])
	if err != nil { return FibonacciSequence{},err }
	if value.Goal.Raw() != id.Raw() { return FibonacciSequence{},invalid() }
	return value,nil
}

// Result receives one terminal outcome for an owned ID under a receive-result grant.
func (x *ActionClient) Result(id GoalID, out []byte) (FibonacciSequence, error) {
	if id.value == 0 {
		return FibonacciSequence{}, invalid()
	}
	a := id.Bytes()
	n, err := x.cap.Request(OpActionReceiveResult, a[:], out)
	if err != nil {
		return FibonacciSequence{}, err
	}
	value, err := DecodeFibonacciResult(out[:n])
	if err != nil { return FibonacciSequence{},err }
	if value.Goal.Raw() != id.Raw() { return FibonacciSequence{},invalid() }
	return value,nil
}

// PollGoal decodes one incoming Fibonacci ID/order from the advertised goal queue.
func (x *ActionServer) PollGoal() (GoalID, int32, error) {
	if err:=callbackIdle(x.state); err!=nil { return GoalID{},0,err }
	return x.pollGoal()
}
func (x *ActionServer) pollGoal() (GoalID, int32, error) {
	var b [12]byte
	n, err := x.events.Poll(b[:])
	if err != nil {
		return GoalID{}, 0, err
	}
	return DecodeFibonacciGoalEvent(b[:n])
}

// RespondFibonacci sends feedback1/success2/abort3/cancel4 under the respond grant.
// The host requires a genuinely cancelling goal before accepting kind4.
func (x *ActionServer) RespondFibonacci(id GoalID, kind uint8, values []int32, scratch []byte) error {
	n, err := EncodeFibonacciReply(id, kind, values, scratch)
	if err != nil {
		return err
	}
	_, err = x.respond.Request(OpActionRespond, scratch[:n], nil)
	return err
}

// Get invokes the exact ParameterGet canonical RPC with an explicit output capacity.
// The existing alias must independently grant this operation; incompatible backend returns95.
func (x *Parameters) Get(request, out []byte) (uint64, error) {
	return x.cap.Request(OpParameterGet, request, out)
}

// Set invokes the exact ParameterSet canonical RPC with an explicit output capacity.
// The existing alias must independently grant this operation; incompatible backend returns95.
func (x *Parameters) Set(request, out []byte) (uint64, error) {
	return x.cap.Request(OpParameterSet, request, out)
}

// Delete invokes the exact ParameterDelete canonical RPC with an explicit output capacity.
// The existing alias must independently grant this operation; incompatible backend returns95.
func (x *Parameters) Delete(request, out []byte) (uint64, error) {
	return x.cap.Request(OpParameterDelete, request, out)
}

// Exists invokes the exact ParameterExists canonical RPC with an explicit output capacity.
// The existing alias must independently grant this operation; incompatible backend returns95.
func (x *Parameters) Exists(request, out []byte) (uint64, error) {
	return x.cap.Request(OpParameterExists, request, out)
}

// List invokes the exact ParameterList canonical RPC with an explicit output capacity.
// The existing alias must independently grant this operation; incompatible backend returns95.
func (x *Parameters) List(request, out []byte) (uint64, error) {
	return x.cap.Request(OpParameterList, request, out)
}

// Search invokes the exact ParameterSearch canonical RPC with an explicit output capacity.
// The existing alias must independently grant this operation; incompatible backend returns95.
func (x *Parameters) Search(request, out []byte) (uint64, error) {
	return x.cap.Request(OpParameterSearch, request, out)
}

// Describe invokes the exact ParameterDescribe canonical RPC with an explicit output capacity.
// The existing alias must independently grant this operation; incompatible backend returns95.
func (x *Parameters) Describe(request, out []byte) (uint64, error) {
	return x.cap.Request(OpParameterDescribe, request, out)
}

// Declare invokes the exact ParameterDeclare canonical RPC with an explicit output capacity.
// The existing alias must independently grant this operation; incompatible backend returns95.
func (x *Parameters) Declare(request, out []byte) (uint64, error) {
	return x.cap.Request(OpParameterDeclare, request, out)
}

// Undeclare invokes the exact ParameterUndeclare canonical RPC with an explicit output capacity.
// The existing alias must independently grant this operation; incompatible backend returns95.
func (x *Parameters) Undeclare(request, out []byte) (uint64, error) {
	return x.cap.Request(OpParameterUndeclare, request, out)
}

// InspectTopic invokes the exact GraphInspectTopic canonical RPC with an explicit output capacity.
// The existing alias must independently grant this operation; incompatible backend returns95.
func (x *Graph) InspectTopic(request, out []byte) (uint64, error) {
	return x.cap.Request(OpGraphInspectTopic, request, out)
}

// InspectNode invokes the exact GraphInspectNode canonical RPC with an explicit output capacity.
// The existing alias must independently grant this operation; incompatible backend returns95.
func (x *Graph) InspectNode(request, out []byte) (uint64, error) {
	return x.cap.Request(OpGraphInspectNode, request, out)
}

// InspectService invokes the exact GraphInspectService canonical RPC with an explicit output capacity.
// The existing alias must independently grant this operation; incompatible backend returns95.
func (x *Graph) InspectService(request, out []byte) (uint64, error) {
	return x.cap.Request(OpGraphInspectService, request, out)
}

// InspectAction invokes the exact GraphInspectAction canonical RPC with an explicit output capacity.
// The existing alias must independently grant this operation; incompatible backend returns95.
func (x *Graph) InspectAction(request, out []byte) (uint64, error) {
	return x.cap.Request(OpGraphInspectAction, request, out)
}

// ListTopics invokes the exact GraphListTopics canonical RPC with an explicit output capacity.
// The existing alias must independently grant this operation; incompatible backend returns95.
func (x *Graph) ListTopics(request, out []byte) (uint64, error) {
	return x.cap.Request(OpGraphListTopics, request, out)
}

// ListNodes invokes the exact GraphListNodes canonical RPC with an explicit output capacity.
// The existing alias must independently grant this operation; incompatible backend returns95.
func (x *Graph) ListNodes(request, out []byte) (uint64, error) {
	return x.cap.Request(OpGraphListNodes, request, out)
}

// ListServices invokes the exact GraphListServices canonical RPC with an explicit output capacity.
// The existing alias must independently grant this operation; incompatible backend returns95.
func (x *Graph) ListServices(request, out []byte) (uint64, error) {
	return x.cap.Request(OpGraphListServices, request, out)
}

// Lookup invokes the exact TfLookup canonical RPC with an explicit output capacity.
// The existing alias must independently grant this operation; incompatible backend returns95.
func (x *Tf) Lookup(request, out []byte) (uint64, error) {
	return x.cap.Request(OpTfLookup, request, out)
}

// Broadcast invokes the exact TfBroadcast canonical RPC with an explicit output capacity.
// The existing alias must independently grant this operation; incompatible backend returns95.
func (x *Tf) Broadcast(request, out []byte) (uint64, error) {
	return x.cap.Request(OpTfBroadcast, request, out)
}

// GetState invokes the exact LifecycleGetState canonical RPC with an explicit output capacity.
// The existing alias must independently grant this operation; incompatible backend returns95.
func (x *Lifecycle) GetState(request, out []byte) (uint64, error) {
	return x.cap.Request(OpLifecycleGetState, request, out)
}

// ChangeState invokes the exact LifecycleChangeState canonical RPC with an explicit output capacity.
// The existing alias must independently grant this operation; incompatible backend returns95.
func (x *Lifecycle) ChangeState(request, out []byte) (uint64, error) {
	return x.cap.Request(OpLifecycleChangeState, request, out)
}

// SubscribeEvents activates a separately granted ROS2 parameter event queue.
// The returned subscription shares retirement state; transfer ownership and close
// it once. Scalar get/set/metadata grants cannot satisfy this operation.
func (x *Parameters) SubscribeEvents() (*Subscription, error) {
	if err := x.cap.SubscribeParameterEvents(); err != nil {
		return nil, err
	}
	return &Subscription{x.cap}, nil
}

// GetValue reads and decodes the exact scalar/array kind selected by host policy.
func (x *Parameters) GetValue(kind ParameterKind, out []byte) (ParameterValue, error) {
	n, err := x.Get(nil, out)
	if err != nil {
		return ParameterValue{}, err
	}
	return DecodeParameter(kind, out[:n])
}

// SetValue encodes one selected typed scalar/array under its explicit set grant.
func (x *Parameters) SetValue(v ParameterValue) error {
	b, err := v.Encode()
	if err != nil {
		return err
	}
	_, err = x.Set(b, nil)
	return err
}

// DeclareValue declares a selected value only on an explicitly named ROS2 Gateway node.
// The current native adapter does not support remote declaration.
func (x *Parameters) DeclareValue(v ParameterValue) error {
	b, err := v.Encode()
	if err != nil {
		return err
	}
	_, err = x.Declare(b, nil)
	return err
}

// Snapshot reads only a graph operation and decodes its bounded canonical String.
func (x *Graph) Snapshot(op Operation, out []byte) (string, error) {
	switch op {
	case OpGraphInspectTopic, OpGraphInspectNode, OpGraphInspectService, OpGraphInspectAction, OpGraphListTopics, OpGraphListNodes, OpGraphListServices:
	default:
		return "", invalid()
	}
	n, err := x.cap.Request(op, nil, out)
	if err != nil {
		return "", err
	}
	return DecodeString(out[:n])
}

// Read returns exact uint64 nanoseconds for a selected clock-domain operation.
func (x *Clock) Read(op Operation) (uint64, error) {
	switch op {
	case OpTimeReadRos, OpTimeReadWall, OpTimeReadSystem, OpTimeReadSteady:
	default:
		return 0, invalid()
	}
	var b [8]byte
	n, err := x.cap.Request(op, nil, b[:])
	if err != nil {
		return 0, err
	}
	return DecodeTime(b[:n])
}

// Write encodes a canonical String using caller scratch; the log level is host-fixed.
func (x *Logger) Write(text string, scratch []byte) error {
	n, err := EncodeString(text, scratch)
	if err != nil {
		return err
	}
	return x.cap.Log(scratch[:n])
}

// LookupTransform reads the exact frame pair at unsigned nanoseconds; zero means latest.
func (x *Tf) LookupTransform(time uint64, out []byte) (Transform, error) {
	var a [8]byte
	putTime(a[:], time)
	n, err := x.Lookup(a[:], out)
	if err != nil {
		return Transform{}, err
	}
	return DecodeTransform(out[:n])
}

// BroadcastTransform encodes a finite Transform and uses its exact broadcast grant.
func (x *Tf) BroadcastTransform(v Transform) error {
	b, err := v.Encode()
	if err != nil {
		return err
	}
	_, err = x.Broadcast(b, nil)
	return err
}

// State reads the native state ID/label under a separate get-state grant.
func (x *Lifecycle) State(out []byte) (LifecycleState, error) {
	n, err := x.GetState(nil, out)
	if err != nil {
		return LifecycleState{}, err
	}
	return DecodeLifecycleState(out[:n])
}

// Change uses an empty request: only the transition configured by the host is requested.
func (x *Lifecycle) Change() (bool, error) {
	var b [1]byte
	n, err := x.ChangeState(nil, b[:])
	if err != nil {
		return false, err
	}
	return DecodeBool(b[:n])
}

// AddSubscription registers a queue and callback only when no spin is active.
// Nil arguments or callback-time registration attempts are ignored before any
// event is polled. In those cases the caller retains lifetime responsibility;
// register after SpinOnce returns. Callback bytes expire when the next poll
// overwrites scratch, and nested SpinOnce is explicitly refused.
func (x *Executor) AddSubscription(sub *Subscription, callback func([]byte)) {
	if sub == nil || callback == nil {
		return
	}
	x.add(sub, callback)
}

// TryAddSubscription checks registration before mutation. A refused registration
// leaves the caller owning its queue. The existing AddSubscription compatibility
// API ignores the status; use this method for checked application setup.
func (x *Executor) TryAddSubscription(sub *Subscription, callback func([]byte)) error {
	if sub == nil { return Error{Code: Invalid} }
	return x.tryAdd(sub, callback)
}

// RemoveSubscription transfers a registered queue back without closing it.
// During a callback mutation is22; after Close it is9. The index must refer to
// an existing Subscription registration, never an arbitrary native handle.
func (x *Executor) RemoveSubscription(index int) (*Subscription, error) {
	if x == nil || x.closed { return nil, Error{Code: BadHandle} }
	if x.spinning { return nil, Error{Code: Invalid} }
	if index < 0 || index >= len(x.entries) { return nil, Error{Code: Invalid} }
	sub, ok := x.entries[index].source.(*Subscription)
	if !ok { return nil, Error{Code: Invalid} }
	copy(x.entries[index:], x.entries[index+1:])
	x.entries[len(x.entries)-1] = executorEntry{}
	x.entries = x.entries[:len(x.entries)-1]
	return sub, nil
}

// SpinOnce dispatches one builtin Trigger/SetBool event with its exact incoming
// request ID. Buffers remain caller-owned and must not overlap. The handler
// returns an initialized reply prefix; this method checks its ID and complete
// Bool/String response before importing service.respond. Again is normal idle.
func (x *Server) SpinOnce(input, output []byte, handler func(RequestID, []byte, []byte) (uint64, error)) (bool, error) {
	return serviceOnce(x.state,x,input,output,handler)
}

// SpinGoalOnce invokes one guest-thread Fibonacci goal handler. Its opaque ID
// came from this server's event queue; response remains an explicit method under
// the separate respond grant. No native thread or callback enters TinyGo.
func (x *ActionServer) SpinGoalOnce(handler func(GoalID, int32) error) (bool, error) {
	return goalOnce(x.state,x,handler)
}

// Checked owner methods borrow opaque IDs or retire authority exactly once.

// Revoke retires the local Publisher and requests alias revocation once.
func (x *Publisher) Revoke() error { return x.cap.Revoke() }

// RawHandle borrows the exact live ID; it creates no authority or ownership.
func (x *Publisher) RawHandle() (uint64, error) { return x.cap.RawHandle() }

// Revoke retires the local Subscription and requests alias revocation once.
func (x *Subscription) Revoke() error { return x.cap.Revoke() }

// RawHandle borrows the exact live ID; it creates no authority or ownership.
func (x *Subscription) RawHandle() (uint64, error) { return x.cap.RawHandle() }

// Revoke retires the local Client and requests alias revocation once.
func (x *Client) Revoke() error { return x.cap.Revoke() }

// RawHandle borrows the exact live ID; it creates no authority or ownership.
func (x *Client) RawHandle() (uint64, error) { return x.cap.RawHandle() }

// Revoke retires the local ActionClient and requests alias revocation once.
func (x *ActionClient) Revoke() error { return x.cap.Revoke() }

// RawHandle borrows the exact live ID; it creates no authority or ownership.
func (x *ActionClient) RawHandle() (uint64, error) { return x.cap.RawHandle() }

// Revoke retires the local Parameters and requests alias revocation once.
func (x *Parameters) Revoke() error { return x.cap.Revoke() }

// RawHandle borrows the exact live ID; it creates no authority or ownership.
func (x *Parameters) RawHandle() (uint64, error) { return x.cap.RawHandle() }

// Revoke retires the local Graph and requests alias revocation once.
func (x *Graph) Revoke() error { return x.cap.Revoke() }

// RawHandle borrows the exact live ID; it creates no authority or ownership.
func (x *Graph) RawHandle() (uint64, error) { return x.cap.RawHandle() }

// Revoke retires the local Clock and requests alias revocation once.
func (x *Clock) Revoke() error { return x.cap.Revoke() }

// RawHandle borrows the exact live ID; it creates no authority or ownership.
func (x *Clock) RawHandle() (uint64, error) { return x.cap.RawHandle() }

// Revoke retires the local Logger and requests alias revocation once.
func (x *Logger) Revoke() error { return x.cap.Revoke() }

// RawHandle borrows the exact live ID; it creates no authority or ownership.
func (x *Logger) RawHandle() (uint64, error) { return x.cap.RawHandle() }

// Revoke retires the local Tf and requests alias revocation once.
func (x *Tf) Revoke() error { return x.cap.Revoke() }

// RawHandle borrows the exact live ID; it creates no authority or ownership.
func (x *Tf) RawHandle() (uint64, error) { return x.cap.RawHandle() }

// Revoke retires the local Lifecycle and requests alias revocation once.
func (x *Lifecycle) Revoke() error { return x.cap.Revoke() }

// RawHandle borrows the exact live ID; it creates no authority or ownership.
func (x *Lifecycle) RawHandle() (uint64, error) { return x.cap.RawHandle() }

// Revoke attempts both independent Server authorities and preserves the first error.
func (x *Server) Revoke() error { if err:=callbackIdle(x.state); err!=nil { return err }; first := x.events.Revoke(); second := x.respond.Revoke(); if first != nil { return first }; return second }

// RawHandles borrows both live opaque IDs without transferring ownership.
func (x *Server) RawHandles() (uint64, uint64, error) { events, err := x.events.RawHandle(); if err != nil { return 0, 0, err }; respond, err := x.respond.RawHandle(); if err != nil { return 0, 0, err }; return events, respond, nil }

// Revoke attempts both independent ActionServer authorities and preserves the first error.
func (x *ActionServer) Revoke() error { if err:=callbackIdle(x.state); err!=nil { return err }; first := x.events.Revoke(); second := x.respond.Revoke(); if first != nil { return first }; return second }

// RawHandles borrows both live opaque IDs without transferring ownership.
func (x *ActionServer) RawHandles() (uint64, uint64, error) { events, err := x.events.RawHandle(); if err != nil { return 0, 0, err }; respond, err := x.respond.RawHandle(); if err != nil { return 0, 0, err }; return events, respond, nil }

// Private adapters preserve the real capability paths; public Poll retains its guard.
func (x *Server) pollService(out []byte)(uint64,error){ return x.events.Poll(out) }
func (x *Server) respondService(wire []byte)error{ return x.Respond(wire) }
