#pragma once

#include <ddnet/debug/error_codes.hpp>

#include <QFileInfo>
#include <QString>

namespace ddnet::debug {

template <ErrorCode error_code> 
requires (error_code == ErrorCode::File_FailedToRead)
inline QString errorMessage(const QFileInfo& file_path) {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::File_FailedToWrite)
inline QString errorMessage(const QFileInfo& file_path) {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::File_NoData)
inline QString errorMessage(const QFileInfo& file_path) {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::ZLIB_CompressionFailed)
inline QString errorMessage() {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::ZLIB_DecompressionFailed)
inline QString errorMessage() {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::File_MAP_InvalidData)
inline QString errorMessage() {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::File_MAP_InvalidHeader)
inline QString errorMessage() {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::File_MAP_InvalidSignature)
inline QString errorMessage() {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::File_MAP_InvalidItemOffsetsSize)
inline QString errorMessage() {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::File_MAP_InvalidLayerType)
inline QString errorMessage() {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::File_MAP_FailedToLoadItemTypes)
inline QString errorMessage() {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::File_MAP_FailedToLoadItemOffsets)
inline QString errorMessage() {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::File_MAP_FailedToLoadDataOffsets)
inline QString errorMessage() {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::File_MAP_FailedToLoadDataSizes)
inline QString errorMessage() {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::File_MAP_FailedToLoadItemData)
inline QString errorMessage() {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::File_MAP_FailedToLoadCompressedData)
inline QString errorMessage() {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::File_MAP_UnsupportedVersion)
inline QString errorMessage() {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::File_MAP_UnsupportedItemVersion)
inline QString errorMessage() {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::File_MAP_UnsupportedItemEnvelopeVersion)
inline QString errorMessage() {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::File_MAP_UnsupportedItemGroupVersion)
inline QString errorMessage() {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::File_MAP_UnsupportedItemLayerTileVersion)
inline QString errorMessage() {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::File_MAP_UnsupportedItemLayerQuadVersion)
inline QString errorMessage() {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::File_MAP_UnsupportedItemLayerSoundVersion)
inline QString errorMessage() {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::File_DDL_FailedToTokenize)
inline QString errorMessage() {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::File_DDL_FailedToParse)
inline QString errorMessage() {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::Tokenizer_DDL_InvalidToken)
inline QString errorMessage(const QString& token, qint32 line) {
    return "Invalid token '" + token + "' at line: " + QString::number(line);
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::Parser_DDL_MissingIdentifier)
inline QString errorMessage(qint32 line) {
    return "Missing identifier at line: " + QString::number(line);
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::Parser_DDL_MissingAssignment)
inline QString errorMessage(qint32 line) {
    return "Missing '=' at line: " + QString::number(line);
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::Parser_DDL_MissingLiteralOperator)
inline QString errorMessage(qint32 line) {
    return "Missing ':' at line: " + QString::number(line);
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::Parser_DDL_MissingOpenBracket)
inline QString errorMessage(qint32 line) {
    return "Missing '{' at line: " + QString::number(line);
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::Parser_DDL_MissingCloseBracket)
inline QString errorMessage(qint32 line) {
    return "Missing '}' at line: " + QString::number(line);
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::Parser_DDL_UnexpectedToken)
inline QString errorMessage(const QString& token, qint32 line) {
    return "Unexpected token '" + token + "' at line: " + QString::number(line);
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::Resource_Value_InvalidSize)
inline QString errorMessage() {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::Resource_Value_InvalidSpacing)
inline QString errorMessage() {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::Resource_Value_InvalidCoordColor)
inline QString errorMessage() {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::Resource_Config_NoEntryPoint)
inline QString errorMessage() {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::Resource_Config_FailedToRead)
inline QString errorMessage() {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::Resource_Config_NoLangSpecified)
inline QString errorMessage() {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::Resource_Config_NoFontsSpecified)
inline QString errorMessage() {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::Resource_Config_NoThemesSpecified)
inline QString errorMessage() {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::Resource_Config_NoUiAssetsSpecified)
inline QString errorMessage() {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::Resource_Config_NoIconAssetsSpecified)
inline QString errorMessage() {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::Resource_Config_NoTextureAssetsSpecified)
inline QString errorMessage() {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::Resource_Config_MissingAttributeDataDir)
inline QString errorMessage() {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::Resource_Config_MissingAttributeAssetsDir)
inline QString errorMessage() {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::Resource_Config_MissingAttributeFontsDir)
inline QString errorMessage() {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::Resource_Config_MissingAttributeLangDir)
inline QString errorMessage() {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::Resource_Config_MissingAttributeThemesDir)
inline QString errorMessage() {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::Resource_Config_MissingAttributeSettingsFile)
inline QString errorMessage() {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::Resource_Config_MissingAttributeUiDir)
inline QString errorMessage() {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::Resource_Config_MissingAttributeIconsDir)
inline QString errorMessage() {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::Resource_Config_MissingAttributeTexturesDir)
inline QString errorMessage() {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::Resource_Settings_NoEntryPoint)
inline QString errorMessage() {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::Resource_Settings_FailedToRead)
inline QString errorMessage() {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::Resource_Settings_FontsFailedToRead)
inline QString errorMessage() {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::Resource_Settings_ThemeFailedToRead)
inline QString errorMessage() {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::Resource_Settings_ThemePaletteFailedToRead)
inline QString errorMessage() {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::Resource_Settings_ThemeAssetsFailedToRead)
inline QString errorMessage() {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::Resource_Assets_NoEntryPoint)
inline QString errorMessage() {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::Resource_Assets_FailedToRead)
inline QString errorMessage() {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::Resource_Font_FailedToReadFont)
inline QString errorMessage() {
    return {};
}



template <ErrorCode error_code>
requires (error_code == ErrorCode::Resource_Lang_FailedToReadTranslations)
inline QString errorMessage() {
    return {};
}




}
