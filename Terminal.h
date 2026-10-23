#pragma once
// ============================================================================
//  Terminal.h  -  the ONE file that knows how to make the console look good.
//
//  Rule of the whole refactor:  screens never print colors, box characters or
//  widths by hand.  They call the small functions in here (term::Header,
//  term::Menu, term::Money ...).  Change the look once here -> every screen
//  changes.
//
//  Every line below has a comment explaining what it does and why.
// ============================================================================

// ---------------------------------------------------------------------------
// 1) Windows headers (needed for colors, UTF-8 output and key reading)
// ---------------------------------------------------------------------------
#ifndef NOMINMAX                  // windows.h defines macros called min() and max()...
#define NOMINMAX                  // ...which would break your std::numeric_limits<>::max() calls. This stops that.
#endif
#ifndef WIN32_LEAN_AND_MEAN       // windows.h is huge; this skips rarely used parts (faster compile)
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOGDI                     // windows.h also defines a macro named ERROR, which would clash with your Logger::Level::ERROR
#define NOGDI                     // NOGDI removes the graphics part of windows.h (including that macro). We only need console features.
#endif
#include <windows.h>              // SetConsoleOutputCP, GetConsoleMode, SetConsoleMode, GetConsoleScreenBufferInfo
#ifdef ERROR                      // belt and braces: if some other header still defined ERROR...
#undef ERROR                      // ...remove it so Logger::Level::ERROR keeps working
#endif
#include <conio.h>                // _getch(): reads ONE key press without waiting for Enter and without echo

// ---------------------------------------------------------------------------
// 2) Standard headers used by the helpers
// ---------------------------------------------------------------------------
#include <algorithm>              // std::min, std::max
#include <cctype>                 // std::toupper
#include <cmath>                  // std::fabs, std::lround
#include <cstdio>                 // std::snprintf (number -> text with exactly 2 decimals)
#include <cstdlib>                // std::atexit (run a function when the program ends)
#include <ctime>                  // std::time, std::localtime_s, std::strftime (current date/time)
#include <iostream>               // std::cout
#include <string>                 // std::string
#include <vector>                 // std::vector (menu items)

