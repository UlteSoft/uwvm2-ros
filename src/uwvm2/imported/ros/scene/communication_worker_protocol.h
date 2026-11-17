/** @file
 * @brief Fixed, bounded supervisor/communication-worker RPC and exact retry ledger.
 * @details The private stdin/stdout channel is separate from the four RTPS
 * directions. Requests select only a sealed manifest binding and a closed
 * operation. No path, socket, native pointer or caller-supplied authority appears
 * in the envelope. Each generation has one sequential outstanding request. The
 * next sequence explicitly acknowledges the preceding reply. Lost replies can
 * be recovered by retrying the complete identical request; changed retries,
 * skipped sequences and generation mismatch fence the channel before effects.
 * Death after any request bytes were sent remains an unknown native outcome.
 */
#pragma once
#include "communication.h"
#include "wire.h"
#include <uwvm2/utils/container/standard.h>
namespace uwvm2::imported::ros::scene::communication_worker_protocol
{
    using error=communication::error;using effect=communication::effect;
    enum class operation:std::uint64_t
    {
        context_inspect=1,node_inspect,topic_open,topic_publish,topic_take,topic_qos,topic_liveliness,topic_acknowledged,
        service_open,service_submit,service_respond,service_take,service_available,service_qos,service_abandon,
        action_open,action_submit,action_respond,action_take,action_cancel,action_describe,graph_capture,resource_close,context_shutdown,
        topic_reserve,topic_acknowledge,topic_release,service_reserve,service_acknowledge,service_release
    };
    inline constexpr std::size_t header_bytes=48;
    inline constexpr std::uint64_t request_magic=0x514552534f525755ull,reply_magic=0x505245534f525755ull;
    struct request{std::uint64_t generation{},sequence{},binding{};operation action{};wire::bytes body;};
    struct result{error status{error::io};effect observed{effect::none};std::size_t bytes{};};
    struct response{std::uint64_t generation{},sequence{};error status{};effect observed{};wire::bytes body;};
    inline bool valid(error value) noexcept
    {
        switch(value){case error::ok:case error::io:case error::bad_handle:case error::again:case error::no_memory:case error::denied:case error::busy:
            case error::invalid:case error::too_large:case error::unsupported:case error::timeout:case error::stale:case error::cancelled:return true;}
        return false;
    }
    inline bool request_header(std::span<std::uint8_t> output,std::uint64_t generation,std::uint64_t sequence,operation action,std::uint64_t binding,std::uint64_t bytes) noexcept
    {
        if(output.size()!=header_bytes||!generation||!sequence||!binding||action<operation::context_inspect||action>operation::service_release)return false;
        wire::writer w{output};return w.u64(request_magic)&&w.u64(generation)&&w.u64(sequence)&&w.u64(static_cast<std::uint64_t>(action))&&w.u64(binding)&&w.u64(bytes);
    }
    inline bool request_extent(wire::bytes header,std::uint64_t generation,std::uint64_t maximum,std::uint64_t& extent) noexcept
    {
        if(header.size()!=header_bytes)return false;
        wire::reader r{header};std::uint64_t magic{},actual{},sequence{},action{},binding{},bytes{};
        if(!r.u64(magic)||magic!=request_magic||!r.u64(actual)||actual!=generation||!r.u64(sequence)||!sequence||!r.u64(action)||
           action<static_cast<std::uint64_t>(operation::context_inspect)||action>static_cast<std::uint64_t>(operation::service_release)||
           !r.u64(binding)||!binding||!r.u64(bytes)||bytes>maximum||!r.done())return false;
        extent=bytes;return true;
    }
    inline bool decode_request(wire::bytes bytes,std::uint64_t generation,std::uint64_t maximum,request& output) noexcept
    {
        if(bytes.size()<header_bytes)return false;
        std::uint64_t extent{};
        if(!request_extent(bytes.first(header_bytes),generation,maximum,extent)||extent!=bytes.size()-header_bytes)return false;
        wire::reader r{bytes.first(header_bytes)};std::uint64_t ignored{},action{};request value;
        if(!r.u64(ignored)||!r.u64(value.generation)||!r.u64(value.sequence)||!r.u64(action)||!r.u64(value.binding)||!r.u64(ignored))return false;
        value.action=static_cast<operation>(action);value.body=bytes.subspan(header_bytes);output=value;return true;
    }
    inline bool response_extent(wire::bytes header,std::uint64_t generation,std::uint64_t sequence,std::uint64_t maximum,std::uint64_t& extent) noexcept
    {
        if(header.size()!=header_bytes)return false;
        wire::reader r{header};std::uint64_t magic{},actual{},serial{},status{},observed{},bytes{};
        if(!r.u64(magic)||magic!=reply_magic||!r.u64(actual)||actual!=generation||!r.u64(serial)||serial!=sequence||
           !r.u64(status)||status>UINT32_MAX||!valid(static_cast<error>(status))||!r.u64(observed)||observed>static_cast<std::uint64_t>(effect::unknown)||
           !r.u64(bytes)||bytes>maximum||!r.done())return false;
        extent=bytes;return true;
    }
    inline bool decode_response(wire::bytes bytes,std::uint64_t generation,std::uint64_t sequence,std::uint64_t maximum,response& output) noexcept
    {
        if(bytes.size()<header_bytes)return false;
        std::uint64_t extent{};
        if(!response_extent(bytes.first(header_bytes),generation,sequence,maximum,extent)||extent!=bytes.size()-header_bytes)return false;
        wire::reader r{bytes.first(header_bytes)};std::uint64_t ignored{},status{},observed{};response value;
        if(!r.u64(ignored)||!r.u64(value.generation)||!r.u64(value.sequence)||!r.u64(status)||!r.u64(observed)||!r.u64(ignored))return false;
        value.status=static_cast<error>(status);value.observed=static_cast<effect>(observed);value.body=bytes.subspan(header_bytes);output=value;return true;
    }
    /** @brief Single-threaded preallocated exact-retry storage for the worker serve loop.
     * @details Handler output is copied into retained owned storage before the
     * serve loop writes anything. No backing allocation occurs in execute().
     * A handler must preflight its complete output before effects, and return
     * actual observed effect, never infer remote completion from pipe success.
     * An exception/malformed handler result is retained as unknown and fences
     * fresh requests; only an exact replay can recover that receipt.
     */
    class ledger
    {
        ::uwvm2::utils::container::std_vector<std::uint8_t> previous,reply;
        std::size_t request_size{},reply_size{},max_request{},max_response{};
        std::uint64_t generation{},sequence{};bool configured{},fenced{};
    public:
        bool prepare(std::uint64_t incarnation,std::size_t request,std::size_t response) noexcept
        {
            if(configured||!incarnation||!request||request>4u*1024u*1024u||!response||response>4u*1024u*1024u)return false;
            try{previous.resize(header_bytes+request);reply.resize(header_bytes+response);generation=incarnation;max_request=request;max_response=response;configured=true;return true;}
            catch(...){return false;}
        }
        bool failed() const noexcept{return fenced;}
        template<typename Handler> bool execute(wire::bytes input,Handler&& handler,wire::bytes& output) noexcept
        {
            if(!configured)return false;
            request call;if(!decode_request(input,generation,max_request,call)){fenced=true;return false;}
            if(sequence&&call.sequence==sequence)
            {
                if(input.size()!=request_size||!std::equal(input.begin(),input.end(),previous.begin())){fenced=true;return false;}
                output={reply.data(),reply_size};return true;
            }
            if(fenced||sequence==UINT64_MAX||call.sequence!=sequence+1){fenced=true;return false;}
            // Copy the complete owned retry identity before native effects.
            std::memcpy(previous.data(),input.data(),input.size());request_size=input.size();
            result answer;
            try{answer=handler(call,std::span<std::uint8_t>{reply.data()+header_bytes,max_response});}
            catch(...){answer={error::io,effect::unknown,0};fenced=true;}
            if(!valid(answer.status)||answer.observed>effect::unknown||answer.bytes>max_response)
            {answer={error::io,effect::unknown,0};fenced=true;}
            sequence=call.sequence;wire::writer w{{reply.data(),header_bytes}};
            if(!w.u64(reply_magic)||!w.u64(generation)||!w.u64(sequence)||!w.u64(static_cast<std::uint64_t>(answer.status))||
               !w.u64(static_cast<std::uint64_t>(answer.observed))||!w.u64(answer.bytes)){fenced=true;return false;}
            reply_size=header_bytes+answer.bytes;output={reply.data(),reply_size};return true;
        }
    };
}
