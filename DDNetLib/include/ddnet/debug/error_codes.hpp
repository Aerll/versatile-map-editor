#pragma once

namespace ddnet::debug {

enum class ErrorCode {
    NoError = 0,

    // file
    File_FailedToRead,
    File_FailedToWrite,

    // zlib
    ZLIB_CompressionFailed,
    ZLIB_DecompressionFailed,
    
    // map file
    File_MAP_NoData,

    File_MAP_InvalidData,
    File_MAP_InvalidHeader,
    File_MAP_InvalidSignature,
    File_MAP_InvalidItemOffsetsSize,
    File_MAP_InvalidLayerType,

    File_MAP_FailedToLoadItemTypes,
    File_MAP_FailedToLoadItemOffsets,
    File_MAP_FailedToLoadDataOffsets,
    File_MAP_FailedToLoadDataSizes,
    File_MAP_FailedToLoadItemData,
    File_MAP_FailedToLoadCompressedData,

    File_MAP_UnsupportedVersion,
    File_MAP_UnsupportedItemVersion,
    File_MAP_UnsupportedItemEnvelopeVersion,
    File_MAP_UnsupportedItemGroupVersion,
    File_MAP_UnsupportedItemLayerTileVersion,
    File_MAP_UnsupportedItemLayerQuadVersion,
    File_MAP_UnsupportedItemLayerSoundVersion,

    // ddl file
    File_DDL_NoData,
    File_DDL_FailedToTokenize,
    File_DDL_FailedToParse,

    Tokenizer_DDL_InvalidToken,

    Parser_DDL_MissingIdentifier,
    Parser_DDL_MissingAssignment,
    Parser_DDL_MissingLiteralOperator,
    Parser_DDL_MissingOpenBracket,
    Parser_DDL_MissingCloseBracket,

    Parser_DDL_UnexpectedToken,

    // resource manager values
    Resource_Value_InvalidSize,
    Resource_Value_InvalidSpacing,
    Resource_Value_InvalidCoordColor,

    // resource manager config
    Resource_Config_NoEntryPoint,
    Resource_Config_FailedToRead,

    Resource_Config_NoLangSpecified,
    Resource_Config_NoFontsSpecified,
    Resource_Config_NoThemesSpecified,
    Resource_Config_NoUiAssetsSpecified,
    Resource_Config_NoIconAssetsSpecified,
    Resource_Config_NoTextureAssetsSpecified,

    Resource_Config_MissingAttributeDataDir,
    Resource_Config_MissingAttributeAssetsDir,
    Resource_Config_MissingAttributeFontsDir,
    Resource_Config_MissingAttributeLangDir,
    Resource_Config_MissingAttributeThemesDir,
    Resource_Config_MissingAttributeSettingsFile,
    Resource_Config_MissingAttributeUiDir,
    Resource_Config_MissingAttributeIconsDir,
    Resource_Config_MissingAttributeTexturesDir,

    // resource manager settings
    Resource_Settings_NoEntryPoint,
    Resource_Settings_FailedToRead,

    Resource_Settings_FontsFailedToRead,
    Resource_Settings_ThemeFailedToRead,
    Resource_Settings_ThemePaletteFailedToRead,
    Resource_Settings_ThemeAssetsFailedToRead,

    // resource manager assets
    Resource_Assets_NoEntryPoint,
    Resource_Assets_FailedToRead,

    // resource loading
    Resource_Font_FailedToReadFont,
    Resource_Lang_FailedToReadTranslations,
};

inline constexpr bool failed(ErrorCode error_code) noexcept {
    return error_code != ErrorCode::NoError;
}

} // ddnet::debug::
