#include <vt/util/graphics.hpp>

#include <QImage>

namespace vt::util {

void recolor(QImage& image, const QColor& color) {
    if (color == Qt::white)
        return;

    for (qint32 y = 0; y < image.height(); ++y) {
        for (qint32 x = 0; x < image.width(); ++x) {
            QColor pixel_color = image.pixelColor(x, y);
            pixel_color.setRedF(pixel_color.redF() * color.redF());
            pixel_color.setGreenF(pixel_color.greenF() * color.greenF());
            pixel_color.setBlueF(pixel_color.blueF() * color.blueF());
            pixel_color.setAlphaF(pixel_color.alphaF() * color.alphaF());
            image.setPixelColor(x, y, pixel_color);
        }
    }
}



QImage recolor(const QImage& image, const QColor& color) {
    if (color == Qt::white)
        return image;

    QImage new_image = image;
    util::recolor(new_image, color);
    return new_image;
}



QImage crop(const QImage& image, const QSize& size, const QPoint& spacing, const QPoint& coord) {
    QRect rect = {
        coord.y() * size.width() + (coord.y() + 1) * spacing.x(),
        coord.x() * size.height() + (coord.x() + 1) * spacing.y(),
        size.width(),
        size.height()
    };
    return image.copy(rect);
}

}
