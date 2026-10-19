using System;
using Uwvm.RosWasm.Owned;
internal static class AppCommon {
    private static void Attempt(string name,Uwvm.RosWasm.OwnedAttempt a) { Console.Error.WriteLine(name+" entered="+a.Entered+" returned="+a.Returned+" actual-code="+a.Code+" exception="+a.Exception+" cleanup-exception="+a.CleanupException); }
    internal static int Steps(string value) { int n=int.Parse(value);if(n<1||n>65536)throw new ArgumentException("explicit1..65536 local poll work bound");return n; }
    internal static int Finish(OwnedGroup group,Exception? primary) {
        bool cleaned=false;Exception? cleanup=null;
        try { cleaned=group.CloseAll(); }catch(Exception error) { cleanup=error; }
        // Printing is diagnostic only and happens after every cleanup attempt.
        try {
            if(primary!=null)Console.Error.WriteLine("primary: "+primary);
            if(cleanup!=null)Console.Error.WriteLine("cleanup: "+cleanup);
            for(int i=0;i<group.AttemptedEndpoints;i++) {
                var d=group.Diagnostic(i);Console.Error.WriteLine("endpoint "+i+" full-source="+d.Source+" primary="+d.PrimaryCode+" retire-entered="+d.NativeRetire.Entered+" retire-returned="+d.NativeRetire.Returned+" retire-code="+d.NativeRetire.Code);
                Console.Error.WriteLine("saved-primary="+d.PrimaryException+" close-hook="+d.CloseHookException);
                for(int owner=0;owner<3;owner++) { Attempt("raw-open-"+owner,d.Opens[owner]);Console.Error.WriteLine("returned-full-open-handle="+d.ReturnedOpenHandles[owner]); }
                Attempt("activation",d.Activation);Attempt("inspection",d.Inspection);Attempt("submit",d.Submit);Attempt("respond",d.Respond);Attempt("peek",d.Peek);Attempt("commit",d.Commit);Attempt("cancel",d.Cancel);Attempt("abandon",d.Abandon);Attempt("available",d.Available);
                foreach(var attempt in d.NativeRetireHistory)Attempt("native-retire",attempt);
                foreach(ulong retained in d.RemainingRawOwners)Console.Error.WriteLine("remaining-full-raw-owner="+retained);
                foreach(var close in d.RawCloses)Console.Error.WriteLine("raw-close full-id="+close.FullHandle+" code="+close.Result?.Error+" phase="+close.Retirement.Phase+" exception="+close.Exception);
            }
        }catch(Exception) { return 1; }
        return primary==null&&cleanup==null&&cleaned?0:1;
    }
}
