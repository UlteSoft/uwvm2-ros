#include <assert.h>
#include <string.h>
#include "application.h"
int main(void){
    uwvm_clearpath_motion_limits limits={0.2,0.4};
    roswasmc_twist out={{0,0,0},{0,0,0}},saved;
    assert(!uwvm_clearpath_make_base_motion(&limits,0.2,-0.4,&out));
    assert(out.linear[0]==0.2&&out.linear[1]==0&&out.angular[2]==-0.4);
    saved=out;
    assert(uwvm_clearpath_make_base_motion(&limits,NAN,0,&out)==22);
    assert(!memcmp(&saved,&out,sizeof out));
    assert(uwvm_clearpath_make_base_motion(&limits,0.201,0,&out)==22);
    assert(uwvm_clearpath_make_base_motion(&limits,0,INFINITY,&out)==22);
    limits.linear_mps=0;
    assert(uwvm_clearpath_make_base_motion(&limits,0,0,&out)==22);
    return 0;
}
