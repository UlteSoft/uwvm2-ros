/** @file
 * @brief Bounded immutable-buffer media execution with independent source/output authority.
 * @details The compiled raw image codecs copy packed RGB/BGR/mono bytes or swap
 * RGB/BGR channels. They are complete content validators, not native plugin
 * callbacks. A stream binds one exact sealed input resource and output profile;
 * it cannot create a listener, choose a codec library or send data externally.
 * Source data.export and buffer.read and output create/write/seal/read rights
 * are independent of media authority. Other media transports require another
 * explicitly qualified binding, never an implicit WASI or socket fallback.
 *
 * All storage is reserved from finite deployment capacities and uses recoverable
 * project fast_io containers. Normal submissions and close have separate next
 * keys/replay records. Whole immutable outputs survive failed delivery; guest
 * callbacks execute without provider or authority locks. Revocation fences new
 * work and retires undelivered outputs without closing unrelated input handles.
 */
#pragma once
#include "authority.h"
#include "buffers.h"
#include "robot_provider_contract.h"
#include "robot_provider_response.h"
#include "proposal_policy.h"
#include "proposal_workspace.h"
#include <uwvm2/utils/container/function_ref.h>

namespace uwvm2::imported::ros::scene
{
    class media_dispatch
    {
    public:
        using operation=robot_contract::operation;using bytes=wire::bytes;
        using sink=::uwvm2::utils::container::function_ref<bool(bytes)>;
        using cancellation=::uwvm2::utils::container::function_ref<bool()>;
        enum class codec:std::uint8_t {copy,swap_red_blue};
        /** @brief Immutable raw image support; dimensions are exact, not untrusted expansion claims. */
        struct plan
        {
            buffers::profile selected,input_profile,output_profile;
            buffers::text<255> alias,input_alias,output_alias;
            codec transform{};buffers::color_space input_color{},output_color{};
            std::uint64_t width{},height{},channels{},streams{},frames{},storage_bytes{},workspace_bytes{},workspace_calls{},deadline_ns{};
        };
        struct limits {std::uint64_t bindings{},storage_bytes{};};
        struct inspection {std::uint64_t streams{},retained_streams{},pending{},retained_bytes{},workspace_bytes{},workspace_calls{},completed{},delivered{};bool retired{};};
        using workspace_lease=proposals::workspace::lease;
    private:
        struct stream
        {
            handle id{},source_cap{},source{},output_cap{},key{},close_key{},sequence{};
            buffers::digest digest{},close_digest{};robot_contract::receipt last,closed;
            bool published{},busy{},closing{};
        };
        struct frame
        {
            handle ticket{},stream{},output{},input{},sequence{};
            robot_contract::receipt receipt;
            buffers::registry::sealed_lease input_pin,output_pin;
            bool ready{},delivering{};
        };
        struct binding
        {
            plan policy;handle generation{};mutable std::mutex mutex;
            ::uwvm2::utils::container::std_vector<stream> streams;
            ::uwvm2::utils::container::std_vector<frame> frames;
            proposals::workspace workspace;std::uint64_t completed{},delivered{};bool retired{};
        };
        std::shared_ptr<authority> permissions_;buffers::registry& buffers_;
        handle generation_{};limits limits_;mutable std::mutex mutex_;
        ::uwvm2::utils::container::std_vector<std::shared_ptr<binding>> bindings_;
        std::uint64_t storage_{};bool ready_{},retired_{};
        using clock_function=std::uint64_t(*)()noexcept;
        clock_function clock_{};
#if defined(__linux__)
        ::pid_t process_{::getpid()};
#endif
        bool same_process()const noexcept
        {
#if defined(__linux__)
            return process_==::getpid();
#else
            return true;
#endif
        }
        static bool is_media(operation op)noexcept
        {return op>=operation::media_open&&op<=operation::media_close;}
        static std::string_view action(operation op)noexcept
        {return robot_contract::operations[static_cast<std::size_t>(op)].name;}
        static bool profile(robot_contract::profile const& left,buffers::profile const& right)noexcept
        {return left.identity==right.identity.view()&&left.digest.size()==32&&std::equal(left.digest.begin(),left.digest.end(),right.sha256.begin());}
        static bool write_profile(wire::writer& out,buffers::profile const& value)noexcept
        {return out.text(value.identity.view())&&out.blob(value.sha256);}
        static bool write_receipt(wire::writer& out,robot_contract::receipt const& value)noexcept
        {return out.u64(value.request)&&out.text(value.state)&&out.text(value.effect)&&out.u64(value.provider_generation)&&out.text(value.reason);}
        std::uint64_t now()const noexcept{return clock_?clock_():static_cast<std::uint64_t>(resources::steady_now());}
        std::shared_ptr<binding> named(std::string_view alias)const noexcept
        {for(auto const& row:bindings_)if(row->policy.alias.view()==alias) { return row; }return {};}
        error select(caller who,operation op,handle cap,std::size_t extent,std::shared_ptr<binding>& row,bool check_budget)
        {
            if(!same_process()||!permissions_||!is_media(op)) { return error::denied; }
            lease held;auto result=check_budget?permissions_->preflight(who,cap,action(op),extent,held):permissions_->admit(who,cap,action(op),0,held,false);
            if(result!=error::success) { return result; }
            auto const& grant=held.specification();if(grant.kind!=purpose::media||grant.binding!="immutable-buffer-media") { return error::denied; }
            std::lock_guard lock{mutex_};if(!ready_||retired_||!(row=named(grant.alias))) { return error::denied; }
            return error::success;
        }
        static stream* owned(binding& row,handle id)noexcept
        {for(auto& value:row.streams)if(id&&value.id==id&&value.published) { return &value; }return nullptr;}
        /** @brief Atomically validate the exact original input and output grants.
         * @details Validation performs no charging. Charge is consumed only by
         * the eventual successful publication, after allocations have succeeded.
         */
        error admission(caller who,handle cap,operation op,binding const& row,stream const& selected,
                        std::uint64_t transfer,std::uint64_t input_bytes,group_lease& held,bool charge=false,bool budget=false)
        {
            bool submitting=op==operation::media_submit;
            std::array claims{
                admission_claim{cap,action(op),transfer,false,false},
                admission_claim{selected.source_cap,"buffer.read",submitting?input_bytes:0,false,!submitting},
                admission_claim{selected.source_cap,"data.export",0,true,true},
                admission_claim{selected.output_cap,"buffer.create",0,false,!submitting},
                admission_claim{selected.output_cap,"buffer.write",submitting?input_bytes:0,false,!submitting},
                admission_claim{selected.output_cap,"buffer.seal",0,false,!submitting},
                admission_claim{selected.output_cap,"buffer.read",0,false,true}};
            auto result=permissions_->admit_group(who,claims,held,charge,budget);if(result!=error::success) { return result; }
            auto input=held.specification(1),output=held.specification(3),primary=held.specification(0);
            if(!primary||primary->kind!=purpose::media||primary->binding!="immutable-buffer-media"||primary->alias!=row.policy.alias.view()||
                !input||input->kind!=purpose::buffer||input->binding!="sealed-buffer"||input->alias!=row.policy.input_alias.view()||
                !output||output->kind!=purpose::buffer||output->binding!="sealed-buffer"||output->alias!=row.policy.output_alias.view())return error::denied;
            return !charge||held.consume(who,claims)?error::success:error::denied;
        }
        static bool result_frame(wire::writer& out,binding const& row,frame const& item,stream const& selected)noexcept
        {
            auto const& p=row.policy;auto width=p.width*p.channels;
            return write_receipt(out,item.receipt)&&out.u64(item.input)&&out.u64(row.generation)&&out.u64(item.sequence)&&
                out.boolean(false)&&out.boolean(false)&&out.boolean(false)&&out.u64(0)&&
                write_profile(out,p.output_profile)&&out.text("image")&&out.u64(item.sequence)&&out.u64(p.width)&&out.u64(p.height)&&
                out.u64(p.channels)&&out.u64(0)&&out.u64(1)&&out.u64(0)&&out.u64(width)&&out.u64(width)&&out.u64(p.height)&&
                out.u64(selected.output_cap)&&out.u64(item.output)&&out.u64(0);
        }
        /** @brief Retire only internally retained undelivered buffers; input handles stay untouched. */
        void cleanup(binding& row,handle id)noexcept
        {
            for(auto& item:row.frames)if(item.stream==id&&!item.delivering)
            {
                auto output=item.output;item={};
                if(output)(void)buffers_.close(row.policy.output_alias.view(),output);
            }
        }
        /** @brief Undo a reserved submission on every error, including allocation failure. */
        struct pending
        {
            media_dispatch& owner;binding& row;handle stream_id{},ticket{},output{};bool committed{};
            ~pending()
            {
                if(committed) { return; }
                {std::lock_guard lock{row.mutex};if(auto value=owned(row,stream_id))value->busy=false;
                    for(auto& item:row.frames)if(item.ticket==ticket)item={};}
                if(output)(void)owner.buffers_.close(row.policy.output_alias.view(),output);
            }
        };
        static bool layout_matches(buffers::layout const& value,plan const& p)noexcept
        {
            return value.schema==p.input_profile&&value.element==buffers::element_type::uint8&&value.order==buffers::byte_order::little&&
                value.color==p.input_color&&value.rank==3&&value.shape[0]==p.height&&value.shape[1]==p.width&&value.shape[2]==p.channels&&
                value.strides[0]==p.width*p.channels&&value.strides[1]==p.channels&&value.strides[2]==1&&value.bytes==p.width*p.height*p.channels;
        }
        static bool frame_matches(robot_contract::frame const& value,plan const& p)noexcept
        {
            if(!profile(value.layout,p.input_profile)||value.kind!=robot_contract::frame_kind::image||value.width!=p.width||
                value.height!=p.height||value.channels!=p.channels||value.samples||value.planes.count!=1)return false;
            robot_contract::decoder in{value.planes.encoded,{value.planes.encoded.size(),0,0,0}};robot_contract::plane plane;
            return in.image_plane(plane)&&in.done()&&!plane.offset&&plane.rows==p.height&&plane.stride==p.width*p.channels&&plane.row_bytes==plane.stride;
        }
    public:
        media_dispatch(std::shared_ptr<authority> permissions,buffers::registry& buffers,handle generation,limits capacity,clock_function clock=nullptr)
            :permissions_(std::move(permissions)),buffers_(buffers),generation_(generation),limits_(capacity),clock_(clock){}
        ~media_dispatch(){retire();}
        /** @brief Compute complete finite persistent reservation with overflow checked before allocation. */
        static bool required_storage(plan const& p,std::uint64_t& result)noexcept
        {
            if(!identifier(p.alias.view())||!identifier(p.input_alias.view())||!identifier(p.output_alias.view())||
                !proposals::envelope_profile(p.selected)||!proposals::envelope_profile(p.input_profile)||!proposals::envelope_profile(p.output_profile)||
                !p.width||!p.height||(p.channels!=1&&p.channels!=3)||p.width>buffers::maximum_transfer/p.channels||
                p.height>buffers::maximum_transfer/(p.width*p.channels)||p.transform>codec::swap_red_blue||
                !p.deadline_ns||p.deadline_ns>INT64_MAX||p.workspace_bytes>INT64_MAX||p.workspace_calls>1024)return false;
            if(p.channels==1&&(p.input_color!=buffers::color_space::mono||p.output_color!=buffers::color_space::mono||p.transform!=codec::copy)) { return false; }
            if(p.channels==3&&((p.input_color!=buffers::color_space::rgb&&p.input_color!=buffers::color_space::bgr)||
                (p.output_color!=buffers::color_space::rgb&&p.output_color!=buffers::color_space::bgr)||
                ((p.transform==codec::copy)!=(p.input_color==p.output_color))))return false;
            if(p.streams>(INT64_MAX-sizeof(binding))/sizeof(stream)) { return false; }
            auto size=sizeof(binding)+p.streams*sizeof(stream);if(p.frames>(INT64_MAX-size)/sizeof(frame)) { return false; }
            result=size+p.frames*sizeof(frame);return result<=SIZE_MAX&&p.storage_bytes<=INT64_MAX;
        }
        /** @brief Freeze compiled codec support and reserve registry storage; no authority is created. */
        error bind(plan const& policy)
        {
            if(!same_process()) { return error::denied; }
            std::uint64_t needed{};if(!generation_||!required_storage(policy,needed)) { return error::invalid; }
            std::lock_guard lock{mutex_};if(ready_||retired_) { return error::denied; }if(named(policy.alias.view())) { return error::conflict; }
            if(bindings_.size()>=limits_.bindings||needed>policy.storage_bytes||storage_>limits_.storage_bytes||needed>limits_.storage_bytes-storage_) { return error::too_large; }
            try
            {
                auto row=::uwvm2::utils::container::make_shared<binding>();row->policy=policy;row->generation=generation_;
                row->streams.resize(static_cast<std::size_t>(policy.streams));row->frames.resize(static_cast<std::size_t>(policy.frames));
                auto status=row->workspace.configure(policy.workspace_bytes,policy.workspace_calls);if(status!=error::success) { return status; }
                bindings_.push_back(std::move(row));storage_+=needed;return error::success;
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(std::length_error const&){return error::too_large;}
        }
        error prepare()noexcept
        {if(!same_process()) { return error::denied; }std::lock_guard lock{mutex_};if(ready_||retired_) { return error::denied; }ready_=true;return error::success;}
        error preflight(caller who,operation op,handle cap,std::size_t extent)
        {if(extent>4096) { return error::too_large; }std::shared_ptr<binding> row;return select(who,op,cap,extent,row,true);}
        error reserve_workspace(caller who,operation op,handle cap,std::size_t extent,workspace_lease& output)
        {std::shared_ptr<binding> row;auto result=select(who,op,cap,extent,row,true);return result==error::success?row->workspace.reserve(extent,output):result;}
        /** @brief Execute one canonical media operation; callbacks run with all internal locks released. */
        error invoke(caller who,operation op,handle cap,bytes input,std::size_t capacity,std::size_t& required,sink deliver,cancellation cancelled={})
        {
            required=0;if(!deliver) { return error::invalid; }auto status=preflight(who,op,cap,input.size());if(status!=error::success) { return status; }
            robot_contract::request request;if(!robot_contract::decode(op,input,{4096,4096,255,16},request)) { return error::invalid; }
            std::shared_ptr<binding> row;status=select(who,op,cap,input.size(),row,false);if(status!=error::success) { return status; }
            auto check=[&](){if(cancelled&&cancelled()) { return error::cancelled; }std::shared_ptr<binding> current;
                auto s=select(who,op,cap,0,current,false);return s==error::success&&current!=row?error::denied:s;};
            auto receipt_delivery=[&](robot_contract::receipt const& receipt)->error
            {std::array<std::uint8_t,256> buffer;wire::writer out{buffer};if(!write_receipt(out,receipt)) { return error::io; }required=out.size();
                if(capacity<required) { return error::too_large; }auto s=check();if(s!=error::success) { return s; }
                if(!deliver(out.written())) { return error::io; }return check();};
            auto bytes_per_frame=row->policy.width*row->policy.height*row->policy.channels;
            try
            {
                workspace_lease scratch;status=row->workspace.reserve(0,scratch);if(status!=error::success) { return status; }
                if(op==operation::media_open)
                {
                    required=8;if(capacity<required) { return error::too_large; }if(!profile(request.selected,row->policy.selected)||request.has_egress) { return error::denied; }
                    stream value;value.source_cap=request.source.capability;value.source=request.source.resource;value.output_cap=request.output_capability;
                    group_lease granted;status=admission(who,cap,op,*row,value,input.size()+required,0,granted,false,true);if(status!=error::success) { return status; }
                    buffers::information source;status=buffers_.describe(row->policy.input_alias.view(),value.source,source);
                    if(status!=error::success) { return status; }if(!source.sealed||!layout_matches(source.value,row->policy)) { return error::denied; }
                    auto maximum=granted.specification(0)->max_resources;granted.release();
                    {std::lock_guard lock{row->mutex};if(row->retired) { return error::denied; }std::uint64_t used{};stream* slot{};
                        for(auto& candidate:row->streams)if(candidate.id)++used;else if(!slot)slot=&candidate;
                        if(!slot||used>=maximum) { return error::again; }value.id=resources::allocate_identity();if(!value.id) { return error::too_large; }*slot=value;}
                    status=admission(who,cap,op,*row,value,input.size()+required,0,granted,true);granted.release();
                    bool copied{};if(status==error::success&&check()==error::success)
                    {std::array<std::uint8_t,8> result;wire::writer out{result};out.u64(value.id);try{copied=deliver(out.written());}catch(...){copied=false;}}
                    auto current=check();if(current==error::success)current=admission(who,cap,op,*row,value,0,0,granted,false);
                    {std::lock_guard lock{row->mutex};for(auto& slot:row->streams)if(slot.id==value.id)
                        {if(!copied||current!=error::success||row->retired)slot={};else slot.published=true;}}
                    return status!=error::success?status:current!=error::success?current:!copied?error::io:error::success;
                }
                stream selected;{std::lock_guard lock{row->mutex};auto value=owned(*row,request.resource);if(!value) { return error::bad_handle; }selected=*value;}
                if(op==operation::media_close)
                {
                    auto digest=buffers::hash(input);robot_contract::receipt result;bool replay{};
                    {std::lock_guard lock{row->mutex};auto value=owned(*row,request.resource);if(!value) { return error::bad_handle; }
                        if(request.key.sequence==value->close_key){if(digest!=value->close_digest) { return error::conflict; }result=value->closed;replay=true;}}
                    if(replay) { return receipt_delivery(result); }
                    result={1,row->generation,"succeeded","committed",""};auto measure=wire::writer::measuring(256);write_receipt(measure,result);required=measure.size();
                    if(capacity<required) { return error::too_large; }auto current=now();if(current>=request.deadline||request.deadline-current>row->policy.deadline_ns) { return error::timeout; }
                    status=check();if(status!=error::success) { return status; }
                    lease allowed;status=permissions_->admit(who,cap,action(op),input.size()+required,allowed,true);if(status!=error::success) { return status; }
                    {std::lock_guard lock{row->mutex};auto value=owned(*row,request.resource);if(!value||row->retired) { return error::denied; }
                        if(value->close_key==UINT64_MAX||request.key.sequence!=value->close_key+1) { return error::conflict; }
                        result.request=resources::allocate_identity();if(!result.request) { return error::too_large; }
                        value->closing=true;value->close_key=request.key.sequence;value->close_digest=digest;value->closed=result;cleanup(*row,value->id);}
                    allowed.release();return receipt_delivery(result);
                }
                if(op==operation::media_submit)
                {
                    auto digest=buffers::hash(input);robot_contract::receipt result;bool replay{};
                    {std::lock_guard lock{row->mutex};auto value=owned(*row,request.resource);if(!value) { return error::bad_handle; }
                        if(value->key==request.key.sequence){if(digest!=value->digest) { return error::conflict; }result=value->last;replay=true;}}
                    if(replay) { return receipt_delivery(result); }
                    result={1,row->generation,"succeeded","committed",""};auto measure=wire::writer::measuring(256);write_receipt(measure,result);required=measure.size();
                    if(capacity<required) { return error::too_large; }
                    if(request.buffer.capability!=selected.source_cap||request.buffer.resource!=selected.source||!frame_matches(request.media,row->policy)) { return error::denied; }
                    auto current=now();if(current>=request.deadline||request.deadline-current>row->policy.deadline_ns) { return error::timeout; }
                    group_lease granted;status=admission(who,cap,op,*row,selected,input.size()+required,bytes_per_frame,granted,false,true);if(status!=error::success) { return status; }
                    buffers::registry::sealed_lease pin;status=buffers_.pin(row->policy.input_alias.view(),selected.source,pin);if(status!=error::success) { return status; }
                    granted.release();
                    handle ticket{};
                    {std::lock_guard lock{row->mutex};auto value=owned(*row,request.resource);if(!value||row->retired) { return error::denied; }
                        if(value->closing) { return error::cancelled; }if(value->busy) { return error::again; }
                        if(value->key==UINT64_MAX||request.key.sequence!=value->key+1||value->sequence==UINT64_MAX||request.media.sequence!=value->sequence+1) { return error::conflict; }
                        frame* slot{};for(auto& item:row->frames)if(!item.ticket){slot=&item;break;}if(!slot) { return error::again; }
                        ticket=resources::allocate_identity();if(!ticket) { return error::too_large; }slot->ticket=ticket;slot->stream=value->id;value->busy=true;}
                    pending undo{*this,*row,selected.id,ticket};
                    status=scratch.grow(bytes_per_frame);if(status!=error::success) { return status; }
                    byte_buffer content;content.resize_for_overwrite(static_cast<std::size_t>(bytes_per_frame));buffers::layout format;
                    status=admission(who,cap,op,*row,selected,0,0,granted,false);if(status!=error::success) { return status; }
                    error content_status=error::success;
                    status=pin.visit([&](bytes bytes,buffers::layout const& actual,buffers::digest const&)
                    {if(!layout_matches(actual,row->policy)||bytes.size()!=bytes_per_frame){content_status=error::denied;return false;}
                        format=actual;std::copy(bytes.begin(),bytes.end(),content.writable().begin());return true;});granted.release();
                    if(content_status!=error::success) { return content_status; }if(status!=error::success) { return status; }
                    if(row->policy.transform==codec::swap_red_blue)
                    {auto data=content.writable();for(std::size_t n{};n<data.size();n+=3)std::swap(data[n],data[n+2]);}
                    format.schema=row->policy.output_profile;format.color=row->policy.output_color;
                    status=check();if(status!=error::success) { return status; }if(now()>=request.deadline) { return error::timeout; }
                    status=admission(who,cap,op,*row,selected,input.size()+required,bytes_per_frame,granted,false,true);if(status!=error::success) { return status; }
                    buffers::registry::sealed_creation staged;
                    status=buffers_.stage_sealed(row->policy.output_alias.view(),format,granted.specification(3)->max_resources,content.view(),staged);
                    if(status!=error::success) { return status; }
                    buffers::registry::sealed_lease output_pin;
                    granted.release();status=check();if(status!=error::success) { return status; }
                    status=admission(who,cap,op,*row,selected,input.size()+required,bytes_per_frame,granted,false,true);if(status!=error::success) { return status; }
                    {std::lock_guard lock{row->mutex};auto value=owned(*row,selected.id);if(!value||value->closing||row->retired) { return error::cancelled; }
                        if(now()>=request.deadline) { return error::timeout; }frame* slot{};for(auto& item:row->frames)if(item.ticket==ticket)slot=&item;if(!slot) { return error::cancelled; }
                        group_lease committed;
                        status=admission(who,cap,op,*row,selected,input.size()+required,bytes_per_frame,committed,true);if(status!=error::success) { return status; }
                        undo.output=staged.identity();status=buffers_.publish_sealed(staged,output_pin);if(status!=error::success) { return status; }
                        result.request=ticket;slot->output=undo.output;slot->input=selected.source;slot->sequence=request.media.sequence;slot->receipt=result;
                        slot->input_pin=std::move(pin);slot->output_pin=std::move(output_pin);slot->ready=true;
                        value->key=request.key.sequence;value->digest=digest;value->last=result;value->sequence=request.media.sequence;value->busy=false;
                        if(row->completed!=UINT64_MAX){++row->completed;}undo.committed=true;}
                    granted.release();return receipt_delivery(result);
                }
                // The only remaining operation is nonconsuming-on-failure take.
                frame description;handle ticket{};
                {std::lock_guard lock{row->mutex};auto value=owned(*row,selected.id);if(!value||row->retired) { return error::denied; }if(value->closing) { return error::cancelled; }
                    frame* first{};for(auto& item:row->frames)if(item.stream==selected.id&&item.ready&&(!first||item.ticket<first->ticket))first=&item;
                    if(!first||first->delivering) { return error::again; }description.ticket=ticket=first->ticket;description.stream=first->stream;
                    description.output=first->output;description.input=first->input;description.sequence=first->sequence;description.receipt=first->receipt;}
                std::array<std::uint8_t,1024> output;wire::writer out{output};if(!result_frame(out,*row,description,selected)) { return error::io; }
                required=out.size();if(capacity<required) { return error::too_large; }
                group_lease granted;status=admission(who,cap,op,*row,selected,input.size()+required,0,granted,false,true);if(status!=error::success) { return status; }
                {std::lock_guard lock{row->mutex};if(row->retired) { return error::denied; }auto value=owned(*row,selected.id);if(!value||value->closing) { return error::cancelled; }
                    bool found{};for(auto& item:row->frames)if(item.ticket==ticket){if(item.delivering) { return error::again; }item.delivering=true;found=true;}if(!found) { return error::again; }}
                granted.release();bool copied{};auto current=check();if(current==error::success)try{copied=deliver(out.written());}catch(...){copied=false;}
                if(copied)current=check();
                if(copied&&current==error::success)current=admission(who,cap,op,*row,selected,input.size()+required,0,granted,true);
                {std::lock_guard lock{row->mutex};auto value=owned(*row,selected.id);bool alive=value&&!value->closing&&!row->retired;
                    for(auto& item:row->frames)if(item.ticket==ticket)
                    {item.delivering=false;if(copied&&current==error::success&&alive){item={};if(row->delivered!=UINT64_MAX)++row->delivered;}else if(!alive)cleanup(*row,selected.id);}
                    if(!alive&&current==error::success)current=error::cancelled;}
                return current!=error::success?current:copied?error::success:error::io;
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(std::length_error const&){return error::too_large;}catch(...){return error::io;}
        }
        /** @brief Explicit logical handle retirement; borrowed data remains accounted by its owner. */
        error close(caller who,handle cap,handle id)
        {
            if(!same_process()) { return error::denied; }lease held;auto status=permissions_->admit(who,cap,"resource.close",0,held,false);if(status!=error::success) { return status; }
            auto const& g=held.specification();if(g.kind!=purpose::media||g.binding!="immutable-buffer-media") { return error::denied; }
            std::shared_ptr<binding> row;{std::lock_guard lock{mutex_};row=named(g.alias);}if(!row) { return error::denied; }
            std::lock_guard lock{row->mutex};auto value=owned(*row,id);if(!value) { return error::bad_handle; }value->closing=true;cleanup(*row,id);
            if(value->busy) { return error::again; }for(auto const& item:row->frames){if(item.stream==id) { return error::again; }}*value={};return error::success;
        }
        error inspect(std::string_view alias,inspection& output)const noexcept
        {
            if(!same_process()) { return error::denied; }std::shared_ptr<binding> row;
            {std::unique_lock lock{mutex_,std::try_to_lock};if(!lock.owns_lock()) { return error::again; }row=named(alias);}if(!row) { return error::denied; }
            std::unique_lock lock{row->mutex,std::try_to_lock};if(!lock.owns_lock()) { return error::again; }
            inspection value;value.retired=row->retired;value.completed=row->completed;value.delivered=row->delivered;value.retained_bytes=row->policy.storage_bytes;
            for(auto const& stream:row->streams){value.streams+=bool(stream.id&&stream.published);value.retained_streams+=bool(stream.id);}
            for(auto const& item:row->frames)value.pending+=bool(item.ticket);
            row->workspace.inspect(value.workspace_bytes,value.workspace_calls);output=value;return error::success;
        }
        void revoke(std::string_view alias)noexcept
        {
            if(!same_process()) { return; }
            std::lock_guard lock{mutex_};
            for(auto& row:bindings_)
                if(row->policy.alias.view()==alias||row->policy.input_alias.view()==alias||row->policy.output_alias.view()==alias)
                {
                    std::lock_guard owned{row->mutex};row->retired=true;row->workspace.retire();
                    for(auto& stream:row->streams)if(stream.id){stream.closing=true;cleanup(*row,stream.id);}
                }
        }
        void retire()noexcept
        {
            if(!same_process()) { return; }std::lock_guard lock{mutex_};if(retired_) { return; }retired_=true;
            for(auto& row:bindings_){std::lock_guard owned{row->mutex};row->retired=true;row->workspace.retire();for(auto& stream:row->streams)if(stream.id){stream.closing=true;cleanup(*row,stream.id);}}
        }
    };
}
