// SPDX-License-Identifier: Apache-2.0
#pragma once
// Bounded data codec for SDK portable contract 0.1.0. None of these data types
// grants authority or registers a Host import. The Host must separately retain
// genuine caller, policy, mapping and native-resource leases across effects.
#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <span>

namespace uwvm2::imported::ros::portable
{
    enum class error : ::std::uint32_t
    {
        ok = 0, bad_handle = 9, no_fresh_state = 11, denied = 13,
        malformed = 22, unavailable = 38, too_small = 90,
        unsupported = 95, deadline = 110, stale = 116
    };
    enum class outcome : ::std::uint32_t
    { accepted = 1, succeeded = 2, rejected = 3, cancelled = 4, indeterminate = 5 };
    inline constexpr ::std::size_t max_joints{16};
    inline constexpr ::std::int64_t position_limit{1000000000};
    inline constexpr ::std::uint64_t fixture_duration_ns{1000000000};
    inline constexpr ::std::uint32_t all_rights{15}, software_simulation{1};
    using digest = ::std::array<::std::uint8_t,32>;

    struct joint_limits
    {
        ::std::int64_t lower{}, upper{};
        ::std::uint64_t max_step{}, tolerance{};
    };
    struct descriptor
    {
        ::std::uint64_t epoch{};
        ::std::uint32_t rights{}, count{};
        digest mapping{};
        ::std::array<joint_limits,max_joints> joints{};
    };
    struct state
    {
        ::std::uint64_t epoch{}, sequence{}, observed_ns{}, valid_until_ns{};
        ::std::uint32_t count{};
        ::std::array<::std::int64_t,max_joints> positions{};
    };
    struct command
    {
        ::std::uint64_t epoch{}, id{}, based_on_sequence{}, duration_ns{};
        ::std::uint32_t count{};
        digest mapping{};
        ::std::array<::std::int64_t,max_joints> targets{};
    };
    struct receipt
    {
        ::std::uint64_t epoch{}, command_id{}, observed_sequence{}, observed_ns{};
        outcome result{};
        ::std::int32_t native_status{};
        digest command_sha256{};
    };

    namespace wire_detail
    {
        using input = ::std::span<::std::uint8_t const>;
        using output = ::std::span<::std::uint8_t>;
        constexpr ::std::uint64_t get(input bytes,::std::size_t at,unsigned width) noexcept
        {
            ::std::uint64_t value{};
            for(unsigned i{};i!=width;++i) { value |= static_cast<::std::uint64_t>(bytes[at+i]) << (8*i); }
            return value;
        }
        constexpr void put(output bytes,::std::size_t at,::std::uint64_t value,unsigned width) noexcept
        { for(unsigned i{};i!=width;++i) { bytes[at+i]=static_cast<::std::uint8_t>(value >> (8*i)); } }
        constexpr bool zero(input bytes,::std::size_t begin,::std::size_t end) noexcept
        { for(auto i{begin};i!=end;++i) { if(bytes[i]) { return false; } } return true; }
        constexpr bool count_valid(::std::uint32_t count) noexcept { return count && count<=max_joints; }
        constexpr bool position_valid(::std::int64_t position) noexcept
        { return position>=-position_limit && position<=position_limit; }
        constexpr bool prefix(input bytes,unsigned kind,::std::size_t base,::std::size_t stride,::std::uint32_t& count) noexcept
        {
            if(bytes.size()<base || bytes[0]!='U' || bytes[1]!='R' || bytes[2]!='P' || bytes[3]!='1' ||
               get(bytes,4,2)!=1 || get(bytes,6,2)!=kind || get(bytes,8,4)!=bytes.size()) { return false; }
            auto n{static_cast<::std::uint32_t>(get(bytes,12,4))};
            if(kind==4 ? n!=0 : !count_valid(n)) { return false; }
            if(bytes.size()!=base+stride*n) { return false; }
            count=n;return true;
        }
        constexpr void prefix(output bytes,unsigned kind,::std::size_t size,::std::uint32_t count) noexcept
        {
            for(::std::size_t i{};i!=size;++i) { bytes[i]=0; }
            bytes[0]='U';bytes[1]='R';bytes[2]='P';bytes[3]='1';
            put(bytes,4,1,2);put(bytes,6,kind,2);put(bytes,8,size,4);put(bytes,12,count,4);
        }
        constexpr digest read_digest(input bytes,::std::size_t at) noexcept
        { digest value{};for(::std::size_t i{};i!=value.size();++i) { value[i]=bytes[at+i]; } return value; }
        constexpr void write_digest(output bytes,::std::size_t at,digest const& value) noexcept
        { for(::std::size_t i{};i!=value.size();++i) { bytes[at+i]=value[i]; } }
        constexpr ::std::int64_t signed64(input bytes,::std::size_t at) noexcept
        { return ::std::bit_cast<::std::int64_t>(get(bytes,at,8)); }
    }

