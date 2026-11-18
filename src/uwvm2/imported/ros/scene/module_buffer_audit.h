/** @file @brief Bounded, operator-selected capture of committed registered messages.
 * @details Only the router can present an authenticated message span after a
 * successful business operation. The caller chooses the exact environment,
 * route and visible stage. The writer has four preallocated entries, a bounded
 * per-message payload, and independent total record and file-byte limits.
 * Guest dispatch never performs filesystem I/O or allocates capture storage.
 * Audit pressure and disk failure are observable in host counters and cannot
 * change an application result. Every consumed queue entry is overwritten.
 */
#pragma once
#include "identifier.h"
#include <uwvm2/utils/container/unique.h>
#include <cstdint>
#include <span>
#include "../common/audit_file.h"
#include <array>
#include <atomic>
#include <cstring>
#include <mutex>
#if defined(__linux__)
#include <cerrno>
#include <pthread.h>
#include <signal.h>
#endif
// Explicit global attachment gives textual runtime and named-module CLI
// consumers one private journal and one shutdown target.
extern "C++" {
namespace uwvm2::imported::ros::scene::module_buffer_audit
{
    /** @brief Closed stage codes used in private diagnostic data frames. */
    enum class stage : std::uint8_t { submit=1, take=2, respond=3, result=4 };
    inline constexpr std::uint8_t bit(stage value) noexcept
    {return static_cast<std::uint8_t>(1u << (static_cast<unsigned>(value)-1));}
    /** @brief Accept one exact public stage spelling from a host selector. */
    inline bool parse_stage(std::string_view text,stage& value) noexcept
    {
        constexpr std::array<std::string_view,4> names{"submit","take","respond","result"};
        for(std::size_t i{};i!=names.size();++i)
            if(names[i]==text){value=static_cast<stage>(i+1);return true;}
        return false;
    }
    /** @brief Immutable ceilings shared by all selected scopes in one process. */
    struct limits {std::uint64_t records{},bytes{},payload{};};
    /** @brief Detached counters without message content, guest pointers or tokens. */
    struct stats {std::uint64_t enqueued{},written{},oversize{},budget{},full{},io_errors{};};
    class journal
    {
        static constexpr std::size_t slots=4, fixed_bytes=28, footer_bytes=56;
        static constexpr std::array<std::uint8_t,8> magic{'W','R','O','S','C','A','P',1};
        mutable std::mutex mutex_;
        std::atomic<std::uint32_t> sequence_{};
        void notify() noexcept {sequence_.fetch_add(1,std::memory_order_release);sequence_.notify_one();}
        limits limits_{};
        stats stats_{};
        ::uwvm2::utils::container::std_unique_ptr<std::uint8_t[]> storage_;
        void clear_storage() noexcept
        {
            if(storage_){::fast_io::secure_clear(storage_.get(),slots*stride_);storage_.reset();}
        }
        std::array<std::size_t,slots> lengths_{};
        std::size_t stride_{},head_{},queued_{};
        std::uint64_t charged_{},offset_{};
        std::atomic<bool> running_{};
        bool stopping_{},used_{};
#if defined(__linux__)
        int fd_{-1};
        std::atomic<pid_t> pid_{};
        dev_t device_{};ino_t inode_{};
        pthread_t thread_{};
        std::atomic<unsigned> startup_{};
        bool joinable_{};
        static void add(std::uint64_t& n) noexcept {if(n!=UINT64_MAX)++n;}
        static void put(std::span<std::uint8_t> b,std::size_t offset,std::uint64_t value,std::size_t bytes) noexcept
        {for(std::size_t i{};i<bytes;++i)b[offset+i]=static_cast<std::uint8_t>(value>>(i*8));}
        bool write_file(std::span<std::uint8_t const> data) noexcept
        {
            if(::getpid()!=pid_.load(std::memory_order_relaxed))return false;
            // Pin the open file description for this entire write. A concurrent
            // reassignment of the configured numeric FD between validation and a
            // partial pwrite cannot redirect the remainder of the message.
            struct pinned_file
            {
                int value;
                ~pinned_file(){if(value>=0)::close(value);}
            } target{::fcntl(fd_,F_DUPFD_CLOEXEC,3)};
            struct stat info{};
            if(target.value<0 || !audit_file::validate(target.value,static_cast<off_t>(offset_),info) ||
               info.st_dev!=device_ || info.st_ino!=inode_)return false;
            std::size_t done{};
            while(done<data.size())
            {
                auto n=::pwrite(target.value,data.data()+done,data.size()-done,static_cast<off_t>(offset_+done));
                if(n<0 && errno==EINTR)continue;
                if(n<=0)return false;
                done+=static_cast<std::size_t>(n);
            }
            offset_+=done;return true;
        }
        void consume() noexcept
        {
            // A file-size limit can send SIGXFSZ as well as returning EFBIG.
            // Mask it in this private worker before any header or payload I/O;
            // the guest's signal dispositions and caller mask stay untouched.
            sigset_t signals;
            ::sigemptyset(&signals);::sigaddset(&signals,SIGXFSZ);::sigaddset(&signals,SIGPIPE);
            bool healthy=!::pthread_sigmask(SIG_BLOCK,&signals,nullptr) && write_file(magic);
            startup_.store(healthy?1u:2u,std::memory_order_release);startup_.notify_one();
            if(!healthy)return;
            std::unique_lock lock{mutex_};
            for(;;)
            {
                if(!queued_)
                {
                    if(stopping_)break;
                    auto signal=sequence_.load(std::memory_order_relaxed);
                    lock.unlock();sequence_.wait(signal,std::memory_order_acquire);lock.lock();continue;
                }
                // The head remains occupied while unlocked. Producers only
                // write an unoccupied tail and cannot overwrite this span.
                std::span<std::uint8_t> bytes{storage_.get()+head_*stride_,lengths_[head_]};
                lock.unlock();
                bool wrote=healthy && write_file(bytes);
                ::fast_io::secure_clear(bytes.data(),bytes.size());
                lock.lock();
                if(wrote)add(stats_.written);
                else if(healthy){healthy=false;add(stats_.io_errors);running_.store(false,std::memory_order_release);}
                head_=(head_+1)%slots;--queued_;
            }
            if(healthy)
            {
                std::array<std::uint8_t,footer_bytes> footer{};
                put(footer,0,footer.size(),4);
                std::uint64_t fields[]{stats_.enqueued,stats_.written,stats_.oversize,stats_.budget,stats_.full,stats_.io_errors};
                for(std::size_t i{};i<std::size(fields);++i)put(footer,8+8*i,fields[i],8);
                lock.unlock();bool wrote=write_file(footer);lock.lock();
                if(!wrote)add(stats_.io_errors);
            }
        }
#endif
    public:
        journal()=default;
        journal(journal const&)=delete;
        journal& operator=(journal const&)=delete;
        /** @brief Indicates whether the private writer can accept another observation. */
        bool enabled()const noexcept{return running_.load(std::memory_order_acquire);}
        static constexpr bool valid_configuration(int fd,limits cap) noexcept
        {
            return fd>=3 && fd<=1048575 && cap.records && cap.records<=1048576 && cap.payload && cap.payload<=65536 &&
                cap.bytes>=magic.size()+footer_bytes+fixed_bytes+2 && cap.bytes<=67108864;
        }
        /** @brief Consume an empty private FD after successful initialization.
         * @details The deployment configures this once before starting guests.
         * The fixed writer stack is bounded to 256 KiB and joins on shutdown.
         */
        bool configure(int fd,limits cap) noexcept
        {
#if defined(__linux__)
            if(!valid_configuration(fd,cap))return false;
            auto owner=pid_.load(std::memory_order_acquire);
            if(owner && owner!=::getpid())return false;
            std::lock_guard lock{mutex_};
            if(used_)return false;
            int copy=::fcntl(fd,F_DUPFD_CLOEXEC,3);if(copy<0)return false;
            // Serialize the empty-file check and header against another VM,
            // including two processes that inherited the same open file
            // description. Closing the supplied FD later releases this POSIX
            // lock; the nonempty header then excludes subsequent claimants.
            struct flock claim{};
            claim.l_type=F_WRLCK;claim.l_whence=SEEK_SET;claim.l_len=1;
            struct stat info{};
            if(!audit_file::validate(copy,0,info) || ::fcntl(copy,F_SETLK,&claim)<0 || !audit_file::validate(copy,0,info))
            {::close(copy);return false;}
            stride_=fixed_bytes+2*255+static_cast<std::size_t>(cap.payload);
            storage_.reset(::uwvm2::utils::allocator::fast_io_strict::try_array<std::uint8_t>(slots*stride_,false));
            if(!storage_){::close(copy);return false;}
            fd_=copy;device_=info.st_dev;inode_=info.st_ino;pid_.store(::getpid(),std::memory_order_relaxed);
            limits_=cap;charged_=magic.size()+footer_bytes;
            pthread_attr_t attrs;
            int err=::pthread_attr_init(&attrs);
            if(!err)
            {
                err=::pthread_attr_setstacksize(&attrs,262144);
                if(!err)err=::pthread_create(&thread_,&attrs,[](void* p)noexcept->void*{
                    static_cast<journal*>(p)->consume();return nullptr;},this);
                ::pthread_attr_destroy(&attrs);
            }
            if(err){::close(fd_);fd_=-1;clear_storage();return false;}
            // The writer publishes header status without taking mutex_.
            // A failed header leaves the operator's descriptor intact.
            used_=true;startup_.wait(0,std::memory_order_acquire);
            if(startup_.load(std::memory_order_acquire)!=1)
            {
                ::pthread_join(thread_,nullptr);add(stats_.io_errors);
                ::close(fd_);fd_=-1;clear_storage();return false;
            }
            joinable_=true;::close(fd);running_.store(true,std::memory_order_release);return true;
#else
            (void)fd;(void)cap;return false;
#endif
        }
        /** @brief Copy only an already authorized body with an exact route and stage.
         * @details Packet header, grant token, guest addresses, unused capacity,
         * and any denied message are outside this interface. File charging is
         * atomic with queue insertion. The writer never holds this mutex during I/O.
         */
        void capture(std::string_view env,std::string_view route,stage point,std::uint64_t generation,
                     std::span<std::uint8_t const> body) noexcept
        {
#if defined(__linux__)
            if(!enabled() || ::getpid()!=pid_.load(std::memory_order_relaxed))return;
            std::lock_guard lock{mutex_};
            if(!enabled() || !identifier(env) || !identifier(route) || !generation ||
                static_cast<unsigned>(point)<1 || static_cast<unsigned>(point)>4)return;
            if(body.size()>limits_.payload){add(stats_.oversize);return;}
            auto size=fixed_bytes+env.size()+route.size()+body.size();
            if(stats_.enqueued==limits_.records || size>limits_.bytes-charged_){add(stats_.budget);return;}
            if(queued_==slots){add(stats_.full);return;}
            auto tail=(head_+queued_)%slots;
            std::span<std::uint8_t> bytes{storage_.get()+tail*stride_,size};
            put(bytes,0,size,4);bytes[4]=static_cast<std::uint8_t>(point);
            bytes[5]=static_cast<std::uint8_t>(env.size());bytes[6]=static_cast<std::uint8_t>(route.size());bytes[7]=0;
            put(bytes,8,stats_.enqueued+1,8);put(bytes,16,generation,8);
            put(bytes,24,body.size(),4);
            std::memcpy(bytes.data()+fixed_bytes,env.data(),env.size());
            std::memcpy(bytes.data()+fixed_bytes+env.size(),route.data(),route.size());
            if(!body.empty())std::memcpy(bytes.data()+fixed_bytes+env.size()+route.size(),body.data(),body.size());
            lengths_[tail]=size;charged_+=size;++queued_;++stats_.enqueued;notify();
#else
            (void)env;(void)route;(void)point;(void)generation;(void)body;
#endif
        }
        /** @brief Obtain host-only delivery and loss counts without flushing the sink. */
        stats inspect()const noexcept
        {
#if defined(__linux__)
            if(pid_.load(std::memory_order_acquire)!=::getpid())return {};
#endif
            std::lock_guard lock{mutex_};return stats_;
        }
        /** @brief Stop accepting entries, drain and overwrite the queue, and close the file.
         * @details Called after guest threads have stopped. A forced process
         * exit can lose buffered records. This journal is a diagnostic channel.
         */
        void finish() noexcept
        {
#if defined(__linux__)
            auto pid=pid_.load(std::memory_order_acquire);
            if(!pid)return;
            if(pid!=::getpid())
            {
                // Only the forking thread survived. Never touch the parent's
                // inherited locks, join handle or output in this child.
                if(fd_>=0){::close(fd_);fd_=-1;}
                return;
            }
            {
                std::lock_guard lock{mutex_};if(!joinable_)return;
                running_.store(false,std::memory_order_release);stopping_=true;joinable_=false;notify();
            }
            ::pthread_join(thread_,nullptr);::close(fd_);fd_=-1;clear_storage();
#endif
        }
        ~journal(){finish();}
    };
    inline journal output{};
}
}
