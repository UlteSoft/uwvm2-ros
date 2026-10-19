/** @file
 * @brief C++20 Node, typed resources, and a guest-driven executor for ABI v1.
 *
 * Select imports explicitly with ros1.h or ros2.h before creating a Node. This
 * is a standalone capability API inspired by familiar ROS programming patterns;
 * it is not source/binary compatible with rclcpp or roscpp. It creates no native
 * thread and performs no hidden retries. The host controls endpoints, types,
 * QoS, quotas, transitions, severity, frames, and rights through alias grants.
 * Use abi/ros1_freestanding.hpp or abi/ros2_freestanding.hpp for allocation-free
 * C++11 raw byte ownership. WASI tutorials compile with -fno-exceptions: ROS
 * refusals still use numeric errors, while allocation exhaustion follows the
 * target standard library's termination policy. Callbacks must not throw in
 * that profile. Exception-enabled native applications retain normal unwinding.
 */
#pragma once
#include "roswasmc.h"
#include "session.h"
#include "action_bundle.h"
#include "scheduler.hpp"
#include "codec.hpp"
#include <functional>
#include <utility>
#include <algorithm>
#include <deque>

/** @brief C++ capability API. Resource objects own handles and must stay in their guest instance. */
#include "events.h"
namespace roswasmcpp {
/** @brief Canonical bytes and complete codecs, including arrays and typed parameter events. */
namespace wire = ::uwvm_ros;
/** @brief Resource result with explicit required-length validity. No error triggers automatic replay. */
using Result = ::roswasmc_result;
/** @brief Supported topic message values. Canonical names are selected by host aliases. */
namespace msg {
/** @brief UTF-8 String payload; this owning object is independent of its wire buffer. */
struct String { std::string data; };
/** @brief Bool payload, canonically encoded as byte 0 or 1. */
struct Bool { bool data{}; };
/** @brief Finite linear XYZ/angular XYZ Twist fields. */
struct Twist { std::array<double,3> linear{},angular{}; };
/** @brief Complete canonical Image value; ABI includes sequence even for ROS2's sequence-free Header. */
using Image = wire::image;
/** @brief Supported TransformStamped value; host policy fixes parent/child frames. */
using Transform = wire::transform;
/** @brief Legacy String parameter event; exact node/name/type authority is granted separately. */
using ParameterEvent = wire::parameter_event;
/** @brief Typed/deletion-only parameter event with complete scalar and array fields. */
using TypedParameterEvent = wire::typed_parameter_event;
}
/** @brief Supported service definitions; requests/responses use canonical ABI bytes. */
namespace srv {
/** @brief Trigger service: empty request, Bool plus String response. */
struct Trigger {struct Request {};using Response=wire::service_response;};
/** @brief SetBool service: one Bool request, Bool plus String response. */
struct SetBool {struct Request {bool data{};};using Response=wire::service_response;};
}
/** @brief Decoder/encoder specializations for trusted builtin message values. */
template<class T> struct Codec;
/** @brief std_msgs String wire mapping. Encode validates UTF-8 and canonical byte bounds. */
template<> struct Codec<msg::String> {
    /** @brief Encode a complete supported value; false preserves the supplied owning buffer. */
    static bool encode(msg::String const& value,wire::bytes& out) {
        if(value.data.size()>4193782u||!wire::parameter_array_detail::text_valid(value.data,true)) return false;
        out=wire::string(value.data);return true;
    }
    /** @brief Decode exact canonical bytes; false preserves the supplied destination. */
    static bool decode(std::span<unsigned char const> bytes,msg::String& out) {
        msg::String next;if(!wire::decode_string(bytes,next.data)||!wire::parameter_array_detail::text_valid(next.data,true)) return false;out=std::move(next);return true;
    }
};
/** @brief std_msgs Bool wire mapping. */
template<> struct Codec<msg::Bool> {
    /** @brief Encode a complete supported value; false preserves the supplied owning buffer. */
    static bool encode(msg::Bool const& value,wire::bytes& out) {out=wire::boolean(value.data);return true;}
    /** @brief Decode exact canonical bytes; false preserves the supplied destination. */
    static bool decode(std::span<unsigned char const> bytes,msg::Bool& out) {return wire::decode_boolean(bytes,out.data);}
};
/** @brief geometry_msgs Twist wire mapping; nonfinite fields fail before import. */
template<> struct Codec<msg::Twist> {
    /** @brief Encode a complete supported value; false preserves the supplied owning buffer. */
    static bool encode(msg::Twist const& value,wire::bytes& out) {
        std::array<double,6> fields{};for(unsigned i=0;i<3;++i) {fields[i]=value.linear[i];fields[i+3]=value.angular[i];}
        for(auto field:fields) if(!std::isfinite(field)) return false;out=wire::twist(fields);return true;
    }
    /** @brief Decode exact canonical bytes; false preserves the supplied destination. */
    static bool decode(std::span<unsigned char const> bytes,msg::Twist& out) {
        std::array<double,6> fields{};if(!wire::decode_twist(bytes,fields)) return false;
        for(auto field:fields) if(!std::isfinite(field)) return false;
        msg::Twist next;for(unsigned i=0;i<3;++i) {next.linear[i]=fields[i];next.angular[i]=fields[i+3];}out=next;return true;
    }
};
/** @brief Image encoding delegates to the complete existing canonical codec. */
template<> struct Codec<msg::Image> {
    /** @brief Encode a complete supported value; false preserves the supplied owning buffer. */
    static bool encode(msg::Image const& value,wire::bytes& out) {auto next=wire::encode(value);if(next.empty()) return false;out=std::move(next);return true;}
    /** @brief Decode exact canonical bytes; false preserves the supplied destination. */
    static bool decode(std::span<unsigned char const> bytes,msg::Image& out) {return wire::decode(bytes,out);}
};
/** @brief Transform encoding delegates to the complete existing canonical codec. */
template<> struct Codec<msg::Transform> {
    /** @brief Encode a structurally complete transform only when all seven doubles are finite.
     * Host policy separately enforces frame names, translation bounds and quaternion norm.
     */
    static bool encode(msg::Transform const& value,wire::bytes& out) {
        for(auto number:value.translation) if(!std::isfinite(number)) return false;
        for(auto number:value.quaternion) if(!std::isfinite(number)) return false;
        auto next=wire::encode(value);if(next.empty()) return false;out=std::move(next);return true;
    }
    /** @brief Decode transactionally and reject nonfinite translation/quaternion fields. */
    static bool decode(std::span<unsigned char const> bytes,msg::Transform& out) {
        msg::Transform next;if(!wire::decode(bytes,next)) return false;
        for(auto number:next.translation) if(!std::isfinite(number)) return false;
        for(auto number:next.quaternion) if(!std::isfinite(number)) return false;
        out=std::move(next);return true;
    }
};
/** @brief Legacy parameter-event decoding for separately granted event subscriptions. */
template<> struct Codec<msg::ParameterEvent> {
    /** @brief Decode exact canonical bytes; false preserves the supplied destination. */
    static bool decode(std::span<unsigned char const> bytes,msg::ParameterEvent& out) {return wire::decode(bytes,out);}
};
/** @brief Complete typed/deletion parameter-event codec with transactional refusal. */
template<> struct Codec<msg::TypedParameterEvent> {
    /** @brief Encode a complete supported value; false preserves the supplied owning buffer. */
    static bool encode(msg::TypedParameterEvent const& value,wire::bytes& out) {return wire::encode_typed_parameter_event(value,out);}
    /** @brief Decode exact canonical bytes; false preserves the supplied destination. */
    static bool decode(std::span<unsigned char const> bytes,msg::TypedParameterEvent& out) {return wire::decode_typed_parameter_event(bytes,out);}
};
/** @brief Move-only capability with explicit open/close and raw bounded operations.
 * Destruction attempts close once. Transport failure does not guarantee a live
 * host handle, so explicit close always relinquishes this local owner.
 */
class Capability {
    friend class EventStream;
    friend class ServiceAvailability;
    roswasmc_capability cap_{};
    uint32_t error_{};
public:
    /** @brief Create an empty owner, with no backend or rights. */
    Capability()=default;
    /** @brief Open exactly the supplied alias using the selected context. */
    Capability(roswasmc_context const& context,std::string_view alias) {
        roswasmc_capability_init(&cap_,&context);error_=roswasmc_open(&cap_,alias.data(),alias.size());
    }
    /** @brief Open a registered request role only after local selector validation.
     * Missing backend roles return95 before open; malformed role selections
     * return22. Registration does not imply host permission or native support.
     */
    Capability(roswasmc_context const& context,std::string_view alias,roswasmc_operation operation,bool valid_role=true,size_t maximum_bytes=4193786u) {
        roswasmc_capability_init(&cap_,&context);
        error_=!valid_role?static_cast<uint32_t>(UWVM_ROS_INVALID):maximum_bytes>4193786u?static_cast<uint32_t>(UWVM_ROS_TOO_LARGE):roswasmc_check_operation(&context,operation);
        if(!error_) error_=roswasmc_open(&cap_,alias.data(),alias.size());
    }
    /** @brief Build a checked factory result without opening after a local failure. */
    static Capability checked(roswasmc_context const& context,std::string_view alias,uint32_t validation_error) {
        if(!validation_error) return Capability{context,alias};
        Capability result;roswasmc_capability_init(&result.cap_,&context);result.error_=validation_error;return result;
    }
    Capability(Capability const&)=delete;
    Capability& operator=(Capability const&)=delete;
    /** @brief Transfer local ownership; the moved-from owner is inert. */
    Capability(Capability&& other) noexcept:cap_{other.cap_},error_{other.error_} {other.cap_.handle=0;}
    /** @brief Close the current owner once, then transfer the incoming owner. */
    Capability& operator=(Capability&& other) noexcept {
        replace_checked(std::move(other));return *this;
    }
    /** @brief Retire this owner once, then transfer the incoming owner, exposing cleanup.
     * A failure107/110 does not retain the old resource or prevent the explicit
     * transfer. Ordinary move assignment/destruction is best-effort; use this
     * method when the old owner's independent cleanup result matters.
     */
    uint32_t replace_checked(Capability&& other) noexcept {
        if(this==&other) return 0;uint32_t code=cap_.handle?roswasmc_close(&cap_):0;
        cap_=other.cap_;error_=other.error_;other.cap_.handle=0;return code;
    }
    /** @brief Attempt one close, without throwing or replaying a failed cleanup. */
    ~Capability() {if(cap_.handle) roswasmc_close(&cap_);}
    /** @brief Return the construction error. Runtime operations return their own fresh errors. */
    uint32_t error() const noexcept {return error_;}
    /** @brief Borrow the opaque instance-owned handle; do not transfer or fabricate it. */
    uint64_t handle() const noexcept {return cap_.handle;}
    /** @brief Inspect ownership without opening or importing any resource. */
    bool is_open() const noexcept {return cap_.handle!=0;}
    /** @brief Relinquish ownership after one close attempt. */
    uint32_t close() {return roswasmc_close(&cap_);}
    /** @brief Revoke the alias and retire this local owner. */
    uint32_t revoke() {return roswasmc_revoke(&cap_);}
    /** @brief Publish canonical bytes through this owner's exact operation grant. */
    uint32_t publish_bytes(std::span<unsigned char const> bytes) const {return roswasmc_publish(&cap_,bytes.data(),bytes.size());}
    /** @brief Activate the topic queue; publication is separately authorized. */
    uint32_t subscribe() const {return roswasmc_subscribe(&cap_);}
    /** @brief Activate an advertised service queue. */
    uint32_t advertise_service() const {return roswasmc_service_advertise(&cap_);}
    /** @brief Activate a separately granted ROS2 parameter-event queue. */
    uint32_t subscribe_parameter_events() const {return roswasmc_parameter_subscribe_events(&cap_);}
    /** @brief Poll once; output is usable only when error is zero and length fits. */
    Result poll(std::span<unsigned char> output) const {return roswasmc_poll(&cap_,output.data(),output.size());}
    /** @brief Perform one named request with explicit storage; never automatically retry. */
    Result request(roswasmc_operation operation,std::span<unsigned char const> input,std::span<unsigned char> output) const {
        return roswasmc_request(&cap_,operation,input.data(),input.size(),output.data(),output.size());
    }
    /** @brief Log UTF-8 bytes at the host-selected severity and logger. */
    uint32_t log(std::string_view text) const {auto bytes=wire::string(text);if(bytes.empty()) return UWVM_ROS_INVALID;return roswasmc_log(&cap_,reinterpret_cast<char const*>(bytes.data()),bytes.size());}
};
/** @brief Copied event result. Body bytes remain in the caller-owned output. */
struct EventPoll { Result result;roswasmc_event_header header{}; };
/** @brief Independent observer/source association with exact local audit binding.
 * Both Capability owners and the extension table must outlive this noncopyable
 * facade. It opens no aliases and grants no rights. Source cleanup leaves the
 * independently owned observer available for permitted closed/error metadata.
 * Use existing Session/Node ownership for grouped checked cleanup.
 */
class EventStream {
    Capability& observer_;Capability& source_;roswasmc_event_backend const& api_;
    uint64_t source_handle_{},correlation_{},sequence_{},instance_{},session_{};
    uint32_t error_{};bool dispatching_{};
public:
    /** @brief Bind explicit owners once; nonzero correlation is only for Action. */
    EventStream(Capability& observer,Capability& source,roswasmc_event_backend const& api,uint64_t correlation=0)
      :observer_{observer},source_{source},api_{api},source_handle_{source.handle()},correlation_{correlation},
       error_{roswasmc_event_watch(&api_,&observer_.cap_,&source_.cap_,correlation)} {}
    EventStream(EventStream const&)=delete;EventStream& operator=(EventStream const&)=delete;
    /** @brief Return association error without attempting an implicit reopen. */
    uint32_t error() const noexcept {return error_;}
    /** @brief Copy/validate one full record;90 retains it,11 means currently idle.
     * No callback enters native code. The unchanged body begins at output+64.
     */
    EventPoll poll(std::span<unsigned char> output) {
        if(dispatching_)return {{UWVM_ROS_CONFLICT,0,0},{}};
        if(error_)return {{error_,0,0},{}};
        auto result=roswasmc_event_poll(&api_,&observer_.cap_,output.data(),output.size());EventPoll value{result,{}};
        if(result.error)return value;
        uint32_t error=roswasmc_event_decode(output.data(),result.length,&value.header);auto const& h=value.header;
        if(!error&&(h.backend!=api_.version||h.source!=source_handle_||sequence_==UINT64_MAX||h.sequence!=sequence_+1||
          (instance_&&(h.instance!=instance_||h.session!=session_))||(correlation_&&h.correlation&&h.correlation!=correlation_)))error=UWVM_ROS_INVALID;
        if(error){value.result.error=error;return value;}sequence_=h.sequence;instance_=h.instance;session_=h.session;return value;
    }
    /** @brief Dispatch a complete bound event on the caller thread once.
     * The body span expires when the callback returns. Source/session/sequence
     * binding is committed first; callback exceptions propagate without replay.
     * The scope guard restores dispatch state on ordinary exception unwinding.
     * Owners must outlive the stream and must not be moved/retired directly
     * while borrowed. Facade poll/close/revoke refuse17 during this callback.
     */
    uint32_t dispatch(std::span<unsigned char> output,
        std::function<void(roswasmc_event_header const&,std::span<unsigned char const>)> const& callback) {
        if(dispatching_)return UWVM_ROS_CONFLICT;if(!callback)return UWVM_ROS_INVALID;
        auto event=poll(output);if(event.result.error)return event.result.error;
        struct Reset {bool& value;~Reset(){value=false;}};dispatching_=true;Reset reset{dispatching_};
        callback(event.header,{output.data()+64,static_cast<size_t>(event.header.payload_bytes)});return 0;
    }
    /** @brief Read finite counters without reserving any future admission. */
    uint32_t ready(roswasmc_event_readiness& out) const {return error_?error_:roswasmc_event_ready(&api_,&observer_.cap_,&out);}
    /** @brief Admit an asynchronous service request once; completion is an event.
     * No automatic replay or synchronous RPC disguised as a Future occurs.
     */
    uint32_t submit(std::span<unsigned char const> input,uint64_t& request_id) const {return error_?error_:roswasmc_service_submit(&api_,&source_.cap_,&observer_.cap_,input.data(),input.size(),&request_id);}
    /** @brief Close only the explicit observer owner once, exposing its status. */
    uint32_t close_checked(){return dispatching_?static_cast<uint32_t>(UWVM_ROS_CONFLICT):observer_.close();}
    /** @brief Destructively revoke only the explicit observer alias. */
    uint32_t revoke_checked(){return dispatching_?static_cast<uint32_t>(UWVM_ROS_CONFLICT):observer_.revoke();}
};
/** @brief Unified event entity accepted by Executor::add without legacy polling.
 * Construct the stream from two explicitly opened owners first. This entity
 * borrows that stream and owns only fixed application-selected scratch/callback
 * state; it opens no resource. Remove its Executor registration before moving,
 * destroying or closing it. Service/Action bodies retain their existing codecs.
 */
class EventSubscription {
    EventStream& stream_;wire::bytes buffer_;uint32_t error_{};
    using Callback=std::function<void(roswasmc_event_header const&,std::span<unsigned char const>)>;
    std::shared_ptr<Callback> callback_;
public:
    /** @brief Capacity includes the64-byte envelope and cannot enlarge host quotas. */
    EventSubscription(EventStream& stream,size_t capacity,Callback callback)
      :stream_{stream},buffer_(capacity>=64&&capacity<=4194304u?capacity:0),
       error_{capacity<64?static_cast<uint32_t>(UWVM_ROS_INVALID):capacity>4194304u?static_cast<uint32_t>(UWVM_ROS_TOO_LARGE):stream.error()},
       callback_{callback?std::make_shared<Callback>(std::move(callback)):nullptr} {}
    EventSubscription(EventSubscription const&)=delete;EventSubscription& operator=(EventSubscription const&)=delete;
    /** @brief Local setup status; no extra native operation is attempted. */
    uint32_t error() const noexcept {return error_?error_:callback_?0:UWVM_ROS_INVALID;}
    /** @brief Snapshot callback ownership; reassignment affects the next pass only. */
    void on_event(Callback callback){callback_=callback?std::make_shared<Callback>(std::move(callback)):nullptr;}
    /** @brief One validated dispatch;11 idle and90 retained capacity failure. */
    uint32_t poll_once(){if(error())return error();auto callback=callback_;return stream_.dispatch(buffer_,*callback);}
    /** @brief Close observer only; the independent source remains caller-owned. */
    uint32_t close(){return stream_.close_checked();}
    /** @brief Revoke observer only, never an inferred source or cancel-any grant. */
    uint32_t revoke(){return stream_.revoke_checked();}
};
/** @brief Shared local factory/cleanup helpers; no backend operation is simulated. */
namespace detail {
/** @brief Intrinsic polling lease independent of an Executor registration.
 * Entry is checked before construction; exception unwinding resets the guard.
 * Moving/destroying the active resource remains a caller lifetime error.
 */
struct PollLease {
    bool& busy;explicit PollLease(bool& state):busy{state} {busy=true;}
    ~PollLease() {busy=false;}
    PollLease(PollLease const&)=delete;PollLease& operator=(PollLease const&)=delete;
};
/** @brief Check a two-owner server transaction before any host open. */
inline uint32_t server_validation(roswasmc_context const& context,std::string_view advertise,
    std::string_view respond,size_t capacity,bool service) {
    if(!context.backend||advertise.empty()||respond.empty()||advertise.size()>255||respond.size()>255||advertise==respond) return UWVM_ROS_INVALID;
    if(capacity>4193786u) return UWVM_ROS_TOO_LARGE;
    if(!context.backend->poll||(service&&!context.backend->advertise_service)) return UWVM_ROS_UNSUPPORTED;
    auto error=roswasmc_check_operation(&context,service?ROSWASMC_SERVICE_RESPOND:ROSWASMC_ACTION_ADVERTISE);
    if(error||service) return error;return roswasmc_check_operation(&context,ROSWASMC_ACTION_RESPOND);
}
/** @brief Retire one live C++ owner while retaining independent cleanup errors. */
inline uint32_t close_owner(Capability& owner,roswasmc_cleanup_report& report) {
    if(!owner.is_open()) return 0;auto code=owner.close();++report.attempted;
    if(code) {++report.failed;if(!report.cleanup_error) report.cleanup_error=code;}return code;
}
/** @brief Explicit alias mutation, attempted once independently for each owner. */
inline uint32_t revoke_owner(Capability& owner,roswasmc_cleanup_report& report) {
    if(!owner.is_open()) return 0;auto code=owner.revoke();++report.attempted;
    if(code) {++report.failed;if(!report.cleanup_error) report.cleanup_error=code;}return code;
}
}
/** @brief Bounded stable ownership session over an explicit backend.
 * Constructing it only reserves guest memory. It opens no aliases or native
 * node and inherits no rights. Session owns only the Capability slots returned
 * by open/open_for; typed Node resources remain independently owned.
 * Session/context must outlive borrowed slots. No session copy/move is allowed.
 */
class Session {
    roswasmc_context context_;std::vector<Capability> owners_;size_t capacity_;Context* lifecycle_{};
public:
    /** @brief Result whose owner is valid only on code0, until close/destruction. */
    struct OpenResult {uint32_t error;Capability* owner;};
    /** @brief Reserve an explicitly bounded number of owner slots; no ROS call.
     * Allocation failure follows the target standard library policy. The C
     * session.h variant uses caller storage when allocation must be avoided.
     */
    explicit Session(roswasmc_backend const& backend,size_t maximum_owners):context_{&backend},capacity_{maximum_owners} {owners_.reserve(maximum_owners);}
    /** @brief Associate local controlled shutdown; existing slots remain closeable. */
    explicit Session(Context& context,size_t maximum_owners):Session(context.backend(),maximum_owners) {lifecycle_=&context;}
    Session(Session const&)=delete;Session& operator=(Session const&)=delete;
    Session(Session&&)=delete;Session& operator=(Session&&)=delete;
    /** @brief Borrow the stable selected dispatch context; this confers no rights. */
    roswasmc_context const& context() const noexcept {return context_;}
    /** @brief Check actual ABI version1 explicitly, independently of ROS backend version. */
    uint32_t check_abi() const {return lifecycle_&&!lifecycle_->running()?UWVM_ROS_REVOKED:!context_.backend->abi_version?UWVM_ROS_UNSUPPORTED:context_.backend->abi_version()==1?0:UWVM_ROS_UNSUPPORTED;}
    /** @brief Open one alias transactionally without exceeding reserved storage.
     * Capacity is checked before an import; a failed open exposes no owner.
     * Slots never move while alive, because reserve fixes maximum capacity.
     */
    OpenResult open(std::string_view alias) {
        if(lifecycle_&&!lifecycle_->running()) return {UWVM_ROS_REVOKED,nullptr};
        if(owners_.size()>=capacity_) return {UWVM_ROS_TOO_LARGE,nullptr};
        Capability next{context_,alias};if(next.error()) return {next.error(),nullptr};
        owners_.push_back(std::move(next));return {0,&owners_.back()};
    }
    /** @brief Reject unregistered request operations before any alias open. */
    OpenResult open_for(std::string_view alias,roswasmc_operation operation) {
        if(lifecycle_&&!lifecycle_->running()) return {UWVM_ROS_REVOKED,nullptr};
        auto code=roswasmc_check_operation(&context_,operation);return code?OpenResult{code,nullptr}:open(alias);
    }
    /** @brief Close all live slots in reverse order and expose every requested status.
     * Empty statuses means aggregate reporting only. Otherwise supply at least
     * size() entries, indexed by acquisition order. Undersized storage fails
     * before cleanup. Borrowed slot pointers expire after successful cleanup.
     * Cleanup runs once per live owner even after failure107/110; no replay.
     */
    roswasmc_cleanup_report close(std::span<uint32_t> statuses={},uint32_t primary_error=0) {
        auto report=roswasmc_cleanup_init(primary_error);
        if(!statuses.empty()&&statuses.size()<owners_.size()) {report.cleanup_error=UWVM_ROS_INVALID;return report;}
        for(size_t n=owners_.size();n;--n) {auto code=detail::close_owner(owners_[n-1],report);if(!statuses.empty()) statuses[n-1]=code;}
        owners_.clear();return report;
    }
    /** @brief Return number of acquired slots, including explicitly retired ones. */
    size_t size() const noexcept {return owners_.size();}
};
/** @brief Typed publisher; opening its alias cannot choose or reconfigure a native endpoint. */
template<class Message> class Publisher {
    Capability capability_;
public:
    /** @brief Open one topic.publish alias. */
    Publisher(roswasmc_context const& context,std::string_view alias):capability_{Capability::checked(context,alias,
        !context.backend?UWVM_ROS_INVALID:!context.backend->publish?UWVM_ROS_UNSUPPORTED:0)} {}
    /** @brief Report alias-open failure before attempting publication. */
    uint32_t error() const noexcept {return capability_.error();}
    /** @brief Encode one supported builtin value and perform exactly one publication. */
    uint32_t publish(Message const& message) const {
        if(error()) return error();wire::bytes bytes;if(!Codec<Message>::encode(message,bytes)) return UWVM_ROS_INVALID;return capability_.publish_bytes(bytes);
    }
    /** @brief Explicitly close this resource. */
    uint32_t close() {return capability_.close();}
    /** @brief Borrow the actual opaque instance-owned handle for reviewed raw calls.
     * Never fabricate/transfer it or retain authority after close. A closed
     * facade returns zero; a saved old ID remains stale at the runtime. Raw ABI
     * calls operate outside typed borrow guards; never close/revoke/poll an ID
     * that is actively borrowed by a typed resource callback.
     */
    uint64_t handle() const noexcept {return capability_.handle();}
    /** @brief Inspect local lifetime without calling ROS or acquiring rights. */
    bool is_open() const noexcept {return capability_.is_open();}
};
/** @brief Typed event source. Storage is fixed by an explicit maximum at construction.
 * Size it to the host-granted complete event byte quota. An insufficient buffer
 * can consume an event; error90 is returned without an automatic grow/replay.
 */
template<class Message> class Subscription {
    Capability capability_;wire::bytes buffer_;uint32_t error_{};uint32_t rollback_error_{};
    std::shared_ptr<std::function<void(Message const&)>> callback_;bool polling_{};
public:
    /** @brief Open and activate one topic.subscribe or parameter-event alias.
     * @param parameter_events True selects the separately granted ROS2 event operation.
     */
    Subscription(roswasmc_context const& context,std::string_view alias,size_t maximum_bytes,bool parameter_events=false)
        :capability_{Capability::checked(context,alias,maximum_bytes>4193786u?UWVM_ROS_TOO_LARGE:!context.backend?UWVM_ROS_INVALID:
            (!context.backend->poll||(parameter_events?!context.backend->subscribe_parameter_events:!context.backend->subscribe))?UWVM_ROS_UNSUPPORTED:0)},buffer_(maximum_bytes<=4193786u?maximum_bytes:0) {
        error_=capability_.error();if(!error_) error_=parameter_events?capability_.subscribe_parameter_events():capability_.subscribe();
        if(error_&&capability_.is_open()) rollback_error_=capability_.close();
    }
    /** @brief Separate immediate rollback-close failure from the original setup error. */
    uint32_t rollback_error() const noexcept {return rollback_error_;}
    /** @brief Return setup error; a normal empty queue returns 11 only from poll_once. */
    uint32_t error() const noexcept {return error_?error_:capability_.is_open()?0:UWVM_ROS_BAD_HANDLE;}
    /** @brief Install a callback. Reassignment during invocation affects only
     * the next poll; the current poll holds a separate callable snapshot.
     */
    void on_message(std::function<void(Message const&)> callback) {
        callback_=callback?std::make_shared<std::function<void(Message const&)>>(std::move(callback)):nullptr;
    }
    /** @brief Poll/decode once. Direct nested poll/close/revoke returns17 before
     * imports/buffer mutation. The decoded message reference expires on return.
     * A callback exception propagates and resets this intrinsic busy lease.
     */
    uint32_t poll_once() {
        if(polling_) return UWVM_ROS_CONFLICT;detail::PollLease lease{polling_};
        if(error()) return error();auto callback=callback_;if(!callback) return UWVM_ROS_INVALID;auto result=capability_.poll(buffer_);if(result.error) return result.error;
        if(!result.has_length||result.length>buffer_.size()) return UWVM_ROS_INVALID;
        Message message;if(!Codec<Message>::decode(std::span<unsigned char const>{buffer_.data(),static_cast<size_t>(result.length)},message)) return UWVM_ROS_INVALID;
        (*callback)(message);return 0;
    }
    /** @brief Explicitly close the subscription. */
    uint32_t close() {return polling_?static_cast<uint32_t>(UWVM_ROS_CONFLICT):capability_.close();}
    /** @brief Explicit native alias revocation, refused17 during a callback. */
    uint32_t revoke() {return polling_?static_cast<uint32_t>(UWVM_ROS_CONFLICT):capability_.revoke();}
    /** @brief Borrow the actual opaque instance-owned handle for reviewed raw calls.
     * Never fabricate/transfer it or retain authority after close. A closed
     * facade returns zero; a saved old ID remains stale at the runtime. Raw ABI
     * calls operate outside typed borrow guards; never close/revoke/poll an ID
     * that is actively borrowed by a typed resource callback.
     */
    uint64_t handle() const noexcept {return capability_.handle();}
    /** @brief Inspect local lifetime without calling ROS or acquiring rights. */
    bool is_open() const noexcept {return capability_.is_open();}
};
/** @brief Guest-driven executor with explicit registrations and bounded spin passes.
 * Registered resources must outlive their registration. Remove before destroying
 * or moving them. Callbacks never run on native ROS threads; errors are returned.
 */
class Executor {
    struct Task {size_t id;std::function<uint32_t()> poll;};std::vector<Task> tasks_;size_t next_id_{1};bool spinning_{};
    struct TimerTask {size_t id;Timer* timer;};std::vector<TimerTask> timers_;
    struct DeferredTask {std::function<bool()> pending;std::function<uint32_t()> execute;std::function<void()> cancel;};
    std::deque<DeferredTask> deferred_;Context* context_{};size_t maximum_pending_{64},last_completions_{};bool stopped_{};
    struct SpinGuard {
        bool& flag;explicit SpinGuard(bool& value):flag{value} {flag=true;}
        ~SpinGuard() {flag=false;}
        SpinGuard(SpinGuard const&)=delete;SpinGuard& operator=(SpinGuard const&)=delete;
    };
public:
    /** @brief Default local scheduler with64 pending slots and no Context/clock/thread. */
    Executor()=default;
    /** @brief Associate controlled local shutdown and an explicit pending-task bound. */
    explicit Executor(Context& context,size_t maximum_pending=64):context_{&context},maximum_pending_{maximum_pending} {}
    Executor(Executor const&)=delete;Executor& operator=(Executor const&)=delete;
    Executor(Executor&&)=delete;Executor& operator=(Executor&&)=delete;
    /** @brief Cancel all unexecuted local futures on destruction, with no native action. */
    ~Executor() {cancel_pending();}
    /** @brief Queue one callable; it executes once on a later caller-driven spin.
     * The callable returns an owned value plus its real RPC code. Its synchronous
     * SDK call has already returned/released borrowed response storage before
     * completion runs. Completion receives its own value copy, not a native or
     * SDK scratch borrow. Captured resource references must outlive the queued
     * task; cancel pending work before closing/destroying those resources.
     * Queue exhaustion90 and stopped Context125 perform no callable/import.
     */
    template<class T> Future<T> post(std::function<DeferredResult<T>()> callable,
        std::function<void(DeferredResult<T>)> completion={},CallbackGroup* group=nullptr) {
        auto data=std::make_shared<detail::FutureData<T>>();Future<T> future{data};
        auto rejection=!callable?UWVM_ROS_INVALID:stopped_||(context_&&!context_->running())?UWVM_ROS_REVOKED:
            deferred_.size()>=maximum_pending_?UWVM_ROS_TOO_LARGE:0;
        if(rejection) {data->phase=FutureState::ready;data->error=rejection;return future;}
        deferred_.push_back({[data]{return data->phase==FutureState::pending;},
            [data,callable=std::move(callable),completion=std::move(completion),group]() mutable -> uint32_t {
                if(data->phase!=FutureState::pending) return 0;
                if(group&&!group->enter()) return UWVM_ROS_CONFLICT;detail::GroupLease group_lease{group};
                data->phase=FutureState::running;detail::RunningLease<T> running{data};
                auto result=callable();data->error=result.error;
                if(!result.error) {if constexpr(std::is_void_v<T>) data->value=detail::Unit{};else data->value=result.value;}
                data->phase=FutureState::ready;if(completion) completion(std::move(result));return 0;
            },[data]{if(data->phase==FutureState::pending) {data->phase=FutureState::cancelled;data->error=UWVM_ROS_REVOKED;}}});
        return future;
    }
    /** @brief Borrow one Timer. It ticks only through explicit-time spin_some_at.
     * Remove before destroying/moving a timer. Registration during spin is refused.
     */
    size_t add_timer(Timer& timer) {
        if(spinning_||stopped_||(context_&&!context_->running())||!next_id_||timer.error()) return 0;auto id=next_id_++;timers_.push_back({id,&timer});return id;
    }
    /** @brief Cancel local pending tasks, never an in-flight synchronous/native effect. */
    void cancel_pending() {for(auto& task:deferred_) task.cancel();deferred_.clear();}
    /** @brief Controlled local stop; Context and queued work stop, owners remain closeable. */
    void shutdown() {stopped_=true;if(context_) context_->shutdown();cancel_pending();}
    /** @brief Completed deferred callables in the last pass, including actual RPC errors.
     * A completion that throws is excluded from this count even though its
     * Future is already ready. This is a scheduling count, not successful ROS
     * deliveries. Inspect each Future; no-exception builds require callbacks
     * that return normally.
     */
    size_t completed_requests() const noexcept {return last_completions_;}
    /** @brief Borrow a source exposing poll_once; return a removable nonzero registration ID.
     * Return0 without insertion when called during a polling pass or after ID
     * exhaustion. Defer callback-originated registration changes until the pass ends.
     */
    template<class Source> size_t add(Source& source) {
        if(spinning_||stopped_||(context_&&!context_->running())||!next_id_) return 0;auto id=next_id_++;tasks_.push_back({id,[&source]{return source.poll_once();}});return id;
    }
    /** @brief Remove before a source is moved/destroyed, after its entire polling pass ends.
     * Calls during a pass are ignored to keep the currently executing closure and
     * iterator alive. The application must perform deferred removal after spin_some.
     */
    void remove(size_t id) {if(spinning_) return;std::erase_if(tasks_,[id](Task const& task){return task.id==id;});std::erase_if(timers_,[id](TimerTask const& task){return task.id==id;});}
    /** @brief Remove all borrowed registrations outside a polling pass; busy calls are ignored. */
    void clear() {if(!spinning_) {tasks_.clear();timers_.clear();cancel_pending();}}
    /** @brief Poll every registered source at most once, up to maximum_events successes.
     * @param delivered Number of successful event deliveries before return.
     * Queue-empty11 is idle work. Any other error ends the pass without retries.
     * Recursive calls return22 without changing delivered. Registration mutation
     * is blocked during this pass, including while callbacks run. Resources must
     * outlive the entire pass, even after an ignored callback removal attempt.
     * With C++ exceptions enabled, a callback exception propagates and resets
     * the busy flag through SpinGuard. A no-exception build requires callbacks
     * that do not throw; allocation/termination failures are not ROS error codes.
     */
    /** @brief Poll each registered source once; no sleeping, clock or hidden retry. */
    uint32_t spin_once(size_t& delivered) {return spin_some(SIZE_MAX,delivered);}
    /** @brief Inspect registration count without calling a source. */
    size_t size() const noexcept {return tasks_.size()+timers_.size();}
    uint32_t spin_some(size_t maximum_events,size_t& delivered) {
        return spin_impl(false,0,maximum_events,delivered);
    }
    /** @brief Spin with explicit caller time. Timers execute at most once each.
     * Without this overload timers remain untouched. Source/timer deliveries and
     * deferred request completions are counted separately; neither creates a clock.
     */
    uint32_t spin_some_at(uint64_t now,size_t maximum_events,size_t& delivered) {return spin_impl(true,now,maximum_events,delivered);}
private:
    uint32_t spin_impl(bool has_time,uint64_t now,size_t maximum_events,size_t& delivered) {
        if(spinning_) return UWVM_ROS_INVALID;
        if(stopped_||(context_&&!context_->running())) {cancel_pending();return UWVM_ROS_REVOKED;}
        delivered=0;last_completions_=0;SpinGuard guard{spinning_};
        auto pending_count=deferred_.size();
        for(size_t n=0;n<pending_count&&!deferred_.empty();++n) {
            auto task=std::move(deferred_.front());deferred_.pop_front();if(!task.pending()) continue;
            auto code=task.execute();if(code) {deferred_.push_front(std::move(task));return code;}++last_completions_;
            if(stopped_||(context_&&!context_->running())) {cancel_pending();return UWVM_ROS_REVOKED;}
        }
        for(auto const& task:tasks_) {
            if(delivered==maximum_events) break;auto error=task.poll();if(error!=UWVM_ROS_AGAIN) {if(error) return error;++delivered;}
            if(stopped_||(context_&&!context_->running())) {cancel_pending();return UWVM_ROS_REVOKED;}
        }
        if(has_time) for(auto const& task:timers_) {
            if(delivered==maximum_events) break;auto error=task.timer->tick(now);if(error!=UWVM_ROS_AGAIN) {if(error) return error;++delivered;}
            if(stopped_||(context_&&!context_->running())) {cancel_pending();return UWVM_ROS_REVOKED;}
        }
        return 0;
    }
};
/** @brief Supported request type mapping; no custom IDL adapter is fabricated. */
template<class Service> struct ServiceCodec;
/** @brief Trigger request has zero bytes. */
template<> struct ServiceCodec<srv::Trigger> {
    static wire::bytes encode(srv::Trigger::Request const&) {return {};}
    /** @brief Decode exact canonical bytes; false preserves the supplied destination. */
    static bool decode(std::span<unsigned char const> bytes,srv::Trigger::Request&) {return bytes.empty();}
};
/** @brief SetBool request has exactly one canonical Bool byte. */
template<> struct ServiceCodec<srv::SetBool> {
    static wire::bytes encode(srv::SetBool::Request const& request) {return wire::boolean(request.data);}
    /** @brief Decode exact canonical bytes; false preserves the supplied destination. */
    static bool decode(std::span<unsigned char const> bytes,srv::SetBool::Request& out) {return wire::decode_boolean(bytes,out.data);}
};
/** @brief Typed synchronous service client. Explicit response capacity bounds allocation. */
template<class Service> class ServiceClient {
    wire::bytes buffer_;Capability capability_;
public:
    /** @brief Open one service.call alias. Host timeout and quotas remain authoritative. */
    ServiceClient(roswasmc_context const& context,std::string_view alias,size_t response_capacity):buffer_(response_capacity<=4193786u?response_capacity:0),capability_{context,alias,ROSWASMC_SERVICE_CALL,true,response_capacity} {}
    /** @brief Return alias-open status. */
    uint32_t error() const noexcept {return capability_.error();}
    /** @brief Call once and decode transactionally; never repeat an RPC after timeout/quota failure. */
    uint32_t call(typename Service::Request const& request,typename Service::Response& response) {
        if(error()) return error();auto bytes=ServiceCodec<Service>::encode(request);auto result=capability_.request(ROSWASMC_SERVICE_CALL,bytes,buffer_);
        if(result.error) return result.error;typename Service::Response next;
        if(!result.has_length||result.length>buffer_.size()) return UWVM_ROS_INVALID;
        if constexpr(requires {ServiceCodec<Service>::decode_response(std::span<unsigned char const>{},next);}) {
            if(!ServiceCodec<Service>::decode_response({buffer_.data(),static_cast<size_t>(result.length)},next)) return UWVM_ROS_INVALID;
        } else {
            if(!wire::decode({buffer_.data(),static_cast<size_t>(result.length)},next)||!wire::parameter_array_detail::text_valid(next.message,true)) return UWVM_ROS_INVALID;
        }
        response=std::move(next);return 0;
    }
    /** @brief Defer one existing bounded synchronous call on a local Executor.
     * This client must outlive the task. The request is copied into guest-owned
     * task storage; one later spin executes call once and delivers an owned
     * response completion. This does not create a native asynchronous service,
     * deadline, thread, timer or retry. Host timeout/quota policy is unchanged.
     */
    Future<typename Service::Response> async_call(Executor& executor,typename Service::Request request,
        std::function<void(DeferredResult<typename Service::Response>)> completion={},CallbackGroup* group=nullptr) {
        return executor.template post<typename Service::Response>([this,request=std::move(request)] {
            typename Service::Response response{};auto code=call(request,response);return DeferredResult<typename Service::Response>{code,std::move(response)};
        },std::move(completion),group);
    }
    /** @brief Close once, exposing transport errors; ownership retires even on error. */
    uint32_t close() {return capability_.close();}
    /** @brief Borrow this stable source owner for the additive EventStream ABI.
     * The ServiceClient and its context must outlive that stream. This borrow
     * neither opens an observer nor grants submit/watch rights. Never move,
     * close or revoke the source while an event callback borrows it.
     */
    Capability& borrowed_event_source() noexcept {return capability_;}
    /** @brief Borrow the actual opaque instance-owned handle for reviewed raw calls.
     * Never fabricate/transfer it or retain authority after close. A closed
     * facade returns zero; a saved old ID remains stale at the runtime. Raw ABI
     * calls operate outside typed borrow guards; never close/revoke/poll an ID
     * that is actively borrowed by a typed resource callback.
     */
    uint64_t handle() const noexcept {return capability_.handle();}
    /** @brief Inspect local lifetime without calling ROS or acquiring rights. */
    bool is_open() const noexcept {return capability_.is_open();}
};
/** @brief Typed service server with two explicitly granted owners and guest callbacks.
 * Request IDs originate from the gateway. The respond grant must match the
 * advertise resource/type; replying with another server's ID is denied.
 */
template<class Service> class ServiceServer {
    wire::bytes buffer_;Capability advertise_,respond_;uint32_t error_{};roswasmc_cleanup_report rollback_{};
    std::shared_ptr<std::function<typename Service::Response(typename Service::Request const&)>> callback_;bool polling_{};
public:
    /** @brief Open advertise/respond aliases separately; no response rights are implied. */
    ServiceServer(roswasmc_context const& context,std::string_view advertise_alias,std::string_view respond_alias,size_t request_capacity)
        :buffer_(request_capacity<=4193786u?request_capacity:0),advertise_{Capability::checked(context,advertise_alias,detail::server_validation(context,advertise_alias,respond_alias,request_capacity,true))} {
        error_=advertise_.error();
        if(!error_) {respond_=Capability{context,respond_alias,ROSWASMC_SERVICE_RESPOND};error_=respond_.error();}
        if(!error_) error_=advertise_.advertise_service();
        if(error_) rollback_=close_checked(nullptr,error_);
    }
    /** @brief Return alias/advertisement status. */
    uint32_t error() const noexcept {return error_?error_:advertise_.is_open()&&respond_.is_open()?0:UWVM_ROS_BAD_HANDLE;}
    /** @brief Install a synchronous handler. Reassignment affects the next
     * invocation, never destroys the current callable snapshot.
     */
    void on_request(std::function<typename Service::Response(typename Service::Request const&)> callback) {
        callback_=callback?std::make_shared<std::function<typename Service::Response(typename Service::Request const&)>>(std::move(callback)):nullptr;
    }
    /** @brief Install an already allocated callable owner before any event is consumed.
     * The caller constructs this shared function before opening the server. This
     * setter performs only a nonthrowing shared-owner move, with no allocation.
     */
    void on_request_shared(std::shared_ptr<std::function<typename Service::Response(typename Service::Request const&)>> callback) noexcept {callback_=std::move(callback);}
    /** @brief Poll one owned request and send one response. Register this server with Executor. */
    uint32_t poll_once() {
        if(polling_) return UWVM_ROS_CONFLICT;detail::PollLease lease{polling_};
        if(error()) return error();auto callback=callback_;if(!callback) return UWVM_ROS_INVALID;auto result=advertise_.poll(buffer_);if(result.error) return result.error;
        if(!result.has_length||result.length<8||result.length>buffer_.size()) return UWVM_ROS_INVALID;
        auto id=uwvm_ros_load_u64(buffer_.data());if(!id) return UWVM_ROS_INVALID;typename Service::Request request;
        if(!ServiceCodec<Service>::decode({buffer_.data()+8,static_cast<size_t>(result.length)-8},request)) return UWVM_ROS_INVALID;
        auto response=(*callback)(request);
        if constexpr(requires(wire::bytes& payload) {ServiceCodec<Service>::encode_response(response,payload);}) {
            wire::bytes payload;if(!ServiceCodec<Service>::encode_response(response,payload)) return UWVM_ROS_INVALID;
            auto bytes=wire::encode(wire::service_envelope{id,std::move(payload)});
            if(bytes.empty()) return UWVM_ROS_INVALID;return respond_.request(ROSWASMC_SERVICE_RESPOND,bytes,{}).error;
        } else {
            if(!wire::parameter_array_detail::text_valid(response.message,true)) return UWVM_ROS_INVALID;
            auto bytes=wire::service_reply(id,response);return respond_.request(ROSWASMC_SERVICE_RESPOND,bytes,{}).error;
        }
    }
    /** @brief Inspect immediate construction rollback without replacing setup error. */
    roswasmc_cleanup_report rollback_status() const noexcept {return rollback_;}
    /** @brief Raw ABI escape IDs; neither implies another operation grant.
     * Direct imported/backend calls bypass the local facade polling lease.
     * Do not close/revoke/poll these IDs during an active handler.
     */
    uint64_t advertise_handle() const noexcept {return advertise_.handle();}
    uint64_t respond_handle() const noexcept {return respond_.handle();}
    /** @brief Close respond then advertise once, retaining both optional statuses.
     * statuses[0]/[1] correspond to advertise/respond. Empty owners are skipped;
     * the primary_error argument is retained separately from cleanup failures.
     */
    roswasmc_cleanup_report close_checked(uint32_t* statuses=nullptr,uint32_t primary_error=0) {
        auto report=roswasmc_cleanup_init(primary_error);if(polling_) {report.cleanup_error=UWVM_ROS_CONFLICT;return report;}
        auto response=detail::close_owner(respond_,report);auto advertise=detail::close_owner(advertise_,report);
        if(statuses) {statuses[0]=advertise;statuses[1]=response;}return report;
    }
    /** @brief Convenience first-error close. Use close_checked to inspect each owner. */
    uint32_t close() {if(polling_) return UWVM_ROS_CONFLICT;auto report=close_checked();return report.attempted?report.cleanup_error:static_cast<uint32_t>(UWVM_ROS_BAD_HANDLE);}
    /** @brief Explicitly revoke both independent aliases, never an automatic
     * shutdown/timeout action. Busy17 leaves owners/status slots unchanged.
     */
    roswasmc_cleanup_report revoke_checked(uint32_t* statuses=nullptr,uint32_t primary_error=0) {
        auto report=roswasmc_cleanup_init(primary_error);if(polling_) {report.cleanup_error=UWVM_ROS_CONFLICT;return report;}
        auto response=detail::revoke_owner(respond_,report);auto advertise=detail::revoke_owner(advertise_,report);
        if(statuses) {statuses[0]=advertise;statuses[1]=response;}return report;
    }
    /** @brief Deliberately revoke the pair once; inspect revoke_checked for all statuses. */
    uint32_t revoke() {if(polling_) return UWVM_ROS_CONFLICT;auto report=revoke_checked();return report.attempted?report.cleanup_error:static_cast<uint32_t>(UWVM_ROS_BAD_HANDLE);}

};
/** @brief Fibonacci action goal delivered to an explicitly advertised guest server. */
struct FibonacciGoal {uint64_t id{};int32_t order{};};
/** @brief Supported Fibonacci action client; every operation takes its own alias.
 * Use separate Capability objects for send/cancel/feedback/result. This class
 * wraps one chosen operation without implicitly opening the other aliases.
 */
class ActionClient {
    Capability capability_;roswasmc_operation operation_;wire::bytes buffer_;
public:
    /** @brief Open one action operation alias; capacity is an explicit response bound. */
    ActionClient(roswasmc_context const& context,std::string_view alias,roswasmc_operation operation,size_t capacity)
        :capability_{context,alias,operation,operation>=ROSWASMC_ACTION_SEND_GOAL&&operation<=ROSWASMC_ACTION_RECEIVE_RESULT,capacity},operation_{operation},buffer_(capacity<=4193786u?capacity:0) {}
    /** @brief Return alias-open status. */
    uint32_t error() const noexcept {return capability_.error();}
    /** @brief Send one Fibonacci order through a send-goal grant; return its nonzero owned goal ID. */
    uint32_t send_goal(int32_t order,uint64_t& goal) {
        if(error()) return error();if(operation_!=ROSWASMC_ACTION_SEND_GOAL) return UWVM_ROS_DENIED;
        auto bytes=wire::fibonacci_goal(order);auto result=capability_.request(operation_,bytes,buffer_);if(result.error) return result.error;
        if(!result.has_length||result.length!=8||buffer_.size()<8) return UWVM_ROS_INVALID;auto next=uwvm_ros_load_u64(buffer_.data());if(!next) return UWVM_ROS_INVALID;goal=next;return 0;
    }
    /** @brief Cancel one goal through a distinct own-goal/any-goal grant; decode the accepted Bool. */
    uint32_t cancel(uint64_t goal,bool& accepted) {
        if(error()) return error();if(operation_!=ROSWASMC_ACTION_CANCEL_OWN_GOAL&&operation_!=ROSWASMC_ACTION_CANCEL_ANY_GOAL) return UWVM_ROS_DENIED;
        auto bytes=wire::goal_id(goal);auto result=capability_.request(operation_,bytes,buffer_);if(result.error) return result.error;
        if(!result.has_length||result.length>buffer_.size()||!wire::decode_boolean({buffer_.data(),static_cast<size_t>(result.length)},accepted)) return UWVM_ROS_INVALID;return 0;
    }
    /** @brief Cancel all goals only through the independently granted cancel-any operation.
     * The canonical request ID is zero for this operation. Cancel-own does not
     * imply this stronger authority and is refused before issuing an import.
     */
    uint32_t cancel_all(bool& accepted) {
        if(operation_!=ROSWASMC_ACTION_CANCEL_ANY_GOAL) return UWVM_ROS_DENIED;return cancel(0,accepted);
    }
    /** @brief Receive one feedback/result event for an owned goal; error11 means none yet. */
    uint32_t receive(uint64_t goal,wire::action_sequence& sequence) {
        if(error()) return error();bool result_kind=operation_==ROSWASMC_ACTION_RECEIVE_RESULT;
        if(!result_kind&&operation_!=ROSWASMC_ACTION_RECEIVE_FEEDBACK) return UWVM_ROS_DENIED;
        auto bytes=wire::goal_id(goal);auto result=capability_.request(operation_,bytes,buffer_);if(result.error) return result.error;
        wire::action_sequence next;if(!result.has_length||result.length>buffer_.size()||!wire::decode({buffer_.data(),static_cast<size_t>(result.length)},next,result_kind)||next.goal!=goal||(result_kind&&next.kind<2)) return UWVM_ROS_INVALID;
        sequence=std::move(next);return 0;
    }
    /** @brief Close once, exposing transport errors; ownership retires even on error. */
    uint32_t close() {return capability_.close();}
    /** @brief Explicitly revoke only this independently owned role alias. */
    uint32_t revoke() {return capability_.revoke();}
    /** @brief Borrow the actual opaque instance-owned handle for reviewed raw calls.
     * Never fabricate/transfer it or retain authority after close. A closed
     * facade returns zero; a saved old ID remains stale at the runtime. Raw ABI
     * calls operate outside typed borrow guards; never close/revoke/poll an ID
     * that is actively borrowed by a typed resource callback.
     */
    uint64_t handle() const noexcept {return capability_.handle();}
    /** @brief Inspect local lifetime without calling ROS or acquiring rights. */
    bool is_open() const noexcept {return capability_.is_open();}
};
/** @brief Fibonacci action server with explicit advertise/respond grants and guest polling. */
class ActionServer {
    Capability advertise_,respond_;wire::bytes buffer_;uint32_t error_{};roswasmc_cleanup_report rollback_{};std::shared_ptr<std::function<void(FibonacciGoal const&)>> callback_;bool polling_{};
public:
    /** @brief Open both server aliases and activate advertisement with an empty canonical request. */
    ActionServer(roswasmc_context const& context,std::string_view advertise_alias,std::string_view respond_alias,size_t goal_capacity)
        :advertise_{Capability::checked(context,advertise_alias,detail::server_validation(context,advertise_alias,respond_alias,goal_capacity,false))},buffer_(goal_capacity<=4193786u?goal_capacity:0) {
        error_=advertise_.error();
        if(!error_) {respond_=Capability{context,respond_alias,ROSWASMC_ACTION_RESPOND};error_=respond_.error();}
        if(!error_) error_=advertise_.request(ROSWASMC_ACTION_ADVERTISE,{},{}).error;
        if(error_) rollback_=close_checked(nullptr,error_);
    }
    /** @brief Return alias/advertisement status. */
    uint32_t error() const noexcept {return error_?error_:advertise_.is_open()&&respond_.is_open()?0:UWVM_ROS_BAD_HANDLE;}
    /** @brief Install a callback; respond remains allowed inside the handler.
     * Reassignment affects the next poll, never destroys the active snapshot.
     */
    void on_goal(std::function<void(FibonacciGoal const&)> callback) {
        callback_=callback?std::make_shared<std::function<void(FibonacciGoal const&)>>(std::move(callback)):nullptr;
    }
    /** @brief Poll one goal ID/order event. Register this source with Executor. */
    uint32_t poll_once() {
        if(polling_) return UWVM_ROS_CONFLICT;detail::PollLease lease{polling_};
        if(error()) return error();auto callback=callback_;if(!callback) return UWVM_ROS_INVALID;auto result=advertise_.poll(buffer_);if(result.error) return result.error;
        if(!result.has_length||result.length!=12||buffer_.size()<12) return UWVM_ROS_INVALID;
        wire::reader reader{{buffer_.data(),12}};FibonacciGoal goal;uint32_t order;
        if(!reader.integer(goal.id,8)||!goal.id||!reader.u32(order)||!reader.done()) return UWVM_ROS_INVALID;
        goal.order=std::bit_cast<int32_t>(order);(*callback)(goal);return 0;
    }
    /** @brief Send feedback1 or a terminal result2/3/4 for a goal owned by this advertisement. */
    uint32_t respond(wire::action_sequence const& sequence) {
        if(error()) return error();if(!sequence.goal||sequence.kind<1||sequence.kind>4) return UWVM_ROS_INVALID;
        auto bytes=wire::encode(sequence,true);if(bytes.empty()) return UWVM_ROS_INVALID;return respond_.request(ROSWASMC_ACTION_RESPOND,bytes,{}).error;
    }
    /** @brief Inspect immediate construction rollback without replacing setup error. */
    roswasmc_cleanup_report rollback_status() const noexcept {return rollback_;}
    /** @brief Raw ABI escape IDs; neither implies another operation grant.
     * Direct imported/backend calls bypass the local facade polling lease.
     * Do not close/revoke/poll these IDs during an active handler.
     */
    uint64_t advertise_handle() const noexcept {return advertise_.handle();}
    uint64_t respond_handle() const noexcept {return respond_.handle();}
    /** @brief Close respond then advertise once, retaining both optional statuses.
     * statuses[0]/[1] correspond to advertise/respond. Empty owners are skipped;
     * the primary_error argument is retained separately from cleanup failures.
     */
    roswasmc_cleanup_report close_checked(uint32_t* statuses=nullptr,uint32_t primary_error=0) {
        auto report=roswasmc_cleanup_init(primary_error);if(polling_) {report.cleanup_error=UWVM_ROS_CONFLICT;return report;}
        auto response=detail::close_owner(respond_,report);auto advertise=detail::close_owner(advertise_,report);
        if(statuses) {statuses[0]=advertise;statuses[1]=response;}return report;
    }
    /** @brief Convenience first-error close. Use close_checked to inspect each owner. */
    uint32_t close() {if(polling_) return UWVM_ROS_CONFLICT;auto report=close_checked();return report.attempted?report.cleanup_error:static_cast<uint32_t>(UWVM_ROS_BAD_HANDLE);}
    /** @brief Explicit pair alias revocation, refused17 during poll/handler.
     * Every live role is attempted once even after transport failure; it is
     * destructive policy mutation and is never part of RAII cleanup.
     */
    roswasmc_cleanup_report revoke_checked(uint32_t* statuses=nullptr,uint32_t primary_error=0) {
        auto report=roswasmc_cleanup_init(primary_error);if(polling_) {report.cleanup_error=UWVM_ROS_CONFLICT;return report;}
        auto response=detail::revoke_owner(respond_,report);auto advertise=detail::revoke_owner(advertise_,report);
        if(statuses) {statuses[0]=advertise;statuses[1]=response;}return report;
    }
    /** @brief Deliberately revoke the pair; no stronger service/client roles are acquired. */
    uint32_t revoke() {if(polling_) return UWVM_ROS_CONFLICT;auto report=revoke_checked();return report.attempted?report.cleanup_error:static_cast<uint32_t>(UWVM_ROS_BAD_HANDLE);}

};
/** @brief Four independent required aliases and one optional stronger alias.
 * No role has an implicit default alias. Empty cancel_any means no acquisition;
 * the other four must be distinct nonempty explicit views. Host policy still
 * controls the matching action resource/type, ownership and every quota/QoS.
 */
struct ActionClientAliases {
    std::string_view send,cancel_own,feedback,result,cancel_any{};
};
/** @brief Typed Fibonacci client owning an explicitly requested role bundle.
 * Unlike the compatible single-role ActionClient, this facade groups send,
 * own-cancel, feedback and result while preserving four distinct capabilities.
 * Optional cancel-any is separately requested and never inferred. Construction
 * prechecks every role/alias, then acquires transactionally; failure rolls back
 * all acquired owners. Local goal IDs must originate from this bundle's valid
 * send result; unknown IDs are refused9 before import, and terminal results
 * retire only their matching local ID. Runtime ownership remains authoritative.
 *
 * This object is nonmoving: its C owners borrow its stable internal context and
 * bounded ID vector. Methods are caller-driven synchronous calls without replay.
 * Explicit checked close/revoke expose all role statuses; destruction performs
 * ordinary close only as a best-effort fallback, never native cancellation or
 * alias revocation. No rclcpp source/ABI or full native Action parity is claimed.
 */
class ActionClientBundle {
    roswasmc_context context_;std::vector<uint64_t> goals_;roswasmc_action_bundle bundle_{};
    wire::bytes receive_;uint32_t setup_error_{};Result last_{UWVM_ROS_BAD_HANDLE,0,0};
    uint32_t refusal(uint32_t code) {last_=roswasmc_action_bundle_failure(code);return code;}
    uint32_t receive(uint64_t goal,wire::action_sequence& sequence,bool terminal) {
        if(error()) return refusal(error());roswasmc_action_sequence view{};
        last_=terminal?roswasmc_action_bundle_result(&bundle_,goal,receive_.data(),receive_.size(),&view):
            roswasmc_action_bundle_feedback(&bundle_,goal,receive_.data(),receive_.size(),&view);
        if(last_.error) return last_.error;wire::action_sequence next;
        if(!last_.has_length||last_.length>receive_.size()||!wire::decode({receive_.data(),static_cast<size_t>(last_.length)},next,terminal)) {
            last_.error=UWVM_ROS_INVALID;return last_.error;
        }
        sequence=std::move(next);return 0;
    }
public:
    /** @brief Reserve explicit local bounds and acquire only supplied aliases.
     * Goal capacity1..1024 and complete reply bound<=4193786 are independent
     * of host quotas and cannot enlarge them. Allocation uses target C++ policy;
     * choose action_bundle.h caller storage when allocation must be avoided.
     */
    ActionClientBundle(roswasmc_context const& context,ActionClientAliases aliases,size_t maximum_goals,size_t maximum_reply_bytes)
        :context_{context},goals_(maximum_goals&&maximum_goals<=1024u?maximum_goals:0),receive_(maximum_reply_bytes<=4193786u?maximum_reply_bytes:0) {
        if(!maximum_goals||maximum_goals>1024u) {setup_error_=UWVM_ROS_INVALID;return;}
        if(maximum_reply_bytes>4193786u) {setup_error_=UWVM_ROS_TOO_LARGE;return;}
        roswasmc_action_aliases selected{{aliases.send.data(),aliases.send.size()},{aliases.cancel_own.data(),aliases.cancel_own.size()},
            {aliases.feedback.data(),aliases.feedback.size()},{aliases.result.data(),aliases.result.size()},
            {aliases.cancel_any.empty()?nullptr:aliases.cancel_any.data(),aliases.cancel_any.size()}};
        setup_error_=roswasmc_action_bundle_init(&context_,&bundle_,&selected,goals_.data(),goals_.size());
    }
    ActionClientBundle(ActionClientBundle const&)=delete;ActionClientBundle& operator=(ActionClientBundle const&)=delete;
    ActionClientBundle(ActionClientBundle&&)=delete;ActionClientBundle& operator=(ActionClientBundle&&)=delete;
    ~ActionClientBundle() {if(bundle_.active) roswasmc_action_bundle_close_checked(&bundle_,0,nullptr,0);}
    /** @brief Return setup failure or closed9 without any Runtime dispatch. */
    uint32_t error() const noexcept {return setup_error_?setup_error_:roswasmc_action_bundle_check(&bundle_);}
    /** @brief Inspect whether all selected roles remain locally active. */
    bool is_open() const noexcept {return error()==0;}
    /** @brief Inspect independent construction rollback report and role statuses. */
    roswasmc_cleanup_report rollback_status() const noexcept {return bundle_.rollback;}
    std::array<uint32_t,5> rollback_codes() const noexcept {
        return {bundle_.rollback_status[0],bundle_.rollback_status[1],bundle_.rollback_status[2],bundle_.rollback_status[3],bundle_.rollback_status[4]};
    }
    /** @brief Copy actual opaque handles in send/own/feedback/result/any order.
     * These IDs are diagnostic instance-local identities, not transferable rights.
     */
    std::array<uint64_t,5> handles() const noexcept {
        return {bundle_.cap[0].handle,bundle_.cap[1].handle,bundle_.cap[2].handle,bundle_.cap[3].handle,bundle_.cap[4].handle};
    }
    /** @brief Inspect explicit fifth-role acquisition without opening authority. */
    bool has_cancel_any() const noexcept {return is_open()&&bundle_.roles==5&&bundle_.cap[4].handle;}
    /** @brief Count tracked nonzero IDs; table ordering is deliberately unspecified. */
    size_t goal_count() const noexcept {return bundle_.count;}
    /** @brief Local provenance bookkeeping only; Runtime authorization is independent. */
    bool owns_goal(uint64_t goal) const noexcept {return roswasmc_action_bundle_owns(&bundle_,goal)!=0;}
    /** @brief Inspect the last dispatch/local refusal, including error90 length validity.
     * A required-length observation never permits automatic resize/replay.
     */
    Result last_result() const noexcept {return last_;}
    /** @brief Send once; output/table commit only a valid nonzero unique owned ID. */
    uint32_t send_goal(int32_t order,uint64_t& goal) {
        if(error()) return refusal(error());last_=roswasmc_action_bundle_send_goal(&bundle_,order,&goal);return last_.error;
    }
    /** @brief Cancel one tracked goal once; accepted is transactional and ID is retained. */
    uint32_t cancel_own(uint64_t goal,bool& accepted) {
        if(error()) return refusal(error());int next=0;last_=roswasmc_action_bundle_cancel_own(&bundle_,goal,&next);
        if(!last_.error) accepted=next!=0;return last_.error;
    }
    /** @brief Receive one exact feedback/result;11 remains pending, with no loop. */
    uint32_t receive_feedback(uint64_t goal,wire::action_sequence& sequence) {return receive(goal,sequence,false);}
    uint32_t receive_result(uint64_t goal,wire::action_sequence& sequence) {return receive(goal,sequence,true);}
    /** @brief Stronger explicit cancel-any/all; absent optional alias returns13 locally. */
    uint32_t cancel_all(bool& accepted) {
        if(error()) return refusal(error());int next=0;last_=roswasmc_action_bundle_cancel_all(&bundle_,&next);
        if(!last_.error) accepted=next!=0;return last_.error;
    }
    /** @brief Cancel a nonzero selected goal only through the explicit fifth role.
     * This stronger role can select a goal outside the local table; the Runtime
     * still enforces known-ID/resource/any-scope authority. Zero is refused22;
     * use cancel_all deliberately for the distinct all-goals request. No local
     * ID is retired from cancellation acknowledgement alone.
     */
    uint32_t cancel_any(uint64_t goal,bool& accepted) {
        if(error()) return refusal(error());int next=0;last_=roswasmc_action_bundle_cancel_any(&bundle_,goal,&next);
        if(!last_.error) accepted=next!=0;return last_.error;
    }
    /** @brief Queue one existing bounded synchronous send on a local Executor.
     * Bundle must outlive task; cancel pending work before close/destruction.
     * No native async thread, implicit timeout/clock or retry is introduced.
     */
    Future<uint64_t> async_send_goal(Executor& executor,int32_t order,std::function<void(DeferredResult<uint64_t>)> completion={},CallbackGroup* group=nullptr) {
        return executor.post<uint64_t>([this,order]{uint64_t goal=0;auto code=send_goal(order,goal);return DeferredResult<uint64_t>{code,goal};},std::move(completion),group);
    }
    /** @brief Checked reverse close/revoke with acquisition-order status slots.
     * Supply4 or5 entries when nonempty; inadequate storage refuses before
     * retiring anything. Revoke is an explicitly destructive alias operation.
     */
    roswasmc_cleanup_report close_checked(std::span<uint32_t> statuses={},uint32_t primary_error=0) {
        return roswasmc_action_bundle_close_checked(&bundle_,primary_error,statuses.empty()?nullptr:statuses.data(),statuses.size());
    }
    roswasmc_cleanup_report revoke_checked(std::span<uint32_t> statuses={},uint32_t primary_error=0) {
        return roswasmc_action_bundle_revoke_checked(&bundle_,primary_error,statuses.empty()?nullptr:statuses.data(),statuses.size());
    }
    /** @brief Ordinary reverse close, preserving first cleanup failure without replay. */
    uint32_t close() {return close_checked().cleanup_error;}
    /** @brief Deliberate reverse alias mutation; never an automatic destructor action. */
    uint32_t revoke() {return revoke_checked().cleanup_error;}
};
/** @brief Typed parameter value codec. Backend/type support is still enforced by host policy. */
template<class T> struct ParameterCodec;
/** @brief Scalar Bool parameter codec. */
template<> struct ParameterCodec<bool> {static wire::bytes encode(bool v){return wire::parameter_bool(v);}static bool decode(std::span<unsigned char const> b,bool& v){return wire::decode_parameter_bool(b,v);}};
/** @brief ROS1-only signed int32 parameter codec. */
template<> struct ParameterCodec<int32_t> {static wire::bytes encode(int32_t v){return wire::parameter_int32(v);}static bool decode(std::span<unsigned char const> b,int32_t& v){return wire::decode_parameter_int32(b,v);}};
/** @brief ROS2-only signed int64 parameter codec. */
template<> struct ParameterCodec<int64_t> {static wire::bytes encode(int64_t v){return wire::parameter_int64(v);}static bool decode(std::span<unsigned char const> b,int64_t& v){return wire::decode_parameter_int64(b,v);}};
/** @brief Finite Float64 parameter codec. */
template<> struct ParameterCodec<double> {static wire::bytes encode(double v){return wire::parameter_float64(v);}static bool decode(std::span<unsigned char const> b,double& v){return wire::decode_parameter_float64(b,v);}};
/** @brief String parameter codec. Scalar string wire permits NUL; native DDS limits are documented separately. */
template<> struct ParameterCodec<std::string> {
    /** @brief Encode well-formed UTF-8 scalar String while retaining canonical embedded NUL. */
    static wire::bytes encode(std::string const& value) {
        if(!wire::parameter_array_detail::text_valid(value,true)) return {};return wire::string(value);
    }
    /** @brief Decode well-formed UTF-8 scalar String transactionally, preserving the destination on failure. */
    static bool decode(std::span<unsigned char const> bytes,std::string& value) {
        std::string next;if(!wire::decode_string(bytes,next)||!wire::parameter_array_detail::text_valid(next,true)) return false;
        value=std::move(next);return true;
    }
};
/** @brief Complete canonical byte array parameter codec; host selects supported backend/type. */
template<> struct ParameterCodec<std::vector<uint8_t>> {
    static wire::bytes encode(std::vector<uint8_t> const& values){return wire::parameter_byte_array(values);}
    /** @brief Decode exact canonical bytes; false preserves the supplied destination. */
    static bool decode(std::span<unsigned char const> bytes,std::vector<uint8_t>& values){return wire::decode_parameter_byte_array(bytes,values);}
};
/** @brief Complete canonical int32 array parameter codec; host selects supported backend/type. */
template<> struct ParameterCodec<std::vector<int32_t>> {
    static wire::bytes encode(std::vector<int32_t> const& values){return wire::parameter_int32_array(values);}
    /** @brief Decode exact canonical bytes; false preserves the supplied destination. */
    static bool decode(std::span<unsigned char const> bytes,std::vector<int32_t>& values){return wire::decode_parameter_int32_array(bytes,values);}
};
/** @brief Complete canonical int64 array parameter codec; host selects supported backend/type. */
template<> struct ParameterCodec<std::vector<int64_t>> {
    static wire::bytes encode(std::vector<int64_t> const& values){return wire::parameter_int64_array(values);}
    /** @brief Decode exact canonical bytes; false preserves the supplied destination. */
    static bool decode(std::span<unsigned char const> bytes,std::vector<int64_t>& values){return wire::decode_parameter_int64_array(bytes,values);}
};
/** @brief Complete canonical float64 array parameter codec; host selects supported backend/type. */
template<> struct ParameterCodec<std::vector<double>> {
    static wire::bytes encode(std::vector<double> const& values){return wire::parameter_float64_array(values);}
    /** @brief Decode exact canonical bytes; false preserves the supplied destination. */
    static bool decode(std::span<unsigned char const> bytes,std::vector<double>& values){return wire::decode_parameter_float64_array(bytes,values);}
};
/** @brief Complete canonical string array parameter codec; host selects supported backend/type. */
template<> struct ParameterCodec<std::vector<std::string>> {
    static wire::bytes encode(std::vector<std::string> const& values){return wire::parameter_string_array(values);}
    /** @brief Decode exact canonical bytes; false preserves the supplied destination. */
    static bool decode(std::span<unsigned char const> bytes,std::vector<std::string>& values){return wire::decode_parameter_string_array(bytes,values);}
};
/** @brief Boolean array value with one explicit 0/1 byte per element; avoids vector<bool> packing. */
struct BooleanArray {std::vector<uint8_t> values;};
/** @brief Canonical BoolArray parameter codec; rejects bytes above one. */
template<> struct ParameterCodec<BooleanArray> {
    static wire::bytes encode(BooleanArray const& value){return wire::parameter_bool_array(value.values);}
    /** @brief Decode exact canonical bytes; false preserves the supplied destination. */
    static bool decode(std::span<unsigned char const> bytes,BooleanArray& value){return wire::decode_parameter_bool_array(bytes,value.values);}
};
/** @brief One explicitly granted parameter operation over its fixed node/name/type.
 * Construct separate objects for get/set/declare/etc. Metadata aliases are not
 * value aliases, and value grants do not grant parameter-event subscription.
 */
class Parameters {
    Capability capability_;roswasmc_operation operation_;wire::bytes buffer_;
public:
    /** @brief Open one parameter operation alias with an explicit response bound. */
    Parameters(roswasmc_context const& context,std::string_view alias,roswasmc_operation operation,size_t capacity)
        :capability_{context,alias,operation,operation>=ROSWASMC_PARAMETER_GET&&operation<=ROSWASMC_PARAMETER_UNDECLARE,capacity},operation_{operation},buffer_(capacity<=4193786u?capacity:0) {}
    /** @brief Return alias-open status. */
    uint32_t error() const noexcept {return capability_.error();}
    /** @brief Read one typed parameter through a parameter_get alias; output is transactional. */
    template<class T> uint32_t get(T& value) {
        if(error()) return error();if(operation_!=ROSWASMC_PARAMETER_GET) return UWVM_ROS_DENIED;
        auto result=capability_.request(operation_,{},buffer_);if(result.error) return result.error;
        T next{};if(!result.has_length||result.length>buffer_.size()||!ParameterCodec<T>::decode({buffer_.data(),static_cast<size_t>(result.length)},next)) return UWVM_ROS_INVALID;
        value=std::move(next);return 0;
    }
    /** @brief Set or declare one typed value through the chosen matching alias. No replay occurs. */
    template<class T> uint32_t set(T const& value) {
        if(error()) return error();if(operation_!=ROSWASMC_PARAMETER_SET&&operation_!=ROSWASMC_PARAMETER_DECLARE) return UWVM_ROS_DENIED;
        auto bytes=ParameterCodec<T>::encode(value);if(bytes.empty()) return UWVM_ROS_INVALID;
        auto result=capability_.request(operation_,bytes,buffer_);return result.error;
    }
    /** @brief Read exists metadata as Bool through its own ROS1 parameter_exists grant. */
    uint32_t exists(bool& value) {
        if(operation_!=ROSWASMC_PARAMETER_EXISTS) return UWVM_ROS_DENIED;return read_bool(value);
    }
    /** @brief Delete/undeclare using the host-selected exact parameter; no name is supplied by guest. */
    uint32_t erase() {
        if(error()) return error();if(operation_!=ROSWASMC_PARAMETER_DELETE&&operation_!=ROSWASMC_PARAMETER_UNDECLARE) return UWVM_ROS_DENIED;
        return capability_.request(operation_,{},buffer_).error;
    }
    /** @brief Read canonical String metadata for list/search/describe. Policy controls exact scope.
     * ROS1 parent search is available only when separately allowed by host policy.
     */
    uint32_t metadata(std::string& value) {
        if(error()) return error();if(operation_!=ROSWASMC_PARAMETER_LIST&&operation_!=ROSWASMC_PARAMETER_SEARCH&&operation_!=ROSWASMC_PARAMETER_DESCRIBE) return UWVM_ROS_DENIED;
        auto result=capability_.request(operation_,{},buffer_);if(result.error) return result.error;
        if(!result.has_length||result.length>buffer_.size()||!wire::decode_string({buffer_.data(),static_cast<size_t>(result.length)},value)) return UWVM_ROS_INVALID;return 0;
    }
private:
    uint32_t read_bool(bool& value) {
        if(error()) return error();auto result=capability_.request(operation_,{},buffer_);if(result.error) return result.error;
        if(!result.has_length||result.length>buffer_.size()||!wire::decode_boolean({buffer_.data(),static_cast<size_t>(result.length)},value)) return UWVM_ROS_INVALID;return 0;
    }
public:
    /** @brief Close once, exposing transport errors; ownership retires even on error. */
    uint32_t close() {return capability_.close();}
    /** @brief Borrow the actual opaque instance-owned handle for reviewed raw calls.
     * Never fabricate/transfer it or retain authority after close. A closed
     * facade returns zero; a saved old ID remains stale at the runtime. Raw ABI
     * calls operate outside typed borrow guards; never close/revoke/poll an ID
     * that is actively borrowed by a typed resource callback.
     */
    uint64_t handle() const noexcept {return capability_.handle();}
    /** @brief Inspect local lifetime without calling ROS or acquiring rights. */
    bool is_open() const noexcept {return capability_.is_open();}
};
/** @brief Read one host-filtered graph projection through its own inspect/list grant. */
class Graph {
    Capability capability_;roswasmc_operation operation_;wire::bytes buffer_;
public:
    /** @brief Open one graph alias; no unfiltered discovery or endpoint creation is implied. */
    Graph(roswasmc_context const& context,std::string_view alias,roswasmc_operation operation,size_t capacity)
        :capability_{context,alias,operation,operation>=ROSWASMC_GRAPH_INSPECT_TOPIC&&operation<=ROSWASMC_GRAPH_LIST_SERVICES,capacity},operation_{operation},buffer_(capacity<=4193786u?capacity:0) {}
    /** @brief Return alias-open status. */
    uint32_t error() const noexcept {return capability_.error();}
    /** @brief Read canonical String projection. The guest supplies no resource selector. */
    uint32_t inspect(std::string& text) {
        if(error()) return error();if(operation_<ROSWASMC_GRAPH_INSPECT_TOPIC||operation_>ROSWASMC_GRAPH_LIST_SERVICES) return UWVM_ROS_INVALID;
        auto result=capability_.request(operation_,{},buffer_);if(result.error) return result.error;
        if(!result.has_length||result.length>buffer_.size()||!wire::decode_string({buffer_.data(),static_cast<size_t>(result.length)},text)) return UWVM_ROS_INVALID;return 0;
    }
    /** @brief Close once, exposing transport errors; ownership retires even on error. */
    uint32_t close() {return capability_.close();}
    /** @brief Borrow the actual opaque instance-owned handle for reviewed raw calls.
     * Never fabricate/transfer it or retain authority after close. A closed
     * facade returns zero; a saved old ID remains stale at the runtime. Raw ABI
     * calls operate outside typed borrow guards; never close/revoke/poll an ID
     * that is actively borrowed by a typed resource callback.
     */
    uint64_t handle() const noexcept {return capability_.handle();}
    /** @brief Inspect local lifetime without calling ROS or acquiring rights. */
    bool is_open() const noexcept {return capability_.is_open();}
};
/** @brief One explicitly granted ROS or platform clock, independent of WASI clock rights. */
class Clock {
    Capability capability_;roswasmc_operation operation_;
public:
    /** @brief Open the chosen time.read-* alias. Unsupported backend clock operations return95. */
    Clock(roswasmc_context const& context,std::string_view alias,roswasmc_operation operation):capability_{context,alias,operation,operation>=ROSWASMC_TIME_READ_ROS&&operation<=ROSWASMC_TIME_READ_STEADY},operation_{operation} {}
    /** @brief Return alias-open status. */
    uint32_t error() const noexcept {return capability_.error();}
    /** @brief Read nanoseconds once, preserving output on error. */
    uint32_t now(uint64_t& nanoseconds) {
        if(error()) return error();if(operation_<ROSWASMC_TIME_READ_ROS||operation_>ROSWASMC_TIME_READ_STEADY) return UWVM_ROS_INVALID;
        unsigned char bytes[8];auto result=capability_.request(operation_,{},bytes);if(result.error) return result.error;
        if(!result.has_length||result.length!=8) return UWVM_ROS_INVALID;nanoseconds=uwvm_ros_load_u64(bytes);return 0;
    }
    /** @brief Close once, exposing transport errors; ownership retires even on error. */
    uint32_t close() {return capability_.close();}
    /** @brief Borrow the actual opaque instance-owned handle for reviewed raw calls.
     * Never fabricate/transfer it or retain authority after close. A closed
     * facade returns zero; a saved old ID remains stale at the runtime. Raw ABI
     * calls operate outside typed borrow guards; never close/revoke/poll an ID
     * that is actively borrowed by a typed resource callback.
     */
    uint64_t handle() const noexcept {return capability_.handle();}
    /** @brief Inspect local lifetime without calling ROS or acquiring rights. */
    bool is_open() const noexcept {return capability_.is_open();}
};
/** @brief Host-selected logger/severity. Each severity grant is a separate alias. */
class Logger {
    Capability capability_;
public:
    /** @brief Open one log.write alias; no stderr/stdout or logger authority is inferred. */
    Logger(roswasmc_context const& context,std::string_view alias):capability_{Capability::checked(context,alias,!context.backend?UWVM_ROS_INVALID:!context.backend->log?UWVM_ROS_UNSUPPORTED:0)} {}
    /** @brief Return alias-open status. */
    uint32_t error() const noexcept {return capability_.error();}
    /** @brief Encode UTF-8 String and write one log record. */
    uint32_t write(std::string_view text) {if(error()) return error();return capability_.log(text);}
    /** @brief Close once, exposing transport errors; ownership retires even on error. */
    uint32_t close() {return capability_.close();}
    /** @brief Borrow the actual opaque instance-owned handle for reviewed raw calls.
     * Never fabricate/transfer it or retain authority after close. A closed
     * facade returns zero; a saved old ID remains stale at the runtime. Raw ABI
     * calls operate outside typed borrow guards; never close/revoke/poll an ID
     * that is actively borrowed by a typed resource callback.
     */
    uint64_t handle() const noexcept {return capability_.handle();}
    /** @brief Inspect local lifetime without calling ROS or acquiring rights. */
    bool is_open() const noexcept {return capability_.is_open();}
};
/** @brief One TF operation. Lookup/broadcast require independent matching frame grants. */
class TF {
    Capability capability_;roswasmc_operation operation_;wire::bytes buffer_;
public:
    /** @brief Open a tf.lookup or tf.broadcast alias with explicit output capacity. */
    TF(roswasmc_context const& context,std::string_view alias,roswasmc_operation operation,size_t capacity)
        :capability_{context,alias,operation,operation==ROSWASMC_TF_LOOKUP||operation==ROSWASMC_TF_BROADCAST,capacity},operation_{operation},buffer_(capacity<=4193786u?capacity:0) {}
    /** @brief Return alias-open status. */
    uint32_t error() const noexcept {return capability_.error();}
    /** @brief Lookup the granted frame pair; time zero selects its latest transform. */
    uint32_t lookup(uint64_t nanoseconds,msg::Transform& transform) {
        if(error()) return error();if(operation_!=ROSWASMC_TF_LOOKUP) return UWVM_ROS_DENIED;
        auto bytes=wire::lookup_time(nanoseconds);auto result=capability_.request(operation_,bytes,buffer_);if(result.error) return result.error;
        if(!result.has_length||result.length>buffer_.size()||!Codec<msg::Transform>::decode({buffer_.data(),static_cast<size_t>(result.length)},transform)) return UWVM_ROS_INVALID;return 0;
    }
    /** @brief Broadcast a complete transform; policy checks frames, finite bounds and quaternion norm. */
    uint32_t broadcast(msg::Transform const& transform) {
        if(error()) return error();if(operation_!=ROSWASMC_TF_BROADCAST) return UWVM_ROS_DENIED;
        wire::bytes bytes;if(!Codec<msg::Transform>::encode(transform,bytes)) return UWVM_ROS_INVALID;return capability_.request(operation_,bytes,buffer_).error;
    }
    /** @brief Close once, exposing transport errors; ownership retires even on error. */
    uint32_t close() {return capability_.close();}
    /** @brief Borrow the actual opaque instance-owned handle for reviewed raw calls.
     * Never fabricate/transfer it or retain authority after close. A closed
     * facade returns zero; a saved old ID remains stale at the runtime. Raw ABI
     * calls operate outside typed borrow guards; never close/revoke/poll an ID
     * that is actively borrowed by a typed resource callback.
     */
    uint64_t handle() const noexcept {return capability_.handle();}
    /** @brief Inspect local lifetime without calling ROS or acquiring rights. */
    bool is_open() const noexcept {return capability_.is_open();}
};
/** @brief Lifecycle GetState result; a label is a canonical UTF-8 String. */
struct LifecycleState {uint8_t id{};std::string label;};
/** @brief One explicitly granted ROS2 lifecycle operation; ROS1 returns95.
 * Change-state has an empty request: the one permitted transition belongs to
 * host policy, so a guest cannot choose an arbitrary transition ID.
 */
class Lifecycle {
    Capability capability_;roswasmc_operation operation_;wire::bytes buffer_;
public:
    /** @brief Open get-state/change-state alias with an explicit response capacity. */
    Lifecycle(roswasmc_context const& context,std::string_view alias,roswasmc_operation operation,size_t capacity)
        :capability_{context,alias,operation,operation==ROSWASMC_LIFECYCLE_GET_STATE||operation==ROSWASMC_LIFECYCLE_CHANGE_STATE,capacity},operation_{operation},buffer_(capacity<=4193786u?capacity:0) {}
    /** @brief Return alias-open status. */
    uint32_t error() const noexcept {return capability_.error();}
    /** @brief Read state and label, preserving the destination on error or malformed wire. */
    uint32_t get_state(LifecycleState& state) {
        if(error()) return error();if(operation_!=ROSWASMC_LIFECYCLE_GET_STATE) return UWVM_ROS_DENIED;
        auto result=capability_.request(operation_,{},buffer_);if(result.error) return result.error;
        if(!result.has_length||result.length<5||result.length>buffer_.size()) return UWVM_ROS_INVALID;
        LifecycleState next;next.id=buffer_[0];if(!wire::decode_string({buffer_.data()+1,static_cast<size_t>(result.length)-1},next.label)) return UWVM_ROS_INVALID;state=std::move(next);return 0;
    }
    /** @brief Apply the one host-granted transition and return its accepted Bool. */
    uint32_t change_state(bool& accepted) {
        if(error()) return error();if(operation_!=ROSWASMC_LIFECYCLE_CHANGE_STATE) return UWVM_ROS_DENIED;
        auto result=capability_.request(operation_,{},buffer_);if(result.error) return result.error;
        if(!result.has_length||result.length>buffer_.size()||!wire::decode_boolean({buffer_.data(),static_cast<size_t>(result.length)},accepted)) return UWVM_ROS_INVALID;return 0;
    }
    /** @brief Close once, exposing transport errors; ownership retires even on error. */
    uint32_t close() {return capability_.close();}
    /** @brief Borrow the actual opaque instance-owned handle for reviewed raw calls.
     * Never fabricate/transfer it or retain authority after close. A closed
     * facade returns zero; a saved old ID remains stale at the runtime. Raw ABI
     * calls operate outside typed borrow guards; never close/revoke/poll an ID
     * that is actively borrowed by a typed resource callback.
     */
    uint64_t handle() const noexcept {return capability_.handle();}
    /** @brief Inspect local lifetime without calling ROS or acquiring rights. */
    bool is_open() const noexcept {return capability_.is_open();}
};
/** @brief Local construction context for typed resources, with an optional diagnostic label.
 * The label is not a native node name. Native endpoint identities are supplied
 * only by host policy. Node must outlive its dependent resources; moving/copying
 * Node is disabled so stored context references cannot silently dangle.
 */
class Node {
    roswasmc_context context_;std::string label_;Context const* lifecycle_{};roswasmc_context stopped_context_{nullptr};
public:
    /** @brief Select one immutable backend explicitly; this opens no aliases. */
    explicit Node(roswasmc_backend const& backend,std::string_view label={}):context_{&backend},label_{label} {}
    /** @brief Associate controlled local shutdown. New factories reject after stop.
     * Existing owners remain explicitly closeable and retain their original
     * stable context. Shutdown grants/revokes no native authority by itself.
     */
    explicit Node(Context const& context,std::string_view label={}):context_{&context.backend()},label_{label},lifecycle_{&context} {}
    Node(Node const&)=delete;Node& operator=(Node const&)=delete;
    Node(Node&&)=delete;Node& operator=(Node&&)=delete;
    /** @brief Return the local diagnostic label (not a native ROS identity). */
    std::string_view label() const noexcept {return label_;}
    /** @brief Borrow this stable context for advanced C/C++ owner construction. */
    roswasmc_context const& context() const noexcept {return lifecycle_&&!lifecycle_->running()?stopped_context_:context_;}
    /** @brief Open one typed publication alias. Topic/type/QoS remain host-selected. */
    template<class Message> Publisher<Message> create_publisher(std::string_view alias) {return {context(),alias};}
    /** @brief Open/activate one typed subscription with explicit event capacity. */
    template<class Message> Subscription<Message> create_subscription(std::string_view alias,size_t capacity) {return {context(),alias,capacity};}
    /** @brief Open/activate one independently granted typed ROS2 parameter-event subscription. */
    template<class Event=msg::TypedParameterEvent> Subscription<Event> create_parameter_events(std::string_view alias,size_t capacity) {return {context(),alias,capacity,true};}
    /** @brief Open one typed service call alias. */
    template<class Service> ServiceClient<Service> create_client(std::string_view alias,size_t capacity) {return {context(),alias,capacity};}
    /** @brief Open separate typed service advertise/respond aliases. */
    template<class Service> ServiceServer<Service> create_service(std::string_view advertise,std::string_view respond,size_t capacity) {return {context(),advertise,respond,capacity};}
    /** @brief Open one explicitly selected Fibonacci client operation. */
    ActionClient create_action_client(std::string_view alias,roswasmc_operation operation,size_t capacity) {return {context(),alias,operation,capacity};}
    /** @brief Explicit four-role typed client, with optional separate cancel-any.
     * No alias is guessed/default-opened; all required roles validate up front.
     */
    ActionClientBundle create_action_client(ActionClientAliases aliases,size_t maximum_goals,size_t maximum_reply_bytes) {
        return {context(),aliases,maximum_goals,maximum_reply_bytes};
    }
    /** @brief Open separate Fibonacci advertise/respond aliases. */
    ActionServer create_action_server(std::string_view advertise,std::string_view respond,size_t capacity) {return {context(),advertise,respond,capacity};}
    /** @brief Open one parameter value/metadata operation alias. */
    Parameters parameters(std::string_view alias,roswasmc_operation operation,size_t capacity) {return {context(),alias,operation,capacity};}
    /** @brief Open one host-filtered graph inspect/list projection. */
    Graph graph(std::string_view alias,roswasmc_operation operation,size_t capacity) {return {context(),alias,operation,capacity};}
    /** @brief Open one separately granted clock. */
    Clock clock(std::string_view alias,roswasmc_operation operation) {return {context(),alias,operation};}
    /** @brief Open one logger/severity grant. */
    Logger logger(std::string_view alias) {return {context(),alias};}
    /** @brief Open one TF lookup/broadcast grant. */
    TF tf(std::string_view alias,roswasmc_operation operation,size_t capacity) {return {context(),alias,operation,capacity};}
    /** @brief Open one ROS2 lifecycle get/change grant. */
    Lifecycle lifecycle(std::string_view alias,roswasmc_operation operation,size_t capacity) {return {context(),alias,operation,capacity};}
};
} // namespace roswasmcpp