namespace term {                  // everything lives in the namespace "term" -> you always write term::Something

// ---------------------------------------------------------------------------
// 3) Layout constant
// ---------------------------------------------------------------------------
inline constexpr int kW = 66;     // default width (in characters) of a screen's content area. "inline constexpr" = one shared constant across all .cpp files.

// ---------------------------------------------------------------------------
// 4) Glyphs (special characters)
//    Written as raw UTF-8 bytes (\xE2\x94\x80 is the UTF-8 encoding of the character in the comment).
//    This means the compiler never has to understand the symbol itself, so you do NOT need the /utf-8 compiler flag.
//    Each glyph is its own constant because "\x80" followed by a letter A-F would be read as one bigger hex number.
// ---------------------------------------------------------------------------
namespace g {
inline constexpr const char* Rule   = "\xE2\x94\x80";   // ─  thin horizontal line
inline constexpr const char* Marker = "\xE2\x96\x8C";   // ▌  small vertical bar (section label)
inline constexpr const char* Play   = "\xE2\x96\xB6";   // ▶  "selected" arrow
inline constexpr const char* Full   = "\xE2\x96\x88";   // █  filled block (used in bars)
inline constexpr const char* Light  = "\xE2\x96\x91";   // ░  empty block (used in bars)
inline constexpr const char* CornerA= "\xE2\x97\xA2";   // ◢  logo piece 1
inline constexpr const char* CornerB= "\xE2\x97\xA4";   // ◤  logo piece 2
inline constexpr const char* Dot    = "\xE2\x97\x8F";   // ●  status light
inline constexpr const char* Check  = "\xE2\x9C\x94";   // ✔  success tick
inline constexpr const char* Cross  = "\xE2\x9C\x96";   // ✖  error cross
inline constexpr const char* Warn   = "\xE2\x96\xB2";   // ▲  warning triangle
inline constexpr const char* Mid    = "\xC2\xB7";       // ·  middle dot (separator)
inline constexpr const char* Up     = "\xE2\x86\x91";   // ↑
inline constexpr const char* Down   = "\xE2\x86\x93";   // ↓
inline constexpr const char* Arrow  = "\xE2\x86\x92";   // →
}

// ---------------------------------------------------------------------------
// 5) Colors  (ANSI "escape sequences")
//    \x1b is the ESC character. "ESC [ 38;2;R;G;B m" means "make the TEXT color this RGB".
//    "ESC [ 48;2;R;G;B m" means "make the BACKGROUND color this RGB".
// ---------------------------------------------------------------------------
namespace col {
inline constexpr const char* Base    = "\x1b[0m\x1b[48;2;0;0;0m\x1b[38;2;201;209;217m"; // 0m = reset everything, then pure BLACK background, then soft light-grey text. We use this instead of a plain reset so the background stays black.
inline constexpr const char* Bold    = "\x1b[1m";                  // bold text
inline constexpr const char* White   = "\x1b[38;2;255;255;255m";   // bright white text
inline constexpr const char* Cyan    = "\x1b[38;2;0;229;255m";     // main accent color
inline constexpr const char* DimCyan = "\x1b[38;2;10;74;85m";      // dark cyan, for thin lines
inline constexpr const char* Magenta = "\x1b[38;2;255;43;214m";    // highlight color
inline constexpr const char* Amber   = "\x1b[38;2;255;176;0m";     // warnings / money leaving
inline constexpr const char* Green   = "\x1b[38;2;0;255;156m";     // success / money arriving
inline constexpr const char* Red     = "\x1b[38;2;255;82;82m";     // errors
inline constexpr const char* Muted   = "\x1b[38;2;107;122;134m";   // secondary text (labels, hints)
inline constexpr const char* Faint   = "\x1b[38;2;58;70;80m";      // very quiet text (numbers in menu, empty bar blocks)
inline constexpr const char* SelBg   = "\x1b[48;2;6;34;42m";       // dark-teal BACKGROUND for the selected menu row
inline constexpr const char* OkBg    = "\x1b[48;2;4;38;26m";       // dark-green BACKGROUND for success banners
}

// ---------------------------------------------------------------------------
// 6) Start-up / shut-down
// ---------------------------------------------------------------------------
inline void Shutdown() {                               // runs automatically when the program ends (see Init)
    std::cout << "\x1b[?25h" << "\x1b[0m" << std::flush; // ?25h = show the cursor again, 0m = give the terminal its normal colors back
}

inline void Clear() {                                  // replaces system("cls")
    std::cout << col::Base                             // select the black background FIRST, so the cleared area is painted black
              << "\x1b[3J"                             // 3J = erase the scroll-back history too
              << "\x1b[H"                              // H  = move the cursor to the top-left corner
              << "\x1b[2J"                             // 2J = erase the visible screen (using the black background)
              << std::flush;                           // push it to the screen now
}

inline void Init() {                                   // call ONCE at the top of main()
    SetConsoleOutputCP(CP_UTF8);                       // tell Windows our text is UTF-8 so box/block characters display correctly
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);      // get a "handle" (a ticket) to the console output
    DWORD mode = 0;                                    // will receive the console's current settings
    if (GetConsoleMode(out, &mode))                    // read the settings (fails if output is redirected to a file -> then we skip)
        SetConsoleMode(out, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING); // turn ON "understand \x1b[...m color codes". "|" adds the flag without removing the others
    std::atexit(Shutdown);                             // when the program ends, restore normal colors + cursor
    Clear();                                           // paint the whole window black
}

// ---------------------------------------------------------------------------
// 7) Text helpers (measuring, padding, cutting, formatting)
// ---------------------------------------------------------------------------
inline int WindowWidth() {                             // how many characters fit in one console line right now
    CONSOLE_SCREEN_BUFFER_INFO info;                   // struct that Windows fills with window details
    if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info)) // ask Windows
        return 120;                                    // if it fails (e.g. output redirected), assume a normal 120-column window
    return info.srWindow.Right - info.srWindow.Left + 1; // visible window width = right edge - left edge + 1
}

