package org.uwvm.roswasm;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.function.Consumer;
import java.util.function.Function;

/** Typed capability factories and guest-driven callbacks.
 * A local Node grants no native ROS authority. Endpoints, types, QoS and native
 * identity belong to host alias contracts; no method synthesizes ROS discovery.
 */
public final class Node {
    public final Session session;
    public Node(Session session){if(session==null)throw new IllegalArgumentException("session");this.session=session;}

    /** Complete typed topic publication; publish invokes the host exactly once. */
    public static final class Publisher<T> implements AutoCloseable {
        public final Capability owner;private final Codec<T> codec;
        Publisher(Capability owner,Codec<T> codec){this.owner=owner;this.codec=codec;}
        public Result publish(T value){return owner.invoke("topic_publish",codec.encode(value),0);}
        public void close(){owner.close();}
    }
    public <T> Publisher<T> createPublisher(Codec<T> codec,String alias){
        if(codec==null)throw new IllegalArgumentException("codec");Capability owner=session.open(alias);
        try{return new Publisher<>(owner,codec);}catch(RuntimeException|Error e){rollback(e,owner);throw e;}
    }
    /** Each scheduling source returns11 when idle; no background callback occurs. */
    public interface Pollable {Result pollOnce();}
    public static final class Subscription<T> implements Pollable,AutoCloseable {
        public final Capability owner;private final Codec<T> codec;private final int capacity;private final Consumer<T> callback;
        Subscription(Capability owner,Codec<T> codec,int capacity,Consumer<T> callback){this.owner=owner;this.codec=codec;this.capacity=capacity;this.callback=callback;}
        public Result pollOnce(){
            if(!owner.acquire())return new Result(17);
            try{Result r=owner.invoke("event_poll",new byte[0],capacity);if(r.error==0)callback.accept(codec.decode(r.data));return r;}
            finally{owner.release();}
        }
        public void close(){owner.close();}
    }
    public <T> Subscription<T> createSubscription(Codec<T> codec,String alias,int capacity,Consumer<T> callback){
        if(codec==null||callback==null||capacity<0||capacity>4194304)throw new IllegalArgumentException("bounded subscription");
        Capability owner=session.open(alias);
        try{owner.invoke("topic_subscribe",new byte[0],0).checked();return new Subscription<>(owner,codec,capacity,callback);}
        catch(RuntimeException|Error e){rollback(e,owner);throw e;}
    }
    public static final class Client<Q,S> implements AutoCloseable {
        public final Capability owner;private final Codec<Q> request;private final Codec<S> response;private final int capacity;
        Client(Capability owner,Codec<Q> request,Codec<S> response,int capacity){this.owner=owner;this.request=request;this.response=response;this.capacity=capacity;}
        public S call(Q value){return response.decode(owner.invoke("service_call",request.encode(value),capacity).checked().data);}
        /** Admission ID is not completion; observe response/resource-error events. */
        public Result submit(Q value,Capability observer){return owner.invoke("service_submit",request.encode(value),0,observer,0);}
        public void close(){owner.close();}
    }
    public <Q,S> Client<Q,S> createClient(Codec<Q> request,Codec<S> response,String alias,int capacity){
        if(request==null||response==null||capacity<0||capacity>4194304)throw new IllegalArgumentException("bounded client");
        Capability owner=session.open(alias);try{return new Client<>(owner,request,response,capacity);}catch(RuntimeException|Error e){rollback(e,owner);throw e;}
    }
    public static final class Service<Q,S> implements Pollable {
        public final Capability advertise,respond;
        private final Codec<Q> request;private final Codec<S> response;private final Function<Q,S> callback;private final int capacity;
        Service(Capability a,Capability r,Codec<Q> q,Codec<S> s,int capacity,Function<Q,S> callback){advertise=a;respond=r;request=q;response=s;this.capacity=capacity;this.callback=callback;}
        public Result pollOnce(){
            if(!advertise.acquire())return new Result(17);
            if(!respond.acquire()){advertise.release();return new Result(17);}
            try{
                Result r=advertise.invoke("event_poll",new byte[0],capacity);if(r.error!=0)return r;
                Codecs.Reader reader=new Codecs.Reader(r.data);long id=reader.unsigned(8);
                if(id==0)throw new IllegalArgumentException("zero request ID");
                Q value=request.decode(reader.take(reader.remaining()));reader.end();
                return respond.invoke("service_respond",Codecs.prefixId(id,response.encode(callback.apply(value))),0);
            }finally{respond.release();advertise.release();}
        }
    }
    public <Q,S> Service<Q,S> createService(Codec<Q> request,Codec<S> response,String advertiseAlias,String respondAlias,int capacity,Function<Q,S> callback){
        if(request==null||response==null||callback==null||advertiseAlias==null||advertiseAlias.equals(respondAlias)||capacity<8||capacity>4194304)
            throw new IllegalArgumentException("separate bounded service owners");
        Capability advertise=session.open(advertiseAlias),respond=null;
        try{respond=session.open(respondAlias);advertise.invoke("service_advertise",new byte[0],0).checked();return new Service<>(advertise,respond,request,response,capacity,callback);}
        catch(RuntimeException|Error e){if(respond!=null)rollback(e,respond);rollback(e,advertise);throw e;}
    }
    private static void rollback(Throwable primary,Capability owner){
        try{
            Result r=owner.closeChecked();
            if(r.error!=0)try{primary.addSuppressed(new RosException(r));}catch(RuntimeException|Error unavailable){}
        }catch(RuntimeException|Error cleanup){try{primary.addSuppressed(cleanup);}catch(RuntimeException|Error unavailable){}}
    }
    /** Explicit parameter/TF/graph/lifecycle/time/logger alias, no implicit scope. */
    public Capability openOperation(String alias){return session.open(alias);}

