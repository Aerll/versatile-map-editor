#include <editor/sidebar.hpp>

#include <QLabel>
#include <QVBoxLayout>

namespace ddnet::editor {

Sidebar::Sidebar(QWidget* parent)
    : QWidget{ parent }
{
    setBackgroundRole(QPalette::Window);
    setAutoFillBackground(true);

    widget = new QWidget{ this };

    QVBoxLayout* layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(widget);

    setLayout(layout);
}

}
