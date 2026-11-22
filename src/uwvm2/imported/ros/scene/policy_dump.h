/** @file @brief Bounded offline authorization report for an already validated ROSv2 deployment. */
#pragma once
#include "deployment.h"

namespace uwvm2::imported::ros::scene
{
    namespace policy_dump
    {
        /** @brief Assemble privately before publishing. Byte and row limits apply to the full document. */
        struct writer
        {
            static constexpr std::size_t maximum_bytes=4u*1024u*1024u, maximum_rows=4096;
            ::uwvm2::utils::container::std_string data;
            std::size_t rows{};
            bool bounded{true};
            void append(std::string_view value)
            {
                if(!bounded)return;
                if(value.size()>maximum_bytes-data.size()){bounded=false;return;}
                data.append(value);
            }
            bool row(bool& first)
            {
                if(!bounded||rows==maximum_rows){bounded=false;return false;}
                ++rows;
                if(!first)append(",");
                first=false;return bounded;
            }
            // Identifiers are canonical ASCII. A module display can also contain
            // printable punctuation. Arbitrary Wasm module bytes use moduleHex.
            void quoted(std::string_view value)
            {
                append("\"");
                for(char c:value)
                {
                    if(c<' '||static_cast<unsigned char>(c)>126){bounded=false;return;}
                    if(c=='"'||c=='\\')append("\\");
                    append({&c,1});
                }
                append("\"");
            }
            void module(std::string_view value)
            {
                append("{\"module\":");
                if(std::all_of(value.begin(),value.end(),[](unsigned char c){return c>=32&&c<=126;}))quoted(value);
                else append("null");
                append(",\"moduleHex\":\"");
                for(unsigned char c:value)hex_byte(c);
                append("\"");
            }
            void integer(std::uint64_t value)
            {
                char buffer[20];auto converted=std::to_chars(buffer,buffer+sizeof(buffer),value);
                append("\"");append({buffer,static_cast<std::size_t>(converted.ptr-buffer)});append("\"");
            }
            void key(std::string_view name){append(",");quoted(name);append(":");}
            void field(std::string_view name,std::string_view value){key(name);quoted(value);}
            void number(std::string_view name,std::uint64_t value){key(name);integer(value);}
            void hex_byte(std::uint8_t value)
            {
                constexpr char digits[]="0123456789abcdef";
                char pair[]{digits[value>>4],digits[value&15]};append({pair,2});
            }
            template<class Words> void identity(std::string_view key_name,Words const& words)
            {
                key(key_name);append("\"");
                for(auto word:words)for(unsigned i{};i<sizeof(word);++i)
                    hex_byte(static_cast<std::uint8_t>(word>>(8*i)));
                append("\"");
            }
            template<class Binding> void session(Binding const& binding,drivers::credentials peer,
                drivers::protocol::identity scope,drivers::protocol::identity provider,std::string_view transport)
            {
                field("transport",transport);
                number("peerPid",peer.pid);number("peerUid",peer.uid);number("peerGid",peer.gid);
                identity("deploymentId",binding.deployment);identity("vmId",binding.vm);
                identity("scopeId",scope);identity("providerId",provider);
                identity("sessionId",binding.session);identity("contractSha256",binding.contract);
            }
            void limit(std::uint64_t value)
            {if(value==UINT64_MAX)append("\"unlimited\"");else integer(value);}
            void optional(std::optional<std::uint64_t> value)
            {if(value)integer(*value);else append("\"unlimited\"");}
        };
    }

