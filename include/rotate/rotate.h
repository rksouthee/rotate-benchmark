#pragma once

#include <algorithm>

namespace rks {
template <typename I>
// TODO: Specify concept requirements
[[nodiscard]] I three_reverse(I first, I middle, I last)
{
    // TODO: Implement our own reverse to guarantee we're using vector instructions when possible.
    std::reverse(first, middle);
    std::reverse(middle, last);
    std::reverse(first, last);
    return first + (last - middle);
}
} // namespace rks
