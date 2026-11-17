// Included inside uwvm2::runtime::lib after the native-memory pin helper.
namespace exchange_memory_details
{
    /** @brief Wipe both retained exchange extents under one current backend pin.
     * @details Every extent is checked independently using subtraction before
     * pointer arithmetic. A failed payload extent never skips a valid length
     * extent and vice versa. The caller quarantines on any failure. An absent
     * length extent is skipped, matching a single-region input wipe. Neither
     * owner identity nor the pin survives this synchronous call. Relaxed byte
     * atomics preserve concurrent shared-memory accesses; ordinary vectorized
     * memset is not permitted. No allocation or guest callback occurs here.
     */
    inline bool erase(native_memory_t const& memory,::std::uint64_t offset,::std::uint64_t size,
                      ::std::uint64_t length_offset=0,::std::uint64_t length_size=0)noexcept
    {
        return with_native_preload_copy_access(memory,[&](::std::byte* begin,::std::size_t extent)noexcept
        {
            auto region=[&](::std::uint64_t start,::std::uint64_t count)noexcept
            {
                if(start>extent||count>extent-start)return false;
                for(::std::size_t i{};i<static_cast<::std::size_t>(count);++i)
                    ::std::atomic_ref<::std::byte>{begin[static_cast<::std::size_t>(start)+i]}.store(::std::byte{},::std::memory_order_relaxed);
                return true;
            };
            bool payload=region(offset,size),length=!length_size||region(length_offset,length_size);
            return payload&&length;
        });
    }
}
