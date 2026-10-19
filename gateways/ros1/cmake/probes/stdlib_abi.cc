// Discover the actual selected target-header bit without running target code.
// Defining a GCC-specific macro does not make libc++ use GCC's dual ABI.
#include <string>
#if !defined(__GLIBCXX__) || !defined(_GLIBCXX_USE_CXX11_ABI) || _GLIBCXX_USE_CXX11_ABI != UWVM_ROS1_EXPECT_CXX11_ABI
# error "This is not the requested libstdc++ string ABI"
#endif
int main() { return 0; }
