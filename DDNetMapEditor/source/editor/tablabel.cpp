#include <editor/tablabel.hpp>

#include <editor/app.hpp>

#include <QPushButton>
#include <QHBoxLayout>
#include <QFontMetrics>
#include <QVariant>

namespace ddnet::editor {

TabLabel::TabLabel(const QString& string, QWidget* parent)
    : QLabel{ parent }
{
    auto& style_attributes = App::resources.widgetStyles(WidgetStyleType::DDNet, "TabBar::tab");
    qint32 tab_width = style_attributes["width"].toInt();

    QFontMetricsF font_metrics{ font() };
    QString elided_string = font_metrics.elidedText(string, Qt::TextElideMode::ElideRight, tab_width - 24.0 - font_metrics.width("..."));

    setText(elided_string);
    setAlignment(Qt::AlignLeft);
    setMaximumWidth(tab_width - 24);
    setMargin(5);
}

} // ddnet::editor::