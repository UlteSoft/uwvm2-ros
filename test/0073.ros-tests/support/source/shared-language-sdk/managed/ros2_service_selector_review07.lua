-- Real xmake module/selector checks. The common callbacks below only record
-- request/argv contracts; they execute no compiler, checker, Guest or ROS actor.
function main(source_root, output_json)
    local sdk = path.join(path.absolute(source_root), "sdk/origin")
    local managed = import("tools.xmake.managed", {rootdir = sdk, anonymous = true})
    local examples = import("tools.xmake.examples.managed", {rootdir = sdk, anonymous = true})
    local config = import("core.project.config")
    local json = import("core.base.json")
    local checks = 0
    local function check(value)
        checks = checks + 1
        assert(value, "service selector local check " .. checks)
    end
    local function refuses(callback)
        local returned = false
        try {function() callback(); returned = true end, catch {function(_errors) end}}
        check(not returned)
    end
    local profile = {target = "wasm32-wasip1", address_bits = 32, wasi = "wasip1", core_baseline = "3.0",
        startup = "command", exceptions = false, threads = false}
    local forwarded, selected
    local common = {}
    function common.bind(script_import) check(type(script_import) == "function") end
    function common.request(language, options)
        selected = options.consumer
        return {format = "uwvm-sdk-build-request-v1", language = language, profile = profile,
            language_options = options}
    end
    function common.resolve(request, output)
        local result = {format = "uwvm-sdk-resolved-profile-v1", driver = "uwvm-managed-sdk-consumer-v1",
            language = request.language, profile = profile, language_options = request.language_options,
            compiler_argv = {}, linker_argv = {}, default_authority = {}}
        json.savefile(output, result)
        return result
    end
    function common.invoke(_resolved, phase, argv)
        check(phase == "link")
        forwarded = argv
        return {exit_code = 0}
    end
    local function request(consumer)
        return managed.build_request(common, "as", {runtime_mode = "assemblyscript",
            runtime_version = "0.28.20", consumer = consumer})
    end
    request({builder = "as", backend = 2, pure = false, ros2_rmw_service = true})
    check(selected.ros2_rmw_service == true)
    request({builder = "as", backend = 1, pure = false})
    check(selected.ros2_rmw_service == nil)
    for _, consumer in ipairs({{builder = "as", backend = 1, ros2_rmw_service = true},
        {builder = "as", backend = 2, pure = true, ros2_rmw_service = true},
        {builder = "lua", backend = 2, ros2_rmw_service = true},
        {builder = "as", backend = 2, ros2_rmw_service = "true"},
        {builder = "as", backend = 2, ros2_rmw_service = 1}}) do
        refuses(function() request(consumer) end)
    end
    for _, language in ipairs({"java", "js"}) do
        local consumer = {builder = language, backend = 2, pure = false, ros2_rmw_service = true,
            entry = language == "java" and "clock-program/src/main/java/Example.java" or "clock-program/app.mjs"}
        if language == "java" then consumer.project = "clock-program/pom.xml"
        else consumer.js_recipe = "clock-program/app.recipe.json" end
        local function application()
            return managed.build_request(common, language, {consumer = consumer})
        end
        application()
        check(selected.ros2_rmw_service == true)
        consumer.backend = 1
        refuses(application)
        consumer.backend, consumer.pure = 2, true
        refuses(application)
        consumer.pure = false
        if language == "java" then
            consumer.lesson = "telemetry"
            refuses(application)
            consumer.lesson, consumer.project = nil, "../outside/pom.xml"
        else consumer.js_recipe = "../outside.json" end
        refuses(application)
    end
    local inputs = output_json .. ".inputs.json"
    json.savefile(inputs, {scope = "local contract only; not compiler inputs"})
    config.set("ros-sdk-managed-inputs", inputs)
    config.set("ros-sdk-managed-inputs-sha256", hash.sha256(inputs):lower())
    config.set("ros-sdk-as-runtime", "assemblyscript")
    config.set("ros-sdk-as-runtime-version", "0.28.20")
    for _, enabled in ipairs({false, true}) do
        local suffix = enabled and ".enabled" or ".disabled"
        examples.build_selected(common, sdk,
            {platform = "ros", language = "as", id = "telemetry-ros2", backend = 2,
             ros2_rmw_service = enabled}, output_json .. suffix)
        check(selected.ros2_rmw_service == enabled)
        local count = 0
        for _, word in ipairs(forwarded) do
            if word == "--ros2-rmw-service" then count = count + 1 end
        end
        check(count == (enabled and 1 or 0))
    end
    config.set("ros-sdk-managed-ros2-rmw-service", true)
    examples.build_selected(common, sdk,
        {platform = "ros", language = "as", id = "telemetry-ros2", backend = 2}, output_json .. ".configured")
    check(selected.ros2_rmw_service == true)
    local count = 0
    for _, word in ipairs(forwarded) do if word == "--ros2-rmw-service" then count = count + 1 end end
    check(count == 1)
    refuses(function()
        examples.build_selected(common, sdk,
            {platform = "ros", language = "as", id = "telemetry-ros1", backend = 1,
             ros2_rmw_service = true}, output_json .. ".ros1-refused")
    end)
    json.savefile(output_json, {status = "ACTUAL_XMAKE_SELECTOR_LOCAL_CONTRACT_PASS", checks = checks,
        compiler_executions = 0, checker_executions = 0, guest_executions = 0, native_ros_executions = 0})
end
