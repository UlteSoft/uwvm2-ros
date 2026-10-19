/* Actual typed C++ Reader over the same complete original state. */
#include "state.hpp"
static std::uint32_t read_complete(roswasmc_capability const* cap,uwvm_turtlebot3_state* value,uwvm_turtlebot3_storage*) {
 roswasm_turtlebot3::Reader reader(*cap);return reader.poll(*value);
}
#define UWVM_TURTLEBOT3_EXAMPLE_POLL(c,v,s) read_complete(c,v,s)
#include "entry.h"
int main(int argc,char** argv){return uwvm_turtlebot3_entry(argc,argv);}
