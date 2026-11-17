/** @file
 * @brief Exact-grant authority reduction with a bounded recoverable receipt.
 * @details capability.revoke uses the common seven-argument record carrier.
 * The request is two little-endian u64 words: a nonzero caller retry key and
 * the target capability. The carrier capability MUST equal that target. Its
 * exact grant must contain capability.revoke, and the import must be exposed.
 * This cannot revoke another alias or environment and does not add authority.
 *
 * The four-word response is key, target capability, revocation identity and
 * retirement state (1 = fenced, provider retirement pending; 2 = all local
 * retirement notifications returned). State 2 does not prove worker exit,
 * remote cancellation or a physical safe stop. Existing unknown outcomes and
 * cleanup debt remain owned by their providers. Independent safety supervisors
 * retain responsibility for physical stop and native process reclamation.
 *
 * One fixed receipt per configured grant is reserved before guest execution.
 * An exact retry by the same runtime incarnation and capability returns it even
 * after revocation; a different key/caller/target is denied. Closing the handle
 * or retiring the caller removes that recovery path. No traffic quota or extra
 * allocation can block an already authorized reduction. A short or invalid
 * output is rejected before fencing; failed output after fencing is recovered
 * by exact retry. Reopening any handle cannot resurrect a revoked grant.
 */
#pragma once
#include <array>
#include <cstddef>
#include <cstdint>
#include <string_view>
#define UWVM_ROS_REVOCATION_FUNCTIONS(X) X(capability_revoke, revoke, "capability.revoke", 2, 32)
#ifndef UWVM_SCENE_REVOCATION_CONTRACT_MACROS_ONLY
namespace uwvm2::imported::ros::revocation_contract
{
    enum class operation:std::uint8_t{revoke};
    struct descriptor{operation id;std::string_view name;std::size_t words,bytes;};
    inline constexpr std::array operations{descriptor{operation::revoke,"capability.revoke",2,32}};
    enum class retirement:std::uint64_t{pending=1,notified};
    struct receipt
    {std::uint64_t key{},target{},identity{};retirement state{retirement::pending};};
}
#endif
