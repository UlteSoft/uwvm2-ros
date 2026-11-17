/** @file
 * @brief Exercise the real bounded scene grammar/formatter and detached DTO.
 * @details No native provider is faked. These checks validate parser admission,
 * byte-safe metadata output, bounded pagination and default-deny hook absence.
 * Execute only inside the existing deployment test cgroup.
 */
#include <string>
#include <iostream>
#include <uwvm2/uwvm/debugger/scene_state.h>
namespace scene=uwvm2::uwvm::debugger::scene_state;
namespace data=uwvm2::imported::ros::scene::debug;
void check(bool condition,char const* name){if(!condition){std::cerr<<name<<'\n';std::abort();}}
int main(int argc,char** argv)
{
    scene::request request;scene::view value;
    scene::read(request,value);check(value.result==scene::status::not_selected&&value.size==0,"unselected hook");
    check(scene::parse("info scene authority environment=robot alias=log first=0 count=1 if-stop=11",request),"complete grammar");
    check(request.stop==11&&request.environment.view()=="robot"&&request.alias.view()=="log","exact fields");
    for(auto text:{"info scene revoke","info scene authority count=0","info scene authority count=9",
                   "info scene authority first=4097","info scene authority count=1 count=1","info scene authority fd=3",
                   "info scene resources if-stop=0","info scene resources first=-1","info scene resources first=+1",
                   "info scene resources environment=a/b","info scene authority alias=a\ncontinue"})
        check(!scene::parse(fast_io::string_view{text,std::char_traits<char>::length(text)},request),text);
    request={};request.count=1;value={};value.result=scene::status::ok;
    data::row row;row.environment.assign("robot");row.alias.assign("log");row.binding.assign("\"\n\xff");row.id=UINT64_MAX;
    data::add(request,value,row);data::add(request,value,row);
    check(value.size==1&&value.total==2&&value.next==1&&value.more,"bounded pagination");
    std::string wire;auto output=fast_io::ostring_ref_std{&wire};scene::print(output,value,11);
    check(wire.find("binding=\"\\x22\\x0a\\xff\"")!=wire.npos,"byte escaping");
    check(wire.find("id=18446744073709551615")!=wire.npos,"exact counter");
    check(wire.find("page total=2 next=1 more=1\n")!=wire.npos,"canonical footer");
    if(argc==2&&std::string_view(argv[1])=="--packet"){std::cout<<wire;return 0;}
    request={};request.selected=data::selection::resources;
    value={};value.result=scene::status::ok;value.selected=request.selected;
    row={};row.kind=data::row_kind::resource;row.type.assign("driver");row.binding.assign("registered-driver");
    row.environment.assign("robot");row.alias.assign("arm");row.resource.assign("left-arm");row.state.assign("failed");
    row.sequence=9007199254740993ULL;row.query_count=1;row.status_code=7;
    check(data::valid(row),"external driver uncertainty");data::add(request,value,row);wire.clear();scene::print(output,value,11);
    check(wire.find("invalid bounded snapshot")==wire.npos,"driver state formats");
    if(argc==2&&std::string_view(argv[1])=="--driver-packet"){std::cout<<wire;return 0;}
    row.armed=true;check(!data::valid(row),"driver transport cannot claim physical arming");row.armed=false;
    row.state.assign("ready");check(!data::valid(row),"uncertain driver cannot report ready");
    request={};request.selected=data::selection::resources;
    value={};value.result=scene::status::ok;value.selected=request.selected;
    row={};row.kind=data::row_kind::resource;row.type.assign("artifact");
    row.environment.assign("robot");row.alias.assign("weights");row.resource.assign("perception-model");row.binding.assign("sealed-input");
    row.id=UINT64_MAX;row.size_bytes=UINT64_MAX;row.immutable=true;
    data::add(request,value,row);wire.clear();scene::print(output,value,11);
    check(wire.find("size_bytes=18446744073709551615")!=wire.npos,"exact artifact byte size");
    check(wire.find("immutable=1")!=wire.npos,"immutable artifact guarantee");
    if(argc==2&&std::string_view(argv[1])=="--artifact-packet"){std::cout<<wire;return 0;}
    auto invalid_artifact=value;invalid_artifact.rows[0].immutable=false;wire.clear();scene::print(output,invalid_artifact,11);
    check(wire.find("invalid bounded snapshot")!=wire.npos&&wire.find("row ")==wire.npos,"inconsistent artifact withheld");
    invalid_artifact=value;invalid_artifact.rows[0].type.assign("guard");wire.clear();scene::print(output,invalid_artifact,11);
    check(wire.find("invalid bounded snapshot")!=wire.npos&&wire.find("row ")==wire.npos,"artifact fields cannot label scheduler resources");
    value={};value.result=scene::status::ok;value.selected=request.selected;
    row={};row.kind=data::row_kind::resource;row.type.assign("artifact.output");row.state.assign("staged");
    row.environment.assign("robot");row.alias.assign("cache");row.resource.assign("frames");row.binding.assign("volatile-output");
    row.id=UINT64_MAX;row.accepted_bytes=9007199254740993ULL;row.reserved_bytes=UINT64_MAX;row.retained_receipts=1;
    check(data::valid(row),"valid staged output");data::add(request,value,row);
    row.state.assign("published");row.published=true;check(data::valid(row),"volatile publication");data::add(request,value,row);
    row.type.assign("artifact.receipt");row.reserved_bytes=0;row.state.assign("aborted");row.published=false;
    check(data::valid(row),"abort receipt");data::add(request,value,row);
    row.state.assign("removed");check(data::valid(row),"remove receipt");data::add(request,value,row);
    row.state.assign("none");row.id=0;row.accepted_bytes=0;
    check(data::valid(row),"failed begin receipt");data::add(request,value,row);
    wire.clear();scene::print(output,value,11);
    check(wire.find("accepted_bytes=9007199254740993 reserved_bytes=18446744073709551615 retained_receipts=1")!=wire.npos,"exact output accounting");
    check(wire.find("state=\"published\"")!=wire.npos&&wire.find("published=1 durable=0")!=wire.npos,"publication is not durability");
    if(argc==2&&std::string_view(argv[1])=="--output-packet"){std::cout<<wire;return 0;}
    auto withheld=[&](data::row changed,char const* name)
    {
        auto malformed=value;malformed.rows[1]=changed;wire.clear();scene::print(output,malformed,11);
        check(wire.find("invalid bounded snapshot")!=wire.npos&&wire.find("row ")==wire.npos,name);
    };
    auto changed=value.rows[0];changed.durable=true;withheld(changed,"volatile cannot report durable");
    changed=value.rows[0];changed.state.assign("unknown");withheld(changed,"closed output state");
    changed=value.rows[0];changed.state.size=changed.state.bytes.size()+1;withheld(changed,"bounded state");
    changed=value.rows[0];changed.published=true;withheld(changed,"state publication mismatch");
    changed=value.rows[0];changed.id=0;withheld(changed,"live output needs identity");
    changed=value.rows[0];changed.retained_receipts=2;withheld(changed,"bounded last receipt count");
    changed=value.rows[0];changed.reserved_bytes=changed.accepted_bytes-1;withheld(changed,"retained allocation covers bytes");
    changed=value.rows[2];changed.reserved_bytes=1;withheld(changed,"receipt reserves no content");
    changed=value.rows[2];changed.retained_receipts=0;withheld(changed,"receipt must remain retained");
    changed=value.rows[2];changed.state.assign("staged");withheld(changed,"receipt cannot impersonate live output");
    changed=value.rows[2];changed.id=0;withheld(changed,"terminal receipt retains original identity");
    changed=value.rows[4];changed.id=1;withheld(changed,"absent receipt has no object");
    changed=value.rows[4];changed.accepted_bytes=1;withheld(changed,"absent receipt has no content");
    changed={};changed.accepted_bytes=1;withheld(changed,"grant cannot carry transaction metadata");
    value={};value.result=scene::status::ok;value.selected=request.selected;
    row={};row.kind=data::row_kind::resource;row.type.assign("lifecycle");row.state.assign("unconfigured");
    row.environment.assign("robot");row.alias.assign("managed");row.resource.assign("navigation");row.binding.assign("local-lifecycle");
    row.generation=1;row.open_handles=2;
    check(data::valid(row),"cached lifecycle primary state");data::add(request,value,row);
    row.state.assign("configuring");row.generation=9007199254740993ULL;row.transition=1;row.deadline_ns=INT64_MAX;row.callback_pending=true;
    check(data::valid(row),"queued lifecycle callback");data::add(request,value,row);
    row.callback_delivered=true;check(data::valid(row),"delivered lifecycle callback");data::add(request,value,row);
    row.state.assign("error_processing");row.callback_delivered=false;check(data::valid(row),"lifecycle error callback");data::add(request,value,row);
    row.state.assign("finalized");row.transition=row.deadline_ns=0;row.callback_pending=false;row.status_code=110;
    check(data::valid(row),"lifecycle expired completion");data::add(request,value,row);
    wire.clear();scene::print(output,value,11);
    check(wire.find("generation=9007199254740993 transition=1 deadline_ns=9223372036854775807")!=wire.npos,"exact lifecycle generation/deadline");
    check(wire.find("callback_pending=1 callback_delivered=1")!=wire.npos,"callback delivery metadata");
    if(argc==2&&std::string_view(argv[1])=="--lifecycle-packet"){std::cout<<wire;return 0;}
    changed=value.rows[0];changed.generation=0;withheld(changed,"lifecycle generation cannot be zero");
    changed=value.rows[0];changed.id=1;withheld(changed,"lifecycle row is not a handle");
    changed=value.rows[0];changed.binding.assign("native");withheld(changed,"closed lifecycle binding");
    changed=value.rows[0];changed.state.assign("unknown");withheld(changed,"closed lifecycle state");
    changed=value.rows[0];changed.callback_pending=true;withheld(changed,"primary cannot have pending callback");
    changed=value.rows[0];changed.callback_delivered=true;withheld(changed,"primary cannot have delivered callback");
    changed=value.rows[1];changed.callback_pending=false;withheld(changed,"intermediate must have callback");
    changed=value.rows[1];changed.transition=2;withheld(changed,"transition must match phase");
    changed=value.rows[1];changed.deadline_ns=0;withheld(changed,"pending deadline cannot be zero");
    changed=value.rows[1];changed.deadline_ns=UINT64_MAX;withheld(changed,"pending deadline signed bound");
    changed=value.rows[1];changed.status_code=110;withheld(changed,"pending callback cannot be completed");
    changed=value.rows[4];changed.status_code=9;withheld(changed,"closed completion status");
    changed=value.rows[4];changed.retained_receipts=1;withheld(changed,"lifecycle cannot carry output accounting");
    changed={};changed.generation=1;withheld(changed,"nonlifecycle cannot carry lifecycle metadata");
    value={};value.result=scene::status::ok;value.selected=request.selected;
    row={};row.kind=data::row_kind::resource;row.type.assign("buffer");row.binding.assign("sealed-buffer");
    row.environment.assign("robot");row.alias.assign("image");row.resource.assign("frames");
    row.members=2;row.open_handles=1;row.reserved_bytes=9007199254740993ULL;row.lease_count=UINT64_MAX;row.workspace_bytes=INT64_MAX;
    check(data::valid(row),"aggregate retained buffer accounting");data::add(request,value,row);
    wire.clear();scene::print(output,value,11);
    check(wire.find("lease_count=18446744073709551615 workspace_bytes=9223372036854775807")!=wire.npos,"exact buffer accounting");
    if(argc==2&&std::string_view(argv[1])=="--buffer-packet"){std::cout<<wire;return 0;}
    auto invalid_buffer=[&](data::row candidate,char const* name)
    {check(!data::valid(candidate),name);auto invalid=value;invalid.rows[0]=candidate;wire.clear();scene::print(output,invalid,11);
     check(wire.find("invalid bounded snapshot")!=wire.npos&&wire.find("row ")==wire.npos,name);};
    changed=row;changed.id=1;invalid_buffer(changed,"buffer identity withheld");
    changed=row;changed.binding.assign("native");invalid_buffer(changed,"closed buffer binding");
    changed=row;changed.open_handles=3;invalid_buffer(changed,"buffer handles bounded by retained objects");
    changed=row;changed.state.assign("sealed");invalid_buffer(changed,"aggregate is not one object state");
    changed=row;changed.accepted_bytes=1;invalid_buffer(changed,"buffer cannot carry output state");
    changed=row;changed.generation=1;invalid_buffer(changed,"buffer cannot carry lifecycle state");
    changed={};changed.lease_count=1;invalid_buffer(changed,"lease accounting limited to buffer");
    changed={};changed.workspace_bytes=1;invalid_buffer(changed,"workspace accounting limited to buffer");
    value={};value.result=scene::status::ok;value.selected=request.selected;
    row={};row.kind=data::row_kind::resource;row.type.assign("model");row.binding.assign("local-model");
    row.environment.assign("robot");row.alias.assign("brain");row.resource.assign("vision");
    row.open_handles=2;row.members=1;row.input_bytes=9007199254740993ULL;row.output_bytes=INT64_MAX;
    row.output_items=UINT64_MAX;row.workspace_bytes=INT64_MAX;
    check(data::valid(row),"model aggregate independent sessions/requests");data::add(request,value,row);
    wire.clear();scene::print(output,value,11);
    check(wire.find("input_bytes=9007199254740993 output_bytes=9223372036854775807 output_items=18446744073709551615")!=wire.npos,"exact model capacities");
    if(argc==2&&std::string_view(argv[1])=="--model-packet"){std::cout<<wire;return 0;}
    changed=row;changed.id=1;invalid_buffer(changed,"model request identifiers withheld");
    changed=row;changed.binding.assign("remote");invalid_buffer(changed,"model binding must match provider");
    changed=row;changed.state.assign("running");invalid_buffer(changed,"aggregate cannot impersonate request state");
    changed=row;changed.lease_count=1;invalid_buffer(changed,"model input bytes are not buffer pins");
    changed=row;changed.retained_receipts=1;invalid_buffer(changed,"model aggregate cannot carry transaction receipt");
    changed={};changed.input_bytes=1;invalid_buffer(changed,"input bytes limited to model row");
    changed={};changed.output_bytes=1;invalid_buffer(changed,"output bytes limited to model row");
    changed={};changed.output_items=1;invalid_buffer(changed,"output items limited to model row");
    value={};value.result=scene::status::ok;value.selected=request.selected;
    row={};row.kind=data::row_kind::resource;row.type.assign("transform");row.binding.assign("local-transform");
    row.environment.assign("robot");row.alias.assign("frames");row.resource.assign("approved-frames");
    row.open_handles=2;row.members=3;row.reserved_bytes=INT64_MAX;row.workspace_bytes=9007199254740993ULL;
    row.sample_count=9007199254740993ULL;row.query_count=UINT64_MAX;row.pending_queries=9007199254740993ULL;
    row.query_bytes=INT64_MAX;row.carrier_bytes=UINT64_MAX;row.retired=true;
    check(data::valid(row),"transform aggregate retains closed-session capacity");data::add(request,value,row);
    wire.clear();scene::print(output,value,11);
    check(wire.find("sample_count=9007199254740993 query_count=18446744073709551615 pending_queries=9007199254740993 query_bytes=9223372036854775807 carrier_bytes=18446744073709551615")!=wire.npos,"exact transform counters");
    if(argc==2&&std::string_view(argv[1])=="--transform-packet"){std::cout<<wire;return 0;}
    changed=row;changed.id=1;invalid_buffer(changed,"transform session/query identities withheld");
    changed=row;changed.binding.assign("native");invalid_buffer(changed,"transform binding is exact");
    changed=row;changed.open_handles=4;invalid_buffer(changed,"transform live sessions cannot exceed retained sessions");
    changed=row;changed.query_count=1;invalid_buffer(changed,"transform pending count cannot exceed requests");
    changed=row;changed.state.assign("ready");invalid_buffer(changed,"transform aggregate cannot expose query state");
    changed=row;changed.input_bytes=1;invalid_buffer(changed,"transform cannot carry model input accounting");
    changed=row;changed.lease_count=1;invalid_buffer(changed,"transform cannot carry buffer leases");
    changed=row;changed.deadline_ns=1;invalid_buffer(changed,"transform inspection cannot export a query deadline");
    changed=row;changed.accepted_bytes=1;invalid_buffer(changed,"transform cannot carry output transaction state");
    changed=row;changed.published=true;invalid_buffer(changed,"transform publication is not transaction publication");
    changed=row;changed.sequence=1;invalid_buffer(changed,"transform query sequences withheld");
    changed=row;changed.armed=true;invalid_buffer(changed,"transform aggregate cannot impersonate timer state");
    for(auto field:{&data::row::sample_count,&data::row::query_count,&data::row::pending_queries,&data::row::query_bytes,&data::row::carrier_bytes})
    {changed={};changed.*field=1;invalid_buffer(changed,"transform-only counters withheld on other rows");}
    value={};value.result=scene::status::ok;value.selected=request.selected;
    row={};row.kind=data::row_kind::resource;row.type.assign("log");row.binding.assign("private-log");
    row.environment.assign("robot");row.alias.assign("log");row.resource.assign("records");row.members=2;
    row.emitted_records=UINT64_MAX;row.exported_records=9007199254740993ULL;row.lost_records=9007199254740993ULL;
    row.reserved_bytes=INT64_MAX;row.retained_bytes=1024;row.copy_failures=UINT64_MAX;row.refused=UINT64_MAX;
    row.counters_saturated=true;row.busy=true;check(data::valid(row),"bounded log metadata");data::add(request,value,row);
    changed=row;changed.id=1;invalid_buffer(changed,"log record identities withheld");
    changed=row;changed.pending_page=true;invalid_buffer(changed,"log does not own diagnostic cursors");
    changed=row;changed.retained_bytes=UINT64_MAX;invalid_buffer(changed,"retained log bytes fit reservation");
    changed=row;changed.emitted_records=1;invalid_buffer(changed,"log exports cannot exceed emitted records");
    row={};row.kind=data::row_kind::resource;row.type.assign("diagnostic");row.binding.assign("local-diagnostic");
    row.environment.assign("robot");row.alias.assign("health");row.resource.assign("cached-counters");
    row.members=3;row.reserved_bytes=512;row.pending_page=true;row.busy=true;
    check(data::valid(row),"cached diagnostic metadata");data::add(request,value,row);
    wire.clear();scene::print(output,value,11);
    if(argc==2&&std::string_view(argv[1])=="--log-diagnostic-packet"){std::cout<<wire;return 0;}
    changed=row;changed.deadline_ns=1;invalid_buffer(changed,"diagnostic inspection omits expiry");
    changed=row;changed.refused=1;invalid_buffer(changed,"diagnostic cannot impersonate admission counters");
    changed=row;changed.counters_saturated=true;invalid_buffer(changed,"diagnostic does not expose sampled contents");
    for(auto field:{&data::row::emitted_records,&data::row::exported_records,&data::row::lost_records,&data::row::retained_bytes,&data::row::copy_failures})
    {changed=row;changed.*field=1;invalid_buffer(changed,"log counters excluded from diagnostic metadata");}
    value={};value.result=scene::status::ok;value.selected=request.selected;
    row={};row.kind=data::row_kind::resource;row.type.assign("recording");row.binding.assign("private-recording");
    row.environment.assign("robot");row.alias.assign("recording");row.resource.assign("private-records");row.state.assign("active");
    row.open_handles=3;row.members=2;row.reserved_bytes=INT64_MAX;row.retained_bytes=9007199254740993ULL;
    row.sample_count=UINT64_MAX;row.query_count=2;row.lease_count=4;
    check(data::valid(row),"private recording cached accounting");data::add(request,value,row);
    changed=row;changed.id=1;invalid_buffer(changed,"recording object identities withheld");
    changed=row;changed.retained_bytes=UINT64_MAX;invalid_buffer(changed,"sealed bytes must fit reserved capacity");
    changed=row;changed.members=4;invalid_buffer(changed,"sealed objects retain owned handles");
    changed=row;changed.published=true;invalid_buffer(changed,"recording does not assert durable publication");
    changed=row;changed.retired=true;invalid_buffer(changed,"recording cached state reflects retirement");
    row={};row.kind=data::row_kind::resource;row.type.assign("replay");row.binding.assign("private-replay");
    row.environment.assign("robot");row.alias.assign("replay");row.state.assign("active");row.open_handles=row.members=1;row.reserved_bytes=512;
    check(data::valid(row),"replay exposes counts without cursor");data::add(request,value,row);
    changed=row;changed.members=2;invalid_buffer(changed,"replay count equals retained owned handles");
    changed=row;changed.sample_count=1;invalid_buffer(changed,"replay position is withheld");
    row={};row.kind=data::row_kind::resource;row.type.assign("sensor");row.binding.assign("bounded-sensor");
    row.environment.assign("robot");row.alias.assign("sensor");row.state.assign("failed");row.status_code=5;
    row.open_handles=1;row.members=2;row.retained_bytes=128;row.reserved_bytes=1024;row.sample_count=3;row.lease_count=2;
    row.input_bytes=9007199254740993ULL;row.output_items=UINT64_MAX;row.workspace_bytes=512;row.busy=true;row.lost_records=9;row.counters_saturated=true;
    check(data::valid(row),"sensor has cached ingress counts and failure only");data::add(request,value,row);
    changed=row;changed.state.assign("active");invalid_buffer(changed,"sensor failure needs exact cached state");
    changed=row;changed.status_code=UINT64_MAX;invalid_buffer(changed,"sensor error is bounded");
    changed=row;changed.pending_page=true;invalid_buffer(changed,"sensor does not expose diagnostic cursor");
    row={};row.kind=data::row_kind::resource;row.type.assign("model.proposal");row.binding.assign("model-proposals");
    row.environment.assign("robot");row.alias.assign("proposal");row.state.assign("active");row.members=4;row.lease_count=2;
    row.sample_count=3;row.query_count=2;row.workspace_bytes=9007199254740993ULL;
    check(data::valid(row),"inert proposal aggregate has no approval token");data::add(request,value,row);
    changed=row;changed.open_handles=1;invalid_buffer(changed,"proposal sessions are not per-alias aggregates");
    changed=row;changed.deadline_ns=1;invalid_buffer(changed,"proposal freshness timestamp withheld");
    changed=row;changed.binding.assign("actuator");invalid_buffer(changed,"proposal inspection cannot impersonate actuation");
    wire.clear();scene::print(output,value,11);
    if(argc==2&&std::string_view(argv[1])=="--private-provider-packet"){std::cout<<wire;return 0;}
    value={};value.result=scene::status::ok;value.selected=request.selected;
    row={};row.kind=data::row_kind::resource;row.type.assign("communication");row.binding.assign("native-communication");
    row.environment.assign("robot");row.alias.assign("publisher");row.resource.assign("status");row.state.assign("ready");
    row.members=1;row.open_handles=1;row.retained_receipts=1;
    check(data::valid(row),"communication exposes only cached worker-local registry counts");data::add(request,value,row);
    changed=row;changed.id=1;invalid_buffer(changed,"communication omits native and guest identities");
    changed=row;changed.members=2;invalid_buffer(changed,"communication row describes exactly one binding");
    changed=row;changed.retained_receipts=2;invalid_buffer(changed,"one configured resource has at most one orphan");
    changed=row;changed.workspace_bytes=1;invalid_buffer(changed,"registry counters cannot claim provider workspace");
    changed=row;changed.deadline_ns=1;invalid_buffer(changed,"communication inspection cannot sample clocks");
    changed=row;changed.retired=true;invalid_buffer(changed,"cached retirement must match state");
    changed=row;changed.state.assign("safe");invalid_buffer(changed,"communication cannot report physical safety");
    changed=row;changed.binding.assign("native-action");check(data::valid(changed),"action shares the closed cached communication DTO");
    if(argc==2&&std::string_view(argv[1])=="--action-packet")
    {value={};value.result=scene::status::ok;value.selected=request.selected;data::add(request,value,changed);wire.clear();scene::print(output,value,11);std::cout<<wire;return 0;}
    changed=row;changed.binding.assign("native-graph");changed.retained_receipts=0;
    check(data::valid(changed),"graph exposes cached watch count without graph entities");
    auto graph_row=changed;changed.open_handles=2;invalid_buffer(changed,"one graph binding has at most one watch");
    changed=graph_row;changed.binding.assign("event-stream");changed.open_handles=2;
    check(data::valid(changed),"event binding counts multiple bounded streams");
    if(argc==2&&std::string_view(argv[1])=="--observation-packet")
    {value={};value.result=scene::status::ok;value.selected=request.selected;data::add(request,value,graph_row);data::add(request,value,changed);
        wire.clear();scene::print(output,value,11);std::cout<<wire;return 0;}
    row.state.assign("retired");row.retired=true;
    check(data::valid(row),"retired communication can retain fenced native cleanup debt");data::add(request,value,row);
    wire.clear();scene::print(output,value,11);
    if(argc==2&&std::string_view(argv[1])=="--communication-packet"){std::cout<<wire;return 0;}
    value={};value.result=scene::status::ok;value.selected=request.selected;
    row={};row.kind=data::row_kind::resource;row.type.assign("parameter");row.binding.assign("local-parameters");
    row.environment.assign("robot");row.alias.assign("tuning");row.resource.assign("controller-parameters");
    row.members=2;row.open_handles=1;row.sample_count=5;row.query_count=3;row.retained_bytes=9007199254740993ULL;
    row.workspace_bytes=UINT64_MAX;row.input_bytes=1024;row.output_items=4;
    check(data::valid(row),"parameter cached aggregate accounting");data::add(request,value,row);
    changed=row;changed.id=1;invalid_buffer(changed,"parameter scope identity withheld");
    changed=row;changed.open_handles=3;invalid_buffer(changed,"parameter retained scopes cover live scopes");
    changed=row;changed.retained_receipts=1;invalid_buffer(changed,"parameter receipt bytes are not receipt counts");
    changed=row;changed.state.assign("safe");invalid_buffer(changed,"parameter aggregate does not imply safety");
    changed=row;changed.deadline_ns=1;invalid_buffer(changed,"parameter inspection cannot expose expiry");
    changed=row;changed.binding.assign("remote");invalid_buffer(changed,"parameter aggregate is local");
    changed=row;changed.lease_count=1;invalid_buffer(changed,"parameter aggregate is not a buffer lease");
    wire.clear();scene::print(output,value,11);
    if(argc==2&&std::string_view(argv[1])=="--parameter-packet"){std::cout<<wire;return 0;}
    value={};value.result=scene::status::ok;value.selected=request.selected;
    row={};row.kind=data::row_kind::resource;row.environment.assign("robot");row.alias.assign("camera");
    row.type.assign("media");row.binding.assign("immutable-buffer-media");row.state.assign("active");
    row.open_handles=row.members=1;row.output_items=9007199254740993ULL;row.exported_records=9007199254740992ULL;
    check(data::valid(row),"media preserves exact production counters");data::add(request,value,row);
    changed=row;changed.exported_records=row.output_items+1;invalid_buffer(changed,"delivered frames cannot exceed produced frames");
    row={};row.kind=data::row_kind::resource;row.environment.assign("robot");row.alias.assign("route");
    row.type.assign("bridge");row.binding.assign("topic-forwarder");row.state.assign("outcome_unknown");
    row.members=row.sample_count=row.query_count=1;row.lost_records=9007199254740993ULL;
    check(data::valid(row),"bridge exposes retained unknown reservation");data::add(request,value,row);
    changed=row;changed.sample_count=0;invalid_buffer(changed,"unknown bridge retains its pending item");
    row={};row.kind=data::row_kind::resource;row.environment.assign("robot");row.alias.assign("drive");
    row.type.assign("device");row.binding.assign("framed-scalar");row.state.assign("stop_accepted");
    row.platform.assign("uwvm_ros_v2.platform_extension.fixture.v1");row.platform_enabled=true;
    check(data::valid(row),"receiver stop acceptance is not physical stop");data::add(request,value,row);
    changed=row;changed.platform.assign("");invalid_buffer(changed,"child switch requires module identity");
    changed=row;changed.platform.assign("uwvm_ros_v2.platform_extension.fixture.v01");invalid_buffer(changed,"child ABI name is canonical");
    changed=row;changed.platform.truncated=true;invalid_buffer(changed,"child identity cannot be truncated");
    changed=row;changed.state.assign("stopped");invalid_buffer(changed,"physical stopped claim withheld");
    changed=row;changed.armed=true;invalid_buffer(changed,"stopped lane is not ready for commands");
    row.type.assign("control");row.binding.assign("framed-control");row.generation=9007199254740993ULL;row.sequence=UINT64_MAX;
    check(data::valid(row),"exact controller epoch and phase sequence");data::add(request,value,row);
    changed=row;changed.open_handles=2;invalid_buffer(changed,"one controller per admitted binding");
    row={};row.kind=data::row_kind::resource;row.environment.assign("robot");row.alias.assign("worker");
    row.type.assign("component");row.binding.assign("isolated-roles");row.state.assign("outcome_unknown");
    row.exit_observed=true;row.sample_count=2;row.query_count=1;row.output_items=9007199254740993ULL;
    check(data::valid(row),"observed process exit retains downstream uncertainty");data::add(request,value,row);
    changed=row;changed.members=1;invalid_buffer(changed,"exit observation cannot coexist with a live process");
    changed=row;changed.type.assign("device");invalid_buffer(changed,"process exit is not device evidence");
    changed=row;changed.state.assign("exited");changed.exit_observed=false;invalid_buffer(changed,"exit requires observed kernel evidence");
    changed=row;changed.state.assign("failed");invalid_buffer(changed,"failure requires retained cause");
    changed.status_code=4;check(data::valid(changed),"channel fault survives exit");
    changed.status_code=10;invalid_buffer(changed,"unknown component fault is refused");
    changed.status_code=4;changed.state.assign("ready");invalid_buffer(changed,"fault cannot advertise readiness");
    wire.clear();scene::print(output,value,11);
    check(wire.find("exit_observed=1")!=wire.npos,"process exit field emitted");
    if(argc==2&&std::string_view(argv[1])=="--robot-packet"){std::cout<<wire;return 0;}
    value.result=scene::status::forbidden;wire.clear();scene::print(output,value,11);
    check(wire.find("row ")==wire.npos,"refused rows withheld");
    request={};request.environment.assign(std::string(256,'a'));check(!scene::valid(request),"truncated request refused");
    std::cout<<"PASS scene grammar, bounded metadata, pagination, byte escaping and absent-hook default denial\n";
}
