/** @file
 * @brief Layer2: RAII ownership and the complete canonical codec library.
 * The host grants `pub` as String publication with sufficient explicit limits.
 * Capability destruction closes once. Errors remain numeric and never trigger
 * automatic retries or resource creation under a guessed alias.
 */
#include "example_support.hpp"
/** @brief Open one owner, encode a String, and publish its canonical bytes once. */
int main() {
    auto error=example_abi();if(error) return static_cast<int>(error);
    roswasmc_context context{ROSWASM_BACKEND()};
    roswasmcpp::Capability pub{context,"pub"};if(pub.error()) return static_cast<int>(pub.error());
    auto bytes=roswasmcpp::wire::string("hello from encoded C++");
    error=pub.publish_bytes(bytes);return static_cast<int>(example_finish(pub,error));
}