    constexpr bool valid(descriptor const& value) noexcept
    {
        if(!value.epoch || !value.rights || (value.rights&~all_rights) || !wire_detail::count_valid(value.count)) { return false; }
        for(::std::uint32_t i{};i!=value.count;++i)
        {
            auto const& joint{value.joints[i]};
            if(!wire_detail::position_valid(joint.lower) || !wire_detail::position_valid(joint.upper) ||
               joint.lower>=joint.upper || !joint.max_step || joint.max_step>10000 || joint.tolerance>1000) { return false; }
        }
        return true;
    }
    constexpr bool valid(state const& value) noexcept
    {
        if(!value.epoch || !value.sequence || !wire_detail::count_valid(value.count) || value.valid_until_ns<=value.observed_ns) { return false; }
        for(::std::uint32_t i{};i!=value.count;++i) { if(!wire_detail::position_valid(value.positions[i])) { return false; } }
        return true;
    }
    constexpr bool valid(command const& value) noexcept
    {
        if(!value.epoch || !value.id || !value.based_on_sequence || !wire_detail::count_valid(value.count) ||
           value.duration_ns!=fixture_duration_ns) { return false; }
        for(::std::uint32_t i{};i!=value.count;++i) { if(!wire_detail::position_valid(value.targets[i])) { return false; } }
        return true;
    }
    constexpr bool valid(receipt const& value) noexcept
    {
        auto code{static_cast<::std::uint32_t>(value.result)};
        return value.epoch && value.command_id && code>=1 && code<=5 &&
            (value.result!=outcome::succeeded || (value.observed_sequence && value.native_status==0));
    }

