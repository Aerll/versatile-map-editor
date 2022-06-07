#include <editor/resourcemanager.hpp>

#include <vt/io/ddlfilestream.hpp>
#include <vt/util/macros.hpp>
#include <vt/util/graphics.hpp>
#include <vt/util/utility.hpp>

#include <QFileInfo>
#include <QDirIterator>
#include <QDir>

#include <algorithm>
#include <tuple>

namespace vt::editor {

debug::ErrorCode ResourceManager::initResources() {

#define LOAD_CONFIG_(property_, variable_name_, attribute_name_, error_code_) \
    if (!property_.contains(attribute_name_))                                 \
        return error_code_;                                                   \
    QString& variable_name_ = property_[attribute_name_]
#define LOAD_SETTING_(property_, setting_index_, attribute_name_, default_value_) { \
    QString& temp_string = property_[attribute_name_];                              \
    if (temp_string.isEmpty())                                                      \
        temp_string = default_value_;                                               \
    settings[util::toUnderlying(setting_index_)] = temp_string;                     \
} FORCE_SEMICOLON_

    debug::ErrorCode error_code;
    
    // read config
    io::DDLFileStream config_ddl;
    if (util::failed(config_ddl.loadFile(QFileInfo{ "config.ddl" }))) [[unlikely]]
        return debug::ErrorCode::Resource_Config_FailedToRead;

    io::DDLProperty config_main_property = { .name = "Config", .value = "Main" };
    if (!config_ddl.data.properties.contains(config_main_property))
        return debug::ErrorCode::Resource_Config_NoEntryPoint;

    auto& config_main = config_ddl.data.properties[config_main_property];

    LOAD_CONFIG_(config_main, data_dir,     "DataDir",     debug::ErrorCode::Resource_Config_MissingAttributeDataDir    );
    LOAD_CONFIG_(config_main, assets_dir,   "AssetsDir",   debug::ErrorCode::Resource_Config_MissingAttributeAssetsDir  );
    LOAD_CONFIG_(config_main, fonts_dir,    "FontsDir",    debug::ErrorCode::Resource_Config_MissingAttributeFontsDir   );
    LOAD_CONFIG_(config_main, lang_dir,     "LangDir",     debug::ErrorCode::Resource_Config_MissingAttributeLangDir    );
    LOAD_CONFIG_(config_main, themes_dir,   "ThemesDir",   debug::ErrorCode::Resource_Config_MissingAttributeThemesDir  );
    LOAD_CONFIG_(config_main, ui_dir,       "UiDir",       debug::ErrorCode::Resource_Config_MissingAttributeUiDir      );
    LOAD_CONFIG_(config_main, icons_dir,    "IconsDir",    debug::ErrorCode::Resource_Config_MissingAttributeIconsDir   );
    LOAD_CONFIG_(config_main, textures_dir, "TexturesDir", debug::ErrorCode::Resource_Config_MissingAttributeTexturesDir);

    LOAD_CONFIG_(config_main, settings_file, "SettingsFile", debug::ErrorCode::Resource_Config_MissingAttributeSettingsFile);



    // read settings
    io::DDLFileStream settings_ddl;
    if (util::failed(settings_ddl.loadFile(QFileInfo{ settings_file }))) [[unlikely]]
        return debug::ErrorCode::Resource_Settings_FailedToRead;

    io::DDLProperty settings_main_property = { .name = "Settings", .value = "Main" };
    if (!settings_ddl.data.properties.contains(settings_main_property))
        return debug::ErrorCode::Resource_Settings_NoEntryPoint;

    auto& settings_main = settings_ddl.data.properties[settings_main_property];

    settings.resize(util::toUnderlying(SettingIndex::TotalCount));
    LOAD_SETTING_(settings_main, SettingIndex::Language,   "Language",   "english"        );
    LOAD_SETTING_(settings_main, SettingIndex::Theme,      "Theme",      "darkdefault"    );
    LOAD_SETTING_(settings_main, SettingIndex::FontFamily, "FontFamily", "Source Sans Pro");
    LOAD_SETTING_(settings_main, SettingIndex::FontSize,   "FontSize",   "12"             );



