import("core.cache.detectcache")
import("lib.detect.find_program")
---@alias map_t table<string, string>

-- Hashing and prefix discovery need the actual driver pathname. Keep the
-- symlink name because clang and clang++ select their mode from argv[0].
function find_compiler_file(program)
    if program ~= path.filename(program) then
        program = path.absolute(program)
    end
    -- Toolchain detection can cache a bare command. Search the current PATH
    -- explicitly, including on systems without the external which utility.
    local file = find_program(program, {paths = {"$(env PATH)"}, cachekey = "uwvm.compiler-file",
        force = true, norunfile = is_host("windows")})
    if file then
        -- Xmake can return the selected Windows executable without its suffix.
        if is_host("windows") and not os.isfile(file) and os.isfile(file .. ".exe") then
            file = file .. ".exe"
        end
        if os.isfile(file) then return path.absolute(file) end
    end
end

---Determine if a toolchain is clang
---@return boolean --Is the clang toolchain
function is_clang()
    return string.find(get_config("toolchain") or "", "clang", 1, true) ~= nil
end

---Cache keys used in this module
local cache_key = "toolchain.utility"

---Getting Cache Information
---@return table<string, any> --Cache Information Table
function get_cache()
    local cache_info = detectcache:get(cache_key)
    if not cache_info then
        cache_info = {}
        detectcache:set(cache_key, cache_info)
    end
    return cache_info
end

---Updating Cache Information
---@param cache_info table --Cache information to be saved
---@return nil
function update_cache(cache_info)
    detectcache:set(cache_key, cache_info)
    detectcache:save()
end

---Get a normalized boolean config value
---@param name string --Config name
---@param default boolean | nil --Default value when config is unset
---@return boolean --Normalized boolean value
function get_config_bool(name, default)
    local value = get_config(name)
    if value == nil then
        return default == true
    end
    if type(value) == "boolean" then
        return value
    end
    if type(value) == "string" then
        local normalized = string.lower(string.trim(value))
        if normalized == "y" or normalized == "yes" or normalized == "true" or normalized == "on" or normalized == "1" then
            return true
        end
        if normalized == "n" or normalized == "no" or normalized == "false" or normalized == "off" or normalized == "0" then
            return false
        end
    end
    return value and true or false
end
