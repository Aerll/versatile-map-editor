#pragma once

#include <QToolBar>

#include <vector>

namespace vt::editor {

class ToolBar final : public QToolBar {
    Q_OBJECT

public:
    ToolBar(QWidget* parent = nullptr);
    ~ToolBar() = default;

    void setVisibleFileBar(bool visible);
    void setVisibleEditBar(bool visible);
    void updateBarSeparators();

private:
    void initActions();

    bool hasVisibleBarAfter(qint32 bar_index) const;

public:
    std::vector<bool> is_visible;

    QAction* separator_file;
    QAction* separator_edit;

    QAction* action_new;
    QAction* action_open;
    QAction* action_save;
    QAction* action_import;
    QAction* action_export;
    QAction* action_undo;
    QAction* action_redo;
    QAction* action_delete;
};

}
