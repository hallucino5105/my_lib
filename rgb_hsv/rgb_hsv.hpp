#pragma once


// reference of
// http://d.hatena.ne.jp/tt_clown/20111208/boost_gil_hsvfilter


#include <cmath>


namespace hoshi
{


typedef struct _RGB {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} RGB;


typedef struct _HSV {
    int h;
    int s;
    int v;
} HSV;


static HSV rgb2hsv(const RGB& rgb)
{
    const int r = std::min(rgb.r, static_cast<unsigned char>(-1));
    const int g = std::min(rgb.g, static_cast<unsigned char>(-1));
    const int b = std::min(rgb.b, static_cast<unsigned char>(-1));

    const int maximum = std::max(std::max(r, g), b);
    const int minimum = std::min(std::min(r, g), b);

    const double v = maximum;
    const double s = (maximum != 0) ? 255.0 * (maximum - minimum) / static_cast<double>(maximum) : 0.0;

    double h = 0.0;
    if(s > 0.0 && maximum != minimum) {
        if(maximum == r) {
            h = 60.0 * (g - b) / static_cast<double>(maximum - minimum);
        } else if(maximum == g) {
            h = 60.0 * (b - r) / static_cast<double>(maximum - minimum) + 120.0;
        } else {
            h = 60.0 * (r - g) / static_cast<double>(maximum - minimum) + 240.0;
        }

        if(h < 0.0) {
            h += 360.0;
        }
    }

    HSV hsv;

    hsv.h = static_cast<int>(h);
    hsv.s = static_cast<int>(s);
    hsv.v = static_cast<int>(v);

    return hsv;
}


static RGB hsv2rgb(const HSV& hsv)
{
    RGB rgb;

    rgb.r = rgb.g = rgb.b = static_cast<unsigned char>(-1);

    const int h = hsv.h;
    const int s = hsv.s;
    const int v = hsv.v;

    const int hi = h / 60;
    const double f = h / 60.0 - hi;
    const double m = (1.0 - s / 255.0) * v;
    const double n = (1.0 - s / 255.0 * f) * v;
    const double k = (1.0 - s / 255.0 * (1.0 - f)) * v;

    switch (hi) {
        case 0: { rgb.r = v; rgb.g = k; rgb.b = m; return rgb; }
        case 1: { rgb.r = n; rgb.g = v; rgb.b = m; return rgb; }
        case 2: { rgb.r = m; rgb.g = v; rgb.b = k; return rgb; }
        case 3: { rgb.r = m; rgb.g = n; rgb.b = v; return rgb; }
        case 4: { rgb.r = k; rgb.g = m; rgb.b = v; return rgb; }
        case 5: { rgb.r = v; rgb.g = m; rgb.b = n; return rgb; }
        default: break;
    }

    return rgb;
}


};

