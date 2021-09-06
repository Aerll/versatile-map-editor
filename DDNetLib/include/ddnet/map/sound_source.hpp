#pragma once

#include <QPoint>
#include <QSize>

namespace ddnet::map {

struct SoundSource {
    QPointF position;

    qint32 delay_seconds;
    qint32 falloff; // from 0 to 255

    qint32 position_envelope_id;
    qint32 position_envelope_offset;

    qint32 sound_envelope_id;
    qint32 sound_envelope_offset;

    bool is_looped;
    bool has_panning;
};

struct SoundSourceCircle : SoundSource {
    qint32 radius;
};

struct SoundSourceRectangle : SoundSource {
    QSize size;
};

} // ddnet::map::
