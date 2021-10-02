#include <editor/mainwindow.hpp>

#include <editor/app.hpp>
#include <editor/tabwidget.hpp>
#include <editor/toolbar.hpp>
#include <editor/tablabel.hpp>
#include <editor/closebutton.hpp>
#include <editor/menu.hpp>
#include <editor/sidebar.hpp>

#include <QPushButton>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QSplitter>
#include <QVBoxLayout>

#include <windows.h>

namespace ddnet::editor {

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow{ parent }
{
    // changes the default fill color for an empty region, displayed when the window is resized
    SetClassLongPtrW(HWND(window()->winId()), GCLP_HBRBACKGROUND, LONG_PTR(GetStockObject(NULL_BRUSH)));

    setCentralWidget(new QWidget);

    tool_bar = new editor::ToolBar{ this };
    tab_widget = new editor::TabWidget{ this };

    addToolBar(tool_bar);

    QSplitter* splitter = new QSplitter{ this };

    editor::Sidebar* left_sidebar = new editor::Sidebar{ this };
    editor::Sidebar* right_sidebar = new editor::Sidebar{ this };

    splitter->addWidget(left_sidebar);
    splitter->addWidget(tab_widget);
    splitter->addWidget(right_sidebar);

    QHBoxLayout* main_layout = new QHBoxLayout;
    main_layout->setSpacing(0);
    main_layout->setContentsMargins(0, 0, 0, 0);
    main_layout->addWidget(splitter);

    centralWidget()->setLayout(main_layout);

    // initialize actions
    initActions();

    // initialize menus
    initMenuFile();
    initMenuEdit();
    initMenuView();
    initMenuSettings();
    initMenuHelp();

    // connect signals
    connectSignals();

    showMaximized();
}



void MainWindow::initActions() {
    // file menu
    action_new = new QAction{ App::resources.string(StringIndex::MenuFileNew), this };
    action_new->setShortcut(QKeySequence{ Qt::CTRL | Qt::Key_N });
    action_new->setIcon(QPixmap::fromImage(App::resources.asset(AssetIndex::Icon16_Normal_NewFile)));

    action_open = new QAction{ App::resources.string(StringIndex::MenuFileOpen), this };
    action_open->setShortcut(QKeySequence{ Qt::CTRL | Qt::Key_O });
    action_open->setIcon(QPixmap::fromImage(App::resources.asset(AssetIndex::Icon16_Normal_OpenFile)));
          
    action_save = new QAction{ App::resources.string(StringIndex::MenuFileSave), this };
    action_save->setShortcut(QKeySequence{ Qt::CTRL | Qt::Key_S });
    action_save->setIcon(QPixmap::fromImage(App::resources.asset(AssetIndex::Icon16_Normal_SaveFile)));
          
    action_save_as = new QAction{ App::resources.string(StringIndex::MenuFileSaveAs), this };
          
    action_save_all = new QAction{ App::resources.string(StringIndex::MenuFileSaveAll), this };
    action_save_all->setShortcut(QKeySequence{ Qt::CTRL | Qt::SHIFT | Qt::Key_S });
          
    action_import = new QAction{ App::resources.string(StringIndex::MenuFileImport), this };
    action_import->setShortcut(QKeySequence{ Qt::CTRL | Qt::SHIFT | Qt::Key_I });
    action_import->setIcon(QPixmap::fromImage(App::resources.asset(AssetIndex::Icon16_Normal_Import)));
          
    action_export = new QAction{ App::resources.string(StringIndex::MenuFileExport), this };
    action_export->setShortcut(QKeySequence{ Qt::CTRL | Qt::Key_E });
    action_export->setIcon(QPixmap::fromImage(App::resources.asset(AssetIndex::Icon16_Normal_Export)));
          
    action_export_as = new QAction{ App::resources.string(StringIndex::MenuFileExportAs), this };
    action_export_as->setShortcut(QKeySequence{ Qt::CTRL | Qt::SHIFT | Qt::Key_E });
          
    action_exit = new QAction{ App::resources.string(StringIndex::MenuFileExit), this };
    action_exit->setShortcut(QKeySequence{ Qt::ALT | Qt::Key_F4 });

    // edit menu
    action_undo = new QAction{ App::resources.string(StringIndex::MenuEditUndo), this };
    action_undo->setShortcut(QKeySequence{ Qt::CTRL | Qt::Key_Z });
    action_undo->setIcon(QPixmap::fromImage(App::resources.asset(AssetIndex::Icon16_Normal_Undo)));
          
    action_redo = new QAction{ App::resources.string(StringIndex::MenuEditRedo), this };
    action_redo->setShortcut(QKeySequence{ Qt::CTRL | Qt::Key_Y });
    action_redo->setIcon(QPixmap::fromImage(App::resources.asset(AssetIndex::Icon16_Normal_Redo)));
          
    action_cut = new QAction{ App::resources.string(StringIndex::MenuEditCut), this };
    action_cut->setShortcut(QKeySequence{ Qt::CTRL | Qt::Key_X });
          
    action_copy = new QAction{ App::resources.string(StringIndex::MenuEditCopy), this };
    action_copy->setShortcut(QKeySequence{ Qt::CTRL | Qt::Key_C });
          
    action_paste = new QAction{ App::resources.string(StringIndex::MenuEditPaste), this };
    action_paste->setShortcut(QKeySequence{ Qt::CTRL | Qt::Key_V });
          
    action_delete = new QAction{ App::resources.string(StringIndex::MenuEditDelete), this };
    action_delete->setShortcut(Qt::Key_Delete);
    action_delete->setIcon(QPixmap::fromImage(App::resources.asset(AssetIndex::Icon16_Normal_Delete)));
    
    action_select_all = new QAction{ App::resources.string(StringIndex::MenuEditSelectAll), this };
    action_select_all->setShortcut(QKeySequence{ Qt::CTRL | Qt::Key_A });

    // view menu
    action_show_tool_bar = new QAction{ App::resources.string(StringIndex::SubMenuViewToolBar), this };
    action_show_tool_bar->setCheckable(true);
    action_show_tool_bar->setChecked(true);

    action_show_file_bar = new QAction{ App::resources.string(StringIndex::SubMenuViewFileBar), this };
    action_show_file_bar->setCheckable(true);
    action_show_file_bar->setChecked(true);

    action_show_edit_bar = new QAction{ App::resources.string(StringIndex::SubMenuViewEditBar), this };
    action_show_edit_bar->setCheckable(true);
    action_show_edit_bar->setChecked(true);

    // settings menu
    action_preferences = new QAction{ App::resources.string(StringIndex::MenuSettingsPreferences), this };

    // help menu
    action_about = new QAction{ App::resources.string(StringIndex::MenuHelpAbout), this };
    action_about->setShortcut(Qt::Key_F1);
}



void MainWindow::initMenuFile() {
    menu_file = new editor::Menu{ menuBar() };
    menu_file->setTitle(App::resources.string(StringIndex::MenuFile));
    menuBar()->addMenu(menu_file);

    menu_file->addAction(action_new);
    menu_file->addAction(action_open);
    menu_file->addSeparator();
    menu_file->addAction(action_save);
    menu_file->addAction(action_save_as);
    menu_file->addAction(action_save_all);
    menu_file->addSeparator();
    menu_file->addAction(action_import);
    menu_file->addAction(action_export);
    menu_file->addAction(action_export_as);
    menu_file->addSeparator();
    menu_file->addAction(action_exit);
}



void MainWindow::initMenuEdit() {
    menu_edit = new editor::Menu{ menuBar() };
    menu_edit->setTitle(App::resources.string(StringIndex::MenuEdit));
    menuBar()->addMenu(menu_edit);

    menu_edit->addAction(action_undo);
    menu_edit->addAction(action_redo);
    menu_edit->addSeparator();
    menu_edit->addAction(action_cut);
    menu_edit->addAction(action_copy);
    menu_edit->addAction(action_paste);
    menu_edit->addAction(action_delete);
    menu_edit->addSeparator();
    menu_edit->addAction(action_select_all);
}



void MainWindow::initMenuView() {
    menu_view = new editor::Menu{ menuBar() };
    menu_view->setTitle(App::resources.string(StringIndex::MenuView));
    menuBar()->addMenu(menu_view);

    submenu_toolbar = new editor::Menu{ menu_view };
    submenu_toolbar->setTitle(App::resources.string(StringIndex::SubMenuToolBar));
    menu_view->addMenu(submenu_toolbar);

    submenu_toolbar->addAction(action_show_tool_bar);
    submenu_toolbar->addSeparator();
    submenu_toolbar->addAction(action_show_file_bar);
    submenu_toolbar->addAction(action_show_edit_bar);
}



void MainWindow::initMenuSettings() {
    menu_settings = new editor::Menu{ menuBar() };
    menu_settings->setTitle(App::resources.string(StringIndex::MenuSettings));
    menuBar()->addMenu(menu_settings);

    menu_settings->addAction(action_preferences);
}



void MainWindow::initMenuHelp() {
    menu_help = new editor::Menu{ menuBar() };
    menu_help->setTitle(App::resources.string(StringIndex::MenuHelp));
    menuBar()->addMenu(menu_help);

    menu_help->addAction(action_about);
}



void MainWindow::connectSignals() {
    // menu bar
    connect(action_new,       &QAction::triggered, this, &MainWindow::New_     );
    connect(action_open,      &QAction::triggered, this, &MainWindow::Open_    );
    connect(action_save,      &QAction::triggered, this, &MainWindow::Save_    );
    connect(action_save_as,   &QAction::triggered, this, &MainWindow::SaveAs_  );
    connect(action_save_all,  &QAction::triggered, this, &MainWindow::SaveAll_ );
    connect(action_import,    &QAction::triggered, this, &MainWindow::Import_  );
    connect(action_export,    &QAction::triggered, this, &MainWindow::Export_  );
    connect(action_export_as, &QAction::triggered, this, &MainWindow::ExportAs_);
    connect(action_exit,      &QAction::triggered, this, &MainWindow::Exit_    );
                  
    connect(action_undo,       &QAction::triggered, this, &MainWindow::Undo_     );
    connect(action_redo,       &QAction::triggered, this, &MainWindow::Redo_     );
    connect(action_cut,        &QAction::triggered, this, &MainWindow::Cut_      );
    connect(action_copy,       &QAction::triggered, this, &MainWindow::Copy_     );
    connect(action_paste,      &QAction::triggered, this, &MainWindow::Paste_    );
    connect(action_delete,     &QAction::triggered, this, &MainWindow::Delete_   );
    connect(action_select_all, &QAction::triggered, this, &MainWindow::SelectAll_);

    connect(action_show_tool_bar, &QAction::triggered, this, &MainWindow::ToggleShowToolBar_);
    connect(action_show_file_bar, &QAction::triggered, this, &MainWindow::ToggleShowFileBar_);
    connect(action_show_edit_bar, &QAction::triggered, this, &MainWindow::ToggleShowEditBar_);
                  
    connect(action_preferences, &QAction::triggered, this, &MainWindow::Preferences_);
                  
    connect(action_about, &QAction::triggered, this, &MainWindow::About_);
    
    // tool bar
    connect(tool_bar->action_new,    &QAction::triggered, this, &MainWindow::New_   );
    connect(tool_bar->action_open,   &QAction::triggered, this, &MainWindow::Open_  );
    connect(tool_bar->action_save,   &QAction::triggered, this, &MainWindow::Save_  );
    connect(tool_bar->action_import, &QAction::triggered, this, &MainWindow::Import_);
    connect(tool_bar->action_export, &QAction::triggered, this, &MainWindow::Export_);

    connect(tool_bar->action_undo,   &QAction::triggered, this, &MainWindow::Undo_  );
    connect(tool_bar->action_redo,   &QAction::triggered, this, &MainWindow::Redo_  );
    connect(tool_bar->action_delete, &QAction::triggered, this, &MainWindow::Delete_);
}



void MainWindow::HandleMessage_(quint32 instance_id, QByteArray message) {
    QString strMessage = message.data();
    QStringList args = strMessage.split(", ", Qt::SkipEmptyParts);

    QString filePath;
    for (const auto& arg : args) {
        if (arg.endsWith(".ddm")) {
            filePath = arg;
            break;
        }
    }
    // TODO openFile
}



void MainWindow::New_() {

}



void MainWindow::Open_() {

}



void MainWindow::Save_() {

}



void MainWindow::SaveAs_() {

}



void MainWindow::SaveAll_() {

}



void MainWindow::Import_() {

}



void MainWindow::Export_() {

}



void MainWindow::ExportAs_() {

}



void MainWindow::Exit_() {
    close();
}



void MainWindow::Undo_() {

}



void MainWindow::Redo_() {

}



void MainWindow::Cut_() {

}



void MainWindow::Copy_() {

}



void MainWindow::Paste_() {

}



void MainWindow::Delete_() {

}



void MainWindow::SelectAll_() {

}



void MainWindow::ToggleShowToolBar_() {
    tool_bar->setVisible(action_show_tool_bar->isChecked());
}



void MainWindow::ToggleShowFileBar_() {
    tool_bar->setVisibleFileBar(action_show_file_bar->isChecked());
    tool_bar->updateBarSeparators();
}



void MainWindow::ToggleShowEditBar_() {
    tool_bar->setVisibleEditBar(action_show_edit_bar->isChecked());
    tool_bar->updateBarSeparators();
}



void MainWindow::Preferences_() {

}



void MainWindow::About_() {

}

}
