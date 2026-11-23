// Fail each successive cold-parser allocation. No Wasm/native read is involved.
#include <uwvm2/uwvm/debugger/source_dwarf_expression.h>
#include <cstdlib>
#include <cstdio>
#include <new>
#include "fast_io_allocation_probe.h"
static void require(bool ok) {
    if(!ok) { std::fputs("FAIL source-expression allocation cleanup\n",stderr);std::abort(); }
}
int main() {
    namespace d=uwvm2::uwvm::debugger::source_dwarf;
    std::string text=std::string(65536,'(')+std::string(1024,'*')+"producer::long_root_name";
    for(unsigned i=0;i<80;++i)text+=".long_member_name_requiring_owned_storage";
    text+=std::string(65536,')');
    unsigned failures{};
    for(std::size_t limit=0;limit!=64;++limit) {
        d::source_expression value{"previous_root",{{d::source_expression_step_kind::member,"previous_member",0}}};
        fast_io_allocation_probe::remaining=limit;fast_io_allocation_probe::armed=true;
        auto status=d::parse_source_expression(text,value);
        fast_io_allocation_probe::armed=false;
        require(status==d::object_selector_error::allocation_failure);
        require(value.root_name.empty()&&value.steps.empty());
        ++failures;
    }
    d::source_expression value;
    require(d::parse_source_expression(text,value)==d::object_selector_error::none);
    require(value.root_name=="producer::long_root_name"&&value.steps.size()==1104);
    std::printf("PASS %u injected allocation failures with cleared output; subsequent parse succeeds\n",failures);
}
