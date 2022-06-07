#pragma once

#include <vt/debug/error_code.hpp>
#include <vt/util/concepts.hpp>
#include <vt/util/constants.hpp>

#include <QtGlobal>
#include <QDataStream>

#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

namespace vt::util {

template <concepts::Integral _Type>
consteval _Type flag(_Type bit) noexcept {
    return static_cast<_Type>(1) << bit;
}



template <typename _ContainerType>
[[nodiscard]] inline qsizetype byteSize(const _ContainerType& container) noexcept {
    return container.size() * sizeof(_ContainerType::value_type);
}



template <typename _ContainerType>
requires (
    requires (_ContainerType type) {
        typename _ContainerType::value_type;
        { type.size() } -> concepts::ConvertibleTo<qint32>;
    })
[[nodiscard]] inline qint32 totalSize(const std::vector<_ContainerType>& vector) noexcept {
    qint32 result = 0;
    for (const auto& elem : vector)
        result += elem.size();
    return result;
}



template <typename _Type, typename _VariantType, size_t _Index = 0>
requires (
    _Index < std::variant_size_v<_VariantType>
    )
[[nodiscard]] constexpr size_t typeIndex(const _VariantType& variant) {
    if constexpr (concepts::SameAs<std::variant_alternative_t<_Index, _VariantType>, _Type>)
        return _Index;
    else
        return util::typeIndex<_Type, _VariantType, _Index + 1>(variant);
}



template <concepts::Enum _EnumType, typename _UnderlyingType = std::underlying_type_t<_EnumType>>
[[nodiscard]] constexpr _UnderlyingType toUnderlying(_EnumType value) noexcept {
    return static_cast<_UnderlyingType>(value);
}



template <typename _Type>
inline qint32 readRawData(QDataStream& data_stream, _Type* destination, qint32 destination_size) {
    return data_stream.readRawData(reinterpret_cast<char*>(destination), destination_size);
}



template <typename _Type>
inline qint32 writeRawData(QDataStream& data_stream, _Type* source, qint32 source_size) {
    return data_stream.writeRawData(reinterpret_cast<const char*>(source), source_size);
}



[[nodiscard]] constexpr qint32 floatToFixed(qreal value) noexcept {
    return static_cast<qint32>(value * static_cast<qreal>(1 << constants::_fractional_bits));
}



[[nodiscard]] constexpr qreal fixedToFloat(qint32 value) noexcept {
    return value * (1.0 / (1 << constants::_fractional_bits));
}



[[nodiscard]] constexpr bool failed(debug::ErrorCode error_code) noexcept {
    return error_code != debug::ErrorCode::NoError;
}

}
