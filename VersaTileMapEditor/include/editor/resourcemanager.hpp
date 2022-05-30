#pragma once

#include <editor/constants.hpp>
#include <vt/debug/error_code.hpp>
#include <vt/io/ddldata.hpp>

#include <QByteArray>
#include <QString>
#include <QPalette>
#include <QImage>

#include <map>
#include <vector>

class QDir;

namespace vt::editor {

class DDLData;

class ResourceManager {
public:
    inline ResourceManager(const ResourceManager&) = delete;
    inline ResourceManager& operator=(const ResourceManager&) = delete;
     
    inline ResourceManager() = default;
    inline ResourceManager(ResourceManager&&) = default;
    inline ResourceManager& operator=(ResourceManager&&) = default;

    debug::ErrorCode initResources();

    QString relativePath(ResourceType resource_type, const QString& file_name);

    const QString& setting(SettingIndex setting_index) const;
    const QString& string(StringIndex string_index) const;
    const QImage& asset(AssetIndex asset_index) const;
    std::map<QString, QString>& widgetStyles(WidgetStyleType style_type, const QString& widget_name);

    std::vector<QString> fonts(debug::ErrorCode& error_code);
    QByteArray theme(debug::ErrorCode& error_code);
    QPalette themePalette(debug::ErrorCode& error_code);

private:
    void listResourceFiles(ResourceType resource_type, const QDir& dir);
    bool extensionSupported(ResourceType resource_type, const QString& extension) const;

    debug::ErrorCode initAssets();
    debug::ErrorCode initTranslationStrings();

    debug::ErrorCode createAssets(io::DDLData& data, const QString& sub_dir);
    debug::ErrorCode createTranslationStrings(io::DDLData& data);

public:
    std::map<ResourceType, std::vector<QString>> resource_files;

    std::vector<QString> settings;
    std::vector<QString> strings;
    std::vector<QImage> assets;

    io::DDLData widget_styles;
};

}
