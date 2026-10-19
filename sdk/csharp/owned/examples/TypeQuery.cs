using System;
using Uwvm.RosWasm.Owned;
using Uwvm.RosWasm.Generated.Control;
using R=Uwvm.RosWasm.Generated.Control.BoundedRecords;
internal static class TypeQuery {
    // call-alias observer-alias availability-alias-or-- actual-type actual-RIHS steps
    private static int Main(string[] args) {
        if(args.Length!=6) { Console.Error.WriteLine("call observer availability-or-- type-name actual-type-hash steps");return 2; }
        OwnedGroup? group=null;Exception? primary=null;
        try {
            int steps=AppCommon.Steps(args[5]);var definition=ControlRoots.Ros2Type_description_interfacesSrvGetTypeDescription();
            group=new OwnedGroup(1,1,definition.MaximumRequest,definition.MaximumResponse);
            var client=group.OpenClient(definition,args[0],args[1],args[2]=="-"?null:args[2]);
            if(args[2]!="-"&&!client.Available(0))throw new InvalidOperationException("actual retained client was not available");
            var request=new R._Ros2Type_description_interfacesSrvGetTypeDescription_Request { @type_name=args[3],@type_hash=args[4],@include_type_sources=true };
            Ticket ticket=client.Submit(request);bool complete=false;
            for(int i=0;i<steps;i++) {
                var delivery=client.Poll();if(!delivery.Present)continue;
                if(delivery.IsControl)throw new InvalidOperationException("actual source control "+delivery.Control.Error);
                var value=delivery.Value!;
                if(delivery.Ticket.Value!=ticket.Value||!value.@successful||value.@type_sources.Length==0)throw new InvalidOperationException("actual typed response unavailable or mismatched");
                Console.WriteLine("original GetTypeDescription sources="+value.@type_sources.Length+" references="+value.@type_description.@referenced_type_descriptions.Length+" native-sequence="+delivery.Information.NativeSequence);
                // Raw source bytes remain byte[], including NUL/invalid UTF8. This
                // example does not reinterpret a source file or execute it.
                complete=true;break;
            }
            if(!complete) { client.Abandon(ticket);throw new InvalidOperationException("finite poll work exhausted; local abandonment is not DDS cancellation"); }
        }catch(Exception error) { primary=error; }
        if(group==null) { Console.Error.WriteLine(primary);return 1; }
        return AppCommon.Finish(group,primary);
    }
}
