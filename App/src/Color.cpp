/*
 *******************************************************************
 *** This software is copyright 1985-2014 by Michael H Riley     ***
 *** You have permission to use, modify, copy, and distribute    ***
 *** this software so long as this copyright notice is retained. ***
 *** This software may not be used in commercial applications    ***
 *** without express written permission from the author.         ***
 *******************************************************************
*/


#include "SmrAppFramework.h"

namespace SmrFramework {

  Color::Color(int r, int g, int b) {
    this->red = r;
    this->green = g;
    this->blue = b;
    this->pixel = Color::RGB(r, g, b);
    }

  Color::~Color() {
    }

  u_int32_t Color::RGB(int red, int green, int blue) {
    u_int32_t pixel;
    if (application->red_shift < 0)
      pixel = (red & application->red_mask) << -application->red_shift;
    else
      pixel = (red & application->red_mask) >> application->red_shift;
    if (application->green_shift < 0)
      pixel |= (green & application->green_mask) << -application->green_shift;
    else
      pixel |= (green & application->green_mask) >> application->green_shift;
    if (application->blue_shift < 0)
      pixel |= (blue & application->blue_mask) << -application->blue_shift;
    else
      pixel |= (blue & application->blue_mask) >> application->blue_shift;
    return pixel;
    }

  u_int32_t Color::Pixel() {
    return pixel;
    }

  u_int32_t Color::AliceBlue() {
    return RGB(0xF0F0, 0xF8F8, 0xFFFF);
    }

  u_int32_t Color::AntiqueWhite() {
    return RGB(0xFAFA, 0xEBEB, 0xD7D7);
    }

  u_int32_t Color::Aqua() {
    return RGB(0x0000, 0xFFFF, 0xFFFF);
    }

  u_int32_t Color::Aquamarine() {
    return RGB(0x7F7F, 0xFFFF, 0xD4D4);
    }

  u_int32_t Color::Azure() {
    return RGB(0xF0F0, 0xFFFF, 0xFFFF);
    }

  u_int32_t Color::Beige() {
    return RGB(0xF5F5, 0xF5F5, 0xDCDC);
    }

  u_int32_t Color::Bisque() {
    return RGB(0xFFFF, 0xE4E4, 0xC4C4);
    }

  u_int32_t Color::Black() {
    return RGB(0x0000, 0x0000, 0x0000);
    }

  u_int32_t Color::BlanchedAlmond() {
    return RGB(0xFFFF, 0xEBEB, 0xCDCD);
    }

  u_int32_t Color::Blue() {
    return RGB(0x0000, 0x0000, 0xFFFF);
    }

  u_int32_t Color::BlueViolet() {
    return RGB(0x8A8A, 0x2B2B, 0xE2E2);
    }

  u_int32_t Color::Brown() {
    return RGB(0xA5A5, 0x2A2A, 0x2A2A);
    }

  u_int32_t Color::Burlywood() {
    return RGB(0xDEDE, 0xB8B8, 0x8787);
    }

  u_int32_t Color::CadetBlue() {
    return RGB(0x5F5F, 0x9E9E, 0xA0A0);
    }

  u_int32_t Color::Chartreuse() {
    return RGB(0x7F7F, 0xFFFF, 0x0000);
    }

  u_int32_t Color::Chocolate() {
    return RGB(0xD2D2, 0x6969, 0x1E1E);
    }

  u_int32_t Color::Coral() {
    return RGB(0xFFFF, 0x7F7F, 0x5050);
    }

  u_int32_t Color::CornflowerBlue() {
    return RGB(0x6464, 0x9595, 0xEDED);
    }

  u_int32_t Color::Cornsilk() {
    return RGB(0xFFFF, 0xF8F8, 0xDCDC);
    }

  u_int32_t Color::Crimson() {
    return RGB(0xDCDC, 0x1414, 0x3C3C);
    }

