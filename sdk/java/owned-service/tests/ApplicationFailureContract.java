package org.uwvm.roswasm;

/** LOCAL model Source only, not an executed main/Guest/native acceptance result.
 * Compile with the unchanged OwnedServiceModel Source. The production Group,
 * Session, Capability, cleanup and application-envelope bodies are exercised;
 * only business/native effects are explicitly modelled below.
 */
public final class ApplicationFailureContract {
    private static void check(boolean value){if(!value)throw new AssertionError();}
    private static final class Business implements Transport {
        long next=0x8000000100000001L;int closes;boolean faults;
        final long[] order=new long[3];
        final RuntimeException first=new IllegalStateException("first raw close model"),last=new IllegalStateException("last raw close model");
        public int backend(){return 2;}
        public Result invoke(String op,long handle,byte[] input,int capacity,long other,long correlation){
            if(op.equals("abi_version"))return Result.value(1);
            if(op.equals("capability_open"))return Result.value(next++);
            if(op.equals("capability_close")){
                int row=closes++;order[row]=handle;
                if(faults&&row==0)throw first;if(faults&&row==2)throw last;
                return new Result(faults?110:0);
            }
            return new Result(95);
        }
    }
    private static final class Fixture {
        final Business business=new Business();final OwnedServiceModel.Native nativeModel=new OwnedServiceModel.Native();
        final RuntimeException nativeFailure=new IllegalStateException("native retire model");boolean faultNative;
        final Session session=new Session(business,3);
        final OwnedService.Group group=new OwnedService.Group(session,new OwnedService.Options(1,4,1,1,0,8192),new OwnedService.NativeCalls(){
            public int call(int op,OwnedService.Endpoint e,long a,long b,int size){
                if(faultNative&&op==OwnedService.RETIRE){nativeModel.retires++;throw nativeFailure;}
                return nativeModel.call(op,e,a,b,size);
            }
        },true);
        // Every envelope, diagnostic, owner row and Throwable precedes open().
        final OwnedApplicationFailure outcome=new OwnedApplicationFailure(session,group);
        final OwnedService.Diagnostic target=new OwnedService.Diagnostic();
        final RuntimeException primary=new IllegalArgumentException("exact body primary"),report=new IllegalStateException("report model");
        void open(){OwnedService.Definition<Boolean,Boolean> d=new OwnedService.Definition<>("rcl_interfaces/srv/Model",null,OwnedServiceModel.ID,Codecs.BOOL,1,Codecs.BOOL,1);
            check(group.client(d,"call","observe","available",OwnedServiceModel.qos(),OwnedServiceModel.qos()).code==0);}
    }
    private static void expectSame(OwnedApplicationFailure expected){
        try{expected.throwIfFailed();}catch(OwnedApplicationFailure actual){check(actual==expected);return;}
        throw new AssertionError("failed application envelope returned normally");
    }
    private static void success(){Fixture f=new Fixture();f.open();f.outcome.finish();check(!f.outcome.failed());f.outcome.throwIfFailed();check(!f.session.active());}
    private static void cleanupOnly(){Fixture f=new Fixture();f.open();f.business.faults=true;f.outcome.finish();
        check(f.outcome.primaryFailure()==null&&f.outcome.cleanupFailed()&&f.outcome.cleanupCode()!=0);expectSame(f.outcome);
        check(f.business.closes==3&&f.nativeModel.retires==1);f.outcome.finish();expectSame(f.outcome);check(f.business.closes==3&&f.nativeModel.retires==1);}
    private static void primaryOnly(){Fixture f=new Fixture();f.open();f.outcome.recordPrimary(f.primary);f.outcome.finish();
        check(!f.outcome.cleanupFailed()&&f.outcome.primaryFailure()==f.primary&&f.outcome.getCause()==f.primary);expectSame(f.outcome);}
    private static void everyFailure(){Fixture f=new Fixture();f.open();f.business.faults=true;f.faultNative=true;f.outcome.recordPrimary(f.primary);f.outcome.finish();
        check(f.outcome.primaryFailure()==f.primary&&f.outcome.getCause()==f.primary&&f.outcome.cleanupCode()==115&&f.business.closes==3);
        f.outcome.copyDiagnostic(0,f.target);check(f.target.operations[OwnedService.RETIRE].failure==f.nativeFailure);
        check(f.target.close[2].failure==f.business.first&&f.target.close[1].code==110&&f.target.close[0].failure==f.business.last);
        for(int slot=0;slot<3;slot++){
            OwnedApplicationFailure.OwnerEvidence owner=f.outcome.owner(0,slot);
            check(owner.present()&&owner.beforeHandle()!=0&&owner.remainingHandle()==0&&owner.retirementHandle()==owner.beforeHandle()&&owner.attempted());
        }
        check(f.outcome.owner(0,2).failure()==f.business.first&&f.outcome.owner(0,0).failure()==f.business.last);
        // A caller's mutable detached target cannot corrupt retained evidence.
        f.target.close[2].failure=null;f.outcome.copyDiagnostic(0,f.target);check(f.target.close[2].failure==f.business.first);
        f.outcome.recordReportingFailure(f.report);check(f.outcome.reportingFailure()==f.report&&f.outcome.primaryFailure()==f.primary);expectSame(f.outcome);
        f.outcome.finish();check(f.business.closes==3&&f.nativeModel.retires==1);
    }
    public static void main(String[] args){success();cleanupOnly();primaryOnly();everyFailure();}
}
