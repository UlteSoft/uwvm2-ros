/** @file Finite caller-driven industrial Action lifecycle, independent of ROS ABI.
 * This value journal creates no thread, clock, resource, grant, retry or emergency
 * stop. Actual generated complete Goal/Feedback/Result codecs and SDK Action
 * owners perform wire/ID/permission checks. A source API entry is deliberately
 * recorded before the call: an error cannot prove zero native effect. A fresh
 * journal is a fresh local transaction, never authority to resend an old goal.
 */
#pragma once
#include <cstdint>
#include <array>
#include <roswasm/roswasmcpp.hpp>

namespace uwvm_industrial {
enum class Phase : unsigned char { fresh, submitted_unknown, active,
    cancel_unknown, cancel_acknowledged, cancel_rejected, terminal, retired };
struct Outcome {
    std::uint32_t primary{}, cleanup{};
    std::uint64_t goal{};
    std::uint8_t terminal_kind{};
    bool source_api_entered{}, native_outcome_unknown{}, cancel_acknowledged{};
    std::uint32_t cleanup_attempted{};
    std::array<std::uint32_t,5> role_cleanup{};
};

/** Stable full-width identity and once-only local effect guards. Not a lease.
 * No mutator accepts a peer-supplied replacement goal ID or invents terminal
 * success from a cancel ACK. Terminal kinds are actual SDK2/3/4. FJT success
 * additionally requires the complete actual result error_code to equal0.
 */
class Journal final {
    Phase phase_{Phase::fresh}; Outcome value_{};
public:
    Phase phase() const noexcept {return phase_;}
    Outcome const& outcome() const noexcept {return value_;}
    std::uint32_t before_send() noexcept {
        if(phase_!=Phase::fresh)return 17;
        phase_=Phase::submitted_unknown;value_.source_api_entered=true;
        value_.native_outcome_unknown=true;return 0;
    }
    std::uint32_t accepted(std::uint64_t id) noexcept {
        if(phase_!=Phase::submitted_unknown||!id)return 22;
        value_.goal=id;phase_=Phase::active;return 0;
    }
    std::uint32_t before_cancel(std::uint64_t id) noexcept {
        if(phase_!=Phase::active||!id||id!=value_.goal)return 9;
        phase_=Phase::cancel_unknown;return 0;
    }
    std::uint32_t cancelled(std::uint64_t id,bool accepted) noexcept {
        if(phase_!=Phase::cancel_unknown||id!=value_.goal)return 22;
        value_.cancel_acknowledged=accepted;
        phase_=accepted?Phase::cancel_acknowledged:Phase::cancel_rejected;return 0;
    }
    std::uint32_t feedback(std::uint64_t id) const noexcept {
        return id&&id==value_.goal&&(phase_==Phase::active||phase_==Phase::cancel_unknown||
            phase_==Phase::cancel_acknowledged||phase_==Phase::cancel_rejected)?0u:9u;
    }
    std::uint32_t terminal(std::uint64_t id,std::uint8_t kind,std::int32_t controller_error) noexcept {
        if(feedback(id)||kind<2||kind>4)return 22;
        value_.terminal_kind=kind;value_.native_outcome_unknown=false;
        phase_=Phase::terminal;
        // Preserve the vendor/controller result separately in its actual typed
        // object. Local125 is cancellation,5 is a real non-success terminal.
        if(!value_.primary)value_.primary=kind==2&&controller_error==0?0u:kind==4?125u:5u;
        return 0;
    }
    void failure(std::uint32_t primary) noexcept {if(primary&&!value_.primary)value_.primary=primary;}
    void timeout() noexcept {failure(110); /* Unknown until a genuine terminal result. */}
    std::uint32_t before_retire() noexcept {
        if(phase_==Phase::retired)return 9;
        phase_=Phase::retired;return 0;
    }
    void retired(std::array<std::uint32_t,5> const& statuses,std::uint32_t first,std::uint32_t attempted=5) noexcept {
        value_.role_cleanup=statuses;value_.cleanup=first;value_.cleanup_attempted=attempted;
    }
};

/** Nonmoving typed Action owner supplied by the actual generated family.
 * The caller's backend/context and all four aliases remain alive through this
 * object. Aliases never imply rights. Constructor/factory rollback remains the
 * generated SDK owner's checked implementation. All five cleanup results are
 * preserved; ambiguous submit/cancel never triggers an automatic resend.
 */
template<class Client> class Trajectory final {
    Journal journal_; Client client_;
public:
    using Goal=typename Client::Goal;using Feedback=typename Client::Feedback;using Result=typename Client::Result;
    Trajectory(roswasmc_context const& context,roswasmc_action_aliases const& aliases,std::size_t capacity=1)
        :journal_{},client_{context,aliases,capacity}{journal_.failure(client_.error());
            if constexpr(requires{client_.constructor_report();client_.constructor_results();}){
                auto r=client_.constructor_report();journal_.retired(client_.constructor_results(),r.cleanup_error,r.attempted);}}
    Trajectory(Trajectory const&)=delete;Trajectory& operator=(Trajectory const&)=delete;
    Trajectory(Trajectory&&)=delete;
    Journal const& journal() const noexcept {return journal_;}
    std::uint32_t error() const noexcept {return client_.error();}
    std::uint32_t submit(Goal const& goal) {
        if(auto e=client_.error())return e;
        if(auto e=journal_.before_send())return e;
        std::uint64_t id{};auto r=client_.send_goal(goal,id);
        if(r.error){journal_.failure(r.error);return r.error;}
        auto e=journal_.accepted(id);if(e)journal_.failure(e);return e;
    }
    std::uint32_t feedback(Feedback& out) {
        auto id=journal_.outcome().goal;
        if(auto e=journal_.feedback(id))return e;
        auto r=client_.feedback(id,out);if(r.error!=11)journal_.failure(r.error);return r.error;
    }
    std::uint32_t result(Result& out,std::uint8_t& kind) {
        auto id=journal_.outcome().goal;
        if(auto e=journal_.feedback(id))return e;
        // Both destinations remain private until the actual whole SDK decoder
        // and returned ID/kind validation succeed. Original error_string and
        // every other Result field remain available to the application.
        Result next{};std::uint8_t next_kind{};auto r=client_.result(id,next_kind,next);
        if(r.error){if(r.error!=11)journal_.failure(r.error);return r.error;}
        if(auto e=journal_.terminal(id,next_kind,next.error_code)){journal_.failure(e);return e;}
        out=next;kind=next_kind;return 0;
    }
    std::uint32_t cancel_own(bool& acknowledged) {
        auto id=journal_.outcome().goal;
        if(auto e=journal_.before_cancel(id))return e;
        int next{};auto r=client_.cancel_own(id,next);
        if(r.error){journal_.failure(r.error);return r.error;}
        if(next!=0&&next!=1){journal_.failure(22);return 22;}
        if(auto e=journal_.cancelled(id,next!=0)){journal_.failure(e);return e;}
        acknowledged=next!=0;return 0;
    }
    void timeout() noexcept {journal_.timeout();}
    Outcome close() {
        if(journal_.before_retire())return journal_.outcome();
        std::array<std::uint32_t,5> statuses{};
        if(!client_.is_open())return journal_.outcome();
        auto r=client_.close_checked(journal_.outcome().primary,statuses);
        journal_.retired(statuses,r.cleanup_error,r.attempted);return journal_.outcome();
    }
};
} // namespace uwvm_industrial
