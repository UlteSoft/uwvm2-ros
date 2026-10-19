# Actual ROS2 native BoundedVector compatibility

This narrow source change removes a concrete native-header compile blocker.
The bounded generator previously included only
`rosidl_runtime_cpp/bounded_vector.hpp` and specialized only
`rosidl_runtime_cpp::BoundedVector`. Actual Ardent, Bouncy and Dashing headers
provide `rosidl_generator_cpp/bounded_vector.hpp` and
`rosidl_generator_cpp::BoundedVector` instead. Modern ROS2 uses the runtime
header and namespace. The generated header now selects the modern header first,
then the actual older header when the modern one is absent. The specialization
uses exactly the same selection. Missing both headers is an explicit compile
error. There is no global namespace alias, copied container, stub header,
ordinary-vector substitution or SDK-version guess.

The modern specialization retains the same element/allocator/bound pattern;
only an explicit global qualification is added. Ordinary `std::vector`, fixed
array traits, Lyrical buffer handling, native shape/identity checks, all Guest
codecs, catalog formats, native exported hashes and endpoint registration are
unchanged. This patch changes only `emit_native` in the production generator.
ROS1 generation does not use either new conditional branch.

The historical generated SDK bundles and their receipts are not regenerated.
Applications must generate a new bundle from actual qualified source and native
identities before compiling it with a selected SDK. A newly emitted header is
not proof of all-release compatibility or native ROS delivery.

The independent actual-SDK contract is
`native_bounded_vector_contract.cc`. It includes the real freshly generated
`native_ros2.hpp`, instantiates the real selected allocator-aware SDK container
and checks the generated trait's exact element and bound. It also retains the
unbounded ordinary-vector trait and rejects an unrelated fixed container as a
sequence. Eleven runtime checks exercise only small actual SDK container
operations, including both successful boundary operations and refused
out-of-bound operations with preserved contents. No native message type,
container or type-support identity is fabricated. The source creates no ROS
Node, endpoint, Guest, network route or robot command.

Use the owned Linux lane with the actual frozen source, selected compiler/SDK,
fresh native-bound generated directory and fresh output directory. The following
is an invocation shape for the Root owner to construct from actual selected
roles, not an executed command or permission to use ambient toolchains:

```sh
"$ACTUAL_CXX" -std=gnu++17 -O1 -g0 -UNDEBUG -MD \
  -MF "$FRESH_OUT/native_bounded_vector_contract.o.d" \
  -MT "$FRESH_OUT/native_bounded_vector_contract.o" \
  "${ACTUAL_SDK_COMPILE_FLAGS[@]}" \
  -I"$FROZEN_SOURCE" -I"$FROZEN_SOURCE/src" -I"$ACTUAL_GENERATED" \
  -c "$FROZEN_SOURCE/tests/ros_custom_types/bounded-v2/native_bounded_vector_contract.cc" \
  -o "$FRESH_OUT/native_bounded_vector_contract.o"
"$ACTUAL_CXX" "$FRESH_OUT/native_bounded_vector_contract.o" \
  -Wl,-t "${ACTUAL_SDK_LINK_FLAGS[@]}" \
  -o "$FRESH_OUT/native_bounded_vector_contract"
# The existing owner must select the actual native loader or separately qualified
# target QEMU, prove the process/maps, and record its controlled execution.
```

`ACTUAL_SDK_COMPILE_FLAGS` and `ACTUAL_SDK_LINK_FLAGS` are arrays supplied from the
real selected compiler/SDK recipe, not shell-split invented strings. Do not add a
fake include directory to make either branch available. The full dependency file
must prove the current generator-derived `native_ros2.hpp`, its real records and
all actual selected SDK headers; object target/primary spelling and every library
remain qualified by the existing owner. The selected header marker is a
readable result, while actual MD, ELF, maps, input/root preservation and cleanup
provide the physical association. Native success requires return code zero,
exact marker `PASS_NATIVE_BOUNDED_VECTOR_HEADER_TRAITS`, `checks=11` and
`failures=0`; a marker alone is insufficient. A target executable is never run
directly as a foreign ELF. Missing SDK/compiler/loader or a failed build is
unverified or failed, never a successful compatibility test.

The generated native bundle may use the complete standard Header/JointState
mode or a genuine existing custom package. It must be newly native-bound with
actual identities. The contract verifies container/trait compatibility; the
ordinary complete native message roundtrip and actual Gateway/ROS acceptance
are still independent requirements.

This candidate was authored and statically inspected only. No generator,
compiler, contract, Python policy suite, ROS process, QEMU or SSH call was run.
The actual future source MF and generated artifacts are not predicted.

Primary source references:

- [Ardent actual bounded container](https://raw.githubusercontent.com/ros2/rosidl/ardent/rosidl_generator_cpp/include/rosidl_generator_cpp/bounded_vector.hpp)
- [Bouncy actual bounded container](https://raw.githubusercontent.com/ros2/rosidl/bouncy/rosidl_generator_cpp/include/rosidl_generator_cpp/bounded_vector.hpp)
- [Dashing actual bounded container](https://raw.githubusercontent.com/ros2/rosidl/dashing/rosidl_generator_cpp/include/rosidl_generator_cpp/bounded_vector.hpp)
- [Jazzy actual bounded container](https://raw.githubusercontent.com/ros2/rosidl/jazzy/rosidl_runtime_cpp/include/rosidl_runtime_cpp/bounded_vector.hpp)
