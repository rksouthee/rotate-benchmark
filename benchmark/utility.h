#pragma once

#include <string>
#include <vector>

#include <cstdint>

struct Trivial64
{
    std::uint64_t values[8];
};

void fill(std::vector<std::uint32_t>& v);
void fill(std::vector<std::string>& v);
void fill(std::vector<Trivial64>& v);
