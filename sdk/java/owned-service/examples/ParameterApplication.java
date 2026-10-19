package org.uwvm.roswasm.examples.owned;

import java.util.Arrays;
import org.uwvm.roswasm.*;
import org.uwvm.roswasm.generated.BoundedRecords.*;
import org.uwvm.roswasm.generated.ControlServices;

/** Finite original GetParameterTypes caller. Names/type results preserve the
 * actual ordered full records. No parameter enumeration/set privilege is added. */
public final class ParameterApplication {
    public static void main(String[] args){
        CallerArguments a=new CallerArguments(args,7);
        Ros2Rcl_interfacesSrvGetParameterTypes_Request request=new Ros2Rcl_interfacesSrvGetParameterTypes_Request();request.names=Arrays.copyOfRange(args,6,args.length);
        OwnedService.Definition<Ros2Rcl_interfacesSrvGetParameterTypes_Request,Ros2Rcl_interfacesSrvGetParameterTypes_Response> definition=ControlServices.Ros2Rcl_interfacesSrvGetParameterTypes();
        Session session=new Session(new Ros2OwnedServiceTransport(),3);OwnedService.Group group=new OwnedService.Group(session,new OwnedService.Options(1,8,definition.requestMaximum,definition.responseMaximum,0,1024*1024));
        OwnedApplicationFailure failure=new OwnedApplicationFailure(session,group);
        try{
            OwnedService.Opening<OwnedService.Client<Ros2Rcl_interfacesSrvGetParameterTypes_Request,Ros2Rcl_interfacesSrvGetParameterTypes_Response>> opening=group.client(definition,a.call,a.observer,a.availability,a.request,a.response);
            CallerArguments.require(opening.code);OwnedService.Client<Ros2Rcl_interfacesSrvGetParameterTypes_Request,Ros2Rcl_interfacesSrvGetParameterTypes_Response> client=opening.value;
            CallerArguments.available(client,a.availability!=null);CallerArguments.require(client.submit(request).code);OwnedService.Delivery<Ros2Rcl_interfacesSrvGetParameterTypes_Response> delivery=CallerArguments.receive(client,a.polls);CallerArguments.info(delivery);
            if(delivery.value.types.length!=request.names.length)throw new IllegalStateException("actual response type/name arity differs");
            for(int i=0;i<request.names.length;i++)System.out.println(request.names[i]+" type="+delivery.value.types[i]);
        }catch(Throwable primary){failure.recordPrimary(primary);}finally{failure.finish();}
        failure.throwIfFailed();
    }
}
