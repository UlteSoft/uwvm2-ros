-- Real xmake module load, route/refusal and local binding/argv contract.
-- Spies below cannot earn actual compiler-process, Wasm or native ROS credit.
function main(source_root, output_json)
    local sdk = path.join(path.absolute(source_root), "sdk/origin")
    local managed = import("tools.xmake.managed", {rootdir = sdk, anonymous = true})
    local examples = import("tools.xmake.examples.managed", {rootdir = sdk, anonymous = true})
    local json = import("core.base.json")
    local checks = 0
    local function check(value) checks = checks + 1; assert(value, "managed review02 local contract " .. checks) end
    local function refuses(call)
        local returned = false
        try {function() call(); returned = true end, catch {function(_errors) end}}
        check(not returned)
    end
    local request_profile = {target = "wasm32-wasip1", address_bits = 32, wasi = "wasip1",
        core_baseline = "3.0", startup = "command", exceptions = false, threads = false}
    local bindings, forwarded = 0, nil
    local common = {}
    function common.bind(script_import)
        check(type(script_import) == "function")
        bindings = bindings + 1
        function common.request(language, options)
            return {format = "uwvm-sdk-build-request-v1", language = language,
                profile = request_profile, language_options = options}
        end
        function common.resolve(request)
            return {format = "uwvm-sdk-resolved-profile-v1", language = request.language,
                profile = request.profile, language_options = request.language_options, default_authority = {}}
        end
        function common.invoke(resolved, phase, argv)
            check(resolved.language == "java" and phase == "link")
            forwarded = argv
            return {exit_code = 0}
        end
        return common
    end
    local resolved = managed.resolve(common, "java", {runtime_mode = "teavm"}, output_json .. ".unused")
    check(bindings == 1 and resolved.language_options.runtime.version == "0.13.1")
    local argv = {"space path", "", "Unicode-λ", "@literal-response-spelling"}
    check(managed.invoke(common, resolved, "link", argv).exit_code == 0)
    check(bindings == 2 and forwarded == argv and forwarded[2] == "")
    refuses(function() managed.describe("java", "teavm", {language_version = "8"}) end)
    refuses(function() managed.describe("as", "assemblyscript", {runtime_version = "latest"}) end)
    refuses(function() managed.check_arguments("csharp", managed.describe("csharp", "mono8"), {"--runtime=linux-x64"}) end)
    refuses(function() managed.check_arguments("kotlin", managed.describe("kotlin", "teavm-jvm"), {"-jvm-target", "8"}) end)
    for _, language in ipairs({"dart", "r", "scala", "grain", "ruby", "php", "lua"}) do
        refuses(function() managed.describe(language) end)
    end
    local lua = managed.describe("lua", "lua54-wasi-eh")
    check(lua.exceptions and lua.required_features[1] == "exceptions" and lua.capabilities == nil)
    refuses(function() managed.validate_request({format = "uwvm-sdk-build-request-v1", language = "lua", profile = request_profile}, lua) end)
    local seen = {}
    for _, row in ipairs(examples.entries()) do
        local key = row.language .. "/" .. row.id
        check(not seen[key]); seen[key] = true
        check(row.id:match("^[a-z][a-z0-9_.-]*$") ~= nil)
        if row.source ~= "" then check(os.isfile(path.join(sdk, "examples", row.source)))
        else check(row.builder == "mono-aot" and row.requires_aot_admission) end
        check(examples.catalog(row.language).format == "uwvm-sdk-examples-v1")
    end
    refuses(function() examples.select("py", "no-such-entry") end)
    json.savefile(output_json, {format = "uwvm-managed-xmake-local-contract-v1", checks = checks,
        compiler_process_cases = 0, guest_cases = 0, native_ros_cases = 0})
    print("PASS_MANAGED_XMAKE_LOCAL_CHECKS=" .. checks)
end
