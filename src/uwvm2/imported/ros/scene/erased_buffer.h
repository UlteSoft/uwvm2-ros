/** @file
 * @brief Owned exchange bytes overwritten before reuse and before deallocation.
 * @details fast_io::secure_clear retains its stores with the implementation's
 * pointer-bearing compiler barrier, including at -O3. No allocator-specific
 * clearing or debug-only assertion is relied on.
 * Allocation is recoverable fast_io allocation. An allocation failure leaves
 * existing bytes owned; destruction still erases them. This primitive covers
 * its own storage, not independently copied guest data, CPU caches or registers.
 */
#pragma once
#include <uwvm2/utils/container/unique.h>
#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
#include <utility>
namespace uwvm2::imported::ros::scene
{
    inline void erase_bytes(std::span<std::uint8_t> bytes)noexcept
    {
        if(!bytes.empty())::fast_io::secure_clear(bytes.data(),bytes.size());
    }
    class erased_buffer
    {
        std::array<std::uint8_t,512> small;
        ::uwvm2::utils::container::std_unique_ptr<std::uint8_t[]> large;
        std::size_t extent{};
    public:
        erased_buffer()=default;
        erased_buffer(erased_buffer const&)=delete;erased_buffer& operator=(erased_buffer const&)=delete;
        /** @brief Erase every byte owned by this buffer before releasing storage.
         * @details resize_for_overwrite clears the complete previous extent
         * before shrinking or switching storage. Bytes outside the live extent
         * have either already been erased or were never owned/written by this
         * buffer; erasing all 512 inline bytes again provides no extra payload
         * erasure. The retained capacity of a route slot remains its extent.
         */
        ~erased_buffer(){clear();}
        void clear()noexcept
        {erase_bytes(writable());large.reset();extent=0;}
        void resize_for_overwrite(std::size_t size)
        {
            auto next=size>small.size()?::uwvm2::utils::container::make_unique_for_overwrite<std::uint8_t[]>(size):nullptr;
            clear();large=std::move(next);extent=size;
        }
        std::span<std::uint8_t> writable()noexcept{return {large?large.get():small.data(),extent};}
        std::span<std::uint8_t const> view()const noexcept{return {large?large.get():small.data(),extent};}
        /** @brief Wipe retained storage without releasing its capacity. */
        void erase()noexcept{erase_bytes(writable());}
    };
}
