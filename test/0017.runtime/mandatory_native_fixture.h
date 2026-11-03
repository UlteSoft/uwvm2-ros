#pragma once
#include <cstdio>
#include <cstring>

namespace uwvm2test::mandatory_native
{
    // Test argument admission only. The actual production initializer/compiler
    // remains responsible for its mandatory generated-chain native unwind check.
    [[nodiscard]] inline bool arguments(int argc, char** argv) noexcept
    {
        for(int i{1}; i < argc; ++i)
        {
            auto const value{argv[i]};
            if(std::strcmp(value, "auto") == 0 || std::strcmp(value, "none") == 0 ||
               std::strcmp(value, "instruction") == 0 || std::strcmp(value, "unwind") == 0 ||
               std::strcmp(value, "unwind-uncheck") == 0 ||
               std::strncmp(value, "-Rllvm-call-stack", 17) == 0 ||
               std::strncmp(value, "--runtime-llvm-jit-call-stack", 29) == 0)
            {
                std::fputs("unsupported legacy stack selector: this runtime fixture uses mandatory checked native unwind\n", stderr);
                return false;
            }
        }
        return true;
    }
}
