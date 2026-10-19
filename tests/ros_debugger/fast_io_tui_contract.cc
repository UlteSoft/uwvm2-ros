/** Exact terminal-output contracts for the real debugger TUI formatter.
 *
 * Compile this translation unit with the project's actual C++20-or-newer
 * compiler, src include root and vendored fast_io include root. Run the
 * resulting native executable inside the normal bounded test owner. It uses
 * no terminal device, Runtime, ROS middleware, network or Guest capability:
 * retained_frame returns bytes in memory, and the assertions compare them.
 * Source presence alone is not a successful build or test result.
 */
#include <uwvm2/uwvm/debugger/console_tui.h>
#include <array>
#include <string>

namespace tui = uwvm2::uwvm::debugger::console_tui;
static unsigned checks{}, failures{};
static void check(bool ok, char const* label)
{
    ++checks;
    if(!ok)
    {
        ++failures;
        fast_io::io::perrln("FAIL_FAST_IO_TUI ",fast_io::mnp::os_c_str(label));
    }
}
static std::string bytes(fast_io::string const& text)
{ return text.empty() ? std::string{} : std::string{text.data(),text.size()}; }

static void sanitization()
{
    // All byte values independently exercise the signed-char boundary, fixed
    // two-digit hex output and the small scratch view. No terminal is opened.
    constexpr char hex[]{"0123456789abcdef"};
    for(unsigned value{};value!=256u;++value)
    {
        char input[]{static_cast<char>(value)};
        std::string expected;
        if(value==10u) { expected="\n"; }
        else if(value==9u) { expected="    "; }
        else if(value>=32u && value<127u) { expected.assign(input,1u); }
        else { expected="\\x";expected.push_back(hex[value>>4u]);expected.push_back(hex[value&15u]); }
        check(bytes(tui::clean(tui::view{input,1u}))==expected,"exact single-octet sanitization");
        check(bytes(tui::clean(tui::view{input,1u},expected.size()))==expected,"exact output capacity");
        check(tui::clean(tui::view{input,1u},expected.size()-1u).empty(),"reject whole scalar before partial output");
    }
    check(tui::clean("anything",0u).empty(),"zero capacity");
    check(bytes(tui::clean("A\tB",4u))=="A","tab expands atomically at boundary");
    check(bytes(tui::clean("A\tB",5u))=="A    ","tab exactly fills remaining capacity");
    check(bytes(tui::clean("A\tB",6u))=="A    B","text after expanded tab");
    check(bytes(tui::clean("a\xc2\x85" "b",8u))=="a","C1 scalar is not partly escaped");
    check(bytes(tui::clean("a\xc2\x85" "b",9u))=="a\\xc2\\x85","complete two-octet C1 escape");
    check(bytes(tui::clean("a\xc2\x85" "b",10u))=="a\\xc2\\x85b","text after C1 scalar");
    check(bytes(tui::clean("\xc3\xa9\xe4\xb8\xad\xf0\x9f\x98\x80"))==
          "\xc3\xa9\xe4\xb8\xad\xf0\x9f\x98\x80","valid two/three/four-octet UTF8 preserved");
    check(bytes(tui::clean("\xf0\x9f\x98\x80",3u)).empty(),"four-octet scalar admission is atomic");
    check(bytes(tui::clean("\xe2\x82"))=="\\xe2\\x82","truncated UTF8 remains explicit");
    check(bytes(tui::clean("\xed\xa0\x80"))=="\\xed\\xa0\\x80","surrogate encoding is escaped");
    check(bytes(tui::styled_clean("\x1b[31mA\x1b[0m\t"))=="\x1b[31mA\x1b[0m    ","SGR preserved around cleaned text");
    check(bytes(tui::styled_clean("\x1b[2J"))=="\\x1b[2J","non-SGR control cannot clear terminal");
    std::string boundary(32767u,'a');boundary.push_back('\t');
    check(tui::styled_clean(tui::view{boundary.data(),boundary.size()}).size()==32767u,"styled cap does not publish partial tab");
    boundary.assign(32764u,'a');boundary.push_back('\t');
    check(tui::styled_clean(tui::view{boundary.data(),boundary.size()}).size()==32768u,"styled exact32768byte boundary");
}