inline int Margin(int w = kW) {                        // spaces needed on the left to CENTER a block that is w wide
    return std::max(0, (WindowWidth() - w) / 2);       // never negative (if the window is too narrow we just don't center)
}

inline std::string Gap(int w = kW) {                   // the margin as an actual string of spaces
    return std::string(static_cast<size_t>(Margin(w)), ' ');
}

inline size_t VisLen(const std::string& s) {           // how many CHARACTERS the user SEES (not bytes!)
    size_t n = 0;                                      // counter
    for (size_t i = 0; i < s.size(); ++i) {            // walk every byte
        if (s[i] == '\x1b') {                          // ESC starts a color code like \x1b[38;2;0;229;255m ...
            while (i < s.size() && s[i] != 'm') ++i;   // ...skip until its final letter 'm'. Color codes are invisible, they take 0 columns.
            continue;                                  // go to the next byte
        }
        if ((static_cast<unsigned char>(s[i]) & 0xC0) != 0x80) ++n; // UTF-8 trick: bytes that look like 10xxxxxx are "continuation" bytes of a multi-byte character, so only count the first byte of each character
    }
    return n;                                          // std::setw can't do this - it counts bytes, which breaks alignment with symbols
}

inline std::string Pad(const std::string& s, size_t w) {      // pad on the RIGHT with spaces up to w visible characters (left-aligned text)
    size_t v = VisLen(s);                              // visible length
    return v >= w ? s : s + std::string(w - v, ' ');   // already long enough? return as is. Otherwise add the missing spaces
}

inline std::string PadLeft(const std::string& s, size_t w) {  // pad on the LEFT (right-aligned text, used for money)
    size_t v = VisLen(s);
    return v >= w ? s : std::string(w - v, ' ') + s;
}

inline std::string Fit(const std::string& s, size_t w) {      // cut text so it is at most w visible characters, ending with "..." if it was cut
    if (VisLen(s) <= w) return s;                      // already fits -> unchanged
    if (w < 3) return s.substr(0, w);                  // absurdly small width -> hard cut
    size_t keep = w - 3;                               // we keep w-3 characters and add 3 dots
    size_t chars = 0, i = 0;                           // chars = characters kept so far, i = byte position
    while (i < s.size() && chars < keep) {             // advance one whole character at a time
        ++i;                                           // move past the first byte of the character
        while (i < s.size() && (static_cast<unsigned char>(s[i]) & 0xC0) == 0x80) ++i; // also skip its continuation bytes
        ++chars;                                       // one more character kept
    }
    return s.substr(0, i) + "...";                     // everything before position i, plus the dots
}

inline std::string Repeat(const char* glyph, int n) {  // glyph repeated n times (e.g. a line of ─)
    std::string r;                                     // result
    for (int i = 0; i < n; ++i) r += glyph;            // append n times
    return r;
}

inline std::string Money(double v) {                   // 12450 -> "$12,450.00"
    char buf[64];                                      // scratch buffer for the number text
    std::snprintf(buf, sizeof buf, "%.2f", std::fabs(v)); // write the absolute value with exactly 2 decimals, e.g. "12450.00"
    std::string s = buf;                               // work with a std::string
    size_t dot = s.find('.');                          // position of the decimal point
    for (long i = static_cast<long>(dot) - 3; i > 0; i -= 3) // every 3 digits to the left of the dot...
        s.insert(static_cast<size_t>(i), ",");         // ...insert a comma (12450.00 -> 12,450.00)
    return (v < 0 ? "-$" : "$") + s;                   // put $ in front (and a minus sign if negative)
}

inline std::string Upper(std::string s) {              // "first" -> "FIRST" (used for prompt labels)
    for (char& c : s) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c))); // convert every character
    return s;
}

inline std::string Fixed(double v, int decimals = 2) { // 3.14159 -> "3.14" (fixed number of decimals, no thousands separator)
    char buf[64];
    std::snprintf(buf, sizeof buf, "%.*f", decimals, v);
    return buf;
}

