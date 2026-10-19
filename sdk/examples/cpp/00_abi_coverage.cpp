/** @file
 * @brief C++20 full raw ABI compile/import coverage, not a runnable ROS lesson.
 * Use the identical C-compatible implementation and exact public ABI types.
 * Its externally exported selector prevents removal of otherwise unused raw
 * imports. Never invoke it with its placeholder arguments. main performs no
 * ROS call; neither compiler success nor import coverage proves permissions,
 * Guest startup or native ROS delivery. Typed runnable examples stay separate.
 */
#include "../c/abi_coverage_impl.h"
int main() { return 0; }
