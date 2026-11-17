// Unified snapshot container for authenticated private WASI Threads histories.
// Decoding yields data only. Current native launch authority is checked again
// by the runtime; a recorded unknown host result never authorizes fresh I/O.
#pragma once
#ifndef UWVM_MODULE
#include <fast_io.h>
#include <fast_io_crypto.h>
#include "execution_journal.h"
#include <uwvm2/runtime/diagnostic/snapshot_archive.h>
#include <uwvm2/runtime/diagnostic/snapshot_wasip1.h>
#include <uwvm2/runtime/diagnostic/snapshot_ros.h>
#endif
namespace uwvm2::runtime::wasm_threads
{
    inline constexpr ::std::uint64_t threaded_events_schema{0x54484a38u};
    struct threaded_environment_sample
    {
        ::std::uint64_t cursor{};
        ::uwvm2::utils::container::std_vector<::std::byte> archive;
    };
    struct threaded_snapshot
    {
        threaded_execution_history history;
        ::uwvm2::utils::container::std_vector<threaded_module_image> modules;
        ::uwvm2::runtime::diagnostic::snapshot_archive::platform origin{};
        ::uwvm2::utils::container::std_vector<threaded_environment_sample> environments;
    };
    [[nodiscard]] inline auto threaded_history_label(threaded_execution_history const& history) noexcept
    {
        ::std::array<::std::byte,16> label{};
        if(!history.events.empty() && history.events.front().input.size()>=label.size())
            ::std::copy_n(history.events.front().input.begin(),label.size(),label.begin());
        if(::std::all_of(label.begin(),label.end(),[](::std::byte b){return b==::std::byte{};}))label[0]=::std::byte{1};
        return label;
    }
    [[nodiscard]] inline bool valid_threaded_environments(threaded_execution_history const& history,
        ::std::span<threaded_module_image const> modules,::std::span<threaded_environment_sample const> samples) noexcept
    {
        namespace ar=::uwvm2::runtime::diagnostic::snapshot_archive;
        namespace wasi=::uwvm2::runtime::diagnostic::snapshot_wasip1;
        namespace ros=::uwvm2::runtime::diagnostic::snapshot_ros;
        try
        {
            auto label=threaded_history_label(history);::std::uint64_t previous{};
            for(auto const& sample:samples)
            {
                if(!sample.cursor || sample.cursor<previous || sample.cursor>history.events.size())return false;
                previous=sample.cursor;ar::decoded saved;
                if(!ar::decode(sample.archive,saved)||saved.disclosure!=ar::visibility::full||saved.origin.logical_abi||saved.origin.continuation_abi)return false;
                for(auto const& record:saved.records)
                {
                    if(record.kind==ar::component::wasip1)
                    {
                        wasi::environment value;
                        if(!wasi::decode(saved.disclosure,record,value)||value.recording!=label||value.module>=modules.size())return false;
                        for(auto member:value.members)if(member>=modules.size())return false;
                        auto const& bytes=modules[static_cast<::std::size_t>(value.module)].wasm;
                        ::fast_io::sha256_context hash{};hash.update(bytes.data(),bytes.data()+bytes.size());hash.do_final();
                        ::std::array<::std::byte,32> digest{};hash.digest_to_byte_ptr(digest.data());
                        if(digest!=value.original_wasm)return false;
                    }
                    else if(record.kind==ar::component::ros1||record.kind==ar::component::ros2)
                    {
                        ros::environment value;
                        if(!ros::decode(saved.disclosure,record,value)||value.recording!=label)return false;
                        for(auto const& group:value.groups)for(auto const& member:group.members)
                            if(member.module>=modules.size())return false;
                    }
                    else return false;
                }
            }
            return true;
        }
        catch(...){return false;}
    }
    [[nodiscard]] inline bool threaded_source_digest(::std::span<threaded_module_image const> sources,
        ::std::array<::std::byte,32>& result) noexcept
    {
        if(sources.empty())return false;
        ::fast_io::sha256_context hash{};
        auto number=[&](::std::uint64_t value){::std::array<::std::byte,8> bytes{};
            for(unsigned n{};n<8u;++n)bytes[n]=static_cast<::std::byte>(value>>(8u*n));hash.update(bytes.data(),bytes.data()+8u);};
        number(sources.size());
        constexpr ::std::array<::std::byte,8> magic{::std::byte{},::std::byte{0x61},::std::byte{0x73},::std::byte{0x6d},::std::byte{1}};
        for(::std::size_t i{};i<sources.size();++i)
        {
            auto const& source=sources[i];
            if(source.identifier!=i||source.name.empty()||source.wasm.size()<8u||!::std::equal(magic.begin(),magic.end(),source.wasm.begin()))return false;
            if(i&&!::std::lexicographical_compare(sources[i-1u].name.begin(),sources[i-1u].name.end(),source.name.begin(),source.name.end()))return false;
            for(::std::size_t j{};j<i;++j)
                if(source.identifier==sources[j].identifier||source.name==sources[j].name)return false;
            number(source.identifier);number(source.name.size());hash.update(source.name.data(),source.name.data()+source.name.size());
            number(source.wasm.size());hash.update(source.wasm.data(),source.wasm.data()+source.wasm.size());
        }
        hash.do_final();hash.digest_to_byte_ptr(result.data());return true;
    }
    [[nodiscard]] inline bool encode_threaded_snapshot(threaded_execution_history const& history,
        ::std::span<threaded_module_image const> modules,::uwvm2::utils::container::std_vector<::std::byte>& output,
        ::std::span<threaded_environment_sample const> environments={}) noexcept
    {
        namespace ar=::uwvm2::runtime::diagnostic::snapshot_archive;
        try
        {
            ::std::array<::std::byte,32> digest{};
            if(!valid_execution_history(history)||!valid_threaded_environments(history,modules,environments)||!threaded_source_digest(modules,digest)||
               !::std::equal(digest.begin(),digest.end(),history.events.front().input.begin()+32u))return false;
            for(auto const& event:history.events)
                if(event.kind==journal_kind::host_begin&&(event.b>>1u)>=modules.size())return false;
            ::uwvm2::utils::container::std_vector<::std::byte> journal,payload;
            if(!encode_execution_history(history,journal))return false;
            auto put=[&](::std::uint64_t value){for(unsigned n{};n<8u;++n)payload.push_back(static_cast<::std::byte>(value>>(8u*n)));};
            put(threaded_events_schema);put(modules.size());
            for(auto const& source:modules)
            {put(source.identifier);put(source.name.size());payload.insert(payload.end(),source.name.begin(),source.name.end());}
            put(journal.size());payload.insert(payload.end(),journal.begin(),journal.end());
            put(environments.size());
            for(auto const& sample:environments)
            {put(sample.cursor);put(sample.archive.size());payload.insert(payload.end(),sample.archive.begin(),sample.archive.end());}
            ::uwvm2::utils::container::std_vector<ar::record> records;
            records.push_back({ar::component::events,ar::completeness::complete,0u,threaded_events_schema,payload});
            for(auto const& source:modules)records.push_back({ar::component::wasm_module,ar::completeness::complete,source.identifier,1u,source.wasm});
            ::std::sort(records.begin(),records.end(),[](auto const& a,auto const& b)
                {return a.kind<b.kind||(a.kind==b.kind&&a.id<b.id);});
            auto platform=ar::native_platform();platform.logical_abi=7u;
            // No serialized native stack, PC, pointer or register ABI is used.
            platform.continuation_abi=0u;
            ::uwvm2::utils::container::std_vector<::std::byte> bytes;
            auto writer=[](void* pointer,::std::span<::std::byte const> next) noexcept
            {
                try{auto& out=*static_cast<::uwvm2::utils::container::std_vector<::std::byte>*>(pointer);
                    out.insert(out.end(),next.begin(),next.end());return true;}catch(...){return false;}
            };
            if(!ar::encode({platform,records,ar::visibility::full},&bytes,writer))return false;
            output=::std::move(bytes);return true;
        }
        catch(...){return false;}
    }
    [[nodiscard]] inline bool decode_threaded_snapshot(::std::span<::std::byte const> input,threaded_snapshot& output) noexcept
    {
        namespace ar=::uwvm2::runtime::diagnostic::snapshot_archive;
        try
        {
            ar::decoded archive;if(!ar::decode(input,archive)||archive.disclosure!=ar::visibility::full||
                archive.origin.logical_abi!=7u||archive.origin.continuation_abi||archive.origin.endian!=ar::byte_order::little||
                archive.origin.os!=ar::operating_system::linux||archive.origin.arch==ar::architecture::unknown||archive.records.size()<2u)return false;
            auto const& record=archive.records.back();
            if(record.kind!=ar::component::events||record.id||record.schema!=threaded_events_schema||record.capture!=ar::completeness::complete)return false;
            threaded_snapshot result;result.origin=archive.origin;
            auto bytes=record.payload;::std::size_t position{};
            auto get=[&](::std::uint64_t& value)
            {if(bytes.size()-position<8u)return false;value=0u;for(unsigned n{};n<8u;++n)value|=::std::to_integer<::std::uint64_t>(bytes[position++])<<(8u*n);return true;};
            ::std::uint64_t schema{},count{};
            if(!get(schema)||schema!=threaded_events_schema||!get(count)||count!=archive.records.size()-1u||count>(bytes.size()-position)/16u)return false;
            result.modules.reserve(static_cast<::std::size_t>(count));
            for(::std::size_t i{};i<count;++i)
            {
                threaded_module_image source;::std::uint64_t length{};
                if(!get(source.identifier)||!get(length)||!length||length>bytes.size()-position)return false;
                source.name.assign(bytes.begin()+position,bytes.begin()+position+static_cast<::std::size_t>(length));position+=static_cast<::std::size_t>(length);
                auto found=::std::find_if(archive.records.begin(),archive.records.end(),[&](auto const& item)
                    {return item.kind==ar::component::wasm_module&&item.id==source.identifier;});
                if(found==archive.records.end()||found->schema!=1u||found->capture!=ar::completeness::complete)return false;
                source.wasm.assign(found->payload.begin(),found->payload.end());result.modules.push_back(::std::move(source));
            }
            ::std::uint64_t length{};
            if(!get(length)||length>bytes.size()-position||!decode_execution_history(bytes.subspan(position,static_cast<::std::size_t>(length)),result.history))return false;
            position+=static_cast<::std::size_t>(length);
            if(!get(count)||count>(bytes.size()-position)/16u)return false;
            result.environments.reserve(static_cast<::std::size_t>(count));
            for(::std::size_t n{};n<count;++n)
            {
                threaded_environment_sample sample;
                if(!get(sample.cursor)||!get(length)||length>bytes.size()-position)return false;
                sample.archive.assign(bytes.begin()+position,bytes.begin()+position+static_cast<::std::size_t>(length));
                position+=static_cast<::std::size_t>(length);result.environments.push_back(::std::move(sample));
            }
            if(position!=bytes.size()||!valid_threaded_environments(result.history,result.modules,result.environments))return false;
            for(auto const& event:result.history.events)
                if(event.kind==journal_kind::host_begin&&(event.b>>1u)>=result.modules.size())return false;
            ::std::array<::std::byte,32> digest{};
            if(!threaded_source_digest(result.modules,digest)||!::std::equal(digest.begin(),digest.end(),result.history.events.front().input.begin()+32u))return false;
            output=::std::move(result);return true;
        }
        catch(...){return false;}
    }
}
