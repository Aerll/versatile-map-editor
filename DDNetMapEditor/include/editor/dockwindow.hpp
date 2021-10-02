#pragma once

#include <editor/mainwindow.hpp>

#include <QWidget>

namespace ddnet::editor {

class DockWindow : public QWidget {
    Q_OBJECT

public:
    DockWindow(editor::MainWindow* parent = nullptr);
    ~DockWindow() = default;

    void snap();

protected:
    inline editor::MainWindow* parentWindow() const { return static_cast<editor::MainWindow*>(parentWidget()); }

protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;

protected:
    QWidget* title_bar_widget;
    QWidget* content_widget;

    QPoint dragging_cursor_offset;
    bool dragging = false;
};

}
