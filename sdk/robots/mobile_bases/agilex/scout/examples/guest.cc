/* Complete C++20 caller using the typed client and the shared WASI lifecycle.
 * No ROS import is invoked until the real explicit capability open succeeds.
 */
#include "scout.hpp"
static roswasm_agilex::scout::Client client;
static std::uint32_t typed_status(roswasmc_capability const* cap,uwvm_scout_status* value,
                                  uwvm_scout_storage*) noexcept {return client.status(*cap,*value);}
#define UWVM_SCOUT_EXAMPLE_STATUS typed_status
#include "guest_entry.h"
int main(int argc,char** argv) {return uwvm_scout_example_main(argc,argv);}
