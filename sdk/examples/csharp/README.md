# C# capability examples and local policy sources

All files here are SOURCE IMPLEMENTATION, NOT EXECUTED. The real generic Mono
native hook's success is background ABI evidence, not a ROS SDK/runtime/delivery
result for these files. No compiler or model was invoked by this author.
The actual pinned build/API details live in ../../csharp/README.md. Stage the
entire same-byte sdk/csharp plus this directory; select one backend for the
whole project and its original roswasm_bridge.o. Source/table/native/SDK
association, all dependencies, actual PInvoke table/link/Wasm and process owner
checks are mandatory before invocation.

Program.cs is a real managed command entry with bounded256 caller-paced polls.
Root must start an independently checked native peer, finish its ready barrier,
and choose fresh Instance authority before command launch. Supply alias strings
and original native identities through the real Runtime/Gateway policy, never
by embedding grants in managed source. A local Node or an import list grants
nothing. Run each case in a fresh Instance; no one command inherits another's
queue/sequence/resources. The current built-in String includes embedded NUL
and Unicode; only exact whole values reach the callback example.

Unexecuted guest argument shapes, after selecting the actual output Wasm:

* `deny deliberately-ungranted-alias` requires the genuine open errno13 in a
  fresh default-denied Instance and zero acquired owners. Version1 is checked
  first; local maximum-owner90 is not substituted for actual default denial.
* `topic publish-alias subscribe-alias` subscribes once, publishes the complete
  fixed String once, then requires the matching complete value. Native source
  identity and delivery must be established separately, not by its print marker.
* `service call-alias` sends one empty Trigger request and decodes the entire
  Bool+String response. Errors90/110 are never repeated.
* `serve advertise-alias respond-alias` uses separate aliases, decodes the real
  ticket and full Trigger request, calls the managed handler and responds once.
* `observe observer-alias subscribe-alias` explicitly watches/activates one
  source and accepts one complete source-bound topic event. State/error events
  are not a successful data delivery. Repeated polls are caller-driven idle
  scheduling, not an automatic replay of an operation that failed90/110.
* `action send-alias cancel-own-alias feedback-alias result-alias` sends the
  actual builtin Fibonacci goal5, waits by bounded polls for matching successful
  terminal outcome2 and validates all six sequence values. The separately owned
  native fixture must implement that exact complete contract. Cancel-own does
  not imply cancel-any; a stalled/failed test does not silently cancel/retry or
  establish a physical robot hold. Typed incoming ActionServer is also real
  SDK code, requiring independent native-client and callback tests next.

Every command retains the application primary and an independently inspectable
Session.CloseChecked cleanup report. Session.CloseChecked attempts each live
owner once despite ordinary managed exceptions; whole lane success also needs
Runtime disconnection/normal Frame.finish and no live owned processes. A print,
count or policy result cannot establish ROS network/native/hardware acceptance.

## Unexecuted pure policy model

PolicyContract.cs supplies16 named local cases over an explicitly labelled
in-memory ITransport model. They check complete UTF8/NUL/BOM, malformed UTF8,
Bool framing, full Twist and finite/signed-zero values, int64 high bits, actual
versioned event header/reserved/extent, a global two-source observer sequence,
no90/110 replay, UINT64_MAX opaque ownership, cleanup continuation, busy callback
preflight, whole-alias revocation, factory rollback and scalar array framing.
They are not native calls, fake ROS success receipts or compiler/SDK proofs.
Root alone may later run them in a genuinely owned local test process:

```sh
"$DOTNET" build "$SOURCE/sdk/examples/csharp/PolicyContract.csproj" \
  -p:RosBuildRoot="$POLICY_OUT" --no-restore -m:1
"$DOTNET" "$POLICY_OUT/bin/PolicyContract/ros-csharp-local-policy-only.dll"
```

The SDK's handwritten classes and full native bridge still require actual
compilation. The source-author supplied0 executed policy cases and0 ROS cases.
Static ABI/DATA checks cannot substitute for real pin lifetime under GC,
allocator-failure cleanup, native slot behavior, .NET trimming/PInvoke tables,
all Runtime backends, native ROS identity/delivery and official/vendor tests.
The project's larger language inventory is extensible; a C# builtin SDK does
not claim generated custom/vendor C# codecs or rcldotnet compatibility.
