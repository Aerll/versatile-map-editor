#pragma once

#include <QWidget>

namespace vt::editor {

class DockPanel : public QWidget {
    Q_OBJECT

public:
    DockPanel(QWidget* parent = nullptr);
    ~DockPanel() = default;

public:

};

}
