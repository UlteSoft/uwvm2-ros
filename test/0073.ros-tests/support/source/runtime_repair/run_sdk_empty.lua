-- @file run_sdk_empty.lua
-- @brief Exercise the SDK's empty-authority launcher through actual Xmake Lua.
-- @details The caller supplies the exact SDK, runtime and independently encoded
-- Wasm probe. The production helper selects checked native unwind and retains
-- its explicit per-module denials. No build, gateway or device is provisioned.
-- @param[in] sdk_root Absolute SDK source directory under qualification.
-- @param[in] runtime Absolute source-paired uwvm2-ros executable.
-- @param[in] artifact Absolute real Wasm guest containing its own assertions.
function main(sdk_root, runtime, artifact)
    assert(path.is_absolute(sdk_root) and os.isdir(sdk_root), "An actual SDK root is required")
    assert(path.is_absolute(runtime) and os.isfile(runtime), "An actual runtime is required")
    assert(path.is_absolute(artifact) and os.isfile(artifact), "An actual Wasm guest is required")
    local backend = import("tools.platform.nvidia.examples.build_backend", {rootdir = sdk_root})
    backend.run_empty(
        {runtime = runtime},
        {execution = "guest-empty-authority", required_permissions = {}},
        {artifacts = {{name = "main", kind = "wasm", path = artifact}}}
    )
end
