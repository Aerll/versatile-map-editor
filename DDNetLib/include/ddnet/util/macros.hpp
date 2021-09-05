#pragma once

#define FORCE_SEMICOLON_ static_assert(true)



#define ENUM_DEFINE_AND_(_EnumType)                                                         \
    inline constexpr _EnumType operator&(_EnumType lhs, _EnumType rhs) noexcept {           \
        return static_cast<_EnumType>(static_cast<qint32>(lhs) & static_cast<qint32>(rhs)); \
    }                                                                                       \
    inline constexpr _EnumType& operator&=(_EnumType& lhs, _EnumType rhs) noexcept {        \
        return lhs = lhs & rhs;                                                             \
    } FORCE_SEMICOLON_
// ENUM_DEFINE_AND

#define ENUM_DEFINE_OR_(_EnumType)                                                          \
    inline constexpr _EnumType operator|(_EnumType lhs, _EnumType rhs) noexcept {           \
        return static_cast<_EnumType>(static_cast<qint32>(lhs) | static_cast<qint32>(rhs)); \
    }                                                                                       \
    inline constexpr _EnumType& operator|=(_EnumType& lhs, _EnumType rhs) noexcept {        \
        return lhs = lhs | rhs;                                                             \
    } FORCE_SEMICOLON_
// ENUM_DEFINE_OR

#define ENUM_DEFINE_XOR_(_EnumType)                                                         \
    inline constexpr _EnumType operator^(_EnumType lhs, _EnumType rhs) noexcept {           \
        return static_cast<_EnumType>(static_cast<qint32>(lhs) ^ static_cast<qint32>(rhs)); \
    }                                                                                       \
    inline constexpr _EnumType& operator^=(_EnumType& lhs, _EnumType rhs) noexcept {        \
        return lhs = lhs ^ rhs;                                                             \
    } FORCE_SEMICOLON_
// ENUM_DEFINE_XOR

#define ENUM_DEFINE_ALL_(_EnumType) \
    ENUM_DEFINE_AND_(_EnumType);    \
    ENUM_DEFINE_OR_(_EnumType);     \
    ENUM_DEFINE_XOR_(_EnumType)
// ENUM_DEFINE_ALL
