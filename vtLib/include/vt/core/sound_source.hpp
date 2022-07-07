#pragma once

#include <QtGlobal>
#include <QPoint>
#include <QSize>

namespace vt::core {

struct SoundSource {
    QPointF position;

    qint32 position_envelope_id;
    qint32 position_envelope_offset;

    qint32 sound_envelope_id;
    qint32 sound_envelope_offset;

    qint32 delay_seconds;
    quint8 falloff;

    bool is_looped;
    bool has_panning;
};



struct SoundSourceCircle : SoundSource {
    qint32 radius;
};



struct SoundSourceRectangle : SoundSource {
    QSize size;
};

}
