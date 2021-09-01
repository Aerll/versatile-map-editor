#pragma once

#include <ddnet/util/utility.hpp>

namespace ddnet::editor {

enum class ButtonState {
    None    = 0,
    Normal  = 1 << 0,
    Hover   = 1 << 1,
    Pressed = 1 << 2,
    Clicked = 1 << 3,
};

inline constexpr ButtonState operator|(ButtonState lhs, ButtonState rhs) noexcept { return static_cast<ButtonState>(util::toUnderlying(lhs) | util::toUnderlying(rhs)); }
inline constexpr ButtonState& operator|=(ButtonState& lhs, ButtonState rhs) noexcept { lhs = lhs | rhs; return lhs; }
inline constexpr ButtonState operator&(ButtonState lhs, ButtonState rhs) noexcept { return static_cast<ButtonState>(util::toUnderlying(lhs) & util::toUnderlying(rhs)); }
inline constexpr ButtonState& operator&=(ButtonState& lhs, ButtonState rhs) noexcept { lhs = lhs & rhs; return lhs; }
inline constexpr ButtonState operator^(ButtonState lhs, ButtonState rhs) noexcept { return static_cast<ButtonState>(util::toUnderlying(lhs) ^ util::toUnderlying(rhs)); }
inline constexpr ButtonState& operator^=(ButtonState& lhs, ButtonState rhs) noexcept { lhs = lhs ^ rhs; return lhs; }

enum class ResourceType {
    AssetUi,
    AssetIcon,
    AssetTexture,
    Font,
    Theme,
    Lang,
};

enum class WidgetStyleType {
    Qt,
    DDNet,
};

enum class AssetIndex : std::size_t {
    // ui
    Ui16_Normal_ArrowFold = 0,
    Ui16_Normal_ArrowUnfold,
    Ui16_Normal_ArrowScrollBarUp,
    Ui16_Normal_ArrowScrollBarDown,
    Ui16_Normal_ArrowScrollBarLeft,
    Ui16_Normal_ArrowScrollBarRight,
    Ui16_Normal_ArrowDropDown,
    Ui16_Normal_ArrowMenu,
    Ui16_Normal_ButtonMinimize,
    Ui16_Normal_ButtonMaximize,
    Ui16_Normal_ButtonClose,
    Ui16_Normal_RadioButton,
    Ui16_Normal_CheckBox,
    Ui16_Normal_CheckMenu,
    Ui16_Normal_ToggleShow,
    Ui16_Normal_ToggleHide,

    Ui16_Hover_ArrowFold,
    Ui16_Hover_ArrowUnfold,
    Ui16_Hover_ArrowScrollBarUp,
    Ui16_Hover_ArrowScrollBarDown,
    Ui16_Hover_ArrowScrollBarLeft,
    Ui16_Hover_ArrowScrollBarRight,
    Ui16_Hover_ArrowDropDown,
    Ui16_Hover_ArrowMenu,
    Ui16_Hover_ButtonMinimize,
    Ui16_Hover_ButtonMaximize,
    Ui16_Hover_ButtonClose,
    Ui16_Hover_RadioButton,
    Ui16_Hover_CheckBox,
    Ui16_Hover_CheckMenu,
    Ui16_Hover_ToggleShow,
    Ui16_Hover_ToggleHide,

    Ui16_Pressed_ArrowFold,
    Ui16_Pressed_ArrowUnfold,
    Ui16_Pressed_ArrowScrollBarUp,
    Ui16_Pressed_ArrowScrollBarDown,
    Ui16_Pressed_ArrowScrollBarLeft,
    Ui16_Pressed_ArrowScrollBarRight,
    Ui16_Pressed_ArrowDropDown,
    Ui16_Pressed_ArrowMenu,
    Ui16_Pressed_ButtonMinimize,
    Ui16_Pressed_ButtonMaximize,
    Ui16_Pressed_ButtonClose,
    Ui16_Pressed_RadioButton,
    Ui16_Pressed_CheckBox,
    Ui16_Pressed_CheckMenu,
    Ui16_Pressed_ToggleShow,
    Ui16_Pressed_ToggleHide,

    // icons
    Icon16_Normal_NewFile,
    Icon16_Normal_OpenFile,
    Icon16_Normal_SaveFile,
    Icon16_Normal_Import,
    Icon16_Normal_Export,
    Icon16_Normal_LayerTile,
    Icon16_Normal_LayerQuad,
    Icon16_Normal_LayerSound,
    Icon16_Normal_MapresImage,
    Icon16_Normal_MapresSound,
    Icon16_Normal_Grid,
    Icon16_Normal_Undo,
    Icon16_Normal_Redo,
    Icon16_Normal_Delete,

    Icon24_Normal_NewFile,
    Icon24_Normal_OpenFile,
    Icon24_Normal_SaveFile,
    Icon24_Normal_Import,
    Icon24_Normal_Export,
    Icon24_Normal_LayerTile,
    Icon24_Normal_LayerQuad,
    Icon24_Normal_LayerSound,
    Icon24_Normal_MapresImage,
    Icon24_Normal_MapresSound,
    Icon24_Normal_Grid,
    Icon24_Normal_Undo,
    Icon24_Normal_Redo,
    Icon24_Normal_Delete,

    // textures
    Ui_Texture_Checker,

    TotalCount,
};

enum class StringIndex : std::size_t {
    MenuFile = 0,
    MenuFileNew,
    MenuFileOpen,
    MenuFileSave,
    MenuFileSaveAs,
    MenuFileSaveAll,
    MenuFileImport,
    MenuFileExport,
    MenuFileExportAs,
    MenuFileExit,
    MenuEdit,
    MenuEditUndo,
    MenuEditRedo,
    MenuEditCut,
    MenuEditCopy,
    MenuEditPaste,
    MenuEditDelete,
    MenuEditSelectAll,
    MenuView,
    SubMenuToolBar,
    SubMenuViewToolBar,
    SubMenuViewFileBar,
    SubMenuViewEditBar,
    MenuSettings,
    MenuSettingsPreferences,
    MenuHelp,
    MenuHelpAbout,

    ToolTipFileNew,
    ToolTipFileOpen,
    ToolTipFileSave,
    ToolTipFileImport,
    ToolTipFileExport,
    ToolTipEditUndo,
    ToolTipEditRedo,
    ToolTipEditDelete,

    TotalCount,
};

enum class SettingIndex : std::size_t {
    Language = 0,
    Theme,
    FontFamily,
    FontSize,

    TotalCount,
};

} // ddnet::editor::
