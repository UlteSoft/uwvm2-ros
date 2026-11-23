/** @file
 * @brief Closed canonical parameter definitions and checked deployment budgets.
 * @details This parser accepts only complete owned bytes supplied by trusted
 * configuration. It opens no path, starts no node and confers no authority.
 * Every limit must be explicitly assigned once, including deliberate zeroes.
 * Profile and limits precede the one immutable definition; freeze requires all
 * three phases. Allocation failure is recoverable and leaves phase state intact.
 *
 * File schema: text magic, text parameter identity, blob logical SHA256, then
 * body {profile node, text algebra, bool nonfinite, bool deferred_validation,
 * f64 absolute_tolerance, text search_root, u64 search_candidates, array<key>}.
 * Empty root and zero candidates explicitly disable search. Otherwise ROS1
 * requires a canonical absolute root and 1..256 candidate slots. These semantic
 * boundaries are hashed; they cannot be widened by a guest or runtime quota.
 * Logical SHA256 covers the exact complete
 * body after the first three fields, never its own digest. A separate transport
 * file SHA256, checked by the caller, covers the entire file. These digests have
 * distinct meanings. Runtime capacities and optional rates are deployment
 * restrictions and are not part of this semantic profile body.
 *
 * Each key contains a complete canonical parameter descriptor, ascending unique
 * array<text> allowed types, explicit integer-range presence/value, explicit
 * floating-range presence/value, and explicit override presence/blob. A false
 * presence bit is an explicit absence; a missing field is malformed. The store
 * checks actual values against both trusted and guest-narrowed constraints.
 */
#pragma once
#include "parameter_store.h"

