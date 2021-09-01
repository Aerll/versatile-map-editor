#pragma once

#include <editor/constants.hpp>

#include <QPushButton>

namespace ddnet::editor {

class CloseButton final : public QPushButton {
    Q_OBJECT

public:
    CloseButton(QWidget* parent = nullptr);
    ~CloseButton() = default;

private:
    void enterEvent(QEvent* event) final;
    void leaveEvent(QEvent* event) final;

    void mousePressEvent(QMouseEvent* event) final;
    void mouseReleaseEvent(QMouseEvent* event) final;

    void paintEvent(QPaintEvent* event) final;

public:
    ButtonState state = ButtonState::Normal;
};

} // ddnet::editor::
