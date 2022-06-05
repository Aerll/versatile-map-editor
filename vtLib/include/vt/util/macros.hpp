#pragma once

#define FORCE_SEMICOLON_ static_assert(true)



#define DEFAULT_COPY_(_ClassName)                   \
    inline _ClassName(const _ClassName&) = default; \
    inline _ClassName& operator=(const _ClassName&) = default
#define DEFAULT_MOVE_(_ClassName)              \
    inline _ClassName(_ClassName&&) = default; \
    inline _ClassName& operator=(_ClassName&&) = default

#define DELETE_COPY_(_ClassName)                   \
    inline _ClassName(const _ClassName&) = delete; \
    inline _ClassName& operator=(const _ClassName&) = delete
#define DELETE_MOVE_(_ClassName)              \
    inline _ClassName(_ClassName&&) = delete; \
    inline _ClassName& operator=(_ClassName&&) = delete



#define ENUM_DEFINE_AND_(_EnumType)                                                         \
    constexpr _EnumType operator&(_EnumType lhs, _EnumType rhs) noexcept {                  \
        return static_cast<_EnumType>(static_cast<qint32>(lhs) & static_cast<qint32>(rhs)); \
    }                                                                                       \
    constexpr _EnumType& operator&=(_EnumType& lhs, _EnumType rhs) noexcept {               \
        return lhs = lhs & rhs;                                                             \
    } FORCE_SEMICOLON_

#define ENUM_DEFINE_OR_(_EnumType)                                                          \
    constexpr _EnumType operator|(_EnumType lhs, _EnumType rhs) noexcept {                  \
        return static_cast<_EnumType>(static_cast<qint32>(lhs) | static_cast<qint32>(rhs)); \
    }                                                                                       \
    constexpr _EnumType& operator|=(_EnumType& lhs, _EnumType rhs) noexcept {               \
        return lhs = lhs | rhs;                                                             \
    } FORCE_SEMICOLON_

#define ENUM_DEFINE_XOR_(_EnumType)                                                         \
    constexpr _EnumType operator^(_EnumType lhs, _EnumType rhs) noexcept {                  \
        return static_cast<_EnumType>(static_cast<qint32>(lhs) ^ static_cast<qint32>(rhs)); \
    }                                                                                       \
    constexpr _EnumType& operator^=(_EnumType& lhs, _EnumType rhs) noexcept {               \
        return lhs = lhs ^ rhs;                                                             \
    } FORCE_SEMICOLON_

#define ENUM_DEFINE_BITWISE_OPS_(_EnumType) \
    ENUM_DEFINE_AND_(_EnumType);            \
    ENUM_DEFINE_OR_(_EnumType);             \
    ENUM_DEFINE_XOR_(_EnumType)
