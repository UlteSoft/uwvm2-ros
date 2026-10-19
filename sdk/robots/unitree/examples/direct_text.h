/** @file Closed ASCII-marker formatting only, using the actual fast_io API.
 *
 * Go2/GO/HG STEP, READY, AGAIN, PASS and denial markers contain only literal
 * character arrays and uint32 words. Keep the actual existing spelling and
 * caller's basic_io short-write/error semantics. No Guest import or write is
 * performed by this formatter. STATE carrier formatting remains unchanged.
 *
 * The finite type/extent contract is checked before formatting. An output owns
 * its complete inline storage; it never retains the obuffer_view's local
 * pointer or borrows a temporary string. This header is deliberately separate
 * from basic_io.h so the same real formatter has an ordinary C++ source-value
 * contract, without inventing WASI or ROS imports.
 */
#ifndef UWVM_UNITREE_EXAMPLE_DIRECT_TEXT_H
#define UWVM_UNITREE_EXAMPLE_DIRECT_TEXT_H
#include <fast_io.h>
#include <cstddef>
#include <cstdint>
#include <type_traits>
namespace uwvm_unitree_example_text {
inline constexpr std::size_t capacity=128;
struct record {
    char storage[capacity]{};
    std::size_t used{};
    char const* data()const noexcept{return storage;}
    std::size_t size()const noexcept{return used;}
};
template<class T>inline constexpr bool supported=
    std::is_same_v<std::remove_cv_t<T>,std::uint32_t>||
    (std::is_array_v<T>&&std::is_same_v<std::remove_cv_t<std::remove_extent_t<T>>,char>&&std::extent_v<T>!=0);
template<class T>requires(supported<T>)
inline constexpr std::size_t maximum()noexcept {
    if constexpr(std::is_same_v<std::remove_cv_t<T>,std::uint32_t>)
        return fast_io::print_reserve_size(fast_io::io_reserve_type<char,std::uint32_t>);
    else return std::extent_v<T>-1;
}
static_assert(maximum<std::uint32_t>()==10);
template<class... Values>requires((supported<Values>)&&...)
inline record format(Values const&... values) {
    static_assert((std::size_t{}+...+maximum<Values>())<=capacity,
        "Explicit finite marker does not fit; do not silently truncate or allocate");
    record output{};
    fast_io::obuffer_view sink{output.storage,output.storage+capacity};
    fast_io::print(sink,values...);
    output.used=sink.size();
    return output;
}
}
#endif
