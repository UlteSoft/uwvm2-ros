// A failed ABI probe alone is not evidence of another standard library.
// Positively establish libc++ before publishing NOT_LIBSTDCXX metadata.
#include <string>
#if defined(__GLIBCXX__) || !defined(_LIBCPP_VERSION)
# error "This is not the selected libc++ target library"
#endif
int main() { return std::string{"link_selected_library"}.empty()?1:0; }
