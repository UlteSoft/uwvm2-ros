/** @file @brief Canonical sensor delivery with authentic sealed-buffer and native-sequence evidence. */
#pragma once
#include "sensor_contract.h"
#include "sensor_ingress.h"
namespace uwvm2::imported::ros::sensor_contract
{
    using namespace scene;
    struct request{buffers::profile selected;handle sensor{};};
    inline bool decode(operation action,wire::bytes input,request& output)noexcept
    {
        wire::reader reader{input};request value;
        if(action==operation::open){if(!proposals::read_profile(reader,value.selected))return false;}
        else if(action==operation::take||action==operation::describe){if(!reader.u64(value.sensor)||!value.sensor)return false;}
        else return false;
        if(!reader.done())return false;
        output=value;return true;
    }
    /** @brief A generic delivery followed by its actual buffer/schema/frame/native provenance.
     * @details metadata.sequence is the local capture ordinal, not a fabricated
     * native sequence. Native sequence availability is separately explicit.
     * The source timestamp stays absent when unavailable; receive time always
     * names its actual host clock/epoch. No publisher identity is fabricated.
     */
    inline bool encode_take(wire::writer& out,handle sensor,sensors::delivery const& frame,handle buffer,buffers::profile const& buffer_profile)noexcept
    {
        auto const& m=frame.metadata;auto const& native=m.source;
        return out.u64(sensor)&&out.u64(native.generation)&&out.u64(m.capture_sequence)&&out.boolean(native.stamped)&&
            (!native.stamped||(out.u64(native.clock)&&out.u64(native.epoch)&&out.i64(native.source_ns)))&&out.boolean(true)&&
            out.u64(m.received_clock)&&out.u64(m.received_epoch)&&out.i64(static_cast<std::int64_t>(m.captured_steady_ns))&&out.boolean(false)&&out.u64(m.loss_count)&&
            out.u64(0)&&out.text("sensor.frame")&&out.text(frame.type)&&out.text(frame.encoding)&&out.blob(frame.body)&&out.u64(buffer)&&
            proposals::write_profile(out,frame.schema)&&proposals::write_profile(out,buffer_profile)&&out.text(frame.frame)&&out.text(frame.units)&&
            out.u64(native.source)&&out.boolean(native.sequence!=0)&&(!native.sequence||out.u64(native.sequence));
    }
    inline std::size_t maximum_take_size(sensors::source::configuration const& policy)noexcept
    {
        sensors::delivery frame;frame.schema=policy.ingress.schema;frame.type=policy.ingress.type.view();frame.encoding=policy.ingress.encoding.view();
        frame.frame=policy.ingress.frame.view();frame.units=policy.ingress.units.view();frame.metadata.source.stamped=policy.ingress.source_stamp;
        frame.metadata.source.sequence=policy.ingress.source_sequence?1:0;
        auto out=wire::writer::measuring(maximum_reply_bytes);
        return encode_take(out,1,frame,1,policy.buffer_profile)&&policy.ingress.body_bytes<=maximum_reply_bytes-out.size()?out.size()+policy.ingress.body_bytes:0;
    }
    inline bool encode_state(wire::writer& out,sensors::source::configuration const& policy,sensors::usage const& value,sensors::ingress::usage const& native)noexcept
    {
        return proposals::write_profile(out,policy.ingress.schema)&&proposals::write_profile(out,policy.buffer_profile)&&out.text(policy.ingress.type.view())&&
            out.text(policy.ingress.encoding.view())&&out.text(policy.ingress.frame.view())&&out.text(policy.ingress.units.view())&&
            out.boolean(policy.ingress.source_sequence)&&out.boolean(policy.ingress.source_stamp)&&out.u64(policy.ingress.body_bytes)&&out.u64(policy.ingress.records)&&
            out.u64(policy.history)&&out.u64(value.handles)&&out.u64(value.records)&&out.u64(value.payload_bytes)&&out.u64(value.captures)&&out.u64(value.losses)&&
            out.u64(value.full_refusals)&&out.u64(value.invalid_frames)&&out.boolean(value.retired)&&out.boolean(value.counters_saturated)&&
            out.u64(native.reads)&&out.u64(native.bytes)&&out.u64(native.frames)&&out.u64(native.pending_bytes)&&out.boolean(native.active)&&out.boolean(native.retired);
    }
    inline bool encode_description(wire::writer& out,sensors::source::configuration const& policy,sensors::usage const& value,sensors::ingress::usage const& native)noexcept
    {
        std::array<std::uint8_t,1536> body;wire::writer writer{body};if(!encode_state(writer,policy,value,native))return false;
        return proposals::write_profile(out,policy.ingress.profile)&&out.text("ros.sensor.state")&&out.text("le64")&&out.blob(writer.written());
    }
}
