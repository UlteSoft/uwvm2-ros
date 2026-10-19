package org.ros2.rcljava.migration;

import java.lang.ref.WeakReference;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.concurrent.TimeUnit;
import org.uwvm.roswasm.Capability;
import org.uwvm.roswasm.Codec;
import org.uwvm.roswasm.Codecs;
import org.uwvm.roswasm.Result;
import org.uwvm.roswasm.RetirementAttempt;
import org.uwvm.roswasm.RosException;
import org.uwvm.roswasm.Session;
import org.ros2.rcljava.concurrent.Callback;
import org.ros2.rcljava.consumers.Consumer;
import org.ros2.rcljava.contexts.Context;
import org.ros2.rcljava.interfaces.Disposable;
import org.ros2.rcljava.interfaces.MessageDefinition;
import org.ros2.rcljava.node.Node;
import org.ros2.rcljava.publisher.Publisher;
import org.ros2.rcljava.subscription.Subscription;
import org.ros2.rcljava.qos.QoSProfile;
import org.ros2.rcljava.timer.Timer;
import org.ros2.rcljava.timer.WallTimer;

/** One finite, single-Guest-thread migration context over the actual ROS2 SDK.
 * No method creates a native rcl node/context, JNI pointer, wait set or thread.
 * Fixed registration and cleanup storage precede every resource effect. An
 * active callback leases the WHOLE context: all close/revoke/factory/shutdown
 * requests fail17, including requests involving another node or alias. Ordinary
 * read/publish operations may still run inside a callback. The core Session
 * is private so this facade cannot accidentally expose an unleased close path.
 */
public final class MigrationContext implements Context {
    private final Deployment.Profile profile;
    private final NodeImpl[] nodes;
    private final Resource[] resources;
    private Session session;
    private int remainingCredits;
    private boolean initialized,active,dispatching,closing,availabilityWaitInProgress;
    private final CleanupReport contextCleanup;
    private final CleanupFailure contextFailure;
    private CleanupAttempt lastFactoryCleanup;
    private static final Result ALREADY_RETIRED = new Result(0);
    private static final byte[] EMPTY=new byte[0];

