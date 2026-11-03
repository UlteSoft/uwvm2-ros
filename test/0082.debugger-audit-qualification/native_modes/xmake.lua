set_project("debugger-native-mode-qualification")
local source_dir = os.scriptdir()
task("native_modes")
    set_category("plugin")
    on_run(function ()
        import("core.base.option")
        assert(is_host("linux"), "Run only inside the admitted Linux executor")
        local arguments = {"-B", path.join(source_dir, "run.py")}
        for _, name in ipairs({"plan", "plan-sha256", "executor", "executor-sha256", "out"}) do
            local value = option.get(name)
            assert(value and #value > 0, "Explicit --" .. name .. " is required")
            table.insert(arguments, "--" .. name)
            table.insert(arguments, value)
        end
        os.execv("python3", arguments)
    end)
    set_menu {
        usage = "xmake native_modes [options]",
        description = "Run pinned native debugger modes with an explicit live-admission provider",
        options = {
            {nil, "plan", "kv", nil, "Frozen JSON workload/product plan"},
            {nil, "plan-sha256", "kv", nil, "Full SHA-256 of the plan"},
            {nil, "executor", "kv", nil, "Frozen trusted Linux admission adapter"},
            {nil, "executor-sha256", "kv", nil, "Full SHA-256 of that adapter"},
            {nil, "out", "kv", nil, "New owned output directory"}
        }
    }
