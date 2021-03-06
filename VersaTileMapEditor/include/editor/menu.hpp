#pragma once

#include <QMenu>

namespace vt::editor {

class Menu final : public QMenu {
    Q_OBJECT

public:
    Menu(QWidget* parent = nullptr);
    ~Menu() = default;

private:
    void paintEvent(QPaintEvent* event) final;
    
private:
    qint32 border_size = 0;
};

}
