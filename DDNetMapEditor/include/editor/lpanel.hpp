#pragma once

#include <QWidget>

namespace ddnet::editor {

class LPanel : public QWidget {
    Q_OBJECT

public:
    LPanel(QWidget* parent = nullptr);
    ~LPanel() = default;
};

} // ddnet::editor::
