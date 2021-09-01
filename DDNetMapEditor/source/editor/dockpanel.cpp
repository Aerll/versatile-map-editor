#include <editor/dockpanel.hpp>

#include <QLabel>
#include <QVBoxLayout>

namespace ddnet::editor {

DockPanel::DockPanel(QWidget* parent)
    : QWidget{ parent }
{
    setMinimumWidth(200);
}

} // ddnet::editor::
