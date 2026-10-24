/** Unrun C++20 typed API contract using the actual generated record aliases. */
#include "../../../../../../../../sdk/origin/robots/mobile_manipulation/fjt_client.hpp"
#include "../../../../../../../../sdk/origin/robots/mobile_manipulation/examples/fjt_lesson.hpp"
#include "../../../../../../../../sdk/origin/robots/mobile_manipulation/fjt_goal.h"
int main() {
    uwvm_mobile::Goal value{};double lift{0.3};
    if(mm_make_stretch_lift_goal(&value,lift))return 1;
    roswasmcpp::wire::bytes wire;
    if(!roswasmcpp::Codec<uwvm_mobile::Goal>::encode(value,wire))return 2;
    uwvm_mobile::Goal decoded{};
    if(!roswasmcpp::Codec<uwvm_mobile::Goal>::decode(wire,decoded))return 3;
    return decoded.trajectory.points.data[0].positions.data[0]==lift?0:4;
}
