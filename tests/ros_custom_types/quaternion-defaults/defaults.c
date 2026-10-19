/** Compile against freshly generated fixture/catalog.preparation.json C source.
 * This is a pure value test; it has no ROS imports, endpoint or native identity.
 */
#include "bounded_records.h"
#include <string.h>
int main(void){
    typedef roswasmc_bounded_2_geometry_msgs_msg_Quaternion Q;
    typedef roswasmc_bounded_2_default_fixture_msg_Bundle B;
    Q q={0},before={0};B value={0},decoded={0};unsigned char bytes[ROSWASMC_BOUNDED_2_DEFAULT_FIXTURE_MSG_BUNDLE_MAX_WIRE_BYTES];size_t n=0;
    if(q.w!=0||roswasmc_bounded_2_geometry_msgs_msg_Quaternion_default(&q)||q.x||q.y||q.z||q.w!=1)return 1;
    if(roswasmc_bounded_2_geometry_msgs_msg_Quaternion_default(0)!=22)return 2;
    if(roswasmc_bounded_2_default_fixture_msg_Bundle_default(&value)||value.fixed[0].w!=1||value.fixed[1].w!=1||value.pose.orientation.w!=1||value.dynamic.size)return 3;
    value.fixed[0].w=.25;if(value.fixed[1].w!=1)return 4;
    if(roswasmc_bounded_2_geometry_msgs_msg_Quaternion_default(&value.dynamic.data[0]))return 5;value.dynamic.size=1;
    if(roswasmc_bounded_2_default_fixture_msg_Bundle_encode(&value,bytes,sizeof bytes,&n)||roswasmc_bounded_2_default_fixture_msg_Bundle_decode(bytes,n,&decoded))return 6;
    if(decoded.fixed[0].w!=.25||decoded.fixed[1].w!=1||decoded.pose.orientation.w!=1||decoded.dynamic.size!=1||decoded.dynamic.data[0].w!=1)return 7;
    before.w=9;q=before;memset(bytes,0,32);
    if(!roswasmc_bounded_2_geometry_msgs_msg_Quaternion_decode(bytes,31,&q)||q.w!=before.w)return 8;
    if(roswasmc_bounded_2_geometry_msgs_msg_Quaternion_decode(bytes,32,&q)||q.w!=0)return 9;
    return 0;
}
