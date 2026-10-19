/** @file
 * @brief One owned native metadata/canonical-body queue for the new RMW path.
 *
 * A source grant is authenticated and leased by the future transport, outside
 * this class. These numeric associations are anti-mixup checks, not authority.
 * No legacy event queue, separately sampled metadata, or last-message cache is
 * used. Every accepted record owns its exact encoded URMI header and body.
 */
#pragma once
#include <uwvm2/imported/ros/ipc/rmw_metadata.h>
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <mutex>
#include <new>
#include <vector>

namespace uwvm2::ros_gateway::native_rmw_queue
{
    namespace wire=::uwvm2::imported::ros::ipc::rmw_endpoint;
    namespace info=::uwvm2::imported::ros::ipc::rmw_metadata;
    enum class overflow { drop_oldest,drop_newest,reject };
    struct options
    {
        std::size_t records{},bytes{},body_bytes{};
        overflow policy{overflow::reject};
    };
    inline bool valid(options const& o) noexcept
    {
        return o.records&&o.records<=65536&&o.body_bytes<=::uwvm2::imported::ros::ipc::max_payload_bytes-info::header_bytes&&
            o.bytes>=info::header_bytes&&o.bytes<=1073741824&&o.body_bytes<=o.bytes-info::header_bytes&&
            (o.policy==overflow::drop_oldest||o.policy==overflow::drop_newest||o.policy==overflow::reject);
    }
    struct statistics
    {
        std::uint64_t accepted{},consumed{},drop_oldest{},drop_newest{},rejected{},retired_records{},
            callback_errors{},metadata_unavailable{},before_activation{},last_sequence{},last_reservation{},high_records{},high_bytes{};
        std::uint32_t first_error{};
        std::size_t records{},bytes{};
        bool active{},retired{},reserved{};
    };
    /** A transactional owned snapshot. Exactly one header followed by its body.
     * Never pass its storage into Guest memory until the current grants, full
     * output extent and native metadata requirements have all been checked.
     * Destruction alone does not consume; call commit or cancel explicitly.
     */
    struct snapshot
    {
        std::uint64_t sequence{},reservation{}; // Record identity, separate borrowing-attempt token.
        std::size_t size{};
        std::unique_ptr<std::uint8_t[]> bytes;
    };
    class bounded_queue
    {
        struct record
        {
            std::array<std::uint8_t,info::header_bytes> header{};
            std::vector<std::uint8_t> body;
            std::uint64_t sequence{};
            std::size_t charge{};
            // An unsupported actual metadata capture remains attached to this
            // same record. It cannot be mistaken for a complete native receipt.
            std::uint32_t capture_error{};
        };
        options options_;
        wire::binding owner_;
        std::uint64_t incarnation_{};
        std::vector<record> slots_;
        std::size_t head_{},size_{},bytes_{};
        // A record sequence and a borrowing attempt are different identities.
        // Cancel followed by another peek keeps the former but changes the
        // latter, so an old attempt cannot act on the new reservation (ABA).
        std::uint64_t reserved_{},reservation_{};
        statistics statistics_;
        mutable std::mutex mutex_;
        bounded_queue(options o,wire::binding owner,std::uint64_t incarnation):
            options_{o},owner_{owner},incarnation_{incarnation},slots_(o.records){}
        static void bump(std::uint64_t& n) noexcept {if(n!=UINT64_MAX){++n;}}
        bool owns(wire::binding const& owner,std::uint64_t incarnation) const noexcept
        {return wire::same(owner,owner_)&&incarnation==incarnation_;}
        void note(std::uint32_t code) noexcept
        {bump(statistics_.callback_errors);if(!statistics_.first_error){statistics_.first_error=code;}}
        void pop() noexcept
        {
            auto& r{slots_[head_]};bytes_-=r.charge;
            // Release payload capacity now. clear() alone would retain bytes
            // outside the current queue charge and falsify the memory ledger.
            std::vector<std::uint8_t>{}.swap(r.body);r.charge=0;r.sequence=0;r.capture_error=0;
            head_=(head_+1)%slots_.size();--size_;
        }
    public:
        bounded_queue(bounded_queue const&)=delete;
        bounded_queue& operator=(bounded_queue const&)=delete;
        static std::uint32_t create(options o,wire::binding const& owner,std::uint64_t incarnation,
            std::shared_ptr<bounded_queue>& output) noexcept
        {
            if(!valid(o)||!wire::valid(owner)||!incarnation){return 22;}
            try{auto q{std::shared_ptr<bounded_queue>{new bounded_queue{o,owner,incarnation}}};output=std::move(q);return 0;}
            catch(std::bad_alloc const&){return 12;}catch(...){return 5;}
        }
        std::uint32_t activate() noexcept
        {
            std::lock_guard<std::mutex> lock{mutex_};
            if(statistics_.retired){return 125;}if(statistics_.active){return 17;}
            statistics_.active=true;return 0;
        }
        /** Cheap pre-encoding refusal. This does not reserve admission: enqueue
         * checks retirement again at the actual business commit point. */
        std::uint32_t ingress() noexcept
        {
            std::lock_guard<std::mutex> lock{mutex_};if(statistics_.retired){return 125;}
            if(!statistics_.active){bump(statistics_.before_activation);return 11;}return 0;
        }
        /** Encode/validate before evicting anything. The capacity charge covers
         * the retained vector allocation, not merely its logical wire extent.
         * Preallocated record controls are separately bounded by records<=65536.
         * Incoming payload is moved only on successful acceptance. Refusals do
         * not modify it, existing records or the next accepted sequence.
         */
        std::uint32_t enqueue(info::header h,std::vector<std::uint8_t>& body,std::uint32_t capture_error=0) noexcept
        {
            std::lock_guard<std::mutex> lock{mutex_};
            if(statistics_.retired){return 125;}
            if(!statistics_.active){bump(statistics_.before_activation);return 11;}
            if(!owns(h.owner,h.incarnation)||h.kind!=::uwvm2::imported::ros::ipc::event_kind::topic_message||
                (capture_error&&capture_error!=95)){note(22);return 22;}
            if(statistics_.last_sequence==UINT64_MAX){note(90);return 90;}
            if(body.size()>options_.body_bytes||body.capacity()>options_.bytes-info::header_bytes){note(90);return 90;}
            auto charge{info::header_bytes+body.capacity()};
            h.sequence=statistics_.last_sequence+1;h.payload_bytes=body.size();
            std::array<std::uint8_t,info::header_bytes> encoded{};
            if(!info::encode(h,encoded)){note(22);return 22;}
            auto full=[&]() noexcept{return size_==slots_.size()||charge>options_.bytes-bytes_;};
            if(full())
            {
                if(options_.policy==overflow::drop_newest){bump(statistics_.drop_newest);return 90;}
                if(options_.policy==overflow::reject||reserved_){bump(statistics_.rejected);return 90;}
                // The new record fits an empty queue. No later allocation or
                // validation can fail after this explicit drop-oldest policy.
                while(full()){pop();bump(statistics_.drop_oldest);}
            }
            auto& destination{slots_[(head_+size_)%slots_.size()]};
            destination.header=encoded;destination.body=std::move(body);destination.sequence=h.sequence;
            destination.charge=charge;destination.capture_error=capture_error;
            ++size_;bytes_+=charge;statistics_.last_sequence=h.sequence;bump(statistics_.accepted);
            if(capture_error||!info::complete(h))
            {bump(statistics_.metadata_unavailable);if(!statistics_.first_error){statistics_.first_error=95;}}
            statistics_.high_records=std::max<std::uint64_t>(statistics_.high_records,size_);
            statistics_.high_bytes=std::max<std::uint64_t>(statistics_.high_bytes,bytes_);return 0;
        }
        /** Require complete actual native metadata and its ABI extent BEFORE
         * allocation, reservation, output change or consumption. All-zero GID
         * value bytes are valid. expected_gid_bytes is an admitted consumer ABI
         * requirement, never an attempt to infer the publisher identity.
         */
        std::uint32_t peek(wire::binding const& owner,std::uint64_t incarnation,std::size_t expected_gid_bytes,
            snapshot& output,std::size_t capacity=::uwvm2::imported::ros::ipc::max_payload_bytes) noexcept
        {
            std::lock_guard<std::mutex> lock{mutex_};if(!owns(owner,incarnation)){return 13;}
            if(statistics_.retired){return 125;}if(!size_){return 11;}if(reserved_){return 17;}
            auto const& r{slots_[head_]};info::header h{};
            if(!info::decode(r.header.data(),r.header.size(),r.body.size(),h)){return 5;}
            if(r.capture_error||!info::complete(h)||!expected_gid_bytes||expected_gid_bytes>32||h.gid_size!=expected_gid_bytes){return 95;}
            // Optional independently validated consumer wire capacity includes
            // header+body. A too-small destination never borrows a token.
            if(capacity<info::header_bytes+r.body.size()){return 90;}
            // Tokens are positive, never reset or reused. Unsupported metadata
            // wins before exhaustion and changes neither token nor queue state.
            if(statistics_.last_reservation==UINT64_MAX){return 90;}
            try
            {
                snapshot next{};next.sequence=r.sequence;next.size=info::header_bytes+r.body.size();
                next.reservation=statistics_.last_reservation+1;
                next.bytes=std::make_unique<std::uint8_t[]>(next.size);
                std::copy(r.header.begin(),r.header.end(),next.bytes.get());
                std::copy(r.body.begin(),r.body.end(),next.bytes.get()+info::header_bytes);
                auto token{next.reservation};output=std::move(next);
                reserved_=r.sequence;reservation_=token;statistics_.last_reservation=token;return 0;
            }
            catch(std::bad_alloc const&){return 12;}catch(...){return 5;}
        }
        std::uint32_t commit(wire::binding const& owner,std::uint64_t incarnation,std::uint64_t sequence,
            std::uint64_t reservation) noexcept
        {
            std::lock_guard<std::mutex> lock{mutex_};if(!owns(owner,incarnation)){return 13;}
            if(statistics_.retired){return 125;}
            if(!sequence||!reservation||reserved_!=sequence||reservation_!=reservation||!size_||slots_[head_].sequence!=sequence){return 13;}
            pop();reserved_=reservation_=0;bump(statistics_.consumed);return 0;
        }
        std::uint32_t cancel(wire::binding const& owner,std::uint64_t incarnation,std::uint64_t sequence,
            std::uint64_t reservation) noexcept
        {
            std::lock_guard<std::mutex> lock{mutex_};if(!owns(owner,incarnation)){return 13;}
            if(statistics_.retired){return 125;}
            if(!sequence||!reservation||reserved_!=sequence||reservation_!=reservation){return 13;}
            reserved_=reservation_=0;return 0;
        }
        void callback_error(std::uint32_t code) noexcept
        {std::lock_guard<std::mutex> lock{mutex_};note(code?code:5);}
        statistics observe() const noexcept
        {
            std::lock_guard<std::mutex> lock{mutex_};auto s{statistics_};
            s.records=size_;s.bytes=bytes_;s.reserved=reserved_!=0;return s;
        }
        /** This is the ingress/consumer retirement linearization point. A native
         * callback already encoding may finish its local work, but cannot add
         * business after this returns. Native destruction/notify follows in the
         * owning endpoint. No mutex waits for native executor completion.
         */
        void retire() noexcept
        {
            std::lock_guard<std::mutex> lock{mutex_};statistics_.active=false;statistics_.retired=true;reserved_=reservation_=0;
            while(size_){pop();bump(statistics_.retired_records);}
        }
    };
}
