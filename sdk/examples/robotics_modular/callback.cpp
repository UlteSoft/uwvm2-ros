/** Pure C++ callback Wasm. C linkage plus export_name preserves the scalar ABI;
 * no std::function representation or C++ ABI crosses a module boundary. */
#include "callback_impl.h"
