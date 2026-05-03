#include "utility.h"

#include <cstddef>

void fill(std::vector<std::uint32_t>& v)
{
    for (std::size_t i = 0; i < v.size(); ++i)
    {
        v[i] = static_cast<std::uint32_t>(i);
    }
}

void fill(std::vector<std::string>& v)
{
    for (std::size_t i = 0; i < v.size(); ++i)
    {
        // TODO: Should we avoid Small String Optimisation
        v[i] = "string " + std::to_string(i);
    }
}

void fill(std::vector<Trivial64>& v)
{
    for (std::size_t i = 0; i < v.size(); ++i)
    {
        Trivial64& value = v[i];
        constexpr std::size_t size = std::size(value.values);
        for (std::size_t j = 0; j < size; ++j)
        {
            value.values[j] = (i * size) + j;
        }
    }
}