    // initialize a list of resources
    listResourceFiles(ResourceType::AssetUi,      { data_dir + assets_dir + ui_dir       });
    listResourceFiles(ResourceType::AssetIcon,    { data_dir + assets_dir + icons_dir    });
    listResourceFiles(ResourceType::AssetTexture, { data_dir + assets_dir + textures_dir });
    listResourceFiles(ResourceType::Font,         { data_dir + fonts_dir                 });
    listResourceFiles(ResourceType::Theme,        { data_dir + themes_dir                });
    listResourceFiles(ResourceType::Lang,         { data_dir + lang_dir                  });

    // initialize translation strings
    error_code = initTranslationStrings();
    if (util::failed(error_code))
        return error_code;

    error_code = initAssets();
    if (util::failed(error_code))
        return error_code;

    return debug::ErrorCode::NoError;
}



QString ResourceManager::relativePath(ResourceType resource_type, const QString& file_name) {
    auto& file_paths = resource_files[resource_type];
    auto file_path = std::ranges::find_if(file_paths, [&file_name](const auto& file_path) {
        return file_path.lastIndexOf(file_name) != -1;
    });
    if (file_path == file_paths.end())
        return {};
    return *file_path;
}


std::map<QString, QString>& ResourceManager::widgetStyles(WidgetStyleType style_type, const QString& widget_name) {
    if (style_type == WidgetStyleType::Vt)
        return widget_styles.properties[{ .name = "Vt", .value = widget_name }];
    else // style_type == WidgetStyleType::Qt
        return widget_styles.properties[{ .name = "Qt", .value = widget_name }];
}



std::vector<QString> ResourceManager::fonts(debug::ErrorCode& error_code) {
    error_code = debug::ErrorCode::NoError;
    if (!resource_files.contains(ResourceType::Font)) [[unlikely]] {
        error_code = debug::ErrorCode::Resource_Config_NoFontsSpecified;
        return {};
    }

    // add an extension to the file name
    std::vector<QString>& font_files = resource_files[ResourceType::Font];

    if (font_files.empty()) {
        error_code = debug::ErrorCode::Resource_Settings_FontsFailedToRead;
        return {};
    }
    return font_files;
}



QByteArray ResourceManager::theme(debug::ErrorCode& error_code) {
    error_code = debug::ErrorCode::NoError;
    if (!resource_files.contains(ResourceType::Theme)) [[unlikely]] {
        error_code = debug::ErrorCode::Resource_Config_NoThemesSpecified;
        return {};
    }

    // add a suffix to the theme name
    QString theme_file_name = setting(SettingIndex::Theme) + "-widgets.ddl";

    io::DDLFileStream theme_ddl;
    if (util::failed(theme_ddl.loadFile(QFileInfo{ relativePath(ResourceType::Theme, theme_file_name) }))) [[unlikely]] {
        error_code = debug::ErrorCode::Resource_Settings_ThemeFailedToRead;
        return {};
    }
    widget_styles = std::move(theme_ddl.data);
    return widget_styles.toQss();
}



