// Detached formatter/store qualification; this does not execute a ROS gateway.
#include <uwvm2/uwvm/debugger/ros_state.h>
#include <fast_io_unit/string.h>
#include <fstream>
#include <string>
#include <stdexcept>

namespace data=uwvm2::imported::ros::debug;
namespace console=uwvm2::uwvm::debugger::ros_state;

static void emit(std::string const& directory,std::string const& name,data::view const& value)
{
    std::string text;
    fast_io::ostring_ref_std output{&text};console::print(output,value,19);
    std::ofstream file{directory+"/"+name+".packet",std::ios::binary};
    file.write(text.data(),static_cast<std::streamsize>(text.size()));
    if(!file) {throw std::runtime_error{"packet write failed"};}
}
static data::trace_record record()
{
    data::trace_record row{};row.sequence=7;row.monotonic_ns=100;
    row.backend=2;row.instance=3;row.generation=5;row.error=13;
    row.module.assign("main");row.function.assign("publish");row.operation.assign("topic.publish");
    row.alias.assign("pub");row.resource.assign("/topic_42");return row;
}
static data::view page(data::trace_record const& row)
{
    data::view value{};value.result=data::status::ok;value.selected=data::selection::trace;
    value.trace[0]=row;value.trace_size=1;value.total=1;value.next=row.sequence;return value;
}
int main(int argc,char** argv)
{
    if(argc!=2) {return 2;}
    std::string out{argv[1]};auto row{record()};emit(out,"public",page(row));
    char escaped[]{'m','\n',static_cast<char>(255),'"'};
    row.module.assign({escaped,sizeof escaped});emit(out,"escaped",page(row));
    row=record();row.resource.assign("https://secret.example/token");emit(out,"automatic-redaction",page(row));
    row=record();row.resource_redacted=true;emit(out,"explicit-redaction",page(row));
    row=record();row.resource.assign("/"+std::string(70,'n'));emit(out,"truncated",page(row));
    row=record();row.resource.assign("");row.result_known=false;row.error=0;emit(out,"unobserved",page(row));
    for(auto error:{0u,2u,9u,11u,13u,17u,22u,90u,95u,107u,110u,125u,UINT32_MAX})
    {row=record();row.error=error;emit(out,"errno-"+std::to_string(error),page(row));}

    // This is the actual bounded trace_store, not a synthetic reply sequence.
    // Repeated reads must not consume a row or mutate any page field.
    data::trace_store store{};
    for(unsigned i{};i<258;++i) {store.append(record());}
    data::request query{};query.selected=data::selection::trace;query.count=2;query.after=1;
    data::view value{};store.read(query,value);emit(out,"ring-first",value);
    store.read(query,value);emit(out,"ring-repeat",value);
    query.after=4;store.read(query,value);emit(out,"ring-next",value);
    data::trace_store empty{};query.after=39;empty.read(query,value);emit(out,"empty-after",value);
    query.after=UINT64_MAX;empty.read(query,value);emit(out,"empty-max-after",value);

    // Detached boundary rendering only; this does not claim to execute 2^64
    // appends or force the store's private sequence-exhaustion state.
    row=record();row.sequence=UINT64_MAX;row.error=UINT32_MAX;value=page(row);
    value.overwritten=UINT64_MAX;value.dropped_busy=UINT64_MAX;value.dropped_sequence=UINT64_MAX;
    emit(out,"saturated-loss",value);
    value={};value.result=data::status::ok;value.selected=data::selection::capabilities;
    emit(out,"capability",value);
    value={};value.result=data::status::ok;value.selected=data::selection::calls;
    value.interface_size=1;value.total=1;value.next=1;auto& call{value.interfaces[0]};
    call.backend=2;call.instance=3;call.generation=5;call.module.assign("main");call.alias.assign("pub");
    call.operation.assign("topic.publish");call.function.assign("publish");call.calls=1;call.errors=1;
    value.dropped_busy=1;value.dropped_full=2;value.dropped_labels=3;emit(out,"calls",value);
}
