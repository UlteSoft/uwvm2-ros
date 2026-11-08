#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/uwvm/debugger/console.h>
#include <cstdio>
#include <cstdlib>
#include <string>
int main()
{
 namespace dbg=uwvm2::uwvm::debugger;
 auto const command{dbg::parse_console_command("memtags 2 1 15 2")};
 dbg::controller_reply reply{};reply.memory={std::byte{},std::byte{1}};
 reply.memory_tag_info={uwvm2::extension::memtag::debug_tag_status::available,4096u,0u,2u,4u};
 auto text{dbg::details::format_reply(reply,command)};
 std::string expected{"Wasm memory tags module=2 index=1 offset=15 bytes=2 status=available memory-bytes=4096 tag-bits=4 first-granule=0 count=2\ntag offset=0 value=0x00 color=#ff4040\ntag offset=16 value=0x01 color=#40d7ff\nmemory-tags-end\n"};
 if(text!=expected) { std::printf("%s",text.c_str());std::abort(); }
 reply.memory.clear();reply.memory_tag_info.status=uwvm2::extension::memtag::debug_tag_status::disabled;reply.memory_tag_info.tag_bits=0;
 text=dbg::details::format_reply(reply,command);
 if(text.find("status=disabled")==std::string::npos || text.find("count=0")==std::string::npos || text.find("tag offset=")!=std::string::npos) { std::abort(); }
 std::puts("memory tag native formatter passed");
}