QPalette ResourceManager::themePalette(debug::ErrorCode& error_code) {
    error_code = debug::ErrorCode::NoError;
    if (!resource_files.contains(ResourceType::Theme)) [[unlikely]] {
        error_code = debug::ErrorCode::Resource_Config_NoThemesSpecified;
        return {};
    }

    // add a suffix to a theme name
    QString theme_palette_file_name = setting(SettingIndex::Theme) + "-palette.ddl";

    io::DDLFileStream palette_ddl;
    if (util::failed(palette_ddl.loadFile(QFileInfo{ relativePath(ResourceType::Theme, theme_palette_file_name) }))) [[unlikely]] {
        error_code = debug::ErrorCode::Resource_Settings_ThemePaletteFailedToRead;
        return {};
    }

    QPalette palette;

    auto _GetColor = [&](const QString& hex_color) {
        QColor color;
        color.setNamedColor(hex_color);
        return color;
    };

    auto _SetColors = [&](QPalette::ColorGroup color_group, const io::DDLProperty& property) -> void {
        auto& group_property = palette_ddl.data.properties[property];
        palette.setColor(color_group, QPalette::Window,          _GetColor(group_property["Window"])         );
        palette.setColor(color_group, QPalette::WindowText,      _GetColor(group_property["WindowText"])     );
        palette.setColor(color_group, QPalette::Base,            _GetColor(group_property["Base"])           );
        palette.setColor(color_group, QPalette::AlternateBase,   _GetColor(group_property["AlternateBase"])  );      
        palette.setColor(color_group, QPalette::ToolTipBase,     _GetColor(group_property["ToolTipBase"])    );    
        palette.setColor(color_group, QPalette::ToolTipText,     _GetColor(group_property["ToolTipText"])    );    
        palette.setColor(color_group, QPalette::PlaceholderText, _GetColor(group_property["PlaceholderText"]));        
        palette.setColor(color_group, QPalette::Text,            _GetColor(group_property["Text"])           );
        palette.setColor(color_group, QPalette::Button,          _GetColor(group_property["Button"])         );
        palette.setColor(color_group, QPalette::ButtonText,      _GetColor(group_property["ButtonText"])     );
        palette.setColor(color_group, QPalette::BrightText,      _GetColor(group_property["BrightText"])     );
        palette.setColor(color_group, QPalette::Light,           _GetColor(group_property["Light"])          );
        palette.setColor(color_group, QPalette::Midlight,        _GetColor(group_property["Midlight"])       );
        palette.setColor(color_group, QPalette::Dark,            _GetColor(group_property["Dark"])           );
        palette.setColor(color_group, QPalette::Mid,             _GetColor(group_property["Mid"])            );
        palette.setColor(color_group, QPalette::Shadow,          _GetColor(group_property["Shadow"])         );
        palette.setColor(color_group, QPalette::Highlight,       _GetColor(group_property["Highlight"])      );
        palette.setColor(color_group, QPalette::HighlightedText, _GetColor(group_property["HighlightedText"]));
        palette.setColor(color_group, QPalette::Link,            _GetColor(group_property["Link"])           );
        palette.setColor(color_group, QPalette::LinkVisited,     _GetColor(group_property["LinkVisited"])    );
    };

    _SetColors(QPalette::Active,   { .name = "Group", .value = "Normal"   });
    _SetColors(QPalette::Inactive, { .name = "Group", .value = "Inactive" });
    _SetColors(QPalette::Disabled, { .name = "Group", .value = "Disabled" });
    return palette;
}



void ResourceManager::listResourceFiles(ResourceType resource_type, const QDir& dir) {
    QDirIterator it{ dir, QDirIterator::Subdirectories };
    
    std::vector<QString> temp_buffer;
    while (it.hasNext()) {
        QFileInfo file_info = QFileInfo{ it.next() };
        if (extensionSupported(resource_type, file_info.suffix()))
            temp_buffer.push_back(file_info.filePath());
    }

    if (!temp_buffer.empty())
        resource_files[resource_type] = std::move(temp_buffer);
}



bool ResourceManager::extensionSupported(ResourceType resource_type, const QString& extension) const {
    switch (resource_type) {
        case ResourceType::AssetUi:
        case ResourceType::AssetIcon:
            if (extension != "png" && extension != "svg")
                return false;
            break;
        case ResourceType::AssetTexture:
            if (extension != "png")
                return false;
            break;

        case ResourceType::Font:
            if (extension != "ttf")
                return false;
            break;

        case ResourceType::Theme:
        case ResourceType::Lang:
            if (extension != "ddl")
                return false;
            break;
    }
    return true;
}



