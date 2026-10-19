/* Actual typed C++ Reader over the same complete original state. */
#include "state.hpp"
static std::uint32_t read_complete(roswasmc_capability const* cap,uwvm_clearpath_state* value,uwvm_clearpath_storage*) {
 roswasm_clearpath::Reader reader(*cap);return reader.poll(*value);
}
#define UWVM_CLEARPATH_EXAMPLE_POLL(c,v,s) read_complete(c,v,s)
#include "entry.h"
int main(int argc,char** argv){return uwvm_clearpath_entry(argc,argv);}
