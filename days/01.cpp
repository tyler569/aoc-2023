#include "aoc.h"

#include <print>
#include <sstream>

int calibration_number(std::string_view s)
{
    auto first = s.find_first_of("0123456789");
    if (first == std::string_view::npos) {
        return 0;
    }
    auto last = s.find_last_of("0123456789");
    auto first_digit = s[first] - '0';
    auto last_digit = s[last] - '0';
    return first_digit * 10 + last_digit;
}

std::string_view numerals[] = {
    "0",
    "zero",
    "1",
    "one",
    "2",
    "two",
    "3",
    "three",
    "4",
    "four",
    "5",
    "five",
    "6",
    "six",
    "7",
    "seven",
    "8",
    "eight",
    "9",
    "nine",
};

int calibration_number_2(std::string_view s)
{
    size_t index = s.size();
    int first_num;
    for (int i = 0; i < 20; i++) {
        auto numeral = numerals[i];
        auto ix = s.find(numeral);
        if ((ix != std::string_view::npos) && (ix < index)) {
            index = ix;
            first_num = i / 2;
        }
    }

    index = 0;
    int second_num;
    for (int i = 0; i < 20; i++) {
        auto numeral = numerals[i];
        auto ix = s.rfind(numeral);
        if ((ix != std::string_view::npos) && (ix >= index)) {
            index = ix;
            second_num = i / 2;
        }
    }

    return first_num * 10 + second_num;
}

const char *test_input = R"(two1nine
eightwothree
abcone2threexyz
xtwone3four
4nineeightseven2
zoneight234
7pqrstsixteen)";

int main()
{
    auto s = aoc::input(2023, 1);
    auto stream = std::istringstream(s);

    int sum_1 = 0;
    int sum_2 = 0;
    std::string line;
    while (std::getline(stream, line)) {
        sum_1 += calibration_number(line);
        sum_2 += calibration_number_2(line);
    }

    std::print("part 1: {}\n", sum_1);
    std::print("part 2: {}\n", sum_2);
}