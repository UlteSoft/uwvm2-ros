/** @file
 * @brief Complete authenticated-by-deployment, receive-only fixed sensor frames.
 * @details Trusted deployment supplies an already opened nonblocking read pipe
 * or PTY. The guest supplies no descriptor, path, route, direction or symbol.
 * The channel accepts only its configured schema digest and the private source
 * core's actual source incarnation. SHA256 detects complete-frame corruption;
 * it is not peer authentication. Deployment owns and isolates the writer.
 *
 * Each pump performs at most one bounded read after validation of a real live
 * source handle's current sensor.open authority. This validation does not debit
 * or consult the creation rate. No ingress rate attenuation is implemented:
 * explicitly configured frame/storage/syscall bounds are capacities, while
 * sensor.take admission budgets independently govern guest delivery. Incomplete
 * frames never reach the source. Malformed/closed streams latch and retire the
 * source; the parser never scans for a replacement magic or resumes a new peer.
 * Complete frames blocked by the source queue remain retained for a later pump.
 */
#pragma once
#include "sensor_source.h"
#include "device_channel.h"
namespace uwvm2::imported::ros::scene::sensors::ingress_protocol
{
    inline constexpr std::uint64_t magic=UINT64_C(0x524f534e45535255); // URSENSOR
    inline constexpr std::size_t fields_bytes=112,header_bytes=152;
    struct header{source_sample sample;std::uint64_t body_bytes{};buffers::digest schema{},digest{};};
    inline bool valid(header const& value)noexcept
    {
        auto const& v=value.sample;
        return v.source&&v.generation&&value.body_bytes<=buffers::maximum_transfer&&
            (v.stamped?(v.clock&&v.epoch):(!v.clock&&!v.epoch&&!v.source_ns));
    }
    inline bool fields(wire::writer& out,header const& value)noexcept
    {
        auto const& v=value.sample;return out.u64(magic)&&out.u64(value.body_bytes)&&out.u64(v.source)&&out.u64(v.generation)&&out.u64(v.sequence)&&
            out.u64(v.clock)&&out.u64(v.epoch)&&out.i64(v.source_ns)&&out.boolean(v.stamped)&&out.blob(value.schema);
    }
    inline buffers::digest hash(bytes prefix,bytes body)noexcept
    {return devices::protocol::content_hash(prefix,body);}
    /** @brief Encode one complete frame into caller-owned bounded output, without allocation. */
    inline error encode(source_sample sample,buffers::profile const& schema,bytes body,std::span<std::uint8_t> output,std::size_t& written)noexcept
    {
        if(!proposals::envelope_profile(schema)||body.size()>buffers::maximum_transfer||output.size()<header_bytes||body.size()>output.size()-header_bytes)return error::too_large;
        header value{sample,body.size(),schema.sha256,{}};if(!valid(value))return error::invalid;
        auto from=reinterpret_cast<std::uintptr_t>(body.data()),to=reinterpret_cast<std::uintptr_t>(output.data());
        if((from>=to&&from-to<output.size())||(to>from&&to-from<body.size()))return error::invalid;
        std::array<std::uint8_t,fields_bytes> prefix;wire::writer first{prefix};if(!fields(first,value)||first.size()!=fields_bytes)return error::invalid;
        value.digest=hash(prefix,body);wire::writer frame{output.first(header_bytes)};
        if(!fields(frame,value)||!frame.blob(value.digest)||frame.size()!=header_bytes)return error::invalid;
        std::copy(body.begin(),body.end(),output.begin()+header_bytes);written=header_bytes+body.size();return error::success;
    }
    inline error decode_header(bytes input,header& output)noexcept
    {
        if(input.size()!=header_bytes)return error::invalid;
        wire::reader reader{input};header value;std::uint64_t tag{};bytes schema,digest;auto& v=value.sample;
        if(!reader.u64(tag)||tag!=magic||!reader.u64(value.body_bytes)||!reader.u64(v.source)||!reader.u64(v.generation)||!reader.u64(v.sequence)||
            !reader.u64(v.clock)||!reader.u64(v.epoch)||!reader.i64(v.source_ns)||!reader.boolean(v.stamped)||!reader.blob(schema)||schema.size()!=32||
            !reader.blob(digest)||digest.size()!=32||!reader.done()||!valid(value))return error::invalid;
        std::copy(schema.begin(),schema.end(),value.schema.begin());std::copy(digest.begin(),digest.end(),value.digest.begin());output=value;return error::success;
    }
}
namespace uwvm2::imported::ros::scene::sensors
{
    class ingress
    {
    public:
        struct plan{std::uint64_t frame_bytes{},read_bytes{},retained_bytes{};};
        struct usage{std::uint64_t reads{},bytes{},frames{},pending_bytes{},reserved_bytes{};bool active{},busy{},retired{};error failure{};};
    private:
        source& source_;devices::channel::endpoint channel_;mutable std::mutex mutex_;
        ::uwvm2::utils::container::std_unique_ptr<std::uint8_t[]> storage_;
        plan policy_;buffers::profile schema_;ingress_protocol::header header_;
        std::size_t received_{},expected_{ingress_protocol::header_bytes};usage counters_;bool configured_{};
        error poison(std::unique_lock<std::mutex>& lock,error status)noexcept
        {counters_.failure=status;counters_.active=false;channel_.close();lock.unlock();source_.retire();return status;}
    public:
        explicit ingress(source& provider):source_(provider){}
        ingress(ingress const&)=delete;ingress&operator=(ingress const&)=delete;
        ~ingress(){retire();}
        static bool required_storage(plan const& policy,std::uint64_t& output)noexcept
        {
            if(!policy.frame_bytes||policy.frame_bytes>buffers::maximum_transfer||!policy.read_bytes||policy.read_bytes>4096)return false;
            output=policy.frame_bytes+ingress_protocol::header_bytes+::uwvm2::utils::allocator::fast_io_strict::array_prefix<std::uint8_t>;return true;
        }
        /** @brief Duplicate only a trusted, direction-checked endpoint; no read or guest grant occurs. */
        error bind(int descriptor,plan const& policy)
        {
            if(!channel_.same_process())return error::denied;
            std::lock_guard lock{mutex_};if(configured_||counters_.retired)return error::denied;
            std::uint64_t needed{};if(!required_storage(policy,needed)||policy.retained_bytes>INT64_MAX)return error::invalid;
            if(needed>policy.retained_bytes)return error::too_large;
            source::configuration source_policy;auto status=source_.ingress_description(source_policy);if(status!=error::success)return status;
            if(policy.frame_bytes>source_policy.ingress.body_bytes)return error::too_large;
#if defined(__linux__)
            // A byte-framed terminal must already be configured by trusted
            // deployment. No canonical buffering, echo, signal generation or
            // newline/flow-control byte rewriting is accepted or configured here.
            ::termios terminal{};
            if(::tcgetattr(descriptor,&terminal)==0&&
                ((terminal.c_lflag&(ICANON|ECHO|ISIG|IEXTEN))||(terminal.c_iflag&(BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL|IXON|IXOFF))||
                 (terminal.c_oflag&OPOST)||(terminal.c_cflag&CSIZE)!=CS8))return error::denied;
#endif
            status=channel_.bind(descriptor,devices::channel::direction::receive);if(status!=error::success)return status;
            try{storage_=::uwvm2::utils::container::make_unique_for_overwrite<std::uint8_t[]>(policy.frame_bytes+ingress_protocol::header_bytes);}
            catch(std::bad_alloc const&){channel_.close();return error::no_memory;}
            policy_=policy;schema_=source_policy.ingress.schema;counters_.reserved_bytes=needed;counters_.active=true;configured_=true;return error::success;
        }
        /** @brief One bounded read or retry of one retained complete frame; no blocking waits. */
        error pump()
        {
            if(!channel_.same_process())return error::denied;
            source::ingress_lease allowed;auto status=source_.admit_ingress(allowed);if(status!=error::success)return status;
            std::unique_lock lock{mutex_};if(!configured_||counters_.retired)return error::denied;
            if(counters_.failure!=error::success)return counters_.failure;
            if(counters_.busy)return error::again;
            if(received_<expected_)
            {
                if(counters_.reads==UINT64_MAX||counters_.bytes>UINT64_MAX-policy_.read_bytes)return poison(lock,error::too_large);
                std::size_t read{};status=channel_.read({storage_.get()+received_,std::min<std::size_t>(policy_.read_bytes,expected_-received_)},read);
                if(status==error::again)return status;
                if(status!=error::success)return poison(lock,status);
                ++counters_.reads;counters_.bytes+=read;received_+=read;counters_.pending_bytes=received_;
                if(received_==ingress_protocol::header_bytes)
                {
                    status=ingress_protocol::decode_header({storage_.get(),received_},header_);
                    if(status!=error::success||header_.schema!=schema_.sha256||header_.body_bytes>policy_.frame_bytes)return poison(lock,error::invalid);
                    expected_=ingress_protocol::header_bytes+static_cast<std::size_t>(header_.body_bytes);
                }
                if(received_<expected_)return error::again;
            }
            bytes body{storage_.get()+ingress_protocol::header_bytes,static_cast<std::size_t>(header_.body_bytes)};
            if(ingress_protocol::hash({storage_.get(),ingress_protocol::fields_bytes},body)!=header_.digest)return poison(lock,error::invalid);
            if(counters_.frames==UINT64_MAX)return poison(lock,error::too_large);
            counters_.busy=true;lock.unlock();status=source_.ingest(allowed,header_.sample,body);lock.lock();counters_.busy=false;
            if(counters_.retired)return error::cancelled;
            if(status==error::again)return status;
            if(status!=error::success)return poison(lock,status);
            ++counters_.frames;counters_.pending_bytes=received_=0;expected_=ingress_protocol::header_bytes;header_={};return error::success;
        }
        error inspect(usage& output)const noexcept
        {if(!channel_.same_process())return error::denied;std::lock_guard lock{mutex_};output=counters_;return error::success;}
        /** @brief Nonblocking cached receiver diagnostics; no descriptor probing or reads. */
        error inspect_cached(usage& output)const noexcept
        {if(!channel_.same_process())return error::denied;std::unique_lock lock{mutex_,std::try_to_lock};if(!lock.owns_lock())return error::again;output=counters_;return error::success;}
        void retire()noexcept
        {
            if(!channel_.same_process())return;
            {std::lock_guard lock{mutex_};if(counters_.retired)return;counters_.retired=true;counters_.active=false;channel_.close();}
            source_.retire();
        }
    };
}
