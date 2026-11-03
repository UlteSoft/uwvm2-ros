// SPDX-License-Identifier: Apache-2.0
#pragma once
#include "../ipc/protocol.h"
#include <array>
#include <atomic>
#include <charconv>
#include <chrono>
#include <mutex>
#include <memory>
#include <new>
#include <span>
#include <string_view>
#ifndef UWVM_MODULE
#include <fast_io_crypto.h>
#endif
#if defined(__linux__)
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cerrno>
#endif

// An opt-in, separate host-only journal. This never supplies capabilities or
// native CDR evidence. Source approval and collector authentication are external
// admission requirements; a journal alone is not a production qualification.
namespace uwvm2::imported::ros::buffer_audit
{
    using digest=::std::array<::std::uint8_t,32>;
    inline digest hash(::std::span<::std::uint8_t const> bytes) noexcept
    {
        ::fast_io::sha256_context value{};digest out{};
        auto first{reinterpret_cast<::std::byte const*>(bytes.data())};
        if(!bytes.empty()) {value.update(first,first+bytes.size());}
        value.do_final();value.digest_to_byte_ptr(reinterpret_cast<::std::byte*>(out.data()));return out;
    }
    inline digest session_tag(::std::string_view run,unsigned backend,::std::uint64_t session) noexcept
    {
        constexpr char domain[]{"uwvm-sdk-ros-native-session-tag-v1"};
        ::std::array<::std::uint8_t,sizeof(domain)+2+128+1+8> bytes{};::std::size_t size{};
        for(auto c:domain) {bytes[size++]=static_cast<::std::uint8_t>(c);} // includes exactly one NUL
        bytes[size++]=static_cast<::std::uint8_t>(run.size());bytes[size++]=static_cast<::std::uint8_t>(run.size()>>8);
        for(auto c:run) {bytes[size++]=static_cast<::std::uint8_t>(c);}
        bytes[size++]=static_cast<::std::uint8_t>(backend);
        for(unsigned i{};i!=8;++i) {bytes[size++]=static_cast<::std::uint8_t>(session>>(8*i));}
        return hash({bytes.data(),size});
    }
    class record
    {
        ::std::span<char> data_;::std::size_t size_{};bool valid_{true};
    public:
        explicit record(::std::span<char> storage) noexcept : data_{storage} {}
        void append(::std::string_view text) noexcept
        {if(!valid_||text.size()>data_.size()-size_) {valid_=false;return;}for(auto c:text) {data_[size_++]=c;}}
        void quoted(::std::string_view text) noexcept
        {
            constexpr char digits[]{"0123456789abcdef"};append("\"");
            for(unsigned char c:text)
            {
                if(c=='"'||c=='\\') {char pair[]{'\\',static_cast<char>(c)};append({pair,2});}
                else if(c<32) {char escaped[]{'\\','u','0','0',digits[c>>4],digits[c&15]};append({escaped,6});}
                else {char one{static_cast<char>(c)};append({&one,1});}
            }
            append("\"");
        }
        template<typename T> void number(T value) noexcept
        {::std::array<char,32> text;auto r{::std::to_chars(text.data(),text.data()+text.size(),value)};if(r.ec!=::std::errc{}){valid_=false;return;}append({text.data(),static_cast<::std::size_t>(r.ptr-text.data())});}
        void boolean(bool value) noexcept {append(value?"true":"false");}
        void hex(::std::span<::std::uint8_t const> bytes) noexcept
        {constexpr char digits[]{"0123456789abcdef"};append("\"");for(auto b:bytes){char pair[]{digits[b>>4],digits[b&15]};append({pair,2});}append("\"");}
        ::std::string_view view() const noexcept {return valid_ ? ::std::string_view{data_.data(),size_} : ::std::string_view{};}
    };
    struct transfer
    {
        ::std::string_view module,alias,operation,resource,type,identity;
        digest wasm{};::std::uint64_t wasm_bytes{},instance{},generation{},handle{},function_index{};
        unsigned backend{},memory{};
        ::std::uint64_t input_offset{},output_offset{},capacity{},length_offset{},response_size{};
        bool source_known{},input_read{},output_written{},length_written{},response_authorized{},input_snapshot_failed{},payload_over_limit{};
        ::std::span<::std::uint8_t const> input{},output{};
        ipc::checked_call checked{};
        ::std::uint32_t error{};
    };
    struct limits
    {
        ::std::uint64_t payload_bytes{8192},operation_records{20000},total_bytes{67108864},record_bytes{32768};
    };
    class channel
    {
        ::std::mutex mutex_{};::std::atomic<bool> enabled_{};
        ::std::array<char,128> run_{};::std::size_t run_size_{};
        int fd_{-1};::std::uint64_t offset_{},sequence_{},dropped_{},suppressed_{};
        ::std::uint32_t pid_{};bool configured_{},limits_selected_{};
        limits limits_{};
        ::std::array<char,64> profile_{};
        // Allocated once at configuration, bounded by maximum_record_bytes.
        // No storage grows with the number of operations or the disk quota.
        static constexpr ::std::uint64_t maximum_record_bytes=33554432;
        ::std::unique_ptr<char[]> buffer_{};
        ::std::uint64_t attempted_{},emitted_{},source_suppressed_{},payload_suppressed_{},count_suppressed_{},record_dropped_{},byte_dropped_{},io_dropped_{},capture_dropped_{};
        static constexpr ::std::uint64_t footer_reserve=4096;
        static void increment(::std::uint64_t& value) noexcept {if(value!=UINT64_MAX){++value;}}
        bool write(::std::string_view bytes) noexcept
        {
#if defined(__linux__)
            struct stat info{};
            if(bytes.empty()||::getpid()!=static_cast<int>(pid_)||::fstat(fd_,&info)||info.st_size<0||static_cast<::std::uint64_t>(info.st_size)!=offset_) {return false;}
            ::std::size_t done{};
            while(done<bytes.size())
            {
                auto n{::pwrite(fd_,bytes.data()+done,bytes.size()-done,static_cast<off_t>(offset_+done))};
                if(n<0&&errno==EINTR) {continue;}if(n<=0){return false;}done+=static_cast<::std::size_t>(n);
            }
            offset_+=done;return true;
#else
            (void)bytes;return false;
#endif
        }
        void prefix(record& r,::std::string_view kind) noexcept
        {
            r.append("{\"format\":\"uwvm-ros-buffer-journal-v1\",\"producer\":\"uwvm2-ros/ros-host\",\"kind\":");r.quoted(kind);
            r.append(",\"run_id\":");r.quoted({run_.data(),run_size_});r.append(",\"pid\":");r.number(pid_);
            r.append(",\"sequence\":");r.number(sequence_);
            r.append(",\"monotonic_ns\":");r.number(::std::chrono::duration_cast<::std::chrono::nanoseconds>(::std::chrono::steady_clock::now().time_since_epoch()).count());
        }
    public:
        bool enabled() const noexcept {return enabled_.load(::std::memory_order_relaxed);}
        // Configuration is immutable before any Guest thread is started.
        ::std::uint64_t payload_capacity() const noexcept {return limits_.payload_bytes;}
        bool configure_limits(limits value,::std::string_view profile) noexcept
        {
            ::std::lock_guard guard{mutex_};
            if(configured_||limits_selected_||!value.payload_bytes||value.payload_bytes>ipc::max_payload_bytes||
               !value.operation_records||value.operation_records>UINT32_MAX||value.total_bytes<8192||
               value.total_bytes>(UINT64_C(1)<<42)||value.record_bytes<1024||value.record_bytes>maximum_record_bytes||profile.size()!=64)
            {return false;}
            bool nonzero{};
            for(auto c:profile){if(!((c>='0'&&c<='9')||(c>='a'&&c<='f'))){return false;}nonzero|=c!='0';}
            if(!nonzero){return false;}
            limits_=value;for(::std::size_t i{};i!=profile_.size();++i){profile_[i]=profile[i];}
            limits_selected_=true;return true;
        }
        bool configure(::std::string_view run,unsigned descriptor) noexcept
        {
#if defined(__linux__)
            ::std::lock_guard guard{mutex_};
            if(configured_||run.empty()||run.size()>128||descriptor<3||descriptor>1048575) {return false;}
            for(auto c:run) {if(!((c>='A'&&c<='Z')||(c>='a'&&c<='z')||(c>='0'&&c<='9')||c=='.'||c=='_'||c=='-')){return false;}}
            auto buffer=::std::unique_ptr<char[]>{new(::std::nothrow) char[static_cast<::std::size_t>(limits_.record_bytes)]};
            if(!buffer){return false;}
            int copy{::fcntl(static_cast<int>(descriptor),F_DUPFD_CLOEXEC,3)};if(copy<0){return false;}
            struct stat info{};int flags{::fcntl(copy,F_GETFL)};
            if(::fstat(copy,&info)||!S_ISREG(info.st_mode)||info.st_nlink!=1||info.st_uid!=::geteuid()||
               (info.st_mode&07777)!=0600||info.st_size!=0||flags<0||(flags&O_ACCMODE)!=O_WRONLY||(flags&(O_APPEND|O_PATH)))
            {::close(copy);return false;}
            // No stdout, socket, pipe, inherited pre-existing data or Guest
            // descriptor. Consume the supplied descriptor in this process,
            // retaining only a CLOEXEC duplicate. The launcher keeps its own
            // process copy and responsibility for collector/source identity.
            ::close(static_cast<int>(descriptor));
            fd_=copy;run_size_=run.size();for(::std::size_t i{};i!=run.size();++i){run_[i]=run[i];}
            pid_=static_cast<::std::uint32_t>(::getpid());buffer_=::std::move(buffer);configured_=true;
            record r{{buffer_.get(),static_cast<::std::size_t>(limits_.record_bytes)}};prefix(r,"begin");
            r.append(",\"max_records\":");r.number(limits_.operation_records);r.append(",\"max_bytes\":");r.number(limits_.total_bytes);
            r.append(",\"max_payload_bytes\":");r.number(limits_.payload_bytes);r.append(",\"max_record_bytes\":");r.number(limits_.record_bytes);
            r.append(",\"footer_reserved_bytes\":");r.number(footer_reserve);r.append(",\"requested_profile_sha256\":");
            if(limits_selected_){r.quoted({profile_.data(),profile_.size()});}else{r.append("null");}
            r.append(",\"limit_scope\":\"process-global-all-modules\",\"production_admission\":false}\n");
            if(!write(r.view())) {::close(fd_);fd_=-1;return false;}
            enabled_.store(true,::std::memory_order_relaxed);return true;
#else
            (void)run;(void)descriptor;return false;
#endif
        }
        void emit(transfer const& v) noexcept
        {
            if(!enabled()) {return;}::std::lock_guard guard{mutex_};
            if(!enabled()) {return;}
            increment(attempted_);
            if(!v.source_known){increment(suppressed_);increment(source_suppressed_);return;}
            if(v.input_snapshot_failed){increment(dropped_);increment(capture_dropped_);return;}
            if(v.payload_over_limit||v.input.size()>limits_.payload_bytes||v.output.size()>limits_.payload_bytes)
            {increment(suppressed_);increment(payload_suppressed_);return;}
            if(sequence_>=limits_.operation_records){increment(suppressed_);increment(count_suppressed_);return;}
            record r{{buffer_.get(),static_cast<::std::size_t>(limits_.record_bytes)}};++sequence_;prefix(r,"operation");
            r.append(",\"module\":");r.quoted(v.module);r.append(",\"module_sha256\":");r.hex(v.wasm);
            r.append(",\"module_bytes\":");r.number(v.wasm_bytes);r.append(",\"module_instance\":");r.number(v.instance);
            r.append(",\"runtime_generation\":");r.number(v.generation);r.append(",\"builtin_function_index\":");r.number(v.function_index);
            r.append(",\"backend\":");r.number(v.backend);r.append(",\"owner_handle\":");r.number(v.handle);
            r.append(",\"alias\":");r.quoted(v.alias);r.append(",\"operation\":");r.quoted(v.operation);
            r.append(",\"resource\":");r.quoted(v.resource);r.append(",\"type\":");r.quoted(v.type);r.append(",\"identity\":");r.quoted(v.identity);
            r.append(",\"memory_index\":");r.number(v.memory);r.append(",\"input_offset\":");r.number(v.input_offset);
            r.append(",\"output_offset\":");r.number(v.output_offset);r.append(",\"output_capacity\":");r.number(v.capacity);
            r.append(",\"length_slot_offset\":");r.number(v.length_offset);r.append(",\"response_size\":");r.number(v.response_size);
            r.append(",\"guest_memory_read\":");r.boolean(v.input_read);r.append(",\"guest_memory_written\":");r.boolean(v.output_written);
            r.append(",\"length_slot_written\":");r.boolean(v.length_written);r.append(",\"response_authorized\":");r.boolean(v.response_authorized);
            r.append(",\"error\":");r.number(v.error);
            r.append(",\"request_id\":");r.number(v.checked.request_id);r.append(",\"peer_pid\":");r.number(v.checked.peer_pid);r.append(",\"peer_uid\":");r.number(v.checked.peer_uid);
            r.append(",\"session_tag\":");
            if(v.checked.session_id) {r.hex(session_tag({run_.data(),run_size_},v.backend,v.checked.session_id));}else{r.append("null");}
            r.append(",\"possible_peer_effects\":");r.boolean(v.checked.possible_peer_effects);
            r.append(",\"response_checked\":");r.boolean(v.checked.response_checked);
            r.append(",\"canonical_input_hex\":");r.hex(v.input);r.append(",\"canonical_input_sha256\":");r.hex(hash(v.input));
            r.append(",\"canonical_output_hex\":");r.hex(v.output);r.append(",\"canonical_output_sha256\":");r.hex(hash(v.output));r.append("}\n");
            auto bytes{r.view()};
            if(bytes.empty()){increment(dropped_);increment(record_dropped_);return;}
            if(offset_>limits_.total_bytes-footer_reserve||bytes.size()>limits_.total_bytes-footer_reserve-offset_)
            {increment(dropped_);increment(byte_dropped_);return;}
            if(!write(bytes)){increment(dropped_);increment(io_dropped_);enabled_.store(false,::std::memory_order_relaxed);}
            else{increment(emitted_);}
        }
        // proc_exit bypasses C++ destructors. The executable shutdown boundary
        // calls this explicitly; the destructor is an idempotent fallback.
        void finish() noexcept
        {
#if defined(__linux__)
            ::std::lock_guard guard{mutex_};
            if(fd_<0) {return;}
            enabled_.store(false,::std::memory_order_relaxed);
            if(::getpid()==static_cast<int>(pid_))
            {
                ++sequence_;::std::uint64_t footer_bytes{};
                // Fixed-point sizing includes this footer in total_file_bytes.
                // At most a few decimal-width changes, never an unbounded loop.
                for(unsigned attempt{};attempt!=8;++attempt)
                {
                    record r{{buffer_.get(),static_cast<::std::size_t>(limits_.record_bytes)}};prefix(r,"end");
                    r.append(",\"dropped_records\":");r.number(dropped_);r.append(",\"suppressed_records\":");r.number(suppressed_);
                    r.append(",\"attempted_operations\":");r.number(attempted_);r.append(",\"emitted_operations\":");r.number(emitted_);
                    r.append(",\"source_suppressed\":");r.number(source_suppressed_);r.append(",\"payload_suppressed\":");r.number(payload_suppressed_);
                    r.append(",\"count_suppressed\":");r.number(count_suppressed_);r.append(",\"record_dropped\":");r.number(record_dropped_);
                    r.append(",\"byte_dropped\":");r.number(byte_dropped_);r.append(",\"io_dropped\":");r.number(io_dropped_);
                    r.append(",\"capture_dropped\":");r.number(capture_dropped_);
                    r.append(",\"bytes_before_footer\":");r.number(offset_);r.append(",\"total_file_bytes\":");r.number(offset_+footer_bytes);r.append("}\n");
                    auto bytes{r.view()};
                    if(bytes.empty()||bytes.size()>footer_reserve||offset_>limits_.total_bytes||bytes.size()>limits_.total_bytes-offset_){break;}
                    if(bytes.size()==footer_bytes){(void)write(bytes);break;}
                    footer_bytes=bytes.size();
                }
            }
            ::close(fd_);fd_=-1;
#endif
        }
        ~channel() {finish();}
    };
    inline channel journal{};
}