inline std::string Today() {                           // "20 OCT 2026 · 21:14"
    std::time_t t = std::time(nullptr);                // seconds since 1970 (the computer's clock)
    std::tm local{};                                   // broken-down time (year, month, day, hour...), zero-initialised
#ifdef _MSC_VER
    localtime_s(&local, &t);                           // Microsoft's safe version (fills 'local' from 't')
#else
    localtime_r(&t, &local);                           // same thing on other compilers (only used when testing outside Visual Studio)
#endif
    char d[32], h[16];                                 // two small text buffers
    std::strftime(d, sizeof d, "%d %b %Y", &local);    // date part -> "20 Oct 2026"
    std::strftime(h, sizeof h, "%H:%M", &local);       // time part -> "21:14"
    std::string s = std::string(d) + " " + g::Mid + " " + h; // glue them together with a middle dot
    for (char& c : s) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c))); // UPPERCASE it ("OCT") to match the style
    return s;
}

// ---------------------------------------------------------------------------
// 8) Basic printing
// ---------------------------------------------------------------------------
inline void Print(const std::string& s, int w = kW) {  // print ONE centered line. Every other function uses this.
    std::cout << Gap(w) << s << col::Base << '\n';     // margin + text + "reset to black background" + newline (the reset stops colors leaking into the next line)
}

inline void Blank() {                                  // print an empty line
    std::cout << '\n';
}

inline void Prompt(const std::string& label, int w = kW) {   // print "  label ▶ " and STAY on the same line so cin can read the answer
    std::cout << Gap(w) << col::Muted << "  " << label << " " << col::Cyan << g::Play << " " << col::Base; // muted label, cyan arrow, then back to normal colors for what the user types
    std::cout.flush();                                 // make sure the prompt is visible before we wait for typing
}

inline void Success(const std::string& m, int w = kW) { Print(std::string(col::Green) + "  " + g::Check + " " + m, w); } // green tick + message
inline void Error  (const std::string& m, int w = kW) { Print(std::string(col::Red)   + "  " + g::Cross + " " + m, w); } // red cross + message
inline void Warn   (const std::string& m, int w = kW) { Print(std::string(col::Amber) + "  " + g::Warn  + " " + m, w); } // amber triangle + message
inline void Info   (const std::string& m, int w = kW) { Print(std::string(col::Muted) + "  " + m, w); }                    // quiet grey message

inline void Section(const std::string& label, const std::string& hint = "", int w = kW) { // small section title like  ▌MODULES   (hint)
    Print(std::string(col::Cyan) + "  " + g::Marker + col::Muted + label + (hint.empty() ? "" : std::string("   ") + col::Faint + hint), w);
}

inline void Rule(int w = kW) {                         // a thin horizontal divider line
    Print(std::string(col::DimCyan) + "  " + Repeat(g::Rule, w - 4), w); // 2 spaces left + line + 2 spaces right = total width w
}

inline void Field(const std::string& label, const std::string& value,
                  const char* valueColor = col::Base, int w = kW) { // one "LABEL   value" line, used by cards (client, user, currency)
    Print(std::string(col::Muted) + "  " + Pad(label, 12) + valueColor + value, w); // grey label padded to 12 columns, then the value in its own color
}

inline std::string Bar(double fraction, int width, const char* color = col::Cyan) { // text progress bar: Bar(0.5, 10) = █████░░░░░
    fraction = std::max(0.0, std::min(1.0, fraction)); // force the value into the range 0..1
    int filled = static_cast<int>(std::lround(fraction * width)); // how many blocks to fill (rounded)
    if (fraction > 0.0 && filled == 0) filled = 1;     // a tiny-but-not-zero value still shows ONE block so it doesn't look empty
    return std::string(color) + Repeat(g::Full, filled) + col::Faint + Repeat(g::Light, width - filled) + col::Base; // colored filled part + faint empty part
}

