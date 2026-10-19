# Complete AssemblyScript semantic contracts

`assemblyscript_semantic_contract.ts` imports the actual generated KUKA complete
SDK. Generate `sdk/custom-types/kuka-ros2-complete` from the retained original
KUKA catalog with the current captured parent, bounded producer, vendor adapter
and all four RPC templates before compiling this contract. Use the genuine
AssemblyScript SDK/compiler route documented in `sdk/assemblyscript/README.md`;
keep the module at its checked relative path or adjust only the import path in
an isolated test workspace. Export and invoke `run_contract`; success is exactly
zero. Keep compiler, complete Wasm bytes and runtime invocation receipts.

The contract exercises normalized ROS2 Time/Duration at 999999999 and 1000000000
nanoseconds, valid negative seconds, optional JointState arrays, and mismatching
nonempty JointState arrays. It checks both typed encoder rejection and independent
malformed canonical wires, and requires failed checked decoders to publish no
value. The test uses complete original standard records from the selected KUKA
catalog. It does not substitute an interface definition, Native type identity,
Host stub or pre-authorized capability. It has not been compiled or executed as
part of Source implementation.
