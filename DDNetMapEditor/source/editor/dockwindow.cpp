#include <editor/dockwindow.hpp>

#include <editor/closebutton.hpp>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>

namespace ddnet::editor {

DockWindow::DockWindow(editor::MainWindow* parent)
    : QWidget{ parent }
{
    setFixedSize(200, 200);
    move(20, 20);
    setBackgroundRole(QPalette::Window);
    setAutoFillBackground(true);
    setVisible(true);
    setMouseTracking(true);

    title_bar_widget = new QWidget{ this };
    content_widget = new QWidget{ this };

    title_bar_widget->setFixedHeight(24);
    title_bar_widget->setBackgroundRole(QPalette::Button);
    title_bar_widget->setAutoFillBackground(true);

    content_widget->setBackgroundRole(QPalette::Window);
    content_widget->setAutoFillBackground(true);

    QVBoxLayout* main_layout = new QVBoxLayout{ this };
    main_layout->setContentsMargins(0, 0, 0, 0);
    main_layout->addWidget(title_bar_widget);
    main_layout->addWidget(content_widget);

    QLabel* label = new QLabel{ title_bar_widget };
    editor::CloseButton* close_button = new editor::CloseButton{ title_bar_widget };
    close_button->move(width() - close_button->width() - 4, (title_bar_widget->height() / 2) - (close_button->height() / 2));

    QHBoxLayout* title_bar_layout = new QHBoxLayout{ title_bar_widget };
    title_bar_layout->setContentsMargins(0, 0, 0, 0);
    title_bar_layout->addWidget(label);
}



void DockWindow::snap() {
    if (y() < parentWindow()->centralWidget()->y()) {
        move(x(), parentWindow()->centralWidget()->y());
    }
}



void DockWindow::mousePressEvent(QMouseEvent* event) {
    QPoint mouse_pos = event->pos();
    if (title_bar_widget->geometry().contains(mouse_pos)) {
        dragging_cursor_offset = mouse_pos;
        dragging = true;
    }
}



void DockWindow::mouseReleaseEvent(QMouseEvent* event) {
    dragging = false;
    snap();
}



void DockWindow::mouseMoveEvent(QMouseEvent* event) {
    if (dragging) {
        move(parentWidget()->mapFromGlobal(QCursor::pos()) - dragging_cursor_offset);
        parentWidget()->update();
    }
}

} // ddnet::editor::
