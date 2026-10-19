// Compile and link only. This program is never run by configuration, including
// cross builds. The adapter deliberately retains its C++17 implementation.
#include <array>
#include <charconv>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>
inline constexpr unsigned selected_standard = 17;
template<typename T> unsigned value(T input)
{
    if constexpr(std::is_integral_v<T>) { return static_cast<unsigned>(input); }
    else { return 0; }
}
int main()
{
    std::array<char,32> bytes{};
    auto output = std::to_chars(bytes.data(),bytes.data()+bytes.size(),selected_standard);
    unsigned number{};
    auto input = std::from_chars(bytes.data(),output.ptr,number);
    std::optional<std::string> text{std::string{bytes.data(),output.ptr}};
    std::string_view view{*text};
    return output.ec==std::errc{}&&input.ec==std::errc{}&&!view.empty()&&value(number)==17?0:1;
}
