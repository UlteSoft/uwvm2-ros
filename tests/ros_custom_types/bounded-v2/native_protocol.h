/** @file
 * @brief Checked C++17 native control/text I/O for official Noetic/Jazzy peers.
 * The project's native_io adapter uses bounded to_chars and checked FD writes;
 * vendored fast_io requires C++20 and cannot be included in Noetic GCC9 builds.
 * A control gate reads exactly one byte with a finite readiness deadline. It
 * performs no Guest import and grants no Runtime or native endpoint authority.
 */
#pragma once
#include "../../../gateways/common/native_io.h"
#include <poll.h>
#include <stdexcept>
namespace bounded_native_io
{
    namespace io=uwvm2::ros_gateway::native_io;
    inline void emit(int descriptor,std::string_view text)
    {if(!io::write_all(descriptor,text)) {throw std::runtime_error{"bounded native text write failed"};}}
    template<class Value> bool append(io::fixed_text<512>& text,Value const& value)
    {if constexpr(std::is_integral_v<Value>) {if constexpr(std::is_same_v<Value,char>) {return text.append({&value,1});}else {return text.number(value);}}else {return text.append(value);}}
    template<class... Values> void print(int descriptor,Values const&... values)
    {io::fixed_text<512> text;if(!(append(text,values)&&...)) {throw std::length_error{"bounded native fixed marker ceiling"};}emit(descriptor,text.view());}
    inline void gate(char expected)
    {pollfd input{STDIN_FILENO,POLLIN,0};int ready;do {ready=::poll(&input,1,30000);}while(ready<0&&errno==EINTR);if(ready!=1||!(input.revents&POLLIN)) {throw std::runtime_error{"bounded native control deadline"};}char byte{};ssize_t count;do {count=::read(STDIN_FILENO,&byte,1);}while(count<0&&errno==EINTR);if(count!=1||byte!=expected) {throw std::runtime_error{"bounded native wrong control byte"};}}
}
