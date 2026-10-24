-- Native application-data acceptance only. No ROS import stubs or transport.
-- Actual Guest builds use the separate installed SDK-local application recipe.
for _,name in ipairs({"source","sdk","family","generated","output","version"}) do
    option("task06-"..name)
        set_showmenu(true)
        set_description("Explicit task06 native data test input: "..name)
    option_end()
end
toolchain("task06_native_clang22")
    set_kind("standalone")
    set_toolset("cc","/usr/bin/clang-22")
    set_toolset("cxx","/usr/bin/clang++-22")
    set_toolset("ld","/usr/bin/clang++-22")
toolchain_end()
target("task06_dobot_data")
    set_kind("binary")
    set_languages("cxx20")
    set_toolchains("task06_native_clang22")
    set_toolset("cc","/usr/bin/clang-22")
    set_toolset("cxx","/usr/bin/clang++-22")
    set_toolset("ld","/usr/bin/clang++-22")
    add_cxxflags("-O1","-fno-exceptions",{force=true})
    add_defines("UWVM_DOBOT_DEMO_PROCESS_ONLY=1")
    on_load(function(target)
        local config=import("core.project.config")
        local values={}
        for _,name in ipairs({"source","sdk","family","generated","output","version"}) do
            values[name]=assert(config.get("task06-"..name),"All native test inputs must be explicitly supplied")
        end
        assert(values.version=="3" or values.version=="4","Exact original demo version required")
        assert(os.isfile(values.source) and os.isfile(path.join(values.generated,"dobot_demo_alarm_data.h")),"Actual source/header required")
        target:add("files",values.source)
        target:add("includedirs",path.join(values.sdk,"include"),values.family,values.generated,
            path.join(path.directory(values.source),"support"))
        target:add("defines","UWVM_DOBOT_DEMO_VERSION="..values.version)
        target:set("targetdir",values.output)
    end)
target_end()
