/** @file
 * @brief Owned, direction-bound nonblocking streams for the closed device protocol.
 * @details Trusted deployment supplies already-open PTY ends or receive-only pipes;
 * no guest request names a path, descriptor, socket or direction. The helper
 * duplicates a bounded descriptor only after checking its kind/access/nonblocking
 * mode. It never changes the original file description flags. Regular files,
 * sockets and unrelated character devices are refused before any byte I/O.
 *
 * One fixed frame is retained per sender/receiver. A sender does at most one
 * bounded write per pump under its private lock. Its lease must be held by the
 * provider across each pump. Blocking waits occur outside this object. Command
 * and stop objects need distinct configured native channels and independent
 * worker/queue reservations. This class creates no thread or watchdog and does
 * not by itself prove that two deployment channels are independent.
 */
#pragma once
#include "device_protocol.h"
#if defined(__linux__)
#include <cerrno>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <termios.h>
#endif
namespace uwvm2::imported::ros::scene::devices::channel
{
    enum class direction:std::uint8_t{send,receive};
    struct identity{std::uint64_t device{},inode{},special{},terminal{};bool multiplexer{};friend bool operator==(identity const&,identity const&)=default;};
    /** @brief One owned duplicate; descriptors are never exposed by this class. */
    class endpoint
    {
        int fd_{-1};identity identity_;direction direction_{};
#if defined(__linux__)
        ::pid_t process_{::getpid()};
#endif
    public:
        endpoint()=default;endpoint(endpoint const&)=delete;endpoint&operator=(endpoint const&)=delete;
        ~endpoint(){close();}
        bool same_process()const noexcept
        {
#if defined(__linux__)
            return process_==::getpid();
#else
            return true;
#endif
        }
        error bind(int selected,direction flow)noexcept
        {
            if(!same_process())return error::denied;
            if(fd_>=0)return error::conflict;
#if defined(__linux__)
            if(selected<0||static_cast<unsigned>(flow)>1)return error::invalid;
            struct ::stat info{};if(::fstat(selected,&info))return error::bad_handle;
            int flags=::fcntl(selected,F_GETFL);if(flags<0)return error::bad_handle;
            if(!(flags&O_NONBLOCK))return error::denied;
            if(S_ISFIFO(info.st_mode))
            {
                // A generic pipe write may deliver process-fatal SIGPIPE. This
                // profile supports pipe ingress only; PTY writes return errno.
                if(flow!=direction::receive||(flags&O_ACCMODE)!=O_RDONLY)return error::denied;
            }
            else if(S_ISCHR(info.st_mode))
            {::termios terminal{};if(::tcgetattr(selected,&terminal)||(flags&O_ACCMODE)!=O_RDWR)return error::denied;}
            else return error::denied;
            unsigned terminal{};bool multiplexer=S_ISCHR(info.st_mode)&&::ioctl(selected,TIOCGPTN,&terminal)==0;
            // Canonicalize a PTY master to its actual slave inode. Numeric PTY
            // indices alone collide across devpts namespaces. Opening the peer
            // through this ioctl neither follows a caller path nor writes data.
            if(multiplexer)
            {
#if defined(TIOCGPTPEER)
                int peer=::ioctl(selected,TIOCGPTPEER,O_RDWR|O_NOCTTY|O_NONBLOCK|O_CLOEXEC);
                if(peer<0)return error::denied;
                struct ::stat slave{};auto checked=::fstat(peer,&slave);::close(peer);
                if(checked||!S_ISCHR(slave.st_mode))return error::denied;
                info=slave;terminal=0;multiplexer=false;
#else
                return error::unsupported;
#endif
            }
            auto duplicate=::fcntl(selected,F_DUPFD_CLOEXEC,0);if(duplicate<0)return errno==EMFILE||errno==ENFILE?error::again:error::io;
            fd_=duplicate;direction_=flow;identity_={static_cast<std::uint64_t>(info.st_dev),static_cast<std::uint64_t>(info.st_ino),static_cast<std::uint64_t>(info.st_rdev),terminal,multiplexer};return error::success;
#else
            static_cast<void>(selected);static_cast<void>(flow);return error::unsupported;
#endif
        }
        void close()noexcept
        {
#if defined(__linux__)
            if(fd_>=0){auto old=std::exchange(fd_,-1);::close(old);}
#endif
        }
        explicit operator bool()const noexcept{return fd_>=0&&same_process();}
        identity source()const noexcept{return identity_;}
        /** @brief Internal syscall only; public device imports never expose this endpoint. */
        error write(bytes input,std::size_t& amount)noexcept
        {
            amount=0;if(!*this||direction_!=direction::send)return error::denied;
#if defined(__linux__)
            auto flags=::fcntl(fd_,F_GETFL);if(flags<0)return error::io;if(!(flags&O_NONBLOCK))return error::denied;
            auto size=::write(fd_,input.data(),input.size());if(size<0)return errno==EAGAIN||errno==EWOULDBLOCK||errno==EINTR?error::again:error::io;
            if(!size)return error::io;
            amount=static_cast<std::size_t>(size);return error::success;
#else
            static_cast<void>(input);return error::unsupported;
#endif
        }
        error read(std::span<std::uint8_t> output,std::size_t& amount)noexcept
        {
            amount=0;if(!*this||direction_!=direction::receive)return error::denied;
#if defined(__linux__)
            auto flags=::fcntl(fd_,F_GETFL);if(flags<0)return error::io;if(!(flags&O_NONBLOCK))return error::denied;
            auto size=::read(fd_,output.data(),output.size());if(size<0)return errno==EAGAIN||errno==EWOULDBLOCK||errno==EINTR?error::again:error::io;
            if(!size)return error::io;
            amount=static_cast<std::size_t>(size);return error::success;
#else
            static_cast<void>(output);return error::unsupported;
#endif
        }
    };
    enum class transmission:std::uint8_t{empty,queued,partial,awaiting_ack,failed,unknown};
    struct send_status{transmission state{};std::size_t written{},bytes{};handle request{},provider{};};
    class sender
    {
        mutable std::mutex mutex_;endpoint endpoint_;protocol::kind kind_;protocol::packet retained_;protocol::header header_;
        std::size_t offset_{},maximum_write_{};transmission state_{};bool retired_{};
    public:
        explicit sender(protocol::kind kind):kind_(kind){}
        /** @brief Trusted activation; at most one descriptor and finite write extent. */
        error bind(int fd,std::size_t maximum_write)noexcept
        {
            if(!endpoint_.same_process())return error::denied;
            std::lock_guard lock{mutex_};if(retired_||(kind_!=protocol::kind::command&&kind_!=protocol::kind::stop&&kind_!=protocol::kind::controller)||!maximum_write||maximum_write>4096)return error::invalid;
            auto result=endpoint_.bind(fd,direction::send);if(result==error::success)maximum_write_=maximum_write;return result;
        }
        /** @brief Retain one complete validated frame before any physical byte write. */
        error stage(protocol::packet const& packet)noexcept
        {
            if(!endpoint_.same_process())return error::denied;
            std::lock_guard lock{mutex_};if(!endpoint_||retired_)return error::denied;if(state_!=transmission::empty)return error::again;
            if(packet.size>packet.data.size())return error::invalid;
            protocol::header header;bytes body;
            auto decoded=protocol::decode(packet.view(),kind_,header,body);if(decoded!=error::success)return decoded;
            std::copy(packet.view().begin(),packet.view().end(),retained_.data.begin());retained_.size=packet.size;header_=header;offset_=0;state_=transmission::queued;return error::success;
        }
        /** @brief One bounded nonblocking write under the caller's current authority lease.
         * @details Zero bytes plus refusal is proven local no-write. Any partial
         * transmission followed by expiry/error/retire is unknown and remains
         * occupied. A complete write only enters awaiting_ack. No retry/reset of
         * unknown exists; a new trusted session requires reconciliation outside.
         */
        error pump(std::uint64_t now)noexcept
        {
            if(!endpoint_.same_process())return error::denied;
            std::lock_guard lock{mutex_};if(!endpoint_||retired_)return error::denied;
            if(state_!=transmission::queued&&state_!=transmission::partial)return error::again;
            if((kind_==protocol::kind::command||kind_==protocol::kind::controller)&&(!now||now>=header_.deadline||now>=header_.lease_expires))
            {state_=offset_?transmission::unknown:transmission::failed;return error::timeout;}
            std::size_t written{};auto status=endpoint_.write(retained_.view().subspan(offset_,std::min(maximum_write_,retained_.size-offset_)),written);
            if(status==error::again)return status;
            if(status!=error::success){state_=offset_?transmission::unknown:transmission::failed;return status;}
            offset_+=written;state_=offset_==retained_.size?transmission::awaiting_ack:transmission::partial;return error::success;
        }
        /** @brief Release only a digest-bound remote receipt or proven zero-byte failure. */
        error acknowledge(bytes ack,handle sequence,protocol::acknowledgement& outcome)noexcept
        {
            if(!endpoint_.same_process())return error::denied;
            std::lock_guard lock{mutex_};if(state_!=transmission::awaiting_ack)return error::conflict;
            auto result=protocol::verify_ack(ack,header_,sequence,outcome);if(result!=error::success){state_=transmission::unknown;return result;}
            state_=transmission::empty;retained_.size=offset_=0;header_={};return error::success;
        }
        error discard_unwritten()noexcept
        {
            if(!endpoint_.same_process())return error::denied;
            std::lock_guard lock{mutex_};if(offset_||state_==transmission::awaiting_ack||state_==transmission::unknown)return error::denied;
            state_=transmission::empty;retained_.size=0;header_={};return error::success;
        }
        void retire()noexcept
        {
            if(!endpoint_.same_process())return;
            std::lock_guard lock{mutex_};retired_=true;if(state_!=transmission::empty)state_=offset_?transmission::unknown:transmission::failed;endpoint_.close();
        }
        error inspect(send_status& value)const noexcept
        {if(!endpoint_.same_process())return error::denied;std::lock_guard lock{mutex_};value={state_,offset_,retained_.size,header_.request,header_.provider};return error::success;}
        identity source()const noexcept{if(!endpoint_.same_process())return {};std::lock_guard lock{mutex_};return endpoint_.source();}
    };
    class acknowledgements
    {
        mutable std::mutex mutex_;endpoint endpoint_;protocol::stream_decoder decoder_{protocol::kind::acknowledgement};bool retired_{},poisoned_{};
    public:
        error bind(int fd)noexcept
        {if(!endpoint_.same_process())return error::denied;std::lock_guard lock{mutex_};if(retired_)return error::denied;return endpoint_.bind(fd,direction::receive);}
        /** @brief Read at most 512 bytes; retain one complete ACK until explicit consume. */
        error pump()noexcept
        {
            if(!endpoint_.same_process())return error::denied;
            std::lock_guard lock{mutex_};if(retired_||!endpoint_)return error::denied;if(poisoned_)return error::invalid;
            if(!decoder_.frame().empty())return error::again;
            std::array<std::uint8_t,512> bytes;std::size_t size{};auto status=endpoint_.read({bytes.data(),std::min(bytes.size(),decoder_.remaining())},size);
            if(status==error::again)return status;
            if(status!=error::success){poisoned_=true;return status;}
            std::size_t consumed{};status=decoder_.feed({bytes.data(),size},consumed);if(status==error::invalid)poisoned_=true;return status;
        }
        error copy(protocol::packet& output)const noexcept
        {
            if(!endpoint_.same_process())return error::denied;
            std::lock_guard lock{mutex_};if(retired_||poisoned_)return error::denied;auto frame=decoder_.frame();if(frame.empty())return error::again;
            std::copy(frame.begin(),frame.end(),output.data.begin());output.size=frame.size();return error::success;
        }
        error consume()noexcept
        {if(!endpoint_.same_process())return error::denied;std::lock_guard lock{mutex_};if(retired_||poisoned_)return error::denied;return decoder_.next();}
        void retire()noexcept{if(!endpoint_.same_process())return;std::lock_guard lock{mutex_};retired_=true;endpoint_.close();}
        identity source()const noexcept{if(!endpoint_.same_process())return {};std::lock_guard lock{mutex_};return endpoint_.source();}
    };
}
