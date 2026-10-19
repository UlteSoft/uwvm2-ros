using System;
using Uwvm.RosWasm.Owned;
using Uwvm.RosWasm.Generated.Control;
using R=Uwvm.RosWasm.Generated.Control.BoundedRecords;
internal static class Parameters {
    // This process-local application store is intentionally caller-selected.
    // It does not claim to be a transplanted rcl Node/native parameter service.
    private static int Main(string[] args) {
        OwnedGroup? group=null;Exception? primary=null;
        try {
            if(args.Length<1)throw new ArgumentException("get/set/serve mode required");
            var get=ControlRoots.Ros2Rcl_interfacesSrvGetParameters();var set=ControlRoots.Ros2Rcl_interfacesSrvSetParameters();
            group=new OwnedGroup(2,32,Math.Max(get.MaximumRequest,set.MaximumRequest),Math.Max(get.MaximumResponse,set.MaximumResponse));
            if(args[0]=="get") {
                if(args.Length!=6)throw new ArgumentException("get call observer parameter-name steps reserved--");
                if(args[5]!="-")throw new ArgumentException("last argument must be explicit -");
                int steps=AppCommon.Steps(args[4]);var c=group.OpenClient(get,args[1],args[2]);
                var ticket=c.Submit(new R._Ros2Rcl_interfacesSrvGetParameters_Request { @names=new[]{args[3]} });bool complete=false;
                for(int i=0;i<steps;i++) { var d=c.Poll();if(!d.Present)continue;if(d.IsControl)throw new InvalidOperationException("source control "+d.Control.Error);
                    if(d.Ticket.Value!=ticket.Value||d.Value!.@values.Length!=1)throw new InvalidOperationException("actual GetParameters correlation/count");
                    var v=d.Value.@values[0];Console.WriteLine("parameter type="+v.@type+" signed-integer="+v.@integer_value);complete=true;break;
                }
                if(!complete) { c.Abandon(ticket);throw new InvalidOperationException("poll work exhausted"); }
            } else if(args[0]=="set") {
                if(args.Length!=7)throw new ArgumentException("set call observer parameter-name signed-int64 steps reserved--");
                if(args[6]!="-")throw new ArgumentException("last argument must be explicit -");
                int steps=AppCommon.Steps(args[5]);var c=group.OpenClient(set,args[1],args[2]);
                var parameter=new R._Ros2Rcl_interfacesMsgParameter { @name=args[3],@value=new R._Ros2Rcl_interfacesMsgParameterValue { @type=2,@integer_value=long.Parse(args[4]) } };
                var ticket=c.Submit(new R._Ros2Rcl_interfacesSrvSetParameters_Request { @parameters=new[]{parameter} });bool complete=false;
                for(int i=0;i<steps;i++) { var d=c.Poll();if(!d.Present)continue;if(d.IsControl)throw new InvalidOperationException("source control "+d.Control.Error);
                    if(d.Ticket.Value!=ticket.Value||d.Value!.@results.Length!=1||!d.Value.@results[0].@successful)throw new InvalidOperationException("actual SetParameters denied or mismatched");complete=true;break;
                }
                if(!complete) { c.Abandon(ticket);throw new InvalidOperationException("poll work exhausted"); }
            } else if(args[0]=="serve") {
                if(args.Length!=8)throw new ArgumentException("serve get-advertise get-respond set-advertise set-respond owned-name initial-signed-int64 steps");
                int steps=AppCommon.Steps(args[7]);string ownedName=args[5];long stored=long.Parse(args[6]);
                var reader=group.OpenServer(get,args[1],args[2]);var writer=group.OpenServer(set,args[3],args[4]);int served=0;
                Func<R._Ros2Rcl_interfacesSrvGetParameters_Request,ServiceInformation,R._Ros2Rcl_interfacesSrvGetParameters_Response> read=(request,info)=> {
                    var values=new R._Ros2Rcl_interfacesMsgParameterValue[request.@names.Length];
                    for(int i=0;i<values.Length;i++)values[i]=new R._Ros2Rcl_interfacesMsgParameterValue { @type=(byte)(request.@names[i]==ownedName?2:0),@integer_value=request.@names[i]==ownedName?stored:0 };
                    return new R._Ros2Rcl_interfacesSrvGetParameters_Response { @values=values };
                };
                Func<R._Ros2Rcl_interfacesSrvSetParameters_Request,ServiceInformation,R._Ros2Rcl_interfacesSrvSetParameters_Response> write=(request,info)=> {
                    // Allocate the complete response BEFORE changing application state.
                    var response=new R._Ros2Rcl_interfacesSrvSetParameters_Response { @results=new R._Ros2Rcl_interfacesMsgSetParametersResult[request.@parameters.Length] };
                    for(int i=0;i<response.@results.Length;i++) { var p=request.@parameters[i];bool accept=p.@name==ownedName&&p.@value.@type==2;
                        response.@results[i]=new R._Ros2Rcl_interfacesMsgSetParametersResult { @successful=accept,@reason=accept?"":"application permits only its selected int64 parameter" };
                    }
                    for(int i=0;i<request.@parameters.Length;i++)if(response.@results[i].@successful)stored=request.@parameters[i].@value.@integer_value;
                    return response;
                };
                for(int i=0;i<steps;i++) { if(reader.ServeOnce(read))served++;if(writer.ServeOnce(write))served++; }
                if(served==0)throw new InvalidOperationException("no actual request served within finite local work");
            } else throw new ArgumentException("unknown mode");
        }catch(Exception error) { primary=error; }
        if(group==null) { Console.Error.WriteLine(primary);return 1; }
        return AppCommon.Finish(group,primary);
    }
}
