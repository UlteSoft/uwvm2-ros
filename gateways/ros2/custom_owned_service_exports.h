/* SPDX-License-Identifier: Apache-2.0 */
#pragma once
#include <cstddef>
#include <string_view>
namespace uwvm2::ros_gateway::custom_owned_services
{
    /** Immutable SOURCE-selected independent native export, not a runtime
     * property read back and called its own expected identity. Every reachable
     * root/Request/Response/Event/Time/Info/nested Message needs a separate row.
     * Source SHA is over this exact native TypeSource's full counted raw bytes;
     * implicit empty sources have their genuine SHA256(empty), never zero.
     * actual_export_sha256 and SDK receipt SHA associate original owned export
     * and full native SDK/MD/ELF/maps evidence. They confer no Host authority.
     */
    struct exported_identity {std::string_view type,native_identity,encoding,source_sha256;};
    struct export_view
    {
        exported_identity const* records{};std::size_t count{};
        std::string_view actual_export_sha256,actual_sdk_receipt_sha256;
        // Explicit source-generated REAL Native sizeof/offsetof closure.
        bool (*typed_layout)(std::string_view,void const*)noexcept{};
    };
    template<class Service>struct independent_native_exports
    {static export_view get()noexcept{return {};}}; // ABSENT, not a default hash.
}
// Trusted separately acquired/generated immutable header, selected by build.
// Its missing data leaves all custom factories95 before native creation. Never
// define this from IPC, a guessed catalog digest or an ambient native getter.
#ifdef UWVM_ROS_CUSTOM_ROS2_OWNED_EXPORT_HEADER
#include UWVM_ROS_CUSTOM_ROS2_OWNED_EXPORT_HEADER
#endif
