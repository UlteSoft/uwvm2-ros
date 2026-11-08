// Licensed under APL-2.0 (see LICENSE).
// https://github.com/WebAssembly/tool-conventions/blob/main/Coredump.md
#pragma once
#ifndef UWVM_MODULE
# include <array>
# include <bit>
# include <cstddef>
# include <cstdint>
# include <limits>
# include <span>
# include <string_view>
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif

UWVM_MODULE_EXPORT namespace uwvm2::runtime::diagnostic::wasm_coredump
{
    enum class error : unsigned char
    {
        none, invalid_index, invalid_range, invalid_utf8, invalid_value,
        format_overflow, inconsistent_capture, read_failed, write_failed,
        capture_denied, stale_snapshot, capture_busy, allocation_failed, unsupported_snapshot
    };
    enum class capture : unsigned char { complete, partial, unavailable };
    enum class value_kind : unsigned char { missing=1, i32=0x7f, i64=0x7e, f32=0x7d, f64=0x7c };
    struct value { value_kind kind{value_kind::missing}; ::std::uint64_t bits{}; };
    struct region { ::std::uint64_t offset{}, bytes{}; };
    struct tag_sample { ::std::uint32_t tag{}; bool available{}; };
    // Callbacks borrow only the supplied guest range. Contexts are private to the
    // collector and are NEVER serialized; no API accepts a native address to dump.
    using write_function=bool (*)(void*,::std::span<::std::byte const>) noexcept;
    using read_function=bool (*)(void*,::std::uint64_t,::std::span<::std::byte>) noexcept;
    using read_tags_function=bool (*)(void*,::std::uint64_t,::std::span<tag_sample>) noexcept;
    struct tag_view
    {
        bool enabled{};
        ::std::uint8_t bits{}, granule_log2{4};
        capture state{capture::unavailable};
        ::std::span<region const> regions{}; // offsets/counts are GRANULE indices.
        void* context{};
        read_tags_function read{};
    };
    struct memory_view
    {
        bool memory64{}, shared{};
        ::std::uint64_t bytes{}; // committed byte length, a multiple of 64 KiB.
        capture state{capture::unavailable};
        ::std::span<region const> regions{}; // sorted, non-overlapping guest ranges.
        void* context{};
        read_function read{};
        tag_view tags{};
    };
    struct global_view
    {
        // Unsupported reference/vector globals are explicit missing i32 slots.
        // original_type records their real Wasm type in the metadata extension.
        value data{};
        ::std::uint8_t original_type{};
    };
    struct module_view { ::std::string_view name{}; ::std::uint64_t functions{}; };
    struct instance_view
    {
        ::std::uint64_t module{};
        // Repeated dump indices preserve aliases without copying an owner again.
        ::std::span<::std::uint32_t const> memories{}, globals{};
    };
    struct frame_view
    {
        ::std::uint64_t instance{}, function{}, code_offset{};
        bool code_offset_available{};
        ::std::span<value const> locals{}, stack{};
    };
    struct thread_view
    {
        ::std::string_view name{};
        capture state{capture::unavailable};
        ::std::span<frame_view const> frames{}; // youngest first, per convention.
    };
    struct dump_view
    {
        ::std::string_view executable{}, reason{};
        bool coherent{}; // collector attests all owners/threads were stopped.
        ::std::span<module_view const> modules{};
        ::std::span<instance_view const> instances{};
        ::std::span<memory_view const> memories{};
        ::std::span<global_view const> globals{};
        ::std::span<thread_view const> threads{};
        ::std::string_view diagnostic_metadata{}; // Explicit redaction manifest; no authority.
    };
    struct result
    {
        error status{};
        ::std::uint64_t bytes{};
        bool memory_chunks_required{};
        [[nodiscard]] constexpr explicit operator bool() const noexcept { return status==error::none; }
    };

    namespace details
    {
        inline constexpr ::std::uint64_t section_max{UINT32_MAX};
        [[nodiscard]] inline constexpr bool utf8(::std::string_view text) noexcept
        {
            for(::std::size_t i{};i<text.size();)
            {
                auto c=static_cast<unsigned char>(text[i++]);
                if(c<0x80u)continue;
                unsigned n{},point{},minimum{};
                if(c>=0xc2u&&c<=0xdfu){n=1;point=c&31u;minimum=0x80u;}
                else if(c>=0xe0u&&c<=0xefu){n=2;point=c&15u;minimum=0x800u;}
                else if(c>=0xf0u&&c<=0xf4u){n=3;point=c&7u;minimum=0x10000u;}
                else return false;
                if(n>text.size()-i)return false;
                for(unsigned j{};j<n;++j){auto d=static_cast<unsigned char>(text[i++]);if((d&0xc0u)!=0x80u)return false;point=(point<<6u)|(d&63u);}
                if(point<minimum||point>0x10ffffu||(point>=0xd800u&&point<=0xdfffu))return false;
            }
            return true;
        }
        [[nodiscard]] inline constexpr bool valid_capture(capture c) noexcept
        { return c==capture::complete||c==capture::partial||c==capture::unavailable; }
        [[nodiscard]] inline constexpr bool valid_value(value const& v) noexcept
        {
            switch(v.kind)
            {
                case value_kind::missing:return v.bits==0u;
                case value_kind::i32:case value_kind::f32:return v.bits<=UINT32_MAX;
                case value_kind::i64:case value_kind::f64:return true;
                default:return false;
            }
        }
        [[nodiscard]] inline constexpr error ranges(::std::span<region const> spans,::std::uint64_t bound,capture state) noexcept
        {
            if(!valid_capture(state))return error::inconsistent_capture;
            if(state==capture::unavailable&&!spans.empty())return error::inconsistent_capture;
            ::std::uint64_t end{};
            for(auto const& part:spans)
            {
                if(part.offset<end||part.offset>bound||part.bytes>bound-part.offset)return error::invalid_range;
                if(state==capture::complete&&part.offset!=end)return error::inconsistent_capture;
                end=part.offset+part.bytes;
            }
            if(state==capture::complete&&end!=bound)return error::inconsistent_capture;
            return error::none;
        }
        [[nodiscard]] inline error validate(dump_view const& dump) noexcept
        {
            if(dump.modules.size()>UINT32_MAX||dump.instances.size()>UINT32_MAX||dump.memories.size()>UINT32_MAX||
                dump.globals.size()>UINT32_MAX||dump.threads.size()>UINT32_MAX)return error::format_overflow;
            auto name=[](::std::string_view s) noexcept {return s.size()<=UINT32_MAX&&utf8(s);};
            if(!name(dump.executable)||!name(dump.reason)||!name(dump.diagnostic_metadata))return error::invalid_utf8;
            for(auto const& m:dump.modules)
            {if(!name(m.name))return error::invalid_utf8;if(m.functions>UINT32_MAX)return error::format_overflow;}
            for(auto const& i:dump.instances)
            {
                if(i.module>=dump.modules.size())return error::invalid_index;
                if(i.memories.size()>UINT32_MAX||i.globals.size()>UINT32_MAX)return error::format_overflow;
                for(auto v:i.memories)if(v>=dump.memories.size())return error::invalid_index;
                for(auto v:i.globals)if(v>=dump.globals.size())return error::invalid_index;
            }
            for(auto const& g:dump.globals)if(!valid_value(g.data))return error::invalid_value;
            for(auto const& m:dump.memories)
            {
                if((m.bytes&65535u)!=0u||(!m.memory64&&m.bytes>(::std::uint64_t{1}<<32u)))return error::invalid_range;
                if(auto e=ranges(m.regions,m.bytes,m.state);e!=error::none)return e;
                for(auto r:m.regions)
                {
                    if(!m.memory64&&r.offset>UINT32_MAX)return error::invalid_range;
                    if(r.bytes&&m.read==nullptr)return error::read_failed;
                }
                auto const& tags=m.tags;
                if(!tags.enabled)
                {if(!tags.regions.empty()||tags.state!=capture::unavailable)return error::inconsistent_capture;continue;}
                if(tags.bits==0u||tags.bits>32u||tags.granule_log2>63u)return error::invalid_value;
                auto count=m.bytes>>tags.granule_log2;
                if((m.bytes&((::std::uint64_t{1}<<tags.granule_log2)-1u))!=0u)++count;
                if(auto e=ranges(tags.regions,count,tags.state);e!=error::none)return e;
                for(auto r:tags.regions)if(r.bytes&&tags.read==nullptr)return error::read_failed;
            }
            for(auto const& thread:dump.threads)
            {
                if(!name(thread.name))return error::invalid_utf8;
                if(!valid_capture(thread.state)||(thread.state==capture::unavailable&&!thread.frames.empty()))return error::inconsistent_capture;
                if(thread.frames.size()>UINT32_MAX)return error::format_overflow;
                for(auto const& frame:thread.frames)
                {
                    if(frame.instance>=dump.instances.size())return error::invalid_index;
                    auto const& module=dump.modules[dump.instances[frame.instance].module];
                    if(frame.function>=module.functions)return error::invalid_index;
                    if(frame.code_offset>UINT32_MAX||(!frame.code_offset_available&&frame.code_offset!=0u))return error::invalid_range;
                    if(frame.locals.size()>UINT32_MAX||frame.stack.size()>UINT32_MAX)return error::format_overflow;
                    for(auto v:frame.locals)if(!valid_value(v))return error::invalid_value;
                    for(auto v:frame.stack)if(!valid_value(v))return error::invalid_value;
                }
            }
            return error::none;
        }

        struct encoder
        {
            void* context{};
            write_function write{};
            error failure{};
            ::std::uint64_t count{};
            bool counting{true};

            void reserve(::std::uint64_t n) noexcept
            {if(n>UINT64_MAX-count)failure=error::format_overflow;else count+=n;}
            void raw(::std::span<::std::byte const> data) noexcept
            {
                if(failure!=error::none)return;
                if(data.size()>UINT64_MAX-count){failure=error::format_overflow;return;}
                if(!counting&&!data.empty()&&!write(context,data)){failure=error::write_failed;return;}
                count+=data.size();
            }
            void byte(unsigned value) noexcept {auto b=static_cast<::std::byte>(value);raw({&b,1});}
            void unsigned_leb(::std::uint64_t value) noexcept
            {
                ::std::array<::std::byte,10> bytes{};::std::size_t length{};
                do{auto b=static_cast<unsigned>(value&127u);value>>=7u;bytes[length++]=static_cast<::std::byte>(b|(value?128u:0u));}while(value);
                raw({bytes.data(),length});
            }
            void signed_leb(::std::int64_t value) noexcept
            {
                ::std::array<::std::byte,10> bytes{};::std::size_t length{};bool more;
                do{auto b=static_cast<unsigned>(value)&127u;value>>=7u;more=!((value==0&&(b&64u)==0)||(value==-1&&(b&64u)!=0));bytes[length++]=static_cast<::std::byte>(b|(more?128u:0u));}while(more);
                raw({bytes.data(),length});
            }
            void little(::std::uint64_t bits,unsigned width) noexcept
            {::std::array<::std::byte,8> bytes{};for(unsigned i{};i<width;++i)bytes[i]=static_cast<::std::byte>(bits>>(8u*i));raw({bytes.data(),width});}
            void name(::std::string_view s) noexcept
            {unsigned_leb(s.size());raw({reinterpret_cast<::std::byte const*>(s.data()),s.size()});}
            void numeric(value const& v,bool opcode=false) noexcept
            {
                switch(v.kind)
                {
                    case value_kind::missing:byte(opcode?0x41u:1u);if(opcode)signed_leb(0);break;
                    case value_kind::i32:byte(opcode?0x41u:0x7fu);signed_leb(::std::bit_cast<::std::int32_t>(static_cast<::std::uint32_t>(v.bits)));break;
                    case value_kind::i64:byte(opcode?0x42u:0x7eu);signed_leb(::std::bit_cast<::std::int64_t>(v.bits));break;
                    case value_kind::f32:byte(opcode?0x43u:0x7du);little(v.bits,4);break;
                    case value_kind::f64:byte(opcode?0x44u:0x7cu);little(v.bits,8);break;
                    default:failure=error::invalid_value;
                }
            }
            template<typename Function> void section(unsigned id,Function body) noexcept
            {
                if(failure!=error::none)return;
                encoder measure{};body(measure);
                if(measure.failure!=error::none){failure=measure.failure;return;}
                if(measure.count>section_max){failure=error::format_overflow;return;}
                byte(id);unsigned_leb(measure.count);body(*this);
            }
            template<typename Function> void custom(::std::string_view title,Function body) noexcept
            {section(0u,[&](encoder& out) noexcept {out.name(title);body(out);});}
            void memory_bytes(memory_view const& memory,::std::uint64_t offset,::std::uint64_t bytes) noexcept
            {
                if(failure!=error::none)return;
                if(counting){reserve(bytes);return;}
                ::std::array<::std::byte,4096> buffer{};
                while(bytes&&failure==error::none)
                {
                    auto n=static_cast<::std::size_t>(bytes>buffer.size()?buffer.size():bytes);
                    if(!memory.read(memory.context,offset,{buffer.data(),n})){failure=error::read_failed;return;}
                    raw({buffer.data(),n});offset+=n;bytes-=n;
                }
            }
            void tag_bytes(tag_view const& tags,::std::uint64_t offset,::std::uint64_t count_tags) noexcept
            {
                if(failure!=error::none)return;
                if(counting){if(count_tags>UINT64_MAX/5u){failure=error::format_overflow;return;}reserve(count_tags*5u);return;}
                ::std::array<tag_sample,128> samples{};
                ::std::array<::std::byte,128*5> bytes{};
                while(count_tags&&failure==error::none)
                {
                    auto n=static_cast<::std::size_t>(count_tags>samples.size()?samples.size():count_tags);
                    if(!tags.read(tags.context,offset,{samples.data(),n})){failure=error::read_failed;return;}
                    for(::std::size_t i{};i<n;++i)
                    {
                        auto const& sample=samples[i];
                        if((!sample.available&&sample.tag!=0u)||(tags.bits<32u&&sample.tag>=(::std::uint32_t{1}<<tags.bits)))
                        {failure=error::invalid_value;return;}
                        bytes[i*5]=sample.available?::std::byte{1}: ::std::byte{};
                        for(unsigned j{};j<4;++j)bytes[i*5+j+1]=static_cast<::std::byte>(sample.tag>>(8u*j));
                    }
                    raw({bytes.data(),n*5u});offset+=n;count_tags-=n;
                }
            }
        };

        inline void data_body(encoder& out,dump_view const& dump) noexcept
        {
            ::std::uint64_t count{};
            for(auto const& memory:dump.memories)
            {if(memory.regions.size()>UINT32_MAX-count){out.failure=error::format_overflow;return;}count+=memory.regions.size();}
            out.unsigned_leb(count);
            for(::std::size_t index{};index<dump.memories.size();++index)
            {
                auto const& memory=dump.memories[index];
                for(auto part:memory.regions)
                {
                    if(part.bytes>UINT32_MAX){out.failure=error::format_overflow;return;}
                    out.unsigned_leb(2u);out.unsigned_leb(index);
                    if(memory.memory64){out.byte(0x42u);out.signed_leb(::std::bit_cast<::std::int64_t>(part.offset));}
                    else{out.byte(0x41u);out.signed_leb(::std::bit_cast<::std::int32_t>(static_cast<::std::uint32_t>(part.offset)));}
                    out.byte(0x0bu);out.unsigned_leb(part.bytes);out.memory_bytes(memory,part.offset,part.bytes);
                }
            }
        }
        inline void emit(encoder& out,dump_view const& dump,bool chunks) noexcept
        {
            constexpr ::std::array<::std::byte,8> header{::std::byte{},::std::byte{0x61},::std::byte{0x73},::std::byte{0x6d},::std::byte{1}};
            out.raw(header);
            out.custom("core",[&](encoder& e) noexcept {e.byte(0);e.name(dump.executable);});
            out.custom("coremodules",[&](encoder& e) noexcept {e.unsigned_leb(dump.modules.size());for(auto const& m:dump.modules){e.byte(0);e.name(m.name);}});
            out.custom("coreinstances",[&](encoder& e) noexcept
            {
                e.unsigned_leb(dump.instances.size());
                for(auto const& i:dump.instances){e.byte(0);e.unsigned_leb(i.module);e.unsigned_leb(i.memories.size());for(auto n:i.memories)e.unsigned_leb(n);e.unsigned_leb(i.globals.size());for(auto n:i.globals)e.unsigned_leb(n);}
            });
            for(auto const& thread:dump.threads)
            {
                out.custom("corestack",[&](encoder& e) noexcept
                {
                    e.byte(0);e.name(thread.name);e.unsigned_leb(thread.frames.size());
                    for(auto const& frame:thread.frames){e.byte(0);e.unsigned_leb(frame.instance);e.unsigned_leb(frame.function);e.unsigned_leb(frame.code_offset);
                        e.unsigned_leb(frame.locals.size());for(auto v:frame.locals)e.numeric(v);
                        e.unsigned_leb(frame.stack.size());for(auto v:frame.stack)e.numeric(v);}
                });
            }
            out.section(5u,[&](encoder& e) noexcept
            {
                e.unsigned_leb(dump.memories.size());
                for(auto const& m:dump.memories)
                {auto pages=m.bytes>>16u;e.byte((m.memory64?4u:0u)|(m.shared?3u:0u));e.unsigned_leb(pages);if(m.shared)e.unsigned_leb(pages);}
            });
            out.section(6u,[&](encoder& e) noexcept
            {
                e.unsigned_leb(dump.globals.size());
                for(auto const& g:dump.globals){e.byte(g.data.kind==value_kind::missing?0x7fu:static_cast<unsigned>(g.data.kind));e.byte(0);e.numeric(g.data,true);e.byte(0x0bu);}
            });
            out.custom("uwvm.core.meta",[&](encoder& e) noexcept
            {
                e.unsigned_leb(1);e.byte(dump.coherent?1u:0u);e.byte(chunks?1u:0u);e.name(dump.reason);
                e.unsigned_leb(dump.memories.size());
                for(auto const& m:dump.memories)
                {
                    e.unsigned_leb(m.bytes);e.byte(static_cast<unsigned>(m.state));e.unsigned_leb(m.regions.size());
                    ::std::uint64_t captured{};for(auto r:m.regions)captured+=r.bytes;e.unsigned_leb(captured);
                    e.byte(m.tags.enabled?1u:0u);
                    if(m.tags.enabled){e.byte(m.tags.bits);e.byte(m.tags.granule_log2);e.byte(static_cast<unsigned>(m.tags.state));e.unsigned_leb(m.tags.regions.size());}
                }
                e.unsigned_leb(dump.globals.size());
                for(auto const& g:dump.globals){e.byte(g.data.kind==value_kind::missing?0u:1u);e.byte(g.original_type);}
                e.unsigned_leb(dump.threads.size());
                for(auto const& t:dump.threads){e.byte(static_cast<unsigned>(t.state));e.unsigned_leb(t.frames.size());for(auto const& f:t.frames)e.byte(f.code_offset_available?1u:0u);}
            });
            if(!dump.diagnostic_metadata.empty())out.custom("uwvm.core.diagnostic",[&](encoder& e) noexcept
            {e.raw({reinterpret_cast<::std::byte const*>(dump.diagnostic_metadata.data()),dump.diagnostic_metadata.size()});});
            if(!chunks)out.section(11u,[&](encoder& e) noexcept {data_body(e,dump);});
            else
            {
                // Standard data is explicitly partial, never silently truncated.
                // Repeated versioned sections have u32-sized payloads but no
                // aggregate 4-GiB cap; all requested ranges are streamed.
                out.section(11u,[](encoder& e) noexcept {e.unsigned_leb(0);});
                constexpr ::std::uint64_t max_chunk{UINT32_MAX-128u};
                for(::std::size_t i{};i<dump.memories.size();++i)
                {
                    auto const& m=dump.memories[i];
                    for(auto r:m.regions)while(r.bytes&&out.failure==error::none)
                    {
                        auto n=r.bytes>max_chunk?max_chunk:r.bytes;
                        out.custom("uwvm.core.memory-chunks",[&](encoder& e) noexcept
                        {e.unsigned_leb(1);e.unsigned_leb(i);e.unsigned_leb(r.offset);e.unsigned_leb(n);e.memory_bytes(m,r.offset,n);});
                        r.offset+=n;r.bytes-=n;
                    }
                }
            }
            constexpr ::std::uint64_t max_tags{(UINT32_MAX-128u)/5u};
            for(::std::size_t i{};i<dump.memories.size();++i)
            {
                auto const& tags=dump.memories[i].tags;if(!tags.enabled)continue;
                for(auto r:tags.regions)while(r.bytes&&out.failure==error::none)
                {
                    auto n=r.bytes>max_tags?max_tags:r.bytes;
                    out.custom("uwvm.core.memtag",[&](encoder& e) noexcept
                    {e.unsigned_leb(1);e.unsigned_leb(i);e.unsigned_leb(r.offset);e.unsigned_leb(n);e.tag_bytes(tags,r.offset,n);});
                    r.offset+=n;r.bytes-=n;
                }
            }
        }
    }

    // Preflight traverses metadata and calculates the entire output length without
    // reading guest memory, allocating memory, or invoking the output callback.
    [[nodiscard]] inline result measure(dump_view const& dump) noexcept
    {
        if(auto e=details::validate(dump);e!=error::none)return {e};
        // Reject aggregate-size overflow before iterating billions of chunks.
        // Thirty bytes is a LOWER bound for a memory chunk section's framing;
        // no representable output is rejected by this early bound.
        ::std::uint64_t lower_bound{};
        auto add=[&](::std::uint64_t bytes) noexcept
        {if(bytes>UINT64_MAX-lower_bound)return false;lower_bound+=bytes;return true;};
        for(auto const& m:dump.memories)
        {
            for(auto r:m.regions)
            {
                constexpr ::std::uint64_t capacity{UINT32_MAX-128u};
                auto count=r.bytes/capacity+(r.bytes%capacity!=0u);
                if(!add(r.bytes)||!add(count*30u))return {error::format_overflow};
            }
            for(auto r:m.tags.regions)
            {
                constexpr ::std::uint64_t capacity{(UINT32_MAX-128u)/5u};
                auto count=r.bytes/capacity+(r.bytes%capacity!=0u);
                if(r.bytes>UINT64_MAX/5u||!add(r.bytes*5u)||!add(count*23u))return {error::format_overflow};
            }
        }
        details::encoder data{};details::data_body(data,dump);
        bool chunks=data.failure==error::format_overflow||data.count>UINT32_MAX;
        if(data.failure!=error::none&&data.failure!=error::format_overflow)return {data.failure};
        details::encoder all{};details::emit(all,dump,chunks);
        return {all.failure,all.count,chunks};
    }
    // A failed read/write leaves an incomplete output. The caller must discard
    // its temporary file and publish only a successful result. All views and
    // capture leases must remain stable throughout measure+encode.
    [[nodiscard]] inline result encode(dump_view const& dump,void* context,write_function write) noexcept
    {
        auto plan=measure(dump);if(!plan)return plan;
        if(write==nullptr)return {error::write_failed};
        details::encoder out{context,write,error::none,0u,false};details::emit(out,dump,plan.memory_chunks_required);
        if(out.failure==error::none&&out.count!=plan.bytes)out.failure=error::inconsistent_capture;
        return {out.failure,out.count,plan.memory_chunks_required};
    }
}
