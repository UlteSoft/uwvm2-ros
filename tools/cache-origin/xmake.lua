-- @file xmake.lua
-- @brief Build the optional Linux administrator helper without linking the VM.
-- @details It restores a root-owned seed before the VM starts. It is not a
-- setuid program and is never granted to a Wasm module. Select the same native
-- platform ABI as the deployment, for example xmake f --toolchain=clang.
set_xmakever("2.9.8")
set_project("uwvm2-ros-cache-origin")
set_allowedplats("linux", "cross")
set_languages("c11")
set_warnings("all", "extra", "pedantic", "error")
add_rules("mode.release", "mode.debug")

target("uwvm2-ros-cache-origin")
    set_kind("binary")
    add_files("main.c")
target_end()
