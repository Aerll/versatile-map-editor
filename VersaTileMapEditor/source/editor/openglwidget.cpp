#include <editor/openglwidget.hpp>

#include <editor/app.hpp>
#include <vt/util/graphics.hpp>

#include <QPainter>
#include <QPaintEvent>

namespace vt::editor {

OpenGLWidget::OpenGLWidget(QWidget* parent)
    : QOpenGLWidget{ parent }
{
}



void OpenGLWidget::paintEvent(QPaintEvent* event) {
    QPainter painter{ this };
    painter.fillRect(event->rect(), App::resources.asset(AssetIndex::Ui_Texture_Checker));
}

}
