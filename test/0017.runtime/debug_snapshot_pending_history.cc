// A real retained-native archive is readable as historical DATA in any process,
// but neither the origin decoder nor complete replay accepts its unknown result.
#include <uwvm2/runtime/diagnostic/snapshot_state.h>
#include <fast_io.h>
#include <fast_io_device.h>
#include <cstdlib>
namespace ar=::uwvm2::runtime::diagnostic::snapshot_archive;
namespace ss=::uwvm2::runtime::diagnostic::snapshot_state;
namespace cp=::uwvm2::uwvm::debugger::checkpoint;
static void require(bool ok,unsigned line)
{if(!ok){::fast_io::perrln("pending-history failure line=",line);::std::abort();}}
#define REQUIRE(x) require(static_cast<bool>(x),__LINE__)
int main(int argc,char** argv)
{
    if(argc!=2)return 2;
    ::fast_io::native_file_loader file{::fast_io::mnp::os_c_str(argv[1])};
    ::std::span<::std::byte const> bytes{reinterpret_cast<::std::byte const*>(file.data()),file.size()};
    ar::decoded archive{};REQUIRE(ar::decode(bytes,archive));
    cp::execution_replay::decoded history{};
    REQUIRE(ss::inspect_pending_history(archive,history));
    REQUIRE(!history.events.empty());
    auto const expected=history.origin.recording_id;
    auto const count=history.events.size();
    ::std::shared_ptr<cp::state const> executable{};
    REQUIRE(!ss::decode(archive,executable,{},true));REQUIRE(!executable);
    REQUIRE(!ss::decode(archive,executable,{},true,true));REQUIRE(!executable);
    for(auto const& row:archive.records)if(row.kind==ar::component::wasm_state)
    {
        cp::state graph{};REQUIRE(cp::decode_database(row.payload,graph)==cp::error::none);
        cp::execution_replay::decoded complete{};
        REQUIRE(cp::execution_replay::decode(graph,complete)!=cp::error::none);
        REQUIRE(cp::execution_replay::inspect_pending_prefix(graph,complete)==cp::error::none);
    }
    // Missing, duplicated, diagnostic or malformed markers never become
    // accepted observations; failure also leaves the previous output intact.
    auto original=archive;
    ::std::erase_if(archive.records,[](auto const& r){return r.kind==ar::component::retained_native;});
    REQUIRE(!ss::inspect_pending_history(archive,history));
    REQUIRE(history.origin.recording_id==expected && history.events.size()==count);
    archive=original;archive.disclosure=ar::visibility::diagnostic;
    REQUIRE(!ss::inspect_pending_history(archive,history));
    archive=original;
    for(auto& row:archive.records)if(row.kind==ar::component::retained_native)row.schema=2u;
    REQUIRE(!ss::inspect_pending_history(archive,history));
    archive=original;
    for(auto const& row:original.records)if(row.kind==ar::component::retained_native)archive.records.push_back(row);
    REQUIRE(!ss::inspect_pending_history(archive,history));
    archive=original;archive.origin.features^=1u;
    REQUIRE(!ss::inspect_pending_history(archive,history));
    REQUIRE(history.origin.recording_id==expected && history.events.size()==count);
    ::fast_io::println("pending-history data inspection and executable refusal passed");
}
