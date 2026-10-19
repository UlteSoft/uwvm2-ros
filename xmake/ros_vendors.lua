-- Vendor schemas are opt-in trusted build inputs. Native robot SDKs remain in
-- separately built Gateways; none is linked into the Wasm execution process.
-- Keep this list independent of the research inventory's acceptance status.
local ros_vendor_options = {
    {"unitree", "Unitree"}, {"agilex", "AgileX"},
    {"elephant", "Elephant Robotics"}, {"ufactory", "UFACTORY / xArm"},
    {"dobot", "DOBOT"}, {"flexiv", "Flexiv"}, {"leju", "Leju"},
    {"deep-robotics", "DEEP Robotics"}, {"realman", "RealMan"},
    {"clearpath", "Clearpath"}, {"robotis", "ROBOTIS"},
    {"universal-robots", "Universal Robots"}, {"franka", "Franka"},
    {"pal", "PAL Robotics"}, {"doosan", "Doosan"},
    {"trossen", "Trossen / Interbotix"}, {"agibot", "AgiBot"},
    {"ubtech", "UBTECH"}, {"fourier", "Fourier"},
    {"fanuc", "FANUC"}, {"kuka", "KUKA"}, {"yaskawa", "Yaskawa"},
    {"boston-dynamics", "Boston Dynamics / Spot"}, {"kinova", "Kinova"},
    {"abb", "ABB"}, {"booster", "Booster Robotics"},
    {"limx", "LimX Dynamics"}, {"hello-robot", "Hello Robot / Stretch"}
}
local ros_platform_options = {
    {"isaac-ros", "NVIDIA Isaac ROS"},
    {"isaac-sim", "NVIDIA Isaac Sim"},
    {"groot", "NVIDIA GR00T model bridge"}
}

local function declare_selection(prefix, entries)
    for _, entry in ipairs(entries) do
        local name, label = prefix .. entry[1], entry[2]
        option(name, function()
            set_default(false)
            set_showmenu(true)
            set_description("Select only the reviewed " .. label .. " schema bundle; requires an explicit ROS import and its registry-header option. Grants no permissions.")
        end)
        option(name .. "-registry-header", function()
            set_default("none")
            set_showmenu(true)
            set_description("Actual native-bound generated manifest.hpp for " .. label .. "; ignored bundles are never compiled. Multiple selections require the same explicitly merged manifest.")
        end)
    end
end

declare_selection("ros-vendor-", ros_vendor_options)
declare_selection("ros-platform-", ros_platform_options)

option("ros-platform-jetson", function()
    set_default(false)
    set_showmenu(true)
    set_description("Select the Jetson ARM64 deployment profile; requires an ARM64 Linux target and explicit ROS2 imports. Adds no CUDA, JetPack or robot driver library.")
end)

-- Called for every actual Runtime/CLI/test translation-unit target. This
-- returns the sole physical registry source: independently generated bundles
-- must not be concatenated because their dependency types can violate ODR.
function uwvm_ros_selected_vendor_registry()
    local selected = nil
    local enabled = 0
    local function select_entries(prefix, entries, ros2_only)
        for _, entry in ipairs(entries) do
            local name = prefix .. entry[1]
            local header = get_config(name .. "-registry-header")
            local supplied = header and header ~= "none" and header ~= ""
            if get_config(name) then
                enabled = enabled + 1
                if not is_plat("linux") then
                    error("--" .. name .. " requires the Linux ROS Gateway boundary")
                end
                if not (get_config("import-ros1") or get_config("import-ros2")) or
                    (ros2_only and not get_config("import-ros2")) then
                    error("--" .. name .. " requires an explicitly selected ROS backend")
                end
                if not supplied or header:find('[;\"\r\n]') or not os.isfile(header) then
                    error("--" .. name .. " requires an existing safe --" .. name .. "-registry-header=manifest.hpp")
                end
                local absolute = path.absolute(header):gsub("\\", "/")
                if selected and selected ~= absolute then
                    error("Selected ROS vendors/platforms require one physically identical merged manifest; regenerate a deduplicated catalog instead of linking independent bundles")
                end
                selected = absolute
            elseif supplied then
                error("--" .. name .. "-registry-header requires --" .. name .. "=y; an unselected vendor cannot inject schema source")
            end
        end
    end
    select_entries("ros-vendor-", ros_vendor_options, false)
    select_entries("ros-platform-", ros_platform_options, true)
    if get_config("ros-platform-jetson") then
        if not is_plat("linux") or not is_arch("arm64", "aarch64") or not get_config("import-ros2") then
            error("--ros-platform-jetson requires Linux ARM64 and explicit --import-ros2=y")
        end
    end
    local original = get_config("ros-custom-registry-header")
    if original and original ~= "none" and original ~= "" then
        if selected and path.absolute(original):gsub("\\", "/") ~= selected then
            error("--ros-custom-registry-header conflicts with the explicitly selected vendor manifest")
        end
        return original
    end
    -- No vendor option silently selects a schema, SDK, ROS backend or grant.
    if enabled == 0 then return nil end
    return selected
end
