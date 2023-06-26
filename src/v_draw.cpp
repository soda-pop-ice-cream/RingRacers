// DR. ROBOTNIK'S RING RACERS
//-----------------------------------------------------------------------------
// Copyright (C) 2023 by James Robert Roman
//
// This program is free software distributed under the
// terms of the GNU General Public License, version 2.
// See the 'LICENSE' file for more details.
//-----------------------------------------------------------------------------

#include "doomdef.h" // skincolornum_t
#include "doomtype.h"
#include "hu_stuff.h"
#include "k_hud.h"
#include "m_fixed.h"
#include "r_draw.h"
#include "v_draw.hpp"
#include "v_video.h"
#include "w_wad.h"
#include "z_zone.h"

using srb2::Draw;
using Chain = Draw::Chain;

int Draw::TextElement::width() const
{
	return font_ ? font_width(*font_, default_font_flags(*font_) | flags_.value_or(0), string_.c_str()) / FRACUNIT : 0;
}

void Chain::patch(patch_t* patch) const
{
	const auto _ = Clipper(*this);

	const bool stretchH = stretch_ == Stretch::kWidth || stretch_ == Stretch::kBoth;
	const bool stretchV = stretch_ == Stretch::kHeight || stretch_ == Stretch::kBoth;

	const fixed_t h = stretchH ? FloatToFixed(width_ / patch->width) : FRACUNIT;
	const fixed_t v = stretchV ? FloatToFixed(height_ / patch->height) : FRACUNIT;

	V_DrawStretchyFixedPatch(FloatToFixed(x_), FloatToFixed(y_), h * scale_, v * scale_, flags_, patch, colormap_);
}

void Chain::patch(const char* name) const
{
	patch(static_cast<patch_t*>(W_CachePatchName(name, PU_CACHE)));
}

void Chain::thumbnail(UINT16 mapnum) const
{
	const auto _ = Clipper(*this);

	K_DrawMapThumbnail(FloatToFixed(x_), FloatToFixed(y_), FloatToFixed(width_), flags_, mapnum, colormap_);
}

void Chain::fill(UINT8 color) const
{
	const auto _ = Clipper(*this);

	V_DrawFill(x_, y_, width_, height_, color);
}

void Chain::string(const char* str, INT32 flags, Font font) const
{
	const auto _ = Clipper(*this);

	flags |= default_font_flags(font);

	fixed_t x = FloatToFixed(x_);
	fixed_t y = FloatToFixed(y_);

	switch (align_)
	{
	case Align::kLeft:
		break;

	case Align::kCenter:
		x -= (font_width(font, flags, str) / 2) * scale_;
		break;

	case Align::kRight:
		x -= font_width(font, flags, str) * scale_;
		break;
	}

	V_DrawStringScaled(x, y, FloatToFixed(scale_), FRACUNIT, FRACUNIT, flags, colormap_, font_to_fontno(font), str);
}

Chain::Clipper::Clipper(const Chain& chain)
{
	V_SetClipRect(
		FloatToFixed(chain.clipx1_),
		FloatToFixed(chain.clipy1_),
		FloatToFixed(chain.clipx2_ - chain.clipx1_),
		FloatToFixed(chain.clipy2_ - chain.clipy1_),
		0
	);
}

Chain::Clipper::~Clipper()
{
	V_ClearClipRect();
}

int Draw::font_to_fontno(Font font)
{
	switch (font)
	{
	case Font::kThin:
		return TINY_FONT;

	case Font::kGamemode:
		return GM_FONT;

	case Font::kConsole:
		return HU_FONT;

	case Font::kFreeplay:
		return KART_FONT;
	}

	return TINY_FONT;
};

INT32 Draw::default_font_flags(Font font)
{
	INT32 flags = V_ALLOWLOWERCASE;

	if (font == Font::kThin)
	{
		flags |= V_6WIDTHSPACE;
	}

	return flags;
};

fixed_t Draw::font_width(Font font, INT32 flags, const char* string)
{
	return V_StringScaledWidth(FRACUNIT, FRACUNIT, FRACUNIT, flags, font_to_fontno(font), string);
}
