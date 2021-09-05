#pragma once

#include <ddnet/util/concepts.hpp>
#include <ddnet/util/constants.hpp>

#include <QtGlobal>
#include <QDataStream>

#include <cstddef>
#include <utility>
#include <variant>

namespace ddnet::concepts::detail {

template <typename _Type>
concept ContainerWithSize = requires(_Type type) {
    typename _Type::value_type;
    { type.size() } -> concepts::ConvertibleTo<qint32>;
};

} // ddnet::concepts::detail::

namespace ddnet::util {

template <concepts::detail::ContainerWithSize _Container>
inline qint32 byteSize(const _Container& container) noexcept {
    return container.size() * sizeof(_Container::value_type);
}

template <concepts::detail::ContainerWithSize _Container>
inline qint32 totalSize(const std::vector<_Container>& vector) noexcept {
    qint32 result = 0;
    for (const auto& elem : vector)
        result += elem.size();
    return result;
}

template <typename _Type>
inline qint32 readRawData(QDataStream& data_stream, _Type* destination, qint32 destination_size) {
    return data_stream.readRawData(reinterpret_cast<char*>(destination), destination_size);
}

template <typename _Type>
inline qint32 writeRawData(QDataStream& data_stream, _Type* source, qint32 source_size) {
    return data_stream.writeRawData(reinterpret_cast<const char*>(source), source_size);
}

template <typename _Type, typename _VariantType, size_t _Index = 0>
requires (
    _Index < std::variant_size_v<_VariantType>
    )
inline constexpr size_t typeIndex(const _VariantType& variant) {
    if constexpr (concepts::SameAs<std::variant_alternative_t<_Index, _VariantType>, _Type>)
        return _Index;
    else
        return util::typeIndex<_Type, _VariantType, _Index + 1>(variant);
}

template <concepts::Enum _EnumType, typename _UnderlyingType = std::underlying_type_t<_EnumType>>
inline constexpr _UnderlyingType toUnderlying(_EnumType value) noexcept {
    return static_cast<_UnderlyingType>(value);
}

inline constexpr qint32 floatToFixed(qreal value) noexcept { 
    return static_cast<int>(value * static_cast<qreal>(1 << constants::_fractional_bits));
}

inline constexpr float fixedToFloat(qint32 value) noexcept { 
    return value * (1.f / (1 << constants::_fractional_bits));
}

} // ddnet::util::
