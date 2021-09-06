#pragma once

#include <QColor>
#include <QPoint>

#include <array>

namespace ddnet::map {

struct QuadVertex {
    QPointF position;
    QPoint tex_coord;
    QColor color;
};

struct Quad {
    std::array<QuadVertex, 4> vertices;
    QPointF pivot_position;

    qint32 position_envelope_id;
    qint32 position_envelope_offset;

    qint32 color_envelope_id;
    qint32 color_envelope_offset;
};

} // ddnet::map::
