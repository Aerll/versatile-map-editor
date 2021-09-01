#include <editor/resizer.hpp>

#include <QMouseEvent>

namespace ddnet::editor {

Resizer::Resizer(QWidget* parent)
    : QWidget{ parent }
{
    setFixedSize(200, 200);
    setBackgroundRole(QPalette::Window);
    setAutoFillBackground(true);
    setVisible(true);
    setMouseTracking(true);

    widget = new QWidget{ this };

    widget->setFixedHeight(24);
    widget->setBackgroundRole(QPalette::Button);
    widget->setAutoFillBackground(true);
}



void Resizer::mousePressEvent(QMouseEvent* event) {
    QPoint mouse_pos = event->pos();
    if (widget->geometry().contains(mouse_pos)) {
        dragging_cursor_offset = mouse_pos;
        dragging = true;
    }
}



void Resizer::mouseReleaseEvent(QMouseEvent* event) {
    dragging = false;
}



void Resizer::mouseMoveEvent(QMouseEvent* event) {
    if (dragging) {
        move(parentWidget()->mapFromGlobal(QCursor::pos()) - dragging_cursor_offset);
        parentWidget()->update();
    }
}

} // ddnet::editor::
