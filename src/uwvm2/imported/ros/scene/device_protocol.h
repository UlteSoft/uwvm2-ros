/** @file
 * @brief Closed scalar-command/stop/acknowledgement frames for a bound device peer.
 * @details Frames contain no path, descriptor, native opcode, address or selectable
 * route. Distinct prebound channels carry commands, stops and acknowledgements.
 * SHA256 detects complete-frame corruption; it is not peer authentication.
 * Deployment must authenticate/isolate the actual peer. The receiver installs
 * controller leases through a separate trusted management path, never from a
 * command. Host steady deadlines require a verified shared clock domain; a
 * remote network adapter must establish its own qualified clock mapping.
 *
 * A failed/truncated stream is not scanned for another magic value. The decoder
 * latches malformed input and requires a new trusted session. Partial reception
 * never invokes a device. All buffers and parser work have hard finite bounds.
 */
#pragma once
#include "device_core.h"
namespace uwvm2::imported::ros::scene::devices::protocol
{
    inline constexpr std::uint64_t magic=UINT64_C(0x4543495645445255); // URDEVICE
    inline constexpr std::size_t header_fields=88,header_bytes=128,maximum_payload=9000,maximum_frame=header_bytes+maximum_payload;
    enum class kind:std::uint8_t{command=1,stop=2,acknowledgement=3,controller=4};
    enum class acknowledgement:std::uint8_t{rejected=1,executed=2,stop_accepted=3,stopped=4,controller_installed=5};
    struct header
    {
        kind type{};handle provider{},sequence{},request{},controller{},controller_epoch{},authority_epoch{};
        std::uint64_t lease_expires{},deadline{},body_bytes{};buffers::digest digest{};
    };
    struct packet{std::array<std::uint8_t,maximum_frame> data;std::size_t size{};bytes view()const noexcept{return {data.data(),size};}};
    inline bool fields_valid(header const& value)noexcept
    {
        if(static_cast<unsigned>(value.type)<1||static_cast<unsigned>(value.type)>4||!value.provider||!value.sequence||!value.request||
           !value.body_bytes||value.body_bytes>maximum_payload)return false;
        if(value.type==kind::command||value.type==kind::controller)return value.controller&&value.controller_epoch&&value.authority_epoch&&value.lease_expires&&
            value.lease_expires<=INT64_MAX&&value.deadline&&value.deadline<=value.lease_expires;
        return !value.controller&&!value.controller_epoch&&!value.authority_epoch&&!value.lease_expires&&!value.deadline;
    }
    inline bool write_fields(wire::writer& out,header const& value)noexcept
    {return out.u64(magic)&&out.u64(static_cast<std::uint64_t>(value.type))&&out.u64(value.provider)&&out.u64(value.sequence)&&out.u64(value.request)&&
        out.u64(value.controller)&&out.u64(value.controller_epoch)&&out.u64(value.authority_epoch)&&out.u64(value.lease_expires)&&out.u64(value.deadline)&&out.u64(value.body_bytes);}
    inline buffers::digest content_hash(bytes fields,bytes body)noexcept
    {
        ::fast_io::sha256_context context{};buffers::digest digest{};
        auto first=reinterpret_cast<std::byte const*>(fields.data());context.update(first,first+fields.size());
        first=reinterpret_cast<std::byte const*>(body.data());if(!body.empty())context.update(first,first+body.size());
        context.do_final();context.digest_to_byte_ptr(reinterpret_cast<std::byte*>(digest.data()));return digest;
    }
    /** @brief Serialize only a complete bounded payload and its full framing digest. */
    inline error encode(header value,bytes body,packet& output)noexcept
    {
        value.body_bytes=body.size();if(!fields_valid(value))return error::invalid;
        auto source=reinterpret_cast<std::uintptr_t>(body.data()),target=reinterpret_cast<std::uintptr_t>(output.data.data());
        if((source>=target&&source-target<output.data.size())||(target>source&&target-source<body.size()))return error::invalid;
        std::array<std::uint8_t,header_fields> fields;wire::writer prefix{fields};if(!write_fields(prefix,value))return error::invalid;
        value.digest=content_hash(fields,body);wire::writer out{output.data};
        if(!write_fields(out,value)||!out.blob(value.digest))return error::invalid;
        std::copy(body.begin(),body.end(),output.data.begin()+header_bytes);output.size=header_bytes+body.size();return error::success;
    }
    inline error decode_header(bytes input,kind expected,header& output)noexcept
    {
        if(input.size()!=header_bytes)return error::invalid;
        wire::reader reader{input};header value;std::uint64_t tag{},type{};bytes digest;
        if(!reader.u64(tag)||tag!=magic||!reader.u64(type)||type!=static_cast<std::uint64_t>(expected)||!reader.u64(value.provider)||
           !reader.u64(value.sequence)||!reader.u64(value.request)||!reader.u64(value.controller)||!reader.u64(value.controller_epoch)||
           !reader.u64(value.authority_epoch)||!reader.u64(value.lease_expires)||!reader.u64(value.deadline)||!reader.u64(value.body_bytes)||
           !reader.blob(digest)||digest.size()!=value.digest.size()||!reader.done())return error::invalid;
        value.type=expected;if(!fields_valid(value))return error::invalid;std::copy(digest.begin(),digest.end(),value.digest.begin());output=value;return error::success;
    }
    inline error decode(bytes input,kind expected,header& output,bytes& body)noexcept
    {
        if(input.size()<header_bytes||input.size()>maximum_frame)return error::invalid;
        header value;auto result=decode_header(input.first(header_bytes),expected,value);if(result!=error::success)return result;
        if(value.body_bytes!=input.size()-header_bytes)return error::invalid;
        auto payload=input.subspan(header_bytes);if(content_hash(input.first(header_fields),payload)!=value.digest)return error::invalid;
        output=value;body=payload;return error::success;
    }
    inline bool profile_write(wire::writer& out,buffers::profile const& value)noexcept
    {return valid_text(value.identity)&&out.text(value.identity.view())&&out.blob(value.sha256);}
    inline bool profile_read(wire::reader& input,buffers::profile const& selected)noexcept
    {std::string_view name;bytes hash;return input.text(name)&&name==selected.identity.view()&&input.blob(hash)&&hash.size()==selected.sha256.size()&&std::equal(hash.begin(),hash.end(),selected.sha256.begin());}
    inline error command(work const& value,scalar_schema const& policy,handle sequence,packet& output)noexcept
    {
        if(value.controller.provider_generation!=value.result.provider_generation)return error::invalid;
        std::array<std::uint8_t,maximum_payload> body;wire::writer writer{body};
        if(!profile_write(writer,policy.profile)||!devices::encode(writer,policy,value.command))return error::invalid;
        auto const& lease=value.controller;header h{kind::command,value.result.provider_generation,sequence,value.result.request,lease.id,lease.epoch,lease.owner.authority_epoch,lease.expires,value.deadline};
        return encode(h,writer.written(),output);
    }
    inline error stop(receipt const& value,buffers::profile const& selected,handle sequence,packet& output)noexcept
    {
        std::array<std::uint8_t,256> body;wire::writer writer{body};if(!profile_write(writer,selected))return error::invalid;
        return encode({kind::stop,value.provider_generation,sequence,value.request},writer.written(),output);
    }
    /** @brief Install a host-issued lease over the separately admitted controller lane.
     * @details This frame carries no physical command. The bound receiver first
     * checks its immutable schema/stop profiles, provider incarnation, strictly
     * increasing controller sequence and lease epoch. Transport provenance must
     * be established by trusted deployment; the framing hash is not a signature.
     */
    inline error controller(control::lease const& value,scalar_schema const& schema,buffers::profile const& stop,
                            handle sequence,packet& output)noexcept
    {
        if(!value.owner||!value.id||!value.epoch||!valid(schema)||value.priority>UINT32_MAX)return error::invalid;
        std::array<std::uint8_t,512> storage;wire::writer out{storage};
        if(!out.u64(value.owner.environment)||!out.u64(value.owner.grant)||!out.u64(value.owner.generation)||!out.u64(value.priority)||
            !profile_write(out,schema.profile)||!profile_write(out,stop))return error::invalid;
        return encode({kind::controller,value.provider_generation,sequence,value.id,value.id,value.epoch,value.owner.authority_epoch,value.expires,value.expires},out.written(),output);
    }
    inline bool valid_ack(kind original,acknowledgement value)noexcept
    {
        if(value==acknowledgement::rejected)return original==kind::command||original==kind::stop||original==kind::controller;
        if(original==kind::command)return value==acknowledgement::executed;
        if(original==kind::stop)return value==acknowledgement::stop_accepted||value==acknowledgement::stopped;
        return original==kind::controller&&value==acknowledgement::controller_installed;
    }
    inline error acknowledge(header const& received,acknowledgement outcome,handle sequence,packet& output)noexcept
    {
        if(!fields_valid(received)||!valid_ack(received.type,outcome))return error::invalid;
        std::array<std::uint8_t,56> body;wire::writer writer{body};writer.u64(static_cast<std::uint64_t>(received.type));writer.u64(static_cast<std::uint64_t>(outcome));writer.blob(received.digest);
        return encode({kind::acknowledgement,received.provider,sequence,received.request},writer.written(),output);
    }
    /** @brief Match the exact original frame, not merely a recyclable request number. */
    inline error verify_ack(bytes input,header const& original,handle expected_sequence,acknowledgement& output)noexcept
    {
        if(!fields_valid(original)||original.type==kind::acknowledgement)return error::invalid;
        header value;bytes body;auto status=decode(input,kind::acknowledgement,value,body);if(status!=error::success)return status;
        if(value.provider!=original.provider||value.request!=original.request||value.sequence!=expected_sequence)return error::conflict;
        wire::reader reader{body};std::uint64_t type{},outcome{};bytes digest;
        if(!reader.u64(type)||type!=static_cast<std::uint64_t>(original.type)||!reader.u64(outcome)||outcome<1||outcome>5||!reader.blob(digest)||
            digest.size()!=original.digest.size()||!std::equal(digest.begin(),digest.end(),original.digest.begin())||!reader.done())return error::invalid;
        if(!valid_ack(original.type,static_cast<acknowledgement>(outcome)))return error::invalid;
        output=static_cast<acknowledgement>(outcome);return error::success;
    }
    /** @brief One complete frame per reservation; malformed framing permanently poisons it. */
    class stream_decoder
    {
        kind expected_;packet retained_;std::size_t wanted_{header_bytes};bool invalid_{},complete_{};
    public:
        explicit stream_decoder(kind expected):expected_(expected){}
        error feed(bytes input,std::size_t& consumed)noexcept
        {
            consumed=0;if(invalid_)return error::invalid;if(complete_)return error::again;
            while(!input.empty()&&!complete_)
            {
                auto count=std::min(input.size(),wanted_-retained_.size);std::copy_n(input.begin(),count,retained_.data.begin()+static_cast<std::ptrdiff_t>(retained_.size));
                retained_.size+=count;consumed+=count;input=input.subspan(count);
                if(retained_.size==header_bytes&&wanted_==header_bytes)
                {
                    header value;if(decode_header(retained_.view(),expected_,value)!=error::success){invalid_=true;return error::invalid;}
                    wanted_=header_bytes+static_cast<std::size_t>(value.body_bytes);
                }
                if(retained_.size==wanted_)
                {header value;bytes body;if(decode(retained_.view(),expected_,value,body)!=error::success){invalid_=true;return error::invalid;}complete_=true;}
            }
            return complete_?error::success:error::again;
        }
        std::size_t remaining()const noexcept{return invalid_||complete_?0:wanted_-retained_.size;}
        bytes frame()const noexcept{return complete_?retained_.view():bytes{};}
        error next()noexcept{if(invalid_)return error::invalid;if(!complete_)return error::again;retained_.size=0;wanted_=header_bytes;complete_=false;return error::success;}
    };
    /** @brief Receiver-side content/fencing gate; trusted lease installation is separate.
     * @details The native adapter serializes this gate with the actual effect and
     * records ACKs only from that effect. Successful validation is not execution.
     * Stops latch before any subsequent command; installing a controller does
     * not clear that latch. Independent stopped-state rearm is host-only.
     * A scalar command's absolute deadline is also its maximum active lifetime.
     * The independent watchdog must service expire() at that deadline even
     * when the controller lease lasts longer. Queued trajectories requiring a
     * different completion contract cannot be treated as this scalar carrier.
     */
    class receiver
    {
        scalar_decoder schema_;buffers::profile stop_;handle provider_{},command_sequence_{},stop_sequence_{},controller_sequence_{},controller_epoch_{},stopped_evidence_{};
        control::lease controller_;std::uint64_t command_deadline_{};bool configured_{},latched_{true};
        bool command_expired(std::uint64_t now)const noexcept
        {return command_deadline_&&(!now||now>=command_deadline_);}
        bool effect_expired(std::uint64_t now)const noexcept
        {return command_expired(now)||(controller_.id&&(!now||now>=controller_.expires));}
    public:
        error configure(handle provider,scalar_schema const& schema,buffers::profile const& stop)noexcept
        {
            if(configured_)return error::denied;
            if(!provider||!valid(schema)||!valid_text(stop.identity))return error::invalid;
            auto status=schema_.configure(schema);if(status!=error::success)return status;
            provider_=provider;stop_=stop;configured_=true;return error::success;
        }
        error install_controller(control::lease const& lease)noexcept
        {
            if(!configured_||!lease.id||!lease.epoch||!lease.owner||lease.provider_generation!=provider_||!lease.expires||lease.expires>INT64_MAX||
                lease.epoch<=controller_epoch_)return error::denied;
            controller_=lease;controller_epoch_=lease.epoch;return error::success;
        }
        /** @brief Accept an exact framed controller lease before accepting commands.
         * @details This method must run in the native adapter under its physical
         * watchdog/effect lock. The same independent timer expires the installed
         * lease even if the VM is paused, dies, or cannot send a stop message.
         */
        error controller(bytes frame,std::uint64_t now,header& output)noexcept
        {
            if(!configured_||latched_||effect_expired(now))return error::denied;
            header value;bytes body;auto status=decode(frame,kind::controller,value,body);if(status!=error::success)return status;
            if(value.provider!=provider_||controller_sequence_==UINT64_MAX||value.sequence!=controller_sequence_+1||value.request!=value.controller||
                value.deadline!=value.lease_expires||!now||now>=value.lease_expires)return error::denied;
            wire::reader in{body};control::lease lease{value.controller,value.controller_epoch,value.provider,{},0,value.lease_expires};
            lease.owner.authority_epoch=value.authority_epoch;
            if(!in.u64(lease.owner.environment)||!in.u64(lease.owner.grant)||!in.u64(lease.owner.generation)||!in.u64(lease.priority)||
                lease.priority>UINT32_MAX||!profile_read(in,schema_.profile())||!profile_read(in,stop_)||!in.done())return error::invalid;
            status=install_controller(lease);if(status!=error::success)return status;
            controller_sequence_=value.sequence;output=value;return error::success;
        }
        /** @brief Fence an expired scalar command or controller lease before the fixed physical stop.
         * @details Return true once when either the last accepted command or
         * the receiver lease expires. Controller replacement preserves the
         * active command deadline; rejected translations never extend it. The
         * native adapter must perform its fixed stop action under the same lock;
         * this portable gate alone is not a running timer or a stopped witness.
         */
        bool expire(std::uint64_t now)noexcept
        {
            if(!configured_||latched_||!controller_.id)return false;
            if(now&&now<controller_.expires&&!command_expired(now))return false;
            latched_=true;controller_={};command_deadline_=0;return true;
        }
        /** @brief Only a trusted observer may establish stopped state before activation. */
        error rearm_after_stopped(handle provider,handle observer_evidence)noexcept
        {if(!configured_||provider!=provider_||!observer_evidence||observer_evidence<=stopped_evidence_)return error::denied;stopped_evidence_=observer_evidence;controller_={};command_deadline_=0;latched_=false;return error::success;}
        /** @brief Validate closed platform semantics before committing the common receive sequence.
         * @details The trusted translator is synchronous, noexcept, nonreentrant
         * and side-effect-free; it may only prepare inert output. It receives a
         * fully bounded scalar vector after controller/lease/profile checks.
         * Its refusal changes no receiver state. Successful translation is not
         * execution; the caller still serializes the physical effect/watchdog.
         * No whole receiver copy or temporary second authority state is needed.
         */
        template<typename Translate>error command_translate(bytes frame,std::uint64_t now,header& output,scalar_command& command,Translate&& translate)noexcept
        {
            static_assert(noexcept(translate(std::declval<scalar_command const&>())));
            // Refusal does not consume the watchdog event. expire() must still
            // return true so the adapter emits its reserved fixed stop request.
            if(!configured_||latched_||effect_expired(now))return error::denied;
            header value;bytes body;auto status=decode(frame,kind::command,value,body);if(status!=error::success)return status;
            if(value.provider!=provider_||command_sequence_==UINT64_MAX||value.sequence!=command_sequence_+1)return error::conflict;
            if(!now||value.controller!=controller_.id||value.controller_epoch!=controller_.epoch||value.authority_epoch!=controller_.owner.authority_epoch||
                value.lease_expires!=controller_.expires||now>=value.lease_expires||now>=value.deadline)return error::denied;
            wire::reader reader{body};if(!profile_read(reader,schema_.profile()))return error::denied;
            scalar_command decoded;status=schema_.decode(body.subspan(reader.position()),decoded);if(status!=error::success)return status;
            status=translate(std::as_const(decoded));if(status!=error::success)return status;
            command_sequence_=value.sequence;command_deadline_=value.deadline;output=value;command=decoded;return error::success;
        }
        error command(bytes frame,std::uint64_t now,header& output,scalar_command& command)noexcept
        {return command_translate(frame,now,output,command,[](scalar_command const&)noexcept{return error::success;});}
        error stop(bytes frame,header& output)noexcept
        {
            if(!configured_)return error::denied;
            header value;bytes body;auto status=decode(frame,kind::stop,value,body);if(status!=error::success)return status;
            if(value.provider!=provider_||stop_sequence_==UINT64_MAX||value.sequence!=stop_sequence_+1)return error::conflict;
            wire::reader reader{body};if(!profile_read(reader,stop_)||!reader.done())return error::denied;
            stop_sequence_=value.sequence;latched_=true;controller_={};command_deadline_=0;output=value;return error::success;
        }
        bool latched()const noexcept{return latched_;}
    };
}
