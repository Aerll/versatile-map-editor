#include <editor/dockpanel.hpp>

#include <QLabel>
#include <QVBoxLayout>

namespace vt::editor {

DockPanel::DockPanel(QWidget* parent)
    : QWidget{ parent }
{
    setMinimumWidth(200);
}

}
