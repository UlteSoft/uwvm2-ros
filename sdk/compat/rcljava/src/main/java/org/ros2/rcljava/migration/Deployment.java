package org.ros2.rcljava.migration;

import java.io.InputStream;
import java.io.IOException;
import org.uwvm.roswasm.Transport;
import org.uwvm.roswasm.Ros2Transport;
import org.ros2.rcljava.RCLJava;
import org.ros2.rcljava.interfaces.Disposable;

/** Trusted deployment bootstrap, independent of unchanged business source.
 * Local aliases do not install Host grants. Profile bounds and all fixed
 * ownership tables are validated/allocated before any native open. Production
 * uses exactly one selected Ros2Transport; deterministic Transport injections
 * are only local source/model tests and confer no native acceptance.
 */
public final class Deployment {
    private Deployment() {}
    public interface Driver { boolean nextCredit(); }
    /** Bounded caller-paced credit driver. Only R\n advances one pass; EOF or
     * another byte ends the run. It reads no ambient clock and starts no task.
     */
    public static final class StdinCredits implements Driver {
        private final InputStream input; private int remaining;
        public StdinCredits(InputStream input,int maximum) {
            if(input==null||maximum<0||maximum>65536)throw new IllegalArgumentException("credit bound");
            this.input=input;remaining=maximum;
        }
        public boolean nextCredit() {
            if(remaining==0)return false;
            --remaining;
            try { return input.read()=='R' && input.read()=='\n'; }
            catch(IOException error) { throw new IllegalStateException("explicit credit input failed",error); }
        }
    }
    /** Exact original node/FQN resource, type, QoS-token and operation selection.
     * Clock: operation=time_read_steady, resource=node FQN, type="", qos="".
     * Topic: publish/subscribe use actual std_msgs/msg/String and default token.
     */
    public static final class Binding {
        public final String node,resource,type,qos,operation,alias;
        public final int capacity;
        public Binding(String node,String resource,String type,String qos,String operation,String alias,int capacity) {
            if(node==null||resource==null||type==null||qos==null||operation==null||alias==null||capacity<0||capacity>4194304)
                throw new IllegalArgumentException("exact binding");
            if(!operation.equals("topic_publish")&&!operation.equals("topic_subscribe")&&!operation.equals("time_read_steady"))
                throw new IllegalArgumentException("unsupported migration operation");
            if(!node.matches("/[A-Za-z_][A-Za-z_0-9]*(?:/[A-Za-z_][A-Za-z_0-9]*)*")||!resource.matches("/[A-Za-z_][A-Za-z_0-9]*(?:/[A-Za-z_][A-Za-z_0-9]*)*"))
                throw new IllegalArgumentException("canonical node/resource names required");
            byte[] aliasBytes=org.uwvm.roswasm.Codecs.utf8(alias);
            if(aliasBytes.length==0||aliasBytes.length>255||alias.indexOf('\0')>=0)throw new IllegalArgumentException("alias");
            if(operation.equals("time_read_steady")) {
                if(!resource.equals(node)||!type.isEmpty()||!qos.isEmpty()||capacity!=8)throw new IllegalArgumentException("complete separately bound clock64 required");
            } else if(!type.equals("std_msgs/msg/String")||!qos.equals("official-default-profile")||
                    (operation.equals("topic_publish")?capacity!=0:capacity<4))throw new IllegalArgumentException("complete supported topic binding required");
            this.node=node;this.resource=resource;this.type=type;this.qos=qos;this.operation=operation;this.alias=alias;this.capacity=capacity;
        }
        boolean sameKey(Binding b) { return node.equals(b.node)&&resource.equals(b.resource)&&type.equals(b.type)&&qos.equals(b.qos)&&operation.equals(b.operation); }
    }
    /** Explicit service pair. Every actual role/type/resource/QoS binding is
     * selected before business main; aliases select existing grants only.
     * Server first=advertise, second=respond; client first=call, second=observer.
     */
    /** Cooperative Native-wait boundary, explicitly selected by bootstrap.
     * Called only between completed native queries, with no dispatch/import
     * lease held. It may request Context.shutdown. It supplies neither time nor
     * readiness and must not block indefinitely; caller owns physical budgets.
     * Recursive availability waits and executor progress are refused17.
     */
    public interface WaitCheckpoint { void betweenQueries(); }
    /** Optional independent query grant. A nonnull clock alias is an explicit
     * time.read steady grant for REAL elapsed-time waits, not an ambient clock.
     * The finite query-pass budget limits this finite facade even for negative
     * (official infinite) Duration: exhaustion throws90, never false/timeout.
     */
    public static final class AvailabilityBinding {
        public final String queryAlias,steadyClockAlias;
        public final long maximumQueryWaitNs;
        public final int maximumQueries;
        public final WaitCheckpoint checkpoint;
        public AvailabilityBinding(String queryAlias,String steadyClockAlias,long maximumQueryWaitNs,int maximumQueries,WaitCheckpoint checkpoint){
            explicitAlias(queryAlias);
            if(steadyClockAlias!=null){explicitAlias(steadyClockAlias);if(steadyClockAlias.equals(queryAlias))throw new IllegalArgumentException("distinct query and clock grants");}
            if(maximumQueryWaitNs<1||maximumQueryWaitNs>org.uwvm.roswasm.ServiceClient.MAXIMUM_WAIT_NS||maximumQueries<1||maximumQueries>65536)throw new IllegalArgumentException("finite query bounds");
            this.queryAlias=queryAlias;this.steadyClockAlias=steadyClockAlias;this.maximumQueryWaitNs=maximumQueryWaitNs;this.maximumQueries=maximumQueries;this.checkpoint=checkpoint;
        }
    }
    private static void explicitAlias(String alias){
        if(alias==null)throw new IllegalArgumentException("explicit alias");
        byte[] bytes=org.uwvm.roswasm.Codecs.utf8(alias);
        if(bytes.length==0||bytes.length>255||alias.indexOf('\0')>=0)throw new IllegalArgumentException("alias");
    }
    public static final class ServiceBinding {
        public final String node,resource,type,qos,role,firstAlias,secondAlias;
        public final int capacity,maximumPending;
        public final AvailabilityBinding availability;
        /** Original constructor: no query owner, optional ABI, Clock or wait. */
        public ServiceBinding(String node,String resource,String type,String qos,String role,String firstAlias,String secondAlias,int capacity,int maximumPending){
            this(node,resource,type,qos,role,firstAlias,secondAlias,capacity,maximumPending,null);
        }
        public ServiceBinding(String node,String resource,String type,String qos,String role,String firstAlias,String secondAlias,int capacity,int maximumPending,AvailabilityBinding availability){
            if(node==null||resource==null||type==null||qos==null||role==null||firstAlias==null||secondAlias==null||firstAlias.equals(secondAlias))throw new IllegalArgumentException("two distinct explicit aliases");
            if(!node.matches("/[A-Za-z_][A-Za-z_0-9]*(?:/[A-Za-z_][A-Za-z_0-9]*)*")||!resource.matches("/[A-Za-z_][A-Za-z_0-9]*(?:/[A-Za-z_][A-Za-z_0-9]*)*"))throw new IllegalArgumentException("canonical resource");
            if(!role.equals("server")&&!role.equals("client"))throw new IllegalArgumentException("role");
            if(!ServiceTypes.known(type)||(!qos.equals("official-default-service-profile")&&!qos.equals("official-default-profile")))throw new IllegalArgumentException("complete selected service/type/QoS token");
            for(String alias:new String[]{firstAlias,secondAlias}){byte[] bytes=org.uwvm.roswasm.Codecs.utf8(alias);if(bytes.length==0||bytes.length>255||alias.indexOf('\0')>=0)throw new IllegalArgumentException("alias");}
            if(capacity<(role.equals("client")?72:24)||capacity>4194304||maximumPending<1||maximumPending>256)throw new IllegalArgumentException("finite service buffer/inflight bounds");
            if(availability!=null){
                if(!role.equals("client")||availability.queryAlias.equals(firstAlias)||availability.queryAlias.equals(secondAlias)||availability.steadyClockAlias!=null&&(availability.steadyClockAlias.equals(firstAlias)||availability.steadyClockAlias.equals(secondAlias)))throw new IllegalArgumentException("independent client-only query/clock roles");
            }
            this.node=node;this.resource=resource;this.type=type;this.qos=qos;this.role=role;this.firstAlias=firstAlias;this.secondAlias=secondAlias;this.capacity=capacity;this.maximumPending=maximumPending;this.availability=availability;
        }
    }
    public static final class Profile {
        final Transport transport; final Binding[] bindings; final Driver driver;
        final ServiceBinding[] services;
        final int maximumNodes,maximumOwners,maximumNodeResources,maximumCredits;
        public Profile(Transport transport,Binding[] bindings,Driver driver,int nodes,int owners,int resources,int maximumCredits) {
            this(transport,bindings,driver,nodes,owners,resources,maximumCredits,new ServiceBinding[0]);
        }
        public Profile(Transport transport,Binding[] bindings,Driver driver,int nodes,int owners,int resources,int maximumCredits,ServiceBinding[] services) {
            if(transport==null||transport.backend()!=2||bindings==null||bindings.length>65536||nodes<1||nodes>256||owners<0||owners>65536||resources<1||resources>256||maximumCredits<0||maximumCredits>65536)
                throw new IllegalArgumentException("explicit ROS2 and finite profile required");
            if(services==null||services.length>65536)throw new IllegalArgumentException("finite service selection");
            this.services=services.clone();
            java.util.HashSet<String> serviceKeys=new java.util.HashSet<String>();
            for(ServiceBinding b:this.services){if(b==null||!serviceKeys.add(b.node+'\0'+b.resource+'\0'+b.type+'\0'+b.qos+'\0'+b.role))throw new IllegalArgumentException("duplicate service binding");}
            this.transport=transport;this.bindings=bindings.clone();this.driver=driver;
            maximumNodes=nodes;maximumOwners=owners;maximumNodeResources=resources;this.maximumCredits=maximumCredits;
            java.util.HashSet<String> keys=new java.util.HashSet<String>();
            for(Binding binding:this.bindings) {
                if(binding==null)throw new IllegalArgumentException("null binding");
                String key=binding.node+'\0'+binding.resource+'\0'+binding.type+'\0'+binding.qos+'\0'+binding.operation;
                if(!keys.add(key))throw new IllegalArgumentException("duplicate binding key");
            }
            validateOptionalAliases();
        }
        /** Enabling the optional namespace opts into a closed whole-profile
         * role table. Every selected endpoint/clock owner has a distinct alias;
         * no query alias can shadow another endpoint or an independent grant.
         * Profiles using only the original constructor keep legacy selection.
         */
        private void validateOptionalAliases(){
            boolean enabled=false;for(ServiceBinding s:services)if(s.availability!=null)enabled=true;
            if(!enabled)return;
            if(!(transport instanceof org.uwvm.roswasm.ServiceClientTransport))throw new IllegalArgumentException("explicit optional transport required");
            java.util.HashSet<String> aliases=new java.util.HashSet<String>();
            for(Binding b:bindings)if(!aliases.add(b.alias))throw new IllegalArgumentException("duplicate selected alias");
            for(ServiceBinding s:services){
                if(!aliases.add(s.firstAlias)||!aliases.add(s.secondAlias))throw new IllegalArgumentException("duplicate service role alias");
                if(s.availability!=null){AvailabilityBinding a=s.availability;
                    if(!aliases.add(a.queryAlias)||a.steadyClockAlias!=null&&!aliases.add(a.steadyClockAlias))throw new IllegalArgumentException("duplicate availability/clock alias");}
            }
        }
        public static Profile denied() { return new Profile(new Ros2Transport(),new Binding[0],null,64,256,256,0); }
    }
    /** Install before RCLJava.init; no business-source changes are necessary. */
    public static void install(Profile profile) { RCLJava.installDeployment(profile); }
    public static MigrationContext context() { return RCLJava.migrationContext(); }
    public static void revoke(Disposable resource) { context().revoke(resource); }
    /** Launcher finally path preserving an application primary; all cleanup
     * outcomes remain in the preallocated context reports even under Error.
     */
    public static void shutdown(Throwable primary) {
        try { RCLJava.shutdown(); }
        catch(RuntimeException|Error cleanup) {
            if(primary==null)throw cleanup;
            if(cleanup!=primary)try { primary.addSuppressed(cleanup); }catch(RuntimeException|Error unavailable) {}
        }
    }
}
