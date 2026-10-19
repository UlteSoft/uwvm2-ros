module UwvmAotFSharp
open System
open Uwvm.RosWasm.Aot
open Uwvm.RosWasm.Owned
open Uwvm.RosWasm.Generated.Control
type TQ = BoundedRecords._Ros2Type_description_interfacesSrvGetTypeDescription_Request
type Get = BoundedRecords._Ros2Rcl_interfacesSrvGetParameters_Request
type Set = BoundedRecords._Ros2Rcl_interfacesSrvSetParameters_Request
type Parameter = BoundedRecords._Ros2Rcl_interfacesMsgParameter
type ParameterValue = BoundedRecords._Ros2Rcl_interfacesMsgParameterValue

let typeQuery (life: ApplicationLifetime) (a: string array) =
    if a.Length <> 7 then invalidArg "args" "type-query call observe available-or-- native-type RIHS steps"
    let credits = ApplicationLifetime.Steps(a.[6])
    let definition = ControlRoots.Ros2Type_description_interfacesSrvGetTypeDescription()
    life.Group <- OwnedGroup(1, 1, definition.MaximumRequest, definition.MaximumResponse)
    let client = life.Group.OpenClient(definition, a.[1], a.[2], (if a.[3] = "-" then null else a.[3]))
    if a.[3] <> "-" && not (client.Available(0UL)) then failwith "actual separately granted availability false"
    let request = TQ()
    request.type_name <- a.[4]
    request.type_hash <- a.[5]
    request.include_type_sources <- true
    let ticket = client.Submit(request) // DBG_ROS_SUBMIT_FSHARP
    let mutable completed = false
    for _ in 1 .. credits do
        if not completed then
            let delivery = client.Poll()
            if delivery.Present then
                if delivery.IsControl || delivery.Ticket.Value <> ticket.Value || not delivery.Value.successful || delivery.Value.type_sources.Length = 0 then
                    failwith "actual complete TypeQuery refusal/correlation/control"
                printfn "source-files=%d referenced=%d native-seq=%d" delivery.Value.type_sources.Length delivery.Value.type_description.referenced_type_descriptions.Length delivery.Information.NativeSequence // DBG_ROS_DELIVERY_FSHARP
                completed <- true
    if not completed then
        client.Abandon(ticket)
        failwith "caller credits exhausted; abandonment is not DDS cancellation or resend"

let getParameter (life: ApplicationLifetime) (a: string array) =
    if a.Length <> 5 then invalidArg "args" "get call observe parameter-name steps"
    let credits = ApplicationLifetime.Steps(a.[4])
    let definition = ControlRoots.Ros2Rcl_interfacesSrvGetParameters()
    life.Group <- OwnedGroup(1, 1, definition.MaximumRequest, definition.MaximumResponse)
    let client = life.Group.OpenClient(definition, a.[1], a.[2], null)
    let request = Get()
    request.names <- [|a.[3]|]
    let ticket = client.Submit(request)
    let mutable completed = false
    for _ in 1 .. credits do
        if not completed then
            let delivery = client.Poll()
            if delivery.Present then
                if delivery.IsControl || delivery.Ticket.Value <> ticket.Value || delivery.Value.values.Length <> 1 then failwith "complete GetParameters failed"
                let value = delivery.Value.values.[0]
                printfn "type=%d signed-int64=%d" value.``type`` value.integer_value
                completed <- true
    if not completed then
        client.Abandon(ticket)
        failwith "caller credits exhausted"

let setParameter (life: ApplicationLifetime) (a: string array) =
    if a.Length <> 6 then invalidArg "args" "set call observe parameter-name signed-int64 steps"
    let credits = ApplicationLifetime.Steps(a.[5])
    let number = Int64.Parse(a.[4], Globalization.CultureInfo.InvariantCulture)
    let definition = ControlRoots.Ros2Rcl_interfacesSrvSetParameters()
    life.Group <- OwnedGroup(1, 1, definition.MaximumRequest, definition.MaximumResponse)
    let client = life.Group.OpenClient(definition, a.[1], a.[2], null)
    let value = ParameterValue()
    value.``type`` <- 2uy
    value.integer_value <- number
    let parameter = Parameter()
    parameter.name <- a.[3]
    parameter.value <- value
    let request = Set()
    request.parameters <- [|parameter|]
    let ticket = client.Submit(request)
    let mutable completed = false
    for _ in 1 .. credits do
        if not completed then
            let delivery = client.Poll()
            if delivery.Present then
                if delivery.IsControl || delivery.Ticket.Value <> ticket.Value || delivery.Value.results.Length <> 1 || not delivery.Value.results.[0].successful then failwith "actual SetParameters rejected"
                completed <- true
    if not completed then
        client.Abandon(ticket)
        failwith "caller credits exhausted"

[<EntryPoint>]
let main args =
    let life = ApplicationLifetime()
    try
        AotWitness.CheckGuestRoots()
        if args.Length < 1 then invalidArg "args" "type-query|get|set required"
        match args.[0] with
        | "type-query" -> typeQuery life args
        | "get" -> getParameter life args
        | "set" -> setParameter life args
        | _ -> invalidArg "args" "unsupported caller mode"
    with error -> life.Primary <- error
    life.Finish()
