-- Actual xmake module-load / local adapter-contract candidate. The injected
-- common functions below are spies: this proves adapter validation/argv
-- forwarding only, not common process execution or any Guest/native ROS.
function main(installed_sdk_root)
    assert(installed_sdk_root, "supply the explicit installed SDK root")
    local managed = import("tools.xmake.managed", {rootdir = path.absolute(installed_sdk_root)})
    local checks = 0
    local function check(ok) checks = checks + 1; assert(ok, "managed adapter contract " .. checks) end
    local function refuses(fn) local ok = pcall(fn); check(not ok) end
    local function request(language, eh)
        return {format = "uwvm-sdk-build-request-v1", language = language,
            profile = {target = "wasm32-wasip1", address_bits = 32, wasi = "wasip1",
                exceptions = eh or false, threads = false, startup = "command", core_baseline = "3.0"}}
    end
    local java = managed.describe("java", "teavm")
    check(java.version == "0.13.1" and java.address_bits == 32)
    check(next(java.default_authority) == nil and java.runtime_tested == false)
    check(managed.validate_request(request("java"), java).profile.exceptions == false)
    local lua = managed.describe("lua", "lua54-wasi-eh")
    check(lua.exceptions == true and lua.required_features[1] == "exceptions")
    check(lua.capabilities == nil) -- requirements must not assert Host support
    check(managed.validate_request(request("lua", true), lua).profile.exceptions == true)
    refuses(function() managed.validate_request(request("lua"), lua) end)
    refuses(function() managed.validate_request(request("java", true), java) end)
    local wide = request("java"); wide.profile.target = "wasm64-wasip1"; wide.profile.address_bits = 64
    refuses(function() managed.validate_request(wide, java) end)
    local threads = request("java"); threads.profile.threads = true
    refuses(function() managed.validate_request(threads, java) end)
    local reactor = request("java"); reactor.profile.startup = "reactor"
    refuses(function() managed.validate_request(reactor, java) end)
    refuses(function() managed.describe("java", "teavm", {runtime_version = "0.14.0"}) end)
    refuses(function() managed.describe("as", "assemblyscript", {runtime_version = "latest"}) end)
    refuses(function() managed.describe("moonbit", "linear", {runtime_version = ">=0.1"}) end)
    refuses(function() managed.describe("csharp", "mono9-aot", {aot_mode = "AOT"}) end)
    local hybrid = managed.describe("csharp", "mono9-aot", {aot_mode = "LLVMOnlyInterp"})
    check(hybrid.interpreter_fallback == true)
    local full = managed.describe("csharp", "mono9-aot", {aot_mode = "LLVMOnly"})
    check(full.interpreter_fallback == false)
    local mono = managed.describe("csharp", "mono8")
    local managed_arguments = {"-p:TargetFramework=net8.0", "-p:RuntimeIdentifier=wasi-wasm", "space path", ""}
    check(managed.check_arguments("csharp", mono, managed_arguments) == managed_arguments)
    refuses(function() managed.check_arguments("csharp", mono, {"--runtime=linux-x64"}) end)
    refuses(function() managed.check_arguments("csharp", mono, {"/P:runtimEidentifier=browser-wasm"}) end)
    refuses(function() managed.check_arguments("csharp", mono, {"-p:RunAOTCompilation=true"}) end)
    refuses(function() managed.check_arguments("csharp", mono, {"-p:Other=x;RuntimeIdentifier=linux-x64"}) end)
    refuses(function() managed.check_arguments("java", java, {"-Dteavm.targetType=WEBASSEMBLY_GC"}) end)
    refuses(function() managed.check_arguments("kotlin", managed.describe("kotlin", "teavm-jvm"), {"-jvm-target", "11"}) end)
    for _, language in ipairs({"dart", "r", "grain", "ruby", "php", "scala", "lua"}) do
        refuses(function() managed.describe(language) end)
    end
    local ruby = managed.describe("ruby", "cruby-wasi", {runtime_version = "3.4.0"})
    check(ruby.source_ready == false)
    refuses(function() managed.validate_request(request("ruby"), ruby) end)
    local arguments = {"space path", "", "Unicode-λ", "@response file.rsp", "-Wl,--max-memory=134217728"}
    local resolved = {format = "uwvm-sdk-resolved-profile-v1", language = "java", language_options = {runtime = java}}
    local called = false
    local common = {invoke = function(received, phase, argv)
        called = true
        check(received == resolved and phase == "link" and argv == arguments)
        check(#argv == 5 and argv[2] == "" and argv[3] == "Unicode-λ")
        return {exit_code = 0}
    end}
    check(managed.invoke(common, resolved, "link", arguments).exit_code == 0 and called)
    print("PASS_XMAKE_MANAGED_LOCAL_CONTRACT_CHECKS=" .. checks)
    print("COMMON_PROCESS_EXECUTION=NOT_RUN;GUEST=NOT_RUN;NATIVE_ROS=NOT_RUN")
end
