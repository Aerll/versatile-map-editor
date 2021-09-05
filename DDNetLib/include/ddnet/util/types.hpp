#pragma once

#include <QtGlobal>

namespace ddnet::util {

template <typename _Type>
struct Point {
    inline bool operator==(const Point&) const = default;

    _Type x;
    _Type y;
};

template <typename _Type>
struct Size {
    inline bool operator==(const Size&) const = default;

    _Type width;
    _Type height;
};

struct Color {
    qint32 r;
    qint32 g;
    qint32 b;
    qint32 a;
};



// aliases
using RectangleSize = Size<qint32>;
using CircleRadius = qint32;

} // ddnet::util::
