#pragma once
#include <cstdint>
#include <limits>
#include <mutex>
#include <optional>
#include <type_traits>
#include <utility>

namespace uwvm2::ros_gateway
{
    namespace id_detail
    {
        /** A constant-space permutation of the complete nonzero unsigned domain.
         * The seed selects the first value, not a shorter allocation range.
         * Explicit maximum-to-one transition avoids arithmetic overflow; the
         * allocator becomes exhausted only after every nonzero value was issued.
         * Zero is reserved for failure and never becomes a live correlation ID.
         *
         * This object has no internal mutex. Its owning allocator must serialize
         * all calls. The small unsigned instantiations also permit exhaustive
         * checks of the same full-cycle/exhaustion semantics without a test hook
         * or an artificial production lifetime limit.
         */
        template<class Unsigned> class nonreusing_sequence
        {
            static_assert(std::is_unsigned_v<Unsigned> && !std::is_same_v<Unsigned,bool>,
                "correlation identifiers require a non-bool unsigned integer");
            Unsigned first_,next_;
            bool exhausted_;
        public:
            explicit nonreusing_sequence(Unsigned first) noexcept:
                first_{first},next_{first},exhausted_{first==0} {}
            nonreusing_sequence(nonreusing_sequence const&)=delete;
            nonreusing_sequence& operator=(nonreusing_sequence const&)=delete;
            Unsigned take() noexcept
            {
                if(exhausted_) { return 0; }
                auto value{next_};
                next_=value==std::numeric_limits<Unsigned>::max()?Unsigned{1}:static_cast<Unsigned>(value+1);
                exhausted_=next_==first_;
                return value;
            }
        };
    }

    /** Process-lifetime, synchronized, constant-space correlation identifiers.
     * One shared instance belongs to one identifier domain: all Service states,
     * or all goals of a selected Action adapter, not one endpoint/session. No
     * close, expiry, rejected queue insertion or adapter destruction resets it.
     *
     * The production caller supplies the existing ipc::random_session entropy
     * source. A failed entropy read returns zero without initializing the
     * sequence; a later call may retry. Once initialized, no further entropy is
     * needed, and exhaustion never reseeds or reuses any previously issued ID.
     * This is a checked ordering of integers, not a cryptographic generator.
     *
     * IDs remain opaque correlations, not bearer authority. Response/cancel
     * admission still requires the authenticated Worker/session, separately
     * granted operation/resource/type, and that owner's live pending map.
     * Predictability after observing one ID grants none of those permissions.
     */
    class nonreusing_id_allocator
    {
        std::mutex mutex_;
        std::optional<id_detail::nonreusing_sequence<std::uint64_t>> sequence_;
    public:
        nonreusing_id_allocator()=default;
        nonreusing_id_allocator(nonreusing_id_allocator const&)=delete;
        nonreusing_id_allocator& operator=(nonreusing_id_allocator const&)=delete;
        template<class Entropy> std::uint64_t allocate(Entropy&& entropy)
        {
            std::lock_guard<std::mutex> guard{mutex_};
            if(!sequence_)
            {
                auto first{static_cast<std::uint64_t>(std::forward<Entropy>(entropy)())};
                if(!first) { return 0; }
                sequence_.emplace(first);
            }
            return sequence_->take();
        }
    };
}
