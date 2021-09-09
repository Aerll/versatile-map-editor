#pragma once

#include <QtGlobal>
#include <QString>
#include <QColor>

#include <vector>

namespace ddnet::map {

enum class CurveType {
    Step = 0,
    Linear,
    Slow,
    Fast,
    Smooth,
};



struct EnvelopePoint {
    qreal time_point;
    CurveType curve_type = CurveType::Linear;
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

} // ddnet::map::
