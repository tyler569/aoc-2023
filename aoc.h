#ifndef AOC_2023_AOC_H
#define AOC_2023_AOC_H

#include <string>
#include <vector>

namespace aoc {

std::string input(int year, int day);
std::vector<long> parse_numbers(std::istream &iss);
std::vector<long> parse_numbers(const std::string &string);

}

#endif // AOC_2023_AOC_H
