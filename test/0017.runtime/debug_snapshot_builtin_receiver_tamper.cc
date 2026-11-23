// Test input generator: a correct digest must not authorize a different builtin
// receiver. Uses the real typed codecs; no product restore capability is minted.
#include <uwvm2/runtime/diagnostic/snapshot_state.h>
#include <fstream>
#include <iterator>
#include <vector>
#include <cstring>
namespace ar=::uwvm2::runtime::diagnostic::snapshot_archive;
namespace cp=::uwvm2::uwvm::debugger::checkpoint;
int main(int argc,char** argv)
{
    if(argc!=3 && (argc!=4 || ::std::strcmp(argv[3],"--event")!=0))return 2;
    ::std::ifstream input{argv[1],::std::ios::binary};
    ::std::vector<char> raw{::std::istreambuf_iterator<char>{input},{}};
    if(!input || raw.empty())return 3;
    auto bytes=::std::as_bytes(::std::span{raw});ar::decoded archive{};
    if(!ar::decode(bytes,archive))return 4;
    for(auto& record:archive.records)
    {
        if(record.kind!=ar::component::wasm_state)continue;
        cp::state state{};
        if(cp::decode_database(record.payload,state)!=cp::error::none)return 5;
        bool changed{};
        if(argc==4)
        {
            cp::object event{};event.kind=cp::object_kind::event;
            event.words[0u]=1u;event.words[2u]=1u;event.words[3u]=1u;
            state.objects.push_back(::std::move(event));changed=true;
        }
        for(auto& receiver:state.objects)
        {
            if(changed)break;
            if(receiver.kind!=cp::object_kind::function || receiver.flags!=1u)continue;
            auto const& binding=state.objects[receiver.links[1u]-1u];
            for(auto const& other:state.objects)
            {
                if(other.kind!=cp::object_kind::function || other.flags!=1u || other.links[0u]==receiver.links[0u])continue;
                auto const& other_binding=state.objects[other.links[1u]-1u];
                if(binding.words!=other_binding.words || binding.bytes!=other_binding.bytes)continue;
                receiver.links[0u]=other.links[0u];receiver.words[0u]=other.words[0u];receiver.words[2u]=other.words[2u];
                changed=true;break;
            }
            if(changed)break;
        }
        if(!changed || cp::validate_graph(state)!=cp::error::none)return 6;
        ::std::uint64_t body{};
        if(!cp::codec_details::measure(state,{},body) || body>SIZE_MAX-cp::header_bytes-cp::footer_bytes)return 7;
        ::std::vector<::std::byte> payload(static_cast<::std::size_t>(body)+cp::header_bytes+cp::footer_bytes);
        auto first=reinterpret_cast<unsigned char*>(payload.data());
        ::fast_io::basic_obuffer_view<unsigned char> output{first,first+payload.size()};
        if(cp::encode_database(output,state)!=cp::error::none || output.curr_ptr!=first+payload.size())return 7;
        record.payload=payload;
        ::std::vector<::std::byte> encoded;
        auto append=[](void* context,::std::span<::std::byte const> data) noexcept
        {
            try
            {
                auto& buffer=*static_cast<::std::vector<::std::byte>*>(context);
                buffer.insert(buffer.end(),data.begin(),data.end());return true;
            }
            catch(...){return false;}
        };
        if(!ar::encode({archive.origin,archive.records,archive.disclosure},&encoded,append))return 8;
        ::fast_io::native_file file{::fast_io::mnp::os_c_str(argv[2]),::fast_io::open_mode::out|::fast_io::open_mode::excl};
        ::fast_io::operations::write_all_bytes(file,encoded.data(),encoded.data()+encoded.size());file.close();return 0;
    }
    return 9;
}