  u_int32_t Color::Cyan() {
    return RGB(0x0000, 0xFFFF, 0xFFFF);
    }

  u_int32_t Color::DarkBlue() {
    return RGB(0x0000, 0x0000, 0x8B8B);
    }

  u_int32_t Color::DarkCyan() {
    return RGB(0x0000, 0x8B8B, 0x8B8B);
    }

  u_int32_t Color::DarkGoldenrod() {
    return RGB(0xB8B8, 0x8686, 0x0B0B);
    }

  u_int32_t Color::DarkGray() {
    return RGB(0xA9A9, 0xA9A9, 0xA9A9);
    }

  u_int32_t Color::DarkGreen() {
    return RGB(0x0000, 0x6464, 0x0000);
    }

  u_int32_t Color::DarkKhaki() {
    return RGB(0xBDBD, 0xB7B7, 0x6B6B);
    }

  u_int32_t Color::DarkMagenta() {
    return RGB(0x8B8B, 0x0000, 0x8B8B);
    }

  u_int32_t Color::DarkOliveGreen() {
    return RGB(0x5555, 0x6B6B, 0x2F2F);
    }

  u_int32_t Color::DarkOrange() {
    return RGB(0xFFFF, 0x8C8C, 0x0000);
    }

  u_int32_t Color::DarkOrchid() {
    return RGB(0x9999, 0x3232, 0xCCCC);
    }

  u_int32_t Color::DarkRed() {
    return RGB(0x8B8B, 0x0000, 0x0000);
    }

  u_int32_t Color::DarkSalmon() {
    return RGB(0xE9E9, 0x9696, 0x7A7A);
    }

  u_int32_t Color::DarkSeaGreen() {
    return RGB(0x8F8F, 0xBCBC, 0x8F8F);
    }

  u_int32_t Color::DarkSlateBlue() {
    return RGB(0x4848, 0x3D3D, 0x8B8B);
    }

  u_int32_t Color::DarkSlateGray() {
    return RGB(0x2F2F, 0x4F4F, 0x4F4F);
    }

  u_int32_t Color::DarkTurquoise() {
    return RGB(0x0000, 0xCECE, 0xD1D1);
    }

  u_int32_t Color::DarkViolet() {
    return RGB(0x9494, 0x0000, 0xD3D3);
    }

  u_int32_t Color::DeepPink() {
    return RGB(0xFFFF, 0x1414, 0x9393);
    }

  u_int32_t Color::DeepSkyBlue() {
    return RGB(0x0000, 0xBFBF, 0xFFFF);
    }

  u_int32_t Color::DimGray() {
    return RGB(0x6969, 0x6969, 0x6969);
    }

  u_int32_t Color::DodgerBlue() {
    return RGB(0x1E1E, 0x9090, 0xFFFF);
    }

  u_int32_t Color::Firebrick() {
    return RGB(0xB2B2, 0x2222, 0x2222);
    }

  u_int32_t Color::FloralWhite() {
    return RGB(0xFFFF, 0xFAFA, 0xF0F0);
    }

  u_int32_t Color::ForestGreen() {
    return RGB(0x2222, 0x8B8B, 0x2222);
    }

  u_int32_t Color::Fuchsia() {
    return RGB(0xFFFF, 0x0000, 0xFFFF);
    }

  u_int32_t Color::Gainsboro() {
    return RGB(0xDCDC, 0xDCDC, 0xDCDC);
    }

  u_int32_t Color::GhostWhite() {
    return RGB(0xF8F8, 0xF8F8, 0xFFFF);
    }

  u_int32_t Color::Gold() {
    return RGB(0xFFFF, 0xD7D7, 0x0000);
    }

  u_int32_t Color::Goldenrod() {
    return RGB(0xDADA, 0xA5A5, 0x2020);
    }

  u_int32_t Color::Gray() {
    return RGB(0xBEBE, 0xBEBE, 0xBEBE);
    }

