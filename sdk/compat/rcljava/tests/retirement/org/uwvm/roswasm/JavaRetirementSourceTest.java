package org.uwvm.roswasm;

import java.util.ArrayList;
import java.util.List;
import org.ros2.rcljava.migration.Deployment;
import org.ros2.rcljava.migration.MigrationContext;
import org.ros2.rcljava.node.Node;
import org.ros2.rcljava.publisher.Publisher;

/** UNEXECUTED regression source. Host counters below are authored JVM models.
 * The production-transport case reaches only actual Frame/reentrancy rejection;
 * it must NEVER reach a native Raw method on the JVM. No ROS/Wasm result is
 * inferred. Compile/run only in the original SSH Linux cgroup after release.
 */
public final class JavaRetirementSourceTest {
    private static int checks,cases;
    private static final long HIGH=0x8000000000000042L;
    private static final Error PREP=new OutOfMemoryError("injected known preparation failure");
    private static final Error DELIVERY=new OutOfMemoryError("injected post-return delivery failure");
    private static final Error UNKNOWN=new LinkageError("injected effect-unknown boundary failure");
    private interface Action { void run(); }
    private static void check(boolean value){++checks;if(!value)throw new AssertionError("check "+checks);}
    private static void fails(Throwable same,Action action){try{action.run();throw new AssertionError("missing failure");}catch(RuntimeException|Error actual){check(actual==same);}}
    private static void errno(int value,Action action){try{action.run();throw new AssertionError("missing errno");}catch(RosException actual){check(actual.result.error==value);}}

