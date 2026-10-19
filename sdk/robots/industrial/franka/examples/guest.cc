/* Actual typed C++ Reader over the same complete original state. */
#include "state.hpp"
static std::uint32_t read_complete(roswasmc_capability const* cap,uwvm_franka_state* value,uwvm_franka_storage*) {
 roswasm_franka::Reader reader(*cap);return reader.poll(*value);
}
#define UWVM_FRANKA_EXAMPLE_POLL(c,v,s) read_complete(c,v,s)
#include "entry.h"
int main(int argc,char** argv){return uwvm_franka_entry(argc,argv);}
