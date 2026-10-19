/* Genuine WASI C++ command; borrowed readers share one exact source domain. */
#include "sdk/robots/mobile_bases/clearpath/platform/include/state.hpp"
#include "sdk/robots/arms/elephant/mycobot/include/state.hpp"
static uint32_t pair_clearpath(roswasmc_capability const* c,uwvm_clearpath_state* v,uwvm_clearpath_storage*){
 roswasm_clearpath::Reader reader(*c);return reader.poll(*v);
}
static uint32_t pair_elephant(roswasmc_capability const* c,uwvm_elephant_state* v,uwvm_elephant_storage*){
 roswasm_elephant::Reader reader(*c);return reader.poll(*v);
}
#define UWVM_PAIR_CLEARPATH_POLL pair_clearpath
#define UWVM_PAIR_ELEPHANT_POLL pair_elephant
#include "entry.h"
int main(int argc,char** argv){return uwvm_clearpath_elephant_entry(argc,argv);}
