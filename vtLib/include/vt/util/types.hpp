#pragma once

#include <QtGlobal>

namespace vt::util {

struct Point {
    inline bool operator==(const Point&) const = default;

    qint32 x;
    qint32 y;
};



struct Size {
    inline bool operator==(const Size&) const = default;

    qint32 width;
    qint32 height;
};



struct Color {
    inline bool operator==(const Color&) const = default;

    qint32 r;
    qint32 g;
    qint32 b;
    qint32 a;
};



using RectangleSize = Size;
using CircleRadius = qint32;

}