    /** Separate Action aliases and caller-owned goal IDs. No timeout is replayed. */
    public static final class ActionClient {
        private final Capability send,feedback,result,cancel;
        private final int maximum;private final Set<Long> goals=new HashSet<>();
        public ActionClient(Capability send,Capability feedback,Capability result,Capability cancel,int maximumGoals){
            if(send==null||maximumGoals<1||maximumGoals>65536)throw new IllegalArgumentException("Action owner/bound");
            for(Capability owner:new Capability[]{feedback,result,cancel})if(owner!=null&&owner.session!=send.session)throw new IllegalArgumentException("foreign session");
            this.send=send;this.feedback=feedback;this.result=result;this.cancel=cancel;this.maximum=maximumGoals;
        }
        public long sendGoal(byte[] canonicalGoal){
            if(goals.size()>=maximum)throw new RosException(new Result(90));
            byte[] bytes=send.invoke("action_send_goal",canonicalGoal,8).checked().data;
            long id=Codecs.INT64.decode(bytes);if(id==0||goals.contains(id))throw new IllegalArgumentException("invalid/duplicate goal ID");
            goals.add(id);return id;
        }
        public Result cancelGoal(long id){
            if(!goals.contains(id))return new Result(9);if(cancel==null)return new Result(95);
            return cancel.invoke("action_cancel_own_goal",Codecs.INT64.encode(id),1);
        }
        public Result receive(long id,int capacity,boolean terminal){
            if(!goals.contains(id))return new Result(9);
            Capability owner=terminal?result:feedback;if(owner==null)return new Result(95);
            Result r=owner.invoke(terminal?"action_receive_result":"action_receive_feedback",Codecs.INT64.encode(id),capacity);
            if(r.error==0){
                Codecs.Reader reader=new Codecs.Reader(r.data);
                if(reader.unsigned(8)!=id)throw new IllegalArgumentException("foreign goal response");
                if(terminal){long outcome=reader.unsigned(1);if(outcome<2||outcome>4)throw new IllegalArgumentException("nonterminal result");goals.remove(id);}
            }
            return r;
        }
    }
    /** Bounded round-robin executor. Blocking/waiting requires an explicit driver. */
    public static final class Executor {
        private final int maximum;private final List<Pollable> sources=new ArrayList<>();private int position;private boolean spinning;
        public Executor(int maximum){if(maximum<0||maximum>65536)throw new IllegalArgumentException("source bound");this.maximum=maximum;}
        public void add(Pollable source){if(source==null)throw new IllegalArgumentException("source");if(spinning||sources.contains(source))throw new RosException(new Result(17));if(sources.size()>=maximum)throw new RosException(new Result(90));sources.add(source);}
        public void remove(Pollable source){if(spinning)throw new RosException(new Result(17));sources.remove(source);position%=Math.max(1,sources.size());}
        public Result spinOnce(){
            if(spinning)throw new RosException(new Result(17));spinning=true;
            try{for(int i=0;i<sources.size();++i){Pollable source=sources.get(position);position=(position+1)%sources.size();Result r=source.pollOnce();if(r.error!=11)return r;}return new Result(11);}
            finally{spinning=false;}
        }
    }
}
