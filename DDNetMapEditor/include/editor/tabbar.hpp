#pragma once

#include <QTabBar>

namespace ddnet::editor {

class CloseButton;

class TabBar final : public QTabBar {
    Q_OBJECT

public:
    TabBar(QWidget* parent = nullptr);
    ~TabBar();

    void updateCloseButtons(QPoint mouse_pos);

    void setCloseButton(qint32 index);

public slots:
    void closeTab();

private:
    bool eventFilter(QObject* object, QEvent* event) final;
};

} // ddnet::editor::
