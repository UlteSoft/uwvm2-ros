-- @file ros_product.lua
-- @brief Admit the supported ROS product before acquiring compiler dependencies.
-- @details This is a product policy, not a list of upstream Uwvm2 capabilities.
-- Keep the wider backend sources available for future qualification. New targets
-- require a reviewed ROS ABI and real runtime tests before changing this table.
-- The injected getter also lets Linux tests exercise the exact production policy.
function uwvm_ros_product_check(get, host, host_arch, reject_configuration)
    local function value(name, default)
        local result = get(name)
        if result == nil then return default end
        -- Original CLI booleans may still be y/n strings during option checks.
        -- Normalize Boolean fields only; enum values such as "none" stay enums.
        if type(default) == "boolean" and type(result) == "string" then
            local boolean = string.lower(result)
            if boolean == "y" or boolean == "yes" or boolean == "true" then return true end
            if boolean == "n" or boolean == "no" or boolean == "false" then return false end
        end
        return result
    end
    local function reject(reason)
        reject_configuration("uwvm2-ros configuration rejected: " .. reason ..
            ". Upstream Uwvm2 may support this configuration; the current ROS product does not.")
    end
    -- Check user input before dependency resolution appends the paired provider
    -- flags. A later flag can otherwise override a validated named option.
    for _, field in ipairs({"cflags", "cxflags", "cxxflags", "ldflags", "shflags"}) do
        local raw = value(field, "")
        if type(raw) == "table" then raw = table.concat(raw, " ") end
        -- Project-description scope has no sandbox os.argv(). Inspect the
        -- original text conservatively; quotes/escapes must not hide an ABI
        -- override before the full dependency sandbox exists.
        raw = tostring(raw):gsub(string.char(34), ""):gsub(string.char(39), ""):gsub(string.char(92), "")
        local separate_macro = false
        for flag in raw:gmatch("%S+") do
            local macro = separate_macro and flag or nil
            separate_macro = false
            local operation, attached = flag:match("^%-([DU])(.*)$")
            if operation then
                if attached == "" then separate_macro = true else macro = attached end
            end
            -- Named, checked product options own these switches. In particular
            -- -U UWVM_DISABLE_INT must not undo execution-int=none after checks.
            -- Check both attached and separate compiler-driver macro syntax.
            local reserved_macro = macro and (macro:match("^UWVM") or macro:match("^__")
                or macro:match("^_LIBCPP_") or macro:match("^_LIBCXXABI_")
                or macro:match("^_LIBUNWIND") or macro:match("^_GLIBCXX_"))
            local forbidden = reserved_macro or flag:find("@", 1, true) or flag:match("^%-Xclang")
                or flag:match("^%-Wp,") or flag == "-Xpreprocessor"
                or flag:match("^%-%-define%-macro") or flag:match("^%-%-undefine%-macro")
                or flag == "-mllvm" or flag:match("^%-fmodule")
                or flag:match("^%-include") or flag:match("^%-imacros")
                or flag == "-fno-exceptions" or flag == "-fignore-exceptions"
                or flag == "-fno-cxx-exceptions" or flag == "-fsjlj-exceptions"
                or flag == "-fno-unwind-tables" or flag == "-fno-asynchronous-unwind-tables"
                or flag == "-fno-threadsafe-statics" or flag == "-ffreestanding"
                or flag == "-fshort-enums" or flag == "-fshort-wchar" or flag:match("^%-fpack%-struct")
                or flag == "-ffast-math" or flag == "-Ofast" or flag == "-ffinite-math-only"
                or flag == "-m32" or flag == "-mx32" or flag == "-mbig-endian"
                or flag == "-msoft-float" or flag:match("^%-mabi=")
                or flag:match("^%-mfloat%-abi=soft") or flag:match("^%-+target[= ]?")
                or flag:match("^%-stdlib=") or flag:match("^%-+unwindlib=")
                or flag:find("-lunwind", 1, true) or flag:find("-lgcc_s", 1, true) or flag:find("-lgcc_eh", 1, true)
                or flag:find("-lstdc++", 1, true) or flag:find("-lc++", 1, true)
                or flag:match("libunwind%.[aso]") or flag:match("libc%+%+.*%.[aso]")
                or flag:find("_LIBCPP_ABI_", 1, true) or flag:find("_LIBUNWIND", 1, true)
                or flag:find("UWVM_ROS_PAIRED_RUNTIME_HEADERS", 1, true)
                or flag:find("UWVM_USE_THREAD_LOCAL", 1, true)
                or flag:find("_Unwind_", 1, true) or flag:find("__cxa_", 1, true)
                or flag:find("--allow-multiple-definition", 1, true) or flag:find("-z,muldefs", 1, true)
            if forbidden then reject("raw " .. field .. " overrides a required ABI/EH contract: " .. flag) end
        end
    end
    local plat = value("plat", host)
    if plat ~= "linux" then
        reject("only Linux is supported; requested platform=" .. tostring(plat))
    end
    if value("ros-product-policy", true) ~= true then reject("the mandatory product policy cannot be disabled") end
    local aliases = {
        x86_64="x86_64", x64="x86_64", amd64="x86_64",
        aarch64="aarch64", arm64="aarch64"
    }
    local requested = string.lower(tostring(value("arch", host_arch)))
    local arch = aliases[requested]
    if not arch then
        reject("the current Linux ROS product admits x86_64 and little-endian AArch64 GNU ABIs; " ..
            "ARMv7 native debugging remains unqualified and its backend is retained for candidate testing; requested arch=" .. requested)
    end
    for _, option in ipairs({"target", "llvm-target"}) do
        local triple = value(option, "detect")
        if triple ~= "detect" and triple ~= "none" and triple ~= "default" then
            triple = string.lower(tostring(triple))
            local actual
            if triple:match("^x86_64%-") and triple:match("%-linux%-gnu$") then actual = "x86_64"
            elseif triple:match("^aarch64%-") and triple:match("%-linux%-gnu$") then actual = "aarch64"
            elseif (triple:match("^arm%-") or triple:match("^armv7a?%-"))
                and triple:match("gnueabihf$") then actual = "armhf" end
            if actual ~= arch or not triple:find("linux", 1, true) or not triple:find("gnu", 1, true) then
                reject("--" .. option .. "=" .. triple .. " conflicts with the selected Linux GNU ROS ABI " .. arch)
            end
        end
    end
    -- Modern ROS threads, C++ EH, signed caches and the genuine AOT backend are
    -- product requirements. Legacy DOS/Win9x workarounds cannot silently alter them.
    for _, option in ipairs({"use-thread-local", "use-llvm-compiler", "enable-wsea", "linux-native-debug",
        "enable-local-imported-wasip1-wasm64", "detailed-debug-check"}) do
        if value(option, true) ~= true then reject("--" .. option .. "=false is not recommended or supported") end
    end
    for _, option in ipairs({"fno-exceptions", "disable-local-imported-wasip1",
        "macos-x64-native-step", "macos-4gib-test", "numeric-exception-fused", "use-cxx-module", "enable-lto"}) do
        if value(option, false) ~= false then reject("--" .. option .. " is not supported by this product") end
    end
    if not value("import-ros1", true) and not value("import-ros2", true) then
        reject("at least one qualified ROS bridge must be included")
    end
    local required = { ["execution-int"]="none", ["execution-jit"]="llvm",
        ["gc-jit-access-profile"]="default", ["gc-allocation-profile"]="default",
        ["mingw-min"]="default", ["apple-platform"]="default" }
    for option, expected in pairs(required) do
        if value(option, expected) ~= expected then
            reject("--" .. option .. "=" .. tostring(value(option, expected)) .. "; required value is " .. expected)
        end
    end
    local runtime = value("llvm-runtimes-provider", "auto")
    if runtime ~= "auto" and runtime ~= "bundled" then reject("external C++ ABI/unwind providers are not admitted") end
    local library = value("stdlib", "default")
    if library ~= "default" and library ~= "libc++" then reject("the paired LLVM libc++ ABI is required") end
    local unwind = value("unwindlib", "default")
    if unwind ~= "default" and unwind ~= "libunwind" and unwind ~= "force-libunwind" then
        reject("the source-owned paired libunwind is required")
    end
    local memory = value("wasm-memory-model", "default")
    if memory ~= "default" and memory ~= "mmap" then reject("the qualified mmap memory backend is required") end
    if value("static", "none") == "compiler" then reject("a fully static host C library has not been qualified") end
    local legacy = { ["enable-uwvm-int-combine-ops"]="soft", ["enable-uwvm-int-delay-local"]="heavy",
        ["enable-uwvm-int-instruction-reorder"]=false, ["enable-uwvm-int-loop-unwind"]=true }
    for option, default in pairs(legacy) do
        if value(option, default) ~= default then reject("--" .. option .. " is an upstream interpreter setting") end
    end
    return arch
end

-- Preserve original explicit values, including false, over incomplete or
-- previous saved configuration. The injected getters support Linux contracts.
function uwvm_ros_product_check_options(get_original, get_configuration, host, host_arch, reject_configuration)
    local function selected(name)
        local original = get_original(name)
        if original ~= nil then return original end
        return get_configuration(name)
    end
    return uwvm_ros_product_check(selected, host, host_arch, reject_configuration)
end

-- Call only in a full option/rule/dependency sandbox. Declarative project
-- description deliberately provides no raise/assert/error API.
function uwvm_ros_product_check_current()
    local arguments = import("core.base.option", {anonymous = true})
    local configuration = import("core.project.config", {anonymous = true})
    local originals = arguments.options() or {}
    local task = arguments.taskname()
    local function original(name)
        -- option.get also supplies menu defaults; they must not hide a saved
        -- project selection. Outside config, target is a build/run positional
        -- name rather than a compiler triple. Nested config has its own context.
        if task == "config" and originals[name] ~= nil then
            return arguments.get(name)
        end
    end
    return uwvm_ros_product_check_options(original, configuration.get, os.host(), os.arch(), raise)
end
