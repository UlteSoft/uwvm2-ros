using System;
using Uwvm.RosWasm.Aot;
using Uwvm.RosWasm.Owned;
using Uwvm.RosWasm.Generated.Control;
using R=Uwvm.RosWasm.Generated.Control.BoundedRecords;
internal static class Program {
    private static int Main(string[] a) {
        var life=new ApplicationLifetime();
        try {
            AotWitness.CheckGuestRoots();
            if(a.Length<1)throw new ArgumentException("type-query|get|set required");
            if(a[0]=="type-query") {
                if(a.Length!=7)throw new ArgumentException("type-query call observe available-or-- native-type RIHS steps");
                int credits=ApplicationLifetime.Steps(a[6]);var def=ControlRoots.Ros2Type_description_interfacesSrvGetTypeDescription();
                life.Group=new OwnedGroup(1,1,def.MaximumRequest,def.MaximumResponse);
                var client=life.Group.OpenClient(def,a[1],a[2],a[3]=="-"?null:a[3]);
                if(a[3]!="-"&&!client.Available(0))throw new InvalidOperationException("actual separately granted availability false");
                var ticket=client.Submit(new R._Ros2Type_description_interfacesSrvGetTypeDescription_Request { @type_name=a[4],@type_hash=a[5],@include_type_sources=true }); // DBG_ROS_SUBMIT_CSHARP
                bool completed=false;
                for(int i=0;i<credits;i++) { var d=client.Poll();if(!d.Present)continue;if(d.IsControl)throw new InvalidOperationException("actual source control "+d.Control.Error);
                    if(d.Ticket.Value!=ticket.Value||!d.Value!.@successful||d.Value.@type_sources.Length==0)throw new InvalidOperationException("complete TypeQuery correlation/native refusal");
                    Console.WriteLine("source-files="+d.Value.@type_sources.Length+" referenced="+d.Value.@type_description.@referenced_type_descriptions.Length+" native-seq="+d.Information.NativeSequence);completed=true;break; // DBG_ROS_DELIVERY_CSHARP
                }
                if(!completed) {client.Abandon(ticket);throw new InvalidOperationException("caller credits exhausted; no implicit cancellation or resend");}
            }else if(a[0]=="get") {
                if(a.Length!=5)throw new ArgumentException("get call observe parameter-name steps");
                int credits=ApplicationLifetime.Steps(a[4]);var def=ControlRoots.Ros2Rcl_interfacesSrvGetParameters();
                life.Group=new OwnedGroup(1,1,def.MaximumRequest,def.MaximumResponse);var client=life.Group.OpenClient(def,a[1],a[2]);
                var ticket=client.Submit(new R._Ros2Rcl_interfacesSrvGetParameters_Request { @names=new[]{a[3]} });bool completed=false;
                for(int i=0;i<credits;i++) {var d=client.Poll();if(!d.Present)continue;if(d.IsControl||d.Ticket.Value!=ticket.Value||d.Value!.@values.Length!=1)throw new InvalidOperationException("complete GetParameters failed");
                    Console.WriteLine("type="+d.Value.@values[0].@type+" signed-int64="+d.Value.@values[0].@integer_value);completed=true;break;}
                if(!completed) {client.Abandon(ticket);throw new InvalidOperationException("caller credits exhausted");}
            }else if(a[0]=="set") {
                if(a.Length!=6)throw new ArgumentException("set call observe parameter-name signed-int64 steps");
                int credits=ApplicationLifetime.Steps(a[5]);long value=long.Parse(a[4],System.Globalization.CultureInfo.InvariantCulture);var def=ControlRoots.Ros2Rcl_interfacesSrvSetParameters();
                life.Group=new OwnedGroup(1,1,def.MaximumRequest,def.MaximumResponse);var client=life.Group.OpenClient(def,a[1],a[2]);
                var p=new R._Ros2Rcl_interfacesMsgParameter { @name=a[3],@value=new R._Ros2Rcl_interfacesMsgParameterValue { @type=2,@integer_value=value } };
                var ticket=client.Submit(new R._Ros2Rcl_interfacesSrvSetParameters_Request { @parameters=new[]{p} });bool completed=false;
                for(int i=0;i<credits;i++) {var d=client.Poll();if(!d.Present)continue;if(d.IsControl||d.Ticket.Value!=ticket.Value||d.Value!.@results.Length!=1||!d.Value.@results[0].@successful)throw new InvalidOperationException("actual SetParameters rejected");completed=true;break;}
                if(!completed) {client.Abandon(ticket);throw new InvalidOperationException("caller credits exhausted");}
            }else throw new ArgumentException("unsupported caller mode");
        }catch(Exception error) {life.Primary=error;}
        return life.Finish();
    }
}
