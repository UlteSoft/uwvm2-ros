-- @file basic_test.lua
-- @brief Exercise the already-built ROS product with short, real runtime probes.
-- @details This task never substitutes source inspection or a version string
-- for execution. Cache refusal, failed native unwinding and unavailable native
-- debugging fail the task. It does not rebuild the VM or grant ROS endpoints.
task("basic_test")
	on_run(function ()
		import("core.base.option")
		import("core.project.config")
		import("core.project.project")
		import("lib.detect.find_tool")
		config.load()
		local product = project.target("uwvm2-ros")
		assert(product, "configure the uwvm2-ros target first")
		local executable = path.absolute(product:targetfile())
		assert(os.isfile(executable), "build uwvm2-ros before running xmake basic_test")
		local python = find_tool("python3")
		assert(python, "basic_test requires Python 3 (no Python packages or Wasm compiler)")
		local arguments = {"-B", path.join(os.projectdir(), "test/0073.ros-tests/basic_test.py"),
			"--uwvm", executable, "--out", path.absolute(option.get("output") or path.join(config.builddir(), "basic_test"))}
		-- os.execv preserves argument boundaries, including paths containing spaces.
		os.execv(python.program, arguments)
	end)
	set_menu {
		usage = "xmake basic_test [options]",
		description = "Check real AOT cache hits, native unwind, WASI/ROS admission and native debugging.",
		options = {
			{nil, "output", "kv", nil, "Parent directory for a new receipt and raw-log folder."}
		}
	}
task_end()