    inline error deployment::dump_effective_policy(::uwvm2::utils::container::std_string& document)const
    {
        document.clear();
        std::lock_guard guard{mutex};
        if(!declarations_only||!frozen||retired)return error::denied;
        policy_dump::writer out;
        out.append("{\"schemaVersion\":1,\"module\":\"uwvm_ros_v2\",\"scope\":\"configured-core-authorization\","
            "\"validation\":\"declarations-only\",\"completeForScope\":true,\"fullDeploymentPolicy\":false,"
            "\"excluded\":[\"native-endpoint-identity-and-alias-checks\",\"transport-availability-and-peer-attestation\","
            "\"provider-specific-profiles-definitions-and-aggregate-budgets\",\"platform-operation-mappings\","
            "\"secret-material-descriptors-and-host-paths\",\"runtime-state-and-live-attenuation\",\"other-host-modules\"],"
            "\"defaults\":{\"unboundExposure\":\"hidden\",\"unconfiguredExposure\":\"hidden\","
            "\"memberSelfReport\":{\"operation\":\"permission.report\",\"effective\":\"allow\"},"
            "\"memberAbi\":{\"operation\":\"abi\",\"effective\":\"allow\"},"
            "\"ungrantedResources\":false,\"unselectedPlatformExtensions\":false},\"callAuditRules\":[");
        bool first_audit=true;
        if(!result_audit.visit_policy([&](call_audit::scope target,std::string_view label,bool denied,call_audit::ceiling limits){
            if(!out.row(first_audit))return false;
            out.append("{\"scope\":");out.quoted(target==call_audit::scope::global?"global":target==call_audit::scope::group?"group":"single");
            out.field("label",label);out.field("mode",denied?"denied":"all");out.number("records",limits.records);
            out.number("bytes",limits.bytes);out.number("rateHz",limits.rate_hz);out.number("burst",limits.burst);out.append("}");return out.bounded;
        }))return error::too_large;
        out.append("],\"payloadJournal\":");
        if(payload_journal)
        {
            out.append("{\"records\":");out.integer(payload_journal->records);out.number("bytes",payload_journal->bytes);
            out.number("payloadBytes",payload_journal->payload);out.append("}");
        }
        else out.append("null");
        out.append(",\"registeredOperations\":[");
        bool first=true;
        for(auto name:callable_names){if(!out.row(first))return error::too_large;out.quoted(name);}
        out.append("],\"memberships\":[");first=true;
        for(auto const& [name,owner]:memberships)
        {
            if(!out.row(first))return error::too_large;
            out.module(name);out.append(",\"environment\":");out.quoted(owner);out.append("}");
        }
        out.append("],\"environments\":[");first=true;
        auto exposure_text=[](exposure mode){return mode==exposure::allow?"allow":mode==exposure::deny?"deny":"hidden";};
        for(auto const& [name,entry]:environments)
        {
            if(!out.row(first))return error::too_large;
            out.append("{\"name\":");out.quoted(name);out.append(",\"configuredImports\":[");
            bool first_rule=true;
            for(auto const& [action,mode]:entry.interfaces)
            {
                if(!out.row(first_rule))return error::too_large;
                out.append("{\"operation\":");out.quoted(action);out.append(",\"configured\":");out.quoted(exposure_text(mode));
                out.append(",\"effective\":");out.quoted(exposure_text(interface_exposure(entry,action).first));out.append("}");
            }
            out.append("],\"grants\":[");bool first_grant=true;
            for(auto const& [alias,g]:entry.grants)
            {
                if(!out.row(first_grant))return error::too_large;
                out.append("{\"alias\":");out.quoted(alias);out.append(",\"purpose\":");out.quoted(purpose_names[static_cast<std::size_t>(g.kind)]);
                out.append(",\"resource\":");out.quoted(g.resource);out.append(",\"binding\":");out.quoted(g.binding);
                out.append(",\"platformOwner\":");auto owner=entry.platform_children.owner(alias);
                if(owner.empty())out.append("null");else out.quoted(owner);
                out.append(",\"actions\":[");bool first_action=true;
                for(auto const& action:g.actions){if(!out.row(first_action))return error::too_large;out.quoted(action);}
                out.append("],\"maxHandles\":");out.limit(g.max_handles);out.append(",\"maxResources\":");out.limit(g.max_resources);
                out.append(",\"maxWaitMembers\":");out.limit(g.max_wait_members);out.append(",\"maxWaitNs\":");out.limit(g.max_wait_ns);
                out.append(",\"infiniteWait\":");out.append(g.infinite_wait?"true":"false");
                out.append(",\"delivery\":");out.quoted(g.limits.uncertain?"may-reject":"stable");
                out.append(",\"callsPerWindow\":");out.optional(g.limits.calls);out.append(",\"bytesPerWindow\":");out.optional(g.limits.bytes);
                out.append(",\"windowNs\":");out.integer(static_cast<std::uint64_t>(g.limits.window.count()));out.append("}");
            }
            out.append("],\"observationDenials\":[");bool first_deny=true;
            if(!entry.observation.visit_reductions([&](std::string_view family,std::string_view function){
                if(!out.row(first_deny))return false;
                out.append("{\"family\":");out.quoted(family);out.append(",\"function\":");
                if(function.empty())out.append("null");else out.quoted(function);out.append("}");return out.bounded;
            }))return error::too_large;
            out.append("],\"platformSelections\":[");bool first_child=true;
            if(!entry.platform_children.visit_selections([&](std::string_view module,bool enabled){
                if(!out.row(first_child))return false;
                out.append("{\"module\":");out.quoted(module);out.append(",\"enabled\":");out.append(enabled?"true}":"false}");
                return out.bounded;
            }))return error::too_large;
            out.append("],\"messageRoutes\":[");bool first_route=true;
            for(auto const& [route,p]:entry.module_call_plans)
            {
                if(!out.row(first_route))return error::too_large;
                out.append("{\"route\":");out.quoted(route);out.field("client",p.client);out.field("server",p.server);
                out.field("operation",p.semantic);out.field("requestSchema",p.request_schema);out.field("responseSchema",p.response_schema);
                out.number("requestBytes",p.request_bytes);out.number("responseBytes",p.response_bytes);out.number("slots",p.slots);
                out.number("deadlineNs",p.deadline_ns);out.key("clearInputAfterCopy");out.append(p.clear_after_copy?"true":"false");
                out.key("captureStages");out.append("[");bool first_stage=true;
                for(auto stage:{"submit","take","respond","result"})
                {
                    module_buffer_audit::stage id{};module_buffer_audit::parse_stage(stage,id);
                    if(!(p.capture_stages & module_buffer_audit::bit(id)))continue;
                    if(!out.row(first_stage))return error::too_large;
                    out.quoted(stage);
                }
                out.append("]}");
            }
            out.append("],\"sessions\":[");bool first_session=true;
            if(entry.drivers&&!entry.drivers->visit_registrations([&](std::string_view alias,std::string_view module,
                drivers::credentials peer,auto const& binding,drivers::limits limits,std::string_view operation,schema_contract::limits schema){
                if(!out.row(first_session))return false;
                out.module(module);out.field("kind","driver");out.field("alias",alias);
                out.session(binding,peer,binding.module,binding.driver,limits.tls_identity_fd>=0?"mutual-tls-1.3":"kernel-seqpacket");
                out.field("peerClock",limits.peer_time_namespace_fd>=0?"provisioned-namespace":"process-namespace");
                out.number("requestBytes",limits.request_bytes);out.number("responseBytes",limits.response_bytes);
                out.number("requestNs",limits.request_ns);out.number("sessionUntilNs",limits.session_until);
                out.field("operation",operation);out.number("schemaBytes",schema.bytes);out.number("schemaNodes",schema.nodes);
                out.number("schemaElements",schema.elements);out.number("schemaDepth",schema.depth);out.append("}");return out.bounded;
            }))return error::too_large;
            for(auto const& [alias,worker]:entry.model_worker_plans)
            {
                if(!out.row(first_session))return error::too_large;
                auto limits=worker.limits();auto const& binding=worker.identity();
                out.append("{\"kind\":\"model\"");out.field("alias",alias);
                out.session(binding,worker.peer(),binding.scope,binding.provider,limits.tls_identity_fd>=0?"mutual-tls-1.3":"kernel-seqpacket");
                out.field("peerClock",limits.peer_time_namespace_fd>=0?"provisioned-namespace":"process-namespace");
                out.number("requestBytes",limits.request_bytes);out.number("responseBytes",limits.response_bytes);
                out.number("startupNs",limits.startup_ns);out.number("pollNs",limits.poll_ns);
                out.number("supervisorBytes",limits.supervisor_bytes);out.number("sessionUntilNs",limits.session_until);out.append("}");
            }
            for(auto const& [worker_name,worker]:entry.communication.workers())
            {
                if(!out.row(first_session))return error::too_large;
                auto const& reg=worker.attachment.endpoints();auto const& profile=worker.attachment.profile();
                out.append("{\"kind\":\"communication\"");out.field("worker",worker_name);
                out.session(reg.identity,reg.peer,reg.identity.module,reg.identity.driver,"kernel-seqpacket-and-seven-pipes");
                out.field("peerClock","process-namespace");out.field("startupProfile",profile.identity.view());
                out.identity("startupSha256",profile.sha256);out.number("startupNs",reg.startup_ns);
                out.number("sessionUntilNs",reg.session_until);out.number("stderrBytes",reg.stderr_bytes);
                out.key("workerLimits");out.append("{");bool first_limit=true;
                for(std::size_t i{};i<worker.limits.size();++i)
                {
                    if(!out.row(first_limit))return error::too_large;
                    out.quoted(communication_deployment_configuration::peer_fields[i].name);out.append(":");out.integer(worker.limits[i]);
                }
                out.append("}}");
            }
            out.append("],\"communicationBindings\":[");bool first_binding=true;
            for(auto const& [alias,binding]:entry.communication.bindings())
            {
                if(!out.row(first_binding))return error::too_large;
                out.append("{\"alias\":");out.quoted(alias);out.field("worker",binding.worker_name);out.append("}");
            }
            out.append("],\"communicationPairs\":[");bool first_pair=true;
            for(auto const& [pair_name,pair]:entry.communication.pairs())
            {
                if(!out.row(first_pair))return error::too_large;
                out.append("{\"name\":");out.quoted(pair_name);out.field("left",pair.left);out.field("right",pair.right);
                out.number("retirementNs",pair.retirement_ns);out.append("}");
            }
            out.append("]}");
        }
        out.append("]}");
        if(!out.bounded)return error::too_large;
        document.swap(out.data);return error::success;
    }
}
