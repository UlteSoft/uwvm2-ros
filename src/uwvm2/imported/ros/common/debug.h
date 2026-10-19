/*************************************************************
 * UWVM2 bounded, read-only ROS debugger DATA. APL-2.0.     *
 *************************************************************/
#pragma once
#include <array>
#include <atomic>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <mutex>
#include <string_view>

// This header has no ROS client-library, Runtime, native IPC or Guest import
// dependency. ROS-disabled debugger builds retain a null observation hook.
// These detached values are diagnostics, never capability/session credentials.
namespace uwvm2::imported::ros::debug
{
    // Even fully escaped bounded text plus every u64 counter fits the existing
    // 65536-byte authenticated console reply cap at these page sizes.
    inline constexpr std::size_t maximum_rows{8}, maximum_trace_rows{16}, trace_capacity{256},maximum_scan{4096};
    enum class selection:std::uint8_t {capabilities,sessions,queues,services,actions,parameters,tf,graph,statistics,calls,trace};
    enum class status:std::uint8_t {ok,invalid_request,not_selected,busy,requires_cooperative_stop,stale_stop,resource_limit};
    enum class row_kind:std::uint8_t {capability,session,queue};
    template<std::size_t Capacity> struct text
    {
        std::array<char,Capacity> bytes{};std::size_t size{};bool truncated{};
        std::string_view view() const noexcept {return {bytes.data(),size};}
        void assign(std::string_view value) noexcept
        {
            size=value.size()<Capacity?value.size():Capacity;truncated=value.size()>Capacity;
            for(std::size_t i{};i<size;++i) {bytes[i]=value[i];}
        }
    };
    struct request
    {
        selection selected{selection::capabilities};std::uint8_t backend{};
        std::uint64_t instance{},first{},count{maximum_rows},after{},stop{};
        std::uint32_t error{};bool error_filter{};text<255> alias{},module{};
        text<64> function{},operation{};
    };
    inline bool valid(request const& query) noexcept
    {
        return query.selected<=selection::trace&&query.backend<=2&&query.alias.size<=query.alias.bytes.size()&&
            !query.alias.truncated&&query.module.size<=query.module.bytes.size()&&!query.module.truncated&&
            query.function.size<=query.function.bytes.size()&&!query.function.truncated&&
            query.operation.size<=query.operation.bytes.size()&&!query.operation.truncated&&
            (query.selected==selection::trace||query.selected==selection::calls||(!query.function.size&&!query.operation.size))&&
            query.count&&query.count<=(query.selected==selection::trace?maximum_trace_rows:maximum_rows)&&
            query.first<=maximum_scan&&(!query.error_filter||query.selected==selection::trace)&&
            (!query.after||query.selected==selection::trace)&&
            (query.selected!=selection::trace||query.first==0);
    }
    inline bool family(selection selected,std::string_view operation) noexcept
    {
        auto starts=[&](std::string_view prefix) {return operation.substr(0,prefix.size())==prefix;};
        if(selected==selection::services) {return starts("service.");}
        if(selected==selection::actions) {return starts("action.");}
        if(selected==selection::parameters) {return starts("param.");}
        if(selected==selection::tf) {return starts("tf.");}
        if(selected==selection::graph) {return starts("graph.");}
        return true;
    }
    struct row
    {
        row_kind kind{};std::uint8_t backend{};std::uint64_t instance{},generation{};
        text<255> module{};text<255> alias{};text<64> operation{};text<256> resource{};text<192> type{};
        bool enabled{},alive{},revoked{},connected{},retired{},resource_redacted{};
        std::uint64_t open_count{},active_handles{},observations{},requests{},delivered{},timeout_ms{};
        // Stored values only: snapshotting never refreshes or consumes tokens.
        double operation_tokens{},creation_tokens{},byte_tokens{};
        std::uint64_t audit_records{},audit_bytes{},audit_suppressed{};
        std::uint64_t depth{},bytes{},pending_depth{},pending_bytes{},reserved_depth{},reserved_bytes{};
        std::uint64_t high_water_depth{},high_water_bytes{},enqueued{},dequeued{},dropped_oldest{},dropped_newest{};
        std::uint64_t rejected{},backpressured{},admission_refused{},completed{},closed{};
        // Detached optional-client metadata only; no raw/native session token.
        std::uint64_t client_bindings{},client_session_label{};
    };
    // Aggregates describe Host import latency, including waits inside the
    // gateway. They are not native ROS executor/callback execution timings.
    // A missing normal return is never an observed successful result.
    struct call_result
    {
        std::uint32_t error{};bool known{};
        std::uint32_t finish(std::uint32_t value) noexcept {error=value;known=true;return value;}
    };
    struct interface_record
    {
        std::uint8_t backend{};std::uint64_t instance{},generation{};
        text<255> module{},alias{};text<64> operation{},function{};
        std::uint64_t calls{},errors{},incomplete{},timed{},total_ns{},min_ns{},max_ns{},window_start_ns{},window_ns{};
        bool unresolved{},saturated{};
    };
    struct trace_record
    {
        std::uint64_t sequence{},monotonic_ns{},instance{},generation{};std::uint8_t backend{};std::uint32_t error{};
        text<255> module{};text<255> alias{};text<64> operation{},function{};
        bool unresolved{};
        text<255> source_alias{};std::uint64_t client_session_label{},client_features{};
        bool client_features_observed{};
        std::uint64_t started_ns{},duration_ns{};bool duration_known{};
        bool result_known{true}; // Direct denial/ABI records already have an observed result.
    };
    // Five bounded text columns plus fixed labels/counters remain within the
    // existing authenticated broker cap, including hex escaping/truncation.
    static_assert((4*(3*255+2*64)+5*13+256+8*20)*maximum_trace_rows+512<=65536);
    struct view
    {
        status result{status::not_selected};selection selected{};
        std::array<row,maximum_rows> rows{};std::size_t size{};
        std::array<trace_record,maximum_trace_rows> trace{};std::size_t trace_size{};
        std::uint64_t total{},next{},oldest{},newest{},overwritten{},dropped_busy{};
        bool more{},gap{};
        std::array<interface_record,maximum_rows> interfaces{};std::size_t interface_size{};
        std::uint64_t dropped_full{},dropped_labels{};
    };
    inline bool matches(request const& query,std::uint8_t backend,std::uint64_t instance,
        std::string_view alias,std::string_view operation,std::string_view module) noexcept
    {
        return (!query.backend||query.backend==backend)&&(!query.instance||query.instance==instance)&&
            (query.alias.size==0||query.alias.view()==alias)&&(query.module.size==0||query.module.view()==module)&&family(query.selected,operation);
    }
    inline bool public_resource(std::string_view value) noexcept
    {
        // Only ROS name metadata is eligible. Never copy connection fields,
        // security roots/URIs, log text or parameter values into diagnostics.
        if(value.empty()||value.front()!='/') {return false;}
        for(char c:value)
        {if(!((c>='a'&&c<='z')||(c>='A'&&c<='Z')||(c>='0'&&c<='9')||c=='_'||c=='/')) {return false;}}
        return true;
    }
    inline void add(request const& query,view& output,row const& value) noexcept
    {
        auto position{output.total++};
        if(position<query.first) {return;}
        if(output.size<query.count&&output.size<output.rows.size()) {output.rows[output.size++]=value;output.next=position+1;}
        else {output.more=true;}
    }
    using snapshot_function=void(*)(request const&,view&) noexcept;
    inline std::atomic<snapshot_function> snapshot_hook{};
    inline void snapshot(request const& query,view& output) noexcept
    {
        output={};output.selected=query.selected;
        if(!valid(query)) {output.result=status::invalid_request;return;}
        auto callback{snapshot_hook.load(std::memory_order_acquire)};
        if(callback) {callback(query,output);}
    }
    inline std::uint64_t monotonic_nanoseconds() noexcept
    {
        auto n{std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count()};
        return n>0?static_cast<std::uint64_t>(n):0;
    }
    class interface_store
    {
        static constexpr std::size_t capacity{128};
        std::mutex mutex_{};std::array<interface_record,capacity> rows_{};std::size_t size_{};
        std::atomic<std::uint64_t> busy_{},full_{},labels_{};
        static void increment(std::atomic<std::uint64_t>& counter) noexcept
        {auto n{counter.load(std::memory_order_relaxed)};while(n!=UINT64_MAX&&!counter.compare_exchange_weak(n,n+1,std::memory_order_relaxed)) {}}
        static void add_saturated(std::uint64_t& counter,std::uint64_t n,bool& saturated) noexcept
        {if(n>UINT64_MAX-counter) {counter=UINT64_MAX;saturated=true;}else {counter+=n;}}
    public:
        void append(trace_record const& call) noexcept
        {
            // Truncated labels cannot establish an exact interface identity.
            if(call.backend<1||call.backend>2||call.module.truncated||call.alias.truncated||
               call.operation.truncated||call.function.truncated||call.module.size>call.module.bytes.size()||
               call.alias.size>call.alias.bytes.size()||call.operation.size>call.operation.bytes.size()||
               call.function.size>call.function.bytes.size()) {increment(labels_);return;}
            std::unique_lock lock{mutex_,std::try_to_lock};if(!lock.owns_lock()) {increment(busy_);return;}
            std::size_t slot{};
            for(;slot<size_;++slot)
            {
                auto const& row{rows_[slot]};
                if(row.backend==call.backend&&row.instance==call.instance&&row.generation==call.generation&&
                   row.unresolved==call.unresolved&&row.module.view()==call.module.view()&&row.alias.view()==call.alias.view()&&
                   row.operation.view()==call.operation.view()&&row.function.view()==call.function.view()) {break;}
            }
            if(slot==size_)
            {
                if(size_==capacity) {increment(full_);return;}
                auto& row{rows_[size_++]};row.backend=call.backend;row.instance=call.instance;row.generation=call.generation;
                row.module=call.module;row.alias=call.alias;row.operation=call.operation;row.function=call.function;row.unresolved=call.unresolved;
                row.window_start_ns=call.duration_known&&call.started_ns<=call.monotonic_ns?call.started_ns:call.monotonic_ns;
            }
            auto& row{rows_[slot]};add_saturated(row.calls,1,row.saturated);
            if(!call.result_known) {add_saturated(row.incomplete,1,row.saturated);}
            else if(call.error) {add_saturated(row.errors,1,row.saturated);}
            if(call.duration_known&&call.started_ns<=call.monotonic_ns&&call.duration_ns<=call.monotonic_ns-call.started_ns)
            {
                if(!row.timed||call.duration_ns<row.min_ns) {row.min_ns=call.duration_ns;}
                if(call.duration_ns>row.max_ns) {row.max_ns=call.duration_ns;}
                add_saturated(row.timed,1,row.saturated);add_saturated(row.total_ns,call.duration_ns,row.saturated);
            }
        }
        void read(request const& query,view& output) noexcept
        {
            output={};output.selected=query.selected;
            if(!valid(query)||query.selected!=selection::calls) {output.result=status::invalid_request;return;}
            std::unique_lock lock{mutex_,std::try_to_lock};if(!lock.owns_lock()) {output.result=status::busy;return;}
            output.result=status::ok;auto const sample{monotonic_nanoseconds()};
            output.dropped_busy=busy_.load(std::memory_order_relaxed);output.dropped_full=full_.load(std::memory_order_relaxed);
            output.dropped_labels=labels_.load(std::memory_order_relaxed);
            for(std::size_t i{};i<size_;++i)
            {
                auto const& row{rows_[i]};
                if(!matches(query,row.backend,row.instance,row.alias.view(),row.operation.view(),row.module.view())||
                   (query.function.size&&query.function.view()!=row.function.view())||
                   (query.operation.size&&query.operation.view()!=row.operation.view())) {continue;}
                auto position{output.total++};if(position<query.first) {continue;}
                if(output.interface_size<query.count)
                {
                    auto& copy{output.interfaces[output.interface_size++]};copy=row;
                    copy.window_ns=sample>=row.window_start_ns?sample-row.window_start_ns:0;output.next=position+1;
                }
                else {output.more=true;}
            }
        }
    };
    class trace_store
    {
        std::mutex mutex_{};std::array<trace_record,trace_capacity> rows_{};
        std::size_t begin_{},size_{};std::uint64_t sequence_{},overwritten_{};
        std::atomic<std::uint64_t> dropped_{};interface_store interfaces_{};
        void dropped() noexcept
        {
            auto value{dropped_.load(std::memory_order_relaxed)};
            while(value!=UINT64_MAX&&!dropped_.compare_exchange_weak(value,value+1,std::memory_order_relaxed)) {}
        }
    public:
        // No audit draining, native call, payload copy or blocking wait. The
        // original host call is unaffected when diagnostic storage is busy.
        void append(trace_record record) noexcept
        {
            record.monotonic_ns=monotonic_nanoseconds();interfaces_.append(record);
            std::unique_lock<std::mutex> lock{mutex_,std::try_to_lock};
            if(!lock.owns_lock()||sequence_==UINT64_MAX) {dropped();return;}
            record.sequence=++sequence_;
            if(size_==trace_capacity) {begin_=(begin_+1)%trace_capacity;if(overwritten_!=UINT64_MAX) {++overwritten_;}}
            else {++size_;}
            rows_[(begin_+size_-1)%trace_capacity]=record;
        }
        void read_calls(request const& query,view& output) noexcept {interfaces_.read(query,output);}
        void read(request const& query,view& output) noexcept
        {
            output={};output.selected=query.selected;
            if(!valid(query)||query.selected!=selection::trace) {output.result=status::invalid_request;return;}
            std::unique_lock<std::mutex> lock{mutex_,std::try_to_lock};
            if(!lock.owns_lock()) {output.result=status::busy;return;}
            output.result=status::ok;output.overwritten=overwritten_;output.dropped_busy=dropped_.load(std::memory_order_relaxed);
            output.oldest=size_?rows_[begin_].sequence:0;output.newest=sequence_;
            output.gap=query.after&&output.oldest&&query.after<output.oldest-1;
            for(std::size_t i{};i<size_;++i)
            {
                auto const& value{rows_[(begin_+i)%trace_capacity]};
                if(value.sequence<=query.after||(query.module.size&&value.module.truncated)||
                   (query.function.size&&(value.function.truncated||query.function.view()!=value.function.view()))||
                   (query.operation.size&&(value.operation.truncated||query.operation.view()!=value.operation.view()))||
                   !matches(query,value.backend,value.instance,value.alias.view(),value.operation.view(),value.module.view())||
                   (query.error_filter&&(!value.result_known||query.error!=value.error))) {continue;}
                ++output.total;
                if(output.trace_size<query.count) {output.trace[output.trace_size++]=value;output.next=value.sequence;}
                else {output.more=true;}
            }
        }
    };
}
