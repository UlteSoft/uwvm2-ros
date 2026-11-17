/** @file
 * @brief Exact startup markers before the communication data protocol activates.
 * @details ready follows the first all-thread syscall fence; attached follows
 * direction validation and the final all-thread FD fence. Neither marker is a
 * guest-grant decision. The supervisor must see both before enabling traffic.
 */
#pragma once
#include "communication_worker_backend.h"
namespace uwvm2::imported::ros::scene::communication_worker
{
    inline constexpr std::string_view ready="uwvm.communication.ready\n";
    inline constexpr std::string_view attached="uwvm.communication.attached\n";
}