    /** Fixed scalar/object slots, allocated before an open or cleanup effect.
     * Result/failure are the real outcomes. A local retired handle is never
     * replayed after timeout/error, even when native notification is uncertain.
     */
    public static final class CleanupAttempt {
        public Throwable primary,failure;
        public CleanupAttempt next;
        public Result result;
        public boolean attempted;
        /** Original full i64, exact native/custom boundary, and NotAttempted
         * state survive disposed resources and a failed factory's table removal.
         * This reserved view changes only on explicit cleanup recovery. */
        public RetirementAttempt retirement;
        void reset() { primary=failure=null;result=null;attempted=false;retirement=null; }
        boolean failed() { for(CleanupAttempt a=this;a!=null;a=a.next)if(a.failure!=null || a.result!=null&&a.result.error!=0)return true;return false; }
    }
    public static final class CleanupReport {
        public final CleanupAttempt[] attempts;
        public boolean completed;
        CleanupReport(int maximum) { attempts=new CleanupAttempt[maximum]; }
        void reset() { for(int i=0;i<attempts.length;++i)attempts[i]=null;completed=false; }
        public boolean failed() { for(CleanupAttempt r:attempts)if(r!=null&&r.failed())return true;return false; }
    }
    public static final class CleanupFailure extends RuntimeException {
        public final CleanupReport report;
        CleanupFailure(CleanupReport report) { super("rcljava migration cleanup failed; inspect reserved report");this.report=report; }
    }
    public MigrationContext(Deployment.Profile profile) {
        if(profile==null)throw new IllegalArgumentException("profile");
        this.profile=profile;remainingCredits=profile.maximumCredits;nodes=new NodeImpl[profile.maximumNodes];resources=new Resource[profile.maximumOwners];
        contextCleanup=new CleanupReport(profile.maximumOwners);contextFailure=new CleanupFailure(contextCleanup);
    }
    public void init() {
        if(initialized) { if(active)return;throw new UnsupportedOperationException("use a fresh Guest for a new context lifecycle"); }
        Session pending=new Session(profile.transport,profile.maximumOwners);
        pending.checkAbi(); // ABI/version failure publishes no live context.
        session=pending;active=true;initialized=true;
    }
    public boolean isValid() { return active; }
    public boolean wasInitialized() { return initialized; }
    public long getHandle() { throw nativePointer(); }
    public CleanupAttempt lastFactoryCleanup() { return lastFactoryCleanup; }
    public CleanupReport lastCleanup() { return contextCleanup; }
    public java.util.List<RetirementAttempt> pendingRetirements() {
        requireLocalMutable();if(session==null)return Collections.emptyList();return session.pendingRetirements();
    }
    public RetirementAttempt lastRevokeAttempt() {
        requireLocalMutable();return session==null?null:session.lastRevokeAttempt();
    }
    /** Explicit cleanup-only recovery, including after shutdown/factory rollback.
     * Does not reactivate a disposed resource/context, spin, grant, or replay an
     * effect-unknown/returned import. Callback and closing leases still apply. */
    public java.util.List<Result> retryNotAttemptedCleanup() {
        requireLocalMutable();if(session==null)return Collections.emptyList();closing=true;
        try{return session.retryNotAttemptedClosures();}finally{closing=false;}
    }
    public int maximumNodes() { return nodes.length; }
    public boolean isDispatching() { return dispatching; }
    public void requireLive() { if(!active)throw error(125); }
    public void requireMutable() { requireLive();requireLocalMutable(); }
    /** Actor-free executor detachment is still valid after context retirement. */
    public void requireLocalMutable() { if(dispatching||closing)throw error(17); }
    private void enterDispatch() { requireLive();if(dispatching||closing)throw error(17);dispatching=true; }
    private void leaveDispatch() { dispatching=false; }
    public boolean nextCredit() {
        if(availabilityWaitInProgress)throw error(17);
        requireMutable();
        if(profile.driver==null)throw new UnsupportedOperationException("blocking spin requires an explicit bounded deployment credit driver; no native wait set exists");
        if(remainingCredits==0)return false;
        --remainingCredits;boolean credit=profile.driver.nextCredit();return active&&credit;
    }
    public boolean hasDriver() { return profile.driver!=null; }
    public Node createNode(String name,String namespace) {
        requireMutable();
        if(name==null||!name.matches("[A-Za-z_][A-Za-z_0-9]*"))throw new IllegalArgumentException("node name");
        String ns=namespace==null||namespace.isEmpty()?"/":namespace;
        if(!ns.matches("/(?:[A-Za-z_][A-Za-z_0-9]*(?:/[A-Za-z_][A-Za-z_0-9]*)*)?"))throw new IllegalArgumentException("namespace");
        int at=free(nodes);if(at<0)throw error(90);
        NodeImpl node=new NodeImpl(name,ns); // all node/report arrays before registration
        nodes[at]=node;return node;
    }
    private NodeImpl own(Node node) {
        if(node==null)throw new IllegalArgumentException("null node");
        for(NodeImpl n:nodes)if(n==node)return n;
        throw new IllegalArgumentException("node is not retained by this exact context");
    }
    public void attachExecutor(Node node,Object executor) {
        requireMutable();NodeImpl n=own(node);n.live();
        if(executor==null||n.executor!=null)throw error(17);n.executor=executor;
    }
    public void detachExecutor(Node node,Object executor) {
        requireLocalMutable();NodeImpl n=own(node);
        if(n.executor!=executor)throw error(17);n.executor=null;
    }
    public int poll(Node node,Object executor) {
        if(availabilityWaitInProgress)throw error(17);
        NodeImpl n=own(node);n.live();if(n.executor!=executor)throw error(17);
        enterDispatch();
        try { return n.pollInside(); } finally { leaveDispatch(); }
    }
    public void pollSnapshot(Node node,Object executor) {
        if(availabilityWaitInProgress)throw error(17);
        NodeImpl n=own(node);n.live();if(n.executor!=executor)throw error(17);
        enterDispatch();
        try {
            for(int count=0;count<n.owned.length;++count) {
                Resource r=n.owned[n.position];n.position=(n.position+1)%n.owned.length;
                if(r!=null&&!r.disposed&&!(r instanceof Pub))r.pollInside();
            }
        } finally { leaveDispatch(); }
    }
    private Deployment.Binding binding(NodeImpl node,String operation,String resource,String type,String qos) {
        for(Deployment.Binding b:profile.bindings)
            if(b.node.equals(node.fqn)&&b.operation.equals(operation)&&b.resource.equals(resource)&&b.type.equals(type)&&b.qos.equals(qos))return b;
        throw error(13);
    }
    private static int free(Object[] table) { for(int i=0;i<table.length;++i)if(table[i]==null)return i;return -1; }
    private void retain(Resource r) {
        requireMutable();r.node.live();int a=free(resources),b=free(r.node.owned);
        if(a<0||b<0)throw error(90);
        r.globalSlot=a;r.nodeSlot=b;resources[a]=r;r.node.owned[b]=r;
    }
    private void failedFactory(Resource r,Throwable primary) {
        // The object/cleanup slots were retained before Session.open. A plain
        // field store after its successful return commits that actual owner.
        r.cleanup.primary=primary;r.retire();lastFactoryCleanup=r.cleanup;
        r.node.owned[r.nodeSlot]=null;resources[r.globalSlot]=null;
        for(CleanupAttempt a=r.cleanup;a!=null;a=a.next) {
            Throwable cleanup=a.failure;
            if(cleanup==null&&a.result!=null&&a.result.error!=0)try{cleanup=new RosException(a.result);}catch(RuntimeException|Error unavailable){}
            if(cleanup!=null&&cleanup!=primary)try{primary.addSuppressed(cleanup);}catch(RuntimeException|Error unavailable){}
        }
    }
    public void revoke(Disposable value) {
        requireMutable();
        Resource selected=null;
        for(Resource r:resources)if(r==value){selected=r;break;}
        if(selected==null||selected.disposed||selected.owner==null)throw error(9);
        try { selected.owner.revokeChecked().checked(); }
        finally {
            // Alias revocation already retires same-session alias siblings.
            // Local Futures cannot remain pending after their real source died;
            // the separate observer alias is still explicitly closed later.
            for(Resource r:resources)if(r instanceof ClientImpl&&r.owner!=null&&r.owner.handle()==0){ClientImpl<?> c=(ClientImpl<?>)r;c.retired=true;c.finishLocally();}
        }
    }
    public void shutdown() {
        requireMutable();contextCleanup.reset();closing=true;
        // No collection grows during retirement, including after a native
        // close throws the same Error as an application/setup primary.
        try {
            for(int i=resources.length-1;i>=0;--i) {
                Resource r=resources[i];if(r==null)continue;
                r.retire();contextCleanup.attempts[i]=r.cleanup;
            }
            session.shutdown();active=false;
            for(NodeImpl n:nodes)if(n!=null)n.disposed=true;
            contextCleanup.completed=true;
        } finally { closing=false; }
        if(contextCleanup.failed())throw contextFailure;
    }
    public void dispose() { if(active)shutdown(); }
    private static RosException error(int code) { return new RosException(new Result(code)); }
    private static UnsupportedOperationException nativePointer() { return new UnsupportedOperationException("native rcl/JNI pointers are not Wasm capabilities"); }
    @SuppressWarnings("unchecked")
    private static <T extends MessageDefinition> Codec<T> codec(Class<T> type) {
        if(type!=std_msgs.msg.String.class)throw new UnsupportedOperationException("only the complete pinned std_msgs.msg.String migration type is implemented");
        return (Codec<T>)STRING;
    }
    private static final Codec<std_msgs.msg.String> STRING=new Codec<std_msgs.msg.String>() {
        public byte[] encode(std_msgs.msg.String v) { if(v==null)throw new IllegalArgumentException("String message");return Codecs.STRING.encode(v.getData()); }
        public std_msgs.msg.String decode(byte[] wire) { String text=Codecs.STRING.decode(wire);std_msgs.msg.String v=new std_msgs.msg.String();v.setData(text);return v; }
    };
    private static String qosKey(QoSProfile profile) { if(profile==null)throw new IllegalArgumentException("QoS");return profile.deploymentKey(); }

