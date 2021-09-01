#pragma once

#include <type_traits>

namespace ddnet::concepts {

template <typename _Type1, typename _Type2>
    concept SameAs = std::is_same_v<_Type1, _Type2>;
template <typename _Type1, typename _Type2>
    concept ConvertibleTo = std::is_convertible_v<_Type1, _Type2>;
template <typename _Type>
    concept Enum = std::is_enum_v<_Type>;

} // ddnet::concepts::
