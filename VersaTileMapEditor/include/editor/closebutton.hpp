#pragma once

#include <editor/constants.hpp>

#include <QPushButton>

namespace vt::editor {

class CloseButton final : public QPushButton {
    Q_OBJECT

public:
    CloseButton(QWidget* parent = nullptr);
    ~CloseButton() = default;

private:
    virtual void enterEvent(QEnterEvent* event) final;
    virtual void leaveEvent(QEvent* event) final;

    virtual void mousePressEvent(QMouseEvent* event) final;
    virtual void mouseReleaseEvent(QMouseEvent* event) final;

    virtual void paintEvent(QPaintEvent* event) final;

public:
    ButtonState state = ButtonState::Normal;
};

}
