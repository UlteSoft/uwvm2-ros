/** @file Compile against the pre-observation headers to check the archive contract. */
#include <uwvm2/runtime/diagnostic/snapshot_state.h>
#include <fstream>
#include <iterator>

namespace archive=uwvm2::runtime::diagnostic::snapshot_archive;
namespace state=uwvm2::runtime::diagnostic::snapshot_state;

int main(int argc,char** argv)
{
    if(argc!=3)return 1;
    std::vector<std::byte> original_core;
    for(int index=1;index<argc;++index)
    {
        std::ifstream in(argv[index],std::ios::binary);
        std::vector<char> data(std::istreambuf_iterator<char>(in),{});
        archive::decoded image;
        if(!archive::decode(std::as_bytes(std::span{data}),image))return 2;
        auto core=std::ranges::find_if(image.records,[](auto const& row)
        {return row.kind==archive::component::wasm_inspection && row.id==0;});
        if(core==image.records.end() || core->schema!=1 || core->capture!=archive::completeness::partial)return 3;
        if(index==1)original_core.assign(core->payload.begin(),core->payload.end());
        else if(!std::ranges::equal(core->payload,original_core))return 4;

        std::shared_ptr<state::cp::state const> graph;
        if(state::decode(image,graph) || graph || image.grants_restore_authority())return 5;
        // Test the incomplete state check independently of the real fallback
        // file's earlier rejection on its materialization ABI.
        image.origin.logical_abi=state::materialization::state_schema_revision;
        image.origin.continuation_abi=state::materialization::native_resume_abi_revision;
        if(state::decode(image,graph).status!=state::error::incomplete_state || graph)return 6;
    }
}