debug::ErrorCode ResourceManager::initAssets() {
    if (!resource_files.contains(ResourceType::Theme)) [[unlikely]]
        return debug::ErrorCode::Resource_Config_NoThemesSpecified;
    if (!resource_files.contains(ResourceType::AssetUi)) [[unlikely]]
        return debug::ErrorCode::Resource_Config_NoUiAssetsSpecified;
    if (!resource_files.contains(ResourceType::AssetIcon)) [[unlikely]]
        return debug::ErrorCode::Resource_Config_NoIconAssetsSpecified;
    if (!resource_files.contains(ResourceType::AssetTexture)) [[unlikely]]
        return debug::ErrorCode::Resource_Config_NoTextureAssetsSpecified;

    QString theme_assets_file_name = setting(SettingIndex::Theme) + "-assets.ddl";

    io::DDLFileStream assets_ddl;
    if (util::failed(assets_ddl.loadFile(QFileInfo{ relativePath(ResourceType::Theme, theme_assets_file_name) }))) [[unlikely]]
        return debug::ErrorCode::Resource_Settings_ThemeAssetsFailedToRead;

    io::DDLProperty assets_config_property = { .name = "Assets", .value = "Config" };
    if (!assets_ddl.data.properties.contains(assets_config_property))
        return debug::ErrorCode::Resource_Assets_NoEntryPoint;

    const QString sub_dir = assets_ddl.data.properties[assets_config_property]["SubDir"];
    return createAssets(assets_ddl.data, sub_dir);
}



debug::ErrorCode ResourceManager::initTranslationStrings() {
    if (!resource_files.contains(ResourceType::Lang)) [[unlikely]]
        return debug::ErrorCode::Resource_Config_NoLangSpecified;

    // add an extension to the file name
    QString language_file_name = setting(SettingIndex::Language) + ".ddl";

    io::DDLFileStream language_ddl;
    if (util::failed(language_ddl.loadFile(QFileInfo{ relativePath(ResourceType::Lang, language_file_name) }))) [[unlikely]]
        return debug::ErrorCode::Resource_Lang_FailedToReadTranslations;

    qsizetype count = 0;
    for (const auto& property : language_ddl.data.properties)
        count += property.second.size();

    if (count != util::toUnderlying(StringIndex::TotalCount))
        return debug::ErrorCode::Resource_Lang_FailedToReadTranslations;

    return createTranslationStrings(language_ddl.data);
}



