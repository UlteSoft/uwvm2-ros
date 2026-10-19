package org.uwvm.roswasm.examples.owned;

import org.uwvm.roswasm.*;
import org.uwvm.roswasm.generated.BoundedRecords.*;
import org.uwvm.roswasm.generated.ControlServices;

/** Actual numeric Guest route when explicitly compiled/linked/deployed on Linux.
 * Query type/name/hash/source-selection are caller inputs, never invented native
 * identities or an implicit graph/introspection grant. SOURCE fixture only here.
 */
public final class TypeQueryApplication {
    public static void main(String[] args){
        CallerArguments a=new CallerArguments(args,9);
        Ros2Type_description_interfacesSrvGetTypeDescription_Request request=new Ros2Type_description_interfacesSrvGetTypeDescription_Request();
        request.type_name=args[6];request.type_hash=args[7];
        if(!args[8].equals("0")&&!args[8].equals("1"))throw new IllegalArgumentException("include sources0|1");request.include_type_sources=args[8].equals("1");
        OwnedService.Definition<Ros2Type_description_interfacesSrvGetTypeDescription_Request,Ros2Type_description_interfacesSrvGetTypeDescription_Response> definition=ControlServices.Ros2Type_description_interfacesSrvGetTypeDescription();
        Session session=new Session(new Ros2OwnedServiceTransport(),3);
        OwnedService.Group group=new OwnedService.Group(session,new OwnedService.Options(1,8,definition.requestMaximum,definition.responseMaximum,0,1024*1024));
        OwnedApplicationFailure failure=new OwnedApplicationFailure(session,group);
        try{
            OwnedService.Opening<OwnedService.Client<Ros2Type_description_interfacesSrvGetTypeDescription_Request,Ros2Type_description_interfacesSrvGetTypeDescription_Response>> opening=group.client(definition,a.call,a.observer,a.availability,a.request,a.response);
            CallerArguments.require(opening.code);OwnedService.Client<Ros2Type_description_interfacesSrvGetTypeDescription_Request,Ros2Type_description_interfacesSrvGetTypeDescription_Response> client=opening.value;
            CallerArguments.available(client,a.availability!=null);OwnedService.Submission submission=client.submit(request);CallerArguments.require(submission.code);
            OwnedService.Delivery<Ros2Type_description_interfacesSrvGetTypeDescription_Response> delivery=CallerArguments.receive(client,a.polls);CallerArguments.info(delivery);
            Ros2Type_description_interfacesSrvGetTypeDescription_Response result=delivery.value;
            System.out.println("successful="+result.successful+" failure="+result.failure_reason+" sources="+result.type_sources.length+" fields="+result.type_description.type_description.fields.length);
            // raw_file_contents is byte[], not text: all original bytes (NUL or
            // invalid UTF8 included) remain available after complete decode/ACK.
            for(Ros2Type_description_interfacesMsgTypeSource source:result.type_sources)System.out.println("source="+source.type_name+" encoding="+source.encoding+" bytes="+source.raw_file_contents.length);
        }catch(Throwable primary){failure.recordPrimary(primary);}finally{failure.finish();}
        failure.throwIfFailed();
    }
}
