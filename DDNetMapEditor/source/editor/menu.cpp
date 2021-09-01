#include <editor/menu.hpp>

#include <editor/app.hpp>

#include <QMenuBar>
#include <QPaintEvent>
#include <QPainter>

namespace ddnet::editor {

Menu::Menu(QWidget* parent)
    : QMenu{ parent }
{
    if (dynamic_cast<QMenuBar*>(parent) != nullptr) { // remove border only from the top level menu
        auto& style_attributes = App::resources.widgetStyles(WidgetStyleType::Qt, "QMenu");
        if (!style_attributes.empty()) {
            QStringList style_border = style_attributes["border"].split(' ');

            for (const auto& style : style_border) {
                bool result; // size
                qint32 style_border_size = style.toInt(&result);
                if (result) {
                    border_size = style_border_size;
                    break;
                }
            }
        }
    }
}



void Menu::paintEvent(QPaintEvent* event) {
    QMenu::paintEvent(event);

    QPainter painter{ menuAction()->menu() };
    painter.setPen(Qt::NoPen);

    // remove a part of the top menu border
    if (border_size > 0) {
        qint32 menu_bar_item_width = static_cast<QMenuBar*>(parentWidget())->actionGeometry(menuAction()).width();

        painter.setBrush(palette().color(QPalette::Dark));
        painter.drawRect(border_size, 0, menu_bar_item_width - 1 - border_size, border_size);
    }

    auto action_list = actions();
    for (auto* action : action_list) {
        QRect action_rect = actionGeometry(action);

        if (action->isChecked()) { // draw check indicator for checkable items
            const QImage& check_image = App::resources.asset(AssetIndex::Ui16_Normal_CheckMenu);
            QRect check_rect;
            check_rect.setX(5);
            check_rect.setY(action_rect.y() + (action_rect.height() / 2) - (check_image.height() / 2));
            check_rect.setWidth(check_image.width());
            check_rect.setHeight(check_image.height());

            painter.drawImage(check_rect, check_image);
        }
        else if (action->menu() != nullptr) { // draw right arrow for sub menus
            const QImage& arrow_image = App::resources.asset(AssetIndex::Ui16_Normal_ArrowMenu);
            QRect arrow_rect;
            arrow_rect.setX(action_rect.width() - arrow_image.width());
            arrow_rect.setY(action_rect.y() + (action_rect.height() / 2) - (arrow_image.height() / 2));
            arrow_rect.setWidth(arrow_image.width());
            arrow_rect.setHeight(arrow_image.height());

            painter.drawImage(arrow_rect, arrow_image);
        }
    }
}

} // ddnet::editor::
