#ifndef AOC_2023_AOC_H
#define AOC_2023_AOC_H

#include <string>

namespace aoc {

std::string input(int year, int day);
std::pair<std::string_view, std::string_view> split(std::string_view str, std::string_view delim);

}

#endif // AOC_2023_AOC_H
