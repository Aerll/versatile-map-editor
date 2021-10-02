#pragma once

#include <ddnet/util/constants.hpp>
#include <ddnet/util/enums.hpp>

#include <QtGlobal>
#include <QString>
#include <QColor>
#include <QPoint>

#include <vector>

namespace ddnet {

struct EnvelopePoint {
    qreal time_point;
    enums::CurveType curve_type = enums::CurveType::Linear;
};



struct ColorEnvelopePoint : EnvelopePoint {
    QColor color;
};



struct PositionEnvelopePoint : EnvelopePoint {
    QPointF point;
    qreal rotation;
};



struct SoundEnvelopePoint : EnvelopePoint {
    qreal volume;
};



class Envelope {
public:
    QString name;
    bool is_synchronized = false;
};



class ColorEnvelope : public Envelope {
public:
    std::vector<ColorEnvelopePoint> points;
};



class PositionEnvelope : public Envelope {
public:
    std::vector<PositionEnvelopePoint> points;
};



class SoundEnvelope : public Envelope {
public:
    std::vector<SoundEnvelopePoint> points;
};

}