inline void Header(const std::string& title, const std::string& info = "",
                   const std::string& user = "", int w = kW) {  // the header used on every screen
    const std::string logo = std::string(" ") + g::CornerA + g::CornerB + " ";  // " ◢◤ " (4 columns)
    const int titleLen = static_cast<int>(VisLen(title));       // visible width of the title
    const int infoLen  = static_cast<int>(VisLen(info));        // visible width of the optional info text (0 if empty)
    // columns used by everything except the long line: logo(4) + title + space(1) [+ space(1) + info + space(1) when info exists]
    const int used = 4 + titleLen + 1 + (info.empty() ? 0 : 1 + infoLen + 1);
    const int line = std::max(2, w - used);            // the rest of the width is filled with the thin line (at least 2)
    Blank();                                           // one empty line above the header for breathing room
    std::string text = std::string(col::Cyan) + logo + col::White + col::Bold + title + col::Base + " "
                     + col::DimCyan + Repeat(g::Rule, line) + col::Base;      // logo + bold title + thin line
    if (!info.empty()) text += std::string(" ") + col::Muted + info + " ";    // optional quiet text at the right end, e.g. "5 RECORDS"
    Print(text, w);
    if (!user.empty())                                 // second line only if a user name was given
        Print(std::string(col::Muted) + "    " + user + " " + g::Mid + " " + Today(), w); // "    admin · 20 OCT 2026 · 21:14"
    Blank();
}

// ---------------------------------------------------------------------------
// 9) Waiting for a key
// ---------------------------------------------------------------------------
inline void WaitKey(const std::string& message = "Press any key to go back") { // replaces the old "cin.ignore + cin.get" trick
    std::cout << '\n' << Gap() << col::Faint << "  " << message << col::Base; // show the hint (faint, so it doesn't shout)
    std::cout.flush();
    int k = _getch();                                  // wait for ONE key press
    if (k == 0 || k == 224) _getch();                  // arrow/function keys send TWO codes (a prefix 0 or 224, then the real code). Swallow the second one so it doesn't leak into the next menu.
    std::cout << '\n';
}

// ---------------------------------------------------------------------------
// 9b) Reading ONE key as a friendly name (used for paging)
// ---------------------------------------------------------------------------
enum class Key { Other, Enter, Esc, Up, Down, Left, Right, PageUp, PageDown, Home, End };   // the keys our screens care about

inline Key ReadKey() {                                 // waits for one key press and translates the raw codes into a Key
    int k = _getch();                                  // first code
    if (k == 0 || k == 224) {                          // arrow / function / page keys send a prefix (0 or 224) then the real code
        switch (_getch()) {
            case 72: return Key::Up;                   // 72 = Up arrow
            case 80: return Key::Down;                 // 80 = Down arrow
            case 75: return Key::Left;                 // 75 = Left arrow
            case 77: return Key::Right;                // 77 = Right arrow
            case 73: return Key::PageUp;               // 73 = Page Up
            case 81: return Key::PageDown;             // 81 = Page Down
            case 71: return Key::Home;                 // 71 = Home
            case 79: return Key::End;                  // 79 = End
            default: return Key::Other;
        }
    }
    if (k == 13) return Key::Enter;                    // 13 = Enter
    if (k == 27) return Key::Esc;                      // 27 = Esc
    return Key::Other;                                 // any other key
}

