#pragma once

#include <QWidget>

namespace vt::editor {

class Sidebar : public QWidget {
    Q_OBJECT

public:
    Sidebar(QWidget* parent = nullptr);
    ~Sidebar() = default;

protected:
    QWidget* widget;
};

}
