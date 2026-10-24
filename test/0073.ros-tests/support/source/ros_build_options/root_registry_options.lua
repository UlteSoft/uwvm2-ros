-- @file root_registry_options.lua
-- @brief Inspect effective registry definitions in the real root Xmake project.
-- @details Run with `xmake lua` after configuring the repository. This check
-- reads actual CLI, runtime and test targets instead of reconstructing their
-- build rules in a synthetic project. It never builds or runs a ROS deployment.

-- @brief Verify one effective definition without accepting conflicting copies.
-- @param definitions Complete definition list from an actual Xmake target.
-- @param name Preprocessor identifier whose effective value is required.
-- @param expected Exact replacement text, or nil when the macro must be absent.
local function verify_definition(definitions, name, expected)
    local observed = nil
    for _, definition in ipairs(definitions) do
        if definition == name or definition:sub(1, #name + 1) == name .. "=" then
            local value = definition == name and "1" or definition:sub(#name + 2)
            assert(observed == nil or observed == value, "Conflicting definitions for " .. name)
            observed = value
        end
    end
    assert(observed == expected,
           name .. ": expected " .. tostring(expected) .. ", observed " .. tostring(observed))
end

-- @brief Check the configured target graph and write a machine-readable receipt.
-- @param output Receipt path outside the source distribution.
-- @param interfaces Expected compiled interface count.
-- @param cells Expected aggregate schema cell count.
-- @param backend Explicit import selection: none, ros1, ros2, or both.
-- @param memory64 Whether the WASI memory64 extension is compiled: yes or no.
function main(output, interfaces, cells, backend, memory64)
    import("core.base.json")
    import("core.project.config")
    import("core.project.project")
    config.load()
    local targets = project.targets()
    assert(targets["uwvm2-ros"] and targets["uwvm_runtime"], "Load the actual root project")
    local report = {format = "uwvm-root-registry-options-v1", targets = {}}
    for name, target in pairs(targets) do
        local definitions = target:get("defines") or {}
        if table.contains(definitions, "UWVM=2") then
            verify_definition(definitions, "UWVM_ROS_MAX_CUSTOM_INTERFACES", interfaces)
            verify_definition(definitions, "UWVM_ROS_MAX_CUSTOM_CELLS", cells)
            verify_definition(definitions, "UWVM_IMPORT_ROS1",
                              (backend == "ros1" or backend == "both") and "1" or nil)
            verify_definition(definitions, "UWVM_IMPORT_ROS2",
                              (backend == "ros2" or backend == "both") and "1" or nil)
            verify_definition(definitions, "UWVM_ENABLE_LOCAL_IMPORTED_WASIP1_WASM64",
                              memory64 == "yes" and "1" or nil)
            report.targets[name] = {defines = definitions, sources = target:sourcefiles()}
        end
    end
    assert(report.targets["uwvm2-ros"] and report.targets["uwvm_runtime"],
           "Both production targets must carry the same registry definitions")
    report.status = "PASS"
    json.savefile(output, report)
end
