// SPDX-License-Identifier: Apache-2.0
#include <uwvm_rmw/codec_composer.h>
#include <array>
#include <atomic>
#include <cstring>
#include <new>
namespace {constexpr std::size_t limit=64,missing=limit;
bool same(uwvm_rmw_codec const&a,uwvm_rmw_codec const&b)noexcept {
 return a.version==b.version&&a.kind==b.kind&&a.native_type==b.native_type&&a.generated_support==b.generated_support&&
 !std::memcmp(a.native_rihs01,b.native_rihs01,32)&&a.maximum_wire_bytes==b.maximum_wire_bytes&&a.validate==b.validate&&
 a.encode==b.encode&&a.decode==b.decode&&a.request==b.request&&a.response==b.response&&a.validation_state==b.validation_state;
}}
struct uwvm_rmw_codec_composer {
 uwvm_rmw_codec_composer const*self{this};std::atomic<bool>exported{};bool ready{};std::size_t count{};
 std::array<uwvm_rmw_codec const*,limit>originals{};
 std::array<uwvm_rmw_codec,limit>snapshots{},copies{};
 std::array<std::array<char,256>,limit>names{};
 std::array<std::size_t,limit>requests{},responses{};
};
namespace {
std::uint32_t validate_copy(uwvm_rmw_codec const*c,void const*supplied)noexcept {
 try {
 if(!c||!c->validation_state)return 22;auto const&s=*static_cast<uwvm_rmw_codec_composer const*>(c->validation_state);
 if(s.self!=&s||!s.ready)return 22;std::size_t i{};for(;i<s.count;++i)if(c==&s.copies[i])break;if(i==s.count)return 22;
 auto const&original=*s.originals[i];auto const&saved=s.snapshots[i];
 if(!same(original,saved)||!original.native_type||std::strcmp(original.native_type,s.names[i].data()))return 22;
 auto expected=saved;expected.native_type=s.names[i].data();expected.validate=&validate_copy;expected.validation_state=&s;
 if(expected.kind==UWVM_RMW_SERVICE_CODEC){if(s.requests[i]>=s.count||s.responses[i]>=s.count)return 22;expected.request=&s.copies[s.requests[i]];expected.response=&s.copies[s.responses[i]];}
 if(!same(*c,expected))return 22;
 return original.validate(s.originals[i],supplied);
 }catch(std::bad_alloc const&){return 12;}catch(...){return 22;}
}}
extern "C" std::uint32_t uwvm_rmw_codec_composer_create(uwvm_rmw_codec_table_view const*views,std::size_t count,uwvm_rmw_codec_composer**out){
 if(!views||!count||count>limit||!out)return 22;
 try {
 std::size_t total{};for(std::size_t i{};i<count;++i){if(!views[i].data||!views[i].count||views[i].count>limit-total)return 22;total+=views[i].count;}
 auto*s=new(std::nothrow)uwvm_rmw_codec_composer;if(!s)return 12;struct Owner{uwvm_rmw_codec_composer*p;~Owner(){delete p;}}owner{s};s->count=total;std::size_t at{};
 for(std::size_t v{};v<count;++v)for(std::size_t j{};j<views[v].count;++j,++at){
 auto const*c=&views[v].data[j];if(!c->native_type||!c->validate||!c->generated_support)return 22;std::size_t n{};while(n<256&&c->native_type[n])++n;if(!n||n==256)return 22;
 for(std::size_t k{};k<at;++k)if(s->originals[k]==c||!std::strcmp(c->native_type,s->names[k].data()))return 22;
 auto error=c->validate(c,c->generated_support);if(error)return error;
 s->originals[at]=c;s->snapshots[at]=*c;s->copies[at]=*c;std::memcpy(s->names[at].data(),c->native_type,n+1);
 }
 for(std::size_t i{};i<total;++i){auto&c=s->copies[i];c.native_type=s->names[i].data();c.validate=&validate_copy;c.validation_state=s;s->requests[i]=s->responses[i]=missing;
 if(c.kind==UWVM_RMW_SERVICE_CODEC){for(std::size_t j{};j<total;++j){if(s->originals[j]==c.request)s->requests[i]=j;if(s->originals[j]==c.response)s->responses[i]=j;}
 if(s->requests[i]>=total||s->responses[i]>=total||s->requests[i]==s->responses[i])return 22;c.request=&s->copies[s->requests[i]];c.response=&s->copies[s->responses[i]];}
 else if(c.kind!=UWVM_RMW_MESSAGE_CODEC||c.request||c.response)return 22;}
 s->ready=true;for(std::size_t i{};i<total;++i){auto error=validate_copy(&s->copies[i],s->copies[i].generated_support);if(error)return error;}
 *out=s;owner.p=nullptr;return 0;
 }catch(std::bad_alloc const&){return 12;}catch(...){return 22;}
}
extern "C" std::uint32_t uwvm_rmw_codec_composer_export(uwvm_rmw_codec_composer*s,uwvm_rmw_codec const**out,std::size_t*count){
 if(!s||s->self!=s||!s->ready||!out||!count)return 22;s->exported.store(true,std::memory_order_release);*out=s->copies.data();*count=s->count;return 0;
}
extern "C" std::uint32_t uwvm_rmw_codec_composer_destroy_unexported(uwvm_rmw_codec_composer**p){
 if(!p||!*p||(*p)->self!=*p)return 22;if((*p)->exported.load(std::memory_order_acquire))return 17;delete *p;*p=nullptr;return 0;
}
