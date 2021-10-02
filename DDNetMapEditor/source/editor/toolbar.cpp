#include <editor/toolbar.hpp>

#include <editor/app.hpp>

namespace ddnet::editor {

ToolBar::ToolBar(QWidget* parent)
    : QToolBar{ parent }
{
    setOrientation(Qt::Orientation::Horizontal);
    setAllowedAreas(Qt::TopToolBarArea);
    setMovable(false);
    setFloatable(false);

    initActions();

    is_visible.resize(2, false);

    setVisibleFileBar(true);
    setVisibleEditBar(true);
    updateBarSeparators();
}



void ToolBar::setVisibleFileBar(bool visible) {
    is_visible[0] = visible;
    action_new->setVisible(visible);
    action_open->setVisible(visible);
    action_save->setVisible(visible);
    action_import->setVisible(visible);
    action_export->setVisible(visible);
}



void ToolBar::setVisibleEditBar(bool visible) {
    is_visible[1] = visible;
    action_undo->setVisible(visible);
    action_redo->setVisible(visible);
    action_delete->setVisible(visible);
}



void ToolBar::updateBarSeparators() {
#define SET_SEPARATOR_VISIBLE_(separator_name, index)         \
    if (!is_visible[index])                                   \
        separator_name->setVisible(false);                    \
    else                                                      \
        separator_name->setVisible(hasVisibleBarAfter(index))

    SET_SEPARATOR_VISIBLE_(separator_file, 0);
    SET_SEPARATOR_VISIBLE_(separator_edit, 1);
}



void ToolBar::initActions() {
    using namespace std::chrono_literals;

    // initialize buttons
    action_new = new QAction{ this }; // TODO load shortcuts from DDL
    action_new->setIcon(QPixmap::fromImage(App::resources.asset(AssetIndex::Icon24_Normal_NewFile)));
    action_new->setToolTip(App::resources.string(StringIndex::ToolTipFileNew));
    action_new->startTimer(500ms);

    action_open = new QAction{ this };
    action_open->setIcon(QPixmap::fromImage(App::resources.asset(AssetIndex::Icon24_Normal_OpenFile)));
    action_open->setToolTip(App::resources.string(StringIndex::ToolTipFileOpen));
    action_open->startTimer(500ms);

    action_save = new QAction{ this };
    action_save->setIcon(QPixmap::fromImage(App::resources.asset(AssetIndex::Icon24_Normal_SaveFile)));
    action_save->setToolTip(App::resources.string(StringIndex::ToolTipFileSave));
    action_save->startTimer(500ms);

    action_import = new QAction{ this };
    action_import->setIcon(QPixmap::fromImage(App::resources.asset(AssetIndex::Icon24_Normal_Import)));
    action_import->setToolTip(App::resources.string(StringIndex::ToolTipFileImport));
    action_import->startTimer(500ms);

    action_export = new QAction{ this };
    action_export->setIcon(QPixmap::fromImage(App::resources.asset(AssetIndex::Icon24_Normal_Export)));
    action_export->setToolTip(App::resources.string(StringIndex::ToolTipFileExport));
    action_export->startTimer(500ms);

    action_undo = new QAction{ this };
    action_undo->setIcon(QPixmap::fromImage(App::resources.asset(AssetIndex::Icon24_Normal_Undo)));
    action_undo->setToolTip(App::resources.string(StringIndex::ToolTipEditUndo));
    action_undo->startTimer(500ms);

    action_redo = new QAction{ this };
    action_redo->setIcon(QPixmap::fromImage(App::resources.asset(AssetIndex::Icon24_Normal_Redo)));
    action_redo->setToolTip(App::resources.string(StringIndex::ToolTipEditRedo));
    action_redo->startTimer(500ms);

    action_delete = new QAction{ this };
    action_delete->setIcon(QPixmap::fromImage(App::resources.asset(AssetIndex::Icon24_Normal_Delete)));
    action_delete->setToolTip(App::resources.string(StringIndex::ToolTipEditDelete));
    action_delete->startTimer(500ms);

    // create buttons
    addAction(action_new);
    addAction(action_open);
    addAction(action_save);
    addAction(action_import);
    addAction(action_export);
    separator_file = addSeparator();
    addAction(action_undo);
    addAction(action_redo);
    addAction(action_delete);
    separator_edit = addSeparator();
}



bool ToolBar::hasVisibleBarAfter(qint32 bar_index) const {
    for (qint32 i = bar_index + 1; i < is_visible.size(); ++i) {
        if (is_visible[i])
            return true;
    }
    return false;
}

}
