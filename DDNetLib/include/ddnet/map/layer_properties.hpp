#pragma once

#include <QtGlobal>
#include <QColor>
#include <QFileInfo>

namespace ddnet::map {

struct LayerProperties {
    quint16 group_id;
    quint16 id;
    QFileInfo asset_path;
    bool is_detail;
};

struct TileLayerProperties : LayerProperties {
    QColor color;
    quint16 color_envelope_id;
    quint32 color_envelope_offset;
};

struct QuadLayerProperties : LayerProperties {};
struct SoundLayerProperties : LayerProperties {};

} // ddnet::map::
