-- Run through the admitted Linux owner with the real xmake Lua interpreter.
-- These are C adapter argument/configuration checks, not C or Guest execution.
function main(module_path)
    local c = import(path.basename(module_path), {rootdir = path.directory(module_path), anonymous = true})
    local count = 0
    local function check(value) assert(value, "C xmake contract failed"); count = count + 1 end
    local function refuses(call)
        local ok = try {function () call(); return true end}
        check(not ok)
    end
    local arguments = {"", "path with spaces/源.c", "-Wl,--export=quoted symbol", "@response file.rsp", "-std=c90"}
    check(c.check_arguments(arguments, "90"))
    check(#arguments == 5 and arguments[1] == "" and arguments[2] == "path with spaces/源.c" and arguments[4] == "@response file.rsp")
    check(c.standard("89").canonical == 90 and c.standard("90").canonical == 90)
    check(c.standard("95").flag == "-std=iso9899:199409")
    check(c.standard("29").draft)
    refuses(function () c.standard("13") end)
    refuses(function () c.check_arguments({"-std=c99"}, "90") end)
    refuses(function () c.check_arguments({"-std", "c99"}, "90") end)
    refuses(function () c.check_arguments({"-std"}, "90") end)
    refuses(function () c.check_arguments({"-DROSWASM_C_STANDARD=99"}, "90") end)
    refuses(function () c.check_arguments({[1] = "a", [3] = "b"}, "90") end)
    refuses(function () c.check_arguments({true}, "90") end)
    refuses(function () c.abi_defines(16, false, false) end)
    refuses(function () c.abi_defines(32, "false", false) end)
    local defines = c.abi_defines(64, false, true)
    check(defines[1] == "-DROSWASM_C_ADDRESS_BITS=64" and defines[2] == "-DROSWASM_C_EXCEPTIONS=0" and defines[3] == "-DROSWASM_C_THREADS=1")
    local c90 = "#define __STDC__ 1\n"
    check(c.check_predefines("90", c90).stdc_version == 0)
    refuses(function () c.check_predefines("90", c90 .. "#define __STDC_VERSION__ 199901L\n") end)
    refuses(function () c.check_predefines("11", c90 .. "#define __STDC_VERSION__ 201710L\n") end)
    refuses(function () c.check_predefines("23", c90 .. "#define __STDC_VERSION__ 202000L\n") end)
    check(c.check_predefines("23", c90 .. "#define __STDC_VERSION__ 202311L\n").stdc_version == 202311)
    check(c.check_predefines("29", c90 .. "#define __STDC_VERSION__ 202400L\n").draft)
    refuses(function () c.check_predefines("90", c90 .. "#define __cplusplus 202002L\n") end)
    refuses(function () c.check_predefines("90", "") end)
    print("C_XMAKE_CONTRACT_CHECKS=" .. tostring(count))
end