    // Decoders commit only after the complete fixed record is canonical.
    constexpr bool decode(::std::span<::std::uint8_t const> bytes,descriptor& out) noexcept
    {
        descriptor value{};
        if(!wire_detail::prefix(bytes,1,64,40,value.count) || wire_detail::get(bytes,28,4)!=software_simulation) { return false; }
        value.epoch=wire_detail::get(bytes,16,8);value.rights=static_cast<::std::uint32_t>(wire_detail::get(bytes,24,4));
        value.mapping=wire_detail::read_digest(bytes,32);
        for(::std::uint32_t i{};i!=value.count;++i)
        {
            auto at{64+40*static_cast<::std::size_t>(i)};
            if(wire_detail::get(bytes,at,4)!=i || wire_detail::get(bytes,at+4,4)!=1) { return false; }
            value.joints[i]={wire_detail::signed64(bytes,at+8),wire_detail::signed64(bytes,at+16),
                             wire_detail::get(bytes,at+24,8),wire_detail::get(bytes,at+32,8)};
        }
        if(!valid(value)) { return false; } out=value;return true;
    }
    constexpr bool decode(::std::span<::std::uint8_t const> bytes,state& out) noexcept
    {
        state value{};
        if(!wire_detail::prefix(bytes,2,64,16,value.count) || !wire_detail::zero(bytes,48,64)) { return false; }
        value.epoch=wire_detail::get(bytes,16,8);value.sequence=wire_detail::get(bytes,24,8);
        value.observed_ns=wire_detail::get(bytes,32,8);value.valid_until_ns=wire_detail::get(bytes,40,8);
        for(::std::uint32_t i{};i!=value.count;++i)
        {
            auto at{64+16*static_cast<::std::size_t>(i)};
            if(wire_detail::get(bytes,at,4)!=i || wire_detail::get(bytes,at+4,4)!=1) { return false; }
            value.positions[i]=wire_detail::signed64(bytes,at+8);
        }
        if(!valid(value)) { return false; } out=value;return true;
    }
    constexpr bool decode(::std::span<::std::uint8_t const> bytes,command& out) noexcept
    {
        command value{};
        if(!wire_detail::prefix(bytes,3,80,16,value.count)) { return false; }
        value.epoch=wire_detail::get(bytes,16,8);value.id=wire_detail::get(bytes,24,8);
        value.based_on_sequence=wire_detail::get(bytes,32,8);value.mapping=wire_detail::read_digest(bytes,40);
        value.duration_ns=wire_detail::get(bytes,72,8);
        for(::std::uint32_t i{};i!=value.count;++i)
        {
            auto at{80+16*static_cast<::std::size_t>(i)};
            if(wire_detail::get(bytes,at,4)!=i || wire_detail::get(bytes,at+4,4)) { return false; }
            value.targets[i]=wire_detail::signed64(bytes,at+8);
        }
        if(!valid(value)) { return false; } out=value;return true;
    }
    constexpr bool decode(::std::span<::std::uint8_t const> bytes,receipt& out) noexcept
    {
        receipt value{};::std::uint32_t count{};
        if(!wire_detail::prefix(bytes,4,96,0,count) || !wire_detail::zero(bytes,88,96)) { return false; }
        value.epoch=wire_detail::get(bytes,16,8);value.command_id=wire_detail::get(bytes,24,8);
        value.result=static_cast<outcome>(wire_detail::get(bytes,32,4));
        value.native_status=::std::bit_cast<::std::int32_t>(static_cast<::std::uint32_t>(wire_detail::get(bytes,36,4)));
        value.observed_sequence=wire_detail::get(bytes,40,8);value.observed_ns=wire_detail::get(bytes,48,8);
        value.command_sha256=wire_detail::read_digest(bytes,56);
        if(!valid(value)) { return false; } out=value;return true;
    }