  u_int32_t Color::WebGray() {
    return RGB(0x8080, 0x8080, 0x8080);
    }

  u_int32_t Color::Green() {
    return RGB(0x0000, 0xFFFF, 0x0000);
    }

  u_int32_t Color::WebGreen() {
    return RGB(0x0000, 0x8080, 0x0000);
    }

  u_int32_t Color::GreenYellow() {
    return RGB(0xADAD, 0xFFFF, 0x2F2F);
    }

  u_int32_t Color::Honeydew() {
    return RGB(0xF0F0, 0xFFFF, 0xF0F0);
    }

  u_int32_t Color::HotPink() {
    return RGB(0xFFFF, 0x6969, 0xB4B4);
    }

  u_int32_t Color::IndianRed() {
    return RGB(0xCDCD, 0x5C5C, 0x5C5C);
    }

  u_int32_t Color::Indigo() {
    return RGB(0x4B4B, 0x0000, 0x8282);
    }

  u_int32_t Color::Ivory() {
    return RGB(0xFFFF, 0xFFFF, 0xF0F0);
    }

  u_int32_t Color::Khaki() {
    return RGB(0xF0F0, 0xE6E6, 0x8C8C);
    }

  u_int32_t Color::Lavender() {
    return RGB(0xE6E6, 0xE6E6, 0xFAFA);
    }

  u_int32_t Color::LavenderBlush() {
    return RGB(0xFFFF, 0xF0F0, 0xF5F5);
    }

  u_int32_t Color::LawnGreen() {
    return RGB(0x7C7C, 0xFCFC, 0x0000);
    }

  u_int32_t Color::LemonChiffon() {
    return RGB(0xFFFF, 0xFAFA, 0xCDCD);
    }

  u_int32_t Color::LightBlue() {
    return RGB(0xADAD, 0xD8D8, 0xE6E6);
    }

  u_int32_t Color::LightCoral() {
    return RGB(0xF0F0, 0x8080, 0x8080);
    }

  u_int32_t Color::LightCyan() {
    return RGB(0xE0E0, 0xFFFF, 0xFFFF);
    }

  u_int32_t Color::LightGoldenrod() {
    return RGB(0xFAFA, 0xFAFA, 0xD2D2);
    }

  u_int32_t Color::LightGray() {
    return RGB(0xD3D3, 0xD3D3, 0xD3D3);
    }

  u_int32_t Color::LightGreen() {
    return RGB(0x9090, 0xEEEE, 0x9090);
    }

  u_int32_t Color::LightPink() {
    return RGB(0xFFFF, 0xB6B6, 0xC1C1);
    }

  u_int32_t Color::LightSalmon() {
    return RGB(0xFFFF, 0xA0A0, 0x7A7A);
    }

  u_int32_t Color::LightSeaGreen() {
    return RGB(0x2020, 0xB2B2, 0xAAAA);
    }

  u_int32_t Color::LightSkyBlue() {
    return RGB(0x8787, 0xCECE, 0xFAFA);
    }

  u_int32_t Color::LightSlateGray() {
    return RGB(0x7777, 0x8888, 0x9999);
    }

  u_int32_t Color::LightSteelBlue() {
    return RGB(0xB0B0, 0xC4C4, 0xDEDE);
    }

  u_int32_t Color::LightYellow() {
    return RGB(0xFFFF, 0xFFFF, 0xE0E0);
    }

  u_int32_t Color::Lime() {
    return RGB(0x0000, 0xFFFF, 0x0000);
    }

  u_int32_t Color::LimeGreen() {
    return RGB(0x3232, 0xCDCD, 0x3232);
    }

  u_int32_t Color::Linen() {
    return RGB(0xFAFA, 0xF0F0, 0xE6E6);
    }

  u_int32_t Color::Magenta() {
    return RGB(0xFFFF, 0x0000, 0xFFFF);
    }

