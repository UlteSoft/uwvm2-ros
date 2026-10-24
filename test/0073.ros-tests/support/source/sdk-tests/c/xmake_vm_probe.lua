-- Actual xmake interpreter observation; no compiler, Guest or ROS operation.
function main(module_path, ...)
    local json = import("core.base.json")
    -- xmake replaces global _VERSION with its own product version and does not
    -- export _G/rawget into this sandbox. Inspect the running parser through
    -- its supported debug module; never guess Lua syntax from that version.
    local native_jit = debug.global("jit")
    local compiler = debug.global("loadstring") or debug.global("load")
    assert(type(compiler) == "function", "actual VM compiler required")
    local probes = {
        {syntax = "Lua54", source = "local value <const> = 1; return value"},
        {syntax = "Lua53", source = "return 3 // 2"},
        {syntax = "Lua52", source = "goto done; ::done:: return 1"},
        {syntax = "Lua51", source = "return 1"}
    }
    local result = {xmake_global_version = debug.global("_VERSION"),
        xmake_luajit = xmake.luajit() or false,
        jit_version = native_jit and native_jit.version or false,
        parser_probes = {}}
    for _, row in ipairs(probes) do
        local chunk = compiler(row.source, "@C_XMAKE_SYNTAX_PROBE")
        local supported = type(chunk) == "function"
        table.insert(result.parser_probes, {syntax = row.syntax, supported = supported})
        if supported and not result.actual_syntax then result.actual_syntax = row.syntax end
    end
    if result.xmake_luajit then result.actual_syntax = "LuaJIT" end
    assert(result.actual_syntax, "supported observed VM syntax required")
    if module_path == "compile" then
        result.compiled_files = {}
        for _, filename in ipairs({...}) do
            local chunk, reason = compiler(io.readfile(filename), "@" .. filename)
            assert(type(chunk) == "function", reason or "actual VM rejected source syntax")
            table.insert(result.compiled_files, filename)
        end
        assert(#result.compiled_files == 4, "all four owned Lua sources required")
    elseif module_path then
        local selected = import(path.basename(module_path), {
            rootdir = path.directory(module_path), anonymous = true})
        local catalog = selected.catalog()
        assert(catalog.format == "uwvm-sdk-examples-v1", "actual C catalog contract required")
        local recipes = {}
        for _, row in ipairs(catalog.examples) do recipes[row.recipe] = true end
        local count = 0
        for _ in pairs(recipes) do count = count + 1 end
        assert(#catalog.examples == 77 and count == 22, "exact C catalog denominator required")
        result.catalog_entries = #catalog.examples
        result.unique_recipes = count
    end
    print("C_XMAKE_VM_JSON=" .. json.encode(result))
end
