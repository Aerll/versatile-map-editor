#pragma once

#include <QtGlobal>
#include <QSize>
#include <QPoint>

class QImage;
class QColor;

namespace vt::util {

void recolor(QImage& image, const QColor& color);
QImage recolor(const QImage& image, const QColor& color);

}