    private abstract class Resource implements Disposable {
        final NodeImpl node;
        final WeakReference<Node> reference;
        final CleanupAttempt cleanup=new CleanupAttempt();
        Capability owner;
        int globalSlot=-1,nodeSlot=-1;
        boolean disposed;
        Resource(NodeImpl node) { this.node=node;reference=new WeakReference<Node>(node); }
        void usable() { node.live();if(disposed||owner==null||owner.handle()==0)throw error(9); }
        void retire() {
            if(disposed)return;
            disposed=true;
            if(owner==null||owner.handle()==0){cleanup.result=ALREADY_RETIRED;return;}
            cleanup.retirement=owner.lastRetirement();
            try { cleanup.result=owner.closeChecked(); }
            catch(RuntimeException|Error failure) { cleanup.failure=failure; }
            finally { cleanup.attempted=cleanup.retirement.attempted(); }
        }
        public void dispose() {
            requireMutable();retire();
            for(CleanupAttempt a=cleanup;a!=null;a=a.next) {
                if(a.failure instanceof RuntimeException)throw (RuntimeException)a.failure;
                if(a.failure instanceof Error)throw (Error)a.failure;
                if(a.result!=null)a.result.checked();
            }
        }
        public long getHandle() { throw nativePointer(); }
        int pollInside() { return 11; }
    }
    private final class Pub<T extends MessageDefinition> extends Resource implements Publisher<T> {
        final Codec<T> codec;
        Pub(NodeImpl node,Codec<T> codec) { super(node);this.codec=codec; }
        public void publish(T value) { usable();owner.invoke("topic_publish",codec.encode(value),0).checked(); }
        public WeakReference<Node> getNodeReference() { return reference; }
    }
    private final class Sub<T extends MessageDefinition> extends Resource implements Subscription<T> {
        final Class<T> type;final Codec<T> codec;final Consumer<T> callback;final int capacity;
        Sub(NodeImpl node,Class<T> type,Codec<T> codec,Consumer<T> callback,int capacity) { super(node);this.type=type;this.codec=codec;this.callback=callback;this.capacity=capacity; }
        public Class<T> getMessageType() { return type; }
        public WeakReference<Node> getNodeReference() { return reference; }
        public void executeCallback(T value) { usable();enterDispatch();try{callback.accept(value);}finally{leaveDispatch();} }
        int pollInside() {
            if(disposed)return 11;usable();
            Result result=owner.invoke("event_poll",new byte[0],capacity);
            if(result.error==0)callback.accept(codec.decode(result.data));
            if(result.error!=0&&result.error!=11)result.checked();
            return result.error;
        }
    }
    private final class Wall extends Resource implements WallTimer {
        final Callback callback;
        long period,last,deadline,observed;
        boolean canceled;
        Wall(NodeImpl node,long period,Callback callback) { super(node);this.period=period;this.callback=callback; }
        long now() {
            usable();byte[] bytes=owner.invoke("time_read_steady",new byte[0],8).checked().data;
            if(bytes.length!=8)throw new IllegalArgumentException("complete clock64 required");
            long value=new Codecs.Reader(bytes).unsigned(8);
            if(value<0)throw new IllegalArgumentException("clock exceeds this signed64 timer profile");
            return value;
        }
        void start() { observed=last=now();deadline=add(last,period); }
        long current() {
            long value=now();
            if(value<observed) { last=value;deadline=add(value,period); }
            observed=value;return value;
        }
        void called(long value) {
            last=value;
            // Preserve phase and skip missed periods. No callback storm, spin
            // sleep, System.nanoTime or implicit WASI clock is introduced.
            if(value>=deadline) {
                long steps=(value-deadline)/period+1;
                if(steps>Long.MAX_VALUE/period)throw new IllegalArgumentException("timer phase range");
                deadline=add(deadline,steps*period);
            }
        }
        public long getTimerPeriodNS() { usable();return period; }
        public void setTimerPeriodNS(long value) { usable();if(value<=0)throw new IllegalArgumentException("positive timer period");period=value; }
        public boolean isCanceled() { return canceled||disposed; }
        public void cancel() { usable();canceled=true; }
        public void reset() { usable();start();canceled=false; }
        public boolean isReady() { return !disposed&&!canceled&&current()>=deadline; }
        public long timeSinceLastCall() { return current()-last; }
        public long timeUntilNextCall() { return deadline-current(); }
        public void callTimer() { usable();if(!canceled)called(current()); }
        public void executeCallback() { usable();if(canceled)return;enterDispatch();try{callback.call();}finally{leaveDispatch();} }
        int pollInside() {
            if(disposed||canceled)return 11;long value=current();if(value<deadline)return 11;
            called(value);callback.call();return 0;
        }
    }
    /** A two-alias endpoint owns two independent native capabilities. Its
     * complete cleanup chain and pending callback slots precede the first open.
     * No public facade exposes the private Session or a native pointer.
     */
    private abstract class ServicePair extends Resource {
        final ServiceTypes.Descriptor schema;
        final Deployment.ServiceBinding selection;
        final String serviceName;
        final CleanupAttempt secondCleanup=new CleanupAttempt();
        Capability second;
        ServicePair(NodeImpl node,ServiceTypes.Descriptor schema,Deployment.ServiceBinding selection,String serviceName) {
            super(node);this.schema=schema;this.selection=selection;this.serviceName=serviceName;cleanup.next=secondCleanup;
        }
        void pairLive() { usable();if(second==null||second.handle()==0)throw error(9); }
        void retire() {
            if(disposed)return;
            // Each owner commits retirement at its proven effect boundary.
            // A thrown/error result on the second owner cannot skip the first.
            if(second==null||second.handle()==0)secondCleanup.result=ALREADY_RETIRED;
            else {secondCleanup.retirement=second.lastRetirement();
                try{secondCleanup.result=second.closeChecked();}catch(RuntimeException|Error failure){secondCleanup.failure=failure;}
                finally{secondCleanup.attempted=secondCleanup.retirement.attempted();}}
            super.retire();onRetired();
        }
        void onRetired() {}
    }
    private final class Server<T extends org.ros2.rcljava.interfaces.ServiceDefinition> extends ServicePair implements org.ros2.rcljava.service.Service<T> {
        final org.ros2.rcljava.consumers.TriConsumer callback;
        Server(NodeImpl node,ServiceTypes.Descriptor schema,Deployment.ServiceBinding binding,String serviceName,org.ros2.rcljava.consumers.TriConsumer callback) {super(node,schema,binding,serviceName);this.callback=callback;}
        @SuppressWarnings("unchecked") public <U extends MessageDefinition> Class<U> getRequestType(){return (Class<U>)schema.request;}
        @SuppressWarnings("unchecked") public <U extends MessageDefinition> Class<U> getResponseType(){return (Class<U>)schema.response;}
        public String getServiceName(){return serviceName;}
        /** The ABI carries an opaque ticket, not RMW writerGUID/sequence.
         * A caller-supplied native header cannot inject a synthetic request.
         */
        public void executeCallback(org.ros2.rcljava.service.RMWRequestId header,MessageDefinition request,MessageDefinition response){throw error(95);}
        @SuppressWarnings("unchecked") int pollInside(){
            if(disposed)return 11;pairLive();
            Result incoming=owner.invoke("event_poll",EMPTY,selection.capacity);
            if(incoming.error!=0){if(incoming.error!=11)incoming.checked();return incoming.error;}
            Codecs.Reader bytes=new Codecs.Reader(incoming.data);long id=bytes.unsigned(8);
            if(id==0)throw error(22);
            MessageDefinition request=schema.decodeRequest(bytes.take(bytes.remaining()));bytes.end();
            MessageDefinition response=schema.newResponse();
            // The real official example ignores this absent header. Never
            // synthesize a native RMW identity from a Wasm correlation ticket.
            callback.accept(null,request,response);
            Result sent=second.invoke("service_respond",Codecs.prefixId(id,schema.encodeResponse(response)),0).checked();
            if(sent.data.length!=0||sent.value!=null||sent.required!=null)throw error(22);return 0;
        }
    }
    /** Actual native event context, separate from a local retirement cause. */
    public static final class ServiceFailure extends RuntimeException {
        public int code,flags;public long requestId;public String origin;
        ServiceFailure(){super("service Future failed; inspect code/flags/requestId/origin");}
    }
    private final class ClientImpl<T extends org.ros2.rcljava.interfaces.ServiceDefinition> extends ServicePair implements org.ros2.rcljava.client.Client<T> {
        final Slot[] pending;
        long lastRequest,sequence,instance,nativeSession;
        boolean bound,retired;
        final CleanupAttempt queryCleanup=new CleanupAttempt(),clockCleanup=new CleanupAttempt();
        Capability queryOwner,waitClock;
        ClientImpl(NodeImpl node,ServiceTypes.Descriptor schema,Deployment.ServiceBinding binding,String serviceName){
            super(node,schema,binding,serviceName);pending=new Slot[binding.maximumPending];
            CleanupAttempt previous=secondCleanup;
            if(binding.availability!=null){previous.next=queryCleanup;previous=queryCleanup;
                if(binding.availability.steadyClockAlias!=null){previous.next=clockCleanup;previous=clockCleanup;}}
            for(int i=0;i<pending.length;++i){pending[i]=new Slot();previous.next=pending[i].callbackCleanup;previous=pending[i].callbackCleanup;}
        }
        @SuppressWarnings("unchecked") public <U extends MessageDefinition> Class<U> getRequestType(){return (Class<U>)schema.request;}
        @SuppressWarnings("unchecked") public <U extends MessageDefinition> Class<U> getResponseType(){return (Class<U>)schema.response;}
        public String getServiceName(){return serviceName;}
        private Deployment.AvailabilityBinding querySelection(){
            if(selection.availability==null)throw error(95);
            if(queryOwner==null||queryOwner.handle()==0)throw error(9);
            return selection.availability;
        }
        private boolean queryOnce(long waitNs){
            liveClient();querySelection();enterDispatch();
            try{return org.uwvm.roswasm.ServiceClient.query(queryOwner,owner,waitNs);}
            finally{leaveDispatch();}
        }
        public boolean isServiceAvailable(){
            requireLocalMutable();if(availabilityWaitInProgress)throw error(17);
            return queryOnce(0);
        }
        public boolean waitForService(){return waitForService(java.time.Duration.ofNanos(-1));}
        public boolean waitForService(java.time.Duration timeout){
            requireLocalMutable();if(availabilityWaitInProgress)throw error(17);
            // Preserve actual Duration null/overflow behavior; no conversion to
            // scheduling credits and no saturation to a fabricated timeout.
            long total=timeout.toNanos();
            if(total==0)return isServiceAvailable();
            if(!active)return false;
            liveClient();Deployment.AvailabilityBinding selected=querySelection();
            if(waitClock==null||waitClock.handle()==0)throw error(13);
            availabilityWaitInProgress=true;
            try{
                long start=waitNow(),observed=start;int remaining=selected.maximumQueries;
                while(active){
                    if(Thread.currentThread().isInterrupted())return false;
                    liveClient();long now=waitNow();if(now<observed)throw error(22);observed=now;
                    long elapsed=now-start;
                    if(total>0&&elapsed>=total)return false;
                    if(remaining==0)throw error(90);
                    --remaining;
                    long slice=total<0?selected.maximumQueryWaitNs:Math.min(selected.maximumQueryWaitNs,total-elapsed);
                    boolean ready=queryOnce(slice);
                    long after=waitNow();if(after<observed)throw error(22);observed=after;
                    if(ready)return true;
                    if(total>0&&after-start>=total)return false;
                    // No owner/import/dispatch lease survives this handoff.
                    // Explicit shutdown is allowed here; recursive waits/spin
                    // are refused17 and cannot invent completion or elapsed time.
                    if(selected.checkpoint!=null)selected.checkpoint.betweenQueries();
                }
                return false;
            }finally{availabilityWaitInProgress=false;}
        }
        private long waitNow(){
            liveClient();if(waitClock==null||waitClock.handle()==0)throw error(13);
            enterDispatch();
            try{
                Result reading=waitClock.invoke("time_read_steady",EMPTY,8).checked();
                byte[] bytes=reading.data;
                if(bytes==null||bytes.length!=8||reading.value!=null||reading.required!=null)throw error(22);
                long now=new Codecs.Reader(bytes).unsigned(8);
                if(now<0)throw error(95); // explicit signed64 clock range of this facade
                return now;
            }finally{leaveDispatch();}
        }
        void retire(){
            if(disposed)return;
            retireExtra(waitClock,clockCleanup);retireExtra(queryOwner,queryCleanup);
            super.retire();
        }
        private void retireExtra(Capability cap,CleanupAttempt attempt){
            if(cap==null)return;
            if(cap.handle()==0){attempt.result=ALREADY_RETIRED;return;}
            attempt.retirement=cap.lastRetirement();
            try{attempt.result=cap.closeChecked();}catch(RuntimeException|Error failure){attempt.failure=failure;}
            finally{attempt.attempted=attempt.retirement.attempted();}
        }
        public <U extends MessageDefinition> void handleResponse(org.ros2.rcljava.service.RMWRequestId header,U response){throw error(95);}
        private void liveClient(){pairLive();if(retired)throw error(125);}
        public <U extends MessageDefinition,V extends MessageDefinition> java.util.concurrent.Future<V> asyncSendRequest(U value){return asyncSendRequest(value,null);}
        @SuppressWarnings("unchecked") public <U extends MessageDefinition,V extends MessageDefinition> java.util.concurrent.Future<V> asyncSendRequest(U value,Consumer<java.util.concurrent.Future<V>> callback){
            liveClient();if(value==null||!schema.request.isInstance(value))throw new IllegalArgumentException("complete selected request type");
            int index=-1;for(int i=0;i<pending.length;++i)if(pending[i].future==null){index=i;break;}if(index<0)throw error(90);
            byte[] wire=schema.encodeRequest(value);
            Slot slot=pending[index];PendingFuture future=new PendingFuture(this,slot);
            // Record allocation and callback/rollback storage precede admission.
            slot.future=future;slot.callback=(Consumer)callback;slot.id=0;slot.callbackCleanup.reset();
            try {
                Result admitted=owner.invoke("service_submit",wire,0,second,0).checked();
                if(admitted.value==null||admitted.value==0||admitted.data.length!=0||admitted.required!=null||Long.compareUnsigned(admitted.value,lastRequest)<=0)throw error(22);
                slot.id=admitted.value;future.requestId=admitted.value;lastRequest=admitted.value;
                return (java.util.concurrent.Future<V>)(java.util.concurrent.Future<?>)future;
            } catch(RuntimeException|Error primary) {
                // A failed/uncertain admission is never replayed. The newly
                // unreturned Future has no delivery callback. Existing requests
                // fail locally125, not with another request's native error.
                slot.future=null;slot.callback=null;retired=true;finishLocally();throw primary;
            }
        }
        int pollInside(){
            if(disposed||retired)return 11;pairLive();
            Result result=second.invoke("event_poll_v1",EMPTY,selection.capacity);
            if(result.error!=0){if(result.error!=11)result.checked();return result.error;}
            Slot target=null;MessageDefinition response=null;int kind,flags,eventError;long correlation;
            try {
                Codecs.Reader r=new Codecs.Reader(result.data);
                if(r.unsigned(4)!=0x45525755L||r.unsigned(2)!=1)throw error(22);
                kind=(int)r.unsigned(2);if(r.unsigned(1)!=2)throw error(22);
                flags=(int)r.unsigned(1);if(r.unsigned(2)!=0)throw error(22);eventError=(int)r.unsigned(4);
                long source=r.unsigned(8);correlation=r.unsigned(8);long next=r.unsigned(8),inst=r.unsigned(8),sess=r.unsigned(8),length=r.unsigned(8);
                if(source!=owner.handle()||source==0||next==0||sequence==-1L||next!=sequence+1||inst==0||sess==0||length<0||length!=r.remaining())throw error(22);
                if(bound&&(inst!=instance||sess!=nativeSession))throw error(22);
                byte[] body=r.take(r.remaining());r.end();
                if(kind==3){if(flags!=0||eventError!=0||correlation==0)throw error(22);}
                else if(kind==6||kind==7){if(body.length!=0||(flags&~3)!=0||(kind==6&&eventError==0)||(kind==7&&(flags&1)==0))throw error(22);}
                else throw error(22);
                if(kind==3||correlation!=0){for(Slot slot:pending)if(slot.future!=null&&slot.id==correlation){target=slot;break;}if(target==null)throw error(22);}
                // Full decoding precedes discarding a locally cancelled
                // tombstone, so malformed response bytes can never pass.
                if(kind==3)response=schema.decodeResponse(body);
                sequence=next;instance=inst;nativeSession=sess;bound=true;
            } catch(RuntimeException|Error primary){retired=true;failAll(primary);throw primary;}
            if(kind==7||(kind==6&&(correlation==0||(flags&1)!=0))){
                retired=true;
                Throwable first=null;
                for(Slot slot:pending)if(slot.future!=null)try{complete(slot,null,eventError==0?125:eventError,flags,"native-source-retirement");}
                catch(RuntimeException|Error failure){slot.callbackCleanup.failure=failure;if(first==null)first=failure;}
                if(first instanceof RuntimeException)throw (RuntimeException)first;
                if(first instanceof Error)throw (Error)first;
            } else complete(target,response,kind==3?0:eventError,flags,"native-event");
            return 0;
        }
        void complete(Slot slot,MessageDefinition value,int code,int flags,String origin){
            PendingFuture future=slot.future;Consumer callback=slot.callback;slot.future=null;slot.callback=null;
            if(future.done)return;future.outcome.code=code;future.outcome.flags=flags;future.outcome.requestId=future.requestId;future.outcome.origin=origin;
            future.value=value;future.done=true;
            if(callback!=null)callback.accept(future);
        }
        void failAll(Throwable primary){
            // Preallocated fixed iteration continues despite an earlier user
            // callback exception; no consumed event or request is replayed.
            for(Slot slot:pending)if(slot.future!=null){
                PendingFuture future=slot.future;Consumer callback=slot.callback;slot.future=null;slot.callback=null;
                if(future.done)continue;future.failure=primary;future.done=true;
                if(callback!=null)try{callback.accept(future);}catch(RuntimeException|Error failure){slot.callbackCleanup.failure=failure;}
            }
        }
        void finishLocally(){
            boolean previous=dispatching;dispatching=true;
            try{for(Slot slot:pending)if(slot.future!=null)try{complete(slot,null,125,0,"local-resource-retirement");}catch(RuntimeException|Error failure){slot.callbackCleanup.failure=failure;}}
            finally{dispatching=previous;}
        }
        void onRetired(){retired=true;finishLocally();}
    }
    private final class Slot {
        long id;PendingFuture future;Consumer callback;
        final CleanupAttempt callbackCleanup=new CleanupAttempt();
    }
    /** A truly deferred standard Future. Its result comes only from a complete
     * matching native event. Finite Guest progress is explicit, never a sync
     * service_call, graph query, thread, implicit clock or invented completion.
     */
    private final class PendingFuture implements java.util.concurrent.Future<MessageDefinition> {
        final ClientImpl<?> client;final Slot slot;final ServiceFailure outcome=new ServiceFailure();
        long requestId;boolean done,cancelled;MessageDefinition value;Throwable failure;
        PendingFuture(ClientImpl<?> client,Slot slot){this.client=client;this.slot=slot;}
        public boolean isDone(){return done;}
        public boolean isCancelled(){return cancelled;}
        public boolean cancel(boolean mayInterruptIfRunning){
            requireMutable();if(done)return false;cancelled=true;done=true;outcome.code=125;outcome.origin="local-cancel";outcome.requestId=requestId;
            // The actual inflight slot/ticket remains a tombstone until drained.
            // No native cancellation or thread interruption is implied.
            return true;
        }
        private MessageDefinition result() throws java.util.concurrent.ExecutionException {
            if(cancelled)throw new java.util.concurrent.CancellationException("local Future cancellation only");
            if(failure!=null)throw new java.util.concurrent.ExecutionException(failure);
            if(outcome.code!=0)throw new java.util.concurrent.ExecutionException(outcome);
            return value;
        }
        public MessageDefinition get() throws InterruptedException,java.util.concurrent.ExecutionException {
            // A terminal result is read-only and never enters Guest progress.
            // Only a pending wait needs the Context mutation/dispatch lease.
            if(done)return result();
            requireLocalMutable();
            while(!done){if(!futurePass(client.node)&&!done)throw new InterruptedException("finite deployment progress exhausted; request not replayed/cancelled");}
            return result();
        }
        public MessageDefinition get(long timeout,TimeUnit unit) throws InterruptedException,java.util.concurrent.ExecutionException,java.util.concurrent.TimeoutException {
            if(done){if(unit==null)throw new NullPointerException("unit");return result();}
            requireLocalMutable();
            if(unit==null)throw new NullPointerException("unit");
            if(timeout<=0)throw new java.util.concurrent.TimeoutException("not ready");
            // Step credits are not nanoseconds. A real granted Clock or bounded
            // native wait extension is required for this exact overload.
            throw error(95);
        }
    }
    private boolean futurePass(NodeImpl node){
        if(availabilityWaitInProgress)throw error(17);
        requireMutable();node.live();if(node.executor!=null)throw error(17);
        if(!hasDriver())throw error(95);if(remainingCredits==0)return false;
        // Lease before the explicit driver callback too. A driver cannot
        // recursively wait/spin or mutate any owner's lifecycle mid-handoff.
        enterDispatch();
        try{--remainingCredits;boolean credit=profile.driver.nextCredit();if(!active||!credit)return false;node.pollInside();return true;}
        finally{leaveDispatch();}
    }
    private Deployment.ServiceBinding serviceBinding(NodeImpl node,String role,String resource,String type,String qos){
        for(Deployment.ServiceBinding b:profile.services)if(b.node.equals(node.fqn)&&b.role.equals(role)&&b.resource.equals(resource)&&b.type.equals(type)&&b.qos.equals(qos))return b;
        throw error(13);
    }

