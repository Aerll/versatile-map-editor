#pragma once

#include <QLabel>

namespace ddnet::editor {

class TabLabel final : public QLabel {
    Q_OBJECT

public:
    TabLabel(const QString& string, QWidget* parent = nullptr);
    ~TabLabel() = default;
};

} // ddnet::editor::
