#pragma once

#include <concepts>
#include <type_traits>

namespace vt::concepts {

template <typename _Type1, typename _Type2>
    concept SameAs = std::same_as<_Type1, _Type2>;

template <typename _Type1, typename _Type2>
    concept ConvertibleTo = std::convertible_to<_Type1, _Type2>;

template <typename _Type>
    concept Enum = std::is_enum_v<_Type>;

template <typename _Type>
    concept Integral = std::integral<_Type>;

template <typename _FunctionT, typename... _ArgsT>
    concept Invocable = std::invocable<_FunctionT, _ArgsT...>;

}
