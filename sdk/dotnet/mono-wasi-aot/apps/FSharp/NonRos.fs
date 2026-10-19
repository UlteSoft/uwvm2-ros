module UwvmAotFSharpNonRos
open System
open System.Runtime.CompilerServices
[<Struct>]
type Sample = { Signed: int64; FullId: uint64; Fraction: double }
[<MethodImpl(MethodImplOptions.NoInlining)>]
let frameThree input sample =
    let typedLocal: int64 = Checked.(+) input sample.Signed
    Console.Error.WriteLine("UWVM_AOT_DBG_READY fsharp")
    let control = Console.Read() // DBG_STOP_FSHARP: caller release G follows inspection.
    if control <> int 'G' then failwith "explicit debugger release G required"
    typedLocal
[<MethodImpl(MethodImplOptions.NoInlining)>]
let frameTwo input sample =
    let result = frameThree input sample
    Console.Error.WriteLine("UWVM_AOT_DBG_RESUME_TWO")
    result
[<MethodImpl(MethodImplOptions.NoInlining)>]
let rec frameOne input sample depth =
    let result = if depth = 0 then frameTwo input sample else frameOne input sample (depth - 1)
    Console.Error.WriteLine("UWVM_AOT_DBG_RESUME_DEPTH=" + string depth)
    result
[<EntryPoint>]
let main args =
    try
        if args.Length <> 2 then invalidArg "args" "signed-int64 expected-nonASCII-label"
        let number = Int64.Parse(args.[0], Globalization.CultureInfo.InvariantCulture)
        let sample = { Signed = -7L; FullId = UInt64.MaxValue; Fraction = 1.25 }
        let result = frameOne number sample 2
        printfn "label=%s result=%d full-id=%d fraction=%f" args.[1] result sample.FullId sample.Fraction
        0
    with error ->
        Console.Error.WriteLine(error)
        1
