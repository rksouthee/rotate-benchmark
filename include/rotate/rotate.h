#pragma once

#include <algorithm>

namespace rks {
template <typename I>
[[nodiscard]] I rotate(I first, I middle, I last)
{
    return std::rotate(first, middle, last);
}
} // namespace rks
