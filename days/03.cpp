#include "aoc.h"

#include <iostream>
#include <print>
#include <sstream>
#include <string>
#include <vector>

std::string_view sample_input = R"(467..114..
...*......
..35..633.
......#...
617*......
.....+.58.
..592.....
......755.
...$.*....
.664.598..)";

struct part;

bool point_touch(int row, int column, const part &p);

struct part_number {
    int number;
    int row, column;

    part_number(int number, int row, int column)
        : number(number)
        , row(row)
        , column(column)
    {
    }

    [[nodiscard]] bool touches(const part &p) const
    {
        if (number < 10) {
            return point_touch(row, column, p);
        } else if (number < 100) {
            return point_touch(row, column, p) || point_touch(row, column + 1, p);
        } else if (number < 1000) {
            return point_touch(row, column, p) || point_touch(row, column + 1, p)
                || point_touch(row, column + 2, p);
        } else {
            return point_touch(row, column, p) || point_touch(row, column + 1, p)
                || point_touch(row, column + 2, p) || point_touch(row, column + 3, p);
        }
    }

    friend std::ostream &operator<<(std::ostream &stream, const part_number &pn)
    {
        return stream << pn.number << ' ' << pn.row << ' ' << pn.column;
    }
};

struct part {
    char sigil;
    int row, column;
    std::vector<const part_number *> part_numbers {};

    part(char sigil, int row, int column)
        : sigil(sigil)
        , row(row)
        , column(column)
    {
    }

    friend std::ostream &operator<<(std::ostream &stream, const part &p)
    {
        return stream << p.sigil << ' ' << p.row << ' ' << p.column;
    }
};

bool point_touch(int row, int column, const part &p)
{
    int x_dist = std::abs(p.row - row);
    int y_dist = std::abs(p.column - column);
    if (x_dist <= 1 && y_dist <= 1) {
        return true;
    }
    return false;
}

int main()
{
    auto s = aoc::input(2023, 3);

    int row { 0 }, column { 0 };
    std::vector<part> parts {};
    std::vector<part_number> part_numbers {};
    for (size_t i = 0; i < s.size(); i++) {
        char c = s[i];
        if (c == '\n') {
            row++;
            column = 0;
            continue;
        } else if (isdigit(c)) {
            part_number pn { c - '0', row, column };
            while (isdigit(s[i + 1])) {
                pn.number *= 10;
                pn.number += s[i + 1] - '0';
                i++;
                column++;
            }
            part_numbers.push_back(pn);
        } else if (c == '.') {
            // do nothing
        } else {
            parts.emplace_back(c, row, column);
        }

        column++;
    }

    int total_part_numbers = 0;
    for (const auto &part_number : part_numbers) {
        bool first = true;
        for (auto &part : parts) {
            if (part_number.touches(part)) {
                part.part_numbers.push_back(&part_number);
                if (first) {
                    total_part_numbers += part_number.number;
                    first = false;
                }
            }
        }
    }

    int total_gear_ratios = 0;
    for (const auto &part : parts) {
        if (part.sigil == '*' && part.part_numbers.size() == 2) {
            total_gear_ratios += part.part_numbers[0]->number * part.part_numbers[1]->number;
        }
    }

    std::println("Part 1: {}", total_part_numbers);
    std::println("Part 2: {}", total_gear_ratios);
}