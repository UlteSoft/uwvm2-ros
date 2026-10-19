/** @file
 * Trusted Native worker cancellation context; no Guest API or numeric import.
 *
 * The actual controller borrows a Linux eventfd from its OWN ChildLease. It is
 * EFD_NONBLOCK|EFD_CLOEXEC, level-triggered: controller first publishes its stop
 * payload/atomics, then writes the fd. Neither this scope nor any Host I/O path
 * reads, drains, duplicates or closes the descriptor. The owner MUST retain it
 * until every physical worker/call has really returned and joined. Integer
 * equality is not fd ownership or lifetime proof; only the trusted bridge's
 * actual lease supplies that contract. No synthetic friend/Guest setter exists.
 *
 * Ordinary callers have no scope (-1). Nested scopes inherit on -1 or retain
 * the same live fd. Conflicting/invalid scopes publish a rejection sentinel;
 * both valid() and all IPC admissions fail closed. A nested scope cannot hide a
 * stopped/rejected parent. Destruction restores the lexical prior TLS state.
 */
#pragma once
#include <cstdint>
#if defined(__linux__)
#include <poll.h>
#include <errno.h>
#endif
namespace uwvm2::runtime::lib {extern "C++" {class runtime_threaded_profile_bridge;}}
namespace uwvm2::imported::ros::ipc
{
    inline int current_cancellation_fd()noexcept;
    class cancellation_scope
    {
        struct state {int fd;bool rejected;};
        inline static thread_local state current_{-1,false};
        state previous_;bool valid_{};
        friend class ::uwvm2::runtime::lib::runtime_threaded_profile_bridge;
        friend int current_cancellation_fd()noexcept;
        // Only the actual source-admitted Runtime bridge can create a scope.
        // There is no public Native adapter, test backdoor or Guest setter.
        explicit cancellation_scope(int borrowed_fd) noexcept:previous_{current_}
        {
            auto next=previous_;
            if(previous_.rejected||borrowed_fd< -1||
                (previous_.fd>=0&&borrowed_fd>=0&&previous_.fd!=borrowed_fd)){next.rejected=true;}
            else if(borrowed_fd>=0){next.fd=borrowed_fd;}
            current_=next;valid_=!next.rejected;
        }
    public:
        cancellation_scope(cancellation_scope const&)=delete;
        cancellation_scope& operator=(cancellation_scope const&)=delete;
        cancellation_scope(cancellation_scope&&)=delete;
        cancellation_scope& operator=(cancellation_scope&&)=delete;
        ~cancellation_scope(){current_=previous_;}
        bool valid()const noexcept{return valid_;}
    };
    inline int current_cancellation_fd()noexcept
    {return cancellation_scope::current_.rejected?-2:cancellation_scope::current_.fd;}
    enum class cancellation_probe:std::uint8_t {clear,ready,rejected,probe_failed};
    /** Zero-wait metadata observation only. POLLNVAL/invalid descriptor/probe
     * interruption are fail-closed admission failures, NOT invented evidence
     * that the controller published a stop payload. No unbounded EINTR loop.
     */
    inline cancellation_probe probe_cancellation(int borrowed_fd)noexcept
    {
        if(borrowed_fd==-1)return cancellation_probe::clear;
        if(borrowed_fd< -1)return cancellation_probe::rejected;
#if defined(__linux__)
        pollfd watched{borrowed_fd,POLLIN,0};auto result=::poll(&watched,1,0);
        if(result<0)return cancellation_probe::probe_failed;
        if(watched.revents&POLLNVAL)return cancellation_probe::rejected;
        if(watched.revents)return cancellation_probe::ready;
        return cancellation_probe::clear;
#else
        return cancellation_probe::rejected;
#endif
    }
    inline bool cancellation_requested(int borrowed_fd)noexcept
    {
        if(probe_cancellation(borrowed_fd)==cancellation_probe::clear)return false;
#if defined(__linux__)
        errno=ECANCELED;
#endif
        return true;
    }
    enum class cancellation_phase:std::uint8_t {none,connect_admission,connect_wait,hello,call_admission,request_transfer,response_transfer};
    /** Cumulative Native diagnostics only. possible_peer_effects is sticky; first
     * and last nonzero old sessions plus a saturating count retain bounded
     * observation history without allocation. It is conservative: a false
     * write_frame may already have sent a partial/whole request. No failure or
     * descriptor close changes that uncertainty into successful rollback. It
     * does not assert a native ROS effect or completion. Old session is retained
     * as private association, NEVER exported as a Guest/debug bearer token.
     */
    struct cancellation_observation
    {
        bool observed{},possible_peer_effects{};
        std::uint64_t first_session{},old_session{},observations{};bool observations_saturated{};cancellation_phase phase{cancellation_phase::none};
        cancellation_probe cause{cancellation_probe::clear};
    };
}
