/** @file
 * @brief Trusted communication-worker startup contract, independent of data framing.
 * @details These native functions belong to a digest-pinned backend closure.
 * Preparation receives only trusted deployment bytes, with no network or broker
 * pipe. It creates every participant, executor and transport-pump thread behind
 * an activation gate. The bootstrap seals the syscall boundary across all
 * threads before receiving four directional broker pipes. Activation may only
 * attach those descriptors and release existing gates; new threads, executable
 * mappings and lazy library loads are denied after the first final filter.
 * This contract never grants a ROS action, peer, topic or device permission.
 */
#pragma once
#include "wire.h"
#include <array>
#include <cstdint>
#include <span>
#include <string_view>

namespace uwvm2::imported::ros::scene::communication_worker
{
    enum class pipe_role : std::uint8_t { discovery_rx, discovery_tx, user_rx, user_tx };
    inline constexpr std::array roles{pipe_role::discovery_rx,pipe_role::discovery_tx,pipe_role::user_rx,pipe_role::user_tx};
    constexpr bool receiving(pipe_role role) noexcept
    {return role==pipe_role::discovery_rx||role==pipe_role::user_rx;}
    struct role_fd {pipe_role role{};int descriptor{-1};};
    enum class io_direction : std::uint8_t { read=1,write=2,both=3 };
    enum class notification_kind : std::uint8_t { event, poll, pipe };
    /** @brief Existing internal wake-up descriptor, never a network socket.
     * @details The bootstrap verifies these already-open descriptors before the
     * second TSYNC filter. Only approved anonymous pipes/eventfds/eventpolls may appear;
     * no path, regular file, socket, duplicate or standard/control descriptor is
     * accepted. This grants only bounded local synchronization within the worker.
     */
    struct internal_fd {int descriptor{-1};io_direction direction{};notification_kind kind{};};
    struct startup_limits
    {
        std::uint64_t request_bytes{},response_bytes{},thread_count{};
    };
    /** @brief Backend-owned state and a bounded declaration of existing wake-up FDs.
     * @details Borrowed descriptors remain owned by the backend. No externally
     * supplied IPC is read until activate. Thread count is verified separately;
     * the declared limit is not an assertion of a kernel startup thread quota.
     */
    struct prepared_backend
    {
        void* context{};
        std::array<internal_fd,32> notifications{};
        std::size_t notification_count{};
    };
    using prepare_function=bool (*)(wire::bytes,startup_limits const&,prepared_backend&) noexcept;
    using activate_function=bool (*)(void*,std::span<role_fd const>) noexcept;
    using run_function=int (*)(void*) noexcept;
    using destroy_function=void (*)(void*) noexcept;
}