debug::ErrorCode ResourceManager::createAssets(io::DDLData& data, const QString& sub_dir) {
    assets.resize(util::toUnderlying(AssetIndex::TotalCount));
    icons.resize(util::toUnderlying(IconIndex::TotalCount));

    debug::ErrorCode error_code = debug::ErrorCode::NoError;
    auto _CreateAsset = [&](const io::DDLProperty& property, AssetIndex asset_index, ResourceType resource_type, const QString& attribute_name) -> void {
        auto value = data.properties[property][attribute_name].split(',');
        if (value.size() < 1 || value.size() > 2) [[unlikely]] {
            error_code = debug::ErrorCode::Resource_Value_InvalidNameColor;
            return;
        }

        QString file_name = std::move(value[0]);
        if (!QFileInfo{ relativePath(resource_type, sub_dir + file_name) }.exists()) [[unlikely]] {
            error_code = debug::ErrorCode::File_FailedToRead;
            return;
        }

        QColor color;
        if (value.size() == 2)
            color.setNamedColor(value[1]);
        else
            color = Qt::white;

        QImage image{ relativePath(resource_type, sub_dir + file_name) };
        util::recolor(image, color);
        assets[util::toUnderlying(asset_index)] = std::move(image);
    };
    
    auto _CreateIcon = [&](const io::DDLProperty& property, IconIndex icon_index, ResourceType resource_type, const QString& attribute_name) -> void {
        auto value = data.properties[property][attribute_name];

        QString file_name = std::move(value);
        if (!QFileInfo{ relativePath(resource_type, sub_dir + file_name) }.exists()) [[unlikely]] {
            error_code = debug::ErrorCode::File_FailedToRead;
            return;
        }

        QIcon icon{ relativePath(resource_type, sub_dir + file_name) };
        icons[util::toUnderlying(icon_index)] = std::move(icon);
    };

    { // ui assets normal
        io::DDLProperty ui_normal = { .name = "Ui", .value = "Normal" };
        if (util::failed(error_code))
            return error_code;

        _CreateAsset(ui_normal, AssetIndex::Ui16_Normal_ArrowMenu,   ResourceType::AssetUi, "ArrowMenu"  ); if (util::failed(error_code)) return error_code;
        _CreateAsset(ui_normal, AssetIndex::Ui16_Normal_ButtonClose, ResourceType::AssetUi, "ButtonClose"); if (util::failed(error_code)) return error_code;
        _CreateAsset(ui_normal, AssetIndex::Ui16_Normal_CheckMenu,   ResourceType::AssetUi, "CheckMenu"  ); if (util::failed(error_code)) return error_code;
    }

    { // ui assets hover
        io::DDLProperty ui_hover = { .name = "Ui", .value = "Hover" };
        if (util::failed(error_code))
            return error_code;

        _CreateAsset(ui_hover, AssetIndex::Ui16_Hover_ArrowMenu,   ResourceType::AssetUi, "ArrowMenu"  ); if (util::failed(error_code)) return error_code;
        _CreateAsset(ui_hover, AssetIndex::Ui16_Hover_ButtonClose, ResourceType::AssetUi, "ButtonClose"); if (util::failed(error_code)) return error_code;
        _CreateAsset(ui_hover, AssetIndex::Ui16_Hover_CheckMenu,   ResourceType::AssetUi, "CheckMenu"  ); if (util::failed(error_code)) return error_code;
    }

    { // ui assets pressed
        io::DDLProperty ui_pressed = { .name = "Ui", .value = "Pressed" };
        if (util::failed(error_code))
            return error_code;

        _CreateAsset(ui_pressed, AssetIndex::Ui16_Pressed_ArrowMenu,   ResourceType::AssetUi, "ArrowMenu"  ); if (util::failed(error_code)) return error_code;
        _CreateAsset(ui_pressed, AssetIndex::Ui16_Pressed_ButtonClose, ResourceType::AssetUi, "ButtonClose"); if (util::failed(error_code)) return error_code;
        _CreateAsset(ui_pressed, AssetIndex::Ui16_Pressed_CheckMenu,   ResourceType::AssetUi, "CheckMenu"  ); if (util::failed(error_code)) return error_code;
    }

    { // ui textures
        io::DDLProperty ui_texture = { .name = "Ui", .value = "Textures" };

        _CreateAsset(ui_texture, AssetIndex::Ui_Texture_Checker, ResourceType::AssetTexture, "CheckerBg"); if (util::failed(error_code)) return error_code;
    }

    { // icon assets normal 16x16
        io::DDLProperty icons_normal_16 = { .name = "Icons", .value = "Normal_16" };
        if (util::failed(error_code))
            return error_code;
        
        _CreateIcon(icons_normal_16, IconIndex::Icon16_Normal_NewFile,     ResourceType::AssetIcon, "NewFile"    ); if (util::failed(error_code)) return error_code;
        _CreateIcon(icons_normal_16, IconIndex::Icon16_Normal_OpenFile,    ResourceType::AssetIcon, "OpenFile"   ); if (util::failed(error_code)) return error_code;
        _CreateIcon(icons_normal_16, IconIndex::Icon16_Normal_SaveFile,    ResourceType::AssetIcon, "SaveFile"   ); if (util::failed(error_code)) return error_code;
        _CreateIcon(icons_normal_16, IconIndex::Icon16_Normal_Import,      ResourceType::AssetIcon, "Import"     ); if (util::failed(error_code)) return error_code;
        _CreateIcon(icons_normal_16, IconIndex::Icon16_Normal_Export,      ResourceType::AssetIcon, "Export"     ); if (util::failed(error_code)) return error_code;
        _CreateAsset(icons_normal_16, AssetIndex::Icon16_Normal_LayerTile,   ResourceType::AssetIcon, "LayerTile"  ); if (util::failed(error_code)) return error_code;
        _CreateAsset(icons_normal_16, AssetIndex::Icon16_Normal_LayerQuad,   ResourceType::AssetIcon, "LayerQuad"  ); if (util::failed(error_code)) return error_code;
        _CreateAsset(icons_normal_16, AssetIndex::Icon16_Normal_LayerSound,  ResourceType::AssetIcon, "LayerSound" ); if (util::failed(error_code)) return error_code;
        _CreateAsset(icons_normal_16, AssetIndex::Icon16_Normal_MapresImage, ResourceType::AssetIcon, "MapresImage"); if (util::failed(error_code)) return error_code;
        _CreateAsset(icons_normal_16, AssetIndex::Icon16_Normal_MapresSound, ResourceType::AssetIcon, "MapresSound"); if (util::failed(error_code)) return error_code;
        _CreateAsset(icons_normal_16, AssetIndex::Icon16_Normal_Grid,        ResourceType::AssetIcon, "Grid"       ); if (util::failed(error_code)) return error_code;
        _CreateIcon(icons_normal_16, IconIndex::Icon16_Normal_Undo,        ResourceType::AssetIcon, "Undo"       ); if (util::failed(error_code)) return error_code;
        _CreateIcon(icons_normal_16, IconIndex::Icon16_Normal_Redo,        ResourceType::AssetIcon, "Redo"       ); if (util::failed(error_code)) return error_code;
        _CreateIcon(icons_normal_16, IconIndex::Icon16_Normal_Delete,      ResourceType::AssetIcon, "Delete"     ); if (util::failed(error_code)) return error_code;
    }

    { // icon assets normal 24x24
        io::DDLProperty icons_normal_24 = { .name = "Icons", .value = "Normal_24" };
        if (util::failed(error_code))
            return error_code;
        
        _CreateIcon(icons_normal_24, IconIndex::Icon24_Normal_NewFile,     ResourceType::AssetIcon, "NewFile"    ); if (util::failed(error_code)) return error_code;
        _CreateIcon(icons_normal_24, IconIndex::Icon24_Normal_OpenFile,    ResourceType::AssetIcon, "OpenFile"   ); if (util::failed(error_code)) return error_code;
        _CreateIcon(icons_normal_24, IconIndex::Icon24_Normal_SaveFile,    ResourceType::AssetIcon, "SaveFile"   ); if (util::failed(error_code)) return error_code;
        _CreateIcon(icons_normal_24, IconIndex::Icon24_Normal_Import,      ResourceType::AssetIcon, "Import"     ); if (util::failed(error_code)) return error_code;
        _CreateIcon(icons_normal_24, IconIndex::Icon24_Normal_Export,      ResourceType::AssetIcon, "Export"     ); if (util::failed(error_code)) return error_code;
        _CreateAsset(icons_normal_24, AssetIndex::Icon24_Normal_LayerTile,   ResourceType::AssetIcon, "LayerTile"  ); if (util::failed(error_code)) return error_code;
        _CreateAsset(icons_normal_24, AssetIndex::Icon24_Normal_LayerQuad,   ResourceType::AssetIcon, "LayerQuad"  ); if (util::failed(error_code)) return error_code;
        _CreateAsset(icons_normal_24, AssetIndex::Icon24_Normal_LayerSound,  ResourceType::AssetIcon, "LayerSound" ); if (util::failed(error_code)) return error_code;
        _CreateAsset(icons_normal_24, AssetIndex::Icon24_Normal_MapresImage, ResourceType::AssetIcon, "MapresImage"); if (util::failed(error_code)) return error_code;
        _CreateAsset(icons_normal_24, AssetIndex::Icon24_Normal_MapresSound, ResourceType::AssetIcon, "MapresSound"); if (util::failed(error_code)) return error_code;
        _CreateAsset(icons_normal_24, AssetIndex::Icon24_Normal_Grid,        ResourceType::AssetIcon, "Grid"       ); if (util::failed(error_code)) return error_code;
        _CreateIcon(icons_normal_24, IconIndex::Icon24_Normal_Undo,        ResourceType::AssetIcon, "Undo"       ); if (util::failed(error_code)) return error_code;
        _CreateIcon(icons_normal_24, IconIndex::Icon24_Normal_Redo,        ResourceType::AssetIcon, "Redo"       ); if (util::failed(error_code)) return error_code;
        _CreateIcon(icons_normal_24, IconIndex::Icon24_Normal_Delete,      ResourceType::AssetIcon, "Delete"     ); if (util::failed(error_code)) return error_code;
    }

    return error_code;
}



