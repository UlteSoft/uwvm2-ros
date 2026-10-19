/** @file
 * @brief Prepared C++20 inclusion test for the allocation-free C value/resource headers.
 * This file uses no backend, does not import ROS, and does not run native ROS.
 */
#include <roswasm/node.h>
/** @brief Construct a zero-capacity reader without requiring a C hosted library or native backend. */
int main() {
    auto reader=roswasmc_reader_init(nullptr,0);
    return roswasmc_reader_done(&reader)?0:1;
}
