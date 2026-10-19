using System;
using Uwvm.RosWasm;

/// <summary>Real command entries. Root owns native peer, exact grants, bounded scheduling and process cleanup.</summary>
internal static class Program {
    private const int Polls=256;
    private sealed class GoalCodec : ICodec<int> {
        public byte[] Encode(int value)=>Codecs.FibonacciGoal(value);
        public int Decode(byte[] bytes) { var r=new Codecs.Reader(bytes);int value=unchecked((int)r.Unsigned(4));r.End();if(value<0||value>46)throw new FormatException("Fibonacci profile");return value; }
    }
    private sealed class SequenceCodec : ICodec<int[]> {
        public byte[] Encode(int[] value) { if(value==null||value.Length>48)throw new ArgumentException("sequence48");var w=new Codecs.Writer(196).Unsigned((uint)value.Length,4);foreach(int n in value) { if(n<0)throw new ArgumentException("nonnegative Fibonacci");w.Unsigned((uint)n,4); }return w.Finish(); }
        public int[] Decode(byte[] bytes) { var r=new Codecs.Reader(bytes);ulong n=r.Unsigned(4);if(n>48||n>(ulong)r.Remaining/4)throw new FormatException("sequence bound");var values=new int[(int)n];for(int i=0;i<values.Length;i++) { values[i]=unchecked((int)r.Unsigned(4));if(values[i]<0)throw new FormatException("negative Fibonacci"); }r.End();return values; }
    }
    private static int Main(string[] args) {
        Exception? primary=null;var session=new Session(new NativeTransport(),32);int code=1;
        try {
            session.CheckAbi();if(args.Length<2)throw new ArgumentException("deny/topic/service/serve/observe/action plus actual aliases required");var node=new Node(session);
            switch(args[0]) {
            case "deny":
                try { session.Open(args[1]);throw new InvalidOperationException("fresh no-authority Instance unexpectedly opened alias"); }
                catch(RosException e) when(e.Result.Error==13) { Console.WriteLine("actual default-denied open13");code=0; }break;
            case "topic":
                if(args.Length!=3)throw new ArgumentException("topic publish-alias subscription-alias");
                string? received=null;var sub=node.CreateSubscription(Codecs.String,args[2],1024,v=>received=v);
                var pub=node.CreatePublisher(Codecs.String,args[1]);pub.Publish("C# ROS \0π中🙂").Checked();
                var executor=new Executor(1);executor.Add(sub);
                for(int i=0;i<Polls&&received==null;i++) { Result r=executor.SpinOnce();if(r.Error!=0&&r.Error!=11)r.Checked(); }
                if(received!="C# ROS \0π中🙂")throw new InvalidOperationException("independent native/transport evidence still required; no exact value received");
                Console.WriteLine("guest full String received; not by itself native delivery proof");code=0;break;
            case "service":
                if(args.Length!=2)throw new ArgumentException("service call-alias");
                var client=node.CreateClient(Codecs.Trigger,Codecs.ServiceResponse,args[1],1024);var response=client.Call(default);
                if(!response.Success)throw new InvalidOperationException("native Service returned false");Console.WriteLine(response.Message);code=0;break;
            case "serve":
                if(args.Length!=3)throw new ArgumentException("serve advertise-alias respond-alias");
                var server=node.CreateService(Codecs.Trigger,Codecs.ServiceResponse,args[1],args[2],1024,_=>new Codecs.Response(true,"actual managed callback"));
                bool responded=false;for(int i=0;i<Polls&&!responded;i++) { Result r=server.PollOnce();if(r.Error==0)responded=true;else if(r.Error!=11)r.Checked(); }
                if(!responded)throw new TimeoutException("bounded caller-paced Service poll ended; no replay");code=0;break;
            case "observe":
                if(args.Length!=3)throw new ArgumentException("observe observer-alias source-subscribe-alias");
                Capability source=session.Open(args[2]);var observer=node.CreateObserver(args[1],2048);observer.Watch(source).Checked();source.Invoke("topic_subscribe").Checked();
                bool seen=false;for(int i=0;i<Polls&&!seen;i++) { Result r=observer.PollOnce();if(r.Error==11)continue;r.Checked();RosEvent e=observer.LastEvent!;if(e.Kind!=1)throw new InvalidOperationException("resource state/data kind differs");string value=Codecs.String.Decode(e.Payload);Console.WriteLine(value);seen=true; }
                if(!seen)throw new TimeoutException("no complete source-bound event; no implicit wait/authority");code=0;break;
            case "action":
                if(args.Length!=5)throw new ArgumentException("action send cancel-own feedback result aliases");
                var action=node.CreateActionClient(new GoalCodec(),new SequenceCodec(),new SequenceCodec(),args[1],args[2],args[3],args[4]);ulong id=action.SendGoal(5);bool done=false;
                for(int i=0;i<Polls&&!done;i++) {
                    try { var completion=action.ReceiveResult(id,196);if(completion.Outcome!=2)throw new InvalidOperationException("Action did not succeed");int[] expected={0,1,1,2,3,5};if(completion.Value.Length!=expected.Length)throw new FormatException("complete Fibonacci length");for(int j=0;j<expected.Length;j++)if(completion.Value[j]!=expected[j])throw new FormatException("full Fibonacci value");done=true; }
                    catch(RosException e) when(e.Result.Error==11) { }
                }
                if(!done)throw new TimeoutException("bounded Action poll ended; effects may remain; no automatic cancel/replay");code=0;break;
            default:throw new ArgumentException("unknown example");
            }
        } catch(Exception error) { primary=error;Console.Error.WriteLine(error);code=1; }
        finally {
            // Unlike IDisposable during exception unwinding, retain primary and every cleanup outcome separately.
            try { CleanupReport cleanup=session.CloseChecked(primary);for(int i=0;i<cleanup.Count;i++)if(cleanup.Items[i].Exception!=null||cleanup.Items[i].Result?.Error!=0) { Console.Error.WriteLine("cleanup failure at owner index "+i);code=1; } }
            catch(Exception cleanup) { Console.Error.WriteLine("cleanup refusal: "+cleanup);code=1; }
        }
        return code;
    }
}