static void frames(bool color)
{
    // Explicit complete byte snapshots protect synchronized-output ordering;
    // the same owner must also produce no bytes for an unchanged frame.
    uwvm2::uwvm::utils::ansies::put_color=color;
    std::string reset{color ? "\x1b[0m" : "\x1b[27m"};
    std::string prefix{"\x1b[?2026h\x1b[?25l\x1b[?7l"};
    std::string suffix{reset+"\x1b[?7h\x1b[1;1H\x1b[?25h\x1b[?2026l"};
    tui::retained_frame frame;
    frame.begin({4u,1u});frame.append("abc");
    check(bytes(frame.finish(1u,1u,false))==prefix+"\x1b[1;1H"+reset+"abc"+suffix,"complete initial retained frame");
    frame.begin({4u,1u});frame.append("abc");
    check(frame.finish(1u,1u,false).empty(),"unchanged cells and cursor produce no bytes");
    frame.begin({4u,1u});frame.append("abc");
    check(bytes(frame.finish(1u,2u,false))==prefix+reset+"\x1b[?7h\x1b[1;2H\x1b[?25h\x1b[?2026l","cursor-only frame has one prefix and suffix");
    frame.begin({4u,1u});frame.append("axc");
    check(bytes(frame.finish(1u,1u,false))==prefix+"\x1b[1;2H"+reset+"x"+suffix,"one changed cell delta");
    frame.begin({4u,1u});frame.append("axc");
    check(bytes(frame.finish(1u,1u,true))==prefix+"\x1b[r\x1b[2J\x1b[H\x1b[1;1H"+reset+"axc"+suffix,"clear stays inside synchronized frame");
    frame.begin({4u,1u});frame.append("\xe4\xb8\xad" "a");
    check(bytes(frame.finish(1u,1u,false))==prefix+"\x1b[1;1H"+reset+"\xe4\xb8\xad" "a"+suffix,"wide glyph renders once");
    frame.begin({4u,1u});frame.append("\xe4\xb8\xad" "a");
    check(frame.finish(1u,1u,false).empty(),"unchanged wide glyph produces no output");
    frame.begin({4u,1u});frame.append("a\xcc\x81" "bc");
    check(bytes(frame.finish(1u,1u,false))==prefix+"\x1b[1;1H"+reset+"a\xcc\x81" "bc"+suffix,"combining mark belongs to its base cell");
    frame.invalidate();frame.begin({4u,1u});frame.append("a\xcc\x81" "bc");
    check(bytes(frame.finish(1u,1u,false))==prefix+"\x1b[1;1H"+reset+"a\xcc\x81" "bc"+suffix,"invalidation redraw preserves complete bytes");
    frame.begin({5u,1u});frame.append("abcd");
    check(bytes(frame.finish(1u,1u,false))==prefix+"\x1b[1;1H"+reset+"abcd"+suffix,"resized frame redraw has one final owner");
    tui::retained_frame styled;
    styled.begin({4u,1u});styled.append("\x1b[31;44;1;7mabc");
    check(bytes(styled.finish(1u,1u,false))==prefix+"\x1b[1;1H"+reset+"\x1b[0;31;44;7;1mabc"+suffix,"foreground background reverse and bold order");
}

int main()
{
    auto const original_color{uwvm2::uwvm::utils::ansies::put_color};
    sanitization();frames(false);frames(true);
    uwvm2::uwvm::utils::ansies::put_color=original_color;
    fast_io::io::println("FAST_IO_TUI checks=",checks," failures=",failures);
    return failures ? 1 : 0;
}
