/* Real typed C++ Reader; every original I/O field is preserved. */
#include "io.hpp"
static std::uint32_t read_complete(roswasmc_capability const* cap,uwvm_ur_io_state* value,uwvm_ur_storage*) {
 roswasm_ur::Reader reader(*cap);return reader.poll(*value);
}
#define UWVM_UR_EXAMPLE_POLL(c,v,s) read_complete(c,v,s)
#include "entry.h"
int main(int argc,char** argv){return uwvm_ur_entry(argc,argv);}
