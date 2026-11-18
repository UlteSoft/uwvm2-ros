/** @file
 * @brief Read-only, bounded permission reports under the ROSv2 parent.
 * @details A report observes policy; it never admits an operation. Its default
 * authority is complete observation after explicit ROSv2 module binding. A
 * deployment can deny a whole family or one exact function, or hide the import.
 * Denials apply before feature lookup, including unbuilt features. Aggregate
 * reports fail closed if a function is redacted; an exact allowed function can
 * still be queried. This prevents an aggregate from bypassing a redaction.
 * No path, environment value, payload, function pointer or native symbol is
 * resolved. These records are not transferable execution capabilities.
 */
#pragma once
#define UWVM_ROS_PERMISSION_REPORT_FUNCTIONS(X) \
 X(permission_report,report,"permission.report")
#define UWVM_ROS_MODULE_OBSERVATION_FUNCTIONS(X) \
 X(module_describe,describe,"module.describe") \
 X(module_imports,imports,"module.imports") \
 X(module_exports,exports,"module.exports") \
 X(module_group,group,"module.group") \
 X(module_importers,importers,"module.importers")
#define UWVM_ROS_MODULE_REPORT_FUNCTIONS(X) \
 X(module_permission_report,report,"module.permission.report")
#define UWVM_ROS_VM_OBSERVATION_FUNCTIONS(X) \
 X(vm_describe,describe,"vm.describe")
#ifndef UWVM_SCENE_PERMISSION_REPORT_MACROS_ONLY
#include <uwvm2/utils/container/standard.h>
#include "erased_buffer.h"
#include "wire.h"
#include <array>
#include <cstdlib>
#include <string_view>
#include <set>

namespace uwvm2::imported::ros::scene::permission_report
{
    inline constexpr std::string_view import_name="permission.report";
    enum class module_operation : std::uint8_t {describe,imports,exports,group,importers};
    inline constexpr std::array module_operations{std::string_view{"module.describe"},std::string_view{"module.imports"},
        std::string_view{"module.exports"},std::string_view{"module.group"},std::string_view{"module.importers"}};
    inline constexpr std::string_view vm_import_name="vm.describe";
    enum class family : std::uint8_t {preloaddl,preloadwasm,wasip1,ros1v1,ros2v1,rosv2};
    inline constexpr std::array names{std::string_view{"preloaddl"},std::string_view{"preloadwasm"},
        std::string_view{"wasip1"},std::string_view{"ros1v1"},std::string_view{"ros2v1"},std::string_view{"rosv2"}};
    inline constexpr std::size_t maximum_request=8+16+8+255+8;
    inline constexpr std::size_t maximum_response=65536,maximum_rows=32,maximum_scan=65536;
    struct query {family target{};std::string_view function;std::uint64_t first{};};
    inline bool parse_family(std::string_view name,family& out)noexcept
    {for(std::size_t i{};i<names.size();++i)if(names[i]==name){out=static_cast<family>(i);return true;}return false;}
    /** @brief Canonical family, optional exact function, and row cursor only.
     * @details There is deliberately no setter, pointer, callback, module
     * impersonation field, capability handle, expression or native filename.
     */
    inline bool decode(std::span<std::uint8_t const> bytes,query& out)noexcept
    {
        if(bytes.size()>maximum_request)return false;
        wire::reader input{bytes};std::string_view name;query next;
        if(!input.text(name)||!parse_family(name,next.target)||!input.text(next.function)||next.function.size()>255||
           !input.u64(next.first)||next.first>maximum_scan||!input.done())return false;
        for(char c:next.function)if(!((c>='a'&&c<='z')||(c>='A'&&c<='Z')||(c>='0'&&c<='9')||c=='_'||c=='.'||c=='-'))return false;
        out=next;return true;
    }
    /** @brief Monotonic observation attenuation, independent of resource grants. */
    class policy
    {
        std::array<bool,names.size()> denied{};
        ::uwvm2::utils::container::std_set<std::pair<family,::uwvm2::utils::container::std_string>> functions;
    public:
        bool deny(family target,std::string_view function)
        {
            auto index=static_cast<std::size_t>(target);if(index>=names.size())return false;
            if(function.empty()){if(denied[index])return false;denied[index]=true;return true;}
            return functions.emplace(target,::uwvm2::utils::container::std_string(function)).second;
        }
        /** @brief Visit the immutable configured reductions under the deployment lock.
         * Empty function names denote an entire family. This does not query guest state.
         */
        template<typename Visitor> bool visit_reductions(Visitor visit) const
        {
            for(std::size_t i{};i<names.size();++i)
                if(denied[i]&&!visit(names[i],std::string_view{}))return false;
            for(auto const& [target,name]:functions)
                if(!visit(names[static_cast<std::size_t>(target)],std::string_view{name}))return false;
            return true;
        }
        bool allows(query const& request)const noexcept
        {
            auto index=static_cast<std::size_t>(request.target);if(index>=names.size()||denied[index])return false;
            for(auto const& [target,name]:functions)
                if(target==request.target&&(request.function.empty()||name==request.function))return false;
            return true;
        }
    };
    /** @brief Canonical field prefixes constructed only from checked ASCII literals.
     * @details Closed VM labels are immutable, so validating and encoding them
     * on every query adds no security. The consteval constructor rejects long
     * fields, non-ASCII input and missing terminators. Its private storage has
     * no mutation API; guest/runtime text must use page's ordinary checked path.
     * The trailing scalar stays dynamic and receives the normal extent check.
     */
    template<std::size_t Scope,std::size_t Key,std::size_t Annotation=1>
    class literal_row
    {
        static_assert(Scope&&Key&&Annotation&&Scope<=256&&Key<=256&&Annotation<=1025);
        std::array<std::uint8_t,24+Scope+Key+Annotation-3> prefix{};
    public:
        consteval literal_row(char const(&scope)[Scope],char const(&key)[Key],char const(&annotation)[Annotation]="")
        {
            std::size_t cursor{};
            auto field=[&]<std::size_t N>(char const(&value)[N])
            {
                if(value[N-1]!='\0')std::abort(); // A non-constant call rejects this literal at compile time.
                for(unsigned i{};i<8;++i)prefix[cursor++]=static_cast<std::uint8_t>(std::uint64_t(N-1)>>(8*i));
                for(std::size_t i{};i<N-1;++i)
                {
                    auto byte=static_cast<unsigned char>(value[i]);
                    if(byte>=128)std::abort(); // Runtime/non-ASCII text uses the fully checked writer.
                    prefix[cursor++]=byte;
                }
            };
            field(scope);field(key);field(annotation);
        }
        constexpr wire::bytes bytes()const noexcept{return prefix;}
    };
    template<std::size_t Scope,std::size_t Key> literal_row(char const(&)[Scope],char const(&)[Key])->literal_row<Scope,Key>;
    template<std::size_t Scope,std::size_t Key,std::size_t Annotation>
    literal_row(char const(&)[Scope],char const(&)[Key],char const(&)[Annotation])->literal_row<Scope,Key,Annotation>;

