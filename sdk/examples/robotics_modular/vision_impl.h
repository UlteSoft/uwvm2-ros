/** @file Four already-measured synthetic distances, not an image/ML driver.
 * Return the nearest bounded sample, or a sentinel. No pointer/length pair is
 * shared with another module, and no floating-point conversion is involved.
 */
#ifndef ROSWASM_ROBOTICS_VISION_IMPL_H
#define ROSWASM_ROBOTICS_VISION_IMPL_H
#include "scalar_abi.h"
#include "robotics_math.h"
ROBOT_EXTERN ROBOT_EXPORT("vision_initialize") uint32_t vision_initialize(void) {return 0;}
ROBOT_EXTERN ROBOT_EXPORT("vision_clearance_mm") int32_t vision_clearance_mm(int32_t a,int32_t b,int32_t c,int32_t d) {
    return robot_math_nearest(a,b,c,d);
}
#endif
