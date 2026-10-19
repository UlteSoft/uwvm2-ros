/** @file
 * Pure service Host transaction values. No native provider, Runtime caller,
 * permission or RPC is implemented by this header. Actual owner serialization
 * and complete two-party leases remain mandatory. Source-recompile seam only.
 */
#pragma once
#ifndef UWVM_MODULE
#include <uwvm2/imported/ros/common/rmw_service_bindings.h>
#endif
#ifndef UWVM_MODULE
#include <uwvm2/imported/ros/ipc/rmw_service.h>
#endif
#include <array>
#include <algorithm>
#include <cstdint>
#include <span>
#include <chrono>
#include <new>
#include <utility>

namespace uwvm2::uwvm::imported::ros::host
{
    struct rmw_service_journal
    {
        // api_* records the Host function result, including local preflight and
        // Guest-copy errors. rpc_* is a separate IPC client-call API observation,
        // not proof of a native effect, valid native reply or ROS completion.
        std::uint32_t first_primary{},last_primary{},api_error{},rpc_error{},cancel{},commit{},cleanup{};
        bool api_returned{},api_rpc_attempted{},rpc_attempted{},rpc_returned{},cancel_attempted{},commit_attempted{},cleanup_attempted{},receipt_observed{};
        std::uint64_t pending_sequence{},pending_reservation{},last_sequence{},last_reservation{},last_committed{};
        void primary(std::uint32_t value) noexcept
        {if(value&&value!=11){if(!first_primary){first_primary=value;}last_primary=value;}}
        void begin_api()noexcept {api_returned=false;api_rpc_attempted=false;api_error=115;}
        void api_result(std::uint32_t value) noexcept {api_returned=true;api_error=value;if(value!=115){primary(value);}}
        void begin_rpc()noexcept {api_rpc_attempted=true;rpc_attempted=true;rpc_returned=false;rpc_error=115;}
        void end_rpc(std::uint32_t value)noexcept {rpc_returned=true;rpc_error=value;}
        void cancelled(std::uint32_t value) noexcept {cancel_attempted=true;cancel=value;}
        void committed(std::uint32_t value) noexcept {commit_attempted=true;commit=value;}
        void cleaned(std::uint32_t value) noexcept {cleanup_attempted=true;cleanup=value;}
        void clear_borrow() noexcept {pending_sequence=pending_reservation=0;}
    };
    namespace rmw_service_values
    {
        namespace ipc=::uwvm2::imported::ros::ipc;
        // Actual service schema keys, not publisher max-message-bytes.
        inline std::string_view send_limit_key(bool respond)noexcept
        {return respond?"max-response-bytes":"max-request-bytes";}
        using deadline=std::chrono::steady_clock::time_point;
        /** Actual parsed finite millisecond budgets share ONE Host entry time.
         * Invalid input preserves output. No phase/new-now reset or step time.
         */
        inline std::uint32_t clamp_deadline(deadline entered,std::uint64_t millis,deadline& output)noexcept
        {
            if(!millis||millis>ipc::max_timeout_ms)return 22;
            output=std::min(output,entered+std::chrono::milliseconds(millis));return 0;
        }
        inline std::uint32_t entry_deadline(deadline entered,std::uint64_t connection_millis,
            std::uint64_t source_millis,deadline& output)noexcept
        {
            if(!connection_millis||connection_millis>ipc::max_timeout_ms||!source_millis||source_millis>ipc::max_timeout_ms)return 22;
            auto next=entered+std::chrono::milliseconds(connection_millis);
            (void)clamp_deadline(entered,source_millis,next);output=next;return 0;
        }
        /** ACTUAL post-association exception boundary. It routes allocation or
         * other exceptions to original exact-borrow compensation, never to a
         * fake Native provider. The callback is nonthrowing and preserves all
         * independent outcomes. Work has no fallible step after Guest copy.
         */
        template<typename Work,typename Failure>
        inline std::uint32_t guarded_borrow(Work&& work,Failure&& failure)noexcept
        {
            static_assert(noexcept(failure(std::uint32_t{})),"Borrow compensation must not throw");
#if defined(__cpp_exceptions)
            try{return std::forward<Work>(work)();}
            catch(std::bad_alloc const&){return failure(12);}
            catch(...){return failure(5);}
#else
            return std::forward<Work>(work)();
#endif
        }

        /** Operation-specific peek idle only: the real Registry guarantees no
         * visible queued record/reservation/ACK. A poll may have disposed of an
         * ALREADY abandoned late response, so this is not a zero-native-take
         * claim. No global numeric-error no-effect inference is permitted.
         * Submit/respond/available/ACK failures keep session retirement armed.
         */
        inline bool peek_idle(std::uint32_t error,bool empty) noexcept
        {return empty&&error==11;}
        /** Borrow eight private RESERVED bytes immediately preceding canonical
         * body as the genuine legacy service ticket prefix. A caller verifies
         * the complete original packet first. The view owns no second body;
         * destruction restores every original byte before Guest copy/RPC.
         * This object never points at Guest memory or a borrowed native struct.
         */
        class ticket_prefix
        {
            std::uint8_t* bytes_;std::array<std::uint8_t,8> saved_{};
        public:
            ticket_prefix(std::uint8_t* private_eight,std::uint64_t ticket) noexcept:bytes_{private_eight}
            {std::copy(bytes_,bytes_+8,saved_.begin());for(unsigned i{};i!=8;++i){bytes_[i]=static_cast<std::uint8_t>(ticket>>(8*i));}}
            ticket_prefix(ticket_prefix const&)=delete;ticket_prefix& operator=(ticket_prefix const&)=delete;
            ~ticket_prefix(){std::copy(saved_.begin(),saved_.end(),bytes_);}
        };
        /** Logical existing UWRE header for actual atomic observer+source
         * admission, not a second delivered record or Native timestamp codec.
         * The caller already validated SAME UMRC/URMI/source/incarnation/body.
         * Existing authorize_event_delivery validates actual mask,64+body limit,
         * Native response codec and independent source/observer byte credit.
         */
        inline bool event(ipc::rmw_metadata::header const& native,ipc::event_header& output) noexcept
        {
            if(!ipc::rmw_metadata::valid(native)||(native.kind!=ipc::event_kind::service_response&&!ipc::event_control(native.kind))||
                (!ipc::event_control(native.kind)&&!ipc::rmw_metadata::complete(native))){return false;}
            ipc::event_header next{};next.backend=native.owner.backend;next.kind=native.kind;
            next.flags=ipc::event_control(native.kind)?native.control_flags:0;next.source_handle=native.owner.source;
            next.instance=native.owner.instance;next.session=native.owner.session;next.sequence=native.sequence;
            next.correlation=native.correlation;next.payload_bytes=native.payload_bytes;next.error=native.error;
            if(!ipc::valid_event_header(next)){return false;}output=next;return true;
        }
    }
}
