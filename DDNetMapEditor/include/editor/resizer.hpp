#pragma once

#include <QWidget>

namespace ddnet::editor {

class Resizer : public QWidget {
    Q_OBJECT

public:
    Resizer(QWidget* parent = nullptr);
    ~Resizer() = default;

protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;

protected:
    QWidget* widget;

    QPoint dragging_cursor_offset;
    bool dragging = false;
};

} // ddnet::editor::