// ---------------------------------------------------------------------------
// 9c) Paging: show a long list page by page
//     count    = how many rows exist,   pageSize = rows per page,   w = width of the table
//     header(info) prints the screen header (info = "PAGE 2/13 · 220 RECORDS" for the right end of the header)
//     layout()     prints the column titles,    row(i) prints row number i (0-based)
//     Keys: Down/Right/PageDown/Enter = next page, Up/Left/PageUp = previous, Home/End = first/last, Esc = leave
// ---------------------------------------------------------------------------
template <class HeaderFn, class LayoutFn, class RowFn>
inline void Paginate(size_t count, size_t pageSize, int w, HeaderFn header, LayoutFn layout, RowFn row) {
    const size_t pages = std::max<size_t>(1, (count + pageSize - 1) / pageSize);   // number of pages, rounded up
    size_t page = 0;                                   // current page (0-based)
    for (;;) {                                         // redraw -> read key -> change page, until the user leaves
        Clear();                                       // fresh black screen for every page
        header("PAGE " + std::to_string(page + 1) + "/" + std::to_string(pages) + "  " + g::Mid + "  " + std::to_string(count) + " RECORDS");
        layout();                                      // column titles
        const size_t from = page * pageSize;           // first row of this page
        const size_t to = std::min(count, from + pageSize);   // one past the last row of this page
        for (size_t i = from; i < to; ++i) row(i);     // print this page's rows
        Rule(w);                                       // closing line
        Print(std::string(col::Faint) + "  " + (pages > 1 ? std::string(g::Up) + g::Down + " / Enter  change page   " + g::Mid + "   " : std::string()) + "Esc  back", w);

        const Key key = ReadKey();                     // one key press
        if (key == Key::Esc) return;                   // leave
        if ((key == Key::Down || key == Key::Right || key == Key::PageDown || key == Key::Enter) && page + 1 < pages) ++page;       // next page
        else if ((key == Key::Up || key == Key::Left || key == Key::PageUp) && page > 0) --page;                                   // previous page
        else if (key == Key::Home) page = 0;           // first page
        else if (key == Key::End) page = pages - 1;    // last page
        else if (key == Key::Enter && page + 1 >= pages) return;   // Enter on the last page = done
    }
}

// ---------------------------------------------------------------------------
// 10) Hidden password input
// ---------------------------------------------------------------------------
inline std::string ReadPassword(const std::string& label, int w = kW) { // shows * instead of the real characters
    Prompt(label, w);                                  // print the prompt
    std::string pass;                                  // what the user typed so far
    for (;;) {                                         // loop until Enter
        int k = _getch();                              // read one key (nothing is echoed by _getch)
        if (k == '\r' || k == '\n') break;             // Enter -> finished
        if (k == 0 || k == 224) { _getch(); continue; }// special key (arrows...) -> swallow second code and ignore
        if (k == '\b') {                               // Backspace
            if (!pass.empty()) {                       // only if there is something to delete
                pass.pop_back();                       // remove last character from the string
                std::cout << "\b \b" << std::flush;    // move cursor back, overwrite the star with a space, move back again
            }
        } else if (k >= 32 && k < 127) {               // normal printable ASCII character
            pass.push_back(static_cast<char>(k));      // store the real character
            std::cout << '*' << std::flush;            // but show a star
        }
    }
    std::cout << '\n';                                 // move to the next line after Enter
    return pass;                                       // hand the password back
}

// ---------------------------------------------------------------------------
// 10b) Yes / No question (single key, no Enter needed)
// ---------------------------------------------------------------------------
inline bool Confirm(const std::string& question, int w = kW) { // asks a yes/no question; returns true for Y, false for N
    std::cout << Gap(w) << col::Cyan << "  ? " << col::Base << question               // margin + cyan question mark + the question
              << col::Muted << "  [Y/N] " << col::Base;                                  // grey "[Y/N]" hint
    std::cout.flush();                                 // show the question before waiting
    for (;;) {                                         // keep asking until a valid key is pressed
        int k = _getch();                              // read one key
        if (k == 0 || k == 224) { _getch(); continue; }// arrow keys etc.: swallow the second code and ignore
        if (k == 'y' || k == 'Y') {                    // yes
            std::cout << col::Green << "Yes" << col::Base << '\n';
            return true;
        }
        if (k == 'n' || k == 'N') {                    // no
            std::cout << col::Red << "No" << col::Base << '\n';
            return false;
        }
        // Enter and every other key are ignored on purpose: with money involved, "default yes" would be dangerous.
    }
}

// ---------------------------------------------------------------------------
// 11) The arrow-key menu
// ---------------------------------------------------------------------------
struct MenuItem {                                      // one line of a menu
    std::string label;                                 // main text, e.g. "SHOW CLIENTS"
    std::string hint;                                  // quiet explanation, e.g. "view every account"
};

