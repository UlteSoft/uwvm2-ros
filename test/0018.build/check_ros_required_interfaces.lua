-- Run with xmake lua. This invokes the actual product admission policy.
function main(output)
    import("core.base.json")
    local policy = import("utility.ros_product",
        {rootdir = path.join(os.projectdir(), "xmake"), anonymous = true})
    local results = {}
    local function run(arch, options, expected, name, originals)
        local observed, reason
        local function rejected(message) reason = message; raise(message) end
        try {
            function()
                if originals then
                    observed = policy.uwvm_ros_product_check_options(
                        function(k) return originals[k] end,
                        function(k) return options[k] end, "linux", arch, rejected)
                else
                    observed = policy.uwvm_ros_product_check(
                        function(k) return options[k] end, "linux", arch, rejected)
                end
            end,
            catch {function(err) assert(reason, tostring(err)) end}
        }
        assert((observed ~= nil) == expected, name .. ": " .. tostring(reason))
        if not expected then
            assert(reason:find("configuration rejected", 1, true))
        end
        table.insert(results, {arch=arch, name=name, passed=true,
            admitted=observed ~= nil, reason=reason})
    end
    for _, arch in ipairs({"x86_64", "i686", "aarch64", "arm", "armhf"}) do
        run(arch, {}, true, "default-wasi-and-rosv2")
        for _, model in ipairs({"mmap", "default", "single-thread-alloc", "multi-thread-alloc"}) do
            run(arch, {["wasm-memory-model"]=model}, model == "mmap" or model == "default", "wasm-memory-model-" .. model)
        end
        for _, option in ipairs({"import-u2rosv2", "enable-local-imported-wasip1-wasm64"}) do
            for _, value in ipairs({false, "n", "no", "false", "FALSE"}) do
                run(arch, {[option]=value}, false, option .. "-removed-" .. tostring(value))
            end
            for _, value in ipairs({true, "y", "yes", "true"}) do
                run(arch, {[option]=value}, true, option .. "-present-" .. tostring(value))
            end
            run(arch, {[option]=true}, false, option .. "-explicit-no-overrides-saved-yes", {[option]="n"})
            run(arch, {[option]=false}, true, option .. "-explicit-yes-overrides-saved-no", {[option]="y"})
        end
        for _, option in ipairs({"import-u2rosv1-ros1", "import-u2rosv1-ros2", "enable-preload-dl"}) do
            for _, value in ipairs({false, "n", "no", "false", "FALSE", true, "y", "yes", "true"}) do
                run(arch, {[option]=value}, true, option .. "-optional-" .. tostring(value))
            end
            run(arch, {[option]=true}, true, option .. "-explicit-no-overrides-saved-yes", {[option]="n"})
            run(arch, {[option]=false}, true, option .. "-explicit-yes-overrides-saved-no", {[option]="y"})
        end
        for _, value in ipairs({true, "y", "yes", "true"}) do
            run(arch, {["disable-local-imported-wasip1"]=value}, false, "wasi-removed-" .. tostring(value))
        end
        for _, value in ipairs({"none", "wasix", "", false}) do
            run(arch, {["enable-local-imported-wasip1-socket"]=value}, false, "socket-" .. tostring(value))
        end
        run(arch, {["enable-local-imported-wasip1-socket"]="wasip1"}, true, "socket-complete")
        run(arch, {["enable-local-imported-wasip1-socket"]="wasip1"}, false,
            "explicit-socket-removal-overrides-saved", {["enable-local-imported-wasip1-socket"]="none"})
        for _, flags in ipairs({"-DUWVM_DISABLE_LOCAL_IMPORTED_WASIP1",
            "-U UWVM_IMPORT_ROS1", "-UUWVM_IMPORT_ROS2", "-U UWVM_ENABLE_WASIP1_SOCKET",
            "-DUWVM_ENABLE_LOCAL_IMPORTED_WASIP1_WASM64=0", "-UUWVM_IMPORT_ROSV2", "-DUWVM_ENABLE_PRELOAD_DL=1"}) do
            run(arch, {cxxflags=flags}, false, "raw-flag-" .. flags)
        end
    end
    assert(#results == 365)
    if output then json.savefile(output, {status="PASS", checks=#results, results=results}) end
    print("default and legacy opt-in product policy: " .. #results .. " checks PASS")
end