    /** @brief Allocation-free page builder. Only detached metadata is encoded.
     * @details A bounded scan preserves one consistent observation under the
     * source's read lock. The header is written only after a successful scan:
     * supported, enabled, first, total, count. Each row is scope/text, key/text,
     * annotation/text, value/u64. UINT64_MAX means unlimited only for documented
     * capacity keys. The caller discards the whole page on any failure.
     */
    class page
    {
        std::span<std::uint8_t> storage;
        wire::writer output;
        std::uint64_t first{},total{},count{};
        bool valid{true};
    public:
        explicit page(std::span<std::uint8_t> buffer,std::uint64_t start)noexcept:
            storage(buffer),output(buffer.size()>=40?buffer.subspan(40):std::span<std::uint8_t>{}),first(start)
        {valid=buffer.size()>=40&&buffer.size()<=maximum_response&&start<=maximum_scan;}
        bool add(std::string_view scope,std::string_view key,std::uint64_t value,std::string_view annotation={})noexcept
        {
            if(!valid||total==maximum_scan){valid=false;return false;}
            auto index=total++;
            if(index<first||count==maximum_rows)return true;
            if(scope.size()>255||key.size()>255||annotation.size()>1024||
               !output.text(scope)||!output.text(key)||!output.text(annotation)||!output.u64(value))return valid=false;
            ++count;return true;
        }
        /** @brief Append checked compile-time metadata with the same scan and page limits.
         * @details This only avoids revalidating immutable field prefixes. The
         * writer still bounds every store; partial failure stays inside the
         * owner's written extent and is securely erased before deallocation.
         * No authorization, identity or dynamic text check is bypassed.
         */
        template<std::size_t Scope,std::size_t Key,std::size_t Annotation>
        bool add(literal_row<Scope,Key,Annotation> const& row,std::uint64_t value)noexcept
        {
            if(!valid||total==maximum_scan){valid=false;return false;}
            auto index=total++;
            if(index<first||count==maximum_rows)return true;
            if(!output.encoded(row.bytes())||!output.u64(value))return valid=false;
            ++count;return true;
        }
        bool finish(bool supported,bool enabled)noexcept
        {
            if(!valid||first>total)return false;
            wire::writer header{storage.first(40)};
            return header.boolean(supported)&&header.boolean(enabled)&&header.u64(first)&&header.u64(total)&&header.u64(count);
        }
        std::span<std::uint8_t const> bytes()const noexcept{return storage.first(40+output.size());}
    };
    /** @brief Own a report and overwrite every encoded byte before deallocation.
     * @details Only page's writer can access the allocation. Its cursor covers
     * every completed field, including the prefix of a row whose later field
     * failed. The fixed header is cleared even if finish() was never reached.
     * The unused allocation tail is never read or written by this owner; it
     * contains no report bytes. Clearing the written extent avoids touching
     * 64 KiB for a short scalar query without weakening payload erasure.
     * Allocation uses recoverable fast_io storage; failure publishes nothing.
     * The page and any borrowed views must remain within this owner's lifetime.
     */
    class owned_page
    {
        ::uwvm2::utils::container::std_unique_ptr<std::uint8_t[]> storage;
        page content;
    public:
        explicit owned_page(std::uint64_t first):
            storage(::uwvm2::utils::container::make_unique_for_overwrite<std::uint8_t[]>(maximum_response)),
            content(std::span{storage.get(),maximum_response},first){}
        owned_page(owned_page const&)=delete;owned_page& operator=(owned_page const&)=delete;
        ~owned_page(){erase_bytes({storage.get(),content.bytes().size()});}
        page& get()noexcept{return content;}
    };
}
#endif