    struct encoded { error status; ::std::size_t bytes; };
    constexpr encoded encode(descriptor const& value,::std::span<::std::uint8_t> bytes) noexcept
    {
        if(!valid(value)) { return {error::malformed,0}; }
        auto size{64+40*static_cast<::std::size_t>(value.count)};
        if(bytes.size()<size) { return {error::too_small,size}; }
        wire_detail::prefix(bytes,1,size,value.count);wire_detail::put(bytes,16,value.epoch,8);
        wire_detail::put(bytes,24,value.rights,4);wire_detail::put(bytes,28,software_simulation,4);
        wire_detail::write_digest(bytes,32,value.mapping);
        for(::std::uint32_t i{};i!=value.count;++i)
        {
            auto at{64+40*static_cast<::std::size_t>(i)};auto const& joint{value.joints[i]};
            wire_detail::put(bytes,at,i,4);wire_detail::put(bytes,at+4,1,4);
            wire_detail::put(bytes,at+8,static_cast<::std::uint64_t>(joint.lower),8);
            wire_detail::put(bytes,at+16,static_cast<::std::uint64_t>(joint.upper),8);
            wire_detail::put(bytes,at+24,joint.max_step,8);wire_detail::put(bytes,at+32,joint.tolerance,8);
        }
        return {error::ok,size};
    }
    constexpr encoded encode(state const& value,::std::span<::std::uint8_t> bytes) noexcept
    {
        if(!valid(value)) { return {error::malformed,0}; }
        auto size{64+16*static_cast<::std::size_t>(value.count)};
        if(bytes.size()<size) { return {error::too_small,size}; }
        wire_detail::prefix(bytes,2,size,value.count);wire_detail::put(bytes,16,value.epoch,8);
        wire_detail::put(bytes,24,value.sequence,8);wire_detail::put(bytes,32,value.observed_ns,8);
        wire_detail::put(bytes,40,value.valid_until_ns,8);
        for(::std::uint32_t i{};i!=value.count;++i)
        {
            auto at{64+16*static_cast<::std::size_t>(i)};
            wire_detail::put(bytes,at,i,4);wire_detail::put(bytes,at+4,1,4);
            wire_detail::put(bytes,at+8,static_cast<::std::uint64_t>(value.positions[i]),8);
        }
        return {error::ok,size};
    }
    constexpr encoded encode(command const& value,::std::span<::std::uint8_t> bytes) noexcept
    {
        if(!valid(value)) { return {error::malformed,0}; }
        auto size{80+16*static_cast<::std::size_t>(value.count)};
        if(bytes.size()<size) { return {error::too_small,size}; }
        wire_detail::prefix(bytes,3,size,value.count);wire_detail::put(bytes,16,value.epoch,8);
        wire_detail::put(bytes,24,value.id,8);wire_detail::put(bytes,32,value.based_on_sequence,8);
        wire_detail::write_digest(bytes,40,value.mapping);wire_detail::put(bytes,72,value.duration_ns,8);
        for(::std::uint32_t i{};i!=value.count;++i)
        {
            auto at{80+16*static_cast<::std::size_t>(i)};wire_detail::put(bytes,at,i,4);
            wire_detail::put(bytes,at+8,static_cast<::std::uint64_t>(value.targets[i]),8);
        }
        return {error::ok,size};
    }
    constexpr encoded encode(receipt const& value,::std::span<::std::uint8_t> bytes) noexcept
    {
        if(!valid(value)) { return {error::malformed,0}; }
        if(bytes.size()<96) { return {error::too_small,96}; }
        wire_detail::prefix(bytes,4,96,0);wire_detail::put(bytes,16,value.epoch,8);
        wire_detail::put(bytes,24,value.command_id,8);wire_detail::put(bytes,32,static_cast<::std::uint32_t>(value.result),4);
        wire_detail::put(bytes,36,static_cast<::std::uint32_t>(value.native_status),4);
        wire_detail::put(bytes,40,value.observed_sequence,8);wire_detail::put(bytes,48,value.observed_ns,8);
        wire_detail::write_digest(bytes,56,value.command_sha256);return {error::ok,96};
    }

    // Data validation only: the caller must already hold separate native grants.
    // This function never treats descriptor.rights as an authorization source.
    constexpr error validate_command_data(command const& request,descriptor const& mapping,
                                          state const& sample,::std::uint64_t now_ns) noexcept
    {
        if(!valid(request) || !valid(mapping) || !valid(sample)) { return error::malformed; }
        if(request.epoch!=mapping.epoch || sample.epoch!=mapping.epoch || request.mapping!=mapping.mapping ||
           request.based_on_sequence!=sample.sequence) { return error::stale; }
        if(request.count!=mapping.count || sample.count!=mapping.count) { return error::unsupported; }
        if(now_ns<sample.observed_ns || now_ns>=sample.valid_until_ns) { return error::no_fresh_state; }
        for(::std::uint32_t i{};i!=mapping.count;++i)
        {
            auto const& bounds{mapping.joints[i]};auto current{sample.positions[i]},target{request.targets[i]};
            if(current<bounds.lower || current>bounds.upper || target<bounds.lower || target>bounds.upper) { return error::unsupported; }
            // All positions are in [-1e9,1e9], so this subtraction cannot wrap.
            auto distance{current<target ? target-current : current-target};
            if(static_cast<::std::uint64_t>(distance)>bounds.max_step) { return error::unsupported; }
        }
        return error::ok;
    }
}
