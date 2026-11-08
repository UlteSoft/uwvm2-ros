-- Run with xmake lua. The exact product checker must fail before provider builds.
function main(output)
    import("core.base.json")
    local policy = import("utility.ros_product",
        {rootdir=path.join(os.projectdir(), "xmake"), anonymous=true})
    local results = {}
    local function run(arch, options, expected, needle, originals)
        local observed, reason
        try {
            function()
                observed = policy.uwvm_ros_product_check_options(
                    function(k) return originals and originals[k] end,
                    function(k) return options[k] end, "linux", arch,
                    function(message) reason=message; raise(message) end)
            end,
            catch {function(err) assert(reason, tostring(err)) end}
        }
        assert((observed ~= nil) == expected, tostring(reason))
        if needle then assert(reason and reason:find(needle, 1, true), tostring(reason)) end
        table.insert(results, {arch=arch, options=options, originals=originals,
            admitted=observed ~= nil, reason=reason, passed=true})
    end
    for _, arch in ipairs({"x86_64", "i686", "aarch64", "arm", "armhf"}) do
        run(arch, {}, true)
        run(arch, {["host-mte"]="off"}, true)
        run(arch, {["host-mte"]="required"}, false,
            arch == "aarch64" and "no qualified complete native host provider" or "needs the AArch64")
        for _, bad in ipairs({"on", "stack", "heap", "globals", "async", "auto", true, false}) do
            run(arch, {["host-mte"]=bad}, false, "--host-mte must be off or required")
        end
        run(arch, {["host-mte"]="required"}, true, nil, {["host-mte"]="off"})
        run(arch, {["host-mte"]="off"}, false, "--host-mte=required", {["host-mte"]="required"})
        for _, field in ipairs({"cflags", "cxflags", "cxxflags", "ldflags", "shflags"}) do
            for _, raw in ipairs({"-fsanitize=memtag", "-fsanitize=address,memtag-stack",
                "-fsanitize-memtag-mode=async", "-fno-sanitize=memtag",
                "-Wl,-z,memtag-stack", "-Wl,-z,memtag-mode=sync",
                "-z memtag-heap", "-Xlinker --android-memtag-heap"}) do
                run(arch, {[field]=raw}, false, "cannot enable or alter native host MTE")
            end
        end
        run(arch, {cxxflags="-march=armv8.5-a+memtag"}, true)
        run(arch, {cxxflags="-fsanitize=undefined"}, true)
    end
    if output then json.savefile(output, {status="PASS", checks=#results, results=results}) end
    print("native host MTE prerequisite policy: " .. #results .. " checks PASS")
end
