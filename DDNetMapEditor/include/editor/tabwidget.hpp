#pragma once

#include <editor/tabbar.hpp>

#include <QTabWidget>

namespace ddnet::editor {

class TabWidget final : public QTabWidget {
    Q_OBJECT

public:
    TabWidget(QWidget* parent = nullptr);
    ~TabWidget() = default;

public:
    editor::TabBar* tabBar();

private:
    void paintEvent(QPaintEvent* event) final;
    
private:
    qint32 underline_height = 0;
    QColor underline_color;
};

} // ddnet::editor::