  u_int32_t Color::Maroon() {
    return RGB(0xB0B0, 0x3030, 0x6060);
    }

  u_int32_t Color::WebMaroon() {
    return RGB(0x8080, 0x0000, 0x0000);
    }

  u_int32_t Color::MediumAquamarine() {
    return RGB(0x6666, 0xCDCD, 0xAAAA);
    }

  u_int32_t Color::MediumBlue() {
    return RGB(0x0000, 0x0000, 0xCDCD);
    }

  u_int32_t Color::MediumOrchid() {
    return RGB(0xBABA, 0x5555, 0xD3D3);
    }

  u_int32_t Color::MediumPurple() {
    return RGB(0x9393, 0x7070, 0xDBDB);
    }

  u_int32_t Color::MediumSeaGreen() {
    return RGB(0x3C3C, 0xB3B3, 0x7171);
    }

  u_int32_t Color::MediumSlateBlue() {
    return RGB(0x7B7B, 0x6868, 0xEEEE);
    }

  u_int32_t Color::MediumSpringGreen() {
    return RGB(0x0000, 0xFAFA, 0x9A9A);
    }

  u_int32_t Color::MediumTurquoise() {
    return RGB(0x4848, 0xD1D1, 0xCCCC);
    }

  u_int32_t Color::MediumVioletRed() {
    return RGB(0xC7C7, 0x1515, 0x8585);
    }

  u_int32_t Color::MidnightBlue() {
    return RGB(0x1919, 0x1919, 0x7070);
    }

  u_int32_t Color::MintCream() {
    return RGB(0xF5F5, 0xFFFF, 0xFAFA);
    }

  u_int32_t Color::MistyRose() {
    return RGB(0xFFFF, 0xE4E4, 0xE1E1);
    }

  u_int32_t Color::Moccasin() {
    return RGB(0xFFFF, 0xE4E4, 0xB5B5);
    }

  u_int32_t Color::NavajoWhite() {
    return RGB(0xFFFF, 0xDEDE, 0xADAD);
    }

  u_int32_t Color::NavyBlue() {
    return RGB(0x0000, 0x0000, 0x8080);
    }

  u_int32_t Color::OldLace() {
    return RGB(0xFDFD, 0xF5F5, 0xE6E6);
    }

  u_int32_t Color::Olive() {
    return RGB(0x8080, 0x8080, 0x0000);
    }

  u_int32_t Color::OliveDrab() {
    return RGB(0x6B6B, 0x8E8E, 0x2323);
    }

  u_int32_t Color::Orange() {
    return RGB(0xFFFF, 0xA5A5, 0x0000);
    }

  u_int32_t Color::OrangeRed() {
    return RGB(0xFFFF, 0x4545, 0x0000);
    }

  u_int32_t Color::Orchid() {
    return RGB(0xDADA, 0x7070, 0xD6D6);
    }

  u_int32_t Color::PaleGoldenrod() {
    return RGB(0xEEEE, 0xE8E8, 0xAAAA);
    }

  u_int32_t Color::PaleGreen() {
    return RGB(0x9898, 0xFBFB, 0x9898);
    }

  u_int32_t Color::PaleTurquoise() {
    return RGB(0xAFAF, 0xEEEE, 0xEEEE);
    }

  u_int32_t Color::PaleVioletRed() {
    return RGB(0xDBDB, 0x7070, 0x9393);
    }

  u_int32_t Color::PapayaWhip() {
    return RGB(0xFFFF, 0xEFEF, 0xD5D5);
    }

  u_int32_t Color::PeachPuff() {
    return RGB(0xFFFF, 0xDADA, 0xB9B9);
    }

  u_int32_t Color::Peru() {
    return RGB(0xCDCD, 0x8585, 0x3F3F);
    }

  u_int32_t Color::Pink() {
    return RGB(0xFFFF, 0xC0C0, 0xCBCB);
    }

