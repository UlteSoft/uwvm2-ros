-- @file xmake.lua
-- @brief Build genuine generated JS codec tests with the shared QuickJS engine.
-- @details The provider selects source inputs, not native ROS authority.
local root = os.getenv("UWVM_ROS_SDK_ROOT")
includes(path.join(root, "tools/xmake/ros.lua"))
set_plat("wasm-wasip1")
set_arch(get_config("arch") or "wasm32")
target("portable_generated_utf8")
    set_kind("binary")
    set_language("js")
    set_ros_platform("kuka")
    set_ros2_version("jazzy")
    set_values("ros.sdk.vendor_version", "kuka-ros2-complete")
    set_values("ros.sdk.vendor_binding", "share/platform/kuka/bindings/js/kuka-ros2-complete.json")
    set_ros_scenario("state_codec")
    -- The fixture owns a hash-checked source closure, with no ROS transport.
    set_values("ros.sdk.bundle_manifest", "share/qualification/generated-utf8.json")
    add_files("main.mjs")
target_end()
