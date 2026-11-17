// Licensed under APL-2.0 (see LICENSE).
#pragma once
#include <uwvm2/utils/container/standard.h>
#ifndef UWVM_MODULE
# include <array>
# include <bit>
# include <cstddef>
# include <cstdint>
# include <limits>
# include <span>
# include <string_view>
# include <vector>
# include <utility>
# include <fast_io.h>
# include <fast_io_crypto.h>
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::runtime::diagnostic::snapshot_archive
{
    // A canonical, endian-independent container. Decoding supplies DATA, never
    // permission to restore code, reconnect a socket or repeat an external effect.
    enum class architecture : ::std::uint16_t { unknown, x86_64, i686, aarch64, arm, armhf };
    enum class operating_system : ::std::uint16_t { unknown, linux, windows, macos, freebsd };
    enum class byte_order : unsigned char { little=1, big=2 };
    enum class component : ::std::uint16_t
    { diagnostic=1, wasm_state, wasm_module, effective_code, debug_source, wasip1, ros1, ros2, events, wasm_inspection, retained_native };
    enum class completeness : unsigned char { complete, partial, redacted, unavailable };
    enum class visibility : unsigned char { diagnostic=1, full=2 };
    enum class error : unsigned char
    { none, truncated, invalid_header, unsupported_version, invalid_platform, invalid_record,
      duplicate_record, invalid_extent, digest_mismatch, incomplete_commit, write_failed, read_failed, limit_exceeded, disclosure_violation };
    struct platform
    {
        architecture arch{}; operating_system os{}; byte_order endian{byte_order::little};
        unsigned char pointer_bits{};
        ::std::array<::std::uint32_t,4> product_version{};
        ::std::uint32_t logical_abi{}, continuation_abi{};
        ::std::uint64_t features{};
        friend bool operator==(platform const&,platform const&)=default;
    };
    [[nodiscard]] inline constexpr platform native_platform() noexcept
    {
        platform result{};
#if defined(__x86_64__) || defined(_M_X64)
        result.arch=architecture::x86_64;
#elif defined(__i386__) || defined(_M_IX86)
        result.arch=architecture::i686;
#elif defined(__aarch64__) || defined(_M_ARM64)
        result.arch=architecture::aarch64;
#elif defined(__arm__) || defined(_M_ARM)
# if defined(__ARM_PCS_VFP)
        result.arch=architecture::armhf;
# else
        result.arch=architecture::arm;
# endif
#endif
#if defined(__linux__)
        result.os=operating_system::linux;
#elif defined(_WIN32)
        result.os=operating_system::windows;
#elif defined(__APPLE__) && defined(__MACH__)
        result.os=operating_system::macos;
#elif defined(__FreeBSD__)
        result.os=operating_system::freebsd;
#endif
        result.pointer_bits=sizeof(void*)*8u;
        result.endian=::std::endian::native==::std::endian::little?byte_order::little:byte_order::big;
        return result;
    }
    struct record
    {
        component kind{}; completeness capture{completeness::unavailable};
        // ID is a file-local module or environment-group ID. Repeated guest
        // aliases refer to that ID inside the typed payload, not duplicate data.
        ::std::uint64_t id{}, schema{};
        ::std::span<::std::byte const> payload{};
    };
    struct image
    {
        platform origin{};
        ::std::span<record const> records{};
        visibility disclosure{visibility::full};
    };
    struct bounds
    {
        // No arbitrary default snapshot size cap. An embedding deployment can
        // explicitly narrow these limits; actual allocation failure still fails.
        ::std::uint64_t bytes{UINT64_MAX};
        ::std::uint64_t records{static_cast<::std::uint64_t>(SIZE_MAX/sizeof(record))};
    };
    using writer=bool(*)(void*,::std::span<::std::byte const>) noexcept;
    struct result {error status{}; ::std::uint64_t bytes{}; explicit operator bool() const noexcept{return status==error::none;}};
    struct decoded
    {
        platform origin{};
        // Borrows immutable input. The caller must keep its owned copied image
        // alive; a writable/truncatable file mapping is not valid backing.
        ::uwvm2::utils::container::std_vector<record> records{};
        visibility disclosure{visibility::full};
        [[nodiscard]] static constexpr bool grants_restore_authority() noexcept{return false;}
    };
    inline constexpr ::std::size_t header_size{64},record_header_size{32},footer_size{48};
    inline constexpr ::std::uint64_t magic{0x003150414e535755ull}; // UWSNAP1\0
    inline constexpr ::std::uint64_t commit_magic{0x0031444e45535755ull}; // UWSEND1\0
    namespace detail
    {
        template<typename T> inline void put(::std::span<::std::byte> out,::std::size_t at,T value,unsigned n) noexcept
        {for(unsigned i{};i<n;++i)out[at+i]=static_cast<::std::byte>(static_cast<::std::uint64_t>(value)>>(i*8u));}
        [[nodiscard]] inline ::std::uint64_t get(::std::span<::std::byte const> in,::std::size_t at,unsigned n) noexcept
        {::std::uint64_t result{};for(unsigned i{};i<n;++i)result|=static_cast<::std::uint64_t>(in[at+i])<<(i*8u);return result;}
        [[nodiscard]] inline constexpr bool permitted(visibility disclosure,component kind) noexcept
        {
            if(disclosure==visibility::full)return true;
            return disclosure==visibility::diagnostic && (kind==component::diagnostic || kind==component::wasm_module ||
                kind==component::effective_code || kind==component::debug_source);
        }
        [[nodiscard]] inline bool valid(platform const& p) noexcept
        {
            if(p.arch>architecture::armhf || p.os>operating_system::freebsd ||
               (p.endian!=byte_order::little&&p.endian!=byte_order::big) ||
               (p.pointer_bits!=32&&p.pointer_bits!=64))return false;
            return p.arch==architecture::unknown || p.pointer_bits==
                ((p.arch==architecture::x86_64||p.arch==architecture::aarch64)?64u:32u);
        }
        [[nodiscard]] inline bool valid(record const& r) noexcept
        {
            return r.kind>=component::diagnostic&&r.kind<=component::retained_native && r.capture<=completeness::unavailable &&
                r.schema!=0 && r.payload.size()<=static_cast<::std::size_t>(PTRDIFF_MAX) &&
                (r.payload.empty()||r.payload.data()!=nullptr) &&
                (r.capture!=completeness::unavailable||r.payload.empty());
        }
        [[nodiscard]] inline bool before(record const& a,record const& b) noexcept
        {return a.kind<b.kind || (a.kind==b.kind&&a.id<b.id);}
        inline void update(::fast_io::sha256_context& hash,::std::span<::std::byte const> bytes) noexcept
        {if(!bytes.empty())hash.update(bytes.data(),bytes.data()+bytes.size());}
    }
    [[nodiscard]] inline result measure(image const& source,bounds cap={}) noexcept
    {
        if(!detail::valid(source.origin))return {error::invalid_platform};
        if(source.disclosure!=visibility::diagnostic&&source.disclosure!=visibility::full)return {error::disclosure_violation};
        if(source.records.size()>cap.records)return {error::limit_exceeded};
        ::std::uint64_t size{header_size+footer_size};
        if(size>cap.bytes)return {error::limit_exceeded};
        for(::std::size_t i{};i<source.records.size();++i)
        {
            auto const& r=source.records[i];
            if(!detail::valid(r))return {error::invalid_record};
            if(!detail::permitted(source.disclosure,r.kind))return {error::disclosure_violation};
            if(i&&!detail::before(source.records[i-1],r))return {error::duplicate_record};
            if(record_header_size>cap.bytes-size)return {error::limit_exceeded};size+=record_header_size;
            if(r.payload.size()>cap.bytes-size)return {error::limit_exceeded};size+=r.payload.size();
        }
        return {error::none,size};
    }
    [[nodiscard]] inline result encode(image const& source,void* context,writer write,bounds cap={}) noexcept
    {
        auto checked=measure(source,cap);if(!checked)return checked;
        if(!write)return {error::write_failed};
        ::fast_io::sha256_context hash{};::std::uint64_t done{};
        auto emit=[&](::std::span<::std::byte const> bytes,bool hashed=true) noexcept
        {if(!write(context,bytes))return false;if(hashed)detail::update(hash,bytes);done+=bytes.size();return true;};
        ::std::array<::std::byte,header_size> header{};
        detail::put(header,0,magic,8);detail::put(header,8,1u,4);detail::put(header,12,header_size,4);
        detail::put(header,16,source.origin.arch,2);detail::put(header,18,source.origin.os,2);
        detail::put(header,20,source.origin.endian,1);detail::put(header,21,source.origin.pointer_bits,1);
        detail::put(header,22,source.disclosure,1);
        for(unsigned i{};i<4;++i)detail::put(header,24+i*4,source.origin.product_version[i],4);
        detail::put(header,40,source.origin.logical_abi,4);detail::put(header,44,source.origin.continuation_abi,4);
        detail::put(header,48,source.origin.features,8);detail::put(header,56,source.records.size(),8);
        if(!emit(header))return {error::write_failed,done};
        for(auto const& r:source.records)
        {
            ::std::array<::std::byte,record_header_size> h{};
            detail::put(h,0,r.kind,2);detail::put(h,2,r.capture,1);detail::put(h,8,r.id,8);
            detail::put(h,16,r.schema,8);detail::put(h,24,r.payload.size(),8);
            if(!emit(h)||!emit(r.payload))return {error::write_failed,done};
        }
        ::std::array<::std::byte,footer_size> footer{};
        detail::put(footer,0,commit_magic,8);detail::put(footer,8,checked.bytes,8);
        detail::update(hash,{footer.data(),16});hash.do_final();hash.digest_to_byte_ptr(footer.data()+16);
        if(!emit(footer,false))return {error::write_failed,done};
        return {error::none,done};
    }
    struct stream_record
    {
        component kind{}; completeness capture{completeness::unavailable};
        ::std::uint64_t id{},schema{},bytes{};
        void* context{};
        bool (*emit)(void*,void*,writer) noexcept{};
    };
    // Large memory snapshots pass directly from the retained capture owner to
    // the output. Neither a second memory-sized buffer nor a 32-bit file cap is
    // introduced. The producer must emit exactly its preflighted byte count.
    [[nodiscard]] inline result encode_stream(platform const& origin,::std::span<stream_record const> records,
        void* context,writer write,bounds cap={},visibility disclosure=visibility::full) noexcept
    {
        if(disclosure!=visibility::diagnostic&&disclosure!=visibility::full)return {error::disclosure_violation};
        if(!detail::valid(origin))return {error::invalid_platform};
        if(records.size()>cap.records || cap.bytes<header_size+footer_size)return {error::limit_exceeded};
        ::std::uint64_t total{header_size+footer_size};record previous{};
        for(::std::size_t i{};i<records.size();++i)
        {
            auto const& part=records[i];record r{part.kind,part.capture,part.id,part.schema,{}};
            if(!detail::valid(r) || (part.bytes&&!part.emit) ||
               (part.capture==completeness::unavailable&&part.bytes))return {error::invalid_record};
            if(!detail::permitted(disclosure,r.kind))return {error::disclosure_violation};
            if(i&&!detail::before(previous,r))return {error::duplicate_record};previous=r;
            if(record_header_size>cap.bytes-total)return {error::limit_exceeded};total+=record_header_size;
            if(part.bytes>cap.bytes-total)return {error::limit_exceeded};total+=part.bytes;
        }
        if(!write)return {error::write_failed};
        struct output
        {
            ::fast_io::sha256_context hash{};void* context{};writer write{};
            ::std::uint64_t emitted{},remaining{};error failure{};
            static bool accept(void* opaque,::std::span<::std::byte const> bytes) noexcept
            {
                auto& self=*static_cast<output*>(opaque);
                if(self.failure!=error::none)return false;
                if(bytes.size()>self.remaining){self.failure=error::invalid_extent;return false;}
                if(!self.write(self.context,bytes)){self.failure=error::write_failed;return false;}
                detail::update(self.hash,bytes);self.remaining-=bytes.size();self.emitted+=bytes.size();return true;
            }
        } out{};
        out.context=context;out.write=write;out.remaining=header_size;
        ::std::array<::std::byte,header_size> header{};
        detail::put(header,0,magic,8);detail::put(header,8,1u,4);detail::put(header,12,header_size,4);
        detail::put(header,16,origin.arch,2);detail::put(header,18,origin.os,2);
        detail::put(header,20,origin.endian,1);detail::put(header,21,origin.pointer_bits,1);
        detail::put(header,22,disclosure,1);
        for(unsigned i{};i<4;++i)detail::put(header,24+i*4,origin.product_version[i],4);
        detail::put(header,40,origin.logical_abi,4);detail::put(header,44,origin.continuation_abi,4);
        detail::put(header,48,origin.features,8);detail::put(header,56,records.size(),8);
        if(!output::accept(&out,header))return {out.failure,out.emitted};
        for(auto const& r:records)
        {
            ::std::array<::std::byte,record_header_size> h{};
            detail::put(h,0,r.kind,2);detail::put(h,2,r.capture,1);detail::put(h,8,r.id,8);
            detail::put(h,16,r.schema,8);detail::put(h,24,r.bytes,8);
            out.remaining=record_header_size;
            if(!output::accept(&out,h))return {out.failure,out.emitted};
            out.remaining=r.bytes;
            if(r.emit&&!r.emit(r.context,&out,output::accept))return {out.failure==error::none?error::read_failed:out.failure,out.emitted};
            if(out.failure!=error::none)return {out.failure,out.emitted};
            if(out.remaining)return {error::invalid_extent,out.emitted};
        }
        ::std::array<::std::byte,footer_size> footer{};
        detail::put(footer,0,commit_magic,8);detail::put(footer,8,total,8);
        detail::update(out.hash,{footer.data(),16});out.hash.do_final();out.hash.digest_to_byte_ptr(footer.data()+16);
        if(!write(context,footer))return {error::write_failed,out.emitted};
        return {error::none,total};
    }
    [[nodiscard]] inline result decode(::std::span<::std::byte const> input,decoded& output,bounds cap={})
    {
        if(input.size()>cap.bytes || input.size()>static_cast<::std::size_t>(PTRDIFF_MAX))return {error::limit_exceeded};
        if(input.size()<header_size+footer_size)return {error::truncated};
        if(detail::get(input,0,8)!=magic)return {error::invalid_header};
        if(detail::get(input,8,4)!=1)return {error::unsupported_version};
        if(detail::get(input,12,4)!=header_size || detail::get(input,23,1))return {error::invalid_header};
        auto const disclosure=static_cast<visibility>(detail::get(input,22,1));
        if(disclosure!=visibility::diagnostic&&disclosure!=visibility::full)return {error::disclosure_violation};
        auto const end=input.size()-footer_size;
        if(detail::get(input,end,8)!=commit_magic || detail::get(input,end+8,8)!=input.size())return {error::incomplete_commit};
        ::fast_io::sha256_context hash{};detail::update(hash,input.first(input.size()-32));hash.do_final();
        ::std::array<::std::byte,32> digest{};hash.digest_to_byte_ptr(digest.data());
        for(unsigned i{};i<32;++i)if(digest[i]!=input[input.size()-32+i])return {error::digest_mismatch};
        auto const count=detail::get(input,56,8);
        if(count>cap.records || count>(end-header_size)/record_header_size)return {error::invalid_extent};
        platform origin{};origin.arch=static_cast<architecture>(detail::get(input,16,2));
        origin.os=static_cast<operating_system>(detail::get(input,18,2));
        origin.endian=static_cast<byte_order>(detail::get(input,20,1));origin.pointer_bits=detail::get(input,21,1);
        for(unsigned i{};i<4;++i)origin.product_version[i]=detail::get(input,24+i*4,4);
        origin.logical_abi=detail::get(input,40,4);origin.continuation_abi=detail::get(input,44,4);origin.features=detail::get(input,48,8);
        if(!detail::valid(origin))return {error::invalid_platform};
        // First pass validates every extent/order before allocating record storage.
        auto scan=[&](::uwvm2::utils::container::std_vector<record>* records) -> error
        {
            ::std::size_t at=header_size;record previous{};
            for(::std::uint64_t i{};i<count;++i)
            {
                if(record_header_size>end-at)return error::invalid_extent;
                record r{};r.kind=static_cast<component>(detail::get(input,at,2));
                r.capture=static_cast<completeness>(detail::get(input,at+2,1));
                if(detail::get(input,at+3,5))return error::invalid_record;
                r.id=detail::get(input,at+8,8);r.schema=detail::get(input,at+16,8);
                auto const length=detail::get(input,at+24,8);at+=record_header_size;
                if(length>end-at)return error::invalid_extent;
                r.payload=input.subspan(at,static_cast<::std::size_t>(length));at+=static_cast<::std::size_t>(length);
                if(!detail::valid(r))return error::invalid_record;
                if(!detail::permitted(disclosure,r.kind))return error::disclosure_violation;
                if(i&&!detail::before(previous,r))return error::duplicate_record;
                previous=r;if(records)records->push_back(r);
            }
            return at==end?error::none:error::invalid_extent;
        };
        if(auto e=scan(nullptr);e!=error::none)return {e};
        decoded candidate{};candidate.origin=origin;candidate.disclosure=disclosure;candidate.records.reserve(static_cast<::std::size_t>(count));
        if(auto e=scan(&candidate.records);e!=error::none)return {e};
        output=::std::move(candidate);return {error::none,input.size()};
    }
}