    private static long add(long a,long b) { if(b<0||a>Long.MAX_VALUE-b)throw new IllegalArgumentException("timer range");return a+b; }

    /** Local node organizer with fixed storage, not a fabricated native node. */
    private final class NodeImpl implements Node {
        final String name,namespace,fqn;
        final Resource[] owned;
        final CleanupReport cleanup;
        final CleanupFailure cleanupFailure;
        int position;
        Object executor;
        boolean disposed;
        NodeImpl(String name,String namespace) {
            this.name=name;this.namespace=namespace;fqn=(namespace.equals("/")?"":namespace)+"/"+name;
            owned=new Resource[profile.maximumNodeResources];cleanup=new CleanupReport(owned.length);cleanupFailure=new CleanupFailure(cleanup);
        }
        void live() { requireLive();if(disposed)throw error(125); }
        String resolve(String topic) {
            if(topic==null)throw new IllegalArgumentException("topic");
            String value=topic.startsWith("~/")?fqn+topic.substring(1):topic.startsWith("/")?topic:(namespace.equals("/")?"":namespace)+"/"+topic;
            if(!value.matches("/[A-Za-z_][A-Za-z_0-9]*(?:/[A-Za-z_][A-Za-z_0-9]*)*"))throw new IllegalArgumentException("topic name");
            return value;
        }
        public <T extends MessageDefinition> Publisher<T> createPublisher(Class<T> type,String topic) { return createPublisher(type,topic,QoSProfile.DEFAULT); }
        public <T extends MessageDefinition> Publisher<T> createPublisher(Class<T> type,String topic,QoSProfile qos) {
            requireMutable();live();Codec<T> c=codec(type);
            Deployment.Binding b=binding(this,"topic_publish",resolve(topic),"std_msgs/msg/String",qosKey(qos));
            Pub<T> resource=new Pub<T>(this,c);retain(resource);
            try { resource.owner=session.open(b.alias);return resource; }
            catch(RuntimeException|Error primary) { failedFactory(resource,primary);throw primary; }
        }
        public <T extends MessageDefinition> Subscription<T> createSubscription(Class<T> type,String topic,Consumer<T> callback) { return createSubscription(type,topic,callback,QoSProfile.DEFAULT); }
        public <T extends MessageDefinition> Subscription<T> createSubscription(Class<T> type,String topic,Consumer<T> callback,QoSProfile qos) {
            requireMutable();live();if(callback==null)throw new IllegalArgumentException("callback");Codec<T> c=codec(type);
            Deployment.Binding b=binding(this,"topic_subscribe",resolve(topic),"std_msgs/msg/String",qosKey(qos));
            Sub<T> resource=new Sub<T>(this,type,c,callback,b.capacity);retain(resource);
            try { resource.owner=session.open(b.alias);resource.owner.invoke("topic_subscribe",new byte[0],0).checked();return resource; }
            catch(RuntimeException|Error primary) { failedFactory(resource,primary);throw primary; }
        }
        public WallTimer createWallTimer(long period,TimeUnit unit,Callback callback) {
            requireMutable();live();if(period<=0||unit==null||callback==null)throw new IllegalArgumentException("timer arguments");
            // TimeUnit saturates overflow. Reject it before any clock capability.
            long scale=unit.toNanos(1);if(scale<=0||period>Long.MAX_VALUE/scale)throw new IllegalArgumentException("timer nanosecond range");
            long ns=period*scale;
            Deployment.Binding b=binding(this,"time_read_steady",fqn,"","");
            Wall resource=new Wall(this,ns,callback);retain(resource);
            try { resource.owner=session.open(b.alias);resource.start();return resource; }
            catch(RuntimeException|Error primary) { failedFactory(resource,primary);throw primary; }
        }
        public <T extends org.ros2.rcljava.interfaces.ServiceDefinition> org.ros2.rcljava.service.Service<T> createService(Class<T> type,String name,org.ros2.rcljava.consumers.TriConsumer<org.ros2.rcljava.service.RMWRequestId,? extends MessageDefinition,? extends MessageDefinition> callback){return createService(type,name,callback,QoSProfile.SERVICES_DEFAULT);}
        public <T extends org.ros2.rcljava.interfaces.ServiceDefinition> org.ros2.rcljava.service.Service<T> createService(Class<T> type,String name,org.ros2.rcljava.consumers.TriConsumer<org.ros2.rcljava.service.RMWRequestId,? extends MessageDefinition,? extends MessageDefinition> callback,QoSProfile qos){
            requireMutable();live();if(callback==null)throw new IllegalArgumentException("callback");ServiceTypes.Descriptor schema=ServiceTypes.find(type);
            Deployment.ServiceBinding b=serviceBinding(this,"server",resolve(name),schema.name,qosKey(qos));
            Server<T> resource=new Server<T>(this,schema,b,name,callback);retain(resource);
            try{resource.owner=session.open(b.firstAlias);resource.second=session.open(b.secondAlias);if(resource.owner.handle()==resource.second.handle())throw error(22);resource.owner.invoke("service_advertise",EMPTY,0).checked();return resource;}
            catch(RuntimeException|Error primary){failedFactory(resource,primary);throw primary;}
        }
        public <T extends org.ros2.rcljava.interfaces.ServiceDefinition> org.ros2.rcljava.client.Client<T> createClient(Class<T> type,String name){return createClient(type,name,QoSProfile.SERVICES_DEFAULT);}
        public <T extends org.ros2.rcljava.interfaces.ServiceDefinition> org.ros2.rcljava.client.Client<T> createClient(Class<T> type,String name,QoSProfile qos){
            requireMutable();live();ServiceTypes.Descriptor schema=ServiceTypes.find(type);
            Deployment.ServiceBinding b=serviceBinding(this,"client",resolve(name),schema.name,qosKey(qos));
            ClientImpl<T> resource=new ClientImpl<T>(this,schema,b,name);retain(resource);
            if(b.availability==null){
                // Whole original two-owner path; no optional import or grant.
                try{resource.owner=session.open(b.firstAlias);resource.second=session.open(b.secondAlias);if(resource.owner.handle()==resource.second.handle())throw error(22);resource.second.invoke("event_watch",EMPTY,0,resource.owner,0).checked();return resource;}
                catch(RuntimeException|Error primary){failedFactory(resource,primary);throw primary;}
            }
            enterDispatch();
            try{
                Deployment.AvailabilityBinding a=b.availability;
                session.preflightOwners(a.steadyClockAlias==null?3:4);
                org.uwvm.roswasm.ServiceClient.checkAbi(profile.transport);
                resource.owner=session.open(b.firstAlias);resource.second=session.open(b.secondAlias);
                resource.queryOwner=session.open(a.queryAlias);
                if(a.steadyClockAlias!=null)resource.waitClock=session.open(a.steadyClockAlias);
                // Full handles are opaque; do not narrow or reject high bits.
                if(resource.owner.handle()==resource.second.handle()||resource.owner.handle()==resource.queryOwner.handle()||resource.second.handle()==resource.queryOwner.handle()||resource.waitClock!=null&&(resource.waitClock.handle()==resource.owner.handle()||resource.waitClock.handle()==resource.second.handle()||resource.waitClock.handle()==resource.queryOwner.handle()))throw error(22);
                if(org.uwvm.roswasm.ServiceClient.features(resource.queryOwner,resource.owner)!=7)throw error(95);
                resource.second.invoke("event_watch",EMPTY,0,resource.owner,0).checked();return resource;
            }catch(RuntimeException|Error primary){failedFactory(resource,primary);throw primary;}
            finally{leaveDispatch();}
        }
        public Collection<org.ros2.rcljava.service.Service> getServices(){ArrayList<org.ros2.rcljava.service.Service> out=new ArrayList<org.ros2.rcljava.service.Service>();for(Resource r:owned)if(r instanceof Server&&!r.disposed)out.add((org.ros2.rcljava.service.Service)r);return Collections.unmodifiableList(out);}
        public Collection<org.ros2.rcljava.client.Client> getClients(){ArrayList<org.ros2.rcljava.client.Client> out=new ArrayList<org.ros2.rcljava.client.Client>();for(Resource r:owned)if(r instanceof ClientImpl&&!r.disposed)out.add((org.ros2.rcljava.client.Client)r);return Collections.unmodifiableList(out);}
        public boolean removeService(org.ros2.rcljava.service.Service value){return remove(value,Server.class);}
        public boolean removeClient(org.ros2.rcljava.client.Client value){return remove(value,ClientImpl.class);}
        public String getName() { return name; }
        public String getNamespace() { return namespace; }
        public long getHandle() { throw nativePointer(); }
        public Collection<Publisher> getPublishers() { ArrayList<Publisher> out=new ArrayList<Publisher>();for(Resource r:owned)if(r instanceof Pub&&!r.disposed)out.add((Publisher)r);return Collections.unmodifiableList(out); }
        public Collection<Subscription> getSubscriptions() { ArrayList<Subscription> out=new ArrayList<Subscription>();for(Resource r:owned)if(r instanceof Sub&&!r.disposed)out.add((Subscription)r);return Collections.unmodifiableList(out); }
        public Collection<Timer> getTimers() { ArrayList<Timer> out=new ArrayList<Timer>();for(Resource r:owned)if(r instanceof Wall&&!r.disposed)out.add((Timer)r);return Collections.unmodifiableList(out); }
        private boolean remove(Disposable value,Class<?> kind) {
            requireMutable();live();
            for(int i=0;i<owned.length;++i)if(owned[i]==value&&kind.isInstance(value)) {
                Resource r=owned[i];if(r.disposed)return false;
                r.dispose();owned[i]=null;return true;
            }
            return false;
        }
        public boolean removePublisher(Publisher value) { return remove(value,Publisher.class); }
        public boolean removeSubscription(Subscription value) { return remove(value,Subscription.class); }
        public void dispose() {
            requireMutable();if(disposed)return;
            cleanup.reset();disposed=true;
            for(int i=owned.length-1;i>=0;--i)if(owned[i]!=null) { owned[i].retire();cleanup.attempts[i]=owned[i].cleanup; }
            cleanup.completed=true;
            if(cleanup.failed())throw cleanupFailure;
        }
        int pollInside() {
            for(int count=0;count<owned.length;++count) {
                Resource r=owned[position];position=(position+1)%owned.length;
                if(r==null||r.disposed||r instanceof Pub)continue;
                int result=r.pollInside();if(result!=11)return result;
            }
            return 11;
        }
    }
}
