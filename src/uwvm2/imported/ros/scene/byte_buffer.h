/** @file
 * @brief Bounded owned scratch bytes for synchronous scene transfers.
 * @details Small transfers use stack storage. Larger transfers allocate without
 * zero-initializing bytes that the checked reader will completely overwrite.
 * Authority and transfer bounds must be checked before resize_for_overwrite().
 * No byte may be parsed or delivered until the complete read succeeds. The
 * buffer is per-call, never shared, retained by a provider or exposed to guests.
 */
#pragma once
#include <uwvm2/utils/container/unique.h>
#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <span>
#include <utility>

namespace uwvm2::imported::ros::scene
{
    class byte_buffer
    {
    public:
        /** @brief Fixed inline scratch extent, including when heap storage is used. */
        inline static constexpr std::size_t inline_capacity=512;
    private:
        std::array<std::uint8_t,inline_capacity> small;
        ::uwvm2::utils::container::std_unique_ptr<std::uint8_t[]> large;
        std::size_t extent{};
    public:
        /** @brief Allocate owned capacity without initializing unread bytes.
         * @details Failure publishes no new extent. Callers must validate
         * their finite operation-specific bound before requesting this memory.
         */
        void resize_for_overwrite(std::size_t size)
        {
            auto next=size>small.size()?::uwvm2::utils::container::make_unique_for_overwrite<std::uint8_t[]>(size):nullptr;
            large=std::move(next);extent=size;
        }
        /** @brief Writable storage whose entire extent must be initialized. */
        std::span<std::uint8_t> writable() noexcept{return {large?large.get():small.data(),extent};}
        /** @brief View only after the caller has initialized the complete extent. */
        std::span<std::uint8_t const> view() const noexcept{return {large?large.get():small.data(),extent};}
    };
}