debug::ErrorCode ResourceManager::createTranslationStrings(io::DDLData& data) {
    strings.resize(util::toUnderlying(StringIndex::TotalCount));

    debug::ErrorCode error_code = debug::ErrorCode::NoError;
    auto _CreateString = [&](const io::DDLProperty& property, StringIndex string_index, const QString& attribute_name) {
        strings[util::toUnderlying(string_index)] = data.properties[property][attribute_name];
    };

    { // menu bar
        io::DDLProperty menu_bar = { .name = "Translation", .value = "MenuBar" };
        _CreateString(menu_bar, StringIndex::MenuFile,                "MenuFile"               );
        _CreateString(menu_bar, StringIndex::MenuFileNew,             "MenuFileNew"            );
        _CreateString(menu_bar, StringIndex::MenuFileOpen,            "MenuFileOpen"           );
        _CreateString(menu_bar, StringIndex::MenuFileSave,            "MenuFileSave"           );
        _CreateString(menu_bar, StringIndex::MenuFileSaveAs,          "MenuFileSaveAs"         );
        _CreateString(menu_bar, StringIndex::MenuFileSaveAll,         "MenuFileSaveAll"        );
        _CreateString(menu_bar, StringIndex::MenuFileImport,          "MenuFileImport"         );
        _CreateString(menu_bar, StringIndex::MenuFileExport,          "MenuFileExport"         );
        _CreateString(menu_bar, StringIndex::MenuFileExportAs,        "MenuFileExportAs"       );
        _CreateString(menu_bar, StringIndex::MenuFileExit,            "MenuFileExit"           );
        _CreateString(menu_bar, StringIndex::MenuEdit,                "MenuEdit"               );
        _CreateString(menu_bar, StringIndex::MenuEditUndo,            "MenuEditUndo"           );
        _CreateString(menu_bar, StringIndex::MenuEditRedo,            "MenuEditRedo"           );
        _CreateString(menu_bar, StringIndex::MenuEditCut,             "MenuEditCut"            );
        _CreateString(menu_bar, StringIndex::MenuEditCopy,            "MenuEditCopy"           );
        _CreateString(menu_bar, StringIndex::MenuEditPaste,           "MenuEditPaste"          );
        _CreateString(menu_bar, StringIndex::MenuEditDelete,          "MenuEditDelete"         );
        _CreateString(menu_bar, StringIndex::MenuEditSelectAll,       "MenuEditSelectAll"      );
        _CreateString(menu_bar, StringIndex::MenuView,                "MenuView"               );
        _CreateString(menu_bar, StringIndex::SubMenuToolBar,          "SubMenuToolBar"         );
        _CreateString(menu_bar, StringIndex::SubMenuViewToolBar,      "SubMenuViewToolBar"     );
        _CreateString(menu_bar, StringIndex::SubMenuViewFileBar,      "SubMenuViewFileBar"     );
        _CreateString(menu_bar, StringIndex::SubMenuViewEditBar,      "SubMenuViewEditBar"     );
        _CreateString(menu_bar, StringIndex::MenuSettings,            "MenuSettings"           );
        _CreateString(menu_bar, StringIndex::MenuSettingsPreferences, "MenuSettingsPreferences");
        _CreateString(menu_bar, StringIndex::MenuHelp,                "MenuHelp"               );
        _CreateString(menu_bar, StringIndex::MenuHelpAbout,           "MenuHelpAbout"          );
    }

    { // tool tip
        io::DDLProperty tool_tip = { .name = "Translation", .value = "ToolTip" };
        _CreateString(tool_tip, StringIndex::ToolTipFileNew,    "ToolTipFileNew"   );
        _CreateString(tool_tip, StringIndex::ToolTipFileOpen,   "ToolTipFileOpen"  );
        _CreateString(tool_tip, StringIndex::ToolTipFileSave,   "ToolTipFileSave"  );
        _CreateString(tool_tip, StringIndex::ToolTipFileImport, "ToolTipFileImport");
        _CreateString(tool_tip, StringIndex::ToolTipFileExport, "ToolTipFileExport");
        _CreateString(tool_tip, StringIndex::ToolTipEditUndo,   "ToolTipEditUndo"  );
        _CreateString(tool_tip, StringIndex::ToolTipEditRedo,   "ToolTipEditRedo"  );
        _CreateString(tool_tip, StringIndex::ToolTipEditDelete, "ToolTipEditDelete");
    }

    return error_code;
}

}
