#pragma once

#include "prelude/opus/ArrayBase.hpp"



namespace opus {

// type aliases
template <TLType T>
using Array = ArrayBase<T, true>;

template <TLType T>
using array = ArrayBase<T, false>;

using String = Array<symbol_t>;
using string = array<symbol_t>;



template <TLType T, bool LHS_BOXED, bool RHS_BOXED>
bool operator==(const ArrayBase<T, LHS_BOXED>& lhs, const ArrayBase<T, RHS_BOXED>& rhs)
{
    if (lhs.size() != rhs.size())
        return false;

    for (size_t i = 0; i < lhs.size(); ++i) {
        if (lhs[i] != rhs[i])
            return false;
    }

    return true;
}



template <bool BOXED>
bool operator==(const ArrayBase<symbol_t, BOXED>& lhs, std::string_view rhs) noexcept
{
    if (lhs.size() != rhs.size())
        return false;

    for (size_t i = 0; i < lhs.size(); ++i) {
        if (lhs[i] != rhs[i])
            return false;
    }
    return true;
}



template <typename T, typename U, bool BOXED>
bool operator==(const ArrayBase<T, BOXED>& lhs, const std::vector<U>& rhs)
{
    if (lhs.size() != rhs.size())
        return false;

    for (size_t i = 0; i < lhs.size(); ++i) {
        if (lhs[i] != rhs[i])
            return false;
    }

    return true;
}

}    // namespace opus