    /** Deliberately package-private-proof MODEL, not an alternative production
     * transport. It uses the actual Frame enter/leave and commit helper; counters
     * stand in for the Host only where explicitly described by a test name.
     */
    private static final class StagedModel implements Transport,RetirementTransport {
        long next=HIGH,failOnly;
        int opens,closes,revokes,closeCode,subscribeCode;
        boolean prepareFailure,deliveryFailure,unknownFailure;
        Runnable beforeCommit;
        final List<Long> ids=new ArrayList<>();
        Result poll=new Result(11);
        public int backend(){return 2;}
        public Result invoke(String op,long handle,byte[] input,int capacity,long other,long correlation){
            if(op.equals("abi_version"))return Result.value(1);
            if(op.equals("capability_open")){++opens;return Result.value(next++);}
            if(op.equals("topic_subscribe"))return new Result(subscribeCode);
            if(op.equals("event_poll"))return poll;
            return new Result(0);
        }
        public Result retire(String op,long handle,byte[] input,RetirementAttempt attempt){
            LinearMemory.Frame frame=LinearMemory.enter(input,0);
            try{
                if(prepareFailure&&(failOnly==0||failOnly==handle))throw PREP;
                if(beforeCommit!=null){Runnable action=beforeCommit;beforeCommit=null;action.run();}
                attempt.markAttempted("AUTHORED MODEL boundary, not native Raw");
                if(op.equals("capability_close")){++closes;ids.add(handle);}else ++revokes;
                if(unknownFailure)throw UNKNOWN;
                attempt.returned(closeCode);
                if(deliveryFailure)throw DELIVERY;
                return frame.finish(closeCode,LinearMemory.NONE);
            }finally{LinearMemory.leave(frame);}
        }
    }
    private static Capability opened(Session s,String alias){return s.open(alias);}
    private static void invalid(Action action){try{action.run();throw new AssertionError("invalid lifecycle accepted");}catch(IllegalArgumentException expected){++checks;}}
    private static void originalProductionPreparation(){
        // Real Ros1/2 entry code, with a synthetic owner ONLY to avoid any open.
        // The outer Frame makes actual reentrancy rejection precede Raw/JNI.
        for(Transport production:new Transport[]{new Ros1Transport(),new Ros2Transport()}){
            Session s=new Session(production,1);
            Capability c=new Capability(s,HIGH,"synthetic-no-host-open");
            LinearMemory.Frame outer=LinearMemory.enter(new byte[0],0);
            try{
                try{c.closeChecked();throw new AssertionError("must reject before Raw");}
                catch(IllegalStateException expected){check(expected.getMessage().equals("reentrant ROS linear-memory call"));}
                check(c.handle()==HIGH&&c.lastRetirement().handle()==HIGH);
                check(c.lastRetirement().notAttempted()&&!c.lastRetirement().returned());
                check(c.lastRetirement().boundary()==null&&c.lastRetirement().failure()!=null);
            }finally{LinearMemory.leave(outer);}
        }
        ++cases;
    }
    private static void knownPreparationAndExplicitRecovery(){
        StagedModel m=new StagedModel();Session s=new Session(m,4);
        Capability c=opened(s,"first"),untouched=opened(s,"other");
        m.prepareFailure=true;fails(PREP,()->c.closeChecked());
        RetirementAttempt report=c.lastRetirement();
        check(c.handle()==HIGH&&report.handle()==HIGH&&report.notAttempted()&&m.closes==0);
        check(report.failure()==PREP&&report.firstPreparationFailure()==PREP);
        check(s.pendingRetirements().size()==1&&s.pendingRetirements().get(0)==report);
        m.prepareFailure=false;s.retryNotAttemptedClosures();
        check(c.handle()==0&&m.closes==1&&m.ids.get(0)==HIGH&&untouched.handle()!=0);
        check(report.number()==2&&report.attempted()&&report.returned()&&report.code()==0);
        check(report.firstPreparationFailure()==PREP&&report.failure()==null);
        s.retryNotAttemptedClosures();check(m.closes==1);s.closeChecked();check(m.closes==2);
        ++cases;
    }
    private static void postBoundaryNeverReplayed(){
        for(int scenario=0;scenario<3;++scenario){
            StagedModel m=new StagedModel();Session s=new Session(m,1);Capability c=opened(s,"one");
            if(scenario==0){m.unknownFailure=true;fails(UNKNOWN,()->c.closeChecked());}
            if(scenario==1){m.closeCode=110;check(c.closeChecked().error==110);}
            if(scenario==2){m.deliveryFailure=true;fails(DELIVERY,()->c.closeChecked());}
            RetirementAttempt r=c.lastRetirement();
            check(c.handle()==0&&r.handle()==HIGH&&r.attempted()&&!r.notAttempted()&&m.closes==1);
            check(r.returned()==(scenario!=0));
            if(scenario!=0)check(r.code()==(scenario==1?110:0));
            s.retryNotAttemptedClosures();s.closeChecked();check(m.closes==1);
        }
        ++cases;
    }
    private static void customTransportHasNoPreparationProof(){
        final int[] calls={0};
        Transport custom=new Transport(){
            public int backend(){return 2;}
            public Result invoke(String op,long id,byte[] input,int capacity,long other,long correlation){
                if(op.equals("capability_open"))return Result.value(HIGH);
                if(op.equals("capability_close")){check(id==HIGH);++calls[0];throw PREP;}
                return new Result(0);
            }
        };
        Session s=new Session(custom,1);Capability c=opened(s,"custom");
        fails(PREP,()->c.closeChecked());
        check(c.handle()==0&&c.lastRetirement().attempted());
        check(c.lastRetirement().boundary().equals("custom Transport.invoke (effect unknown)"));
        s.retryNotAttemptedClosures();check(calls[0]==1&&s.pendingRetirements().isEmpty());++cases;
    }
    private static void aliasRevokeBoundary(){
        StagedModel m=new StagedModel();Session s=new Session(m,4);
        Capability a=opened(s,"same"),b=opened(s,"same"),other=opened(s,"different");
        long bId=b.handle(),otherId=other.handle();
        m.prepareFailure=true;fails(PREP,()->a.revokeChecked());
        check(a.handle()==HIGH&&b.handle()==bId&&other.handle()==otherId&&m.revokes==0);
        check(s.lastRevokeAttempt().handle()==HIGH&&s.lastRevokeAttempt().notAttempted());
        m.prepareFailure=false;m.closeCode=107;check(a.revokeChecked().error==107);
        check(a.handle()==0&&b.handle()==0&&other.handle()==otherId&&m.revokes==1);
        check(s.lastRevokeAttempt().attempted()&&s.lastRevokeAttempt().returned()&&s.lastRevokeAttempt().code()==107);
        check(a.revokeChecked().error==9&&m.revokes==1);s.closeChecked();check(m.closes==1);++cases;
    }
    private static void sessionSweepRetainsKnownUnattempted(){
        StagedModel m=new StagedModel();Session s=new Session(m,3);
        Capability a=opened(s,"a"),b=opened(s,"b");m.prepareFailure=true;
        fails(PREP,()->s.closeChecked());
        check(!s.active()&&a.handle()==HIGH&&b.handle()==HIGH+1&&m.closes==0);
        check(s.pendingRetirements().size()==2&&s.lastCleanupFailures().size()==2);
        m.prepareFailure=false;s.retryNotAttemptedClosures();
        check(m.closes==2&&m.ids.get(0)==HIGH+1&&m.ids.get(1)==HIGH);
        check(s.pendingRetirements().isEmpty()&&!s.active());s.retryNotAttemptedClosures();check(m.closes==2);++cases;
    }
    private static Deployment.Binding topic(String node,boolean publish){
        return new Deployment.Binding(node,"/topic","std_msgs/msg/String","official-default-profile",publish?"topic_publish":"topic_subscribe",publish?"publish":"subscribe",publish?0:1024);
    }
    private static MigrationContext context(StagedModel m,Deployment.Binding...bindings){
        MigrationContext c=new MigrationContext(new Deployment.Profile(m,bindings,null,4,16,8,0));c.init();return c;
    }
    private static void rclFactoryLedgerAndShutdownRecovery(){
        StagedModel m=new StagedModel();MigrationContext c=context(m,topic("/robot",false),topic("/keep",true));
        Node n=c.createNode("robot","");Node keep=c.createNode("keep","");
        Publisher<?> untouched=keep.createPublisher(std_msgs.msg.String.class,"topic");
        m.subscribeCode=13;m.prepareFailure=true;
        errno(13,()->n.createSubscription(std_msgs.msg.String.class,"topic",v->{}));
        MigrationContext.CleanupAttempt failed=c.lastFactoryCleanup();
        check(failed!=null&&!failed.attempted&&failed.failure==PREP&&failed.primary instanceof RosException);
        check(failed.retirement.notAttempted()&&failed.retirement.handle()==HIGH+1&&n.getSubscriptions().isEmpty());
        check(c.pendingRetirements().size()==1&&m.closes==0);
        m.prepareFailure=false;c.retryNotAttemptedCleanup();
        check(m.closes==1&&m.ids.get(0)==HIGH+1&&failed.retirement.number()==2&&failed.retirement.attempted());
        check(failed.failure==PREP&&!failed.attempted&&c.isValid()); // Original attempt history is not overwritten.
        m.prepareFailure=true;
        try{c.shutdown();throw new AssertionError("cleanup failure required");}
        catch(MigrationContext.CleanupFailure expected){check(expected.report==c.lastCleanup());}
        check(!c.isValid()&&c.pendingRetirements().size()==1);
        m.prepareFailure=false;c.retryNotAttemptedCleanup();
        check(m.closes==2&&m.ids.get(1)==HIGH&&!c.isValid());
        c.retryNotAttemptedCleanup();check(m.closes==2);++cases;
    }
    private static void rclCallbackLeaseAndTwoOwnerRecovery(){
        StagedModel m=new StagedModel();MigrationContext c=context(m,topic("/sub",false));Node n=c.createNode("sub","");
        n.createSubscription(std_msgs.msg.String.class,"topic",v->errno(17,()->c.retryNotAttemptedCleanup()));
        Object executor=new Object();c.attachExecutor(n,executor);m.poll=new Result(0,Codecs.STRING.encode("model"),null,null);
        c.poll(n,executor);check(m.closes==0&&!c.isDispatching());c.detachExecutor(n,executor);c.shutdown();
        StagedModel pair=new StagedModel();
        Deployment.ServiceBinding binding=new Deployment.ServiceBinding("/client","/add","example_interfaces/srv/AddTwoInts","official-default-service-profile","client","call","observer",128,8);
        MigrationContext d=new MigrationContext(new Deployment.Profile(pair,new Deployment.Binding[0],null,4,16,8,0,new Deployment.ServiceBinding[]{binding}));
        d.init();d.createNode("client","").createClient(example_interfaces.srv.AddTwoInts.class,"add");
        pair.prepareFailure=true;pair.failOnly=HIGH+1;
        try{d.shutdown();throw new AssertionError("second-owner preparation failure required");}
        catch(MigrationContext.CleanupFailure expected){check(expected.report.completed);}
        check(pair.closes==1&&pair.ids.get(0)==HIGH&&d.pendingRetirements().size()==1);
        boolean found=false;for(MigrationContext.CleanupAttempt a:d.lastCleanup().attempts)
            if(a!=null&&a.next!=null){check(a.attempted&&a.next.retirement.handle()==HIGH+1&&!a.next.attempted&&a.next.failure==PREP);found=true;}
        check(found);pair.prepareFailure=false;d.retryNotAttemptedCleanup();check(pair.closes==2&&pair.ids.get(1)==HIGH+1);
        ++cases;
    }
    private static void genericOpenCannotEscapeInventory(){
        StagedModel m=new StagedModel();Session s=new Session(m,2);Capability a=opened(s,"first");
        invalid(()->a.invoke("capability_open",Codecs.utf8("untracked"),0));
        check(m.opens==1&&a.handle()==HIGH&&s.pendingRetirements().isEmpty());
        Capability b=opened(s,"second");check(m.opens==2&&b.handle()==HIGH+1);
        errno(90,()->s.open("third"));check(m.opens==2);s.closeChecked();check(m.closes==2);++cases;
    }
    private static void genericUnknownCloseDoesNotReplay(){
        final int[] calls={0};
        Transport custom=new Transport(){
            public int backend(){return 2;}
            public Result invoke(String op,long id,byte[] input,int capacity,long other,long correlation){
                if(op.equals("capability_open"))return Result.value(HIGH);
                if(op.equals("capability_close")){check(id==HIGH);++calls[0];throw UNKNOWN;}
                return new Result(0);
            }
        };
        Session s=new Session(custom,1);Capability a=opened(s,"unknown");
        fails(UNKNOWN,()->a.invoke("capability_close",new byte[0],0));
        check(a.handle()==0&&a.lastRetirement().handle()==HIGH&&a.lastRetirement().attempted());
        check(a.invoke("capability_close",new byte[0],0).error==9);
        s.retryNotAttemptedClosures();check(calls[0]==1);++cases;
    }
    private static void genericRevokeUsesAliasCommit(){
        StagedModel m=new StagedModel();Session s=new Session(m,3);
        Capability a=opened(s,"same"),b=opened(s,"same"),other=opened(s,"other");
        m.prepareFailure=true;fails(PREP,()->a.invoke("capability_revoke",new byte[0],0));
        check(a.handle()==HIGH&&b.handle()==HIGH+1&&other.handle()==HIGH+2&&m.revokes==0);
        m.prepareFailure=false;m.closeCode=107;
        check(a.invoke("capability_revoke",new byte[0],0).error==107);
        check(a.handle()==0&&b.handle()==0&&other.handle()==HIGH+2&&m.revokes==1);
        check(s.lastRevokeAttempt().handle()==HIGH&&s.lastRevokeAttempt().attempted());
        s.closeChecked();check(m.closes==1);++cases;
    }
    private static void genericShapeBusyAndInactiveAdmission(){
        StagedModel m=new StagedModel();Session s=new Session(m,4);Capability a=opened(s,"same"),b=opened(s,"same");
        for(String op:new String[]{"capability_close","capability_revoke"}){
            invalid(()->a.invoke(op,null,0));invalid(()->a.invoke(op,new byte[]{1},0));
            invalid(()->a.invoke(op,new byte[0],1));invalid(()->a.invoke(op,new byte[0],0,b,0));
            invalid(()->a.invoke(op,new byte[0],0,null,1));
        }
        check(m.closes==0&&m.revokes==0&&a.handle()==HIGH&&b.handle()==HIGH+1);
        check(a.acquire());check(a.invoke("capability_close",new byte[0],0).error==17);a.release();
        check(b.acquire());check(a.invoke("capability_revoke",new byte[0],0).error==17);b.release();
        check(m.closes==0&&m.revokes==0&&a.handle()==HIGH&&b.handle()==HIGH+1);
        StagedModel retirementModel=new StagedModel();Session retiringSession=new Session(retirementModel,2);
        Capability x=opened(retiringSession,"one"),y=opened(retiringSession,"two");
        retirementModel.beforeCommit=()->{
            check(y.invoke("capability_close",new byte[0],0).error==17);
            check(y.invoke("capability_revoke",new byte[0],0).error==17);
            check(y.handle()==HIGH+1&&retirementModel.closes==0&&retirementModel.revokes==0);
        };
        check(x.invoke("capability_close",new byte[0],0).error==0);
        check(x.handle()==0&&y.handle()==HIGH+1&&retirementModel.closes==1);
        retiringSession.closeChecked();check(retirementModel.closes==2);
        Session foreignSession=new Session(new StagedModel(),1);Capability foreign=opened(foreignSession,"foreign");
        check(a.invoke("capability_open",null,-1,foreign,1).error==9); // Original foreign-owner admission wins.
        s.shutdown();
        for(String op:new String[]{"capability_open","capability_close","capability_revoke"})check(a.invoke(op,null,-1).error==125);
        check(m.opens==2&&m.closes==0&&m.revokes==0);
        // Inactive generic admission is not cleanup authority; explicit close is.
        check(a.closeChecked().error==0&&a.handle()==0&&m.closes==1);
        check(a.invoke("capability_open",null,-1).error==9); // Original stale admission wins.
        s.closeChecked();foreignSession.closeChecked();++cases;
    }
    public static void main(String[] args){
        originalProductionPreparation();knownPreparationAndExplicitRecovery();postBoundaryNeverReplayed();
        customTransportHasNoPreparationProof();aliasRevokeBoundary();sessionSweepRetainsKnownUnattempted();
        rclFactoryLedgerAndShutdownRecovery();rclCallbackLeaseAndTwoOwnerRecovery();
        genericOpenCannotEscapeInventory();genericUnknownCloseDoesNotReplay();genericRevokeUsesAliasCommit();genericShapeBusyAndInactiveAdmission();
        check(cases==12);System.out.println("Java retirement source-model cases: "+cases+"; checks: "+checks);
    }
}
