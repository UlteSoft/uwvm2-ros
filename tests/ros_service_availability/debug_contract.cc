/** Genuine detached debugger DTO/parser/fast_io renderer contract only.
 * No debug control connection, Runtime query, native client or permission. */
#include <uwvm2/uwvm/debugger/ros_state.h>
#include <fast_io_unit/string.h>
#include <string>
#include <cstdlib>
namespace ui=uwvm2::uwvm::debugger::ros_state;namespace data=ui::data;
static void require(bool value){if(!value){std::abort();}}
int main()
{
    data::request request{};require(ui::parse("trace ros function=client.features op=service.available alias=available",request));
    require(request.operation.view()=="service.available"&&request.function.view()=="client.features");
    require(!ui::parse("trace ros source_alias=call",request)); // No unreviewed new selector.
    require(!ui::parse("info ros services mutate=true",request));
    data::trace_store store;data::trace_record value{};value.backend=2;value.instance=31;value.generation=4;
    value.alias.assign("available");value.source_alias.assign("call");value.operation.assign("service.available");value.function.assign("client.features");
    value.client_session_label=2;value.client_features=7;value.client_features_observed=true;store.append(value);
    request={};request.selected=data::selection::trace;request.operation.assign("service.available");data::view output;store.read(request,output);
    require(output.result==data::status::ok&&output.trace_size==1&&output.trace[0].source_alias.view()=="call"&&output.trace[0].client_session_label==2);
    std::string text;auto sink{fast_io::ostring_ref_std{&text}};ui::print(sink,output,9);
    require(text.find("source_alias=\"call\" client_session_label=2 client_features=7")!=text.npos);
    require(text.find("service.available")!=text.npos&&text.find("credential")==text.npos);
    // Worst-case detached/hostile text is escaped inside the original cap.
    std::string maximum(255,'\x01'),shorter(64,'\x02');output={};output.selected=data::selection::trace;output.result=data::status::ok;
    output.trace_size=data::maximum_trace_rows;
    for(auto& row:output.trace){row.module.assign(maximum);row.alias.assign(maximum);row.source_alias.assign(maximum);row.operation.assign(shorter);row.function.assign(shorter);
        row.client_features_observed=true;row.client_features=UINT64_MAX;row.sequence=row.monotonic_ns=row.instance=row.generation=row.client_session_label=UINT64_MAX;row.error=UINT32_MAX;row.backend=255;row.unresolved=true;}
    text.clear();ui::print(sink,output,UINT64_MAX);require(text.size()<=65536&&text.find('\\')!=text.npos&&text.find('\x01')==text.npos);
}