inline std::string MenuRow(const MenuItem& item, int index, bool selected) { // builds ONE row as a string (does not print it)
    char num[16];                                      // buffer for "01", "02" ... (16 is more than enough)
    std::snprintf(num, sizeof num, "%02d", index + 1); // number with a leading zero
    std::string text;                                  // we build the row piece by piece
    if (selected) {                                    // ---- selected row ----
        text += "  ";                                  // start 2 columns further left so the arrow fits in the same 4-column slot
        text += col::Cyan;                             // arrow color
        text += g::Play;                               // the arrow
        text += " ";                                   // space after the arrow
        text += col::Cyan;                             // number color (bright)
    } else {                                           // ---- normal row ----
        text += "    ";                                // 4 spaces (same width as "  " + arrow + space)
        text += col::Faint;                            // number color (quiet)
    }
    text += num;                                       // "01".."10"
    text += "  ";                                      // gap between number and label
    text += selected ? std::string(col::White) + col::Bold : std::string(col::Base); // label: bright bold white if selected, normal otherwise (we avoid Base on the selected row because it would erase the highlight background)
    text += Pad(item.label, 20);                       // label padded to 20 columns so all hints line up
    text += col::Muted;                                // hint color
    text += item.hint;                                 // the hint text
    const size_t len = VisLen(text);                   // how wide did that come out?
    if (len < static_cast<size_t>(kW)) text += std::string(kW - len, ' '); // pad with spaces to the full width so the highlight bar reaches the right edge
    return selected ? std::string(col::SelBg) + text + col::Base : text;    // selected: wrap in the dark-teal background, normal: as is
}

// Shows the menu and lets the user choose with the keyboard.
//   Up/Down  = move      Home/End = first/last      1-9 and 0 = jump (0 means item 10)
//   Enter    = choose  -> returns the item's index (0-based)
//   Esc      = cancel  -> returns -1
inline int Menu(const std::vector<MenuItem>& items, int selected = 0) {
    const int n = static_cast<int>(items.size());      // number of rows
    if (n == 0) return -1;                             // nothing to choose from
    selected = std::max(0, std::min(selected, n - 1)); // make sure the starting row is valid
    std::cout << "\x1b[?25l";                          // hide the blinking cursor while the menu is on screen
    bool firstDraw = true;                             // the first time we print rows, the second time we REDRAW them
    for (;;) {                                         // main loop: draw -> wait for key -> react
        if (!firstDraw) std::cout << "\x1b[" << n << "A"; // ESC[nA = move the cursor UP n lines, back to the first menu row (this is why the screen doesn't flicker: we repaint in place instead of clearing)
        firstDraw = false;                             // from now on we are redrawing
        for (int i = 0; i < n; ++i)                    // print every row
            std::cout << '\r' << Gap() << MenuRow(items[i], i, i == selected) << col::Base << '\n'; // '\r' = go to column 0, then margin + row + newline
        std::cout.flush();                             // show it now

        int k = _getch();                              // wait for a key
        if (k == 0 || k == 224) {                      // special key -> a second code tells which one
            k = _getch();                              // read that code
            if (k == 72) selected = (selected + n - 1) % n;      // 72 = Up arrow   -> previous row (wraps from first to last)
            else if (k == 80) selected = (selected + 1) % n;     // 80 = Down arrow -> next row (wraps from last to first)
            else if (k == 71) selected = 0;                      // 71 = Home       -> first row
            else if (k == 79) selected = n - 1;                  // 79 = End        -> last row
        } else if (k == 13) {                          // 13 = Enter
            std::cout << "\x1b[?25h";                  // show the cursor again
            return selected;                           // tell the caller which row was chosen
        } else if (k == 27) {                          // 27 = Esc
            std::cout << "\x1b[?25h";                  // show the cursor again
            return -1;                                 // -1 = "cancelled"
        } else if (k >= '1' && k <= '9' && (k - '1') < n) { // keys 1..9 jump to that row
            selected = k - '1';                        // '1' is character code 49, so k - '1' gives 0 for key "1", 1 for key "2", ...
        } else if (k == '0' && n >= 10) {              // key 0 jumps to row 10
            selected = 9;
        }
    }
}

} // namespace term
