#include <editor/tabwidget.hpp>

#include <editor/app.hpp>
#include <editor/openglwidget.hpp>
#include <editor/tabbar.hpp>
#include <editor/tablabel.hpp>
#include <editor/closebutton.hpp>

#include <QPaintEvent>
#include <QPainter>

namespace ddnet::editor {

TabWidget::TabWidget(QWidget* parent)
    : QTabWidget{ parent }
{
    setDocumentMode(true);
    setAcceptDrops(true);
    setMovable(true);

    // initialize border's style
    auto& style_attributes = App::resources.widgetStyles(WidgetStyleType::DDNet, "TabWidget::pane");
    if (!style_attributes.empty()) {
        QStringList style_border_top = style_attributes["border-top"].split(' ');

        for (const auto& style : style_border_top) {
            bool result; // height
            qint32 style_border_size = style.toInt(&result);
            if (result) {
                underline_height = style_border_size;
                continue;
            }

            QColor style_border_color; // color
            style_border_color.setNamedColor(style);
            if (style_border_color.isValid()) {
                underline_color = style_border_color;
                continue;
            }
        }
    }

    editor::TabBar* tab_bar = new editor::TabBar{ this };
    setTabBar(tab_bar);

    addTab(new editor::OpenGLWidget, "");
    addTab(new editor::OpenGLWidget, "");
    addTab(new editor::OpenGLWidget, "");

    tabBar()->setTabButton(0, QTabBar::LeftSide, new editor::TabLabel{ "iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii" });
    tabBar()->setTabButton(1, QTabBar::LeftSide, new editor::TabLabel{ "Base-10" });
    tabBar()->setTabButton(2, QTabBar::LeftSide, new editor::TabLabel{ "Behemoth" });

    tabBar()->setCloseButton(0);
    tabBar()->setCloseButton(1);
    tabBar()->setCloseButton(2);
}



editor::TabBar* TabWidget::tabBar() {
    return static_cast<editor::TabBar*>(QTabWidget::tabBar());
}



void TabWidget::paintEvent(QPaintEvent* event) {
    QTabWidget::paintEvent(event);

    // recolor top border
    QPainter painter{ this };
    painter.setPen(Qt::NoPen);
    painter.setBrush(underline_color);
    painter.drawRect(event->rect().x(), event->rect().y() + tabBar()->height(), event->rect().width(), underline_height);
}

}
