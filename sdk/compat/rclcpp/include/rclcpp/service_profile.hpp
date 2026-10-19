/** Finite AddTwoInts server and REAL unified-event asynchronous client.
 * No synchronous call is disguised as asynchronous completion. Every returned
 * request ID originates from service_submit, and only an exact bound event can
 * complete it. Finite service readiness can use a separate explicitly selected
 * native observation table/alias. Missing selection still refuses95; a graph
 * name or successful request admission never supplies readiness. Infinite waits
 * and the rest of native wait-set compatibility remain separate requirements.
 */
#pragma once
#include <example_interfaces/srv/add_two_ints.hpp>
#include <array>
#include <cinttypes>
#include <optional>
#include <cmath>
#include <roswasm/service_client.hpp>
// No native GUID/sequence number is serialized by this Guest ABI. The selected
// official server ignores this parameter. The callback receives a NULL shared
// pointer; dereferencing/inspecting a native request header is unsupported.
struct rmw_request_id_t;
namespace rclcpp {
namespace detail {
using AddTwoInts=::example_interfaces::srv::AddTwoInts;
inline roswasm_rclcpp::ServiceBinding const& service(Core& core,std::string_view name,roswasm_rclcpp::ServiceRole role){
    running();core.prepare_factory();auto profile=state().selection->services;if(!profile)fail(13);
    if(!profile->events||profile->events->version!=2||profile->bindings.size()>32)fail(22);
    roswasm_rclcpp::ServiceBinding const* found{};
    for(auto const& row:profile->bindings)if(row.node==core.node&&row.service==name&&row.role==role){if(found)fail(22);found=&row;}
    if(!found)fail(13);auto const& r=*found;
    if(r.type!="example_interfaces/srv/AddTwoInts"||r.qos_profile!="official-default-service-profile")fail(95);
    if(!text(r.node,128)||!text(r.service,255)||!r.maximum_pending||r.maximum_pending>16)fail(22);
    if(role==roswasm_rclcpp::ServiceRole::client){
        if(!text(r.call_alias,64)||!text(r.observer_alias,64)||r.call_alias==r.observer_alias||!r.advertise_alias.empty()||!r.respond_alias.empty())fail(22);
        if(!profile->events->submit||!profile->events->watch||!profile->events->poll)fail(95);
        if(!r.availability_alias.empty()){
            if(!text(r.availability_alias,64)||r.availability_alias==r.call_alias||r.availability_alias==r.observer_alias)fail(22);
            auto query=profile->clients;
            if(!query||!query->abi_version||!query->features||!query->query)fail(95);
            if(query->version!=2)fail(22);
        }
    }else if(role==roswasm_rclcpp::ServiceRole::server){
        if(!text(r.advertise_alias,64)||!text(r.respond_alias,64)||r.advertise_alias==r.respond_alias||!r.call_alias.empty()||!r.observer_alias.empty()||!r.availability_alias.empty())fail(22);
    }else fail(22);
    return r;
}
struct ServiceLease {
    bool& local;std::vector<std::shared_ptr<Entity>> retained;std::array<std::shared_ptr<Core>,16> retained_cores{};
    explicit ServiceLease(bool& value):local{value}{
        if(value||state().service_dispatching)fail(17);
        // Fixed total registry bound16 Nodes x64 facades. Allocate and retain
        // before consuming an event or entering a user callback. Releasing a
        // final application shared_ptr in that callback cannot run a capability
        // destructor until this complete group lease has ended.
        retained.reserve(16*64);
        std::size_t core_count{};
        for(auto const& weak:state().service_cores)if(auto core=weak.lock()){
            retained_cores[core_count++]=core; // Also pin Nodes owning only a Clock/Logger.
            for(auto const& entity:core->entities)if(auto owner=entity.lock())retained.push_back(std::move(owner));
        }
        local=true;state().service_dispatching=true;
    }
    ~ServiceLease(){state().service_dispatching=false;local=false;}
    // Members retire after the destructor body restores the group state.
};
inline void service_log(std::shared_ptr<Core>const& core,std::string const& rendered){
    if(!core||core->closed)fail(9);running();if(rendered.size()>4095)fail(90);
    if(!core->logger){if(state().service_dispatching)fail(17);if(!text(core->binding.info_logger_alias,64))fail(13);core->logger=std::make_unique<roswasmcpp::Logger>(core->context(),core->binding.info_logger_alias);checked(core->logger->error());}
    checked(core->logger->write(rendered));
}
}
inline void Logger::info(char const* format,std::int64_t a,std::int64_t b)const{
    if(!format||std::string_view{format}!="request: %" PRId64 " + %" PRId64)detail::fail(95);
    detail::service_log(core_.lock(),::fast_io::concat_std("request: ",a," + ",b));
}
inline void Logger::info(char const* format,std::int64_t a,std::int64_t b,std::int64_t c)const{
    if(!format||std::string_view{format}!="result of %" PRId64 " + %" PRId64 " = %" PRId64)detail::fail(95);
    detail::service_log(core_.lock(),::fast_io::concat_std("result of ",a," + ",b," = ",c));
}
inline void Logger::info(char const* literal)const{
    if(!literal||std::string_view{literal}!="waiting for service to appear...")detail::fail(95);
    detail::service_log(core_.lock(),literal);
}
inline void Logger::error(char const*)const{
    // INFO aliases cannot silently acquire ERROR severity. This unsupported
    // original-client failure path remains a required separate logger profile.
    detail::fail(95);
}
template<class S> class Service:public detail::Entity {
    static_assert(std::is_same_v<S,detail::AddTwoInts>,"Only complete acquired AddTwoInts is registered by this source profile");
    using Handler=std::function<typename S::Response(typename S::Request const&)>;
    using Callback=std::function<void(std::shared_ptr<rmw_request_id_t>,std::shared_ptr<typename S::Request>,std::shared_ptr<typename S::Response>)>;
    std::shared_ptr<detail::Core> core_;Callback callback_;std::shared_ptr<Handler> prepared_;
    std::shared_ptr<typename S::Request> request_;std::shared_ptr<typename S::Response> response_;
    std::optional<roswasmcpp::ServiceServer<S>> server_;std::array<std::uint32_t,2> cleanup_statuses_{};roswasmc_cleanup_report cleanup_report_{};bool busy_{},closed_{};
public:
    using SharedPtr=std::shared_ptr<Service>;
    Service(std::shared_ptr<detail::Core> core,Callback callback):core_{std::move(core)},callback_{std::move(callback)},
        prepared_{std::make_shared<Handler>([this](typename S::Request const& input){
            if((input.b>0&&input.a>std::numeric_limits<std::int64_t>::max()-input.b)||
               (input.b<0&&input.a<std::numeric_limits<std::int64_t>::min()-input.b))detail::fail(90);
            *request_=input;*response_={};callback_({},request_,response_);return *response_;})}{if(!core_||!callback_)detail::fail(22);}
private:
    friend class Node;
    void _activate(roswasm_rclcpp::ServiceBinding const& row){
        // The exact official callback logs. Acquire that independent owner
        // before any callback is entered; no lazy factory runs under its lease.
        if(!core_->logger){if(!detail::text(core_->binding.info_logger_alias,64))detail::fail(13);
            auto logger=std::make_unique<roswasmcpp::Logger>(core_->context(),core_->binding.info_logger_alias);
            detail::checked(logger->error());core_->logger=std::move(logger);}
        server_.emplace(core_->context(),row.advertise_alias,row.respond_alias,24);
        if(server_->error()){detail::cleanup(server_->rollback_status().cleanup_error);detail::fail(server_->error());}
        server_->on_request_shared(prepared_); // All callable/registry storage was allocated before opening.
    }
public:
    std::uint32_t poll(std::uint64_t)override{
        if(closed_||core_->closed||!server_)return 9;if(busy_||detail::state().service_dispatching)return 17;
        // Allocate fresh callback owners BEFORE consuming the next actual event.
        // Retained callback pointers therefore never alias a later request.
        auto request=std::make_shared<typename S::Request>();auto response=std::make_shared<typename S::Response>();
        request_=std::move(request);response_=std::move(response);detail::ServiceLease lease{busy_};
        return server_->poll_once();
    }
    std::uint32_t close()noexcept override{
        if(busy_||detail::state().service_dispatching)return 17;if(closed_)return 0;closed_=true;
        if(!server_)return 0;cleanup_report_=server_->close_checked(cleanup_statuses_.data());return cleanup_report_.cleanup_error;
    }
    roswasmc_cleanup_report cleanup_report()const noexcept{return cleanup_report_;}
    std::array<std::uint32_t,2> const& cleanup_statuses()const noexcept{return cleanup_statuses_;} // advertise, respond
    ~Service(){detail::cleanup(close());}
};
/** Completed result owner. Request IDs are opaque full-width Guest IDs, never
 * represented as native rmw_request_id_t or narrowed to a signed sequence.
 * get() never introduces a hidden blocking wait: call spin_until_future_complete
 * first. Pending get explicitly refuses11; failure get throws the actual code.
 * This finite surface is not a general std::shared_future implementation.
 */
template<class S> struct ServiceCompletion {
    std::shared_ptr<typename S::Response> response=std::make_shared<typename S::Response>();
    std::weak_ptr<detail::Core> owner;std::uint64_t request_id{};std::uint32_t error{};std::uint8_t event_flags{};bool ready{},abandoned{};
};
template<class S> class ServiceFuture {
    std::shared_ptr<ServiceCompletion<S>> data_;std::uint8_t retained_flags_{};
    template<class>friend class Client;
public:
    using ServiceType=S;
    ServiceFuture()=default;
    ServiceFuture(ServiceFuture const&)=delete;ServiceFuture& operator=(ServiceFuture const&)=delete;
    ServiceFuture(ServiceFuture&&)=default;ServiceFuture& operator=(ServiceFuture&&)=default;
private:
    explicit ServiceFuture(std::shared_ptr<ServiceCompletion<S>> data):data_{std::move(data)}{}
public:
    bool valid()const noexcept{return bool(data_);}
    bool _belongs(std::shared_ptr<detail::Core>const& owner)const noexcept{return data_&&data_->owner.lock()==owner;}bool _ready()const noexcept{return data_&&data_->ready;}
    std::uint8_t completion_flags()const noexcept{return data_?data_->event_flags:retained_flags_;}
    std::uint64_t request_id()const noexcept{return data_?data_->request_id:0;}
    std::shared_ptr<typename S::Response> get(){if(!data_)detail::fail(22);if(!data_->ready)detail::fail(11);auto state=std::move(data_);retained_flags_=state->event_flags;detail::checked(state->error);return std::move(state->response);}
};
template<class S> class Client:public detail::Entity {
    static_assert(std::is_same_v<S,detail::AddTwoInts>);
    std::shared_ptr<detail::Core> core_;std::optional<roswasmcpp::ServiceClient<S>> client_;
    std::optional<roswasmcpp::Capability> observer_;std::optional<roswasmcpp::EventStream> stream_;
    std::optional<roswasmcpp::ServiceAvailability> availability_;
    mutable bool query_busy_{};std::uint32_t availability_cleanup_status_{};
    bool observe_service(std::uint64_t wait_ns) const {
        detail::running();if(closed_||core_->closed||!client_)detail::fail(9);
        if(source_retired_)detail::fail(source_error_);
        if(busy_||query_busy_||detail::state().service_dispatching)detail::fail(17);
        if(!availability_)detail::fail(95);
        // Retain all group facades and pin lifecycle before invoking even a
        // local test transport. No native callback can enter Wasm, and a
        // reentrant local transport cannot close/regrant a borrowed owner.
        detail::ServiceLease lease{query_busy_};bool ready{};
        detail::checked(availability_->query(wait_ns,ready));return ready;
    }
    std::array<std::shared_ptr<ServiceCompletion<S>>,16> pending_{};std::size_t maximum_{};
    std::array<unsigned char,72> bytes_{};std::array<std::uint32_t,2> cleanup_statuses_{};roswasmc_cleanup_report cleanup_report_{};bool busy_{},closed_{},source_retired_{};std::uint32_t source_error_{};
    void fail_pending(std::uint32_t code)noexcept{for(auto& p:pending_)if(p){if(!p->ready){p->error=code;p->ready=true;}p.reset();}}
public:
    using SharedPtr=std::shared_ptr<Client>;using Future=ServiceFuture<S>;
    struct FutureAndRequestId {
        using ServiceType=S;Future future;std::uint64_t request_id{};
        FutureAndRequestId(Future value,std::uint64_t id):future{std::move(value)},request_id{id}{}
        FutureAndRequestId(FutureAndRequestId const&)=delete;FutureAndRequestId& operator=(FutureAndRequestId const&)=delete;
        FutureAndRequestId(FutureAndRequestId&&)=default;FutureAndRequestId& operator=(FutureAndRequestId&&)=default;
        bool valid()const noexcept{return future.valid();}bool _ready()const noexcept{return future._ready();}
        bool _belongs(std::shared_ptr<detail::Core>const& owner)const noexcept{return future._belongs(owner);}
        std::shared_ptr<typename S::Response> get(){return future.get();}
    };
    explicit Client(std::shared_ptr<detail::Core> core):core_{std::move(core)}{if(!core_)detail::fail(22);}
private:
    friend class Node;
    void _activate(roswasm_rclcpp::ServiceBinding const& row){
        maximum_=row.maximum_pending;client_.emplace(core_->context(),row.call_alias,8);detail::checked(client_->error());
        observer_.emplace(core_->context(),row.observer_alias);detail::checked(observer_->error());
        stream_.emplace(*observer_,client_->borrowed_event_source(),*detail::state().selection->services->events);detail::checked(stream_->error());
        if(!row.availability_alias.empty()){
            availability_.emplace(core_->context(),*detail::state().selection->services->clients,
                client_->borrowed_event_source(),row.availability_alias);
            detail::checked(availability_->error());
        }
    }
public:
    /** One real metadata-only observation; no business request is submitted. */
    bool service_is_ready() const {return observe_service(0);}
    /** Physical finite wait through the optional Host/native query. Conversion
     * rejects negative, nonfinite and overflowing durations before an import;
     * positive fractional nanoseconds follow ordinary duration_cast truncation.
     * The explicit Host grant may impose a smaller limit and retains its errno.
     */
    template<class Rep,class Period> bool wait_for_service(std::chrono::duration<Rep,Period> duration) const {
        auto ns=std::chrono::duration<long double,std::nano>{duration}.count();
        if(!std::isfinite(ns)||ns<0||ns>60000000000.0L)detail::fail(22);
        return observe_service(static_cast<std::uint64_t>(ns));
    }
    /** Infinite default waits require a separately reviewed shutdown/wait-set
     * contract. They are never clamped to a successful finite observation. */
    bool wait_for_service() const {detail::fail(95);}
    FutureAndRequestId async_send_request(std::shared_ptr<typename S::Request> const& request){
        detail::running();if(closed_||core_->closed||!stream_)detail::fail(9);if(source_retired_)detail::fail(source_error_);if(busy_||detail::state().service_dispatching)detail::fail(17);if(!request)detail::fail(22);
        std::size_t slot=maximum_;for(std::size_t i{};i!=maximum_;++i)if(!pending_[i]){slot=i;break;}if(slot==maximum_)detail::fail(90);
        // Allocate complete state, response and encoded request before submit.
        auto state=std::make_shared<ServiceCompletion<S>>();state->owner=core_;auto bytes=roswasmcpp::ServiceCodec<S>::encode(*request);
        Future future{state};pending_[slot]=state;std::uint64_t id{};auto code=stream_->submit(bytes,id);
        if(code){pending_[slot].reset();state->error=code;state->ready=true;detail::fail(code);}
        for(std::size_t i{};i!=maximum_;++i)if(i!=slot&&pending_[i]&&pending_[i]->request_id==id){fail_pending(22);detail::fail(22);}
        state->request_id=id;return {std::move(future),id};
    }
    bool remove_pending_request(FutureAndRequestId const& future){
        detail::running();if(closed_||core_->closed)detail::fail(9);if(busy_||detail::state().service_dispatching)detail::fail(17);
        if(!future.future.data_||future.request_id!=future.future.data_->request_id||!future._belongs(core_))detail::fail(22);
        for(auto& p:pending_)if(p&&p==future.future.data_&&!p->abandoned){p->abandoned=true;p->error=125;p->ready=true;return true;}return false;
    }
    std::uint32_t poll(std::uint64_t)override{
        if(closed_||core_->closed||!stream_)return 9;if(busy_||detail::state().service_dispatching)return 17;detail::ServiceLease lease{busy_};
        auto event=stream_->poll(bytes_);if(event.result.error){if(event.result.error!=11)fail_pending(event.result.error);return event.result.error;}
        auto const& h=event.header;
        if(h.kind==ROSWASMC_RESOURCE_CLOSED||(h.kind==ROSWASMC_RESOURCE_ERROR&&(!h.correlation||(h.flags&1u)))){
            if(h.kind==ROSWASMC_RESOURCE_CLOSED||(h.flags&1u)){source_retired_=true;source_error_=h.error?h.error:9;}
            for(auto& state:pending_)if(state)state->event_flags=h.flags;
            fail_pending(h.error?h.error:9);return 0;
        }
        if(h.kind!=ROSWASMC_SERVICE_RESPONSE&&h.kind!=ROSWASMC_RESOURCE_ERROR){fail_pending(22);return 22;}
        for(auto& state:pending_)if(state&&state->request_id==h.correlation){
            // Local abandonment does not weaken the canonical response wire.
            // Validate the complete value before retiring its tombstone. A bad
            // late response fails22 while the already-local125 stays unchanged.
            typename S::Response next{};
            if(h.kind==ROSWASMC_SERVICE_RESPONSE&&!roswasmcpp::ServiceCodec<S>::decode_response({bytes_.data()+64,static_cast<std::size_t>(h.payload_bytes)},next)){fail_pending(22);return 22;}
            state->event_flags=h.flags;
            if(!state->abandoned&&h.kind==ROSWASMC_RESOURCE_ERROR){state->error=h.error;state->ready=true;}
            else if(!state->abandoned){*state->response=next;state->ready=true;}
            state.reset();return 0;
        }
        fail_pending(22);return 22; // Unknown/duplicate response cannot complete another request.
    }
    std::uint32_t close()noexcept override{
        if(busy_||detail::state().service_dispatching)return 17;if(closed_)return 0;closed_=true;fail_pending(125);cleanup_report_=roswasmc_cleanup_init(0);
        auto record=[&](std::size_t index,std::uint32_t code){cleanup_statuses_[index]=code;++cleanup_report_.attempted;if(code){++cleanup_report_.failed;if(!cleanup_report_.cleanup_error)cleanup_report_.cleanup_error=code;}};
        // The optional query was acquired last and retires first. Preserve the
        // original observer/source status slots for existing callers; its own
        // status is separate while the aggregate counts every real attempt.
        if(availability_&&availability_->is_open()){
            auto code=availability_->close();availability_cleanup_status_=code;++cleanup_report_.attempted;
            if(code){++cleanup_report_.failed;if(!cleanup_report_.cleanup_error)cleanup_report_.cleanup_error=code;}
        }
        if(observer_&&observer_->is_open())record(0,observer_->close());
        if(client_&&client_->is_open())record(1,client_->close());return cleanup_report_.cleanup_error;
    }
    roswasmc_cleanup_report cleanup_report()const noexcept{return cleanup_report_;}
    std::array<std::uint32_t,2> const& cleanup_statuses()const noexcept{return cleanup_statuses_;} // observer, call source
    /** Actual optional-query close result, separate from the two legacy slots. */
    std::uint32_t availability_cleanup_status() const noexcept{return availability_cleanup_status_;}
    ~Client(){detail::cleanup(close());}
};
template<class S,class Callback>std::shared_ptr<Service<S>> Node::create_service(std::string_view name,Callback&& callback){
    auto const& row=detail::service(*core_,name,roswasm_rclcpp::ServiceRole::server);
    using Function=std::function<void(std::shared_ptr<rmw_request_id_t>,std::shared_ptr<typename S::Request>,std::shared_ptr<typename S::Response>)>;
    auto value=std::make_shared<Service<S>>(core_,Function{std::forward<Callback>(callback)});
    // Registry capacity and strong facade/callable ownership already exist.
    value->_activate(row);core_->add(value);return value;
}
template<class S>std::shared_ptr<Client<S>> Node::create_client(std::string_view name){
    auto const& row=detail::service(*core_,name,roswasm_rclcpp::ServiceRole::client);
    auto value=std::make_shared<Client<S>>(core_);value->_activate(row);core_->add(value);return value;
}
enum class FutureReturnCode {SUCCESS,INTERRUPTED,TIMEOUT};
template<class NodeT,class Future> requires requires {typename Future::ServiceType;}
FutureReturnCode spin_until_future_complete(std::shared_ptr<NodeT>const& node,Future const& future){
    static_assert(std::is_base_of_v<Node,NodeT>);if(!node||!future.valid())detail::fail(22);auto retained=node;auto core=retained->_service_core();auto& s=detail::state();
    detail::running();if(!future._belongs(core))detail::fail(22);if(s.spinning||s.service_dispatching||core->dispatching||core->closed)detail::fail(17);
    struct Lease{bool& flag;std::shared_ptr<detail::Core> core;~Lease(){flag=false;core->dispatching=false;}}lease{s.spinning,core};s.spinning=true;
    for(std::size_t pass{};pass!=s.selection->maximum_spin_steps;++pass){
        if(future._ready())return FutureReturnCode::SUCCESS;if(!ok())return FutureReturnCode::INTERRUPTED;
        auto step=s.selection->step(s.selection->step_context);if(step==125){shutdown();return FutureReturnCode::INTERRUPTED;}detail::checked(step);if(!ok())return FutureReturnCode::INTERRUPTED;
        std::vector<std::shared_ptr<detail::Entity>> owners;owners.reserve(core->entities.size());for(auto const& weak:core->entities)if(auto p=weak.lock())owners.push_back(std::move(p));
        auto now=core->clock?core->now():0;
        core->dispatching=true;for(auto const& owner:owners){auto code=owner->poll(now);if(code!=11)detail::checked(code);if(!ok())break;}core->dispatching=false;
    }
    if(future._ready())return FutureReturnCode::SUCCESS;s.last_error=110;return FutureReturnCode::TIMEOUT;
}
}
