#pragma once

#include <QMainWindow>

class QAction;

namespace vt::editor {

class Menu;
class ToolBar;
class TabWidget;

class MainWindow final : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow() = default;

private:
    void initActions();
    void initMenuFile();
    void initMenuEdit();
    void initMenuView();
    void initMenuSettings();
    void initMenuHelp();

    void connectSignals();

public slots:
    void HandleMessage_(quint32 instance_id, QByteArray message);
    void New_();
    void Open_();
    void Save_();
    void SaveAs_();
    void SaveAll_();
    void Import_();
    void Export_();
    void ExportAs_();
    void Exit_();
    void Undo_();
    void Redo_();
    void Cut_();
    void Copy_();
    void Paste_();
    void Delete_();
    void SelectAll_();
    void ToggleShowToolBar_();
    void ToggleShowFileBar_();
    void ToggleShowEditBar_();
    void Preferences_();
    void About_();

private:
    editor::Menu* menu_file;
    QAction* action_new;
    QAction* action_open;
    QAction* action_save;
    QAction* action_save_as;
    QAction* action_save_all;
    QAction* action_import;
    QAction* action_export;
    QAction* action_export_as;
    QAction* action_exit;

    editor::Menu* menu_edit;
    QAction* action_undo;
    QAction* action_redo;
    QAction* action_cut;
    QAction* action_copy;
    QAction* action_paste;
    QAction* action_delete;
    QAction* action_select_all;

    editor::Menu* menu_view;
    editor::Menu* submenu_toolbar;
    QAction* action_show_tool_bar;
    QAction* action_show_file_bar;
    QAction* action_show_edit_bar;

    editor::Menu* menu_settings;
    QAction* action_preferences;

    editor::Menu* menu_help;
    QAction* action_about;

    editor::ToolBar* tool_bar;
    editor::TabWidget* tab_widget;
};

}
