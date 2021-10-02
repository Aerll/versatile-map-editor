#pragma once

#include <QtGlobal>
#include <QSize>
#include <QPoint>

class QImage;
class QColor;

namespace ddnet::util {

void recolor(QImage& image, const QColor& color);
QImage recolor(const QImage& image, const QColor& color);

QImage crop(const QImage& image, const QSize& size, const QPoint& spacing, const QPoint& coord);

}
