#include <editor/closebutton.hpp>

#include <editor/app.hpp>
#include <editor/tabbar.hpp>
#include <vt/util/graphics.hpp>

#include <QPainter>
#include <QPaintEvent>

namespace vt::editor {

CloseButton::CloseButton(QWidget* parent)
    : QPushButton{ parent }
{
    const QImage& x_button = App::resources.asset(AssetIndex::Ui16_Normal_ButtonClose);
    setFixedSize(x_button.size());

    QPalette new_palette;
    new_palette.setBrush(QPalette::Button, x_button);
    setPalette(new_palette);

    setAutoFillBackground(true);
    setFlat(true);
}



void CloseButton::enterEvent(QEnterEvent* event) {
    state |= ButtonState::Hover;
    QPushButton::enterEvent(event);
}



void CloseButton::leaveEvent(QEvent* event) {
    state ^= ButtonState::Hover;
    QPushButton::leaveEvent(event);
}



void CloseButton::mousePressEvent(QMouseEvent* event) {
    state |= ButtonState::Pressed;
    QPushButton::mousePressEvent(event);
}



void CloseButton::mouseReleaseEvent(QMouseEvent* event) {
    state ^= ButtonState::Pressed;
    QPushButton::mouseReleaseEvent(event);
}



void CloseButton::paintEvent(QPaintEvent* event) {
    QPushButton::paintEvent(event);

    bool is_hover   = static_cast<bool>(state & ButtonState::Hover);
    bool is_pressed = static_cast<bool>(state & ButtonState::Pressed);

    QPainter painter{ this };
    if (state == ButtonState::Normal)
        painter.drawImage(rect(), App::resources.asset(AssetIndex::Ui16_Normal_ButtonClose));
    else if (is_hover && is_pressed)
        painter.drawImage(rect(), App::resources.asset(AssetIndex::Ui16_Pressed_ButtonClose));
    else if (is_hover)
        painter.drawImage(rect(), App::resources.asset(AssetIndex::Ui16_Hover_ButtonClose));
    else if (is_pressed)
        painter.drawImage(rect(), App::resources.asset(AssetIndex::Ui16_Pressed_ButtonClose));
}

}
