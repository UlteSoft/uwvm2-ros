package org.uwvm.roswasm.examples.owned;

import java.util.Arrays;
import org.uwvm.roswasm.*;

/** Real-entry Guest fixture Source, never run here. Select type-query|parameter,
 * expected-primary0|1, then the unchanged real application's full arguments.
 * The external admitted provider must cause a REAL cleanup failure, separately
 * from any intended body failure. No model, Guest-invented errno or Host fallback
 * is inserted. Require complete evidence AND the actual uncaught failure exit.
 */
public final class ApplicationFailureGuest {
    public static void main(String[] args){
        if(args.length<2||!args[0].equals("type-query")&&!args[0].equals("parameter")||!args[1].equals("0")&&!args[1].equals("1"))
            throw new IllegalArgumentException("type-query|parameter expected-primary0|1 real-application-arguments...");
        boolean primaryExpected=args[1].equals("1");String[] application=Arrays.copyOfRange(args,2,args.length);
        OwnedService.Diagnostic target=new OwnedService.Diagnostic();
        try{
            if(args[0].equals("type-query"))TypeQueryApplication.main(application);else ParameterApplication.main(application);
        }catch(OwnedApplicationFailure failure){
            try{
                if(!failure.finished()||!failure.cleanupAttempted()||!failure.cleanupFailed()||failure.endpointCount()==0||
                    (failure.primaryFailure()!=null)!=primaryExpected)throw new AssertionError("actual app/cleanup fault branch not established");
                boolean observed=false;
                for(int row=0;row<failure.endpointCount();row++){
                    failure.copyDiagnostic(row,target);OwnedService.Outcome nativeRetire=target.operations[10];
                    if(nativeRetire.code!=0||nativeRetire.failure!=null)observed=true;
                    System.out.println("native row="+row+" attempted="+nativeRetire.attempted+" returned="+nativeRetire.returned+" code="+nativeRetire.code);
                    for(int slot=0;slot<3;slot++){
                        OwnedService.Outcome close=target.close[slot],revoke=target.revoke[slot];
                        if(close.code!=0||close.failure!=null||revoke.code!=0||revoke.failure!=null)observed=true;
                        OwnedApplicationFailure.OwnerEvidence owner=failure.owner(row,slot);
                        System.out.println("raw row="+row+" slot="+slot+" full-original="+Long.toUnsignedString(owner.retirementHandle())+" remaining="+Long.toUnsignedString(owner.remainingHandle())+" close="+close.code+" revoke="+revoke.code);
                    }
                }
                if(!observed)throw new AssertionError("no actual retained native/raw cleanup failure");
                System.out.println("EXPECTED_APPLICATION_FAILURE_COMPLETE primary="+primaryExpected+" cleanup="+failure.cleanupCode());
            }catch(Throwable reporting){failure.recordReportingFailure(reporting);}
            // Same preallocated envelope, exact original primary and all cleanup
            // objects survive diagnostics. Never turn the expected fault into0.
            throw failure;
        }
        throw new AssertionError("real application returned normally; requested fault not qualified");
    }
}
