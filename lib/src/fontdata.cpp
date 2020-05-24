#include "fontdata.h"
#include <algorithm>

namespace f2b {

namespace font {

Glyph::Glyph(Size sz) :
    size_ { sz },
    pixels_ { std::vector<bool>(sz.width * sz.height, false) }
{}

Glyph::Glyph(Size sz, std::vector<bool> pixels) :
    size_ { sz },
    pixels_ { std::move(pixels) }
{
    if (pixels_.size() != sz.width * sz.height) {
        throw std::logic_error { "pixels size must equal glyph size (width * height)" };
    }
}

void Glyph::clear()
{
    pixels_ = std::vector<bool>(size_.width * size_.height, false);
}

std::size_t Glyph::top_margin() const
{
    auto first_set_pixel = std::find(pixels_.begin(), pixels_.end(), true);
    return static_cast<std::size_t>(std::distance(pixels_.begin(), first_set_pixel)) / size_.width;
}

std::size_t Glyph::bottom_margin() const
{
    auto last_set_pixel = std::find(pixels_.rbegin(), pixels_.rend(), true);
    return static_cast<std::size_t>(std::distance(pixels_.rbegin(), last_set_pixel)) / size_.width;
}


Face::Face(const FaceReader &data) :
    Face(data.font_size(), read_glyphs(data))
{
    for (std::size_t i = 0; i < glyphs_.size(); i++) {
        exported_glyph_ids_.insert(i);
    }
}

Face::Face(Size size, std::vector<Glyph> glyphs, std::set<std::size_t> exported_glyph_ids) :
    sz_ { size },
    glyphs_ { std::move(glyphs) },
    exported_glyph_ids_ { std::move(exported_glyph_ids) }
{}

std::vector<Glyph> Face::read_glyphs(const FaceReader &data)
{
    std::vector<Glyph> glyphs;
    glyphs.reserve(data.num_glyphs());

    const std::vector<bool>::size_type pixels_per_glyph { data.font_size().width * data.font_size().height };

    for (std::size_t i = 0; i < data.num_glyphs(); i++) {

        std::vector<bool> pixels;
        pixels.reserve(pixels_per_glyph);

        for (std::size_t y = 0; y < data.font_size().height; y++) {
            for (std::size_t x = 0; x < data.font_size().width; x++) {
                Point p { x, y };
                pixels.push_back(data.is_pixel_set(i, p));
            }
        }

        Glyph g { data.font_size(), pixels };
        glyphs.push_back(g);
    }

    return glyphs;
}

Margins Face::calculate_margins() const noexcept
{
    Margins m {sz_.height, sz_.height};

    std::for_each(glyphs_.begin(), glyphs_.end(), [&](const font::Glyph& g) {
        m.top = std::min(m.top, g.top_margin());
        m.bottom = std::min(m.bottom, g.bottom_margin());
    });

    return m;
}

} // namespace Font
} // namespace f2b