namespace uwvm2::imported::ros::scene::parameters
{
    inline constexpr std::string_view definition_magic="uwvm.ros.parameter.definition";
    /** @brief Aggregate stable capacities; no field is a quota or implicit grant. */
    struct budget
    {
        std::uint64_t scopes{},parameters{},value_bytes{},event_slots{},event_bytes{},cursors{},workspace_bytes{},
            request_bytes{},receipt_bytes{},pending_bytes{},cursor_bytes{},definition_bytes{};
    };
    /** @brief Conservative owned definition payload, including fixed key metadata. */
    inline bool definition_storage(plan const& value,std::uint64_t& output) noexcept
    {
        std::uint64_t const key_capacity=value.keys.capacity();
        if(key_capacity>UINT64_MAX/sizeof(key_policy))return false;
        std::uint64_t result=key_capacity*sizeof(key_policy);
        for(auto const& key:value.keys)if(!add(result,key.override_value.capacity(),result))return false;
        output=result;return true;
    }
    /** @brief Atomically add one frozen plan within explicit global maxima. */
    inline error accumulate(budget const& maximum,plan const& value,budget& total) noexcept
    {
        if(!valid(value))return error::invalid;
        std::uint64_t definition{};if(!definition_storage(value,definition))return error::too_large;
        budget cost{value.max_scopes,value.max_parameters,value.max_total_bytes,value.max_events,value.max_event_bytes,value.max_cursors,
            value.max_workspace,value.max_request_bytes,value.max_receipt_bytes,value.max_pending_bytes,value.max_cursor_bytes,definition};
        budget candidate=total;
        constexpr std::array fields{&budget::scopes,&budget::parameters,&budget::value_bytes,&budget::event_slots,&budget::event_bytes,
            &budget::cursors,&budget::workspace_bytes,&budget::request_bytes,&budget::receipt_bytes,&budget::pending_bytes,&budget::cursor_bytes,&budget::definition_bytes};
        for(auto field:fields)
        {auto limit=maximum.*field,used=candidate.*field,charge=cost.*field;if(limit==UINT64_MAX||used>limit||charge>limit-used)return error::too_large;candidate.*field=used+charge;}
        total=candidate;return error::success;
    }
    class configuration
    {
        plan prepared_;
        std::uint64_t assigned_{},definition_bytes_{},definition_keys_{};
        bool profiled_{},defined_{},frozen_{};
        static bool profile_valid(buffers::profile const& value) noexcept
        {return value.identity.size&&value.identity.size<=buffers::maximum_identity&&value.identity.view().find('\0')==std::string_view::npos&&
            wire::utf8({reinterpret_cast<std::uint8_t const*>(value.identity.bytes.data()),value.identity.size});}
        static bool read_profile(wire::reader& input,buffers::profile& output) noexcept
        {
            std::string_view identity;wire::bytes digest;
            if(!input.text(identity)||!output.identity.assign(identity)||!profile_valid(output)||!input.blob(digest)||digest.size()!=32)return false;
            std::copy(digest.begin(),digest.end(),output.sha256.begin());return true;
        }
    public:
        inline static constexpr std::array<std::string_view,20> limit_names{
            "scopes","parameters","value_bytes","event_slots","event_bytes","cursors","workspace_bytes","batch",
            "request_bytes","receipt_bytes","pending_bytes","cursor_bytes","validation_timeout_ns","cursor_timeout_ns",
            "value.bytes","value.nodes","value.elements","value.depth","definition.bytes","definition.keys"};
        /** @brief Select an exact semantic definition and source-node profile once. */
        error profile(buffers::profile const& selected,buffers::profile const& node,parameter_values::algebra algebra) noexcept
        {
            if(profiled_||defined_||frozen_)return error::conflict;
            if(!profile_valid(selected)||!profile_valid(node)||(algebra!=parameter_values::algebra::ros1&&algebra!=parameter_values::algebra::ros2))return error::invalid;
            prepared_.selected=selected;prepared_.node_profile=node;prepared_.values.selected=algebra;profiled_=true;return error::success;
        }
        /** @brief Assign each finite named capacity exactly once; no unlimited sentinel. */
        error limit(std::string_view name,std::uint64_t value) noexcept
        {
            if(defined_||frozen_)return error::conflict;
            auto found=std::find(limit_names.begin(),limit_names.end(),name);if(found==limit_names.end()||value==UINT64_MAX)return error::invalid;
            auto index=static_cast<std::size_t>(found-limit_names.begin());auto bit=std::uint64_t{1}<<index;
            if(assigned_&bit)return error::conflict;
            constexpr std::array<std::uint64_t,20> maxima{1024,65536,1ull<<30,65536,1ull<<30,4096,1ull<<30,contract::maximum_batch,
                4u*1024u*1024u,1ull<<30,1ull<<30,1ull<<30,INT64_MAX,INT64_MAX,4u*1024u*1024u,65536,65536,parameter_values::maximum_depth,
                4u*1024u*1024u,65536};
            if(value>maxima[index])return error::too_large;
            std::array<std::uint64_t*,20> slots{&prepared_.max_scopes,&prepared_.max_parameters,&prepared_.max_total_bytes,&prepared_.max_events,
                &prepared_.max_event_bytes,&prepared_.max_cursors,&prepared_.max_workspace,&prepared_.max_batch,&prepared_.max_request_bytes,
                &prepared_.max_receipt_bytes,&prepared_.max_pending_bytes,&prepared_.max_cursor_bytes,&prepared_.validation_timeout_ns,
                &prepared_.cursor_timeout_ns,&prepared_.values.bytes,&prepared_.values.nodes,&prepared_.values.elements,&prepared_.values.depth,
                &definition_bytes_,&definition_keys_};
            *slots[index]=value;assigned_|=bit;return error::success;
        }
        /** @brief Parse and own one fully hashed closed definition without native I/O. */
        error definition(wire::bytes input) noexcept
        {
            if(defined_||frozen_)return error::conflict;
            if(!profiled_||assigned_!=((std::uint64_t{1}<<limit_names.size())-1))return error::denied;
            if(!definition_bytes_||input.size()>definition_bytes_||input.size()>4u*1024u*1024u)return error::too_large;
            try
            {
                wire::reader reader{input};std::string_view magic;buffers::profile selected;
                if(!reader.text(magic)||magic!=definition_magic||!read_profile(reader,selected))return error::invalid;
                if(selected!=prepared_.selected)return error::denied;
                auto body=reader.position();if(buffers::hash(input.subspan(body))!=selected.sha256)return error::invalid;
                plan parsed=prepared_;std::string_view algebra;
                if(!read_profile(reader,parsed.node_profile)||parsed.node_profile!=prepared_.node_profile||!reader.text(algebra))return error::denied;
                auto expected=prepared_.values.selected==parameter_values::algebra::ros1?"ros1":"ros2";
                if(algebra!=expected)return error::denied;
                if(!reader.boolean(parsed.values.nonfinite)||!reader.boolean(parsed.deferred_validation)||!reader.f64(parsed.floating_tolerance)||
                    !std::isfinite(parsed.floating_tolerance)||parsed.floating_tolerance<0)return error::invalid;
                std::string_view search_root;
                if(!reader.text(search_root)||!parsed.search_root.assign(search_root)||!reader.u64(parsed.max_search_depth))return error::invalid;
                std::uint64_t count{};if(!reader.u64(count)||count>definition_keys_||count>65536)return error::too_large;
                parsed.keys.reserve(static_cast<std::size_t>(count));
                for(std::uint64_t index{};index<count;++index)
                {
                    key_policy key;key.fixed_descriptor=true;
                    if(!contract::read_descriptor(reader,key.descriptor))return error::invalid;
                    key.name=key.descriptor.name;key.read_only=key.descriptor.read_only;key.dynamic_type=key.descriptor.dynamic_type;
                    if(index&&parsed.keys.back().name.view()>=key.name.view())return error::invalid;
                    std::uint64_t types{};if(!reader.u64(types)||!types||types>contract::type_names.size())return error::invalid;
                    unsigned previous{};bool first=true;
                    for(std::uint64_t i{};i<types;++i)
                    {
                        std::string_view name;if(!reader.text(name))return error::invalid;
                        auto found=std::find(contract::type_names.begin(),contract::type_names.end(),name);if(found==contract::type_names.end())return error::invalid;
                        auto type=contract::type_kinds[static_cast<std::size_t>(found-contract::type_names.begin())];auto bit=static_cast<unsigned>(type);
                        if(!permitted_type(parsed.values.selected,type)||(!first&&bit<=previous))return error::invalid;
                        key.types|=std::uint64_t{1}<<bit;previous=bit;first=false;
                    }
                    if(!reader.boolean(key.integer_constraint)||(key.integer_constraint&&(!reader.i64(key.integers.minimum)||!reader.i64(key.integers.maximum)||
                        !reader.u64(key.integers.step)))||!reader.boolean(key.floating_constraint)||(key.floating_constraint&&(!reader.f64(key.floating.minimum)||
                        !reader.f64(key.floating.maximum)||!reader.f64(key.floating.step)))||!reader.boolean(key.has_override))return error::invalid;
                    key.floating.absolute_tolerance=parsed.floating_tolerance;key.descriptor.floating.absolute_tolerance=parsed.floating_tolerance;
                    if(key.has_override)
                    {
                        wire::bytes bytes;if(!reader.blob(bytes)||bytes.size()>parsed.values.bytes)return error::too_large;
                        parameter_values::scalar scalar;if(!parameter_values::validate(bytes,parsed.values,scalar))return error::invalid;
                        auto type=static_cast<unsigned>(scalar.type);auto const& descriptor=key.descriptor;
                        if(type>63||!(key.types&(std::uint64_t{1}<<type))||
                           (scalar.type!=parameter_values::kind::not_set&&!descriptor.dynamic_type&&scalar.type!=descriptor.declared_type)||
                           (scalar.type==parameter_values::kind::integer&&
                            ((key.integer_constraint&&!key.integers.contains(scalar.integer))||
                             (descriptor.has_range&&!descriptor.floating_range&&!descriptor.integers.contains(scalar.integer))))||
                           (scalar.type==parameter_values::kind::floating&&
                            ((key.floating_constraint&&!key.floating.contains(scalar.floating))||
                             (descriptor.has_range&&descriptor.floating_range&&!descriptor.floating.contains(scalar.floating)))))return error::invalid;
                        key.override_value.assign(bytes.begin(),bytes.end());
                    }
                    parsed.keys.push_back(std::move(key));
                }
                if(!reader.done()||!valid(parsed))return error::invalid;
                prepared_=std::move(parsed);defined_=true;return error::success;
            }
            catch(std::bad_alloc const&){return error::no_memory;}
            catch(std::length_error const&){return error::too_large;}
        }
        /** @brief Bound a trusted configuration read before allocation or file access. */
        error definition_limit(std::uint64_t& output)const noexcept
        {
            output=0;if(defined_||frozen_)return error::conflict;
            if(!profiled_||assigned_!=((std::uint64_t{1}<<limit_names.size())-1))return error::denied;
            if(!definition_bytes_)return error::too_large;
            output=definition_bytes_;return error::success;
        }
        /** @brief Move a fully validated definition into an inert plan exactly once.
         * @details Move publication allocates nothing and cannot leave a half
         * configured output. The caller applies accumulate before activating
         * providers; this method neither grants nor attaches a node.
         */
        error freeze(plan& output) noexcept
        {
            if(frozen_)return error::conflict;
            if(!profiled_||!defined_||assigned_!=((std::uint64_t{1}<<limit_names.size())-1)||!valid(prepared_))return error::denied;
            output=std::move(prepared_);frozen_=true;return error::success;
        }
        /** @brief Validate and reserve the aggregate before publishing any plan. */
        error freeze(plan& output,budget const& maximum,budget& total) noexcept
        {
            if(frozen_)return error::conflict;
            if(!profiled_||!defined_||assigned_!=((std::uint64_t{1}<<limit_names.size())-1))return error::denied;
            auto next=total;auto result=accumulate(maximum,prepared_,next);if(result!=error::success)return result;
            output=std::move(prepared_);frozen_=true;total=next;return error::success;
        }
    };
}
