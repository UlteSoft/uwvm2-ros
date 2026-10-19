package org.uwvm.roswasm;

import java.util.Arrays;

/** Finite single-Guest-thread retained ROS2 clients/services. Constructor opens
 * nothing. Every alias is supplied by the caller and opened by its real Session.
 * Native type/QoS checks remain Host/Gateway policy, not Guest authority.
 */
public final class OwnedService {
    private OwnedService(){}
    static final int ABI=0,ACTIVATE=1,INSPECT=2,SUBMIT=3,ABANDON=4,RESPOND=5,PEEK=6,COMMIT=7,CANCEL=8,AVAILABLE=9,RETIRE=10;
    private static final int RESERVED=1,ADMITTED=2,DELIVERED=3,DONE=4,ABANDONED=5;
    /** Storage and finite work limits; no field is a permission or native quota. */
    public static final class Options {
        public final int endpoints,transactions,maximumRequest,maximumResponse;
        public final long maximumWaitNs,storageBudgetBytes;
        public Options(int endpoints,int transactions,int maximumRequest,int maximumResponse,long maximumWaitNs,long storageBudgetBytes){
            if(endpoints<1||endpoints>256||transactions<1||transactions>256||maximumRequest<0||maximumRequest>OwnedServiceWire.INPUT_MAXIMUM
                ||maximumResponse<0||maximumResponse>OwnedServiceWire.INPUT_MAXIMUM||maximumWaitNs<0||maximumWaitNs>60000000000L||storageBudgetBytes<1||storageBudgetBytes>64L*1024*1024)
                throw new IllegalArgumentException("finite owned-service options");
            long per=2L*Math.max(maximumRequest,maximumResponse)+352+256*2+8+1+transactions*64L+4096;
            if(endpoints*per>storageBudgetBytes)throw new IllegalArgumentException("payload/state allowance exceeded (not exact JVM heap/RSS)");
            this.endpoints=endpoints;this.transactions=transactions;this.maximumRequest=maximumRequest;this.maximumResponse=maximumResponse;
            this.maximumWaitNs=maximumWaitNs;this.storageBudgetBytes=storageBudgetBytes;
        }
    }
    /** Exact actual native QoS pair required for admission, not reconfiguration.
     * Times/depth keep all unsigned64 bits, including high-bit and saturated sec.
     */
    public static final class Qos {
        private final byte[] bytes=new byte[80];
        private Qos(byte[] source){System.arraycopy(source,0,bytes,0,80);}
        /** Exact80-byte caller assertion, not native discovery/authority. */
        public static Qos fromBytes(byte[] source){if(source==null||source.length!=80||!OwnedServiceWire.qos(source,0))throw new IllegalArgumentException("QoS80");return new Qos(source);}
        public Qos(int history,int reliability,int durability,int liveliness,long depth,long deadlineSec,long deadlineNs,long lifespanSec,long lifespanNs,long leaseSec,long leaseNs,boolean avoidNamespaces){
            OwnedServiceWire.put(bytes,0,history,4);OwnedServiceWire.put(bytes,4,reliability,4);OwnedServiceWire.put(bytes,8,durability,4);OwnedServiceWire.put(bytes,12,liveliness,4);
            OwnedServiceWire.put(bytes,16,depth,8);OwnedServiceWire.put(bytes,24,deadlineSec,8);OwnedServiceWire.put(bytes,32,deadlineNs,8);
            OwnedServiceWire.put(bytes,40,lifespanSec,8);OwnedServiceWire.put(bytes,48,lifespanNs,8);OwnedServiceWire.put(bytes,56,leaseSec,8);OwnedServiceWire.put(bytes,64,leaseNs,8);bytes[72]=(byte)(avoidNamespaces?1:0);
            if(!OwnedServiceWire.qos(bytes,0))throw new IllegalArgumentException("actual normalized QoS values required");
        }
    }
    /** Full canonical records only. Root native identity may be absent during
     * preparation; it is never guessed from the canonical codec identity.
     * Encode/decode run under the group lease and must not perform Host effects.
     */
    public static final class Definition<Q,S> {
        public final String nativeType,nativeIdentity,codecIdentity;
        public final Codec<Q> request;public final Codec<S> response;
        public final int requestMaximum,responseMaximum;
        public Definition(String nativeType,String nativeIdentity,String codecIdentity,Codec<Q> request,int requestMaximum,Codec<S> response,int responseMaximum){
            if(nativeType==null||!nativeType.matches("[a-z][a-z0-9_]*/srv/[A-Za-z][A-Za-z0-9_]*")||codecIdentity==null||!codecIdentity.matches("UWVMROS1_[0-9a-f]{64}")
                ||nativeIdentity!=null&&!nativeIdentity.matches("RIHS01_[0-9a-f]{64}")||request==null||response==null||requestMaximum<0||responseMaximum<0
                ||requestMaximum>OwnedServiceWire.INPUT_MAXIMUM||responseMaximum>OwnedServiceWire.INPUT_MAXIMUM)throw new IllegalArgumentException("complete service definition");
            this.nativeType=nativeType;this.nativeIdentity=nativeIdentity;this.codecIdentity=codecIdentity;this.request=request;this.response=response;this.requestMaximum=requestMaximum;this.responseMaximum=responseMaximum;
        }
    }
    /** Reserved before effects. attempted/returned observe the SDK boundary;
     * native bridge115+failure is conservative effect-unknown, never replayed.
     * rawReturned/rawCode/preparationFailure are separate original business
     * close/revoke RetirementAttempt facts. deliveryCleanup* records a failed
     * capability_open result delivery's own original transport cleanup.
     */
    public static final class Outcome {
        public boolean attempted,returned;public int code;public Throwable failure;
        public boolean rawReturned,deliveryCleanupAttempted;public int rawCode,deliveryCleanupCode;
        public Throwable preparationFailure,deliveryCleanupPrimary,deliveryCleanupFailure;
    }
    public static final class Diagnostic {
        public int primary,activationValidation,inspectionValidation;public Throwable primaryFailure;
        public boolean poisoned,originalPresent,inspectionPresent;
        public final Outcome[] open=outcomes(3),close=outcomes(3),revoke=outcomes(3);
        public final Outcome[] operations=outcomes(11);
        public final byte[] original=new byte[256],inspection=new byte[256],lastPacketPrefix=new byte[352];
        public long lastTicket,lastCommittedSequence,lastReservation;public int transactionSlots;
        private static Outcome[] outcomes(int n){Outcome[] a=new Outcome[n];for(int i=0;i<n;i++)a[i]=new Outcome();return a;}
        void copyTo(Diagnostic d){d.primaryFailure=primaryFailure;d.primary=primary;d.activationValidation=activationValidation;d.inspectionValidation=inspectionValidation;d.poisoned=poisoned;d.originalPresent=originalPresent;d.inspectionPresent=inspectionPresent;
            copy(open,d.open);copy(close,d.close);copy(revoke,d.revoke);copy(operations,d.operations);System.arraycopy(original,0,d.original,0,256);System.arraycopy(inspection,0,d.inspection,0,256);System.arraycopy(lastPacketPrefix,0,d.lastPacketPrefix,0,352);
            d.lastTicket=lastTicket;d.lastCommittedSequence=lastCommittedSequence;d.lastReservation=lastReservation;d.transactionSlots=transactionSlots;}
        private static void copy(Outcome[] a,Outcome[] b){for(int i=0;i<a.length;i++){b[i].attempted=a[i].attempted;b[i].returned=a[i].returned;b[i].code=a[i].code;b[i].failure=a[i].failure;b[i].rawReturned=a[i].rawReturned;b[i].rawCode=a[i].rawCode;b[i].preparationFailure=a[i].preparationFailure;b[i].deliveryCleanupAttempted=a[i].deliveryCleanupAttempted;b[i].deliveryCleanupCode=a[i].deliveryCleanupCode;b[i].deliveryCleanupPrimary=a[i].deliveryCleanupPrimary;b[i].deliveryCleanupFailure=a[i].deliveryCleanupFailure;}}
    }
    public static final class Opening<T>{public int code;public Throwable failure;public T value;public final Diagnostic diagnostic;Opening(){this.diagnostic=new Diagnostic();}}
    public static final class Submission {public int code;public long ticket;public Throwable failure;}
    /** An owning result prepared before peek. Metadata belongs to the same
     * complete take. GID32 is retained in diagnostics separately from GUID16.
     * No value is published for an abandoned response, but it is fully decoded.
     */
    public static final class Delivery<T> {
        public int code,kind,controlFlags,nativeError;public boolean hasValue,abandoned;
        public T value;public Throwable failure;
        public long ticket,sequence,reservation,sourceTimestamp,receivedTimestamp,nativeSequence;
        public final byte[] writerGuid=new byte[16];
    }
    public static final class Availability {public int code;public boolean available;public Throwable failure;}
    static final class Transaction {
        long ticket,nativeSequence;int state;boolean nativePresent;final byte[] writerGuid=new byte[16];
    }
    static final class Endpoint {
        final Group group;final Capability[] owners=new Capability[3];final String[] aliases=new String[3];final boolean[] leased=new boolean[3];
        final Diagnostic diagnostic=new Diagnostic();final OwnedServiceWire.Receipt receipt=new OwnedServiceWire.Receipt(),inspectionReceipt=new OwnedServiceWire.Receipt();
        final byte[] original=new byte[256],inspection=new byte[256],ticket=new byte[8],available=new byte[1],input,packet;
        final Transaction[] history;final OwnedServiceWire.Record record=new OwnedServiceWire.Record();
        int role,ownerCount,requestMaximum,responseMaximum,packetCapacity,used;boolean associated,closed;
        Endpoint(Group group){this.group=group;int m=Math.max(group.options.maximumRequest,group.options.maximumResponse);input=new byte[m];packet=new byte[352+m];history=new Transaction[group.options.transactions];for(int i=0;i<history.length;i++)history[i]=new Transaction();}
        long source(){return owners[0].handle();}long peer(){return owners[1].handle();}
        int primary(int n){if(n!=0&&diagnostic.primary==0)diagnostic.primary=n;return n;}
        int call(int op,long a,long b,int size){Outcome o=diagnostic.operations[op];o.attempted=true;o.returned=false;o.code=115;o.failure=null;
            try{int n=group.nativeCalls.call(op,this,a,b,size);o.code=n;o.returned=true;return n;}
            catch(RuntimeException|Error failure){o.failure=failure;if(diagnostic.primaryFailure==null)diagnostic.primaryFailure=failure;primary(115);return 115;}}
        void retire(){if(!associated||diagnostic.operations[RETIRE].attempted)return;call(RETIRE,0,0,0);}
        int terminal(int n){primary(n);diagnostic.poisoned=true;retire();return n;}
        int live(){if(!group.session.active()||closed||diagnostic.operations[RETIRE].attempted)return 125;for(int i=0;i<ownerCount;i++)if(owners[i]==null||!OwnedServiceWire.full(owners[i].handle()))return 9;if(diagnostic.poisoned)return diagnostic.primary==0?125:diagnostic.primary;return associated?0:9;}
        void cleanup(boolean revoke){if(closed)return;closed=true;retire();
            for(int i=ownerCount-1;i>=0;i--){Capability owner=owners[i];if(owner==null)continue;
                if(leased[i]){owner.release();leased[i]=false;}
                if(revoke&&owner.handle()!=0){Outcome o=diagnostic.revoke[i];o.code=115;RetirementAttempt a=group.session.lastRevokeAttempt();long before=a.number();
                    try{Result r=owner.revokeChecked();o.code=r.error;o.returned=true;}catch(RuntimeException|Error failure){o.failure=failure;}
                    if(a.number()!=before){o.attempted=a.attempted();o.rawReturned=a.returned();o.rawCode=a.code();o.preparationFailure=a.firstPreparationFailure();}}
                if(owner.handle()==0)continue;Outcome o=diagnostic.close[i];o.code=115;
                try{Result r=owner.closeChecked();o.code=r.error;o.returned=true;}catch(RuntimeException|Error failure){o.failure=failure;}
                RetirementAttempt a=owner.lastRetirement();o.attempted=a.attempted();o.rawReturned=a.returned();o.rawCode=a.code();o.preparationFailure=a.firstPreparationFailure();
                // Known-not-attempted still-live owners stay in the actual
                // Session ledger; this facade never silently discards/retries them.
            }
        }
        int inspect(){int n=live();if(n!=0)return n;Arrays.fill(inspection,(byte)0xa5);n=call(INSPECT,0,0,0);if(n!=0)return terminal(n);
            diagnostic.inspectionPresent=true;System.arraycopy(inspection,0,diagnostic.inspection,0,256);
            n=OwnedServiceWire.receipt(inspection,2,role,source(),inspectionReceipt);
            if(n==0)for(int i=0;i<256;i++)if(i!=6&&inspection[i]!=original[i]){n=13;break;}
            diagnostic.inspectionValidation=n;return n==0?0:terminal(n);}
        Transaction find(long t){if(t==0)return null;for(int i=0;i<used;i++)if(history[i].ticket==t)return history[i];return null;}
        int cancel(int primary){primary(primary);int n=call(CANCEL,record.sequence,record.reservation,0);if(n!=0)terminal(primary);return primary;}
        int commit(){int n=call(COMMIT,record.sequence,record.reservation,0);if(n!=0)return terminal(n);diagnostic.lastCommittedSequence=record.sequence;return 0;}
        int borrow(){int n=inspect();if(n!=0)return n;if(role==4&&used==history.length)return 90;
            Arrays.fill(packet,0,packetCapacity,(byte)0xa5);n=call(PEEK,0,0,0);if(n!=0){if(n!=11)primary(n);if(n!=11&&n!=90&&n!=95)terminal(n);return n;}
            System.arraycopy(packet,0,diagnostic.lastPacketPrefix,0,352);
            n=OwnedServiceWire.record(packet,packetCapacity,receipt,peer(),role==3?responseMaximum:requestMaximum,record);
            if(!record.token)return terminal(n);
            if(Long.compareUnsigned(record.sequence,diagnostic.lastCommittedSequence)<=0||Long.compareUnsigned(record.reservation,diagnostic.lastReservation)<=0
                ||role==3&&(diagnostic.lastCommittedSequence==-1L||record.sequence!=diagnostic.lastCommittedSequence+1))return terminal(13);
            diagnostic.lastReservation=record.reservation;
            if(n!=0){cancel(n);if(n!=90&&n!=95)terminal(n);return n;}
            Transaction t=find(record.ticket);
            if(role==3&&record.ticket!=0&&(t==null||t.state!=ADMITTED&&t.state!=ABANDONED)){cancel(13);return terminal(13);}
            if(role==4&&record.kind==2){
                if(t!=null){cancel(13);return terminal(13);}
                for(int i=0;i<used;i++){Transaction old=history[i];if(old.nativePresent&&old.nativeSequence==record.nativeSequence&&OwnedServiceWire.same(old.writerGuid,0,record.writerGuid,0,16)){cancel(13);return terminal(13);}}
            }
            return 0;
        }
    }
    interface NativeCalls {int call(int operation,Endpoint e,long a,long b,int size);}
    private static final NativeCalls REAL=new NativeCalls(){public int call(int operation,Endpoint e,long a,long b,int size){return Ros2OwnedServiceTransport.call(operation,e,a,b,size);}};
    public static final class Group implements AutoCloseable {
        final Session session;final Options options;final Endpoint[] entries;final NativeCalls nativeCalls;
        int used,cleanupCode;boolean busy,closed,scopeSet;long instance,generation,nativeSession;
        public Group(Session session,Options options){this(session,options,REAL,false);}
        // Package-private local model seam. It cannot manufacture Host identity;
        // production construction accepts only the concrete numeric transport.
        Group(Session session,Options options,NativeCalls calls,boolean localModel){
            if(session==null||options==null||calls==null||session.transport.backend()!=2||!localModel&&!(session.transport instanceof Ros2OwnedServiceTransport))throw new IllegalArgumentException("explicit genuine backend2 transport required");
            this.session=session;this.options=options;nativeCalls=calls;entries=new Endpoint[options.endpoints];for(int i=0;i<entries.length;i++)entries[i]=new Endpoint(this);
        }
        int enter(){return enter(false);}
        int enter(boolean cleanup){if(busy)return 17;if(closed||!cleanup&&!session.active())return 125;if(session.retiring())return 17;busy=true;
            for(int i=0;i<used;i++){Endpoint e=entries[i];for(int j=0;j<e.ownerCount;j++){Capability o=e.owners[j];if(o!=null&&o.handle()!=0){if(!o.acquire()){leave();return 17;}e.leased[j]=true;}}}return 0;}
        void leave(){for(int i=0;i<used;i++){Endpoint e=entries[i];for(int j=0;j<e.ownerCount;j++)if(e.leased[j]){e.owners[j].release();e.leased[j]=false;}}busy=false;}
        /** Detached snapshot; caller mutation cannot change any lifecycle decision,
         * sequence, once-only flag or cleanup outcome in the live endpoint. */
        public Diagnostic diagnostic(int row){if(row<0||row>=used)throw new IllegalArgumentException("row");Diagnostic result=new Diagnostic();entries[row].diagnostic.copyTo(result);return result;}
        public int rows(){return used;}
        int open(Endpoint e,int role,String first,String second,String query,Qos q0,Qos q1,int requestMaximum,int responseMaximum){
            e.role=role;e.ownerCount=query==null?2:3;e.aliases[0]=first;e.aliases[1]=second;e.aliases[2]=query;e.requestMaximum=requestMaximum;e.responseMaximum=responseMaximum;e.packetCapacity=352+(role==3?responseMaximum:requestMaximum);
            if(q0==null||q1==null||role==4&&query!=null||requestMaximum>options.maximumRequest||responseMaximum>options.maximumResponse)return 22;
            for(int i=0;i<e.ownerCount;i++){String alias=e.aliases[i];if(alias==null)return 22;byte[] a=Codecs.utf8(alias);if(a.length==0||a.length>255||alias.indexOf('\0')>=0)return 22;
                for(int k=0;k<used;k++){Endpoint old=entries[k];for(int j=0;j<old.ownerCount;j++)if(old!=e&&alias.equals(old.aliases[j])||old==e&&j<i&&alias.equals(old.aliases[j]))return 17;}}
            session.preflightOwners(e.ownerCount);session.checkAbi();if(e.call(ABI,0,0,0)!=1)return 95;
            for(int i=0;i<e.ownerCount;i++){Outcome o=e.diagnostic.open[i];o.attempted=true;o.code=115;
                try{Capability owner=session.open(e.aliases[i]);e.owners[i]=owner;o.code=0;o.returned=true;if(!OwnedServiceWire.full(owner.handle()))return 22;
                    for(int j=0;j<i;j++)if(e.owners[j].handle()==owner.handle())return 13;
                    if(!owner.acquire())return 17;e.leased[i]=true;
                }catch(RuntimeException|Error failure){o.failure=failure;if(e.diagnostic.primaryFailure==null)e.diagnostic.primaryFailure=failure;
                    if(session.transport instanceof Ros2OwnedServiceTransport){OpenCleanup c=((Ros2OwnedServiceTransport)session.transport).lastOpenCleanup();o.deliveryCleanupAttempted=c.attempted();o.deliveryCleanupCode=c.code();o.deliveryCleanupPrimary=c.primary();o.deliveryCleanupFailure=c.failure();}
                    return code(failure);}}
            Arrays.fill(e.original,(byte)0xa5);int n=e.call(ACTIVATE,0,0,0);if(n!=0)return n;e.diagnostic.originalPresent=true;System.arraycopy(e.original,0,e.diagnostic.original,0,256);
            n=OwnedServiceWire.receipt(e.original,1,role,e.source(),e.receipt);e.associated=e.receipt.bindingValid;
            if(n==0&&(!OwnedServiceWire.same(e.receipt.qos,0,q0.bytes,0,80)||!OwnedServiceWire.same(e.receipt.qos,80,q1.bytes,0,80)))n=13;
            if(n==0&&scopeSet&&(instance!=e.receipt.instance||generation!=e.receipt.generation||nativeSession!=e.receipt.session))n=13;
            e.diagnostic.activationValidation=n;if(n!=0)return n;
            if(!scopeSet){instance=e.receipt.instance;generation=e.receipt.generation;nativeSession=e.receipt.session;scopeSet=true;}return 0;
        }
        public <Q,S> Opening<Client<Q,S>> client(Definition<Q,S> definition,String call,String observer,String availability,Qos request,Qos response){
            if(definition==null)throw new IllegalArgumentException("definition");Endpoint e=used<entries.length?entries[used]:null;
            Opening<Client<Q,S>> result=new Opening<>();if(busy){result.code=17;return result;}if(e==null){result.code=90;return result;}
            Client<Q,S> wrapper=new Client<>(e,definition);int n=enter();if(n!=0){result.code=n;return result;}
            try{used++;try{n=open(e,3,call,observer,availability,request,response,definition.requestMaximum,definition.responseMaximum);}catch(RuntimeException|Error failure){result.failure=failure;if(e.diagnostic.primaryFailure==null)e.diagnostic.primaryFailure=failure;n=code(failure);}
                if(n!=0){e.primary(n);e.cleanup(false);result.code=n;}else result.value=wrapper;e.diagnostic.copyTo(result.diagnostic);return result;
            }finally{leave();}
        }
        public <Q,S> Opening<Server<Q,S>> server(Definition<Q,S> definition,String advertise,String responder,Qos request,Qos response){
            if(definition==null)throw new IllegalArgumentException("definition");Endpoint e=used<entries.length?entries[used]:null;Opening<Server<Q,S>> result=new Opening<>();if(busy){result.code=17;return result;}
            if(e==null){result.code=90;return result;}Server<Q,S> wrapper=new Server<>(e,definition);int n=enter();if(n!=0){result.code=n;return result;}
            try{used++;try{n=open(e,4,advertise,responder,null,request,response,definition.requestMaximum,definition.responseMaximum);}catch(RuntimeException|Error failure){result.failure=failure;if(e.diagnostic.primaryFailure==null)e.diagnostic.primaryFailure=failure;n=code(failure);}
                if(n!=0){e.primary(n);e.cleanup(false);result.code=n;}else result.value=wrapper;e.diagnostic.copyTo(result.diagnostic);return result;
            }finally{leave();}
        }
        public int closeChecked(){if(busy)return 17;if(closed)return cleanupCode;int n=enter(true);if(n!=0)return n;
            try{closed=true;for(int i=used-1;i>=0;i--){Endpoint e=entries[i];e.cleanup(false);int c=cleanupCode(e);if(cleanupCode==0)cleanupCode=c;}return cleanupCode;}finally{leave();}}
        public void close(){int n=closeChecked();if(n!=0)throw new RosException(new Result(n));}
    }
    static int code(Throwable t){return t instanceof RosException?((RosException)t).result.error:115;}
    static int cleanupCode(Endpoint e){Outcome retire=e.diagnostic.operations[RETIRE];if(retire.attempted&&retire.code!=0)return retire.code;
        for(int i=e.ownerCount-1;i>=0;i--){Outcome r=e.diagnostic.revoke[i],c=e.diagnostic.close[i];if(r.code!=0)return r.code;if(c.code!=0)return c.code;}return 0;}
    static void metadata(OwnedServiceWire.Record d,Delivery<?> result){result.kind=d.kind;result.controlFlags=d.controlFlags;result.nativeError=d.error;result.ticket=d.ticket;result.sequence=d.sequence;result.reservation=d.reservation;
        if(d.kind!=6&&d.kind!=7){result.sourceTimestamp=d.sourceTimestamp;result.receivedTimestamp=d.receivedTimestamp;result.nativeSequence=d.nativeSequence;System.arraycopy(d.writerGuid,0,result.writerGuid,0,16);}}
    static <T> Delivery<T> take(Endpoint e,Codec<T> codec){
        // Complete result, metadata buffers and decode cursor already exist before
        // the first peek. Any later decode/copy/OOM fails BEFORE commit and cancels
        // the one proved borrow; cancellation uncertainty retires once.
        Delivery<T> result=new Delivery<>();int n=e.group.enter();if(n!=0){result.code=n;return result;}boolean pending=false;
        try{n=e.borrow();if(n!=0){result.code=n;return result;}pending=true;OwnedServiceWire.Record d=e.record;metadata(d,result);Transaction t=e.find(d.ticket);
            if(d.kind!=6&&d.kind!=7){byte[] body=Arrays.copyOfRange(e.packet,352,352+d.bodyBytes);T decoded=codec.decode(body);
                // The complete decoder must reject trailing/malformed fields;
                // generated32 codecs do so. Abandoned is still fully decoded.
                result.abandoned=t!=null&&t.state==ABANDONED;if(!result.abandoned){result.value=decoded;result.hasValue=true;}}
            if(!e.group.session.active()){result.value=null;result.hasValue=false;result.code=125;e.cancel(125);pending=false;return result;}
            n=e.commit();pending=false;if(n!=0){result.value=null;result.hasValue=false;result.code=n;result.failure=e.diagnostic.operations[COMMIT].failure;return result;}
            // Only scalar writes/fixed copies occur after a successful ACK.
            if(e.role==4&&d.kind==2){t=e.history[e.used++];t.ticket=d.ticket;t.state=DELIVERED;t.nativePresent=true;t.nativeSequence=d.nativeSequence;System.arraycopy(d.writerGuid,0,t.writerGuid,0,16);e.diagnostic.transactionSlots=e.used;}
            else if(t!=null)t.state=DONE;
            if(d.kind==6||d.kind==7){result.code=d.error==0?125:d.error;if(d.kind==7||d.ticket==0){e.primary(result.code);e.diagnostic.poisoned=true;}}
            else if(result.abandoned)result.code=125;
            return result;
        }catch(RuntimeException|Error failure){result.failure=failure;if(e.diagnostic.primaryFailure==null)e.diagnostic.primaryFailure=failure;e.primary(22);result.code=22;result.value=null;result.hasValue=false;if(pending){e.cancel(22);pending=false;}return result;}
        finally{if(pending)e.cancel(22);e.group.leave();}
    }
    public static final class Client<Q,S> implements AutoCloseable {
        final Endpoint e;final Definition<Q,S> definition;Client(Endpoint e,Definition<Q,S> definition){this.e=e;this.definition=definition;}
        public Submission submit(Q value){Submission result=new Submission();int n=e.group.enter();if(n!=0){result.code=n;return result;}
            try{n=e.live();if(n!=0){result.code=n;return result;}byte[] bytes=definition.request.encode(value);if(bytes==null||bytes.length>e.requestMaximum){result.code=90;return result;}n=e.inspect();if(n!=0){result.code=n;return result;}
                if(e.used==e.history.length){result.code=90;return result;}Transaction t=e.history[e.used++];t.state=RESERVED;e.diagnostic.transactionSlots=e.used;
                System.arraycopy(bytes,0,e.input,0,bytes.length);Arrays.fill(e.ticket,(byte)0);n=e.call(SUBMIT,0,0,bytes.length);if(n!=0){if(n!=11)e.terminal(n);else e.primary(n);result.code=n;result.failure=e.diagnostic.operations[SUBMIT].failure;return result;}
                long ticket=OwnedServiceWire.u(e.ticket,0,8);if(ticket==0||e.find(ticket)!=null){result.code=e.terminal(22);return result;}
                t.ticket=ticket;t.state=ADMITTED;result.ticket=ticket;e.diagnostic.lastTicket=ticket;return result;
            }catch(RuntimeException|Error failure){result.failure=failure;if(e.diagnostic.primaryFailure==null)e.diagnostic.primaryFailure=failure;e.primary(22);result.code=22;return result;}finally{e.group.leave();}}
        public Delivery<S> takeResponse(){return take(e,definition.response);}
        public int abandon(long ticket){int n=e.group.enter();if(n!=0)return n;try{n=e.live();if(n!=0)return n;Transaction t=e.find(ticket);if(t==null||t.state!=ADMITTED)return 9;n=e.inspect();if(n!=0)return n;t.state=ABANDONED;n=e.call(ABANDON, ticket,0,0);return n==0?0:e.terminal(n);}finally{e.group.leave();}}
        public Availability available(long waitNs){Availability result=new Availability();int n=e.group.enter();if(n!=0){result.code=n;return result;}try{n=e.live();if(n!=0){result.code=n;return result;}if(e.ownerCount!=3){result.code=95;return result;}if(waitNs<0||waitNs>e.group.options.maximumWaitNs){result.code=22;return result;}n=e.inspect();if(n!=0){result.code=n;return result;}e.available[0]=(byte)0xa5;n=e.call(AVAILABLE,waitNs,0,0);if(n!=0){result.code=e.primary(n);result.failure=e.diagnostic.operations[AVAILABLE].failure;return result;}if(e.available[0]!=0&&e.available[0]!=1){result.code=e.terminal(22);return result;}result.available=e.available[0]==1;return result;}finally{e.group.leave();}}
        public int closeChecked(){return closeEndpoint(e,false);}public int revokeChecked(){return closeEndpoint(e,true);}public void close(){int n=closeChecked();if(n!=0)throw new RosException(new Result(n));}
    }
    public static final class Server<Q,S> implements AutoCloseable {
        final Endpoint e;final Definition<Q,S> definition;Server(Endpoint e,Definition<Q,S> definition){this.e=e;this.definition=definition;}
        public Delivery<Q> takeRequest(){return take(e,definition.request);}
        public int respond(long ticket,S value){int n=e.group.enter();if(n!=0)return n;try{n=e.live();if(n!=0)return n;Transaction t=e.find(ticket);if(t==null||t.state!=DELIVERED)return 9;
                byte[] bytes=definition.response.encode(value);if(bytes==null||bytes.length>e.responseMaximum)return 90;n=e.inspect();if(n!=0)return n;System.arraycopy(bytes,0,e.input,0,bytes.length);t.state=DONE;n=e.call(RESPOND,ticket,0,bytes.length);return n==0?0:e.terminal(n);
            }catch(RuntimeException|Error failure){if(e.diagnostic.primaryFailure==null)e.diagnostic.primaryFailure=failure;e.primary(22);throw failure;}finally{e.group.leave();}}
        public int closeChecked(){return closeEndpoint(e,false);}public int revokeChecked(){return closeEndpoint(e,true);}public void close(){int n=closeChecked();if(n!=0)throw new RosException(new Result(n));}
    }
    static int closeEndpoint(Endpoint e,boolean revoke){if(e.group.busy)return 17;if(e.closed)return cleanupCode(e);int n=e.group.enter(true);if(n!=0)return n;try{e.cleanup(revoke);return cleanupCode(e);}finally{e.group.leave();}}
}
