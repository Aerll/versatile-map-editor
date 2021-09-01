#include <editor/lpanel.hpp>

#include <editor/resizer.hpp>

#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

namespace ddnet::editor {

LPanel::LPanel(QWidget* parent)
    : QWidget{ parent }
{
    setMaximumWidth(512);
    setMinimumWidth(256);

    editor::Resizer* resizer = new editor::Resizer{ this };

    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(resizer, 0, Qt::AlignVCenter);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    setLayout(layout);
}

} // ddnet::editor::
