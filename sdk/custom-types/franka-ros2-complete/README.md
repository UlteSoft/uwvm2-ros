# Franka ROS2 complete selected records

This directory contains all fourteen unchanged generated source products for
the selected official Jazzy graph at commit
4acc05c01ebfcdf916ee6b3df966df0ad7b2f9ec. It retains 47 complete original definitions, 73 expanded
message/service/action records and 11 constants, together with the original
generation and preparation receipts. Source generation is separate from native
ROS, FCI, controller, model, firmware and upstream-suite qualification.

Start with the [complete vendor kit](../../robots/industrial/franka/README.md).
It explains actual imports, permissions, whole state, raw/typed examples, all
six producer commands, explicit source selection and stop/unknown-outcome rules.

## Select one coherent source domain

C11 uses bounded_records.h and bounded_interfaces.h. C++20 uses the corresponding
.hpp files and roswasm_franka::Reader from the kit. Rust uses .rs; JavaScript
uses .mjs; AssemblyScript uses .ts; TinyGo uses .go. Each file retains the full
original fields and declared constants/defaults. Call an explicit default
factory when the original declares defaults; ordinary zero initialization is
not a substitute. The generated interfaces preserve original service request/
response and action goal/result/feedback values and protocol facades.

Compile this exact family with UWVM_FRANKA_VARIANT=ros2. Do not combine
independently generated record headers or copy an incompatible native type
identity. Dynamic strings/sequences have finite canonical capacities; they are
not the entire value domain of an unbounded native ROS field. The catalog,
source lock, original definitions and licenses are retained in the vendor kit.

## Keep source and authority separate

generation.vendor.json binds each exact generated product, complete member maps,
constants/defaults and the original producer inputs. preparation.vendor.json
retains the actual immutable source-preparation provenance. Preserve these
historical receipts; authored documentation is not an additional generated
code product or execution result.

native_identity.cc and native_constants.hpp are genuine exporter/check inputs.
They are not an installed native SDK, admitted Native identity, Runtime registry
or grant. Independently build and bind against the exact selected native SDK,
then select one coherent reviewed domain for Runtime and Gateway. Every Guest
starts with no ROS authority. Neither importing these files nor constructing a
value can grant robot, file, network, debug, update or policy access.
