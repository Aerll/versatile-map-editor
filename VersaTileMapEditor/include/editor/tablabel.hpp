#pragma once

#include <QLabel>

namespace vt::editor {

class TabLabel final : public QLabel {
    Q_OBJECT

public:
    TabLabel(const QString& string, QWidget* parent = nullptr);
    ~TabLabel() = default;
};

}