  u_int32_t Color::Plum() {
    return RGB(0xDDDD, 0xA0A0, 0xDDDD);
    }

  u_int32_t Color::PowderBlue() {
    return RGB(0xB0B0, 0xE0E0, 0xE6E6);
    }

  u_int32_t Color::Purple() {
    return RGB(0xA0A0, 0x2020, 0xF0F0);
    }

  u_int32_t Color::WebPurple() {
    return RGB(0x8080, 0x0000, 0x8080);
    }

  u_int32_t Color::RebeccaPurple() {
    return RGB(0x6666, 0x3333, 0x9999);
    }

  u_int32_t Color::Red() {
    return RGB(0xFFFF, 0x0000, 0x0000);
    }

  u_int32_t Color::RosyBrown() {
    return RGB(0xBCBC, 0x8F8F, 0x8F8F);
    }

  u_int32_t Color::RoyalBlue() {
    return RGB(0x4141, 0x6969, 0xE1E1);
    }

  u_int32_t Color::SaddleBrown() {
    return RGB(0x8B8B, 0x4545, 0x1313);
    }

  u_int32_t Color::Salmon() {
    return RGB(0xFAFA, 0x8080, 0x7272);
    }

  u_int32_t Color::SandyBrown() {
    return RGB(0xF4F4, 0xA4A4, 0x6060);
    }

  u_int32_t Color::SeaGreen() {
    return RGB(0x2E2E, 0x8B8B, 0x5757);
    }

  u_int32_t Color::Seashell() {
    return RGB(0xFFFF, 0xF5F5, 0xEEEE);
    }

  u_int32_t Color::Sienna() {
    return RGB(0xA0A0, 0x5252, 0x2D2D);
    }

  u_int32_t Color::Silver() {
    return RGB(0xC0C0, 0xC0C0, 0xC0C0);
    }

  u_int32_t Color::SkyBlue() {
    return RGB(0x8787, 0xCECE, 0xEBEB);
    }

  u_int32_t Color::SlateBlue() {
    return RGB(0x6A6A, 0x5A5A, 0xCDCD);
    }

  u_int32_t Color::SlateGray() {
    return RGB(0x7070, 0x8080, 0x9090);
    }

  u_int32_t Color::Snow() {
    return RGB(0xFFFF, 0xFAFA, 0xFAFA);
    }

  u_int32_t Color::SpringGreen() {
    return RGB(0x0000, 0xFFFF, 0x7F7F);
    }

  u_int32_t Color::SteelBlue() {
    return RGB(0x4646, 0x8282, 0xB4B4);
    }

  u_int32_t Color::Tan() {
    return RGB(0xD2D2, 0xB4B4, 0x8C8C);
    }

  u_int32_t Color::Teal() {
    return RGB(0x0000, 0x8080, 0x8080);
    }

  u_int32_t Color::Thistle() {
    return RGB(0xD8D8, 0xBFBF, 0xD8D8);
    }

  u_int32_t Color::Tomato() {
    return RGB(0xFFFF, 0x6363, 0x4747);
    }

  u_int32_t Color::Turquoise() {
    return RGB(0x4040, 0xE0E0, 0xD0D0);
    }

  u_int32_t Color::Violet() {
    return RGB(0xEEEE, 0x8282, 0xEEEE);
    }

  u_int32_t Color::Wheat() {
    return RGB(0xF5F5, 0xDEDE, 0xB3B3);
    }

  u_int32_t Color::White() {
    return RGB(0xFFFF, 0xFFFF, 0xFFFF);
    }

  u_int32_t Color::WhiteSmoke() {
    return RGB(0xF5F5, 0xF5F5, 0xF5F5);
    }

  u_int32_t Color::Yellow() {
    return RGB(0xFFFF, 0xFFFF, 0x0000);
    }

  u_int32_t Color::YellowGreen() {
    return RGB(0x9A9A, 0xCDCD, 0x3232);
    }

  }

