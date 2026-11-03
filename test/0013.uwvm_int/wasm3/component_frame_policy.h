#pragma once
#include <cstring>
#include <cstdio>

namespace uwvm2test::component_frame_policy
{
    // These names select only an isolated translator's compile_option fields.
    // They never change the ROS runtime's mandatory checked native unwind.
    [[nodiscard]] inline bool parse(int argc, char** argv, bool& native_cfi) noexcept
    {
        native_cfi = true;
        if(argc == 1) { return true; }
        if(argc != 2) { return false; }
        if(std::strcmp(argv[1], "component-native-cfi") == 0) { return true; }
        if(std::strcmp(argv[1], "component-instrumented") == 0) { native_cfi = false; return true; }
        return false;
    }

    [[nodiscard]] inline char const* label(bool native_cfi) noexcept
    { return native_cfi ? "component-native-cfi" : "component-instrumented"; }

    inline void announce(bool native_cfi) noexcept
    {
        std::fprintf(stderr, "component-ir-frame-policy=%s; runtime-stack-contract=mandatory-native\n", label(native_cfi));
    }
}
