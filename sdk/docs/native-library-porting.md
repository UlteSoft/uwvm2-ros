# Port native libraries into a capability-scoped Wasm application

Keep the application's portable code inside Wasm and put hardware, DDS and other
native operating-system integration behind the trusted ROS Gateway. This guide
explains what happens to each kind of existing library. The
[source migration guide](source-migration.md) selects the actual client-library
route; the [application tutorial](complete-application-tutorial.md) covers new
programs in every SDK language.

## What changes at the boundary

In a conventional native ROS process, application code, plugins and linked ROS
libraries share the process address space and operating-system credentials.
A topic name, C++ class, Node object or DDS QoS profile does not establish an
in-process memory sandbox. DDS security and separate operating-system processes
can impose additional boundaries, but native code within one process still has
that process's memory and syscall access.

After migration, the application executes in a Wasm instance. Its pointers are
offsets in its admitted linear memory. A ROS operation crosses a registered
numeric import; the Runtime derives the actual caller and checks the selected
backend, incarnation, alias, operation, resource, type, limits and ownership.
The Gateway owns the native ROS objects. Guest code cannot turn a native pointer,
guessed handle, codec, Node name or copied policy text into authority.

This is a boundary design, not a claim that a finite audit proves every VM,
compiler, operating-system or middleware implementation free of defects. The
Runtime, compiler, native Gateway, selected ROS/RMW libraries and trusted launch
configuration remain part of the trusted computing base. Their exact versions,
defaults, negative tests and audit results matter.

## Decide what to do with an existing artifact

| Existing artifact | Wasm migration | Result and verification |
| --- | --- | --- |
| C/C++ source and headers | Recompile portable translation units and their dependencies with the selected Wasm compiler/sysroot | Wasm object files; preserve the public source API only where the selected adapter implements it |
| Native ELF `.so` or Windows `.dll` | Rebuild portable source for Wasm, or keep the native implementation in a separately admitted Gateway adapter | A native shared object is not a Guest Wasm library; no Guest `dlopen` escape is supplied |
| Native `.a` or `.lib` archive | Rebuild every needed member for the Wasm target and archive those objects | A `.a` filename alone says nothing about member architecture; inspect members and final link inputs |
| Wasm-target static `.a` | Link its admitted Wasm objects into the Guest | Check the compiler ABI, libc/C++/exception/thread model, imported functions and final Wasm features |
| Header-only C++ library | Compile its templates as part of the Guest | Audit instantiated OS/native assumptions and allocation/lifetime requirements; header-only does not mean permission-free |
| ROS generated message/type support | Regenerate for the selected Guest toolchain and source profile | Guest object layout stays local; native type identity is checked independently at the Gateway |
| Python native extension wheel | Port the C extension and statically link it into the matching CPython.wasm, or replace it with a checked Guest implementation | Native `_rclpy*.so` is not usable as a Guest extension |
| Java/JNI library pair | Lower application bytecode and the Wasm-compatible Java facade through the selected producer | Native JNI addresses and native rcljava handles do not cross the Guest ABI |
| C# P/Invoke DLL | Build the explicit Wasm C bridge and use the pinned Mono static-link/PInvoke-table route | `DllImport` names resolve to the actual statically linked Guest object, not a host DLL loader |
| Robot/vendor native SDK | Keep hardware transport in a trusted, narrowly configured native adapter; move portable application logic into Wasm | Separate native ABI/version/device qualification and per-operation Guest permissions are required |

An archive rebuilt for x86_64 Linux is still a native archive. Rebuilding it for
aarch64 Linux does not make it a Wasm archive. Conversely, a valid portable Wasm
application can run on different host ISAs only when that host's Runtime supports
its actual Wasm features and imports. Native Gateway libraries are rebuilt and
qualified for each host architecture.

## Rebuild a static portable dependency

Use the compiler, archiver, linker and sysroot from one admitted target
toolchain. The following commands illustrate the actual object/archive boundary;
all paths are explicit inputs and outputs in the guarded Linux build directory:

```sh
"$WASI_CC" --target=wasm32-wasi --sysroot="$WASI_SYSROOT" \
  -O2 -g -I "$DEPENDENCY/include" -c "$DEPENDENCY/src/value.c" \
  -o "$OUT/value.o"
"$WASI_AR" rcs "$OUT/libvalue.a" "$OUT/value.o"
"$WASI_CC" --target=wasm32-wasi --sysroot="$WASI_SYSROOT" \
  -O2 -g -I "$SDK/include" -I "$DEPENDENCY/include" \
  "$APP/main.c" "$OUT/libvalue.a" -o "$OUT/application.wasm"
"$WASM_TOOLS" validate "$OUT/application.wasm"
```

Retain full dependency files and the actual link trace. A successful archive
creation is not a successful final executable. Resolve every required symbol;
do not ignore undefined symbols or supply successful no-op replacements for
unavailable clocks, locks, sockets, type support or native loaders. Inspect the
final Wasm imports before granting anything.

For original ROS C/C++ libraries, use the
[static upstream build recipe](../rmw_wasm/cmake/BUILD.md). It separates host
generators from target libraries, disables dynamic RMW selection explicitly and
keeps the original package dependency graph. Do not place the finite facade
headers on that route's include path. The original rclcpp route still has
documented threading/default-endpoint blockers; a finite facade example is a
different qualification result.

## Keep native integration outside Guest authority

A trusted operator may install a native Gateway adapter with the actual ROS
headers, typesupport, SDK and device access. That code executes with native
process authority and requires its own review. The Guest receives only explicit
business-operation aliases, not a filename, native function pointer, plugin
loader, arbitrary syscall forwarding service or network socket.

Strict ROS Worker validation currently rejects extra native DL/weak plugins,
socket standard streams, inherited host environment and WASI directory/socket
preopens. The separately authenticated Gateway channel carries the admitted ROS
protocol. Do not bypass those checks to make an unported dependency start.

Use canonical complete values at the boundary. A C struct layout, C++ vtable,
Python object, Java reference, C# object or MoonBit GC value is not a wire
format. Preserve integer widths and little-endian slots; bounds and native type
identity must agree on both sides. See [the ABI reference](abi-v1.md) and
[wire format](wire-format.md).

## Acceptance before replacing a native deployment

Use the same application behavior and independent native peers for the old
native application and the Wasm application. Check complete payloads, ordering,
errors, deadlines, QoS, shutdown and permission denials. Preserve unchanged
upstream application sources where that is the claimed migration boundary.
If the source is adapted, identify the changes and qualify that distinct target.

Then measure startup, first compilation, warm execution, transport/copy cost,
latency distribution, throughput and peak memory. The
[platform and performance guide](platform-qualification.md) separates native
VTune hardware observations from QEMU emulator overhead. A library that links
but stalls its executor or makes a non-idempotent retry is not an acceptable
migration.
