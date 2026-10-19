/** Source-only local lifecycle contracts. No ROS, robot or Native delivery proof. */
#ifdef NDEBUG
#error "Industrial lifecycle assertions must remain enabled in every actual build"
#endif
#include <cassert>
#include <limits>
#include "sdk/robots/industrial/include/trajectory_lifecycle.hpp"
using uwvm_industrial::Journal;using uwvm_industrial::Phase;
int main(){
    Journal a;assert(a.before_send()==0);assert(a.before_send()==17);
    assert(a.accepted(0)==22);auto const high=std::numeric_limits<std::uint64_t>::max();
    assert(a.accepted(high)==0);assert(a.feedback(1)==9);assert(a.feedback(high)==0);
    assert(a.before_cancel(1)==9);assert(a.before_cancel(high)==0);assert(a.before_cancel(high)==9);
    assert(a.cancelled(high,true)==0);assert(a.phase()==Phase::cancel_acknowledged);
    assert(a.outcome().native_outcome_unknown);assert(a.feedback(high)==0);
    assert(a.terminal(high,1,0)==22);assert(a.terminal(1,4,0)==22);
    assert(a.terminal(high,4,0)==0);assert(!a.outcome().native_outcome_unknown);
    assert(a.outcome().primary==125);assert(a.feedback(high)==9);assert(a.terminal(high,2,0)==22);
    assert(a.before_retire()==0);assert(a.before_retire()==9);
    std::array<std::uint32_t,5> errors{0,107,0,5,0};a.retired(errors,107);
    assert(a.outcome().primary==125&&a.outcome().cleanup==107&&a.outcome().role_cleanup[3]==5);
    Journal b;assert(!b.before_send());b.failure(107);b.timeout();assert(b.outcome().primary==107);
    assert(b.outcome().goal==0&&b.outcome().native_outcome_unknown);assert(b.before_send()==17);
    Journal c;assert(!c.before_send());assert(!c.accepted(7));assert(!c.before_cancel(7));
    assert(!c.cancelled(7,false));assert(c.phase()==Phase::cancel_rejected);assert(!c.feedback(7));
    assert(!c.terminal(7,2,-4));assert(c.outcome().primary==5);
    Journal d;assert(!d.before_send());assert(!d.accepted(8));assert(!d.terminal(8,2,0));
    assert(d.outcome().primary==0&&!d.outcome().native_outcome_unknown);
}
