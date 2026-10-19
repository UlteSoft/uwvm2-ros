/** @file
 * @brief Checked cleanup shared by runnable C++ application examples.
 * Explicit close exposes transport errors while RAII remains a fallback.
 * No helper grants authority or retries an admitted RPC/close effect.
 */
#pragma once
#include "select_backend.h"
#include <roswasm/roswasmcpp.hpp>
/** @brief Validate ABI1 separately from ROS version without opening rights. */
inline uint32_t example_abi() {return !ROSWASM_BACKEND()->abi_version||ROSWASM_BACKEND()->abi_version()!=1?UWVM_ROS_UNSUPPORTED:0;}
/** @brief Attempt each owned close once and check its numeric result. */
template<class Owner> uint32_t example_finish(Owner& owner,uint32_t primary) {
    auto close_code=owner.is_open()?owner.close():0;return primary?primary:close_code;
}
/** @brief Check both advertise/respond cleanup statuses, preserving primary error. */
template<class Server> uint32_t example_finish_server(Server& server,uint32_t primary) {
    uint32_t statuses[2]{};auto cleanup=server.close_checked(statuses,primary);return primary?primary:cleanup.cleanup_error;
}
