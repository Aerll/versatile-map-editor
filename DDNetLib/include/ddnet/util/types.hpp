#pragma once

#include <QtGlobal>

namespace ddnet::util {

struct Point {
    qint32 x;
    qint32 y;
};

struct Size {
    qint32 width;
    qint32 height;
};

struct Color {
    qint32 r;
    qint32 g;
    qint32 b;
    qint32 a;
};



// aliases
using RectangleSize = Size;
using CircleRadius = qint32;

} // ddnet::util::
