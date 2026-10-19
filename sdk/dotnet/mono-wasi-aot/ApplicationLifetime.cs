using System;
using Uwvm.RosWasm.Owned;
namespace Uwvm.RosWasm.Aot;
/// <summary>Real application-owned failure carrier. Reserve and retain before
/// any open. All diagnostics remain on the actual Group, even if printing fails.
/// This object grants no authority and never retries an entered cleanup.</summary>
public sealed class ApplicationLifetime {
    public static ApplicationLifetime? Last;
    public OwnedGroup? Group;
    public Exception? Primary,CloseError,DiagnosticError;
    public bool CloseStarted,Finished,Cleaned;
    public int ExitCode=1;
    public ApplicationLifetime() { Last=this; }
    public int Finish() {
        if(Finished)return ExitCode;
        if(CloseStarted)return 1;
        CloseStarted=true;
        try { if(Group!=null)Cleaned=Group.CloseAll(); }
        catch(Exception error) { CloseError=error; }
        ExitCode=Primary==null&&CloseError==null&&Group!=null&&Cleaned?0:1;
        try {
            if(Primary!=null)Console.Error.WriteLine(Primary);
            if(CloseError!=null)Console.Error.WriteLine(CloseError);
            if(Group!=null)for(int i=0;i<Group.AttemptedEndpoints;i++) {
                var d=Group.Diagnostic(i);
                Console.Error.WriteLine("full-source="+d.Source+" primary-code="+d.PrimaryCode+" primary="+d.PrimaryException+" close-hook="+d.CloseHookException);
                foreach(var n in d.NativeRetireHistory)Console.Error.WriteLine("native-retire entered="+n.Entered+" returned="+n.Returned+" code="+n.Code+" exception="+n.Exception);
                foreach(var c in d.RawCloses)Console.Error.WriteLine("raw-close full-id="+c.FullHandle+" errno="+c.Result?.Error+" phase="+c.Retirement.Phase+" exception="+c.Exception);
                foreach(ulong id in d.RemainingRawOwners)Console.Error.WriteLine("still-owned-full-id="+id);
            }
        }catch(Exception error) { DiagnosticError=error;ExitCode=1; }
        Finished=true;return ExitCode;
    }
    public static int Steps(string value) {
        int n=int.Parse(value,System.Globalization.CultureInfo.InvariantCulture);
        if(n<1||n>65536)throw new ArgumentOutOfRangeException(nameof(value),"finite1..65536 caller work credits required");
        return n;
    }
}
