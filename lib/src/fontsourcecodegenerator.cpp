#include "fontsourcecodegenerator.h"
#include <iomanip>
#include <string>

Font::Margins pixel_margins(Font::Margins line_margins, Font::Size glyph_size)
{
    return { line_margins.top * glyph_size.width, line_margins.bottom * glyph_size.width };
}

std::string FontSourceCodeGenerator::current_timestamp()
{
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream s;
    s << std::put_time(&tm, "%d-%m-%Y %H:%M:%S");
    return s.str();
}

std::string FontSourceCodeGenerator::comment_for_glyph(std::size_t index)
{
    index += 32;
    std::ostringstream s;
    s << "Character 0x"
      << std::hex << std::setfill('0') << std::setw(2) << index
      << std::dec << " (" << index;

    if (std::isprint(index)) {
        s << ": '" << static_cast<char>(index) << "'";
    }

    s << ")";

    return s.str();
}

std::string FontSourceCodeGenerator::lut_value_for_glyph(std::size_t index)
{
    if (index == 0) {
        return "0";
    }

    std::ostringstream s;
    s << "bytes_per_char * " << index;
    return s.str();
}
